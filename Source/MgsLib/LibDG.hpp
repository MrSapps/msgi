#pragma once

#include "MgsFunction.hpp"

void LibDGCppForceLink();

struct Res_Init_Record
{
    WORD mHashedName;
    WORD mPadding;
    void* mInitFunctionPointer;
};
MSG_ASSERT_SIZEOF(Res_Init_Record, 0x8);

signed int CC LibDG_CHARA_44E9D2(Res_Init_Record *pStartingRecord);
void CC LibDG_SetActiveResourceInitFuncPtrs_457B5B();
