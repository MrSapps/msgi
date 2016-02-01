#include "stdafx.h"
#include "Sound.hpp"

IDirectSoundBuffer* gSndBuffer_dword_77E2D0 = nullptr;
IDirectSoundBuffer* g128_Sound_buffers_dword_77DCA0[256] = {};
IDirectSoundBuffer* gSndBuffer_dword_77E0A0 = nullptr;
DWORD gSndState_dword_77E2D4 = 0;
DWORD gSoundFxIdx_dword_77D884 = 0;
DWORD* dword_68D084;
DWORD* dword_68D088;
DWORD* dword_68D058;
DWORD* dword_68D05C;
IDirectSoundBuffer* g64_dword_77D774[64] = {};
DWORD dword_77E2DC = 0;
DWORD dword_77E2F0 = 0;
DWORD gWaveFile_dword_68CE30 = 0;
DWORD dword_77E2D8;
IDirectSound* gDSound_dword_77E2C0 = nullptr;
IDirectSoundBuffer* gSoundBuffer_dword_77E1B0 = nullptr;
DWORD* gFxState_dword_77D8A0 = 0;
DWORD dword_77E1B4 = 0;
DWORD dword_77E1C4 = 0;
DWORD dword_77D87C = 0;
DWORD gBlockAlign_dword_77E1DC = 0;
LONG gSndVolume_dword_77D88C = 0;
DWORD qword_77E1A8 = 0;
DWORD dword_77D874 = 0;
DWORD qword_77D898 = 0;

void Sound_LoadBufferFromFile(const char*)
{
    // TODO
    abort();
}

// 0x0052269C
signed int __cdecl Sound_Init(HWND hwnd)
{
    signed int result = 0;

    if (true) // TODO: Link fix
        //  if (DirectSoundCreate(0, &gDSound_dword_77E2C0, 0))
    {
        result = 0;
    }
    else if (gDSound_dword_77E2C0->SetCooperativeLevel(hwnd, 3))
    {
        result = 0;
    }
    else if (Sound_CreatePrimarySoundBuffer())
    {
        Sound_InitFx();
        result = Sound_CreateSecondarySoundBuffer();
    }
    else
    {
        result = 0;
    }
    return result;
}

// 0x005227AD
char __cdecl Sound_CharUpperChangeQ(char value)
{
    char ret;

    if (value < '0' || value > '9')
    {
        if (toupper(value) < 'A' || toupper(value) > 'F')
            ret = 0;
        else
            ret = toupper(value) - '7';
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

    if (gWaveFile_dword_68CE30 != -1)
    {
        // TODO: msg_close
        //result = _close(gWaveFile_dword_68CE30);
    }

    gWaveFile_dword_68CE30 = -1;
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
    dword_77E1B4 = (a5 + 4) * blockAlign * a4;

    if (gDSound_dword_77E2C0)
    {
        waveFormat.wFormatTag = 1;
        waveFormat.nChannels = numChannels;
        waveFormat.nSamplesPerSec = samplesPerSecond;
        waveFormat.nAvgBytesPerSec = blockAlign * samplesPerSecond;
        waveFormat.nBlockAlign = blockAlign;
        waveFormat.wBitsPerSample = bitsPerSample;
        waveFormat.cbSize = 0;
        memset(&bufferDesc, 0, 36u);
        bufferDesc.dwSize = 36;
        bufferDesc.dwFlags = 0x100C8;
        bufferDesc.dwBufferBytes = dword_77E1B4;
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
    __int64 result;
    LARGE_INTEGER Frequency;
    __int64 freq;
    LARGE_INTEGER PerformanceCount;
    __int64 v5;

    if (gSndBuffer_dword_77E2D0)
    {
        if (gSndState_dword_77E2D4)
        {
            gSndBuffer_dword_77E2D0->GetVolume(&gSndVolume_dword_77D88C);
            QueryPerformanceFrequency(&Frequency);
            freq = Frequency.QuadPart;
            QueryPerformanceCounter(&PerformanceCount);
            qword_77E1A8 = static_cast<DWORD>(PerformanceCount.QuadPart);
            dword_77D874 = -10000;
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
            qword_77D898 = static_cast<DWORD>(freq * v5 / 1000);
            dword_77E2D8 = 1;
        }
    }
    return result;
}

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


// 0x005231A9
int __cdecl Sound_PlaySample()
{
    int result;

    if (gSoundFxIdx_dword_77D884 != -1)
    {
        result = gSoundFxIdx_dword_77D884;
        if (g128_Sound_buffers_dword_77DCA0[gSoundFxIdx_dword_77D884])
            result = g128_Sound_buffers_dword_77DCA0[gSoundFxIdx_dword_77D884]->Play(
            0,
            0,
            1);
    }

    if (gSndBuffer_dword_77E2D0)
    {
        if (gSndState_dword_77E2D4 == 1)
        {
            result = gSndBuffer_dword_77E2D0->Play(0, 0, 1);
        }
    }

    if (gSndBuffer_dword_77E0A0)
    {
        result = gSndBuffer_dword_77E0A0->Play(0, 0, 1);
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
