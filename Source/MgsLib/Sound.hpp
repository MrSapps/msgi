#pragma once

#include "MgsFunction.hpp"
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>

struct Actor_Movie_Masher;

void SoundCpp_Debug();
void SoundCpp_ForceLink();

signed int __cdecl Sound_Init(HWND hwnd);
char __cdecl Sound_HexCharToInt(char value);
signed int __cdecl Sound_CleanUpRelated();
int __cdecl Sound_CloseWavStopQ();
signed int __cdecl Sound_Res_Movie_CreateBuffer_523A44(int numChannels, signed int bitsPerSample, int samplesPerSecond, int frameSize, int numFramesInterleave);
bool __cdecl Sound_CreatePrimarySoundBuffer();
signed int __cdecl Sound_CreateSecondarySoundBuffer();
__int64 __cdecl Sound_FadeQ(int a1);
bool __cdecl Sound_GetSamp1PosQ();
int __cdecl Sound_GetSomeStateQ();
int __cdecl Sound_InitFx();
signed int __cdecl Sound_LoadBufferFromFile(const char *fileName);
void __cdecl Sound_LoadFxRelatedQ(const char *Str1);
void __cdecl Sound_LoadFxRelatedQ2(const char *Str1);
signed int __cdecl Sound_PlayMusic(unsigned int a1);
int __cdecl Sound_PlaySample();
void __cdecl Sound_PlaySampleRelated(IDirectSoundBuffer* pSoundBuffer, int a2, int a3, signed int a4);
void __cdecl Sound_PopulateBufferQ();
void __cdecl Sound_ReleaseBufferQ();
void __cdecl Sound_ReleaseSecondaryBuffer();
signed int __cdecl Sound_Samp1Related(char *a1, unsigned int a2, IDirectSoundBuffer *snd, int a4); // Seems to do XA conversion
signed int __cdecl Sound_Samp1Related_2(char *a1, unsigned int a2);
void __cdecl Sound_ShutDown();
signed int __cdecl Sound_Start2SamplesQ(BYTE *a1);
signed int __cdecl Sound_Stop2Samples();
int __cdecl Sound_StopSample();
int __cdecl Sound_TableUnknown1(int a1, int rate, int vol);
void __cdecl Sound_SetSoundMusicVolume(int a1);
void __cdecl Sound_SetSoundVolume(int a1);
bool __cdecl Sound_PlayEffect(unsigned __int8 idx, int a2, int a3);
int CC Sound_Masher_Unknown_523E12();

signed int CC Sound_RestoreRelated_523B2C(Actor_Movie_Masher *pMasher,
    int(CC* fnReadFrame)(Actor_Movie_Masher *),
    void *(CC* fnDecodeFrame)(Actor_Movie_Masher *));

int CC Sound_Masher_Write_Audio_Frame_523CF3(Actor_Movie_Masher* pMasher,
    signed int(CC* pFnReadFrame)(Actor_Movie_Masher*),
    void* (CC* fnDecodeFrame)(Actor_Movie_Masher *));

void __cdecl Sound_Unknown6();
int __cdecl Sound_Play(unsigned int playingFlags);
int __cdecl Sound_jPlay(unsigned int playingFlags);

// void Sound_music_relatedQ()
// int __cdecl Sound_music_set()
// _BYTE *__cdecl Sound_set_noise_sound(int a2)
MGS_VAR_EXTERN(DWORD, gBytesWrote_77D880);
