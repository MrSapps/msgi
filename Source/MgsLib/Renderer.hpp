#pragma once

#include "MgsFunction.hpp"
#include "Psx.hpp"

#define DIRECTDRAW_VERSION 0x700
#include <ddraw.h>

#define OT_END_TAG 0xFFFFFF

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
MGS_ASSERT_SIZEOF(MGSVertex, 0x20);

struct texture_struct
{
    IDirectDrawSurface7* mSurface;
    WORD field_4;
    WORD field_6;
    WORD field_8;
    WORD field_A;
    WORD field_C;
    WORD field_E;
    WORD field_10_x;
    WORD field_12_y;
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
MGS_ASSERT_SIZEOF(texture_struct, 0x50);

struct prim_struct
{
    DWORD field_0;
    WORD nTextureIndex;
    WORD nBlendMode;
    DWORD mShadeMode;
    DWORD mPrimTypeQ;
    DWORD dwVertexCount;
};
MGS_ASSERT_SIZEOF(prim_struct, 0x14);

MGS_VAR_EXTERN(DWORD, dword_6FC718);
MGS_VAR_EXTERN(DWORD, dword_688CD0);
MGS_VAR_EXTERN(DWORD, dword_688CD4);
MGS_VAR_EXTERN(DWORD, gSoftwareRendering);
MGS_VAR_EXTERN(float, gXRes);
MGS_VAR_EXTERN(LPDIRECTDRAWSURFACE7, gPrimarySurface_6FC734);
MGS_VAR_EXTERN(LPDIRECTDRAWSURFACE7, g_pBackBuffer_6FC738);
MGS_VAR_EXTERN(DWORD, gModX2);
MGS_ARY_EXTERN(texture_struct, 1500, gTextures_6C0F00);
MGS_VAR_EXTERN(prim_struct*, gPrimBuffer_dword_6C0EFC);
MGS_VAR_EXTERN(MGSVertex*, g_pMGSVertices_6FC780);
MGS_VAR_EXTERN(WORD*, g_pwTextureIndices);
MGS_VAR_EXTERN(DWORD, dword_6FC774);
MGS_VAR_EXTERN(WORD, gNumTextures_word_6FC78C);

void RendererCpp_ForceLink();

void CC Render_Loop_SetWinTitle_422210();
void CC Render_Scene_DispEnv_40DD00(DISPENV* pRect);
void CC PrintDDError(const char* errMsg, HRESULT hrErr);
__int16 CC Render_RestoreAll();
void CC Render_DrawIndex_401619(uint32_t a_nIndex);
int CC Renderer_ClearOTag(DWORD* ot, int otSize);
bool CC ClearDDSurfaceWhite_41E990();

EXTERN_MGS_FUNC_NOT_IMPL(0x422BC0, HRESULT __cdecl (unsigned int, signed int, int), Render_InitTextureStages_422BC0);
EXTERN_MGS_FUNC_NOT_IMPL(0x422A90, HRESULT __cdecl(signed int, int), Render_SetRenderState_422A90);
