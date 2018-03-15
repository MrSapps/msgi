#pragma once

#include "MgsFunction.hpp"
#include "Psx.hpp"

#define DIRECTDRAW_VERSION 0x700
#include <ddraw.h>
#define DIRECT3D_VERSION 0x700
#include "d3d.h"

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
    WORD field_4_y;
    WORD field_6_x;
    WORD field_8_w;
    WORD field_A_h;
    WORD field_C;
    WORD field_E;
    WORD field_10_x;
    WORD field_12_y;
    float float_field_14_uQ;
    float float_field_18_vQ;
    DWORD mSurfaceType;
    DWORD field_20;
    DWORD field_24_flagsQ;
    DWORD field_28_surf_type;
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
MGS_VAR_EXTERN(WORD*, g_pwTextureIndices_6FC72C);
MGS_VAR_EXTERN(DWORD, dword_6FC774);
MGS_VAR_EXTERN(WORD, gNumTextures_word_6FC78C);
MGS_VAR_EXTERN(WORD, g_Render_sub_41C640_ret_650D1A);
MGS_VAR_EXTERN(DWORD *, gImageBufer_dword_6FC728);

void RendererCpp_ForceLink();

void CC Render_Loop_SetWinTitle_422210();
void CC Render_Scene_DispEnv_40DD00(DISPENV* pRect);
void CC PrintDDError(const char* errMsg, HRESULT hrErr);
__int16 CC Render_RestoreAll();
void CC Render_DrawIndex_401619(uint32_t a_nIndex);
int CC Renderer_ClearOTag(DWORD* ot, int otSize);
bool CC ClearDDSurfaceWhite_41E990();
HRESULT CC Render_SetRenderState_422A90(D3DRENDERSTATETYPE renderState, DWORD bEnable);
HRESULT CC Render_InitTextureStages_422BC0(unsigned int stage, D3DTEXTURESTAGESTATETYPE type2, DWORD v);
signed int CC Render_TextureScratchAlloc_41CA80();
signed int CC Render_sub_41D1D0();
bool CC Render_sub_41E730();
void CC Render_Restore_Lost_Surfaces_51E086();
int CC Render_sub_41C640(const PSX_RECT* pRect, const WORD* pallete, const BYTE* pixelData, int surfaceType, const BYTE* pTga, unsigned __int16 tga6, unsigned __int16 tga7);
void CC Render_sub_41C6B0(const PSX_RECT* pRect, const BYTE* pPixelData);
int jImageMove_401A31();
int ImageMove_401A13();
