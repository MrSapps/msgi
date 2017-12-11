#pragma once

#include "MgsFunction.hpp"

void Fs_Cpp_ForceLink();
void DoFsTests();

class AbstractedFileHandle
{
public:
    AbstractedFileHandle() = default;
    explicit AbstractedFileHandle(FILE* file) : mFile(file) { } 
    FILE* mFile = nullptr;
    // TODO: Zip/zlib handle
};

AbstractedFileHandle* CC File_LoadDirFile_51EE8F(const char* fileName, signed int openMode);
size_t CC File_NormalRead_51F0F5(AbstractedFileHandle* File, void* dstBuf, DWORD nNumberOfBytesToRead);
__int32 CC File_GetPos_51F09E(AbstractedFileHandle* File, __int32 Offset, int Origin);
int CC File_Close_51F183(AbstractedFileHandle *File);
signed int CC FS_LoadRequest(const char* fileName, void** buffer, signed int type);
void CC j_FS_CloseFile_40907E();
int Res_loader_load_file_to_mem_408FAE();
signed int CC File_GetStreamState_45837C();
