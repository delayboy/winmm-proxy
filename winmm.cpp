#include <windows.h>
#include <stdio.h>
#include <windows.h>
#include <cstdint>
#include <vector>
#include <string>
#include <array>
typedef ULONG_PTR(WINAPI* ANYFUNCTIONCALL)(...);
// MSVC has no __asm inline assembly on x64, so the Fake* thunks are MASM stubs in winmm.asm.
// Plain extern "C" globals so the asm file can reference them by symbol name.
extern "C"
{
	HMODULE hOrigDll;
	FARPROC OrignalCloseDriver;
	FARPROC OrignalDefDriverProc;
	FARPROC OrignalDriverCallback;
	FARPROC OrignalDrvGetModuleHandle;
	FARPROC OrignalGetDriverModuleHandle;
	FARPROC OrignalOpenDriver;
	FARPROC OrignalPlaySound;
	FARPROC OrignalPlaySoundA;
	FARPROC OrignalPlaySoundW;
	FARPROC OrignalSendDriverMessage;
	FARPROC OrignalWOWAppExit;
	FARPROC OrignalauxGetDevCapsA;
	FARPROC OrignalauxGetDevCapsW;
	FARPROC OrignalauxGetNumDevs;
	FARPROC OrignalauxGetVolume;
	FARPROC OrignalauxOutMessage;
	FARPROC OrignalauxSetVolume;
	FARPROC OrignaljoyConfigChanged;
	FARPROC OrignaljoyGetDevCapsA;
	FARPROC OrignaljoyGetDevCapsW;
	FARPROC OrignaljoyGetNumDevs;
	FARPROC OrignaljoyGetPos;
	FARPROC OrignaljoyGetPosEx;
	FARPROC OrignaljoyGetThreshold;
	FARPROC OrignaljoyReleaseCapture;
	FARPROC OrignaljoySetCapture;
	FARPROC OrignaljoySetThreshold;
	FARPROC OrignalmciDriverNotify;
	FARPROC OrignalmciDriverYield;
	FARPROC OrignalmciExecute;
	FARPROC OrignalmciFreeCommandResource;
	FARPROC OrignalmciGetCreatorTask;
	FARPROC OrignalmciGetDeviceIDA;
	FARPROC OrignalmciGetDeviceIDFromElementIDA;
	FARPROC OrignalmciGetDeviceIDFromElementIDW;
	FARPROC OrignalmciGetDeviceIDW;
	FARPROC OrignalmciGetDriverData;
	FARPROC OrignalmciGetErrorStringA;
	FARPROC OrignalmciGetErrorStringW;
	FARPROC OrignalmciGetYieldProc;
	FARPROC OrignalmciLoadCommandResource;
	FARPROC OrignalmciSendCommandA;
	FARPROC OrignalmciSendCommandW;
	FARPROC OrignalmciSendStringA;
	FARPROC OrignalmciSendStringW;
	FARPROC OrignalmciSetDriverData;
	FARPROC OrignalmciSetYieldProc;
	FARPROC OrignalmidiConnect;
	FARPROC OrignalmidiDisconnect;
	FARPROC OrignalmidiInAddBuffer;
	FARPROC OrignalmidiInClose;
	FARPROC OrignalmidiInGetDevCapsA;
	FARPROC OrignalmidiInGetDevCapsW;
	FARPROC OrignalmidiInGetErrorTextA;
	FARPROC OrignalmidiInGetErrorTextW;
	FARPROC OrignalmidiInGetID;
	FARPROC OrignalmidiInGetNumDevs;
	FARPROC OrignalmidiInMessage;
	FARPROC OrignalmidiInOpen;
	FARPROC OrignalmidiInPrepareHeader;
	FARPROC OrignalmidiInReset;
	FARPROC OrignalmidiInStart;
	FARPROC OrignalmidiInStop;
	FARPROC OrignalmidiInUnprepareHeader;
	FARPROC OrignalmidiOutCacheDrumPatches;
	FARPROC OrignalmidiOutCachePatches;
	FARPROC OrignalmidiOutClose;
	FARPROC OrignalmidiOutGetDevCapsA;
	FARPROC OrignalmidiOutGetDevCapsW;
	FARPROC OrignalmidiOutGetErrorTextA;
	FARPROC OrignalmidiOutGetErrorTextW;
	FARPROC OrignalmidiOutGetID;
	FARPROC OrignalmidiOutGetNumDevs;
	FARPROC OrignalmidiOutGetVolume;
	FARPROC OrignalmidiOutLongMsg;
	FARPROC OrignalmidiOutMessage;
	FARPROC OrignalmidiOutOpen;
	FARPROC OrignalmidiOutPrepareHeader;
	FARPROC OrignalmidiOutReset;
	FARPROC OrignalmidiOutSetVolume;
	FARPROC OrignalmidiOutShortMsg;
	FARPROC OrignalmidiOutUnprepareHeader;
	FARPROC OrignalmidiStreamClose;
	FARPROC OrignalmidiStreamOpen;
	FARPROC OrignalmidiStreamOut;
	FARPROC OrignalmidiStreamPause;
	FARPROC OrignalmidiStreamPosition;
	FARPROC OrignalmidiStreamProperty;
	FARPROC OrignalmidiStreamRestart;
	FARPROC OrignalmidiStreamStop;
	FARPROC OrignalmixerClose;
	FARPROC OrignalmixerGetControlDetailsA;
	FARPROC OrignalmixerGetControlDetailsW;
	FARPROC OrignalmixerGetDevCapsA;
	FARPROC OrignalmixerGetDevCapsW;
	FARPROC OrignalmixerGetID;
	FARPROC OrignalmixerGetLineControlsA;
	FARPROC OrignalmixerGetLineControlsW;
	FARPROC OrignalmixerGetLineInfoA;
	FARPROC OrignalmixerGetLineInfoW;
	FARPROC OrignalmixerGetNumDevs;
	FARPROC OrignalmixerMessage;
	FARPROC OrignalmixerOpen;
	FARPROC OrignalmixerSetControlDetails;
	FARPROC OrignalmmDrvInstall;
	FARPROC OrignalmmGetCurrentTask;
	FARPROC OrignalmmTaskBlock;
	FARPROC OrignalmmTaskCreate;
	FARPROC OrignalmmTaskSignal;
	FARPROC OrignalmmTaskYield;
	FARPROC OrignalmmioAdvance;
	FARPROC OrignalmmioAscend;
	FARPROC OrignalmmioClose;
	FARPROC OrignalmmioCreateChunk;
	FARPROC OrignalmmioDescend;
	FARPROC OrignalmmioFlush;
	FARPROC OrignalmmioGetInfo;
	FARPROC OrignalmmioInstallIOProcA;
	FARPROC OrignalmmioInstallIOProcW;
	FARPROC OrignalmmioOpenA;
	FARPROC OrignalmmioOpenW;
	FARPROC OrignalmmioRead;
	FARPROC OrignalmmioRenameA;
	FARPROC OrignalmmioRenameW;
	FARPROC OrignalmmioSeek;
	FARPROC OrignalmmioSendMessage;
	FARPROC OrignalmmioSetBuffer;
	FARPROC OrignalmmioSetInfo;
	FARPROC OrignalmmioStringToFOURCCA;
	FARPROC OrignalmmioStringToFOURCCW;
	FARPROC OrignalmmioWrite;
	FARPROC OrignalmmsystemGetVersion;
	FARPROC OrignalsndPlaySoundA;
	FARPROC OrignalsndPlaySoundW;
	FARPROC OrignaltimeBeginPeriod;
	FARPROC OrignaltimeEndPeriod;
	FARPROC OrignaltimeGetDevCaps;
	FARPROC OrignaltimeGetSystemTime;
	FARPROC OrignaltimeGetTime;
	FARPROC OrignaltimeKillEvent;
	FARPROC OrignaltimeSetEvent;
	FARPROC OrignalwaveInAddBuffer;
	FARPROC OrignalwaveInClose;
	FARPROC OrignalwaveInGetDevCapsA;
	FARPROC OrignalwaveInGetDevCapsW;
	FARPROC OrignalwaveInGetErrorTextA;
	FARPROC OrignalwaveInGetErrorTextW;
	FARPROC OrignalwaveInGetID;
	FARPROC OrignalwaveInGetNumDevs;
	FARPROC OrignalwaveInGetPosition;
	FARPROC OrignalwaveInMessage;
	FARPROC OrignalwaveInOpen;
	FARPROC OrignalwaveInPrepareHeader;
	FARPROC OrignalwaveInReset;
	FARPROC OrignalwaveInStart;
	FARPROC OrignalwaveInStop;
	FARPROC OrignalwaveInUnprepareHeader;
	FARPROC OrignalwaveOutBreakLoop;
	FARPROC OrignalwaveOutClose;
	FARPROC OrignalwaveOutGetDevCapsA;
	FARPROC OrignalwaveOutGetDevCapsW;
	FARPROC OrignalwaveOutGetErrorTextA;
	FARPROC OrignalwaveOutGetErrorTextW;
	FARPROC OrignalwaveOutGetID;
	FARPROC OrignalwaveOutGetNumDevs;
	FARPROC OrignalwaveOutGetPitch;
	FARPROC OrignalwaveOutGetPlaybackRate;
	FARPROC OrignalwaveOutGetPosition;
	FARPROC OrignalwaveOutGetVolume;
	FARPROC OrignalwaveOutMessage;
	FARPROC OrignalwaveOutOpen;
	FARPROC OrignalwaveOutPause;
	FARPROC OrignalwaveOutPrepareHeader;
	FARPROC OrignalwaveOutReset;
	FARPROC OrignalwaveOutRestart;
	FARPROC OrignalwaveOutSetPitch;
	FARPROC OrignalwaveOutSetPlaybackRate;
	FARPROC OrignalwaveOutSetVolume;
	FARPROC OrignalwaveOutUnprepareHeader;
	FARPROC OrignalwaveOutWrite;
}
FILE* fpOut = NULL;
FILE* fpErr = NULL;
FILE* fpIn = NULL;
void InitConsoleWindow(BOOL switch_on)
{

	const HWND consoleWindow = GetConsoleWindow();
	BOOL no_handle = consoleWindow == NULL;
	if (switch_on && no_handle)
	{
		int hCrt;
		FILE* hf;

		AllocConsole();
		if (fpIn != NULL) fclose(fpIn);
		if (fpOut != NULL) fclose(fpOut);
		if (fpErr != NULL) fclose(fpErr);
		fpIn = freopen("conin$", "r", stdin);
		fpOut = freopen("conout$", "w", stdout);
		fpErr = freopen("conout$", "w", stderr);
		// test code
		printf("InitConsoleWindow OK!\n");
	}
	else if (!switch_on && !no_handle) {
		printf("Closing Console Window...\n");
		fflush(stdout);
		fflush(stderr);
		fflush(stdin);
		// Close the file streams if they were redirected

		FreeConsole();
		if (fpIn != NULL) fclose(fpIn);
		if (fpOut != NULL) fclose(fpOut);
		if (fpErr != NULL) fclose(fpErr);

		// Redirect stdout and stderr back to NUL to avoid invalid pointer issues
		fpIn = freopen("NUL:", "r", stdin);
		fpOut = freopen("NUL:", "w", stdout);
		fpErr = freopen("NUL:", "w", stderr);

	}
}


// ===================== GameAssembly.dll key hook (AOB scan) =====================
// Reference: 6月到8月游戏密钥hook点迁移总结.md §5
// AOB verified unique on 2026-06 and 2026-08 builds.
static const char* KEY_AOB =
	"31 D2 E8 ?? ?? ?? ?? 48 89 C2 E8 ?? ?? ?? ?? 48 89 C3 "
	"48 8B 05 ?? ?? ?? ?? 48 8B 80 B8 00 00 00 48 8B 50 20";
#define KEY_HOOK_OFFSET 0x0F   // "mov rbx,rax": at entry RAX = hex key string (IL2CPP String)

#include "mhook_mid.h"

// Plain C handler. The framework-built stub saved all GPRs into ctx before
// calling us; ctx->Rax is the IL2CPP String at the hook point (mov rbx,rax
// has not yet executed, RAX still holds the hex key string).
// Captured key shared with patcherThread. Set once by keyHandler.
static char g_keyHex[65] = { 0 };

extern "C" void keyHandler(MF_CONTEXT* ctx) {
	printf("[KeyHook] hook fired, RAX=%p\n", (void*)ctx->Rax);
	fflush(stdout);
	__try {
		uint8_t* s = (uint8_t*)ctx->Rax;
		int32_t len = *(int32_t*)(s + 0x10);          // IL2CPP String: length at +0x10
		if (len <= 0 || len > 256) return;
		wchar_t buf[257];
		memcpy(buf, s + 0x14, (size_t)len * 2);       // UTF-16LE chars at +0x14
		buf[len] = 0;
		printf("[KeyHook] PRAGMA key = \"x'%ls'\";\n", buf);
		fflush(stdout);
		if (!g_keyHex[0]) {
			size_t n = 0;
			for (int32_t i = 0; i < len && n < 64; ++i) {
				if (buf[i] >= L'0' && buf[i] <= L'9' || buf[i] >= L'a' && buf[i] <= L'f' || buf[i] >= L'A' && buf[i] <= L'F')
					g_keyHex[n++] = (char)buf[i];
			}
			g_keyHex[n] = 0;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		printf("[KeyHook] exception reading string at %p\n", (void*)ctx->Rax);
	}
	// MessageBoxA(NULL, "OVER", "OVER", MB_OK);
}

static bool InstallKeyHook(uint8_t* hookAddr) {
	if (MF_Initialize() != MH_OK) {
		printf("[KeyHook] MF_Initialize failed\n");
		return false;
	}
	void* trampoline = NULL;
	MH_STATUS s = MF_CreateHookMidFunc(hookAddr, keyHandler, &trampoline);
	if (s != MH_OK) {
		printf("[KeyHook] MF_CreateHookMidFunc failed: %s\n", MH_StatusToString(s));
		return false;
	}
	s = MF_EnableHook(hookAddr);
	if (s != MH_OK) {
		printf("[KeyHook] MF_EnableHook failed: %s\n", MH_StatusToString(s));
		return false;
	}
	printf("[KeyHook] hook installed, trampoline=%p\n", trampoline);
	return true;
}

// Masked pattern scan within one region. Returns first hit or NULL.
static uint8_t* PatternScan(const uint8_t* start, size_t size,
	const uint8_t* pat, const uint8_t* mask, size_t patLen)
{
	const uint8_t first = pat[0];
	const uint8_t* end = start + size;
	for (const uint8_t* p = start; p + patLen <= end; ++p) {
		if (*p != first) continue;
		const uint8_t* q = p;
		for (size_t i = 1; i < patLen; ++i) {
			if (mask[i] && q[i] != pat[i]) goto next;
		}
		return (uint8_t*)p;
	next:;
	}
	return NULL;
}

struct Pat { uint8_t b[128]; uint8_t m[128]; size_t len; };

static bool ParsePattern(const char* text, Pat& out) {
	out.len = 0;
	while (*text && out.len < 128) {
		if (*text == ' ') { ++text; continue; }
		if (text[0] == '?' && text[1] == '?') {
			out.b[out.len] = 0; out.m[out.len] = 0; out.len++;
			text += 2;
		}
		else {
			int v;
			if (sscanf_s(text, "%2x", &v) != 1) return false;
			out.b[out.len] = (uint8_t)v; out.m[out.len] = 1; out.len++;
			text += 2;
		}
	}
	return out.len > 0;
}

static bool IsBlueArchiveHost() {
	char path[MAX_PATH];
	if (!GetModuleFileNameA(NULL, path, MAX_PATH)) return false;
	const char* name = strrchr(path, '\\');
	name = name ? name + 1 : path;
	return _stricmp(name, "BlueArchive.exe") == 0;
}

// If .\BlueArchiveCnPatcher.exe sits next to the game exe, poll once per second
// until the hook captured the key, then run:
//   BlueArchiveCnPatcher.exe --db-password <key>
DWORD WINAPI patcherThread(LPVOID) {
	for (;;) {
		if (g_keyHex[0]) {
			char exeDir[MAX_PATH], patcher[MAX_PATH], params[96];
			GetModuleFileNameA(NULL, exeDir, MAX_PATH);
			char* slash = strrchr(exeDir, '\\');
			if (slash) slash[1] = 0;  // keep only the directory part
			sprintf_s(patcher, "%sBlueArchiveCnPatcher.exe", exeDir);
			sprintf_s(params, "--db-password %s", g_keyHex);
			// The patcher manifest requires admin: CreateProcess gives 740
			// (ERROR_ELEVATION_REQUIRED). ShellExecuteEx with "runas" shows
			// the normal UAC prompt instead. Falls back to plain "open".
			SHELLEXECUTEINFOA sei = { sizeof(sei) };
			sei.lpFile = patcher;
			sei.lpParameters = params;
			sei.lpDirectory = exeDir;
			sei.nShow = SW_SHOWNORMAL;  // elevated console app gets its own visible console
			sei.fMask = SEE_MASK_NOCLOSEPROCESS;
			sei.lpVerb = "runas";
			if (!ShellExecuteExA(&sei)) {
				printf("[Patcher] ShellExecuteEx runas failed (%lu), trying open\n", GetLastError());
				sei.lpVerb = "open";
				if (!ShellExecuteExA(&sei)) {
					printf("[Patcher] ShellExecuteEx failed (%lu)\n", GetLastError());
					fflush(stdout);
					return 0;
				}
			}
			printf("[Patcher] launched: %s %s\n", patcher, params);
			if (sei.hProcess) {
				WaitForSingleObject(sei.hProcess, INFINITE);
				CloseHandle(sei.hProcess);
			}
			fflush(stdout);
			return 0;
		}
		Sleep(1000);
	}
}

DWORD WINAPI keyScanThread(LPVOID) {
	if (!IsBlueArchiveHost()) return 0;
	// Patcher companion: only if the CLI tool is present next to the game exe
	{
		char exeDir[MAX_PATH], patcher[MAX_PATH];
		GetModuleFileNameA(NULL, exeDir, MAX_PATH);
		char* slash = strrchr(exeDir, '\\');
		if (slash) slash[1] = 0;
		sprintf_s(patcher, "%sBlueArchiveCnPatcher.exe", exeDir);
		if (GetFileAttributesA(patcher) != INVALID_FILE_ATTRIBUTES) {
			printf("[Patcher] BlueArchiveCnPatcher.exe detected, starting key watcher\n");
			CreateThread(NULL, 0, patcherThread, NULL, 0, NULL);
		}
		else return 0;
	}
	InitConsoleWindow(TRUE);
	Pat pat;
	if (!ParsePattern(KEY_AOB, pat)) {
		printf("[KeyHook] bad AOB pattern\n");
		return 0;
	}
	printf("[KeyHook] waiting for GameAssembly.dll...\n");
	int notFoundRounds = 0;
	for (;;) {
		HMODULE hMod = GetModuleHandleA("GameAssembly.dll");
		if (!hMod) { Sleep(1000); continue; }

		// scan all executable sections of the module
		uint8_t* base = (uint8_t*)hMod;
		auto dos = (IMAGE_DOS_HEADER*)base;
		auto nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);
		auto sec = IMAGE_FIRST_SECTION(nt);
		uint8_t* hit = NULL;
		int hits = 0;
		for (int i = 0; i < nt->FileHeader.NumberOfSections; ++i, ++sec) {
			if (!(sec->Characteristics & IMAGE_SCN_MEM_EXECUTE)) continue;
			uint8_t* s = base + sec->VirtualAddress;
			size_t sz = sec->Misc.VirtualSize;
			uint8_t* h = PatternScan(s, sz, pat.b, pat.m, pat.len);
			if (h) { ++hits; if (!hit) hit = h; }
		}
		if (hits == 1) {
			uintptr_t rva = (uintptr_t)hit - (uintptr_t)base;
			printf("[KeyHook] AOB unique hit: GameAssembly.dll+0x%llX (VA %p)\n",
				(unsigned long long)rva, (void*)hit);
			uint8_t* target = hit + KEY_HOOK_OFFSET;
			if (InstallKeyHook(target))
				printf("[KeyHook] hook installed at GameAssembly.dll+0x%llX, waiting for key...\n",
					(unsigned long long)(rva + KEY_HOOK_OFFSET));
			return 0;
		}
		if (hits > 1) {
			printf("[KeyHook] WARNING: %d matches, pattern not unique, aborting\n", hits);
			return 0;
		}
		if (++notFoundRounds > 10) {
			printf("[KeyHook] module present but pattern not found after %d rounds, giving up\n", notFoundRounds);
			return 0;
		}
		Sleep(1000);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	char path[MAX_PATH];
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		GetSystemDirectoryA(path, MAX_PATH);
		strcat_s(path, "\\winmm.dll");
		hOrigDll = LoadLibraryA(path);
		CreateThread(NULL, 0, keyScanThread, NULL, 0, NULL);
		OrignalCloseDriver = GetProcAddress(hOrigDll, "CloseDriver");
		OrignalDefDriverProc = GetProcAddress(hOrigDll, "DefDriverProc");
		OrignalDriverCallback = GetProcAddress(hOrigDll, "DriverCallback");
		OrignalDrvGetModuleHandle = GetProcAddress(hOrigDll, "DrvGetModuleHandle");
		OrignalGetDriverModuleHandle = GetProcAddress(hOrigDll, "GetDriverModuleHandle");
		OrignalOpenDriver = GetProcAddress(hOrigDll, "OpenDriver");
		OrignalPlaySound = GetProcAddress(hOrigDll, "PlaySound");
		OrignalPlaySoundA = GetProcAddress(hOrigDll, "PlaySoundA");
		OrignalPlaySoundW = GetProcAddress(hOrigDll, "PlaySoundW");
		OrignalSendDriverMessage = GetProcAddress(hOrigDll, "SendDriverMessage");
		OrignalWOWAppExit = GetProcAddress(hOrigDll, "WOWAppExit");
		OrignalauxGetDevCapsA = GetProcAddress(hOrigDll, "auxGetDevCapsA");
		OrignalauxGetDevCapsW = GetProcAddress(hOrigDll, "auxGetDevCapsW");
		OrignalauxGetNumDevs = GetProcAddress(hOrigDll, "auxGetNumDevs");
		OrignalauxGetVolume = GetProcAddress(hOrigDll, "auxGetVolume");
		OrignalauxOutMessage = GetProcAddress(hOrigDll, "auxOutMessage");
		OrignalauxSetVolume = GetProcAddress(hOrigDll, "auxSetVolume");
		OrignaljoyConfigChanged = GetProcAddress(hOrigDll, "joyConfigChanged");
		OrignaljoyGetDevCapsA = GetProcAddress(hOrigDll, "joyGetDevCapsA");
		OrignaljoyGetDevCapsW = GetProcAddress(hOrigDll, "joyGetDevCapsW");
		OrignaljoyGetNumDevs = GetProcAddress(hOrigDll, "joyGetNumDevs");
		OrignaljoyGetPos = GetProcAddress(hOrigDll, "joyGetPos");
		OrignaljoyGetPosEx = GetProcAddress(hOrigDll, "joyGetPosEx");
		OrignaljoyGetThreshold = GetProcAddress(hOrigDll, "joyGetThreshold");
		OrignaljoyReleaseCapture = GetProcAddress(hOrigDll, "joyReleaseCapture");
		OrignaljoySetCapture = GetProcAddress(hOrigDll, "joySetCapture");
		OrignaljoySetThreshold = GetProcAddress(hOrigDll, "joySetThreshold");
		OrignalmciDriverNotify = GetProcAddress(hOrigDll, "mciDriverNotify");
		OrignalmciDriverYield = GetProcAddress(hOrigDll, "mciDriverYield");
		OrignalmciExecute = GetProcAddress(hOrigDll, "mciExecute");
		OrignalmciFreeCommandResource = GetProcAddress(hOrigDll, "mciFreeCommandResource");
		OrignalmciGetCreatorTask = GetProcAddress(hOrigDll, "mciGetCreatorTask");
		OrignalmciGetDeviceIDA = GetProcAddress(hOrigDll, "mciGetDeviceIDA");
		OrignalmciGetDeviceIDFromElementIDA = GetProcAddress(hOrigDll, "mciGetDeviceIDFromElementIDA");
		OrignalmciGetDeviceIDFromElementIDW = GetProcAddress(hOrigDll, "mciGetDeviceIDFromElementIDW");
		OrignalmciGetDeviceIDW = GetProcAddress(hOrigDll, "mciGetDeviceIDW");
		OrignalmciGetDriverData = GetProcAddress(hOrigDll, "mciGetDriverData");
		OrignalmciGetErrorStringA = GetProcAddress(hOrigDll, "mciGetErrorStringA");
		OrignalmciGetErrorStringW = GetProcAddress(hOrigDll, "mciGetErrorStringW");
		OrignalmciGetYieldProc = GetProcAddress(hOrigDll, "mciGetYieldProc");
		OrignalmciLoadCommandResource = GetProcAddress(hOrigDll, "mciLoadCommandResource");
		OrignalmciSendCommandA = GetProcAddress(hOrigDll, "mciSendCommandA");
		OrignalmciSendCommandW = GetProcAddress(hOrigDll, "mciSendCommandW");
		OrignalmciSendStringA = GetProcAddress(hOrigDll, "mciSendStringA");
		OrignalmciSendStringW = GetProcAddress(hOrigDll, "mciSendStringW");
		OrignalmciSetDriverData = GetProcAddress(hOrigDll, "mciSetDriverData");
		OrignalmciSetYieldProc = GetProcAddress(hOrigDll, "mciSetYieldProc");
		OrignalmidiConnect = GetProcAddress(hOrigDll, "midiConnect");
		OrignalmidiDisconnect = GetProcAddress(hOrigDll, "midiDisconnect");
		OrignalmidiInAddBuffer = GetProcAddress(hOrigDll, "midiInAddBuffer");
		OrignalmidiInClose = GetProcAddress(hOrigDll, "midiInClose");
		OrignalmidiInGetDevCapsA = GetProcAddress(hOrigDll, "midiInGetDevCapsA");
		OrignalmidiInGetDevCapsW = GetProcAddress(hOrigDll, "midiInGetDevCapsW");
		OrignalmidiInGetErrorTextA = GetProcAddress(hOrigDll, "midiInGetErrorTextA");
		OrignalmidiInGetErrorTextW = GetProcAddress(hOrigDll, "midiInGetErrorTextW");
		OrignalmidiInGetID = GetProcAddress(hOrigDll, "midiInGetID");
		OrignalmidiInGetNumDevs = GetProcAddress(hOrigDll, "midiInGetNumDevs");
		OrignalmidiInMessage = GetProcAddress(hOrigDll, "midiInMessage");
		OrignalmidiInOpen = GetProcAddress(hOrigDll, "midiInOpen");
		OrignalmidiInPrepareHeader = GetProcAddress(hOrigDll, "midiInPrepareHeader");
		OrignalmidiInReset = GetProcAddress(hOrigDll, "midiInReset");
		OrignalmidiInStart = GetProcAddress(hOrigDll, "midiInStart");
		OrignalmidiInStop = GetProcAddress(hOrigDll, "midiInStop");
		OrignalmidiInUnprepareHeader = GetProcAddress(hOrigDll, "midiInUnprepareHeader");
		OrignalmidiOutCacheDrumPatches = GetProcAddress(hOrigDll, "midiOutCacheDrumPatches");
		OrignalmidiOutCachePatches = GetProcAddress(hOrigDll, "midiOutCachePatches");
		OrignalmidiOutClose = GetProcAddress(hOrigDll, "midiOutClose");
		OrignalmidiOutGetDevCapsA = GetProcAddress(hOrigDll, "midiOutGetDevCapsA");
		OrignalmidiOutGetDevCapsW = GetProcAddress(hOrigDll, "midiOutGetDevCapsW");
		OrignalmidiOutGetErrorTextA = GetProcAddress(hOrigDll, "midiOutGetErrorTextA");
		OrignalmidiOutGetErrorTextW = GetProcAddress(hOrigDll, "midiOutGetErrorTextW");
		OrignalmidiOutGetID = GetProcAddress(hOrigDll, "midiOutGetID");
		OrignalmidiOutGetNumDevs = GetProcAddress(hOrigDll, "midiOutGetNumDevs");
		OrignalmidiOutGetVolume = GetProcAddress(hOrigDll, "midiOutGetVolume");
		OrignalmidiOutLongMsg = GetProcAddress(hOrigDll, "midiOutLongMsg");
		OrignalmidiOutMessage = GetProcAddress(hOrigDll, "midiOutMessage");
		OrignalmidiOutOpen = GetProcAddress(hOrigDll, "midiOutOpen");
		OrignalmidiOutPrepareHeader = GetProcAddress(hOrigDll, "midiOutPrepareHeader");
		OrignalmidiOutReset = GetProcAddress(hOrigDll, "midiOutReset");
		OrignalmidiOutSetVolume = GetProcAddress(hOrigDll, "midiOutSetVolume");
		OrignalmidiOutShortMsg = GetProcAddress(hOrigDll, "midiOutShortMsg");
		OrignalmidiOutUnprepareHeader = GetProcAddress(hOrigDll, "midiOutUnprepareHeader");
		OrignalmidiStreamClose = GetProcAddress(hOrigDll, "midiStreamClose");
		OrignalmidiStreamOpen = GetProcAddress(hOrigDll, "midiStreamOpen");
		OrignalmidiStreamOut = GetProcAddress(hOrigDll, "midiStreamOut");
		OrignalmidiStreamPause = GetProcAddress(hOrigDll, "midiStreamPause");
		OrignalmidiStreamPosition = GetProcAddress(hOrigDll, "midiStreamPosition");
		OrignalmidiStreamProperty = GetProcAddress(hOrigDll, "midiStreamProperty");
		OrignalmidiStreamRestart = GetProcAddress(hOrigDll, "midiStreamRestart");
		OrignalmidiStreamStop = GetProcAddress(hOrigDll, "midiStreamStop");
		OrignalmixerClose = GetProcAddress(hOrigDll, "mixerClose");
		OrignalmixerGetControlDetailsA = GetProcAddress(hOrigDll, "mixerGetControlDetailsA");
		OrignalmixerGetControlDetailsW = GetProcAddress(hOrigDll, "mixerGetControlDetailsW");
		OrignalmixerGetDevCapsA = GetProcAddress(hOrigDll, "mixerGetDevCapsA");
		OrignalmixerGetDevCapsW = GetProcAddress(hOrigDll, "mixerGetDevCapsW");
		OrignalmixerGetID = GetProcAddress(hOrigDll, "mixerGetID");
		OrignalmixerGetLineControlsA = GetProcAddress(hOrigDll, "mixerGetLineControlsA");
		OrignalmixerGetLineControlsW = GetProcAddress(hOrigDll, "mixerGetLineControlsW");
		OrignalmixerGetLineInfoA = GetProcAddress(hOrigDll, "mixerGetLineInfoA");
		OrignalmixerGetLineInfoW = GetProcAddress(hOrigDll, "mixerGetLineInfoW");
		OrignalmixerGetNumDevs = GetProcAddress(hOrigDll, "mixerGetNumDevs");
		OrignalmixerMessage = GetProcAddress(hOrigDll, "mixerMessage");
		OrignalmixerOpen = GetProcAddress(hOrigDll, "mixerOpen");
		OrignalmixerSetControlDetails = GetProcAddress(hOrigDll, "mixerSetControlDetails");
		OrignalmmDrvInstall = GetProcAddress(hOrigDll, "mmDrvInstall");
		OrignalmmGetCurrentTask = GetProcAddress(hOrigDll, "mmGetCurrentTask");
		OrignalmmTaskBlock = GetProcAddress(hOrigDll, "mmTaskBlock");
		OrignalmmTaskCreate = GetProcAddress(hOrigDll, "mmTaskCreate");
		OrignalmmTaskSignal = GetProcAddress(hOrigDll, "mmTaskSignal");
		OrignalmmTaskYield = GetProcAddress(hOrigDll, "mmTaskYield");
		OrignalmmioAdvance = GetProcAddress(hOrigDll, "mmioAdvance");
		OrignalmmioAscend = GetProcAddress(hOrigDll, "mmioAscend");
		OrignalmmioClose = GetProcAddress(hOrigDll, "mmioClose");
		OrignalmmioCreateChunk = GetProcAddress(hOrigDll, "mmioCreateChunk");
		OrignalmmioDescend = GetProcAddress(hOrigDll, "mmioDescend");
		OrignalmmioFlush = GetProcAddress(hOrigDll, "mmioFlush");
		OrignalmmioGetInfo = GetProcAddress(hOrigDll, "mmioGetInfo");
		OrignalmmioInstallIOProcA = GetProcAddress(hOrigDll, "mmioInstallIOProcA");
		OrignalmmioInstallIOProcW = GetProcAddress(hOrigDll, "mmioInstallIOProcW");
		OrignalmmioOpenA = GetProcAddress(hOrigDll, "mmioOpenA");
		OrignalmmioOpenW = GetProcAddress(hOrigDll, "mmioOpenW");
		OrignalmmioRead = GetProcAddress(hOrigDll, "mmioRead");
		OrignalmmioRenameA = GetProcAddress(hOrigDll, "mmioRenameA");
		OrignalmmioRenameW = GetProcAddress(hOrigDll, "mmioRenameW");
		OrignalmmioSeek = GetProcAddress(hOrigDll, "mmioSeek");
		OrignalmmioSendMessage = GetProcAddress(hOrigDll, "mmioSendMessage");
		OrignalmmioSetBuffer = GetProcAddress(hOrigDll, "mmioSetBuffer");
		OrignalmmioSetInfo = GetProcAddress(hOrigDll, "mmioSetInfo");
		OrignalmmioStringToFOURCCA = GetProcAddress(hOrigDll, "mmioStringToFOURCCA");
		OrignalmmioStringToFOURCCW = GetProcAddress(hOrigDll, "mmioStringToFOURCCW");
		OrignalmmioWrite = GetProcAddress(hOrigDll, "mmioWrite");
		OrignalmmsystemGetVersion = GetProcAddress(hOrigDll, "mmsystemGetVersion");
		OrignalsndPlaySoundA = GetProcAddress(hOrigDll, "sndPlaySoundA");
		OrignalsndPlaySoundW = GetProcAddress(hOrigDll, "sndPlaySoundW");
		OrignaltimeBeginPeriod = GetProcAddress(hOrigDll, "timeBeginPeriod");
		OrignaltimeEndPeriod = GetProcAddress(hOrigDll, "timeEndPeriod");
		OrignaltimeGetDevCaps = GetProcAddress(hOrigDll, "timeGetDevCaps");
		OrignaltimeGetSystemTime = GetProcAddress(hOrigDll, "timeGetSystemTime");
		OrignaltimeGetTime = GetProcAddress(hOrigDll, "timeGetTime");
		OrignaltimeKillEvent = GetProcAddress(hOrigDll, "timeKillEvent");
		OrignaltimeSetEvent = GetProcAddress(hOrigDll, "timeSetEvent");
		OrignalwaveInAddBuffer = GetProcAddress(hOrigDll, "waveInAddBuffer");
		OrignalwaveInClose = GetProcAddress(hOrigDll, "waveInClose");
		OrignalwaveInGetDevCapsA = GetProcAddress(hOrigDll, "waveInGetDevCapsA");
		OrignalwaveInGetDevCapsW = GetProcAddress(hOrigDll, "waveInGetDevCapsW");
		OrignalwaveInGetErrorTextA = GetProcAddress(hOrigDll, "waveInGetErrorTextA");
		OrignalwaveInGetErrorTextW = GetProcAddress(hOrigDll, "waveInGetErrorTextW");
		OrignalwaveInGetID = GetProcAddress(hOrigDll, "waveInGetID");
		OrignalwaveInGetNumDevs = GetProcAddress(hOrigDll, "waveInGetNumDevs");
		OrignalwaveInGetPosition = GetProcAddress(hOrigDll, "waveInGetPosition");
		OrignalwaveInMessage = GetProcAddress(hOrigDll, "waveInMessage");
		OrignalwaveInOpen = GetProcAddress(hOrigDll, "waveInOpen");
		OrignalwaveInPrepareHeader = GetProcAddress(hOrigDll, "waveInPrepareHeader");
		OrignalwaveInReset = GetProcAddress(hOrigDll, "waveInReset");
		OrignalwaveInStart = GetProcAddress(hOrigDll, "waveInStart");
		OrignalwaveInStop = GetProcAddress(hOrigDll, "waveInStop");
		OrignalwaveInUnprepareHeader = GetProcAddress(hOrigDll, "waveInUnprepareHeader");
		OrignalwaveOutBreakLoop = GetProcAddress(hOrigDll, "waveOutBreakLoop");
		OrignalwaveOutClose = GetProcAddress(hOrigDll, "waveOutClose");
		OrignalwaveOutGetDevCapsA = GetProcAddress(hOrigDll, "waveOutGetDevCapsA");
		OrignalwaveOutGetDevCapsW = GetProcAddress(hOrigDll, "waveOutGetDevCapsW");
		OrignalwaveOutGetErrorTextA = GetProcAddress(hOrigDll, "waveOutGetErrorTextA");
		OrignalwaveOutGetErrorTextW = GetProcAddress(hOrigDll, "waveOutGetErrorTextW");
		OrignalwaveOutGetID = GetProcAddress(hOrigDll, "waveOutGetID");
		OrignalwaveOutGetNumDevs = GetProcAddress(hOrigDll, "waveOutGetNumDevs");
		OrignalwaveOutGetPitch = GetProcAddress(hOrigDll, "waveOutGetPitch");
		OrignalwaveOutGetPlaybackRate = GetProcAddress(hOrigDll, "waveOutGetPlaybackRate");
		OrignalwaveOutGetPosition = GetProcAddress(hOrigDll, "waveOutGetPosition");
		OrignalwaveOutGetVolume = GetProcAddress(hOrigDll, "waveOutGetVolume");
		OrignalwaveOutMessage = GetProcAddress(hOrigDll, "waveOutMessage");
		OrignalwaveOutOpen = GetProcAddress(hOrigDll, "waveOutOpen");
		OrignalwaveOutPause = GetProcAddress(hOrigDll, "waveOutPause");
		OrignalwaveOutPrepareHeader = GetProcAddress(hOrigDll, "waveOutPrepareHeader");
		OrignalwaveOutReset = GetProcAddress(hOrigDll, "waveOutReset");
		OrignalwaveOutRestart = GetProcAddress(hOrigDll, "waveOutRestart");
		OrignalwaveOutSetPitch = GetProcAddress(hOrigDll, "waveOutSetPitch");
		OrignalwaveOutSetPlaybackRate = GetProcAddress(hOrigDll, "waveOutSetPlaybackRate");
		OrignalwaveOutSetVolume = GetProcAddress(hOrigDll, "waveOutSetVolume");
		OrignalwaveOutUnprepareHeader = GetProcAddress(hOrigDll, "waveOutUnprepareHeader");
		OrignalwaveOutWrite = GetProcAddress(hOrigDll, "waveOutWrite");

		break;
	}
	case DLL_PROCESS_DETACH:
	{
		MF_Uninitialize();
		FreeLibrary(hOrigDll);
	}
	break;
	}
	return TRUE;
}
