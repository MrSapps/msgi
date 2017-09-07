#pragma once

#include "Actor.hpp"
#include "Psx.hpp"


struct Prim_base
{
    WORD field_0;
    WORD field_2;
    DWORD field_4;
    DWORD field_8;
    DWORD field_C;
    DWORD field_10;
    DWORD field_14;
    DWORD field_18;
    DWORD field_1C;
};
MSG_ASSERT_SIZEOF(Prim_base, 0x20);

struct Prim_unknown
{
    Prim_base mBase;
    DWORD field_20;
    DWORD field_24_maybe_flags;
    WORD field_28_dword_9942A0;
    WORD field_2A_num_items;
    WORD field_2C_r_index;
    WORD field_2E_w_or_h;
    WORD field_30;
    WORD field_32;
    WORD field_34;
    WORD field_36;
    DWORD field_38_g;
    DWORD field_3C_b;
    BYTE* field_40_pDataStart;
    BYTE* field_44_pDataEnd;
    DWORD field_48;
    DWORD field_4C;
    DWORD field_50;
};
MSG_ASSERT_SIZEOF(Prim_unknown, 0x54);

struct Actor_Debug
{
    Actor mBase;
    Prim_unknown* mPrimData;
    POLY_FT4 mPolyFt4;
};

void AddDebugActor();
