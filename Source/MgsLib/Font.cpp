#include "stdafx.h"
#include "Font.hpp"
#include "Renderer.hpp"
#include <gmock/gmock.h>

#define FONT_IMPL true

struct StrReplacement
{
    const char* mFind;
    const char* mReplace;
    int mUnknown1;
    int mUnknown2;
};

StrReplacement stru_68C7D0[25] =
{
    { "O Button", "Action btn", 0, 0 },
    { "O button", "Action btn", 0, 0 },
    { "X Button", "Crawl btn", 0, 0 },
    { "X button", "Crawl btn", 0, 0 },
    { "R1 Button", "Weapon sel btn", 0, 0 },
    { "R1 button", "Weapon sel btn", 0, 0 },
    { "R2 Button", "Weapon inv btn", 0, 0 },
    { "R2 button", "Weapon inv btn", 0, 0 },
    { "L1 Button", "Item inv btn", 0, 0 },
    { "L1 button", "Item inv btn", 0, 0 },
    { "port 2. ", "", 0, 0 },
    { "Controller port 2", "Keyboard", 0, 0 },
    { "Controller port!!", "Keyboard!!", 0, 0 },
    { "Use your R1 Button effectively", "Use Weapon sel btn", 0, 0 },
    { "Plug your Controller into Controller", "Use the Keyboard", 0, 0 },
    { "weapon you used. Press the R1", "weapon you used. Press the Weapon sel", 0, 0 },
    { "Button while you're holding a weapon", "btn while you're holding a weapon", 0, 0 },
    { "To use it, first hold down the R2", "To use it, first hold down the Weapon", 0, 0 },
    { "Button to enter ", "inv btn to enter ", 0, 0 },
    { "by pressing the R1 button or step to", "by pressing Weapon sel btn or step to",  0, 0 },
    { "want, let go of the R2 Button to exit", "want,let go of Weapon inv btn to exit", 0, 0 },
    { "While you're barehanded,", "While you're barehanded, press the", 0, 0 },
    { "press the R1 Button to equip the last", "Weapon sel btn to equip the last", 0, 0 },
    { "Dragged", "Drugged", 0, 0 },
    { "dragged", "drugged", 0, 0 }
};

void CC Font_build_replacement_table_520511();
MGS_FUNC_IMPLEX(0x520511, Font_build_replacement_table_520511, FONT_IMPL);

MGS_ARY(1, 0x77C938, DWORD, 256, gTextCharTable_dword_77C938, {});

void CC Font_build_replacement_table_520511()
{
    Font_build_replacement_table_520511_.Ptr()();
    /*
    for (int i = 0; i < 256; i++)
    {
        LOG_INFO("Value: " << gTextCharTable_dword_77C938[i]);
    }*/
}


int CC Font_45A70D(Font* pFont, PSX_RECT* pRect, __int16 vramX, __int16 vramY)
{
    if (pFont)
    {
        Font font = {};
        memcpy(pFont, &font, sizeof(Font)); // OG bug: copy was done outside of null check

        pFont->field_8 = pRect;
        pFont->field_C_rect.x1 = pRect->x1;
        pFont->field_C_rect.y1 = pRect->y1;

        pFont->field_C_rect.x2 = pRect->x2;
        pFont->field_C_rect.y2 = pRect->y2;

        pFont->field_20_vramx = vramX;
        pFont->field_22_vramy = vramY;
        pFont->field_24_bitDepth = 16;
        pFont->field_26 = 1;
        return Font_45A796(pFont, -1, -1, 0, 0, 4, 0);
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x45A70D, Font_45A70D, FONT_IMPL);


MGS_FUNC_NOT_IMPL(0x45A796, int __cdecl (Font *pFont, int a2, int a3, int a4, int a5, int a6, int a7), Font_45A796);
MGS_FUNC_NOT_IMPL(0x45A89F, void __cdecl(Font *pFont, signed int index, signed int colour1, signed int colour2), Font_ColourRelated_45A89F);
MGS_FUNC_NOT_IMPL(0x45C6FF, void __cdecl (Font* pFont), Font_Init_data_45C6FF);

MGS_VAR(1, 0x6DF240, WORD*, gpFont_field_28_6DF240, nullptr);
MGS_VAR(1, 0x6FC7AC, DWORD, gUseTrueType_dword_6FC7AC, 0);

void CC Font_render_45C76C(Font* pFont)
{
    if (pFont)
    {
        g_Render_sub_41C640_ret_650D1A = Render_sub_41C640(
            &pFont->field_C_rect,
            gpFont_field_28_6DF240,
            pFont->field_14_pallocP32,
            gUseTrueType_dword_6FC7AC ? 5 : 0, // Type 5 is a "magic" 280 byte type
            0,
            0,
            0);
        Render_sub_41C6B0(&pFont->field_C_rect, pFont->field_14_pallocP32);
    }
}
MGS_FUNC_IMPLEX(0x45C76C, Font_render_45C76C, FONT_IMPL);

void CC Font_Set_global_alloc_ptr_45C7F2(Font* pFont)
{
    if (pFont)
    {
        gpFont_field_28_6DF240 = pFont->field_28_ptr_sys_allocated;
    }
}
MGS_FUNC_IMPLEX(0x45C7F2, Font_Set_global_alloc_ptr_45C7F2, FONT_IMPL);

void CC Font_Set_Buffer_45AAE9(Font* pFont, WORD* pAllocated)
{
    if (pFont)
    {
        pFont->field_28_ptr_sys_allocated = pAllocated;
        pFont->field_14_pallocP32 = (BYTE*)pAllocated + 32;
    }
}
MGS_FUNC_IMPLEX(0x45AAE9, Font_Set_Buffer_45AAE9, FONT_IMPL);

void CC Font_set_text_45C80A(Font *pFont, char* pText)
{
    if (pFont)
    {
        Font_Init_data_45C6FF(pFont);
        Font_set_text_shift_jis_45AB2D(pFont, 0, pFont->field_3, pText, pFont->field_5);
    }
}
MGS_FUNC_IMPLEX(0x45C80A, Font_set_text_45C80A, FONT_IMPL);

void* CC Font_Get_Ptr_45AB0B(Font* pFont)
{
    if (pFont)
    {
        return pFont->field_28_ptr_sys_allocated;
    }
    return nullptr;
}
MGS_FUNC_IMPLEX(0x45AB0B, Font_Get_Ptr_45AB0B, FONT_IMPL);

void* CC Font_get_alloc_474D8D(Font* pFont)
{
    return pFont->field_28_ptr_sys_allocated;
}
MGS_FUNC_IMPLEX(0x474D8D, Font_get_alloc_474D8D, FONT_IMPL);

int CC Font_CalcSize_45AA45(Font* pFont)
{
    if (!pFont)
    {
        return 0;
    }
    pFont->field_1A = pFont->field_0 * (pFont->field_2 + 12) - pFont->field_2;
    pFont->field_18_wh = 4 * ((pFont->field_1A + 7) / 8);
    pFont->field_1C_wh = pFont->field_1 * (pFont->field_3 + 12) + 2;
    pFont->field_C_rect.x2 = 2 * pFont->field_18_wh / 4;
    pFont->field_C_rect.y2 = pFont->field_1C_wh;
    return pFont->field_1C_wh * pFont->field_18_wh + 32;
}
MGS_FUNC_IMPLEX(0x45AA45, Font_CalcSize_45AA45, FONT_IMPL);

void __cdecl Font_set_text_shift_jis_45AB2D(Font *pFont, int a2, int a3, char *pText, int a5)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
}
MGS_FUNC_IMPLEX(0x45AB2D, Font_set_text_shift_jis_45AB2D, false); // TODO


MGS_FUNC_NOT_IMPL(0x520458, void __cdecl(char* pText), Font_replace_psx_with_pc_strings_520458);

void DoFontTests()
{
    Font_build_replacement_table_520511();

    {
        char buffer[1024] = "hello world";
        Font_replace_psx_with_pc_strings_520458(buffer);
        ASSERT_STREQ("hello world", buffer);
    }

    {
        char buffer[1024] = "Press O Button to lol";
        Font_replace_psx_with_pc_strings_520458(buffer);
        ASSERT_STREQ("Press Action btn to lol", buffer);
    }
}
