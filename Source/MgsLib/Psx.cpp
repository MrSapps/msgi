#include "stdafx.h"
#include "Psx.hpp"

void PsxCpp_ForceLink() { }

#define IMPL_PSX true

MGS_VAR(1, 0x6C0E98, DRAWENV, gDrawEnv_6C0E98, {});
MGS_VAR(1, 0x6BECF0, DISPENV, gDispEnv_6BECF0, {});
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
MGS_ASSERT_SIZEOF(POLY_FT3, 32);

struct POLY_F3 
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
};
MGS_ASSERT_SIZEOF(POLY_F3, 20);

struct RECT32
{
    DWORD x1, y1, x2, y2;
};
MGS_ASSERT_SIZEOF(RECT32, 16);

struct P_CODE
{
    BYTE r0, g0, b0, code;
};
MGS_ASSERT_SIZEOF(P_CODE, 4);

struct DR_STP 
{
    DWORD* tag;     // Pointer to the next primitive in primitive list(reserved)
    DWORD code[2];  // Primitive ID
};
MGS_ASSERT_SIZEOF(DR_STP, 12);

struct DR_TPAGE
{
    DWORD* tag;     // Pointer to the next primitive in primitive list(reserved)
    DWORD code[2];  // New texture page information specified by SetDrawTPage()
};
MGS_ASSERT_SIZEOF(DR_TPAGE, 12);

struct DR_MODE
{
    DWORD* tag;     // Pointer to the next primitive in primitive list
    DWORD code[2];  // New drawing environment information as specified by SetDrawMode()
};
MGS_ASSERT_SIZEOF(DR_MODE, 12);

struct DR_TWIN
{
    DWORD* tag;     // Pointer to the next primitive in primitive list
    DWORD code[2];  // New texture window information specified by SetDrawTexWindow()
};
MGS_ASSERT_SIZEOF(DR_TWIN, 12);

struct DR_AREA
{
    DWORD* tag;     // Pointer to the next primitive in primitive list
    DWORD code[2];  // New drawing area information specified by SetDrawArea()
};
MGS_ASSERT_SIZEOF(DR_AREA, 12);

struct DR_OFFSET
{
    DWORD* tag;     // Pointer to the next primitive in primitive list
    DWORD code[2];  // New drawing offset information specified by SetDrawOffset()
};
MGS_ASSERT_SIZEOF(DR_OFFSET, 12);

struct DR_MOVE
{
    DWORD* tag;     // Pointer to the next primitive in primitive list
    DWORD code[5];  // Primitive ID
};
MGS_ASSERT_SIZEOF(DR_MOVE, 24);

struct SPRT
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
    BYTE u0, v0;
    WORD clut;
    short w, h; // Custom sized sprite
};
MGS_ASSERT_SIZEOF(SPRT, 20);

struct SPRT_16  // 16x16 sprite
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
    BYTE u0, v0;
    WORD clut;
};
MGS_ASSERT_SIZEOF(SPRT_16, 16);

struct SPRT_8  // 8x8 sprite
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
    BYTE u0, v0;
    WORD clut;
};
MGS_ASSERT_SIZEOF(SPRT_8, 16);

struct TILE
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
    short w, h;
};
MGS_ASSERT_SIZEOF(TILE, 16);

struct TILE_16
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
};
MGS_ASSERT_SIZEOF(TILE_16, 12);

struct TILE_8
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
};
MGS_ASSERT_SIZEOF(TILE_8, 12);

struct TILE_1
{
    DWORD* tag;
    BYTE r0, g0, b0, code;
    short x0, y0;
};
MGS_ASSERT_SIZEOF(TILE_1, 12);

struct DR_LOAD
{
    DWORD* tag;     // Pointer to next primitive(reserved)
    DWORD code[3];  // Primitive ID
    DWORD p[13];    // Transfer data
};
MGS_ASSERT_SIZEOF(DR_LOAD, 68);

struct LINE_F2
{
    DWORD*  tag;
    BYTE    r0, g0, b0, code;
    short   x0, y0;
    short   x1, y1;
};
MGS_ASSERT_SIZEOF(LINE_F2, 16);

struct LINE_G2
{
    DWORD*  tag;
    BYTE    r0, g0, b0, code;
    short   x0, y0;
    BYTE    r1, g1, b1, p1;
    short   x1, y1;
};
MGS_ASSERT_SIZEOF(LINE_G2, 20);

struct LINE_F3
{
    DWORD*  tag;
    BYTE    r0, g0, b0, code;
    short   x0, y0;
    short   x1, y1;
    short   x2, y2;
    DWORD   pad;
};
MGS_ASSERT_SIZEOF(LINE_F3, 24);

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
MGS_ASSERT_SIZEOF(LINE_G3, 32);

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
MGS_ASSERT_SIZEOF(LINE_F4, 28);

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
MGS_ASSERT_SIZEOF(LINE_G4, 40);

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
MGS_ASSERT_SIZEOF(POLY_G3, 28);

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
MGS_ASSERT_SIZEOF(POLY_G4, 36);

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
MGS_ASSERT_SIZEOF(POLY_GT3, 40);

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
MGS_ASSERT_SIZEOF(POLY_GT4, 52);

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
MGS_FUNC_IMPLEX(0x0044A7B0, Resetgraph_AndPrintPsxStructureSizes, IMPL_PSX);

int CC SetGraphDebug(int debug)
{
    printf(".SetGraphDebug(%d)\n", debug);
    return 0;
}
MGS_FUNC_IMPLEX(0x0044AB30, SetGraphDebug, IMPL_PSX);

void CC SetDispMask(int mask)
{
    printf(".SetDispMask(%d)\n", mask);
}
MGS_FUNC_IMPLEX(0x0044AC40, SetDispMask, IMPL_PSX);

int CC ClearImage(PSX_RECT* pRect, BYTE r, BYTE g, BYTE b)
{
    printf(".ClearImage((%d,%d,%d,%d),r=%d,g=%d,b=%d)\n", pRect->x1, pRect->y1, pRect->x2, pRect->y2, r, g, b);
    return 0;
}
MGS_FUNC_IMPLEX(0x0044ABE0, ClearImage, IMPL_PSX);

int CC Psx_OpenEvent(int desc, int spec, int mode, int func)
{
    printf("OpenEvent(%p,%d,%d,%p)\n", desc, spec, mode, func);
    return 0;
}
MGS_FUNC_IMPLEX(0x0044CEC0, Psx_OpenEvent, IMPL_PSX);

int CC EnableEvent(int pEvent)
{
    printf(".EnableEvent(%d)\n", pEvent);
    return 0;
}
MGS_FUNC_IMPLEX(0x0044CF10, EnableEvent, IMPL_PSX);

int CC CloseEvent(int pEvent)
{
    printf(".CloseEvent(%p)\n", pEvent);
    return 0;
}
MGS_FUNC_IMPLEX(0x0044CEF0, CloseEvent, IMPL_PSX);

DRAWENV* CC Renderer_Set_DRAWENV_40DD90(DRAWENV* pDrawEnv)
{
    pDrawEnv->b0 = 255;

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
MGS_FUNC_IMPLEX(0x40DD90, Renderer_Set_DRAWENV_40DD90, IMPL_PSX);

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
    pDrawEnv->b0 = 255;
    pDrawEnv->texturePage = 0;
    pDrawEnv->isbg = 0;
    return pDrawEnv;
}
MGS_FUNC_IMPLEX(0x401888, Renderer_DRAWENV_Init_401888, IMPL_PSX);

DRAWENV* Renderer_Init_DRAWENV_40200D()
{
    DRAWENV drawEnv;
    Renderer_DRAWENV_Init_401888(
        &drawEnv,
        gDispEnv_6BECF0.disp.x1,
        gDispEnv_6BECF0.disp.y1,
        gDispEnv_6BECF0.disp.x2,
        gDispEnv_6BECF0.disp.y2);
    return Renderer_Set_DRAWENV_40DD90(&drawEnv);
}
MGS_FUNC_IMPLEX(0x40200D, Renderer_Init_DRAWENV_40200D, IMPL_PSX);

union Reg_SXY0
{
    struct Regs
    {
        short int SY0;
        short int SX0;
    } regs;
    int SXY0;
};

union Reg_SXY1
{
    struct Regs
    {
        short int SY1;
        short int SX1;
    } regs;
    int SXY1;
};


union Reg_SXY2
{
    struct Regs
    {
        short int SY2;
        short int SX2;
    } regs;
    int SXY2;
};

union Reg_VXY0
{
    struct Regs
    {
        short int VY0;
        short int VX0;
        short int VZ0;
        short int Zero;
    } regs;
    __int64 VXY0;
};


MGS_VAR(1, 0x993EC0, Reg_VXY0, gGte_VXY0_993EC0, {});


// Maybe more regs?
MGS_VAR(1, 0x993EE4, int, gGte_in1_dword_993EE4, 0);
MGS_VAR(1, 0x993EE8, int, gGte_in2_dword_993EE8, 0);
MGS_VAR(1, 0x993EEC, int, gGte_in3_dword_993EEC, 0);

MGS_VAR(1, 0x993EF0, Reg_SXY1, gGte_SXY1_993EF0, {});
MGS_VAR(1, 0x993EF4, Reg_SXY0, gGte_SXY0_993EF4, {});
MGS_VAR(1, 0x993EF8, Reg_SXY2, gGte_SXY2_993EF8, {});


MGS_VAR(1, 0x993F20, int, gGte_MAC0_993F20, 0);

// Could be MAC1-3?
MGS_VAR(1, 0x993F24, int, gGte_out1_dword_993F24, 0);
MGS_VAR(1, 0x993F28, int, gGte_out2_dword_993F28, 0);
MGS_VAR(1, 0x993F2C, int, gGte_out3_dword_993F2C, 0);


MGS_VAR(1, 0x722688, GTE_Data, gGteData_722688, {});

void CC Psx_gte_normal_clip_446E90()
{
    ++gGteData_722688.gte_nclip_count_7226A4;

    gGte_MAC0_993F20 =
        (gGte_SXY0_993EF4.regs.SX0 * gGte_SXY1_993EF0.regs.SY1) +
        (gGte_SXY1_993EF0.regs.SX1 * gGte_SXY2_993EF8.regs.SY2) +
        (gGte_SXY2_993EF8.regs.SX2 * gGte_SXY0_993EF4.regs.SY0) -
        (gGte_SXY0_993EF4.regs.SX0 * gGte_SXY2_993EF8.regs.SY2) -
        (gGte_SXY1_993EF0.regs.SX1 * gGte_SXY0_993EF4.regs.SY0) -
        (gGte_SXY2_993EF8.regs.SX2 * gGte_SXY1_993EF0.regs.SY1);
}
MGS_FUNC_IMPLEX(0x446E90, Psx_gte_normal_clip_446E90, IMPL_PSX);

void CC Psx_gte_nop_44A460()
{
    ++gGteData_722688.gte_nop_count_722688;
}
MGS_FUNC_IMPLEX(0x44A460, Psx_gte_nop_44A460, IMPL_PSX);

// This is never called by the game, but we can call it for debugging purposes

/*
Opcodes used by the Actor_Rank screen:
gte_rtpt_count:                 5640
gte_ncs_count:                    16
gte_RT1_count:                  3438
gte_RT1TR_count:                1146
gte_rtir_count:                 3438
gte_rt_count:                   1146
*/
void CC PsxGpuDebug_44A4D0()
{
    printf("%-21s %14d\n", "gte_rtps_count:", gGteData_722688.gte_rtps_count_7226F0);
    printf("%-21s %14d\n", "gte_rtpt_count:", gGteData_722688.gte_rtpt_count_7226D0);
    printf("%-21s %14d\n", "gte_nccs_count:", gGteData_722688.gte_nccs_count_7226DC);
    printf("%-21s %14d\n", "gte_dpcs_count:", gGteData_722688.gte_dpcs_count_722704);
    printf("%-21s %14d\n", "gte_ncs_count:", gGteData_722688.gte_ncs_count_72268C);
    printf("%-21s %14d\n", "gte_intpl_count:", gGteData_722688.gte_intpl_count_7226F8);
    printf("%-21s %14d\n", "gte_nclip_count:", gGteData_722688.gte_nclip_count_7226A4);
    printf("%-21s %14d\n", "gte_sqr0_count:", gGteData_722688.gte_sqr0_count_722698);
    printf("%-21s %14d\n", "gte_sqr12_count:", gGteData_722688.gte_sqr12_count_722700);
    printf("%-21s %14d\n", "gte_RT1_count:", gGteData_722688.gte_RT1_count_7226AC);
    printf("%-21s %14d\n", "gte_RT1TR_count:", gGteData_722688.gte_RT1TR_count_7226D4);
    printf("%-21s %14d\n", "gte_LT1_count:", gGteData_722688.gte_LT1_count_7226B4);
    printf("%-21s %14d\n", "gte_rtv0_count:", gGteData_722688.gte_rtv0_count_722694);
    printf("%-21s %14d\n", "gte_rtv1_count:", gGteData_722688.gte_rtv1_count_7226B8);
    printf("%-21s %14d\n", "gte_rtv2_count:", gGteData_722688.gte_rtv2_count_7226E8);
    printf("%-21s %14d\n", "gte_rtir_count:", gGteData_722688.gte_rtir_count_7226E0);
    printf("%-21s %14d\n", "gte_llv0_count:", gGteData_722688.gte_llv0_count_7226BC);
    printf("%-21s %14d\n", "gte_ll_count:", gGteData_722688.gte_ll_count_7226C4);
    printf("%-21s %14d\n", "gte_rt_count:", gGteData_722688.gte_rt_count_7226B0);
    printf("%-21s %14d\n", "gte_rtv0tr_count:", gGteData_722688.gte_rtv0tr_count_7226FC);
    printf("%-21s %14d\n", "gte_rtv1tr_count:", gGteData_722688.gte_rtv1tr_count_7226D8);
    printf("%-21s %14d\n", "gte_rtv2tr_count:", gGteData_722688.gte_rtv2tr_count_7226A8);
    printf("%-21s %14d\n", "gte_cc_count:", gGteData_722688.gte_cc_count_722708);
    printf("%-21s %14d\n", "gte_cdp_count:", gGteData_722688.gte_cdp_count_7226CC);
    printf("%-21s %14d\n", "gte_dpcl_count:", gGteData_722688.gte_dpcl_count_7226E4);
    printf("%-21s %14d\n", "gte_gpf0_count:", gGteData_722688.gte_gpf0_count_722690);
    printf("%-21s %14d\n", "gte_gpf12_count:", gGteData_722688.gte_gpf12_count_7226EC);
    printf("%-21s %14d\n", "gte_gpl0_count:", gGteData_722688.gte_gpl0_count_72269C);
    printf("%-21s %14d\n", "gte_gpl12_count:", gGteData_722688.gte_gpl12_count_7226A0);
    printf("%-21s %14d\n", "gte_op0_count:", gGteData_722688.gte_op0_count_7226C8);
    printf("%-21s %14d\n", "gte_op12_count:", gGteData_722688.gte_op12_count_7226C0);
    printf("%-21s %14d\n", "gte_nct_count:", gGteData_722688.gte_nct_count_7226F4);
    printf("%-21s %14d\n", "gte_nop_count:", gGteData_722688.gte_nop_count_722688);
}
MGS_FUNC_IMPLEX(0x44A4D0, PsxGpuDebug_44A4D0, IMPL_PSX);

void CC VectorNormal_44CAE0(const VECTOR* pVec, VECTOR* pUnitVec)
{
    const long double value = 4096.0 / sqrt((double)
        pVec->field_0_x * pVec->field_0_x +
        pVec->field_4_y * pVec->field_4_y +
        pVec->field_8_z * pVec->field_8_z);

    pUnitVec->field_0_x = (signed int)((double)pVec->field_0_x * value);
    pUnitVec->field_4_y = (signed int)((double)pVec->field_4_y * value);
    pUnitVec->field_8_z = (signed int)((double)pVec->field_8_z * value);
}
MGS_FUNC_IMPLEX(0x44CAE0, VectorNormal_44CAE0, IMPL_PSX);

void CC VectorNormalS_44CB70(const VECTOR* pVec, SVECTOR* pUnitVec)
{
    const long double value = 4096.0 / sqrt((double)
        pVec->field_0_x * pVec->field_0_x +
        pVec->field_4_y * pVec->field_4_y +
        pVec->field_8_z * pVec->field_8_z);

    pUnitVec->field_0_x = (short signed int)((double)pVec->field_0_x * value);
    pUnitVec->field_2_y = (short signed int)((double)pVec->field_4_y * value);
    pUnitVec->field_4_z = (short signed int)((double)pVec->field_8_z * value);
}
MGS_FUNC_IMPLEX(0x44CB70, VectorNormalS_44CB70, IMPL_PSX);

void CC VectorNormalSS_44CC00(const SVECTOR* pVec, SVECTOR* pUnitVec)
{
    const long double value = 4096.0 / sqrt((double)
        (signed int)pVec->field_0_x * (signed int)pVec->field_0_x +
        (signed int)pVec->field_2_y * (signed int)pVec->field_2_y +
        (signed int)pVec->field_4_z * (signed int)pVec->field_4_z);

    pUnitVec->field_0_x = (short signed int)((double)pVec->field_0_x * value);
    pUnitVec->field_2_y = (short signed int)((double)pVec->field_2_y * value);
    pUnitVec->field_4_z = (short signed int)((double)pVec->field_4_z * value);
}
MGS_FUNC_IMPLEX(0x44CC00, VectorNormalSS_44CC00, IMPL_PSX);

struct Mtx
{
    short int m[3][3];
    char pad[14];
};
MGS_ASSERT_SIZEOF(Mtx, 32);

MGS_VAR(1, 0x993E40, Mtx, gte_matrix_993E40, {});
MGS_VAR(1, 0x993F34, DWORD, gGte_square_of_ir_dword_993F34, 0);

void CC Psx_gte_RT1_rtir_447480()
{
    ++gGteData_722688.gte_RT1_count_7226AC;
    ++gGteData_722688.gte_rtir_count_7226E0;

    const int v0 =
     (signed __int16)gGte_in1_dword_993EE4 * gte_matrix_993E40.m[0][0]
    + (signed __int16)gGte_in2_dword_993EE8 * gte_matrix_993E40.m[0][1]
    + (signed __int16)gGte_in3_dword_993EEC * gte_matrix_993E40.m[0][2];

    const int v1 = 
      (signed __int16)gGte_in1_dword_993EE4 * gte_matrix_993E40.m[1][0]
    + (signed __int16)gGte_in2_dword_993EE8 * gte_matrix_993E40.m[1][1]
    + (signed __int16)gGte_in3_dword_993EEC * gte_matrix_993E40.m[1][2];

    const int v3 = 
      (signed __int16)gGte_in1_dword_993EE4 * gte_matrix_993E40.m[2][0]
    + (signed __int16)gGte_in2_dword_993EE8 * gte_matrix_993E40.m[2][1]
    + (signed __int16)gGte_in3_dword_993EEC * gte_matrix_993E40.m[2][2];

    gGte_in1_dword_993EE4 = v0 >> 12;
    gGte_out1_dword_993F24 = v0 >> 12;

    gGte_in2_dword_993EE8 = v1 >> 12;
    gGte_out2_dword_993F28 = v1 >> 12;

    gGte_in3_dword_993EEC = v3 >> 12;
    gGte_out3_dword_993F2C = v3 >> 12;
}
MGS_FUNC_IMPLEX(0x447480, Psx_gte_RT1_rtir_447480, true);

void CC Psx_gte_RT1_rtv0_447180()
{
    ++gGteData_722688.gte_RT1_count_7226AC;
    ++gGteData_722688.gte_rtv0_count_722694;

    gGte_out1_dword_993F24 = 
         (gGte_VXY0_993EC0.regs.VY0 * gte_matrix_993E40.m[0][0]
        + gGte_VXY0_993EC0.regs.VX0 * gte_matrix_993E40.m[0][1]
        + gGte_VXY0_993EC0.regs.VZ0 * gte_matrix_993E40.m[0][2]) >> 12;

    gGte_out2_dword_993F28 =
        (gGte_VXY0_993EC0.regs.VY0 * gte_matrix_993E40.m[1][0]
       + gGte_VXY0_993EC0.regs.VX0 * gte_matrix_993E40.m[1][1]
       + gGte_VXY0_993EC0.regs.VZ0 * gte_matrix_993E40.m[1][2]) >> 12;

    gGte_out3_dword_993F2C =
        (gGte_VXY0_993EC0.regs.VY0 * gte_matrix_993E40.m[2][0]
       + gGte_VXY0_993EC0.regs.VX0 * gte_matrix_993E40.m[2][1]
       + gGte_VXY0_993EC0.regs.VZ0 * gte_matrix_993E40.m[2][2]) >> 12;

    gGte_in1_dword_993EE4 = 
         (gGte_VXY0_993EC0.regs.VY0 * gte_matrix_993E40.m[0][0]
        + gGte_VXY0_993EC0.regs.VX0 * gte_matrix_993E40.m[0][1]
        + gGte_VXY0_993EC0.regs.VZ0 * gte_matrix_993E40.m[0][2]) >> 12;

    gGte_in2_dword_993EE8 = 
         (gGte_VXY0_993EC0.regs.VY0 * gte_matrix_993E40.m[1][0]
        + gGte_VXY0_993EC0.regs.VX0 * gte_matrix_993E40.m[1][1]
        + gGte_VXY0_993EC0.regs.VZ0 * gte_matrix_993E40.m[1][2]) >> 12;

    gGte_in3_dword_993EEC = 
         (gGte_VXY0_993EC0.regs.VY0 * gte_matrix_993E40.m[2][0]
        + gGte_VXY0_993EC0.regs.VX0 * gte_matrix_993E40.m[2][1]
        + gGte_VXY0_993EC0.regs.VZ0 * gte_matrix_993E40.m[2][2]) >> 12;
}
MGS_FUNC_IMPLEX(0x447180, Psx_gte_RT1_rtv0_447180, true);

void Test_Psx_gte_RT1_rtir_447480()
{
    gGte_in1_dword_993EE4 = 4096;
    gGte_in2_dword_993EE8 = 4096;
    gGte_in3_dword_993EEC = 4096;

    gte_matrix_993E40.m[0][0] = 4096;
    gte_matrix_993E40.m[1][1] = 4096;
    gte_matrix_993E40.m[2][2] = 4096;


    Psx_gte_RT1_rtir_447480();

    ASSERT_EQ(4096, gGte_out1_dword_993F24);
    ASSERT_EQ(4096, gGte_out2_dword_993F28);
    ASSERT_EQ(4096, gGte_out3_dword_993F2C);

    ASSERT_EQ(0, gGte_square_of_ir_dword_993F34);
}

void Test_Psx_gte_rtpt_445990()
{

}

void DoPsxTests()
{
    Test_Psx_gte_RT1_rtir_447480();
    Test_Psx_gte_rtpt_445990();
}
