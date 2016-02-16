#include "stdafx.h"
#include "Task.hpp"
#include <process.h>

struct Task
{
    DWORD mTaskId;
    unsigned int mThreadId;
    HANDLE mThreadHandle;
    HANDLE mEventHandle;
};
static_assert(sizeof(Task) == 0x10, "Task size must be 0x10");
using ThreadProc = unsigned int(__stdcall *)(void *);

MGS_ARY(1, 0x77C710, Task, 32, gTasks_77C710, {});
MGS_ARY(1, 0x68C4C0, ThreadProc, 32, gFnTbl_off_68C4C0, {}); // TODO: Populate
MGS_VAR(1, 0x7346FC, DWORD, gTaskRequest_dword_7346FC, 0);
MGS_VAR(1, 0x7346F8, DWORD, gTaskResult_dword_7346F8, 0);

// TODO: Enable when bug that breaks 2nd codec convo is fixed
/*
MSG_FUNC_IMPL(0x0051FEFC, Task_Init);
MSG_FUNC_IMPL(0x004605AE, Task_Pause);
MSG_FUNC_IMPL(0x0051FEDC, Task_ResumeQ);
MSG_FUNC_IMPL(0x0051FFC3, Task_TerminateQ);
MSG_FUNC_IMPL(0x00520001, Task_SignalQ);
MSG_FUNC_IMPL(0x00520095, Task_WaitForSignalQ);
MSG_FUNC_IMPL(0x00445490, j_Task_WaitForSignalQ);
MSG_FUNC_IMPL(0x00508115, Task_SyncCall);
MSG_FUNC_IMPL(0x0051FF6A, Task_GetExitCodeQ);

//MSG_FUNC_IMPL(0x00520032, Task_EndSelf);
MSG_FUNC_NOT_IMPL(0x00520032, decltype(Task_EndSelf), _Task_EndSelf);
*/

void TaskCpp_ForceLink()
{

}

void __cdecl Task_Init(int taskId, void* fnThreadProc)
{
    int freeTaskIdx = -1;
    if (fnThreadProc)
    {
        for (int i = 0; i < 32; i++)
        {
            if (!gTasks_77C710[i].mThreadHandle)
            {
                freeTaskIdx = i;
                break;
            }
        }

        if (freeTaskIdx == -1)
        {
            return;
        }

        gTasks_77C710[freeTaskIdx].mTaskId = taskId;
        gTasks_77C710[freeTaskIdx].mEventHandle = CreateEventA(0, 1, 0, 0);
        gTasks_77C710[freeTaskIdx].mThreadHandle = (HANDLE)_beginthreadex(
            0,
            0x10000u,
            gFnTbl_off_68C4C0[freeTaskIdx],
            fnThreadProc,
            0,
            &gTasks_77C710[freeTaskIdx].mThreadId);
    }
}

// TODO: Not tested
DWORD __cdecl Task_Pause()
{
    DWORD result = 0;
    for (int i = 0; i < 32; i++)
    {
        if (gTasks_77C710[i].mThreadHandle)
        {
            result = SuspendThread(gTasks_77C710[i].mThreadHandle);
        }
    }
    return result;
}

// TODO: Not tested
DWORD __cdecl Task_ResumeQ()
{
    DWORD result = 0;
    for (int i = 0; i < 32; i++)
    {
        if (gTasks_77C710[i].mThreadHandle)
        {
            result = ResumeThread(gTasks_77C710[i].mThreadHandle);
        }
    }
    return result;
}

// TODO: Not tested
void* __cdecl Task_TerminateQ()
{
    DWORD result = 0;
    for (int i = 0; i < 32; i++)
    {
        if (gTasks_77C710[i].mThreadHandle)
        {
            result = TerminateThread(gTasks_77C710[i].mThreadHandle, 0);
            CloseHandle(gTasks_77C710[i].mThreadHandle);
            // Existing bug - probably should never terminate threads, plus leaks
            // the event handle.
        }
    }
    return memset(gTasks_77C710, 0, 512);
}

void __cdecl Task_SignalQ(int taskId)
{
    int idx = 0;
    Task* result = &gTasks_77C710[0];
    while (result->mTaskId != taskId || !result->mEventHandle)
    {
        result++;
        ++idx;
        if (idx >= 32)
        {
            return;
        }
    }
    SetEvent(gTasks_77C710[idx].mEventHandle);
}

DWORD __cdecl Task_WaitForSignalQ()
{
    const DWORD threadId = GetCurrentThreadId();
    int taskIdx = 0;
    Task* pTaskInfo = &gTasks_77C710[0];
    while (pTaskInfo->mThreadId != threadId)
    {
        ++pTaskInfo;
        ++taskIdx;
        if (taskIdx >= 32)
        {
            return threadId;
        }
    }

    ResetEvent(gTasks_77C710[taskIdx].mEventHandle);
    return WaitForSingleObject(gTasks_77C710[taskIdx].mEventHandle, INFINITE);
}

DWORD __cdecl j_Task_WaitForSignalQ()
{
    return Task_WaitForSignalQ();
}

// TODO: Not tested
int __cdecl Task_SyncCall(int a1)
{
    gTaskRequest_dword_7346FC = a1;
    printf("REQUEST %X\n", a1);
    j_Task_WaitForSignalQ();
    printf("RESULT %X\n", gTaskResult_dword_7346F8);
    return gTaskResult_dword_7346F8;
}

int __cdecl Task_GetExitCodeQ(DWORD taskId)
{
    int taskIdx = 0;
    Task* pTask = &gTasks_77C710[0];
    while (pTask->mTaskId != taskId)
    {
        ++pTask;
        ++taskIdx;
        if (taskIdx >= 32)
        {
            return 0;
        }
    }
    
    if (!GetExitCodeThread(gTasks_77C710[taskIdx].mThreadHandle, &taskId))
    {
        return 0;
    }

    int result = 0;
    if (taskId == 259)
    {
        result = 6;
    }
    else
    {
        result = gTasks_77C710[taskIdx].mEventHandle != 0 ? 4 : 0;
    }

    return result;
}

// Seems to be called at the end of the thread proc, ends *this* thread
// FIX ME: Ending codec convo calls this and then it breaks
void __cdecl __Task_EndSelf()
{
    const DWORD currentThreadId = GetCurrentThreadId();
    int taskIdx = 0;
    Task* pTaskInfo = &gTasks_77C710[0];
    
    while (pTaskInfo->mThreadId != currentThreadId)
    {
        pTaskInfo++;
        ++taskIdx;
        if (taskIdx >= 32)
        {
            _endthreadex(0);
            return;
        }
    }

    CloseHandle(gTasks_77C710[taskIdx].mEventHandle);
    CloseHandle(gTasks_77C710[taskIdx].mThreadHandle);
    memset(&gTasks_77C710[taskIdx], 0, 16u);

    _endthreadex(0);
}
