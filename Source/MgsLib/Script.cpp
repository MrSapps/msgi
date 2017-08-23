#include "stdafx.h"
#include "MgsFunction.hpp"
#include "logger.hpp"
#include "Task.hpp"
#include "Script.hpp"

MGS_PTR(1, 0x995460, WORD*, word_995460, 0);
MGS_PTR(1, 0x995470, DWORD*, dword_995470, 0);
MGS_PTR(1, 0x995464, DWORD*, dword_995464, 0);
MGS_PTR(1, 0x995468, DWORD*, dword_995468, 0);
MGS_PTR(1, 0x99546C, DWORD*, dword_99546C, 0);
MGS_PTR(1, 0x995462, WORD*, word_995462, 0);

MGS_VAR(1, 0x9942A8, WORD, byte1_flags_word_9942A8, 0);

MGS_VAR_EXTERN(DWORD, script_cancel_non_zero_dword_7227A0);

void ScriptCpp_ForceLink()
{

}

MSG_FUNC_NOT_IMPL(0x00409CAF, signed int __cdecl(char *pScriptBytes, int numberOfArguments), Script_Run);

struct GCL_ProcInfo
{
    WORD mId;
    WORD mOffset;
};

MGS_PTR(1, 0x006BFC60, GCL_ProcInfo**, gProcInfos, nullptr);
MGS_PTR(1, 0x006BFC64, char**, dword_6BFC64, 0);


char* __cdecl Script_FindProc(WORD procId)
{
    for (GCL_ProcInfo* pProcInfo = *gProcInfos; pProcInfo->mId; ++pProcInfo)
    {
        if (pProcInfo->mId == procId)
        {
            return (*dword_6BFC64) + pProcInfo->mOffset;
        }
    }
    printf("PROC %X NOT FOUND\n", procId);
    return nullptr;
}
MSG_FUNC_IMPL(0x00409B1D, Script_FindProc);

signed int __cdecl Script_ProcCancelOrRun(WORD id, int numArgs)
{
    if (script_cancel_non_zero_dword_7227A0 || BYTE1(byte1_flags_word_9942A8) & 0x20)
    {
        printf("proc %d cancel\n", id);
        return 0;
    }
    else
    {
        // stage\init\scenerio.gcx @ 0x5FA
        // 40 00 08 70 04 21 51 00 00 40 08 
        // 24 70 04 C8 CF 00 60 00 12 64 C0
        char* scriptProc = Script_FindProc(id); // For id = 26069 / 0x000065d5
        return Script_Run(scriptProc + 3, numArgs);
    }
}
MSG_FUNC_IMPL(0x00409B53, Script_ProcCancelOrRun);


int __cdecl Script_Operator_Evaluate(int operation, int v1, int v2)
{
    int result = 0;
    switch (operation)
    {
    case 1:
        result = -v2;
        break;
    case 2:
        result = v2 == 0;
        break;
    case 3:
        result = ~v2;
        break;
    case 4:
        result = v2 + v1;
        break;
    case 5:
        result = v1 - v2;
        break;
    case 6:
        result = v2 * v1;
        break;
    case 7:
        result = v1 / v2;
        break;
    case 8:
        result = v1 % v2;
        break;
    case 9:
        result = v1 == v2;
        break;
    case 10:
        result = v1 != v2;
        break;
    case 11:
        result = v1 < v2;
        break;
    case 12:
        result = v1 <= v2;
        break;
    case 13:
        result = v1 > v2;
        break;
    case 14:
        result = v1 >= v2;
        break;
    case 15:
        result = v2 | v1;
        break;
    case 16:
        result = v2 & v1;
        break;
    case 17:
        result = v2 ^ v1;
        break;
    case 18:
        result = v1 >= 0 && v2 >= 0; // TODO Check this is correct __PAIR__(v2, v1) != 0;
        printf("Check me\n");
        break;
    case 19:
        result = v1 && v2;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}
MSG_FUNC_IMPL(0x00409E7C, Script_Operator_Evaluate);
