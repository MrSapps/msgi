#include "stdafx.h"
#include "pcx.hpp"
#include "MgsFunction.hpp"
#include "ResourceNameHash.hpp"
#include "Psx.hpp"
#include "Fs.hpp"
#include "System.hpp"
#include "Actor_GameD.hpp"
#include "LibDG.hpp"
#include "WinMain.hpp"
#include "Renderer.hpp"

#define PCX_IMPL true

MGS_VAR_EXTERN(DWORD, gStageIs_s11e_6FC778);

const BYTE* CC GV_pcx_file_RLE_decompress_4bit_402F30(const BYTE* pInput, BYTE* pOutput, int count);

void CC GV_pcx_file_RLE_decompress_8bit_402FB4(const BYTE *pIn, BYTE *pOut, int bytesPerScanLine, signed int w, int h)
{
    static BYTE byte_6BEE84[132] = {}; // TODO: Unclear why this is static in the original code

    for (int i=0; i<h; i++)
    {
        pIn = GV_pcx_file_RLE_decompress_4bit_402F30(pIn, byte_6BEE84, 4 * bytesPerScanLine );

        const BYTE* rPtr = &byte_6BEE84[bytesPerScanLine * 0];
        const BYTE* gPtr = &byte_6BEE84[bytesPerScanLine * 1];
        const BYTE* bPtr = &byte_6BEE84[bytesPerScanLine * 2];
        const BYTE* aPtr = &byte_6BEE84[bytesPerScanLine * 3];

        for (int j = w; j > 0; j -= 4)
        {
            const int rBits = *rPtr++;
            const int gBits = *gPtr++;
            const int bBits = *bPtr++;
            const int aBits = *aPtr++;
            
            int mask = 128; // 8th bit
            const int stopMask = (j < 4) ? 8 : 0;
            do
            {
                BYTE resultingPixel = 0;
                if (rBits & mask)
                {
                    resultingPixel = 0x01;
                }

                if (gBits & mask)
                {
                    resultingPixel |= 0x02u;
                }

                if (bBits & mask)
                {
                    resultingPixel |= 0x04u;
                }
                if (aBits & mask)
                {
                    resultingPixel |= 0x08u;
                }
                
                mask = mask >> 1;

                if (rBits & mask)
                {
                    resultingPixel |= 0x10u;
                }
                
                if (gBits & mask)
                {
                    resultingPixel |= 0x20u;
                }

                if (bBits & mask)
                {
                    resultingPixel |= 0x40u;
                }

                if (aBits & mask)
                {
                    resultingPixel |= 0x80u;
                }

                *pOut++ = resultingPixel;

                mask = mask >> 1;

            } while (mask != stopMask);
        }
    }
}
MGS_FUNC_IMPLEX(0x402FB4, GV_pcx_file_RLE_decompress_8bit_402FB4, PCX_IMPL);

struct HiTexRecord
{
    DWORD field_0_id;
    DWORD field_4_is_in_use;
    char* field_8_name;
};
MGS_ASSERT_SIZEOF(HiTexRecord, 0xC);

MGS_VAR(1, 0x734A30, DWORD, gNum_HiTexs_dword_734A30, 0);
MGS_ARY(1, 0x9956A0, HiTexRecord, 8192, gHiText_recs_9956A0, {});
MGS_VAR(1, 0x650D1A, WORD, g_Render_sub_41C640_ret_650D1A, 0);

void CC File_HITEXT_INIT_51D2ED()
{
    // Free existing HiTex entries
    for (DWORD i = 0; i < gNum_HiTexs_dword_734A30; i++)
    {
        free(gHiText_recs_9956A0[i].field_8_name);
        gHiText_recs_9956A0[i].field_0_id = 0;
        const bool dword_6893D4_is_zero = dword_6893D4 == 0;
        if (!dword_6893D4_is_zero)
        {
            gHiText_recs_9956A0[i].field_4_is_in_use = 0;
        }
    }

    dword_6893D4 = 0;
    gNum_HiTexs_dword_734A30 = 0;

    FILE* hFile = fopen("hitex.dir", "rt");
    if (!hFile)
    {
        return;
    }

    char fileBuffer[256];
    for (;;)
    {
        memset(fileBuffer, 0, sizeof(fileBuffer));
        fgets(fileBuffer, 256, hFile);

        // Remove any trailing line feed
        char* newLinePos = strchr(fileBuffer, '\r');
        if (newLinePos)
        {
            *newLinePos = 0;
        }

        // Remove any trailing new line
        newLinePos = strchr(fileBuffer, '\n');
        if (newLinePos)
        {
            *newLinePos = 0;
        }

        // Bail if no more data
        if (!*fileBuffer)
        {
            break;
        }

        // Replace \\ with /
        char* tmp = fileBuffer;
        while (*tmp)
        {
            if (*tmp == '\\')
            {
                *tmp = '/';
            }
            tmp++;
        }

        gHiText_recs_9956A0[gNum_HiTexs_dword_734A30].field_8_name = _strdup(fileBuffer);
        const DWORD id = HiTexHash_51D47A(fileBuffer);
        const DWORD idx = gNum_HiTexs_dword_734A30;
        gHiText_recs_9956A0[idx].field_0_id = id;
        printf("HITEX_INIT: Id: %-5d Name: %s\n", id, gHiText_recs_9956A0[idx].field_8_name);
        gNum_HiTexs_dword_734A30++;
    }

    fclose(hFile);
}
MGS_FUNC_IMPLEX(0x0051D2ED, File_HITEXT_INIT_51D2ED, PCX_IMPL);

const char* CC HITEX_NAME_51D4BC(DWORD id)
{
    for (DWORD i = 0; i < gNum_HiTexs_dword_734A30; i++)
    {
        if (gHiText_recs_9956A0[i].field_4_is_in_use && gHiText_recs_9956A0[i].field_0_id == id)
        {
            printf("HITEX_NAME: Id: %-5d Name: %s\n", id, gHiText_recs_9956A0[i].field_8_name);
            return gHiText_recs_9956A0[i].field_8_name;
        }
    }
    return nullptr;
}
MGS_FUNC_IMPLEX(0x0051D4BC, HITEX_NAME_51D4BC, PCX_IMPL);

void CC Res_loader_EnableHiTex_51D1DB(const char* pcxName)
{
    for (DWORD i = 0; i < gNum_HiTexs_dword_734A30; i++)
    {
        const char* tgaName = strchr(gHiText_recs_9956A0[i].field_8_name, '/');
        if (tgaName)
        {
            // Consume the /
            tgaName++;
        }
        if (*tgaName)
        {
            if (strlen(tgaName) == strlen(pcxName))
            {
                if (strncmp(tgaName, pcxName, strlen(tgaName) - 3) == 0)
                {
                    gHiText_recs_9956A0[i].field_4_is_in_use = 1;
                    return;
                }
            }
        }
    }
}
MGS_FUNC_IMPLEX(0x51D1DB, Res_loader_EnableHiTex_51D1DB, PCX_IMPL);

const BYTE* CC GV_pcx_file_RLE_decompress_4bit_402F30(const BYTE* pInput, BYTE* pOutput, int count)
{
    do
    {
        BYTE inByte = *pInput++;
        if (inByte > 192)
        {
            int runLength = inByte - 192;
            BYTE runValue = *pInput++;
            count -= runLength;
            while (--runLength >= 0)
            {
                *pOutput++ = runValue;
            }
        }
        else
        {
            --count;
            *pOutput++ = inByte;
        }
    } while (count > 0);
    return pInput;
}
MGS_FUNC_IMPLEX(0x402F30, GV_pcx_file_RLE_decompress_4bit_402F30, PCX_IMPL);

void CC GV_pcx_file_pallete_convert_4031B9(const BYTE* pPal, WORD* pOutPal, int colourCount)
{
    for (;;)
    {
        colourCount--;
        if (colourCount < 0)
        {
            break;
        }

        BYTE r = pPal[0];
        BYTE g = pPal[1];
        BYTE b = pPal[2];
        WORD pixel16 = ((b | g | r) & 7) != 0 ? 0x20 : 0; // Top 3 bits = transparency
        if (r || g || b)
        {
            pixel16 =  ((signed int)(unsigned __int8)r >> 3)
               | 32 * (((signed int)(unsigned __int8)g >> 3)
               | 32 * (((signed int)(unsigned __int8)b >> 3) | pixel16));
        }
        *pOutPal = pixel16;
        ++pOutPal;
        pPal += 3;
    }
}
MGS_FUNC_IMPLEX(0x4031B9, GV_pcx_file_pallete_convert_4031B9, PCX_IMPL);

struct pcx_header
{
    char field_0_magic;
    char field_1_version;
    char field_2_bRLE;
    char field_3_bitsPerPlane;
    WORD field_4_Xmin;
    WORD field_6_Ymin;
    WORD field_8_Xmax;
    WORD field_A_Ymax;
    WORD field_C_VertDPI;
    WORD field_E_HorzDPI;
    BYTE field_10_palette[48];
    char field_40_reserved;
    char field_41_colorPlanes;
    WORD field_42_bytesPerPlaneLine;
    WORD field_44_paltype;
    WORD field_46_hScrSize;
    WORD field_48_vScrSize;
    WORD field_4A_mgs_magic_3930;
    WORD field_4C_bpp_mgs;
    WORD field_4E_texX;
    WORD field_50_texY;
    WORD field_52_palX;
    WORD field_54_palY;
    WORD field_56_num_colours;
    char field_58[40];
};
MGS_ASSERT_SIZEOF(pcx_header, 128);

struct pcx_mgs_vram
{
    PSX_RECT field_0_vram_rect;
};
MGS_ASSERT_SIZEOF(pcx_mgs_vram, 8);

struct pcx_mgs
{
    WORD field_0_palx;
    WORD field_2_paly;
    WORD field_4_num_colours;
    WORD field_6_bUnknown;
    WORD field_8_256_pal[256];
    pcx_mgs_vram field_208;
};
MGS_ASSERT_SIZEOF(pcx_mgs, 0x210);


#pragma pack(push)
#pragma pack(1)
struct Tga_header
{
    BYTE field_0_idlength;
    BYTE field_1_colourmaptype;
    BYTE field_2_datatypecode;
    WORD field_3_colourmaporigin;
    WORD field_5_colourmaplength;
    BYTE field_7_colourmapdepth;
    WORD field_8_x_origin;
    WORD field_A_y_origin;
    WORD field_C_width;
    WORD field_E_height;
    BYTE field_10_bitsperpixel;
    BYTE field_11_imagedescriptor;
};
#pragma pack(pop)
MGS_ASSERT_SIZEOF(Tga_header, 0x12);

BYTE* CC jimGetTargetBuffer_42B6A6(TFileNameHash fileNameHash, WORD *pWidth, WORD *pHeight, DWORD** ppAllocated)
{
    const char* pHiTexName = HITEX_NAME_51D4BC(fileNameHash);
    if (!pHiTexName)
    {
        return nullptr;
    }

    FILE* hReplacementTexture = File_LoadDirFile_51EE8F(pHiTexName, 0);
    if (!hReplacementTexture)
    {
        printf("*** $jim *** Error reading file %s.\n", pHiTexName);
        return nullptr;
    }

    const __int32 sizeToRead = File_GetPos_51F09E(hReplacementTexture, 0, 2);

    Tga_header* tgaBuffer = reinterpret_cast<Tga_header*>(malloc(sizeToRead));

    if (!tgaBuffer)
    {
        printf("*** $jim *** Error allocating mempry in jimGetTargetBuffer\n");
        File_Close_51F183(hReplacementTexture);
        return nullptr;
    }

    File_GetPos_51F09E(hReplacementTexture, 0, 0);

    if (File_NormalRead_51F0F5(hReplacementTexture, tgaBuffer, sizeToRead) != static_cast<size_t>(sizeToRead))
    {
        free(tgaBuffer);
        printf("*** $jim *** Error reading file %s.\n", pHiTexName);
        File_Close_51F183(hReplacementTexture);
        return nullptr;
    }

    File_Close_51F183(hReplacementTexture);

    if ((tgaBuffer->field_1_colourmaptype & 0xFF) > 1u       // Only 0 or 1 colour map type supported
        || tgaBuffer->field_2_datatypecode != 2              // Uncompressed, RGB images.
        || tgaBuffer->field_10_bitsperpixel != 16)          // 16 Bits per pixel
    {
        free(tgaBuffer);
        printf("*** $jim *** Error in Targa file %s or unsupported Targa format\n", pHiTexName);
        return 0;
    }

    DWORD colourMapSize = 0;
    if ((tgaBuffer->field_1_colourmaptype & 0xFF) == 1) // has colour map?
    {
        colourMapSize = (tgaBuffer->field_1_colourmaptype & 0xFF) * tgaBuffer->field_7_colourmapdepth / 8;
    }

    *ppAllocated = reinterpret_cast<DWORD*>(tgaBuffer);
    *pWidth = tgaBuffer->field_C_width;
    *pHeight = tgaBuffer->field_E_height;
    return reinterpret_cast<BYTE*>(tgaBuffer) + sizeof(Tga_header) + (colourMapSize + tgaBuffer->field_0_idlength);
}
MGS_FUNC_IMPLEX(0x42B6A6, jimGetTargetBuffer_42B6A6, PCX_IMPL);

Texture_Record* CC sub_40252B(TFileNameHash hashedName, int bpp, __int16 bppShift0x30, PSX_RECT* pVramRect, pcx_mgs* pMgsPcx, BYTE numColours)
{
    Texture_Record* pTexture = nullptr;
    if (LibDG_SearchForTextureRecord_4024D2(hashedName, &pTexture))
    {
        if (pTexture->mUsed)
        {
            pTexture->mHashedName = 0;
        }
    }

    pTexture->mHashedName = hashedName;
    pTexture->mNumColours = numColours;
    pTexture->mUsed = 0;
    pTexture->mTPage = 32 * (bppShift0x30 | 4 * bpp) | (pVramRect->x1 / 64 + 16 * (pVramRect->y1 / 256));
    pTexture->mClut = (pMgsPcx->field_2_paly << 6) | (pMgsPcx->field_0_palx >> 4);

    const BYTE texturePageIndex = pVramRect->x1 % 64;

    BYTE u0 = 0;
    BYTE u1 = 0;
    if (bpp)
    {
        u0 = 2 * texturePageIndex;
        u1 = 2 * pVramRect->x2;
    }
    else
    {
        u0 = 4 * texturePageIndex;
        u1 = 4 * pVramRect->x2;
    }

    pTexture->u0 = u0;
    pTexture->u1 = u1 - 1;
    pTexture->v0 = pVramRect->y1 % 256;
    pTexture->v1 = pVramRect->y2 - 1;
    return pTexture;
}
MGS_FUNC_IMPLEX(0x40252B, sub_40252B, PCX_IMPL);

int CC GV_pcx_file_handler_402B25(void* fileData, TFileNameHash fileNameHash)
{
    pcx_header* pPcxFileData = reinterpret_cast<pcx_header*>(fileData);
    const WORD mgs_bpp = pPcxFileData->field_4C_bpp_mgs;
    WORD maxW = pPcxFileData->field_8_Xmax + 1 - pPcxFileData->field_4_Xmin;
    const WORD maxH = pPcxFileData->field_A_Ymax + 1 - pPcxFileData->field_6_Ymin;
    if (!(mgs_bpp & 1))
    {
        maxW /= 2;
    }
    const int size = maxH * maxW + sizeof(pcx_mgs);
    pcx_mgs* pAllocated = nullptr;
    if (!System_mem_zerod_alloc_40AFA4(gActiveBuffer_dword_791A08, size, (void**)&pAllocated))
    {
        return 0;
    }

    pAllocated->field_0_palx = pPcxFileData->field_52_palX;
    pAllocated->field_2_paly = pPcxFileData->field_54_palY;
    pAllocated->field_4_num_colours = pPcxFileData->field_56_num_colours;
    pAllocated->field_6_bUnknown = 1;

    pcx_mgs_vram* pRect = &pAllocated->field_208;
    pRect->field_0_vram_rect.x1 = pPcxFileData->field_4E_texX;
    pRect->field_0_vram_rect.y1 = pPcxFileData->field_50_texY;
    pRect->field_0_vram_rect.x2 = maxW / 2;
    pRect->field_0_vram_rect.y2 = maxH;

    const BYTE* pPal = nullptr;
    if (mgs_bpp & 1)
    {
        const BYTE* pRet = GV_pcx_file_RLE_decompress_4bit_402F30(
            (const BYTE*)&pPcxFileData[1].field_0_magic,
            (unsigned __int8 *)&pRect[1],
            maxH * maxW);
        pPal = pRet + 1;
    }
    else
    {
        GV_pcx_file_RLE_decompress_8bit_402FB4(
            (const BYTE*)&pPcxFileData[1].field_0_magic,
            (unsigned __int8 *)&pRect[1],
            pPcxFileData->field_42_bytesPerPlaneLine,
            maxW,
            maxH);
        pPal = pPcxFileData->field_10_palette;
    }
    GV_pcx_file_pallete_convert_4031B9(pPal, pAllocated->field_8_256_pal, pAllocated->field_4_num_colours);

    gStageIs_s11e_6FC778 = strcmp(File_StageName_44EB83(), "s11e") == 0;

    // NOTE: Software rendering branch has been pruned here

    WORD tgaWidth = 0;
    WORD tgaHeight = 0;
    DWORD* tgaBackingBuffer = nullptr;
    const BYTE* pTga16BitPixels = jimGetTargetBuffer_42B6A6(fileNameHash, &tgaWidth, &tgaHeight, &tgaBackingBuffer);

    g_Render_sub_41C640_ret_650D1A = Render_sub_41C640(
        &pRect->field_0_vram_rect,
        pAllocated->field_8_256_pal,
        (BYTE*)&pRect[1],
        mgs_bpp & 1,
        pTga16BitPixels,
        tgaWidth,
        tgaHeight);

    if (pTga16BitPixels)
    {
        free(tgaBackingBuffer);
    }

    Render_sub_41C6B0(&pRect->field_0_vram_rect, (BYTE*)&pRect[1]);

    System_VoidAllocation_40B187(gActiveBuffer_dword_791A08, (void**)&pAllocated);
    if (fileNameHash)
    {
        sub_40252B(
            fileNameHash,
            mgs_bpp & 1,
            (mgs_bpp & 0x30) >> 4,
            &pRect->field_0_vram_rect,
            pAllocated,
            static_cast<BYTE>(pAllocated->field_4_num_colours));
    }

    return 1;
}
MGS_FUNC_IMPLEX(0x402B25, GV_pcx_file_handler_402B25, PCX_IMPL);
