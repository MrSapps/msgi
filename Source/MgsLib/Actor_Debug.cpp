#include "stdafx.h"
#include "Actor_Debug.hpp"
#include "ResourceNameHash.hpp"
#include "LibDG.hpp"
#include "Actor_Rank.hpp"



MGS_VAR_EXTERN(int, gActiveBuffer_dword_791A08);

static void CC Debug_Update(Actor_Debug* pDebug)
{
    TextSetXYFlags_459B0B(181, 47, 0);
    TextSetRGB_459B27(255, 255, 255);
    Menu_DrawText("A textual test");

    memcpy((pDebug->mPrimData /*+ gActiveBuffer_dword_791A08*/)->field_40_pDataStart, &pDebug->mPolyF4, sizeof(POLY_FT4));
    pDebug->mPrimData->mBase.field_0 = 3;

}

static void CC Debug_Shutdown(Actor_Debug*)
{

}
MGS_VAR_EXTERN(u32, dword_9942A0);

static int CC Debug_Loader(Actor_Debug* pDebug)
{
    pDebug->mPolyF4 = {};

    int numPrims = 1;
    Prim_unknown* pPrim16Data = PrimAlloc_405050(2066, numPrims, 0, 0, 0);
    if (pPrim16Data)
    {
        PrimAdd_401805(pPrim16Data);
        pPrim16Data->field_28_dword_9942A0 = (signed __int16)dword_9942A0;
    }
    pDebug->mPrimData = pPrim16Data;

    //const WORD resHash = ResourceNameHash("back_l");
    //Res_rank_prim_related_4767CE(nullptr, resHash, &pDebug->mPolyF4, -160, -112, 0, 112, 0, 0);
  
    pDebug->mPolyF4.tag = 0x9000000;
    pDebug->mPolyF4.code = 2;

    pDebug->mPolyF4.r0 = 255;
    pDebug->mPolyF4.g0 = 255;
    pDebug->mPolyF4.b0 = 255;
    
    pDebug->mPolyF4.x0 = 0;
    pDebug->mPolyF4.y0 = 0;

    pDebug->mPolyF4.x1 = 0;
    pDebug->mPolyF4.y1 = 100;

    pDebug->mPolyF4.x2 = 100;
    pDebug->mPolyF4.y2 = 100;

    pDebug->mPolyF4.x3= 0;
    pDebug->mPolyF4.y3 = 100;


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
