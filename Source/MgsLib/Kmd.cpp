#include "stdafx.h"
#include "Kmd.hpp"
#include "System.hpp"
#include "LibDG.hpp"
#include "System.hpp"
#include "Script.hpp"

#define KMD_IMPL true

void ForceLinkKmdCpp() { }

struct kmdObject;

struct Prim_Mesh_0x5C
{
    DWORD field_0;
    DWORD field_4;
    DWORD field_8;
    DWORD field_C;
    DWORD field_10;
    DWORD field_14;
    DWORD field_18;
    DWORD field_1C;
    DWORD field_20;
    DWORD field_24;
    DWORD field_28;
    DWORD field_2C;
    DWORD field_30;
    DWORD field_34;
    DWORD field_38;
    DWORD field_3C;
    kmdObject* field_40_pKmdObj;
    DWORD field_44;
    Prim_Mesh_0x5C* field_48_pLinked;
    DWORD field_4C;
    WORD field_50;
    WORD field_52;
    DWORD field_54;
    DWORD field_58;
};
MGS_ASSERT_SIZEOF(Prim_Mesh_0x5C, 0x5C);

struct KmdHeader
{
    DWORD mUnknownOrNumFaces;
    DWORD mNumberOfMeshes;
    DWORD x_coord1;
    DWORD y_coord1;
    DWORD z_coord1;
    DWORD x_coord2;
    DWORD y_coord2;
    DWORD z_coord2;
};
MGS_ASSERT_SIZEOF(KmdHeader, 0x20);

struct kmdObject
{
    DWORD field_0;
    DWORD field_4;
    DWORD x1_8;
    DWORD y1_C;
    DWORD z1_10;
    DWORD x2_14;
    DWORD y2_18;
    DWORD z2_1C;
    DWORD horizontal_pad_20;
    DWORD mVertPadding_24;
    DWORD mDeepPadding_28;
    DWORD mRef_2C;
    int mObjPosNum_30;
    DWORD mFFFFFFFF_34;
    DWORD mNumVerts_38;
    DWORD mVertCoordsOffset_3C;
    DWORD mVertexOrderOffset_40;
    DWORD mNumNormals_44;
    DWORD mNormalVertexCoordsOffset_48;
    DWORD mNormalVertexOrderOffset_4C;
    DWORD mTextureVertexOffset_50;
    DWORD mTextureNameOffset_54;
};
MGS_ASSERT_SIZEOF(kmdObject, 0x58);

// TODO: Reverse KMD loading as it seems very closely linked to the GV lib prim rendering/queues which is the linked to the GTE emu

int __cdecl Prim_444096(kmdObject* pKmdObj)
{
    DWORD field_0 = pKmdObj->field_0;
    int result = 0;
    if (field_0 & 768)
    {
        DWORD field_0_bits = (field_0 >> 12) & 3;
        result = 250 * (4 - field_0_bits);
        if (!(field_0 & 256))
        {
            result = -250 * (4 - field_0_bits);
        }
    }
    return result;
}
MGS_FUNC_IMPLEX(0x444096, Prim_444096, KMD_IMPL);

void CC PrimObj_407BC1(Prim_unknown_0x48* pObj)
{
    memcpy(&pObj->field_0_matrix.m, gte_rotation_matrix_993E40.m, sizeof(MATRIX3x3));

    pObj->field_0_matrix.t[0] = gGte_translation_vector_993E54.x;
    pObj->field_0_matrix.t[1] = gGte_translation_vector_993E54.y;
    pObj->field_0_matrix.t[2] = gGte_translation_vector_993E54.z;
}
MGS_FUNC_IMPLEX(0x407BC1, PrimObj_407BC1, KMD_IMPL);

struct PrimUnknownData
{
    char field_0;
    char field_1;
    char field_2;
    char field_3;
};
MGS_ASSERT_SIZEOF(PrimUnknownData, 0x4);

// Data maybe related to Resetgraph_AndPrintPsxStructureSizes() ?
MGS_ARY(1, 0x650194, PrimUnknownData, 25, byte_650194,
{
    { 16,   2,   8,   4 },
    { 24,   3,   8,   4 },
    { 28,   4,   8,   4 },
    { 20,   2,   8,   8 },
    { 32,   3,   8,   8 },
    { 40,   4,   8,   8 },
    { 20,   1,   8,   0 },
    { 16,   1,   8,   0 },
    { 16,   1,   8,   0 },
    { 16,   1,   8,   0 },
    { 12,   1,   8,   0 },
    { 12,   1,   8,   0 },
    { 12,   1,   8,   0 },
    { 20,   3,   8,   4 },
    { 24,   4,   8,   4 },
    { 28,   3,   8,   8 },
    { 36,   4,   8,   8 },
    { 32,   3,   8,   8 },
    { 40,   4,   8,   8 },
    { 40,   3,   8,  12 },
    { 52,   4,   8,  12 },
    { 40,   2,   8,   8 },
    { 52,   2,   8,  12 },
    { 12,   1,   8,   0 },
    { 0,    0,   0,   0 }
});

// Dynamically sets the last item of byte_650194
void CC sub_40514F(char a1, char a2, char a3, char a4)
{
    byte_650194[23].field_0 = a1;
    byte_650194[23].field_1 = a2;
    byte_650194[23].field_2 = a3;
    byte_650194[23].field_3 = a4;
}
MGS_FUNC_IMPLEX(0x40514F, sub_40514F, KMD_IMPL);

Prim_unknown_0x48* CC Obj_Alloc_443FEC(KmdHeader* pFileData, int countOrType_0x40Flag, __int16 usuallyZero)
{
    const int primSize = sizeof(Prim_unknown_0x48) + (sizeof(Prim_Mesh_0x5C) * pFileData->mNumberOfMeshes);
    Prim_unknown_0x48* pAllocated = (Prim_unknown_0x48 *)System_2_zerod_allocate_memory_40B296(primSize);
    if (pAllocated)
    {
        MemClearUnknown_40B231(pAllocated, primSize);
        memcpy(&pAllocated->field_0_matrix, &gIdentity_matrix_6501F8, sizeof(PSX_MATRIX));
        pAllocated->field_24_pKmdFileData = pFileData;
        pAllocated->field_2E_UnknownOrNumFaces = static_cast<WORD>(pFileData->mUnknownOrNumFaces);
        pAllocated->field_28_flags_or_type = countOrType_0x40Flag;
        pAllocated->field_30_size = usuallyZero;
        pAllocated->field_34_pVec = &gLightNormalVec_650128;

        kmdObject* pKmdObject = (kmdObject *)&pFileData[1];
        if (pFileData->mNumberOfMeshes > 0)
        {
            Prim_Mesh_0x5C* pFirstMesh = reinterpret_cast<Prim_Mesh_0x5C*>(&pAllocated[1]);
            for (DWORD i = 0; i < pFileData->mNumberOfMeshes; i++)
            {
                pFirstMesh[i].field_40_pKmdObj = &pKmdObject[i];
                if (pKmdObject[i].mObjPosNum_30 >= 0)
                {
                    pFirstMesh[i].field_48_pLinked = &pFirstMesh[pKmdObject[i].mObjPosNum_30];
                }
                else
                {
                    pFirstMesh[i].field_48_pLinked = nullptr;
                }

                pFirstMesh[i].field_50 = static_cast<WORD>(Prim_444096(&pKmdObject[i]));
                pFirstMesh[i].field_52 = static_cast<WORD>(pKmdObject[i].field_4);
            }
        }
    }
    return pAllocated;
}
MGS_FUNC_IMPLEX(0x443FEC, Obj_Alloc_443FEC, KMD_IMPL);

Prim_unknown_0x54* CC PrimAlloc_405050(int maybeFlags, int numItems, __int16 gv_index, int size, int field_3C)
{
    const int idx = (maybeFlags & 31);
    assert(idx < 25);

    const PrimUnknownData* pData = &byte_650194[idx];
    const int baseSize2 = numItems * pData->field_0;

    // allocate double amount for active buffer switching?

    // TODO: Allocating 0x54, not 0x48!
    // alloc 12 more bytes? 12/4=3 dwords
    Prim_unknown_0x54* pMem = (Prim_unknown_0x54 *)System_2_zerod_allocate_memory_40B296(2 * baseSize2 + sizeof(Prim_unknown_0x54));
    if (pMem)
    {
        MemClearUnknown_40B231(pMem, sizeof(Prim_unknown_0x54));
        memcpy(&pMem->field_0_matrix, &gIdentity_matrix_6501F8, sizeof(PSX_MATRIX));
        pMem->field_24_pKmdFileData = (KmdHeader*)maybeFlags;
        pMem->field_2A = static_cast<WORD>(numItems);
        pMem->field_2C_index = gv_index;
        pMem->field_38_size24b = size;
        pMem->field_3C = field_3C;
        pMem->field_30_size = pData->field_0;
        pMem->field_32 = pData->field_1;
        pMem->field_34 = pData->field_2;
        pMem->field_36 = pData->field_3;

        // Point into the extra allocated space after the structure
        BYTE* endPtr = reinterpret_cast<BYTE*>(&pMem[1]);
        pMem->field_40_pDataStart[0] = endPtr;
        pMem->field_40_pDataStart[1] = endPtr + baseSize2;
    }
    return pMem;
}
MGS_FUNC_IMPLEX(0x405050, PrimAlloc_405050, KMD_IMPL);

signed int CC PrimAdd_401805(Prim_unknown_0x54* pPrimBuffer)
{
    assert(pPrimBuffer->field_2C_index == 0);
    struct_gv* pGv = &gLibGVStruct1_6BC36C + pPrimBuffer->field_2C_index; // Always 0?
    if (pGv->gPrimQueue2_word_6BC3C0_256 > pGv->gObjectQueue_word_6BC3C2_0)
    {
        pGv->gObjects_dword_6BC3C4[pGv->gPrimQueue2_word_6BC3C0_256 - 1] = pPrimBuffer; // PrimObject = Prim_unknown + extra ??
        pGv->gPrimQueue2_word_6BC3C0_256--;
        return 0;
    }
    return -1;
}
MGS_FUNC_IMPLEX(0x401805, PrimAdd_401805, KMD_IMPL);

int CC Object_Add_40178F(Prim_unknown_0x48* pPrim)
{
    assert(pPrim->field_30_size == 0);
    struct_gv* pGv = &gLibGVStruct1_6BC36C + pPrim->field_30_size;
    if (pGv->gObjectQueue_word_6BC3C2_0 >= pGv->gPrimQueue2_word_6BC3C0_256)
    {
        return -1;
    }
    pGv->gObjects_dword_6BC3C4[pGv->gObjectQueue_word_6BC3C2_0++] = (Prim_unknown_0x54 *)pPrim;
    return 0;
}
MGS_FUNC_IMPLEX(0x40178F, Object_Add_40178F, KMD_IMPL);
