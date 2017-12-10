#include "stdafx.h"
#include "File.hpp"
#include "MgsFunction.hpp"
#include <type_traits>
#include <functional>

// 0x00520157
void DebugLog(const char *Format, ...)
{
    char Dest[512] = {};
    va_list va;
    va_start(va, Format);
    vsprintf(Dest, Format, va);
    va_end(va);
    //OutputDebugStringA(Dest);
    printf("%s", Dest);
}
