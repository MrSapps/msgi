#include "stdafx.h"
#include "Menu.hpp"
#include "LibDG.hpp"
#include "MgsFunction.hpp"
#include "Actor.hpp"
#include "Actor_GameD.hpp"
#include "Script.hpp"
#include "WinMain.hpp"
#include "Psx.hpp"

#define MENU_IMPL true

struct MenuMan;
using TMenuUpdateFn = void(CC*)(MenuMan*, int*);

// TODO: Discover true size of structure/other data
struct MenuMan
{
    Actor mBase;
    DWORD field_20_gTextDraws;
    ButtonStates* field_24_input;
    BYTE field_28_flags;
    BYTE field_29;
    BYTE field_2A;
    BYTE field_2B;
    TMenuUpdateFn m7FnPtrs_field_2C[7];
    DR_ENV mDR_ENV_field_48;
    DR_ENV mDR_ENV_field_88;
    DR_ENV DR_ENV_field_C8;
    DWORD field_108;
    DWORD field_10C;
    DWORD field_110;
    DWORD field_114;
    DWORD field_118;
    DWORD field_11C;
    DWORD field_120;
    DWORD field_124;
    DWORD field_128;
    DWORD field_12C;
    DWORD field_130;
    DWORD field_134;
    DWORD field_138;
    DWORD field_13C;
    DWORD field_140;
    DWORD field_144;
    PSX_RECT field_148_rect;
    DR_ENV mDrEnvDst_field_150;
    DR_ENV mDrEnvDst_field_190;
    WORD field_1D0;
    WORD field_1D2;
    BYTE field_1D4;
    BYTE field_1D5;
    WORD field_1D6;
    DWORD field_1D8;
    DWORD field_1DC;
    DWORD field_1E0;
    DWORD field_1E4;
    DWORD field_1E8;
    DWORD field_1EC;
    DWORD field_1F0;
    DWORD field_1F4;
    DWORD field_1F8;
    DWORD field_1FC;
    BYTE field_200;
    BYTE field_201;
    WORD field_202;
    DWORD field_204;
    DWORD field_208;
    DWORD field_20C;
    DWORD field_210_size_19F2_q;
    DWORD field_214;
    DWORD field_218;
};
MGS_ASSERT_SIZEOF(MenuMan, 0x21C);


MGS_VAR(1, 0x722788, Texture_Record, gMenuTexture_A0BE_722788, {});
MGS_VAR(1, 0x725FC0, MenuMan, gMenuMan_stru_725FC0, {});

void CC Res_MenuMan_create_459A9A()
{
    Actor_PushBack_40A2AF(1, &gMenuMan_stru_725FC0.mBase, nullptr);
    Actor_Init_40A347(&gMenuMan_stru_725FC0.mBase, nullptr, nullptr, "C:\\mgs\\source\\Menu\\menuman.c");
}
MGS_FUNC_IMPLEX(0x00459A9A, Res_MenuMan_create_459A9A, MENU_IMPL);

MGS_FUNC_NOT_IMPL(0x462A3D, void __cdecl(MenuMan* pMenu, int* ot), Menu_update_helper_462A3D);
MGS_FUNC_NOT_IMPL(0x459971, void __cdecl(MenuMan* pMenu), Menu_shutdown_459971);
MGS_FUNC_NOT_IMPL(0x459991, void __cdecl(MenuMan* pMenu), Menu_create_helper_459991);

TILE* CC Menu_render_rect_46B79F(BYTE **ot, __int16 x, __int16 y, __int16 w, __int16 h, int rgb)
{
    TILE* pTile = reinterpret_cast<TILE*>(*ot);
    *ot += sizeof(TILE);

    pTile->r0 = BYTE0(rgb);
    pTile->g0 = BYTE1(rgb);
    pTile->b0 = BYTE2(rgb);

    pTile->x0 = x;
    pTile->y0 = y;
    pTile->w = w;
    pTile->h = h;
    
    setTile(pTile);
    addPrim(ot[1], pTile);

    return pTile;
}
MGS_FUNC_IMPLEX(0x0046B79F, Menu_render_rect_46B79F, MENU_IMPL);

struct BarConfig
{
    const char* mText;
    BYTE mLeftRGB[3];
    BYTE mRightRGB[3];
    WORD mBarHeight;
};
MGS_ASSERT_SIZEOF(BarConfig, 0xC);

void __cdecl Menu_draw_bar_468DA6(int* ot, int xpos, int ypos, int redFillLength, int normalFillLength, int barLength, BarConfig *pBar);
MGS_FUNC_IMPLEX(0x00468DA6, Menu_draw_bar_468DA6, true);
void __cdecl Menu_draw_bar_468DA6(int* ot, int xpos, int ypos, int redFillLength, int normalFillLength, int barLength, BarConfig* pBar)
{

    // Setting bit 0x40000000 in pBar enables the red text/damage display
    Menu_draw_bar_468DA6_.Ptr()(ot, xpos , ypos, redFillLength, normalFillLength, barLength, pBar);
}

MGS_ARY(1, 0x733868, DWORD, 16, dword_733868, {});
MGS_ARY(1, 0x7338A8, DWORD, 16, dword_7338A8, {});
MGS_VAR(1, 0x7265E4, int*, gTextCurrentOt_7265E4, nullptr);


void CC sub_462E8D()
{
    int idx = 0;
    do
    {
        dword_733868[idx] = 0;
        dword_7338A8[idx] = 0;
        idx += 2; // TODO: Why are some elements skipped?
    } while (idx < 16);
}
MGS_FUNC_IMPLEX(0x00462E8D, sub_462E8D, MENU_IMPL);

MGS_VAR(1, 0x995348, WORD, word_995348, 0);

void CC Menu_update_4598BC(MenuMan* pMenu)
{
    int* pOtText1 = gTextCurrentOt_7265E4;
    int* pOtText2 = gTextCurrentOt_7265E4;
    pMenu->field_24_input = gpActiveButtons_995324 + 2;

    Menu_update_helper_462A3D(pMenu, pOtText2);

    if (!(gActorPauseFlags_dword_791A0C & 2)
        && gLoaderState_dword_9942B8 > 0
        && !script_cancel_non_zero_dword_7227A0
        && game_state_dword_72279C >= 0)
    {
        const int field_28_flags = pMenu->field_28_flags;
        for (int i = 0; i < 7; i++)
        {
            if ((1 << i) & field_28_flags)
            {
                // 0 = life bars
                // 1 = right side inventory
                // 2 = left side inventory
                // 3 = radar
                // 4 = enter/exit codec
                // 5/6 = not set by default
                pMenu->m7FnPtrs_field_2C[i](pMenu, pOtText1);
            }
        }
    }

    /* TODO: This is probably a TILE prim? 
       Check if this causes the codec rect to appear at the ninja fight or not
    *(&pMenu->mDR_ENV_field_48.tag + 16 * gActiveBuffer_dword_791A08) ^= (*(&pMenu->mDR_ENV_field_48.tag
        + 16 * gActiveBuffer_dword_791A08) ^ *pOtText1) & 0xFFFFFF;
    *pOtText1 ^= (*pOtText1 ^ (unsigned int)(&pMenu->mDR_ENV_field_48 + gActiveBuffer_dword_791A08)) & 0xFFFFFF;
    */
}
MGS_FUNC_IMPLEX(0x004598BC, Menu_update_4598BC, MENU_IMPL);

void CC Menu_create_459891()
{
    Actor_Init_40A347(&gMenuMan_stru_725FC0.mBase,
        (TActorFunction)Menu_update_4598BC,
        (TActorFunction)Menu_shutdown_459971.Ptr(),
        "C:\\mgs\\source\\Menu\\menuman.c");

    Menu_create_helper_459991(&gMenuMan_stru_725FC0);
    sub_462E8D();
}
MGS_FUNC_IMPLEX(0x00459891, Menu_create_459891, MENU_IMPL);

void CC MenuTextureLoad_44DEB3()
{
    Texture_Record* pFound = LibDG_FindTexture_4024A0(0xA0BE);

    gMenuTexture_A0BE_722788.mHashedName = pFound->mHashedName;
    gMenuTexture_A0BE_722788.mUsed = pFound->mUsed;
    gMenuTexture_A0BE_722788.mNumColours = pFound->mNumColours;

    gMenuTexture_A0BE_722788.mTPage = pFound->mTPage;
    gMenuTexture_A0BE_722788.mClut = pFound->mClut;

    gMenuTexture_A0BE_722788.u0 = pFound->u0;
    gMenuTexture_A0BE_722788.v0 = pFound->v0;
    gMenuTexture_A0BE_722788.u1 = pFound->u1;
    gMenuTexture_A0BE_722788.v1 = pFound->v1;

    gMenuTexture_A0BE_722788.mHashedName = 0;
}
MGS_FUNC_IMPLEX(0x44DEB3, MenuTextureLoad_44DEB3, MENU_IMPL);
