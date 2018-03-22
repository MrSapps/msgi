#include "stdafx.h"
#include "Font.hpp"
#include "Renderer.hpp"
#include "Actor_GameD.hpp"
#include "ResourceNameHash.hpp"
#include "LibGV.hpp"

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
MGS_FUNC_IMPLEX(0x520511, Font_build_replacement_table_520511, false);

MGS_ARY(1, 0x77C938, DWORD, 256, gTextCharTable_dword_77C938, {});

void CC Font_build_replacement_table_520511()
{
    //Font_build_replacement_table_520511_.Ptr()();
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

        pFont->field_8_pRect = pRect;
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
MGS_ASSERT_SIZEOF(FontTextLine, 280);

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
MGS_FUNC_IMPLEX(0x45C80A, Font_set_text_45C80A, false);

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
    pFont->field_1A = pFont->field_0_max_line_w * (pFont->field_2_char_spacing + 12) - pFont->field_2_char_spacing;
    pFont->field_18_wh = 4 * ((pFont->field_1A + 7) / 8);
    pFont->field_1C_wh = pFont->field_1_max_lines_y * (pFont->field_3_line_spacing + 12) + 2;
    pFont->field_C_rect.x2 = (2 * pFont->field_18_wh) / 4;
    pFont->field_C_rect.y2 = pFont->field_1C_wh;
    return pFont->field_1C_wh * pFont->field_18_wh + 32;
}
MGS_FUNC_IMPLEX(0x45AA45, Font_CalcSize_45AA45, FONT_IMPL);

MGS_VAR(1, 0x66C530, DWORD, dword_66C530, 1);
MGS_VAR(1, 0x732E1C, DWORD, gFontFirstLine_dword_732E1C, 0);

MGS_ARY(1, 0x732E28, DWORD*, 4, dword_732E28, {});

MGS_VAR(1, 0x732E3C, DWORD, dword_732E3C, 0);
MGS_VAR(1, 0x732E40, DWORD, dword_732E40, 0);
MGS_VAR(1, 0x732E44, DWORD, dword_732E44, 0);
MGS_VAR(1, 0x732E48, DWORD, dword_732E48, 0);


MGS_VAR(1, 0x732E14, DWORD, gFont_wxh_dword_732E14, 0);
MGS_VAR(1, 0x732E18, BYTE*, gFont_pixel_buffer_732E18, 0);

FontTextLineSource* CC Font_add_blank_line_record_4242CB(FontTextLineSource* pBuffer, BYTE xpos, BYTE ypos, BYTE width, BYTE height, __int16 unknown, DWORD* pRemainderSize)
{
    FontTextLineSource* pRet = pBuffer;

    if (!pBuffer || *pRemainderSize < 264u) // TODO: calculate structure size
    {
        return nullptr;
    }

    // Get to the end of the buffer
    while (pRet->field_0_text_length)
    {
        const DWORD itemSize = pRet->field_0_text_length + 7;
        *pRemainderSize -= itemSize;
        pRet = (FontTextLineSource *)((BYTE*)pRet + itemSize);
    }

    pRet->field_0_text_length = 0;
    pRet->field_3_width = width;
    pRet->field_4_height = height;
    pRet->field_7_text_buffer[0] = 0;
    pRet->field_1_x = xpos;
    pRet->field_2_y = ypos;
    pRet->field_5_unknown = unknown;
    return pRet;
}
MGS_FUNC_IMPLEX(0x4242CB, Font_add_blank_line_record_4242CB, FONT_IMPL);

FontTextLineSource* CC Font_add_char_to_line_record_42431F(FontTextLineSource* pTextLine, char xpos, int ypos, char width, int height, __int16 unknown, unsigned __int8 charToRender, DWORD* pRemainderSize)
{
    if (pTextLine)
    {
        while (*pRemainderSize >= 264u) // TODO: calculate structure size
        {
            // in printable range?
            if (charToRender > 127u || charToRender < 12u)
            {
                return pTextLine;
            }

            if (!pTextLine->field_2_y)
            {
                pTextLine->field_2_y = ypos;
            }

            // Add to current line
            if (pTextLine->field_2_y == ypos && pTextLine->field_4_height == height)
            {
                if (pTextLine->field_0_text_length != 255)
                {
                    pTextLine->field_7_text_buffer[pTextLine->field_0_text_length++] = charToRender;
                    pTextLine->field_7_text_buffer[pTextLine->field_0_text_length] = 0;
                    pTextLine->field_3_width += width;
                }
                return pTextLine;
            }

            // Create a new line
            pTextLine = Font_add_blank_line_record_4242CB(pTextLine, xpos, ypos, width, height, unknown, pRemainderSize);
            if (!pTextLine)
            {
                return 0;
            }
        }
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x42431F, Font_add_char_to_line_record_42431F, FONT_IMPL);

MGS_VAR(1, 0x732E38, void*, gRubi_res_dword_732E38, nullptr);
MGS_VAR(1, 0x732E20, DWORD*, gFontRes_pTable_732E20, nullptr);
MGS_VAR(1, 0x732E24, DWORD*, gpFontResImageData_dword_732E24, nullptr);

struct FontRes
{
    DWORD field_0_mTableSize;
    DWORD field_4_mUnknown;
    DWORD field_8_mEntries[0];
};

MGS_VAR(1, 0x72AE10, FontRes*, gFont_res_72AE10, nullptr);

void CC Font_load_fonts_45A5AF()
{
    gFont_res_72AE10 = (FontRes*)LibGV_FindFile_40A603(HashFileName_40A5A2("font", 'r')); // .res
    if (gFont_res_72AE10)
    {
        gRubi_res_dword_732E38 = LibGV_FindFile_40A603(HashFileName_40A5A2("rubi", 'r')); // .res
        
        gFont_res_72AE10->field_0_mTableSize = _byteswap_ulong(gFont_res_72AE10->field_0_mTableSize);
        gFont_res_72AE10->field_4_mUnknown = _byteswap_ulong(gFont_res_72AE10->field_4_mUnknown);

        gFontRes_pTable_732E20 = gFont_res_72AE10->field_8_mEntries;
        gpFontResImageData_dword_732E24 = (DWORD *)((char *)gFont_res_72AE10 + gFont_res_72AE10->field_0_mTableSize);
        
        
        const int tableCount = (gFont_res_72AE10->field_0_mTableSize / 4) - (sizeof(DWORD) * 2);

        //DWORD* pEndOfFontData = gFont_res_72AE10->field_8_mEntries + tableCount;
        // Note: Inlined in real game
        sub_45A6F6(0, (DWORD *)((char *)gFont_res_72AE10 + gFont_res_72AE10->field_4_mUnknown));

        for (int i = 0; i <= tableCount; i++)
        {
            gFont_res_72AE10->field_8_mEntries[i] = _byteswap_ulong(gFont_res_72AE10->field_8_mEntries[i]);
        }
    }
}
MGS_FUNC_IMPLEX(0x45A5AF, Font_load_fonts_45A5AF, FONT_IMPL);

void CC sub_45A6F6(int idx, void* value)
{
    assert(idx >= 0 && idx <= 4);
    dword_732E28[idx] = reinterpret_cast<DWORD*>(value);
}
MGS_FUNC_IMPLEX(0x45A6F6, sub_45A6F6, FONT_IMPL);

int CC Font_look_up_char_45C138(signed int charToLookUp)
{
    signed int ret = charToLookUp;
    if (ret > 0)
    {
        if (ret > 128) // Over 128 is last printable ascii char
        {
            ret -= 34; // 34 "extra" unprintable chars?
        }
        ret -= 32; // 32 = space, first printable acsii char
    }

    // 13/line feed will read negatively out of bounds.. seems to be OG bug?
    //assert(ret >= 0 && ret <= (gFont_res_72AE10->field_0_mFileSize / 4) - (sizeof(DWORD) * 2));
    return gFontRes_pTable_732E20[ret];
}
MGS_FUNC_IMPLEX(0x45C138, Font_look_up_char_45C138, FONT_IMPL);

int CC Font_Get_rubi_code_index_45C3C2(signed int toMap)
{
    switch (toMap)
    {
    case 0x8201: return 1;   case 0x8202: return 2;   case 0x8203: return 3;
    case 0x8204: return 4;   case 0x8205: return 5;   case 0x8206: return 6;
    case 0x8207: return 7;   case 0x8208: return 8;   case 0x8209: return 9;
    case 0x820A: return 10;  case 0x820B: return 11;  case 0x820C: return 12;
    case 0x820D: return 13;  case 0x820E: return 14;  case 0x820F: return 15;
    case 0x8210: return 16;  case 0x8211: return 17;  case 0x8212: return 18;
    case 0x8213: return 19;  case 0x8214: return 20;  case 0x8215: return 21;
    case 0x8216: return 22;  case 0x8217: return 23;  case 0x8218: return 24;
    case 0x8219: return 25;  case 0x821A: return 26;  case 0x821B: return 27;
    case 0x821C: return 28;  case 0x821D: return 29;  case 0x821E: return 30;
    case 0x821F: return 31;  case 0x8220: return 32;  case 0x8221: return 33;
    case 0x8222: return 34;  case 0x8223: return 35;  case 0x8224: return 36;
    case 0x8225: return 37;  case 0x8226: return 38;  case 0x8227: return 39;
    case 0x8228: return 40;  case 0x8229: return 41;  case 0x822A: return 42;
    case 0x822B: return 43;  case 0x822C: return 44;  case 0x822D: return 45;
    case 0x822E: return 46;  case 0x822F: return 47;  case 0x8230: return 48;
    case 0x8231: return 49;  case 0x8232: return 50;  case 0x8233: return 51;
    case 0x8234: return 52;  case 0x8235: return 53;  case 0x8236: return 54;
    case 0x8237: return 55;  case 0x8238: return 56;  case 0x8239: return 57;
    case 0x823A: return 58;  case 0x823B: return 59;  case 0x823C: return 60;
    case 0x823D: return 61;  case 0x823E: return 62;  case 0x823F: return 63;
    case 0x8240: return 64;  case 0x8241: return 65;  case 0x8242: return 66;
    case 0x8243: return 67;  case 0x8244: return 68;  case 0x8245: return 69;
    case 0x8246: return 70;  case 0x8247: return 71;  case 0x8248: return 72;
    case 0x8249: return 73;  case 0x824A: return 74;  case 0x824B: return 75;
    case 0x824C: return 76;  case 0x824D: return 77;  case 0x824F: return 78;
    case 0x8252: return 79;  case 0x8253: return 80;

    // Jump/skip in range

    case 0x8030: return 81;  case 0x8031: return 82;  case 0x8032: return 83;
    case 0x8033: return 84;  case 0x8034: return 85;  case 0x8035: return 86;
    case 0x8036: return 87;  case 0x8037: return 88;  case 0x8038: return 89;
    case 0x8039: return 90;
    
    // Jump/skip in range

    case 0x8041: return 91;  case 0x8042: return 92;  case 0x8043: return 93;
    case 0x8044: return 94;  case 0x8045: return 95;  case 0x8046: return 96;
    case 0x8047: return 97;  case 0x8048: return 98;  case 0x8049: return 99;
    case 0x804A: return 100; case 0x804B: return 101; case 0x804C: return 102;
    case 0x804D: return 103; case 0x804E: return 104; case 0x804F: return 105;
    case 0x8050: return 106; case 0x8051: return 107; case 0x8052: return 108;
    case 0x8053: return 109; case 0x8054: return 110; case 0x8055: return 111;
    case 0x8056: return 112; case 0x8057: return 113; case 0x8058: return 114;
    case 0x8059: return 115; case 0x805A: return 116;

    case 0x807F: return 118; case 0x802E: return 118;

    case 0x802D: return 117; case 0x9006: return 117;

    case 0x8020: return 0;   case 0x9001: return 0;
    }

    return -1;
}
MGS_FUNC_IMPLEX(0x45C3C2, Font_Get_rubi_code_index_45C3C2, FONT_IMPL);

int __cdecl Font_map_char_45B80A(signed int input)
{
    //return Font_Get_rubi_code_index_45C3C2(input);
    static int t = 1;
    static int d = 0;
    d++;
    if (d > 50)
    {
        d = 0;
        t++;
    }
    return t;
}
MGS_FUNC_IMPLEX(0x45B80A, Font_map_char_45B80A, false);

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

void __cdecl Font_rubi_add_45C1E9(BYTE* pTexturePixels, signed int a2, int a3, int a4, int a5)
{

}
MGS_FUNC_IMPLEX(0x45C1E9, Font_rubi_add_45C1E9, false); // TODO

unsigned int __cdecl Font_45C16A(signed int a1)
{
    return 0;
}
MGS_FUNC_IMPLEX(0x45C16A, Font_45C16A, false); // TODO


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

static WORD NextVarChar(BYTE*& pTextIter, bool incrementDouble = true, bool incrementSingle = true)
{
    WORD text2Chars = 0;
    if (*pTextIter >= 128)
    {
        text2Chars = pTextIter[1] | (*pTextIter << 8);
        if (incrementDouble)
        {
            pTextIter += 2;
        }
    }
    else
    {
        text2Chars = pTextIter[0] | (0x80 << 8);
        if (incrementSingle)
        {
            pTextIter++;
        }
    }
    return text2Chars;
}

int HandleCharAdd(BYTE*& pTextIter, int textCurrentX, Font* pFont, int charToAdd, int xpos, int ypos, BYTE* pBakedPixelData, int v69)
{
    if (charToAdd >= 0x8100)
    {
        const int v31 = Font_map_char_45B80A(charToAdd);
        if (v31 <= 0)
        {
            Font_45B90B(pBakedPixelData, xpos, ypos, v69, 0);
        }
        else
        {
            Font_45B90B(pBakedPixelData, xpos, ypos, v69, (BYTE *)(36 * ((v31 & 0xFFF) - 1) + dword_732E28[v31 / 4096]));
        }
        textCurrentX = pFont->field_2_char_spacing + 12;
    }
    else
    {
        textCurrentX = pFont->field_2_char_spacing + Font_add_char_to_atlas_45BD91(pBakedPixelData, xpos, ypos, v69, charToAdd);
        if (charToAdd == 0x8021 || charToAdd == 0x803F)
        {
            const int nextChar3 = NextVarChar(pTextIter, false, false);
            const signed int nextChar3Masked = nextChar3;
            BYTE1(nextChar3Masked) &= 0x9Fu;
            if (nextChar3Masked != 0x8021 && nextChar3Masked != 0x803F)
            {
                if (nextChar3Masked == 0x8029 || nextChar3Masked == 0x8028 || nextChar3Masked <= 36878 && nextChar3Masked >= 36873 || nextChar3Masked == 36885)
                {
                    textCurrentX += 2;
                }
                else
                {
                    textCurrentX += 8;
                }
            }
        }
    }
    return textCurrentX;
}

void HandleCharSkip(BYTE*& pTextIter, int xpos, int ypos, int field_1a)
{
    if (dword_66C530)
    {
        dword_732E48 = 1;
        sub_45C1BF(field_1a, xpos, ypos);
        int nextChar4 = NextVarChar(pTextIter, false, false);
        int nextChar4Masked = nextChar4;
        BYTE1(nextChar4Masked) &= 0x9Fu;
        if (nextChar4Masked == 0x8021)
        {
            pTextIter += 2;
        }
    }
    else
    {
        int nextChar5 = NextVarChar(pTextIter, false, false);
        int nextChar5Masked = nextChar5;
        BYTE1(nextChar5Masked) &= 0x9Fu;
        int nextChar5MaskedCopy = nextChar5Masked;
        if (nextChar5Masked == 0x8021)
        {
            while (nextChar5MaskedCopy != 0x802C && nextChar5MaskedCopy != 0x9002)
            {
                int nextChar6 = NextVarChar(pTextIter, true, true);
                int nextChar6Masked = nextChar6;
                BYTE1(nextChar6Masked) &= 0x9Fu;
                nextChar5MaskedCopy = nextChar6Masked;
            }
        }
    }
}
void __cdecl Font_set_text_shift_jis_45AB2D(Font *pFont, int kZero, int field_3, BYTE *pText, int field_5);
MGS_FUNC_IMPLEX(0x45AB2D, Font_set_text_shift_jis_45AB2D, false); // TODO

void __cdecl Font_set_text_shift_jis_45AB2D(Font *pFont, int kZero, int field_3, BYTE *pText, int field_5)
{

  //  return Font_set_text_shift_jis_45AB2D_.Ptr()(pFont, kZero, field_3, pText, field_5);

    BYTE *pUpdatedText; // eax
    signed int nextChar2Masked; // eax
    int nextChar8Masked; // eax
    int nextChar1Masked2; // eax
    signed int nextChar10Masked; // eax
    signed int v34; // eax
    int nextChar10Masked2; // eax
    int nextChar11Masked; // eax
    int v40; // eax
    int maskedGameStates; // [esp+4h] [ebp-68h]
    int v43; // [esp+20h] [ebp-4Ch]
    int nextChar2; // [esp+24h] [ebp-48h]
    int textCurrentX = 0; // [esp+28h] [ebp-44h]
    signed int numLines; // [esp+2Ch] [ebp-40h]
    int textYMax; // [esp+30h] [ebp-3Ch]
    BYTE *pTextIter; // [esp+34h] [ebp-38h]
    signed int charToAdd; // [esp+38h] [ebp-34h]
    int nextChar8; // [esp+38h] [ebp-34h]
    int nextChar9; // [esp+38h] [ebp-34h]
    int nextChar10; // [esp+38h] [ebp-34h]
    int field_1a; // [esp+3Ch] [ebp-30h]
    BYTE *pOldTextIter; // [esp+40h] [ebp-2Ch]
    int nextChar1; // [esp+48h] [ebp-24h]
    int nextChar7; // [esp+48h] [ebp-24h]
    int nextChar10Masked2Copy; // [esp+48h] [ebp-24h]
    int nextChar12; // [esp+48h] [ebp-24h]
    signed int v65; // [esp+4Ch] [ebp-20h]
    int ypos = 0; // [esp+50h] [ebp-1Ch]
    int xpos = 0; // [esp+54h] [ebp-18h]
    signed int v68; // [esp+58h] [ebp-14h]
    int v69; // [esp+64h] [ebp-8h]

    
    //gUseTrueType_dword_6FC7AC = 0;
    BYTE test[] = { 0x90, 0x02, 0x90, 0x12, 0x82, 0x3A, 0x82, 0x3b, 0x82, 0x3c, 0x82, 0x3d, 0x80, 0x7d, 0x0,  0x0 };
    pText = test;

    // 0x9010 = %
    // 0x9011 = &
    // 0x9012 = [
    // 0x9013 = ]
    // 0x9040 = jp char
    Font_rubi_add_45C1E9(pFont->field_14_pPixelData, xpos, ypos, pFont->field_18_wh, (int)test);
    pFont->field_7_out_max_width = 200;

    return;

    if (!gFont_res_72AE10 || !pFont)
    {
        return;
    }

    if (gUseTrueType_dword_6FC7AC)
    {
        gFont_wxh_dword_732E14 = pFont->field_1C_wh * pFont->field_18_wh;
        gFont_pixel_buffer_732E18 = pFont->field_14_pPixelData;
        gFontFirstLine_dword_732E1C = 1;
    }

    pUpdatedText = Font_set_text_520419(pText);
    BYTE* pBakedPixelData = pFont->field_14_pPixelData;
    dword_732E3C = 0;
    field_1a = pFont->field_1A;
    textYMax = pFont->field_1C_wh;
    numLines = 0;
    if (!(pFont->field_6_flags & 2))
    {
        field_1a -= 12;
    }
    v69 = pFont->field_18_wh;
    dword_732E40 = field_5;
    v68 = 0;
    xpos = kZero;
    ypos = field_3;
    pFont->field_7_out_max_width = 0;
    pFont->field_1E_line_ypos = pFont->field_3_line_spacing + 14;
    pFont->field_6_flags &= 0xEFu;
    v65 = 0;
    pTextIter = pUpdatedText;
    while (1)
    {
        if (!pTextIter || !*pTextIter)
        {
            return;
        }

        nextChar1 = NextVarChar(pTextIter, false, false);
        const int nextChar1Masked = nextChar1;
        BYTE1(nextChar1Masked) &= 0x9Fu;
        charToAdd = nextChar1Masked;
        if (nextChar1 < 0x8020)
        {
            nextChar1 = NextVarChar(pTextIter);
            goto LABEL_112;
        }

        if (nextChar1Masked == 0x8023)
        {
            NextVarChar(pTextIter, true, true); // Skip char
            nextChar2 = NextVarChar(pTextIter, true, true);
            nextChar2Masked = nextChar2;
            BYTE1(nextChar2Masked) &= 0x9Fu;
            if (nextChar2Masked > 0x8054)
            {
                if (nextChar2Masked != 0x8057)
                {
                    if (nextChar2Masked != 0x807B)
                    {
                        switch (nextChar2Masked)
                        {
                        case 0x9006:
                            textCurrentX = Font_add_char_to_atlas_45BD91(pBakedPixelData, xpos, ypos, v69, 0);
                            goto LABEL_160;
                        case 0x901D:
                            if ((gGameStates_78E7E0.gFlags_dword_78E7E4 & 7) == 2)
                            {
                                charToAdd = 0x9024;
                            }
                            else
                            {
                                charToAdd = nextChar2Masked;
                            }
                            textCurrentX = HandleCharAdd(pTextIter, textCurrentX, pFont, charToAdd, xpos, ypos, pBakedPixelData, v69);
                            v65 = 1;
                            goto LABEL_160;
                        case 0x901E:
                        case 0x9024:
                            maskedGameStates = gGameStates_78E7E0.gFlags_dword_78E7E4 & 7;
                            if (gGameStates_78E7E0.gFlags_dword_78E7E4 & 7)
                            {
                                if (maskedGameStates == 1)
                                {
                                    charToAdd = 0x9018;
                                }
                                else if (maskedGameStates == 2)
                                {
                                    charToAdd = 0x901D;
                                }
                            }
                            else
                            {
                                charToAdd = nextChar2Masked;
                            }
                            textCurrentX = HandleCharAdd(pTextIter, textCurrentX, pFont, charToAdd, xpos, ypos, pBakedPixelData, v69);
                            v65 = 1;
                            goto LABEL_160;
                        }
                        pTextIter -= 4;
                        goto LABEL_112;
                    }
                    dword_732E44 = 1;
                    HandleCharSkip(pTextIter, xpos, ypos, field_1a);
                    goto LABEL_112;
                }
                if (xpos > pFont->field_7_out_max_width)
                {
                    pFont->field_7_out_max_width = xpos;
                }
                if (v65)
                {
                    if (xpos > pFont->field_7_out_max_width)
                    {
                        pFont->field_7_out_max_width = xpos;
                    }
                    return;
                }
                textCurrentX = 0;
                goto LABEL_160;
            }

            if (nextChar2Masked != 0x8054)
            {
                switch (nextChar2Masked)
                {
                case 0x802D:
                    textCurrentX = Font_add_char_to_atlas_45BD91(pBakedPixelData, xpos, ypos, v69, 0);
                    goto LABEL_160;
                case 0x8031:
                    charToAdd = 0x8040;
                    textCurrentX = HandleCharAdd(pTextIter, textCurrentX, pFont, charToAdd, xpos, ypos, pBakedPixelData, v69);
                    v65 = 1;
                    goto LABEL_160;
                case 0x8032:
                    charToAdd = 32894;
                    textCurrentX = HandleCharAdd(pTextIter, textCurrentX, pFont, charToAdd, xpos, ypos, pBakedPixelData, v69);
                    v65 = 1;
                    goto LABEL_160;
                case 0x804E:
                    nextChar1 = 0x800A;
                    goto LABEL_112;
                }
                if (nextChar2Masked != 0x8052)
                {
                    if (nextChar2Masked == 0x8053)
                    {
                        nextChar1 = 0x8009;
                        goto LABEL_112;
                    }
                    pTextIter -= 4;
                    goto LABEL_112;
                }
                pTextIter += 2;
                dword_732E44 = 1;
                HandleCharSkip(pTextIter, xpos, ypos, field_1a);
                goto LABEL_112;
            }
            pTextIter += 2;
            if ((signed int)pFont->field_3_line_spacing > 2)
            {
                dword_732E3C = 1;
            }
            dword_732E44 = 1;
        LABEL_112:
            if (nextChar1 >= 0x8020)
            {
                pOldTextIter = pTextIter;
                nextChar1 = NextVarChar(pTextIter, true, true);
                nextChar1Masked2 = nextChar1;
                BYTE1(nextChar1Masked2) &= 0x9Fu;
                charToAdd = nextChar1Masked2;

                textCurrentX = HandleCharAdd(pTextIter, textCurrentX, pFont, charToAdd, xpos, ypos, pBakedPixelData, v69);
                v65 = 1;
                goto LABEL_160;
            }
            switch (nextChar1)
            {
            case 0x8009:
                v43 = (unsigned __int8)pFont->field_4 * (pFont->field_2_char_spacing + 12);
                if (v43 > 0)
                {
                    xpos = v43 * (xpos / v43 + 1);
                }
                break;
            case 0x800A:
                if (v68)
                {
                    v68 = 0;
                }
                else
                {
                    if (xpos > pFont->field_7_out_max_width)
                    {
                        pFont->field_7_out_max_width = xpos;
                    }
                    xpos = kZero;
                    ypos += pFont->field_3_line_spacing + 12;
                    pFont->field_1E_line_ypos = ypos + 14;
                    ++numLines;
                    if (ypos + 11 >= textYMax || pFont->field_6_flags & 1 || numLines >= pFont->field_1_max_lines_y)
                    {
                        if (xpos > pFont->field_7_out_max_width)
                        {
                            pFont->field_7_out_max_width = xpos;
                        }
                        return;
                    }
                }
                break;
            case 0x800C:
                nextChar7 = NextVarChar(pTextIter, true, true);
                dword_732E40 = nextChar7 - 0x8030;
                break;
            }
        }
        else
        {
            if (!dword_732E44 || nextChar1Masked != 0x9002 && nextChar1Masked != 0x9004 && nextChar1Masked != 0x807D)
            {
                if (nextChar1Masked == 0x807C)
                {
                    NextVarChar(pTextIter, true, true); // Skip char
                    nextChar1 = 0x800A;
                }
                goto LABEL_112;
            }
            if (dword_732E48 == 1 && (nextChar1Masked == 0x9002 || nextChar1Masked == 0x9004))
            {
                Font_rubi_add_45C1E9(pBakedPixelData, xpos, ypos, v69, (int)(pTextIter + 2));
            }
            dword_732E48 = 0;

            do
            {
                nextChar8 = NextVarChar(pTextIter, true, true);
                nextChar8Masked = nextChar8;
                BYTE1(nextChar8Masked) &= 0x9Fu;
            } while (nextChar8Masked != 0x807D);

            nextChar9 = NextVarChar(pTextIter, false, false);
            if (nextChar9 == 0x8023)
            {
                NextVarChar(pTextIter, true, true); // Skip char
            }
            dword_732E3C = 0;
            dword_732E44 = 0;
            textCurrentX = 0;


        LABEL_160:
            nextChar10 = NextVarChar(pTextIter, false, false);
            nextChar10Masked = nextChar10;
            BYTE1(nextChar10Masked) &= 0x9Fu;
            v34 = Font_45C16A(nextChar10Masked);
            if (v34 <= 0 || v34 + textCurrentX + xpos + pFont->field_2_char_spacing - 1 < field_1a)
            {
                xpos += textCurrentX;
                v68 = 0;
                if (xpos > pFont->field_7_out_max_width)
                {
                    pFont->field_7_out_max_width = xpos;
                }
                continue;
            }
            if (!(pFont->field_6_flags & 2))
            {
                if (nextChar1 & 0x2000)
                {
                    Font_45B90B(pBakedPixelData, xpos, ypos, v69, 0);
                    pTextIter = pOldTextIter;
                    textCurrentX = 0;
                }
                else if (nextChar10 & 0x4000)
                {
                    nextChar10Masked2 = nextChar10;
                    BYTE1(nextChar10Masked2) &= 0x9Fu;
                    nextChar10Masked2Copy = nextChar10Masked2;
                    if (dword_732E44 && (nextChar10Masked2 == 0x9002 || nextChar10Masked2 == 0x9004 || nextChar10Masked2 == 0x807D))
                    {
                        xpos += textCurrentX;
                        v68 = 0;
                        if (xpos > pFont->field_7_out_max_width)
                        {
                            pFont->field_7_out_max_width = xpos;
                        }
                        continue;
                    }

                    int nextChar11 = NextVarChar(pTextIter, false, false);

                    if (nextChar10Masked2Copy == 0x9003 || !(nextChar11 & 0x4000) || (nextChar11Masked = nextChar11, BYTE1(nextChar11Masked) &= 0x9Fu, nextChar11Masked == 0x8123))
                    {
                        nextChar12 = NextVarChar(pTextIter, true, true);
                        const signed int nextChar12Masked = nextChar12;
                        BYTE1(nextChar12Masked) &= 0x9Fu;
                        if (nextChar12Masked >= 0x8100)
                        {
                            v40 = Font_map_char_45B80A(nextChar12Masked);
                            if (v40 <= 0)
                            {
                                Font_45B90B(pBakedPixelData, textCurrentX + xpos, ypos, v69, 0);
                            }
                            else
                            {
                                Font_45B90B(
                                    pBakedPixelData,
                                    textCurrentX + xpos,
                                    ypos,
                                    v69,
                                    (BYTE *)(36 * ((v40 & 4095) - 1) + dword_732E28[v40 / 4096]));
                            }
                            textCurrentX += 12;
                        }
                        else
                        {
                            textCurrentX += Font_add_char_to_atlas_45BD91(pBakedPixelData, textCurrentX + xpos, ypos, v69, nextChar12);
                        }
                    }
                }
            }
            if (dword_732E48)
            {
                Font_map_unknown_45C1DC(textCurrentX + xpos);
            }
            v68 = 1;
            if (textCurrentX + xpos > pFont->field_7_out_max_width)
            {
                pFont->field_7_out_max_width = textCurrentX + xpos;
            }

            xpos = kZero;
            ypos += pFont->field_3_line_spacing + 12;
            pFont->field_1E_line_ypos = ypos + 14;

            ++numLines;

            if (pFont->field_6_flags & 1 || ypos + 11 >= textYMax || numLines >= pFont->field_1_max_lines_y)
            {
                if (dword_732E48)
                {
                    dword_732E48 = 2;
                }
                if (*pTextIter)
                {
                    if (xpos > pFont->field_7_out_max_width)
                    {
                        pFont->field_7_out_max_width = xpos;
                    }
                    return;
                }
                return;
            }

            if (xpos > pFont->field_7_out_max_width)
            {
                pFont->field_7_out_max_width = xpos;
            }
        }
    } // Loop end
}


MGS_FUNC_NOT_IMPL(0x520458, void __cdecl(char* pText), Font_replace_psx_with_pc_strings_520458);

void DoFontTests()
{
    Font_build_replacement_table_520511();
/*
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
*/
}
