#pragma once

#include "MgsFunction.hpp"

void ScriptCpp_ForceLink();
void CC Script_BindInits_452610();

struct SVECTOR;

struct GCL_Proc_Arguments
{
    WORD mNumArgs;
    WORD mPadding;
    DWORD* mPArgs;
};
MGS_ASSERT_SIZEOF(GCL_Proc_Arguments, 8);

struct GCL_ProcInfo
{
    WORD mId;
    WORD mOffset;
};
MGS_ASSERT_SIZEOF(GCL_ProcInfo, 4);

BYTE* CC Script_InitProcTables_sub_409C87(BYTE* pScript);
signed int CC Script_Run(BYTE* pScriptBytes, GCL_Proc_Arguments* pArgs);
int CC Script_RunProc(BYTE* pScript);
BYTE* CC Script_GCL_Execute(BYTE* pScript, DWORD* ppScript, DWORD* pRet);
signed int CC Script_ProcCancelOrRun(WORD id, GCL_Proc_Arguments* pArgs);
DWORD CC Script_ParamExists(char paramId);
DWORD CC Script_get_int();
BYTE* CC Script_GetReturnAddress();
void CC ScriptEngineInit_4090A7();
void CC ScriptSetArgs_409893(DWORD* pArgs);
int CC Script_Operator_Evaluate(int operation, int v1, int v2);
void CC Script_RunMainProc_409D49();
char* CC Script_read_string_arg_40997B(BYTE* pScript);
DWORD CC Script_Unknown8_409924(BYTE* pScript);
int CC Script_Read3Words_409945(BYTE* pScript, WORD* pOut);


MGS_VAR_EXTERN(int, gGameOverTimer_dword_7227A4);
MGS_VAR_EXTERN(DWORD, script_cancel_non_zero_dword_7227A0);
MGS_ARY_EXTERN(__int16, 96, save_data_192_word_78E7E0);
MGS_ARY_EXTERN(DWORD, 512, save_data_2048_unk_78D7C0);
MGS_VAR_EXTERN(SVECTOR, gLightNormalVec_650128);
MGS_VAR_EXTERN(DWORD, byte1_flags_word_9942A8);

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

static inline DWORD ToDWORD(const BYTE* ptr)
{
    return (ptr[3]) | (ptr[2] << 8) | (ptr[1] << 16) | (ptr[0] << 24);
}

static inline WORD ToWORD(const BYTE* ptr)
{
    return (ptr[1]) | (ptr[0] << 8);
}
