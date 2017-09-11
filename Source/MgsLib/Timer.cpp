#include "stdafx.h"
#include "Timer.hpp"
#include "types.hpp"

#define TIMER_IMPL true

MGS_VAR(1, 0x006DF218, LARGE_INTEGER, gTimer_Frequency, {});
MGS_VAR(1, 0x006FC710, LARGE_INTEGER, gTimer_PerformanceCount, {});

MGS_VAR(1, 0x006FC7D8, u64, dword_6FC7D8, 0);
MGS_VAR(1, 0x006DF248, u64, qword_6DF248, 0);

MGS_VAR(1, 0x995648, PerformanceFreqStruct, gPerformanceFreq_995648, {});

static void TimerImpl(DWORD divisor, DWORD value_6FC7D8)
{
    QueryPerformanceFrequency(&gTimer_Frequency);
    QueryPerformanceCounter(&gTimer_PerformanceCount);
    gTimer_Frequency.QuadPart /= divisor;
    dword_6FC7D8 = value_6FC7D8;
    qword_6DF248 = 0;
}

void CC Timer_30_1()
{
    TimerImpl(30, 0);
}
MGS_FUNC_IMPLEX(0x00421680, Timer_30_1, TIMER_IMPL);

void CC Timer_30()
{
    TimerImpl(30, 1);
}
MGS_FUNC_IMPLEX(0x004217A0, Timer_30, TIMER_IMPL);

void CC Timer_24()
{
    TimerImpl(24, 0);
}
MGS_FUNC_IMPLEX(0x004216E0, Timer_24, TIMER_IMPL);

void CC Timer_15()
{
    TimerImpl(15, 1);
}
MGS_FUNC_IMPLEX(0x00421740, Timer_15, TIMER_IMPL);

LARGE_INTEGER CC TimerInitBaseLineAndGetCurrentTime_5201A6()
{
    LARGE_INTEGER currentCounter = {};
    static bool sDoneQuery_dword_77C920 = false;
    static LARGE_INTEGER sLargeInteger_stru_995690 = {};

    if (!sDoneQuery_dword_77C920)
    {
        QueryPerformanceFrequency(&sLargeInteger_stru_995690);
        gPerformanceFreq_995648.mFreq60 = sLargeInteger_stru_995690.QuadPart / 60;
        gPerformanceFreq_995648.mFreq30 = sLargeInteger_stru_995690.QuadPart / 30;
        gPerformanceFreq_995648.mFreq24 = sLargeInteger_stru_995690.QuadPart / 24;
        gPerformanceFreq_995648.mFreq23 = sLargeInteger_stru_995690.QuadPart / 23;
        gPerformanceFreq_995648.mFreq22 = sLargeInteger_stru_995690.QuadPart / 22;
        gPerformanceFreq_995648.mFreq21 = sLargeInteger_stru_995690.QuadPart / 21;
        gPerformanceFreq_995648.mFreq20 = sLargeInteger_stru_995690.QuadPart / 20;
        gPerformanceFreq_995648.mFreq25 = sLargeInteger_stru_995690.QuadPart / 25;
        gPerformanceFreq_995648.mFreq15 = sLargeInteger_stru_995690.QuadPart / 15;
        sDoneQuery_dword_77C920 = true;
    }
    QueryPerformanceCounter(&currentCounter);
    return currentCounter;
}
//MGS_FUNC_IMPLEX(0x005201A6, TimerInitBaseLineAndGetCurrentTime_5201A6, TIMER_IMPL);
// TODO: FIX ME MGS_FUNC_IMPLEX broken with return by value structures
