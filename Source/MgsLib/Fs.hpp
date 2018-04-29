#pragma once

#include "MgsFunction.hpp"

void Fs_Cpp_ForceLink();
void DoFsTests();

FILE* CC File_LoadDirFile_51EE8F(const char* fileName, signed int openMode);
size_t CC File_NormalRead_51F0F5(FILE* File, void* dstBuf, DWORD nNumberOfBytesToRead);
__int32 CC File_GetPos_51F09E(FILE* File, __int32 Offset, int Origin);
int CC File_Close_51F183(FILE* File);
signed int CC FS_LoadRequest(const char* fileName, void** buffer, signed int type);
void CC j_FS_CloseFile_40907E();
int Res_loader_load_file_to_mem_408FAE();
signed int CC File_GetStreamState_45837C();
void CC FS_StreamEnd_521210();
void CC FS_StreamActorStop_4583BB();
char* FS_StreamDebugStr_52078F();
