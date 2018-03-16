#include "stdafx.h"
#include "Font.hpp"
#include "Renderer.hpp"

#include "Actor_GameD.hpp"

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

struct FontTextLine
{
    DWORD field_0_x;
    DWORD field_4_y;
    DWORD field_8_width;
    DWORD field_C_height;
    char field_10_pText[256];
    DWORD field_110;
    FontTextLine* field_114_pNext;
};
MGS_ASSERT_SIZEOF(FontTextLine, 0x118);

#pragma pack(push)
#pragma pack(1)
struct FontTextLineSource
{
    BYTE field_0_text_length;
    BYTE field_1_x;
    BYTE field_2_y;
    BYTE field_3_width;
    BYTE field_4_height;
    WORD field_5_unknown;
    char field_7_text_buffer[1];
};
#pragma pack(pop)

void* CC Render_alloc_font_type5_4241C2(const BYTE* pixelData, const WORD* pPallete)
{
    DWORD palBits = 0;
    for (int i = 0; i < 16; i++)
    {
        palBits |= pPallete[i];
    }

    if (!palBits)
    {
        return nullptr;
    }

    FontTextLine* pFirst = nullptr;
    const FontTextLineSource* pSrc = reinterpret_cast<const FontTextLineSource*>(pixelData);
    if (pSrc->field_0_text_length > 0)
    {
        FontTextLine* pPrevious = nullptr;
        do
        {
            FontTextLine* pAllocatedTextLine = (FontTextLine *)malloc(sizeof(FontTextLine));
            
            // Stop if allocation failed
            if (!pAllocatedTextLine)
            {
                break;
            }

            // Set next to the previously allocated item
            if (pPrevious)
            {
                pPrevious->field_114_pNext = pAllocatedTextLine;
            }

            // Keep a pointer to the very first item as the return value
            if (!pFirst)
            {
                pFirst = pAllocatedTextLine;
            }

            // Copy scaled fields
            pAllocatedTextLine->field_0_x = static_cast<DWORD>(pSrc->field_1_x * gXRes);
            pAllocatedTextLine->field_4_y = static_cast<DWORD>(pSrc->field_2_y * gXRes);
            pAllocatedTextLine->field_8_width = static_cast<DWORD>(pSrc->field_3_width * gXRes);
            pAllocatedTextLine->field_C_height = static_cast<DWORD>(pSrc->field_4_height * gXRes);
            pAllocatedTextLine->field_110 = pSrc->field_5_unknown;

            // Copy text buffer
            DWORD textBufferIdx = 0;
            for (textBufferIdx = 0; textBufferIdx < pSrc->field_0_text_length; textBufferIdx++)
            {
                if (textBufferIdx >= sizeof(FontTextLine::field_10_pText) - 2)
                {
                    break;
                }
                pAllocatedTextLine->field_10_pText[textBufferIdx] = pSrc->field_7_text_buffer[textBufferIdx];
            }
            
            // Add null terminator
            pAllocatedTextLine->field_10_pText[textBufferIdx] = 0;
            pAllocatedTextLine->field_114_pNext = nullptr;

            pPrevious = pAllocatedTextLine;

            // Move to the next source item
            const DWORD curTextLen = pSrc->field_0_text_length;
            pSrc = (FontTextLineSource *)(((BYTE*)pSrc) + curTextLen + 7);
        } while (pSrc->field_0_text_length > 0);
    }
    return pFirst;
}
MGS_FUNC_IMPLEX(0x4241C2, Render_alloc_font_type5_4241C2, FONT_IMPL);

void CC Font_TrueTypeLinesFree_4241A4(FontTextLine* pSurface)
{
    FontTextLine* pCurrent = pSurface;
    if (pCurrent)
    {
        FontTextLine* pNext = nullptr;
        do
        {
            pNext = pCurrent->field_114_pNext;
            free(pCurrent);
            pCurrent = pNext;
        } while (pNext);
    }
}
MGS_FUNC_IMPLEX(0x4241A4, Font_TrueTypeLinesFree_4241A4, FONT_IMPL);

MGS_FUNC_NOT_IMPL(0x45A796, int __cdecl (Font *pFont, int a2, int a3, int a4, int a5, int a6, int a7), Font_45A796);
MGS_FUNC_NOT_IMPL(0x45A89F, void __cdecl(Font *pFont, signed int index, signed int colour1, signed int colour2), Font_ColourRelated_45A89F);
MGS_FUNC_NOT_IMPL(0x45C6FF, void __cdecl (Font* pFont), Font_Init_data_45C6FF);

MGS_VAR(1, 0x6DF240, WORD*, gpFont_field_28_palette_6DF240, nullptr);
MGS_VAR(1, 0x6FC7AC, DWORD, gUseTrueType_dword_6FC7AC, 0);

void CC Font_render_45C76C(Font* pFont)
{
    if (pFont)
    {
        // field_14_pPixelData is
        // x
        // y
        // ??
        // ??
        // ??
        // [text buffer]
        // Repeated while x != 0

        g_Render_sub_41C640_ret_650D1A = Render_sub_41C640(
            &pFont->field_C_rect,
            gpFont_field_28_palette_6DF240,
            pFont->field_14_pPixelData,
            gUseTrueType_dword_6FC7AC ? 5 : 0, // Type 5 is a "magic" 280 byte type
            0,
            0,
            0);
        Render_sub_41C6B0(&pFont->field_C_rect, pFont->field_14_pPixelData);
    }
}
MGS_FUNC_IMPLEX(0x45C76C, Font_render_45C76C, FONT_IMPL);

void CC Font_Set_global_alloc_ptr_45C7F2(Font* pFont)
{
    if (pFont)
    {
        gpFont_field_28_palette_6DF240 = pFont->field_28_palette_ptr_sys_allocated;
    }
}
MGS_FUNC_IMPLEX(0x45C7F2, Font_Set_global_alloc_ptr_45C7F2, FONT_IMPL);

void CC Font_Set_Buffer_45AAE9(Font* pFont, WORD* pAllocated)
{
    if (pFont)
    {
        pFont->field_28_palette_ptr_sys_allocated = pAllocated; // 16 colours in the palette
        pFont->field_14_pPixelData = (BYTE*)pAllocated + 32; // Other data is the indexed texture/pixel buffer
    }
}
MGS_FUNC_IMPLEX(0x45AAE9, Font_Set_Buffer_45AAE9, FONT_IMPL);

void CC Font_set_text_45C80A(Font* pFont, char* pText)
{
    if (pFont)
    {
        Font_Init_data_45C6FF(pFont);
        Font_set_text_shift_jis_45AB2D(pFont, 0, pFont->field_3_line_spacing, (BYTE*)pText, pFont->field_5);
    }
}
MGS_FUNC_IMPLEX(0x45C80A, Font_set_text_45C80A, FONT_IMPL);

void* CC Font_Get_Palette_45AB0B(Font* pFont)
{
    if (pFont)
    {
        return pFont->field_28_palette_ptr_sys_allocated;
    }
    return nullptr;
}
MGS_FUNC_IMPLEX(0x45AB0B, Font_Get_Palette_45AB0B, FONT_IMPL);

void* CC Font_Get_Palette_474D8D(Font* pFont)
{
    return pFont->field_28_palette_ptr_sys_allocated;
}
MGS_FUNC_IMPLEX(0x474D8D, Font_Get_Palette_474D8D, FONT_IMPL);

int CC Font_CalcSize_45AA45(Font* pFont)
{
    if (!pFont)
    {
        return 0;
    }
    pFont->field_1A = pFont->field_0 * (pFont->field_2 + 12) - pFont->field_2;
    pFont->field_18_wh = 4 * ((pFont->field_1A + 7) / 8);
    pFont->field_1C_wh = pFont->field_1_max_lines * (pFont->field_3_line_spacing + 12) + 2;
    pFont->field_C_rect.x2 = 2 * pFont->field_18_wh / 4;
    pFont->field_C_rect.y2 = pFont->field_1C_wh;
    return pFont->field_1C_wh * pFont->field_18_wh + 32;
}
MGS_FUNC_IMPLEX(0x45AA45, Font_CalcSize_45AA45, FONT_IMPL);

MGS_VAR(1, 0x66C530, DWORD, dword_66C530, 1);
MGS_VAR(1, 0x732E1C, DWORD, dword_732E1C, 0);

MGS_ARY(1, 0x732E28, DWORD, 4, dword_732E28, {});

MGS_VAR(1, 0x732E3C, DWORD, dword_732E3C, 0);
MGS_VAR(1, 0x732E40, DWORD, dword_732E40, 0);
MGS_VAR(1, 0x732E44, DWORD, dword_732E44, 0);
MGS_VAR(1, 0x732E48, DWORD, dword_732E48, 0);

MGS_VAR(1, 0x72AE10, BYTE*, gFile_CA68u_font_res_72AE10, nullptr);

MGS_VAR(1, 0x732E14, DWORD, gFont_wxh_dword_732E14, 0);
MGS_VAR(1, 0x732E18, BYTE*, gFont_pixel_buffer_732E18, 0);



char __cdecl Font_45B90B(BYTE* pTexturePixels, int a2, int a3, int a4, BYTE *a5)
{
    return 0;
}
MGS_FUNC_IMPLEX(0x45B90B, Font_45B90B, false); // TODO

int __cdecl Font_add_char_to_atlas_45BD91(BYTE* pTexturePixels, signed int xpos, int ypos, int fontTexturePitch, char charToRender)
{
    return 0;
}
MGS_FUNC_IMPLEX(0x45BD91, Font_add_char_to_atlas_45BD91, false); // TODO

void __cdecl Font_rubi_map2_45C1E9(BYTE* pTexturePixels, signed int a2, int a3, int a4, int a5)
{

}
MGS_FUNC_IMPLEX(0x45C1E9, Font_rubi_map2_45C1E9, false); // TODO

unsigned int __cdecl Font_45C16A(signed int a1)
{
    return 0;
}
MGS_FUNC_IMPLEX(0x45C16A, Font_45C16A, false); // TODO

int __cdecl Font_map_char_45B80A(signed int a1)
{
    return 0;
}
MGS_FUNC_IMPLEX(0x45B80A, Font_map_char_45B80A, false); // TODO

int __cdecl Font_map_unknown_45C1DC(int a1)
{
    return 0;
}
MGS_FUNC_IMPLEX(0x45C1DC, Font_map_unknown_45C1DC, false); // TODO

BYTE *__cdecl Font_set_text_520419(BYTE *Source)
{
    return nullptr;
}
MGS_FUNC_IMPLEX(0x520419, Font_set_text_520419, false); // TODO

int __cdecl sub_45C1BF(int a1, int a2, int a3)
{
    return 0;
}
MGS_FUNC_IMPLEX(0x45C1BF, sub_45C1BF, false); // TODO

static WORD NextVarChar(BYTE*& pTextIter)
{
    WORD text2Chars = 0;
    if (*pTextIter >= 128)
    {
        text2Chars = pTextIter[1] | (*pTextIter << 8);
    }
    else
    {
        text2Chars = pTextIter[0] | (0x80 << 8);
    }
    return text2Chars;
}

void __cdecl Font_set_text_shift_jis_45AB2D(Font *pFont, int kZero, int field_3, BYTE *pText, int field_5)
{
    BYTE *pUpdatedText; // eax
    int singlechar; // eax
    int v7; // eax
    int v8; // eax
    int v9; // eax
    int v10; // ST38_4
    int v11; // eax
    signed int v12; // eax
    int v13; // eax
    int v14; // eax
    int v15; // eax
    int v16; // eax
    int v17; // eax
    int v18; // eax
    int v19; // eax
    int v20; // eax
    int v21; // eax
    int v22; // eax
    int v23; // ST4C_4
    int v24; // eax
    int v25; // ST5C_4
    int v26; // eax
    int v27; // eax
    int v28; // eax
    int v29; // eax
    signed int v30; // eax
    int v31; // eax
    int v32; // eax
    signed int v33; // eax
    signed int v34; // eax
    int v35; // eax
    int v36; // eax
    int v37; // eax
    int v38; // eax
    signed int v39; // eax
    int v40; // eax
    int v41; // [esp+4h] [ebp-68h]
    int v42; // [esp+18h] [ebp-54h]
    int v43; // [esp+20h] [ebp-4Ch]
    int v44; // [esp+24h] [ebp-48h]
    int v45; // [esp+24h] [ebp-48h]
    int v46; // [esp+24h] [ebp-48h]
    int v47; // [esp+24h] [ebp-48h]
    int v48; // [esp+24h] [ebp-48h]
    int v49; // [esp+28h] [ebp-44h]
    signed int v50; // [esp+2Ch] [ebp-40h]
    int v51; // [esp+30h] [ebp-3Ch]
    BYTE *pTextIter; // [esp+34h] [ebp-38h]
    unsigned __int8 *pTextItera; // [esp+34h] [ebp-38h]
    signed int v54; // [esp+38h] [ebp-34h]
    int v55; // [esp+38h] [ebp-34h]
    int v56; // [esp+38h] [ebp-34h]
    int v57; // [esp+38h] [ebp-34h]
    int v58; // [esp+38h] [ebp-34h]
    int v59; // [esp+3Ch] [ebp-30h]
    BYTE *v60; // [esp+40h] [ebp-2Ch]
    int text2Chars; // [esp+48h] [ebp-24h]
    int text2Charsa; // [esp+48h] [ebp-24h]
    int text2Charsb; // [esp+48h] [ebp-24h]
    int text2Charsc; // [esp+48h] [ebp-24h]
    signed int v65; // [esp+4Ch] [ebp-20h]
    int v66; // [esp+50h] [ebp-1Ch]
    int v67; // [esp+54h] [ebp-18h]
    signed int v68; // [esp+58h] [ebp-14h]
    int v69; // [esp+64h] [ebp-8h]

    /*
    gUseTrueType_dword_6FC7AC = 0;

    //gUseTrueType_dword_6FC7AC = 0;
    BYTE test[] = { 'p',  0x90, 0x41,  0x0 };
    pText = test;
    */

    // 0x9010 = %
    // 0x9011 = &
    // 0x9012 = [
    // 0x9013 = ]
    // 0x9040 = jp char


    if (gFile_CA68u_font_res_72AE10 && pFont)
    {
        if (gUseTrueType_dword_6FC7AC)
        {
            gFont_wxh_dword_732E14 = pFont->field_1C_wh * pFont->field_18_wh;
            gFont_pixel_buffer_732E18 = pFont->field_14_pPixelData;
            dword_732E1C = 1;
        }
        pUpdatedText = Font_set_text_520419(pText);
        BYTE* pBakedPixelData = pFont->field_14_pPixelData;
        dword_732E3C = 0;
        v59 = pFont->field_1A;
        v51 = pFont->field_1C_wh;
        v50 = 0;
        if (!(pFont->field_6_flags & 2))
        {
            v59 -= 12;
        }
        v69 = pFont->field_18_wh;
        dword_732E40 = field_5;
        v68 = 0;
        v67 = kZero;
        v66 = field_3;
        pFont->field_7_out_max_width = 0;
        pFont->field_1E = pFont->field_3_line_spacing + 14;
        pFont->field_6_flags &= 0xEFu;
        v65 = 0;
        pTextIter = pUpdatedText;
        while (1)
        {
            if (!pTextIter || !*pTextIter)
            {
                return;
            }
            v7 = NextVarChar(pTextIter);

            if ((signed int)(unsigned __int8)*pTextIter >= 128)
            {
                text2Chars = (unsigned __int8)pTextIter[1] | ((unsigned __int8)*pTextIter << 8);
            }
            else
            {
                singlechar = (unsigned __int8)*pTextIter;
                BYTE1(singlechar) |= 0x80u;
                text2Chars = singlechar;
            }

            v7 = text2Chars;
            BYTE1(v7) &= 0x9Fu;
            v54 = v7;
            if (text2Chars < 0x8020)
            {
                if ((signed int)(unsigned __int8)*pTextIter >= 128)
                {
                    text2Chars = (unsigned __int8)pTextIter[1] | ((unsigned __int8)*pTextIter << 8);
                    pTextIter += 2;
                }
                else
                {
                    v8 = (unsigned __int8)*pTextIter;
                    BYTE1(v8) |= 0x80u;
                    text2Chars = v8;
                    ++pTextIter;
                }
                goto LABEL_112;
            }
            if (v7 == 0x8023)
            {
                if ((signed int)(unsigned __int8)*pTextIter >= 128)
                {
                    v10 = (unsigned __int8)pTextIter[1] | ((unsigned __int8)*pTextIter << 8);
                    pTextItera = (unsigned __int8 *)(pTextIter + 2);
                }
                else
                {
                    v9 = (unsigned __int8)*pTextIter;
                    pTextItera = (unsigned __int8 *)(pTextIter + 1);
                }
                if ((signed int)*pTextItera >= 128)
                {
                    v44 = pTextItera[1] | (*pTextItera << 8);
                    pTextIter = (pTextItera + 2);
                }
                else
                {
                    v11 = *pTextItera;
                    BYTE1(v11) |= 0x80u;
                    v44 = v11;
                    pTextIter = (pTextItera + 1);
                }
                v12 = v44;
                BYTE1(v12) &= 0x9Fu;
                if (v12 > 0x8054)
                {
                    if (v12 != 0x8057)
                    {
                        if (v12 != 0x807B)
                        {
                            switch (v12)
                            {
                            case 0x9006:
                            LABEL_60:
                                v49 = Font_add_char_to_atlas_45BD91(pBakedPixelData, v67, v66, v69, 0);
                                goto LABEL_160;
                            case 0x901D:
                                if ((gGameStates_78E7E0.gFlags_dword_78E7E4 & 7) == 2)
                                {
                                    v54 = 0x9024;
                                }
                                else
                                {
                                    v54 = v12;
                                }
                                goto LABEL_139;
                            case 0x901E:
                            case 0x9024:
                                v41 = gGameStates_78E7E0.gFlags_dword_78E7E4 & 7;
                                if (gGameStates_78E7E0.gFlags_dword_78E7E4 & 7)
                                {
                                    if (v41 == 1)
                                    {
                                        v54 = 0x9018;
                                    }
                                    else if (v41 == 2)
                                    {
                                        v54 = 0x901D;
                                    }
                                }
                                else
                                {
                                    v54 = v12;
                                }
                                goto LABEL_139;
                            }
                        LABEL_84:
                            pTextIter -= 4;
                            goto LABEL_112;
                        }
                        goto LABEL_42;
                    }
                    if (v67 > pFont->field_7_out_max_width)
                    {
                        pFont->field_7_out_max_width = v67;
                    }
                    if (v65)
                    {
                        goto LABEL_207;
                    }
                    v49 = 0;
                    goto LABEL_160;
                }
                if (v12 != 0x8054)
                {
                    switch (v12)
                    {
                    case 0x802D:
                        goto LABEL_60;
                    case 0x8031:
                        v54 = 0x8040;
                    LABEL_139:
                        if (v54 >= 0x8100)
                        {
                            v31 = Font_map_char_45B80A(v54);
                            if (v31 <= 0)
                            {
                                Font_45B90B(pBakedPixelData, v67, v66, v69, 0);
                            }
                            else
                            {
                                Font_45B90B(pBakedPixelData, v67, v66, v69, (BYTE *)(36 * ((v31 & 0xFFF) - 1) + dword_732E28[v31 / 4096]));
                            }
                            v49 = pFont->field_2 + 12;
                        }
                        else
                        {
                            v49 = pFont->field_2 + Font_add_char_to_atlas_45BD91(pBakedPixelData, v67, v66, v69, v54);
                            if (v54 == 0x8021 || v54 == 0x803F)
                            {
                                if ((signed int)(unsigned __int8)*pTextIter >= 128)
                                {
                                    v57 = (unsigned __int8)pTextIter[1] | ((unsigned __int8)*pTextIter << 8);
                                }
                                else
                                {
                                    v29 = (unsigned __int8)*pTextIter;
                                    BYTE1(v29) |= 0x80u;
                                    v57 = v29;
                                }
                                v30 = v57;
                                BYTE1(v30) &= 0x9Fu;
                                if (v30 != 0x8021 && v30 != 0x803F)
                                {
                                    if (v30 == 0x8029 || v30 == 0x8028 || v30 <= 36878 && v30 >= 36873 || v30 == 36885)
                                    {
                                        v49 += 2;
                                    }
                                    else
                                    {
                                        v49 += 8;
                                    }
                                }
                            }
                        }
                        v65 = 1;
                        goto LABEL_160;
                    case 0x8032:
                        v54 = 32894;
                        goto LABEL_139;
                    case 0x804E:
                        text2Chars = 0x800A;
                        goto LABEL_112;
                    }
                    if (v12 != 0x8052)
                    {
                        if (v12 == 0x8053)
                        {
                            text2Chars = 0x8009;
                            goto LABEL_112;
                        }
                        goto LABEL_84;
                    }
                    pTextIter += 2;
                LABEL_42:
                    dword_732E44 = 1;
                    if (dword_66C530)
                    {
                        dword_732E48 = 1;
                        sub_45C1BF(v59, v67, v66);
                        if ((signed int)(unsigned __int8)*pTextIter >= 128)
                        {
                            v45 = (unsigned __int8)pTextIter[1] | ((unsigned __int8)*pTextIter << 8);
                        }
                        else
                        {
                            v13 = (unsigned __int8)*pTextIter;
                            BYTE1(v13) |= 0x80u;
                            v45 = v13;
                        }
                        v14 = v45;
                        BYTE1(v14) &= 0x9Fu;
                        if (v14 == 0x8021)
                        {
                            pTextIter += 2;
                        }
                    }
                    else
                    {
                        if ((signed int)(unsigned __int8)*pTextIter >= 128)
                        {
                            v46 = (unsigned __int8)pTextIter[1] | ((unsigned __int8)*pTextIter << 8);
                        }
                        else
                        {
                            v15 = (unsigned __int8)*pTextIter;
                            BYTE1(v15) |= 0x80u;
                            v46 = v15;
                        }
                        v16 = v46;
                        BYTE1(v16) &= 0x9Fu;
                        v47 = v16;
                        if (v16 == 0x8021)
                        {
                            while (v47 != 0x802C && v47 != 0x9002)
                            {
                                if ((signed int)(unsigned __int8)*pTextIter >= 128)
                                {
                                    v48 = (unsigned __int8)pTextIter[1] | ((unsigned __int8)*pTextIter << 8);
                                    pTextIter += 2;
                                }
                                else
                                {
                                    v17 = (unsigned __int8)*pTextIter;
                                    BYTE1(v17) |= 0x80u;
                                    v48 = v17;
                                    ++pTextIter;
                                }
                                v18 = v48;
                                BYTE1(v18) &= 0x9Fu;
                                v47 = v18;
                            }
                        }
                    }
                    goto LABEL_112;
                }
                pTextIter += 2;
                if ((signed int)pFont->field_3_line_spacing > 2)
                {
                    dword_732E3C = 1;
                }
                dword_732E44 = 1;
            LABEL_112:
                if (text2Chars >= 0x8020)
                {
                    v60 = pTextIter;
                    if ((signed int)(unsigned __int8)*pTextIter >= 128)
                    {
                        text2Chars = (unsigned __int8)pTextIter[1] | ((unsigned __int8)*pTextIter << 8);
                        pTextIter += 2;
                    }
                    else
                    {
                        v27 = (unsigned __int8)*pTextIter;
                        BYTE1(v27) |= 0x80u;
                        text2Chars = v27;
                        ++pTextIter;
                    }
                    v28 = text2Chars;
                    BYTE1(v28) &= 0x9Fu;
                    v54 = v28;
                    goto LABEL_139;
                }
                switch (text2Chars)
                {
                case 0x8009:
                    v43 = (unsigned __int8)pFont->field_4 * (pFont->field_2 + 12);
                    if (v43 > 0)
                    {
                        v67 = v43 * (v67 / v43 + 1);
                    }
                    break;
                case 0x800A:
                    if (v68)
                    {
                        v68 = 0;
                    }
                    else
                    {
                        if (v67 > pFont->field_7_out_max_width)
                        {
                            pFont->field_7_out_max_width = v67;
                        }
                        v67 = kZero;
                        v66 += pFont->field_3_line_spacing + 12;
                        pFont->field_1E = v66 + 14;
                        ++v50;
                        if (v66 + 11 >= v51 || pFont->field_6_flags & 1 || v50 >= pFont->field_1_max_lines)
                        {
                            goto LABEL_207;
                        }
                    }
                    break;
                case 0x800C:
                    if ((signed int)(unsigned __int8)*pTextIter >= 128)
                    {
                        text2Charsa = (unsigned __int8)pTextIter[1] | ((unsigned __int8)*pTextIter << 8);
                        pTextIter += 2;
                    }
                    else
                    {
                        v26 = (unsigned __int8)*pTextIter;
                        BYTE1(v26) |= 0x80u;
                        text2Charsa = v26;
                        ++pTextIter;
                    }
                    dword_732E40 = text2Charsa - 0x8030;
                    break;
                }
            }
            else
            {
                if (!dword_732E44 || v7 != 0x9002 && v7 != 0x9004 && v7 != 0x807D)
                {
                    if (v7 == 0x807C)
                    {
                        if ((signed int)(unsigned __int8)*pTextIter >= 128)
                        {
                            v25 = (unsigned __int8)pTextIter[1] | ((unsigned __int8)*pTextIter << 8);
                            pTextIter += 2;
                        }
                        else
                        {
                            v24 = (unsigned __int8)*pTextIter++;
                        }
                        text2Chars = 0x800A;
                    }
                    goto LABEL_112;
                }
                if (dword_732E48 == 1 && (v7 == 0x9002 || v7 == 0x9004))
                {
                    Font_rubi_map2_45C1E9(pBakedPixelData, v67, v66, v69, (int)(pTextIter + 2));
                }
                dword_732E48 = 0;
                do
                {
                    if ((signed int)(unsigned __int8)*pTextIter >= 128)
                    {
                        v55 = (unsigned __int8)pTextIter[1] | ((unsigned __int8)*pTextIter << 8);
                        pTextIter += 2;
                    }
                    else
                    {
                        v19 = (unsigned __int8)*pTextIter;
                        BYTE1(v19) |= 0x80u;
                        v55 = v19;
                        ++pTextIter;
                    }
                    v20 = v55;
                    BYTE1(v20) &= 0x9Fu;
                } while (v20 != 0x807D);
                if ((signed int)(unsigned __int8)*pTextIter >= 128)
                {
                    v56 = (unsigned __int8)pTextIter[1] | ((unsigned __int8)*pTextIter << 8);
                }
                else
                {
                    v21 = (unsigned __int8)*pTextIter;
                    BYTE1(v21) |= 0x80u;
                    v56 = v21;
                }
                if (v56 == 0x8023)
                {
                    if ((signed int)(unsigned __int8)*pTextIter >= 128)
                    {
                        v23 = (unsigned __int8)pTextIter[1] | ((unsigned __int8)*pTextIter << 8);
                        pTextIter += 2;
                    }
                    else
                    {
                        v22 = (unsigned __int8)*pTextIter++;
                    }
                }
                dword_732E3C = 0;
                dword_732E44 = 0;
                v49 = 0;
            LABEL_160:
                if ((signed int)(unsigned __int8)*pTextIter >= 128)
                {
                    v58 = (unsigned __int8)pTextIter[1] | ((unsigned __int8)*pTextIter << 8);
                }
                else
                {
                    v32 = (unsigned __int8)*pTextIter;
                    BYTE1(v32) |= 0x80u;
                    v58 = v32;
                }
                v33 = v58;
                BYTE1(v33) &= 0x9Fu;
                v34 = Font_45C16A(v33);
                if (v34 <= 0 || v34 + v49 + v67 + pFont->field_2 - 1 < v59)
                {
                LABEL_202:
                    v67 += v49;
                    v68 = 0;
                    goto LABEL_203;
                }
                if (!(pFont->field_6_flags & 2))
                {
                    if (text2Chars & 0x2000)
                    {
                        Font_45B90B(pBakedPixelData, v67, v66, v69, 0);
                        pTextIter = v60;
                        v49 = 0;
                    }
                    else if (v58 & 0x4000)
                    {
                        v35 = v58;
                        BYTE1(v35) &= 0x9Fu;
                        text2Charsb = v35;
                        if (dword_732E44 && (v35 == 0x9002 || v35 == 0x9004 || v35 == 0x807D))
                        {
                            goto LABEL_202;
                        }
                        if ((signed int)(unsigned __int8)pTextIter[2] >= 128)
                        {
                            v42 = (unsigned __int8)pTextIter[3] | ((unsigned __int8)pTextIter[2] << 8);
                        }
                        else
                        {
                            v36 = (unsigned __int8)pTextIter[2];
                            BYTE1(v36) |= 0x80u;
                            v42 = v36;
                        }
                        if (text2Charsb == 0x9003 || !(v42 & 0x4000) || (v37 = v42, BYTE1(v37) &= 0x9Fu, v37 == 33059))
                        {
                            if ((signed int)(unsigned __int8)*pTextIter >= 128)
                            {
                                text2Charsc = (unsigned __int8)pTextIter[1] | ((unsigned __int8)*pTextIter << 8);
                                pTextIter += 2;
                            }
                            else
                            {
                                v38 = (unsigned __int8)*pTextIter;
                                BYTE1(v38) |= 0x80u;
                                text2Charsc = v38;
                                ++pTextIter;
                            }
                            v39 = text2Charsc;
                            BYTE1(v39) &= 0x9Fu;
                            if (v39 >= 0x8100)
                            {
                                v40 = Font_map_char_45B80A(v39);
                                if (v40 <= 0)
                                {
                                    Font_45B90B(pBakedPixelData, v49 + v67, v66, v69, 0);
                                }
                                else
                                {
                                    Font_45B90B(
                                        pBakedPixelData,
                                        v49 + v67,
                                        v66,
                                        v69,
                                        (BYTE *)(36 * ((v40 & 4095) - 1) + dword_732E28[v40 / 4096]));
                                }
                                v49 += 12;
                            }
                            else
                            {
                                v49 += Font_add_char_to_atlas_45BD91(pBakedPixelData, v49 + v67, v66, v69, text2Charsc);
                            }
                        }
                    }
                }
                if (dword_732E48)
                {
                    Font_map_unknown_45C1DC(v49 + v67);
                }
                v68 = 1;
                if (v49 + v67 > pFont->field_7_out_max_width)
                {
                    pFont->field_7_out_max_width = v49 + v67;
                }
                v67 = kZero;
                v66 += pFont->field_3_line_spacing + 12;
                pFont->field_1E = v66 + 14;
                ++v50;
                if (pFont->field_6_flags & 1 || v66 + 11 >= v51 || v50 >= pFont->field_1_max_lines)
                {
                    if (dword_732E48)
                    {
                        dword_732E48 = 2;
                    }
                    if (*pTextIter)
                    {
                    LABEL_207:
                        if (v67 > pFont->field_7_out_max_width)
                        {
                            pFont->field_7_out_max_width = v67;
                        }
                        return;
                    }
                    return;
                }
            LABEL_203:
                if (v67 > pFont->field_7_out_max_width)
                {
                    pFont->field_7_out_max_width = v67;
                }
            }
        }
    }

}
MGS_FUNC_IMPLEX(0x45AB2D, Font_set_text_shift_jis_45AB2D, true);


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
