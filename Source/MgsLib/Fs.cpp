#include "stdafx.h"
#include "Fs.hpp"
#include "System.hpp"
#include "Actor_Loader.hpp"
#include "Actor_GameD.hpp"
#include "File.hpp"
#include <gmock/gmock.h>
#include <fcntl.h>


#define FS_IMPL true

#pragma pack(push)
#pragma pack(1)
struct Zip_Mgs_File_Record
{
    DWORD field_0_uncompressed_size;
    DWORD field_4_compressed_size;
    DWORD field_8_crc;
    DWORD field_C_offset_to_local_header;
    WORD field_10_record_size;
    WORD field_12_file_name_length;
    char field_14_compression_method;
    char field_15_file_name_buffer[1]; // Var length
};
MGS_ASSERT_SIZEOF(Zip_Mgs_File_Record, 0x16);
#pragma pack(pop)

struct Zip_Zlib_Wrapper;

struct ZipContext
{
    int field_0_hMgzFile;
    int field_4_last_err;
    DWORD field_8_flags_or_counter;
    Zip_Zlib_Wrapper* field_C_84_byte_ptr;
    void* field_10_ptr_shared_buffer;
    Zip_Mgs_File_Record* field_14_local_file_headers_ptr;
    void* field_18_pTo_field_14_local_file_headers; // Non owned pointer
    Zip_Zlib_Wrapper* field_1C_zstream;
    DWORD field_20;
    DWORD field_24;
    DWORD field_28;
    DWORD field_2C;
    DWORD field_30;
    DWORD field_34;
};
MGS_ASSERT_SIZEOF(ZipContext, 0x38);

#pragma pack(push)
#pragma pack(1)
struct Zip_Central_Directory_Record
{
    DWORD field_0_magic;
    WORD field_4_mCreatedByVersion;
    WORD field_6_ExtractVersion;
    WORD field_8_flags;
    WORD field_a_compression_method;
    DWORD field_C_dos_time;
    DWORD field_10_crc;
    DWORD field_14_compressed_size;
    DWORD field_18_uncompressed_size;
    WORD field_1c_filename_length;
    WORD field_1E_extra_field_length;
    WORD field_20_file_comment_length;
    DWORD field_22_disk_number;
    DWORD field_26_file_attributes;
    DWORD field_2A_offset_to_local_header;
};
MGS_ASSERT_SIZEOF(Zip_Central_Directory_Record, 0x2E);
#pragma pack(pop)

struct Zip_Open_File_Handle
{
    Zip_Open_File_Handle* field_0_pZipStream;
    ZipContext* field_4_zip_file_handle;
    DWORD field_8_file_size;
    DWORD field_C;
};
MGS_ASSERT_SIZEOF(Zip_Open_File_Handle, 0x10);

#pragma pack(push)
#pragma pack(1)
struct Zip_End_Of_Central_Directory
{
    DWORD field_0_magic;
    WORD field_4_mThisDiskNumber;
    WORD field_6_mStartCentralDirectoryDiskNumber;
    WORD field_8_mNumEntriesInCentaralDirectoryOnThisDisk;
    WORD field_A_mNumEntriesInCentaralDirectory;
    DWORD field_C_mCentralDirectorySize;
    DWORD field_10_mCentralDirectoryStartOffset;
    WORD field_14_mCommentSize;
};
MGS_ASSERT_SIZEOF(Zip_End_Of_Central_Directory, 0x16);
#pragma pack(pop)

struct MgzMapping
{
    const char* field_0_path;
    const char* field_4_mgz;
    ZipContext* field_8_file_handle;
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
    char* pRet = pOutput;
    bool wroteSlash = false;
    while (*pInput)
    {
        if (*pInput == '\\' || *pInput == '/')
        {
            if (!wroteSlash)
            {
                *pOutput = '/';
                pOutput++;
            }
            wroteSlash = true;
        }
        else
        {
            *pOutput = *pInput;
            pOutput++;
            wroteSlash = false;
        }
        pInput++;
    }
    *pOutput = 0;

    return pRet;
}
MGS_FUNC_IMPLEX(0x0051EC38, ReplaceBackWithFowardSlashes_51EC38, FS_IMPL)

void CC CloseCachedMgzFiles_51EE23()
{
    // TODO: Close open handles to items in the zip itself

    for (MgzMapping& mapping : gMgzTable_6898E8)
    {
        if (mapping.field_8_file_handle)
        {
            // TODO
            //fclose(mapping.field_8_file_handle);
            mapping.field_8_file_handle = nullptr;
        }
    }
}
MGS_FUNC_IMPLEX(0x0051EE23, CloseCachedMgzFiles_51EE23, false)

int CC Zip_Try_Open_With_Other_Extensions_642870(const char* pFileName, int openMode)
{
    char fileNameBuffer[512] = {};
    const size_t fileNameLen = strlen(pFileName) + 1;
    if ((fileNameLen - 1 + 4) < 512)
    {
        memcpy(fileNameBuffer, pFileName, fileNameLen);
        char* pStrEnd = &fileNameBuffer[fileNameLen -1];
        strcpy(pStrEnd, ".zip");
        int hFile = _open(fileNameBuffer, openMode);
        if (hFile == -1)
        {
            strcpy(pStrEnd, ".ZIP");
            hFile = _open(fileNameBuffer, openMode);
        }
    }
    return -1;
}
MGS_FUNC_IMPLEX(0x00642870, Zip_Try_Open_With_Other_Extensions_642870, FS_IMPL)

void CC Zip_free_642740(ZipContext* pZip)
{
    if (pZip->field_0_hMgzFile >= 0)
    {
        _close(pZip->field_0_hMgzFile);
    }

    if (pZip->field_14_local_file_headers_ptr)
    {
        free(pZip->field_14_local_file_headers_ptr);
    }

    if (pZip->field_C_84_byte_ptr)
    {
        free(pZip->field_C_84_byte_ptr);
    }

    if (pZip->field_10_ptr_shared_buffer)
    {
        free(pZip->field_10_ptr_shared_buffer);
    }

    free(pZip);
}
MGS_FUNC_IMPLEX(0x00642740, Zip_free_642740, FS_IMPL)

int Zip_get_file_size_642840(int hMgzFile)
{
    return _filelength(hMgzFile);
}
MGS_FUNC_IMPLEX(0x00642840, Zip_get_file_size_642840, FS_IMPL)

int CC Zip_locate_end_of_central_directory_record_6423D0(int hMgzFile, unsigned int fileSize, Zip_End_Of_Central_Directory* pEndOfCentralDir)
{
    if (!pEndOfCentralDir)
    {
        return sizeof(Zip_End_Of_Central_Directory);
    }

    if (fileSize < sizeof(Zip_End_Of_Central_Directory))
    {
        return -4121;
    }

    char buffer[512];
    LONG readPos = fileSize - sizeof(Zip_End_Of_Central_Directory);
    Zip_End_Of_Central_Directory* pDir = nullptr;
    for (;;)
    {
        // On first pass read just the size of a record from the end of the file, else reading 512 blocks
        if (_lseek(hMgzFile, readPos, SEEK_SET) < 0)
        {
            return -4119;
        }

        if (_read(hMgzFile, buffer, sizeof(Zip_End_Of_Central_Directory)) < sizeof(Zip_End_Of_Central_Directory))
        {
            return -4120;
        }

        // Original game bug - checked if buffer was null, can never happen
        
        pDir = reinterpret_cast<Zip_End_Of_Central_Directory*>(buffer);
        if (pDir->field_0_magic == 0x06054b50)
        {
            break;
        }

        // Simple case of it being right at the end of the file has failed, now we need to search backwards in blocks

        // TODO: Implement me
        /*
        for (;;)
        {
            if (_lseek(hMgzFile, readPos, SEEK_SET) < 0)
            {
                return -4119;
            }

            if (_read(hMgzFile, buffer, sizeof(buffer)) < sizeof(buffer))
            {
                return -4120;
            }

            readPos -= 512;
            //remainingSize += 512;
            if (remainingSize <= 65536)
            {

            }
        }*/
        return -4122;
    }

    memcpy(pEndOfCentralDir, pDir, sizeof(Zip_End_Of_Central_Directory));

    return 0;
}
MGS_FUNC_IMPLEX(0x006423D0, Zip_locate_end_of_central_directory_record_6423D0, FS_IMPL)

WORD CC Zip_ByteSwap_Word_6423C0(WORD* pData)
{
    // VC6 fail
    return *pData;
}
MGS_FUNC_IMPLEX(0x006423C0, Zip_ByteSwap_Word_6423C0, FS_IMPL)

DWORD CC Zip_ByteSwap_DWord_642390(DWORD* pData)
{
    // VC6 fail
    return *pData;
}
MGS_FUNC_IMPLEX(0x00642390, Zip_ByteSwap_DWord_642390, FS_IMPL)

int CC Zip_load_central_directory_642540(int hMgzFile, Zip_End_Of_Central_Directory* pEndOfCentralDir, Zip_Mgs_File_Record** pField_14)
{
    Zip_Mgs_File_Record* pCentralDirSizeBuffer = (Zip_Mgs_File_Record*)calloc(1, pEndOfCentralDir->field_C_mCentralDirectorySize); // Original game bug- didn't zero out
    if (!pCentralDirSizeBuffer)
    {
        return -4123;
    }

    Zip_Mgs_File_Record* pOutputBufferIter = pCentralDirSizeBuffer;
    DWORD recordOffset = 0;
    for (DWORD i = 0; i < pEndOfCentralDir->field_A_mNumEntriesInCentaralDirectory; i++)
    {
        // Seek to current record
        if (_lseek(hMgzFile, pEndOfCentralDir->field_10_mCentralDirectoryStartOffset + recordOffset, SEEK_SET) < 0)
        {
            free(pCentralDirSizeBuffer); // Original game bug- didn't free
            return -4119;
        }

        // Read it
        Zip_Central_Directory_Record dirRecord = {};
        if (_read(hMgzFile, &dirRecord, sizeof(Zip_Central_Directory_Record)) < sizeof(Zip_Central_Directory_Record))
        {
            free(pCentralDirSizeBuffer); // Original game bug- didn't free
            return -4120;
        }

        pOutputBufferIter->field_8_crc = Zip_ByteSwap_DWord_642390(&dirRecord.field_10_crc);
        pOutputBufferIter->field_4_compressed_size = Zip_ByteSwap_DWord_642390(&dirRecord.field_14_compressed_size);
        pOutputBufferIter->field_0_uncompressed_size = Zip_ByteSwap_DWord_642390(&dirRecord.field_18_uncompressed_size);
        pOutputBufferIter->field_C_offset_to_local_header = Zip_ByteSwap_DWord_642390(&dirRecord.field_2A_offset_to_local_header);
        pOutputBufferIter->field_12_file_name_length = dirRecord.field_1c_filename_length;

        if (dirRecord.field_a_compression_method > 255)
        {
            pOutputBufferIter->field_14_compression_method = -1;
        }
        else
        {
            pOutputBufferIter->field_14_compression_method = static_cast<char>(dirRecord.field_a_compression_method);
        }

        _read(hMgzFile, &pOutputBufferIter->field_15_file_name_buffer[0], pOutputBufferIter->field_12_file_name_length);
     
        // Null terminate - although should already be a null at the end of the buffer
        pOutputBufferIter->field_15_file_name_buffer[pOutputBufferIter->field_12_file_name_length] = 0;

        recordOffset += 
            dirRecord.field_1c_filename_length + 
            dirRecord.field_20_file_comment_length + 
            dirRecord.field_1E_extra_field_length + 
            sizeof(Zip_Central_Directory_Record);

        if (recordOffset > pEndOfCentralDir->field_C_mCentralDirectorySize)
        {
            break;
        }

        pOutputBufferIter->field_10_record_size = static_cast<WORD>(RoundUpPowerOf2(sizeof(Zip_Mgs_File_Record) + pOutputBufferIter->field_12_file_name_length + 3, 2));
        pOutputBufferIter = reinterpret_cast<Zip_Mgs_File_Record*>(reinterpret_cast<BYTE*>(pOutputBufferIter) + pOutputBufferIter->field_10_record_size);
    }

    if (pField_14)
    {
        *pField_14 = pCentralDirSizeBuffer;
    }
    else
    {
        free(pCentralDirSizeBuffer); // Original game bug- didn't free
    }

    return 0;
}
MGS_FUNC_IMPLEX(0x00642540, Zip_load_central_directory_642540, FS_IMPL)

static void DumpZipRecords(Zip_Mgs_File_Record* pRec)
{
    BYTE* pIter = reinterpret_cast<BYTE*>(pRec);
    while (pRec->field_10_record_size)
    {
        char* pName = &pRec->field_15_file_name_buffer[0];
        LOG_INFO(pName);
        pIter += pRec->field_10_record_size;
        pRec = reinterpret_cast<Zip_Mgs_File_Record*>(pIter);
    }
}

ZipContext* CC Zip_alloc_642790(int hMgzFile, int* pRet)
{
    int errCode = 0;
    Zip_End_Of_Central_Directory zipEndOfCentralDir = {};

    ZipContext* pZip = (ZipContext *)calloc(1u, sizeof(ZipContext));
    if (!pZip)
    {
        errCode = -4116;
        if (pRet)
        {
            *pRet = errCode;
        }
        return 0;
    }
    
    pZip->field_0_hMgzFile = hMgzFile;
    const int fileSize = Zip_get_file_size_642840(hMgzFile);

    if (fileSize < 0)
    {
        errCode = -4118;
        Zip_free_642740(pZip);

        if (pRet)
        {
            *pRet = errCode;
        }
        return 0;
    }
    
    errCode = Zip_locate_end_of_central_directory_record_6423D0(pZip->field_0_hMgzFile, fileSize, &zipEndOfCentralDir);

    if (errCode)
    {
        Zip_free_642740(pZip);

        if (pRet)
        {
            *pRet = errCode;
        }
        return 0;
    }

    errCode = Zip_load_central_directory_642540(pZip->field_0_hMgzFile, &zipEndOfCentralDir, &pZip->field_14_local_file_headers_ptr);
    DumpZipRecords(pZip->field_14_local_file_headers_ptr);

    if (errCode)
    {
        Zip_free_642740(pZip);

        if (pRet)
        {
            *pRet = errCode;
        }
        return 0;
    }

    pZip->field_18_pTo_field_14_local_file_headers = pZip->field_14_local_file_headers_ptr;
    pZip->field_8_flags_or_counter |= 0x10000000u;
    
    if (pRet)
    {
        *pRet = 0;
    }

    return pZip;
}
MGS_FUNC_IMPLEX(0x00642790, Zip_alloc_642790, FS_IMPL)

ZipContext* CC Zip_Open_642920(const char* pFileName, int* pRet)
{
    int hFile = _open(pFileName, O_BINARY);
    if (hFile == -1)
    {
        hFile = Zip_Try_Open_With_Other_Extensions_642870(pFileName, O_BINARY);
        if (hFile == -1)
        {
            if (pRet)
            {
                *pRet = -4117;
            }
            return nullptr;
        }
    }
    return Zip_alloc_642790(hFile, pRet);
}
MGS_FUNC_IMPLEX(0x00642920, Zip_Open_642920, FS_IMPL)

ZipContext* CC OpenMGZ_051ED67(const char* pFileName)
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
            pMapping = &mapping;
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

        int openRet = 0;
        pMapping->field_8_file_handle = Zip_Open_642920(buffer, &openRet);
    }

    return pMapping->field_8_file_handle;
}
MGS_FUNC_IMPLEX(0x0051ED67, OpenMGZ_051ED67, FS_IMPL)

MGS_ARY(1, 0x776960, Zip_Open_File_Handle, 32, gZipStreams_dword_776960, {});

static Zip_Mgs_File_Record* ZipFindRecord(ZipContext* pZip, const char* pToFind, int flags)
{
    if (!pZip->field_14_local_file_headers_ptr)
    {
        pZip->field_4_last_err = -4124;
        return nullptr;
    }

    auto fn_strcmp = _strcmpi;
    if (!(flags & 8))
    {
        fn_strcmp = strcmp;
    }

    if (flags & 0x200)
    {
        const char* forwardSlashPos = strrchr(pToFind, '/');
        if (forwardSlashPos)
        {
            pToFind = forwardSlashPos + 1;
        }
    }

    BYTE* pIter = reinterpret_cast<BYTE*>(pZip->field_14_local_file_headers_ptr);
    Zip_Mgs_File_Record* pRec = reinterpret_cast<Zip_Mgs_File_Record*>(pIter);
    while (pRec->field_10_record_size)
    {
        const char* pNameInZip = &pRec->field_15_file_name_buffer[0];
        if (flags & 0x200)
        {
            const char* slashPos = strrchr(pNameInZip, '/');
            if (slashPos)
            {
                pNameInZip = slashPos + 1;
            }
        }

        if (fn_strcmp(pNameInZip, pToFind) == 0)
        {
            return pRec;
        }

        pIter += pRec->field_10_record_size;
        pRec = reinterpret_cast<Zip_Mgs_File_Record*>(pIter);
    }

    pZip->field_4_last_err = -4124;
    return nullptr;
}

struct Zip_Zlib_Wrapper 
{
   ZipContext* field_0_zip_context;

   void* field_14_zip_ctx_field10_ptr_or32k_malloc;
}; // TODO


struct Zip_Local_File_Header
{

}; // TODO

Zip_Open_File_Handle* CC OpenFileInMGZ_642BB0(ZipContext* pZip, const char* pFileName, int flags)
{
    Zip_Mgs_File_Record* pRecord = ZipFindRecord(pZip, pFileName, flags);
    if (!pRecord)
    {
        return nullptr;
    }

    // If we have a compression method it must be deflate
    if (pRecord->field_14_compression_method && pRecord->field_14_compression_method != 8)
    {
        pZip->field_4_last_err = -4125;
        return nullptr;
    }

    if (pZip->field_C_84_byte_ptr)
    {
        pZip->field_C_84_byte_ptr = nullptr;
    }
    else
    {
        pZip->field_C_84_byte_ptr = (Zip_Zlib_Wrapper *)calloc(1u, 0x54u);
        if (!pZip->field_C_84_byte_ptr)
        {
            pZip->field_4_last_err = -4116;
            return nullptr;
        }
    }

    pZip->field_C_84_byte_ptr->field_0_zip_context = pZip;
    ++pZip->field_8_flags_or_counter;

    if (pZip->field_10_ptr_shared_buffer)
    {
        pZip->field_C_84_byte_ptr->field_14_zip_ctx_field10_ptr_or32k_malloc = pZip->field_10_ptr_shared_buffer;
        pZip->field_10_ptr_shared_buffer = nullptr;
    }
    else
    {
        pZip->field_C_84_byte_ptr->field_14_zip_ctx_field10_ptr_or32k_malloc = malloc(0x8000u);
    }

    int err = 0;
    if (pZip->field_C_84_byte_ptr->field_14_zip_ctx_field10_ptr_or32k_malloc)
    {
        /*
        if (DoFileSeek_642DE0(pZip->field_1C_zstream) >= 0)
        {
            pZip->field_C_84_byte_ptr->field_18_seeked_pos = pRecord->field_C_offset_to_local_header;
            pZip->field_1C_zstream = pZip->field_C_84_byte_ptr;
            if (_lseek(pZip->field_0_hMgzFile, pRecord->field_C_offset_to_local_header, 0) >= 0)
            {
                if (_read(pZip->field_0_hMgzFile, pZip->field_C_84_byte_ptr->field_14_zip_ctx_field10_ptr_or32k_malloc, 0x1Eu) >= 0x1E)
                {
                    pFileData = reinterpret_cast<Zip_Local_File_Header*>(pZip->field_C_84_byte_ptr->field_14_zip_ctx_field10_ptr_or32k_malloc);

                    if (pFileData->field_0_magic != 0x04034b50)
                    {
                        err = -4127;
                    }
                    else
                    {
                        extra_field_length = Zip_ByteSwap_Word_6423C0(&pFileData->field_1C_extra_field_length);
                        filename_length = Zip_ByteSwap_Word_6423C0(&pFileData->field_1A_filename_length);

                        // Seek to file data starting offset
                        if (_lseek(pZip->field_0_hMgzFile, filename_length + extra_field_length, 1u) >= 0)
                        {
                            err = Zip_zlib_create_642E10(pZip->field_C_84_byte_ptr, pRecord);
                            if (!err)
                            {
                                return pZip->field_C_84_byte_ptr;
                            }
                        }
                        else
                        {
                            err = -4119;
                        }
                    }
                }
                else
                {
                    err = -4120;
                }
            }
            else
            {
                err = -4119;
            }
        }
        else
        {
            err = -4119;
        }*/
    }
    else
    {
        err = -4116;
    }

    if (pZip->field_C_84_byte_ptr)
    {
        //Zip_File_Close_642B30(pZip->field_C_84_byte_ptr);
    }

    pZip->field_4_last_err = err;
    return nullptr;
}
MGS_FUNC_IMPLEX(0x00642BB0, OpenFileInMGZ_642BB0, false) // TODO

AbstractedFileHandle* CC OpenArchiveFile_51EFB2(char* pFileName)
{
    char normalizedFileName[256] = {};
    ReplaceBackWithFowardSlashes_51EC38(pFileName, normalizedFileName);
    ZipContext* pZipCtx = OpenMGZ_051ED67(normalizedFileName);
    if (!pZipCtx)
    {
        return nullptr;
    }

    Zip_Open_File_Handle* pZipStream = nullptr;
    for (int i = 0; i < 32; i++)
    {
        if (gZipStreams_dword_776960[i].field_0_pZipStream)
        {
            pZipStream = &gZipStreams_dword_776960[i];
            break;
        }
    }

    if (!pZipStream)
    {
        return nullptr;
    }

    pZipStream->field_0_pZipStream = OpenFileInMGZ_642BB0(pZipCtx, normalizedFileName, 8);
    if (pZipStream->field_0_pZipStream)
    {
        // Real game has a helper that returns more info, but our helper avoids having to reimpl 0x643230 and avoids duplication in 0x642BB0
        pZipStream->field_8_file_size = ZipFindRecord(pZipCtx, pFileName, 8)->field_0_uncompressed_size;
        pZipStream->field_4_zip_file_handle = pZipCtx;
        return reinterpret_cast<AbstractedFileHandle*>(pZipStream); // Original game bug - this was an index casted to FILE*, but its possible for FILE* to be <= 32
    }

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
