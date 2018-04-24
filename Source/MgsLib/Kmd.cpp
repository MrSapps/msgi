#include "stdafx.h"
#include "Kmd.hpp"
#include "System.hpp"
#include "LibDG.hpp"
#include "System.hpp"
#include "Script.hpp"
#include "ResourceNameHash.hpp"
#include "Psx.hpp"
#include "Table_665A3C.hpp"

#define KMD_IMPL true

void ForceLinkKmdCpp() { }

struct kmdObject;


// TODO: Reverse KMD loading as it seems very closely linked to the GV lib prim rendering/queues which is the linked to the GTE emu

int CC Prim_444096(kmdObject* pKmdObj)
{
    DWORD field_0 = pKmdObj->field_0_numObj;
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


struct PrimUnknownData
{
    char field_0_prim_type_size;
    char field_1_vert_count;
    char field_2;
    char field_3;
};
MGS_ASSERT_SIZEOF(PrimUnknownData, 0x4);

// Data maybe related to Resetgraph_AndPrintPsxStructureSizes() ?
MGS_ARY(1, 0x650194, PrimUnknownData, 25, byte_650194,
{
    // header size ?
    // size per attribute
    { 16,   2,   8,   4 }, // LINE_F2
    { 24,   3,   8,   4 }, // LINE_F3
    { 28,   4,   8,   4 }, // LINE_F4
    { 20,   2,   8,   8 }, // LINE_G2
    { 32,   3,   8,   8 }, // LINE_G3
    { 40,   4,   8,   8 }, // LINE_G4
    { 20,   1,   8,   0 }, // SPRT
    { 16,   1,   8,   0 }, // SPRT_16 ?
    { 16,   1,   8,   0 }, // SPRT_8 ?
    { 16,   1,   8,   0 }, // TILE ?
    { 12,   1,   8,   0 }, // TILE_16 ?
    { 12,   1,   8,   0 }, // TILE_8 ?
    { 12,   1,   8,   0 }, // TILE_1 ?
    { 20,   3,   8,   4 }, // POLY_F3
    { 24,   4,   8,   4 }, // POLY_F4
    { 28,   3,   8,   8 }, // POLY_G3
    { 36,   4,   8,   8 }, // POLY_G4
    { 32,   3,   8,   8 }, // POLY_FT3
    { 40,   4,   8,   8 }, // POLY_FT4
    { 40,   3,   8,  12 }, // POLY_GT3
    { 52,   4,   8,  12 }, // POLY_GT4
    { 40,   2,   8,   8 }, // ?
    { 52,   2,   8,  12 }, // poly line?
    { 12,   1,   8,   0 }, // ?
    { 0,    0,   0,   0 }  // dynamically set
});

// Dynamically sets the last item of byte_650194
void CC sub_40514F(char sizeInBytes, char vertCount, char a3, char a4)
{
    byte_650194[23].field_0_prim_type_size = sizeInBytes;
    byte_650194[23].field_1_vert_count = vertCount;
    byte_650194[23].field_2 = a3;
    byte_650194[23].field_3 = a4;
}
MGS_FUNC_IMPLEX(0x40514F, sub_40514F, KMD_IMPL);

Prim_Union* CC Obj_Alloc_443FEC(KmdHeader* pFileData, int countOrType_0x40Flag, __int16 usuallyZero)
{
    const int primSize = sizeof(Prim_unknown_0x48) + (sizeof(Prim_Mesh_0x5C) * pFileData->mNumberOfObjects);
    Prim_unknown_0x48* pAllocated = (Prim_unknown_0x48 *)System_2_zerod_allocate_memory_40B296(primSize);
    if (pAllocated)
    {
        MemClearUnknown_40B231(pAllocated, primSize);
        memcpy(&pAllocated->field_0_matrix, &gIdentity_matrix_6501F8, sizeof(PSX_MATRIX));
        pAllocated->field_24_pKmdFileData = pFileData;
        pAllocated->field_2E_UnknownOrNumFaces = static_cast<WORD>(pFileData->mNumberOfMeshes);
        pAllocated->field_28_flags_or_type = countOrType_0x40Flag;
        pAllocated->field_30_size = usuallyZero;
        pAllocated->field_34_light_mtx_array = &gLightNormalVec_650128;

        kmdObject* pKmdObject = (kmdObject *)&pFileData[1];
        if (pFileData->mNumberOfObjects > 0)
        {
            Prim_Mesh_0x5C* pFirstMesh = reinterpret_cast<Prim_Mesh_0x5C*>(&pAllocated[1]);
            for (DWORD i = 0; i < pFileData->mNumberOfObjects; i++)
            {
                pFirstMesh[i].field_40_pKmdObj = &pKmdObject[i];
                if (pKmdObject[i].mObjPosNum_30_translationUnk >= 0)
                {
                    pFirstMesh[i].field_48_pLinked = &pFirstMesh[pKmdObject[i].mObjPosNum_30_translationUnk];
                }
                else
                {
                    pFirstMesh[i].field_48_pLinked = nullptr;
                }

                pFirstMesh[i].field_50_numObjTranslated = static_cast<WORD>(Prim_444096(&pKmdObject[i]));
                pFirstMesh[i].field_52_num_faces = static_cast<WORD>(pKmdObject[i].field_4_numFaces);
            }
        }
    }
    return reinterpret_cast<Prim_Union*>(pAllocated);
}
MGS_FUNC_IMPLEX(0x443FEC, Obj_Alloc_443FEC, KMD_IMPL);

Prim_Union* CC PrimAlloc_405050(int maybeFlags, int numItems, __int16 gv_index, Prim_24b* size, void* field_3C)
{
    const int idx = (maybeFlags & 31);
    assert(idx < 25);

    const PrimUnknownData* pData = &byte_650194[idx];
    const int primBufferSize = numItems * pData->field_0_prim_type_size;

    // As the rendering is double buffered we allocate twice as much
    Prim_unknown_0x54* pMem = (Prim_unknown_0x54 *)System_2_zerod_allocate_memory_40B296((primBufferSize*2) + sizeof(Prim_unknown_0x54));
    if (pMem)
    {
        MemClearUnknown_40B231(pMem, sizeof(Prim_unknown_0x54));
        memcpy(&pMem->field_0_matrix, &gIdentity_matrix_6501F8, sizeof(PSX_MATRIX));
        pMem->field_24_flags2 = maybeFlags;
        pMem->field_2A_num_prims = static_cast<WORD>(numItems);
        pMem->field_2C_gv_index = gv_index;
        pMem->field_38_size24b = size;
        pMem->field_3C = field_3C;
        pMem->field_30_prim_size = pData->field_0_prim_type_size;
        pMem->field_32_primF2_vert_count = pData->field_1_vert_count;
        pMem->field_34_primF3 = pData->field_2;
        pMem->field_36_primF4 = pData->field_3;

        // Point into the extra allocated space after the structure
        BYTE* endPtr = reinterpret_cast<BYTE*>(&pMem[1]);
        pMem->field_40_pDataStart[0] = endPtr;                      // 1st prim buffer
        pMem->field_40_pDataStart[1] = endPtr + primBufferSize;     // 2nd prim buffer
    }
    return reinterpret_cast<Prim_Union*>(pMem);
}
MGS_FUNC_IMPLEX(0x405050, PrimAlloc_405050, KMD_IMPL);

signed int CC PrimAdd_401805(Prim_Union* pPrimBuffer)
{
    assert(pPrimBuffer->prim_54.field_2C_gv_index == 0);
    struct_gv* pGv = &gLibGVStruct1_6BC36C + pPrimBuffer->prim_54.field_2C_gv_index; // Always 0?
    if (pGv->gPrimQueue2_word_6BC3C0_256 > pGv->gObjectQueue_word_6BC3C2_0)
    {
        pGv->gObjects_dword_6BC3C4[pGv->gPrimQueue2_word_6BC3C0_256 - 1] = pPrimBuffer; // PrimObject = Prim_unknown + extra ??
        pGv->gPrimQueue2_word_6BC3C0_256--;
        return 0;
    }
    return -1;
}
MGS_FUNC_IMPLEX(0x401805, PrimAdd_401805, KMD_IMPL);

int CC Object_Add_40178F(Prim_Union* pPrim)
{
    assert(pPrim->prim_48.field_30_size == 0);
    struct_gv* pGv = &gLibGVStruct1_6BC36C + pPrim->prim_48.field_30_size;
    if (pGv->gObjectQueue_word_6BC3C2_0 >= pGv->gPrimQueue2_word_6BC3C0_256)
    {
        return -1;
    }
    pGv->gObjects_dword_6BC3C4[pGv->gObjectQueue_word_6BC3C2_0++] = pPrim;
    return 0;
}
MGS_FUNC_IMPLEX(0x40178F, Object_Add_40178F, KMD_IMPL);

void CC Vector_subtract_40B4ED(const SVECTOR* aLhs, const SVECTOR* aRhs, SVECTOR* out)
{
    out->field_0_x = aLhs->field_0_x - aRhs->field_0_x;
    out->field_2_y = aLhs->field_2_y - aRhs->field_2_y;
    out->field_4_z = aLhs->field_4_z - aRhs->field_4_z;
}
MGS_FUNC_IMPLEX(0x40B4ED, Vector_subtract_40B4ED, KMD_IMPL);

void CC Vector_add_40B4BD(const SVECTOR*pRhs, const SVECTOR* pLhs, SVECTOR* pOut)
{
    pOut->field_0_x = pLhs->field_0_x + pRhs->field_0_x;
    pOut->field_2_y = pLhs->field_2_y + pRhs->field_2_y;
    pOut->field_4_z = pLhs->field_4_z + pRhs->field_4_z;
}
MGS_FUNC_IMPLEX(0x40B4BD, Vector_add_40B4BD, KMD_IMPL);

__int64 CC j_sqrt(signed int value)
{
    return (__int64)sqrt(static_cast<float>(value));
}
MGS_FUNC_IMPLEX(0x44ADB0, j_sqrt, KMD_IMPL);

__int64 CC Vector_unknown_40B51D(const SVECTOR* pVec)
{
    VECTOR3 vec = {};
    vec.x = pVec->field_0_x;
    vec.y = pVec->field_2_y;
    vec.z = pVec->field_4_z;
    Psx_gte_sqr0_44B030(&vec, &vec);
    return j_sqrt(vec.x + vec.y + vec.z);
}
MGS_FUNC_IMPLEX(0x40B51D, Vector_unknown_40B51D, KMD_IMPL);

void CC Vector_unknown_40B55D(const SVECTOR* pVec, SVECTOR* pOut, signed int value1, int value2)
{
    const int fixedWholeNum = value2 << 12;
    if (!value1)
    {
        value1 = 1;
    }
    pOut->field_0_x = static_cast<short int>(fixedWholeNum / value1 * pVec->field_0_x / 4096);
    pOut->field_2_y = static_cast<short int>(fixedWholeNum / value1 * pVec->field_2_y / 4096);
    pOut->field_4_z = static_cast<short int>(fixedWholeNum / value1 * pVec->field_4_z / 4096);
}
MGS_FUNC_IMPLEX(0x40B55D, Vector_unknown_40B55D, false);

int CC FixedSubtract_40B6BD(__int16 value1, __int16 value2)
{
    return (value2 - value1) & 4095;
}
MGS_FUNC_IMPLEX(0x40B6BD, FixedSubtract_40B6BD, KMD_IMPL);


void CC Res_base_unknown_407B3D(const SVECTOR* pVec)
{
    gGte_VXY0_993EC0.regs.VX = pVec->field_0_x;
    gGte_VXY0_993EC0.regs.VY = pVec->field_2_y;
    gGte_VXY0_993EC0.regs.VZ = pVec->field_4_z;
    gGte_VXY0_993EC0.regs.Zero = pVec->field_6_padding;

    Psx_gte_RT1TR_rt_4477A0(); // Also called MVMVA?
    gGte_translation_vector_993E54.x = gGte_MAC1_993F24.MAC_32;
    gGte_translation_vector_993E54.y = gGte_MAC2_993F28.MAC_32;
    gGte_translation_vector_993E54.z = gGte_MAC3_993F2C.MAC_32;
}
MGS_FUNC_IMPLEX(0x407B3D, Res_base_unknown_407B3D, KMD_IMPL);

void CC sub_44B690(PSX_MATRIX* pMtx)
{
    for (int i = 0; i < 3; i++)
    {
        gGte_IR1_993EE4.IR_32 = (unsigned __int16)pMtx->m[0][i];
        gGte_IR2_993EE8.IR_32 = (unsigned __int16)pMtx->m[1][i];
        gGte_IR3_993EEC.IR_32 = (unsigned __int16)pMtx->m[2][i];
        Psx_gte_RT1_rtir_447480();
        pMtx->m[0][i] = gGte_IR1_993EE4.IR_16;
        pMtx->m[1][i] = gGte_IR2_993EE8.IR_16;
        pMtx->m[2][i] = gGte_IR3_993EEC.IR_16;
    }
}
MGS_FUNC_IMPLEX(0x44B690, sub_44B690, KMD_IMPL);

#define BOXKERI_IMPL true

MGS_VAR(1, 0x9942B0, SVECTOR, gSnakePos_stru_9942B0, {});

const MATRIX3x3 gIdentityMatrix_6659BC ={ { { 4096, 0, 0 },{ 0, 4096, 0 },{ 0, 0, 4096 } } };

void CC RotMatrixY_44C270(__int16 value, PSX_MATRIX* pMtx)
{
    const int tbl1 = dword_665A3C[(value + 1024) & 4095];
    const int tbl2 = dword_665A3C[value & 4095];

    const int m_2_0 = pMtx->m[2][0];
    const int m_0_0 = pMtx->m[0][0];
    pMtx->m[2][0] = static_cast<short>((tbl1 * m_2_0 - tbl2 * m_0_0) >> 12);
    pMtx->m[0][0] = static_cast<short>((tbl2 * m_2_0 + tbl1 * m_0_0) >> 12);

    const int m_2_1 = pMtx->m[2][1];
    const int m_0_1 = pMtx->m[0][1];
    pMtx->m[2][1] = static_cast<short>((tbl1 * m_2_1 - tbl2 * m_0_1) >> 12);
    pMtx->m[0][1] = static_cast<short>((tbl2 * m_2_1 + tbl1 * m_0_1) >> 12);

    const int m_2_2 = pMtx->m[2][2];
    const int m_0_2 = pMtx->m[0][2];
    pMtx->m[2][2] = static_cast<short>((tbl1 * m_2_2 - tbl2 * m_0_2) >> 12);
    pMtx->m[0][2] = static_cast<short>((tbl2 * m_2_2 + tbl1 * m_0_2) >> 12);
}
MGS_FUNC_IMPLEX(0x44C270, RotMatrixY_44C270, KMD_IMPL);

void CC RotMatrixX_44C1C0(__int16 value, PSX_MATRIX* pMatrix)
{
    const int tbl1 = dword_665A3C[(value + 1024) & 4095];
    const int tbl2 = dword_665A3C[value & 4095];

    const int m_1_0 = pMatrix->m[1][0];
    const int m_2_0 = pMatrix->m[2][0];
    pMatrix->m[2][0] = static_cast<short>((tbl2 * m_1_0 + tbl1 * m_2_0) >> 12);
    pMatrix->m[1][0] = static_cast<short>((tbl1 * m_1_0 - tbl2 * m_2_0) >> 12);

    const int m_1_1 = pMatrix->m[1][1];
    const int m_2_1 = pMatrix->m[2][1];
    pMatrix->m[2][1] = static_cast<short>((tbl2 * m_1_1 + tbl1 * m_2_1) >> 12);
    pMatrix->m[1][1] = static_cast<short>((tbl1 * m_1_1 - tbl2 * m_2_1) >> 12);

    const int m_1_2 = pMatrix->m[1][2];
    const int m_2_2 = pMatrix->m[2][2];
    pMatrix->m[2][2] = static_cast<short>((tbl2 * m_1_2 + tbl1 * m_2_2) >> 12);
    pMatrix->m[1][2] = static_cast<short>((tbl1 * m_1_2 - tbl2 * m_2_2) >> 12);
}
MGS_FUNC_IMPLEX(0x44C1C0, RotMatrixX_44C1C0, KMD_IMPL);

void CC RotMatrixZ_44C320(__int16 value, PSX_MATRIX* pMatrix)
{
    const int tbl1 = dword_665A3C[(value + 1024) & 4095];
    const int tbl2 = dword_665A3C[value & 4095];

    const int m_0_0 = pMatrix->m[0][0];
    const int m_1_0 = pMatrix->m[1][0];
    pMatrix->m[1][0] = static_cast<short>((tbl2 * m_0_0 + tbl1 * m_1_0) >> 12);
    pMatrix->m[0][0] = static_cast<short>((tbl1 * m_0_0 - tbl2 * m_1_0) >> 12);

    const int m_0_1 = pMatrix->m[0][1];
    const int m_1_1 = pMatrix->m[1][1];
    pMatrix->m[1][1] = static_cast<short>((tbl2 * m_0_1 + tbl1 * m_1_1) >> 12);
    pMatrix->m[0][1] = static_cast<short>((tbl1 * m_0_1 - tbl2 * m_1_1) >> 12);

    const int m_0_2 = pMatrix->m[0][2];
    const int m_1_2 = pMatrix->m[1][2];
    pMatrix->m[1][2] = static_cast<short>((tbl2 * m_0_2 + tbl1 * m_1_2) >> 12);
    pMatrix->m[0][2] = static_cast<short>((tbl1 * m_0_2 - tbl2 * m_1_2) >> 12);
}
MGS_FUNC_IMPLEX(0x44C320, RotMatrixZ_44C320, KMD_IMPL);


void CC RotMatrixZYX_gte_44C880(const SVECTOR* pVec, MATRIX3x3* pMtx)
{
    memcpy(&pMtx->m, &gIdentityMatrix_6659BC.m, sizeof(MATRIX3x3::m));

    // ============================================================================

    {
        const int tab_vec_x_1 = dword_665A3C[(pVec->field_0_x + 1024) & 4095];
        const int tab_vex_x_2 = dword_665A3C[pVec->field_0_x & 4095];

        const int mtx_1_0 = pMtx->m[1][0];
        const int mtx_2_0 = pMtx->m[2][0];

        pMtx->m[2][0] = static_cast<short>((tab_vex_x_2 * mtx_1_0 + tab_vec_x_1 * mtx_2_0) >> 12);
        pMtx->m[1][0] = static_cast<short>((tab_vec_x_1 * mtx_1_0 - tab_vex_x_2 * mtx_2_0) >> 12);

        const int mtx_1_1 = pMtx->m[1][1];
        const int mtx_2_1 = pMtx->m[2][1];

        pMtx->m[1][1] = static_cast<short>((tab_vec_x_1 * mtx_1_1 - tab_vex_x_2 * mtx_2_1) >> 12);
        pMtx->m[2][1] = static_cast<short>((tab_vex_x_2 * mtx_1_1 + tab_vec_x_1 * mtx_2_1) >> 12);

        const int mtx_2_2 = pMtx->m[2][2];
        const int mtx_1_2 = pMtx->m[1][2];

        pMtx->m[1][2] = static_cast<short>((tab_vec_x_1 * mtx_1_2 - tab_vex_x_2 * mtx_2_2) >> 12);
        pMtx->m[2][2] = static_cast<short>((tab_vex_x_2 * mtx_1_2 + tab_vec_x_1 * mtx_2_2) >> 12);
    }

    // ============================================================================

    {
        const int tab_vec_y_1 = dword_665A3C[(pVec->field_2_y + 1024) & 4095];
        const int tab_vec_y_2 = dword_665A3C[pVec->field_2_y & 4095];

        const int mtx_0_0 = pMtx->m[0][0];
        const int mtx_2_0 = pMtx->m[2][0];
        pMtx->m[0][0] = static_cast<short>((tab_vec_y_2 * mtx_2_0 + tab_vec_y_1 * mtx_0_0) >> 12);
        pMtx->m[2][0] = static_cast<short>((tab_vec_y_1 * mtx_2_0 - tab_vec_y_2 * mtx_0_0) >> 12);

        const int mtx_0_1 = pMtx->m[0][1];
        const int mtx_2_1 = pMtx->m[2][1];
        pMtx->m[0][1] = static_cast<short>((tab_vec_y_2 * mtx_2_1 + tab_vec_y_1 * mtx_0_1) >> 12);
        pMtx->m[2][1] = static_cast<short>((tab_vec_y_1 * mtx_2_1 - tab_vec_y_2 * mtx_0_1) >> 12);

        const int mtx_0_2 = pMtx->m[0][2];
        const int mtx_2_2 = pMtx->m[2][2];
        pMtx->m[2][2] = static_cast<short>((tab_vec_y_1 * mtx_2_2 - tab_vec_y_2 * mtx_0_2) >> 12);
        pMtx->m[0][2] = static_cast<short>((tab_vec_y_2 * mtx_2_2 + tab_vec_y_1 * mtx_0_2) >> 12);
    }

    // ============================================================================

    {
        const int tab_vec_z_1 = dword_665A3C[pVec->field_4_z & 4095];
        const int tab_vec_z_2 = dword_665A3C[(pVec->field_4_z + 1024) & 4095];

        const int mtx_0_0 = pMtx->m[0][0];
        const int mtx_1_0 = pMtx->m[1][0];
        pMtx->m[0][0] = static_cast<short>((tab_vec_z_2 * mtx_0_0 - tab_vec_z_1 * mtx_1_0) >> 12);
        pMtx->m[1][0] = static_cast<short>((tab_vec_z_1 * mtx_0_0 + tab_vec_z_2 * mtx_1_0) >> 12);

        const int mtx_0_1 = pMtx->m[0][1];
        const int mtx_1_1 = pMtx->m[1][1];
        pMtx->m[0][1] = static_cast<short>((tab_vec_z_2 * mtx_0_1 - tab_vec_z_1 * mtx_1_1) >> 12);
        pMtx->m[1][1] = static_cast<short>((tab_vec_z_1 * mtx_0_1 + tab_vec_z_2 * mtx_1_1) >> 12);

        const int mtx_0_2 = pMtx->m[0][2];
        const int mtx_1_2 = pMtx->m[1][2];
        pMtx->m[0][2] = static_cast<short>((tab_vec_z_2 * mtx_0_2 - tab_vec_z_1 * mtx_1_2) >> 12);
        pMtx->m[1][2] = static_cast<short>((tab_vec_z_1 * mtx_0_2 + tab_vec_z_2 * mtx_1_2) >> 12);
    }
}
MGS_FUNC_IMPLEX(0x44C880, RotMatrixZYX_gte_44C880, KMD_IMPL);

void CC VectorRotationMatrix_unknown_44C620(const SVECTOR* pVec, PSX_MATRIX* pMatrix)
{
    memcpy(pMatrix->m, gIdentityMatrix_6659BC.m, sizeof(MATRIX3x3::m));

    // ============================================================================

    const int tbl_vec_z_1 = dword_665A3C[(pVec->field_4_z + 1024) & 4095];
    const int tbl_vec_z_2 = dword_665A3C[pVec->field_4_z & 4095];

    const int matrix_0_0 = pMatrix->m[0][0];
    const int matrix_1_0 = pMatrix->m[1][0];
    pMatrix->m[1][0] = static_cast<short>((tbl_vec_z_2 * matrix_0_0 + tbl_vec_z_1 * matrix_1_0) >> 12);
    pMatrix->m[0][0] = static_cast<short>((tbl_vec_z_1 * matrix_0_0 - tbl_vec_z_2 * matrix_1_0) >> 12);

    const int matrix_0_1 = pMatrix->m[0][1];
    const int matrix_1_1 = pMatrix->m[1][1];
    pMatrix->m[0][1] = static_cast<short>((tbl_vec_z_1 * matrix_0_1 - tbl_vec_z_2 * matrix_1_1) >> 12);
    pMatrix->m[1][1] = static_cast<short>((tbl_vec_z_2 * matrix_0_1 + tbl_vec_z_1 * matrix_1_1) >> 12);

    const int matrix_1_2 = pMatrix->m[1][2];
    const int matrix_0_2 = pMatrix->m[0][2];
    pMatrix->m[1][2] = static_cast<short>((tbl_vec_z_2 * matrix_0_2 + tbl_vec_z_1 * matrix_1_2) >> 12);
    pMatrix->m[0][2] = static_cast<short>((tbl_vec_z_1 * matrix_0_2 - tbl_vec_z_2 * matrix_1_2) >> 12);

    // ============================================================================

    const int tbl_vec_x_1 = dword_665A3C[(pVec->field_0_x + 1024) & 4095];
    const int tbl_vec_x_2 = dword_665A3C[pVec->field_0_x & 4095];

    const int matrix_1_0_2 = pMatrix->m[1][0];
    const int matrix_2_0 = pMatrix->m[2][0];
    pMatrix->m[1][0] = static_cast<short>((tbl_vec_x_1 * matrix_1_0_2 - tbl_vec_x_2 * matrix_2_0) >> 12);
    pMatrix->m[2][0] = static_cast<short>((tbl_vec_x_2 * matrix_1_0_2 + tbl_vec_x_1 * matrix_2_0) >> 12);

    const int matrix_1_1_1 = pMatrix->m[1][1];
    const int matrix_2_1_1 = pMatrix->m[2][1];
    pMatrix->m[1][1] = static_cast<short>((tbl_vec_x_1 * matrix_1_1_1 - tbl_vec_x_2 * matrix_2_1_1) >> 12);
    pMatrix->m[2][1] = static_cast<short>((tbl_vec_x_2 * matrix_1_1_1 + tbl_vec_x_1 * matrix_2_1_1) >> 12);

    const int matrix_2_2_2 = pMatrix->m[2][2];
    pMatrix->m[1][2] = static_cast<short>((tbl_vec_x_1 * matrix_1_2 - tbl_vec_x_2 * matrix_2_2_2) >> 12);
    pMatrix->m[2][2] = static_cast<short>((tbl_vec_x_2 * matrix_1_2 + tbl_vec_x_1 * matrix_2_2_2) >> 12);

    // ============================================================================

    const int tbl_vec_y_1 = dword_665A3C[pVec->field_2_y & 4095];
    const int tbl_vec_y_2 = dword_665A3C[(pVec->field_2_y + 1024) & 4095];

    const int matrix_0_0_1 = pMatrix->m[0][0];
    const int matrix_2_0_1 = pMatrix->m[2][0];
    pMatrix->m[0][0] = static_cast<short>((tbl_vec_y_1 * matrix_2_0_1 + tbl_vec_y_2 * matrix_0_0_1) >> 12);
    pMatrix->m[2][0] = static_cast<short>((tbl_vec_y_2 * matrix_2_0_1 - tbl_vec_y_1 * matrix_0_0_1) >> 12);

    const int matrix_0_1_1 = pMatrix->m[0][1];
    const int matrix_2_1 = pMatrix->m[2][1];
    pMatrix->m[0][1] = static_cast<short>((tbl_vec_y_1 * matrix_2_1 + tbl_vec_y_2 * matrix_0_1_1) >> 12);
    pMatrix->m[2][1] = static_cast<short>((tbl_vec_y_2 * matrix_2_1 - tbl_vec_y_1 * matrix_0_1_1) >> 12);

    const int matrix_0_2_1 = pMatrix->m[0][2];
    const int matrix_2_2_1 = pMatrix->m[2][2];
    pMatrix->m[0][2] = static_cast<short>((tbl_vec_y_1 * matrix_2_2_1 + tbl_vec_y_2 * matrix_0_2_1) >> 12);
    pMatrix->m[2][2] = static_cast<short>((tbl_vec_y_2 * matrix_2_2_1 - tbl_vec_y_1 * matrix_0_2_1) >> 12);
}
MGS_FUNC_IMPLEX(0x44C620, VectorRotationMatrix_unknown_44C620, KMD_IMPL);

void CC Res_base_unknown_407ADA(const SVECTOR* pVec1, SVECTOR* pVec2)
{
    PSX_MATRIX matrix = {};

    VectorRotationMatrix_unknown_44C620(pVec2, &matrix);

    memcpy(&gte_rotation_matrix_993E40.m, &matrix.m, sizeof(PSX_MATRIX::m));

    gGte_translation_vector_993E54.x = pVec1->field_0_x;
    gGte_translation_vector_993E54.y = pVec1->field_2_y;
    gGte_translation_vector_993E54.z = pVec1->field_4_z;
}
MGS_FUNC_IMPLEX(0x407ADA, Res_base_unknown_407ADA, KMD_IMPL);

void CC Res_base_unknown_407B79(const SVECTOR* pRotVec)
{
    PSX_MATRIX rotMatrix = {};
    VectorRotationMatrix_unknown_44C620(pRotVec, &rotMatrix);
    sub_44B690(&rotMatrix);
    memcpy(gte_rotation_matrix_993E40.m, rotMatrix.m, sizeof(MATRIX3x3::m));
}
MGS_FUNC_IMPLEX(0x407B79, Res_base_unknown_407B79, KMD_IMPL);

void CC RotMatrixYXZ_gte_44BD00(const SVECTOR* pVec, PSX_MATRIX* pMtx)
{
    memcpy(&pMtx->m, &gIdentityMatrix_6659BC.m, sizeof(MATRIX3x3::m));

    // ============================================================================
    {
        const int vec_z_table1 = dword_665A3C[(pVec->field_4_z + 1024) & 4095];
        const int vec_z_table2 = dword_665A3C[pVec->field_4_z & 4095];

        const int mtx_0_0 = pMtx->m[0][0];
        const int mtx_1_0 = pMtx->m[1][0];
        pMtx->m[0][0] = static_cast<short>((vec_z_table1 * mtx_0_0 - vec_z_table2 * mtx_1_0) >> 12);
        pMtx->m[1][0] = static_cast<short>((vec_z_table2 * mtx_0_0 + vec_z_table1 * mtx_1_0) >> 12);

        const int mtx_0_1 = pMtx->m[0][1];
        const int mtx_1_1 = pMtx->m[1][1];
        pMtx->m[0][1] = static_cast<short>((vec_z_table1 * mtx_0_1 - vec_z_table2 * mtx_1_1) >> 12);
        pMtx->m[1][1] = static_cast<short>((vec_z_table2 * mtx_0_1 + vec_z_table1 * mtx_1_1) >> 12);

        const int mtx_0_2 = pMtx->m[0][2];
        const int mtx_1_2 = pMtx->m[1][2];
        pMtx->m[0][2] = static_cast<short>((vec_z_table1 * mtx_0_2 - vec_z_table2 * mtx_1_2) >> 12);
        pMtx->m[1][2] = static_cast<short>((vec_z_table2 * mtx_0_2 + vec_z_table1 * mtx_1_2) >> 12);
    }

    // ============================================================================

    {
        const int vec_x_table1 = dword_665A3C[(pVec->field_0_x + 1024) & 4095];
        const int vec_x_table2 = dword_665A3C[pVec->field_0_x & 4095];

        const int mtx_1_0 = pMtx->m[1][0];
        const int mtx_2_0 = pMtx->m[2][0];
        pMtx->m[1][0] = static_cast<short>((vec_x_table1 * mtx_1_0 - vec_x_table2 * mtx_2_0) >> 12);
        pMtx->m[2][0] = static_cast<short>((vec_x_table2 * mtx_1_0 + vec_x_table1 * mtx_2_0) >> 12);

        const int mtx_1_1 = pMtx->m[1][1];
        const int mtx_2_1 = pMtx->m[2][1];
        pMtx->m[1][1] = static_cast<short>((vec_x_table1 * mtx_1_1 - vec_x_table2 * mtx_2_1) >> 12);
        pMtx->m[2][1] = static_cast<short>((vec_x_table2 * mtx_1_1 + vec_x_table1 * mtx_2_1) >> 12);

        const int mtx_1_2 = pMtx->m[1][2];
        const int mtx_2_2 = pMtx->m[2][2];
        pMtx->m[1][2] = static_cast<short>((vec_x_table1 * mtx_1_2 - vec_x_table2 * mtx_2_2) >> 12);
        pMtx->m[2][2] = static_cast<short>((vec_x_table2 * mtx_1_2 + vec_x_table1 * mtx_2_2) >> 12);
    }

    // ============================================================================

    {
        const int tbl2 = dword_665A3C[(pVec->field_2_y + 1024) & 4095];
        const int tbl1 = dword_665A3C[pVec->field_2_y & 4095];

        const int mtx_0_0 = pMtx->m[0][0];
        const int mtx_2_0 = pMtx->m[2][0];
        pMtx->m[0][0] = static_cast<short>((tbl1 * mtx_2_0 + tbl2 * mtx_0_0) >> 12);
        pMtx->m[2][0] = static_cast<short>((tbl2 * mtx_2_0 - tbl1 * mtx_0_0) >> 12);

        const int mtx_0_1 = pMtx->m[0][1];
        const int mtx_2_1 = pMtx->m[2][1];
        pMtx->m[0][1] = static_cast<short>((tbl1 * mtx_2_1 + tbl2 * mtx_0_1) >> 12);
        pMtx->m[2][1] = static_cast<short>((tbl2 * mtx_2_1 - tbl1 * mtx_0_1) >> 12);

        const int mtx_0_2 = pMtx->m[0][2];
        const int mtx_2_2 = pMtx->m[2][2];
        pMtx->m[0][2] = static_cast<short>((tbl1 * mtx_2_2 + tbl2 * mtx_0_2) >> 12);
        pMtx->m[2][2] = static_cast<short>((tbl2 * mtx_2_2 - tbl1 * mtx_0_2) >> 12);
    }
}
MGS_FUNC_IMPLEX(0x44BD00, RotMatrixYXZ_gte_44BD00, KMD_IMPL);

void CC RotMatrixZYX_gte_44BF60(const SVECTOR* pVec, PSX_MATRIX* pMtx)
{
    memcpy(&pMtx->m, &gIdentityMatrix_6659BC.m, sizeof(PSX_MATRIX::m));

    // ============================================================================
    {
        const int tbl_vec_x1 = dword_665A3C[(pVec->field_0_x + 1024) & 4095];
        const int tbl_vec_x2 = dword_665A3C[pVec->field_0_x & 4095];

        const int mtx_1_0 = pMtx->m[1][0];
        const int mtx_2_0 = pMtx->m[2][0];
        pMtx->m[2][0] = static_cast<short int>((tbl_vec_x2 * mtx_1_0 + tbl_vec_x1 * mtx_2_0) >> 12);
        pMtx->m[1][0] = static_cast<short int>((tbl_vec_x1 * mtx_1_0 - tbl_vec_x2 * mtx_2_0) >> 12);

        const int mtx_1_1 = pMtx->m[1][1];
        const int mtx_2_1 = pMtx->m[2][1];
        pMtx->m[1][1] = static_cast<short int>((tbl_vec_x1 * mtx_1_1 - tbl_vec_x2 * mtx_2_1) >> 12);
        pMtx->m[2][1] = static_cast<short int>((tbl_vec_x2 * mtx_1_1 + tbl_vec_x1 * mtx_2_1) >> 12);

        const int mtx_1_2 = pMtx->m[1][2];
        const int mtx_2_2 = pMtx->m[2][2];
        pMtx->m[1][2] = static_cast<short int>((tbl_vec_x1 * mtx_1_2 - tbl_vec_x2 * mtx_2_2) >> 12);
        pMtx->m[2][2] = static_cast<short int>((tbl_vec_x2 * mtx_1_2 + tbl_vec_x1 * mtx_2_2) >> 12);
    }

    // ============================================================================
    {
        const int tbl_vec_y1 = dword_665A3C[(pVec->field_2_y + 1024) & 4095];
        const int tbl_vec_y2 = dword_665A3C[pVec->field_2_y & 4095];

        const int mtx_2_0 = pMtx->m[2][0];
        const int mtx_0_0 = pMtx->m[0][0];
        pMtx->m[0][0] = static_cast<short int>((tbl_vec_y2 * mtx_2_0 + tbl_vec_y1 * mtx_0_0) >> 12);
        pMtx->m[2][0] = static_cast<short int>((tbl_vec_y1 * mtx_2_0 - tbl_vec_y2 * mtx_0_0) >> 12);

        const int mtx_2_1 = pMtx->m[2][1];
        const int mtx_0_1 = pMtx->m[0][1];
        pMtx->m[0][1] = static_cast<short int>((tbl_vec_y2 * mtx_2_1 + tbl_vec_y1 * mtx_0_1) >> 12);
        pMtx->m[2][1] = static_cast<short int>((tbl_vec_y1 * mtx_2_1 - tbl_vec_y2 * mtx_0_1) >> 12);

        const int mtx_2_2 = pMtx->m[2][2];
        const int mtx_0_2 = pMtx->m[0][2];
        pMtx->m[2][2] = static_cast<short int>((tbl_vec_y1 * mtx_2_2 - tbl_vec_y2 * mtx_0_2) >> 12);
        pMtx->m[0][2] = static_cast<short int>((tbl_vec_y2 * mtx_2_2 + tbl_vec_y1 * mtx_0_2) >> 12);
    }

    // ============================================================================
    {
        const int tbl_vec_z1 = dword_665A3C[(pVec->field_4_z + 1024) & 4095];
        const int tbl_vec_z2 = dword_665A3C[pVec->field_4_z & 4095];

        const int mtx_0_0 = pMtx->m[0][0];
        const int mtx_1_0 = pMtx->m[1][0];
        pMtx->m[0][0] = static_cast<short int>((tbl_vec_z1 * mtx_0_0 - tbl_vec_z2 * mtx_1_0) >> 12);
        pMtx->m[1][0] = static_cast<short int>((tbl_vec_z2 * mtx_0_0 + tbl_vec_z1 * mtx_1_0) >> 12);

        const int mtx_0_1 = pMtx->m[0][1];
        const int mtx_1_1 = pMtx->m[1][1];
        pMtx->m[0][1] = static_cast<short int>((tbl_vec_z1 * mtx_0_1 - tbl_vec_z2 * mtx_1_1) >> 12);
        pMtx->m[1][1] = static_cast<short int>((tbl_vec_z2 * mtx_0_1 + tbl_vec_z1 * mtx_1_1) >> 12);

        const int mtx_0_2 = pMtx->m[0][2];
        const int mtx_1_2 = pMtx->m[1][2];
        pMtx->m[0][2] = static_cast<short int>((tbl_vec_z1 * mtx_0_2 - tbl_vec_z2 * mtx_1_2) >> 12);
        pMtx->m[1][2] = static_cast<short int>((tbl_vec_z2 * mtx_0_2 + tbl_vec_z1 * mtx_1_2) >> 12);
    }
}
MGS_FUNC_IMPLEX(0x44BF60, RotMatrixZYX_gte_44BF60, KMD_IMPL);

void CC RotMatrix_gte_44BAB0(const SVECTOR* pVec, PSX_MATRIX* pMtx)
{
    memcpy(&pMtx->m, &gIdentityMatrix_6659BC.m, sizeof(PSX_MATRIX::m));

    // ============================================================================
    {
        const int tbl_vec_z1 = dword_665A3C[(pVec->field_4_z + 1024) & 4095];
        const int tbl_vec_z2 = dword_665A3C[pVec->field_4_z & 4095];

        const int mtx_1_0 = pMtx->m[1][0];
        const int mtx_0_0 = pMtx->m[0][0];
        pMtx->m[1][0] = static_cast<short int>((tbl_vec_z2 * mtx_0_0 + tbl_vec_z1 * mtx_1_0) >> 12);
        pMtx->m[0][0] = static_cast<short int>((tbl_vec_z1 * mtx_0_0 - tbl_vec_z2 * mtx_1_0) >> 12);

        const int mtx_0_1 = pMtx->m[0][1];
        const int mtx_1_1 = pMtx->m[1][1];
        pMtx->m[0][1] = static_cast<short int>((tbl_vec_z1 * mtx_0_1 - tbl_vec_z2 * mtx_1_1) >> 12);
        pMtx->m[1][1] = static_cast<short int>((tbl_vec_z2 * mtx_0_1 + tbl_vec_z1 * mtx_1_1) >> 12);

        const int mtx_1_2 = pMtx->m[1][2];
        const int mtx_0_2 = pMtx->m[0][2];
        pMtx->m[1][2] = static_cast<short int>((tbl_vec_z2 * mtx_0_2 + tbl_vec_z1 * mtx_1_2) >> 12);
        pMtx->m[0][2] = static_cast<short int>((tbl_vec_z1 * mtx_0_2 - tbl_vec_z2 * mtx_1_2) >> 12);
    }

    // ============================================================================
    {
        const int tbl_vec_y1 = dword_665A3C[(pVec->field_2_y + 1024) & 4095];
        const int tbl_vec_y2 = dword_665A3C[pVec->field_2_y & 4095];

        const int mtx_2_0 = pMtx->m[2][0];
        const int mtx_0_0 = pMtx->m[0][0];
        pMtx->m[0][0] = static_cast<short int>((tbl_vec_y2 * mtx_2_0 + tbl_vec_y1 * mtx_0_0) >> 12);
        pMtx->m[2][0] = static_cast<short int>((tbl_vec_y1 * mtx_2_0 - tbl_vec_y2 * mtx_0_0) >> 12);

        const int mtx_0_1 = pMtx->m[0][1];
        const int mtx_2_1 = pMtx->m[2][1];
        pMtx->m[0][1] = static_cast<short int>((tbl_vec_y2 * mtx_2_1 + tbl_vec_y1 * mtx_0_1) >> 12);
        pMtx->m[2][1] = static_cast<short int>((tbl_vec_y1 * mtx_2_1 - tbl_vec_y2 * mtx_0_1) >> 12);

        const int mtx_0_2 = pMtx->m[0][2];
        const int mtx_2_2 = pMtx->m[2][2];
        pMtx->m[0][2] = static_cast<short int>((tbl_vec_y2 * mtx_2_2 + tbl_vec_y1 * mtx_0_2) >> 12);
        pMtx->m[2][2] = static_cast<short int>((tbl_vec_y1 * mtx_2_2 - tbl_vec_y2 * mtx_0_2) >> 12);
    }

    // ============================================================================
    {
        const int tbl_vec_x1 = dword_665A3C[(pVec->field_0_x + 1024) & 4095];
        const int tbl_vec_x2 = dword_665A3C[pVec->field_0_x & 4095];

        const int mtx_2_0 = pMtx->m[2][0];
        const int mtx_1_0 = pMtx->m[1][0];
        pMtx->m[2][0] = static_cast<short int>((tbl_vec_x2 * mtx_1_0 + tbl_vec_x1 * mtx_2_0) >> 12);
        pMtx->m[1][0] = static_cast<short int>((tbl_vec_x1 * mtx_1_0 - tbl_vec_x2 * mtx_2_0) >> 12);

        const int mtx_1_1 = pMtx->m[1][1];
        const int mtx_2_1 = pMtx->m[2][1];
        pMtx->m[2][1] = static_cast<short int>((tbl_vec_x2 * mtx_1_1 + tbl_vec_x1 * mtx_2_1) >> 12);
        pMtx->m[1][1] = static_cast<short int>((tbl_vec_x1 * mtx_1_1 - tbl_vec_x2 * mtx_2_1) >> 12);

        const int mtx_1_2 = pMtx->m[1][2];
        const int mtx_2_2 = pMtx->m[2][2];
        pMtx->m[2][2] = static_cast<short int>((tbl_vec_x2 * mtx_1_2 + tbl_vec_x1 * mtx_2_2) >> 12);
        pMtx->m[1][2] = static_cast<short int>((tbl_vec_x1 * mtx_1_2 - tbl_vec_x2 * mtx_2_2) >> 12);
    }
}
MGS_FUNC_IMPLEX(0x44BAB0, RotMatrix_gte_44BAB0, KMD_IMPL);

void CC RotMatrixC_44C3D0(const SVECTOR* pVec, PSX_MATRIX* pMtx)
{
    memcpy(&pMtx->m, &gIdentityMatrix_6659BC.m, sizeof(PSX_MATRIX::m));

    // ============================================================================
    {
        const int tbl_vec_z1 = dword_665A3C[(pVec->field_4_z + 1024) & 4095];
        const int tbl_vec_z2 = dword_665A3C[pVec->field_4_z & 4095];

        const int m_1_0 = pMtx->m[1][0];
        const int m_0_0 = pMtx->m[0][0];
        pMtx->m[1][0] = static_cast<short int>((tbl_vec_z2 * m_0_0 + tbl_vec_z1 * m_1_0) >> 12);
        pMtx->m[0][0] = static_cast<short int>((tbl_vec_z1 * m_0_0 - tbl_vec_z2 * m_1_0) >> 12);

        const int m_0_1 = pMtx->m[0][1];
        const int m_1_1 = pMtx->m[1][1];
        pMtx->m[0][1] = static_cast<short int>((tbl_vec_z1 * m_0_1 - tbl_vec_z2 * m_1_1) >> 12);
        pMtx->m[1][1] = static_cast<short int>((tbl_vec_z2 * m_0_1 + tbl_vec_z1 * m_1_1) >> 12);

        const int m_1_2 = pMtx->m[1][2];
        const int m_0_2 = pMtx->m[0][2];
        pMtx->m[1][2] = static_cast<short int>((tbl_vec_z2 * m_0_2 + tbl_vec_z1 * m_1_2) >> 12);
        pMtx->m[0][2] = static_cast<short int>((tbl_vec_z1 * m_0_2 - tbl_vec_z2 * m_1_2) >> 12);
    }

    // ============================================================================
    {
        const int tbl_vec_y1 = dword_665A3C[(pVec->field_2_y + 1024) & 4095];
        const int tbl_vec_y2 = dword_665A3C[pVec->field_2_y & 4095]; // Not Z2 ?

        const int m_2_0 = pMtx->m[2][0];
        const int m_0_0 = pMtx->m[0][0];
        pMtx->m[0][0] = static_cast<short int>((tbl_vec_y2 * m_2_0 + tbl_vec_y1 * m_0_0) >> 12);
        pMtx->m[2][0] = static_cast<short int>((tbl_vec_y1 * m_2_0 - tbl_vec_y2 * m_0_0) >> 12);

        const int m_2_1 = pMtx->m[2][1];
        const int m_0_1 = pMtx->m[0][1];
        pMtx->m[0][1] = static_cast<short int>((tbl_vec_y2 * m_2_1 + tbl_vec_y1 * m_0_1) >> 12);
        pMtx->m[2][1] = static_cast<short int>((tbl_vec_y1 * m_2_1 - tbl_vec_y2 * m_0_1) >> 12);

        const int m_0_2 = pMtx->m[0][2];
        const int m_2_2 = pMtx->m[2][2];
        pMtx->m[0][2] = static_cast<short int>((tbl_vec_y2 * m_2_2 + tbl_vec_y1 * m_0_2) >> 12);
        pMtx->m[2][2] = static_cast<short int>((tbl_vec_y1 * m_2_2 - tbl_vec_y2 * m_0_2) >> 12);
    }

    // ============================================================================
    {
        const int tbl_vec_x1 = dword_665A3C[pVec->field_0_x & 4095];
        const int tbl_vec_x2 = dword_665A3C[(pVec->field_0_x + 1024) & 4095];

        const int m_2_0 = pMtx->m[2][0];
        const int m_1_0 = pMtx->m[1][0];
        pMtx->m[2][0] = static_cast<short int>((tbl_vec_x1 * m_1_0 + tbl_vec_x2 * m_2_0) >> 12);
        pMtx->m[1][0] = static_cast<short int>((tbl_vec_x2 * m_1_0 - tbl_vec_x1 * m_2_0) >> 12);

        const int m_1_1 = pMtx->m[1][1];
        const int m_2_1 = pMtx->m[2][1];
        pMtx->m[2][1] = static_cast<short int>((tbl_vec_x1 * m_1_1 + tbl_vec_x2 * m_2_1) >> 12);
        pMtx->m[1][1] = static_cast<short int>((tbl_vec_x2 * m_1_1 - tbl_vec_x1 * m_2_1) >> 12);

        const int m_1_2 = pMtx->m[1][2];
        const int m_2_2 = pMtx->m[2][2];
        pMtx->m[2][2] = static_cast<short int>((tbl_vec_x1 * m_1_2 + tbl_vec_x2 * m_2_2) >> 12);
        pMtx->m[1][2] = static_cast<short int>((tbl_vec_x2 * m_1_2 - tbl_vec_x1 * m_2_2) >> 12);
    }
}
MGS_FUNC_IMPLEX(0x44C3D0, RotMatrixC_44C3D0, KMD_IMPL);

int CC Res_get_constant_44AF00(__int16 value)
{
    return dword_665A3C[value & 4095];
}
MGS_FUNC_IMPLEX(0x44AF00, Res_get_constant_44AF00, KMD_IMPL);

int CC Res_base_get_constant_44AEE0(__int16 idx)
{
    return dword_665A3C[(idx + 1024) & 4095];
}
MGS_FUNC_IMPLEX(0x44AEE0, Res_base_get_constant_44AEE0, KMD_IMPL);

void CC Res_base_get_gte_rot_mtx_44AE10(PSX_MATRIX* pMtx)
{
    memcpy(&pMtx->m, &gte_rotation_matrix_993E40.m, sizeof(PSX_MATRIX::m));
    pMtx->t[0] = gGte_translation_vector_993E54.x;
    pMtx->t[1] = gGte_translation_vector_993E54.y;
    pMtx->t[2] = gGte_translation_vector_993E54.z;
}
MGS_FUNC_IMPLEX(0x44AE10, Res_base_get_gte_rot_mtx_44AE10, KMD_IMPL);

void CC Gte_set_rot_matrix_44ADE0(const MATRIX3x3* pMatrix)
{
    memcpy(&gte_rotation_matrix_993E40.m, pMatrix, sizeof(MATRIX3x3));
}
MGS_FUNC_IMPLEX(0x44ADE0, Gte_set_rot_matrix_44ADE0, KMD_IMPL);

void CC VectorMatrix_40B66B(const SVECTOR* pVec, int value, SVECTOR* pOutVec)
{
    PSX_MATRIX mtx = {};
    RotMatrixYXZ_gte_44BD00(pVec, &mtx);
    pOutVec->field_0_x = static_cast<short int>(value * mtx.m[0][2] / 4096);
    pOutVec->field_2_y = static_cast<short int>(value * mtx.m[1][2] / 4096);
    pOutVec->field_4_z = static_cast<short int>(value * mtx.m[2][2] / 4096);
}
MGS_FUNC_IMPLEX(0x40B66B, VectorMatrix_40B66B, KMD_IMPL);

void CC Vector_unknown_44B250(const VECTOR3* pVec1, const VECTOR3* pVec2, VECTOR3* pOut)
{
    pOut->x = pVec1->y * pVec2->z - pVec2->y * pVec1->z;
    pOut->y = pVec2->x * pVec1->z - pVec2->z * pVec1->x;
    pOut->z = pVec2->y * pVec1->x - pVec1->y * pVec2->x;
}
MGS_FUNC_IMPLEX(0x44B250, Vector_unknown_44B250, KMD_IMPL);

void CC Gte_set_translation_vector_from_mtx_44AE60(const PSX_MATRIX* pMtx)
{
    gGte_translation_vector_993E54.x = pMtx->t[0];
    gGte_translation_vector_993E54.y = pMtx->t[1];
    gGte_translation_vector_993E54.z = pMtx->t[2];
}
MGS_FUNC_IMPLEX(0x44AE60, Gte_set_translation_vector_from_mtx_44AE60, KMD_IMPL);

void CC Gte_set_light_source_matrix_44AE80(const MATRIX3x3* pMtx)
{
    memcpy(&gGte_light_source_matrix_993E60, pMtx, sizeof(MATRIX3x3));
}
MGS_FUNC_IMPLEX(0x44AE80, Gte_set_light_source_matrix_44AE80, KMD_IMPL);

void CC Gte_set_light_colour_matrix_source_44AEB0(const MATRIX3x3* pMtx)
{
    memcpy(&gGte_light_colour_matrix_source_993E80, pMtx, sizeof(MATRIX3x3));
}
MGS_FUNC_IMPLEX(0x44AEB0, Gte_set_light_colour_matrix_source_44AEB0, KMD_IMPL);

void CC Gte_get_rot_matrix_and_trans_vec_407C0D(PSX_MATRIX* pMatrix)
{
    memcpy(&pMatrix->m, &gte_rotation_matrix_993E40.m, sizeof(PSX_MATRIX::m));
    pMatrix->t[0] = gGte_translation_vector_993E54.x;
    pMatrix->t[1] = gGte_translation_vector_993E54.y;
    pMatrix->t[2] = gGte_translation_vector_993E54.z;
}
MGS_FUNC_IMPLEX(0x407C0D, Gte_get_rot_matrix_and_trans_vec_407C0D, KMD_IMPL);

void CC VectorSqr_40225C(const VECTOR3* pInVec, signed int value, SVECTOR* pResult)
{
    VECTOR3 sqrtVec = {};
    Psx_gte_sqr0_44B030(pInVec, &sqrtVec);

    __int64 squareRoot = j_sqrt(sqrtVec.x + sqrtVec.y + sqrtVec.z);
    if (!squareRoot)
    {
        squareRoot = 1;
    }

    __int64 calc = 2 * value - squareRoot;
    if (calc < 0)
    {
        calc = 0;
    }
    pResult->field_0_x = static_cast<short int>(calc * (3072 * pInVec->x / squareRoot) / value);// 3072 fixed point of 0.75f
    pResult->field_2_y = static_cast<short int>(calc * (3072 * pInVec->y / squareRoot) / value);
    pResult->field_4_z = static_cast<short int>(calc * (3072 * pInVec->z / squareRoot) / value);
}
MGS_FUNC_IMPLEX(0x40225C, VectorSqr_40225C, KMD_IMPL);

struct Light
{
    short int field_0_r;
    short int field_2_g;
    short int field_4_b;
    short int field_6_a;
};
MGS_ASSERT_SIZEOF(Light, 0x8);

void CC Kmd_verts_unknown_443C39(const SVECTOR* pVerts, int vertCount, SVECTOR* pScratchBuffer, const Light* pLights, int lightCount)
{
    if (vertCount - 1 < 0)
    {
        return;
    }

    SVECTOR* pScratchIter = pScratchBuffer;
    for (int i = 0; i < vertCount; i++)
    {
        gGte_VXY0_993EC0.regs.VX = pVerts[i].field_0_x;
        gGte_VXY0_993EC0.regs.VY = pVerts[i].field_2_y;
        gGte_VXY0_993EC0.regs.VZ = pVerts[i].field_4_z;
      
        Psx_gte_RT1TR_rt_4477A0();

        const Light* pLightIter = pLights;
        int halvesWrote = 0;
        for (int j = 0; j < lightCount; j++)
        {
            VECTOR3 vec = {};
            const int light1_g = pLightIter[1].field_2_g;
            vec.x = gGte_MAC1_993F24.MAC_32 - pLightIter[0].field_0_r;
            if (!(vec.x < -light1_g || vec.x > light1_g))
            {
                vec.y = gGte_MAC2_993F28.MAC_32 - pLightIter[0].field_2_g;
                if (!(vec.y < -light1_g || vec.y > light1_g))
                {
                    vec.z = gGte_MAC3_993F2C.MAC_32 - pLightIter[0].field_4_b;
                    if (!(vec.z < -light1_g || vec.z > light1_g))
                    {
                        halvesWrote++;
                        if (halvesWrote == 2)
                        {
                            VectorSqr_40225C(&vec, pLightIter[1].field_0_r, pScratchIter + 1);
                            pScratchIter[2].field_4_z = pLightIter[1].field_4_b;
                            pScratchIter[2].field_6_padding = pLightIter[1].field_6_a;
                            break;
                        }
                        VectorSqr_40225C(&vec, pLightIter[1].field_0_r, pScratchIter);
                        pScratchIter[2].field_0_x = pLightIter[1].field_4_b;
                        pScratchIter[2].field_2_y = pLightIter[1].field_6_a;
                    }
                }
            }
            pLightIter += 2;
        }

        if (halvesWrote == 0)
        {
            // Nothing wrote, zero everything.
            pScratchIter[2].field_0_x = 0;
            pScratchIter[2].field_2_y = 0;
            pScratchIter[2].field_4_z = 0;
            pScratchIter[2].field_6_padding = 0;
        }
        else if (halvesWrote == 1)
        {
            // Only the first half wrote, zero out the 2nd half.
            pScratchIter[2].field_4_z = 0;
            pScratchIter[2].field_6_padding = 0;
        }

        pScratchIter += 3;
    }
}
MGS_FUNC_IMPLEX(0x443C39, Kmd_verts_unknown_443C39, true); // TODO

MGS_FUNC_NOT_IMPL(0x40241F, int __cdecl(SVECTOR *a1, PSX_MATRIX *pMtxAry), Res_base_unknown_40241F); // TODO


void CC Res_Enemy_boxkeri_update_5B6EF7(Actor_boxkeri* pBox)
{
    signed int ticks; // ecx
    Prim_unknown_0x48 *pKmd; // eax
    __int16 zPos; // ax
    __int16 xPos; // ax

    ticks = pBox->field_74_ticks;
    if (ticks > 40)
    {
        Actor_DestroyOnNextUpdate_40A3ED(&pBox->mBase);
        return;
    }
    if (ticks >= 9)
    {
        if (ticks >= 12)
        {
            if (ticks >= 15)
            {
                if (ticks >= 18)
                {
                    if (ticks >= 19)
                    {
                        pKmd = &pBox->field_20_kmd.field_0_pObj->prim_48;
                        if (ticks % 2)
                        {
                            //LOBYTE(pKmd->field_28_flags_or_type) |= 0x80u;
                        }
                        else
                        {
                            //LOBYTE(pKmd->field_28_flags_or_type) &= 0x7Fu;
                        }
                    }
                    else
                    {
                        pBox->field_78_pos2 += 32;
                    }
                }
                else
                {
                    pBox->field_78_pos2 -= 32;
                }
            }
            else
            {
                pBox->field_78_pos2 += 64;
            }
        }
        else
        {
            pBox->field_78_pos2 -= 128;
        }
    }
    else
    {
        pBox->field_7A_pos1 += 80;
        pBox->field_78_pos2 += 256;
    }

    switch (pBox->field_76_state)
    {
    case 0:
        if (ticks < 9)
        {
            pBox->field_7A_pos1 += 5;
        }
        pBox->field_4C.field_4_z = -pBox->field_7A_pos1;
        xPos = -pBox->field_78_pos2;
        goto LABEL_29;
    case 1:
        pBox->field_4C.field_0_x = -pBox->field_7A_pos1;
        zPos = pBox->field_78_pos2;
        goto LABEL_21;
    case 2:
        if (ticks < 9)
        {
            pBox->field_7A_pos1 += 100;
        }
        pBox->field_4C.field_4_z = pBox->field_7A_pos1;
        xPos = pBox->field_78_pos2;
    LABEL_29:
        pBox->field_44.field_0_x = xPos;
        break;
    case 3:
        pBox->field_4C.field_0_x = pBox->field_7A_pos1;
        zPos = -pBox->field_78_pos2;
    LABEL_21:
        pBox->field_44.field_4_z = zPos;
        break;
    }
    // TODO
    //mapChangeFlagsOrScriptBinds = (char *)map_change_flags_dword_99535C;
    PsxSetRotationAndTranslation_407A8F(&pBox->field_54_mtx);
    Res_base_unknown_407B3D(&pBox->field_4C);
    Res_base_unknown_407B79(&pBox->field_44);
    PsxGetRotationAndTranslation_407BC1(&pBox->field_20_kmd.field_0_pObj->prim_48.field_0_matrix);
    Res_base_unknown_40241F(&gSnakePos_stru_9942B0, pBox->field_7C_set_on_kmd_light_matrix_ptrs);
    ++pBox->field_74_ticks;
}
MGS_FUNC_IMPLEX(0x5B6EF7, Res_Enemy_boxkeri_update_5B6EF7, false); // TODO


void CC Res_Enemy_boxkeri_shutdown_5B701F(Actor_boxkeri* pBox)
{
    Kmd_free_4500DD(&pBox->field_20_kmd);
}
MGS_FUNC_IMPLEX(0x5B701F, Res_Enemy_boxkeri_shutdown_5B701F, BOXKERI_IMPL);

void CC Res_Enemy_boxkeri_loader_mesg_5B711B()
{
    LibGV_Msg msg = {};
    msg.field_0_res_hash = ResourceNameHash("スネーク");
    msg.field_4_action_hash_or_ptr = ResourceNameHash("段ボール");
    msg.field_6_hash = 1;
    msg.field_12_num_valid_fields = 2;
    LibGV_mesg_write_40B3ED(&msg);
}
MGS_FUNC_IMPLEX(0x5B711B, Res_Enemy_boxkeri_loader_mesg_5B711B, BOXKERI_IMPL);

void CC Kmd_Set_Light_matrices_450109(struc_kmd* pKmd, PSX_MATRIX* pLightMtxAry)
{
    pKmd->field_8_light_mtx_array = pLightMtxAry;
    pKmd->field_0_pObj->prim_48.field_34_light_mtx_array = pLightMtxAry;
}
MGS_FUNC_IMPLEX(0x450109, Kmd_Set_Light_matrices_450109, BOXKERI_IMPL);

int CC Kmd_TotalObjectSizeInBytes_443FAF(KmdHeader* pKmdHeader)
{
    int totalCount = 0;
    for (DWORD i = 0; i < pKmdHeader->mNumberOfObjects; i++)
    {
        kmdObject* pKmdObj = (kmdObject *)&pKmdHeader[1];
        totalCount += pKmdObj[i].field_4_numFaces;
    }
    return (16 * totalCount); // TODO: 16 = sizeof(?)
}
MGS_FUNC_IMPLEX(0x443FAF, Kmd_TotalObjectSizeInBytes_443FAF, BOXKERI_IMPL);

void CC Object_Remove_4017C3(Prim_unknown_0x48* pPrim)
{
    struct_gv* pGv = &gLibGVStruct1_6BC36C + pPrim->field_30_size;
    int used = pGv->gObjectQueue_word_6BC3C2_0;
    if (used > 0)
    {
        // Find position in the array
        Prim_unknown_0x48** ppPrimIter = (Prim_unknown_0x48**)pGv->gObjects_dword_6BC3C4;
        int locationOfItemToRemove = used;
        while (*ppPrimIter != pPrim)
        {
            ++ppPrimIter;
            if (--locationOfItemToRemove <= 0)
            {
                return;
            }
        }
        
        // Decrement the used count as we are about to erase an item
        const int newCount = locationOfItemToRemove - 1;
        if (newCount > 0)
        {
            // Overwrite the old item with everything after it up to the new count
            memcpy(ppPrimIter, ppPrimIter + 1, sizeof(Prim_unknown_0x48*) * newCount);
        }
        pGv->gObjectQueue_word_6BC3C2_0 = static_cast<s16>(used - 1);
    }
}
MGS_FUNC_IMPLEX(0x4017C3, Object_Remove_4017C3, BOXKERI_IMPL);

void CC Prim_free_colour_buffer_443FCB(Prim_unknown_0x48* pPrim)
{
    Prim_Mesh_0x5C* pMesh = (Prim_Mesh_0x5C *)&pPrim[1];
    if (pMesh->field_44_colour_buffer)
    {
        System_2_free_40B2A7(pMesh->field_44_colour_buffer);
        pMesh->field_44_colour_buffer = nullptr;
    }
}
MGS_FUNC_IMPLEX(0x443FCB, Prim_free_colour_buffer_443FCB, BOXKERI_IMPL);

void CC Prim_void_and_free_4440BE(Prim_unknown_0x48* pPrim)
{
    Prim_Mesh_0x5C* pMeshIter = (Prim_Mesh_0x5C *)&pPrim[1];
    if (pPrim->field_2E_UnknownOrNumFaces > 0)
    {
        for (int i=0; i<pPrim->field_2E_UnknownOrNumFaces; i++)
        {
            LibGV_void_active_prim_buffer_4073E8(pMeshIter, 0);
            LibGV_void_active_prim_buffer_4073E8(pMeshIter, 1);
            pMeshIter++;
        }
    }
    Prim_free_colour_buffer_443FCB(pPrim);
    System_2_free_40B2A7(pPrim);
}
MGS_FUNC_IMPLEX(0x4440BE, Prim_void_and_free_4440BE, BOXKERI_IMPL);

void CC Kmd_free_4500DD(struc_kmd* pKmd)
{
    Prim_unknown_0x48* pObj = &pKmd->field_0_pObj->prim_48;
    if (pObj)
    {
        Object_Remove_4017C3(pObj);
        Prim_void_and_free_4440BE(pObj);
    }
}
MGS_FUNC_IMPLEX(0x4500DD, Kmd_free_4500DD, BOXKERI_IMPL);

signed int CC LoadKmdImpl_450243(struc_kmd* pKmd, int resHash)
{
    Prim_unknown_0x48* pOldObj = &pKmd->field_0_pObj->prim_48;
    KmdHeader* pFileData = (KmdHeader *)LibGV_FindFile_40A603(HashFileName_40A58B(resHash, 'k'));
    if (!pFileData)
    {
        pFileData = (KmdHeader *)LibGV_FindFile_40A603(HashFileName_40A58B(ResourceNameHash("null"), 'k'));
    }

    Prim_Union* pPrimObj = Obj_Alloc_443FEC(pFileData, pKmd->field_4_size, 0);
    if (!pPrimObj)
    {
        return -1;
    }

    if (pOldObj)
    {
        Object_Remove_4017C3(pOldObj);
        Prim_void_and_free_4440BE(pOldObj);
    }

    PSX_MATRIX* pKmdLightMtxAry = pKmd->field_8_light_mtx_array;
    pKmd->field_0_pObj = pPrimObj;
    pPrimObj->prim_48.field_34_light_mtx_array = pKmdLightMtxAry;
    pPrimObj->prim_48.field_2C_index = pKmd->field_C_mapflags_or_script_binds;
    Object_Add_40178F(pPrimObj);
    return 0;
}
MGS_FUNC_IMPLEX(0x450243, LoadKmdImpl_450243, BOXKERI_IMPL);

void CC LoadKmdRelated_44FF7C(struc_kmd* pObj, int resHash, int size)
{
    MemClearUnknown_40B231(pObj, sizeof(struc_kmd));
    pObj->field_4_size = size;
    pObj->field_8_light_mtx_array = &gLightNormalVec_650128;
    // TODO
    //pObj->field_C_mapflags_or_script_binds = (signed __int16)mapChangeFlagsOrScriptBinds;
    LoadKmdImpl_450243(pObj, resHash);
}
MGS_FUNC_IMPLEX(0x44FF7C, LoadKmdRelated_44FF7C, false); // TODO

int CC Res_Enemy_boxkeri_loader_5B702E(Actor_boxkeri* pBox, PSX_MATRIX* pMtx, SVECTOR* pVec)
{
    // TODO
    //mapChangeFlagsOrScriptBinds = (char *)map_change_flags_dword_99535C;
    LoadKmdRelated_44FF7C(&pBox->field_20_kmd, ResourceNameHash("cb_box"), 109);
    Kmd_Set_Light_matrices_450109(&pBox->field_20_kmd, pBox->field_7C_set_on_kmd_light_matrix_ptrs);

    //pBox->field_20_kmd[2].field_0_matrix.m[1][1] = 500;


    /* TODO
    *(DWORD *)&pBox->field_44.field_0_x = *(DWORD *)&pControl_6BEF28.field_0_vec.field_0_x;
    *(DWORD *)&pBox->field_44.field_4_z = *(DWORD *)&pControl_6BEF28.field_0_vec.field_4_z;
    *(DWORD *)&pBox->field_4C.field_0_x = *(DWORD *)&pControl_6BEF28.field_0_vec.field_0_x;
    *(DWORD *)&pBox->field_4C.field_4_z = *(DWORD *)&pControl_6BEF28.field_0_vec.field_4_z;
    */

    pBox->field_78_pos2 = 0;
    pBox->field_7A_pos1 = 0;

    SVECTOR vec = {};
    Vector_subtract_40B4ED(pVec, &gSnakePos_stru_9942B0, &vec);
    const int v5 = 0;// FixedSubtract_40B6BD(dword_99534C->field_8_3_words[1], Res_base_unknown_40B612(&vec)); // TODO
    if (v5 < 512 || v5 > 3606)
    {
        pBox->field_76_state = 0;
    }
    else if (v5 >= 1536)
    {
        if (v5 >= 2584)
        {
            pBox->field_76_state = 3;
        }
        else
        {
            pBox->field_76_state = 2;
        }
    }
    else
    {
        pBox->field_76_state = 1;
    }
    memcpy(&pBox->field_54_mtx, pMtx, sizeof(pBox->field_54_mtx));
    pBox->field_74_ticks = 0;
    Res_Enemy_boxkeri_loader_mesg_5B711B();
    return 0;
}
MGS_FUNC_IMPLEX(0x5B702E, Res_Enemy_boxkeri_loader_5B702E, false); // TODO

Actor_boxkeri* CC Res_Enemy_boxkeri_create_5B6EA9(PSX_MATRIX* pMtx, SVECTOR* pVec)
{
    Actor_boxkeri* pBox = Actor_ResourceAllocT<Actor_boxkeri>(4);
    if (!pBox)
    {
        return nullptr;
    }

    Actor_Init_40A347(&pBox->mBase,
        reinterpret_cast<TActorFunction>(Res_Enemy_boxkeri_update_5B6EF7),
        reinterpret_cast<TActorFunction>(Res_Enemy_boxkeri_shutdown_5B701F),
        "C:\\mgs\\source\\Enemy\\boxkeri.c");

    if (Res_Enemy_boxkeri_loader_5B702E(pBox, pMtx, pVec) >= 0)
    {
        return pBox;
    }

    Actor_DestroyOnNextUpdate_40A3ED(&pBox->mBase);
    return nullptr;
}
MGS_FUNC_IMPLEX(0x5B6EA9, Res_Enemy_boxkeri_create_5B6EA9, BOXKERI_IMPL);
