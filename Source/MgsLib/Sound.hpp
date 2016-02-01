#pragma once

#include "MgsFunction.hpp"
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>

signed int __cdecl Sound_Init(HWND hwnd);
char __cdecl Sound_CharUpperChangeQ(char value);
signed int __cdecl Sound_CleanUpRelated();
int __cdecl Sound_CloseWavStopQ();
signed int __cdecl Sound_CreateBufferQ(int numChannels, signed int bitsPerSample, int samplesPerSecond, int a4, int a5);
bool __cdecl Sound_CreatePrimarySoundBuffer();
signed int __cdecl Sound_CreateSecondarySoundBuffer();
__int64 __cdecl Sound_FadeQ(int a1);
bool __cdecl Sound_GetSamp1PosQ();
int __cdecl Sound_GetSomeStateQ();
int __cdecl Sound_InitFx();
signed int __cdecl Sound_LoadBufferFromFile(const char *fileName);

//
int __cdecl Sound_PlaySample();
void __cdecl Sound_PlaySampleRelated(IDirectSoundBuffer* pSoundBuffer, int a2, int a3, signed int a4);
//
int __cdecl Sound_TableUnknown1(int a1, int rate, int vol);
