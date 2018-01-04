#pragma once


#define DIRECTINPUT_VERSION 0x700
#include <dinput.h>
#define DIRECTDRAW_VERSION 0x700
#include <ddraw.h>
#define DIRECT3D_VERSION 0x700
#include "d3d.h"


MGS_VAR_EXTERN(DWORD, gFlags_dword_78E7E4);
MGS_VAR_EXTERN(DWORD, counter_dword_6BED20);

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
MGS_VAR_EXTERN(DWORD, g_dwDisplayWidth);
MGS_VAR_EXTERN(DWORD, g_dwDisplayHeight);
MGS_VAR_EXTERN(DWORD, gFps);
MGS_VAR_EXTERN(LPDIRECT3DDEVICE7, gD3dDevice_6FC74C);
EXTERN_MGS_FUNC_NOT_IMPL(0x51E086, int __cdecl(), Render_Restore_Surfaces_51E086);

void CC Main_sub_401C02();
