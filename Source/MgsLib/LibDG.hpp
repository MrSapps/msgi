#pragma once

#include "MgsFunction.hpp"
#include "Actor.hpp"

void LibDGCpp_ForceLink();

using ResInitFn = signed int(CC *)(DWORD, int, BYTE *);

struct Res_Init_Record
{
    WORD mHashedName;
    WORD mPadding;
    ResInitFn mInitFunctionPointer;
};
MSG_ASSERT_SIZEOF(Res_Init_Record, 0x8);

struct LibDG_Struct
{
    Actor mBase;
    DWORD dword_6BB950_do_not_flip_buffers;
    DWORD dword_6BB954;
    BYTE gG_byte_6BB958;
    BYTE gB_byte_6BB959;
    BYTE gR_byte_6BB95A;
    BYTE mPadding;
};
MSG_ASSERT_SIZEOF(LibDG_Struct, 0x2C);
MGS_VAR_EXTERN(LibDG_Struct, gLibDG_2_stru_6BB930);


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

signed int CC LibDG_CHARA_44E9D2(Res_Init_Record *pStartingRecord);
void CC LibDG_SetActiveResourceInitFuncPtrs_457B5B();
void CC LibDG_ClearActiveResourceFunctionPointerList_457B7C();
ResInitFn CC LibDG_GetResourceInitFuncPtr_457BAC(WORD hashedName);
void CC LibDg_Init_40111A();

signed int CC Script_tbl_chara_sub_451AC3(BYTE* pScript);
signed int CC PrimAdd_401805(Prim_unknown* pPrimBuffer);
void CC LibDG_ExecFnPtrs_40171C(int activeBuffer);
