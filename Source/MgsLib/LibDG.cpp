#include "stdafx.h"
#include "LibDG.hpp"

void LibDGCppForceLink() { }

MGS_ARY(1, 0x669AE0, Res_Init_Record, 512, gKnownResInitFuncs_669AE0, {});      // TODO: Rip array from exe
MGS_ARY(1, 0x994320, Res_Init_Record, 512, gDynamicResFuncs_word_994320, {});   // TODO: Rip array from exe
MGS_ARY(1, 0x650040, Res_Init_Record, 3, gStaticResInitFuncs_stru_650040, {});  // TODO: Rip array from exe
MGS_PTR(1, 0x993F44, Res_Init_Record**, gpToDynamicResInitFuncs_dword_993F44, nullptr);

MGS_VAR(1, 0x994304, DWORD, gSleep5000_after_res_funcs_1_dword_994304, 0);
MGS_VAR(1, 0x99430C, DWORD, gSleep5000_after_res_funcs_2_dword_99430C, 0);

MGS_VAR(1, 0x6BB930, LibDG_Struct, gLibDG_2_stru_6BB930, {});


// TODO: sub_457B9A - call GCL script to get res fn func ptr, then caller creates the resource

signed int CC LibDG_CHARA_44E9D2(Res_Init_Record* pStartingRecord)
{
    int dst_idx = 0;
    for (;;)
    {
        if (!pStartingRecord->mInitFunctionPointer)
        {
            break;
        }

        int src_idx = 0;
        while (gKnownResInitFuncs_669AE0[src_idx].mHashedName)
        {
            if (gKnownResInitFuncs_669AE0[src_idx].mHashedName == pStartingRecord->mHashedName)
            {
                gDynamicResFuncs_word_994320[dst_idx].mHashedName = pStartingRecord->mHashedName;
                gDynamicResFuncs_word_994320[dst_idx].mInitFunctionPointer = gKnownResInitFuncs_669AE0[src_idx].mInitFunctionPointer;
                break;
            }
            src_idx++;
        }
        printf("CHARA %d, %p\n", gDynamicResFuncs_word_994320[dst_idx].mHashedName, gDynamicResFuncs_word_994320[dst_idx].mInitFunctionPointer);

        ++dst_idx;
        ++pStartingRecord;
    }

    // Set last entry to 0 to terminate
    gDynamicResFuncs_word_994320[dst_idx].mHashedName = 0;
    gDynamicResFuncs_word_994320[dst_idx].mInitFunctionPointer = nullptr;
    
    if (gSleep5000_after_res_funcs_1_dword_994304 || gSleep5000_after_res_funcs_2_dword_99430C)
    {
        ::Sleep(5000);
    }

    gSleep5000_after_res_funcs_1_dword_994304 = 0;
    gSleep5000_after_res_funcs_2_dword_99430C = 0;

    return 1;
}
MSG_FUNC_IMPL(0x0044E9D2, LibDG_CHARA_44E9D2);

void CC LibDG_SetActiveResourceInitFuncPtrs_457B5B()
{
    memcpy(gDynamicResFuncs_word_994320, gKnownResInitFuncs_669AE0, 4096u); // 4096u = sizeof(gResInitFuncs_669AE0)
    *gpToDynamicResInitFuncs_dword_993F44 = gDynamicResFuncs_word_994320;
}
MSG_FUNC_IMPL(0x457B5B, LibDG_SetActiveResourceInitFuncPtrs_457B5B);

void CC LibDG_ClearActiveResourceFunctionPointerList_457B7C()
{
    memset(gDynamicResFuncs_word_994320, 0, 4096u); // 4096u = sizeof(gResInitFuncs_669AE0)
    *gpToDynamicResInitFuncs_dword_993F44 = gDynamicResFuncs_word_994320;
}
MSG_FUNC_IMPL(0x457B7C, LibDG_ClearActiveResourceFunctionPointerList_457B7C);

static ResInitFn FindFnPtrByHash(Res_Init_Record* arrayToSearch, WORD hashName)
{
    Res_Init_Record* pRecord = arrayToSearch;
    while (pRecord->mInitFunctionPointer)
    {
        if (pRecord->mHashedName == hashName)
        {
            return pRecord->mInitFunctionPointer;
        }
        ++pRecord;
    }
    return nullptr;
}

ResInitFn CC LibDG_GetResourceInitFuncPtr_457BAC(WORD hashedName)
{
    // Search in the 3 "static" records
    ResInitFn result = FindFnPtrByHash(gStaticResInitFuncs_stru_650040, hashedName);
    if (result)
    {
        return result;
    }

    // Search in the 512 "dynamic" records
    result = FindFnPtrByHash(*gpToDynamicResInitFuncs_dword_993F44, hashedName);
    return result;
}
MSG_FUNC_IMPL(0x457BAC, LibDG_GetResourceInitFuncPtr_457BAC);
