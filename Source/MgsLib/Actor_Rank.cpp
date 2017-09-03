#include "stdafx.h"
#include "Actor_Rank.hpp"
#include <gmock/gmock.h>

#define ACTOR_RANK_IMPL true

void Actor_RankCPP_ForceLink() {}

MGS_VAR_EXTERN(DWORD, game_state_dword_72279C);

BYTE* CC Res_rank_prim_related_4767CE(Actor_Rank *pRank, WORD resourceNameHash, int pData, __int16 x, __int16 y, __int16 w, __int16 h, int flagQ, int type);

MSG_FUNC_IMPLEX(0x4767CE, Res_rank_prim_related_4767CE, ACTOR_RANK_IMPL);

BYTE* CC Res_rank_prim_related_4767CE(Actor_Rank *pRank, WORD resourceNameHash, int pData, __int16 x, __int16 y, __int16 w, __int16 h, int flagQ, int type)
{
    // Forcing these to zero does nothing ??!
    return Res_rank_prim_related_4767CE_.Ptr()(pRank, resourceNameHash, pData, 0, 0, 0, 0, flagQ, type);
}
MGS_VAR_EXTERN(int, gActiveBuffer_dword_791A08);

void CC RankRenderPrimsQ_46ED0A(Actor_Rank* pRank);
MSG_FUNC_IMPLEX(0x46ED0A, RankRenderPrimsQ_46ED0A, ACTOR_RANK_IMPL);


void CC RankRenderPrimsQ_46ED0A(Actor_Rank* pRank)
{
    POLY_FT4* poly = &pRank->field_234_rank_spe_camera;

    // state 2 = text fade out
    // state 3 = sound effect + text box telling to reload save for special items
    // state 4 = ditto
    // state 5 = black screen/nothing

    pRank->field_484_state = 0;

    poly->r0 = 255;
    poly->g0 = 255;
    poly->b0 = 0;

    poly->x0 += 25;
    poly->y0 += 25;

    RankRenderPrimsQ_46ED0A_.Ptr()(pRank);

    if (pRank->field_480_ticks > 50)
    {
        pRank->field_480_ticks = 0;
    }

    /*
    BYTE* pDst = *(BYTE **)(pRank->field_24_ptr_16_prims + 4 * gActiveBuffer_dword_791A08 + 64);
    BYTE* pSrc = (BYTE *)&pRank->field_2C_back_l_type0_0x40_start;
    for (int i = 0; i < 16; ++i)
    {
        memcpy(pDst, pSrc, 40u);                   // PolyFT4?
        BYTE* pDst = ((BYTE *)&pRank->field_41C_16_prim_dst + i);
        pDst += 40;
        pSrc += 40;
    }

    BYTE* pDst2 = *(BYTE **)(pRank->field_28_ptr_9_prims + 4 * gActiveBuffer_dword_791A08 + 64);
    BYTE* pSrc2 = (BYTE *)&pRank->field_2AC_cur_lu;
    for (int j = 0; j < 9; ++j)
    {
        memcpy(pDst2, pSrc2, 40u);                 // PolyFT4
        BYTE* pDst2 = ((BYTE *)&pRank->field_45C_cur_ru_q + j);
        pDst2 += 40;
        pSrc2 += 40;
    }*/

}

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


void CC Rank_update_46EC75(Actor_Rank* pRank)
{

}
MSG_FUNC_IMPLEX(0x46EC75, Rank_update_46EC75, false) // TODO

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
