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

int __cdecl Prim_444096(kmdObject* a1)
{
    unsigned int v1; // ecx
    int result; // eax
    int v3; // edx

    v1 = a1->field_0;
    result = 0;
    if (v1 & 0x300)
    {
        v3 = (v1 >> 12) & 3;
        result = 250 * (4 - v3);
        if (!(v1 & 256))
            result = -250 * (4 - v3);
    }
    return result;
}
MGS_FUNC_IMPLEX(0x444096, Prim_444096, KMD_IMPL);

Prim_unknown_0x48* CC PrimObj_Alloc_443FEC(KmdHeader* pFileData, int countOrType_0x40Flag, __int16 usuallyZero)
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
            for (DWORD i=0; i<pFileData->mNumberOfMeshes; i++)
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
MGS_FUNC_IMPLEX(0x443FEC, PrimObj_Alloc_443FEC, KMD_IMPL);

void CC PrimObj_407BC1(Prim_unknown_0x48* pObj)
{
    memcpy(&pObj->field_0_matrix.m, gte_rotation_matrix_993E40.m, sizeof(MATRIX3x3));

    pObj->field_0_matrix.t[0] = gGte_translation_vector_993E54.x;
    pObj->field_0_matrix.t[1] = gGte_translation_vector_993E54.y;
    pObj->field_0_matrix.t[2] = gGte_translation_vector_993E54.z;
}
MGS_FUNC_IMPLEX(0x407BC1, PrimObj_407BC1, KMD_IMPL);
