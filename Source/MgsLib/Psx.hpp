#pragma once

#include "MgsFunction.hpp"
#include "types.hpp"

void PsxCpp_ForceLink();

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

MGS_VAR_EXTERN(PSX_RECT, gClipRect_6BECF0);
MGS_VAR_EXTERN(DRAWENV, gDrawEnv_6C0E98);

signed int CC Resetgraph_AndPrintPsxStructureSizes(int mode);
int CC SetGraphDebug(int debug);
void CC SetDispMask(int mask);
int CC ClearImage(PSX_RECT* pRect, BYTE r, BYTE g, BYTE b);
int CC Psx_OpenEvent(int desc, int spec, int mode, int func); // Psx_ to avoid windows.h name clash
