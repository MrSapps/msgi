#include "stdafx.h"
#include "Actor_Rank.hpp"

void Actor_RankCPP_ForceLink() {}

BYTE* CC Res_rank_prim_related_4767CE(Actor_Rank *pRank, WORD resourceNameHash, int pData, __int16 x, __int16 y, __int16 w, __int16 h, int flagQ, int type);

MSG_FUNC_IMPL(0x4767CE, Res_rank_prim_related_4767CE);

BYTE* CC Res_rank_prim_related_4767CE(Actor_Rank *pRank, WORD resourceNameHash, int pData, __int16 x, __int16 y, __int16 w, __int16 h, int flagQ, int type)
{
    // Forcing these to zero does nothing ??!
    return Res_rank_prim_related_4767CE_.Ptr()(pRank, resourceNameHash, pData, 0, 0, 0, 0, flagQ, type);
}
MGS_VAR_EXTERN(int, gActiveBuffer_dword_791A08);

void CC RankRenderPrimsQ_46ED0A(Actor_Rank* pRank);
MSG_FUNC_IMPL(0x46ED0A, RankRenderPrimsQ_46ED0A);


void CC RankRenderPrimsQ_46ED0A(Actor_Rank* pRank)
{
    POLY_FT4* poly = &pRank->field_234_rank_spe_camera;

    // state 2 = text fade out
    // state 3 = sound effect + text box telling to reload save for special items
    // state 4 = ditto
    // state 5 = black screen/nothing

    //pRank->field_484_state = 5;

    poly->r0 = 255;
    poly->g0 = 255;
    poly->b0 = 0;

    poly->x0 += 25;
    poly->y0 += 25;

    RankRenderPrimsQ_46ED0A_.Ptr()(pRank);

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
