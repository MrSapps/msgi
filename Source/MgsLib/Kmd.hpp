#pragma once

#include "MgsFunction.hpp"
#include "Actor.hpp"
#include "Psx.hpp"

void ForceLinkKmdCpp();

struct KmdHeader;

struct Prim_unknown_0x48
{
    PSX_MATRIX field_0_matrix;
    void* field_20;
    KmdHeader* field_24_pKmdFileData;
    DWORD field_28_flags_or_type;
    WORD field_2C_index;
    WORD field_2E_UnknownOrNumFaces;
    WORD field_30_size;
    WORD field_32;
    PSX_MATRIX* field_34_light_mtx_array;
    DWORD field_38_size24b;
    DWORD field_3C;
    BYTE* field_40_pDataStart[2];
};
MGS_ASSERT_SIZEOF(Prim_unknown_0x48, 0x48);

struct Prim_unknown_0x54
{
    PSX_MATRIX field_0_matrix;
    void* field_20;
    KmdHeader* field_24_pKmdFileData;
    WORD field_28_flags_or_type;
    WORD field_2A_num_prims;
    WORD field_2C_gv_index;
    WORD field_2E_UnknownOrNumFaces;
    WORD field_30_prim_size;
    WORD field_32;
    WORD field_34; // most likely also a matrix pointer ?
    WORD field_36;
    DWORD field_38_size24b;
    DWORD field_3C;
    BYTE* field_40_pDataStart[2];
    // ^^^ BASE

    DWORD field_48;
    DWORD field_4C;
    DWORD field_50;
};
MGS_ASSERT_SIZEOF(Prim_unknown_0x54, 0x54);

union Prim_Union
{
    Prim_unknown_0x54 prim_54;
    Prim_unknown_0x48 prim_48;
};
MGS_ASSERT_SIZEOF(Prim_Union, 0x54);

struct struc_kmd_14
{
    VECTOR3 v;
};

struct struc_kmd
{
    Prim_Union* field_0_pObj;
    int field_4_size;
    PSX_MATRIX* field_8_light_mtx_array;
    short field_C_mapflags_or_script_binds;
    short field_E_anim_id;
    int field_10;
    struc_kmd_14 field_14_struc;
    int field_20;
};
MGS_ASSERT_SIZEOF(struc_kmd, 0x24);

struct Actor_boxkeri
{
    Actor mBase;
    struc_kmd field_20_kmd;
    SVECTOR field_44;
    SVECTOR field_4C;
    PSX_MATRIX field_54_mtx;
    __int16 field_74_ticks;
    __int16 field_76_state;
    __int16 field_78_pos2;
    __int16 field_7A_pos1;
    PSX_MATRIX field_7C_set_on_kmd_light_matrix_ptrs[2];
};
MGS_ASSERT_SIZEOF(Actor_boxkeri, 0xBC);

Actor_boxkeri* CC Res_Enemy_boxkeri_create_5B6EA9(PSX_MATRIX* pMtx, SVECTOR* pVec);
void CC LoadKmdRelated_44FF7C(struc_kmd* pObj, int resHash, int size);
