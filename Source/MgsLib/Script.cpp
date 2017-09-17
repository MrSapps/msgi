#include "stdafx.h"
#include "MgsFunction.hpp"
#include "logger.hpp"
#include "Task.hpp"
#include "Script.hpp"
#include "Actor_Delay.hpp"
#include "LibDG.hpp"
#include "LibGV.hpp"
#include <gmock/gmock.h>

MGS_VAR(1, 0x9942A8, WORD, byte1_flags_word_9942A8, 0);
MGS_VAR(1, 0x7227A4, DWORD, dword_7227A4, 0);
MGS_VAR(1, 0x6BFBB4, int, gScriptFileNameHashedToLoad_6BFBB4, 0);

#define SCRIPT_IMPL true

MGS_FUNC_NOT_IMPL(0x409924, DWORD CC(BYTE *pScript), Script_Unknown8);
MGS_FUNC_NOT_IMPL(0x409A3A, void CC(), Script_sub_409A3A);
MGS_FUNC_NOT_IMPL(0x4093ED, void CC(), SaveDataStructuresRelated_4093ED);
MGS_PTR(1, 0x6BFC68, BYTE**, gScriptMainProc_dword_6BFC68, 0);
MGS_VAR(1, 0x06BFC3C, BYTE**, gScriptStackPos_dword_6BFC3C, 0); // Pointer to an array of 32 BYTE*'s
MGS_VAR(1, 0x078D7B4, BYTE*, gScriptExecuteRet_dword_78D7B4, 0);
MGS_VAR(1, 0x06BFBB8, DWORD*, script_args_dword_6BFBB8, 0);


MGS_FUNC_NOT_IMPL(0x45A6F6, int __cdecl(int a1, void* a2), sub_45A6F6);

void ScriptCpp_ForceLink()
{

}

struct proc_struct_sub;

struct proc_struct
{
    proc_struct* pNext;
    DWORD mNumCommands;
    proc_struct_sub* pCommandsArray;
};

using proc_sub_ptr = int(CC*)(BYTE *);

struct proc_struct_sub
{
    WORD mId;
    WORD mUnknown;
    proc_sub_ptr mCommandFunction;
};

MGS_PTR(1, 0x6BFC6C, proc_struct**, gScriptCmdTable_dword_6BFC6C, 0);

proc_struct_sub* CC Script_GetCommand(WORD cmdToFind)
{
    proc_struct* i = *gScriptCmdTable_dword_6BFC6C;
    while (i)
    {
        for (DWORD j = 0; j<i->mNumCommands; j++) // WARN: Real game does it backwards but can't see why
        {
            if (i->pCommandsArray[j].mId == cmdToFind)
            {
                return &i->pCommandsArray[j];
            }
        }
        i = i->pNext;
    }
    printf("command not found\n");
    return 0;
}
MGS_FUNC_IMPLEX(0x00409ACC, Script_GetCommand, SCRIPT_IMPL);

int CC Script_InitCommandTable(proc_struct* pCmdTbl)
{
    pCmdTbl->pNext = *gScriptCmdTable_dword_6BFC6C;
    *gScriptCmdTable_dword_6BFC6C = pCmdTbl;
    proc_struct_sub* pItem = pCmdTbl->pCommandsArray;
    if (pCmdTbl->mNumCommands > 0)
    {
        DWORD cmdNum = pCmdTbl->mNumCommands;
        do
        {
            printf("COM %d ADR %X\n", pItem->mId, pItem->mCommandFunction);
            ++pItem;
            --cmdNum;
        } while (cmdNum);
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x00409A4F, Script_InitCommandTable, SCRIPT_IMPL);

DWORD CC Script_ParamExists(char paramId)
{
    DWORD ret = 0;
    DWORD ppScript = 0;

    BYTE* pushedArgumentValue = (BYTE *)*(gScriptStackPos_dword_6BFC3C - 1);
    for (int i=0; i<4096; i++)
    {
        pushedArgumentValue = Script_GCL_Execute(pushedArgumentValue, &ppScript, &ret);
        
        if (!ppScript)
        {
            return 0;
        }

        // Check for 0x[00][id][00][50]
        if ((BYTE)ppScript == 0x50 && (signed int)ppScript >> 16 == paramId)
        {
            gScriptExecuteRet_dword_78D7B4 = (BYTE*)ret;
            return ret;
        }
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x004098D2, Script_ParamExists, SCRIPT_IMPL);

DWORD CC Script_get_int()
{
    DWORD ppScript = 0;
    DWORD pRet = 0;
    BYTE* pScript = Script_GetReturnAddress();
    gScriptExecuteRet_dword_78D7B4 = Script_GCL_Execute(pScript, &ppScript, &pRet);
    return pRet;
}
MGS_FUNC_IMPLEX(0x004099B7, Script_get_int, SCRIPT_IMPL);

BYTE* CC Script_GetReturnAddress()
{
    BYTE* pScript = gScriptExecuteRet_dword_78D7B4;
    if (!gScriptExecuteRet_dword_78D7B4 || !*gScriptExecuteRet_dword_78D7B4 || *gScriptExecuteRet_dword_78D7B4 == 0x50)
    {
        return nullptr;
    }
    return pScript;
}
MGS_FUNC_IMPLEX(0x004099A0, Script_GetReturnAddress, SCRIPT_IMPL);

int CC Script_Unknown6(BYTE* pScript, DWORD* pRet)
{
    UNREFERENCED_PARAMETER(pScript);
    UNREFERENCED_PARAMETER(pRet);
    return 0;
}
MGS_FUNC_IMPLEX(0x00409D77, Script_Unknown6, false); // TODO: Implement me

BYTE* CC Script_4094DC(BYTE* pScript, unsigned int* ppScript, bool* ret)
{
    return nullptr;
}
MGS_FUNC_IMPLEX(0x004094DC, Script_4094DC, false); // TODO: Implement me

BYTE* CC Script_GCL_Execute(BYTE* pScript, DWORD* ppScript, DWORD* pRet)
{
    const signed __int32 gcl_code = *pScript;

    if ((gcl_code & 0xF0) == 16)
    {
        // Read var/mem ??
        return Script_4094DC(pScript, (unsigned int *)ppScript, (bool *)pRet);
    }

    *ppScript = gcl_code;
    BYTE* pScriptByte1 = pScript + 1;

    switch (gcl_code)
    {
    // Read stack argument
    case 0x20:
    {
        *pRet = script_args_dword_6BFBB8[-*pScriptByte1 - 1];
        *ppScript = 1;
        pScriptByte1 = pScript + 2;
    }
    break;

    // ??
    case 0x30:
    {
        *pRet = Script_Unknown6(pScript + 2, pRet);
        pScriptByte1 += *pScriptByte1;
    }
    break;

    // Jump
    case 0x40:
    {
        pScriptByte1 = &pScript[ToWORD(pScriptByte1) + 1];
        *pRet = (DWORD)(pScript + 3);
    }
    break;

    // Parameter
    case 0x50:
    {
        DWORD paramTypeAndId = *pScriptByte1 << 16;
        paramTypeAndId |= 0x50;
        *ppScript = paramTypeAndId;
        *pRet = (DWORD)(pScript + 3);
        pScriptByte1 += pScript[2] + 1;
    }
    break;

    // End
    case 0:
    {
        pScriptByte1 = 0;
    }
    break;

    // Read s16
    case 1:
    {
        int value = static_cast<s16>(ToWORD(pScriptByte1));
        *pRet = value;
        pScriptByte1 = pScript + 3;
    }
    break;

    // Read u8
    case 2:
    case 3:
    case 4:
    {
        *pRet = *pScriptByte1;
        pScriptByte1 = pScript + 2;
    }
    break;

    // Read string
    case 7:
    {
        *pRet = reinterpret_cast<DWORD>(pScript + 2);
        pScriptByte1 += *pScriptByte1 + 1;
    }
    break;

    // Read u16
    case 6:
    case 8:
    {
        *pRet = ToWORD(pScriptByte1);
        pScriptByte1 = pScript + 3;
    }
    break;

    // Read u32
    case 9:
    case 10:
    {
        *pRet = ToDWORD(pScriptByte1);
        pScriptByte1 = pScript + 5;
    }
    break;

    default:
        printf("GCL:WRONG CODE %x\n", gcl_code);
        break;
    }

    gScriptExecuteRet_dword_78D7B4 = pScriptByte1;
    return pScriptByte1;
}
MGS_FUNC_IMPLEX(0x004096CE, Script_GCL_Execute, SCRIPT_IMPL);

static void Test_GCL_Execute_Read_DWORD_9()
{
    DWORD ppScript = 0;
    DWORD ret = 0;
    BYTE script[] = { 0x9, 0x1, 0x2, 0x3, 0x4, 0x5 };
    BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

    ASSERT_EQ(ret, 0x01020304);
    ASSERT_EQ(0x05, *pExeRet);
    ASSERT_EQ(0x09, ppScript);
}

static void Test_GCL_Execute_Read_DWORD_10()
{
    DWORD ppScript = 0;
    DWORD ret = 0;
    BYTE script[] = { 0x0A, 0x1, 0x2, 0x3, 0x4, 0x5 };
    BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

    ASSERT_EQ(ret, 0x01020304);
    ASSERT_EQ(0x05, *pExeRet);
    ASSERT_EQ(0x0A, ppScript);
}

static void Test_GCL_Execute_Read_Argument_Number_0x20()
{
    DWORD args[32] = {};
    for (int i = 0; i < 32; i++)
    {
        args[i] = 0xDEADBEEF + i;
    }
    ScriptSetArgs_409893(args+32);

    DWORD ppScript = 0;
    DWORD ret = 0;
    BYTE script[] = { 0x20, 0x0, 0xf };
    for (int i = 0; i < 32; i++)
    {
        script[1] = static_cast<BYTE>(i);
        BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

        ASSERT_EQ(ret, args[31-i]);
        ASSERT_EQ(0xf, *pExeRet);
        ASSERT_EQ(0x1, ppScript);
    }
}

static void Test_GCL_Execute_Jump1_0x40()
{
    DWORD ppScript = 0;
    DWORD ret = 0;
    BYTE script[] = { 0x40, 0x00, 0xFF, 0xBC };
    BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

    ASSERT_EQ(*(BYTE*)ret, 0xBC);
    ASSERT_EQ(script + 0xFF + 1, pExeRet);
    ASSERT_EQ(0x40, ppScript);
}

static void Test_GCL_Execute_Jump2_0x40()
{
    DWORD ppScript = 0;
    DWORD ret = 0;
    BYTE script[] = { 0x40, 0xF, 0x00, 0xBC };
    BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

    ASSERT_EQ(*(BYTE*)ret, 0xBC);
    ASSERT_EQ(script + 0xF00 + 1, pExeRet);
    ASSERT_EQ(0x40, ppScript);
}

static void Test_GCL_Get_Param0x50()
{
    DWORD ppScript = 0;
    DWORD ret = 0;
    BYTE script[] = { 0x50, 0xAB, 0x03, 0x1, 0x4, 0x12 }; // AB = param name, 2 = len, data
    BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

    ASSERT_EQ(*(BYTE*)ret, 0x1);
    ASSERT_EQ(0x12, *pExeRet);
    ASSERT_EQ(0x00ab0050, ppScript);
}

static void Test_GCL_Execute_Read_WORD_8()
{
    DWORD ppScript = 0;
    DWORD ret = 0;
    BYTE script[] = { 0x08, 0xAA, 0xBB, 0x34 };
    BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

    ASSERT_EQ(ret, 0xAABB);
    ASSERT_EQ(0x34, *pExeRet);
    ASSERT_EQ(0x08, ppScript);
}

static void Test_GCL_Execute_Zero_0()
{
    DWORD ppScript = 0;
    DWORD ret = 0;
    BYTE script[] = { 0x00, 0xFF };
    BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

    ASSERT_EQ(ret, 0x0);
    ASSERT_EQ(nullptr, pExeRet);
    ASSERT_EQ(0x00, ppScript);
}

static void Test_GCL_Execute_WrongCodes_0x60_5()
{
    {
        DWORD ppScript = 0;
        DWORD ret = 0;
        BYTE script[] = { 0x60, 0xFF };
        BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

        ASSERT_EQ(ret, 0x0);
        ASSERT_EQ(0xFF, *pExeRet);
        ASSERT_EQ(0x60, ppScript);
    }

    {
        DWORD ppScript = 0;
        DWORD ret = 0;
        BYTE script[] = { 0x05, 0xAA };
        BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

        ASSERT_EQ(ret, 0x0);
        ASSERT_EQ(0xAA, *pExeRet);
        ASSERT_EQ(0x05, ppScript);
    }
}

static void Test_GCL_Execute_Read_WORD_1() // Also 4, 5 ?
{
    DWORD ppScript = 0;
    DWORD ret = 0;
    BYTE script[] = { 0x01, 0xAA, 0xBB, 0x34 };
    BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

    ASSERT_EQ(ret, 0xffffAABB);
    ASSERT_EQ(0x34, *pExeRet);
    ASSERT_EQ(0x01, ppScript);
}

static void Test_GCL_Execute_ReadByte_2()
{
    DWORD ppScript = 0;
    DWORD ret = 0;
    BYTE script[] = { 0x02, 0xBC, 0xDD };
    BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

    ASSERT_EQ(ret, 0xBC);
    ASSERT_EQ(0xDD, *pExeRet);
    ASSERT_EQ(0x02, ppScript);
}

static void Test_GCL_Execute_ReadByte_3()
{
    DWORD ppScript = 0;
    DWORD ret = 0;
    BYTE script[] = { 0x03, 0xBC, 0xDD };
    BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

    ASSERT_EQ(ret, 0xBC);
    ASSERT_EQ(0xDD, *pExeRet);
    ASSERT_EQ(0x03, ppScript);
}

static void Test_GCL_Execute_ReadByte_4()
{
    DWORD ppScript = 0;
    DWORD ret = 0;
    BYTE script[] = { 0x04, 0xBC, 0xDD };
    BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

    ASSERT_EQ(ret, 0xBC);
    ASSERT_EQ(0xDD, *pExeRet);
    ASSERT_EQ(0x04, ppScript);
}

static void Test_GCL_Execute_ReadWORD_6()
{
    DWORD ppScript = 0;
    DWORD ret = 0;
    BYTE script[] = { 0x06, 0xBC, 0xDD, 0xAA, 0x23 };
    BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

    ASSERT_EQ(ret, 0xBCDD);
    ASSERT_EQ(0xAA, *pExeRet);
    ASSERT_EQ(0x06, ppScript);
}

static void Test_GCL_Execute_ReadString_7()
{
    DWORD ppScript = 0;
    DWORD ret = 0;
    BYTE script[] = { 0x07, 0x5, 'p', 'a', 'u', 'l', 0x0, 0xFE };
    BYTE* pExeRet = Script_GCL_Execute(script, &ppScript, &ret);

    ASSERT_STREQ(reinterpret_cast<const char*>(ret), "paul");
    ASSERT_EQ(0xFE, *pExeRet);
    ASSERT_EQ(0x07, ppScript);
}

static void Test_GCL_Execute()
{
    Test_GCL_Execute_Read_WORD_8();
    Test_GCL_Execute_Read_DWORD_9();
    Test_GCL_Execute_Read_DWORD_10();
    Test_GCL_Execute_Read_Argument_Number_0x20();
    Test_GCL_Execute_Jump1_0x40();
    Test_GCL_Execute_Jump2_0x40();
    Test_GCL_Get_Param0x50();
    Test_GCL_Execute_Zero_0();
    Test_GCL_Execute_WrongCodes_0x60_5();
    Test_GCL_Execute_Read_WORD_1();
    Test_GCL_Execute_ReadByte_2();
    Test_GCL_Execute_ReadByte_3();
    Test_GCL_Execute_ReadByte_4();
    Test_GCL_Execute_ReadWORD_6();
    Test_GCL_Execute_ReadString_7();

    // Nibble1 being 0x10 is not tested because it calls external function
    // 0x30 case is not tested because it calls external function
}

MGS_FUNC_NOT_IMPL(0x00451A5E, int __cdecl(BYTE*), Script_tbl_mesg_sub_451A5E);
MGS_FUNC_NOT_IMPL(0x00451688, int __cdecl(BYTE*), Script_tbl_ntrap_removeQ_451688);
MGS_FUNC_NOT_IMPL(0x0045151D, int __cdecl(BYTE*), Script_tbl_map_sub_45151D);
MGS_FUNC_NOT_IMPL(0x00451673, int __cdecl(BYTE*), Script_tbl_hzd_related_sub_451673);
MGS_FUNC_NOT_IMPL(0x004512E5, int __cdecl(BYTE*), script_tbl_camera_sub_4512E5);
MGS_FUNC_NOT_IMPL(0x00451239, int __cdecl(BYTE*), Script_tbl_light_sub_451239);
MGS_FUNC_NOT_IMPL(0x00451B0E, int __cdecl(BYTE*), Script_tbl_start_sub_451B0E);
MGS_FUNC_NOT_IMPL(0x00451BBF, int __cdecl(BYTE*), Script_tbl_load_451BBF);
MGS_FUNC_NOT_IMPL(0x00451D5C, int __cdecl(BYTE*), Script_tbl_radio_sub_451D5C);
MGS_FUNC_NOT_IMPL(0x00451F22, int __cdecl(BYTE*), Script_tbl_str_status_sub_451F22);
MGS_FUNC_NOT_IMPL(0x00452064, int __cdecl(BYTE*), Script_tbl_demo_sub_452064);
MGS_FUNC_NOT_IMPL(0x00451778, int __cdecl(BYTE*), Script_tbl_ntrap_451778);
MGS_FUNC_NOT_IMPL(0x00452132, int __cdecl(BYTE*), Script_tbl_pad_452132);
MGS_FUNC_NOT_IMPL(0x00451F89, int __cdecl(BYTE*), Script_tbl_varsave_sub_451F89);
MGS_FUNC_NOT_IMPL(0x00451FE3, int __cdecl(BYTE*), Script_tbl_system_sub_451FE3);
MGS_FUNC_NOT_IMPL(0x0045219B, int __cdecl(BYTE*), Script_tbl_sound_45219B);
MGS_FUNC_NOT_IMPL(0x004521A7, int __cdecl(BYTE*), Script_tbl_menu_sub_4521A7);
MGS_FUNC_NOT_IMPL(0x004523C7, int __cdecl(BYTE*), Script_Tbl_rand_4523C7);
MGS_FUNC_NOT_IMPL(0x004523E7, int __cdecl(BYTE*), Script_tbl_sub_4523E7);
MGS_FUNC_NOT_IMPL(0x0045256B, int __cdecl(BYTE*), Script_tbl_print_sub_45256B);
MGS_FUNC_NOT_IMPL(0x004525D6, int __cdecl(BYTE*), Script_tbl_jimaku_4525D6);
MGS_ARY(1, 0x66B000, proc_struct_sub, 24, script_funcs_tbl_66B000,
{
    { 0x22FF, 0x0, Script_tbl_mesg_sub_451A5E.Ptr() },
    { 0xD4CB, 0x0, Script_tbl_ntrap_removeQ_451688.Ptr() },
    { 0x9906, 0x0, Script_tbl_chara_sub_451AC3 },
    { 0xC091, 0x0, Script_tbl_map_sub_45151D.Ptr() },
    { 0x7D50, 0x0, Script_tbl_hzd_related_sub_451673.Ptr() },
    { 0xEEE9, 0x0, script_tbl_camera_sub_4512E5.Ptr() },
    { 0x306A, 0x0, Script_tbl_light_sub_451239.Ptr() },
    { 0x9A1F, 0x0, Script_tbl_start_sub_451B0E.Ptr() },
    { 0xC8BB, 0x0, Script_tbl_load_451BBF.Ptr() },
    { 0x24E1, 0x0, Script_tbl_radio_sub_451D5C.Ptr() },
    { 0xE43C, 0x0, Script_tbl_str_status_sub_451F22.Ptr() },
    { 0xA242, 0x0, Script_tbl_demo_sub_452064.Ptr() },
    { 0xDBAB, 0x0, Script_tbl_ntrap_451778.Ptr() },
    { 0x430D, 0x0, Script_tbl_Delay_sub_4519C7 },
    { 0xCC85, 0x0, Script_tbl_pad_452132.Ptr() },
    { 0x5C9E, 0x0, Script_tbl_varsave_sub_451F89.Ptr() },
    { 0x4AD9, 0x0, Script_tbl_system_sub_451FE3.Ptr() },
    { 0x698D, 0x0, Script_tbl_sound_45219B.Ptr() },
    { 0x226D, 0x0, Script_tbl_menu_sub_4521A7.Ptr() },
    { 0x925E, 0x0, Script_Tbl_rand_4523C7.Ptr() },
    { 0xE257, 0x0, Script_tbl_sub_4523E7.Ptr() },
    { 0xB96E, 0x0, Script_tbl_print_sub_45256B.Ptr() },
    { 0xEC9D, 0x0, Script_tbl_jimaku_4525D6.Ptr() }
});
MGS_ARY(1, 0x66B0B8, proc_struct, 1, gScriptBindFns_66B0B8, { nullptr, 23, script_funcs_tbl_66B000 }); // TODO: Not an array, only done this way so it compiles

MGS_FUNC_NOT_IMPL(0x004525E6, int CC(), Script_binds_init_4525E6);

void CC Script_BindInits_452610()
{
    Script_binds_init_4525E6();
    Script_InitCommandTable(gScriptBindFns_66B0B8);
}
MGS_FUNC_IMPLEX(0x00452610, Script_BindInits_452610, SCRIPT_IMPL);

int CC Script_tbl_eval_sub_40915D(BYTE* pScript)
{
    DWORD cmd = 0;
    DWORD ret = 0;
    Script_GCL_Execute(pScript, &cmd, &ret);
    return 0;
}
MGS_FUNC_IMPLEX(0x40915D, Script_tbl_eval_sub_40915D, SCRIPT_IMPL);

MGS_FUNC_NOT_IMPL(0x004090EA, int CC(BYTE*), Script_tbl_if_sub_4090EA);
MGS_FUNC_NOT_IMPL(0x00409178, int CC(BYTE*), Script_tbl_unknown_loop_sub_409178);
signed int script_tbl_nop_sub_4091F6(BYTE*)
{
    return 1;
}
//MSG_FUNC_IMPL(0x004091F6, script_tbl_nop_sub_4091F6); // Can't hook because func is too small


MGS_ARY(1, 0x6506E0, proc_struct_sub, 4, gEarlyScriptBinds_Tbl_6506E0,
{
    { 0x0d86, 0x0, Script_tbl_if_sub_4090EA.Ptr() },
    { 0x64C0, 0x0, Script_tbl_eval_sub_40915D },
    { 0xCD3A, 0x0, script_tbl_nop_sub_4091F6 },
    { 0x7636, 0x0, Script_tbl_unknown_loop_sub_409178.Ptr() }
});
MGS_ARY(1, 0x650700, proc_struct, 1, gEarlyScriptBinds_650700, { nullptr, 4, gEarlyScriptBinds_Tbl_6506E0 }); // TODO: Not an array, only done this way so it compiles

void CC Script_sub_4091FA()
{
    Script_InitCommandTable(gEarlyScriptBinds_650700);
}
MGS_FUNC_IMPLEX(0x004091FA, Script_sub_4091FA, SCRIPT_IMPL);

MGS_PTR(1, 0x006BFC60, GCL_ProcInfo**, gProcInfos, nullptr);
MGS_PTR(1, 0x006BFC64, BYTE**, dword_6BFC64, 0);

BYTE* CC Script_InitProcTables_sub_409C87(BYTE* pScript)
{
    // pScript skips the first 4 bytes of the script
    for (;;)
    {
        // Reverse the GCL_ProcInfo data
        std::swap(pScript[0], pScript[1]);
        std::swap(pScript[2], pScript[3]);
        pScript += 4;

        // 4 NULL bytes is the end of the table
        if (pScript[0] == 0 && pScript[1] == 0 && pScript[2] == 0 && pScript[3] == 0)
        {
            break;
        }
    }

    return pScript + 4;
}
MGS_FUNC_IMPLEX(0x00409C87, Script_InitProcTables_sub_409C87, SCRIPT_IMPL);

int CC Script_Init_sub_409C19(BYTE* pScript)
{
    DWORD offset = ToDWORD(pScript);
    *gProcInfos = reinterpret_cast<GCL_ProcInfo*>(pScript + 4);
    
    // Swap bytes in the table of script functions so they are the correct ordering
    *dword_6BFC64 = Script_InitProcTables_sub_409C87(pScript + 4);

    // Get pointer to main script function start
    *gScriptMainProc_dword_6BFC68 = (pScript + 4 + offset + 4);

    DWORD mainScriptLen = ToDWORD((*gScriptMainProc_dword_6BFC68)-4);
    BYTE* pEndOfScriptData = (*gScriptMainProc_dword_6BFC68) + mainScriptLen + 4;

    sub_45A6F6(2, pEndOfScriptData);

    return 0;
}
MGS_FUNC_IMPLEX(0x00409C19, Script_Init_sub_409C19, SCRIPT_IMPL);

signed int CC GCX_FileHandler_4090CF(void* pScript, int fileNameHashed)
{
    if (fileNameHashed == gScriptFileNameHashedToLoad_6BFBB4)
    {
        Script_Init_sub_409C19(reinterpret_cast<BYTE*>(pScript));
    }
    return 1;
}
MGS_FUNC_IMPLEX(0x004090CF, GCX_FileHandler_4090CF, SCRIPT_IMPL);

void CC Script_Set_MainOrDemo_40908E(int bMain)
{
    gScriptFileNameHashedToLoad_6BFBB4 = bMain != 1  ? 
        0x6EA54 :  // scenerio 
        0x6A242;   // demo
}
MGS_FUNC_IMPLEX(0x0040908E, Script_Set_MainOrDemo_40908E, SCRIPT_IMPL);

void CC ScriptEngineInit_4090A7()
{
    Script_sub_409A3A(); // Stack setup?
    SaveDataStructuresRelated_4093ED(); // Script vars memory?
    Script_sub_4091FA();
    LibGV_Set_FileExtHandler_40A68D('g', GCX_FileHandler_4090CF); // .gcx
    gScriptFileNameHashedToLoad_6BFBB4 = 0x6EA54;       // Load only scenerio gcx by default instead of demo gcx
}
MGS_FUNC_IMPLEX(0x004090A7, ScriptEngineInit_4090A7, SCRIPT_IMPL);

BYTE* CC Script_FindProc(WORD procId)
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
MGS_FUNC_IMPLEX(0x00409B1D, Script_FindProc, SCRIPT_IMPL);

signed int CC Script_Run_Proc_sub_409B03(WORD procId, GCL_Proc_Arguments* pArgs)
{
    BYTE* pScriptProc = Script_FindProc(procId);
    return Script_Run(pScriptProc + 3, pArgs);
}
MGS_FUNC_IMPLEX(0x409B03, Script_Run_Proc_sub_409B03, SCRIPT_IMPL);

signed int CC Script_ProcCancelOrRun(WORD id, GCL_Proc_Arguments* pArgs)
{
    if (script_cancel_non_zero_dword_7227A0 || BYTE1(byte1_flags_word_9942A8) & 0x20)
    {
        printf("proc %d cancel\n", id);
        return 0;
    }
    else
    {
        BYTE* scriptProc = Script_FindProc(id);
        assert(scriptProc);
        return Script_Run(scriptProc + 3, pArgs);
    }
}
MGS_FUNC_IMPLEX(0x00409B53, Script_ProcCancelOrRun, SCRIPT_IMPL);

int CC Script_Operator_Evaluate(int operation, int v1, int v2)
{
    int result = 0;
    switch (operation)
    {
    case eNegate:
        result = -v2;
        break;
    case eIsZero:
        result = v2 == 0;
        break;
    case eComplement:
        result = ~v2;
        break;
    case eAdd:
        result = v2 + v1;
        break;
    case eSubtract:
        result = v1 - v2;
        break;
    case eMultiply:
        result = v2 * v1;
        break;
    case eDivide:
        result = v1 / v2;
        break;
    case eModulus:
        result = v1 % v2;
        break;
    case eEquals:
        result = v1 == v2;
        break;
    case eNotEquals:
        result = v1 != v2;
        break;
    case eLessThan:
        result = v1 < v2;
        break;
    case eLessThanOrEqual:
        result = v1 <= v2;
        break;
    case eGreaterThan:
        result = v1 > v2;
        break;
    case eGreaterThanOrEqual:
        result = v1 >= v2;
        break;
    case eBitWiseOR:
        result = v2 | v1;
        break;
    case eBitWiseAND:
        result = v2 & v1;
        break;
    case eBitWiseXOR:
        result = v2 ^ v1;
        break;
    case eOr:
        result = v1 || v2;
        break;
    case eAnd:
        result = v1 && v2;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}
MGS_FUNC_IMPLEX(0x00409E7C, Script_Operator_Evaluate, SCRIPT_IMPL);

void CC Script_Push(BYTE *arg)
{
    *gScriptStackPos_dword_6BFC3C = arg;
    ++(gScriptStackPos_dword_6BFC3C);
}
MGS_FUNC_IMPLEX(0x004098B7, Script_Push, SCRIPT_IMPL);

void CC Script_Pop()
{
    --(gScriptStackPos_dword_6BFC3C);
}
MGS_FUNC_IMPLEX(0x004098CA, Script_Pop, SCRIPT_IMPL);

void CC Script_SetReturnAddress(BYTE* pScript)
{
    (gScriptExecuteRet_dword_78D7B4) = pScript;
}
MGS_FUNC_IMPLEX(0x004096C4, Script_SetReturnAddress, SCRIPT_IMPL);

int CC Script_CommandExecute(BYTE* pScript)
{
    const WORD cmdId = ToWORD(pScript);
    proc_struct_sub* pScriptCmd = Script_GetCommand(cmdId);
    Script_Push(&pScript[pScript[2] + 2]);
    Script_SetReturnAddress(pScript + 3);
    const int cmdRet = pScriptCmd->mCommandFunction(pScript + 3);
    Script_Pop();
    return cmdRet;
}
MGS_FUNC_IMPLEX(0x00409A8D, Script_CommandExecute, SCRIPT_IMPL);

DWORD* CC Script_PushArgs_409845(GCL_Proc_Arguments* pArgs)
{
    if (!pArgs)
    {
        return nullptr;
    }

    DWORD* oldArgStack = script_args_dword_6BFBB8;
    DWORD* pInputArgsReverse = &pArgs->mPArgs[pArgs->mNumArgs - 1];
    for (int i = 0; i < pArgs->mNumArgs; i++)
    {
        // Write arg
        *script_args_dword_6BFBB8 = *pInputArgsReverse;
        ++script_args_dword_6BFBB8;
        --pInputArgsReverse;
    }

    // Write num args
    *script_args_dword_6BFBB8 = pArgs->mNumArgs;
    ++script_args_dword_6BFBB8;

    // Return write start pos
    return oldArgStack;
}
MGS_FUNC_IMPLEX(0x00409845, Script_PushArgs_409845, SCRIPT_IMPL);

void CC ScriptSetArgs_409893(DWORD* pArgs)
{
    if (pArgs)
    {
        script_args_dword_6BFBB8 = pArgs;
    }
}
MGS_FUNC_IMPLEX(0x00409893, ScriptSetArgs_409893, SCRIPT_IMPL);

signed int CC Script_Run(BYTE* pScriptBytes, GCL_Proc_Arguments* pArgs)
{
    BYTE* pScript = pScriptBytes;
    DWORD* pStackOldArgs = Script_PushArgs_409845(pArgs);
    while (pScript)
    {
        // Are we at the end?
        BYTE cmd = *pScript;
        if (!*pScript)
        {
            // Reset the stack
            ScriptSetArgs_409893(pStackOldArgs);
            return 0;
        }

        if (cmd == 0x30)
        {
            DWORD dwRet = 0;
            Script_Unknown6(pScript + 2, &dwRet /* &pScriptBytes*/);
            const int length = *(pScript + 1);
            pScript = pScript + length + 1;
        }
        else if (cmd == 0x60)
        {
            if (Script_CommandExecute(pScript + 3) == 1)
            {
                // Returned error
                return 1;
            }

            const int length = ToWORD(pScript + 1);
            pScript = pScript + length + 1;
        }
        else if (cmd == 0x70) // Call
        {
            Script_RunProc(pScript + 2);
            const int length = *(pScript + 1);
            pScript = pScript + length + 1;
        }
        else
        {
            printf("SCRIPT COMMAND ERROR %x\n", cmd);
        }
    }
    printf("ERROR in script\n");
    return 1;
}
MGS_FUNC_IMPLEX(0x00409CAF, Script_Run, SCRIPT_IMPL);

int CC Script_RunProc(BYTE* pScript)
{
    // Get the proc id/hashed name
    const WORD id = ToWORD(pScript);

    WORD numArgs = 0;

    DWORD argsArray[8] = {};
    DWORD ret = 0;
    DWORD ppScript = 0;
    BYTE* pExecRet = Script_GCL_Execute(pScript + 2, &ppScript, &ret);
    if (ppScript)
    {
        DWORD* pArg = argsArray;
        do
        {
            if (numArgs >= 8)
            {
                printf("TOO MANY ARGS PROC\n");
            }
           
            *pArg = ret;
            ++pArg;
            ++numArgs;
            pExecRet = Script_GCL_Execute(pExecRet, &ppScript, &ret);
        } while (ppScript);
    }

    GCL_Proc_Arguments args = {};
    args.mNumArgs = numArgs;
    args.mPArgs = argsArray;
    Script_ProcCancelOrRun(id, &args);
    return 0;
}
MGS_FUNC_IMPLEX(0x00409B92, Script_RunProc, SCRIPT_IMPL);


static void Test_Script_Operator_Evaluate()
{
    ASSERT_EQ(-50, Script_Operator_Evaluate(eNegate, 0, 50));
    ASSERT_EQ(0, Script_Operator_Evaluate(eIsZero, 0, 50));
    ASSERT_EQ(1, Script_Operator_Evaluate(eIsZero, 0, 0));
    ASSERT_EQ(~50, Script_Operator_Evaluate(eComplement, 0, 50));
    ASSERT_EQ(4, Script_Operator_Evaluate(eAdd, 2, 2));
    ASSERT_EQ(3, Script_Operator_Evaluate(eSubtract, 5, 2));
    ASSERT_EQ(10, Script_Operator_Evaluate(eMultiply, 5, 2));
    ASSERT_EQ(5, Script_Operator_Evaluate(eDivide, 10, 2));
    ASSERT_EQ(1, Script_Operator_Evaluate(eModulus, 10, 3));
    ASSERT_EQ(0, Script_Operator_Evaluate(eEquals, 10, 3));
    ASSERT_EQ(1, Script_Operator_Evaluate(eEquals, 10, 10));
    ASSERT_EQ(1, Script_Operator_Evaluate(eNotEquals, 10, 3));
    ASSERT_EQ(0, Script_Operator_Evaluate(eNotEquals, 10, 10));
    ASSERT_EQ(1, Script_Operator_Evaluate(eLessThan, 9, 10));
    ASSERT_EQ(0, Script_Operator_Evaluate(eLessThan, 10, 10));
    ASSERT_EQ(1, Script_Operator_Evaluate(eLessThanOrEqual, 9, 10));
    ASSERT_EQ(1, Script_Operator_Evaluate(eLessThanOrEqual, 10, 10));
    ASSERT_EQ(0, Script_Operator_Evaluate(eLessThanOrEqual, 11, 10));
    ASSERT_EQ(1, Script_Operator_Evaluate(eGreaterThan, 10, 9));
    ASSERT_EQ(0, Script_Operator_Evaluate(eGreaterThan, 10, 10));
    ASSERT_EQ(1, Script_Operator_Evaluate(eGreaterThanOrEqual, 10, 9));
    ASSERT_EQ(1, Script_Operator_Evaluate(eGreaterThanOrEqual, 10, 10));
    ASSERT_EQ(0, Script_Operator_Evaluate(eGreaterThanOrEqual, 10, 11));
    ASSERT_EQ(5, Script_Operator_Evaluate(eBitWiseOR, 1, 4));
    ASSERT_EQ(4, Script_Operator_Evaluate(eBitWiseAND, 5, 4));
    ASSERT_EQ(6, Script_Operator_Evaluate(eBitWiseXOR, 2, 4));
    ASSERT_EQ(0, Script_Operator_Evaluate(eOr, 0, 0));
    ASSERT_EQ(1, Script_Operator_Evaluate(eOr, 1, 0));
    ASSERT_EQ(1, Script_Operator_Evaluate(eOr, 1, 1));
    ASSERT_EQ(1, Script_Operator_Evaluate(eOr, 0, 1));
    ASSERT_EQ(0, Script_Operator_Evaluate(eAnd, 0, 0));
    ASSERT_EQ(0, Script_Operator_Evaluate(eAnd, 1, 0));
    ASSERT_EQ(1, Script_Operator_Evaluate(eAnd, 1, 1));
    ASSERT_EQ(0, Script_Operator_Evaluate(eAnd, 0, 1));
}

void DoScriptTests()
{
    Test_Script_Operator_Evaluate();
    Test_GCL_Execute();
}
