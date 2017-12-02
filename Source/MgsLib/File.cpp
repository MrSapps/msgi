#include "stdafx.h"
#include "File.hpp"
#include "MgsFunction.hpp"
#include <type_traits>
#include <functional>

// Memory allocation
MGS_STDLIB(malloc, 0x00539990);
MGS_STDLIB(free, 0x0053A400);
MGS_STDLIB(realloc, 0x00539E20);
MGS_STDLIB(calloc, 0x00539DA0);

// File I/O
MGS_STDLIB(fopen, 0x0053CB40);
MGS_STDLIB(fputs, 0x0053C970);
MGS_STDLIB(fflush, 0x0053C6C0);
MGS_STDLIB(fclose, 0x0053C4A0);

using TOpen = decltype(&open);
TOpen gMgsOpen = (TOpen)0x0053DBE0;

int mgs_open(const char* filename, int openFlag)
{
    if (IsMgsi())
    {
        return gMgsOpen(filename, openFlag);
    }
    return open(filename, openFlag);
}

int mgs_open(const char* filename, int openFlag, DWORD mode)
{
    if (IsMgsi())
    {
        return gMgsOpen(filename, openFlag, mode);
    }
    return open(filename, openFlag, mode);
}

int mgs_fprintf(FILE* file, const char* format, ...)
{
    if (IsMgsi())
    {
        char Dest[4096] = {};
        va_list va;
        va_start(va, format);
        int len = vsprintf(Dest, format, va);
        va_end(va);

        return mgs_fwrite(Dest, len, 1, file);
    }
    else
    {
        va_list va;
        va_start(va, format);
        int ret = vfprintf(file, format, va);
        va_end(va);
        return ret;
    }
}

MGS_STDLIB(fwrite, 0x0053F550);
MGS_STDLIB(close, 0x0053D680);
MGS_STDLIB(lseek, 0x0053E180);
MGS_STDLIB(read, 0x0053D1A0);
MGS_STDLIB(fread, 0x0053CEE0);
MGS_STDLIB(fseek, 0x0053CD00);

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

int mgs_printf(const char* fmt, ...)
{
    va_list myargs;
    va_start(myargs, fmt);
    int ret = vprintf(fmt, myargs);
    va_end(myargs);
    return ret;
}
