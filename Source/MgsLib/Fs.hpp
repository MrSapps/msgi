#pragma once

#include "MgsFunction.hpp"

void Fs_Cpp_ForceLink();

FILE* CC File_LoadDirFile_51EE8F(const char* fileName, signed int openMode);
size_t CC File_NormalRead_51F0F5(FILE* File, void* dstBuf, DWORD nNumberOfBytesToRead);

EXTERN_MGS_FUNC_NOT_IMPL(0x51F09E, __int32 CC(FILE* File, __int32 Offset, int Origin), File_GetPos_51F09E);
EXTERN_MGS_FUNC_NOT_IMPL(0x51F183, int CC(FILE *File), File_Close_51F183);


signed int CC FS_LoadRequest(const char* fileName, void** buffer, signed int type);
void CC j_FS_CloseFile_40907E();
int Res_loader_load_file_to_mem_408FAE();
