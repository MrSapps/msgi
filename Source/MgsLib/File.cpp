#include "stdafx.h"
#include "File.hpp"

// We must call MSG version of stdlib functions for shared var, e.g the FILE* struct for the
// stdlib used by MSGI.exe isn't the same as ours, mixing them will lead to a bad time.
MSG_FUNC_NOT_IMPL(0x0053CB40, FILE* __cdecl(const char*, const char*), mgs_fopen);
MSG_FUNC_NOT_IMPL(0x0053C970, int __cdecl(const char*, FILE*), mgs_fputs);
MSG_FUNC_NOT_IMPL(0x0053C6C0, int __cdecl(FILE*), mgs_fflush);
MSG_FUNC_NOT_IMPL(0x0053C4A0, int __cdecl(FILE *File), mgs_fclose);


// Memory allocation
MSG_FUNC_IMPL(0x00539990, mgs_malloc);
MSG_FUNC_IMPL(0x0053A400, mgs_free);
MSG_FUNC_IMPL(0x00539E20, mgs_realloc);
MSG_FUNC_IMPL(0x00539DA0, mgs_calloc);

void *__cdecl mgs_malloc(size_t Size)
{
    if (IsMgsi())
    {
        void* ptr = mgs_malloc_.Ptr()(Size);
        if (ptr)
        {
            MgsVar::TrackAlloc(ptr, Size);
        }
        return ptr;
    }
    return malloc(Size);
}

void __cdecl mgs_free(void *Memory)
{
    if (IsMgsi())
    {
        if (Memory)
        {
            MgsVar::TrackFree(Memory);
        }
        mgs_free_.Ptr()(Memory);
    }
    else
    {
        free(Memory);
    }
}

void *__cdecl mgs_realloc(void *Memory, size_t NewSize)
{
    if (IsMgsi())
    {
        // TODO: Track
        LOG_WARNING("realloc() not tracked");
        return mgs_realloc_.Ptr()(Memory, NewSize);
    }
    else
    {
        return realloc(Memory, NewSize);
    }
}

void *__cdecl mgs_calloc(size_t NumOfElements, size_t SizeOfElements)
{
    if (IsMgsi())
    {
        // TODO: Track
        LOG_WARNING("calloc() not tracked");
        return mgs_calloc_.Ptr()(NumOfElements, SizeOfElements);
    }
    else
    {
        return calloc(NumOfElements, SizeOfElements);
    }
}


// Can't seem to make this work, calling this will crash due to issue mentioned above
//MSG_FUNC_NOT_IMPL(0x0053C5F0, int __cdecl(FILE*, const char*, ...), mgs_fprintf);
// So temp HACK - just get a pointer to the MSG func and call directly, remove when all funcs using it
// are re-impled
TMgs_fprintf mgs_fprintf = (TMgs_fprintf)0x0053C5F0;
TMgs_open mgs_open = (TMgs_open)0x0053DBE0;

MSG_FUNC_NOT_IMPL(0x0053D680, int __cdecl(int), mgs_close);
MSG_FUNC_NOT_IMPL(0x0053E180, int __cdecl(int, LONG, DWORD), mgs_lseek);
MSG_FUNC_NOT_IMPL(0x0053D1A0, int __cdecl(int, void*, DWORD), mgs_read);

// File lib
MSG_FUNC_NOT_IMPL(0x0051EE8F, FILE *__cdecl(const char*, signed int), File_LoadDirFileQ);
MSG_FUNC_NOT_IMPL(0x0051F0F5, size_t __cdecl(FILE*, void*, DWORD), File_NormalRead);
MSG_FUNC_NOT_IMPL(0x0051F183, int __cdecl(FILE*), File_CloseQ);
