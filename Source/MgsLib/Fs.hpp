#pragma once

#include "MgsFunction.hpp"

void Fs_Cpp_ForceLink();

EXTERN_MGS_FUNC_NOT_IMPL(0x51EE8F, FILE* CC(const char* fileName, signed int openMode), File_LoadDirFile_51EE8F);
EXTERN_MGS_FUNC_NOT_IMPL(0x51F09E, __int32 CC(FILE* File, __int32 Offset, int Origin), File_GetPos_51F09E);
EXTERN_MGS_FUNC_NOT_IMPL(0x51F183, int CC(FILE *File), File_Close_51F183);
EXTERN_MGS_FUNC_NOT_IMPL(0x51F0F5, int CC(FILE* File, void* dstBuf, DWORD nNumberOfBytesToRead), File_NormalRead_51F0F5);


signed int CC FS_LoadRequest(const char* fileName, void** buffer, signed int type);
