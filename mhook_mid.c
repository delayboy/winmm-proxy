// mhook_mid.c — mid-function inline hook on top of MinHook.
//
// Hook flow:
//   target:  E9 rel32 -> relay               (5 bytes + NOP to instr boundary)
//   relay:   jmp [rip+0] -> stub             (14 bytes)
//   stub:    pushfq / push all 16 GPRs (Rax..R15; original RSP via lea)
//            -> MF_CONTEXT on stack
//            rcx = &ctx, align stack, call handler, restore, pop, popfq
//            jmp [rip+0] -> trampoline
//   trampoline: MinHook's CreateTrampolineFunction:
//            relocated stolen insns + jmp back to target+stolenLen
//
// Plain C (not C++). Page layout: offset 0 relay(14), offset 64 stub(~85),
// offset 256 trampoline (MinHook writes here). We VirtualAlloc our own page
// within +-2GB of the target (MinHook's AllocateBuffer slot is only 64B).
#include <windows.h>
#include <tlhelp32.h>
#include <stdint.h>
#include <string.h>
#include "mhook_mid.h"
#include "MinHook.h"
#include "buffer.h"      // IsExecutableAddress (compiled in; AllocateBuffer unused)
#include "trampoline.h"  // CreateTrampolineFunction, TRAMPOLINE

typedef struct _MF_HOOK_ENTRY {
    void*    pTarget;
    void*    pPage;          // near-VirtualAlloc'd page (relay+stub+trampoline)
    void*    pRelay;
    void*    pStub;
    void*    pTrampoline;
    uint8_t  backup[16];     // first stolen bytes for disable
    size_t   patchedLen;     // bytes overwritten (to instruction boundary)
    void*    pHandler;
    BOOL     isEnabled;
} MF_HOOK_ENTRY;

#define INITIAL_MF_CAPACITY 8
static MF_HOOK_ENTRY* g_mfHooks = NULL;
static size_t g_mfCapacity = 0;
static size_t g_mfSize = 0;
static BOOL g_mfInited = FALSE;

// ---------------------------------------------------------------------------
static MF_HOOK_ENTRY* FindMFHook(void* pTarget) {
    for (size_t i = 0; i < g_mfSize; ++i)
        if (g_mfHooks[i].pTarget == pTarget) return &g_mfHooks[i];
    return NULL;
}

static MF_HOOK_ENTRY* AddMFHook(void) {
    if (g_mfHooks == NULL) {
        g_mfCapacity = INITIAL_MF_CAPACITY;
        g_mfHooks = (MF_HOOK_ENTRY*)HeapAlloc(GetProcessHeap(), 0,
            g_mfCapacity * sizeof(MF_HOOK_ENTRY));
        if (!g_mfHooks) return NULL;
    } else if (g_mfSize >= g_mfCapacity) {
        size_t nc = g_mfCapacity * 2;
        MF_HOOK_ENTRY* p = (MF_HOOK_ENTRY*)HeapReAlloc(GetProcessHeap(), 0,
            g_mfHooks, nc * sizeof(MF_HOOK_ENTRY));
        if (!p) return NULL;
        g_mfCapacity = nc; g_mfHooks = p;
    }
    return &g_mfHooks[g_mfSize++];
}

// Allocate a page within +-2GB of pTarget so the 5-byte E9 rel32 patch reaches.
static void* AllocNearPage(void* pTarget) {
    ULONG_PTR base = (ULONG_PTR)pTarget & ~0xFFFFULL;
    for (int i = 1; i <= 8192; ++i) {
        ULONG_PTR hint = base - (ULONG_PTR)i * 0x10000;
        void* p = VirtualAlloc((LPVOID)hint, 0x1000,
            MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (p) return p;
    }
    for (int i = 1; i <= 8192; ++i) {
        ULONG_PTR hint = base + (ULONG_PTR)i * 0x10000;
        void* p = VirtualAlloc((LPVOID)hint, 0x1000,
            MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (p) return p;
    }
    return NULL;
}

static void putb(uint8_t** p, uint8_t b) { *(*p)++ = b; }
static void putq(uint8_t** p, uint64_t v) { memcpy(*p, &v, 8); *p += 8; }

// Emit a 14-byte absolute indirect jump: FF 25 00000000 <abs_addr>.
static void emit_jmp_abs(uint8_t** p, void* target) {
    putb(p, 0xFF); putb(p, 0x25);
    putb(p, 0x00); putb(p, 0x00); putb(p, 0x00); putb(p, 0x00);
    putq(p, (uint64_t)target);
}

// Build the stub. Returns byte length written. The trailing `jmp [rip+0]
// trampoline` is appended by the caller (trampoline addr known after build).
//
// Push order (last push = MF_CONTEXT offset 0):
//   pushfq                     (above ctx)
//   push r15..rbp              (offsets 0x78..0x28)
//   lea r8,[rsp+0x60]          ; original RSP at stub entry (12 pushes before lea)
//   push r8                    (offset 0x20)
//   push rbx/rdx/rcx/rax       (offsets 0x18/0x10/0x08/0x00)
static size_t build_stub(uint8_t* out, void* handler) {
    uint8_t* p = out;
    putb(&p, 0x9C);                 // pushfq
    putb(&p, 0x41); putb(&p, 0x57); // push r15
    putb(&p, 0x41); putb(&p, 0x56); // push r14
    putb(&p, 0x41); putb(&p, 0x55); // push r13
    putb(&p, 0x41); putb(&p, 0x54); // push r12
    putb(&p, 0x41); putb(&p, 0x53); // push r11
    putb(&p, 0x41); putb(&p, 0x52); // push r10
    putb(&p, 0x41); putb(&p, 0x51); // push r9
    putb(&p, 0x41); putb(&p, 0x50); // push r8
    putb(&p, 0x57);                  // push rdi
    putb(&p, 0x56);                  // push rsi
    putb(&p, 0x55);                  // push rbp
    // lea r8, [rsp+0x60]  -> 4C 8D 44 24 60
    // REX must be 0x4C (W=1,R=1,B=0): R=1 for r8 destination, B=0 so SIB base
    // 0x24 stays rsp. 0x49 would set B=1 -> base becomes r12 (real bug, seen in CE).
    putb(&p, 0x4C); putb(&p, 0x8D); putb(&p, 0x44); putb(&p, 0x24); putb(&p, 0x60);
    putb(&p, 0x41); putb(&p, 0x50); // push r8 (original RSP -> ctx+0x20)
    putb(&p, 0x53);                  // push rbx  -> ctx+0x18
    putb(&p, 0x52);                  // push rdx  -> ctx+0x10
    putb(&p, 0x51);                  // push rcx  -> ctx+0x08
    putb(&p, 0x50);                  // push rax  -> ctx+0x00
    putb(&p, 0x48); putb(&p, 0x89); putb(&p, 0xE1);  // mov rcx, rsp   (handler arg = &ctx)
    putb(&p, 0x48); putb(&p, 0x89); putb(&p, 0xE3);  // mov rbx, rsp   (callee-saved: survives handler)
    putb(&p, 0x48); putb(&p, 0x83); putb(&p, 0xE4); putb(&p, 0xF0);  // and rsp, -16
    putb(&p, 0x48); putb(&p, 0x83); putb(&p, 0xEC); putb(&p, 0x20);  // sub rsp, 32
    putb(&p, 0x48); putb(&p, 0xB8);  // mov rax, imm64
    putq(&p, (uint64_t)handler);
    putb(&p, 0xFF); putb(&p, 0xD0);  // call rax
    putb(&p, 0x48); putb(&p, 0x89); putb(&p, 0xDC);  // mov rsp, rbx
    putb(&p, 0x58);                   // pop rax
    putb(&p, 0x59);                   // pop rcx
    putb(&p, 0x5A);                   // pop rdx
    putb(&p, 0x5B);                   // pop rbx
    // add rsp, 8  (skip Rsp slot)
    putb(&p, 0x48); putb(&p, 0x83); putb(&p, 0xC4); putb(&p, 0x08);
    putb(&p, 0x5D);                   // pop rbp
    putb(&p, 0x5E);                   // pop rsi
    putb(&p, 0x5F);                   // pop rdi
    putb(&p, 0x41); putb(&p, 0x58);  // pop r8
    putb(&p, 0x41); putb(&p, 0x59);  // pop r9
    putb(&p, 0x41); putb(&p, 0x5A);  // pop r10
    putb(&p, 0x41); putb(&p, 0x5B);  // pop r11
    putb(&p, 0x41); putb(&p, 0x5C);  // pop r12
    putb(&p, 0x41); putb(&p, 0x5D);  // pop r13
    putb(&p, 0x41); putb(&p, 0x5E);  // pop r14
    putb(&p, 0x41); putb(&p, 0x5F);  // pop r15
    putb(&p, 0x9D);                   // popfq
    return (size_t)(p - out);
}

static BOOL write_buf(void* dst, const void* src, size_t n) {
    DWORD oldp;
    if (!VirtualProtect(dst, n, PAGE_EXECUTE_READWRITE, &oldp)) return FALSE;
    memcpy(dst, src, n);
    FlushInstructionCache(GetCurrentProcess(), dst, n);
    VirtualProtect(dst, n, oldp, &oldp);
    return TRUE;
}

// ---------------------------------------------------------------------------
MH_STATUS MF_Initialize(void) {
    if (g_mfInited) return MH_OK;
    MH_STATUS s = MH_Initialize();
    if (s != MH_OK && s != MH_ERROR_ALREADY_INITIALIZED) return s;
    g_mfInited = TRUE;
    return MH_OK;
}

MH_STATUS MF_CreateHookMidFunc(void* pTarget, MF_HANDLER pHandler, void** ppTrampoline) {
    if (!g_mfInited) return MH_ERROR_NOT_INITIALIZED;
    if (!IsExecutableAddress(pTarget)) return MH_ERROR_NOT_EXECUTABLE;
    if (FindMFHook(pTarget)) return MH_ERROR_ALREADY_CREATED;

    uint8_t* page = (uint8_t*)AllocNearPage(pTarget);
    if (!page) return MH_ERROR_MEMORY_ALLOC;

    uint8_t* relay = page + 0;
    uint8_t* stub  = page + 64;
    uint8_t* tramp = page + 256;

    // Trampoline via MinHook (rip-relative relocation + jmp back to target+stolen).
    TRAMPOLINE ct;
    ct.pTarget     = pTarget;
    ct.pDetour     = NULL;   // mid-function: MinHook's relay unused
    ct.pTrampoline = tramp;
    if (!CreateTrampolineFunction(&ct)) {
        VirtualFree(page, 0, MEM_RELEASE);
        return MH_ERROR_UNSUPPORTED_FUNCTION;
    }

    // Stub (handler call); append trailing jmp to trampoline.
    size_t stubLen = build_stub(stub, (void*)pHandler);
    uint8_t* jmpSlot = stub + stubLen;
    emit_jmp_abs(&jmpSlot, tramp);

    // Relay -> stub.
    uint8_t* rp = relay;
    emit_jmp_abs(&rp, stub);

    // Serialize the generated code for other cores before any thread runs it.
    FlushInstructionCache(GetCurrentProcess(), page, 0x1000);

    // patchedLen = end-of-last-stolen-instruction offset (MinHook oldIPs).
    size_t patched = ct.oldIPs[ct.nIP - 1];

    MF_HOOK_ENTRY* e = AddMFHook();
    if (!e) { VirtualFree(page, 0, MEM_RELEASE); return MH_ERROR_MEMORY_ALLOC; }
    e->pTarget    = pTarget;
    e->pPage      = page;
    e->pRelay     = relay;
    e->pStub      = stub;
    e->pTrampoline = tramp;
    e->pHandler    = (void*)pHandler;
    e->patchedLen = patched;
    memcpy(e->backup, pTarget, sizeof(e->backup));
    e->isEnabled   = FALSE;

    if (ppTrampoline) *ppTrampoline = tramp;
    return MH_OK;
}

static BOOL patch_target(void* pTarget, void* pRelay, size_t patchedLen) {
    uint8_t buf[16];
    memset(buf, 0x90, sizeof(buf));
    int64_t rel = (int64_t)pRelay - ((int64_t)pTarget + 5);
    if (rel > INT32_MAX || rel < INT32_MIN) return FALSE;
    buf[0] = 0xE9;
    int32_t r = (int32_t)rel;
    memcpy(buf + 1, &r, 4);
    return write_buf(pTarget, buf, patchedLen);
}

// ---------------------------------------------------------------------------
// Thread freezing around patch writes (same idea as MinHook's Freeze/Unfreeze).
// Without this, another thread executing the hot code while we write the
// 10-byte patch can run half-written bytes and derail — this corrupted the
// game's first pass through the key path (garbage RAX at the hook point).
#define INITIAL_FROZEN_CAPACITY 128
typedef struct _MF_FROZEN_THREADS {
    LPDWORD pItems;
    UINT    capacity;
    UINT    size;
} MF_FROZEN_THREADS;

static BOOL FreezeThreads(MF_FROZEN_THREADS* pThreads) {
    pThreads->pItems = NULL; pThreads->capacity = 0; pThreads->size = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return FALSE;
    BOOL ok = FALSE;
    THREADENTRY32 te; te.dwSize = sizeof(te);
    if (Thread32First(hSnap, &te)) {
        ok = TRUE;
        do {
            if (te.dwSize >= (FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(DWORD))
                && te.th32OwnerProcessID == GetCurrentProcessId()
                && te.th32ThreadID != GetCurrentThreadId()) {
                if (pThreads->pItems == NULL) {
                    pThreads->capacity = INITIAL_FROZEN_CAPACITY;
                    pThreads->pItems = (LPDWORD)HeapAlloc(GetProcessHeap(), 0,
                        pThreads->capacity * sizeof(DWORD));
                    if (!pThreads->pItems) { ok = FALSE; break; }
                } else if (pThreads->size >= pThreads->capacity) {
                    pThreads->capacity *= 2;
                    LPDWORD p = (LPDWORD)HeapReAlloc(GetProcessHeap(), 0,
                        pThreads->pItems, pThreads->capacity * sizeof(DWORD));
                    if (!p) { ok = FALSE; break; }
                    pThreads->pItems = p;
                }
                HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT
                    | THREAD_QUERY_INFORMATION | THREAD_SET_CONTEXT, FALSE, te.th32ThreadID);
                BOOL suspended = FALSE;
                if (hThread) {
                    if (SuspendThread(hThread) != 0xFFFFFFFF) suspended = TRUE;
                    CloseHandle(hThread);
                }
                // Remember only actually-suspended threads (0 = skip on resume).
                pThreads->pItems[pThreads->size++] = suspended ? te.th32ThreadID : 0;
            }
            te.dwSize = sizeof(te);
        } while (Thread32Next(hSnap, &te));
    }
    CloseHandle(hSnap);
    if (!ok && pThreads->pItems) {
        // best-effort resume what we suspended before failing
        for (UINT i = 0; i < pThreads->size; ++i) {
            if (pThreads->pItems[i]) {
                HANDLE h = OpenThread(THREAD_SUSPEND_RESUME, FALSE, pThreads->pItems[i]);
                if (h) { ResumeThread(h); CloseHandle(h); }
            }
        }
        HeapFree(GetProcessHeap(), 0, pThreads->pItems);
        pThreads->pItems = NULL;
    }
    return ok;
}

static void UnfreezeThreads(MF_FROZEN_THREADS* pThreads) {
    if (!pThreads->pItems) return;
    for (UINT i = 0; i < pThreads->size; ++i) {
        if (pThreads->pItems[i]) {
            HANDLE h = OpenThread(THREAD_SUSPEND_RESUME, FALSE, pThreads->pItems[i]);
            if (h) { ResumeThread(h); CloseHandle(h); }
        }
    }
    HeapFree(GetProcessHeap(), 0, pThreads->pItems);
    pThreads->pItems = NULL;
}

// While threads are frozen: if a thread's RIP is inside the range we are about
// to overwrite, redirect it to the trampoline (enable) so it re-executes the
// stolen instructions instead of half-patched bytes. Mid-instruction RIPs are
// unrecoverable in general; redirecting to the trampoline is best-effort.
static void FixRipsInRange(void* pRangeStart, size_t rangeLen, void* pRedirect,
                           MF_FROZEN_THREADS* pThreads) {
    for (UINT i = 0; i < pThreads->size; ++i) {
        if (!pThreads->pItems[i]) continue;
        HANDLE h = OpenThread(THREAD_GET_CONTEXT | THREAD_SET_CONTEXT, FALSE,
            pThreads->pItems[i]);
        if (!h) continue;
        CONTEXT c;
        c.ContextFlags = CONTEXT_CONTROL;
        if (GetThreadContext(h, &c)) {
            if (c.Rip >= (DWORD64)(ULONG_PTR)pRangeStart
                && c.Rip < (DWORD64)(ULONG_PTR)pRangeStart + rangeLen) {
                c.Rip = (DWORD64)(ULONG_PTR)pRedirect;
                SetThreadContext(h, &c);
            }
        }
        CloseHandle(h);
    }
}

MH_STATUS MF_EnableHook(void* pTarget) {
    MF_HOOK_ENTRY* e = FindMFHook(pTarget);
    if (!e) return MH_ERROR_NOT_CREATED;
    if (e->isEnabled) return MH_ERROR_ENABLED;

    MF_FROZEN_THREADS ft;
    if (!FreezeThreads(&ft)) return MH_ERROR_MEMORY_ALLOC;
    FixRipsInRange(e->pTarget, e->patchedLen, e->pTrampoline, &ft);
    BOOL ok = patch_target(e->pTarget, e->pRelay, e->patchedLen);
    UnfreezeThreads(&ft);

    if (!ok) return MH_ERROR_MEMORY_PROTECT;
    e->isEnabled = TRUE;
    return MH_OK;
}

MH_STATUS MF_DisableHook(void* pTarget) {
    MF_HOOK_ENTRY* e = FindMFHook(pTarget);
    if (!e) return MH_ERROR_NOT_CREATED;
    if (!e->isEnabled) return MH_ERROR_DISABLED;

    MF_FROZEN_THREADS ft;
    if (!FreezeThreads(&ft)) return MH_ERROR_MEMORY_ALLOC;
    // A frozen thread parked at the trampoline must go back to the target so
    // it executes the restored original bytes.
    FixRipsInRange(e->pTrampoline, 64, e->pTarget, &ft);
    BOOL ok = write_buf(e->pTarget, e->backup, e->patchedLen);
    UnfreezeThreads(&ft);

    if (!ok) return MH_ERROR_MEMORY_PROTECT;
    e->isEnabled = FALSE;
    return MH_OK;
}

MH_STATUS MF_Uninitialize(void) {
    if (!g_mfInited) return MH_ERROR_NOT_INITIALIZED;
    for (size_t i = 0; i < g_mfSize; ++i) {
        if (g_mfHooks[i].isEnabled) {
            write_buf(g_mfHooks[i].pTarget, g_mfHooks[i].backup,
                g_mfHooks[i].patchedLen);
            g_mfHooks[i].isEnabled = FALSE;
        }
        if (g_mfHooks[i].pPage) VirtualFree(g_mfHooks[i].pPage, 0, MEM_RELEASE);
    }
    if (g_mfHooks) HeapFree(GetProcessHeap(), 0, g_mfHooks);
    g_mfHooks = NULL; g_mfCapacity = 0; g_mfSize = 0;
    g_mfInited = FALSE;
    return MH_OK;
}