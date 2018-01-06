#pragma once

#include "MgsFunction.hpp"
#include "types.hpp"

void PsxCpp_ForceLink();
void DoPsxTests();

struct P_TAG
{
    unsigned addr : 24;
    unsigned len : 8; // Tagged pointer bits?
    BYTE r0, g0, b0, code;
};
MGS_ASSERT_SIZEOF(P_TAG, 8);

struct P_CODE
{
    BYTE r0, g0, b0, code;
};
MGS_ASSERT_SIZEOF(P_CODE, 4);

struct DR_MODE
{
    DWORD tag;     // Pointer to the next primitive in primitive list
    DWORD code[2];  // New drawing environment information as specified by SetDrawMode()
};
MGS_ASSERT_SIZEOF(DR_MODE, 12);

struct DR_TPAGE
{
    DWORD tag;     // Pointer to the next primitive in primitive list(reserved)
    DWORD code;  // New texture page information specified by SetDrawTPage()
};
MGS_ASSERT_SIZEOF(DR_TPAGE, 8);

struct TILE
{
    DWORD tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
    short w, h;
};
MGS_ASSERT_SIZEOF(TILE, 16);

#define setlen( p, _len) (((P_TAG *)(p))->len  = (u_char)(_len))
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (u_long)(_addr))
#define getaddr(p)   (u_long)(((P_TAG *)(p))->addr)
#define setcode(p, _code) (((P_TAG *)(p))->code = (u_char)(_code))
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
#define termPrim(p) setaddr(p, 0xffffffff)

#define setRGB0(p,_r0,_g0,_b0) (p)->r0 = _r0,(p)->g0 = _g0,(p)->b0 = _b0
#define setPolyFT4(p) setlen(p, 9),  setcode(p, 0x2c)
#define setPolyG4(p)  setlen(p, 8),  setcode(p, 0x38)
#define setTile(p)    setlen(p, 3),  setcode(p, 0x60)

#define _get_mode(dfe, dtd, tpage) \
        ((0xe1000000)|((dtd)?0x0200:0)| \
        ((dfe)?0x0400:0)|((tpage)&0x9ff))

#define setDrawTPage(p, dfe, dtd, tpage) \
    setlen(p, 1), \
    ((unsigned int *)(p))[1] = _get_mode(dfe, dtd, tpage)

struct PSX_RECT // Should be called RECT but will clash with windows.h for now
{
    s16 x1, y1, x2, y2;
};
MGS_ASSERT_SIZEOF(PSX_RECT, 8);

struct DR_ENV
{
    DWORD tag;
    DWORD code[15];
};
MGS_ASSERT_SIZEOF(DR_ENV, 0x40);

struct DRAWENV
{
    PSX_RECT clip;
    WORD offx;
    WORD offy;
    PSX_RECT textureWindow;
    BYTE texturePage;
    BYTE dtd;
    BYTE dfe;
    BYTE isbg;
    BYTE r0;
    BYTE g0;
    BYTE b0;
    DR_ENV dr_env;
};
MGS_ASSERT_SIZEOF(DRAWENV, 0x5C);

struct POLY_FT4
{
    DWORD   tag;
    BYTE    r0, g0, b0, code;
    short   x0, y0;
    BYTE    u0, v0;
    WORD    clut;
    short   x1, y1;
    BYTE    u1, v1;
    WORD    tpage;
    short   x2, y2;
    BYTE    u2, v2;
    WORD    pad1;
    short   x3, y3;
    BYTE    u3, v3;
    WORD    pad2;
};
MGS_ASSERT_SIZEOF(POLY_FT4, 40);

struct POLY_F4
{
    DWORD tag;      // Pointer to the next primitive
    BYTE r0, g0, b0; // RGB color values
    BYTE code;       // Primitive ID(reserved)
    short x0, y0;    // Vertex coordinates 
    short x1, y1;    // Vertex coordinates 
    short x2, y2;    // Vertex coordinates 
    short x3, y3;    // Vertex coordinates
};
MGS_ASSERT_SIZEOF(POLY_F4, 0x18);


struct POLY_G4
{
    DWORD   tag;
    BYTE    r0, g0, b0, code;
    short   x0, y0;
    BYTE    r1, g1, b1, pad1;
    short   x1, y1;
    BYTE    r2, g2, b2, pad2;
    short   x2, y2;
    BYTE    r3, g3, b3, pad3;
    short   x3, y3;
};
MGS_ASSERT_SIZEOF(POLY_G4, 36);

struct DISPENV
{
    PSX_RECT disp;   // Display area within frame buffer.Width: 256, 320, 384, 512, or 640. Height : 240 or 480.
    PSX_RECT screen; // Output screen display area.It is calculated without regard to the value of
                     // disp, using the standard monitor screen upper left - hand point(0, 0) and
                     // lower right - hand point(256, 240).
    BYTE isinter;    // Interlace mode flag. 0: non - interlace; 1: interlace
    BYTE isrgb24;    // 24 - bit mode flag. 0: 16 - bit mode; 1: 24 - bit mode
    BYTE pad0, pad1; // Reserved by system
};
MGS_ASSERT_SIZEOF(DISPENV, 20);

struct VECTOR
{
    int field_0_x;
    int field_4_y;
    int field_8_z;
    int padding;
};
MGS_ASSERT_SIZEOF(VECTOR, 0x10);

struct SVECTOR
{
    short int field_0_x;
    short int field_2_y;
    short int field_4_z;
    short int padding;
};
MGS_ASSERT_SIZEOF(SVECTOR, 0x8);


struct GTE_Data
{
    int gte_nop_count_722688;
    int gte_ncs_count_72268C;
    int gte_gpf0_count_722690;
    int gte_rtv0_count_722694;
    int gte_sqr0_count_722698;
    int gte_gpl0_count_72269C;
    int gte_gpl12_count_7226A0;
    int gte_nclip_count_7226A4;
    int gte_rtv2tr_count_7226A8;
    int gte_RT1_count_7226AC;
    int gte_rt_count_7226B0;
    int gte_LT1_count_7226B4;
    int gte_rtv1_count_7226B8;
    int gte_llv0_count_7226BC;
    int gte_op12_count_7226C0;
    int gte_ll_count_7226C4;
    int gte_op0_count_7226C8;
    int gte_cdp_count_7226CC;
    int gte_rtpt_count_7226D0;
    int gte_RT1TR_count_7226D4;
    int gte_rtv1tr_count_7226D8;
    int gte_nccs_count_7226DC;
    int gte_rtir_count_7226E0;
    int gte_dpcl_count_7226E4;
    int gte_rtv2_count_7226E8;
    int gte_gpf12_count_7226EC;
    int gte_rtps_count_7226F0;
    int gte_nct_count_7226F4;
    int gte_intpl_count_7226F8;
    int gte_rtv0tr_count_7226FC;
    int gte_sqr12_count_722700;
    int gte_dpcs_count_722704;
    int gte_cc_count_722708;
};
MGS_ASSERT_SIZEOF(GTE_Data, 0x84);

MGS_VAR_EXTERN(DISPENV, gDispEnv_6BECF0);
MGS_VAR_EXTERN(DRAWENV, gDrawEnv_6C0E98);
MGS_VAR_EXTERN(GTE_Data, gGteData_722688);

signed int CC Resetgraph_AndPrintPsxStructureSizes(int mode);
int CC SetGraphDebug(int debug);
void CC SetDispMask(int mask);
int CC ClearImage(PSX_RECT* pRect, BYTE r, BYTE g, BYTE b);
int CC Psx_OpenEvent(void* desc, int spec, int mode, void* func); // Psx_ to avoid windows.h name clash
void CC PsxGpuDebug_44A4D0();
DRAWENV* CC Renderer_DRAWENV_Init_401888(DRAWENV* pDrawEnv, __int16 clipX1, __int16 clipY1, __int16 clipX2, __int16 clipY2);
void CC SetDrawEnv_40DDE0(DR_ENV* pPacked, DRAWENV* drawEnv);;
