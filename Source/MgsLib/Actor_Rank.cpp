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

/*
void CC RankRenderPrimsQ_46ED0A(Actor_Rank* pRank)
{

}
MSG_FUNC_IMPL(0x46ED0A, RankRenderPrimsQ_46ED0A);
*/
