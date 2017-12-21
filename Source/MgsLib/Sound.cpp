#include "stdafx.h"
#include "Sound.hpp"
#include "File.hpp"
#include "Fs.hpp"

// TODO: Fix funcs using SKIP
#ifdef _DEBUG
#define SOUND_IMPL true
#else
// TODO: Extra fix me, in release sound seems to get out of sync and eventually make codec calls etc hang
#define SOUND_IMPL false
#endif

using QWORD = __int64;

#define REDIRECT_SOUND 1

struct StageMusicInfoStruct
{
    unsigned char mSampleSetNumber;
    unsigned char mMusicTrackNumber;
    const char* mStageName;
};
static_assert(sizeof(StageMusicInfoStruct) == 0x8, "StageMusicInfoStruct must be 0x8");


MGS_ARY(REDIRECT_SOUND, 0x77DCA0, IDirectSoundBuffer*, 256, g128_Sound_buffers_dword_77DCA0, {});
MGS_ARY(REDIRECT_SOUND, 0x77D8A0, DWORD, 256, gFxState_dword_77D8A0, {});
MGS_VAR(REDIRECT_SOUND, 0x77E2D0, IDirectSoundBuffer*, gSndBuffer_dword_77E2D0, nullptr);
MGS_VAR(REDIRECT_SOUND, 0x68CE30, int, gMusicWavFile_dword_68CE30, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E0A0, IDirectSoundBuffer*, gSndBuffer_dword_77E0A0, nullptr);
MGS_VAR(REDIRECT_SOUND, 0x77E2D4, DWORD, gSndState_dword_77E2D4, 0);
MGS_VAR(REDIRECT_SOUND, 0x77D884, int, gSoundFxIdx_dword_77D884, 0);

// TODO: Use macro
static DWORD* dword_68D058 = (DWORD*)0x68D058; // part of below array?
static DWORD* dword_68D05C = (DWORD*)0x68D05C; // 21 array?
static DWORD* dword_68D084 = (DWORD*)0x68D084; // part of below array?
static DWORD* dword_68D088 = (DWORD*)0x68D088; // 10 array?

MGS_ARY(REDIRECT_SOUND, 0x77D774, IDirectSoundBuffer*, 64, g64_dword_77D774, {});
MGS_VAR(REDIRECT_SOUND, 0x77E2DC, DWORD, dword_77E2DC, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E2F0, DWORD, dword_77E2F0, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E2D8, DWORD, dword_77E2D8, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E2C0, IDirectSound*, gDSound_dword_77E2C0, nullptr);
MGS_VAR(REDIRECT_SOUND, 0x77E1B0, IDirectSoundBuffer*, gSoundBuffer_dword_77E1B0, nullptr);
MGS_VAR(REDIRECT_SOUND, 0x77E1B4, DWORD, gSoundBufferSize_77E1B4, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E1C4, DWORD, dword_77E1C4, 0);
MGS_VAR(REDIRECT_SOUND, 0x77D87C, DWORD, dword_77D87C, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E1DC, DWORD, gBlockAlign_dword_77E1DC, 0);
MGS_VAR(REDIRECT_SOUND, 0x77D88C, LONG, gSndVolume_dword_77D88C, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E1A8, QWORD, qword_77E1A8, 0);
MGS_VAR(REDIRECT_SOUND, 0x77D874, int, gMusicVolPlaying_dword_77D874, 0);
MGS_VAR(REDIRECT_SOUND, 0x77D898, QWORD, qword_77D898, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E2C4, IDirectSoundBuffer*, gSndSamp1_dword_77E2C4, nullptr);
MGS_VAR(REDIRECT_SOUND, 0x77E2C8, IDirectSoundBuffer*, gSndSamp2_dword_77E2C8, nullptr);
MGS_VAR(REDIRECT_SOUND, 0x77E1D0, DWORD, gSamp1PlayPos_dword_77E1D0, 0);
MGS_VAR(REDIRECT_SOUND, 0x68E318, DWORD, dword_68E318, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E2F8, DWORD, dword_77E2F8, 0);
MGS_VAR(REDIRECT_SOUND, 0x68CE18, DWORD, gSoundVol_dword_68CE18, 0);
MGS_VAR(REDIRECT_SOUND, 0x77D894, DWORD, dword_77D894, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E2F4, DWORD, gMusicTrack_dword_77E2F4, 0);
MGS_ARY(0, 0x68D0B0, StageMusicInfoStruct, 176, gStageInfo_68D0B0, 
{
    { 19, 0, "stage/abst" },
    { 32, 63, "stage/brf" },
    { 19, 0, "stage/camera" },
    { 1, 17, "stage/d01a" },
    { 33, 24, "stage/d04b" },
    { 0, 16, "stage/d11c" },
    { 37, 20, "stage/d12a" },
    { 16, 42, "stage/d16e" },
    { 42, 62, "stage/d18a" },
    { 42, 62, "stage/d18ar" },
    { 19, 0, "stage/demosel" },
    { 19, 0, "stage/preope" },
    { 19, 0, "stage/rank" },
    { 0, 16, "stage/s00a" },
    { 1, 17, "stage/s01a" },
    { 2, 18, "stage/s02a" },
    { 2, 18, "stage/s02b" },
    { 2, 18, "stage/s02c" },
    { 2, 18, "stage/s02d" },
    { 2, 18, "stage/s02e" },
    { 3, 19, "stage/s03a" },
    { 3, 19, "stage/s03ar" },
    { 38, 60, "stage/s03b" },
    { 20, 45, "stage/s03c" },
    { 3, 19, "stage/s03d" },
    { 3, 19, "stage/s03dr" },
    { 3, 19, "stage/s03e" },
    { 3, 19, "stage/s03er" },
    { 4, 24, "stage/s04a" },
    { 33, 24, "stage/s04b" },
    { 33, 24, "stage/s04br" },
    { 33, 24, "stage/s04c" },
    { 34, 23, "stage/s05a" },
    { 6, 26, "stage/s06a" },
    { 7, 27, "stage/s07a" },
    { 36, 28, "stage/s07b" },
    { 36, 28, "stage/s07br" },
    { 7, 27, "stage/s07c" },
    { 7, 27, "stage/s07cr" },
    { 8, 29, "stage/s08a" },
    { 35, 35, "stage/s08b" },
    { 35, 35, "stage/s08br" },
    { 45, 30, "stage/s08c" },
    { 45, 30, "stage/s08cr" },
    { 9, 31, "stage/s09a" },
    { 9, 31, "stage/s09ar" },
    { 37, 32, "stage/s10a" },
    { 37, 32, "stage/s10ar" },
    { 10, 33, "stage/s11a" },
    { 44, 25, "stage/s11b" },
    { 10, 33, "stage/s11c" },
    { 39, 34, "stage/s11d" },
    { 10, 33, "stage/s11e" },
    { 44, 25, "stage/s11g" },
    { 44, 25, "stage/s11h" },
    { 11, 36, "stage/s11i" },
    { 12, 37, "stage/s12a" },
    { 12, 37, "stage/s12b" },
    { 12, 37, "stage/s12c" },
    { 13, 38, "stage/s13a" },
    { 14, 39, "stage/s14e" },
    { 40, 40, "stage/s15a" },
    { 15, 41, "stage/s15b" },
    { 17, 43, "stage/s15c" },
    { 16, 42, "stage/s16a" },
    { 16, 42, "stage/s16b" },
    { 16, 42, "stage/s16c" },
    { 16, 42, "stage/s16d" },
    { 41, 46, "stage/s17a" },
    { 41, 46, "stage/s17ar" },
    { 42, 21, "stage/s18a" },
    { 42, 21, "stage/s18ar" },
    { 43, 44, "stage/s19a" },
    { 43, 44, "stage/s19ar" },
    { 43, 44, "stage/s19b" },
    { 43, 44, "stage/s19br" },
    { 71, 61, "stage/title" },
    { 19, 0, "stagevr/camera" },
    { 69, 64, "stagevr/photo_m1" },
    { 69, 64, "stagevr/photo_m2" },
    { 69, 64, "stagevr/photo_n1" },
    { 69, 64, "stagevr/photo_n2" },
    { 69, 64, "stagevr/photo_n3" },
    { 64, 62, "stagevr/selectvr" },
    { 64, 59, "stagevr/vab_cfr" },
    { 64, 59, "stagevr/vab_clm" },
    { 64, 59, "stagevr/vab_fms" },
    { 64, 59, "stagevr/vab_grn" },
    { 64, 59, "stagevr/vab_nkt" },
    { 64, 59, "stagevr/vab_psg" },
    { 64, 59, "stagevr/vab_scm" },
    { 64, 59, "stagevr/vab_stg" },
    { 64, 59, "stagevr/vab_sud" },
    { 68, 68, "stagevr/vcd_n01" },
    { 68, 68, "stagevr/vcd_n02" },
    { 68, 68, "stagevr/vcd_n03" },
    { 64, 67, "stagevr/vcd_n04" },
    { 65, 66, "stagevr/vcd_n05" },
    { 64, 59, "stagevr/vcd_n06" },
    { 64, 65, "stagevr/vefgh_01" },
    { 64, 65, "stagevr/vefgh_02" },
    { 64, 65, "stagevr/vefgh_03" },
    { 64, 65, "stagevr/vefgh_04" },
    { 64, 65, "stagevr/vefgh_05" },
    { 64, 65, "stagevr/vefgh_06" },
    { 64, 65, "stagevr/vefgh_07" },
    { 64, 65, "stagevr/vefgh_08" },
    { 64, 65, "stagevr/vefgh_09" },
    { 67, 65, "stagevr/vefgh_10" },
    { 64, 59, "stagevr/vijkl_01" },
    { 64, 59, "stagevr/vijkl_02" },
    { 64, 59, "stagevr/vijkl_03" },
    { 64, 59, "stagevr/vijkl_04" },
    { 64, 59, "stagevr/vijkl_05" },
    { 64, 59, "stagevr/vijkl_06" },
    { 64, 65, "stagevr/vijkl_07" },
    { 64, 59, "stagevr/vijkl_08" },
    { 64, 59, "stagevr/vijkl_09" },
    { 64, 59, "stagevr/vijkl_10" },
    { 18, 62, "stagevr/vrsave" },
    { 70, 62, "stagevr/vrtitle" },
    { 64, 59, "stagevr/vr_cfr01" },
    { 64, 59, "stagevr/vr_cfr02" },
    { 64, 59, "stagevr/vr_cfr03" },
    { 64, 59, "stagevr/vr_cfr04" },
    { 64, 59, "stagevr/vr_cfr05" },
    { 64, 59, "stagevr/vr_clm01" },
    { 64, 59, "stagevr/vr_clm02" },
    { 64, 59, "stagevr/vr_clm03" },
    { 64, 59, "stagevr/vr_clm04" },
    { 64, 59, "stagevr/vr_clm05" },
    { 64, 59, "stagevr/vr_fms01" },
    { 64, 59, "stagevr/vr_fms02" },
    { 64, 59, "stagevr/vr_fms03" },
    { 64, 59, "stagevr/vr_fms04" },
    { 64, 59, "stagevr/vr_fms05" },
    { 64, 59, "stagevr/vr_grn01" },
    { 64, 59, "stagevr/vr_grn02" },
    { 64, 59, "stagevr/vr_grn03" },
    { 64, 59, "stagevr/vr_grn04" },
    { 64, 59, "stagevr/vr_grn05" },
    { 64, 59, "stagevr/vr_nkt01" },
    { 64, 59, "stagevr/vr_nkt02" },
    { 64, 59, "stagevr/vr_nkt03" },
    { 64, 59, "stagevr/vr_nkt04" },
    { 64, 59, "stagevr/vr_nkt05" },
    { 64, 59, "stagevr/vr_psg01" },
    { 64, 59, "stagevr/vr_psg02" },
    { 64, 59, "stagevr/vr_psg03" },
    { 64, 59, "stagevr/vr_psg04" },
    { 64, 59, "stagevr/vr_psg05" },
    { 64, 59, "stagevr/vr_scm01" },
    { 64, 59, "stagevr/vr_scm02" },
    { 64, 59, "stagevr/vr_scm03" },
    { 64, 59, "stagevr/vr_scm04" },
    { 64, 59, "stagevr/vr_scm05" },
    { 64, 59, "stagevr/vr_stg01" },
    { 64, 59, "stagevr/vr_stg02" },
    { 64, 59, "stagevr/vr_stg03" },
    { 64, 59, "stagevr/vr_stg04" },
    { 64, 59, "stagevr/vr_stg05" },
    { 64, 59, "stagevr/vr_sud01" },
    { 64, 59, "stagevr/vr_sud02" },
    { 64, 59, "stagevr/vr_sud03" },
    { 64, 59, "stagevr/vr_sud04" },
    { 64, 59, "stagevr/vr_sud05" },
    { 64, 59, "stagevr/vr_sud06" },
    { 64, 59, "stagevr/vr_sud07" },
    { 64, 59, "stagevr/vr_sud08" },
    { 64, 59, "stagevr/vr_sud09" },
    { 64, 59, "stagevr/vr_sud10" },
    { 64, 59, "stagevr/vr_sud11" },
    { 64, 59, "stagevr/vr_sud12" },
    { 64, 59, "stagevr/vr_sud13" },
    { 64, 59, "stagevr/vr_sud14" },
    { 64, 59, "stagevr/vr_sud15" }
});

#include <sstream>

void DumpArray()
{
    std::stringstream ss;
    StageMusicInfoStruct* ar = (StageMusicInfoStruct*)0x68D0B0;
    for (int i = 0; i < 177; i++)
    {
        ss << "{ " << static_cast<unsigned int>(ar->mSampleSetNumber) << ", ";
        ss << static_cast<unsigned int>(ar->mMusicTrackNumber) << ", ";
        ss << "\"" << ar->mStageName << "\" },\n";
        ar++;
    }
    std::string v = ss.str();
    const char* s = v.c_str();
    std::cout << s << std::endl;
}

void SoundCpp_Debug()
{
    //DumpArray();
}

MGS_ARY(REDIRECT_SOUND, 0x68D630, __int16, 1550, unk_68D630, {});
MGS_VAR(REDIRECT_SOUND, 0x68CE34, DWORD, gSampleSet_dword_68CE34, 0);
MGS_PTR(1, 0x68CE38, unsigned char*, byte_68CE38, nullptr); // TODO: Figure out array size and dump it
MGS_PTR(1, 0x68CE39, unsigned char*, byte_68CE39, nullptr); // TODO: Figure out array size and dump it
MGS_VAR(REDIRECT_SOUND, 0x77E2E0, DWORD, dword_77E2E0, 0);
MGS_VAR(REDIRECT_SOUND, 0x68CE2C, DWORD, dword_68CE2C, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E2EC, DWORD, dword_77E2EC, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E2E8, DWORD, dword_77E2E8, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E1C0, DWORD, gMusicVol2_dword_77E1C0, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E1C8, DWORD, dword_77E1C8, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E1BC, DWORD, dword_77E1BC, 0);
MGS_VAR(REDIRECT_SOUND, 0x77D770, DWORD, dword_77D770, 0);
MGS_VAR(REDIRECT_SOUND, 0x77D878, DWORD, dword_77D878, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E1A4, DWORD, dword_77E1A4, 0);
MGS_VAR(REDIRECT_SOUND, 0x77D888, BYTE, byte_77D888, 0);
MGS_ARY(1, 0x68D02C, DWORD, 11, gMusicVolTbl_68D02C, { 0, 64, 70, 74, 77, 80, 83, 85, 87, 89, 90 });
MGS_ARY(1, 0x68D000, DWORD, 11, gSoundVolTbl_68D000, { 0, 76, 82, 86, 89, 92, 94, 96, 98, 99, 100 });
MGS_PTR(1, 0x68CEE4, DWORD*, dword_68CEE4, nullptr);// TODO: Figure out array size and dump it
MGS_VAR(REDIRECT_SOUND, 0x77D880, DWORD, dword_77D880, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E1B8, DWORD, dword_77E1B8, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E1CC, DWORD, dword_77E1CC, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E1D8, DWORD, dword_77E1D8, 0);
MGS_VAR(REDIRECT_SOUND, 0x77D890, DWORD, gSndTime_dword_77D890, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E1D4, DWORD, gOffsetToLock_77E1D4, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E2E4, DWORD, dword_77E2E4, 0);
MGS_PTR(1, 0x68E2D0, float*, byte_68E2D0, nullptr); // XA K0 TODO: Figure out array size and dump it
MGS_VAR(REDIRECT_SOUND, 0x77E300, double, dbl_77E300, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E308, double, dbl_77E308, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E310, double, dbl_77E310, 0);
MGS_VAR(REDIRECT_SOUND, 0x77E318, double, dbl_77E318, 0);
MGS_PTR(1, 0x77E1E0, double*, dbl_77E1E0, nullptr); // array?
MGS_PTR(1, 0x77E1E8, double*, dbl_77E1E8, nullptr); // array?
MGS_PTR(1, 0x68E2C8, DWORD*, dword_68E2C8, nullptr); // array?
MGS_VAR(1, 0x77E2CC, DWORD, dword_77E2CC, 0); // Used outside of sound module

#define SKIP false

MGS_FUNC_IMPLEX(0x0052269C, Sound_Init, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x005227AD, Sound_HexCharToInt, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00522BCE, Sound_CleanUpRelated, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00522466, Sound_CloseWavStopQ, SKIP); // File I/O
MGS_FUNC_IMPLEX(0x00523A44, Sound_CreateBufferQ, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00522601, Sound_CreatePrimarySoundBuffer, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00521982, Sound_CreateSecondarySoundBuffer, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x0052236D, Sound_FadeQ, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x005234EA, Sound_GetSamp1PosQ, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x005224BE, Sound_GetSomeStateQ, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00522A33, Sound_InitFx, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x005227FF, Sound_LoadBufferFromFile, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00522A9C, Sound_LoadFxRelatedQ, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00522B8D, Sound_LoadFxRelatedQ2, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00521A54, Sound_PlayMusic, SKIP);
MGS_FUNC_IMPLEX(0x005231A9, Sound_PlaySample, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x0052307F, Sound_PlaySampleRelated, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00521F82, Sound_PopulateBufferQ, SKIP); // File I/O
MGS_FUNC_IMPLEX(0x00523A1F, Sound_ReleaseBufferQ, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00521A18, Sound_ReleaseSecondaryBuffer, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00523B2C, Sound_RestoreRelatedQ, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00523563, Sound_Samp1Related, SKIP);  // causes raspy codec if redirected
MGS_FUNC_IMPLEX(0x005239B5, Sound_Samp1Related_2, SKIP); // causes raspy codec if redirected
MGS_FUNC_IMPLEX(0x005226EB, Sound_ShutDown, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00523232, Sound_Start2SamplesQ, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00523466, Sound_Stop2Samples, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x0052313B, Sound_StopSample, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00521898, Sound_TableUnknown1, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x0052255B, Sound_SetSoundMusicVolume, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x005224C8, Sound_SetSoundVolume, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00522CB2, Sound_PlayEffect, SKIP);
MGS_FUNC_IMPLEX(0x00523E12, Sound_Unknown4, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00523CF3, Sound_Unknown5, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00523CB9, Sound_Unknown6, SOUND_IMPL);
MGS_FUNC_IMPLEX(0x00646660, Sound_Play, SKIP); // calls to broken funcs
MGS_FUNC_IMPLEX(0x0044FF6C, Sound_jPlay, SKIP); // calls to broken funcs


#pragma comment(lib, "Winmm.lib") // timegettime()

void SoundCpp_ForceLink()
{
    // Make sure the linker won't throw this module away
}

MGS_FUNC_NOT_IMPL(0x005530A8, HRESULT __stdcall(LPGUID, LPDIRECTSOUND*, LPUNKNOWN), MgsDirectSoundCreate);

// 0x0052269C
signed int __cdecl Sound_Init(HWND hwnd)
{
    HRESULT hr = DirectSoundCreate(0, &gDSound_dword_77E2C0, 0);
    if (FAILED(hr))
    {
        return 0;
    }

    hr = gDSound_dword_77E2C0->SetCooperativeLevel(hwnd, DSSCL_EXCLUSIVE);
    if (FAILED(hr))
    {
        return 0;
    }

    if (!Sound_CreatePrimarySoundBuffer())
    {
        return 0;
    }
   
    Sound_InitFx();
    return Sound_CreateSecondarySoundBuffer();
}

// 0x005227AD
char __cdecl Sound_HexCharToInt(char value)
{
    char ret;
    if (value < '0' || value > '9')
    {
        if (toupper(value) < 'A' || toupper(value) > 'F')
        {
            ret = 0;
        }
        else
        {
            ret = static_cast<char>(toupper(value)) - '7';
        }
    }
    else
    {
        ret = value - '0';
    }
    return ret;
}

// 0x00522BCE
signed int __cdecl Sound_CleanUpRelated()
{
    for (int i = 0; i < 64; ++i)
    {
        if (g64_dword_77D774[i])
        {
            g64_dword_77D774[i]->Stop();
            g64_dword_77D774[i]->Release();
            g64_dword_77D774[i] = 0;
        }
    }

    for (int i = 128; i < 256; ++i)
    {
        if (g128_Sound_buffers_dword_77DCA0[i])
        {
            g128_Sound_buffers_dword_77DCA0[i]->Stop();
            g128_Sound_buffers_dword_77DCA0[i]->Release();
            g128_Sound_buffers_dword_77DCA0[i] = 0;
        }
    }
    dword_77E2DC = 0;
    Sound_CloseWavStopQ();
    gSoundFxIdx_dword_77D884 = -1;
    return 1;
}

// 0x00522466
int __cdecl Sound_CloseWavStopQ()
{
    int result = 0;

    if (!dword_77E2DC)
    {
        dword_77E2F0 = 0;
    }

    if (gSndBuffer_dword_77E2D0)
    {
        result = gSndBuffer_dword_77E2D0->Stop();
    }

    if (gMusicWavFile_dword_68CE30 != -1)
    {
        result = close(gMusicWavFile_dword_68CE30);
    }

    gMusicWavFile_dword_68CE30 = -1;
    gSndState_dword_77E2D4 = 0;
    dword_77E2D8 = 0;
    return result;
}

// 0x00523A44
signed int __cdecl Sound_CreateBufferQ(int numChannels, signed int bitsPerSample, int samplesPerSecond, int a4, int a5)
{
    DSBUFFERDESC bufferDesc;
    WAVEFORMATEX waveFormat;

    int blockAlign = bitsPerSample / 8 * numChannels;
    gSoundBufferSize_77E1B4 = (a5 + 4) * blockAlign * a4;

    if (gDSound_dword_77E2C0)
    {
        waveFormat.wFormatTag = 1;
        waveFormat.nChannels = static_cast<WORD>(numChannels);
        waveFormat.nSamplesPerSec = samplesPerSecond;
        waveFormat.nAvgBytesPerSec = blockAlign * samplesPerSecond;
        waveFormat.nBlockAlign = static_cast<WORD>(blockAlign);
        waveFormat.wBitsPerSample = static_cast<WORD>(bitsPerSample);
        waveFormat.cbSize = 0;
        memset(&bufferDesc, 0, 36u);
        bufferDesc.dwSize = 36;
        bufferDesc.dwFlags = 0x100C8;
        bufferDesc.dwBufferBytes = gSoundBufferSize_77E1B4;
        bufferDesc.lpwfxFormat = &waveFormat;
        gDSound_dword_77E2C0->CreateSoundBuffer(&bufferDesc, &gSndBuffer_dword_77E0A0, 0);
    }

    dword_77E1C4 = a5;
    dword_77D87C = a4;
    gBlockAlign_dword_77E1DC = blockAlign;

    if (gSndBuffer_dword_77E0A0)
    {
        gSndBuffer_dword_77E0A0->SetCurrentPosition(0);
    }
    return 1;
}

// 0x00522601
bool __cdecl Sound_CreatePrimarySoundBuffer()
{
    bool result;
    DSBUFFERDESC bufferDesc;
    WAVEFORMATEX waveFormat;

    waveFormat.wFormatTag = 1;
    waveFormat.nChannels = 2;
    waveFormat.nSamplesPerSec = 22050;
    waveFormat.nAvgBytesPerSec = 88200;
    waveFormat.nBlockAlign = 4;
    waveFormat.wBitsPerSample = 16;
    waveFormat.cbSize = 0;

    memset(&bufferDesc, 0, 36u);
    bufferDesc.dwSize = 36;
    bufferDesc.dwFlags = 1;
    bufferDesc.lpwfxFormat = 0;
    bufferDesc.dwBufferBytes = 0;

    if (gDSound_dword_77E2C0->CreateSoundBuffer(&bufferDesc, &gSoundBuffer_dword_77E1B0, 0))
    {
        result = 0;
    }
    else
    {
        result = gSoundBuffer_dword_77E1B0->SetFormat(&waveFormat) == 0;
    }
    return result;
}

// 0x00521982
signed int __cdecl Sound_CreateSecondarySoundBuffer()
{
    signed int result;
    DSBUFFERDESC bufferDesc;
    WAVEFORMATEX waveFormat;

    waveFormat.wFormatTag = 1;
    waveFormat.nChannels = 2;
    waveFormat.nSamplesPerSec = 22050;
    waveFormat.nAvgBytesPerSec = 44100;
    waveFormat.nBlockAlign = 2;
    waveFormat.wBitsPerSample = 8;
    waveFormat.cbSize = 0;
    memset(&bufferDesc, 0, 36u);
    bufferDesc.dwSize = 36;
    bufferDesc.dwFlags = 0x10088;
    bufferDesc.dwBufferBytes = 176400;
    bufferDesc.lpwfxFormat = &waveFormat;
    if (gDSound_dword_77E2C0->CreateSoundBuffer(&bufferDesc, &gSndBuffer_dword_77E2D0, 0))
    {
        result = 0;
    }
    else
    {
        gSndBuffer_dword_77E2D0->SetCurrentPosition(0);
        result = 1;
    }
    return result;
}

// 0x0052236D
__int64 __cdecl Sound_FadeQ(int a1)
{
    __int64 result = 0; // Hack can sometimes take un-inited path?
    LARGE_INTEGER Frequency;
    __int64 freq;
    LARGE_INTEGER PerformanceCount;
    __int64 v5 = 0;

    if (gSndBuffer_dword_77E2D0)
    {
        if (gSndState_dword_77E2D4)
        {
            gSndBuffer_dword_77E2D0->GetVolume(&gSndVolume_dword_77D88C);
            QueryPerformanceFrequency(&Frequency);
            freq = Frequency.QuadPart;
            QueryPerformanceCounter(&PerformanceCount);
            qword_77E1A8 = PerformanceCount.QuadPart;
            gMusicVolPlaying_dword_77D874 = -10000;
            if (a1)
            {
                switch (a1)
                {
                case 1:
                    v5 = 750i64;
                    break;
                case 2:
                    v5 = 2000i64;
                    break;
                case 3:
                    v5 = 3000i64;
                    break;
                }
            }
            else
            {
                v5 = 250i64;
            }
            result = freq * v5 / 1000;
            qword_77D898 = freq * v5 / 1000;
            dword_77E2D8 = 1;
        }
    }
    return result;
}

// 0x005234EA
bool __cdecl Sound_GetSamp1PosQ()
{
    bool result; // eax@2

    if (dword_68E318 == -1)
    {
        if (dword_77E2CC)
        {
            result = 0;
        }
        else if (gSndSamp1_dword_77E2C4)
        {
            gSndSamp1_dword_77E2C4->GetCurrentPosition(&gSamp1PlayPos_dword_77E1D0, 0);
            if (gSamp1PlayPos_dword_77E1D0 <= dword_77E2F8)
            {
                gSamp1PlayPos_dword_77E1D0 += 176400;
            }
            result = dword_77E2F8 + 28672 <= gSamp1PlayPos_dword_77E1D0;
        }
        else
        {
            result = 0;
        }
    }
    else
    {
        result = 1;
    }
    return result;
}

// 0x005224BE
int __cdecl Sound_GetSomeStateQ()
{
   return gSndState_dword_77E2D4;
}


// 0x00522A33
int __cdecl Sound_InitFx()
{
    int result;
    char fxFileName[256] = {};
    for (int fxNum = 0; fxNum < 128; ++fxNum)
    {
        sprintf(fxFileName, "%s0x%02x.wav", "efx/", fxNum);
        Sound_LoadBufferFromFile(fxFileName);
        gFxState_dword_77D8A0[fxNum] = 0;
        result = fxNum + 1;
    }
    return result;
}

// 0x005227FF
signed int __cdecl Sound_LoadBufferFromFile(const char *fileName)
{
    signed int result;
    size_t v3;
    char v4[20] = {};
    DWORD sizeToRead;
    DSBUFFERDESC bufferDesc;
    void* v9;
    DWORD v10;
    size_t fileNameLength; 
    WAVEFORMATEX waveFormat;
    void *soundBuffer;

    gSoundFxIdx_dword_77D884 = -1;
    fileNameLength = strlen(fileName) - 6;
    const char v1 = 16 * Sound_HexCharToInt(fileName[fileNameLength]);
    const unsigned __int8 idx = Sound_HexCharToInt(fileName[fileNameLength + 1]) + v1;
    memset(&bufferDesc, 0, 36u);
    bufferDesc.dwSize = 36;
    bufferDesc.dwFlags = 0x100C8;
    bufferDesc.lpwfxFormat = &waveFormat;
    FILE* File = File_LoadDirFile_51EE8F(fileName, 0);
    if (File
        && (File_NormalRead_51F0F5(File, v4, 20u), File_NormalRead_51F0F5(File, &waveFormat, 18u) == 18)
        && (File_NormalRead_51F0F5(File, v4, 2u), File_NormalRead_51F0F5(File, &sizeToRead, 4u) == 4))
    {
        bufferDesc.dwBufferBytes = sizeToRead;
        if (gDSound_dword_77E2C0)
        {
            if (gDSound_dword_77E2C0->CreateSoundBuffer(
                &bufferDesc,
                &g128_Sound_buffers_dword_77DCA0[idx],
                0))
            {
                return 0;
            }
            g128_Sound_buffers_dword_77DCA0[idx]->SetCurrentPosition(0);
            if (g128_Sound_buffers_dword_77DCA0[idx]->Lock(
                0,
                sizeToRead,
                &soundBuffer,
                &sizeToRead,
                &v9,
                &v10,
                0) == DSERR_BUFFERLOST)
            {
                g128_Sound_buffers_dword_77DCA0[idx]->Restore();
                g128_Sound_buffers_dword_77DCA0[idx]->Lock(
                    0,
                    sizeToRead,
                    &soundBuffer,
                    &sizeToRead,
                    &v9,
                    &v10,
                    0);
            }
            v3 = File_NormalRead_51F0F5(File, soundBuffer, sizeToRead);
            if (v3 != sizeToRead)
            {
                return 0;
            }
            g128_Sound_buffers_dword_77DCA0[idx]->Unlock(
                soundBuffer,
                sizeToRead,
                v9,
                v10);
        }
        File_Close_51F183(File);
        result = 1;
    }
    else
    {
        File_Close_51F183(File);
        result = 0;
    }
    return result;
}

// 0x00522A9C
void __cdecl Sound_LoadFxRelatedQ(const char *Str1)
{
    int sampleSet = 255;
    gMusicTrack_dword_77E2F4 = 0;
    for (int i = 0; i < 176; ++i)
    {
        const StageMusicInfoStruct& ptr = gStageInfo_68D0B0[i];
        if (!strcmp(Str1, ptr.mStageName))
        {
            gMusicTrack_dword_77E2F4 = ptr.mMusicTrackNumber;
            sampleSet = ptr.mSampleSetNumber;
            break;
        }
    }
 
    // 82 is biggest samples in a set, 1550 is total number of sample set samples, excluding main/ones in root dir
    for (int i = 0; i < 1550; ++i) 
    {
        const unsigned __int16 soundNum = unk_68D630[i]; // used as 2 bytes, casts are important!
        if (((soundNum >> 8) & 127) == sampleSet)
        {
            char soundFileName[512] = {};
            sprintf(soundFileName, "%ssample%02x/0x%02x.wav", "efx/", sampleSet, (unsigned char)soundNum);
            Sound_LoadBufferFromFile(soundFileName);
            gFxState_dword_77D8A0[(unsigned char)soundNum] = 0; // Seems to be a dead array?
        }
    }
    gSampleSet_dword_68CE34 = sampleSet;
}

// 0x00522B8D
void __cdecl Sound_LoadFxRelatedQ2(const char *Str1)
{
    for (int i = 0; i < 64; ++i)
    {
        g64_dword_77D774[i] = 0;
    }
    dword_77D894 = 0;
    Sound_LoadFxRelatedQ(Str1);
}

// 0x00521A54
signed int __cdecl Sound_PlayMusic(unsigned int flags)
{
    DWORD v2;
    bool v3;
    bool v4;
    LARGE_INTEGER Frequency;
    char soundFileName[256] = {};
    __int64 freq;
    unsigned __int8 sndNumber;
    void* v9;
    void* v10;
    DWORD v11;
    DWORD nNumberOfBytesToRead;
    LARGE_INTEGER PerformanceCount;

    if ((flags & 0xFFFF00) == 0xFFFF00)
    {
        if (flags >= 6 && flags <= 13 || flags == 255)
        {
            if (flags == 255)
            {
                Sound_CloseWavStopQ();
            }
            else
            {
                Sound_FadeQ((flags - 6) % 4);
                v4 = flags >= 6 && flags <= 9;
                dword_77E2DC = v4;
            }
            if (!gSndBuffer_dword_77E2D0)
                Sound_CloseWavStopQ();
            return 1;
        }
        if (flags < 3 || flags > 5)
            return 1;
        flags = dword_77E2F0;
    }
    if (flags > 8 || flags < 1)
        return 1;
    if (flags == dword_77E2F0)
    {
        if (!dword_77E2DC)
            return 1;
    }
    else
    {
        dword_77E2DC = 0;
        Sound_CloseWavStopQ();
    }
    dword_77E2DC = 0;
    dword_77E2F0 = flags;
    if (gMusicTrack_dword_77E2F4 == 59 || gMusicTrack_dword_77E2F4 >= 62)
    {
        switch (flags)
        {
        case 1:
            sndNumber = byte_68CE38[2 * gMusicTrack_dword_77E2F4];
            break;
        case 2:
            sndNumber = byte_68CE39[2 * gMusicTrack_dword_77E2F4];
            break;
        case 3:
            sndNumber = 33;
            break;
        case 4:
            sndNumber = 32;
            break;
        case 5:
            sndNumber = 30;
            break;
        case 6:
            sndNumber = 31;
            break;
        case 7:
            sndNumber = 27;
            break;
        default:
            sndNumber = byte_68CE38[2 * gMusicTrack_dword_77E2F4];
            break;
        }
        goto LABEL_74;
    }
    if (flags == 1)
    {
        sndNumber = byte_68CE38[2 * gMusicTrack_dword_77E2F4];
        goto LABEL_74;
    }
    if (flags == 2)
    {
        sndNumber = 6;
        goto LABEL_74;
    }
    if (flags != 3)
    {
        if (flags != 4)
            return 1;
        if (gMusicTrack_dword_77E2F4 != 29 && gMusicTrack_dword_77E2F4 != 45)
            sndNumber = byte_68CE39[2 * gMusicTrack_dword_77E2F4];
        else
            sndNumber = byte_68CE38[2 * gMusicTrack_dword_77E2F4];
        goto LABEL_74;
    }
    if (gMusicTrack_dword_77E2F4 > 36)
    {
        if (gMusicTrack_dword_77E2F4 != 39 && gMusicTrack_dword_77E2F4 != 42 && (gMusicTrack_dword_77E2F4 <= 43 || gMusicTrack_dword_77E2F4 > 46))
            return 1;
    }
    else if (gMusicTrack_dword_77E2F4 < 35 && gMusicTrack_dword_77E2F4 != 18)
    {
        if (gMusicTrack_dword_77E2F4 == 19)
        {
            sndNumber = 13;
            goto LABEL_74;
        }
        if (gMusicTrack_dword_77E2F4 <= 23 || gMusicTrack_dword_77E2F4 > 25 && (gMusicTrack_dword_77E2F4 <= 26 || gMusicTrack_dword_77E2F4 > 32))
            return 1;
    }
    sndNumber = byte_68CE39[2 * gMusicTrack_dword_77E2F4];
LABEL_74:
    v3 = sndNumber == 20 || sndNumber == 32 || sndNumber == 33;
    dword_77E2E0 = v3;
    dword_68CE2C = static_cast<DWORD>(-1);
    if (!sndNumber)
        return 1;
    sprintf(soundFileName, "%s0x%02x.wav", "mdx/", sndNumber);
    gMusicWavFile_dword_68CE30 = open(soundFileName, 0x8000);
    if (gMusicWavFile_dword_68CE30 == -1)
        return 0;
    lseek(gMusicWavFile_dword_68CE30, 40, 0);
    if (read(gMusicWavFile_dword_68CE30, &dword_77E2EC, 4u) != 4)
        return 0;
    if (gSndBuffer_dword_77E2D0)
    {
        if (gSndBuffer_dword_77E2D0->Lock(
            0,
            88200,
            &v10,
            &nNumberOfBytesToRead,
            &v9,
            &v11,
            0) == DSERR_BUFFERLOST)
        {
            gSndBuffer_dword_77E2D0->Restore();
            gSndBuffer_dword_77E2D0->Lock(0, 88200, &v10, &nNumberOfBytesToRead, &v9, &v11, 0);
        }
        v2 = read(gMusicWavFile_dword_68CE30, v10, nNumberOfBytesToRead);
        if (v2 != nNumberOfBytesToRead)
        {
            close(gMusicWavFile_dword_68CE30);
            gSndBuffer_dword_77E2D0->Unlock(v10, nNumberOfBytesToRead, v9, v11);
            return 0;
        }
        gSndBuffer_dword_77E2D0->Unlock(v10, nNumberOfBytesToRead, v9, v11);
    }
    dword_77E2E8 = 88200;
    QueryPerformanceFrequency(&Frequency);
    freq = Frequency.QuadPart;
    QueryPerformanceCounter(&PerformanceCount);
    qword_77E1A8 = PerformanceCount.QuadPart;
    gMusicVolPlaying_dword_77D874 = gMusicVol2_dword_77E1C0;
    if (dword_77E2E0)
    {
        gSndVolume_dword_77D88C = gMusicVol2_dword_77E1C0;
        qword_77D898 = 0i64;
    }
    else
    {
        gSndVolume_dword_77D88C = -10000;
        qword_77D898 = 500 * freq / 1000;
    }
    if (gSndBuffer_dword_77E2D0)
    {
        gSndBuffer_dword_77E2D0->SetCurrentPosition( 0);
        gSndBuffer_dword_77E2D0->SetVolume(gSndVolume_dword_77D88C);
        gSndBuffer_dword_77E2D0->Play(0, 0, 1);
    }
    dword_77E2D8 = 0;
    gSndState_dword_77E2D4 = 1;
    dword_77E2EC -= 88200;
    return 1;
}

// 0x005231A9
int __cdecl Sound_PlaySample()
{
    int result = 0;

    if (gSoundFxIdx_dword_77D884 != -1)
    {
        result = gSoundFxIdx_dword_77D884;
        if (g128_Sound_buffers_dword_77DCA0[gSoundFxIdx_dword_77D884])
        {
            result = g128_Sound_buffers_dword_77DCA0[gSoundFxIdx_dword_77D884]->Play(
                0,
                0,
                DSBPLAY_LOOPING);
        }
    }

    if (gSndBuffer_dword_77E2D0)
    {
        if (gSndState_dword_77E2D4 == 1)
        {
            result = gSndBuffer_dword_77E2D0->Play(0, 0, DSBPLAY_LOOPING);
        }
    }
    
    if (gSndBuffer_dword_77E0A0)
    {
        result = gSndBuffer_dword_77E0A0->Play(0, 0, DSBPLAY_LOOPING);
    }

    return result;
}

// 0x0052307F
void __cdecl Sound_PlaySampleRelated(IDirectSoundBuffer* pSoundBuffer, int a2, int a3, signed int a4)
{
    int pan;
    int vol;

    if (pSoundBuffer)
    {
        if (a2 < 0)
        {
            pan = Sound_TableUnknown1(100 * (a2 + 32) / 32, -10000, 0);
        }
        else
        {
            pan = -Sound_TableUnknown1(100 * (32 - a2) / 32, -10000, 0);
        }
        if (a4 > -2500)
        {
            vol = Sound_TableUnknown1(100 * a3 / 63, -2500, a4);
        }
        else
        {
            vol = a4;
        }
        pSoundBuffer->SetPan(pan);
        pSoundBuffer->SetVolume(vol);
    }
}

// 0x00521F82
void __cdecl Sound_PopulateBufferQ()
{
    DWORD v0;
    DWORD v1;
    LARGE_INTEGER PerformanceCount;
    __int64 v3;
    void* v4;
    void* v5;
    DWORD v6;
    DWORD nNumberOfBytesToRead;
    DWORD a1;
    int v9;
    LONG lDistanceToMove;
    BYTE buffer[68] = {};
    DWORD v12;
    int v13;

    if (gSndBuffer_dword_77E2D0)
    {
        if (gSndState_dword_77E2D4)
        {
            QueryPerformanceCounter(&PerformanceCount);
            v3 = PerformanceCount.QuadPart;
            if (PerformanceCount.QuadPart - qword_77E1A8 <= qword_77D898)
            {
                a1 = static_cast<DWORD>(100 * (v3 - qword_77E1A8) / qword_77D898);
                v13 = Sound_TableUnknown1(a1, gSndVolume_dword_77D88C, gMusicVolPlaying_dword_77D874);
                gSndBuffer_dword_77E2D0->SetVolume(v13);
            }
            else
            {
                gSndBuffer_dword_77E2D0->SetVolume(gMusicVolPlaying_dword_77D874);
                if (dword_77E2D8)
                {
                    Sound_CloseWavStopQ();
                }
            }

            v12 = 0x2000;

            if (dword_77E2EC < 0x2000)
            {
                v12 = dword_77E2EC;
            }

            gSndBuffer_dword_77E2D0->GetCurrentPosition(&dword_77E2E4, 0);

            if (dword_77E2E4 <= dword_77E2E8)
            {
                dword_77E2E4 += 176400;
            }

            if (dword_68CE2C == -1 || dword_77E2E4 < dword_77E2E8 || dword_77E2E4 >= dword_68CE2C)
            {
                if (v12 + dword_77E2E8 <= dword_77E2E4)
                {
                    if (gSndBuffer_dword_77E2D0->Lock(
                        dword_77E2E8,
                        v12,
                        &v5,
                        &nNumberOfBytesToRead,
                        &v4,
                        &v6,
                        0) == DSERR_BUFFERLOST)
                    {
                        gSndBuffer_dword_77E2D0->Restore();
                        gSndBuffer_dword_77E2D0->Lock(
                            dword_77E2E8,
                            v12,
                            &v5,
                            &nNumberOfBytesToRead,
                            &v4,
                            &v6,
                            0);
                    }
                    
                    v0 = read(gMusicWavFile_dword_68CE30, v5, nNumberOfBytesToRead);
                    
                    if (v0 != nNumberOfBytesToRead)
                    {
                        close(gMusicWavFile_dword_68CE30);
                        gMusicWavFile_dword_68CE30 = -1;
                        gSndState_dword_77E2D4 = 0;
                    }

                    if (nNumberOfBytesToRead < 0x2000)
                    {
                        v1 = read(gMusicWavFile_dword_68CE30, v4, v6);
                        if (v1 != v6)
                        {
                            close(gMusicWavFile_dword_68CE30);
                            gMusicWavFile_dword_68CE30 = -1;
                            gSndState_dword_77E2D4 = 0;
                        }
                    }

                    gSndBuffer_dword_77E2D0->Unlock(v5, nNumberOfBytesToRead, v4, v6);
                    dword_77E2E8 += v12;

                    if (dword_77E2E8 >= 0x2B110)
                    {
                        dword_77E2E8 -= 176400;
                    }

                    if (dword_77E2EC == v12)
                    {
                        dword_77E2EC = 0;
                        v9 = 1;
                    }
                    else
                    {
                        dword_77E2EC -= 0x2000;
                        v9 = 0;
                    }

                    if (v9)
                    {
                        lDistanceToMove = 0;
                        if (read(gMusicWavFile_dword_68CE30, buffer, 68u) == 68 && read(gMusicWavFile_dword_68CE30, buffer, 12u) == 12)
                        {
                            buffer[11] = 0;
                            lDistanceToMove = 441
                                * (10
                                * (10 * (10 * (10 * (buffer[5] - 48) + buffer[6] - 48) + buffer[8] - 48) + buffer[9] - 48)
                                + buffer[10]
                                - 48)
                                / 10;
                        }
                        lseek(gMusicWavFile_dword_68CE30, 40, 0);
                        read(gMusicWavFile_dword_68CE30, &dword_77E2EC, 4u);
                        lseek(gMusicWavFile_dword_68CE30, lDistanceToMove, 1u);
                        dword_77E2EC -= lDistanceToMove;

                        if (dword_77E2E0)
                        {
                            dword_68CE2C = dword_77E2E8;
                        }
                    }
                }
            }
            else
            {
                Sound_CloseWavStopQ();
            }
        }
    }
}

// 0x00523A1F
void __cdecl Sound_ReleaseBufferQ()
{
    if (gSndBuffer_dword_77E0A0)
    {
        gSndBuffer_dword_77E0A0->Release();
        gSndBuffer_dword_77E0A0 = 0;
    }
}

// 0x00521A18
void __cdecl Sound_ReleaseSecondaryBuffer()
{
    dword_77E2F0 = 0;
    if (gSndBuffer_dword_77E2D0)
    {
        gSndBuffer_dword_77E2D0->Stop();
        gSndBuffer_dword_77E2D0->Release();
        gSndBuffer_dword_77E2D0 = 0;
    }
}

// 0x00523B2C
signed int __cdecl Sound_RestoreRelatedQ(int a1, int(__cdecl *fnRead)(DWORD), BYTE*(__cdecl *a3)(DWORD))
{
    BYTE* v4;
    void* v6 = nullptr; // TODO: Some of these vars could be used without being assigned, is this correct?
    BYTE* v7 = nullptr; 
    DWORD v8 = 0;
    DWORD v9 = 0;
    BYTE *pDst;
    size_t Size;

    Size = gBlockAlign_dword_77E1DC * dword_77D87C;
    pDst = 0;
    if (gSndBuffer_dword_77E0A0)
    {
        if (gSndBuffer_dword_77E0A0->Lock(0, dword_77E1C4 * Size, (LPVOID*)&v7, &v9, &v6, &v8, 0) == DSERR_BUFFERLOST)
        {
            gSndBuffer_dword_77E0A0->Restore();
            gSndBuffer_dword_77E0A0->Lock(0, dword_77E1C4 * Size, (LPVOID*)&v7, &v9, &v6, &v8, 0);
        }
        pDst = v7;
    }
    for (unsigned int i = 0; i < dword_77E1C4; ++i)
    {
        if (!fnRead(a1))
        {
            if (gSndBuffer_dword_77E0A0)
            {
                gSndBuffer_dword_77E0A0->Unlock(v7, v9, v6, v8);
            }
            return 0;
        }
        
        v4 = a3(a1);

        if (v4)
        {
            if (gSndBuffer_dword_77E0A0)
            {
                if (pDst)
                {
                    memcpy(pDst, v4, Size);
                    pDst = pDst + Size;
                }
            }
        }
    }

    if (gSndBuffer_dword_77E0A0)
    {
        gSndBuffer_dword_77E0A0->Unlock(v7, v9, v6, v8);
    }

    gOffsetToLock_77E1D4 = dword_77E1C4 * Size;
    dword_77D880 = dword_77E1C4 * Size;
    dword_77E1CC = 0;
    dword_77E1B8 = 0;
    gSndTime_dword_77D890 = timeGetTime();
    dword_77E1D8 = 0;
    return 1;
}

// 0x00523563
signed int __cdecl Sound_Samp1Related(char *a1, unsigned int a2, IDirectSoundBuffer *snd, int a4)
{
    signed int result;
    signed int v5;
    double v6 = 0.0; // hack sometimes used un-inited
    size_t v7;
    double v8 = 0.0; // hack sometimes used un-inited
    unsigned int Size;
    signed int i;
    signed int j;
    WORD *v12;
    int v13;
    WORD *Dst;
    DWORD v15;
    DWORD v16;
    int v17;
    int v18;
    unsigned int v19;
    int v20;
    int v21;
    WORD *v22;
    char *v23;

    if (!a1)
    {
        a2 = 4096;
    }

    if (a2 & 0xF)
    {
        result = 0;
    }
    else
    {
        Size = (7 * a2) >> 1;
        if (snd)
        {
            snd->GetCurrentPosition(&gSamp1PlayPos_dword_77E1D0, 0);
            
            if (gSamp1PlayPos_dword_77E1D0 <= dword_77E2F8)
            {
                gSamp1PlayPos_dword_77E1D0 += 176400;
            }

            if (dword_68E318 == -1)
            {
                if (Size + dword_77E2F8 <= gSamp1PlayPos_dword_77E1D0)
                {
                    if (snd->Lock(dword_77E2F8, Size, (LPVOID*)&Dst, &v16, (LPVOID*)&v12, &v15, 0) == DSERR_BUFFERLOST)
                    {
                        snd->Restore();
                        snd->Lock(dword_77E2F8, Size, (LPVOID*)&Dst, &v16, (LPVOID*)&v12, &v15, 0);
                    }

                    if (a1)
                    {
                        if (a4)
                        {
                            v8 = dbl_77E300;
                            v6 = dbl_77E308;
                        }
                        else
                        {
                            v8 = dbl_77E310;
                            v6 = dbl_77E318;
                        }
                        if (a4)
                            dword_77E2F8 += Size;
                        if (dword_77E2F8 >= 0x2B110)
                            dword_77E2F8 -= 176400;
                        v22 = Dst;
                        v7 = v16;
                        v19 = 0;
                        v5 = 0;
                        while (v19 < a2)
                        {
                            v20 = *a1;
                            v23 = a1 + 1;
                            v13 = v20 & 0xF;
                            v20 >>= 4;
                            v21 = *v23;
                            a1 = v23 + 1;
                            if (v21 == 7)
                                v5 = 1;
                            if (!v13)
                                memset(a1, 0, 0xEu);
                            if (v5)
                            {
                                v13 = 12;
                                v20 = 0;
                                v21 = 2;
                                memset(a1, 0, 0xEu);
                                break;
                            }
                            for (i = 0; i < 28; i += 2)
                            {
                                v17 = *a1;
                                a1 = a1 + 1;
                                v18 = (v17 & 0xF) << 12;
                                if (((v17 & 0xF) << 12) & 0x8000)
                                    v18 |= 0xFFFF0000;
                                dbl_77E1E0[i] = (v18 >> v13);
                                v18 = (v17 & 0xF0) << 8;
                                if (((v17 & 0xF0) << 8) & 0x8000)
                                    v18 |= 0xFFFF0000;
                                dbl_77E1E8[i] = (v18 >> v13);
                            }
                            for (j = 0; j < 28; ++j)        // xa 28?
                            {
                                dbl_77E1E0[j] = v8 * *&dword_68E2C8[4 * v20] + dbl_77E1E0[j] + v6 * byte_68E2D0[2 * v20];
                                v6 = v8;
                                v8 = dbl_77E1E0[j];
                                if (!v7)
                                {
                                    v22 = v12;
                                    v7 = v15;
                                }
                                *v22 = static_cast<WORD>(dbl_77E1E0[j]);
                                ++v22;
                                v7 -= 2;
                            }
                            v19 += 16;
                        }
                    }
                    else
                    {
                        if (Size == v16)
                        {
                            memset(Dst, 0, Size);
                        }
                        else
                        {
                            memset(Dst, 0, v16);
                            memset(v12, 0, v15);
                        }

                        if (a4)
                        {
                            dword_68E318 = gSamp1PlayPos_dword_77E1D0;
                        }
                    }
                    snd->Unlock(Dst, v16, v12, v15);
                    if (!byte_77D888)
                    {
                        snd->SetVolume(gSoundVol_dword_68CE18);
                        snd->Play(0, 0, 1);
                        if (a4)
                        {
                            byte_77D888 = 1;
                        }
                    }
                    if (a4)
                    {
                        dbl_77E300 = v8;
                        dbl_77E308 = v6;
                    }
                    else
                    {
                        dbl_77E310 = v8;
                        dbl_77E318 = v6;
                    }
                    result = 1;
                }
                else
                {
                    result = 0;
                }
            }
            else
            {
                if (gSamp1PlayPos_dword_77E1D0 >= dword_77E2F8 && gSamp1PlayPos_dword_77E1D0 < dword_68E318)
                {
                    dword_77E2CC = 1;
                    dword_68E318 = static_cast<DWORD>(-1);
                }
                result = 1;
            }
        }
        else
        {
            result = 0;
        }
    }
    return result;
}

// 0x005239B5
signed int __cdecl Sound_Samp1Related_2(char *a1, unsigned int a2)
{
    signed int result;

    if (!a1)
        a2 = 0;
    if (dword_77E1A4)
    {
        Sound_Samp1Related(a1, a2 >> 1, gSndSamp1_dword_77E2C4, 0);
        result = Sound_Samp1Related(&a1[a2 >> 1], a2 >> 1, gSndSamp2_dword_77E2C8, 1);
    }
    else
    {
        result = Sound_Samp1Related(a1, a2, gSndSamp1_dword_77E2C4, 1);
    }
    return result;
}

// 0x005226EB
void __cdecl Sound_ShutDown()
{
    Sound_ReleaseSecondaryBuffer();

    for (int i = 0; i < 128; ++i)
    {
        if (g128_Sound_buffers_dword_77DCA0[i])
        {
            g128_Sound_buffers_dword_77DCA0[i]->Stop();
            g128_Sound_buffers_dword_77DCA0[i]->Release();
            g128_Sound_buffers_dword_77DCA0[i] = 0;
        }
    }

    if (gSoundBuffer_dword_77E1B0)
    {
        gSoundBuffer_dword_77E1B0->Stop();
        gSoundBuffer_dword_77E1B0->Release();
        gSoundBuffer_dword_77E1B0 = 0;
    }

    if (gDSound_dword_77E2C0)
    {
        gDSound_dword_77E2C0->Release();
        gDSound_dword_77E2C0 = 0;
    }
}

// 0x00523232
signed int __cdecl Sound_Start2SamplesQ(BYTE *a1)
{
    DSBUFFERDESC bufferDesc;
    WAVEFORMATEX waveFormat;
    
    if (gSoundFxIdx_dword_77D884 != -1 && gSampleSet_dword_68CE34 == 38)
    {
        g128_Sound_buffers_dword_77DCA0[gSoundFxIdx_dword_77D884]->Stop();
        gSoundFxIdx_dword_77D884 = -1;
    }

    if (gSndSamp1_dword_77E2C4)
    {
        Sound_Stop2Samples();
    }
    
    if (!a1)
    {
        return 0;
    }

    dword_77E1C8 = *a1 << 24;
    dword_77E1C8 |= a1[1] << 16;
    dword_77E1C8 |= a1[2] << 8;
    dword_77E1C8 |= a1[3];
    dword_77E1BC = a1[4] << 8;
    dword_77E1BC |= a1[5];
    dword_77D770 = a1[6] << 8;
    dword_77D770 |= a1[7];
    dword_77D878 = a1[9];
    dword_77E1A4 = a1[8] != 1;
    waveFormat.wFormatTag = 1;
    waveFormat.nChannels = 1;
    waveFormat.nSamplesPerSec = 21 * dword_77D770 / 2;
    waveFormat.nAvgBytesPerSec = 2 * waveFormat.nSamplesPerSec;
    waveFormat.nBlockAlign = 2;
    waveFormat.wBitsPerSample = 16;
    waveFormat.cbSize = 0;
    memset(&bufferDesc, 0, 36u);
    bufferDesc.dwSize = 36;
    bufferDesc.dwFlags = 65736;
    bufferDesc.dwBufferBytes = 176400;
    bufferDesc.lpwfxFormat = &waveFormat;
    
    if (gDSound_dword_77E2C0->CreateSoundBuffer(&bufferDesc, &gSndSamp1_dword_77E2C4, 0))
    {
        return 0;
    }

    gSndSamp1_dword_77E2C4->SetCurrentPosition(0);
    if (dword_77E1A4)
    {
        if (gDSound_dword_77E2C0->CreateSoundBuffer(&bufferDesc, &gSndSamp2_dword_77E2C8, 0))
        {
            return 0;
        }
        gSndSamp2_dword_77E2C8->SetCurrentPosition(0);
        gSndSamp1_dword_77E2C4->SetPan(-10000);
        gSndSamp2_dword_77E2C8->SetPan(10000);
    }
    dword_77E2F8 = 0;
    dword_68E318 = static_cast<DWORD>(-1);
    dword_77E2CC = 0;
    byte_77D888 = 0;
    return 1;
}

// 0x00523466
signed int __cdecl Sound_Stop2Samples()
{
    //sprintf(byte_77E0A4, "Play Stopped\n");
    //OutputDebugStringA(byte_77E0A4);

    if (gSndSamp1_dword_77E2C4)
    {
        gSndSamp1_dword_77E2C4->Stop();
        gSndSamp1_dword_77E2C4->Release();
        gSndSamp1_dword_77E2C4 = 0;
    }

    if (gSndSamp2_dword_77E2C8)
    {
        gSndSamp2_dword_77E2C8->Stop();
        gSndSamp2_dword_77E2C8->Release();
        gSndSamp2_dword_77E2C8 = 0;
    }

    return 1;
}

// 0x0052313B
int __cdecl Sound_StopSample()
{
    int result = 0; // TODO: Should default to error?

    if (gSoundFxIdx_dword_77D884 != -1)
    {
        result = gSoundFxIdx_dword_77D884;
        if (g128_Sound_buffers_dword_77DCA0[gSoundFxIdx_dword_77D884])
        {
            result = g128_Sound_buffers_dword_77DCA0[gSoundFxIdx_dword_77D884]->Stop();
        }
    }

    if (gSndBuffer_dword_77E2D0)
    {
        result = gSndBuffer_dword_77E2D0->Stop();
    }

    if (gSndBuffer_dword_77E0A0)
    {
        result = gSndBuffer_dword_77E0A0->Stop();
    }

    return result;
}

// 0x00521898
int __cdecl Sound_TableUnknown1(int a1, int rate, int vol)
{
    int v4;
    int v5;
    int v6;

    v4 = 10 * a1 / 100;
    if (rate <= vol)
        v6 = dword_68D084[v4] * (vol - rate) / 100 + rate;
    else
        v6 = dword_68D058[v4] * (vol - rate) / 100 + rate;
    if (v4 == 10)
    {
        v5 = 0;
    }
    else if (rate <= vol)
    {
        v5 = dword_68D088[v4] * (vol - rate) / 100 + rate - v6;
    }
    else
    {
        v5 = dword_68D05C[v4] * (vol - rate) / 100 + rate - v6;
    }
    return (a1 - 100 * (10 * a1 / 100) / 10) * v5 / 10 + v6;
}

// 0x0052255B
void __cdecl Sound_SetSoundMusicVolume(int vol)
{
    int idx = 10 * vol / 100;
    int converted = 10000 * gMusicVolTbl_68D02C[idx] / 100 - 10000;
    int finalValue = 0;

    if (idx == 10)
    {
        finalValue = 0;
    }
    else
    {
        finalValue = 10000 * gMusicVolTbl_68D02C[idx + 1] / 100 - 10000 - converted;
    }

    gMusicVol2_dword_77E1C0 = (vol - 100 * (10 * vol / 100) / 10) * finalValue / 10 + converted;
    
    if (gSndState_dword_77E2D4)
    {
        gMusicVolPlaying_dword_77D874 = gMusicVol2_dword_77E1C0;
    }
}

// 0x005224C8
void __cdecl Sound_SetSoundVolume(int vol)
{
    int idx = 10 * vol / 100;
    int converted = 10000 * gSoundVolTbl_68D000[idx] / 100 - 10000;
    
    int finalValue = 0;
    if (idx == 10)
    {
        finalValue = 0;
    }
    else
    {
        finalValue = 10000 * gSoundVolTbl_68D000[idx + 1] / 100 - 10000 - converted;
    }

    gSoundVol_dword_68CE18 = (vol - 100 * (10 * vol / 100) / 10) * finalValue / 10 + converted;
}

// 0x00522CB2
bool __cdecl Sound_PlayEffect(unsigned __int8 idx, int a2, int a3)
{
    bool result; 
    DWORD status;
    int playFlags;
    DWORD index;

    if (!g128_Sound_buffers_dword_77DCA0[idx])
    {
        if (dword_68CEE4[gSampleSet_dword_68CE34])
        {
            if (gSampleSet_dword_68CE34 == 2)
            {
                switch (idx)
                {
                case 160u:
                    idx += 2;
                    break;
                case 163u:
                    idx -= 2;
                    break;
                case 164u:
                case 165u:
                    idx -= 3;
                    break;
                }
            }
            else if (idx != 162 && idx != 163)
            {
                if (idx == 164 || idx == 165)
                {
                    idx -= 4;
                }
            }
            else
            {
                idx -= 2;
            }
        }
    }

    if (g128_Sound_buffers_dword_77DCA0[idx])
    {
        g128_Sound_buffers_dword_77DCA0[idx]->GetStatus(&status);
        if (gFxState_dword_77D8A0[idx] && status & 1)
        {
            if (g64_dword_77D774[dword_77D894])
            {
                g64_dword_77D774[dword_77D894]->Stop();
                g64_dword_77D774[dword_77D894]->Release();
            }

            gDSound_dword_77E2C0->DuplicateSoundBuffer(
                g128_Sound_buffers_dword_77DCA0[idx],
                &g64_dword_77D774[dword_77D894]);

            index = dword_77D894++;
            
            if (dword_77D894 == 64)
            {
                dword_77D894 = 0;
            }

            Sound_PlaySampleRelated(g64_dword_77D774[index], a2, a3, gSoundVol_dword_68CE18);
            g64_dword_77D774[index]->SetCurrentPosition(0);
            result = g64_dword_77D774[index]->Play( 0, 0, 0) == 0;
        }
        else
        {
            playFlags = 0;
            if (gSoundFxIdx_dword_77D884 == -1)
            {
                if (gSampleSet_dword_68CE34 == 10 && idx == 179
                    || gSampleSet_dword_68CE34 == 43 && idx == 192
                    || gSampleSet_dword_68CE34 == 38 && idx == 128
                    || gSampleSet_dword_68CE34 == 64 && idx == 183
                    || (!gSampleSet_dword_68CE34 || gSampleSet_dword_68CE34 == 2 || gSampleSet_dword_68CE34 == 16) && idx == 178)
                {
                    gSoundFxIdx_dword_77D884 = idx;
                    playFlags = DSBPLAY_LOOPING;
                }
            }
            else if (gSampleSet_dword_68CE34 == 10 && idx == 181
                || gSampleSet_dword_68CE34 == 43 && idx == 182
                || gSampleSet_dword_68CE34 == 38 && idx == 181
                || gSampleSet_dword_68CE34 == 64 && (idx == 15 || idx == 195)
                || (!gSampleSet_dword_68CE34 || gSampleSet_dword_68CE34 == 2 || gSampleSet_dword_68CE34 == 16) && idx == 179
                || idx == 26
                || idx == 107)
            {
                g128_Sound_buffers_dword_77DCA0[gSoundFxIdx_dword_77D884]->Stop();
                gSoundFxIdx_dword_77D884 = -1;
            }
            Sound_PlaySampleRelated(g128_Sound_buffers_dword_77DCA0[idx], a2, a3, gSoundVol_dword_68CE18);
            g128_Sound_buffers_dword_77DCA0[idx]->SetCurrentPosition(0);
            result = g128_Sound_buffers_dword_77DCA0[idx]->Play(0, 0, playFlags) == 0;
        }
    }
    else
    {
        result = true;
    }
    return result;
}

// 0x00523E12
bool __cdecl Sound_Unknown4()
{
    DWORD v2;
    bool ret;
    int v4;
    DWORD pos;

    if (gSndBuffer_dword_77E0A0)
    {
        v4 = gBlockAlign_dword_77E1DC * dword_77D87C;
        dword_77D880 += gBlockAlign_dword_77E1DC * dword_77D87C;
        gSndBuffer_dword_77E0A0->GetCurrentPosition(&pos, 0);
        if (dword_77E1B8 - pos > gSoundBufferSize_77E1B4 / 2)
        {
            ++dword_77E1CC;
        }
        dword_77E1B8 = pos;
        v2 = dword_77E1C4 * v4 + pos + gSoundBufferSize_77E1B4 * dword_77E1CC;
        ret = dword_77D880 < v2 || dword_77D880 > v4 + v2;
        while (dword_77D880 >= v2 && dword_77D880 <= v4 + v2)
        {
            gSndBuffer_dword_77E0A0->GetCurrentPosition(&pos, 0);
            if (dword_77E1B8 - pos > gSoundBufferSize_77E1B4 / 2)
            {
                ++dword_77E1CC;
            }
            dword_77E1B8 = pos;

            // dead statement?
            v2 = dword_77E1C4 * v4 + pos + gSoundBufferSize_77E1B4 * dword_77E1CC;
        }
    }
    else
    {
        ret = 1000 * dword_77E1D8 / 15 < timeGetTime() - gSndTime_dword_77D890;
        while (timeGetTime() - gSndTime_dword_77D890 <= 1000 * dword_77E1D8 / 15)
        {

        }
    }
    return ret;
}

// 0x00523CF3
int __cdecl Sound_Unknown5(int a1, int /*a2*/, BYTE*(__cdecl* fnRead)(DWORD))
{
    // TODO: Why a2 isn't used?
    void *sndPtr;
    DWORD sndBufSize;
    void *Dst;
    DWORD Size;

    BYTE* Src = fnRead(a1);
    if (Src)
    {
        const int numBytesToLock = gBlockAlign_dword_77E1DC * dword_77D87C;
        if (gSndBuffer_dword_77E0A0)
        {
            if (gSndBuffer_dword_77E0A0->Lock(
                gOffsetToLock_77E1D4,
                numBytesToLock,
                &Dst,
                &Size,
                &sndPtr,
                &sndBufSize,
                0) == DSERR_BUFFERLOST)
            {
                gSndBuffer_dword_77E0A0->Restore();
                gSndBuffer_dword_77E0A0->Lock(
                    gOffsetToLock_77E1D4,
                    numBytesToLock,
                    &Dst,
                    &Size,
                    &sndPtr,
                    &sndBufSize,
                    0);
            }
            if (Dst)
            {
                memcpy(Dst, Src, Size);
            }
            
            if (sndPtr)
            {
                memcpy(sndPtr, Src + Size, sndBufSize);
            }

            gSndBuffer_dword_77E0A0->Unlock(Dst, Size, sndPtr, sndBufSize);
        }
        
        gOffsetToLock_77E1D4 += numBytesToLock;

        if (gOffsetToLock_77E1D4 >= gSoundBufferSize_77E1B4)
        {
            gOffsetToLock_77E1D4 = 0;
        }
    }
    return dword_77E1D8++ + 1;
}

// 0x00523CB9
void __cdecl Sound_Unknown6()
{
    if (gSndBuffer_dword_77E0A0)
    {
        gSndBuffer_dword_77E0A0->SetVolume(gSoundVol_dword_68CE18);
        gSndBuffer_dword_77E0A0->Play(0, 0, DSBPLAY_LOOPING);
    }
}

// 0x00646660
int __cdecl Sound_Play(unsigned int playingFlags)
{
    if (playingFlags & 0xFF000000)
    {
        if ((playingFlags & 0xFF000000) == 0x1000000)
        {
            Sound_PlayMusic(playingFlags & 0xFFFFFF);
        }
    }
    else
    {
        Sound_PlayEffect(static_cast<unsigned char>(playingFlags), (playingFlags >> 16), playingFlags >> 8);
    }
    return 0;
}

// 0x0044FF6C
int __cdecl Sound_jPlay(unsigned int playingFlags)
{
    return Sound_Play(playingFlags);
}
