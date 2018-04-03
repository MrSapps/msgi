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


struct Actor_boxkeri
{
    Actor mBase;
    Prim_unknown_0x48 *field_20_kmd;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;

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
