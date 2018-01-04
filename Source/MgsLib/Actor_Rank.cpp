#include "stdafx.h"
#include "Actor_Rank.hpp"
#include "Script.hpp"
#include <gmock/gmock.h>
#include <array>
#include "LibDG.hpp"
#include "ResourceNameHash.hpp"
#include "WinMain.hpp"

#define ACTOR_RANK_IMPL true

void Actor_RankCPP_ForceLink() {}

BYTE* CC Res_rank_prim_related_4767CE(Actor_Rank *pRank, WORD resourceNameHash, POLY_FT4* pData, __int16 x, __int16 y, __int16 w, __int16 h, int flagQ, int type)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
    return nullptr;
}
MGS_FUNC_IMPLEX(0x4767CE, Res_rank_prim_related_4767CE, false); // TODO

union PolyTag 
{
    struct 
    {
        WORD LowPart;
        WORD HighPart;
    };
    DWORD WordPart;
};

void SetDepth(POLY_FT4* p, u16 depth)
{
    PolyTag* pTag = (PolyTag*)&p->tag;
    pTag->LowPart = depth;
    //pTag->HighPart = 0x0900;
}

void CC RankRenderPrimsQ_46ED0A(Actor_Rank* pRank)
{
    POLY_FT4* pDst = (POLY_FT4 *)pRank->field_24_ptr_16_prims->field_40_pDataStart[gActiveBuffer_dword_791A08];
    POLY_FT4* pSrc = &pRank->field_2C_back_l_type0_0x40_start;

    for (int i = 0; i < 16; i++)
    {
        memcpy(&pDst[i], &pSrc[i], sizeof(POLY_FT4));
        SetDepth(&pDst[i], pRank->field_41C_16_prim_dst[i]);
    }

    POLY_FT4* pDst2 = (POLY_FT4 *)pRank->field_28_ptr_9_prims->field_40_pDataStart[gActiveBuffer_dword_791A08];
    POLY_FT4* pSrc2 = &pRank->field_2AC_cur_lu;
    for (int i = 0; i < 9; i++)
    {
        memcpy(&pDst2[i], &pSrc2[i], sizeof(POLY_FT4));
        SetDepth(&pDst2[i], pRank->field_45C_8_prim_dst[i]);
    }
}
MGS_FUNC_IMPLEX(0x46ED0A, RankRenderPrimsQ_46ED0A, true); // TODO

void CC POLYFT4_SetTransparency_40E0D0(POLY_FT4* pPoly, BOOL bIsSemiTransparent)
{
    if (bIsSemiTransparent)
    {
        pPoly->code |= 2u;
    }
    else
    {
        pPoly->code &= ~2u;
    }
}
MGS_FUNC_IMPLEX(0x40E0D0, POLYFT4_SetTransparency_40E0D0, ACTOR_RANK_IMPL);

void CC Rank_Init_POLYFT4_476A96(Actor_Rank* /*pRank*/, POLY_FT4* pPoly, __int16 x0, __int16 y0, __int16 x1, __int16 y2, int bIsSemiTransparent)
{
    setPolyFT4(pPoly);
    setRGB0(pPoly, 64, 64, 64);
    pPoly->x0 = x0;
    pPoly->y0 = y0;
    pPoly->x1 = x1;
    pPoly->y1 = y0;
    pPoly->x2 = x0;
    pPoly->y2 = y2;
    pPoly->x3 = x1;
    pPoly->y3 = y2;
    POLYFT4_SetTransparency_40E0D0(pPoly, bIsSemiTransparent);
}
MGS_FUNC_IMPLEX(0x476A96, Rank_Init_POLYFT4_476A96, ACTOR_RANK_IMPL);


int CC Menu_DrawText(const char* Format, int a2, int a3, int a4, int a5)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
    return 0;
}
MGS_FUNC_IMPLEX(0x459B63, Menu_DrawText, false);  // TODO

void CC TextSetXYFlags_459B0B(int x, int y, int flags)
{
    // 0x1 = right align
    // 0x2 = center align
    // 0x10 = larger font size
    // other bits = left align/do nothing
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;

}
MGS_FUNC_IMPLEX(0x459B0B, TextSetXYFlags_459B0B, false);  // TODO

int CC TextSetRGB_459B27(int r, int g, int b)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
    return 0;
}
MGS_FUNC_IMPLEX(0x459B27, TextSetRGB_459B27, false);  // TODO

// TODO: Vars
WORD gNumSaves_word_78E890 = 5;
WORD gContinues_word_78E88E = 20;
WORD gTimesSpotted_word_78E87C = 30;
WORD gNumEnemiesKilled_word_78E87E = 40;
WORD gNumRations_word_78E88C = 18;
DWORD gDiffcultyLevel = -1; // -1 = Very easy, 0 = Easy
MGS_PTR(1, 0x67676C, BYTE*, gRankXPosTable_byte_67676C, {});

static void Rank_RenderPlayTime(Actor_Rank* pRank)
{
    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(164, 60, 17);
        break;
    case 1:
        TextSetXYFlags_459B0B(164, 57, 17);
        break;
    case 2:
        TextSetXYFlags_459B0B(164, 53, 17);
        break;
    case 3:
        TextSetXYFlags_459B0B(164, 47, 17);
        break;
    }
    TextSetRGB_459B27(82, 140, 123);
    Menu_DrawText("PLAY TIME /");

    DWORD field_488_time = pRank->field_488_time;
    DWORD field_48C_time_mins = pRank->field_48C_time_mins;
    DWORD field_490_time_secs = pRank->field_490_time_secs;
    if (field_488_time >= 100)
    {
        field_488_time = 99;
        field_48C_time_mins = 59;
        field_490_time_secs = 59;
    }
    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(172, 60, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(172, 57, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(172, 53, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(172, 47, 16);
        break;
    }
    TextSetRGB_459B27(140, 181, 181);
    Menu_DrawText("%d", field_488_time / 10);

    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(181, 60, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(181, 57, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(181, 53, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(181, 47, 16);
        break;
    }
    TextSetRGB_459B27(140, 181, 181);
    Menu_DrawText("%d", field_488_time % 10);

    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(193, 60, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(193, 57, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(193, 53, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(193, 47, 16);
        break;
    }
    TextSetRGB_459B27(140, 181, 181);
    Menu_DrawText(":", field_488_time % 10);
 
    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(199, 60, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(199, 57, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(199, 53, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(199, 47, 16);
        break;
    }
    TextSetRGB_459B27(140, 181, 181);
    Menu_DrawText("%d", field_48C_time_mins / 10);

    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(208, 60, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(208, 57, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(208, 53, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(208, 47, 16);
        break;
    }
    TextSetRGB_459B27(140, 181, 181);
    Menu_DrawText("%d", field_48C_time_mins % 10);

    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(220, 60, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(220, 57, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(220, 53, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(220, 47, 16);
        break;
    }
    TextSetRGB_459B27(140, 181, 181);
    Menu_DrawText(":", field_48C_time_mins % 10);

    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(226, 60, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(226, 57, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(226, 53, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(226, 47, 16);
        break;
    }
    TextSetRGB_459B27(140, 181, 181);
    Menu_DrawText("%d", field_490_time_secs / 10);

    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(235, 60, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(235, 57, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(235, 53, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(235, 47, 16);
        break;
    }
    TextSetRGB_459B27(140, 181, 181);
    Menu_DrawText("%d", field_490_time_secs % 10);
}

static void Rank_RenderNumSaves(Actor_Rank* pRank)
{
    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(164, 74, 17);
        break;
    case 1:
        TextSetXYFlags_459B0B(164, 68, 17);
        break;
    case 2:
        TextSetXYFlags_459B0B(164, 64, 17);
        break;
    case 3:
        TextSetXYFlags_459B0B(164, 58, 17);
        break;
    }
    TextSetRGB_459B27(82, 140, 123);
    Menu_DrawText("SAVE /");

    WORD gNumSaves_word_78E890_copy = gNumSaves_word_78E890;
    if (gNumSaves_word_78E890 >= 1000)
    {
        gNumSaves_word_78E890_copy = 999;
    }
    WORD savesDiv100 = gNumSaves_word_78E890_copy / 100;
    WORD savesMod100Div10 = gNumSaves_word_78E890_copy % 100 / 10;
    if (gNumSaves_word_78E890_copy / 100)
    {
        switch (pRank->field_498_mc_no)
        {
        case 0:
            TextSetXYFlags_459B0B(172, 74, 16);
            break;
        case 1:
            TextSetXYFlags_459B0B(172, 68, 16);
            break;
        case 2:
            TextSetXYFlags_459B0B(172, 64, 16);
            break;
        case 3:
            TextSetXYFlags_459B0B(172, 58, 16);
            break;
        }
        TextSetRGB_459B27(140, 181, 181);
        Menu_DrawText("%d", savesDiv100);
    }

    if (savesMod100Div10 || savesDiv100)
    {
        switch (pRank->field_498_mc_no)
        {
        case 0:
            TextSetXYFlags_459B0B(181, 74, 16);
            break;
        case 1:
            TextSetXYFlags_459B0B(181, 68, 16);
            break;
        case 2:
            TextSetXYFlags_459B0B(181, 64, 16);
            break;
        case 3:
            TextSetXYFlags_459B0B(181, 58, 16);
            break;
        }
        TextSetRGB_459B27(140, 181, 181);
        Menu_DrawText("%d", savesMod100Div10);
    }

    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(190, 74, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(190, 68, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(190, 64, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(190, 58, 16);
        break;
    }
    TextSetRGB_459B27(140, 181, 181);
    Menu_DrawText("%d", gNumSaves_word_78E890_copy % 10);

    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(214, 74, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(214, 68, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(214, 64, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(214, 58, 16);
        break;
    }
    TextSetRGB_459B27(82, 140, 123);
    Menu_DrawText("TIMES");
}

static void Rank_RenderNumContinues(Actor_Rank* pRank)
{
    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(164, 88, 17);
        break;
    case 1:
        TextSetXYFlags_459B0B(164, 79, 17);
        break;
    case 2:
        TextSetXYFlags_459B0B(164, 75, 17);
        break;
    case 3:
        TextSetXYFlags_459B0B(164, 69, 17);
        break;
    }
    TextSetRGB_459B27(82, 140, 123);
    Menu_DrawText("CONTINUE /");

    DWORD numContinues = gContinues_word_78E88E;
    if (gContinues_word_78E88E >= 1000)
    {
        numContinues = 999;
    }
    DWORD numContinuesDiv100 = numContinues / 100;
    DWORD numContinuesMod100Div10 = numContinues % 100 / 10;
    if (numContinues / 100)
    {
        switch (pRank->field_498_mc_no)
        {
        case 0:
            TextSetXYFlags_459B0B(172, 88, 16);
            break;
        case 1:
            TextSetXYFlags_459B0B(172, 79, 16);
            break;
        case 2:
            TextSetXYFlags_459B0B(172, 75, 16);
            break;
        case 3:
            TextSetXYFlags_459B0B(172, 69, 16);
            break;
        }
        TextSetRGB_459B27(140, 181, 181);
        Menu_DrawText("%d", numContinuesDiv100);
    }
    if (numContinuesMod100Div10 || numContinuesDiv100)
    {
        switch (pRank->field_498_mc_no)
        {
        case 0:
            TextSetXYFlags_459B0B(181, 88, 16);
            break;
        case 1:
            TextSetXYFlags_459B0B(181, 79, 16);
            break;
        case 2:
            TextSetXYFlags_459B0B(181, 75, 16);
            break;
        case 3:
            TextSetXYFlags_459B0B(181, 69, 16);
            break;
        }
        TextSetRGB_459B27(140, 181, 181);
        Menu_DrawText("%d", numContinuesMod100Div10);
    }

    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(190, 88, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(190, 79, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(190, 75, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(190, 69, 16);
        break;
    }
    TextSetRGB_459B27(140, 181, 181);
    Menu_DrawText("%d", numContinues % 10);

    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(214, 88, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(214, 79, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(214, 75, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(214, 69, 16);
        break;
    }
    TextSetRGB_459B27(82, 140, 123);
    Menu_DrawText("TIMES");
}

static void Rank_RenderTimesSpotted(Actor_Rank* pRank)
{
    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(164, 102, 17);
        break;
    case 1:
        TextSetXYFlags_459B0B(164, 90, 17);
        break;
    case 2:
        TextSetXYFlags_459B0B(164, 86, 17);
        break;
    case 3:
        TextSetXYFlags_459B0B(164, 80, 17);
        break;
    }

    TextSetRGB_459B27(82, 140, 123);
    Menu_DrawText("BEING FOUND /");
    DWORD numTimesSpotted = gTimesSpotted_word_78E87C;
    if (gTimesSpotted_word_78E87C >= 1000)
    {
        numTimesSpotted = 999;
    }
    DWORD numTimesSpottedDiv100 = numTimesSpotted / 100;
    DWORD numTimesSpottedMod100Div10 = numTimesSpotted % 100 / 10;
    if (numTimesSpotted / 100)
    {
        switch (pRank->field_498_mc_no)
        {
        case 0:
            TextSetXYFlags_459B0B(172, 102, 16);
            break;
        case 1:
            TextSetXYFlags_459B0B(172, 90, 16);
            break;
        case 2:
            TextSetXYFlags_459B0B(172, 86, 16);
            break;
        case 3:
            TextSetXYFlags_459B0B(172, 80, 16);
            break;
        }
        TextSetRGB_459B27(140, 181, 181);
        Menu_DrawText("%d", numTimesSpottedDiv100);
    }

    if (numTimesSpottedMod100Div10 || numTimesSpottedDiv100)
    {
        switch (pRank->field_498_mc_no)
        {
        case 0:
            TextSetXYFlags_459B0B(181, 102, 16);
            break;
        case 1:
            TextSetXYFlags_459B0B(181, 90, 16);
            break;
        case 2:
            TextSetXYFlags_459B0B(181, 86, 16);
            break;
        case 3:
            TextSetXYFlags_459B0B(181, 80, 16);
            break;
        }
        TextSetRGB_459B27(140, 181, 181);
        Menu_DrawText("%d", numTimesSpottedMod100Div10);
    }

    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(190, 102, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(190, 90, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(190, 86, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(190, 80, 16);
        break;
    }
    TextSetRGB_459B27(140, 181, 181);
    Menu_DrawText("%d", numTimesSpotted % 10);

    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(214, 102, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(214, 90, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(214, 86, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(214, 80, 16);
        break;
    }
    TextSetRGB_459B27(82, 140, 123);
    Menu_DrawText("TIMES");
}

static void Rank_RenderNumKilled(Actor_Rank* pRank)
{
    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(164, 116, 17);
        break;
    case 1:
        TextSetXYFlags_459B0B(164, 101, 17);
        break;
    case 2:
        TextSetXYFlags_459B0B(164, 97, 17);
        break;
    case 3:
        TextSetXYFlags_459B0B(164, 91, 17);
        break;
    }
    TextSetRGB_459B27(82, 140, 123);
    Menu_DrawText("ENEMIES /");

    DWORD numEnemiesKilled = gNumEnemiesKilled_word_78E87E;
    if (gNumEnemiesKilled_word_78E87E >= 1000)
    {
        numEnemiesKilled = 999;
    }
    DWORD numEnemiesKilledDiv100 = numEnemiesKilled / 100;
    DWORD numEnemiesKilledMod100Div10 = numEnemiesKilled % 100 / 10;
    if (numEnemiesKilled / 100)
    {
        switch (pRank->field_498_mc_no)
        {
        case 0:
            TextSetXYFlags_459B0B(172, 116, 16);
            break;
        case 1:
            TextSetXYFlags_459B0B(172, 101, 16);
            break;
        case 2:
            TextSetXYFlags_459B0B(172, 97, 16);
            break;
        case 3:
            TextSetXYFlags_459B0B(172, 91, 16);
            break;
        }
        TextSetRGB_459B27(140, 181, 181);
        Menu_DrawText("%d", numEnemiesKilledDiv100);
    }

    if (numEnemiesKilledMod100Div10 || numEnemiesKilledDiv100)
    {
        switch (pRank->field_498_mc_no)
        {
        case 0:
            TextSetXYFlags_459B0B(181, 116, 16);
            break;
        case 1:
            TextSetXYFlags_459B0B(181, 101, 16);
            break;
        case 2:
            TextSetXYFlags_459B0B(181, 97, 16);
            break;
        case 3:
            TextSetXYFlags_459B0B(181, 91, 16);
            break;
        }
        TextSetRGB_459B27(140, 181, 181);
        Menu_DrawText("%d", numEnemiesKilledMod100Div10);
    }

    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(190, 116, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(190, 101, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(190, 97, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(190, 91, 16);
        break;
    }
    TextSetRGB_459B27(140, 181, 181);
    Menu_DrawText("%d", numEnemiesKilled % 10);

    switch (pRank->field_498_mc_no)
    {
    case 0:
        TextSetXYFlags_459B0B(214, 116, 16);
        break;
    case 1:
        TextSetXYFlags_459B0B(214, 101, 16);
        break;
    case 2:
        TextSetXYFlags_459B0B(214, 97, 16);
        break;
    case 3:
        TextSetXYFlags_459B0B(214, 91, 16);
        break;
    }
    TextSetRGB_459B27(82, 140, 123);
    Menu_DrawText("KILLED");
}

static void Rank_RenderText(Actor_Rank* pRank, const char* text, int formatParam, int xpos, std::array<int, 4> yposes, int flags, int r, int g, int b)
{
    TextSetXYFlags_459B0B(xpos, yposes[pRank->field_498_mc_no], flags);
    TextSetRGB_459B27(r, g, b);
    Menu_DrawText(text, formatParam);
}


static void Rank_RenderNumRations(Actor_Rank* pRank)
{
    std::array<int, 4> ypos = { 130, 112, 108, 102 };
    const int r = 140;
    const int g = 181;
    const int b = 181;

    Rank_RenderText(pRank, "RATIONS /", 0, 164, ypos, 17, 82, 140, 123);

    DWORD numRationsCapped = gNumRations_word_78E88C;
    if (gNumRations_word_78E88C >= 1000)
    {
        numRationsCapped = 999;
    }
    
    const DWORD numRationsDiv100 = numRationsCapped / 100;
    const DWORD numRationsMod100Div10 = numRationsCapped % 100 / 10;
    if (numRationsCapped / 100)
    {
        Rank_RenderText(pRank, "%d", numRationsDiv100, 172, ypos, 16, r, g, b);
    }

    if (numRationsMod100Div10 || numRationsDiv100)
    {
        Rank_RenderText(pRank, "%d", numRationsMod100Div10 % 10, 181, ypos, 16, r, g, b);
    }

    Rank_RenderText(pRank, "%d", numRationsCapped % 10, 190, ypos, 16, r, g, b);
    Rank_RenderText(pRank, "USED", 0, 214, ypos, 16, 82, 140, 123);
}

void CC Rank_RenderGameCompletionScreen(Actor_Rank* pRank)
{

    Rank_RenderPlayTime(pRank);
    Rank_RenderNumSaves(pRank);
    Rank_RenderNumContinues(pRank);
    Rank_RenderTimesSpotted(pRank);
    Rank_RenderNumKilled(pRank);
    Rank_RenderNumRations(pRank);

    // If pRank->field_498_mc_no == 0 then "GAME LEVEL" is never displayed, along with USED ITEM list

    if (pRank->field_498_mc_no == 1)
    {
        if (pRank->field_49C_radar)
        {
            TextSetXYFlags_459B0B(164, 46, 17);
            TextSetRGB_459B27(82, 140, 123);
            Menu_DrawText("GAME LEVEL /");
            TextSetXYFlags_459B0B(172, 46, 16);
            TextSetRGB_459B27(140, 181, 181);

            switch (gDiffcultyLevel)
            {
            case -1:
                Menu_DrawText("VERY EASY");
                break;
            case 0:
                Menu_DrawText("EASY");
                break;
            case 1:
                Menu_DrawText("NORMAL");
                break;
            case 2:
                Menu_DrawText("HARD");
                break;
            case 3:
                Menu_DrawText("EXTREME");
                break;
            }

        }
        else if (pRank->field_4A0_stealth)
        {
            TextSetXYFlags_459B0B(164, 112, 17);
            TextSetRGB_459B27(82, 140, 123);
            Menu_DrawText("USED ITEM /");
            TextSetXYFlags_459B0B(172, 112, 16);
            TextSetRGB_459B27(140, 181, 181);
            Menu_DrawText("STEALTH");
        }
        else
        {
            TextSetXYFlags_459B0B(164, 112, 17);
            TextSetRGB_459B27(82, 140, 123);
            Menu_DrawText("USED ITEM /");
            TextSetXYFlags_459B0B(172, 112, 16);
            TextSetRGB_459B27(140, 181, 181);
            Menu_DrawText("BANDANA");
        }
    }
    else if (pRank->field_498_mc_no == 2)
    {
        if (pRank->field_49C_radar)
        {
            TextSetXYFlags_459B0B(164, 42, 17);
            TextSetRGB_459B27(82, 140, 123);
            Menu_DrawText("GAME LEVEL /");
            TextSetXYFlags_459B0B(172, 42, 16);
            TextSetRGB_459B27(140, 181, 181);

            switch (gDiffcultyLevel)
            {
            case -1:
                Menu_DrawText("VERY EASY");
                break;
            case 0:
                Menu_DrawText("EASY");
                break;
            case 1:
                Menu_DrawText("NORMAL");
                break;
            case 2:
                Menu_DrawText("HARD");
                break;
            case 3:
                Menu_DrawText("EXTREME");
                break;
            }

            if (pRank->field_4A0_stealth)
            {
                TextSetXYFlags_459B0B(164, 119, 17);
                TextSetRGB_459B27(82, 140, 123);
                Menu_DrawText("USED ITEM /");
                TextSetXYFlags_459B0B(172, 119, 16);
                TextSetRGB_459B27(140, 181, 181);
                Menu_DrawText("STEALTH");
            }
            else
            {
                TextSetXYFlags_459B0B(164, 119, 17);
                TextSetRGB_459B27(82, 140, 123);
                Menu_DrawText("USED ITEM /");
                TextSetXYFlags_459B0B(172, 119, 16);
                TextSetRGB_459B27(140, 181, 181);
                Menu_DrawText("BANDANA");
            }
        }
        else
        {
            TextSetXYFlags_459B0B(164, 108, 17);
            TextSetRGB_459B27(82, 140, 123);
            Menu_DrawText("USED ITEMS /");
            TextSetXYFlags_459B0B(172, 108, 16);
            TextSetRGB_459B27(140, 181, 181);
            Menu_DrawText("STEALTH");
            TextSetXYFlags_459B0B(172, 119, 16);
            TextSetRGB_459B27(140, 181, 181);
            Menu_DrawText("BANDANA");
        }
    }
    else if (pRank->field_498_mc_no == 3) // Or != 0
    {
        TextSetXYFlags_459B0B(164, 36, 17);
        TextSetRGB_459B27(82, 140, 123);
        Menu_DrawText("GAME LEVEL /");
        TextSetXYFlags_459B0B(172, 36, 16);
        TextSetRGB_459B27(140, 181, 181);

        switch (gDiffcultyLevel)
        {
        case -1:
            Menu_DrawText("VERY EASY");
            break;
        case 0:
            Menu_DrawText("EASY");
            break;
        case 1:
            Menu_DrawText("NORMAL");
            break;
        case 2:
            Menu_DrawText("HARD");
            break;
        case 3:
            Menu_DrawText("EXTREME");
            break;
        }

        TextSetXYFlags_459B0B(164, 113, 17);
        TextSetRGB_459B27(82, 140, 123);
        Menu_DrawText("USED ITEMS /");
        TextSetXYFlags_459B0B(172, 113, 16);
        TextSetRGB_459B27(140, 181, 181);
        Menu_DrawText("STEALTH");
        TextSetXYFlags_459B0B(172, 124, 16);
        TextSetRGB_459B27(140, 181, 181);
        Menu_DrawText("BANDANA");
    }

    TextSetXYFlags_459B0B(115 - gRankXPosTable_byte_67676C[pRank->field_494_ranking], 143, 16);
    TextSetRGB_459B27(82, 140, 123);
    Menu_DrawText("CODE NAME");
    if (gDiffcultyLevel != -1)
    {
        TextSetXYFlags_459B0B(107, 163, 16);
        TextSetRGB_459B27(82, 140, 123);
        Menu_DrawText("SPECIAL ITEMS");
    }
}
MGS_FUNC_IMPLEX(0x46EDE8, Rank_RenderGameCompletionScreen, ACTOR_RANK_IMPL);

void CC Rank_ToState2If_473E69(Actor_Rank* pRank)
{
    if (pRank->field_20_dword_7919E0[1]) // TODO: Discover what this field really is
    {
        pRank->field_484_state = 2;
        pRank->field_480_ticks = 0;
    }
}
MGS_FUNC_IMPLEX(0x473E69, Rank_ToState2If_473E69, ACTOR_RANK_IMPL);

void CC Rank_473E9C(Actor_Rank* pRank)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
}
MGS_FUNC_IMPLEX(0x473E9C, Rank_473E9C, false); // TODO

void CC Rank_GameCompletionRelatedQ(Actor_Rank* pRank)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
}
MGS_FUNC_IMPLEX(0x470AE6, Rank_GameCompletionRelatedQ, false); // TODO

void CC Rank_SaveAfterGameCompleteQ(Actor_Rank* pRank)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
}
MGS_FUNC_IMPLEX(0x474315, Rank_SaveAfterGameCompleteQ, false); // TODO

void CC Rank_End_474CE8(Actor_Rank* pRank)
{
    Script_ProcCancelOrRun((WORD)pRank->field_418_script_t, nullptr);
    Actor_DestroyOnNextUpdate_40A3ED(&pRank->mBase);
}
MGS_FUNC_IMPLEX(0x474CE8, Rank_End_474CE8, ACTOR_RANK_IMPL);

void CC Rank_Animate_472832(Actor_Rank* pRank)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
}
MGS_FUNC_IMPLEX(0x472832, Rank_Animate_472832, false);  // TODO

void CC Rank_update_46EC75(Actor_Rank* pRank)
{
    if ((pRank->field_480_ticks % 60) == 0)
    {
        pRank->field_498_mc_no++;
        if (pRank->field_498_mc_no > 3)
        {
            pRank->field_498_mc_no = 0;
        }
    }

    switch (pRank->field_484_state)
    {
    case 0:
        // state 0 handler - animates the [SCORE] box ?
        Rank_Animate_472832(pRank);
        break;
    case 1:
        // renders only text with no background ??
        Rank_RenderGameCompletionScreen(pRank);
        
        // Disabled for now so above func can be impl'd
        //Rank_ToState2If_473E69(pRank);
        break;
    case 2:
        Rank_473E9C(pRank);
        Rank_GameCompletionRelatedQ(pRank); // Only displays rank name?
        break;
    case 3:
        Rank_SaveAfterGameCompleteQ(pRank);
        break;
    case 4:
        Rank_End_474CE8(pRank);
        break;
    }
    RankRenderPrimsQ_46ED0A(pRank);
    pRank->field_480_ticks++;
}
MGS_FUNC_IMPLEX(0x46EC75, Rank_update_46EC75, ACTOR_RANK_IMPL);

void CC Res_rank_shutdown_474D08(Actor_Rank* pRank)
{
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
}
MGS_FUNC_IMPLEX(0x474D08, Res_rank_shutdown_474D08, false) // TODO

MGS_VAR_EXTERN(u32, dword_9942A0); // From Actor

MGS_VAR(1, 0x7919E0, char*, dword_7919E0, 0);
MGS_VAR(1, 0x78E896, WORD, gGameTime_word_78E896, 0);
MGS_VAR(1, 0x78E898, WORD, gGameTime_word_78E898, 0);

MGS_FUNC_NOT_IMPL(0x475BCE, __int16 __cdecl(Actor_Rank *pRank), Rank_RankCalcs_475BCE);
MGS_FUNC_NOT_IMPL(0x47589A, int __cdecl(Actor_Rank *pRank, int index), Rank_47589A);
MGS_FUNC_NOT_IMPL(0x475A4A, int __cdecl(Actor_Rank *a2, int idx), Rank_475A4A);
MGS_FUNC_NOT_IMPL(0x4742E1, int __cdecl(Actor_Rank *a1, int a2, signed int a3), Rank_4742E1);


int CC Res_rank_loader(Actor_Rank* pRank, int a3)
{
    dword_9942A0 = (u32)a3;
    pRank->field_5E0_896 = 896;
    pRank->field_5E4_256 = 256;
    pRank->field_5E8_896 = 896;
    pRank->field_5EC_276 = 276;

    Prim_unknown* pPrim16Data = PrimAlloc_405050(2066, 16, 0, 0, 0);
    if (pPrim16Data)
    {
        PrimAdd_401805(pPrim16Data);
        pPrim16Data->field_28_dword_9942A0 = (signed __int16)dword_9942A0;
    }
    pRank->field_24_ptr_16_prims = pPrim16Data;
    
    Prim_unknown* pPrim9Data = PrimAlloc_405050(2066, 9, 0, 0, 0);
    if (pPrim9Data)
    {
        PrimAdd_401805(pPrim9Data);
        pPrim9Data->field_28_dword_9942A0 = (signed __int16)dword_9942A0;
    }
    pRank->field_28_ptr_9_prims = pPrim9Data;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("back_l"), &pRank->field_2C_back_l_type0_0x40_start, -160, -112, 0, 112, 0, 0);
    pRank->field_41C_16_prim_dst[0] = 0;
    
    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("back_r"), &pRank->field_54_back_r_type0, 0, -112, 160, 112, 0, 0);
    pRank->field_41C_16_prim_dst[1] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("rank_rank"), &pRank->field_7C_rank_rank_type0, -34, -94, 34, -82, 1, 0);
    pRank->field_7C_rank_rank_type0.r0 = 46;
    pRank->field_7C_rank_rank_type0.g0 = 72;
    pRank->field_7C_rank_rank_type0.b0 = 61;
    pRank->field_41C_16_prim_dst[2] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("rank_rank"), &pRank->field_A4_rank_rank_type0, -34, -94, 34, -82, 1, 0);
    pRank->field_A4_rank_rank_type0.r0 = 46;
    pRank->field_A4_rank_rank_type0.g0 = 72;
    pRank->field_A4_rank_rank_type0.b0 = 61;
    pRank->field_41C_16_prim_dst[3] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("rank_line1"), &pRank->field_CC_rank_line1_type1, 125, -57, 129, 13, 0, 1);
    pRank->field_CC_rank_line1_type1.r0 = 82;
    pRank->field_CC_rank_line1_type1.g0 = 140;
    pRank->field_CC_rank_line1_type1.b0 = 123;
    pRank->field_41C_16_prim_dst[4] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("rank_line1"), &pRank->field_F4_rank_line1_type1, -125, -57, -121, 13, 0, 1);
    pRank->field_F4_rank_line1_type1.r0 = 82;
    pRank->field_F4_rank_line1_type1.g0 = 140;
    pRank->field_F4_rank_line1_type1.b0 = 123;
    pRank->field_41C_16_prim_dst[5] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("rank_line2"), &pRank->field_11C_rank_line2, 8, -52, 90, -51, 0, 2);
    pRank->field_11C_rank_line2.r0 = 140;
    pRank->field_11C_rank_line2.g0 = 181;
    pRank->field_11C_rank_line2.b0 = 181;
    pRank->field_41C_16_prim_dst[6] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("rank_line2"), &pRank->field_144_rank_line2, 8, -38, 44, -37, 0, 2);
    pRank->field_144_rank_line2.r0 = 140;
    pRank->field_144_rank_line2.g0 = 181;
    pRank->field_144_rank_line2.b0 = 181;
    pRank->field_41C_16_prim_dst[7] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("rank_line2"), &pRank->field_16C_rank_line2, 8, -24, 44, -23, 0, 2);
    pRank->field_16C_rank_line2.r0 = 140;
    pRank->field_16C_rank_line2.g0 = 181;
    pRank->field_16C_rank_line2.b0 = 181;
    pRank->field_41C_16_prim_dst[8] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("rank_line2"), &pRank->field_194_rank_line2, 8, -10, 44, -9, 0, 2);
    pRank->field_194_rank_line2.r0 = 140;
    pRank->field_194_rank_line2.g0 = 181;
    pRank->field_194_rank_line2.b0 = 181;
    pRank->field_41C_16_prim_dst[9] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("rank_line2"), &pRank->field_1BC_rank_line2, 8, 4, 44, 5, 0, 2);
    pRank->field_1BC_rank_line2.r0 = 140;
    pRank->field_1BC_rank_line2.g0 = 181;
    pRank->field_1BC_rank_line2.b0 = 181;
    pRank->field_41C_16_prim_dst[10] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("rank_line2"), &pRank->field_1E4_rank_line2, 8, 18, 44, 19, 0, 2);
    pRank->field_1E4_rank_line2.r0 = 140;
    pRank->field_1E4_rank_line2.g0 = 181;
    pRank->field_1E4_rank_line2.b0 = 181;
    pRank->field_41C_16_prim_dst[11] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("rank_fox"), &pRank->field_20C_rank_fox, -22, 34, 22, 42, 0, 0);
    pRank->field_20C_rank_fox.r0 = 165;
    pRank->field_20C_rank_fox.g0 = 74;
    pRank->field_20C_rank_fox.b0 = 74;
    pRank->field_41C_16_prim_dst[12] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("rank_spe_camera"), &pRank->field_234_rank_spe_camera, -22, 33, 22, 42, 0, 0);
    pRank->field_234_rank_spe_camera.r0 = 74;
    pRank->field_234_rank_spe_camera.g0 = 107;
    pRank->field_234_rank_spe_camera.b0 = 148;
    pRank->field_41C_16_prim_dst[13] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("rank_spe_stealth"), &pRank->field_25C_rank_spe_stealth, -22, 33, 22, 42, 0, 0);
    pRank->field_25C_rank_spe_stealth.r0 = 74;
    pRank->field_25C_rank_spe_stealth.g0 = 107;
    pRank->field_25C_rank_spe_stealth.b0 = 148;
    pRank->field_41C_16_prim_dst[14] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("rank_spe_mugen"), &pRank->field_284_rank_spe_mugen, -22, 33, 22, 42, 0, 0);
    pRank->field_284_rank_spe_mugen.r0 = 74;
    pRank->field_284_rank_spe_mugen.g0 = 107;
    pRank->field_284_rank_spe_mugen.b0 = 148;
    pRank->field_41C_16_prim_dst[15] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("cur_lu"), &pRank->field_2AC_cur_lu, 0, 0, 0, 0, 1, 0);
    pRank->field_45C_8_prim_dst[0] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("cur_ru"), &pRank->field_2D4_cur_ru, 0, 0, 0, 0, 1, 0);
    pRank->field_45C_8_prim_dst[1] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("cur_ld"), &pRank->field_2FC_cur_ld, 0, 0, 0, 0, 1, 0);
    pRank->field_45C_8_prim_dst[2] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("cur_rd"), &pRank->field_324_cur_rd, 0, 0, 0, 0, 1, 0);
    pRank->field_45C_8_prim_dst[3] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("cur_u"), &pRank->field_34C_cur_u, 0, 0, 0, 0, 1, 2);
    pRank->field_45C_8_prim_dst[4] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("cur_d"), &pRank->field_374_cur_d, 0, 0, 0, 0, 1, 2);
    pRank->field_45C_8_prim_dst[5] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("cur_l"), &pRank->field_39C_cur_l, 0, 0, 0, 0, 1, 1);
    pRank->field_45C_8_prim_dst[6] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("cur_r"), &pRank->field_3C4_cur_r, 0, 0, 0, 0, 1, 1);
    pRank->field_45C_8_prim_dst[7] = 0;

    Res_rank_prim_related_4767CE(pRank, ResourceNameHash("cur_c"), &pRank->field_3EC_cur_c, 0, 0, 0, 0, 1, 3);
    pRank->field_45C_8_prim_dst[8] = 0;

    pRank->field_20_dword_7919E0 = (WORD *)&dword_7919E0;

    if (Script_ParamExists('x'))
    {
        BYTE* script_x = Script_GetReturnAddress();
        pRank->field_5F8_script_x =  Script_Unknown8_409924(script_x);
    }
    else
    {
        pRank->field_5F8_script_x = 0;
    }

    if (Script_ParamExists('e'))
    {
        BYTE* script_e = Script_GetReturnAddress();
        pRank->field_414_script_e =  Script_Unknown8_409924(script_e);
    }
    else
    {
        pRank->field_414_script_e = -1;
    }

    if (Script_ParamExists('t'))
    {
        BYTE* script_t = Script_GetReturnAddress();
        pRank->field_418_script_t = (DWORD *) Script_Unknown8_409924(script_t);
    }
    else
    {
        pRank->field_418_script_t = (DWORD *)-1;
    }

    for (int i = 0; i < 1; ++i)
    {
        BYTE* v44 = Script_GetReturnAddress();
        auto scriptVar = Script_read_string_arg_40997B(v44);
        *(&pRank->field_5C8 + 56 * i) = (int)scriptVar;
        *((WORD *)&pRank->field_5CC + 112 * i) = 0;
        Rank_47589A(pRank, i);
    }

    for (int i = 0; i < 1; ++i)
    {
        Rank_475A4A(pRank, i);
        Rank_4742E1(pRank, i, 0);
    }

    if (pRank->field_5F8_script_x)
    {
        // Go directly to the "please reload this save to play with special items screen"
        if (pRank->field_5F8_script_x == 1)
        {
            for (int i = 0; i < 16; ++i)
            {
                pRank->field_41C_16_prim_dst[i] = 0;
            }
            Rank_4742E1(pRank, 0, 0);
            pRank->field_484_state = 3;
            pRank->field_5F0 = 0;
            pRank->field_5F4 = 0;
        }
    }
    else
    {
        pRank->field_480_ticks = 0;
        pRank->field_484_state = 0;
        pRank->field_488_time = gGameTime_word_78E896;
        pRank->field_48C_time_mins = gGameTime_word_78E898 / 60;
        pRank->field_490_time_secs = gGameTime_word_78E898 % 60;
        pRank->field_498_mc_no = 0;
        pRank->field_49C_radar = 0;
        pRank->field_4A0_stealth = 0;
        pRank->field_4A4_mugen = 0;
        pRank->field_4A8_spe_no = 0;
        pRank->field_4AC_camera = 0;
        pRank->field_4B0_stealth = 0;
        pRank->field_4B4_mugen = 0;
        pRank->field_5F4 = 0;
        Rank_RankCalcs_475BCE(pRank);
    }

    RankRenderPrimsQ_46ED0A(pRank);

    return 0;
}
MGS_FUNC_IMPLEX(0x474D98, Res_rank_loader, ACTOR_RANK_IMPL)

signed int CC Res_rank_Create_46EC0E(DWORD scriptUnknown, int a_dword_722A40, BYTE* pScript)
{
    game_state_dword_72279C.flags |= 0x4A6000u; // TODO: Could be a code loc?

    Actor_Rank* pRank = Actor_ResourceAllocT<Actor_Rank>(1);
    if (pRank)
    {
        Actor_Init_40A347(&pRank->mBase,
            reinterpret_cast<TActorFunction>(Rank_update_46EC75),
            reinterpret_cast<TActorFunction>(Res_rank_shutdown_474D08), 
            "C:\\mgs\\source\\Onoda\\rank\\rank.c");

        if (Res_rank_loader(pRank, a_dword_722A40) < 0)
        {
            Actor_DestroyOnNextUpdate_40A3ED(&pRank->mBase);
            return 0;
        }
    }

    return 1;
}
MGS_FUNC_IMPLEX(0x46EC0E, Res_rank_Create_46EC0E, ACTOR_RANK_IMPL);


static void Test_Rank_Set_POLYFT4_code_40E0D0()
{
    // Turn bit 1 on
    POLY_FT4 poly = {};
    POLYFT4_SetTransparency_40E0D0(&poly, TRUE);
    ASSERT_EQ(poly.code, 2);

    // Turn bit 1 off
    poly.code = 0xFF;
    POLYFT4_SetTransparency_40E0D0(&poly, FALSE);
    ASSERT_EQ(poly.code, 0xFD);
}

void DoActor_RankTests()
{
    Test_Rank_Set_POLYFT4_code_40E0D0();
}
