#include "stdafx.h"
#include "Actor_Movie.hpp"
#include "MgsFunction.hpp"
#include "ResourceNameHash.hpp"
#include "Actor.hpp"
#include "Script.hpp"
#include <gmock/gmock.h>
#include "Renderer.hpp"
#include "Sound.hpp"
#include "LibDG.hpp"
#include "WinMain.hpp"
#include "Actor_GameD.hpp"
#include "Timer.hpp"
#include "Masher.hpp"

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


MGS_FUNC_NOT_IMPL(0x528993, void __cdecl(Actor_Movie_Masher *pMasher), Res_movie_masher_decode_image_528993); // TODO
MGS_FUNC_NOT_IMPL(0x52897C, signed int __cdecl(Actor_Movie_Masher *pMasher), Res_movie_masher_read_blocking_52897C); // TODO
MGS_FUNC_NOT_IMPL(0x52899C, void* __cdecl(Actor_Movie_Masher *pMasher), Res_movie_masher_sound_read_52899C); // TODO
MGS_FUNC_NOT_IMPL(0x528973, DWORD __cdecl(Actor_Movie_Masher *pMasher), Res_movie_masher_528973); // TODO

int CC Res_movie_update_helper_45675A()
{
    /*
    if (gMovieData_724A00.field_20_sound_pos)
    {
        Res_movie_masher_decode_image_528993(gMovieData_724A00.field_0_masher_ptr);
    }
    else
    {
        jMovie_MMX_Decode_528985(gMovieData_724A00.field_0_masher_ptr, gMovieData_724A00.gMovieBuffer_724A14);
    }
    */

    if (gMovieData_724A00.field_0_masher_ptr->field_6C_frame_num == 0)
    {
        jMovie_MMX_Decode_528985(gMovieData_724A00.field_0_masher_ptr, gMovieData_724A00.gMovieBuffer_724A14);
    }

    if (gMovieData_724A00.field_20_sound_pos)
    {
        jMovie_MMX_Decode_528985(gMovieData_724A00.field_0_masher_ptr, gMovieData_724A00.gMovieBuffer_724A14);
    }

    if (!gMovieData_724A00.field_2C_audio_play_started)
    {
        Sound_Unknown6();
        gMovieData_724A00.field_2C_audio_play_started = 1;
    }

    Sound_Masher_write_data_523CF3(
        gMovieData_724A00.field_0_masher_ptr,
        Res_movie_masher_read_blocking_52897C.Ptr(),
        Res_movie_masher_sound_read_52899C.Ptr());
    
    gMovieData_724A00.field_1C_read_ret = Res_movie_masher_528973(gMovieData_724A00.field_0_masher_ptr);
    
    gMovieData_724A00.field_20_sound_pos = Sound_Unknown4();
    
    return gMovieData_724A00.field_1C_read_ret;
}
MGS_FUNC_IMPLEX(0x45675A, Res_movie_update_helper_45675A, MOVIE_IMPL);

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
            const int sizeOfRow = 2 * width; // 16 bit pixels
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

MGS_VAR(1, 0x7248D0, Actor_Movie, gMovie_actor_stru_7248D0, {});

MGS_FUNC_NOT_IMPL(0x4562AA, signed int __cdecl(int movieNameHashed), Res_movie_create_helper_4562AA); // TODO
MGS_FUNC_NOT_IMPL(0x456588, void __cdecl(Actor_Movie *pMovie), Res_movie_update_456588); // TODO
MGS_FUNC_NOT_IMPL(0x52895A, void __cdecl(Actor_Movie_Masher *pMasher), Masher_destructor_wrapper_52895A); // TODO

MGS_VAR(1, 0x7248C8, DWORD, gMovieTicks_dword_7248C8, 0);


void CC Res_movie_shutdown_4567DE(Actor_Movie* pMovie)
{
    System_Init_0_And_1_40A465();
    gActorPauseFlags_dword_791A0C &= 0xFFFFFFFE;
    sub_40191F();
    if (gMovieData_724A00.gMovieBuffer_724A14)
    {
        free(gMovieData_724A00.gMovieBuffer_724A14);
        gMovieData_724A00.gMovieBuffer_724A14 = 0;
    }
    Masher_destructor_wrapper_52895A(gMovieData_724A00.field_0_masher_ptr);
    gMovieData_724A00.field_0_masher_ptr = 0;
    Sound_ReleaseBufferQ();
    game_state_dword_72279C.flags &= 0x7FFFFFFFu;
    Timer_30_1();
    if (pMovie->dword_7248F8_script_param_p != -1)
    {
        Script_ProcCancelOrRun(static_cast<WORD>(pMovie->dword_7248F8_script_param_p), 0);
    }
}
MGS_FUNC_IMPLEX(0x4567DE, Res_movie_shutdown_4567DE, MOVIE_IMPL);

Actor_Movie* CC Res_movie_create_4561DF(int movieNameHashed, int bIsEnding)
{
    if (gMovieData_724A00.field_0_masher_ptr)
    {
        return 0;
    }

    MemClearUnknown_40B231(&gMovie_actor_stru_7248D0, sizeof(Actor_Movie));
    MemClearUnknown_40B231(&gMovieData_724A00, sizeof(Actor_Movie_Data) - 4); // -4 because sMovie_IO_Ptrs_Inited_dword_724A30 isn't included

    if (!Res_movie_create_helper_4562AA(movieNameHashed))
    {
        return 0;
    }
    game_state_dword_72279C.flags |= 0x80000000;
    Actor_PushBack_40A2AF(1, &gMovie_actor_stru_7248D0.mBase, 0);
    Actor_Init_40A347(
        &gMovie_actor_stru_7248D0.mBase,
        reinterpret_cast<TActorFunction>(Res_movie_update_456588.Ptr()),
        reinterpret_cast<TActorFunction>(Res_movie_shutdown_4567DE),
        "C:\\mgs\\source\\Game\\movie.c");

    gActorPauseFlags_dword_791A0C |= 1;
    gMovie_actor_stru_7248D0.word_7248F2_11_param_i = 11;
    gMovie_actor_stru_7248D0.dword_7248F4_param_o = static_cast<short>(gMovieData_724A00.field_4_ddv_header->field_C_number_of_frames - 11);
    gMovie_actor_stru_7248D0.dword_7248F8_script_param_p = -1;
    gMovie_actor_stru_7248D0.word_7248F0_counter = 0;
    gMovie_actor_stru_7248D0.dword_7248FC_bIsEnding = bIsEnding;
    gMovieTicks_dword_7248C8 = 0;
    return &gMovie_actor_stru_7248D0;
}
MGS_FUNC_IMPLEX(0x4561DF, Res_movie_create_4561DF, MOVIE_IMPL);

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
