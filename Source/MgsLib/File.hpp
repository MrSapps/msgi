#pragma once

#include <windows.h>
#include <stdio.h>
#include "MgsFunction.hpp"

EXTERN_MSG_FUNC_NOT_IMPL(0x0051EE8F, FILE *__cdecl(const char*, signed int), File_LoadDirFileQ);
EXTERN_MSG_FUNC_NOT_IMPL(0x0051F0F5, size_t __cdecl(FILE*, void*, DWORD), File_NormalRead);
EXTERN_MSG_FUNC_NOT_IMPL(0x0051F183, int __cdecl(FILE*), File_CloseQ);
