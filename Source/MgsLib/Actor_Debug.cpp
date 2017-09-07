#include "stdafx.h"
#include "Actor_Debug.hpp"
#include "ResourceNameHash.hpp"
#include "Actor_Rank.hpp"


MSG_FUNC_NOT_IMPL(0x405050, Prim_unknown* CC(int maybeFlags, int numItems, __int16 rQ, int gQ, int bQ), PrimAlloc_405050);
MSG_FUNC_NOT_IMPL(0x401805, signed int CC(Prim_unknown* pPrimBuffer), PrimAddQ_401805);

MGS_VAR_EXTERN(int, gActiveBuffer_dword_791A08);

static void CC Debug_Update(Actor_Debug* pDebug)
{
    memcpy((pDebug->mPrimData /*+ gActiveBuffer_dword_791A08*/)->field_40_pDataStart, &pDebug->mPolyFt4, sizeof(POLY_FT4));
    pDebug->mPrimData->mBase.field_0 = 3;

}

static void CC Debug_Shutdown(Actor_Debug*)
{

}
MGS_VAR_EXTERN(u32, dword_9942A0);

static int CC Debug_Loader(Actor_Debug* pDebug)
{
    pDebug->mPolyFt4 = {};

    int numPrims = 1;
    Prim_unknown* pPrim16Data = PrimAlloc_405050(2066, numPrims, 0, 0, 0);
    if (pPrim16Data)
    {
        PrimAddQ_401805(pPrim16Data);
        pPrim16Data->field_28_dword_9942A0 = (signed __int16)dword_9942A0;
    }
    pDebug->mPrimData = pPrim16Data;

    const WORD resHash = ResourceNameHash("back_l");
    Res_rank_prim_related_4767CE(nullptr, resHash, &pDebug->mPolyFt4, -160, -112, 0, 112, 0, 0);
  
    pDebug->mPolyFt4.r0 = 255;
    pDebug->mPolyFt4.g0 = 255;
    pDebug->mPolyFt4.b0 = 255;

    pDebug->mPolyFt4.x0 = 0;
    pDebug->mPolyFt4.y0 = 0;

    pDebug->mPolyFt4.x1 = 50;
    pDebug->mPolyFt4.y1 = 50;

    pDebug->mPolyFt4.x2 = 100;
    pDebug->mPolyFt4.y2 = 100;

    pDebug->mPolyFt4.x3 = 250;
    pDebug->mPolyFt4.y3 = 250;

    return 0;
}

void AddDebugActor()
{
    Actor_Debug* pDebug = Actor_ResourceAllocT<Actor_Debug>(1);
    if (pDebug)
    {
        Actor_Init(&pDebug->mBase,
            reinterpret_cast<TActorFunction>(Debug_Update),
            reinterpret_cast<TActorFunction>(Debug_Shutdown),
            __FILE__);

        if (Debug_Loader(pDebug) < 0)
        {
            Actor_DestroyOnNextUpdate(&pDebug->mBase);
        }
    }
}
