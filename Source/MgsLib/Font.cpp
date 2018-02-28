#include "stdafx.h"
#include "Font.hpp"

#define FONT_IMPL true

MGS_FUNC_NOT_IMPL(0x45A70D, int __cdecl(Font *ptr, PSX_RECT *pRect, __int16 vramX, __int16 vramY), Font_45A70D);
MGS_FUNC_NOT_IMPL(0x45A796, int __cdecl (Font *pFont, int a2, int a3, int a4, int a5, int a6, int a7), Font_45A796);
MGS_FUNC_NOT_IMPL(0x45AA45, int __cdecl (Font *pFont), Font_CalcSize_45AA45);
MGS_FUNC_NOT_IMPL(0x45A89F, void __cdecl(Font *pFont, signed int index, signed int colour1, signed int colour2), Font_ColourRelated_45A89F);
MGS_FUNC_NOT_IMPL(0x45C6FF, void __cdecl (Font* pFont), Font_Init_data_45C6FF);
MGS_FUNC_NOT_IMPL(0x45AB2D, void __cdecl (Font* pFont, int a2, int a3, const char* pText, int a5), Font_45AB2D);

MGS_VAR(1, 0x6DF240, void*, gpFont_field_28_6DF240, nullptr);

void CC Font_Set_global_alloc_ptr_45C7F2(Font* pFont)
{
    if (pFont)
    {
        gpFont_field_28_6DF240 = pFont->field_28_ptr_sys_allocated;
    }
}
MGS_FUNC_IMPLEX(0x45C7F2, Font_Set_global_alloc_ptr_45C7F2, FONT_IMPL);

void CC Font_Set_Buffer_45AAE9(Font* pFont, void* pAllocated)
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
