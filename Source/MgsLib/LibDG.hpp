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

signed int CC LibDG_CHARA_44E9D2(Res_Init_Record *pStartingRecord);
void CC LibDG_SetActiveResourceInitFuncPtrs_457B5B();
void CC LibDG_ClearActiveResourceFunctionPointerList_457B7C();
ResInitFn CC LibDG_GetResourceInitFuncPtr_457BAC(WORD hashedName);

signed int CC Script_tbl_chara_sub_451AC3(BYTE* pScript);
