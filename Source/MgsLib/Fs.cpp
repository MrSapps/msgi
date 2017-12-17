#include "stdafx.h"
#include "Fs.hpp"
#include "System.hpp"
#include "Actor_Loader.hpp"
#include "Actor_GameD.hpp"
#include "File.hpp"
#include <gmock/gmock.h>
#include <fcntl.h>

#include <zlib.h>

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

MGS_ASSERT_SIZEOF(z_stream, 0x38);

struct ZipContext
{
    int field_0_hMgzFile;
    int field_4_last_err;
    DWORD field_8_flags_or_counter;
    Zip_Zlib_Wrapper* field_C_free_zip_stream;
    void* field_10_saved_stream_buffer;
    Zip_Mgs_File_Record* field_14_local_file_headers_ptr;
    void* field_18_pTo_field_14_local_file_headers; // Non owned pointer
    Zip_Zlib_Wrapper* field_1C_zip_stream_last_seeked;
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
    Zip_Zlib_Wrapper* field_0_pZipStream;
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

struct Zip_Zlib_Wrapper
{
    ZipContext* field_0_zip_context;
    DWORD field_4;
    DWORD field_8_compression_method;
    DWORD field_C_uncompressed_size;
    DWORD field_10_compressed_size_avail;
    void* field_14_32k_stream_buffer;
    DWORD field_18_seeked_pos;
    z_stream field_1C_zstream;
};
MGS_ASSERT_SIZEOF(Zip_Zlib_Wrapper, 0x54);

#pragma pack(push)
#pragma pack(1)
struct Zip_Local_File_Header
{
    DWORD field_0_magic;
    WORD field_4_version_to_extract;
    WORD field_6_flags;
    WORD field_8_compression_method;
    DWORD field_A_timestamp;
    DWORD field_E_crc;
    DWORD field_12_compressed_size;
    DWORD field_16_uncompressed_size;
    WORD field_1A_filename_length;
    WORD field_1C_extra_field_length;
};
MGS_ASSERT_SIZEOF(Zip_Local_File_Header, 0x1E);
#pragma pack(pop)

struct MgzMapping
{
    const char* field_0_path;
    const char* field_4_mgz;
    ZipContext* field_8_zip_context;
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

MGS_ARY(1, 0x776960, Zip_Open_File_Handle, 32, gZipStreams_dword_776960, {});


static FILE* const kInvalidFileHandle = reinterpret_cast<FILE*>(-2);

static inline bool IsValidFILE(FILE* f)
{
    return f && f != kInvalidFileHandle && f != reinterpret_cast<FILE*>(-1);
}

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

    if (pZip->field_C_free_zip_stream)
    {
        free(pZip->field_C_free_zip_stream);
    }

    if (pZip->field_10_saved_stream_buffer)
    {
        free(pZip->field_10_saved_stream_buffer);
    }

    free(pZip);
}
MGS_FUNC_IMPLEX(0x00642740, Zip_free_642740, FS_IMPL)

void CC Zip_Close_642720(ZipContext* pZip)
{
    const bool bFree = (pZip->field_8_flags_or_counter & 0xEFFFFFFF) == 0;
    pZip->field_8_flags_or_counter &= 0xEFFFFFFF;
    if (bFree)
    {
        Zip_free_642740(pZip);
    }
}
MGS_FUNC_IMPLEX(0x00642720, Zip_Close_642720, FS_IMPL)

void CC Zip_Stream_free_642B30(Zip_Zlib_Wrapper* pZipStream)
{
    ZipContext* pZipContext = pZipStream->field_0_zip_context;
    if (pZipStream->field_8_compression_method)
    {
        inflateEnd(&pZipStream->field_1C_zstream);
    }

    if (pZipStream->field_14_32k_stream_buffer)
    {
        // Does the zip have a cached stream?
        if (pZipContext->field_10_saved_stream_buffer)
        {
            // Yes so free the current stream
            free(pZipStream->field_14_32k_stream_buffer);
        }
        else
        {
            // No so save the current stream in the zip for re-use
            pZipContext->field_10_saved_stream_buffer = pZipStream->field_14_32k_stream_buffer;
        }
    }

    if (pZipContext->field_1C_zip_stream_last_seeked == pZipStream)
    {
        pZipContext->field_1C_zip_stream_last_seeked = nullptr;
    }

    --pZipContext->field_8_flags_or_counter;

    memset(pZipStream, 0, sizeof(Zip_Zlib_Wrapper));

    // Do we have a cached stream?
    if (pZipContext->field_C_free_zip_stream)
    {
        // Yes so free this one
        free(pZipStream);
    }
    else
    {
        // Else use it as the cached stream
        pZipContext->field_C_free_zip_stream = pZipStream;
    }

    // See if we need the free the zip archive itself
    if (!pZipContext->field_8_flags_or_counter)
    {
        Zip_Close_642720(pZipContext);
    }
}
MGS_FUNC_IMPLEX(0x00642B30, Zip_Stream_free_642B30, FS_IMPL);

void CC CloseCachedMgzFiles_51EE23()
{
    for (int i = 0; i < 32; i++)
    {
        if (gZipStreams_dword_776960[i].field_0_pZipStream)
        {
            Zip_Stream_free_642B30(gZipStreams_dword_776960[i].field_0_pZipStream);
        }
    }

    for (MgzMapping& mapping : gMgzTable_6898E8)
    {
        if (mapping.field_8_zip_context)
        {
            Zip_Close_642720(mapping.field_8_zip_context);
        }
    }
}
MGS_FUNC_IMPLEX(0x0051EE23, CloseCachedMgzFiles_51EE23, FS_IMPL);

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

    LONG readPos = fileSize - sizeof(Zip_End_Of_Central_Directory);
    Zip_End_Of_Central_Directory record = {};

    // On first pass read just the size of a record from the end of the file, else reading 512 blocks
    if (_lseek(hMgzFile, readPos, SEEK_SET) < 0)
    {
        return -4119;
    }

    if (_read(hMgzFile, &record, sizeof(Zip_End_Of_Central_Directory)) < sizeof(Zip_End_Of_Central_Directory))
    {
        return -4120;
    }

    // Original game bug - checked if buffer was null, can never happen

    if (record.field_0_magic == 0x06054b50)
    {
        memcpy(pEndOfCentralDir, &record, sizeof(Zip_End_Of_Central_Directory));
        return 0;
    }

    // Simple case of it being right at the end of the file has failed, load up to 64k to try to find it
    char buffer[(1024 * 64) + sizeof(Zip_End_Of_Central_Directory)] = {}; // 64k comment + header size
    int readSize = 0;
    if (sizeof(buffer) > fileSize)
    {
        // File is smaller than 64k so read all of it
        readPos = 0;
        readSize = fileSize;
    }
    else
    {
        // File is bigger than 64k, just read last 64k
        readPos = fileSize - sizeof(buffer);
        readSize = sizeof(buffer);
    }

    if (_lseek(hMgzFile, readPos, SEEK_SET) < 0)
    {
        return -4119;
    }

    if (_read(hMgzFile, buffer, readSize) < readSize)
    {
        return -4120;
    }

    char* pBufferIter = buffer;
    while (pBufferIter < (buffer + sizeof(buffer) - sizeof(Zip_End_Of_Central_Directory)))
    {
        Zip_End_Of_Central_Directory* pDir = reinterpret_cast<Zip_End_Of_Central_Directory*>(pBufferIter);
        if (pDir->field_0_magic == 0x06054b50)
        {
            memcpy(pEndOfCentralDir, pDir, sizeof(Zip_End_Of_Central_Directory));
            return 0;
        }
        pBufferIter++;
    }
    return -4122;
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

    if (!pMapping->field_8_zip_context)
    {
        char buffer[256] = {};
        strcpy(buffer, ".");
        strcat(buffer, "/");
        strcat(buffer, pMapping->field_4_mgz);

        int openRet = 0;
        pMapping->field_8_zip_context = Zip_Open_642920(buffer, &openRet);
    }

    return pMapping->field_8_zip_context;
}
MGS_FUNC_IMPLEX(0x0051ED67, OpenMGZ_051ED67, FS_IMPL)

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

signed int CC Zip_Stream_Seek_642DE0(Zip_Zlib_Wrapper* pZipStream)
{
    if (pZipStream)
    {
        const int seek_ret = _lseek(pZipStream->field_0_zip_context->field_0_hMgzFile, 0, 1u);
        if (seek_ret < 0)
        {
            return -1;
        }
        pZipStream->field_18_seeked_pos = seek_ret;
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x00642DE0, Zip_Stream_Seek_642DE0, FS_IMPL)

int CC Zip_zlib_create_642E10(Zip_Zlib_Wrapper* pZipStream, Zip_Mgs_File_Record* pFileRecord)
{
    pZipStream->field_8_compression_method = pFileRecord->field_14_compression_method;
    pZipStream->field_C_uncompressed_size = pFileRecord->field_0_uncompressed_size;
    if (!pZipStream->field_8_compression_method)
    {
        return 0;
    }

    const int initRet = inflateInit2(&pZipStream->field_1C_zstream, -MAX_WBITS); // The -max bits craziness is to prevent zlib looking for gzip or zlib headers
    if (initRet == Z_OK)
    {
        pZipStream->field_10_compressed_size_avail = pFileRecord->field_4_compressed_size;
        return 0;
    }

    if (pZipStream)
    {
        Zip_Stream_free_642B30(pZipStream);
    }

    return initRet;
}
MGS_FUNC_IMPLEX(0x00642E10, Zip_zlib_create_642E10, FS_IMPL)

Zip_Zlib_Wrapper* CC OpenFileInMGZ_642BB0(ZipContext* pZip, const char* pFileName, int flags)
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

    // Take the free zip stream if we have one
    Zip_Zlib_Wrapper* field_C_84_byte_ptr = pZip->field_C_free_zip_stream;
    if (field_C_84_byte_ptr)
    {
        pZip->field_C_free_zip_stream = nullptr;
    }
    else
    {
        // Else we need to allocate a new one
        field_C_84_byte_ptr = (Zip_Zlib_Wrapper *)calloc(1u, sizeof(Zip_Zlib_Wrapper));
        if (!field_C_84_byte_ptr)
        {
            pZip->field_4_last_err = -4116;
            return nullptr;
        }
    }

    field_C_84_byte_ptr->field_0_zip_context = pZip;
    ++pZip->field_8_flags_or_counter;

    // Take the saved stream buffer if we have one
    if (pZip->field_10_saved_stream_buffer)
    {
        field_C_84_byte_ptr->field_14_32k_stream_buffer = pZip->field_10_saved_stream_buffer;
        pZip->field_10_saved_stream_buffer = nullptr;
    }
    else
    {
        // Else allocate a new one
        field_C_84_byte_ptr->field_14_32k_stream_buffer = malloc(0x8000u);
    }

    int err = 0;
    if (field_C_84_byte_ptr->field_14_32k_stream_buffer)
    {
        if (Zip_Stream_Seek_642DE0(pZip->field_1C_zip_stream_last_seeked) >= 0)
        {
            field_C_84_byte_ptr->field_18_seeked_pos = pRecord->field_C_offset_to_local_header;
            pZip->field_1C_zip_stream_last_seeked = field_C_84_byte_ptr;
            if (_lseek(pZip->field_0_hMgzFile, pRecord->field_C_offset_to_local_header, 0) >= 0)
            {
                if (_read(pZip->field_0_hMgzFile, field_C_84_byte_ptr->field_14_32k_stream_buffer, sizeof(Zip_Local_File_Header)) >= sizeof(Zip_Local_File_Header))
                {
                    Zip_Local_File_Header* pFileData = (Zip_Local_File_Header *)field_C_84_byte_ptr->field_14_32k_stream_buffer;

                    if (pFileData->field_0_magic != 0x04034b50)
                    {
                        err = -4127;
                    }
                    else
                    {
                        const WORD extra_field_length = Zip_ByteSwap_Word_6423C0(&pFileData->field_1C_extra_field_length);
                        const WORD filename_length = Zip_ByteSwap_Word_6423C0(&pFileData->field_1A_filename_length);
                        if (_lseek(pZip->field_0_hMgzFile, filename_length + extra_field_length, SEEK_CUR) >= 0)
                        {
                            err = Zip_zlib_create_642E10(field_C_84_byte_ptr, pRecord);
                            if (!err)
                            {
                                return field_C_84_byte_ptr;
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
        }
    }
    else
    {
        err = -4116;
    }

    if (field_C_84_byte_ptr)
    {
        Zip_Stream_free_642B30(field_C_84_byte_ptr);
    }

    pZip->field_4_last_err = err;
    return nullptr;
}
MGS_FUNC_IMPLEX(0x00642BB0, OpenFileInMGZ_642BB0, FS_IMPL)

FILE* CC OpenArchiveFile_51EFB2(char* pFileName)
{
    char normalizedFileName[256] = {};
    ReplaceBackWithFowardSlashes_51EC38(pFileName, normalizedFileName);
    ZipContext* pZipCtx = OpenMGZ_051ED67(normalizedFileName);
    if (!pZipCtx)
    {
        return nullptr;
    }

    Zip_Open_File_Handle* pFreeZipStream = nullptr;
    for (int i = 0; i < 32; i++)
    {
        if (!gZipStreams_dword_776960[i].field_0_pZipStream)
        {
            pFreeZipStream = &gZipStreams_dword_776960[i];
            break;
        }
    }

    if (!pFreeZipStream)
    {
        return nullptr;
    }

    pFreeZipStream->field_0_pZipStream = OpenFileInMGZ_642BB0(pZipCtx, normalizedFileName, 8);
    if (pFreeZipStream->field_0_pZipStream)
    {
        // Real game has a helper that returns more info, but our helper avoids having to reimpl 0x643230 and avoids duplication in 0x642BB0
        pFreeZipStream->field_8_file_size = ZipFindRecord(pZipCtx, pFileName, 8)->field_0_uncompressed_size;
        pFreeZipStream->field_4_zip_file_handle = pZipCtx;
        return reinterpret_cast<FILE*>(pFreeZipStream); // Original game bug - this was an index casted to FILE*, but its possible for FILE* to be <= 32
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
        hFile = fopen(fileToOpen, strOpenMode);
        if (!hFile)
        {
            // For some reason try again one more time?
            strcpy(fileToOpen, ".");
            strcat(fileToOpen, "/");
            strcat(fileToOpen, remappedFileName);
            hFile = fopen(fileToOpen, strOpenMode);
        }
    }
    return hFile;
}
MGS_FUNC_IMPLEX(0x0051EE8F, File_LoadDirFile_51EE8F, FS_IMPL)

int CC Zip_Read_File_642EA0(Zip_Zlib_Wrapper* pFileRecord, void* dstBuf, signed int nNumberOfBytesToRead)
{
    if (!pFileRecord || !pFileRecord->field_0_zip_context)
    {
        return 0;
    }

    const int remainderSize = pFileRecord->field_C_uncompressed_size;
    int remainder = 0;
    int actualSizeToRead = 0;
    if (remainderSize <= nNumberOfBytesToRead)
    {
        // Trying to read more than is left in the file so cap to that
        remainder = pFileRecord->field_C_uncompressed_size;
        actualSizeToRead = remainderSize;
    }
    else
    {
        // Trying to read less than what is left in the file
        actualSizeToRead = nNumberOfBytesToRead;
        remainder = nNumberOfBytesToRead;
    }

    if (remainderSize)
    {
        // If the last used stream isn't this one then seek the zip file to the correct location as the 
        // file handle is shared between all open zip streams.
        if (pFileRecord->field_0_zip_context->field_1C_zip_stream_last_seeked != pFileRecord)
        {
            if (Zip_Stream_Seek_642DE0(pFileRecord->field_0_zip_context->field_1C_zip_stream_last_seeked) < 0
                || _lseek(pFileRecord->field_0_zip_context->field_0_hMgzFile, pFileRecord->field_18_seeked_pos, 0) < 0)
            {
                pFileRecord->field_0_zip_context->field_4_last_err = -4119;
                return -1;
            }
            pFileRecord->field_0_zip_context->field_1C_zip_stream_last_seeked = pFileRecord;
        }

        // Only attempt to decompress if the file is actually compressed
        if (pFileRecord->field_8_compression_method)
        {
            // Set up zlib output size/buffer
            pFileRecord->field_1C_zstream.avail_out = actualSizeToRead;
            pFileRecord->field_1C_zstream.next_out = (Bytef*)dstBuf;

            // Decompression loop
            for(;;)
            {
                int sizeToRead = pFileRecord->field_10_compressed_size_avail;
                if (sizeToRead > 0 && !pFileRecord->field_1C_zstream.avail_in)
                {
                    if (sizeToRead > 32768)
                    {
                        sizeToRead = 32768;
                    }

                    const int sizeRead = _read(pFileRecord->field_0_zip_context->field_0_hMgzFile, pFileRecord->field_14_32k_stream_buffer, sizeToRead);
                    if (sizeRead <= 0)
                    {
                        pFileRecord->field_0_zip_context->field_4_last_err = -4120;
                        return -1;
                    }

                    pFileRecord->field_10_compressed_size_avail -= sizeRead;
                    pFileRecord->field_1C_zstream.avail_in = sizeRead;
                    pFileRecord->field_1C_zstream.next_in = (Bytef*)pFileRecord->field_14_32k_stream_buffer;
                }

                const int prev_total_out = pFileRecord->field_1C_zstream.total_out;
                const int infateRet = inflate(&pFileRecord->field_1C_zstream, Z_NO_FLUSH);
                if (infateRet == Z_STREAM_END)
                {
                    pFileRecord->field_C_uncompressed_size = 0;
                }
                else
                {
                    if (infateRet) // Can only be Z_NEED_DICT?
                    {
                        pFileRecord->field_0_zip_context->field_4_last_err = infateRet;
                        return -1;
                    }
                    pFileRecord->field_C_uncompressed_size += prev_total_out - pFileRecord->field_1C_zstream.total_out;
                }

                if (!pFileRecord->field_C_uncompressed_size || !pFileRecord->field_1C_zstream.avail_out)
                {
                    return remainder - pFileRecord->field_1C_zstream.avail_out;
                }
            }
        }
        else
        {
            // Uncompressed read
            const int result = _read(pFileRecord->field_0_zip_context->field_0_hMgzFile, dstBuf, actualSizeToRead);
            if (result <= 0)
            {
                if (result < 0)
                {
                    pFileRecord->field_0_zip_context->field_4_last_err = -4120;
                }
            }
            else
            {
                pFileRecord->field_C_uncompressed_size -= result;
            }
            return result;
        }
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x00642EA0, Zip_Read_File_642EA0, FS_IMPL)

static inline Zip_Open_File_Handle* AsZipFile(FILE* file)
{
    auto pZip = reinterpret_cast<Zip_Open_File_Handle*>(file);
    if (pZip >= &gZipStreams_dword_776960[0] && pZip <= &gZipStreams_dword_776960[31])
    {
        return pZip;
    }
    return nullptr;
}

size_t CC File_NormalRead_51F0F5(FILE* File, void* dstBuf, DWORD nNumberOfBytesToRead)
{
    if (AsZipFile(File))
    {
        return Zip_Read_File_642EA0(AsZipFile(File)->field_0_pZipStream, dstBuf, nNumberOfBytesToRead);
    }
    else if (IsValidFILE(File))
    {
        return fread(dstBuf, 1, nNumberOfBytesToRead, File);
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x0051F0F5, File_NormalRead_51F0F5, FS_IMPL)

__int32 CC File_GetPos_51F09E(FILE* File, __int32 Offset, int Origin)
{
    if (AsZipFile(File))
    {
        if (Origin == SEEK_END)
        {
            return AsZipFile(File)->field_8_file_size;
        }
        else
        {
            LOG_ERROR("Origin " << Origin << " not supported for ZIP streams");
        }
        return 0;
    }
    else if (IsValidFILE(File))
    {
        fseek(File, Offset, Origin);
        return ftell(File);
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x0051F09E, File_GetPos_51F09E, FS_IMPL)

void CC Zip_Stream_Close_51F1AD(Zip_Open_File_Handle* pZipItem)
{
    Zip_Stream_free_642B30(pZipItem->field_0_pZipStream);
    pZipItem->field_0_pZipStream = nullptr;
    pZipItem->field_4_zip_file_handle = nullptr;
    pZipItem->field_8_file_size = 0;
}
MGS_FUNC_IMPLEX(0x0051F1AD, Zip_Stream_Close_51F1AD, FS_IMPL)

int CC File_Close_51F183(FILE* File)
{
    if (AsZipFile(File))
    {
        Zip_Stream_Close_51F1AD(AsZipFile(File));
        return 0;
    }
    else if (IsValidFILE(File))
    {
        return fclose(File);
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x0051F183, File_Close_51F183, FS_IMPL)

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
