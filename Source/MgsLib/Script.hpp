#pragma once

#include "MgsFunction.hpp"

void ScriptCpp_ForceLink();
void CC Script_BindInits_452610();

struct GCL_Proc_Arguments
{
    WORD mNumArgs;
    WORD mPadding;
    DWORD* mPArgs;
};
MSG_ASSERT_SIZEOF(GCL_Proc_Arguments, 8);

signed int CC Script_Run(BYTE* pScriptBytes, GCL_Proc_Arguments* pArgs);

enum GCLOperators
{
    eNegate = 1,
    eIsZero = 2,
    eComplement = 3,
    eAdd = 4,
    eSubtract = 5,
    eMultiply = 6,
    eDivide = 7,
    eModulus = 8,
    eEquals = 9,
    eNotEquals = 10,
    eLessThan = 11,
    eLessThanOrEqual = 12,
    eGreaterThan = 13,
    eGreaterThanOrEqual = 14,
    eBitWiseOR = 15,
    eBitWiseAND = 16,
    eBitWiseXOR = 17,
    eOr = 18,
    eAnd = 19,
};

void DoScriptTests();
