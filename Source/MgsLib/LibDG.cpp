#include "stdafx.h"
#include "LibDG.hpp"

void LibDGCppForceLink() { }

MGS_ARY(1, 0x669AE0, Res_Init_Record, 512, gResInitFuncs_669AE0, {});
MGS_ARY(1, 0x994320, Res_Init_Record, 512, gActiveResFuncs1_word_994320, {});
MGS_PTR(1, 0x993F44, Res_Init_Record**, gpActiveResInitFuncs_dword_993F44, nullptr);

MGS_VAR(1, 0x994304, DWORD, gSleep5000_after_res_funcs_1_dword_994304, 0);
MGS_VAR(1, 0x99430C, DWORD, gSleep5000_after_res_funcs_2_dword_99430C, 0);

signed int CC LibDG_CHARA_44E9D2(Res_Init_Record* pStartingRecord)
{
    int dst_idx = 0;
    for (;;)
    {
        int src_idx = 0;
        if (!pStartingRecord->mInitFunctionPointer)
        {
            break;
        }

        while (gResInitFuncs_669AE0[src_idx].mHashedName)
        {
            if (gResInitFuncs_669AE0[src_idx].mHashedName == pStartingRecord->mHashedName)
            {
                gActiveResFuncs1_word_994320[dst_idx].mHashedName = pStartingRecord->mHashedName;
                gActiveResFuncs1_word_994320[dst_idx].mInitFunctionPointer = gResInitFuncs_669AE0[src_idx].mInitFunctionPointer;
                break;
            }
            src_idx++;
        }
        printf("CHARA %d, %p\n", gActiveResFuncs1_word_994320[dst_idx].mHashedName, gActiveResFuncs1_word_994320[dst_idx].mInitFunctionPointer);

        ++dst_idx;
        ++pStartingRecord;
    }

    // Set last entry to 0 to terminate
    gActiveResFuncs1_word_994320[dst_idx].mHashedName = 0;
    gActiveResFuncs1_word_994320[dst_idx].mInitFunctionPointer = nullptr;
    
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
    memcpy(gActiveResFuncs1_word_994320, gResInitFuncs_669AE0, 4096u); // 4096u = sizeof(gResInitFuncs_669AE0)
    *gpActiveResInitFuncs_dword_993F44 = gActiveResFuncs1_word_994320;
}
MSG_FUNC_IMPL(0x457B5B, LibDG_SetActiveResourceInitFuncPtrs_457B5B);
