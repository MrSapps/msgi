#include "stdafx.h"
#include "LibDG.hpp"
#include "Menu.hpp"
#include "MgsFunction.hpp"
#include "Actor_GameD.hpp"
#include "Psx.hpp"
#include "WinMain.hpp"
#include "LibDG.hpp"
#include "LibGV.hpp"
#include "Script.hpp"

#define MENU_IMPL true

struct MenuPrimBuffer
{
    BYTE* mFreeLocation;
    BYTE* mOt;
    BYTE* mOtEnd;
};

struct MenuMan;
using TMenuUpdateFn = void(CC*)(MenuMan*, int*);

#pragma pack(push)
#pragma pack(1)
struct MenuMan_MenuBars
{
    BYTE field_0_state;
    BYTE field_1_O2_hp;
    short int field_2_bar_x;
    short int field_4_bar_y;
    short int field_6_snake_hp;
    short int field_8;
    short int field_A_k10_decrement;
};
#pragma pack(pop)
MGS_ASSERT_SIZEOF(MenuMan_MenuBars, 0xc);

// TODO: Discover true size of structure/other data
struct MenuMan
{
    Actor mBase;
    MenuPrimBuffer* field_20_prim_buffer;
    ButtonStates* field_24_input;
    BYTE field_28_flags;
    BYTE field_29;
    BYTE field_2A_bSkipUpdateHpBars;
    BYTE field_2B;
    TMenuUpdateFn m7FnPtrs_field_2C[7];
    DR_ENV mDR_ENV_field_48[2];
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
    MenuMan_MenuBars field_200_hp_bars_info;
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
MGS_FUNC_NOT_IMPL(0x46B8CA, void __cdecl(char *pFileName), Menu_create_helper_item_file_46B8CA);

template<class T>
inline static T* PrimAlloc(MenuPrimBuffer* pPrimBuffer)
{
    T* pT = reinterpret_cast<T*>(pPrimBuffer->mFreeLocation);
    pPrimBuffer->mFreeLocation += sizeof(T);
    return pT;
}

TILE* CC Menu_render_rect_46B79F(MenuPrimBuffer* pPrimBuffer, __int16 x, __int16 y, __int16 w, __int16 h, int rgb)
{
    TILE* pTile = PrimAlloc<TILE>(pPrimBuffer);

    setTile(pTile);
    setRGB0(pTile, BYTE0(rgb), BYTE1(rgb), BYTE2(rgb));

    pTile->x0 = x;
    pTile->y0 = y;
    pTile->w = w;
    pTile->h = h;
    
   
    addPrim(pPrimBuffer->mOt, pTile);

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

MGS_ARY(1, 0x733868, DWORD, 16, dword_733868, {});
MGS_ARY(1, 0x7338A8, DWORD, 16, dword_7338A8, {});

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

MGS_VAR(1, 0x7265E0, MenuPrimBuffer, gMenuPrimBuffer_7265E0, {});
MGS_ARY(1, 0x7269F4, BYTE, 8192, gMenuPrimArray1_7269F4, {});
MGS_ARY(1, 0x7289F4, BYTE, 8192, gMenuPrimArray2_7289F4, {});
MGS_ARY(1, 0x7265EC, BYTE*, 2, gMenuPrimBufferArrays_7265EC, {});

MGS_FUNC_NOT_IMPL(0x468158, void __cdecl(MenuMan*, int*), Menu_radar_update_468158);
MGS_FUNC_NOT_IMPL(0x463763, void __cdecl(MenuMan*, int*), Menu_codec_update_463763);


MGS_FUNC_NOT_IMPL(0x465B38, void __cdecl(), Menu_init_radar_helper_465B38);
MGS_FUNC_NOT_IMPL(0x468264, void __cdecl(MenuMan*, int), Menu_radar_468264);
MGS_FUNC_NOT_IMPL(0x465A01, void* __cdecl(int), sub_465A01);



MGS_VAR(1, 0x733950, DWORD, gFn_radar_dword_733950, 0);

void CC Menu_init_radar_468358(MenuMan* pMenu)
{
    pMenu->m7FnPtrs_field_2C[3] = Menu_radar_update_468158.Ptr();
    pMenu->field_28_flags |= 8;
    pMenu->field_1D4 = 1;
    pMenu->field_1D0 = 0;
    pMenu->field_1D2 = 0;
    Menu_radar_468264(pMenu, 0);
    Menu_radar_468264(pMenu, 1);
    memcpy(&pMenu->mDrEnvDst_field_150, &pMenu->mDR_ENV_field_48[0], sizeof(pMenu->mDrEnvDst_field_150));
    memcpy(&pMenu->mDrEnvDst_field_190, &pMenu->mDR_ENV_field_48[1], sizeof(pMenu->mDrEnvDst_field_190));
    Menu_init_radar_helper_465B38();
    gFn_radar_dword_733950 = 0;
    sub_465A01(4096);
}
MGS_FUNC_IMPLEX(0x00468358, Menu_init_radar_468358, MENU_IMPL);

void CC Menu_init_codec_463746(MenuMan* pMenu)
{
    pMenu->m7FnPtrs_field_2C[4] = Menu_codec_update_463763.Ptr();
    pMenu->field_28_flags |= 0x10u;
}
MGS_FUNC_IMPLEX(0x00463746, Menu_init_codec_463746, MENU_IMPL);

using TMenuFn = void(CC*)(MenuMan*);

MGS_FUNC_NOT_IMPL(0x46AD91, void __cdecl(MenuMan*), Menu_init_fn0_46AD91);
MGS_FUNC_NOT_IMPL(0x469E77, void __cdecl(MenuMan*), Menu_init_inventory_left_469E77);
MGS_FUNC_NOT_IMPL(0x4694E4, void __cdecl(MenuMan*), Menu_init_inventory_right_4694E4);
MGS_FUNC_NOT_IMPL(0x468406, void __cdecl(MenuMan*), Menu_init_fn6_468406);
MGS_FUNC_NOT_IMPL(0x462CFC, void __cdecl(MenuMan*), Menu_init_fn7_jimaku_font_buffer_size_sub_462CFC);

struct TextConfig
{
    DWORD gTextX_dword_66C4C0;
    DWORD gTextY_dword_66C4C4;
    DWORD gTextFlags_dword_66C4C8;
    DWORD gTextRGB_dword_66C4CC;
};
MGS_ASSERT_SIZEOF(TextConfig, 0x10);

template<class T>
static inline T UnSetPointerFlag(T ptr, bool& bWasFlagged)
{
    bWasFlagged = (reinterpret_cast<unsigned int>(ptr) & 0x40000000) ? true : false;
    return reinterpret_cast<T>(reinterpret_cast<unsigned int>(ptr) & 0xBFFFFFFF);
}

MGS_VAR(1, 0x66C4C0, TextConfig, gTextConfig_66C4C0, {});

MGS_FUNC_NOT_IMPL(0x468AAF, int __cdecl(MenuPrimBuffer *ot, TextConfig* pTextSettings, const char* pString), Render_Text_Flag0x10_468AAF);
MGS_FUNC_NOT_IMPL(0x468642, int __cdecl(MenuPrimBuffer* ot, TextConfig* pTextSettings, const char* pString), Render_Text_NotFlag0x10_468642);

int CC TextSetRGB_459B27(int r, int g, int b)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
    return 0;
}
MGS_FUNC_IMPLEX(0x459B27, TextSetRGB_459B27, false);  // TODO

void CC Menu_DrawText_459B63(const char* pFormatStr, int formatArg1, int formatArg2, int formatArg3, int formatArg4)
{
    char formattedStr[64] = {};
    if (gMenuPrimBuffer_7265E0.mFreeLocation)
    {
        sprintf(formattedStr, pFormatStr, formatArg1, formatArg2, formatArg3, formatArg4);
        // Check there is enough space in the buffer for a SPRT per char, plus 2 extra
        if (gMenuPrimBuffer_7265E0.mOtEnd - gMenuPrimBuffer_7265E0.mFreeLocation >=
            static_cast<signed int>(sizeof(SPRT) * (strlen(formattedStr) + 2)))
        {
            if (gTextConfig_66C4C0.gTextFlags_dword_66C4C8 & 0x10)
            {
                // Larger font
                Render_Text_Flag0x10_468AAF(&gMenuPrimBuffer_7265E0, &gTextConfig_66C4C0, formattedStr);
            }
            else
            {
                // Smaller font
                Render_Text_NotFlag0x10_468642(&gMenuPrimBuffer_7265E0, &gTextConfig_66C4C0, formattedStr);
            }
            Menu_Set_Text_BlendMode_459BE0();
        }
    }
}
MGS_FUNC_IMPLEX(0x459B63, Menu_DrawText_459B63, MENU_IMPL);

void CC Menu_render_life_bar_468DA6(MenuPrimBuffer* pPrimBuffer, short int xpos, short int ypos, short int redFillLength, short int normalFillLength, short int barLength, BarConfig* pMaybeFlaggedBarConfig)
{
    if (!(game_state_dword_72279C.flags & 0x80020400))
    {
        bool bDrawTextRed = false;
        BarConfig* pBarConfig = UnSetPointerFlag(pMaybeFlaggedBarConfig, bDrawTextRed);

        const short int barHeight = 5 - pBarConfig->mBarHeight;
        const short int scaledBarWidth = (barLength << 7) / 1024;

        TextConfig textConfig = {};
        textConfig.gTextFlags_dword_66C4C8 = 0;
        textConfig.gTextX_dword_66C4C0 = xpos + 4;
        textConfig.gTextY_dword_66C4C4 = ypos + 4;
        textConfig.gTextRGB_dword_66C4CC = bDrawTextRed ? 0x643030FF : 0x64FFFFFF;

        Render_Text_NotFlag0x10_468642(pPrimBuffer, &textConfig, pBarConfig->mText);
        TILE* pBarRectPrim = Menu_render_rect_46B79F(
            pPrimBuffer,
            xpos + 3,
            static_cast<__int16>(textConfig.gTextY_dword_66C4C4) - 1,
            static_cast<__int16>(textConfig.gTextX_dword_66C4C0) - (xpos + 4) + 2,
            7,
            0);

        // Rect is semi trans
        pBarRectPrim->code |= 2u;

        // "Damage" fill can't overflow the max bar length
        if (redFillLength > barLength)
        {
            redFillLength = barLength;
        }

        if (redFillLength > normalFillLength)
        {
            TILE* pTile = PrimAlloc<TILE>(pPrimBuffer);
            setTile(pTile);
            setRGB0(pTile, 200, 0, 0);
            pTile->x0 = xpos + (normalFillLength << 7) / 1024;
            pTile->y0 = ypos + 1;
            pTile->w = ((redFillLength - normalFillLength) << 7) / 1024;
            pTile->h = barHeight;
            addPrim(pPrimBuffer->mOt, pTile);
        }

        POLY_G4* pPolyG4 = PrimAlloc<POLY_G4>(pPrimBuffer);
        setPolyG4(pPolyG4);

        pPolyG4->x0 = xpos;
        pPolyG4->y0 = ypos + 1;

        pPolyG4->x2 = xpos;
        const __int16 x1 = ((normalFillLength + 7) << 7) / 1024 + xpos;
        pPolyG4->y1 = ypos + 1;

        const __int16 barBottom = barHeight + ypos + 1;
        pPolyG4->x1 = x1;
        pPolyG4->y2 = barBottom;

        pPolyG4->x3 = x1;
        pPolyG4->y3 = barBottom;
        
        setRGB0(pPolyG4, pBarConfig->mLeftRGB[0], pBarConfig->mLeftRGB[1], pBarConfig->mLeftRGB[2]);

        const int x1_m_xpos_1 = x1 - xpos;
        
        pPolyG4->r1 = static_cast<BYTE>(pBarConfig->mLeftRGB[0] + x1_m_xpos_1 * (pBarConfig->mRightRGB[0] - pBarConfig->mLeftRGB[0]) / 128);
        pPolyG4->g1 = static_cast<BYTE>(pBarConfig->mLeftRGB[1] + x1_m_xpos_1 * (pBarConfig->mRightRGB[1] - pBarConfig->mLeftRGB[1]) / 128);
        pPolyG4->b1 = static_cast<BYTE>(pBarConfig->mLeftRGB[2] + x1_m_xpos_1 * (pBarConfig->mRightRGB[2] - pBarConfig->mLeftRGB[2]) / 128);

        pPolyG4->r2 = pBarConfig->mLeftRGB[0];
        pPolyG4->g2 = pBarConfig->mLeftRGB[1];
        pPolyG4->b2 = pBarConfig->mLeftRGB[2];

        const int x1_m_xpos_2 = x1 - xpos;

        pPolyG4->r3 = static_cast<BYTE>(pBarConfig->mLeftRGB[0] + x1_m_xpos_2 * (pBarConfig->mRightRGB[0] - pBarConfig->mLeftRGB[0]) / 128);
        pPolyG4->g3 = static_cast<BYTE>(pBarConfig->mLeftRGB[1] + x1_m_xpos_2 * (pBarConfig->mRightRGB[1] - pBarConfig->mLeftRGB[1]) / 128);
        pPolyG4->b3 = static_cast<BYTE>(pBarConfig->mLeftRGB[2] + x1_m_xpos_2 * (pBarConfig->mRightRGB[2] - pBarConfig->mLeftRGB[2]) / 128);

        addPrim(pPrimBuffer->mOt, pPolyG4);

        TILE* pMenuBarBackgroundRect = Menu_render_rect_46B79F(pPrimBuffer, xpos, ypos + 1, scaledBarWidth, barHeight, 0x181800);
        // Set semi trans
        pMenuBarBackgroundRect->code |= 2u;

        Menu_render_rect_46B79F(pPrimBuffer, xpos - 1, ypos, 1, barHeight + 2, 0);
        Menu_render_rect_46B79F(pPrimBuffer, xpos, ypos, scaledBarWidth, 1, 0);
        Menu_render_rect_46B79F(pPrimBuffer, xpos, barHeight + ypos + 1, scaledBarWidth, 1, 0);
        Menu_render_rect_46B79F(pPrimBuffer, xpos + scaledBarWidth, ypos, 1, barHeight + 2, 0);
        
        DR_TPAGE* drTPage = PrimAlloc<DR_TPAGE>(pPrimBuffer);
        setDrawTPage(drTPage, 1, 1, 31);
        addPrim(pPrimBuffer->mOt, drTPage);
    }
}
MGS_FUNC_IMPLEX(0x468DA6, Menu_render_life_bar_468DA6, MENU_IMPL);

MGS_VAR(1, 0x78E7F6, WORD, gSnakeCurrentHealth_78E7F6, 0);

MGS_VAR(1, 0x7339D8, int, gTakeDamageCounter_dword_7339D8, 0);
MGS_VAR(1, 0x78E7F8, WORD, gSnakeMaxHealth_78E7F8, 0);
MGS_VAR(1, 0x995348, WORD, gSnakeCurrentO2_995348, 0);



signed int CC Menu_menu_bars_update_field200_46938A(MenuMan_MenuBars* pMenuBarsUnk)
{
    if (pMenuBarsUnk->field_6_snake_hp == gSnakeCurrentHealth_78E7F6)
    {
        pMenuBarsUnk->field_A_k10_decrement = 10;
        return 0;
    }

    if (pMenuBarsUnk->field_6_snake_hp > gSnakeCurrentHealth_78E7F6)
    {
        if (pMenuBarsUnk->field_A_k10_decrement)
        {
            pMenuBarsUnk->field_A_k10_decrement -= 1;
        }
        else
        {
            pMenuBarsUnk->field_6_snake_hp -= 64;
        }
    }

    if (pMenuBarsUnk->field_6_snake_hp < gSnakeCurrentHealth_78E7F6)
    {
        pMenuBarsUnk->field_6_snake_hp = gSnakeCurrentHealth_78E7F6;
    }
    return 1;
}
MGS_FUNC_IMPLEX(0x46938A, Menu_menu_bars_update_field200_46938A, MENU_IMPL);

// void __cdecl Menu_init_fn6_468406(MenuMan *pMenu)
MGS_VAR(1, 0x6757F0, BarConfig, gSnakeLifeBarConfig_6757F0, {}); // TODO: Populate
MGS_VAR(1, 0x675800, BarConfig, gSnakeO2BarConfig_675800, {}); // TODO: Populate
MGS_VAR(1, 0x7339D4, int, gSnakeLifeYPos_7339D4, 0);

template<class T>
static inline T SetPointerFlag(T ptr)
{
    return reinterpret_cast<T>(reinterpret_cast<unsigned int>(ptr) | 0x40000000);
}

void CC Menu_render_snake_life_bar_469194(MenuPrimBuffer* ot, short xpos, short ypos)
{
    game_state_dword_72279C.mParts.flags1 |= 0x80u;
    Menu_render_life_bar_468DA6(
        ot,
        xpos,
        ypos,
        gSnakeCurrentHealth_78E7F6,
        gSnakeCurrentHealth_78E7F6,
        gSnakeMaxHealth_78E7F8,
        &gSnakeLifeBarConfig_6757F0);
}
MGS_FUNC_IMPLEX(0x469194, Menu_render_snake_life_bar_469194, MENU_IMPL);

void CC Menu_render_auto_stacked_any_size_menu_bar_469160(MenuPrimBuffer* pPrimBuffer, short xpos, short ypos, short redFillLength, short normalFillLength, short barLength, BarConfig *pBarConfig)
{
    game_state_dword_72279C.mParts.flags1 |= 0x80u;
    Menu_render_life_bar_468DA6(
        pPrimBuffer,
        xpos,
        static_cast<short>(gSnakeLifeYPos_7339D4 + ypos - 16),
        redFillLength,
        normalFillLength,
        barLength,
        pBarConfig);
}
MGS_FUNC_IMPLEX(0x469160, Menu_render_auto_stacked_any_size_menu_bar_469160, MENU_IMPL);

void CC Menu_Set_Text_BlendMode_459BE0()
{
    DR_TPAGE* pDrTPage = PrimAlloc<DR_TPAGE>(&gMenuPrimBuffer_7265E0);
    int abr = ((gTextConfig_66C4C0.gTextFlags_dword_66C4C8 >> 8) & 3);
    setDrawTPage(pDrTPage, 1, 1, getTPage(0, abr, 960, 256));
    addPrim(gMenuPrimBuffer_7265E0.mOt, pDrTPage);
}
MGS_FUNC_IMPLEX(0x00459BE0, Menu_Set_Text_BlendMode_459BE0, MENU_IMPL);

void CC TextSetXYFlags_459B0B(int x, int y, int flags)
{
    // TODO: Create enum and replace call sites with enum flags
    // 0x1 = right align
    // 0x2 = center align
    // 0x10 = larger font size
    // 0x20 = semi transparent

    // other bits = left align/do nothing
    gTextConfig_66C4C0.gTextX_dword_66C4C0 = x;
    gTextConfig_66C4C0.gTextY_dword_66C4C4 = y;
    gTextConfig_66C4C0.gTextFlags_dword_66C4C8 = flags;
}
MGS_FUNC_IMPLEX(0x459B0B, TextSetXYFlags_459B0B, MENU_IMPL);

void CC Menu_render_auto_stacked_1024_size_menu_bar_459C6B(short xpos, short ypos, short redFillLength, short normalFillLength, BarConfig* pBarConfig)
{
    game_state_dword_72279C.mParts.flags1 |= 0x80u;
    Menu_render_auto_stacked_any_size_menu_bar_469160(
        &gMenuPrimBuffer_7265E0,
        xpos,
        ypos,
        redFillLength,
        normalFillLength,
        static_cast<short>(1024),
        pBarConfig);
    Menu_Set_Text_BlendMode_459BE0();
}
MGS_FUNC_IMPLEX(0x00459C6B, Menu_render_auto_stacked_1024_size_menu_bar_459C6B, MENU_IMPL);

void CC Menu_render_fixedx_auto_stacked_menu_bar_impl_46912D(MenuPrimBuffer* pPrimBuffer, short ypos, short redFill, short normalFill, short maxFill, BarConfig* pBarConfig)
{
    game_state_dword_72279C.mParts.flags1 |= 0x80u;
    Menu_render_life_bar_468DA6(pPrimBuffer, 16, static_cast<short>(gSnakeLifeYPos_7339D4 + ypos - 16), redFill, normalFill, maxFill, pBarConfig);
}
MGS_FUNC_IMPLEX(0x46912D, Menu_render_fixedx_auto_stacked_menu_bar_impl_46912D, MENU_IMPL);

void CC Menu_render_fixed_xpos_auto_stacked_menu_bar_459C9D(short ypos, short redFillLength, short normalFillLength, short barLength, BarConfig *pBarConfig)
{
    Menu_render_fixedx_auto_stacked_menu_bar_impl_46912D(
        &gMenuPrimBuffer_7265E0,
        ypos,
        redFillLength,
        normalFillLength,
        barLength,
        pBarConfig);
}
MGS_FUNC_IMPLEX(0x459C9D, Menu_render_fixed_xpos_auto_stacked_menu_bar_459C9D, MENU_IMPL);

void CC Menu_menu_bars_draw_snake_life_and_O2_4693D5(MenuPrimBuffer* ot, MenuMan_MenuBars *pField200)
{
    gSnakeLifeYPos_7339D4 = pField200->field_4_bar_y;

    BarConfig* pLifeBarText = &gSnakeLifeBarConfig_6757F0;

    if (game_state_dword_72279C.flags & 0x2000000)
    {
        // Sets hi byte
        game_state_dword_72279C.mParts.flags3 &= 0xFDu;
        gTakeDamageCounter_dword_7339D8 = 8;
    }

    if (gTakeDamageCounter_dword_7339D8 > 0)
    {
        --gTakeDamageCounter_dword_7339D8;
        // Flag causes MenuBar text to render in red
        pLifeBarText = SetPointerFlag(&gSnakeLifeBarConfig_6757F0);
    }

    Menu_render_life_bar_468DA6(
        ot,
        pField200->field_2_bar_x,
        pField200->field_4_bar_y,
        pField200->field_6_snake_hp,
        gSnakeCurrentHealth_78E7F6,
        gSnakeMaxHealth_78E7F8,
        pLifeBarText);

    if (pField200->field_1_O2_hp)
    {
        Menu_render_life_bar_468DA6(
            ot,
            pField200->field_2_bar_x,
            pField200->field_4_bar_y + 12,
            gSnakeCurrentO2_995348,
            gSnakeCurrentO2_995348,
            1024,
            &gSnakeO2BarConfig_675800);
    }
}
MGS_FUNC_IMPLEX(0x4693D5, Menu_menu_bars_draw_snake_life_and_O2_4693D5, MENU_IMPL);

void CC Menu_menu_bars_update_469215(MenuMan* pMenu, int* /*ot*/)
{
    MenuMan_MenuBars* pField_200 = &pMenu->field_200_hp_bars_info;
    signed int bHpChanged = Menu_menu_bars_update_field200_46938A(&pMenu->field_200_hp_bars_info);

    if (gSnakeCurrentO2_995348 < 1024)
    {
        pMenu->field_200_hp_bars_info.field_1_O2_hp = 150;
        bHpChanged = 1;
    }

    if (!pMenu->field_2A_bSkipUpdateHpBars)
    {
        if (game_state_dword_72279C.flags & 0x10000)
        {
            pField_200->field_0_state = 3;
        }

        const char state_copy = pField_200->field_0_state;
        if ((!pField_200->field_0_state || state_copy == 3)
            && (bHpChanged || game_state_dword_72279C.flags & 0x8000 || gSnakeCurrentHealth_78E7F6 <= gSnakeMaxHealth_78E7F8 / 2))
        {
            if (!state_copy)
            {
                pMenu->field_200_hp_bars_info.field_4_bar_y = -48;
            }
            pField_200->field_0_state = 1;
            gTakeDamageCounter_dword_7339D8 = 0;
        }
        if (pField_200->field_0_state)
        {
            if (pField_200->field_0_state == 1)     // animate in the life bar?
            {
                pMenu->field_200_hp_bars_info.field_4_bar_y += 8;
                if (pMenu->field_200_hp_bars_info.field_4_bar_y >= 16)
                {
                    pMenu->field_200_hp_bars_info.field_4_bar_y = 16;
                    pField_200->field_0_state = 2;
                    pMenu->field_200_hp_bars_info.field_8 = 150;
                }
                Menu_menu_bars_draw_snake_life_and_O2_4693D5(pMenu->field_20_prim_buffer, pField_200);
                return;
            }

            if (pField_200->field_0_state == 2)
            {
                if (bHpChanged || gSnakeCurrentHealth_78E7F6 <= gSnakeMaxHealth_78E7F8 / 2 || game_state_dword_72279C.flags & 0x8000)
                {
                    pMenu->field_200_hp_bars_info.field_8 = 150;
                    if (pMenu->field_200_hp_bars_info.field_1_O2_hp > 0)
                    {
                        pMenu->field_200_hp_bars_info.field_1_O2_hp -= 1;
                    }
                }
                else if (--pMenu->field_200_hp_bars_info.field_8 <= 0)
                {
                    pField_200->field_0_state = 3;
                }
                Menu_menu_bars_draw_snake_life_and_O2_4693D5(pMenu->field_20_prim_buffer, pField_200);
                return;
            }

            if (pField_200->field_0_state != 3)
            {
                if (pField_200->field_0_state == 4)
                {
                    pField_200->field_0_state = 0;
                    pMenu->field_200_hp_bars_info.field_4_bar_y = -48;
                }
                Menu_menu_bars_draw_snake_life_and_O2_4693D5(pMenu->field_20_prim_buffer, pField_200);
                return;
            }

            pMenu->field_200_hp_bars_info.field_4_bar_y -= 8;
            if (pMenu->field_200_hp_bars_info.field_4_bar_y > -48)
            {
                Menu_menu_bars_draw_snake_life_and_O2_4693D5(pMenu->field_20_prim_buffer, pField_200);
                return;
            }
            
            pField_200->field_0_state = 0;
            pMenu->field_200_hp_bars_info.field_4_bar_y = -48;
            
            if (game_state_dword_72279C.flags & 0x10000)
            {
                game_state_dword_72279C.flags = game_state_dword_72279C.flags & 0xFFFEFFFF | 0x20000;
            }

            if (gSnakeCurrentO2_995348 == 1024)
            {
                pMenu->field_200_hp_bars_info.field_1_O2_hp = 0;
            }
        }
    }
}
MGS_FUNC_IMPLEX(0x469215, Menu_menu_bars_update_469215, MENU_IMPL);

void CC Menu_init_menu_bars_4691CE(MenuMan* pMenu)
{
    pMenu->field_28_flags |= 1u;
    pMenu->m7FnPtrs_field_2C[0] = Menu_menu_bars_update_469215;

    pMenu->field_200_hp_bars_info.field_0_state = 0;
    pMenu->field_200_hp_bars_info.field_1_O2_hp = 0;
    pMenu->field_200_hp_bars_info.field_6_snake_hp = gSnakeCurrentHealth_78E7F6;
    pMenu->field_200_hp_bars_info.field_A_k10_decrement = 10;
    pMenu->field_200_hp_bars_info.field_2_bar_x = 16;
    pMenu->field_200_hp_bars_info.field_4_bar_y = -48;

    gSnakeLifeYPos_7339D4 = -48;
}
MGS_FUNC_IMPLEX(0x4691CE, Menu_init_menu_bars_4691CE, MENU_IMPL);

MGS_ARY(1, 0x66C480, TMenuFn, 9, gMenuFuncs_inits_66C480,
{
    Menu_init_fn0_46AD91.Ptr(),
    Menu_init_radar_468358,
    Menu_init_codec_463746,
    Menu_init_inventory_left_469E77.Ptr(),
    Menu_init_inventory_right_4694E4.Ptr(),
    Menu_init_menu_bars_4691CE,
    Menu_init_fn6_468406.Ptr(),
    Menu_init_fn7_jimaku_font_buffer_size_sub_462CFC.Ptr(),
    nullptr
});

void CC Menu_TextReset_459ACE()
{
    gTextConfig_66C4C0.gTextFlags_dword_66C4C8 = 0;
    gTextConfig_66C4C0.gTextRGB_dword_66C4CC = 0x64808080;
    gMenuPrimBuffer_7265E0.mFreeLocation = gMenuPrimBufferArrays_7265EC[gActiveBuffer_dword_791A08];
    gMenuPrimBuffer_7265E0.mOtEnd = gMenuPrimBuffer_7265E0.mFreeLocation + 8192; // sizeof gMenuPrimArray1_7269F4/gMenuPrimArray2_7289F4
    gMenuPrimBuffer_7265E0.mOt = gLibGvStruct2_6BC558.mOrderingTables[gActiveBuffer_dword_791A08];
}
MGS_FUNC_IMPLEX(0x00459ACE, Menu_TextReset_459ACE, MENU_IMPL);

void CC Menu_create_helper_459991(MenuMan* pMenu)
{
    pMenu->field_2A_bSkipUpdateHpBars = 0;
    pMenu->field_29 = 0;
    pMenu->field_28_flags = 0;
    pMenu->field_20_prim_buffer = &gMenuPrimBuffer_7265E0;

    gMenuPrimBufferArrays_7265EC[0] = &gMenuPrimArray1_7269F4[0];
    gMenuPrimBufferArrays_7265EC[1] = &gMenuPrimArray2_7289F4[0];

    DRAWENV drEnv = {};
    Renderer_DRAWENV_Init_401888(&drEnv, 0, 0, 320, 224);
    drEnv.isbg = 0;
    drEnv.texturePage = 31;
    SetDrawEnv_40DDE0(&pMenu->mDR_ENV_field_48[0], &drEnv);
    
    Renderer_DRAWENV_Init_401888(&drEnv, 320, 0, 320, 224);
    drEnv.isbg = 0;
    drEnv.texturePage = 31;
    SetDrawEnv_40DDE0(&pMenu->mDR_ENV_field_48[1], &drEnv);

    Menu_create_helper_item_file_46B8CA("item");

    TMenuFn* fnPtr = &gMenuFuncs_inits_66C480[0];
    while (*fnPtr)
    {
        (*fnPtr)(pMenu);
        ++fnPtr;
    }
}
MGS_FUNC_IMPLEX(0x00459991, Menu_create_helper_459991, MENU_IMPL);

void CC TextSetDefaults_459B51()
{
    gTextConfig_66C4C0.gTextFlags_dword_66C4C8 = 0;
    gTextConfig_66C4C0.gTextRGB_dword_66C4CC = 0x64808080;
}
MGS_FUNC_IMPLEX(0x00459B51, TextSetDefaults_459B51, MENU_IMPL);

void CC Menu_inventory_text_4689CB(MenuMan* pMenu, int ot, int xpos, int ypos, const char* pText, int textFlags)
{
    TextConfig textConfig = {};
    textConfig.gTextX_dword_66C4C0 = xpos;
    textConfig.gTextY_dword_66C4C4 = ypos;
    textConfig.gTextFlags_dword_66C4C8 = textFlags;
    textConfig.gTextRGB_dword_66C4CC = 0x64808080;
    Render_Text_NotFlag0x10_468642(pMenu->field_20_prim_buffer, &textConfig, pText);
}
MGS_FUNC_IMPLEX(0x004689CB, Menu_inventory_text_4689CB, MENU_IMPL);

MGS_FUNC_NOT_IMPL(0x468529, void __cdecl(MenuPrimBuffer *pPrimBuffer, TextConfig *pTextConfig, signed int number), Menu_render_number_as_string_468529);

MGS_VAR(1, 0x7339C0, SPRT, gMenu_font1_template_sprite_7339C0, {});

void CC Menu_render_text_fractional_468915(MenuMan* pMenu, int x, int y, signed int currentValue, signed int maxValue)
{
    // Renders as text: "currentValue/maxValue"

    TextConfig textConfig = {};
    textConfig.gTextFlags_dword_66C4C8 = 0;
    textConfig.gTextX_dword_66C4C0 = x;
    textConfig.gTextY_dword_66C4C4 = y;
    textConfig.gTextRGB_dword_66C4CC = currentValue != 0 ? 0x64575757 : 0x64002080;

    // Render first number
    Menu_render_number_as_string_468529(pMenu->field_20_prim_buffer, &textConfig, currentValue);

    // Render dividing backslash
    SPRT* pSprt = PrimAlloc<SPRT>(pMenu->field_20_prim_buffer);
    memcpy(pSprt, &gMenu_font1_template_sprite_7339C0, sizeof(SPRT));

    setRGB0(pSprt, BYTE0(textConfig.gTextRGB_dword_66C4CC), BYTE1(textConfig.gTextRGB_dword_66C4CC), BYTE2(textConfig.gTextRGB_dword_66C4CC));
    pSprt->x0 = textConfig.gTextX_dword_66C4C0;
    pSprt->y0 = textConfig.gTextY_dword_66C4C4;
    pSprt->u0 = 224;
    addPrim(pMenu->field_20_prim_buffer->mOt, pSprt);
    textConfig.gTextX_dword_66C4C0 += 6;

    // Render 2nd number
    Menu_render_number_as_string_468529(pMenu->field_20_prim_buffer, &textConfig, maxValue);
}
MGS_FUNC_IMPLEX(0x00468915, Menu_render_text_fractional_468915, MENU_IMPL);

void CC Menu_update_4598BC(MenuMan* pMenu)
{
    int* pOtText1 = (int*)gMenuPrimBuffer_7265E0.mOt;
    int* pOtText2 = (int*)gMenuPrimBuffer_7265E0.mOt;
    pMenu->field_24_input = gpActiveButtons_995324 + 2;

    Menu_update_helper_462A3D(pMenu, pOtText2);

    if (!(gActorPauseFlags_dword_791A0C & 2)
        && gLoaderState_dword_9942B8 > 0
        && !script_cancel_non_zero_dword_7227A0
        && game_state_dword_72279C.flags >= 0)
    {
        const int field_28_flags = pMenu->field_28_flags;
        int flags = 1;
        for (int i = 0; i < 7; i++)
        {
            if (flags & field_28_flags)
            {
                // 0 = life bars
                // 1 = right side inventory
                // 2 = left side inventory
                // 3 = radar
                // 4 = enter/exit codec
                // 5/6 = not set by default
                pMenu->m7FnPtrs_field_2C[i](pMenu, pOtText1);
            }
            flags *= 2; // To the next bit
        }
    }

    // 11 chars
    Menu_render_text_fractional_468915(pMenu, 40, 50, 50, 800);

    // drawing environment change primitive
    addPrim(pOtText2, &pMenu->mDR_ENV_field_48[gActiveBuffer_dword_791A08]);
}
MGS_FUNC_IMPLEX(0x004598BC, Menu_update_4598BC, MENU_IMPL);

void CC Menu_shutdown_fn1_4683F3(MenuMan* pMenu)
{
    pMenu->field_28_flags &= 0xF7u;
}
MGS_FUNC_IMPLEX(0x004683F3, Menu_shutdown_fn1_4683F3, MENU_IMPL);

void CC Menu_shutdown_radar_4657E6(MenuMan *pMenu)
{
    pMenu->field_28_flags &= 0xEFu;
}
MGS_FUNC_IMPLEX(0x004657E6, Menu_shutdown_radar_4657E6, MENU_IMPL);

void CC Menu_shutdown_codec_46AD80(MenuMan *pMenu)
{
    pMenu->field_28_flags &= 0xFBu;
}
MGS_FUNC_IMPLEX(0x0046AD80, Menu_shutdown_codec_46AD80, MENU_IMPL);

void CC Menu_shutdown_fn4_469E26(MenuMan* pMenu)
{
    pMenu->field_28_flags &= 0xFDu;
}
MGS_FUNC_IMPLEX(0x00469E26, Menu_shutdown_fn4_469E26, MENU_IMPL);

void CC Menu_shutdown_fn5_469476(MenuMan* pMenu)
{
    pMenu->field_28_flags &= 0xFEu;
}
MGS_FUNC_IMPLEX(0x00469476, Menu_shutdown_fn5_469476, MENU_IMPL);

void Menu_shutdown_fn6_nullsub_131(MenuMan*)
{

}

MGS_ARY(1, 0x66C4A4, TMenuFn, 7, gMenuFuncs_shutdown_66C4A4,
{
    Menu_shutdown_fn1_4683F3,
    Menu_shutdown_radar_4657E6,
    Menu_shutdown_codec_46AD80,
    Menu_shutdown_fn4_469E26,
    Menu_shutdown_fn5_469476,
    Menu_shutdown_fn6_nullsub_131,
    nullptr
});

void Menu_shutdown_459971(MenuMan* pMenu)
{
    TMenuFn* fnPtr = &gMenuFuncs_shutdown_66C4A4[0];
    while (*fnPtr)
    {
        (*fnPtr)(pMenu);
    }
}
MGS_FUNC_IMPLEX(0x00459971, Menu_shutdown_459971, MENU_IMPL);

void CC Menu_create_459891()
{
    Actor_Init_40A347(&gMenuMan_stru_725FC0.mBase,
        (TActorFunction)Menu_update_4598BC,
        (TActorFunction)Menu_shutdown_459971,
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
