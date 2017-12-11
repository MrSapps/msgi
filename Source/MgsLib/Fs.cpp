#include "stdafx.h"
#include "Fs.hpp"
#include "System.hpp"
#include "Actor_Loader.hpp"
#include "Actor_GameD.hpp"
#include "File.hpp"
#include <gmock/gmock.h>


#define FS_IMPL true

struct MgzMapping
{
    const char* field_0_path;
    const char* field_4_mgz;
    FILE* field_8_file_handle;
    DWORD field_C_padding;
};
MGS_ASSERT_SIZEOF(MgzMapping, 0x10);

MgzMapping gMgzTable_6898E8[4] =
{
    { "stage/",     "stage.mgz",    nullptr, 0 },
    { "stagevr/",   "stagevr.mgz",  nullptr, 0 },
    { "efx/",       "efx.mgz",      nullptr, 0 },
    { "tga/",       "tga.mgz",      nullptr, 0 }
};


char* CC ReplaceBackWithFowardSlashes_51EC38(char* pInput, char* pOutput)
{
    // TODO
    return nullptr;
}
MGS_FUNC_IMPLEX(0x0051EC38, ReplaceBackWithFowardSlashes_51EC38, false)

void CC CloseCachedMgzFiles_51EE23()
{
    // TODO: Close open handles to items in the zip itself

    for (MgzMapping& mapping : gMgzTable_6898E8)
    {
        if (mapping.field_8_file_handle)
        {
            fclose(mapping.field_8_file_handle);
            mapping.field_8_file_handle = nullptr;
        }
    }
}
MGS_FUNC_IMPLEX(0x0051EE23, CloseCachedMgzFiles_51EE23, false)

FILE* CC OpenMGZ_051ED67(const char* pFileName)
{
    static bool sSetCleanup_776B60 = false;
    if (!sSetCleanup_776B60)
    {
        sSetCleanup_776B60 = true;
        // TODO: Use a direct call at the end of winmain instead
        atexit(CloseCachedMgzFiles_51EE23);
    }

    MgzMapping* pMapping = nullptr;
    for (MgzMapping& mapping : gMgzTable_6898E8)
    {
        if (strncmp(mapping.field_0_path, pFileName, strlen(mapping.field_0_path)) == 0)
        {
            break;
        }
    }

    if (!pMapping)
    {
        return nullptr;
    }

    if (!pMapping->field_8_file_handle)
    {
        char buffer[256] = {};
        strcpy(buffer, ".");
        strcat(buffer, "/");
        strcat(buffer, pMapping->field_4_mgz);

        pMapping->field_8_file_handle = fopen(buffer, "rb");
    }

    return pMapping->field_8_file_handle;
}
MGS_FUNC_IMPLEX(0x00051ED67, OpenMGZ_051ED67, false)

AbstractedFileHandle* CC OpenArchiveFile_51EFB2(char* pFileName)
{
    char normalizedFileName[256] = {};
    ReplaceBackWithFowardSlashes_51EC38(pFileName, normalizedFileName);
    FILE* mgzHandle = OpenMGZ_051ED67(normalizedFileName);
    if (!mgzHandle)
    {
        return nullptr;
    }



    // TODO: Find free entry in gmmf_dword_776960 - only allows for 32 handles

    return nullptr;
}
MGS_FUNC_IMPLEX(0x0051EFB2, OpenArchiveFile_51EFB2, FS_IMPL)


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

AbstractedFileHandle* CC File_LoadDirFile_51EE8F(const char* fileName, signed int openMode)
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
    AbstractedFileHandle* hFile = OpenArchiveFile_51EFB2(remappedFileName);


    if (!hFile)
    {
        // Failed, try opening directly from on disk/extracted MGZ archive
        char fileToOpen[256] = {};
        strcpy(fileToOpen, ".");
        strcat(fileToOpen, "/");
        strcat(fileToOpen, remappedFileName);
        const char* strOpenMode = gOpenModes_689998[openMode % 4];
        FILE* hRealFile = fopen(fileToOpen, strOpenMode);
        if (!hRealFile)
        {
            // For some reason try again one more time?
            strcpy(fileToOpen, ".");
            strcat(fileToOpen, "/");
            strcat(fileToOpen, remappedFileName);
            hRealFile = fopen(fileToOpen, strOpenMode);
        }

        if (hRealFile)
        {
            hFile = new AbstractedFileHandle(hRealFile);
        }
    }
    return hFile;
}
MGS_FUNC_IMPLEX(0x0051EE8F, File_LoadDirFile_51EE8F, FS_IMPL)

size_t CC File_NormalRead_51F0F5(AbstractedFileHandle* File, void* dstBuf, DWORD nNumberOfBytesToRead)
{
    if (File && File->mFile != (FILE *)-1 && File->mFile)
    {
        return fread(dstBuf, 1, nNumberOfBytesToRead, File->mFile);
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x0051F0F5, File_NormalRead_51F0F5, FS_IMPL) // TODO

__int32 CC File_GetPos_51F09E(AbstractedFileHandle* File, __int32 Offset, int Origin)
{
    if (File && File->mFile != (FILE *)-1 && File->mFile)
    {
        fseek(File->mFile, Offset, Origin);
        return ftell(File->mFile);
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x0051F09E, File_GetPos_51F09E, FS_IMPL) // TODO

int CC File_Close_51F183(AbstractedFileHandle *File)
{
    if (!File || File->mFile == (FILE *)-1 || !File->mFile)
    {
        return 0;
    }

    const int ret = fclose(File->mFile);
    delete File;
    return ret;
}
MGS_FUNC_IMPLEX(0x0051F183, File_Close_51F183, FS_IMPL) // TODO

MGS_VAR(1, 0x6BFBB0, void*, gFileBuffer_dword_6BFBB0, 0);
MGS_VAR(1, 0x6BFBA8, AbstractedFileHandle*, gFileHandle_dword_6BFBA8, 0);
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

static AbstractedFileHandle* const kInvalidFileHandle = reinterpret_cast<AbstractedFileHandle*>(-2);

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
    AbstractedFileHandle* hFile = File_LoadDirFile_51EE8F(fileName, 0);
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

static void SetLoadedFileInfo(AbstractedFileHandle* hFile, void** ppBuffer, int fileSize)
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

    AbstractedFileHandle* hFile = File_LoadDirFile_51EE8F(fileToLoad, 0);
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

MGS_VAR(1, 0x724AE0, WORD, gStream_state_word_724AE0, 0);
MGS_VAR(1, 0x9B0A10, DWORD, fs_busy_dword_9B0A10, 0);

int CC FS_StreamIsBusy_521122()
{
    return fs_busy_dword_9B0A10;
}
MGS_FUNC_IMPLEX(0x00521122, FS_StreamIsBusy_521122, FS_IMPL)

signed int CC File_GetStreamState_45837C()
{
    if (gStream_state_word_724AE0 || !FS_StreamIsBusy_521122())
    {
        return gStream_state_word_724AE0 - 1;
    }
    return 2;
}
MGS_FUNC_IMPLEX(0x0045837C, File_GetStreamState_45837C, FS_IMPL)

void Fs_Cpp_ForceLink()
{

}

void DoFsTests()
{
    char str[256] = {};
    ASSERT_STREQ("blah", ReplaceBackWithFowardSlashes_51EC38("blah", str));
    ASSERT_STREQ("blah/", ReplaceBackWithFowardSlashes_51EC38("blah/", str));
    ASSERT_STREQ("blah/", ReplaceBackWithFowardSlashes_51EC38("blah\\", str));
    ASSERT_STREQ("blah/asdf.exe", ReplaceBackWithFowardSlashes_51EC38("blah\\asdf.exe", str));
    ASSERT_STREQ("blah/asdf.exe", ReplaceBackWithFowardSlashes_51EC38("blah/asdf.exe", str));
    ASSERT_STREQ("/", ReplaceBackWithFowardSlashes_51EC38("\\\\\\", str));
}
