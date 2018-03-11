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
#include "ResourceNameHash.hpp"
#include "Renderer.hpp"
#include "pcx.hpp"
#include "Font.hpp"
#include "Actor_Rank.hpp"

#define MENU_IMPL true

struct MenuPrimBuffer
{
    BYTE* mFreeLocation;
    BYTE* mOt;
    BYTE* mOtEnd;
};

struct MenuMan;
using TMenuUpdateFn = void(CC*)(MenuMan*, DWORD*);

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

struct Menu_Item_Unknown_Array_Item
{
    short field_0_item_id_idx;
    short field_2_current_amount;
    short field_4; // max amount ?
    short field_6;
};
MGS_ASSERT_SIZEOF(Menu_Item_Unknown_Array_Item, 0x8);

struct MenuMan_Inventory_Sub
{
    Menu_Item_Unknown_Array_Item field_0;
    struct MenuMan_Inventory_Unk_6764F8* field_8_pMenuMan_Inventory_Unk_6764F8;
};
MGS_ASSERT_SIZEOF(MenuMan_Inventory_Sub, 0xC);

struct Menu_Item_Unknown;

struct MenuMan_Inventory_Menu_0x14
{
    MenuMan_Inventory_Sub field_0_invent;
    Menu_Item_Unknown* field_C_pItem_sys_alloc;
    char field_10_state;
    char field_11_item_idx;
    short field_12;
};
MGS_ASSERT_SIZEOF(MenuMan_Inventory_Menu_0x14, 0x14);

// TODO: Discover true size of structure/other data
struct MenuMan
{
    Actor mBase;
    MenuPrimBuffer* field_20_prim_buffer;
    ButtonStates* field_24_input;
    BYTE field_28_flags;
    BYTE field_29;
    BYTE field_2A_state;
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
    MenuMan_Inventory_Menu_0x14 field_1D8_invetory_menus[2];
    MenuMan_MenuBars field_200_hp_bars_info;
    WORD field_20C_codec_state;
    WORD field_20E;
    DWORD field_210_size_19F2_font;
    DWORD field_214_108bytes;
    DWORD field_218;
};
MGS_ASSERT_SIZEOF(MenuMan, 0x21C);


MGS_VAR(1, 0x722788, Texture_Record, gMenuTexture_A0BE_722788, {});
MGS_VAR(1, 0x725FC0, MenuMan, gMenuMan_stru_725FC0, {});


MGS_VAR(1, 0x9942C0, DWORD, gMenuRightBits_dword_9942C0, 0);
MGS_VAR(1, 0x9942BC, DWORD, gMenuLeftBits_dword_9942BC, 0);

void CC Res_MenuMan_create_459A9A()
{
    Actor_PushBack_40A2AF(1, &gMenuMan_stru_725FC0.mBase, nullptr);
    Actor_Init_40A347(&gMenuMan_stru_725FC0.mBase, nullptr, nullptr, "C:\\mgs\\source\\Menu\\menuman.c");
}
MGS_FUNC_IMPLEX(0x00459A9A, Res_MenuMan_create_459A9A, MENU_IMPL);

MGS_FUNC_NOT_IMPL(0x462A3D, void __cdecl(MenuMan* pMenu, DWORD* ot), Menu_update_helper_462A3D);


struct Menu_rpk_item
{
    char field_0_x;
    char field_1_y;
    char field_2_w;
    char field_3_h;
    BYTE* field_4_pixel_ptr;
};
MGS_ASSERT_SIZEOF(Menu_rpk_item, 0x8);

MGS_VAR(1, 0x733C88, Menu_rpk_item**, gItemFile_table_dword_733C88, nullptr);

template<class T, class Y>
inline T DataAfterStructure(Y pStructure)
{
    return reinterpret_cast<T>(&pStructure[1]);
}

template<class T, class U>
inline void OffsetToPointer(T basePointer, U* offset)
{
    DWORD byteBasePtr = reinterpret_cast<DWORD>(basePointer);
    DWORD* dwOffsetPtr = reinterpret_cast<DWORD*>(offset);
    *dwOffsetPtr += byteBasePtr;
}

Menu_rpk_item* CC Menu_Get_Radar_image_46B920(int idx)
{
    return gItemFile_table_dword_733C88[idx];
}
MGS_FUNC_IMPLEX(0x46B920, Menu_Get_Radar_image_46B920, MENU_IMPL);

Menu_rpk_item* CC Menu_inventory_left_update_get_item_file_46B912(int idx)
{
    return gItemFile_table_dword_733C88[idx];
}
MGS_FUNC_IMPLEX(0x46B912, Menu_inventory_left_update_get_item_file_46B912, MENU_IMPL);

void CC Menu_create_helper_item_file_46B8CA(const char* pFileName)
{
    struct RpkHeader
    {
        BYTE mCount1;
        BYTE mCount2;
        WORD mPadding;
    };
    MGS_ASSERT_SIZEOF(RpkHeader, 0x4);

    const DWORD hashedFileName = HashFileName_40A5A2(pFileName, 'r'); // item.rpk
    RpkHeader* pHeader = LibGV_FindFile_40A603_T<RpkHeader*>(hashedFileName);
    if (pHeader)
    {
        const int count = pHeader->mCount1 + pHeader->mCount2;
        Menu_rpk_item** pTable = DataAfterStructure<Menu_rpk_item**>(pHeader);
        Menu_rpk_item** pLast = &pTable[count];
        for (int i = 0; i < count; i++)
        {
            OffsetToPointer(pLast, &pTable[i]);
        }
        gItemFile_table_dword_733C88 = pTable;
    }
}
MGS_FUNC_IMPLEX(0x46B8CA, Menu_create_helper_item_file_46B8CA, MENU_IMPL);

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

MGS_FUNC_NOT_IMPL(0x463763, void __cdecl(MenuMan*, DWORD*), Menu_codec_update_463763);


MGS_FUNC_NOT_IMPL(0x465B38, void __cdecl(), Menu_init_radar_helper_465B38);
MGS_FUNC_NOT_IMPL(0x465A01, void* __cdecl(int), Menu_scale_465A01); // TODO



MGS_VAR(1, 0x733950, void*, gFn_radar_dword_733950, 0);


MGS_FUNC_NOT_IMPL(0x468264, void __cdecl(MenuMan *pMenu, int buffer), Menu_radar_helper_468264);
MGS_FUNC_NOT_IMPL(0x465B4F, DWORD* __cdecl (MenuMan *pMenu, DWORD *a2), Menu_radar_update_helper_465B4F);

void CC Menu_radar_update_468158(MenuMan* pMenu, DWORD* ot)
{
    if (pMenu->field_1D4)
    {
        int field_1D2 = 0;
        if (!pMenu->field_2A_state)
        {
            if (game_state_dword_72279C.flags & 0x200000)
            {
                field_1D2 = pMenu->field_1D2 - 16;
                if (field_1D2 <= -64)
                {
                    field_1D2 = -64;
                    game_state_dword_72279C.flags |= 0x400000u;
                    game_state_dword_72279C.flags &= 0xFFDFFFFF;
                }
            }
            else if (game_state_dword_72279C.flags & 0x100000)
            {
                field_1D2 = pMenu->field_1D2 + 16;
                game_state_dword_72279C.flags &= 0xFFBFFFFF;
                if (field_1D2 >= 0)
                {
                    field_1D2 = 0;
                    game_state_dword_72279C.flags &= 0xFFEFFFFF;
                }
            }
            else
            {
                field_1D2 = 0;
            }

            if (game_state_dword_72279C.flags & 0x400800)
            {
                pMenu->field_1D2 = static_cast<WORD>(-64);
            }
            else
            {
                pMenu->field_1D2 = static_cast<WORD>(field_1D2);
                Menu_radar_helper_468264(pMenu, gActiveBuffer_dword_791A08);
                Menu_radar_update_helper_465B4F(pMenu, ot);
            }
        }
    }
}
MGS_FUNC_IMPLEX(0x00468158, Menu_radar_update_468158, MENU_IMPL);

void CC Menu_init_radar_468358(MenuMan* pMenu)
{
    pMenu->m7FnPtrs_field_2C[3] = Menu_radar_update_468158;
    pMenu->field_28_flags |= 8;
    pMenu->field_1D4 = 1;
    pMenu->field_1D0 = 0;
    pMenu->field_1D2 = 0;
    Menu_radar_helper_468264(pMenu, 0);
    Menu_radar_helper_468264(pMenu, 1);
    memcpy(&pMenu->mDrEnvDst_field_150, &pMenu->mDR_ENV_field_48[0], sizeof(pMenu->mDrEnvDst_field_150));
    memcpy(&pMenu->mDrEnvDst_field_190, &pMenu->mDR_ENV_field_48[1], sizeof(pMenu->mDrEnvDst_field_190));
    Menu_init_radar_helper_465B38();
    gFn_radar_dword_733950 = 0;
    Menu_scale_465A01(4096);
}
MGS_FUNC_IMPLEX(0x00468358, Menu_init_radar_468358, MENU_IMPL);

void CC Menu_init_codec_463746(MenuMan* pMenu)
{
    pMenu->m7FnPtrs_field_2C[4] = Menu_codec_update_463763.Ptr();
    pMenu->field_28_flags |= 0x10u;
}
MGS_FUNC_IMPLEX(0x00463746, Menu_init_codec_463746, MENU_IMPL);

using TMenuFn = void(CC*)(MenuMan*);

MGS_VAR(1, 0x733978, SPRT, gMenu_sprt3_733978, {});

static void InitLargeFontTemplateSprite()
{
    setSprt(&gMenu_sprt3_733978);
    setRGB0(&gMenu_sprt3_733978, 0x80, 0x80, 0x80);
    gMenu_sprt3_733978.u0 = 0;
    gMenu_sprt3_733978.v0 = 242;
    gMenu_sprt3_733978.w = 8;
    gMenu_sprt3_733978.h = 6;
    gMenu_sprt3_733978.clut = 32765;
}

void CC Menu_get_item_file_item_46B92E(struct MenuMan_Inventory_14h_Unk* pMenuUnk, int img_idx, int pal_idx)
{
    WORD* palPtr = (WORD *)&gItemFile_table_dword_733C88[pal_idx]->field_4_pixel_ptr;
    Menu_rpk_item* pItem = gItemFile_table_dword_733C88[img_idx];
    pMenuUnk->field_9_x = pItem->field_0_x - 2;
    pMenuUnk->field_A_y = pItem->field_1_y - 2;
    pMenuUnk->field_10_w = 4 * pItem->field_2_w;
    pMenuUnk->field_4_word_ptr_pixels = palPtr;
    pMenuUnk->field_12_h = pItem->field_3_h;
    pMenuUnk->field_0_pixels = (BYTE*)&pItem->field_4_pixel_ptr;
}
MGS_FUNC_IMPLEX(0x0046B92E, Menu_get_item_file_item_46B92E, MENU_IMPL);

void CC Menu_init_num_res_font_helper_468A04()
{
    static PSX_RECT local_static_stru_6757E0 = { 960, 498, 0, 0 };

    MenuMan_Inventory_14h_Unk menu_unk = {};
    Menu_get_item_file_item_46B92E(&menu_unk, 45, 44);
    local_static_stru_6757E0.x2 = menu_unk.field_10_w / 4;
    local_static_stru_6757E0.y2 = menu_unk.field_12_h;
    g_Render_sub_41C640_ret_650D1A = Render_sub_41C640(
        &local_static_stru_6757E0,
        menu_unk.field_4_word_ptr_pixels,
        menu_unk.field_0_pixels,
        0,
        0,
        0,
        0);

    Render_sub_41C6B0(&local_static_stru_6757E0, menu_unk.field_0_pixels);

    InitLargeFontTemplateSprite();
}
MGS_FUNC_IMPLEX(0x00468A04, Menu_init_num_res_font_helper_468A04, MENU_IMPL);

struct Psx_Tim_Header
{
    DWORD field_0_magic;
    DWORD field_4_type;
    DWORD field_8_offset_to_image_data;
    WORD field_C_pal_xorg;
    WORD field_E_pal_yorg;
    WORD field_10_num_pal_colours;
    WORD field_12_num_pals;
};
MGS_ASSERT_SIZEOF(Psx_Tim_Header, 0x14);

MGS_VAR(1, 0x733960, SPRT, gMenu_sprt2_733960, {});
MGS_VAR(1, 0x7339C0, SPRT, gMenu_font1_template_sprite_7339C0, {});

static void InitSmallFontTemplateSprite()
{
    // TODO: SetSprt
    gMenu_sprt2_733960.tag = gMenu_sprt2_733960.tag & 0xFFFFFF | 0x4000000;
    gMenu_sprt2_733960.code = 0x64;

    // TODO: SetRGB0
    gMenu_sprt2_733960.r0 = 128;
    gMenu_sprt2_733960.g0 = 128;
    gMenu_sprt2_733960.b0 = 128;

    gMenu_sprt2_733960.u0 = 0;
    gMenu_sprt2_733960.v0 = 237;
    gMenu_sprt2_733960.w = 6;
    gMenu_sprt2_733960.h = 5;
    gMenu_sprt2_733960.clut = 32764;
}

void CC Menu_init_num_res_font_468406(MenuMan* /*pMenu*/)
{
    PSX_RECT rect;
    rect.x1 = 960;
    rect.y1 = 488;
    rect.x2 = 64;
    rect.y2 = 10;

    Psx_Tim_Header* pFileData = (Psx_Tim_Header *)LibGV_FindFile_40A603(HashFileName_40A5A2("num", 'r')); // num.res
    const unsigned int imageDataOffset = (unsigned int)pFileData->field_8_offset_to_image_data >> 1;
    BYTE* pImageHeader = (BYTE *)&pFileData[1] + 2 * imageDataOffset;
    WORD* pPalStart = (WORD*)&pFileData[1].field_0_magic;
    *pPalStart = 0;       // set first palt pixel to black?

    g_Render_sub_41C640_ret_650D1A = Render_sub_41C640(&rect, (WORD *)&pFileData[1], pImageHeader, 0, 0, 0, 0);
    Render_sub_41C6B0(&rect, pImageHeader);

    gMenu_font1_template_sprite_7339C0.tag = gMenu_font1_template_sprite_7339C0.tag & 0xFFFFFF | 0x4000000;
    gMenu_font1_template_sprite_7339C0.code = 0x64; // setSprt
    gMenu_font1_template_sprite_7339C0.r0 = 128;
    gMenu_font1_template_sprite_7339C0.g0 = 128;
    gMenu_font1_template_sprite_7339C0.b0 = 128;
    gMenu_font1_template_sprite_7339C0.u0 = 156;
    gMenu_font1_template_sprite_7339C0.v0 = 232;
    gMenu_font1_template_sprite_7339C0.w = 6;
    gMenu_font1_template_sprite_7339C0.h = 7;
    gMenu_font1_template_sprite_7339C0.clut = 32764;

    InitSmallFontTemplateSprite();


    Menu_init_num_res_font_helper_468A04();
}
MGS_FUNC_IMPLEX(0x00468406, Menu_init_num_res_font_468406, MENU_IMPL);

MGS_VAR(1, 0x733CF4, DWORD, flags_dword_733CF4, 0);
MGS_VAR(1, 0x733DD0, DWORD, gBitFlags_dword_733DD0, 0);

struct Menu_stru_0x18
{
    BYTE field_0_u;
    BYTE field_1_v;
    WORD field_2_clut;
    MenuMan_Inventory_14h_Unk* field_4_pUnk;
    PSX_RECT field_8_rect;
    WORD field_10;
    WORD field_12;
    WORD field_14;
    WORD field_16;
};
MGS_ASSERT_SIZEOF(Menu_stru_0x18, 0x18);

MGS_ARY(1, 0x733CF8, Menu_stru_0x18, 9, gMenu_stru_733CF8, {});
MGS_ARY(1, 0x733C90, Menu_stru_0x18, 4, stru_733C90, {});

void CC Menu_init_fn0_46AD91(MenuMan*)
{
    int pos1 = 0;
    int pos2 = 0;
    Menu_stru_0x18* pItem = &gMenu_stru_733CF8[0];
    char field_1_pos = 128;
    for (int cnt_3 = 0; cnt_3<3; cnt_3++)
    {
        signed int field_0_pos = 64;
        pos2 = pos1 + 4;
        for (int innerCount = 0; innerCount<3; innerCount++)
        {
            pItem->field_0_u = field_0_pos;
            pItem->field_1_v = field_1_pos;
            field_0_pos += 64;
            pItem->field_8_rect.x2 = 16;
            pItem->field_8_rect.y2 = 24;
            pItem->field_14 = 16;
            pItem->field_16 = 1;

            const int field_12 = pos2 % 8 + 480;
            pItem->field_2_clut = ((short)field_12 << 6) | (16 * (pos2 / 8 + 60) >> 4) & 0x3F;
            pItem->field_10 = 16 * (pos2 / 8 + 60);
            pItem->field_8_rect.x1 = ((pos1 % 3 + 1) << 6) / 4 + 960;
            pItem->field_8_rect.y1 = 24 * (pos1 / 3 + 16);
            pItem->field_12 = field_12;
            pItem->field_4_pUnk = nullptr;

            ++pItem;
            ++pos1;
            ++pos2;
        }
        field_1_pos += 24;
    }

    flags_dword_733CF4 = 0;
    gBitFlags_dword_733DD0 = 0;

    signed int kStart16 = 16;
    for (int cnt_4 = 0; cnt_4<4; cnt_4++)
    {
        stru_733C90[cnt_4].field_0_u = 0;
        stru_733C90[cnt_4].field_1_v = kStart16 * 24;
        stru_733C90[cnt_4].field_2_clut = (((unsigned __int16)(cnt_4 % 8) + 480) << 6) | (16 * (cnt_4 / 8 + 60) >> 4) & 0x3F;// tpage ?
        stru_733C90[cnt_4].field_8_rect.x1 = 960;
        stru_733C90[cnt_4].field_8_rect.y1 = 8 * (3 * cnt_4 + 48);
        stru_733C90[cnt_4].field_8_rect.x2 = 16;
        stru_733C90[cnt_4].field_8_rect.y2 = 24;
        stru_733C90[cnt_4].field_10 = 16 * (cnt_4 / 8 + 60);
        stru_733C90[cnt_4].field_12 = cnt_4 % 8 + 480;
        stru_733C90[cnt_4].field_14 = 16;
        stru_733C90[cnt_4].field_16 = 1;
        stru_733C90[cnt_4].field_4_pUnk = nullptr;
        ++kStart16;
    }
}
MGS_FUNC_IMPLEX(0x46AD91, Menu_init_fn0_46AD91, MENU_IMPL);

struct Menu_Item_Unknown_Array_Item;

struct MenuMan_Inventory_Unk_6764F8
{
    using Fn = void(CC*)(MenuMan* pMenu, DWORD* ot, DWORD xpos, DWORD ypos, Menu_Item_Unknown_Array_Item * pSub);
    using Fn2 = void(CC*)(MenuMan_Inventory_Unk_6764F8* pUnk, int bBlendXPos, int* pXPos, int* pYPos);

    WORD field_0;
    WORD field_2;
    DWORD field_4_buttons;
    DWORD field_8;
    DWORD field_C;
    Fn2 field_10_fn_ptrs[2];
    Fn field_18_fn;
};
MGS_ASSERT_SIZEOF(MenuMan_Inventory_Unk_6764F8, 0x1C);

MGS_VAR(1, 0x733CF0, DWORD, g64_to_256_counter_dword_733CF0, 0);

void CC Menu_46B36B(MenuMan_Inventory_Unk_6764F8* pUnk, int bBlendXPos, int* pXPos, int* pYPos)
{
    int field_0 = pUnk->field_0;
    int field_2 = pUnk->field_2;
    if (bBlendXPos)
    {
        *pXPos = field_0 + 56 * bBlendXPos * g64_to_256_counter_dword_733CF0 / 0x10000;
    }
    else
    {
        *pXPos = field_0;
    }
    *pYPos = field_2;
}
MGS_FUNC_IMPLEX(0x0046B36B, Menu_46B36B, MENU_IMPL);

void CC Menu_46B2E0(MenuMan_Inventory_Unk_6764F8* pUnk, int bBlendXPos, int *pXPos, int *pYPos)
{
    int field_0 = pUnk->field_0;
    int field_2 = pUnk->field_2;
    if (bBlendXPos)
    {
        *pXPos = field_0;
        field_2 -= 40 * bBlendXPos * g64_to_256_counter_dword_733CF0 / 0x10000;
    }
    else
    {
        *pXPos = field_0;
    }
    *pYPos = field_2;
}
MGS_FUNC_IMPLEX(0x0046B2E0, Menu_46B2E0, MENU_IMPL);

void CC Menu_46B326(MenuMan_Inventory_Unk_6764F8* pUnk, int bBlendXPos, int *pXPos, int *pYPos)
{
    int field_0 = pUnk->field_0;
    int field_2 = pUnk->field_2;
    if (bBlendXPos)
    {
        field_0 -= 56 * bBlendXPos * g64_to_256_counter_dword_733CF0 / 0x10000;
    }
    *pXPos = field_0;
    *pYPos = field_2;
}
MGS_FUNC_IMPLEX(0x0046B326, Menu_46B326, MENU_IMPL);

MGS_ARY(1, 0x6764F8, MenuMan_Inventory_Unk_6764F8, 2, stru_6764F8,
{
    { 16, 184, 1, 24576, 36864 ,{ Menu_46B36B, Menu_46B2E0 }, nullptr },
    { 256, 184, 2, 12288, 49152,{ Menu_46B2E0, Menu_46B326 }, nullptr }
});

void CC Menu_inventory_right_update_4697F3(MenuMan* pMenu, DWORD* pPrimBuffer)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
}
MGS_FUNC_IMPLEX(0x004697F3, Menu_inventory_right_update_4697F3, false); // TODO

void CC Menu_inventory_common_invoke_handler_46B71E(MenuMan* pMenu, DWORD* ot, MenuMan_Inventory_Menu_0x14 * pInventorySubStruct, int counter, int kZero)
{
    pInventorySubStruct->field_0_invent.field_8_pMenuMan_Inventory_Unk_6764F8->field_18_fn(
        pMenu,
        ot,
        counter + pInventorySubStruct->field_0_invent.field_8_pMenuMan_Inventory_Unk_6764F8->field_0,
        kZero + pInventorySubStruct->field_0_invent.field_8_pMenuMan_Inventory_Unk_6764F8->field_2,
        &pInventorySubStruct->field_0_invent.field_0);
}
MGS_FUNC_IMPLEX(0x0046B71E, Menu_inventory_common_invoke_handler_46B71E, MENU_IMPL);

MGS_VAR(1, 0x993FF8, WORD, word_993FF8, 0);
MGS_VAR(1, 0x733DD4, MenuMan_Inventory_Unk_6764F8*, gMenu_dword_733DD4, 0);

signed int CC Menu_inventory_common_can_open_menu_46B745(MenuMan_Inventory_Menu_0x14 * pInventUnk, ButtonStates* pInput)
{
    if (word_993FF8 & 0x101
        || (game_state_dword_72279C.flags & 0x1020) == 0x20
        || !(pInput->field_0_button_status & pInventUnk->field_0_invent.field_8_pMenuMan_Inventory_Unk_6764F8->field_4_buttons))
    {
        return 0;
    }
    gMenu_dword_733DD4 = pInventUnk->field_0_invent.field_8_pMenuMan_Inventory_Unk_6764F8;
    return 1;
}
MGS_FUNC_IMPLEX(0x0046B745, Menu_inventory_common_can_open_menu_46B745, MENU_IMPL);

signed int CC Menu_inventory_common_can_close_menu_46B77E(MenuMan_Inventory_Menu_0x14 * pInventUnk, ButtonStates* pInput)
{
    if (pInput->field_0_button_status & pInventUnk->field_0_invent.field_8_pMenuMan_Inventory_Unk_6764F8->field_4_buttons)
    {
        return 0;
    }
    gMenu_dword_733DD4 = 0;
    return 1;
}
MGS_FUNC_IMPLEX(0x0046B77E, Menu_inventory_common_can_close_menu_46B77E, MENU_IMPL);

int Menu_inventory_common_update_helper_46B29C()
{
    return g64_to_256_counter_dword_733CF0;
}
MGS_FUNC_IMPLEX(0x0046B29C, Menu_inventory_common_update_helper_46B29C, MENU_IMPL);

void Menu_inventory_common_46B294()
{
    g64_to_256_counter_dword_733CF0 = 0;
}
MGS_FUNC_IMPLEX(0x0046B294, Menu_inventory_common_46B294, MENU_IMPL);

signed int Menu_inventory_common_46B2C2()
{
    g64_to_256_counter_dword_733CF0 -= 64;
    if (g64_to_256_counter_dword_733CF0 > 0)
    {
        return 0;
    }
    g64_to_256_counter_dword_733CF0 = 0;
    return 1;
}
MGS_FUNC_IMPLEX(0x0046B2C2, Menu_inventory_common_46B2C2, MENU_IMPL);

signed int Menu_inventory_common_update_helper_46B2A2()
{
    g64_to_256_counter_dword_733CF0 += 64;
    if (g64_to_256_counter_dword_733CF0 < 256)
    {
        return 0;
    }
    g64_to_256_counter_dword_733CF0 = 256;
    return 1;
}
MGS_FUNC_IMPLEX(0x0046B2A2, Menu_inventory_common_update_helper_46B2A2, MENU_IMPL);

void CC Menu_inventory_common_update_helper_46B6EF(MenuMan* pMenu, DWORD* ot, MenuMan_Inventory_Menu_0x14 * pInventSub)
{
    if ((game_state_dword_72279C.flags & 0x1020) != 0x20)
    {
        pInventSub->field_0_invent.field_8_pMenuMan_Inventory_Unk_6764F8->field_18_fn(
            pMenu, ot,
            pInventSub->field_0_invent.field_8_pMenuMan_Inventory_Unk_6764F8->field_0,
            pInventSub->field_0_invent.field_8_pMenuMan_Inventory_Unk_6764F8->field_2,
            &pInventSub->field_0_invent.field_0);
    }
}
MGS_FUNC_IMPLEX(0x0046B6EF, Menu_inventory_common_update_helper_46B6EF, MENU_IMPL);

MGS_VAR(1, 0x733DF0, Font, gMenuFont_733DF0, {});

void CC Menu_inventory_common_draw_text_46BA69(const char* pItemInfoStr)
{
    gUseTrueType_dword_6FC7AC = gNoTrueType_650D40;
    Font_set_text_45C80A(&gMenuFont_733DF0, pItemInfoStr);
    Font_render_45C76C(&gMenuFont_733DF0);
    gUseTrueType_dword_6FC7AC = 0;
}
MGS_FUNC_IMPLEX(0x0046BA69, Menu_inventory_common_draw_text_46BA69, MENU_IMPL);

const char* gItemInfos_675C90[26] =
{
    "CIGARETTES\nSolid Snake's favorite\nbrand. Smoking is hazardous\nto your health.",
    "MAGNIFYING SCOPE.\nPress <Action Key> to zoom in,\n<Crawl Key> to zoom out.",
    "CARDBOARD BOX A\n< To Heliport > \nis written on it.",
    "CARDBOARD BOX B\n< To Nuclear Warhead\nStorage Building >\nis written on it.",
    "CARDBOARD BOX C\n< To Snow Field >\nis written on it.",
    "NIGHT-VISION GOGGLE\nImage intensifier.\nAllows to see even\nin dark places.",
    "THERMAL GOGGLE\nThermal imaging system\nfor night-vision use.",
    "GAS MASK\nReduces speed of O2 gauge\ndecrease in poison\ngas environment.",
    "BODY ARMOR\nBulletproof vest.\nReduces damage upon\nimpact.",
    "KETCHUP\nItalian-tomato-based\nfood condiment.",
    "STEALTH\nRenders wearer invisible\nthrough light reflection\nsystem.",
    "BANDANA\nWords <Limitless>\nsewn on it.",
    "CAMERA\nTake photos wherever\nyou like!",
    "RATIONS\nRestores life.\nUse by pressing <Action Key>\nwhile menu is open.",
    "MEDICINE\nCold medisine.\nUse by pressing <Action Key>\nwhile menu is open.", // medisine!?
    "DIAZEPAM\nAnti-anxiety drug.\nTemporarily stops\ninvoluntary trembling.",
    "PAL CARD KEY\nEmergency input or\noverride device.",
    "ID CARD\nWhen equipped,\nopens all level %lu security\ndoors.",
    "IDENTITY UNKNOWN.\nPress <Action Key> while menu\nis open to throw.",
    "MINE DETECTOR\nWhen equipped,\nburied mines show up\non radar screen.",
    "MO DISC\nOptic disc containing\nMetal Gear exercise data.",
    "ROPE\nLong, durable rope.\nMade from nylon fibers.",
    "HANDKERCHIEF\nSniper Wolf's handkerchief.\nSmells faintly of her.",
    "SUPPRESSOR\nFor SOCOM pistol.\nMuffles report, blast\nand flash of it.",
    "RATIONS\nFrozen.\nMelt it before\nyou eat.",
    "KETCHUP\nFrozen.\nMelt it before\nyou use."
};

MGS_VAR(1, 0x721E58, DWORD, gRocketLauncherInUse_dword_721E58, 0);

struct Menu_unknown_pair
{
    BYTE field_0;
    BYTE field_1;
};
MGS_ASSERT_SIZEOF(Menu_unknown_pair, 0x2);

MGS_ARY(1, 0x669A93, Menu_unknown_pair, 10, gMenu_flags_word_669A93,
{
    { 144, 67 },
    { 144, 76 },
    { 65, 192 },
    { 32, 32 },
    { 34, 16 },
    { 65, 16 },
    { 65, 76 },
    { 65, 76 },
    { 65, 32 },
    { 146, 0 }
});

// TODO: Index -1 appears to be used
MGS_ARY(1, 0x669AAA, WORD, 27, gMenu_left_item_flags_word_669AAA,
{
    32768,
    32771,
    32769,
    32769,
    32769,
    32768,
    32768,
    32768,
    32768,
    32768,
    32768,
    32768,
    32771,
    8192,
    8192,
    8192,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0 });


DWORD CC Menu_inventory_Is_Item_Disabled_46A128(signed int item_idx)
{
    if ((!(gMenu_flags_word_669A93[gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC].field_0 & 2) // gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC can be -1
        || !(gMenu_left_item_flags_word_669AAA[item_idx] & 1))
        // Snake crouching?
        && (!(byte1_flags_word_9942A8 & 0x42) || item_idx < 2 || item_idx > 4)
        // Remote missile/stringer in use?
        && (!gRocketLauncherInUse_dword_721E58 || (item_idx < 2 || item_idx > 4) && item_idx != Items::eScope && item_idx != Items::eCamera))
    {
        return (gMenuLeftBits_dword_9942BC & (1 << item_idx));
    }
    return 1;

}
MGS_FUNC_IMPLEX(0x0046A128, Menu_inventory_Is_Item_Disabled_46A128, true);

struct Menu_Item_Info
{
    const char* field_0_name;
    DWORD field_4;
};
MGS_ASSERT_SIZEOF(Menu_Item_Info, 0x8);

const Menu_Item_Info gItemInfos_675D30[] =
{
    { "CIGS", 14 },
    { "SCOPE", 30 },
    { "C.BOX A", 17 },
    { "C.BOX B", 17 },
    { "C.BOX C", 17 },
    { "N.V.G", 15 },
    { "THERM.G", 16 },
    { "GASMASK", 19 },
    { "B.ARMOR", 18 },
    { "KETCHUP", 25 },
    { "STEALTH", 32 },
    { "BANDANA", 31 },
    { "CAMERA", 12 },
    { "RATION", 22 },
    { "MEDICINE", 21 },
    { "DIAZEPAM", 21 },
    { "PAL KEY", 23 },
    { "CARD", 27 },
    { "TIMER.B", 26 },
    { "MINE.D", 20 },
    { "DISC", 28 },
    { "ROPE", 24 },
    { "HANDKER", 29 },
    { "SUPPR.", 13 }
};

void CC Menu_inventory_left_update_ShowItemInfo_46A718(short id)
{
    const char* pItemInfoStr = gItemInfos_675C90[id];
    if (id == Items::eKetchup && gGameStates_78E7E0.gItemsAreFrozen_word_78E86A)
    {
        pItemInfoStr = gItemInfos_675C90[Items::eKetchupFrozen];
    }
    else if (id == Items::eRations && gGameStates_78E7E0.gItemsAreFrozen_word_78E86A)
    {
        pItemInfoStr = gItemInfos_675C90[Items::eRationsFrozen];
    }
    else if (id == Items::eIdCard)
    {
        // Note: Real game attempts to re-write 1 char of the string in place, this change prevents writing constant
        // strings and also allows the UI to display a card level > 9
        char buffer[256] = {};
        sprintf(buffer, pItemInfoStr, gGameStates_78E7E0.gItem_states_word_78E82A[Items::eIdCard]);
        Menu_inventory_common_draw_text_46BA69(buffer);
        return;
    }
    else if (id == Items::eMineDetector && (gGameStates_78E7E0.gDiffcultyLevel_78E7E2 == DiffcultyLevels::eHard || gGameStates_78E7E0.gDiffcultyLevel_78E7E2 == DiffcultyLevels::eExtreme))
    {
        pItemInfoStr = "MINE DETECTOR\nCannot be used in\nHARD or EXTREME mode.";
    }
    Menu_inventory_common_draw_text_46BA69(pItemInfoStr);
}
MGS_FUNC_IMPLEX(0x0046A718, Menu_inventory_left_update_ShowItemInfo_46A718, MENU_IMPL);



// Seems to render orange/red/blue PAL key icons and "NO USE" text icon
void CC Menu_inventory_left_render_PAL_key_icon_46A770(MenuMan* pMenu, DWORD* ot, int idx)
{
    Menu_rpk_item* pItem = Menu_inventory_left_update_get_item_file_46B912(2 * idx + 33);
    Menu_rpk_item* itemRpk = Menu_Get_Radar_image_46B920(2 * idx + 34);

    PSX_RECT rect = {};
    rect.x1 = 960;
    rect.y1 = 337;
    rect.x2 = itemRpk->field_2_w;
    rect.y2 = itemRpk->field_3_h;

    g_Render_sub_41C640_ret_650D1A = Render_sub_41C640(
        &rect,
        (WORD *)&pItem->field_4_pixel_ptr,
        (BYTE *)&itemRpk->field_4_pixel_ptr,
        0,
        0,
        0,
        0);
    Render_sub_41C6B0(&rect, (BYTE *)&itemRpk->field_4_pixel_ptr);

    SPRT* pSprt = PrimAlloc<SPRT>(pMenu->field_20_prim_buffer);
    setSprt(pSprt);
    pSprt->u0 = 0;
    pSprt->v0 = 81;
    pSprt->x0 = 230;
    pSprt->y0 = 116;
    pSprt->w = 4 * itemRpk->field_2_w;
    pSprt->h = itemRpk->field_3_h;
    pSprt->r0 = 0x80;
    pSprt->g0 = 0x80;
    pSprt->b0 = 0x80;
    pSprt->clut = 0x543C;

    addPrim(ot, pSprt);
}
MGS_FUNC_IMPLEX(0x0046A770, Menu_inventory_left_render_PAL_key_icon_46A770, MENU_IMPL);


// TODO
MGS_FUNC_NOT_IMPL(0x46AA9B, void(), Menu_inventory_left_update_helper_46AA9B);


MGS_FUNC_NOT_IMPL(0x44FD66, int __cdecl(unsigned __int8 music, unsigned __int8 pan, unsigned __int8 code), Sound_sub_44FD66);

void CC Menu_inventory_clear_unk_46AFB7(MenuMan_Inventory_14h_Unk* pUnk, int size)
{
    for (int i = 0; i < size; i++)
    {
        pUnk[i].field_8_index = -1;
        pUnk[i].field_C_u = 0;
        pUnk[i].field_D_v = 0;
        pUnk[i].field_E_clut = 0;
    }
    flags_dword_733CF4 = 0;
    gBitFlags_dword_733DD0 = 0;
}
MGS_FUNC_IMPLEX(0x0046AFB7, Menu_inventory_clear_unk_46AFB7, MENU_IMPL);

MGS_ARY(1, 0x7339E8, MenuMan_Inventory_14h_Unk, 11, gMenu_inventory_right_11_array_7339E8, {});

void CC Menu_inventory_right_init_11_items_46947F()
{
    Menu_inventory_clear_unk_46AFB7(gMenu_inventory_right_11_array_7339E8, 11);
}
MGS_FUNC_IMPLEX(0x0046947F, Menu_inventory_right_init_11_items_46947F, MENU_IMPL);

void CC Menu_j_inventory_right_init_11_items_459A95()
{
    Menu_inventory_right_init_11_items_46947F();
}
MGS_FUNC_IMPLEX(0x00459A95, Menu_j_inventory_right_init_11_items_459A95, MENU_IMPL);

void CC Menu_inventory_common_checked_free_46B190(void* pAlloc)
{
    if (pAlloc)
    {
        System_2_free_40B2A7(pAlloc);
    }
}
MGS_FUNC_IMPLEX(0x0046B190, Menu_inventory_common_checked_free_46B190, MENU_IMPL);

void CC Menu_inventory_common_set_amounts_46B1A2(Menu_Item_Unknown_Array_Item* pItem, __int16 idx_idx, __int16 amount)
{
    pItem->field_6 = 0;
    pItem->field_0_item_id_idx = idx_idx;
    pItem->field_2_current_amount = amount;
}
MGS_FUNC_IMPLEX(0x0046B1A2, Menu_inventory_common_set_amounts_46B1A2, MENU_IMPL);

struct Menu_Item_Unknown_Main
{
    using Fn = void(CC*)(Menu_Item_Unknown*, int);
    DWORD field_0_array_count;
    DWORD field_4_selected_idx;
    int field_8;
    int field_C;
    DWORD field_10;
    DWORD field_14_fn_ctx;
    int field_18;
    Fn field_1C_fn;
};
MGS_ASSERT_SIZEOF(Menu_Item_Unknown_Main, 0x20);

struct Menu_Item_Unknown
{
    Menu_Item_Unknown_Main field_0_main;
    Menu_Item_Unknown_Array_Item field_20_array;
};

void CC Menu_46B506(Menu_Item_Unknown* pUnk, int amount)
{
    Menu_Item_Unknown_Array_Item* pArray = &pUnk->field_20_array;
    for (DWORD i = 0; i < pUnk->field_0_main.field_0_array_count; i++)
    {
        pArray[i].field_4 += amount;
        if (pArray[i].field_4 <= pUnk->field_0_main.field_C)
        {
            pArray[i].field_4 = pArray[i].field_4 + pUnk->field_0_main.field_8 - pUnk->field_0_main.field_C;
        }
    }
}
MGS_FUNC_IMPLEX(0x46B506, Menu_46B506, MENU_IMPL);

void CC Menu_46B4CC(Menu_Item_Unknown* pUnk, int amount)
{
    Menu_Item_Unknown_Array_Item* pArray = &pUnk->field_20_array;
    for (DWORD i = 0; i < pUnk->field_0_main.field_0_array_count; i++)
    {
        pArray[i].field_4 += amount;
        if (pArray[i].field_4 >= pUnk->field_0_main.field_8)
        {
            pArray[i].field_4 = pArray[i].field_4 + pUnk->field_0_main.field_C - pUnk->field_0_main.field_8;
        }
    }
}
MGS_FUNC_IMPLEX(0x46B4CC, Menu_46B4CC, MENU_IMPL);

void CC Menu_46B473(Menu_Item_Unknown* pMenuUnknown, int buttonsMatch, int k6Or4)
{
    pMenuUnknown->field_0_main.field_10 = k6Or4;
    pMenuUnknown->field_0_main.field_14_fn_ctx = (buttonsMatch << 8) / k6Or4;

    if (buttonsMatch <= 0)
    {
        pMenuUnknown->field_0_main.field_1C_fn = Menu_46B506;
    }
    else
    {
        pMenuUnknown->field_0_main.field_1C_fn = Menu_46B4CC;
    }

    // For some reason.. flip the input
    const int flags = buttonsMatch == -1 ? 1 : -1;

    // Either increment or decrement the selected index wrapped the the max count
    pMenuUnknown->field_0_main.field_4_selected_idx = (pMenuUnknown->field_0_main.field_0_array_count
        + pMenuUnknown->field_0_main.field_4_selected_idx
        + flags)
        % pMenuUnknown->field_0_main.field_0_array_count;

    // The "ding ding" sound each time you cycle to a new inventory item
    Sound_sub_44FD66(0, 0x3Fu, 0x17u);
}
MGS_FUNC_IMPLEX(0x46B473, Menu_46B473, true);


void CC Menu_inventory_common_update_helper_46B3CC(MenuMan_Inventory_Menu_0x14* pInvent, ButtonStates* pInput)
{
    Menu_Item_Unknown* pItem = pInvent->field_C_pItem_sys_alloc;
    MenuMan_Inventory_Unk_6764F8* pInventUnk = pInvent->field_0_invent.field_8_pMenuMan_Inventory_Unk_6764F8;
    const int bRet = Menu_46B540(pItem);

    if ((pInventUnk->field_8 | pInventUnk->field_C) & pInput->field_2_button_pressed)
    {
        if (bRet)
        {
            if (pItem->field_0_main.field_18 > 0)
            {
                pItem->field_0_main.field_18 = 0;
            }
        }
        else
        {
            Menu_46B473(pItem, (pInventUnk->field_8 & pInput->field_2_button_pressed) != 0 ? 1 : -1, 6);
            pItem->field_0_main.field_18 = 10;
        }
    }

    if (pItem->field_0_main.field_18 >= 0)
    {
        if ((pInventUnk->field_8 | pInventUnk->field_C) & pInput->field_0_button_status)
        {
            if (!bRet)
            {
                pItem->field_0_main.field_18--;
                if (pItem->field_0_main.field_18 < 0)
                {
                    Menu_46B473(pItem, (pInventUnk->field_8 & pInput->field_0_button_status) != 0 ? 1 : -1, 4);
                    pItem->field_0_main.field_18 = 0;
                }
            }
        }
        else
        {
            pItem->field_0_main.field_18 = 10;
        }
    }
}
MGS_FUNC_IMPLEX(0x46B3CC, Menu_inventory_common_update_helper_46B3CC, MENU_IMPL);

void CC Menu_46B215(Menu_Item_Unknown* pUnknown)
{
    const int arrayCount = pUnknown->field_0_main.field_0_array_count;

    // Halved because half of the items go on Y, and half on X when the menu opens
    const int arrayCountHalved = arrayCount / 2;

    Menu_Item_Unknown_Array_Item* pArray = &pUnknown->field_20_array;

    pArray[pUnknown->field_0_main.field_4_selected_idx].field_4 = 0;

    pUnknown->field_0_main.field_8 = (arrayCount / 2 << 8) + 128;
    short idx = 1;
    while (idx <= arrayCountHalved)
    {
        int relativeToSelectedIdx = idx + pUnknown->field_0_main.field_4_selected_idx;
        if (relativeToSelectedIdx >= pUnknown->field_0_main.field_0_array_count)
        {
            relativeToSelectedIdx -= pUnknown->field_0_main.field_0_array_count;
        }
        const __int16 value = idx++ << 8;
        pArray[relativeToSelectedIdx].field_4 = value;
    }

    const int v6 = arrayCount - arrayCountHalved - 1;
    pUnknown->field_0_main.field_C = -128 - (v6 << 8);

    int v8 = -v6;

    idx = -1;
    if (v8 <= -1)
    {
        do
        {
            int relativeToSelectedIdx = idx + pUnknown->field_0_main.field_4_selected_idx;
            if (relativeToSelectedIdx < 0)
            {
                relativeToSelectedIdx += pUnknown->field_0_main.field_0_array_count;
            }
            const __int16 value = idx-- << 8;
            pArray[relativeToSelectedIdx].field_4 = value;
        } while (idx >= v8);
    }
}
MGS_FUNC_IMPLEX(0x0046B215, Menu_46B215, MENU_IMPL);

signed int CC Menu_46B540(Menu_Item_Unknown* pItem)
{
    if (pItem->field_0_main.field_10 <= 0)
    {
        return 0;
    }

    pItem->field_0_main.field_10--;

    if (!pItem->field_0_main.field_10)
    {
        Menu_46B215(pItem);
        return 0;
    }

    pItem->field_0_main.field_1C_fn(pItem, pItem->field_0_main.field_14_fn_ctx);
    return 1;
}
MGS_FUNC_IMPLEX(0x0046B540, Menu_46B540, MENU_IMPL);

int CC Menu_inventory_common_item_46B1DF(Menu_Item_Unknown* pItem, int item_idx)
{
    pItem->field_0_main.field_4_selected_idx = -1;
    pItem->field_0_main.field_18 = -1;

    Menu_Item_Unknown_Array_Item* pArrayItem = &pItem->field_20_array;
    for (int i = pItem->field_0_main.field_0_array_count - 1; i >= 0; i--)
    {
        // Set field_4_selected_idx to the index of the item that has item_idx in field_20_array
        if (pArrayItem[i].field_0_item_id_idx == item_idx)
        {
            pItem->field_0_main.field_4_selected_idx = i;
        }
    }

    // Set up the correct bit indexes for expanding the menu
    Menu_46B215(pItem);
    return 1;
}
MGS_FUNC_IMPLEX(0x0046B1DF, Menu_inventory_common_item_46B1DF, MENU_IMPL);

Menu_Item_Unknown* CC Menu_inventory_common_allocate_items_46B160(int itemCount)
{
    const int size = (sizeof(Menu_Item_Unknown_Array_Item) * itemCount) + sizeof(Menu_Item_Unknown_Main);
    Menu_Item_Unknown* pItems = (Menu_Item_Unknown *)System_2_zerod_allocate_memory_40B296(size);
    if (pItems)
    {
        MemClearUnknown_40B231(pItems, size);
        pItems->field_0_main.field_10 = 0;
        pItems->field_0_main.field_0_array_count = itemCount;
    }
    return pItems;
}
MGS_FUNC_IMPLEX(0x0046B160, Menu_inventory_common_allocate_items_46B160, MENU_IMPL);

MGS_ARY(1, 0x733AD8, MenuMan_Inventory_14h_Unk, 21, g21_menu_left_inventory_unk_733AD8, {});
MGS_VAR(1, 0x733AD0, DWORD, gMenuLeft_733AD0, 0);

MGS_VAR(1, 0x733C80, DWORD, gInfoShowing_dword_733C80, 0);
MGS_VAR(1, 0x733C7C, DWORD, gShowInfoDelay_dword_733C7C, 0);

enum InventoryMenuState
{
    eClosed = 0,
    eUnknown_1 = 1,
    eOpening = 2,
    eClosing = 3,
    eUnknown_4 = 4,
};

signed int CC Menu_inventory_left_update_helper_46A305(MenuMan* pMenu)
{
    Menu_Item_Unknown* pAllocatedItem = nullptr;
    if (game_state_dword_72279C.flags & 0x40000)
    {
        pAllocatedItem = Menu_inventory_common_allocate_items_46B160(1);
        pMenu->field_1D8_invetory_menus[0].field_C_pItem_sys_alloc = pAllocatedItem;
        if (!pAllocatedItem)
        {
            return 0;
        }
        Menu_inventory_common_set_amounts_46B1A2(&pAllocatedItem->field_20_array, -1, 1);
    }
    else
    {
        int nonEmptyItemCount = 0;
        for (int i = 0; i < 24; i++)
        {
            if (gGameStates_78E7E0.gItem_states_word_78E82A[i] > 0)
            {
                nonEmptyItemCount++;
            }
        }

        pAllocatedItem = Menu_inventory_common_allocate_items_46B160(nonEmptyItemCount + 1);
        pMenu->field_1D8_invetory_menus[0].field_C_pItem_sys_alloc = pAllocatedItem;
        if (!pAllocatedItem)
        {
            return 0;
        }

        if (gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE == -1 || gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE == 17)
        {
            if (gMenuLeft_733AD0 < 0)
            {
                gMenuLeft_733AD0 = 0;
            }
        }
        else
        {
            gMenuLeft_733AD0 = gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE;
        }

        Menu_Item_Unknown_Array_Item* pArray = &pAllocatedItem->field_20_array;

        int cardLevel = (gGameStates_78E7E0.gItem_states_word_78E82A[17] > 0) - 1;
        for (int idx = 0; idx < 24; idx++)
        {
            if (idx == gMenuLeft_733AD0)
            {
                Menu_inventory_common_set_amounts_46B1A2(pArray, -1, 1);
                ++pArray;
                if (!cardLevel)
                {
                    if (gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE == -1)
                    {
                        Menu_inventory_common_set_amounts_46B1A2(pArray, 17, gGameStates_78E7E0.gItem_states_word_78E82A[Items::eIdCard]);
                        ++pArray;
                    }
                    else
                    {
                        cardLevel = 1;
                    }
                }
            }

            if (gGameStates_78E7E0.gItem_states_word_78E82A[idx] > 0 && idx != Items::eIdCard)
            {
                Menu_inventory_common_set_amounts_46B1A2(pArray, idx, gGameStates_78E7E0.gItem_states_word_78E82A[idx]);
                ++pArray;
            }

            if (cardLevel > 0)
            {
                Menu_inventory_common_set_amounts_46B1A2(pArray, 17, gGameStates_78E7E0.gItem_states_word_78E82A[17]);
                ++pArray;
                cardLevel = 0;
            }
        }
    }

    if (!Menu_inventory_common_update_helper_46B979(0))
    {
        Menu_inventory_common_checked_free_46B190(pAllocatedItem);
        return 0;
    }

    gInfoShowing_dword_733C80 = 0;
    gShowInfoDelay_dword_733C7C = 0;

    pMenu->field_1D8_invetory_menus[0].field_10_state = InventoryMenuState::eOpening;

    Menu_inventory_common_46B294();
    Menu_inventory_clear_unk_46AFB7(g21_menu_left_inventory_unk_733AD8, 21);

    Menu_inventory_common_item_46B1DF(
        pMenu->field_1D8_invetory_menus[0].field_C_pItem_sys_alloc,
        pMenu->field_1D8_invetory_menus[0].field_0_invent.field_0.field_0_item_id_idx);

    Sound_sub_44FD66(0, 0x3Fu, 0x15u);
    return 1;
}
MGS_FUNC_IMPLEX(0x0046A305, Menu_inventory_left_update_helper_46A305, MENU_IMPL);

void CC Menu_inventory_left_use_item_46A916(Menu_Item_Unknown* pItem, char buttonsPressed)
{
    if (buttonsPressed & 0x20)
    {
        const int idx = pItem->field_0_main.field_4_selected_idx;
        Menu_Item_Unknown_Array_Item* pArrayItem = &pItem->field_20_array + idx;
        if (pArrayItem->field_2_current_amount > 0)
        {
            if (pArrayItem->field_0_item_id_idx == Items::eKetchup || pArrayItem->field_0_item_id_idx == Items::eRations)
            {
                if (gGameStates_78E7E0.gItemsAreFrozen_word_78E86A)
                {
                    // Play frozen "ice hit" sound
                    Sound_sub_44FD66(0, 0x3Fu, 0x73u);
                    return;
                }

                if (gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6 == gGameStates_78E7E0.gSnakeMaxHealth_78E7F8)
                {
                    // Play "denied" sound
                    Sound_sub_44FD66(0, 0x3Fu, 0x23u);
                    return;
                }

                signed __int16 rationOrKetchupHealth = 0;
                if (pArrayItem->field_0_item_id_idx == Items::eRations)
                {
                    if (gGameStates_78E7E0.gDiffcultyLevel_78E7E2 == DiffcultyLevels::eVeryEasy)
                    {
                        rationOrKetchupHealth = 1024;
                    }
                    else if (gGameStates_78E7E0.gDiffcultyLevel_78E7E2 == DiffcultyLevels::eEasy)
                    {
                        rationOrKetchupHealth = 384;
                    }
                    else
                    {
                        rationOrKetchupHealth = 256;
                    }
                }
                else
                {
                    // Set no item to be selected
                    pArrayItem->field_0_item_id_idx = -1;

                    // Nuke the one and only possible ketchup
                    gGameStates_78E7E0.gItem_states_word_78E82A[Items::eKetchup] = 0;
                    rationOrKetchupHealth = 64;
                }

                gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6 += rationOrKetchupHealth;
                ++gGameStates_78E7E0.gNumRations_word_78E88C;

                // Cap to max LIFE
                if (gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6 > gGameStates_78E7E0.gSnakeMaxHealth_78E7F8)
                {
                    gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6 = gGameStates_78E7E0.gSnakeMaxHealth_78E7F8;
                }

                // Play "used ration" sound
                Sound_sub_44FD66(0, 0x3Fu, 0xCu);
            }
            else
            {
                switch (pArrayItem->field_0_item_id_idx)
                {
                case Items::eMedicine:
                    if (gGameStates_78E7E0.gSnakeFlags_byte_78E7FA & 1)
                    {
                        gGameStates_78E7E0.gSnakeFlags_byte_78E7FA &= ~1u;
                        gGameStates_78E7E0.gSnakeColdTimer_word_78E876 = 0;
                        gGameStates_78E7E0.gSnakeCold_word_78E87A = 0;
                    }
                    break;
                case Items::eDiazepam:
                    gGameStates_78E7E0.gSnakeFlags_byte_78E7FA |= 4u;
                    if (gGameStates_78E7E0.gSnakeShakeDelay_word_78E872 < 0)
                    {
                        gGameStates_78E7E0.gSnakeShakeDelay_word_78E872 = 0;
                    }
                    gGameStates_78E7E0.gSnakeShakeDelay_word_78E872 += 1200;
                    break;
                case Items::eItemBomb:
                    if (!(byte1_flags_word_9942A8 & 0x362)
                        && !gRocketLauncherInUse_dword_721E58
                        && !Menu_inventory_Is_Item_Disabled_46A128(Items::eItemBomb))
                    {
                        byte1_flags_word_9942A8 |= 0x080000u; // TODO: BYTE2(var) |= 8u check this is correct;
                        pArrayItem->field_0_item_id_idx = -1;
                        gGameStates_78E7E0.gItem_states_word_78E82A[Items::eItemBomb] = -1;

                        // Sounds like the menu cancel sound
                        Sound_sub_44FD66(0, 0x3Fu, 0x21u);
                        return;
                    }
                    else
                    {
                        // Play "denied" sound
                        Sound_sub_44FD66(0, 0x3Fu, 0x23u);
                        return;
                    }

                    // Any other kind of item does nothing when used
                default:
                    return;
                }

                // Play the "Medicine" used sound
                Sound_sub_44FD66(0, 0x3Fu, 0x22u);
            }

            // Decrement the amount
            if (pArrayItem->field_0_item_id_idx >= 0)
            {
                gGameStates_78E7E0.gItem_states_word_78E82A[pArrayItem->field_0_item_id_idx]--;
                pArrayItem->field_2_current_amount = gGameStates_78E7E0.gItem_states_word_78E82A[pArrayItem->field_0_item_id_idx];
            }
        }
    }
}
MGS_FUNC_IMPLEX(0x0046A916, Menu_inventory_left_use_item_46A916, MENU_IMPL);

void CC Menu_inventory_left_update_46A187(MenuMan* pMenu, DWORD* ot)
{
    const int field_2a_state = pMenu->field_2A_state;
    if (field_2a_state == InventoryMenuState::eClosed || field_2a_state == InventoryMenuState::eOpening)
    {
        if (field_2a_state == InventoryMenuState::eClosed)
        {
            if (game_state_dword_72279C.flags & 0x80400)
            {
                return;
            }
            if (!(byte1_flags_word_9942A8 & 0x20208000))
            {
                if (Menu_inventory_common_can_open_menu_46B745(&pMenu->field_1D8_invetory_menus[0], pMenu->field_24_input))
                {
                    if (Menu_inventory_left_update_helper_46A305(pMenu))
                    {
                        pMenu->field_2A_state = InventoryMenuState::eOpening;
                        gActorPauseFlags_dword_791A0C |= 4u;
                    }
                }
                else if (!(game_state_dword_72279C.mParts.flags2 & 4))
                {
                    if (pMenu->field_24_input->field_2_button_pressed & 4)
                    {
                        const int item_idx = gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE;
                        if (gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE < 0)
                        {
                            const char* left_item_idx = &pMenu->field_1D8_invetory_menus[0].field_11_item_idx;
                            if (!Menu_inventory_Is_Item_Disabled_46A128(pMenu->field_1D8_invetory_menus[0].field_11_item_idx)
                                && gGameStates_78E7E0.gItem_states_word_78E82A[*left_item_idx] > 0)
                            {
                                gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE = *left_item_idx;
                            }
                        }
                        else
                        {
                            gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE = -1;
                        }
                        if (item_idx != gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE)
                        {
                            Sound_sub_44FD66(0, 0x3Fu, 0x14u);
                        }
                    }
                }
            }
        }
        else if (field_2a_state == InventoryMenuState::eOpening)
        {
            if (Menu_inventory_common_can_close_menu_46B77E(&pMenu->field_1D8_invetory_menus[0], pMenu->field_24_input))
            {
                pMenu->field_1D8_invetory_menus[0].field_10_state = InventoryMenuState::eClosing;
            }
            // Controls how fast input to the menu is for pressing up/down/use
            else if (Menu_inventory_common_update_helper_46B29C() >= 256)
            {
                // Cycle through items
                Menu_inventory_common_update_helper_46B3CC(&pMenu->field_1D8_invetory_menus[0], pMenu->field_24_input);

                // Use selected item
                Menu_inventory_left_use_item_46A916(pMenu->field_1D8_invetory_menus[0].field_C_pItem_sys_alloc, pMenu->field_24_input->field_2_button_pressed);
            }
        }

        // Render opening/closing/open menu
        Menu_inventory_left_update_helper_46A4C1(pMenu, ot);

        // ??
        Menu_inventory_left_update_helper_46AA9B();
    }
    else if (field_2a_state != InventoryMenuState::eUnknown_4)
    {
        if (gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE >= 0)
        {
            const int counter = Menu_inventory_common_update_helper_46B29C();
            if (counter >= 255)
            {
                return;
            }
            Menu_inventory_common_invoke_handler_46B71E(pMenu, ot, &pMenu->field_1D8_invetory_menus[0], counter / -4, 0);
        }
        pMenu->field_1D8_invetory_menus[0].field_12 = 0;
    }
}
MGS_FUNC_IMPLEX(0x0046A187, Menu_inventory_left_update_46A187, MENU_IMPL);

void CC Menu_inventory_right_46956F(MenuMan* pMenu, DWORD* ot, DWORD a3, DWORD text_ypos, Menu_Item_Unknown_Array_Item* pSub)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
}
MGS_FUNC_IMPLEX(0x0046956F, Menu_inventory_right_46956F, false); // TODO

struct TextConfig
{
    DWORD gTextX_dword_66C4C0;
    DWORD gTextY_dword_66C4C4;
    DWORD gTextFlags_dword_66C4C8;
    DWORD gTextRGB_dword_66C4CC;
};
MGS_ASSERT_SIZEOF(TextConfig, 0x10);


void CC Menu_Set_SPRT_from_unk_46B127(SPRT* pSprt, MenuMan_Inventory_14h_Unk* pMenuUnk, __int16 x, __int16 y)
{
    pSprt->x0 = x + pMenuUnk->field_9_x;
    pSprt->y0 = y + pMenuUnk->field_A_y;
    pSprt->u0 = pMenuUnk->field_C_u;
    pSprt->v0 = pMenuUnk->field_D_v;
    pSprt->clut = pMenuUnk->field_E_clut;
    pSprt->w = pMenuUnk->field_10_w;
    pSprt->h = pMenuUnk->field_12_h;
}
MGS_FUNC_IMPLEX(0x0046B127, Menu_Set_SPRT_from_unk_46B127, MENU_IMPL);

void CC Menu_render_text_image_468CCC(MenuPrimBuffer* pPrimBuffer, int x, int y, MenuMan_Inventory_14h_Unk* pMenuUnk)
{
    SPRT* pSprt = PrimAlloc<SPRT>(pPrimBuffer);
    setSprt(pSprt);
    setRGB0(pSprt, 128, 128, 128);
    Menu_Set_SPRT_from_unk_46B127(pSprt, pMenuUnk, x - 2, y + 6);
    addPrim(pPrimBuffer->mOt, pSprt);
}
MGS_FUNC_IMPLEX(0x00468CCC, Menu_render_text_image_468CCC, MENU_IMPL);


MGS_VAR(1, 0x7339A8, MenuMan_Inventory_14h_Unk, gMenuNoUse_7339A8, {});
MGS_VAR(1, 0x733990, MenuMan_Inventory_14h_Unk, gMenuFrozen_733990, {});

void CC Menu_item_render_no_use_text_468CB2(MenuPrimBuffer* pPrimBuffer, int x, int y)
{
    Menu_render_text_image_468CCC(pPrimBuffer, x, y, &gMenuNoUse_7339A8);
}
MGS_FUNC_IMPLEX(0x00468CB2, Menu_item_render_no_use_text_468CB2, MENU_IMPL);

void CC Menu_item_render_frozen_text_468D30(MenuPrimBuffer* pPrimBuffer, int x, int y)
{
    Menu_render_text_image_468CCC(pPrimBuffer, x, y, &gMenuFrozen_733990);
}
MGS_FUNC_IMPLEX(0x00468D30, Menu_item_render_frozen_text_468D30, MENU_IMPL);

void CC Menu_item_render_frame_rects_46B809(MenuPrimBuffer* pPrimBuffer, __int16 itemXPos, __int16 itemYPos, int bBlueFill)
{
    // Top strip
    Menu_render_rect_46B79F(pPrimBuffer, itemXPos, itemYPos, 47, 1, 0);

    // Left strip
    Menu_render_rect_46B79F(pPrimBuffer, itemXPos - 4, itemYPos, 14, 20, 0);

    // Right strip
    Menu_render_rect_46B79F(pPrimBuffer, itemXPos + 45, itemYPos + 1, 2, 19, 0);

    // Bottom strip
    Menu_render_rect_46B79F(pPrimBuffer, itemXPos - 4, itemYPos + 20, 51, 9, 0);

    // Semi trans background
    Menu_render_rect_46B79F(pPrimBuffer, itemXPos + 10, itemYPos + 1, 35, 19, bBlueFill != 0 ? 0x800000 : 0)->code |= 2u;

    DR_TPAGE* pTPage = PrimAlloc<DR_TPAGE>(pPrimBuffer);
    pTPage->tag = 0x1000000u;
    pTPage->code = 0xE100041F;
    addPrim(pPrimBuffer->mOt, pTPage);
}
MGS_FUNC_IMPLEX(0x0046B809, Menu_item_render_frame_rects_46B809, MENU_IMPL);

void CC Menu_inventory_left_469F14(MenuMan* pMenu, DWORD* ot, DWORD xpos, DWORD ypos, Menu_Item_Unknown_Array_Item * pMenuSub)
{
    if (pMenuSub->field_0_item_id_idx < 0)
    {
        Menu_inventory_text_4689CB(pMenu, ot, xpos + 46, ypos + 22, "NO ITEM", 1);
    }
    else
    {
        MenuMan_Inventory_14h_Unk* pInventItem = &g21_menu_left_inventory_unk_733AD8[gItemInfos_675D30[pMenuSub->field_0_item_id_idx].field_4 - 12];
        Menu_inventory_common_icon_helper_46AFE1(pInventItem);

        if (Menu_inventory_Is_Item_Disabled_46A128(pMenuSub->field_0_item_id_idx))
        {
            Menu_item_render_no_use_text_468CB2(pMenu->field_20_prim_buffer, xpos, ypos);
        }

        if (gGameStates_78E7E0.gItemsAreFrozen_word_78E86A && (pMenuSub->field_0_item_id_idx == Items::eRations || pMenuSub->field_0_item_id_idx == Items::eKetchup))
        {
            Menu_item_render_frozen_text_468D30(pMenu->field_20_prim_buffer, xpos, ypos);
        }

        if (gMenu_left_item_flags_word_669AAA[pMenuSub->field_0_item_id_idx] & 0x2000)
        {
            Menu_render_text_fractional_468915(
                pMenu,
                xpos,
                ypos + 11,
                pMenuSub->field_2_current_amount,
                gGameStates_78E7E0.gItem_states_word_78E82A[pMenuSub->field_0_item_id_idx + 11]); // +11 = the item capacities
        }
        else if (pMenuSub->field_0_item_id_idx == Items::eIdCard)
        {
            TextConfig textConfig = {};
            textConfig.gTextFlags_dword_66C4C8 = 0;
            textConfig.gTextX_dword_66C4C0 = xpos;
            textConfig.gTextY_dword_66C4C4 = ypos + 14;
            textConfig.gTextRGB_dword_66C4CC = 0x64808080;
            Render_Text_Small_font_468642(pMenu->field_20_prim_buffer, &textConfig, "LV.");
            textConfig.gTextY_dword_66C4C4 -= 2;
            Menu_render_number_as_string_468529(pMenu->field_20_prim_buffer, &textConfig, gGameStates_78E7E0.gItem_states_word_78E82A[Items::eIdCard]);
        }
        else if (pMenuSub->field_0_item_id_idx == Items::eItemBomb)
        {
            // The bomb count down timer
            Menu_render_number_as_string_with_flags_4688DC(
                pMenu,
                ot,
                xpos + 10,
                ypos + 10,
                gGameStates_78E7E0.gItem_states_word_78E82A[Items::eItemBomb],
                0);
        }

        if (pInventItem->field_C_u || pInventItem->field_D_v || pInventItem->field_E_clut)
        {
            // Render the item icon
            SPRT* pIconSprt = PrimAlloc<SPRT>(pMenu->field_20_prim_buffer);
            setSprt(pIconSprt);
            if (pMenuSub->field_4 != 0)
            {
                setRGB0(pIconSprt, 0x40, 0x40, 0x40);
            }
            else
            {
                setRGB0(pIconSprt, 0x80, 0x80, 0x80);
            }
            Menu_Set_SPRT_from_unk_46B127(pIconSprt, pInventItem, xpos, ypos);
            addPrim(pMenu->field_20_prim_buffer->mOt, pIconSprt);
        }

        Menu_inventory_text_4689CB(pMenu, ot,
            xpos + 46,
            ypos + 22,
            gItemInfos_675D30[pMenuSub->field_0_item_id_idx].field_0_name,
            1);
    }

    // The bottom right/left items have a blue tinted background as they are
    // the active/equipped items.
    int bBlueBackground = 0;
    if (!pMenuSub->field_4 && !pMenuSub->field_6)
    {
        bBlueBackground = 1;
    }
    Menu_item_render_frame_rects_46B809(pMenu->field_20_prim_buffer, xpos, ypos, bBlueBackground);
}
MGS_FUNC_IMPLEX(0x00469F14, Menu_inventory_left_469F14, MENU_IMPL);

void CC Menu_init_inventory_right_helper_46954B()
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
}
MGS_FUNC_IMPLEX(0x0046954B, Menu_init_inventory_right_helper_46954B, false); // TODO

void CC Menu_init_inventory_left_helper_469EDB()
{
    MenuMan_Inventory_14h_Unk* pItem = g21_menu_left_inventory_unk_733AD8;
    int idx = 12;
    do
    {
        int palIdx = 0; // Red pal
        if (idx == 22)
        {
            // Ration (green pal)
            palIdx = 46;
        }
        else if (idx == 27)
        {
            // Id card (blue pal)
            palIdx = 47;
        }
        Menu_get_item_file_item_46B92E(pItem, idx++, palIdx);
        ++pItem;
    } while (idx - 12 < 21);
}
MGS_FUNC_IMPLEX(0x00469EDB, Menu_init_inventory_left_helper_469EDB, MENU_IMPL);

void CC Menu_Get_current_weapon_info_46948E(MenuMan* pMenu)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
}
MGS_FUNC_IMPLEX(0x0046948E, Menu_Get_current_weapon_info_46948E, false); // TODO

void CC Menu_generic_update_helper_469E37(MenuMan* pMenu)
{
    Menu_render_unk_2_and_3_468C6B();
    int idx = pMenu->field_1D8_invetory_menus[0].field_0_invent.field_0.field_0_item_id_idx;
    if (idx != -1)
    {
        idx = pMenu->field_1D8_invetory_menus[0].field_11_item_idx;
        if (idx != -1)
        {
            Menu_render_unk_46B081(&g21_menu_left_inventory_unk_733AD8[gItemInfos_675D30[idx].field_4 - 12], 0);
        }
    }
}
MGS_FUNC_IMPLEX(0x00469E37, Menu_generic_update_helper_469E37, MENU_IMPL);

void CC Menu_init_inventory_set_fn_46B3B0(MenuMan_Inventory_Menu_0x14 *pMenu_field_1EC, int bLeftOrRight, MenuMan_Inventory_Unk_6764F8::Fn pFn)
{
    MenuMan_Inventory_Unk_6764F8* pUnk = &stru_6764F8[bLeftOrRight];
    pMenu_field_1EC->field_0_invent.field_8_pMenuMan_Inventory_Unk_6764F8 = pUnk;
    pUnk->field_18_fn = pFn;
}
MGS_FUNC_IMPLEX(0x0046B3B0, Menu_init_inventory_set_fn_46B3B0, MENU_IMPL);

MGS_VAR(1, 0x7339E0, DWORD, gMenuRight_7339E0, 0);

void CC Menu_init_inventory_right_4694E4(MenuMan* pMenu)
{
    pMenu->field_28_flags |= 2u;
    pMenu->field_1D8_invetory_menus[1].field_0_invent.field_0.field_0_item_id_idx = -1;
    pMenu->field_1D8_invetory_menus[1].field_11_item_idx = -1;
    pMenu->m7FnPtrs_field_2C[1] = Menu_inventory_right_update_4697F3;
    pMenu->field_1D8_invetory_menus[1].field_10_state = InventoryMenuState::eClosed;
    pMenu->field_1D8_invetory_menus[1].field_12 = 0;
    pMenu->field_1D8_invetory_menus[1].field_0_invent.field_0.field_4 = 0;
    pMenu->field_1D8_invetory_menus[1].field_0_invent.field_0.field_6 = 1;
    gMenuRight_7339E0 = 0;
    Menu_init_inventory_set_fn_46B3B0(&pMenu->field_1D8_invetory_menus[1], 1, Menu_inventory_right_46956F);
    Menu_init_inventory_right_helper_46954B();
    Menu_Get_current_weapon_info_46948E(pMenu);
}
MGS_FUNC_IMPLEX(0x004694E4, Menu_init_inventory_right_4694E4, MENU_IMPL);


void Menu_init_inventory_left_helper_468C87()
{
    gMenuNoUse_7339A8.field_8_index = 0;
    Menu_get_item_file_item_46B92E(&gMenuNoUse_7339A8, 40, 39);
    Menu_get_item_file_item_46B92E(&gMenuFrozen_733990, 48, 39);
    Menu_render_unk_2_and_3_468C6B();
}
MGS_FUNC_IMPLEX(0x00468C87, Menu_init_inventory_left_helper_468C87, MENU_IMPL);

void CC Menu_init_inventory_left_469E77(MenuMan* pMenu)
{
    pMenu->field_28_flags |= 4u;
    pMenu->field_1D8_invetory_menus[0].field_0_invent.field_0.field_0_item_id_idx = -1;
    pMenu->field_1D8_invetory_menus[0].field_11_item_idx = -1;
    pMenu->m7FnPtrs_field_2C[2] = Menu_inventory_left_update_46A187;
    pMenu->field_1D8_invetory_menus[0].field_10_state = InventoryMenuState::eClosed;
    pMenu->field_1D8_invetory_menus[0].field_0_invent.field_0.field_4 = 0;
    pMenu->field_1D8_invetory_menus[0].field_0_invent.field_0.field_6 = 1;
    gMenuLeft_733AD0 = 0;
    Menu_init_inventory_set_fn_46B3B0(&pMenu->field_1D8_invetory_menus[0], 0, Menu_inventory_left_469F14);
    Menu_init_inventory_left_helper_469EDB();
    Menu_generic_update_helper_469E37(pMenu);
    Menu_init_inventory_left_helper_468C87();
}
MGS_FUNC_IMPLEX(0x00469E77, Menu_init_inventory_left_469E77, MENU_IMPL);

MGS_FUNC_NOT_IMPL(0x462CFC, void __cdecl(MenuMan*), Menu_init_fn7_jimaku_font_buffer_size_sub_462CFC);


template<class T>
static inline T UnSetPointerFlag(T ptr, bool& bWasFlagged)
{
    bWasFlagged = (reinterpret_cast<unsigned int>(ptr) & 0x40000000) ? true : false;
    return reinterpret_cast<T>(reinterpret_cast<unsigned int>(ptr) & 0xBFFFFFFF);
}

MGS_VAR(1, 0x66C4C0, TextConfig, gTextConfig_66C4C0, {});

struct SpecialChar
{
    char field_0_char;
    BYTE field_1_width;
};
MGS_ASSERT_SIZEOF(SpecialChar, 0x2);

struct SpecialChar_Array
{
    SpecialChar mChars[16];
};

const SpecialChar_Array kSpecialCharData =
{
     '.', 2,
     '@', 2,
     ':', 2,
     '_', 4,
     '!', 2,
     '?', 6,
     '+', 6,
     '-', 6,
     '/', 6,
     '*', 6,
     '{', 3,
     '}', 3,
     '\0', 0,
     '\0', 0,
     '\0', 0,
     '\0', 0
};

MGS_VAR(1, 0x6757C0,  SpecialChar_Array, gSpecialChars_byte_6757C0,
{
    kSpecialCharData
});


int CC Render_Text_SetGlyphPositions_4687E8(SPRT* pFirstSprt, SPRT* pLastSprt, int updated_x, int xpos, DWORD flags)
{
    int result = updated_x;
    if (pFirstSprt && pFirstSprt < pLastSprt)
    {
        // Align all of the sprites for each number according to the alignment flags
        int adjustValue = 0;
        if ((flags & 0xF) == 1)
        {
            // Right aligned
            adjustValue = updated_x - xpos;
            result = updated_x - xpos;
        }
        else if ((flags & 0xF) == 2)
        {
            // Center aligned
            adjustValue = updated_x - xpos / 2;
            result = xpos;
        }
        else
        {
            // Left aligned
            adjustValue = updated_x;
            result = updated_x + xpos;
        }

        while (pFirstSprt < pLastSprt)
        {
            pFirstSprt->x0 += static_cast<short>(adjustValue);
            pFirstSprt++;
        }
    }

    return result;
}
MGS_FUNC_IMPLEX(0x4687E8, Render_Text_SetGlyphPositions_4687E8, MENU_IMPL);


const int kFontLineHeight = 8;

struct CharToFontAtlasIndex
{
    char mChar;
    char mIdx;
};

// TODO: Can't be used.. special table works in another way ?
const CharToFontAtlasIndex kFontCharMappings[] =
{
    { ':', 12 },{ '!', 14 },{ '?', 15 },
    { '+', 16 },{ '-', 17 },{ '/', 18 },
    { '*', 19 },{ '{', 20 },{ '}', 21 }
};

const char kFontSpacedChars[] = ".!:iI";

// 0-9 =      idx*6, 232 | idx*8, 248, h = 5
// a-z, A-Z = idx*6, 237 | idx*8, 242, h = 6
struct FontSettings
{
    BYTE mCharWidth;
    BYTE m0to9V;
    BYTE mAtoZV;
};

const FontSettings kFontSettings[] =
{
    { 8, 248, 242 }, // Large font
    { 6, 232, 237 }  // Small font
};

static void RenderTextHelper(MenuPrimBuffer* pPrimBuffer, TextConfig* pTextSettings, const char* pString, const FontSettings& fontSettings, const SPRT& templateSprite)
{
    if (!*pString)
    {
        return;
    }

    SPRT* firstSprt = nullptr;
    short xpos = 0;
    const char* iterChar = pString;
    while (*iterChar)
    {
        while (*iterChar == '\n')
        {
            Render_Text_SetGlyphPositions_4687E8(
                firstSprt,
                (SPRT *)pPrimBuffer->mFreeLocation,
                pTextSettings->gTextX_dword_66C4C0,
                xpos,
                pTextSettings->gTextFlags_dword_66C4C8);

            // Start again on the left
            xpos = 0;

            // Move future text down 1 line height
            pTextSettings->gTextY_dword_66C4C4 += kFontLineHeight;

            // Reset the "from" sprite for when we fix up the next line
            firstSprt = (SPRT *)pPrimBuffer->mFreeLocation;
            iterChar++;
            if (!*iterChar)
            {
                break;
            }
        }

        short advanceXBy = fontSettings.mCharWidth + 1;
        BYTE char_u0 = 0;
        BYTE char_v0 = 0;
        bool valid = true;

        const char curChar = (*iterChar) | 0x20; // 0x20 is used to disable @ and some others?
        if (curChar >= '0' && curChar <= '9')
        {
            // 0-9 handling
            advanceXBy = fontSettings.mCharWidth + 1;
            char_u0 = fontSettings.mCharWidth * (curChar - '0');
            char_v0 = fontSettings.m0to9V;
        }
        else
        {
            if ((curChar >= 'a' && curChar <= 'z') || (curChar >= 'A' && curChar <= 'Z'))
            {
                if (curChar == 'i')
                {
                    xpos++;
                    advanceXBy = fontSettings.mCharWidth / 2;
                }
                char_u0 = fontSettings.mCharWidth * (toupper(curChar) - 'A');
                char_v0 = fontSettings.mAtoZV;
            }
            else
            {
                if (curChar == '#')
                {
                    if (fontSettings.mCharWidth != 8)
                    {
                        advanceXBy = fontSettings.mCharWidth;
                    }
                    else
                    {
                        iterChar++;
                        xpos += (*iterChar) - '0';
                        iterChar++;
                        char_u0 = fontSettings.mCharWidth * (*iterChar - '0');
                        char_v0 = fontSettings.m0to9V;
                    }
                }
                else if (curChar == ' ')
                {
                    advanceXBy = fontSettings.mCharWidth / 2;
                    valid = false;
                }
                else
                {
                    // Look up curChar in gSpecialChars_byte_6757C0
                    bool found = false;
                    for (int i = 0; i < 16; i++)
                    {
                        if (gSpecialChars_byte_6757C0.mChars[i].field_0_char == curChar)
                        {
                            found = true;

                            BYTE base = 80;
                            if (fontSettings.mCharWidth != 8)
                            {
                                base = 60;
                            }
                            char_u0 = static_cast<BYTE>((fontSettings.mCharWidth  * i) + base);
                            //char_u0 = fontSettings.mCharWidth * (curChar - '0');
                            char_v0 = fontSettings.m0to9V;

                            advanceXBy = gSpecialChars_byte_6757C0.mChars[i].field_1_width;
                            if (advanceXBy < 3)
                            {
                                ++xpos;
                                ++advanceXBy;
                                break;
                            }

                            if (advanceXBy != 6)
                            {
                                break;
                            }

                            advanceXBy = fontSettings.mCharWidth + 1;
                            break;
                        }
                    }

                    valid = found;
                    if (!valid)
                    {
                        // If we are not rendering a char then don't advance x pos
                        advanceXBy = 0;
                    }
                }
            }
        }

        if (valid)
        {
            SPRT* pTextSprt = PrimAlloc<SPRT>(pPrimBuffer);
            if (!firstSprt)
            {
                firstSprt = pTextSprt;
            }

            memcpy(pTextSprt, &templateSprite, sizeof(SPRT));

            setRGB0(pTextSprt,
                BYTE0(pTextSettings->gTextRGB_dword_66C4CC),
                BYTE1(pTextSettings->gTextRGB_dword_66C4CC),
                BYTE2(pTextSettings->gTextRGB_dword_66C4CC));

            pTextSprt->x0 = xpos;
            pTextSprt->y0 = static_cast<short>(pTextSettings->gTextY_dword_66C4C4);

            pTextSprt->v0 = char_v0;
            pTextSprt->u0 = char_u0;
            addPrim(pPrimBuffer->mOt, pTextSprt);
        }

        xpos += advanceXBy;
        iterChar++;
    }

    // Position the final line/batch
    Render_Text_SetGlyphPositions_4687E8(
        firstSprt,
        (SPRT *)pPrimBuffer->mFreeLocation,
        pTextSettings->gTextX_dword_66C4C0,
        xpos,
        pTextSettings->gTextFlags_dword_66C4C8);
}


void CC Render_Text_Large_font_468AAF(MenuPrimBuffer* pPrimBuffer, TextConfig* pTextSettings, const char* pString)
{
    RenderTextHelper(pPrimBuffer, pTextSettings, pString, kFontSettings[0], gMenu_sprt3_733978);
}
MGS_FUNC_IMPLEX(0x468AAF, Render_Text_Large_font_468AAF, true);

void CC Render_Text_Small_font_468642(MenuPrimBuffer* pPrimBuffer, TextConfig* pTextSettings, const char* pText)
{
    char specialChar;
    char specialUPos;
    SpecialChar *pSpecialIter;
    __int16 ypos;
    int v12;
    int rgb;
    char texture_v0;
    signed int xpos;
    signed int charWidth;

    SPRT* pFirstSprt = nullptr;
    xpos = 0;
    rgb = pTextSettings->gTextRGB_dword_66C4CC;
    while (*pText)
    {
        bool handledNewLine = false;
        while (*pText == '\n')
        {
            Render_Text_SetGlyphPositions_4687E8(
                pFirstSprt,
                (SPRT *)pPrimBuffer->mFreeLocation,
                pTextSettings->gTextX_dword_66C4C0,
                xpos,
                pTextSettings->gTextFlags_dword_66C4C8);
            xpos = 0;
            pTextSettings->gTextY_dword_66C4C4 += 8;
            pFirstSprt = (SPRT *)pPrimBuffer->mFreeLocation;
            handledNewLine = true;
            pText++;
        }

        if (handledNewLine)
        {
            continue;
        }

        int texture_u0 = 0;
        signed int curChar = *pText;
        curChar = curChar | ' ';
        if (curChar >= '0' && curChar <= '9')
        {
            // Handle number
            texture_v0 = 232;
            texture_u0 = 2 * (3 * curChar + 112);
        }
        else
        {
            // Handle not a number and not a-z
            if (curChar < 'a' || curChar > 'z')
            {
                if (curChar == ' ')
                {
                    xpos += 4;
                }
                else if (curChar == '#')
                {
                    xpos += 6;
                }
                else
                {
                    specialChar = gSpecialChars_byte_6757C0.mChars[0].field_0_char;
                    specialUPos = 0;
                    pSpecialIter = gSpecialChars_byte_6757C0.mChars;
                    while (specialChar)
                    {
                        if (specialChar == curChar)
                        {
                            texture_u0 = specialUPos + 60;
                            texture_v0 = 232;
                            charWidth = pSpecialIter->field_1_width;
                            if (charWidth < 3)
                            {
                                ++xpos;
                                ++charWidth;
                            }
                            goto add_sprite;
                        }
                        ++pSpecialIter;
                        specialUPos += 6;
                        specialChar = pSpecialIter->field_0_char;
                    }
                }
                ++pText;
                continue; // ignore unknown char
            }
            texture_v0 = 237;
            texture_u0 = 2 * (3 * curChar - 291);
            if (curChar == 'i')
            {
                ++xpos;
                charWidth = 3;
                goto add_sprite;
            }
        }
        charWidth = 6;

    add_sprite:
        SPRT* pSprt = PrimAlloc<SPRT>(pPrimBuffer);
        if (!pFirstSprt)
        {
            pFirstSprt = pSprt;
        }
        memcpy(pSprt, &gMenu_sprt2_733960, sizeof(SPRT));
        *(DWORD *)&pSprt->r0 = rgb;
        pSprt->x0 = xpos;
        ypos = pTextSettings->gTextY_dword_66C4C4;
        pSprt->u0 = texture_u0;
        pSprt->y0 = ypos;
        pSprt->v0 = texture_v0;

        addPrim(pPrimBuffer->mOt, pSprt);
        xpos += charWidth;

        ++pText;
    }

    pTextSettings->gTextX_dword_66C4C0 = Render_Text_SetGlyphPositions_4687E8(
        pFirstSprt,
        (SPRT *)pPrimBuffer->mFreeLocation,
        pTextSettings->gTextX_dword_66C4C0,
        xpos,
        pTextSettings->gTextFlags_dword_66C4C8);
}
MGS_FUNC_IMPLEX(0x468642, Render_Text_Small_font_468642, MENU_IMPL); // TODO: Refactor

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
                Render_Text_Large_font_468AAF(&gMenuPrimBuffer_7265E0, &gTextConfig_66C4C0, formattedStr);
            }
            else
            {
                Render_Text_Small_font_468642(&gMenuPrimBuffer_7265E0, &gTextConfig_66C4C0, formattedStr);
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

        Render_Text_Small_font_468642(pPrimBuffer, &textConfig, pBarConfig->mText);
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


MGS_VAR(1, 0x7339D8, int, gTakeDamageCounter_dword_7339D8, 0);
MGS_VAR(1, 0x995348, WORD, gSnakeCurrentO2_995348, 0);



signed int CC Menu_menu_bars_update_field200_46938A(MenuMan_MenuBars* pMenuBarsUnk)
{
    if (pMenuBarsUnk->field_6_snake_hp == gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6)
    {
        pMenuBarsUnk->field_A_k10_decrement = 10;
        return 0;
    }

    if (pMenuBarsUnk->field_6_snake_hp > gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6)
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

    if (pMenuBarsUnk->field_6_snake_hp < gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6)
    {
        pMenuBarsUnk->field_6_snake_hp = gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6;
    }
    return 1;
}
MGS_FUNC_IMPLEX(0x46938A, Menu_menu_bars_update_field200_46938A, MENU_IMPL);

const BarConfig kLifeConfig = { "LIFE",{ 0x10, 0x8F, 0x7F },{ 0x1F, 0xDF, 0x3F } , 0 };
const BarConfig kO2Config = { "O2",{ 0x1F, 0x3F, 0xC0 },{ 0x1F, 0x7F, 0xFF }, 1 };

MGS_VAR(0, 0x6757F0, BarConfig, gSnakeLifeBarConfig_6757F0, kLifeConfig);
MGS_VAR(1, 0x675800, BarConfig, gSnakeO2BarConfig_675800, kO2Config);
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
        gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6,
        gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6,
        gGameStates_78E7E0.gSnakeMaxHealth_78E7F8,
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
        gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6,
        gGameStates_78E7E0.gSnakeMaxHealth_78E7F8,
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

void CC Menu_menu_bars_update_469215(MenuMan* pMenu, DWORD* /*ot*/)
{
    MenuMan_MenuBars* pField_200 = &pMenu->field_200_hp_bars_info;
    signed int bHpChanged = Menu_menu_bars_update_field200_46938A(&pMenu->field_200_hp_bars_info);

    if (gSnakeCurrentO2_995348 < 1024)
    {
        pMenu->field_200_hp_bars_info.field_1_O2_hp = 150;
        bHpChanged = 1;
    }

    if (!pMenu->field_2A_state)
    {
        if (game_state_dword_72279C.flags & 0x10000)
        {
            pField_200->field_0_state = 3;
        }

        const char state_copy = pField_200->field_0_state;
        if ((!pField_200->field_0_state || state_copy == 3)
            && (bHpChanged || game_state_dword_72279C.flags & 0x8000 || gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6 <= gGameStates_78E7E0.gSnakeMaxHealth_78E7F8 / 2))
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
                if (bHpChanged || gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6 <= gGameStates_78E7E0.gSnakeMaxHealth_78E7F8 / 2 || game_state_dword_72279C.flags & 0x8000)
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
    pMenu->field_200_hp_bars_info.field_6_snake_hp = gGameStates_78E7E0.gSnakeCurrentHealth_78E7F6;
    pMenu->field_200_hp_bars_info.field_A_k10_decrement = 10;
    pMenu->field_200_hp_bars_info.field_2_bar_x = 16;
    pMenu->field_200_hp_bars_info.field_4_bar_y = -48;

    gSnakeLifeYPos_7339D4 = -48;
}
MGS_FUNC_IMPLEX(0x4691CE, Menu_init_menu_bars_4691CE, MENU_IMPL);

MGS_ARY(1, 0x66C480, TMenuFn, 9, gMenuFuncs_inits_66C480,
{
    Menu_init_fn0_46AD91,
    Menu_init_radar_468358,
    Menu_init_codec_463746,
    Menu_init_inventory_left_469E77,
    Menu_init_inventory_right_4694E4,
    Menu_init_menu_bars_4691CE,
    Menu_init_num_res_font_468406,
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
    pMenu->field_2A_state = 0;
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

MGS_VAR(1, 0x733DD8, SPRT, gMenu_inventory_text_header_background_733DD8, {});


const PSX_RECT sMenu_rect{ 960, 256, 50, 80 };
MGS_VAR(1, 0x676530, PSX_RECT, sMenu_rect_676530, { sMenu_rect });

struct uv_pair
{
    __int16 u;
    __int16 v;
};

struct uv_pair_array
{
    uv_pair uvs[2];
};
uv_pair_array kStru_676538 = {  86, 72 , 44, 72  };
MGS_VAR(1, 0x676538, uv_pair_array, stru_676538, { kStru_676538 });

signed int CC Menu_inventory_common_update_helper_46B979(int idx)
{
    setSprt(&gMenu_inventory_text_header_background_733DD8);

    gMenu_inventory_text_header_background_733DD8.r0 = 128;
    gMenu_inventory_text_header_background_733DD8.g0 = 128;
    gMenu_inventory_text_header_background_733DD8.b0 = 128;

    gMenu_inventory_text_header_background_733DD8.v0 = static_cast<BYTE>(sMenu_rect_676530.y1);
    gMenu_inventory_text_header_background_733DD8.u0 = 0;

    gMenu_inventory_text_header_background_733DD8.w = 200;
    gMenu_inventory_text_header_background_733DD8.h = 80;

    const __int16 x0 = stru_676538.uvs[idx].u; // TODO: Actually POINT's?
    const __int16 y0 = stru_676538.uvs[idx].v;
    gMenu_inventory_text_header_background_733DD8.x0 = x0;
    gMenu_inventory_text_header_background_733DD8.y0 = y0;

    gMenu_inventory_text_header_background_733DD8.clut = 32700;
    ClearImage(&sMenu_rect_676530, 0, 0, 0);
    Font_45A70D(&gMenuFont_733DF0, &sMenu_rect_676530, 960, 510);
    Font_45A796(&gMenuFont_733DF0, -1, -1, 0, 6, 2, 0);
    const int sizeToAlloc = Font_CalcSize_45AA45(&gMenuFont_733DF0);
    void* pAllocated = System_2_zerod_allocate_memory_40B296(sizeToAlloc);
    if (!pAllocated)
    {
        return 0;
    }
    Font_Set_Buffer_45AAE9(&gMenuFont_733DF0, (WORD*)pAllocated);
    Font_ColourRelated_45A89F(&gMenuFont_733DF0, 0, 0x6739, 0);
    Font_Set_global_alloc_ptr_45C7F2(&gMenuFont_733DF0);
    return 1;
}
MGS_FUNC_IMPLEX(0x46B979, Menu_inventory_common_update_helper_46B979, MENU_IMPL);

void CC Menu_inventory_draw_item_header_and_background_with_hp_bar_46BA95(MenuMan* pMenu, DWORD* pOt, const char* pText)
{
    pMenu->field_2B |= 2u;

    SPRT* itemTextSprite = PrimAlloc<SPRT>(pMenu->field_20_prim_buffer);
    memcpy(itemTextSprite, &gMenu_inventory_text_header_background_733DD8, sizeof(SPRT));
    addPrim(pMenu->field_20_prim_buffer->mOt, itemTextSprite); // A texture with the item text on it ??

    const int textWidth = Menu_inventory_text_4689CB(
        pMenu,
        pOt,
        gMenu_inventory_text_header_background_733DD8.x0 - 10 + 2,
        gMenu_inventory_text_header_background_733DD8.y0 - 7,
        pText,
        0); // EQUIP or WEAPON/NO ITEM text

    Menu_render_snake_life_bar_469194(pMenu->field_20_prim_buffer, gMenu_inventory_text_header_background_733DD8.x0 - 10, 24); // HP bar

    const short int width = static_cast<short int>(textWidth + 2);
    for (int i = 0; i<2; i++)
    {
        POLY_F4* pPolyFT4 = PrimAlloc<POLY_F4>(pMenu->field_20_prim_buffer); // Sloping header polygon that contains EQUIP/WEAPON text
        setPolyF4(pPolyFT4);
        setRGB0(pPolyFT4, 0, 0, 0);
        setSemiTrans(pPolyFT4, 1);
        pPolyFT4->x0 = gMenu_inventory_text_header_background_733DD8.x0 - 10;
        pPolyFT4->y0 = gMenu_inventory_text_header_background_733DD8.y0 - 9;
        pPolyFT4->x1 = width;
        pPolyFT4->x2 = gMenu_inventory_text_header_background_733DD8.x0 - 10;
        pPolyFT4->y1 = gMenu_inventory_text_header_background_733DD8.y0 - 9;
        pPolyFT4->x3 = width;
        pPolyFT4->y2 = gMenu_inventory_text_header_background_733DD8.y0;
        pPolyFT4->x3 = width + 10;
        pPolyFT4->y3 = gMenu_inventory_text_header_background_733DD8.y0;
        addPrim(pOt, pPolyFT4);

        TILE* pTile = PrimAlloc<TILE>(pMenu->field_20_prim_buffer); // Big square background for the item/weapon text
        setTile(pTile);
        setRGB0(pTile, 0, 0, 0);
        setSemiTrans(pTile, 1);
        pTile->x0 = gMenu_inventory_text_header_background_733DD8.x0 - 10;
        pTile->y0 = gMenu_inventory_text_header_background_733DD8.y0;
        pTile->w = gMenu_inventory_text_header_background_733DD8.w + 10;
        pTile->h = gMenu_inventory_text_header_background_733DD8.h;
        addPrim(pOt, pTile);
    }
}
MGS_FUNC_IMPLEX(0x46BA95, Menu_inventory_draw_item_header_and_background_with_hp_bar_46BA95, MENU_IMPL);

int CC Menu_inventory_text_4689CB(MenuMan* pMenu, DWORD* /*ot*/, int xpos, int ypos, const char* pText, int textFlags)
{
    TextConfig textConfig = {};
    textConfig.gTextX_dword_66C4C0 = xpos;
    textConfig.gTextY_dword_66C4C4 = ypos;
    textConfig.gTextFlags_dword_66C4C8 = textFlags;
    textConfig.gTextRGB_dword_66C4CC = 0x64808080;
    Render_Text_Small_font_468642(pMenu->field_20_prim_buffer, &textConfig, pText);
    return textConfig.gTextX_dword_66C4C0;
}
MGS_FUNC_IMPLEX(0x4689CB, Menu_inventory_text_4689CB, MENU_IMPL);


const short kCharWidth = 6;

void CC Menu_render_number_as_string_468529(MenuPrimBuffer* pPrimBuffer, TextConfig* pTextConfig, signed int numberRemainder)
{
    int maxCount = 0;
    if (pTextConfig->gTextFlags_dword_66C4C8 & 0x40)
    {
        maxCount = pTextConfig->gTextFlags_dword_66C4C8 >> 8;
    }
    else
    {
        maxCount = 0;
    }

    short int xpos = 0;
    SPRT* pFirstSprt = nullptr;
    int numSprts = 0;
    for (;;)
    {
        xpos -= 6;
        const int currentNum = numberRemainder % 10;
        SPRT* pSprt = PrimAlloc<SPRT>(pPrimBuffer);
        numberRemainder /= 10;
        if (!pFirstSprt)
        {
            pFirstSprt = pSprt;
        }
        memcpy(pSprt, &gMenu_font1_template_sprite_7339C0, sizeof(SPRT));

        setRGB0(pSprt,
            BYTE0(pTextConfig->gTextRGB_dword_66C4CC),
            BYTE1(pTextConfig->gTextRGB_dword_66C4CC),
            BYTE2(pTextConfig->gTextRGB_dword_66C4CC));

        pSprt->x0 = xpos;
        pSprt->y0 = static_cast<short>(pTextConfig->gTextY_dword_66C4C4);
        pSprt->u0 = static_cast<BYTE>(kCharWidth * (currentNum + 26)); // the numbers are after the alphabet in the sprite sheet, hence + 26

        addPrim(pPrimBuffer->mOt, pSprt);
        numSprts++;

        --maxCount;
        if (numberRemainder <= 0 && maxCount <= 0)
        {
            break;
        }
    }

    // Similar to Render_Text_SetGlyphPositions_4687E8
    if (pFirstSprt)
    {
        // Align all of the sprites for each number according to the alignment flags
        int adjustValue = 0;
        if ((pTextConfig->gTextFlags_dword_66C4C8 & 0xF) == 1)
        {
            // Right aligned
            adjustValue = pTextConfig->gTextX_dword_66C4C0;
            pTextConfig->gTextX_dword_66C4C0 += xpos;
        }
        else if ((pTextConfig->gTextFlags_dword_66C4C8 & 0xF) == 2)
        {
            // Center aligned
            adjustValue = pTextConfig->gTextX_dword_66C4C0 - xpos / 2;
        }
        else
        {
            // Left aligned
            adjustValue = pTextConfig->gTextX_dword_66C4C0 - xpos;
            pTextConfig->gTextX_dword_66C4C0 = adjustValue;
        }

        for (int i = 0; i < numSprts; i++)
        {
            pFirstSprt[i].x0 += static_cast<short>(adjustValue);
        }
    }
}
MGS_FUNC_IMPLEX(0x00468529, Menu_render_number_as_string_468529, MENU_IMPL);

void CC Menu_render_number_as_string_with_flags_4688DC(MenuMan* pMenu, DWORD* /*ot*/, int textX, int textY, signed int number, int textFlags)
{
    TextConfig textConfig = {};
    textConfig.gTextX_dword_66C4C0 = textX;
    textConfig.gTextY_dword_66C4C4 = textY;
    textConfig.gTextFlags_dword_66C4C8 = textFlags;
    textConfig.gTextRGB_dword_66C4CC = 0x64808080;
    Menu_render_number_as_string_468529(pMenu->field_20_prim_buffer, &textConfig, number);
}
MGS_FUNC_IMPLEX(0x004688DC, Menu_render_number_as_string_with_flags_4688DC, MENU_IMPL);

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

    setRGB0(pSprt,
        BYTE0(textConfig.gTextRGB_dword_66C4CC),
        BYTE1(textConfig.gTextRGB_dword_66C4CC),
        BYTE2(textConfig.gTextRGB_dword_66C4CC));

    pSprt->x0 = static_cast<short>(textConfig.gTextX_dword_66C4C0);
    pSprt->y0 = static_cast<short>(textConfig.gTextY_dword_66C4C4);
    pSprt->u0 = 224;
    addPrim(pMenu->field_20_prim_buffer->mOt, pSprt);
    textConfig.gTextX_dword_66C4C0 += kCharWidth;

    // Render 2nd number
    Menu_render_number_as_string_468529(pMenu->field_20_prim_buffer, &textConfig, maxValue);
}
MGS_FUNC_IMPLEX(0x00468915, Menu_render_text_fractional_468915, MENU_IMPL);

// Used when skipping/scrolling through codec text
void CC Menu_draw_blinking_arrow_463652(MenuMan* pMenu, BYTE* ot)
{
    if (g_lib_gv_stru_6BFEE0.gRenderedFramesCount_dword_6BFF00 % 16 >= 4)
    {
        POLY_F3* pPoly = PrimAlloc<POLY_F3>(pMenu->field_20_prim_buffer);

        setPolyF3(pPoly);
        setRGB0(pPoly, 0x80, 0x80, 0x80);

        pPoly->x1 = 288;
        pPoly->y1 = 210;
        pPoly->x0 = pPoly->x1 - 4;
        pPoly->x2 = pPoly->x1 + 4;
        pPoly->y2 = pPoly->y1 - 4;
        pPoly->y0 = pPoly->y2;

        addPrim(ot, pPoly);
    }
}
MGS_FUNC_IMPLEX(0x00463652, Menu_draw_blinking_arrow_463652, MENU_IMPL);

MGS_FUNC_NOT_IMPL(0x45F1D0, void __cdecl (MenuMan *pMenu, char *Source), Menu_font_45F1D0);

void CC Menu_update_4598BC(MenuMan* pMenu)
{
    DWORD* pOtText1 = (DWORD*)gMenuPrimBuffer_7265E0.mOt;
    DWORD* pOtText2 = (DWORD*)gMenuPrimBuffer_7265E0.mOt;
    pMenu->field_24_input = gpActiveButtons_995324 + 2;

    Menu_update_helper_462A3D(pMenu, pOtText2);

    if (!(gActorPauseFlags_dword_791A0C & 2)
        && gLoaderState_dword_9942B8 > 0
        && !script_cancel_non_zero_dword_7227A0
        && (game_state_dword_72279C.flags & 0x80000000) == 0)
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

// Called for each of the 7 possible inventory slots being draw.
// It appears to set the pal or sprite for each item.
void CC Menu_inventory_common_set_icon_46B007(MenuMan_Inventory_14h_Unk* pUnk)
{
    signed int flagBits = 1;
    int index = 0;
    while (flagBits & flags_dword_733CF4)
    {
        flagBits *= 2;
        if (++index >= 9)
        {
            return;
        }
    }
    flags_dword_733CF4 |= flagBits;

    pUnk->field_8_index = index;
    pUnk->field_C_u = gMenu_stru_733CF8[index].field_0_u;
    pUnk->field_D_v = gMenu_stru_733CF8[index].field_1_v;
    pUnk->field_E_clut = gMenu_stru_733CF8[index].field_2_clut;

    gMenu_stru_733CF8[index].field_4_pUnk = pUnk;
    gMenu_stru_733CF8[index].field_8_rect.x2 = pUnk->field_10_w / 4;
    gMenu_stru_733CF8[index].field_8_rect.y2 = pUnk->field_12_h;

    PSX_RECT* pRect = &gMenu_stru_733CF8[index].field_8_rect;
    g_Render_sub_41C640_ret_650D1A = Render_sub_41C640(
        pRect,
        pUnk->field_4_word_ptr_pixels,
        pUnk->field_0_pixels,
        0, 0, 0, 0);
    Render_sub_41C6B0(pRect, pUnk->field_0_pixels);
}
MGS_FUNC_IMPLEX(0x46B007, Menu_inventory_common_set_icon_46B007, MENU_IMPL);

// Seems to render the active/selected menu item
void CC Menu_render_unk_46B081(MenuMan_Inventory_14h_Unk *pUnk, int idx)
{
    Menu_stru_0x18* pStru = &stru_733C90[idx];

    pUnk->field_8_index = 31 - idx;
    pUnk->field_C_u = pStru->field_0_u;
    pUnk->field_D_v = pStru->field_1_v;
    pUnk->field_E_clut = pStru->field_2_clut;

    pStru->field_8_rect.x2 = pUnk->field_10_w / 4;
    pStru->field_8_rect.y2 = pUnk->field_12_h;

    g_Render_sub_41C640_ret_650D1A = Render_sub_41C640(
        &pStru->field_8_rect,
        pUnk->field_4_word_ptr_pixels,
        pUnk->field_0_pixels,
        0, 0, 0, 0);
    Render_sub_41C6B0(&pStru->field_8_rect, pUnk->field_0_pixels);
}
MGS_FUNC_IMPLEX(0x46B081, Menu_render_unk_46B081, MENU_IMPL);

void Menu_render_unk_2_and_3_468C6B()
{
    Menu_render_unk_46B081(&gMenuNoUse_7339A8, 2);
    Menu_render_unk_46B081(&gMenuFrozen_733990, 3);
}
MGS_FUNC_IMPLEX(0x468C6B, Menu_render_unk_2_and_3_468C6B, MENU_IMPL);

void CC Menu_inventory_common_icon_helper_46AFE1(MenuMan_Inventory_14h_Unk* pMenuUnk)
{
    const WORD uvOrClutSet = pMenuUnk->field_C_u | pMenuUnk->field_E_clut | pMenuUnk->field_E_clut;
    if (!uvOrClutSet)
    {
        Menu_inventory_common_set_icon_46B007(pMenuUnk);
    }
    if (pMenuUnk->field_8_index >= 0)
    {
        gBitFlags_dword_733DD0 |= 1 << pMenuUnk->field_8_index;
    }
}
MGS_FUNC_IMPLEX(0x46AFE1, Menu_inventory_common_icon_helper_46AFE1, MENU_IMPL);

void CC TextSetRGB_459B27(int r, int g, int b)
{
    DWORD bEdited = b;
    if (gTextConfig_66C4C0.gTextFlags_dword_66C4C8 & 0x20)
    {
        bEdited |= 0x6600u;
    }
    else
    {
        bEdited |= 0x6400u;
    }
    gTextConfig_66C4C0.gTextRGB_dword_66C4CC = r | ((g | (bEdited << 8)) << 8);
}
MGS_FUNC_IMPLEX(0x459B27, TextSetRGB_459B27, MENU_IMPL);

DWORD ScriptReadBits_45238C()
{
    DWORD ret = 0;
    while (Script_GetReturnAddress())
    {
        const DWORD value = Script_get_int();
        if (value > 32)
        {
            return 0;
        }
        ret |= 1 << value;
    }
    return ret;
}
MGS_FUNC_IMPLEX(0x45238C, ScriptReadBits_45238C, MENU_IMPL);

void CC Menu_Remove_all_items_44D020()
{
    for (int i = 0; i < 24; i++)
    {
        gGameStates_78E7E0.gItem_states_word_78E82A[i] |= 0x8000u;
    }

    for (int i = 0; i < 10; i++)
    {
        gGameStates_78E7E0.gWeapon_states_word_78E802[i] |= 0x8000u;
    }
}
MGS_FUNC_IMPLEX(0x44D020, Menu_Remove_all_items_44D020, MENU_IMPL);

void CC Menu_give_all_items_44D047()
{
    for (int i = 0; i < 24; i++)
    {
        gGameStates_78E7E0.gItem_states_word_78E82A[i] &= ~0x8000u;
    }

    for (int i = 0; i < 10; i++)
    {
        gGameStates_78E7E0.gWeapon_states_word_78E802[i] &= ~0x8000u;
    }
}
MGS_FUNC_IMPLEX(0x44D047, Menu_give_all_items_44D047, MENU_IMPL);

void Menu_update_game_state_4691C6()
{
    game_state_dword_72279C.mParts.flags1 &= 0x7Fu;
}
MGS_FUNC_IMPLEX(0x4691C6, Menu_update_game_state_4691C6, MENU_IMPL);

void Menu_UnsetStates_Free_FontData_46BC17()
{
    gMenuMan_stru_725FC0.field_2B &= 0xFDu;
    Menu_update_game_state_4691C6();
    System_2_free_40B2A7(Font_Get_Ptr_45AB0B(&gMenuFont_733DF0));
}
MGS_FUNC_IMPLEX(0x46BC17, Menu_UnsetStates_Free_FontData_46BC17, MENU_IMPL);

void CC Menu_set_sub_from_unknown_46B1BD(MenuMan_Inventory_Menu_0x14* pMenuSub, Menu_Item_Unknown_Array_Item* pArrayItem)
{
    pMenuSub->field_0_invent.field_0.field_0_item_id_idx = pArrayItem->field_0_item_id_idx;
    pMenuSub->field_0_invent.field_0.field_4 = 0;
    pMenuSub->field_0_invent.field_0.field_2_current_amount = pArrayItem->field_2_current_amount;
    pMenuSub->field_0_invent.field_0.field_6 = 1;
}
MGS_FUNC_IMPLEX(0x46B1BD, Menu_set_sub_from_unknown_46B1BD, MENU_IMPL);

void CC Menu_inventory_left_update_helper_46A856(MenuMan* pMenu)
{
    Menu_Item_Unknown* pItemUnknown = pMenu->field_1D8_invetory_menus[0].field_C_pItem_sys_alloc;
    MenuMan_Inventory_Menu_0x14* pInvent = &pMenu->field_1D8_invetory_menus[0];
    pInvent->field_10_state = InventoryMenuState::eClosed;

    Menu_set_sub_from_unknown_46B1BD(
        pInvent,
        &pItemUnknown->field_20_array + pItemUnknown->field_0_main.field_4_selected_idx);

    if (pInvent->field_0_invent.field_0.field_2_current_amount <= 0)
    {
        pInvent->field_0_invent.field_0.field_0_item_id_idx = -1;
    }

    const int idx = pInvent->field_0_invent.field_0.field_0_item_id_idx;
    if (idx < 0 || Menu_inventory_Is_Item_Disabled_46A128(pInvent->field_0_invent.field_0.field_0_item_id_idx))
    {
        if (idx != -1)
        {
            gMenuLeft_733AD0 = idx;
        }
        gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE = -1;
        pInvent->field_0_invent.field_0.field_0_item_id_idx = -1;
    }
    else
    {
        gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE = idx;
        Menu_render_unk_46B081(&g21_menu_left_inventory_unk_733AD8[gItemInfos_675D30[idx].field_4 - 12], 0);
        pInvent->field_11_item_idx = gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE;
    }

    pInvent->field_12 = 10;
    Menu_inventory_common_checked_free_46B190(pItemUnknown);
    Menu_UnsetStates_Free_FontData_46BC17();
    Sound_sub_44FD66(0, 0x3Fu, 0x14u);
}
MGS_FUNC_IMPLEX(0x46A856, Menu_inventory_left_update_helper_46A856, MENU_IMPL);

int CC Script_tbl_menu_sub_4521A7(BYTE* pScript)
{
    if (Script_ParamExists('j'))
    {
        if (Script_get_int() & 1)
        {
            game_state_dword_72279C.flags |= 0x800000u;
        }
        else
        {
            game_state_dword_72279C.flags &= 0xFF7FFFFF;
        }
    }

    if (Script_ParamExists('n'))
    {
        if (Script_get_int() & 1)
        {
            Menu_Remove_all_items_44D020();
        }
        else
        {
            Menu_give_all_items_44D047();
        }
    }

    if (Script_ParamExists('m'))
    {
        if (Script_get_int() & 1)
        {
            game_state_dword_72279C.flags &= 0xFFF7FFFF;
        }
        else
        {
            game_state_dword_72279C.flags |= 0x80000u;
        }
    }

    if (Script_ParamExists('l'))
    {
        switch (Script_get_int())
        {
        case 0u:
            game_state_dword_72279C.flags |= 0x20000u;
            break;
        case 2u:
            game_state_dword_72279C.flags |= 0x10000u;
            break;
        case 1u:
        case 3u:
            game_state_dword_72279C.flags &= 0xFFFDFFFF;
            break;
        }
    }

    if (Script_ParamExists('r'))
    {
        switch (Script_get_int())
        {
        case 0u:
            game_state_dword_72279C.flags |= 0x400000u;
            break;
        case 1u:
            game_state_dword_72279C.flags &= 0xFFBFFFFF;
            break;
        case 2u:
            game_state_dword_72279C.flags |= 0x200000u;
            break;
        case 3u:
            game_state_dword_72279C.flags |= 0x100000u;
            break;
        }
    }

    if (Script_ParamExists('p'))
    {
        if (Script_get_int() & 1)
        {
            game_state_dword_72279C.mParts.flags1 |= 0x40u;
        }
        else
        {
            game_state_dword_72279C.mParts.flags1 &= 0xBFu;
        }
    }

    if (Script_ParamExists('s'))
    {
        Menu_scale_465A01(Script_get_int());
    }

    // weapons?
    if (Script_ParamExists('w'))
    {
        gMenuRightBits_dword_9942C0 = ScriptReadBits_45238C();
    }

    // items?
    if (Script_ParamExists('i'))
    {
        gMenuLeftBits_dword_9942BC = ScriptReadBits_45238C();
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x4521A7, Script_tbl_menu_sub_4521A7, MENU_IMPL);

void CC Res_countdwn_loader_set_radar_fn_465A87(void* pFn)
{
    gFn_radar_dword_733950 = pFn; // TODO: Type
}
MGS_FUNC_IMPLEX(0x465A87, Res_countdwn_loader_set_radar_fn_465A87, MENU_IMPL);

void sub_462EA6(void)
{
    sub_462E8D();
}
MGS_FUNC_IMPLEX(0x462EA6, sub_462EA6, MENU_IMPL);

MGS_FUNC_NOT_IMPL(0x465967, void(), Radio_unknown_465967);

void CC Menu_inits_459A48()
{
    Radio_unknown_465967();
    sub_462EA6();
    Menu_scale_465A01(4096);
    Res_countdwn_loader_set_radar_fn_465A87(nullptr);
    gMenuMan_stru_725FC0.field_1D5 = 0;
    gMenuMan_stru_725FC0.field_1D6 = 0;
    gMenuMan_stru_725FC0.field_2B = 0;
    gMenuMan_stru_725FC0.field_1D8_invetory_menus[0].field_12 = 0;
    gMenuMan_stru_725FC0.field_1D8_invetory_menus[1].field_12 = 0;
    Menu_init_menu_bars_4691CE(&gMenuMan_stru_725FC0);
}
MGS_FUNC_IMPLEX(0x459A48, Menu_inits_459A48, MENU_IMPL);

void CC Menu_inventory_left_update_helper_46A4C1(MenuMan* pMenu, DWORD* pPrimBuffer)
{
    if (pMenu->field_1D8_invetory_menus[0].field_10_state)
    {
        if (pMenu->field_1D8_invetory_menus[0].field_10_state != InventoryMenuState::eUnknown_1)
        {
            if (pMenu->field_1D8_invetory_menus[0].field_10_state != InventoryMenuState::eOpening)
            {
                if (pMenu->field_1D8_invetory_menus[0].field_10_state != InventoryMenuState::eClosing)
                {
                    return;
                }
                if (Menu_inventory_common_46B2C2())
                {
                    pMenu->field_2A_state = 0;
                    gActorPauseFlags_dword_791A0C &= 0xFFFFFFFB;
                    Menu_inventory_left_update_helper_46A856(pMenu);
                    return;
                }
                Menu_inventory_common_update_helper_46B56C(pMenu, pPrimBuffer, &pMenu->field_1D8_invetory_menus[0]);
                return;
            }
            if (Menu_inventory_common_update_helper_46B2A2())
            {
                pMenu->field_1D8_invetory_menus[0].field_10_state = 1;
            }
            gShowInfoDelay_dword_733C7C = 0;
        }
        
        const Menu_Item_Unknown_Array_Item* pArrayItem = &pMenu->field_1D8_invetory_menus[0].field_C_pItem_sys_alloc->field_20_array
            + pMenu->field_1D8_invetory_menus[0].field_C_pItem_sys_alloc->field_0_main.field_4_selected_idx;

        if (game_state_dword_72279C.flags & 0x1000)
        {
            gInfoShowing_dword_733C80 = 0;
            gShowInfoDelay_dword_733C7C = 0;
        }
        if (pArrayItem->field_4 || pArrayItem->field_0_item_id_idx < 0)
        {
            gInfoShowing_dword_733C80 = 0;
            gShowInfoDelay_dword_733C7C = 0;
        }
        else
        {
            if (++gShowInfoDelay_dword_733C7C == 4)
            {
                Menu_inventory_left_update_ShowItemInfo_46A718(pArrayItem->field_0_item_id_idx);
                gInfoShowing_dword_733C80 = 1;
            }

            if (gInfoShowing_dword_733C80)
            {
                if (pArrayItem->field_0_item_id_idx == Items::ePalKey)
                {
                    Menu_inventory_left_render_PAL_key_icon_46A770(
                        pMenu,
                        pPrimBuffer,
                        gGameStates_78E7E0.gMenu_PAL_card_icon_idx_word_78E862);
                }
                Menu_inventory_draw_item_header_and_background_with_hp_bar_46BA95(pMenu, pPrimBuffer, "EQUIP");
            }
        }
        Menu_inventory_common_update_helper_46B56C(pMenu, pPrimBuffer, pMenu->field_1D8_invetory_menus);
        return;
    }

    const unsigned __int16 field_12 = pMenu->field_1D8_invetory_menus[0].field_12;
    if (field_12 <= 0u)
    {
        if (Menu_inventory_Is_Item_Disabled_46A128(gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE))
        {
            gMenuLeft_733AD0 = gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE;
            gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE = -1;
            pMenu->field_1D8_invetory_menus[0].field_12 = 19;
        }
        else
        {
            const int item_idx = pMenu->field_1D8_invetory_menus[0].field_0_invent.field_0.field_0_item_id_idx;
            signed int bUnknown = 0;
            if (item_idx != gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE)
            {
                bUnknown = 1;
                if (item_idx != -1 && item_idx != 17)
                {
                    gMenuLeft_733AD0 = item_idx;
                }
                pMenu->field_1D8_invetory_menus[0].field_0_invent.field_0.field_0_item_id_idx = gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE;
            }
            if (gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE >= 0)
            {
                if (bUnknown)
                {
                    Menu_render_unk_46B081(&g21_menu_left_inventory_unk_733AD8[gItemInfos_675D30[gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE].field_4 - 12], 0);
                    pMenu->field_1D8_invetory_menus[0].field_11_item_idx = gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE;
                }
                pMenu->field_1D8_invetory_menus[0].field_0_invent.field_0.field_2_current_amount = gGameStates_78E7E0.gItem_states_word_78E82A[gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE];
                Menu_inventory_common_update_helper_46B6EF(pMenu, pPrimBuffer, pMenu->field_1D8_invetory_menus);
            }
        }
    }
    else
    {
        pMenu->field_1D8_invetory_menus[0].field_12 = field_12 - 1;
        const int field_12_mod_4 = field_12 % 4;
        if (field_12_mod_4 > 1)
        {
            Menu_inventory_common_update_helper_46B6EF(pMenu, pPrimBuffer, pMenu->field_1D8_invetory_menus);
            if (field_12_mod_4 == 3)
            {
                const int item_idx = pMenu->field_1D8_invetory_menus[0].field_0_invent.field_0.field_0_item_id_idx;
                if (item_idx != gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE
                    && Menu_inventory_Is_Item_Disabled_46A128(item_idx)
                    && !counter_dword_6BED20)
                {
                    Sound_sub_44FD66(0, 0x3Fu, 0x36u);
                }
            }
        }
    }
}
MGS_FUNC_IMPLEX(0x0046A4C1, Menu_inventory_left_update_helper_46A4C1, MENU_IMPL);

void CC Menu_46B6AD()
{
    const int match = flags_dword_733CF4 & ~gBitFlags_dword_733DD0;
    int bits = 1;
    for (int i=0; i<9; i++)
    {
        if (match & bits)
        {
            gMenu_stru_733CF8[i].field_4_pUnk->field_C_u = 0;
            gMenu_stru_733CF8[i].field_4_pUnk->field_D_v = 0;;
            gMenu_stru_733CF8[i].field_4_pUnk->field_E_clut = 0;
            gMenu_stru_733CF8[i].field_4_pUnk->field_8_index = -1;
            gMenu_stru_733CF8[i].field_4_pUnk = nullptr;
            flags_dword_733CF4 &= ~bits;
        }
        bits *= 2;
    }
}
MGS_FUNC_IMPLEX(0x0046B6AD, Menu_46B6AD, MENU_IMPL);

void CC Menu_inventory_common_update_helper_46B56C(MenuMan* pMenu, DWORD* ot, MenuMan_Inventory_Menu_0x14* pUnk)
{
    gBitFlags_dword_733DD0 = 0;

    Menu_Item_Unknown* pItemUnknown = pUnk->field_C_pItem_sys_alloc;
    MenuMan_Inventory_Unk_6764F8* pInvent = pUnk->field_0_invent.field_8_pMenuMan_Inventory_Unk_6764F8;

    int ypos = 0;
    int xpos = 0;
    Menu_Item_Unknown_Array_Item* pItem = &pItemUnknown->field_20_array + pItemUnknown->field_0_main.field_4_selected_idx;
    if (pItem->field_4 < 0)
    {
        pInvent->field_10_fn_ptrs[1](pInvent, -pItem->field_4, &xpos, &ypos);
    }
    else
    {
        pInvent->field_10_fn_ptrs[0](pInvent, pItem->field_4, &xpos, &ypos);
    }
    pInvent->field_18_fn(pMenu, ot, xpos, ypos, pItem);

    int field_8_capped = pItemUnknown->field_0_main.field_8;
    if (field_8_capped > 1024)
    {
        field_8_capped = 1152;
    }

    int idxOff = 1;
    for (int i=0; i<pItemUnknown->field_0_main.field_0_array_count-1; i++)
    {
        int idx = pItemUnknown->field_0_main.field_4_selected_idx + idxOff;
        if (idx >= pItemUnknown->field_0_main.field_0_array_count)
        {
            idx -= pItemUnknown->field_0_main.field_0_array_count;
        }
        Menu_Item_Unknown_Array_Item* pArrayItem = &pItemUnknown->field_20_array + idx;
        if ((pArrayItem->field_4 & 0x8000u) != 0)
        {
            break;
        }

        if (pArrayItem->field_4 > field_8_capped)
        {
            break;
        }

        int ypos2 = 0;
        int xpos2 = 0;
        pInvent->field_10_fn_ptrs[0](pInvent, pArrayItem->field_4, &ypos2, &xpos2);
        pInvent->field_18_fn(pMenu, ot, ypos2, xpos2, pArrayItem);
        ++idxOff;
    }

    int field_C_capped = pItemUnknown->field_0_main.field_C;
    if (-field_C_capped > 1024)
    {
        field_C_capped = -1152;
    }

    idxOff = -1;
    for (int i = 0; i<pItemUnknown->field_0_main.field_0_array_count - 1; i++)
    {
        int idx = pItemUnknown->field_0_main.field_4_selected_idx + idxOff;
        if (idx < 0)
        {
            idx += pItemUnknown->field_0_main.field_0_array_count;
        }
        Menu_Item_Unknown_Array_Item* pArrayItem = &pItemUnknown->field_20_array + idx;

        if (pArrayItem->field_4 > 0)
        {
            break;
        }

        if (pArrayItem->field_4 < field_C_capped)
        {
            break;
        }

        int ypos2 = 0;
        int xpos2 = 0;
        pInvent->field_10_fn_ptrs[1](pInvent, -pArrayItem->field_4, &xpos2, &ypos2);
        pInvent->field_18_fn(pMenu, ot, xpos2, ypos2, pArrayItem);
        --idxOff;
    }

    Menu_46B6AD();
}
MGS_FUNC_IMPLEX(0x0046B56C, Menu_inventory_common_update_helper_46B56C, MENU_IMPL);

static void Test_Render_Text_Large_font_468AAF()
{
    BYTE backingBuffer[32 * sizeof(SPRT)] = {};
    MenuPrimBuffer buffer;
    buffer.mFreeLocation = backingBuffer;
    buffer.mOt = backingBuffer;
    buffer.mOtEnd = backingBuffer + sizeof(backingBuffer);

    TextConfig config;
    config.gTextX_dword_66C4C0 = 0;
    config.gTextY_dword_66C4C4 = 0;
    config.gTextFlags_dword_66C4C8 = 0;
    config.gTextRGB_dword_66C4CC = 0x64ffffff;

    InitLargeFontTemplateSprite();

    Render_Text_Large_font_468AAF(&buffer, &config, "i Hello\nwor l d \n1234#9567890\n.@:_!?+-/*{}");

    const SPRT expected[31] =
    {
        { 68759560, 255, 255, 255, 100, 1, 0, 64, 242, 32765, 8, 6 },
        { 68758960, 255, 255, 255, 100, 9, 0, 56, 242, 32765, 8, 6 },
        { 68758980, 255, 255, 255, 100, 18, 0, 32, 242, 32765, 8, 6 },
        { 68759000, 255, 255, 255, 100, 27, 0, 88, 242, 32765, 8, 6 },
        { 68759020, 255, 255, 255, 100, 36, 0, 88, 242, 32765, 8, 6 },
        { 68759040, 255, 255, 255, 100, 45, 0, 112, 242, 32765, 8, 6 },
        { 68759060, 255, 255, 255, 100, 0, 8, 176, 242, 32765, 8, 6 },
        { 68759080, 255, 255, 255, 100, 9, 8, 112, 242, 32765, 8, 6 },
        { 68759100, 255, 255, 255, 100, 18, 8, 136, 242, 32765, 8, 6 },
        { 68759120, 255, 255, 255, 100, 31, 8, 88, 242, 32765, 8, 6 },
        { 68759140, 255, 255, 255, 100, 44, 8, 24, 242, 32765, 8, 6 },
        { 68759160, 255, 255, 255, 100, 0, 16, 8, 248, 32765, 8, 6 },
        { 68759180, 255, 255, 255, 100, 9, 16, 16, 248, 32765, 8, 6 },
        { 68759200, 255, 255, 255, 100, 18, 16, 24, 248, 32765, 8, 6 },
        { 68759220, 255, 255, 255, 100, 27, 16, 32, 248, 32765, 8, 6 },
        { 68759240, 255, 255, 255, 100, 45, 16, 40, 248, 32765, 8, 6 },
        { 68759260, 255, 255, 255, 100, 54, 16, 48, 248, 32765, 8, 6 },
        { 68759280, 255, 255, 255, 100, 63, 16, 56, 248, 32765, 8, 6 },
        { 68759300, 255, 255, 255, 100, 72, 16, 64, 248, 32765, 8, 6 },
        { 68759320, 255, 255, 255, 100, 81, 16, 72, 248, 32765, 8, 6 },
        { 68759340, 255, 255, 255, 100, 90, 16, 0, 248, 32765, 8, 6 },
        { 68759360, 255, 255, 255, 100, 1, 24, 80, 248, 32765, 8, 6 },
        { 68759380, 255, 255, 255, 100, 5, 24, 96, 248, 32765, 8, 6 },
        { 68759400, 255, 255, 255, 100, 9, 24, 112, 248, 32765, 8, 6 },
        { 68759420, 255, 255, 255, 100, 12, 24, 120, 248, 32765, 8, 6 },
        { 68759440, 255, 255, 255, 100, 21, 24, 128, 248, 32765, 8, 6 },
        { 68759460, 255, 255, 255, 100, 30, 24, 136, 248, 32765, 8, 6 },
        { 68759480, 255, 255, 255, 100, 39, 24, 144, 248, 32765, 8, 6 },
        { 68759500, 255, 255, 255, 100, 48, 24, 152, 248, 32765, 8, 6 },
        { 68759520, 255, 255, 255, 100, 57, 24, 160, 248, 32765, 8, 6 },
        { 68759540, 255, 255, 255, 100, 60, 24, 168, 248, 32765, 8, 6 }
    };

    const SPRT* pExpectedSprt = &expected[0];
    const SPRT* pActualSprt = reinterpret_cast<SPRT*>(backingBuffer);
    int count = 0;
    while (pActualSprt->tag)
    {
        ASSERT_EQ(pActualSprt->code, pExpectedSprt->code);

        ASSERT_EQ(pActualSprt->r0, pExpectedSprt->r0);
        ASSERT_EQ(pActualSprt->g0, pExpectedSprt->g0);
        ASSERT_EQ(pActualSprt->b0, pExpectedSprt->b0);

        ASSERT_EQ(pActualSprt->x0, pExpectedSprt->x0);
        ASSERT_EQ(pActualSprt->y0, pExpectedSprt->y0);

        ASSERT_EQ(pActualSprt->w, pExpectedSprt->w);
        ASSERT_EQ(pActualSprt->h, pExpectedSprt->h);

        ASSERT_EQ(pActualSprt->u0, pExpectedSprt->u0);
        ASSERT_EQ(pActualSprt->v0, pExpectedSprt->v0);

        ASSERT_EQ(pActualSprt->clut, pExpectedSprt->clut);

        pActualSprt++;
        pExpectedSprt++;
        count++;
    }
    ASSERT_EQ(count, 31);
}

static void Test_Render_Text_Small_font_468642()
{
    BYTE backingBuffer[1024 * 50] = {};
    MenuPrimBuffer buffer;
    buffer.mFreeLocation = backingBuffer;
    buffer.mOt = backingBuffer;
    buffer.mOtEnd = backingBuffer + sizeof(backingBuffer);

    TextConfig config;
    config.gTextX_dword_66C4C0 = 0;
    config.gTextY_dword_66C4C4 = 0;
    config.gTextFlags_dword_66C4C8 = 0;

    InitSmallFontTemplateSprite();

    Render_Text_Small_font_468642(&buffer, &config, "i Hello\nwor l d \n1234#9567890\n.@:_!?+-/*{}");

    const SPRT expected[32] =
    {
        { 68759568, 204, 204, 204, 204, 1, 0, 48, 237, 32764, 6, 5 },
        { 68758948, 204, 204, 204, 204, 8, 0, 42, 237, 32764, 6, 5 },
        { 68758968, 204, 204, 204, 204, 14, 0, 24, 237, 32764, 6, 5 },
        { 68758988, 204, 204, 204, 204, 20, 0, 66, 237, 32764, 6, 5 },
        { 68759008, 204, 204, 204, 204, 26, 0, 66, 237, 32764, 6, 5 },
        { 68759028, 204, 204, 204, 204, 32, 0, 84, 237, 32764, 6, 5 },
        { 68759048, 204, 204, 204, 204, 0, 8, 132, 237, 32764, 6, 5 },
        { 68759068, 204, 204, 204, 204, 6, 8, 84, 237, 32764, 6, 5 },
        { 68759088, 204, 204, 204, 204, 12, 8, 102, 237, 32764, 6, 5 },
        { 68759108, 204, 204, 204, 204, 22, 8, 66, 237, 32764, 6, 5 },
        { 68759128, 204, 204, 204, 204, 32, 8, 18, 237, 32764, 6, 5 },
        { 68759148, 204, 204, 204, 204, 0, 16, 6, 232, 32764, 6, 5 },
        { 68759168, 204, 204, 204, 204, 6, 16, 12, 232, 32764, 6, 5 },
        { 68759188, 204, 204, 204, 204, 12, 16, 18, 232, 32764, 6, 5 },
        { 68759208, 204, 204, 204, 204, 18, 16, 24, 232, 32764, 6, 5 },
        { 68759228, 204, 204, 204, 204, 30, 16, 54, 232, 32764, 6, 5 },
        { 68759248, 204, 204, 204, 204, 36, 16, 30, 232, 32764, 6, 5 },
        { 68759268, 204, 204, 204, 204, 42, 16, 36, 232, 32764, 6, 5 },
        { 68759288, 204, 204, 204, 204, 48, 16, 42, 232, 32764, 6, 5 },
        { 68759308, 204, 204, 204, 204, 54, 16, 48, 232, 32764, 6, 5 },
        { 68759328, 204, 204, 204, 204, 60, 16, 54, 232, 32764, 6, 5 },
        { 68759348, 204, 204, 204, 204, 66, 16, 0, 232, 32764, 6, 5 },
        { 68759368, 204, 204, 204, 204, 1, 24, 60, 232, 32764, 6, 5 },
        { 68759388, 204, 204, 204, 204, 5, 24, 72, 232, 32764, 6, 5 },
        { 68759408, 204, 204, 204, 204, 9, 24, 84, 232, 32764, 6, 5 },
        { 68759428, 204, 204, 204, 204, 12, 24, 90, 232, 32764, 6, 5 },
        { 68759448, 204, 204, 204, 204, 18, 24, 96, 232, 32764, 6, 5 },
        { 68759468, 204, 204, 204, 204, 24, 24, 102, 232, 32764, 6, 5 },
        { 68759488, 204, 204, 204, 204, 30, 24, 108, 232, 32764, 6, 5 },
        { 68759508, 204, 204, 204, 204, 36, 24, 114, 232, 32764, 6, 5 },
        { 68759528, 204, 204, 204, 204, 42, 24, 120, 232, 32764, 6, 5 },
        { 68759548, 204, 204, 204, 204, 45, 24, 126, 232, 32764, 6, 5 }
    };

    const SPRT* pExpectedSprt = &expected[0];
    const SPRT* pActualSprt = reinterpret_cast<SPRT*>(backingBuffer);
    int count = 0;
    while (pActualSprt->tag)
    {
        //ASSERT_EQ(pActualSprt->code, pExpectedSprt->code);

        ASSERT_EQ(pActualSprt->r0, pExpectedSprt->r0);
        ASSERT_EQ(pActualSprt->g0, pExpectedSprt->g0);
        ASSERT_EQ(pActualSprt->b0, pExpectedSprt->b0);

        ASSERT_EQ(pActualSprt->x0, pExpectedSprt->x0);
        ASSERT_EQ(pActualSprt->y0, pExpectedSprt->y0);

        ASSERT_EQ(pActualSprt->w, pExpectedSprt->w);
        ASSERT_EQ(pActualSprt->h, pExpectedSprt->h);

        ASSERT_EQ(pActualSprt->u0, pExpectedSprt->u0);
        ASSERT_EQ(pActualSprt->v0, pExpectedSprt->v0);

        ASSERT_EQ(pActualSprt->clut, pExpectedSprt->clut);

        pActualSprt++;
        pExpectedSprt++;
        count++;
    }
    ASSERT_EQ(count, 32);
}

struct UvPair
{
    int s, u, v;

};
std::map<char, UvPair> gUvs;

static void UVCapture()
{
    InitSmallFontTemplateSprite();

    const char* inStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\n.@:_!?+-/*{}";
    const char* inStrIter = inStr;
    while (*inStrIter)
    {
        BYTE backingBuffer[sizeof(SPRT) * 200] = {};
        MenuPrimBuffer buffer;
        buffer.mFreeLocation = backingBuffer;
        buffer.mOt = backingBuffer;
        buffer.mOtEnd = backingBuffer + sizeof(backingBuffer);

        TextConfig config;
        config.gTextX_dword_66C4C0 = 0;
        config.gTextY_dword_66C4C4 = 0;
        config.gTextFlags_dword_66C4C8 = 0;

        char inBuffer[2] = {};
        inBuffer[0] = *inStrIter;
        Render_Text_Large_font_468AAF(&buffer, &config, inBuffer);

        const SPRT* pActualSprt = reinterpret_cast<SPRT*>(backingBuffer);
        if (pActualSprt->tag)
        {
            gUvs[inBuffer[0]] = { pActualSprt->x0, pActualSprt->u0 , pActualSprt->v0 };
        }
        else
        {
            printf("Nothing for %s\n", inBuffer);
        }

        inStrIter++;
    }

    printf("Done\n");

    for (auto& p : gUvs)
    {
        printf("{ '%c', %d, %d, %d },\n", p.first, p.second.s, p.second.u, p.second.v);
    }
    printf("Done\n");

}

void DoMenuTests()
{
    //UVCapture();
    // Test_Render_Text_Large_font_468AAF();
    // Test_Render_Text_Small_font_468642();
}
