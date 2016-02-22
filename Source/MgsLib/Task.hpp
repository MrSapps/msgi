#pragma once

#include "MgsFunction.hpp"

void TaskCpp_ForceLink();

// 0051FEFC
void __cdecl Task_Init(int taskId, void* fnThreadProc);

// 004605AE
DWORD __cdecl Task_Pause();

// 0051FEDC
DWORD __cdecl Task_ResumeQ();

// 0051FFC3
void* __cdecl Task_TerminateQ();

// 00520001
void __cdecl Task_SignalQ(DWORD taskId);

// 00520095
DWORD __cdecl Task_WaitForSignalQ();

// 00445490
DWORD __cdecl j_Task_WaitForSignalQ();

// 00508115
int __cdecl Task_SyncCall(int a1);

// 0051FF6A
int __cdecl Task_GetExitCodeQ(DWORD taskId);

// 00520032
void __cdecl Task_EndSelf();
