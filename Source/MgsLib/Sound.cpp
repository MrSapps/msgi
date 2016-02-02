#include "stdafx.h"
#include "Sound.hpp"
#include "File.hpp"

using QWORD = __int64;

static IDirectSoundBuffer* gSndBuffer_dword_77E2D0 = nullptr;
static IDirectSoundBuffer* g128_Sound_buffers_dword_77DCA0[256] = {};
static IDirectSoundBuffer* gSndBuffer_dword_77E0A0 = nullptr;
static DWORD gSndState_dword_77E2D4 = 0;
static DWORD gSoundFxIdx_dword_77D884 = 0;
static DWORD* dword_68D058; // part of below array?
static DWORD* dword_68D05C; // 21 array?
static DWORD* dword_68D084; // part of below array?
static DWORD* dword_68D088; // 10 array?
static IDirectSoundBuffer* g64_dword_77D774[64] = {};
static DWORD dword_77E2DC = 0;
static DWORD dword_77E2F0 = 0;
static DWORD gWaveFile_dword_68CE30 = 0;
static DWORD dword_77E2D8;
static IDirectSound* gDSound_dword_77E2C0 = nullptr;
static IDirectSoundBuffer* gSoundBuffer_dword_77E1B0 = nullptr;
static DWORD* gFxState_dword_77D8A0 = 0; // 256 array?
static DWORD dword_77E1B4 = 0;
static DWORD dword_77E1C4 = 0;
static DWORD dword_77D87C = 0;
static DWORD gBlockAlign_dword_77E1DC = 0;
static LONG gSndVolume_dword_77D88C = 0;
static QWORD qword_77E1A8 = 0;
static DWORD dword_77D874 = 0;
static QWORD qword_77D898 = 0;
static IDirectSoundBuffer* gSndSamp1_dword_77E2C4 = nullptr;
static IDirectSoundBuffer* gSndSamp2_dword_77E2C8 = nullptr;
static DWORD gSamp1PlayPos_dword_77E1D0 = 0;
static DWORD dword_68E318 = 0;
static DWORD dword_77E2F8 = 0;
static DWORD dword_68CE18 = 0;

VAR(DWORD, dword_77E2CC, 0x77E2CC);

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
            qword_77E1A8 = PerformanceCount.QuadPart;
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
    char v4;
    DWORD sizeToRead;
    DSBUFFERDESC bufferDesc;
    void* v9;
    DWORD v10;
    size_t fileNameLength; 
    WAVEFORMATEX waveFormat;
    void *soundBuffer;

    gSoundFxIdx_dword_77D884 = -1;
    fileNameLength = strlen(fileName) - 6;
    const char v1 = 16 * Sound_CharUpperChangeQ(fileName[fileNameLength]);
    const unsigned __int8 idx = Sound_CharUpperChangeQ(fileName[fileNameLength + 1]) + v1;
    memset(&bufferDesc, 0, 36u);
    bufferDesc.dwSize = 36;
    bufferDesc.dwFlags = 0x100C8;
    bufferDesc.lpwfxFormat = &waveFormat;
    FILE* File = File_LoadDirFileQ(fileName, 0);
    if (File
        && (File_NormalRead(File, &v4, 20u), File_NormalRead(File, &waveFormat, 18u) == 18)
        && (File_NormalRead(File, &v4, 2u), File_NormalRead(File, &sizeToRead, 4u) == 4))
    {
        bufferDesc.dwBufferBytes = sizeToRead;
        if (gDSound_dword_77E2C0)
        {
            if (gDSound_dword_77E2C0->CreateSoundBuffer(
                &bufferDesc,
                (IDirectSoundBuffer **)g128_Sound_buffers_dword_77DCA0[idx],
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
                0) == 0x88780096)
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
            v3 = File_NormalRead(File, soundBuffer, sizeToRead);
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
        File_CloseQ(File);
        result = 1;
    }
    else
    {
        File_CloseQ(File);
        result = 0;
    }
    return result;
}

DWORD dword_77D894 = 0;

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

// 0x005231A9
int __cdecl Sound_PlaySample()
{
    int result;

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
    int result;

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

// 0x00523CB9
void __cdecl Sound_Unknown6()
{
    if (gSndBuffer_dword_77E0A0)
    {
        gSndBuffer_dword_77E0A0->SetVolume(dword_68CE18);
        gSndBuffer_dword_77E0A0->Play(0, 0, DSBPLAY_LOOPING);
    }
}
