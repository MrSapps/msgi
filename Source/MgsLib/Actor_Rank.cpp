#include "stdafx.h"
#include "Actor_Rank.hpp"
#include "Script.hpp"
#include <gmock/gmock.h>

#define ACTOR_RANK_IMPL true

void Actor_RankCPP_ForceLink() {}

MGS_VAR_EXTERN(DWORD, game_state_dword_72279C);
MGS_VAR_EXTERN(int, gActiveBuffer_dword_791A08);

BYTE* CC Res_rank_prim_related_4767CE(Actor_Rank *pRank, WORD resourceNameHash, int pData, __int16 x, __int16 y, __int16 w, __int16 h, int flagQ, int type)
{
    return nullptr;
}
MSG_FUNC_IMPLEX(0x4767CE, Res_rank_prim_related_4767CE, false); // TODO

void CC RankRenderPrimsQ_46ED0A(Actor_Rank* pRank)
{

}
MSG_FUNC_IMPLEX(0x46ED0A, RankRenderPrimsQ_46ED0A, false); // TODO

void CC POLYFT4_code_2_40E0D0(POLY_FT4* pPoly, BOOL bSetOrUnSetCode2)
{
    if (bSetOrUnSetCode2)
    {
        pPoly->code |= 2u;
    }
    else
    {
        pPoly->code &= ~2u;
    }
}
MSG_FUNC_IMPLEX(0x40E0D0, POLYFT4_code_2_40E0D0, ACTOR_RANK_IMPL);

void CC Rank_Init_POLYFT4_476A96(Actor_Rank* /*pRank*/, POLY_FT4* pPoly, __int16 x0, __int16 y0, __int16 x1, __int16 y2, int bSetCode2)
{
    pPoly->tag = pPoly->tag & 0xFFFFFF | 0x9000000;
    pPoly->code = 44;
    pPoly->r0 = 64;
    pPoly->g0 = 64;
    pPoly->b0 = 64;
    pPoly->x0 = x0;
    pPoly->y0 = y0;
    pPoly->x1 = x1;
    pPoly->y1 = y0;
    pPoly->x2 = x0;
    pPoly->y2 = y2;
    pPoly->x3 = x1;
    pPoly->y3 = y2;
    POLYFT4_code_2_40E0D0(pPoly, bSetCode2);
}
MSG_FUNC_IMPLEX(0x476A96, Rank_Init_POLYFT4_476A96, ACTOR_RANK_IMPL);

void CC Rank_RenderGameCompletionScreen(Actor_Rank* pRank)
{

}
MSG_FUNC_IMPLEX(0x46EDE8, Rank_RenderGameCompletionScreen, false); // TODO

void CC Rank_ToState2If_473E69(Actor_Rank* pRank)
{
    if (pRank->field_20_dword_7919E0[1]) // TODO: Discover what this field really is
    {
        pRank->field_484_state = 2;
        pRank->field_480_ticks = 0;
    }
}
MSG_FUNC_IMPLEX(0x473E69, Rank_ToState2If_473E69, ACTOR_RANK_IMPL);

void CC Rank_473E9C(Actor_Rank* pRank)
{

}
MSG_FUNC_IMPLEX(0x473E9C, Rank_473E9C, false); // TODO

void CC Rank_GameCompletionRelatedQ(Actor_Rank* pRank)
{

}
MSG_FUNC_IMPLEX(0x470AE6, Rank_GameCompletionRelatedQ, false); // TODO

void CC Rank_SaveAfterGameCompleteQ(Actor_Rank* pRank)
{

}
MSG_FUNC_IMPLEX(0x474315, Rank_SaveAfterGameCompleteQ, false); // TODO

void CC Rank_End_474CE8(Actor_Rank* pRank)
{
    Script_ProcCancelOrRun((WORD)pRank->field_418_script_t, nullptr);
    Actor_DestroyOnNextUpdate(&pRank->mBase);
}
MSG_FUNC_IMPLEX(0x474CE8, Rank_End_474CE8, ACTOR_RANK_IMPL);

void CC Rank_Animate_472832(Actor_Rank* pRank)
{

}
MSG_FUNC_IMPLEX(0x472832, Rank_Animate_472832, false);  // TODO

int CC Menu_DrawText(const char* Format, int a2, int a3, int a4, int a5)
{
    return 0;
}
MSG_FUNC_IMPLEX(0x459B63, Menu_DrawText, false);  // TODO

void CC RGB_459B0B(int r, int g, int b)
{

}
MSG_FUNC_IMPLEX(0x459B0B, RGB_459B0B, false);  // TODO

int CC sub_459B27(int r, int g, int b)
{
    return 0;
}
MSG_FUNC_IMPLEX(0x459B27, sub_459B27, false);  // TODO

void CC Rank_update_46EC75(Actor_Rank* pRank)
{
    // x, y, ??
    // 0x1 = right align
    // 0x2 = center align
    // 0x10 = larger font size
    // other bits = left align/do nothing
    DWORD flags = 0;
    static int flagNo = 0;


    if ((pRank->field_480_ticks % 30) == 0)
    {
        ++flagNo;
        if (flagNo >= 31)
        {
            flagNo = 0;
        }
        flags = (1 << flagNo);
        printf("Flags are now 0x%X\n", flags);
    }
    flags = (1 << flagNo);

    RGB_459B0B(10, 20, flags);

    // R G B
    sub_459B27(0, 0, 255);
    Menu_DrawText("1234 PAUL WAS HERE", 1, 255, 255,255);

    switch (pRank->field_484_state)
    {
    case 0:
        // state 0 handler - animates the [SCORE] box ?
        Rank_Animate_472832(pRank);
        break;
    case 1:
        // renders only text with no background ??
        Rank_RenderGameCompletionScreen(pRank);
        Rank_ToState2If_473E69(pRank);
        break;
    case 2:
        Rank_473E9C(pRank);
        Rank_GameCompletionRelatedQ(pRank);
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
MSG_FUNC_IMPLEX(0x46EC75, Rank_update_46EC75, ACTOR_RANK_IMPL);

void CC Res_rank_shutdown_474D08(Actor_Rank* pRank)
{

}
MSG_FUNC_IMPLEX(0x474D08, Res_rank_shutdown_474D08, false) // TODO

int CC Res_rank_loader(Actor_Rank* pRank, int a3)
{
    return 0;
}
MSG_FUNC_IMPLEX(0x474D98, Res_rank_loader, false) // TODO

signed int CC Res_rank_1532_sub_46EC0E(DWORD scriptUnknown, int a_dword_722A40, BYTE* pScript)
{
    game_state_dword_72279C |= 0x4A6000u; // TODO: Could be a code loc?

    Actor_Rank* pRank = Actor_ResourceAllocT<Actor_Rank>(1);
    if (pRank)
    {
        Actor_Init(&pRank->mBase,
            reinterpret_cast<TActorFunction>(Rank_update_46EC75),
            reinterpret_cast<TActorFunction>(Res_rank_shutdown_474D08), 
            "C:\\mgs\\source\\Onoda\\rank\\rank.c");

        if (Res_rank_loader(pRank, a_dword_722A40) < 0)
        {
            Actor_DestroyOnNextUpdate(&pRank->mBase);
            return 0;
        }
    }

    return 1;
}
MSG_FUNC_IMPLEX(0x46EC0E, Res_rank_1532_sub_46EC0E, ACTOR_RANK_IMPL);


static void Test_Rank_Set_POLYFT4_code_40E0D0()
{
    // Turn bit 1 on
    POLY_FT4 poly = {};
    POLYFT4_code_2_40E0D0(&poly, TRUE);
    ASSERT_EQ(poly.code, 2);

    // Turn bit 1 off
    poly.code = 0xFF;
    POLYFT4_code_2_40E0D0(&poly, FALSE);
    ASSERT_EQ(poly.code, 0xFD);
}

void DoActor_RankTests()
{
    Test_Rank_Set_POLYFT4_code_40E0D0();
}
