#include "stdafx.h"
#include "Kmd.hpp"
#include "System.hpp"
#include "LibDG.hpp"
#include "System.hpp"
#include "Script.hpp"
#include "ResourceNameHash.hpp"
#include "Psx.hpp"

#define KMD_IMPL true

void ForceLinkKmdCpp() { }

struct kmdObject;


// TODO: Reverse KMD loading as it seems very closely linked to the GV lib prim rendering/queues which is the linked to the GTE emu

int __cdecl Prim_444096(kmdObject* pKmdObj)
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
    const int primSize = sizeof(Prim_unknown_0x48) + (sizeof(Prim_Mesh_0x5C) * pFileData->unkNum);
    Prim_unknown_0x48* pAllocated = (Prim_unknown_0x48 *)System_2_zerod_allocate_memory_40B296(primSize);
    if (pAllocated)
    {
        MemClearUnknown_40B231(pAllocated, primSize);
        memcpy(&pAllocated->field_0_matrix, &gIdentity_matrix_6501F8, sizeof(PSX_MATRIX));
        pAllocated->field_24_pKmdFileData = pFileData;
        pAllocated->field_2E_UnknownOrNumFaces = static_cast<WORD>(pFileData->numBlocks);
        pAllocated->field_28_flags_or_type = countOrType_0x40Flag;
        pAllocated->field_30_size = usuallyZero;
        pAllocated->field_34_light_mtx_array = &gLightNormalVec_650128;

        kmdObject* pKmdObject = (kmdObject *)&pFileData[1];
        if (pFileData->unkNum > 0)
        {
            Prim_Mesh_0x5C* pFirstMesh = reinterpret_cast<Prim_Mesh_0x5C*>(&pAllocated[1]);
            for (DWORD i = 0; i < pFileData->unkNum; i++)
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

Prim_Union* CC PrimAlloc_405050(int maybeFlags, int numItems, __int16 gv_index, void* size, void* field_3C)
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

int CC sub_40B6BD(__int16 a1, __int16 a2)
{
    return (a2 - a1) & 4095;
}
MGS_FUNC_IMPLEX(0x40B6BD, sub_40B6BD, KMD_IMPL);

void CC Res_base_unknown_407B3D(const SVECTOR* pVec)
{
    gGte_VXY0_993EC0.regs.VX = pVec->field_0_x;
    gGte_VXY0_993EC0.regs.VY = pVec->field_2_y;
    gGte_VXY0_993EC0.regs.VZ = pVec->field_4_z;
    gGte_VXY0_993EC0.regs.Zero = pVec->padding;

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

MGS_ARY(1, 0x665A3C, int, 4096, dword_665A3C, {}); // TODO: values


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

void CC Res_base_unknown_407B79(const SVECTOR* pRotVec)
{
    PSX_MATRIX rotMatrix = {};
    VectorRotationMatrix_unknown_44C620(pRotVec, &rotMatrix);
    sub_44B690(&rotMatrix);
    memcpy(gte_rotation_matrix_993E40.m, rotMatrix.m, sizeof(MATRIX3x3::m));
}
MGS_FUNC_IMPLEX(0x407B79, Res_base_unknown_407B79, KMD_IMPL);

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
    // TODO
    //Kmd_free_4500DD(&pBox->field_20_kmd);
}
MGS_FUNC_IMPLEX(0x5B701F, Res_Enemy_boxkeri_shutdown_5B701F, false); // TODO

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

signed int CC LoadKmdImpl_450243(struc_kmd* pKmd, int resHash)
{
    Prim_unknown_0x48* pOldObj = &pKmd->field_0_pObj->prim_48;
    KmdHeader* pFileData = (KmdHeader *)LibGV_FindFile_40A603(HashFileName_40A58B(resHash, 'k'));
    if (!pFileData)
    {
        auto nk = ResourceNameHash("null");

        pFileData = (KmdHeader *)LibGV_FindFile_40A603(HashFileName_40A58B(0xE224u, 'k'));// null.kmd ?
    }

    Prim_Union* pPrimObj = Obj_Alloc_443FEC(pFileData, pKmd->field_4_size, 0);
    if (!pPrimObj)
    {
        return -1;
    }

    if (pOldObj)
    {
        // TODO
        //Object_Remove_4017C3(pOldObj);
        //Prim_void_and_free_4440BE(pOldObj);
    }

    PSX_MATRIX* pKmdLightMtxAry = pKmd->field_8_light_mtx_array;
    pKmd->field_0_pObj = pPrimObj;
    pPrimObj->prim_48.field_34_light_mtx_array = pKmdLightMtxAry;
    pPrimObj->prim_48.field_2C_index = pKmd->field_C_mapflags_or_script_binds;
    Object_Add_40178F(pPrimObj);
    return 0;
}
MGS_FUNC_IMPLEX(0x450243, LoadKmdImpl_450243, false); // TODO

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
    const int v5 = 0;// sub_40B6BD(dword_99534C->field_8_3_words[1], Res_base_unknown_40B612(&vec)); // TODO
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
