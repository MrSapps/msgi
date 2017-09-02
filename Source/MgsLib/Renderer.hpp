#pragma once

#include "MgsFunction.hpp"

#define DIRECTDRAW_VERSION 0x700
#include <ddraw.h>

struct Rect16
{
    WORD x1, y1, x2, y2;
};
MSG_ASSERT_SIZEOF(Rect16, 8);

struct DR_ENV
{
    DWORD tag;
    DWORD code[15];
};
MSG_ASSERT_SIZEOF(DR_ENV, 0x40);

struct DRAWENV
{
    Rect16 clip;
    WORD offx;
    WORD offy;
    Rect16 textureWindow;
    BYTE texturePage;
    BYTE dtd;
    BYTE dfe;
    BYTE isbg;
    BYTE r0;
    BYTE g0;
    BYTE b0;
    DR_ENV dr_env;
};
MSG_ASSERT_SIZEOF(DRAWENV, 0x5C);

struct MGSVertex
{
    float x;
    float y;
    float z;
    float w;
    DWORD diffuse;
    DWORD specular;
    float u;
    float v;
};
MSG_ASSERT_SIZEOF(MGSVertex, 0x20);

struct texture_struct
{
    IDirectDrawSurface7* mSurface;
    WORD field_4;
    WORD field_6;
    WORD field_8;
    WORD field_A;
    WORD field_C;
    WORD field_E;
    WORD field_10;
    WORD field_12;
    DWORD float_field_14;
    DWORD float_field_18;
    DWORD mSurfaceType;
    DWORD field_20;
    DWORD field_24;
    DWORD field_28;
    DWORD field_2C;
    DWORD field_30;
    DWORD field_34;
    DWORD field_38;
    DWORD field_3C;
    DWORD field_40;
    DWORD field_44;
    DWORD field_48;
    DWORD field_4C;
};
MSG_ASSERT_SIZEOF(texture_struct, 0x50);

struct prim_struct
{
    DWORD field_0;
    WORD nTextureIndex;
    WORD nBlendMode;
    DWORD mShadeMode;
    DWORD mPrimTypeQ;
    DWORD dwVertexCount;
};
MSG_ASSERT_SIZEOF(prim_struct, 0x14);

MGS_VAR_EXTERN(DWORD, dword_6FC718);
MGS_VAR_EXTERN(DWORD, dword_688CD0);
MGS_VAR_EXTERN(DWORD, dword_688CD4);
MGS_VAR_EXTERN(DWORD, gSoftwareRendering);
MGS_VAR_EXTERN(float, gXRes);
MGS_VAR_EXTERN(LPDIRECTDRAWSURFACE7, g_pPrimarySurface);
MGS_VAR_EXTERN(LPDIRECTDRAWSURFACE7, g_pBackBuffer);
MGS_VAR_EXTERN(DWORD, gModX2);
MGS_ARY_EXTERN(texture_struct, 1500, gTextures_dword_6C0F00);
MGS_VAR_EXTERN(prim_struct*, gPrimStructArray);
MGS_VAR_EXTERN(MGSVertex*, g_pMGSVertices);
MGS_VAR_EXTERN(WORD*, g_pwTextureIndices);
MGS_VAR_EXTERN(DWORD, dword_6FC774);
MGS_VAR_EXTERN(WORD, g_NumTextures);
MGS_VAR_EXTERN(Rect16, gClipRect_6BECF0);

void RendererCpp_ForceLink();
void CC PrintDDError(const char* errMsg, HRESULT hrErr);
__int16 CC Render_RestoreAll();
void CC Render_DrawIndex(uint32_t a_nIndex);
