#include "stdafx.h"
#include "Renderer.hpp"
#define DIRECT3D_VERSION 0x700
#include "d3d.h"
#include "LibDG.hpp"

#define RENDERER_IMPL true

void RendererCpp_ForceLink() { }

MSG_FUNC_NOT_IMPL(0x40CC50, uint32_t __cdecl(uint32_t, uint32_t, uint32_t, uint32_t*, uint32_t*), Render_ComputeTextureIdx);
MSG_FUNC_NOT_IMPL(0x51DE0A, void __cdecl(), sub_51DE0A);


MGS_VAR(1, 0x6FC780, MGSVertex*, g_pMGSVertices, 0);
MGS_VAR(1, 0x6FC784, DWORD, g_nVertexOffset, 0);
MGS_VAR(1, 0x791C80, float, g_fXOffset, 0);
MGS_VAR(1, 0x791C84, float, g_fYOffset, 0);
MGS_VAR(1, 0x00650D30, DWORD, gModX2, 0);
MGS_VAR(1, 0x791C54, DWORD, otItemType_dword_791C54, 0);

MGS_VAR(1, 0x791C5C, float, g_fV3, 0);
MGS_VAR(1, 0x791C60, float, g_fV2, 0);
MGS_VAR(1, 0x791C64, float, g_fV1, 0);
MGS_VAR(1, 0x791C68, float, g_fV0, 0);
MGS_VAR(1, 0x791C6C, float, g_fU3, 0);
MGS_VAR(1, 0x791C70, float, g_fU2, 0);
MGS_VAR(1, 0x791C74, float, g_fU1, 0);
MGS_VAR(1, 0x791C78, float, g_fU0, 0);

MGS_VAR(1, 0x688CD0, DWORD, dword_688CD0, 0);
MGS_VAR(1, 0x688CD4, DWORD, dword_688CD4, 0);
MGS_VAR(1, 0x791C58, DWORD, size_dword_791C58, 0);

MGS_VAR(1, 0x6C0EA0, WORD, g_wXOffset, 0);
MGS_VAR(1, 0x6C0EA2, WORD, g_wYOffset, 0);

MGS_VAR(1, 0x6C0EAC, WORD, word_6C0EAC, 0);
MGS_VAR(1, 0x791C7C, DWORD, g_nTextureIndex, 0);
MGS_VAR(1, 0x6FC72C, WORD*, g_pwTextureIndices, 0);
MGS_VAR(1, 0x6FC78C, WORD, g_NumTextures, 0);
MGS_VAR(1, 0x00650D28, float, gXRes, 0.0f);

MGS_VAR(1, 0x6C0EFC, prim_struct*, gPrimStructArray, nullptr); // Dynamically allocated array of 15000 items

MGS_VAR(1, 0x6FC788, DWORD, g_nPrimitiveIndex, 0);
MGS_VAR(1, 0x6FC774, DWORD, dword_6FC774, 0);

MGS_VAR(1, 0x6C0EAE, WORD, word_6C0EAE, 0);
MGS_VAR(1, 0x6C0E9A, WORD, word_6C0E9A, 0);
MGS_VAR(1, 0x6C0E9C, WORD, word_6C0E9C, 0);
MGS_VAR(1, 0x6C0E9E, WORD, word_6C0E9E, 0);
MGS_VAR(1, 0x6FC718, DWORD, dword_6FC718, 0);
MGS_VAR(1, 0x6FC720, DWORD, dword_6FC720, 0);
MGS_VAR(1, 0x6FC768, DWORD, dword_6FC768, 0);
MGS_VAR(1, 0x006FC794, DWORD, gSoftwareRendering, 0);
MGS_VAR(1, 0x6FC738, LPDIRECTDRAWSURFACE7, g_pBackBuffer, nullptr);
MGS_VAR(1, 0x6FC868, void*, g_pBackBufferSurface, 0);
MGS_VAR(1, 0x6FC86C, DWORD, g_BackBufferPitch, 0);
MGS_VAR(1, 0x6FC734, LPDIRECTDRAWSURFACE7, g_pPrimarySurface, nullptr);
MGS_ARY(1, 0x6C0F00, texture_struct, 1500, gTextures_dword_6C0F00, {}); // Array of 1500 items


/* TODO: Implement me
uint32_t __cdecl Render_ComputeUVs(uint32_t textureIdx, uint32_t a1, uint16_t u, uint16_t v, float* outU, float* outV);
MSG_FUNC_IMPL(0x40CD80, Render_ComputeUVs)

uint32_t __cdecl Render_ComputeUVs(uint32_t textureIdx, uint32_t a1, uint16_t u, uint16_t v, float* outU, float* outV)
{
uint32_t ret = Render_ComputeUVs_.Ptr()(textureIdx, a1, u, v, outU, outV);
//LOG_INFO("t: " << textureIdx << " a: " << a1 << " u " << u << " v " << v << " ou " << *outU << " ov " << *outV);
return ret;
}
*/

MSG_FUNC_NOT_IMPL(0x40CD80, uint32_t __cdecl(uint32_t, uint32_t, uint32_t, uint32_t, float*, float*), Render_ComputeUVs);
MSG_FUNC_NOT_IMPL(0x40FF20, uint32_t __cdecl(uint32_t, uint32_t, uint32_t, uint32_t, float*, float*), sub_40FF20);
MSG_FUNC_NOT_IMPL(0x40D540, uint32_t __cdecl(int16_t*, int32_t, int32_t), sub_40D540);
MSG_FUNC_NOT_IMPL(0x418A70, int __cdecl(struct TaggedOrderingTablePointer* a_pStructVert, int a_nSize), Render_Software);
MSG_FUNC_NOT_IMPL(0x421C00, void __cdecl(), Render_DrawHardware);

// TODO: Assert sizes
struct MGSSmallVert
{
    WORD x; // 11.1 bits
    WORD y; // 11.1 bits
};

struct MGSFloatVert
{
    float x;
    float y;
};

struct TaggedOrderingTablePointer
{
    uint8_t mPointerB0;
    uint8_t mPointerB1;
    uint8_t mPointerB2;
    uint8_t structType;
};

struct StructVertType0
{
    uint8_t diffuseR;
    uint8_t diffuseG;
    uint8_t diffuseB;
    uint8_t structType;
    MGSSmallVert Vtxs[4];
};

struct MGSLargeVert
{
    MGSSmallVert Vtx;
    uint8_t u;
    uint8_t v;
    uint16_t textureIdx;
};

struct StructVertType1 // TODO: Poly_FT4 ??
{
    uint8_t diffuseR;
    uint8_t diffuseG;
    uint8_t diffuseB;
    uint8_t structType;
    MGSLargeVert TexVtx[4];
};

struct MGSDiffuseVert
{
    uint8_t diffuseR;
    uint8_t diffuseG;
    uint8_t diffuseB;
    uint8_t padding;
    MGSSmallVert Vtx;
};

struct StructVertType2
{
    MGSDiffuseVert DifVtx[4];
};

struct MGSLargeVertDif
{
    uint8_t diffuseR;
    uint8_t diffuseG;
    uint8_t diffuseB;
    uint8_t padding;
    MGSSmallVert Vtx;
    uint8_t u;
    uint8_t v;
    uint16_t textureIdx;
};

struct StructVertType3
{
    MGSLargeVertDif DifVtx[4];
};

struct MGSVertType4
{
    MGSFloatVert Vtx;
    uint8_t u;
    uint8_t v;
    uint16_t textureIdx;
};

struct StructVertType4
{
    uint8_t diffuseR;
    uint8_t diffuseG;
    uint8_t diffuseB;
    uint8_t padding;
    MGSVertType4 Vtx[4];
};

struct StructVertType5
{
    uint32_t field0;
};

struct VertsBlock
{
    TaggedOrderingTablePointer header;
    uint8_t padding[0x3C];
};
static_assert(sizeof(VertsBlock) == 0x40, "VertsBlock must be of size 0x40");

struct PrimitivesChain // TODO: same as struct_gv ??
{
    TaggedOrderingTablePointer* pStructVerts0[2];
    uint8_t nNumStructs;
    uint8_t padding0;
    uint8_t padding1;
    uint8_t padding2;
    uint16_t fieldC;
    uint8_t fieldE[0x4E];
    uint32_t field5C;
    uint32_t field60;
    uint32_t field64;
    uint32_t field68;
    VertsBlock vertBlock0[2];
    VertsBlock vertBlock1[2];
    VertsBlock vertBlock2[2];
};
static_assert(sizeof(PrimitivesChain) == 0x1EC, "PrimitivesChain must be of size 0x1EC");


static void convertVertexType0(StructVertType0* pStructVert, uint32_t nIndex)
{
    int32_t signedX, signedY;
    signedX = pStructVert->Vtxs[nIndex].x << 20;
    signedX >>= 20;
    g_pMGSVertices[g_nVertexOffset].x = (float)signedX + g_fXOffset;

    signedY = pStructVert->Vtxs[nIndex].y << 20;
    signedY >>= 20;
    g_pMGSVertices[g_nVertexOffset].y = (float)signedY + g_fYOffset;
    g_pMGSVertices[g_nVertexOffset].z = 0.0f;

    uint32_t diffuseColor = 0xFF000000 | (pStructVert->diffuseR << 16) | (pStructVert->diffuseG << 8) | (pStructVert->diffuseB);
    g_pMGSVertices[g_nVertexOffset].diffuse = diffuseColor;
    g_pMGSVertices[g_nVertexOffset].w = 1.0f;
    g_nVertexOffset++;
}

static void convertColorWZType0(StructVertType0* pStructVert)
{
    g_pMGSVertices[g_nVertexOffset].z = 0.0f;

    uint32_t diffuseColor = 0xFF000000 | (pStructVert->diffuseR << 16) | (pStructVert->diffuseG << 8) | (pStructVert->diffuseB);
    g_pMGSVertices[g_nVertexOffset].diffuse = diffuseColor;
    g_pMGSVertices[g_nVertexOffset].w = 1.0f;
    g_nVertexOffset++;
}

static void convertVertexType1(StructVertType1* pStructVert, uint32_t nIndex, float u, float v)
{
    int32_t signedX, signedY;
    signedX = pStructVert->TexVtx[nIndex].Vtx.x << 20;
    signedX >>= 20;
    g_pMGSVertices[g_nVertexOffset].x = (float)signedX + g_fXOffset;

    signedY = pStructVert->TexVtx[nIndex].Vtx.y << 20;
    signedY >>= 20;
    g_pMGSVertices[g_nVertexOffset].y = (float)signedY + g_fYOffset;
    g_pMGSVertices[g_nVertexOffset].z = 0.0f;
    g_pMGSVertices[g_nVertexOffset].u = u;
    g_pMGSVertices[g_nVertexOffset].v = v;

    uint32_t diffuseColor = 0xFF000000 | (pStructVert->diffuseR << 16) | (pStructVert->diffuseG << 8) | (pStructVert->diffuseB);
    if (gModX2 == 0)
    {
        uint32_t R = (pStructVert->diffuseR * 0x19A) >> 8;
        uint32_t G = (pStructVert->diffuseG * 0x19A) >> 8;
        uint32_t B = (pStructVert->diffuseB * 0x19A) >> 8;
        R = min(R, 0xFF);
        G = min(G, 0xFF);
        B = min(B, 0xFF);
        diffuseColor = 0xFF000000 | (R << 16) | (G << 8) | (B);
    }

    g_pMGSVertices[g_nVertexOffset].diffuse = diffuseColor;
    g_pMGSVertices[g_nVertexOffset].w = 1.0f;
    g_nVertexOffset++;
}

static void convertExceptPosType1(StructVertType1* pStructVert, float u, float v)
{
    g_pMGSVertices[g_nVertexOffset].z = 0.0f;
    g_pMGSVertices[g_nVertexOffset].u = u;
    g_pMGSVertices[g_nVertexOffset].v = v;

    uint32_t diffuseColor = 0xFF000000 | (pStructVert->diffuseR << 16) | (pStructVert->diffuseG << 8) | (pStructVert->diffuseB);
    if (gModX2 == 0)
    {
        uint32_t R = (pStructVert->diffuseR * 0x19A) >> 8;
        uint32_t G = (pStructVert->diffuseG * 0x19A) >> 8;
        uint32_t B = (pStructVert->diffuseB * 0x19A) >> 8;
        R = min(R, 0xFF);
        G = min(G, 0xFF);
        B = min(B, 0xFF);
        diffuseColor = 0xFF000000 | (R << 16) | (G << 8) | (B);
    }

    g_pMGSVertices[g_nVertexOffset].diffuse = diffuseColor;
    g_pMGSVertices[g_nVertexOffset].w = 1.0f;
    g_nVertexOffset++;
}

static void convertVertexType2(StructVertType2* pStructVert, uint32_t nIndex)
{
    int32_t signedX, signedY;
    signedX = pStructVert->DifVtx[nIndex].Vtx.x << 20;
    signedX >>= 20;
    g_pMGSVertices[g_nVertexOffset].x = (float)signedX + g_fXOffset;

    signedY = pStructVert->DifVtx[nIndex].Vtx.y << 20;
    signedY >>= 20;
    g_pMGSVertices[g_nVertexOffset].y = (float)signedY + g_fYOffset;
    g_pMGSVertices[g_nVertexOffset].z = 0.0f;

    uint32_t diffuseColor = 0xFF000000 | (pStructVert->DifVtx[nIndex].diffuseR << 16) | (pStructVert->DifVtx[nIndex].diffuseG << 8) | (pStructVert->DifVtx[nIndex].diffuseB);
    g_pMGSVertices[g_nVertexOffset].diffuse = diffuseColor;
    g_pMGSVertices[g_nVertexOffset].w = 1.0f;
    g_nVertexOffset++;
}

static void convertVertexType3(StructVertType3* pStructVert, uint32_t nIndex, float u, float v)
{
    int32_t signedX, signedY;
    signedX = pStructVert->DifVtx[nIndex].Vtx.x << 20;
    signedX >>= 20;
    g_pMGSVertices[g_nVertexOffset].x = (float)signedX + g_fXOffset;

    signedY = pStructVert->DifVtx[nIndex].Vtx.y << 20;
    signedY >>= 20;
    g_pMGSVertices[g_nVertexOffset].y = (float)signedY + g_fYOffset;
    g_pMGSVertices[g_nVertexOffset].z = 0.0f;
    g_pMGSVertices[g_nVertexOffset].u = u;
    g_pMGSVertices[g_nVertexOffset].v = v;

    uint32_t diffuseColor = 0xFF000000 | (pStructVert->DifVtx[nIndex].diffuseR << 16) | (pStructVert->DifVtx[nIndex].diffuseG << 8) | (pStructVert->DifVtx[nIndex].diffuseB);
    if (gModX2 == 0)
    {
        uint32_t R = (pStructVert->DifVtx[nIndex].diffuseR * 0x19A) >> 8;
        uint32_t G = (pStructVert->DifVtx[nIndex].diffuseG * 0x19A) >> 8;
        uint32_t B = (pStructVert->DifVtx[nIndex].diffuseB * 0x19A) >> 8;
        R = min(R, 0xFF);
        G = min(G, 0xFF);
        B = min(B, 0xFF);
        diffuseColor = 0xFF000000 | (R << 16) | (G << 8) | (B);
    }

    g_pMGSVertices[g_nVertexOffset].diffuse = diffuseColor;
    g_pMGSVertices[g_nVertexOffset].w = 1.0f;
    g_nVertexOffset++;
}

static float convertPositionFloat(WORD n)
{
    int32_t signedN;
    signedN = n << 20;
    signedN >>= 20;
    return (float)signedN;
}

static uint32_t calculateModX2Diffuse(uint32_t diffuseR, uint32_t diffuseG, uint32_t diffuseB)
{
    uint32_t diffuseColor = 0xFF000000 | (diffuseR << 16) | (diffuseG << 8) | (diffuseB);
    if (gModX2 == 0)
    {
        uint32_t R = (diffuseR * 0x19A) >> 8;
        uint32_t G = (diffuseG * 0x19A) >> 8;
        uint32_t B = (diffuseB * 0x19A) >> 8;
        R = min(R, 0xFF);
        G = min(G, 0xFF);
        B = min(B, 0xFF);
        diffuseColor = 0xFF000000 | (R << 16) | (G << 8) | (B);
    }

    return diffuseColor;
}


static void handleBlendMode(uint16_t nBlend)
{
    if ((otItemType_dword_791C54 & 2) != 0)
    {
        gPrimStructArray[g_nPrimitiveIndex].nBlendMode = 1 + ((nBlend >> 5) & 3);
    }
    else
    {
        gPrimStructArray[g_nPrimitiveIndex].nBlendMode = 0;
    }
}

static void handleBlendMode(uint16_t nBlend, uint16_t offset)
{
    if ((otItemType_dword_791C54 & 2) != 0)
    {
        gPrimStructArray[g_nPrimitiveIndex].nBlendMode = 1 + offset + ((nBlend >> 5) & 3);
    }
    else
    {
        gPrimStructArray[g_nPrimitiveIndex].nBlendMode = offset;
    }
}

static void convertVertexType4(StructVertType4* pStructVert, uint32_t nIndex, float u, float v)
{
    g_pMGSVertices[g_nVertexOffset].x = pStructVert->Vtx[nIndex].Vtx.x + g_fXOffset;
    g_pMGSVertices[g_nVertexOffset].y = pStructVert->Vtx[nIndex].Vtx.y + g_fYOffset;
    g_pMGSVertices[g_nVertexOffset].z = 0.0f;
    g_pMGSVertices[g_nVertexOffset].u = u;
    g_pMGSVertices[g_nVertexOffset].v = v;
    g_pMGSVertices[g_nVertexOffset].diffuse = calculateModX2Diffuse(pStructVert->diffuseR, pStructVert->diffuseG, pStructVert->diffuseB);
    g_pMGSVertices[g_nVertexOffset].w = 1.0f;
    g_nVertexOffset++;
}

// Untested for the moment
//MSG_FUNC_NOT_IMPL(0x410560, int __cdecl(StructVert* a_pStructVert, int a_nSize), ConvertPolys_Hardware);
int __cdecl ConvertPolys_Hardware(TaggedOrderingTablePointer* otItem, int otItemSize)
{
    uint32_t var14 = dword_688CD4;
    uint32_t var1C = dword_688CD0;

    for (;;)
    {
        if (otItemSize <= 0)
        {
            return 1;
        }

        otItemType_dword_791C54 = otItem->structType;
        size_dword_791C58 = 0;
        g_fV3 = g_fV2 = g_fV1 = g_fV0 = 0;
        g_fU3 = g_fU2 = g_fU1 = g_fU0 = 0;

        gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 0;
        gPrimStructArray[g_nPrimitiveIndex].nBlendMode = 0;

        // 100-103 case has an issue, causes corrupted text
        //LOG_INFO("VTX type: " << dword_791C54);

        switch (otItemType_dword_791C54)
        {
        case 0:
        {
            return 1;
        }

        case 32: // monochrome 3 point polygon
        case 33:
        case 34:
        case 35:
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType0(pStructVert, 0);
            convertVertexType0(pStructVert, 1);
            convertVertexType0(pStructVert, 2);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 3;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLELIST;
            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 4;
            break;
        }

        case 40: // monchrome 4 point polygon
        case 41:
        case 42:
        case 43:
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType0(pStructVert, 0);
            convertVertexType0(pStructVert, 1);
            convertVertexType0(pStructVert, 2);
            convertVertexType0(pStructVert, 3);

            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;
            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            size_dword_791C58 = 5;
            break;
        }

        case 44:
        case 45:
        case 46:
        case 47:
        {
            StructVertType1* pStructVert = (StructVertType1*)otItem;
            uint32_t TextureIdx0, TextureIdx1;
            Render_ComputeTextureIdx(pStructVert->TexVtx[1].textureIdx, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v, &TextureIdx0, &TextureIdx1);
            TextureIdx0 &= 0xFFFF;
            TextureIdx1 &= 0xFFFF;

            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = g_pwTextureIndices[TextureIdx1 * 0x400 + TextureIdx0];
            g_nTextureIndex = gPrimStructArray[g_nPrimitiveIndex].nTextureIndex;

            if (g_nTextureIndex >= g_NumTextures)
            {
                gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0;
            }
            else
            {
                const uint32_t texturePage = (pStructVert->TexVtx[1].textureIdx & 0x180) >> 7;
                Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v, &g_fU0, &g_fV0);
                Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[1].u, pStructVert->TexVtx[1].v, &g_fU1, &g_fV1);
                Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[2].u, pStructVert->TexVtx[2].v, &g_fU2, &g_fV2);
                Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[3].u, pStructVert->TexVtx[3].v, &g_fU3, &g_fV3);
            }

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(pStructVert->TexVtx[1].textureIdx);

            convertVertexType1(pStructVert, 0, g_fU0, g_fV0);
            convertVertexType1(pStructVert, 1, g_fU1, g_fV1);
            convertVertexType1(pStructVert, 2, g_fU2, g_fV2);
            convertVertexType1(pStructVert, 3, g_fU3, g_fV3);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            size_dword_791C58 = 9;
            break;
        }

        case 48:
        case 49:
        case 50:
        case 51:
        {
            StructVertType2* pStructVert = (StructVertType2*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType2(pStructVert, 0);
            convertVertexType2(pStructVert, 1);
            convertVertexType2(pStructVert, 2);

            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;
            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 3;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_GOURAUD;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLELIST;

            size_dword_791C58 = 6;
            break;
        }

        case 52:
        case 53:
        case 54:
        case 55:
        {
            StructVertType3* pStructVert = (StructVertType3*)otItem;

            uint32_t TextureIdx0, TextureIdx1;
            Render_ComputeTextureIdx(pStructVert->DifVtx[1].textureIdx, pStructVert->DifVtx[0].u, pStructVert->DifVtx[0].v, &TextureIdx0, &TextureIdx1);
            TextureIdx0 &= 0xFFFF;
            TextureIdx1 &= 0xFFFF;

            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = g_pwTextureIndices[TextureIdx1 * 0x400 + TextureIdx0];
            g_nTextureIndex = gPrimStructArray[g_nPrimitiveIndex].nTextureIndex;

            if (g_nTextureIndex >= g_NumTextures)
            {
                gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0;
            }
            else
            {
                const uint32_t texturePage = (pStructVert->DifVtx[1].textureIdx & 0x180) >> 7;
                Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->DifVtx[0].u, pStructVert->DifVtx[0].v, &g_fU0, &g_fV0);
                Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->DifVtx[1].u, pStructVert->DifVtx[1].v, &g_fU1, &g_fV1);
                Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->DifVtx[2].u, pStructVert->DifVtx[2].v, &g_fU2, &g_fV2);
            }

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(pStructVert->DifVtx[1].textureIdx);

            convertVertexType3(pStructVert, 0, g_fU0, g_fV0);
            convertVertexType3(pStructVert, 1, g_fU1, g_fV1);
            convertVertexType3(pStructVert, 2, g_fU2, g_fV2);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 3;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_GOURAUD;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLELIST;

            size_dword_791C58 = 9;
        }

        case 56:
        case 57:
        case 58:
        case 59:
        {
            StructVertType2* pStructVert = (StructVertType2*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            if (gPrimStructArray[g_nPrimitiveIndex].nBlendMode == 3)
            {
                for (int i = 0; i < 4; i++)
                {
                    pStructVert->DifVtx[i].diffuseB = (uint8_t)min(pStructVert->DifVtx[i].diffuseB << 2, 0xFF);
                    pStructVert->DifVtx[i].diffuseG = (uint8_t)min(pStructVert->DifVtx[i].diffuseG << 2, 0xFF);
                    pStructVert->DifVtx[i].diffuseR = (uint8_t)min(pStructVert->DifVtx[i].diffuseR << 2, 0xFF);
                }
            }

            convertVertexType2(pStructVert, 0);
            convertVertexType2(pStructVert, 1);
            convertVertexType2(pStructVert, 2);
            convertVertexType2(pStructVert, 3);

            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;
            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_GOURAUD;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            if (gPrimStructArray[g_nPrimitiveIndex].nBlendMode == 3 && dword_6FC774 != 0)
            {
                convertVertexType2(pStructVert, 0);
                convertVertexType2(pStructVert, 1);
                convertVertexType2(pStructVert, 2);
                convertVertexType2(pStructVert, 3);

                gPrimStructArray[g_nPrimitiveIndex].dwVertexCount += 4;
            }

            size_dword_791C58 = 8;
            break;
        }

        case 60:
        case 61:
        case 62:
        case 63:
        {
            StructVertType3* pStructVert = (StructVertType3*)otItem;
            float position[16];
            position[11] = 0.5f;
            position[10] = 0.5f;
            position[9] = 0.5f;
            position[8] = 0.5f;
            position[15] = 1.0f;
            position[14] = 0.999999f;
            position[13] = 0.999999f;
            position[12] = 0.999999f;

            position[3] = convertPositionFloat(pStructVert->DifVtx[0].Vtx.x);
            position[2] = convertPositionFloat(pStructVert->DifVtx[1].Vtx.x);
            position[1] = convertPositionFloat(pStructVert->DifVtx[2].Vtx.x);
            position[0] = convertPositionFloat(pStructVert->DifVtx[3].Vtx.x);

            position[7] = convertPositionFloat(pStructVert->DifVtx[0].Vtx.y);
            position[6] = convertPositionFloat(pStructVert->DifVtx[1].Vtx.y);
            position[5] = convertPositionFloat(pStructVert->DifVtx[2].Vtx.y);
            position[4] = convertPositionFloat(pStructVert->DifVtx[3].Vtx.y);

            if ((pStructVert->DifVtx[3].textureIdx & 0x8000) != 0 && var1C != 0)
            {
                float* var68 = (float*)(0x734A40 + ((pStructVert->DifVtx[3].textureIdx & 0xFFF) << 6)); // TODO : Var this struct (sizeof = 0x40)
                if (var68[8] > 0.0005f)
                    position[15] = 1.0f / var68[8];
                if (var68[9] > 0.0005f)
                    position[14] = 1.0f / var68[9];
                if (var68[10] > 0.0005f)
                    position[13] = 1.0f / var68[10];
                if (var68[11] > 0.0005f)
                    position[12] = 1.0f / var68[11];

                if (var14 != 0)
                {
                    position[3] = var68[0];
                    position[2] = var68[1];
                    position[1] = var68[2];
                    position[0] = var68[3];

                    position[7] = var68[4];
                    position[6] = var68[5];
                    position[5] = var68[6];
                    position[4] = var68[7];
                }
            }

            uint32_t TextureIdx0, TextureIdx1;
            if (Render_ComputeTextureIdx(pStructVert->DifVtx[1].textureIdx, pStructVert->DifVtx[0].u, pStructVert->DifVtx[0].v, &TextureIdx0, &TextureIdx1) != 0)
            {
                gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFE;
                g_fU0 = (float)(TextureIdx0 & 0xFFFF);
                g_fV0 = (float)(TextureIdx1 & 0xFFFF);

                Render_ComputeTextureIdx(pStructVert->DifVtx[1].textureIdx, pStructVert->DifVtx[1].u, pStructVert->DifVtx[1].v, &TextureIdx0, &TextureIdx1);
                g_fU1 = (float)(TextureIdx0 & 0xFFFF);
                g_fV1 = (float)(TextureIdx1 & 0xFFFF);

                Render_ComputeTextureIdx(pStructVert->DifVtx[1].textureIdx, pStructVert->DifVtx[2].u, pStructVert->DifVtx[2].v, &TextureIdx0, &TextureIdx1);
                g_fU2 = (float)(TextureIdx0 & 0xFFFF);
                g_fV2 = (float)(TextureIdx1 & 0xFFFF);

                Render_ComputeTextureIdx(pStructVert->DifVtx[1].textureIdx, pStructVert->DifVtx[3].u, pStructVert->DifVtx[3].v, &TextureIdx0, &TextureIdx1);
                g_fU3 = (float)(TextureIdx0 & 0xFFFF);
                g_fV3 = (float)(TextureIdx1 & 0xFFFF);
            }
            else
            {
                TextureIdx0 &= 0xFFFF;
                TextureIdx1 &= 0xFFFF;

                gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = g_pwTextureIndices[TextureIdx1 * 0x400 + TextureIdx0];
                g_nTextureIndex = gPrimStructArray[g_nPrimitiveIndex].nTextureIndex;

                if (g_nTextureIndex >= g_NumTextures)
                {
                    gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0;
                }
                else
                {
                    const uint32_t texturePage = (pStructVert->DifVtx[1].textureIdx & 0x180) >> 7;
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->DifVtx[0].u, pStructVert->DifVtx[0].v, &g_fU0, &g_fV0);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->DifVtx[1].u, pStructVert->DifVtx[1].v, &g_fU1, &g_fV1);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->DifVtx[2].u, pStructVert->DifVtx[2].v, &g_fU2, &g_fV2);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->DifVtx[3].u, pStructVert->DifVtx[3].v, &g_fU3, &g_fV3);

                    uint16_t* pIndex = (uint16_t*)(0x6FC728 + ((pStructVert->DifVtx[0].textureIdx >> 6) << 11) + ((pStructVert->DifVtx[0].textureIdx & 0x3F) << 5));
                    if (*pIndex == 0xEDED)
                    {
                        gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFF0;
                        otItemType_dword_791C54 &= 0xFFFFFFFD;
                    }
                }
            }

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(pStructVert->DifVtx[1].textureIdx);

            g_pMGSVertices[g_nVertexOffset].x = position[3] + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = position[7] + g_fYOffset;
            g_pMGSVertices[g_nVertexOffset].z = position[11];
            g_pMGSVertices[g_nVertexOffset].w = position[15];
            g_pMGSVertices[g_nVertexOffset].u = g_fU0;
            g_pMGSVertices[g_nVertexOffset].v = g_fV0;
            g_pMGSVertices[g_nVertexOffset].diffuse = calculateModX2Diffuse(pStructVert->DifVtx[0].diffuseR, pStructVert->DifVtx[0].diffuseG, pStructVert->DifVtx[0].diffuseB);
            g_nVertexOffset++;

            g_pMGSVertices[g_nVertexOffset].x = position[2] + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = position[6] + g_fYOffset;
            g_pMGSVertices[g_nVertexOffset].z = position[10];
            g_pMGSVertices[g_nVertexOffset].w = position[14];
            g_pMGSVertices[g_nVertexOffset].u = g_fU1;
            g_pMGSVertices[g_nVertexOffset].v = g_fV1;
            g_pMGSVertices[g_nVertexOffset].diffuse = calculateModX2Diffuse(pStructVert->DifVtx[1].diffuseR, pStructVert->DifVtx[1].diffuseG, pStructVert->DifVtx[1].diffuseB);
            g_nVertexOffset++;

            g_pMGSVertices[g_nVertexOffset].x = position[1] + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = position[5] + g_fYOffset;
            g_pMGSVertices[g_nVertexOffset].z = position[9];
            g_pMGSVertices[g_nVertexOffset].w = position[13];
            g_pMGSVertices[g_nVertexOffset].u = g_fU2;
            g_pMGSVertices[g_nVertexOffset].v = g_fV2;
            g_pMGSVertices[g_nVertexOffset].diffuse = calculateModX2Diffuse(pStructVert->DifVtx[2].diffuseR, pStructVert->DifVtx[2].diffuseG, pStructVert->DifVtx[2].diffuseB);
            g_nVertexOffset++;

            g_pMGSVertices[g_nVertexOffset].x = position[0] + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = position[4] + g_fYOffset;
            g_pMGSVertices[g_nVertexOffset].z = position[8];
            g_pMGSVertices[g_nVertexOffset].w = position[12];
            g_pMGSVertices[g_nVertexOffset].u = g_fU3;
            g_pMGSVertices[g_nVertexOffset].v = g_fV3;
            g_pMGSVertices[g_nVertexOffset].diffuse = calculateModX2Diffuse(pStructVert->DifVtx[3].diffuseR, pStructVert->DifVtx[3].diffuseG, pStructVert->DifVtx[3].diffuseB);
            g_nVertexOffset++;

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_GOURAUD;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            size_dword_791C58 = 0xC;
            break;
        }

        case 64:
        case 65:
        case 66:
        case 67:
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            float fXSize, fYSize;
            sub_40FF20(pStructVert->Vtxs[0].x, pStructVert->Vtxs[0].y, pStructVert->Vtxs[1].x, pStructVert->Vtxs[1].y, &fXSize, &fYSize);
            handleBlendMode(word_6C0EAC);

            convertVertexType0(pStructVert, 0);
            convertVertexType0(pStructVert, 0);
            g_pMGSVertices[g_nVertexOffset - 1].x += fXSize;
            g_pMGSVertices[g_nVertexOffset - 1].y += fYSize;
            convertVertexType0(pStructVert, 1);
            convertVertexType0(pStructVert, 1);
            g_pMGSVertices[g_nVertexOffset - 1].x += fXSize;
            g_pMGSVertices[g_nVertexOffset - 1].y += fYSize;

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;
            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 3;
            break;
        }

        case 72:
        case 73:
        case 74:
        case 75:
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType0(pStructVert, 0);
            convertVertexType0(pStructVert, 1);
            convertVertexType0(pStructVert, 2);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 3;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_LINESTRIP;
            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 5;
            break;
        }

        case 76:
        case 77:
        case 78:
        case 79:
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType0(pStructVert, 0);
            convertVertexType0(pStructVert, 1);
            convertVertexType0(pStructVert, 2);
            convertVertexType0(pStructVert, 3);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_LINESTRIP;
            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 6;
            break;
        }

        case 80:
        case 81:
        case 82:
        case 83:
        {
            StructVertType2* pStructVert = (StructVertType2*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType2(pStructVert, 0);
            convertVertexType2(pStructVert, 1);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 2;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_GOURAUD;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_LINELIST;
            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 4;
            break;
        }

        case 96:
        case 97:
        case 98:
        case 99:
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;

            float fSecondX = convertPositionFloat(pStructVert->Vtxs[0].x) + (float)((int16_t)pStructVert->Vtxs[1].x);
            float fSecondY = convertPositionFloat(pStructVert->Vtxs[0].y) + (float)((int16_t)pStructVert->Vtxs[1].y);

            handleBlendMode(word_6C0EAC);
            convertVertexType0(pStructVert, 0);
            g_pMGSVertices[g_nVertexOffset].x = fSecondX + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = convertPositionFloat(pStructVert->Vtxs[0].y) + g_fYOffset;
            convertColorWZType0(pStructVert);
            g_pMGSVertices[g_nVertexOffset].x = convertPositionFloat(pStructVert->Vtxs[0].x) + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = fSecondY + g_fYOffset;
            convertColorWZType0(pStructVert);
            g_pMGSVertices[g_nVertexOffset].x = fSecondX + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = fSecondY + g_fYOffset;
            convertColorWZType0(pStructVert);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;
            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 3;
            break;
        }

        case 100:
        case 101:
        case 102:
        case 103:
        {
            StructVertType1* pStructVert = (StructVertType1*)otItem;
            float fInverseRes = 1.0f / gXRes;

            int16_t diffX = (int16_t)pStructVert->TexVtx[1].Vtx.x;
            int16_t diffY = (int16_t)pStructVert->TexVtx[1].Vtx.y;
            float fSecondX = convertPositionFloat(pStructVert->TexVtx[0].Vtx.x) + (float)diffX;
            float fSecondY = convertPositionFloat(pStructVert->TexVtx[0].Vtx.y) + (float)diffY;

            uint32_t TextureIdx0, TextureIdx1;
            if (Render_ComputeTextureIdx(word_6C0EAC, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v, &TextureIdx0, &TextureIdx1) != 0)
            {
                gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFD;
                g_fU0 = ((float)(TextureIdx0 & 0xFFFF)) / fInverseRes;
                g_fV0 = ((float)(TextureIdx1 & 0xFFFF)) / fInverseRes;

                Render_ComputeTextureIdx(word_6C0EAC, pStructVert->TexVtx[0].u + diffX - 1, pStructVert->TexVtx[0].v, &TextureIdx0, &TextureIdx1);
                g_fU1 = ((float)(TextureIdx0 & 0xFFFF)) / fInverseRes;
                g_fV1 = ((float)(TextureIdx1 & 0xFFFF)) / fInverseRes;

                Render_ComputeTextureIdx(word_6C0EAC, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v + diffY, &TextureIdx0, &TextureIdx1);
                g_fU2 = ((float)(TextureIdx0 & 0xFFFF)) / fInverseRes;
                g_fV2 = ((float)(TextureIdx1 & 0xFFFF)) / fInverseRes;

                Render_ComputeTextureIdx(word_6C0EAC, pStructVert->TexVtx[0].u + diffX - 1, pStructVert->TexVtx[0].v + diffY, &TextureIdx0, &TextureIdx1);
                g_fU3 = ((float)(TextureIdx0 & 0xFFFF)) / fInverseRes;
                g_fV3 = ((float)(TextureIdx1 & 0xFFFF)) / fInverseRes;
            }
            else
            {
                if (pStructVert->TexVtx[0].textureIdx & 0x8000)
                {
                    gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 1 + (pStructVert->TexVtx[0].textureIdx & 0xF);
                    g_nTextureIndex = gPrimStructArray[g_nPrimitiveIndex].nTextureIndex;
                    pStructVert->TexVtx[0].u = 0;
                    pStructVert->TexVtx[0].v = 0;
                }
                else
                {
                    TextureIdx0 &= 0xFFFF;
                    TextureIdx1 &= 0xFFFF;
                    gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = g_pwTextureIndices[TextureIdx1 * 0x400 + TextureIdx0];
                    g_nTextureIndex = gPrimStructArray[g_nPrimitiveIndex].nTextureIndex;
                }
                if (g_nTextureIndex >= g_NumTextures)
                {
                    gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0;
                }
                else
                {
                    const uint32_t texturePage = (word_6C0EAC & 0x180) >> 7;
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v, &g_fU0, &g_fV0);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[0].u + diffX, pStructVert->TexVtx[0].v, &g_fU1, &g_fV1);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[0].u, pStructVert->TexVtx[0].v + diffY, &g_fU2, &g_fV2);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->TexVtx[0].u + diffX, pStructVert->TexVtx[0].v + diffY, &g_fU3, &g_fV3);
                }
            }

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC, 0x8000);

            convertVertexType1(pStructVert, 0, g_fU0, g_fV0);
            g_pMGSVertices[g_nVertexOffset].x = fSecondX + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = convertPositionFloat(pStructVert->TexVtx[0].Vtx.y) + g_fYOffset;
            convertExceptPosType1(pStructVert, g_fU1, g_fV1);
            g_pMGSVertices[g_nVertexOffset].x = convertPositionFloat(pStructVert->TexVtx[0].Vtx.x) + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = fSecondY + g_fYOffset;
            convertExceptPosType1(pStructVert, g_fU2, g_fV2);
            g_pMGSVertices[g_nVertexOffset].x = fSecondX + g_fXOffset;
            g_pMGSVertices[g_nVertexOffset].y = fSecondY + g_fYOffset;
            convertExceptPosType1(pStructVert, g_fU3, g_fV3);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            size_dword_791C58 = 4;
            break;
        }

        case 104:
        case 105:
        case 106:
        case 107:
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            float fHalfOffset = (((gXRes - 1.0f) / 2.0f) + 1.0f) / gXRes;

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            float centerX = convertPositionFloat(pStructVert->Vtxs[0].x);
            float centerY = convertPositionFloat(pStructVert->Vtxs[0].y);
            g_pMGSVertices[g_nVertexOffset].x = centerX + g_fXOffset - fHalfOffset;
            g_pMGSVertices[g_nVertexOffset].y = centerY + g_fYOffset - fHalfOffset;
            convertColorWZType0(pStructVert);
            g_pMGSVertices[g_nVertexOffset].x = centerX + g_fXOffset + fHalfOffset;
            g_pMGSVertices[g_nVertexOffset].y = centerY + g_fYOffset - fHalfOffset;
            convertColorWZType0(pStructVert);
            g_pMGSVertices[g_nVertexOffset].x = centerX + g_fXOffset - fHalfOffset;
            g_pMGSVertices[g_nVertexOffset].y = centerY + g_fYOffset + fHalfOffset;
            convertColorWZType0(pStructVert);
            g_pMGSVertices[g_nVertexOffset].x = centerX + g_fXOffset + fHalfOffset;
            g_pMGSVertices[g_nVertexOffset].y = centerY + g_fYOffset + fHalfOffset;
            convertColorWZType0(pStructVert);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;
            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 2;
            break;
        }

        case 128: // $80     move image in frame buffer
        {
            StructVertType0* pStructVert = (StructVertType0*)otItem;
            int16_t rawPos[4];

            // source coord
            rawPos[1] = pStructVert->Vtxs[0].y;
            rawPos[0] = pStructVert->Vtxs[0].x;

            // dest coord
            int16_t varDC = pStructVert->Vtxs[1].y;
            int16_t varD8 = pStructVert->Vtxs[1].x;

            // w/h to xfer
            rawPos[3] = pStructVert->Vtxs[2].y;
            rawPos[2] = pStructVert->Vtxs[2].x;

            // move image
            sub_40D540(rawPos, varD8, varDC);

            size_dword_791C58 = 4;
            break;
        }

        case 144:
        case 145:
        case 146:
        case 147:
        {
            StructVertType4* pStructVert = (StructVertType4*)otItem;
            if ((pStructVert->Vtx[0].textureIdx & 0x8000) != 0)
            {
                gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFF0;
            }
            else
            {
                uint32_t TextureIdx0, TextureIdx1;
                Render_ComputeTextureIdx(pStructVert->Vtx[1].textureIdx, pStructVert->Vtx[0].u, pStructVert->Vtx[0].v, &TextureIdx0, &TextureIdx1);
                TextureIdx0 &= 0xFFFF;
                TextureIdx1 &= 0xFFFF;
                gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = g_pwTextureIndices[TextureIdx1 * 0x400 + TextureIdx0];
                g_nTextureIndex = gPrimStructArray[g_nPrimitiveIndex].nTextureIndex;

                if (g_nTextureIndex >= g_NumTextures)
                {
                    gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0;
                }
                else
                {
                    const uint32_t texturePage = (pStructVert->Vtx[1].textureIdx & 0x180) >> 7;
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->Vtx[0].u, pStructVert->Vtx[0].v, &g_fU0, &g_fV0);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->Vtx[1].u, pStructVert->Vtx[1].v, &g_fU1, &g_fV1);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->Vtx[2].u, pStructVert->Vtx[2].v, &g_fU2, &g_fV2);
                    Render_ComputeUVs(g_nTextureIndex, texturePage, pStructVert->Vtx[3].u, pStructVert->Vtx[3].v, &g_fU3, &g_fV3);
                }
            }

            g_fXOffset = g_wXOffset;
            g_fYOffset = g_wYOffset;
            handleBlendMode(word_6C0EAC);

            convertVertexType4(pStructVert, 0, g_fU0, g_fV0);
            convertVertexType4(pStructVert, 1, g_fU1, g_fV1);
            convertVertexType4(pStructVert, 2, g_fU2, g_fV2);
            convertVertexType4(pStructVert, 3, g_fU3, g_fV3);

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 4;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = D3DPT_TRIANGLESTRIP;

            size_dword_791C58 = 0xD;
            break;
        }

        case 225:
        {
            StructVertType5* pStructVert = (StructVertType5*)otItem;

            word_6C0EAC = pStructVert->field0 & 0x1FF;
            word_6C0EAE = (((pStructVert->field0 >> 10) & 1) << 8) | ((pStructVert->field0 >> 9) & 1);

            size_dword_791C58 = 1;
            break;
        }

        case 226:
        {
            size_dword_791C58 = 1;
            break;
        }

        case 227:
        {
            StructVertType5* pStructVert = (StructVertType5*)otItem;

            gDrawEnv_6C0E98.clip.x1 = pStructVert->field0 & 0x3FF;
            word_6C0E9A = (pStructVert->field0 >> 10) & 0x3FF;

            size_dword_791C58 = 1;
            break;
        }

        case 228:
        {
            StructVertType5* pStructVert = (StructVertType5*)otItem;

            word_6C0E9C = (pStructVert->field0 & 0x3FF) - gDrawEnv_6C0E98.clip.x1 + 1;
            word_6C0E9E = ((pStructVert->field0 >> 10) & 0x3FF) - word_6C0E9A + 1;

            g_pMGSVertices[g_nVertexOffset].x = (float)gDrawEnv_6C0E98.clip.x1;
            g_pMGSVertices[g_nVertexOffset].y = (float)word_6C0E9A;
            g_pMGSVertices[g_nVertexOffset].u = (float)word_6C0E9C;
            g_pMGSVertices[g_nVertexOffset].v = (float)word_6C0E9E;
            g_nVertexOffset++;

            gPrimStructArray[g_nPrimitiveIndex].dwVertexCount = 1;
            gPrimStructArray[g_nPrimitiveIndex].mShadeMode = D3DSHADE_FLAT;
            gPrimStructArray[g_nPrimitiveIndex].mPrimTypeQ = 0x7D0; // ?
            gPrimStructArray[g_nPrimitiveIndex].nTextureIndex = 0xFFFF;

            size_dword_791C58 = 1;
            break;
        }

        case 229:
        {
            StructVertType5* pStructVert = (StructVertType5*)otItem;

            g_wXOffset = pStructVert->field0 & 0x7FF;
            g_wYOffset = (pStructVert->field0 >> 11) & 0x3FF;

            size_dword_791C58 = 1;
            break;
        }

        case 255:
        {
            size_dword_791C58 = 1;
            break;
        }

        default:
            break;
        }

        if (gPrimStructArray[g_nPrimitiveIndex].dwVertexCount != 0)
        {
            g_nPrimitiveIndex++;
        }

        if (size_dword_791C58 == 0)
        {
            uint8_t* pValue = (uint8_t*)(0x650A5C + otItemType_dword_791C54);
            size_dword_791C58 = *pValue;
        }

        if (size_dword_791C58 == 0)
        {
            size_dword_791C58 = 1;
        }

        otItemSize -= size_dword_791C58;
        otItem = (TaggedOrderingTablePointer*)((intptr_t)otItem + size_dword_791C58 * 4);
    }
}
MSG_FUNC_IMPLEX(0x410560, ConvertPolys_Hardware, RENDERER_IMPL);


#define OT_END_TAG 0xFFFFFF

int CC Renderer_ClearOTag(DWORD* ot, int otSize)
{
    if (otSize - 1 <= 0)
    {
        // As we only have 1 item, set the start of the table to be the end marker
        *ot = OT_END_TAG;
    }
    else
    {
        // Get a pointer to the last item
        DWORD* pOTItem = &ot[otSize - 1];
        int count = otSize - 1;
        do
        {
            // Set the current item to point to the previous item
            *pOTItem = reinterpret_cast<DWORD>(pOTItem - 1);
            if (reinterpret_cast<DWORD>(pOTItem) & 0xFF000000)
            {
                printf(
                    "\n"
                    "\n"
                    "***** ERROR: ClearOTag() found a pointer which uses more than 24 bit *****\n"
                    " Invalid pointer value caused overflow [%x]\n"
                    "\n",
                    *pOTItem);
            }
            --pOTItem;
            --count;
        } while (count);

        // Set the first item to the end marker
        *ot = OT_END_TAG;
    }
    return 0;
}
MSG_FUNC_IMPLEX(0x0044AB80, Renderer_ClearOTag, RENDERER_IMPL);


//MSG_FUNC_NOT_IMPL(0x4103B0, void __cdecl(StructVert*), Render_DrawGeneric);
void CC Render_DrawGeneric(TaggedOrderingTablePointer* a_pStructVert)
{
    if (dword_6FC718 == 1)
    {
        dword_6FC718 = 0;
        dword_6FC720 = 1;
        return;
    }

    dword_6FC718 = 0;
    if (dword_6FC720 == 0)
    {
        if (gSoftwareRendering != 0)
        {
            DDSURFACEDESC2 desc;
            memset(&desc, 0, sizeof(DDSURFACEDESC2));
            desc.dwSize = sizeof(DDSURFACEDESC2);
            g_pBackBuffer->Lock(NULL, &desc, 0, 0);
            g_pBackBufferSurface = desc.lpSurface;
            g_BackBufferPitch = desc.lPitch;
        }
        do
        {
            if (a_pStructVert->structType != 0 && dword_6FC768 == 0)
            {
                if (gSoftwareRendering != 0)
                {
                    Render_Software(&a_pStructVert[1], a_pStructVert->structType);
                }
                else
                {
                    ConvertPolys_Hardware(&a_pStructVert[1], a_pStructVert->structType);
                }
            }

            // Get the pointer bytes of the OT, the remainder byte is the type.. TODO - how does the vertex info fit into the OT?
            uint32_t nextStructVert = ((uint32_t*)a_pStructVert)[0] & 0x00FFFFFF;
            a_pStructVert = (TaggedOrderingTablePointer*)nextStructVert;
        } while ((uint32_t)a_pStructVert != OT_END_TAG);

        if (gSoftwareRendering != 0)
        {
            g_pBackBuffer->Unlock(0);
        }
        if (gSoftwareRendering == 0)
        {
            Render_DrawHardware();
        }
    }
    if (gSoftwareRendering == 0)
    {
        sub_51DE0A();
    }
}
MSG_FUNC_IMPLEX(0x4103B0, Render_DrawGeneric, RENDERER_IMPL);

//MSG_FUNC_NOT_IMPL(0x401619, void __cdecl(uint32_t), Render_DrawIndex);
void CC Render_DrawIndex(uint32_t activeBuffer)
{
    TaggedOrderingTablePointer* pStructVert = (TaggedOrderingTablePointer*)&gLibGvStruct0_6BC180.dword_6BC3D8_dst[16 * activeBuffer];
    Render_DrawGeneric(pStructVert);
}
MSG_FUNC_IMPLEX(0x401619, Render_DrawIndex, RENDERER_IMPL);


//MSG_FUNC_NOT_IMPL(0x0041CC30, __int16 __cdecl(), Render_RestoreAll);
__int16 CC Render_RestoreAll()
{
    if (g_pPrimarySurface->IsLost() == DDERR_SURFACELOST)
    {
        const HRESULT hr = g_pPrimarySurface->Restore();
        if (FAILED(hr))
        {
            PrintDDError("Prim restore caput", hr);
        }
    }

    if (g_pBackBuffer->IsLost() == DDERR_SURFACELOST)
    {
        const HRESULT hr = g_pBackBuffer->Restore();
        if (FAILED(hr))
        {
            PrintDDError("Ren restore caput", hr);
        }
    }

    for (int i = 0; i < g_NumTextures; i++)
    {
        if (gTextures_dword_6C0F00[i].mSurface)
        {
            if (gTextures_dword_6C0F00[i].mSurface->IsLost() == DDERR_SURFACELOST)
            {
                const HRESULT hr = gTextures_dword_6C0F00[i].mSurface->Restore();
                if (FAILED(hr))
                {
                    PrintDDError("tex #%i restore caput", i);
                }
            }
        }
    }

    return g_NumTextures;
}
MSG_FUNC_IMPLEX(0x0041CC30, Render_RestoreAll, RENDERER_IMPL);


//MSG_FUNC_NOT_IMPL(0x00422D40, char *__cdecl(char*, HRESULT), PrintDDError);
void CC PrintDDError(const char* errMsg, HRESULT hrErr)
{
    char* pStrErr = nullptr;
    switch (hrErr)
    {
    case DDERR_NOBLTHW:                 pStrErr = "DD - NOBLTHW";                   break;
    case DDERR_WRONGMODE:               pStrErr = "DD - WRONGMODE";                 break;
    case DDERR_IMPLICITLYCREATED:       pStrErr = "DD - IMPLICITLYCREATED";         break;
    case DDERR_CLIPPERISUSINGHWND:      pStrErr = "DD - CLIPPERISUSINGHWND";        break;
    case DDERR_WASSTILLDRAWING:         pStrErr = "DD - WASSTILLDRAWING";           break;
    case DDERR_SURFACELOST:             pStrErr = "DD - SURFACELOST";               break;
    case DDERR_NOEXCLUSIVEMODE:         pStrErr = "DD - NOEXCLUSIVEMODE";           break;
    case DDERR_SURFACEBUSY:             pStrErr = "DD - DSURFACEBUSY";              break;
    case DDERR_INVALIDRECT:             pStrErr = "DD - INVALIDRECT";               break;
    case DDERR_INCOMPATIBLEPRIMARY:     pStrErr = "DD - DDERR_INCOMPATIBLEPRIMARY"; break;
    case DDERR_INVALIDCLIPLIST:         pStrErr = "DD - INVALIDCLIPLIST";           break;
    case DDERR_INVALIDOBJECT:           pStrErr = "DD - INVALIDOBJECT";             break;
    case DDERR_EXCEPTION:               pStrErr = "DD - EXCEPTION";                 break;
    case DDERR_INVALIDPARAMS:           pStrErr = "DD - INVALIDPARAMS";             break;
    case DDERR_OUTOFMEMORY:             pStrErr = "DD - DDERR_OUTOFMEMORY";         break;
    case DDERR_UNSUPPORTED:             pStrErr = "DD - UNSUPPORTED";               break;
    case DDERR_GENERIC:                 pStrErr = "DD - GENERIC";                   break;
    }

    char Dest[224] = {};
    if (!pStrErr)
    {
        sprintf(Dest, "Code Err=%i", hrErr);
        pStrErr = Dest;
    }

    if (hrErr != S_OK)
    {
        for (int i = 0; i < 5; ++i)
        {
            printf("!edq %s !  %s\n", errMsg, pStrErr);
        }
    }
}
MSG_FUNC_IMPLEX(0x00422D40, PrintDDError, RENDERER_IMPL);
