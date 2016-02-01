#include "stdafx.h"
#include "MgsFunction.hpp"
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>

IDirectSoundBuffer* gSndBuffer_dword_77E2D0 = nullptr;
IDirectSoundBuffer* g128_Sound_buffers_dword_77DCA0[128] = {};
IDirectSoundBuffer* gSndBuffer_dword_77E0A0 = nullptr;

DWORD gSndState_dword_77E2D4 = 0;
DWORD gSoundFxIdx_dword_77D884 = 0;

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
