#pragma once

#include <windows.h>
#include <stdio.h>
#include "MgsFunction.hpp"

// stdlib
EXTERN_MSG_FUNC_NOT_IMPL(0x0053CB40, FILE* __cdecl(const char*, const char*), mgs_fopen);
EXTERN_MSG_FUNC_NOT_IMPL(0x0053C970, int __cdecl(const char*, FILE*), mgs_fputs);
EXTERN_MSG_FUNC_NOT_IMPL(0x0053C6C0, int __cdecl(FILE*), mgs_fflush);
EXTERN_MSG_FUNC_NOT_IMPL(0x0053C4A0, int __cdecl(FILE *File), mgs_fclose);

// Memory allocation
void *__cdecl mgs_malloc(size_t Size);
void __cdecl mgs_free(void *Memory);
void *__cdecl mgs_realloc(void *Memory, size_t NewSize);
void *__cdecl mgs_calloc(size_t NumOfElements, size_t SizeOfElements);


// Special case varadics..
int msg_internal_fprintf(FILE *File, const char *Format, ...);
using TMgs_fprintf = decltype(&msg_internal_fprintf);
extern TMgs_fprintf mgs_fprintf;

int mgs_internal_open(const char *lpFileName, int a2, ...);
using TMgs_open = decltype(&mgs_internal_open);
extern TMgs_open mgs_open;

EXTERN_MSG_FUNC_NOT_IMPL(0x0053D680, int __cdecl(int), mgs_close);
EXTERN_MSG_FUNC_NOT_IMPL(0x0053E180, int __cdecl(int, LONG, DWORD), mgs_lseek);
EXTERN_MSG_FUNC_NOT_IMPL(0x0053D1A0, int __cdecl(int, void*, DWORD), mgs_read);

// Internal file lib
EXTERN_MSG_FUNC_NOT_IMPL(0x0051EE8F, FILE *__cdecl(const char*, signed int), File_LoadDirFileQ);
EXTERN_MSG_FUNC_NOT_IMPL(0x0051F0F5, size_t __cdecl(FILE*, void*, DWORD), File_NormalRead);
EXTERN_MSG_FUNC_NOT_IMPL(0x0051F183, int __cdecl(FILE*), File_CloseQ);
