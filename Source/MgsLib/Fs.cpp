#include "stdafx.h"
#include "Fs.hpp"
#include "System.hpp"
#include "Actor_Loader.hpp"
#include "Actor_GameD.hpp"

void Fs_Cpp_ForceLink() { }

#define FS_IMPL true

MGS_FUNC_NOT_IMPL(0x51EE8F, FILE* CC(const char* fileName, signed int openMode), File_LoadDirFile_51EE8F);
MGS_FUNC_NOT_IMPL(0x51F09E, __int32 CC(FILE* File, __int32 Offset, int Origin), File_GetPos_51F09E);
MGS_FUNC_NOT_IMPL(0x51F183, int CC(FILE *File), File_Close_51F183);
MGS_FUNC_NOT_IMPL(0x51F0F5, int CC(FILE* File, void* dstBuf, DWORD nNumberOfBytesToRead), File_NormalRead_51F0F5);

MGS_VAR(1, 0x6BFBB0, void*, gFileBuffer_dword_6BFBB0, 0);
MGS_VAR(1, 0x6BFBA8, FILE*, gFileHandle_dword_6BFBA8, 0);
MGS_VAR(1, 0x6BFBAC, DWORD, gFileSizeToRead_dword_6BFBAC, 0);

// Given data.cnf then returns stage/init/data.cnf for example
void CC ToFullStagePath_408EA0(const char* pFileName, char* pOutFullName)
{
    strcpy(pOutFullName, gStage_Name_byte_6504C8);
    strcat(pOutFullName, "/");
    strcat(pOutFullName, pFileName);
}
MGS_FUNC_IMPLEX(0x00408EA0, ToFullStagePath_408EA0, true) // TODO: Impl

static const char* FileLoadModeToString(signed int type)
{
    switch (type)
    {
    case Actor_Loader::eCache:    return "Cache";
    case Actor_Loader::eNoCache:  return "NoCache";
    case Actor_Loader::eResident: return "Resident";
    case Actor_Loader::eSound:    return "Sound";
    default:                      return "Unknown";
    }
}

static FILE* const kInvalidFileHandle = reinterpret_cast<FILE*>(-2);

void CC FS_CloseFile_40907E()
{
    //Ret0_51EE8C();
    gFileHandle_dword_6BFBA8 = kInvalidFileHandle;
}
MGS_FUNC_IMPLEX(0x0040907E, FS_CloseFile_40907E, FS_IMPL)

void CC j_FS_CloseFile_40907E()
{
    FS_CloseFile_40907E();
}
MGS_FUNC_IMPLEX(0x00408086, j_FS_CloseFile_40907E, FS_IMPL)

int CC FS_FileReadToMem_409040()
{
    if (gFileHandle_dword_6BFBA8 != kInvalidFileHandle)
    {
        File_NormalRead_51F0F5(gFileHandle_dword_6BFBA8, gFileBuffer_dword_6BFBB0, gFileSizeToRead_dword_6BFBAC);
        File_Close_51F183(gFileHandle_dword_6BFBA8);
        gFileSizeToRead_dword_6BFBAC = 0;
        gFileHandle_dword_6BFBA8 = kInvalidFileHandle;
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x00409040, FS_FileReadToMem_409040, FS_IMPL)

int Res_loader_load_file_to_mem_408FAE()
{
    File_NormalRead_51F0F5(gFileHandle_dword_6BFBA8, gFileBuffer_dword_6BFBB0, gFileSizeToRead_dword_6BFBAC);
    File_Close_51F183(gFileHandle_dword_6BFBA8);
    gFileSizeToRead_dword_6BFBAC = 0;
    gFileHandle_dword_6BFBA8 = kInvalidFileHandle;
    return 0;
}
MGS_FUNC_IMPLEX(0x00408FAE, Res_loader_load_file_to_mem_408FAE, FS_IMPL)

static void SetLoadedFileInfo(FILE* hFile, void** ppBuffer, int fileSize)
{
    gFileBuffer_dword_6BFBB0 = *ppBuffer;
    gFileHandle_dword_6BFBA8 = hFile;
    gFileSizeToRead_dword_6BFBAC = fileSize;
}

signed int CC FS_LoadRequest(const char* fileName, void** ppBuffer, signed int type)
{
    printf("FS_LoadRequest: %s %p %s\n", fileName, ppBuffer, FileLoadModeToString(type));

    const char* fileNameWithoutAsterix = fileName;
    if (*fileName == '*')
    {
        fileNameWithoutAsterix = fileName + 1;
    }
    char fileToLoad[64] = {};
    ToFullStagePath_408EA0(fileNameWithoutAsterix, fileToLoad);

    FILE* hFile = File_LoadDirFile_51EE8F(fileToLoad, 0);
    const DWORD fileSize = File_GetPos_51F09E(hFile, 0, 2);
    File_GetPos_51F09E(hFile, 0, 0);

    switch (type)
    {
    case Actor_Loader::eResident:
        *ppBuffer = ResidentTopAllocate_40B379(fileSize + 1);
        SetLoadedFileInfo(hFile, ppBuffer, fileSize);
        break;

    case Actor_Loader::eSound:
        SetLoadedFileInfo(hFile, ppBuffer, fileSize);
        break;

    case Actor_Loader::eCache:      // Fall through
    case Actor_Loader::eNoCache:
        *ppBuffer = System_mem_alloc_40AF91(2, fileSize + 1);
        if (*ppBuffer)
        {
            SetLoadedFileInfo(hFile, ppBuffer, fileSize);
            return fileSize;
        }
        else
        {
            if (hFile)
            {
                File_Close_51F183(hFile);
            }
            return -1;
        }
        break;

    default:
        SetLoadedFileInfo(hFile, ppBuffer, fileSize);
        break;
    }
    
    return fileSize;
}
MGS_FUNC_IMPLEX(0x00408EEF, FS_LoadRequest, FS_IMPL)
