#pragma once


#define DIRECTINPUT_VERSION 0x700
#include <dinput.h>
#define DIRECTDRAW_VERSION 0x700
#include <ddraw.h>
#define DIRECT3D_VERSION 0x700
#include "d3d.h"


MGS_VAR_EXTERN(DWORD, counter_dword_6BED20);
MGS_VAR_EXTERN(DWORD, dword_6FC7A0);
MGS_VAR_EXTERN(IDirectDraw7*, g_pDirectDraw);
MGS_VAR_EXTERN(DWORD, g_surface565Mode);
MGS_VAR_EXTERN(DWORD, gColourKey);
MGS_ARY_EXTERN(BYTE, 256, gKeys_9AD880);
MGS_ARY_EXTERN(char, 32, gDest_78E7C0);
MGS_VAR_EXTERN(DWORD, gFreeCameraCheat_77C934);
MGS_VAR_EXTERN(DWORD, gCheatsEnabled_71687C);
MGS_VAR_EXTERN(DWORD, gMouseMove_dword_717348);
MGS_VAR_EXTERN(DWORD, gInput_MouseX_dword_734908);
MGS_VAR_EXTERN(DWORD, dword_732E64);
MGS_VAR_EXTERN(DWORD, gInput_MouseY_dword_73490C);
MGS_VAR_EXTERN(DWORD, gEscapePressed_NoMouseNoBgState_dword_717354);

union UGameStates
{
    struct Parts
    {
        BYTE flags0;
        BYTE flags1;
        BYTE flags2;
        BYTE flags3;
    };
    Parts mParts;
    DWORD flags;
};
MGS_ASSERT_SIZEOF(UGameStates, 0x4);

MGS_VAR_EXTERN(UGameStates, game_state_dword_72279C);
MGS_VAR_EXTERN(int, gActiveBuffer_dword_791A08);
MGS_VAR_EXTERN(DWORD, gAlphaModulate_dword_6FC798);
MGS_VAR_EXTERN(DWORD, g_surface565Mode);
MGS_VAR_EXTERN(int, gBlendMode);

extern HWND& gHwnd;
extern DWORD& gWindowedMode;

MGS_VAR_EXTERN(DWORD, gInfiniteAmmoCheat_650D4C);
MGS_VAR_EXTERN(DWORD, g_dwDisplayWidth_6DF214);
MGS_VAR_EXTERN(DWORD, g_dwDisplayHeight);
MGS_VAR_EXTERN(DWORD, gFps);
MGS_VAR_EXTERN(LPDIRECT3DDEVICE7, gD3dDevice_6FC74C);
MGS_VAR_EXTERN(DWORD, gUseTrueType_650D40);

void CC Main_sub_401C02();
void __cdecl ClearAll();
