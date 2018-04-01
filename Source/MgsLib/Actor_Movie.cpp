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


MGS_FUNC_NOT_IMPL(0x528993, void __cdecl(Actor_Movie_Masher *pMasher), Res_movie_masher_decode_image_528993); // TODO


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

   // if (gMovieData_724A00.field_0_masher_ptr->field_6C_frame_num == 0)
    {
        jMovie_MMX_Decode_528985(gMovieData_724A00.field_0_masher_ptr, gMovieData_724A00.gMovieBuffer_724A14);
    }

   // if (gMovieData_724A00.field_20_sound_pos)
    {
      //  jMovie_MMX_Decode_528985(gMovieData_724A00.field_0_masher_ptr, gMovieData_724A00.gMovieBuffer_724A14);
    }

    if (!gMovieData_724A00.field_2C_audio_play_started)
    {
        Sound_Unknown6();
        gMovieData_724A00.field_2C_audio_play_started = 1;
    }

    Sound_Masher_Write_Audio_Frame_523CF3(
        gMovieData_724A00.field_0_masher_ptr,
        Res_movie_masher_read_blocking_52897C,
        Res_movie_masher_sound_read_52899C);
    
    gMovieData_724A00.field_1C_read_ret = Res_movie_masher_read_frame_data_528973(gMovieData_724A00.field_0_masher_ptr);
    
    int sound_pos = Sound_Masher_Unknown_523E12();
    //LOG_INFO("sound_pos " << sound_pos << " gBytesWrote_77D880 " << gBytesWrote_77D880);
    gMovieData_724A00.field_20_sound_pos = sound_pos;

    //Sleep(50);

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

MGS_VAR(1, 0x7248C8, DWORD, gMovieTicks_dword_7248C8, 0);

void CC Res_movie_update_helper_456622(Actor_Movie* pMovie)
{
    bool bKeepPlaying = true;
    if (!pMovie->dword_7248FC_bIsEnding || !(gMovieTicks_dword_7248C8 % 2))
    {
        bKeepPlaying = Res_movie_update_helper_45675A() ? true : false;
        // NOTE: Trimmed dead branches here called stub function
        ++pMovie->word_7248F0_counter;
    }

    Res_movie_copy_frame_to_back_buffer_51D613(
        gMovieData_724A00.field_24_double_width * gMovieData_724A00.field_8_video_header->field_4_width,
        gMovieData_724A00.field_28_double_height * gMovieData_724A00.field_8_video_header->field_8_height,
        gMovieData_724A00.gMovieBuffer_724A14);

    ++gMovieTicks_dword_7248C8;

    if (GameD_Input_445610() & PsxButtonBits::eCross)
    {
        bKeepPlaying = false;
    }

    if (!bKeepPlaying)
    {
        Actor_DestroyOnNextUpdate_40A3ED(&pMovie->mBase);
    }
}
MGS_FUNC_IMPLEX(0x456622, Res_movie_update_helper_456622, MOVIE_IMPL);

char* CC Res_movie_GetName_4564F5(char* currentDir, int movieNameHashed)
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

Actor_Movie_Masher* CC Masher_constructor_wrapper_5288F8(const char* pMovieFileName, Actor_Movie_DDV_Header** pDDVHeader, Actor_Movie_DDV_VideoHeader** pVideoHeader, Actor_Movie_DDV_AudioHeader** pAudioHeader, DWORD* errCode)
{
    Actor_Movie_Masher* pMasher = new (std::nothrow)Actor_Movie_Masher();
    if (!pMasher)
    {
        *errCode = 2;
        return nullptr;
    }

    const int ctorRet = Masher_constructor_523FA0(pMasher, 0, pMovieFileName);
    *errCode = ctorRet;

    if (ctorRet)
    {
        Masher_destructor_524214(pMasher, 0);
        delete pMasher;
        return nullptr;
    }

    *pDDVHeader = &pMasher->field_4_ddv_header;
    *pVideoHeader = &pMasher->field_14_video_header;
    *pAudioHeader = &pMasher->field_2C_audio_header;

    return pMasher;
}

void CC Masher_destructor_wrapper_52895A(Actor_Movie_Masher *pMasher)
{
    if (pMasher)
    {
        Masher_destructor_524214(pMasher, 0);
        delete pMasher;
    }
}

void CC Res_movie_helper_free_45654B()
{
    if (gMovieData_724A00.field_0_masher_ptr)
    {
        Masher_destructor_wrapper_52895A(gMovieData_724A00.field_0_masher_ptr);
        gMovieData_724A00.field_0_masher_ptr = nullptr;
    }

    if (gMovieData_724A00.gMovieBuffer_724A14)
    {
        free(gMovieData_724A00.gMovieBuffer_724A14);
        gMovieData_724A00.gMovieBuffer_724A14 = nullptr;
    }
}
MGS_FUNC_IMPLEX(0x45654B, Res_movie_helper_free_45654B, MOVIE_IMPL);

char CC Res_Movie_GetBackBufferPixelFormat_51D566()
{
    if (!g_pBackBuffer_6FC738)
    {
        return -1;
    }

    DDPIXELFORMAT pixelFormat = {};
    pixelFormat.dwSize = sizeof(DDPIXELFORMAT);

    if (SUCCEEDED(g_pBackBuffer_6FC738->GetPixelFormat(&pixelFormat)))
    {
        if (pixelFormat.dwRGBBitCount == 16)
        {
            switch (pixelFormat.dwRBitMask)
            {
            case 0x7C00u:
                if (pixelFormat.dwGBitMask == 0x3E0 && pixelFormat.dwBBitMask == 31)
                {
                    return 1;
                }
                break;

            case 0xF800u:
                if (pixelFormat.dwGBitMask == 0x7E0 && pixelFormat.dwBBitMask == 31)
                {
                    return 2;
                }
                break;

            case 0x1Fu:
                if (pixelFormat.dwGBitMask == 0x3E0 && pixelFormat.dwBBitMask == 0x7C00)
                {
                    return 3;
                }
                else if (pixelFormat.dwGBitMask == 0x7E0 && pixelFormat.dwBBitMask == 0xF800)
                {
                    return 4;
                }
                break;
            }
        }
    }
    return -1;
}
MGS_FUNC_IMPLEX(0x51D566, Res_Movie_GetBackBufferPixelFormat_51D566, MOVIE_IMPL);

MGS_FUNC_NOT_IMPL(0x528683, int __cdecl(int movieRowLengthBytes, unsigned __int8 backBufferPixelFormat, int flags), Res_movie_create_helper_528683); // TODO
MGS_FUNC_NOT_IMPL(0x52A812, void __cdecl(), Res_movie_create_sound_table_52A812); // TODO



signed int CC Res_movie_create_helper_4562AA(int movieNameHashed)
{
    if (!gMovieData_724A00.sMovie_IO_Ptrs_Inited_dword_724A30)
    {
        File_Ptrs_Init_5289B3(1);
        atexit(Res_movie_helper_free_45654B);
        gMovieData_724A00.sMovie_IO_Ptrs_Inited_dword_724A30 = 1;
    }

    char* pMovieFileName = Res_movie_GetName_4564F5(".", movieNameHashed);
    gMovieData_724A00.field_0_masher_ptr = Masher_constructor_wrapper_5288F8(
        pMovieFileName,
        &gMovieData_724A00.field_4_ddv_header,
        &gMovieData_724A00.field_8_video_header,
        &gMovieData_724A00.field_C_audio_header,
        &gMovieData_724A00.gbMovieError_dword_724A10);

    if (gMovieData_724A00.gbMovieError_dword_724A10)
    {
        if (gMovieData_724A00.field_0_masher_ptr)
        {
            Masher_destructor_wrapper_52895A(gMovieData_724A00.field_0_masher_ptr);
            gMovieData_724A00.field_0_masher_ptr = 0;
        }

        pMovieFileName = Res_movie_GetName_4564F5(".", movieNameHashed);
        gMovieData_724A00.field_0_masher_ptr = Masher_constructor_wrapper_5288F8(
            pMovieFileName,
            &gMovieData_724A00.field_4_ddv_header,
            &gMovieData_724A00.field_8_video_header,
            &gMovieData_724A00.field_C_audio_header,
            &gMovieData_724A00.gbMovieError_dword_724A10);
    }

    if (gMovieData_724A00.gbMovieError_dword_724A10)
    {
        if (gMovieData_724A00.field_0_masher_ptr)
        {
            Masher_destructor_wrapper_52895A(gMovieData_724A00.field_0_masher_ptr);
            gMovieData_724A00.field_0_masher_ptr = 0;
        }
        return 0;
    }

    DWORD w = 0;
    DWORD h = 0;
    Renderer_GetWH_51D50D(&w, &h);
    gMovieData_724A00.field_28_double_height = 1;
    gMovieData_724A00.field_24_double_width = 1;

    if (w >= 2 * gMovieData_724A00.field_8_video_header->field_4_width)
    {
        gMovieData_724A00.field_24_double_width = 2;
        if (h >= 2 * gMovieData_724A00.field_8_video_header->field_8_height)
        {
            gMovieData_724A00.field_28_double_height = 2;
        }
    }

    gMovieData_724A00.gMovieBuffer_724A14 = calloc(
        2
        * gMovieData_724A00.field_28_double_height
        * gMovieData_724A00.field_8_video_header->field_8_height
        * gMovieData_724A00.field_24_double_width
        * gMovieData_724A00.field_8_video_header->field_4_width
        + 0x10000,
        1u);

    if (!gMovieData_724A00.gMovieBuffer_724A14)
    {
        Masher_destructor_wrapper_52895A(gMovieData_724A00.field_0_masher_ptr);
        gMovieData_724A00.field_0_masher_ptr = 0;

        return 0;
    }

    const char backBufferPixelFormat = Res_Movie_GetBackBufferPixelFormat_51D566();
    if (backBufferPixelFormat == -1)
    {
        free(gMovieData_724A00.gMovieBuffer_724A14);
        gMovieData_724A00.gMovieBuffer_724A14 = 0;
        Masher_destructor_wrapper_52895A(gMovieData_724A00.field_0_masher_ptr);
        gMovieData_724A00.field_0_masher_ptr = 0;
        return 0;
    }
    
    Res_movie_create_sound_table_52A812();
    /*
    Res_movie_create_helper_528683(
        2 * gMovieData_724A00.field_24_double_width * gMovieData_724A00.field_8_video_header->field_4_width,
        backBufferPixelFormat,
        flags);
        */

    Sound_Res_Movie_CreateBuffer_523A44(
        ((gMovieData_724A00.field_C_audio_header->field_0_audio_format & 1) != 0) + 1,
        (gMovieData_724A00.field_C_audio_header->field_0_audio_format & 2) != 0 ? 16 : 8,// 16 or 8 bit audio ?
        gMovieData_724A00.field_C_audio_header->field_4_samples_per_second,
        gMovieData_724A00.field_C_audio_header->field_C_single_audio_frame_size,
        gMovieData_724A00.field_C_audio_header->field_10_num_frames_interleave);

    return 1;
}
MGS_FUNC_IMPLEX(0x4562AA, Res_movie_create_helper_4562AA, MOVIE_IMPL);

MGS_FUNC_NOT_IMPL(0x4018E0, void __cdecl(), MarkObjectQueueVoid_4018E0);


int CC Res_movie_write_sound_buffer_4565CA()
{
    Sound_RestoreRelated_523B2C(
        gMovieData_724A00.field_0_masher_ptr,
        Res_movie_masher_read_blocking_52897C,
        Res_movie_masher_sound_read_52899C);

    const int read2FramesRet =
        Res_movie_masher_read_frame_data_528973(gMovieData_724A00.field_0_masher_ptr)
     && Res_movie_masher_read_frame_data_528973(gMovieData_724A00.field_0_masher_ptr);
    
    gMovieData_724A00.field_1C_read_ret = read2FramesRet;

    return read2FramesRet;
}
MGS_FUNC_IMPLEX(0x4565CA, Res_movie_write_sound_buffer_4565CA, MOVIE_IMPL);

void CC Res_movie_update_456588(Actor_Movie* pMovie)
{
    MarkObjectQueueVoid_4018E0();
    System_Init_0_And_1_40A465();

    if (Res_movie_write_sound_buffer_4565CA())
    {
        if (pMovie->dword_7248FC_bIsEnding)
        {
            Timer_30();
        }
        else
        {
            Timer_15();
        }
        pMovie->mBase.mFnUpdate = reinterpret_cast<TActorFunction>(Res_movie_update_helper_456622);
    }
    else
    {
        Actor_DestroyOnNextUpdate_40A3ED(&pMovie->mBase);
    }
}
MGS_FUNC_IMPLEX(0x456588, Res_movie_update_456588, MOVIE_IMPL);

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
    Sound_Masher_Release_Buffer_523A1F();
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
        reinterpret_cast<TActorFunction>(Res_movie_update_456588),
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
