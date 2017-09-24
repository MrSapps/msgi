#pragma once

#include "MgsFunction.hpp"
#include <windows.h>

LARGE_INTEGER CC TimerInitBaseLineAndGetCurrentTime_5201A6();
void CC Timer_30_1();
void CC Timer_30();
void CC Timer_24();
void CC Timer_15();

struct PerformanceFreqStruct
{
    LONGLONG mFreq60;
    LONGLONG mFreq30;
    LONGLONG mFreq24;
    LONGLONG mFreq23;
    LONGLONG mFreq22;
    LONGLONG mFreq21;
    LONGLONG mFreq20;
    LONGLONG mFreq25;
    LONGLONG mFreq15;
};
MGS_VAR_EXTERN(PerformanceFreqStruct, gPerformanceFreq_995648);
