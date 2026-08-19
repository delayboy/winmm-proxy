; winmm.dll x64 proxy - export thunks, assembled with ml64 (MASM).
; Add this file to the proxy project with Item Type = Microsoft Macro Assembler
; (Project -> Build Dependencies -> Build Customizations -> check masm).
.code

extern OrignalCloseDriver:qword
FakeCloseDriver proc
	jmp qword ptr [OrignalCloseDriver]
FakeCloseDriver endp

extern OrignalDefDriverProc:qword
FakeDefDriverProc proc
	jmp qword ptr [OrignalDefDriverProc]
FakeDefDriverProc endp

extern OrignalDriverCallback:qword
FakeDriverCallback proc
	jmp qword ptr [OrignalDriverCallback]
FakeDriverCallback endp

extern OrignalDrvGetModuleHandle:qword
FakeDrvGetModuleHandle proc
	jmp qword ptr [OrignalDrvGetModuleHandle]
FakeDrvGetModuleHandle endp

extern OrignalGetDriverModuleHandle:qword
FakeGetDriverModuleHandle proc
	jmp qword ptr [OrignalGetDriverModuleHandle]
FakeGetDriverModuleHandle endp

extern OrignalOpenDriver:qword
FakeOpenDriver proc
	jmp qword ptr [OrignalOpenDriver]
FakeOpenDriver endp

extern OrignalPlaySound:qword
FakePlaySound proc
	jmp qword ptr [OrignalPlaySound]
FakePlaySound endp

extern OrignalPlaySoundA:qword
FakePlaySoundA proc
	jmp qword ptr [OrignalPlaySoundA]
FakePlaySoundA endp

extern OrignalPlaySoundW:qword
FakePlaySoundW proc
	jmp qword ptr [OrignalPlaySoundW]
FakePlaySoundW endp

extern OrignalSendDriverMessage:qword
FakeSendDriverMessage proc
	jmp qword ptr [OrignalSendDriverMessage]
FakeSendDriverMessage endp

extern OrignalWOWAppExit:qword
FakeWOWAppExit proc
	jmp qword ptr [OrignalWOWAppExit]
FakeWOWAppExit endp

extern OrignalauxGetDevCapsA:qword
FakeauxGetDevCapsA proc
	jmp qword ptr [OrignalauxGetDevCapsA]
FakeauxGetDevCapsA endp

extern OrignalauxGetDevCapsW:qword
FakeauxGetDevCapsW proc
	jmp qword ptr [OrignalauxGetDevCapsW]
FakeauxGetDevCapsW endp

extern OrignalauxGetNumDevs:qword
FakeauxGetNumDevs proc
	jmp qword ptr [OrignalauxGetNumDevs]
FakeauxGetNumDevs endp

extern OrignalauxGetVolume:qword
FakeauxGetVolume proc
	jmp qword ptr [OrignalauxGetVolume]
FakeauxGetVolume endp

extern OrignalauxOutMessage:qword
FakeauxOutMessage proc
	jmp qword ptr [OrignalauxOutMessage]
FakeauxOutMessage endp

extern OrignalauxSetVolume:qword
FakeauxSetVolume proc
	jmp qword ptr [OrignalauxSetVolume]
FakeauxSetVolume endp

extern OrignaljoyConfigChanged:qword
FakejoyConfigChanged proc
	jmp qword ptr [OrignaljoyConfigChanged]
FakejoyConfigChanged endp

extern OrignaljoyGetDevCapsA:qword
FakejoyGetDevCapsA proc
	jmp qword ptr [OrignaljoyGetDevCapsA]
FakejoyGetDevCapsA endp

extern OrignaljoyGetDevCapsW:qword
FakejoyGetDevCapsW proc
	jmp qword ptr [OrignaljoyGetDevCapsW]
FakejoyGetDevCapsW endp

extern OrignaljoyGetNumDevs:qword
FakejoyGetNumDevs proc
	jmp qword ptr [OrignaljoyGetNumDevs]
FakejoyGetNumDevs endp

extern OrignaljoyGetPos:qword
FakejoyGetPos proc
	jmp qword ptr [OrignaljoyGetPos]
FakejoyGetPos endp

extern OrignaljoyGetPosEx:qword
FakejoyGetPosEx proc
	jmp qword ptr [OrignaljoyGetPosEx]
FakejoyGetPosEx endp

extern OrignaljoyGetThreshold:qword
FakejoyGetThreshold proc
	jmp qword ptr [OrignaljoyGetThreshold]
FakejoyGetThreshold endp

extern OrignaljoyReleaseCapture:qword
FakejoyReleaseCapture proc
	jmp qword ptr [OrignaljoyReleaseCapture]
FakejoyReleaseCapture endp

extern OrignaljoySetCapture:qword
FakejoySetCapture proc
	jmp qword ptr [OrignaljoySetCapture]
FakejoySetCapture endp

extern OrignaljoySetThreshold:qword
FakejoySetThreshold proc
	jmp qword ptr [OrignaljoySetThreshold]
FakejoySetThreshold endp

extern OrignalmciDriverNotify:qword
FakemciDriverNotify proc
	jmp qword ptr [OrignalmciDriverNotify]
FakemciDriverNotify endp

extern OrignalmciDriverYield:qword
FakemciDriverYield proc
	jmp qword ptr [OrignalmciDriverYield]
FakemciDriverYield endp

extern OrignalmciExecute:qword
FakemciExecute proc
	jmp qword ptr [OrignalmciExecute]
FakemciExecute endp

extern OrignalmciFreeCommandResource:qword
FakemciFreeCommandResource proc
	jmp qword ptr [OrignalmciFreeCommandResource]
FakemciFreeCommandResource endp

extern OrignalmciGetCreatorTask:qword
FakemciGetCreatorTask proc
	jmp qword ptr [OrignalmciGetCreatorTask]
FakemciGetCreatorTask endp

extern OrignalmciGetDeviceIDA:qword
FakemciGetDeviceIDA proc
	jmp qword ptr [OrignalmciGetDeviceIDA]
FakemciGetDeviceIDA endp

extern OrignalmciGetDeviceIDFromElementIDA:qword
FakemciGetDeviceIDFromElementIDA proc
	jmp qword ptr [OrignalmciGetDeviceIDFromElementIDA]
FakemciGetDeviceIDFromElementIDA endp

extern OrignalmciGetDeviceIDFromElementIDW:qword
FakemciGetDeviceIDFromElementIDW proc
	jmp qword ptr [OrignalmciGetDeviceIDFromElementIDW]
FakemciGetDeviceIDFromElementIDW endp

extern OrignalmciGetDeviceIDW:qword
FakemciGetDeviceIDW proc
	jmp qword ptr [OrignalmciGetDeviceIDW]
FakemciGetDeviceIDW endp

extern OrignalmciGetDriverData:qword
FakemciGetDriverData proc
	jmp qword ptr [OrignalmciGetDriverData]
FakemciGetDriverData endp

extern OrignalmciGetErrorStringA:qword
FakemciGetErrorStringA proc
	jmp qword ptr [OrignalmciGetErrorStringA]
FakemciGetErrorStringA endp

extern OrignalmciGetErrorStringW:qword
FakemciGetErrorStringW proc
	jmp qword ptr [OrignalmciGetErrorStringW]
FakemciGetErrorStringW endp

extern OrignalmciGetYieldProc:qword
FakemciGetYieldProc proc
	jmp qword ptr [OrignalmciGetYieldProc]
FakemciGetYieldProc endp

extern OrignalmciLoadCommandResource:qword
FakemciLoadCommandResource proc
	jmp qword ptr [OrignalmciLoadCommandResource]
FakemciLoadCommandResource endp

extern OrignalmciSendCommandA:qword
FakemciSendCommandA proc
	jmp qword ptr [OrignalmciSendCommandA]
FakemciSendCommandA endp

extern OrignalmciSendCommandW:qword
FakemciSendCommandW proc
	jmp qword ptr [OrignalmciSendCommandW]
FakemciSendCommandW endp

extern OrignalmciSendStringA:qword
FakemciSendStringA proc
	jmp qword ptr [OrignalmciSendStringA]
FakemciSendStringA endp

extern OrignalmciSendStringW:qword
FakemciSendStringW proc
	jmp qword ptr [OrignalmciSendStringW]
FakemciSendStringW endp

extern OrignalmciSetDriverData:qword
FakemciSetDriverData proc
	jmp qword ptr [OrignalmciSetDriverData]
FakemciSetDriverData endp

extern OrignalmciSetYieldProc:qword
FakemciSetYieldProc proc
	jmp qword ptr [OrignalmciSetYieldProc]
FakemciSetYieldProc endp

extern OrignalmidiConnect:qword
FakemidiConnect proc
	jmp qword ptr [OrignalmidiConnect]
FakemidiConnect endp

extern OrignalmidiDisconnect:qword
FakemidiDisconnect proc
	jmp qword ptr [OrignalmidiDisconnect]
FakemidiDisconnect endp

extern OrignalmidiInAddBuffer:qword
FakemidiInAddBuffer proc
	jmp qword ptr [OrignalmidiInAddBuffer]
FakemidiInAddBuffer endp

extern OrignalmidiInClose:qword
FakemidiInClose proc
	jmp qword ptr [OrignalmidiInClose]
FakemidiInClose endp

extern OrignalmidiInGetDevCapsA:qword
FakemidiInGetDevCapsA proc
	jmp qword ptr [OrignalmidiInGetDevCapsA]
FakemidiInGetDevCapsA endp

extern OrignalmidiInGetDevCapsW:qword
FakemidiInGetDevCapsW proc
	jmp qword ptr [OrignalmidiInGetDevCapsW]
FakemidiInGetDevCapsW endp

extern OrignalmidiInGetErrorTextA:qword
FakemidiInGetErrorTextA proc
	jmp qword ptr [OrignalmidiInGetErrorTextA]
FakemidiInGetErrorTextA endp

extern OrignalmidiInGetErrorTextW:qword
FakemidiInGetErrorTextW proc
	jmp qword ptr [OrignalmidiInGetErrorTextW]
FakemidiInGetErrorTextW endp

extern OrignalmidiInGetID:qword
FakemidiInGetID proc
	jmp qword ptr [OrignalmidiInGetID]
FakemidiInGetID endp

extern OrignalmidiInGetNumDevs:qword
FakemidiInGetNumDevs proc
	jmp qword ptr [OrignalmidiInGetNumDevs]
FakemidiInGetNumDevs endp

extern OrignalmidiInMessage:qword
FakemidiInMessage proc
	jmp qword ptr [OrignalmidiInMessage]
FakemidiInMessage endp

extern OrignalmidiInOpen:qword
FakemidiInOpen proc
	jmp qword ptr [OrignalmidiInOpen]
FakemidiInOpen endp

extern OrignalmidiInPrepareHeader:qword
FakemidiInPrepareHeader proc
	jmp qword ptr [OrignalmidiInPrepareHeader]
FakemidiInPrepareHeader endp

extern OrignalmidiInReset:qword
FakemidiInReset proc
	jmp qword ptr [OrignalmidiInReset]
FakemidiInReset endp

extern OrignalmidiInStart:qword
FakemidiInStart proc
	jmp qword ptr [OrignalmidiInStart]
FakemidiInStart endp

extern OrignalmidiInStop:qword
FakemidiInStop proc
	jmp qword ptr [OrignalmidiInStop]
FakemidiInStop endp

extern OrignalmidiInUnprepareHeader:qword
FakemidiInUnprepareHeader proc
	jmp qword ptr [OrignalmidiInUnprepareHeader]
FakemidiInUnprepareHeader endp

extern OrignalmidiOutCacheDrumPatches:qword
FakemidiOutCacheDrumPatches proc
	jmp qword ptr [OrignalmidiOutCacheDrumPatches]
FakemidiOutCacheDrumPatches endp

extern OrignalmidiOutCachePatches:qword
FakemidiOutCachePatches proc
	jmp qword ptr [OrignalmidiOutCachePatches]
FakemidiOutCachePatches endp

extern OrignalmidiOutClose:qword
FakemidiOutClose proc
	jmp qword ptr [OrignalmidiOutClose]
FakemidiOutClose endp

extern OrignalmidiOutGetDevCapsA:qword
FakemidiOutGetDevCapsA proc
	jmp qword ptr [OrignalmidiOutGetDevCapsA]
FakemidiOutGetDevCapsA endp

extern OrignalmidiOutGetDevCapsW:qword
FakemidiOutGetDevCapsW proc
	jmp qword ptr [OrignalmidiOutGetDevCapsW]
FakemidiOutGetDevCapsW endp

extern OrignalmidiOutGetErrorTextA:qword
FakemidiOutGetErrorTextA proc
	jmp qword ptr [OrignalmidiOutGetErrorTextA]
FakemidiOutGetErrorTextA endp

extern OrignalmidiOutGetErrorTextW:qword
FakemidiOutGetErrorTextW proc
	jmp qword ptr [OrignalmidiOutGetErrorTextW]
FakemidiOutGetErrorTextW endp

extern OrignalmidiOutGetID:qword
FakemidiOutGetID proc
	jmp qword ptr [OrignalmidiOutGetID]
FakemidiOutGetID endp

extern OrignalmidiOutGetNumDevs:qword
FakemidiOutGetNumDevs proc
	jmp qword ptr [OrignalmidiOutGetNumDevs]
FakemidiOutGetNumDevs endp

extern OrignalmidiOutGetVolume:qword
FakemidiOutGetVolume proc
	jmp qword ptr [OrignalmidiOutGetVolume]
FakemidiOutGetVolume endp

extern OrignalmidiOutLongMsg:qword
FakemidiOutLongMsg proc
	jmp qword ptr [OrignalmidiOutLongMsg]
FakemidiOutLongMsg endp

extern OrignalmidiOutMessage:qword
FakemidiOutMessage proc
	jmp qword ptr [OrignalmidiOutMessage]
FakemidiOutMessage endp

extern OrignalmidiOutOpen:qword
FakemidiOutOpen proc
	jmp qword ptr [OrignalmidiOutOpen]
FakemidiOutOpen endp

extern OrignalmidiOutPrepareHeader:qword
FakemidiOutPrepareHeader proc
	jmp qword ptr [OrignalmidiOutPrepareHeader]
FakemidiOutPrepareHeader endp

extern OrignalmidiOutReset:qword
FakemidiOutReset proc
	jmp qword ptr [OrignalmidiOutReset]
FakemidiOutReset endp

extern OrignalmidiOutSetVolume:qword
FakemidiOutSetVolume proc
	jmp qword ptr [OrignalmidiOutSetVolume]
FakemidiOutSetVolume endp

extern OrignalmidiOutShortMsg:qword
FakemidiOutShortMsg proc
	jmp qword ptr [OrignalmidiOutShortMsg]
FakemidiOutShortMsg endp

extern OrignalmidiOutUnprepareHeader:qword
FakemidiOutUnprepareHeader proc
	jmp qword ptr [OrignalmidiOutUnprepareHeader]
FakemidiOutUnprepareHeader endp

extern OrignalmidiStreamClose:qword
FakemidiStreamClose proc
	jmp qword ptr [OrignalmidiStreamClose]
FakemidiStreamClose endp

extern OrignalmidiStreamOpen:qword
FakemidiStreamOpen proc
	jmp qword ptr [OrignalmidiStreamOpen]
FakemidiStreamOpen endp

extern OrignalmidiStreamOut:qword
FakemidiStreamOut proc
	jmp qword ptr [OrignalmidiStreamOut]
FakemidiStreamOut endp

extern OrignalmidiStreamPause:qword
FakemidiStreamPause proc
	jmp qword ptr [OrignalmidiStreamPause]
FakemidiStreamPause endp

extern OrignalmidiStreamPosition:qword
FakemidiStreamPosition proc
	jmp qword ptr [OrignalmidiStreamPosition]
FakemidiStreamPosition endp

extern OrignalmidiStreamProperty:qword
FakemidiStreamProperty proc
	jmp qword ptr [OrignalmidiStreamProperty]
FakemidiStreamProperty endp

extern OrignalmidiStreamRestart:qword
FakemidiStreamRestart proc
	jmp qword ptr [OrignalmidiStreamRestart]
FakemidiStreamRestart endp

extern OrignalmidiStreamStop:qword
FakemidiStreamStop proc
	jmp qword ptr [OrignalmidiStreamStop]
FakemidiStreamStop endp

extern OrignalmixerClose:qword
FakemixerClose proc
	jmp qword ptr [OrignalmixerClose]
FakemixerClose endp

extern OrignalmixerGetControlDetailsA:qword
FakemixerGetControlDetailsA proc
	jmp qword ptr [OrignalmixerGetControlDetailsA]
FakemixerGetControlDetailsA endp

extern OrignalmixerGetControlDetailsW:qword
FakemixerGetControlDetailsW proc
	jmp qword ptr [OrignalmixerGetControlDetailsW]
FakemixerGetControlDetailsW endp

extern OrignalmixerGetDevCapsA:qword
FakemixerGetDevCapsA proc
	jmp qword ptr [OrignalmixerGetDevCapsA]
FakemixerGetDevCapsA endp

extern OrignalmixerGetDevCapsW:qword
FakemixerGetDevCapsW proc
	jmp qword ptr [OrignalmixerGetDevCapsW]
FakemixerGetDevCapsW endp

extern OrignalmixerGetID:qword
FakemixerGetID proc
	jmp qword ptr [OrignalmixerGetID]
FakemixerGetID endp

extern OrignalmixerGetLineControlsA:qword
FakemixerGetLineControlsA proc
	jmp qword ptr [OrignalmixerGetLineControlsA]
FakemixerGetLineControlsA endp

extern OrignalmixerGetLineControlsW:qword
FakemixerGetLineControlsW proc
	jmp qword ptr [OrignalmixerGetLineControlsW]
FakemixerGetLineControlsW endp

extern OrignalmixerGetLineInfoA:qword
FakemixerGetLineInfoA proc
	jmp qword ptr [OrignalmixerGetLineInfoA]
FakemixerGetLineInfoA endp

extern OrignalmixerGetLineInfoW:qword
FakemixerGetLineInfoW proc
	jmp qword ptr [OrignalmixerGetLineInfoW]
FakemixerGetLineInfoW endp

extern OrignalmixerGetNumDevs:qword
FakemixerGetNumDevs proc
	jmp qword ptr [OrignalmixerGetNumDevs]
FakemixerGetNumDevs endp

extern OrignalmixerMessage:qword
FakemixerMessage proc
	jmp qword ptr [OrignalmixerMessage]
FakemixerMessage endp

extern OrignalmixerOpen:qword
FakemixerOpen proc
	jmp qword ptr [OrignalmixerOpen]
FakemixerOpen endp

extern OrignalmixerSetControlDetails:qword
FakemixerSetControlDetails proc
	jmp qword ptr [OrignalmixerSetControlDetails]
FakemixerSetControlDetails endp

extern OrignalmmDrvInstall:qword
FakemmDrvInstall proc
	jmp qword ptr [OrignalmmDrvInstall]
FakemmDrvInstall endp

extern OrignalmmGetCurrentTask:qword
FakemmGetCurrentTask proc
	jmp qword ptr [OrignalmmGetCurrentTask]
FakemmGetCurrentTask endp

extern OrignalmmTaskBlock:qword
FakemmTaskBlock proc
	jmp qword ptr [OrignalmmTaskBlock]
FakemmTaskBlock endp

extern OrignalmmTaskCreate:qword
FakemmTaskCreate proc
	jmp qword ptr [OrignalmmTaskCreate]
FakemmTaskCreate endp

extern OrignalmmTaskSignal:qword
FakemmTaskSignal proc
	jmp qword ptr [OrignalmmTaskSignal]
FakemmTaskSignal endp

extern OrignalmmTaskYield:qword
FakemmTaskYield proc
	jmp qword ptr [OrignalmmTaskYield]
FakemmTaskYield endp

extern OrignalmmioAdvance:qword
FakemmioAdvance proc
	jmp qword ptr [OrignalmmioAdvance]
FakemmioAdvance endp

extern OrignalmmioAscend:qword
FakemmioAscend proc
	jmp qword ptr [OrignalmmioAscend]
FakemmioAscend endp

extern OrignalmmioClose:qword
FakemmioClose proc
	jmp qword ptr [OrignalmmioClose]
FakemmioClose endp

extern OrignalmmioCreateChunk:qword
FakemmioCreateChunk proc
	jmp qword ptr [OrignalmmioCreateChunk]
FakemmioCreateChunk endp

extern OrignalmmioDescend:qword
FakemmioDescend proc
	jmp qword ptr [OrignalmmioDescend]
FakemmioDescend endp

extern OrignalmmioFlush:qword
FakemmioFlush proc
	jmp qword ptr [OrignalmmioFlush]
FakemmioFlush endp

extern OrignalmmioGetInfo:qword
FakemmioGetInfo proc
	jmp qword ptr [OrignalmmioGetInfo]
FakemmioGetInfo endp

extern OrignalmmioInstallIOProcA:qword
FakemmioInstallIOProcA proc
	jmp qword ptr [OrignalmmioInstallIOProcA]
FakemmioInstallIOProcA endp

extern OrignalmmioInstallIOProcW:qword
FakemmioInstallIOProcW proc
	jmp qword ptr [OrignalmmioInstallIOProcW]
FakemmioInstallIOProcW endp

extern OrignalmmioOpenA:qword
FakemmioOpenA proc
	jmp qword ptr [OrignalmmioOpenA]
FakemmioOpenA endp

extern OrignalmmioOpenW:qword
FakemmioOpenW proc
	jmp qword ptr [OrignalmmioOpenW]
FakemmioOpenW endp

extern OrignalmmioRead:qword
FakemmioRead proc
	jmp qword ptr [OrignalmmioRead]
FakemmioRead endp

extern OrignalmmioRenameA:qword
FakemmioRenameA proc
	jmp qword ptr [OrignalmmioRenameA]
FakemmioRenameA endp

extern OrignalmmioRenameW:qword
FakemmioRenameW proc
	jmp qword ptr [OrignalmmioRenameW]
FakemmioRenameW endp

extern OrignalmmioSeek:qword
FakemmioSeek proc
	jmp qword ptr [OrignalmmioSeek]
FakemmioSeek endp

extern OrignalmmioSendMessage:qword
FakemmioSendMessage proc
	jmp qword ptr [OrignalmmioSendMessage]
FakemmioSendMessage endp

extern OrignalmmioSetBuffer:qword
FakemmioSetBuffer proc
	jmp qword ptr [OrignalmmioSetBuffer]
FakemmioSetBuffer endp

extern OrignalmmioSetInfo:qword
FakemmioSetInfo proc
	jmp qword ptr [OrignalmmioSetInfo]
FakemmioSetInfo endp

extern OrignalmmioStringToFOURCCA:qword
FakemmioStringToFOURCCA proc
	jmp qword ptr [OrignalmmioStringToFOURCCA]
FakemmioStringToFOURCCA endp

extern OrignalmmioStringToFOURCCW:qword
FakemmioStringToFOURCCW proc
	jmp qword ptr [OrignalmmioStringToFOURCCW]
FakemmioStringToFOURCCW endp

extern OrignalmmioWrite:qword
FakemmioWrite proc
	jmp qword ptr [OrignalmmioWrite]
FakemmioWrite endp

extern OrignalmmsystemGetVersion:qword
FakemmsystemGetVersion proc
	jmp qword ptr [OrignalmmsystemGetVersion]
FakemmsystemGetVersion endp

extern OrignalsndPlaySoundA:qword
FakesndPlaySoundA proc
	jmp qword ptr [OrignalsndPlaySoundA]
FakesndPlaySoundA endp

extern OrignalsndPlaySoundW:qword
FakesndPlaySoundW proc
	jmp qword ptr [OrignalsndPlaySoundW]
FakesndPlaySoundW endp

extern OrignaltimeBeginPeriod:qword
FaketimeBeginPeriod proc
	jmp qword ptr [OrignaltimeBeginPeriod]
FaketimeBeginPeriod endp

extern OrignaltimeEndPeriod:qword
FaketimeEndPeriod proc
	jmp qword ptr [OrignaltimeEndPeriod]
FaketimeEndPeriod endp

extern OrignaltimeGetDevCaps:qword
FaketimeGetDevCaps proc
	jmp qword ptr [OrignaltimeGetDevCaps]
FaketimeGetDevCaps endp

extern OrignaltimeGetSystemTime:qword
FaketimeGetSystemTime proc
	jmp qword ptr [OrignaltimeGetSystemTime]
FaketimeGetSystemTime endp

extern OrignaltimeGetTime:qword
FaketimeGetTime proc
	jmp qword ptr [OrignaltimeGetTime]
FaketimeGetTime endp

extern OrignaltimeKillEvent:qword
FaketimeKillEvent proc
	jmp qword ptr [OrignaltimeKillEvent]
FaketimeKillEvent endp

extern OrignaltimeSetEvent:qword
FaketimeSetEvent proc
	jmp qword ptr [OrignaltimeSetEvent]
FaketimeSetEvent endp

extern OrignalwaveInAddBuffer:qword
FakewaveInAddBuffer proc
	jmp qword ptr [OrignalwaveInAddBuffer]
FakewaveInAddBuffer endp

extern OrignalwaveInClose:qword
FakewaveInClose proc
	jmp qword ptr [OrignalwaveInClose]
FakewaveInClose endp

extern OrignalwaveInGetDevCapsA:qword
FakewaveInGetDevCapsA proc
	jmp qword ptr [OrignalwaveInGetDevCapsA]
FakewaveInGetDevCapsA endp

extern OrignalwaveInGetDevCapsW:qword
FakewaveInGetDevCapsW proc
	jmp qword ptr [OrignalwaveInGetDevCapsW]
FakewaveInGetDevCapsW endp

extern OrignalwaveInGetErrorTextA:qword
FakewaveInGetErrorTextA proc
	jmp qword ptr [OrignalwaveInGetErrorTextA]
FakewaveInGetErrorTextA endp

extern OrignalwaveInGetErrorTextW:qword
FakewaveInGetErrorTextW proc
	jmp qword ptr [OrignalwaveInGetErrorTextW]
FakewaveInGetErrorTextW endp

extern OrignalwaveInGetID:qword
FakewaveInGetID proc
	jmp qword ptr [OrignalwaveInGetID]
FakewaveInGetID endp

extern OrignalwaveInGetNumDevs:qword
FakewaveInGetNumDevs proc
	jmp qword ptr [OrignalwaveInGetNumDevs]
FakewaveInGetNumDevs endp

extern OrignalwaveInGetPosition:qword
FakewaveInGetPosition proc
	jmp qword ptr [OrignalwaveInGetPosition]
FakewaveInGetPosition endp

extern OrignalwaveInMessage:qword
FakewaveInMessage proc
	jmp qword ptr [OrignalwaveInMessage]
FakewaveInMessage endp

extern OrignalwaveInOpen:qword
FakewaveInOpen proc
	jmp qword ptr [OrignalwaveInOpen]
FakewaveInOpen endp

extern OrignalwaveInPrepareHeader:qword
FakewaveInPrepareHeader proc
	jmp qword ptr [OrignalwaveInPrepareHeader]
FakewaveInPrepareHeader endp

extern OrignalwaveInReset:qword
FakewaveInReset proc
	jmp qword ptr [OrignalwaveInReset]
FakewaveInReset endp

extern OrignalwaveInStart:qword
FakewaveInStart proc
	jmp qword ptr [OrignalwaveInStart]
FakewaveInStart endp

extern OrignalwaveInStop:qword
FakewaveInStop proc
	jmp qword ptr [OrignalwaveInStop]
FakewaveInStop endp

extern OrignalwaveInUnprepareHeader:qword
FakewaveInUnprepareHeader proc
	jmp qword ptr [OrignalwaveInUnprepareHeader]
FakewaveInUnprepareHeader endp

extern OrignalwaveOutBreakLoop:qword
FakewaveOutBreakLoop proc
	jmp qword ptr [OrignalwaveOutBreakLoop]
FakewaveOutBreakLoop endp

extern OrignalwaveOutClose:qword
FakewaveOutClose proc
	jmp qword ptr [OrignalwaveOutClose]
FakewaveOutClose endp

extern OrignalwaveOutGetDevCapsA:qword
FakewaveOutGetDevCapsA proc
	jmp qword ptr [OrignalwaveOutGetDevCapsA]
FakewaveOutGetDevCapsA endp

extern OrignalwaveOutGetDevCapsW:qword
FakewaveOutGetDevCapsW proc
	jmp qword ptr [OrignalwaveOutGetDevCapsW]
FakewaveOutGetDevCapsW endp

extern OrignalwaveOutGetErrorTextA:qword
FakewaveOutGetErrorTextA proc
	jmp qword ptr [OrignalwaveOutGetErrorTextA]
FakewaveOutGetErrorTextA endp

extern OrignalwaveOutGetErrorTextW:qword
FakewaveOutGetErrorTextW proc
	jmp qword ptr [OrignalwaveOutGetErrorTextW]
FakewaveOutGetErrorTextW endp

extern OrignalwaveOutGetID:qword
FakewaveOutGetID proc
	jmp qword ptr [OrignalwaveOutGetID]
FakewaveOutGetID endp

extern OrignalwaveOutGetNumDevs:qword
FakewaveOutGetNumDevs proc
	jmp qword ptr [OrignalwaveOutGetNumDevs]
FakewaveOutGetNumDevs endp

extern OrignalwaveOutGetPitch:qword
FakewaveOutGetPitch proc
	jmp qword ptr [OrignalwaveOutGetPitch]
FakewaveOutGetPitch endp

extern OrignalwaveOutGetPlaybackRate:qword
FakewaveOutGetPlaybackRate proc
	jmp qword ptr [OrignalwaveOutGetPlaybackRate]
FakewaveOutGetPlaybackRate endp

extern OrignalwaveOutGetPosition:qword
FakewaveOutGetPosition proc
	jmp qword ptr [OrignalwaveOutGetPosition]
FakewaveOutGetPosition endp

extern OrignalwaveOutGetVolume:qword
FakewaveOutGetVolume proc
	jmp qword ptr [OrignalwaveOutGetVolume]
FakewaveOutGetVolume endp

extern OrignalwaveOutMessage:qword
FakewaveOutMessage proc
	jmp qword ptr [OrignalwaveOutMessage]
FakewaveOutMessage endp

extern OrignalwaveOutOpen:qword
FakewaveOutOpen proc
	jmp qword ptr [OrignalwaveOutOpen]
FakewaveOutOpen endp

extern OrignalwaveOutPause:qword
FakewaveOutPause proc
	jmp qword ptr [OrignalwaveOutPause]
FakewaveOutPause endp

extern OrignalwaveOutPrepareHeader:qword
FakewaveOutPrepareHeader proc
	jmp qword ptr [OrignalwaveOutPrepareHeader]
FakewaveOutPrepareHeader endp

extern OrignalwaveOutReset:qword
FakewaveOutReset proc
	jmp qword ptr [OrignalwaveOutReset]
FakewaveOutReset endp

extern OrignalwaveOutRestart:qword
FakewaveOutRestart proc
	jmp qword ptr [OrignalwaveOutRestart]
FakewaveOutRestart endp

extern OrignalwaveOutSetPitch:qword
FakewaveOutSetPitch proc
	jmp qword ptr [OrignalwaveOutSetPitch]
FakewaveOutSetPitch endp

extern OrignalwaveOutSetPlaybackRate:qword
FakewaveOutSetPlaybackRate proc
	jmp qword ptr [OrignalwaveOutSetPlaybackRate]
FakewaveOutSetPlaybackRate endp

extern OrignalwaveOutSetVolume:qword
FakewaveOutSetVolume proc
	jmp qword ptr [OrignalwaveOutSetVolume]
FakewaveOutSetVolume endp

extern OrignalwaveOutUnprepareHeader:qword
FakewaveOutUnprepareHeader proc
	jmp qword ptr [OrignalwaveOutUnprepareHeader]
FakewaveOutUnprepareHeader endp

extern OrignalwaveOutWrite:qword
FakewaveOutWrite proc
	jmp qword ptr [OrignalwaveOutWrite]
FakewaveOutWrite endp

end
