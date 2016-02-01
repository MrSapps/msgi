#pragma once

#include "MgsFunction.hpp"
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>

signed int __cdecl Sound_Init(HWND hwnd);
char __cdecl Sound_CharUpperChangeQ(char value);
signed int __cdecl Sound_CleanUpRelated();
int __cdecl Sound_CloseWavStopQ();
bool __cdecl Sound_CreatePrimarySoundBuffer();
signed int __cdecl Sound_CreateSecondarySoundBuffer();
int __cdecl Sound_InitFx();
int __cdecl Sound_PlaySample();
void __cdecl Sound_PlaySampleRelated(IDirectSoundBuffer* pSoundBuffer, int a2, int a3, signed int a4);
int __cdecl Sound_TableUnknown1(int a1, int rate, int vol);