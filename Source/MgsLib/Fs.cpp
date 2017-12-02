#include "stdafx.h"
#include "Fs.hpp"
#include "System.hpp"
#include "Actor_Loader.hpp"
#include "Actor_GameD.hpp"
#include "File.hpp"


#define FS_IMPL true


MGS_FUNC_NOT_IMPL(0x51EFB2, FILE* CC(char *pFileName), OpenArchiveFile_51EFB2);

struct FileToFileMap
{
    const char* mFrom;
    const char* mTo;
};

static const FileToFileMap gDarResNames_689928[14] =
{
    { "stage/init/res_mdl1.dar", "stage/init_ve/res_mdl1.dar" },
    { "stage/init/res_tex1.dar", "stage/init_ve/res_tex1.dar" },
    { "stage/init/font.res", nullptr },
    { "stage/init/rubi.res", nullptr },
    { "stage/init/num.res", nullptr },
    { "stage/init/call.res", nullptr },
    { "stage/init_ve/font.res", nullptr },
    { "stage/init_ve/rubi.res", nullptr },
    { "stage/init_ve/num.res", nullptr },
    { "stage/init_ve/call.res", nullptr },
    { "stage/init_tux/font.res", nullptr },
    { "stage/init_tux/rubi.res", nullptr },
    { "stage/init_tux/num.res", nullptr },
    { "stage/init_tux/call.res", nullptr }
};

const char* gOpenModes_689998[] =
{
    "rb",
    "wb",
    "rb+"
};

FILE* CC File_LoadDirFile_51EE8F(const char* fileName, signed int openMode)
{
    LOG_INFO(fileName);

    char remappedFileName[256] = {};
    strcpy(remappedFileName, fileName);
    for (const auto& fileRec : gDarResNames_689928)
    {
        // If the file name matches
        if (strcmp(fileName, fileRec.mFrom) == 0)
        {
            // Then prevent it being loaded if there is no "to" mapping
            // else replace to the file to load with the "to" mapping
            if (fileRec.mTo == nullptr)
            {
                LOG_INFO(fileName << " was blocked from loading");
                return 0;
            }
            LOG_INFO(fileName << " was redirected to " << fileRec.mTo);
            strcpy(remappedFileName, fileRec.mTo);
            break;
        }
    }

    // Open in the MGZ archive
    FILE* hFile = OpenArchiveFile_51EFB2(remappedFileName);
    if (!hFile)
    {
        // Failed, try opening directly from on disk/extracted MGZ archive
        char fileToOpen[256] = {};
        strcpy(fileToOpen, ".");
        strcat(fileToOpen, "/");
        strcat(fileToOpen, remappedFileName);
        const char* strOpenMode = gOpenModes_689998[openMode % 4];
        hFile = mgs_fopen(fileToOpen, strOpenMode);
        if (!hFile)
        {
            // For some reason try again one more time?
            strcpy(fileToOpen, ".");
            strcat(fileToOpen, "/");
            strcat(fileToOpen, remappedFileName);
            hFile = mgs_fopen(fileToOpen, strOpenMode);
        }
    }
    return hFile;
}
MGS_FUNC_IMPLEX(0x0051EE8F, File_LoadDirFile_51EE8F, FS_IMPL)

size_t CC File_NormalRead_51F0F5(FILE* File, void* dstBuf, DWORD nNumberOfBytesToRead)
{
    return mgs_fread(dstBuf, 1, nNumberOfBytesToRead, File);
}
MGS_FUNC_IMPLEX(0x0051F0F5, File_NormalRead_51F0F5, false) // TODO

__int32 CC File_GetPos_51F09E(FILE* File, __int32 Offset, int Origin)
{
    if (File != (FILE *)-1 && File)
    {
        mgs_fseek(File, Offset, Origin);
        return ftell(File);
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x0051F09E, File_GetPos_51F09E, false) // TODO

int CC File_Close_51F183(FILE *File)
{
    if (File == (FILE *)-1 || !File)
    {
        return 0;
    }

    return mgs_fclose(File);
}
MGS_FUNC_IMPLEX(0x0051F183, File_Close_51F183, false) // TODO

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
MGS_FUNC_IMPLEX(0x00408EA0, ToFullStagePath_408EA0, FS_IMPL)

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

static const char* sDirFileArray_6505C8[] =
{
    "stage.dir",
    "radio.dat",
    "face.dat",
    "zmovie.str",
    "vox.dat",
    "demo.dat",
    "brf.dat"
};

void CC FS_LoadDirFileByIndex_408FE7(int dirFileIndex, int offset, int sizeToRead, void *pFileBuffer)
{
    char fileName[64] = {};
    sprintf(fileName, "%s", sDirFileArray_6505C8[dirFileIndex]);
    FILE* hFile = File_LoadDirFile_51EE8F(fileName, 0);
    File_GetPos_51F09E(hFile, offset << 11, 0);
    gFileSizeToRead_dword_6BFBAC = sizeToRead;
    gFileHandle_dword_6BFBA8 = hFile;
    gFileBuffer_dword_6BFBB0 = pFileBuffer;
}
MGS_FUNC_IMPLEX(0x00408FE7, FS_LoadDirFileByIndex_408FE7, FS_IMPL)

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

void Fs_Cpp_ForceLink()
{

}