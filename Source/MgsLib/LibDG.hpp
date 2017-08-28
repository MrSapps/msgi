#pragma once

#include "MgsFunction.hpp"

void LibDGCppForceLink();

using ResInitFn = void*;

struct Res_Init_Record
{
    WORD mHashedName;
    WORD mPadding;
    ResInitFn mInitFunctionPointer;
};
MSG_ASSERT_SIZEOF(Res_Init_Record, 0x8);

signed int CC LibDG_CHARA_44E9D2(Res_Init_Record *pStartingRecord);
void CC LibDG_SetActiveResourceInitFuncPtrs_457B5B();
void CC LibDG_ClearActiveResourceFunctionPointerList_457B7C();
ResInitFn CC LibDG_GetResourceInitFuncPtr_457BAC(WORD hashedName);
