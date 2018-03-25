#include "stdafx.h"
#include "Actor_Movie.hpp"
#include "MgsFunction.hpp"
#include "ResourceNameHash.hpp"
#include "Actor.hpp"
#include "Script.hpp"
#include <gmock/gmock.h>
#include "Renderer.hpp"

void Force_Actor_Movie_Cpp_Link() { }

#define MOVIE_IMPL true

struct File_ASync
{
    FILE* field_0_file_handle;
    BYTE* field_4_read_buffer;
    DWORD field_8_read_size;
    bool field_C_bQuit;
    // 3 byte padding
    DWORD field_10_read_ret;
    HANDLE field_14_hThread;
    HANDLE field_18_w32Event;
    DWORD field_1C_thread_id;
};
MGS_ASSERT_SIZEOF(File_ASync, 0x20);

int CC File_ASync_WaitFinish(File_ASync* pHandle)
{
    for(;;)
    {
        const DWORD ret = WaitForSingleObject(pHandle->field_18_w32Event, 1000u);
        if (ret == WAIT_OBJECT_0)
        {
            return pHandle->field_10_read_ret;
        }

        if (ret != WAIT_TIMEOUT)
        {
            break;
        }
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x528A20, File_ASync_WaitFinish, MOVIE_IMPL);

void CC File_ASync_Close(File_ASync* pHandle)
{
    for (;;)
    {
        const DWORD ret = WaitForSingleObject(pHandle->field_18_w32Event, 1000u);
        if (ret == WAIT_OBJECT_0)
        {
            break;
        }

        if (ret != WAIT_TIMEOUT)
        {
            break;
        }
    }

    DWORD ret = 0;
    do
    {
        ret = WaitForSingleObject(pHandle->field_18_w32Event, 1000u);
    } while (ret == WAIT_TIMEOUT && ret != WAIT_OBJECT_0);

    if (pHandle->field_0_file_handle)
    {
        fclose(pHandle->field_0_file_handle);
    }

    // Signal thread proc to exit
    pHandle->field_C_bQuit = true; // TODO: OG bug - these inter-thread flags should be atomic

    if (pHandle->field_14_hThread)
    {
        // OG bug - should wait for thread to exit instead of terminating it.
        // Terminate will leak memory and possibly corrupt the heap.
        //TerminateThread(pHandle->field_14_hThread, 0);

        PostThreadMessageA(pHandle->field_1C_thread_id, 0x400u, 0x115Cu, 5555 + 1); // Force thread proc to re-check quit condition
        WaitForSingleObject(pHandle->field_14_hThread, INFINITE); // Wait for thread to exit
    }

    if (pHandle->field_18_w32Event)
    {
        CloseHandle(pHandle->field_18_w32Event);
    }

    // OG bug - should be calling free instead
    //delete(pHandle);
    free(pHandle);
}
MGS_FUNC_IMPLEX(0x528A58, File_ASync_Close, MOVIE_IMPL);

DWORD WINAPI File_ThreadProcASyncRead(LPVOID param)
{
    File_ASync* pHandle = (File_ASync*)param;
    MSG msg = {};
    while (!pHandle->field_C_bQuit)
    {
        if (GetMessageA(&msg, 0, 0x400u, 0x400u) != -1 && msg.wParam == 4444 && msg.lParam == 5555)
        {
            pHandle->field_10_read_ret = pHandle->field_8_read_size == fread(
                pHandle->field_4_read_buffer,
                1u,
                pHandle->field_8_read_size,
                pHandle->field_0_file_handle);
            SetEvent(pHandle->field_18_w32Event);
        }
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x528B38, File_ThreadProcASyncRead, MOVIE_IMPL);

FILE* CC File_ASync_Open_528ACD(const char* filename)
{
    File_ASync* pHandle = (File_ASync *)malloc(sizeof(File_ASync));
    if (!pHandle)
    {
        return nullptr;
    }
    pHandle->field_C_bQuit = false;
    pHandle->field_14_hThread = CreateThread(
        0,
        16384u,
        File_ThreadProcASyncRead,
        pHandle,
        0,
        &pHandle->field_1C_thread_id);

    pHandle->field_18_w32Event = CreateEventA(0, 1, 1, 0);
    pHandle->field_10_read_ret = 1;
    pHandle->field_0_file_handle = fopen(filename, "rb");

    if (!pHandle->field_0_file_handle)
    {
        File_ASync_Close(pHandle);
        return nullptr;
    }

    return (FILE *)pHandle;
}
MGS_FUNC_IMPLEX(0x528ACD, File_ASync_Open_528ACD, MOVIE_IMPL);

signed int CC File_ASync_Read(File_ASync* pHandle, BYTE* readBuffer, int readSize)
{
    signed int result = 0;
    for (;;)
    {
        const DWORD ret = WaitForSingleObject(pHandle->field_18_w32Event, 1000u);
        if (ret == WAIT_OBJECT_0)
        {
            result = pHandle->field_10_read_ret;
            break;
        }

        if (ret != WAIT_TIMEOUT)
        {
            break;
        }
    }

    if (result)
    {
        pHandle->field_4_read_buffer = readBuffer;
        pHandle->field_8_read_size = readSize;
        ResetEvent(pHandle->field_18_w32Event);
        for (;;)
        {
            if (PostThreadMessageA(pHandle->field_1C_thread_id, 0x400u, 0x115Cu, 5555))
            {
                break;
            }
            Sleep(200u);
        }
        result = 1;
    }
    return result;
}
MGS_FUNC_IMPLEX(0x528BBA, File_ASync_Read, MOVIE_IMPL);

int CC File_ASync_Seek(File_ASync* pHandle, __int32 offset, int origin)
{
    signed int result = 0;
    for (;;)
    {
        const DWORD ret = WaitForSingleObject(pHandle->field_18_w32Event, 1000u);
        if (ret == WAIT_OBJECT_0)
        {
            result = pHandle->field_10_read_ret;
            break;
        }

        if (ret != WAIT_TIMEOUT)
        {
            break;
        }
    }

    if (result)
    {
        result = fseek(pHandle->field_0_file_handle, offset, origin) != 0;
    }
    return result;
}
MGS_FUNC_IMPLEX(0x528C65, File_ASync_Seek, MOVIE_IMPL);

void CC Res_movie_copy_frame_to_back_buffer_51D613(unsigned int width, unsigned int height, const void *pPixelData)
{
    if (!g_pBackBuffer_6FC738)
    {
        return;
    }

    DDSURFACEDESC2 surfaceDesc = {};
    surfaceDesc.dwSize = sizeof(DDSURFACEDESC2);
    int lockAttempts = 0;
    while (FAILED(g_pBackBuffer_6FC738->Lock(0, &surfaceDesc, DDLOCK_WRITEONLY | DDLOCK_WAIT, 0)))
    {
        lockAttempts++;
        if (lockAttempts > 100)
        {
            return;
        }
    }

    if (surfaceDesc.dwWidth >= width && surfaceDesc.dwHeight >= height)
    {
        const LONG pitch = surfaceDesc.lPitch;
        BYTE* pRowDst = reinterpret_cast<BYTE*>(surfaceDesc.lpSurface)
            + 2 * ((surfaceDesc.dwWidth - width) >> 1)
            + pitch * ((surfaceDesc.dwHeight - height) >> 1);

        if (height > 0)
        {
            const int sizeOfRow = 2 * width;
            for (DWORD i=0; i<height; i++)
            {
                memcpy(pRowDst, pPixelData, sizeOfRow);
                pPixelData = (BYTE *)pPixelData + sizeOfRow;
                pRowDst += pitch;
            }
        }
    }

    g_pBackBuffer_6FC738->Unlock(0);
}
MGS_FUNC_IMPLEX(0x51D613, Res_movie_copy_frame_to_back_buffer_51D613, MOVIE_IMPL);

char* CC Res_movie_GetName_4564F5(char* currentDir, WORD movieNameHashed)
{
    static const char* sMovieNames_66B9D8[] =
    {
        "genbaku",
        "kasou",
        "police",
        "kaitai",
        "alaska",
        "wangan",
        "idenshi",
        "inuzori",
        "e399",
        "null"
    };

    static char sMoviePath_724900[256] = {};

    int i = 0;
    for (i = 0; i < _countof(sMovieNames_66B9D8)-1; i++)
    {
        if (ResourceNameHash(sMovieNames_66B9D8[i]) == movieNameHashed)
        {
            break;
        }
    }
    sprintf(sMoviePath_724900, "%s/movie/%s.ddv", currentDir, sMovieNames_66B9D8[i]);
    return sMoviePath_724900;
}
MGS_FUNC_IMPLEX(0x4564F5, Res_movie_GetName_4564F5, MOVIE_IMPL);

MGS_FUNC_NOT_IMPL(0x4561DF, Actor_Movie *__cdecl (int movieNameHashed, int bIsEnding), Res_movie_create_4561DF); // TODO

Actor_Movie* CC Res_movie_create_456860(int movieNameHashed)
{
    Actor_Movie* pActorMovie = Res_movie_create_4561DF(movieNameHashed, 0);
    if (pActorMovie)
    {
        if (Script_ParamExists('i'))
        {
            pActorMovie->word_7248F2_11_param_i += static_cast<WORD>(Script_Unknown8_409924(Script_GetReturnAddress()));
        }

        if (Script_ParamExists('o'))
        {
            pActorMovie->dword_7248F4_param_o -= static_cast<WORD>(Script_Unknown8_409924(Script_GetReturnAddress()));
        }

        if (Script_ParamExists('p'))
        {
            pActorMovie->dword_7248F8_script_param_p = static_cast<WORD>(Script_Unknown8_409924(Script_GetReturnAddress()));
        }
        return pActorMovie;
    }

    if (Script_ParamExists('p'))
    {
        Script_ProcCancelOrRun(static_cast<WORD>(Script_Unknown8_409924(Script_GetReturnAddress())), 0);
    }
    return nullptr;
}
MGS_FUNC_IMPLEX(0x456860, Res_movie_create_456860, MOVIE_IMPL);

Actor* CC Res_movie_create_4561C6(DWORD scriptData, int /*scriptBinds*/, BYTE* /*pScript*/)
{
    return &Res_movie_create_456860(scriptData)->mBase;
}
MGS_FUNC_IMPLEX(0x4561C6, Res_movie_create_4561C6, MOVIE_IMPL);

void DoMovie_Tests()
{
    ASSERT_STREQ("./movie/null.ddv", Res_movie_GetName_4564F5(".", ResourceNameHash("lols")));
    ASSERT_STREQ("./movie/genbaku.ddv", Res_movie_GetName_4564F5(".", ResourceNameHash("genbaku")));
}
