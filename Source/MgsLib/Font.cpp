#include "stdafx.h"
#include "Font.hpp"
#include "Renderer.hpp"

#define FONT_IMPL true

MGS_FUNC_NOT_IMPL(0x45A70D, int __cdecl(Font *ptr, PSX_RECT *pRect, __int16 vramX, __int16 vramY), Font_45A70D);
MGS_FUNC_NOT_IMPL(0x45A796, int __cdecl (Font *pFont, int a2, int a3, int a4, int a5, int a6, int a7), Font_45A796);
MGS_FUNC_NOT_IMPL(0x45A89F, void __cdecl(Font *pFont, signed int index, signed int colour1, signed int colour2), Font_ColourRelated_45A89F);
MGS_FUNC_NOT_IMPL(0x45C6FF, void __cdecl (Font* pFont), Font_Init_data_45C6FF);
MGS_FUNC_NOT_IMPL(0x45AB2D, void __cdecl (Font* pFont, int a2, int a3, const char* pText, int a5), Font_45AB2D);

MGS_VAR(1, 0x6DF240, WORD*, gpFont_field_28_6DF240, nullptr);
MGS_VAR(1, 0x6FC7AC, DWORD, gUseTrueType_dword_6FC7AC, 0);

void CC Font_render_45C76C(Font* pFont)
{
    if (pFont)
    {
        if (gUseTrueType_dword_6FC7AC)
        {
            g_Render_sub_41C640_ret_650D1A = Render_sub_41C640(
                &pFont->field_C_rect,
                gpFont_field_28_6DF240,
                pFont->field_14_pallocP32,
                5,
                0,
                0,
                0);
        }
        else
        {
            g_Render_sub_41C640_ret_650D1A = Render_sub_41C640(
                &pFont->field_C_rect,
                gpFont_field_28_6DF240,
                pFont->field_14_pallocP32,
                0,
                0,
                0,
                0);
        }
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

void CC Font_set_text_45C80A(Font *pFont, const char* pText)
{
    if (pFont)
    {
        Font_Init_data_45C6FF(pFont);
        Font_45AB2D(pFont, 0, pFont->field_3, pText, pFont->field_5);
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
