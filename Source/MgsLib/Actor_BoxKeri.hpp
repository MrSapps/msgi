#pragma once

#include "MgsFunction.hpp"
#include "Actor.hpp"
#include "Psx.hpp"
#include "Kmd.hpp"

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
