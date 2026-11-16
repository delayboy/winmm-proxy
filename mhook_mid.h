// mhook_mid.h — Mid-function inline hook API built on top of MinHook.
//
// MinHook's MH_CreateHook assumes a function entry: the detour is called as
// a function and returns. MF_CreateHookMidFunc instead hooks at an arbitrary
// instruction boundary inside a function (where a register already holds a
// value we want), and the auto-generated stub saves all GPRs, hands a
// MF_CONTEXT* to a plain C handler, restores the (possibly modified) GPRs,
// then tail-jumps into the trampoline (which executes the relocated stolen
// instructions and jumps back to target+stolenLen).
//
// Reuses MinHook's CreateTrampolineFunction (rip-relative relocation, hde64
// disasm) for the trampoline. Does its own near-VirtualAlloc for the
// relay+stub+trampoline page (MinHook's AllocateBuffer slot is 64B, too small).
#pragma once
#include <stdint.h>
#include <windows.h>
#include "MinHook.h"  // for MH_STATUS

#ifdef __cplusplus
extern "C" {
#endif

// All 16 GPRs saved by the stub. Members are modifiable: the handler edits
// ctx->Rax (or any) and the stub restores the new value into the live register.
// Field order MUST match the push order in mhook_mid.c (build_stub).
#pragma pack(push, 8)
typedef struct _MF_CONTEXT {
    uint64_t Rax;   // +0x00
    uint64_t Rcx;   // +0x08
    uint64_t Rdx;   // +0x10
    uint64_t Rbx;   // +0x18
    uint64_t Rsp;   // +0x20  (original RSP at the hook point, read-only)
    uint64_t Rbp;   // +0x28
    uint64_t Rsi;   // +0x30
    uint64_t Rdi;   // +0x38
    uint64_t R8;    // +0x40
    uint64_t R9;    // +0x48
    uint64_t R10;   // +0x50
    uint64_t R11;   // +0x58
    uint64_t R12;   // +0x60
    uint64_t R13;   // +0x68
    uint64_t R14;   // +0x70
    uint64_t R15;   // +0x78
} MF_CONTEXT, *PMF_CONTEXT;
#pragma pack(pop)

// Plain C handler. Reads ctx (e.g. ctx->Rax for our key hook) and may edit
// ctx fields; the stub restores them to the live registers.
typedef void (*MF_HANDLER)(MF_CONTEXT* ctx);

// Initialize (also calls MH_Initialize so MinHook internals + buffer pool are
// ready; that pool is used by trampoline.c's IsExecutableAddress path which
// is not exercised here, but kept so the source compiles unmodified).
MH_STATUS MF_Initialize(void);

// Create a mid-function hook at pTarget. The patch is a 5-byte E9 rel32 to a
// relay on a near page; the relay jumps to the auto-generated stub which
// provides ctx and forwards to the trampoline returned via ppTrampoline.
MH_STATUS MF_CreateHookMidFunc(void* pTarget, MF_HANDLER pHandler, void** ppTrampoline);

MH_STATUS MF_EnableHook(void* pTarget);
MH_STATUS MF_DisableHook(void* pTarget);
MH_STATUS MF_Uninitialize(void);

#ifdef __cplusplus
}
#endif