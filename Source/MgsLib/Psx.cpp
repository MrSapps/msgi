#include "stdafx.h"
#include "Psx.hpp"

void PsxCpp_ForceLink() { }

#define IMPL_PSX true

MGS_VAR(1, 0x6C0E98, DRAWENV, gDrawEnv_6C0E98, {});
MGS_VAR(1, 0x6BECF0, PSX_RECT, gClipRect_6BECF0, {});
MGS_VAR(1, 0x6C0EAA, WORD, word_6C0EAA, 0x0);
MGS_VAR(1, 0x6C0EA8, WORD, word_6C0EA8, 0x0);

struct POLY_FT3
{
    DWORD* tag;      // Pointer to the next primitive
    BYTE r0, g0, b0; // RGB color values
    BYTE code;       // Primitive ID(reserved)
    
    short x0, y0;    // Vertex coordinates 
    BYTE u0, v0;    // Texture coordinates
    WORD clut;       // CLUT ID(color - look - up table for 4 - bit / 8 - bit mode only)

    short x1, y1;    // Vertex coordinates
    BYTE u1, v1;     // Texture coordinates
    WORD tpage;      // Texture page ID

    short x2, y2;    // Vertex coordinates
    BYTE u2, v2;     // Texture coordinates
    WORD pad1;       // Reserved by the system
};
MSG_ASSERT_SIZEOF(POLY_FT3, 32);

struct POLY_F3 
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
};
MSG_ASSERT_SIZEOF(POLY_F3, 20);

struct POLY_F4
{
    DWORD* tag;      // Pointer to the next primitive
    BYTE r0, g0, b0; // RGB color values
    BYTE code;       // Primitive ID(reserved)
    short x0, y0;    // Vertex coordinates 
    short x1, y1;    // Vertex coordinates 
    short x2, y2;    // Vertex coordinates 
    short x3, y3;    // Vertex coordinates
};
MSG_ASSERT_SIZEOF(POLY_F4, 0x18);

struct RECT32
{
    DWORD x1, y1, x2, y2;
};
MSG_ASSERT_SIZEOF(RECT32, 16);


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
MSG_ASSERT_SIZEOF(DISPENV, 20);

struct P_TAG
{
    unsigned addr : 24;
    unsigned len : 8; // Tagged pointer bits?
    BYTE r0, g0, b0, code;
};
MSG_ASSERT_SIZEOF(P_TAG, 8);

struct P_CODE
{
    BYTE r0, g0, b0, code;
};
MSG_ASSERT_SIZEOF(P_CODE, 4);

struct DR_STP 
{
    DWORD* tag;     // Pointer to the next primitive in primitive list(reserved)
    DWORD code[2];  // Primitive ID
};
MSG_ASSERT_SIZEOF(DR_STP, 12);

struct DR_TPAGE
{
    DWORD* tag;     // Pointer to the next primitive in primitive list(reserved)
    DWORD code[2];  // New texture page information specified by SetDrawTPage()
};
MSG_ASSERT_SIZEOF(DR_TPAGE, 12);

struct DR_MODE
{
    DWORD* tag;     // Pointer to the next primitive in primitive list
    DWORD code[2];  // New drawing environment information as specified by SetDrawMode()
};
MSG_ASSERT_SIZEOF(DR_MODE, 12);

struct DR_TWIN
{
    DWORD* tag;     // Pointer to the next primitive in primitive list
    DWORD code[2];  // New texture window information specified by SetDrawTexWindow()
};
MSG_ASSERT_SIZEOF(DR_TWIN, 12);

struct DR_AREA
{
    DWORD* tag;     // Pointer to the next primitive in primitive list
    DWORD code[2];  // New drawing area information specified by SetDrawArea()
};
MSG_ASSERT_SIZEOF(DR_AREA, 12);

struct DR_OFFSET
{
    DWORD* tag;     // Pointer to the next primitive in primitive list
    DWORD code[2];  // New drawing offset information specified by SetDrawOffset()
};
MSG_ASSERT_SIZEOF(DR_OFFSET, 12);

struct DR_MOVE
{
    DWORD* tag;     // Pointer to the next primitive in primitive list
    DWORD code[5];  // Primitive ID
};
MSG_ASSERT_SIZEOF(DR_MOVE, 24);

struct SPRT
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
    BYTE u0, v0;
    WORD clut;
    short w, h; // Custom sized sprite
};
MSG_ASSERT_SIZEOF(SPRT, 20);

struct SPRT_16  // 16x16 sprite
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
    BYTE u0, v0;
    WORD clut;
};
MSG_ASSERT_SIZEOF(SPRT_16, 16);

struct SPRT_8  // 8x8 sprite
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
    BYTE u0, v0;
    WORD clut;
};
MSG_ASSERT_SIZEOF(SPRT_8, 16);

struct TILE
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
    short w, h;
};
MSG_ASSERT_SIZEOF(TILE, 16);

struct TILE_16
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
};
MSG_ASSERT_SIZEOF(TILE_16, 12);

struct TILE_8
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
};
MSG_ASSERT_SIZEOF(TILE_8, 12);

struct TILE_1
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
};
MSG_ASSERT_SIZEOF(TILE_1, 12);

struct DR_LOAD
{
    DWORD* tag;     // Pointer to next primitive(reserved)
    DWORD code[3];  // Primitive ID
    DWORD p[13];    // Transfer data
};
MSG_ASSERT_SIZEOF(DR_LOAD, 68);

struct LINE_F2
{
    DWORD*  tag;
    BYTE    r0, g0, b0, code;
    short   x0, y0;
    short   x1, y1;
};
MSG_ASSERT_SIZEOF(LINE_F2, 16);

struct LINE_G2
{
    DWORD*  tag;
    BYTE    r0, g0, b0, code;
    short   x0, y0;
    BYTE    r1, g1, b1, p1;
    short   x1, y1;
};
MSG_ASSERT_SIZEOF(LINE_G2, 20);

struct LINE_F3
{
    DWORD*  tag;
    BYTE    r0, g0, b0, code;
    short   x0, y0;
    short   x1, y1;
    short   x2, y2;
    DWORD   pad;
};
MSG_ASSERT_SIZEOF(LINE_F3, 24);

struct LINE_G3 
{
    DWORD*  tag;
    BYTE    r0, g0, b0, code;
    short   x0, y0;
    BYTE    r1, g1, b1, p1;
    short   x1, y1;
    BYTE    r2, g2, b2, p2;
    short   x2, y2;
    DWORD   pad;
};
MSG_ASSERT_SIZEOF(LINE_G3, 32);

struct LINE_F4
{
    DWORD*  tag;
    BYTE    r0, g0, b0, code;
    short   x0, y0;
    short   x1, y1;
    short   x2, y2;
    short   x3, y3;
    DWORD   pad;
};
MSG_ASSERT_SIZEOF(LINE_F4, 28);

struct LINE_G4 
{
    DWORD*  tag;
    BYTE    r0, g0, b0, code;
    short   x0, y0;
    BYTE    r1, g1, b1, p1;
    short   x1, y1;
    BYTE    r2, g2, b2, p2;
    short   x2, y2;
    BYTE    r3, g3, b3, p3;
    short   x3, y3;
    BYTE    pad;
};
MSG_ASSERT_SIZEOF(LINE_G4, 40);

struct POLY_FT4
{
    DWORD*  tag;
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
MSG_ASSERT_SIZEOF(POLY_FT4, 40);

struct POLY_G3 
{
    DWORD*  tag;
    BYTE    r0, g0, b0, code;
    short   x0, y0;
    BYTE    r1, g1, b1, pad1;
    short   x1, y1;
    BYTE    r2, g2, b2, pad2;
    short   x2, y2;
};
MSG_ASSERT_SIZEOF(POLY_G3, 28);

struct POLY_G4
{
    DWORD*  tag;
    BYTE    r0, g0, b0, code;
    short   x0, y0;
    BYTE    r1, g1, b1, pad1;
    short   x1, y1;
    BYTE    r2, g2, b2, pad2;
    short   x2, y2;
    BYTE    r3, g3, b3, pad3;
    short   x3, y3;
};
MSG_ASSERT_SIZEOF(POLY_G4, 36);

struct POLY_GT3 
{
    DWORD*  tag;
    BYTE    r0, g0, b0, code;
    short   x0, y0;
    BYTE    u0, v0;
    WORD    clut;
    BYTE    r1, g1, b1, p1;
    short   x1, y1;
    BYTE    u1, v1;
    WORD    tpage;
    BYTE    r2, g2, b2, p2;
    short   x2, y2;
    BYTE    u2, v2;
    WORD    pad2;
};
MSG_ASSERT_SIZEOF(POLY_GT3, 40);

struct POLY_GT4 
{
    DWORD*  tag;
    BYTE    r0, g0, b0, code;
    short   x0, y0;
    BYTE    u0, v0;
    WORD    clut;
    BYTE    r1, g1, b1, p1;
    short   x1, y1;
    BYTE    u1, v1;
    WORD    tpage;
    BYTE    r2, g2, b2, p2;
    short   x2, y2;
    BYTE    u2, v2;
    WORD    pad2;
    BYTE    r3, g3, b3, p3;
    short   x3, y3;
    BYTE    u3, v3;
    WORD    pad3;
};
MSG_ASSERT_SIZEOF(POLY_GT4, 52);

#define PRINT_SIZE(sturcture, name)   printf("sizeof( %10.10s ):\t%2d(%2X), %2d(%2X) longs\n", name, sizeof(sturcture), sizeof(sturcture), sizeof(sturcture) / sizeof(DWORD), sizeof(sturcture) / sizeof(DWORD))

signed int CC Resetgraph_AndPrintPsxStructureSizes(int mode)
{
    printf(".Resetgraph(%d)\n", mode);
    PRINT_SIZE(PSX_RECT, "RECT");
    PRINT_SIZE(RECT32, "RECT32");
    PRINT_SIZE(DR_ENV, "DR_ENV");
    PRINT_SIZE(DRAWENV, "DRAWENV");
    PRINT_SIZE(DISPENV, "DISPENV");
    PRINT_SIZE(P_TAG, "P_TAG");
    PRINT_SIZE(P_CODE, "P_CODE");
    PRINT_SIZE(POLY_F3, "POLY_F3");
    PRINT_SIZE(POLY_F4, "POLY_F4");
    PRINT_SIZE(POLY_FT3, "POLY_FT3");
    PRINT_SIZE(POLY_FT4, "POLY_FT4");
    PRINT_SIZE(POLY_G3, "POLY_G3");
    PRINT_SIZE(POLY_G4, "POLY_G4");
    PRINT_SIZE(POLY_GT3, "POLY_GT3");
    PRINT_SIZE(POLY_FT3, "POLY_FT3");
    PRINT_SIZE(POLY_GT4, "POLY_GT4");
    PRINT_SIZE(LINE_F2, "LINE_F2");
    PRINT_SIZE(LINE_G2, "LINE_G2");
    PRINT_SIZE(LINE_F3, "LINE_F3");
    PRINT_SIZE(LINE_G3, "LINE_G3");
    PRINT_SIZE(LINE_F4, "LINE_F4");
    PRINT_SIZE(LINE_G4, "LINE_G4");
    PRINT_SIZE(SPRT, "SPRT");
    PRINT_SIZE(SPRT_16, "SPRT_16");
    PRINT_SIZE(SPRT_8, "SPRT_8");
    PRINT_SIZE(TILE, "TILE");
    PRINT_SIZE(TILE_16, "TILE_16");
    PRINT_SIZE(TILE_8, "TILE_8");
    PRINT_SIZE(TILE_1, "TILE_1");
    PRINT_SIZE(DR_MODE, "DR_MODE");
    PRINT_SIZE(DR_TWIN, "DR_TWIN");
    PRINT_SIZE(DR_AREA, "DR_AREA");
    PRINT_SIZE(DR_OFFSET, "DR_OFFSET");
    PRINT_SIZE(DR_MOVE, "DR_MOVE");
    PRINT_SIZE(DR_LOAD, "DR_LOAD");
    PRINT_SIZE(DR_TPAGE, "DR_TPAGE");
    PRINT_SIZE(DR_STP, "DR_STP");
    return 1;
}
MSG_FUNC_IMPLEX(0x0044A7B0, Resetgraph_AndPrintPsxStructureSizes, IMPL_PSX);

int CC SetGraphDebug(int debug)
{
    printf(".SetGraphDebug(%d)\n", debug);
    return 0;
}
MSG_FUNC_IMPLEX(0x0044AB30, SetGraphDebug, IMPL_PSX);

void CC SetDispMask(int mask)
{
    printf(".SetDispMask(%d)\n", mask);
}
MSG_FUNC_IMPLEX(0x0044AC40, SetDispMask, IMPL_PSX);

int CC ClearImage(PSX_RECT* pRect, BYTE r, BYTE g, BYTE b)
{
    printf(".ClearImage((%d,%d,%d,%d),r=%d,g=%d,b=%d)\n", pRect->x1, pRect->y1, pRect->x2, pRect->y2, r, g, b);
    return 0;
}
MSG_FUNC_IMPLEX(0x0044ABE0, ClearImage, IMPL_PSX);

int CC Psx_OpenEvent(int desc, int spec, int mode, int func)
{
    printf("OpenEvent(%p,%d,%d,%p)\n", desc, spec, mode, func);
    return 0;
}
MSG_FUNC_IMPLEX(0x0044CEC0, Psx_OpenEvent, IMPL_PSX);

int CC EnableEvent(int pEvent)
{
    printf(".EnableEvent(%d)\n", pEvent);
    return 0;
}
MSG_FUNC_IMPLEX(0x0044CF10, EnableEvent, IMPL_PSX);

int CC CloseEvent(int pEvent)
{
    printf(".CloseEvent(%p)\n", pEvent);
    return 0;
}
MSG_FUNC_IMPLEX(0x0044CEF0, CloseEvent, IMPL_PSX);

DRAWENV* CC Renderer_Set_DRAWENV_40DD90(DRAWENV* pDrawEnv)
{
    memcpy(&gDrawEnv_6C0E98, pDrawEnv, sizeof(DRAWENV));

    if (!word_6C0EAA)
    {
        word_6C0EAA = 255;
    }

    if (!word_6C0EA8)
    {
        word_6C0EA8 = 255;
    }

    return &gDrawEnv_6C0E98;
}
MSG_FUNC_IMPLEX(0x40DD90, Renderer_Set_DRAWENV_40DD90, IMPL_PSX);

DRAWENV* CC Renderer_DRAWENV_Init_401888(DRAWENV* pDrawEnv, __int16 clipX1, __int16 clipY1, __int16 clipX2, __int16 clipY2)
{
    pDrawEnv->clip.x1 = clipX1;
    pDrawEnv->clip.x2 = clipX2;
    pDrawEnv->offx = clipX1;
    pDrawEnv->clip.y2 = clipY2;
    pDrawEnv->clip.y1 = clipY1;
    pDrawEnv->offy = clipY1;
    pDrawEnv->dtd = 1;
    pDrawEnv->dfe = 0;
    pDrawEnv->textureWindow.x1 = 0;
    pDrawEnv->textureWindow.y1 = 0;
    pDrawEnv->textureWindow.x2 = 0;
    pDrawEnv->textureWindow.y2 = 0;
    pDrawEnv->r0 = 0;
    pDrawEnv->g0 = 0;
    pDrawEnv->b0 = 0;
    pDrawEnv->texturePage = 0;
    pDrawEnv->isbg = 0;
    return pDrawEnv;
}
MSG_FUNC_IMPLEX(0x401888, Renderer_DRAWENV_Init_401888, IMPL_PSX);

DRAWENV* Renderer_Init_DRAWENV_40200D()
{
    DRAWENV drawEnv;
    Renderer_DRAWENV_Init_401888(
        &drawEnv,
        gClipRect_6BECF0.x1,
        gClipRect_6BECF0.y1,
        gClipRect_6BECF0.x2,
        gClipRect_6BECF0.y2);
    return Renderer_Set_DRAWENV_40DD90(&drawEnv);
}
MSG_FUNC_IMPLEX(0x40200D, Renderer_Init_DRAWENV_40200D, IMPL_PSX);
