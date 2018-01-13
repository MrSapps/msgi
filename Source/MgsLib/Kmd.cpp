#include "stdafx.h"
#include "Kmd.hpp"
#include "System.hpp"
#include "LibDG.hpp"

#define KMD_IMPL true

void ForceLinkKmdCpp();

struct KmdHeader
{

};

struct kmdObject
{

};

// TODO: Reverse KMD loading as it seems very closely linked to the GV lib prim rendering/queues which is the linked to the GTE emu

/*
Prim_unknown* CC PrimObj_Alloc_443FEC(KmdHeader* pFileData, int countOrType_0x40Flag, __int16 alwaysZero)
{
    primSize = 92 * pFileData->mUnknown + 72;     // 72 bytes header and 92 bytes * num items other data
    pAllocated = (Prim_unknown *)System_2_zerod_allocate_memory_40B296(primSize);
    pAllocated2 = pAllocated;
    if (pAllocated)
    {
        MemClearUnknown_40B231(pAllocated, primSize);
        memcpy(pAllocated2, dword_6501F8, 32u);
        pAllocated2->field_24_maybe_flags = (int)pFileData;
        pAllocated2->field_2E_w_or_h = pFileData->NumberOfObjects;// mUnknown ?
        *(DWORD *)&pAllocated2->field_28_dword_9942A0 = countOrType_0x40Flag;
        pAllocated2->field_30_size = alwaysZero;
        pAllocated2->field_34 = (DWORD)&qword_650128;
        pAfterHeader = (kmdObject *)&pFileData[1];
        if (pFileData->mUnknown > 0)
        {
            p92Struct = (int)&pAllocated2[1].field_34;
            count = (KmdHeader *)pFileData->mUnknown;
            do
            {
                *(DWORD *)(p92Struct - 8) = *(DWORD *)pAfterHeader;
                objPosNum = pAfterHeader->mObjPosNum_30;
                if (objPosNum >= 0)
                    *(DWORD *)p92Struct = *(DWORD*)(char *)&pAllocated2[objPosNum] + 72;
                else
                    *(DWORD *)p92Struct = 0;
                *(WORD *)(p92Struct + 8) = Prim_444096((unsigned int *)pAfterHeader);
                *(WORD *)(p92Struct + 10) = pAfterHeader->field_4;
                p92Struct += 0x5C;
                ++pAfterHeader;
                count = (KmdHeader *)((char *)count - 1);
            } while (count);
        }
        pAllocated = pAllocated2;
    }
    return pAllocated;
}
MGS_FUNC_IMPLEX(0x443FEC, PrimObj_Alloc_443FEC, KMD_IMPL);
*/
