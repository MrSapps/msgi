#pragma once

#include "MgsFunction.hpp"

void PsxCpp_ForceLink();

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

MGS_VAR_EXTERN(Rect16, gClipRect_6BECF0);
MGS_VAR_EXTERN(DRAWENV, gDrawEnv_6C0E98);

signed int CC Resetgraph_AndPrintPsxStructureSizes(int mode);
int CC SetGraphDebug(int debug);
void CC SetDispMask(int mask);
int CC ClearImage(Rect16* pRect, BYTE r, BYTE g, BYTE b);