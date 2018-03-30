#include "stdafx.h"
#include "MgsFunction.hpp"
#include "logger.hpp"
#include "Task.hpp"
#include "Script.hpp"
#include "Actor_Delay.hpp"
#include "LibDG.hpp"
#include "LibGV.hpp"
#include "ResourceNameHash.hpp"
#include "Actor_GameD.hpp"
#include "LibGV.hpp"
#include "Map.hpp"
#include "Menu.hpp"
#include "WinMain.hpp"
#include "LibDG.hpp"
#include "Font.hpp"
#include <gmock/gmock.h>

MGS_VAR(1, 0x9942A8, DWORD, byte1_flags_word_9942A8, 0);
MGS_VAR(1, 0x7227A4, int, gGameOverTimer_dword_7227A4, 0);
MGS_VAR(1, 0x6BFBB4, TFileNameHash, gScriptFileNameHashedToLoad_6BFBB4, 0);

#define SCRIPT_IMPL true

MGS_FUNC_NOT_IMPL(0x4093ED, void CC(), SaveDataStructuresRelated_4093ED);
MGS_VAR(1, 0x6BFC68, BYTE*, gScriptMainProc_dword_6BFC68, nullptr);
MGS_VAR(1, 0x06BFC3C, BYTE**, gScriptStackPos_dword_6BFC3C, 0); // Pointer to an array of 32 BYTE*'s
MGS_VAR(1, 0x078D7B4, BYTE*, gScriptExecuteRet_dword_78D7B4, 0);
MGS_VAR(1, 0x06BFBB8, DWORD*, script_args_dword_6BFBB8, 0);

MGS_ARY(1, 0x6BFBBC, DWORD, 32, gScript_args_stack_32_dword_6BFBBC, {});
MGS_ARY(1, 0x6BFC40, BYTE*, 8, gScript_stack_8_dword_6BFC40, {});

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

MGS_PTR(1, 0x6BFC6C, proc_struct*, gScriptCmdTable_dword_6BFC6C, nullptr);

void CC Script_sub_409A3A()
{
    script_args_dword_6BFBB8 = gScript_args_stack_32_dword_6BFBBC;
    gScriptStackPos_dword_6BFC3C = gScript_stack_8_dword_6BFC40;
}
MGS_FUNC_IMPLEX(0x00409A3A, Script_sub_409A3A, SCRIPT_IMPL);

proc_struct_sub* CC Script_GetCommand(WORD cmdToFind)
{
    proc_struct* i = gScriptCmdTable_dword_6BFC6C;
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
    pCmdTbl->pNext = gScriptCmdTable_dword_6BFC6C;
    gScriptCmdTable_dword_6BFC6C = pCmdTbl;
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

BYTE* CC Script_VarWrite_40957B(BYTE* pScript, int)
{
    return pScript + 4;
}
MGS_FUNC_IMPLEX(0x40957B, Script_VarWrite_40957B, false); // TODO

struct GCL_Context
{
    DWORD field_0_eval_ret;
    BYTE* field_4_script_ip;
};
MGS_ASSERT_SIZEOF(GCL_Context, 8);

MGS_ARY(1, 0x992040, GCL_Context, 40, gGclContextStack_992040, {});

int CC Script_Unknown6_409D77(BYTE* pScript, DWORD* pRet)
{
    int stackPos = 0;
    GCL_Context* pScriptContext = &gGclContextStack_992040[0];
    for (BYTE* pScriptIp = pScript; ; pScriptIp += 2)
    {
        for (;;)
        {
            if (!pScriptIp)
            {
                if (pRet)
                {
                    *pRet = 0;
                }
                return 0;
            }
            DWORD cmd = *pScriptIp;
            if (*pScriptIp == 0x31) // Switch to popping items
            {
                break;
            }
            pScriptContext->field_4_script_ip = pScriptIp;
            DWORD ret = 0;
            pScriptIp = Script_GCL_Execute(pScriptIp, &cmd, &ret);
            pScriptContext->field_0_eval_ret = ret;
            ++pScriptContext;
            stackPos++;

            assert(stackPos < 40); // TODO: Its unknown what the max allowed depth should be
        } // End context push loop

        // Final item
        if (!pScriptIp[1])
        {
            break;
        }

        if (pScriptIp[1] == 0x14)
        {
            Script_VarWrite_40957B(pScriptContext[-2].field_4_script_ip, pScriptContext[-1].field_0_eval_ret);
            pScriptContext[-2].field_0_eval_ret = pScriptContext[-1].field_0_eval_ret;
        }
        else
        {
            pScriptContext[-2].field_0_eval_ret = Script_Operator_Evaluate(pScriptIp[1],
                    pScriptContext[-2].field_0_eval_ret,
                    pScriptContext[-1].field_0_eval_ret);
            pScriptContext[-2].field_4_script_ip = 0;
        }
        --pScriptContext;
        stackPos--;
    }

    assert(stackPos == 1);

    if (pRet)
    {
        *pRet = pScriptContext[-1].field_0_eval_ret;
    }

    return pScriptContext[-1].field_0_eval_ret;
}
MGS_FUNC_IMPLEX(0x00409D77, Script_Unknown6_409D77, SCRIPT_IMPL);

MGS_ARY(1, 0x78E7E0, __int16, 96, save_data_192_word_78E7E0, {});
MGS_ARY(1, 0x78D7C0, DWORD, 512, save_data_2048_unk_78D7C0, {});

BYTE* CC Script_VarRead_4094DC(BYTE* pScript, DWORD* ppScript, DWORD* ret)
{
    DWORD scriptDWORD = ToDWORD(pScript);

    const unsigned int topNibble = (scriptDWORD >> 24) & 0xF;
    *ppScript = topNibble;
    const __int16* pDataBank = save_data_192_word_78E7E0;
    const DWORD tmpBits = scriptDWORD & 0xF00000;
    if ((tmpBits) != 0x800000) // 0x100000 == GCL_VAR_TYPE_LOCAL
    {
        pDataBank = (const __int16 *)save_data_2048_unk_78D7C0;
    }

    WORD idx = ((unsigned __int16)scriptDWORD);
    const __int16* pDataBankEntry = (const __int16 *)((char *)pDataBank + idx);
    
    if (topNibble == 1)
    {
        *ret = (__int16)*pDataBankEntry;
        return pScript + 4;
    }

    if (topNibble <= 1)
    {
        return pScript + 4;
    }

    if (topNibble <= 3)
    {
        *ret = *(BYTE *)pDataBankEntry;
        return pScript + 4;
    }

    if (topNibble == 4)
    {
        DWORD tmp = (scriptDWORD >> 16) & 0xF;
        *ret = (*(BYTE *)pDataBankEntry & (unsigned __int8)(1 << (tmp))) != 0;
        return pScript + 4;
    }

    if (topNibble == 6 || topNibble == 8)
    {
        *ret = (unsigned __int16)*pDataBankEntry;
        return pScript + 4;
    }
    return pScript + 4;
}
MGS_FUNC_IMPLEX(0x004094DC, Script_VarRead_4094DC, false); // TODO: Implement me

BYTE* CC Script_VarWrite_409615(BYTE* pScript)
{
    LOG_INFO("TODO: Write");
    return pScript + 4;
}
MGS_FUNC_IMPLEX(0x00409615, Script_VarWrite_409615, false); // TODO: Implement me

/*
// Is var read          data type
//[1]            [000]  [0000]

// Is var read = b4
// Bank select = b24
// 0b100000000000000000000000
//    if ((*pScript & 0xF0) == 0x10)


// b1 = N1[is var] N2[read/data type]
// b2 = N1[bank select ??] / N2[ bit to read if data type == 4 ?]
// b3 = idx
// b4 = idx

// 0x F0 0000 ) != 0x800000
//    BYTE script[] = { 0xA6, 0xBB, 0xCC, 0xDD, 0x4 };
*/

static void Test_Script_Read_s16_Bank1()
{
    memset(save_data_192_word_78E7E0, 0, 192);
    memset(save_data_2048_unk_78D7C0, 0, 2048);

    save_data_192_word_78E7E0[(0x1234 / 2)] = (WORD)0xAABB;
    save_data_192_word_78E7E0[(0x1234 / 2) + 1] = (WORD)0xCCDD;

    DWORD ppScript = 0;
    DWORD ret = 0;

    BYTE script[] = { 0xA1, 0x8C, 0x12, 0x34, 0xE };
    BYTE* pExeRet = Script_VarRead_4094DC(script, &ppScript, &ret); // 0x a1 0c 12 34 &  0xF0 00 00

    ASSERT_EQ(ret, 0xffffaabb); // read var
    ASSERT_EQ(0x0e, *pExeRet);
    ASSERT_EQ(0x01, ppScript); // top nibble
}

static void Test_Script_Read_s16_Bank2()
{
    /* // TODO FIX ME 0x1234 / 4 is way out of bounds
    memset(save_data_192_word_78E7E0, 0, 192);
    memset(save_data_2048_unk_78D7C0, 0, 2048);

    save_data_2048_unk_78D7C0[0x1234 / 4] = 0xAABBCCDD;

    DWORD ppScript = 0;
    DWORD ret = 0;

    BYTE script[] = { 0xA1, 0x8C, 0x12, 0x34, 0xE };
    BYTE* pExeRet = Script_VarRead_4094DC(script, &ppScript, &ret);

    ASSERT_EQ(ret, 0xffffaabb); // read var
    ASSERT_EQ(0x0e, *pExeRet);
    ASSERT_EQ(0x01, ppScript); // top nibble
    */
}

static void Test_Script_VarRead_4094DC()
{
    Test_Script_Read_s16_Bank1();
    Test_Script_Read_s16_Bank2();
    //Sleep(2000);
    //ExitProcess(0);
}

BYTE* CC Script_GCL_Execute(BYTE* pScript, DWORD* ppScript, DWORD* pRet)
{
    const signed __int32 gcl_code = *pScript;

    if ((gcl_code & 0xF0) == 16) // == GCL_VAR as seen in system.dll of metal gear arcade
    {
        return Script_VarRead_4094DC(pScript, ppScript, pRet);
    }

    *ppScript = gcl_code;
    BYTE* pScriptByte1 = pScript + 1;

    switch (gcl_code)
    {
    // Read stack argument
    case 0x20:
    {
        // GCL_ARRAY in MGA
        *pRet = script_args_dword_6BFBB8[-*pScriptByte1 - 1];
        *ppScript = 1;
        pScriptByte1 = pScript + 2;
    }
    break;

    // ??
    case 0x30:
    {
        *pRet = Script_Unknown6_409D77(pScript + 2, pRet);
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
int CC Script_tbl_mesg_sub_451A5E(BYTE* /*pScript*/)
{
    LibGV_Msg msg = {};
    msg.field_0_res_hash = static_cast<WORD>(Script_get_int());
    WORD* pMsgField = &msg.field_4_action_hash_or_ptr;
    WORD count = 0;
    for(;;)
    {
        BYTE* scriptRet = Script_GetReturnAddress();
        if (!scriptRet)
        {
            break;
        }
        DWORD scriptData =  Script_Unknown8_409924(scriptRet);
        *pMsgField = static_cast<WORD>(scriptData);
        ++pMsgField;
        count++;
    }
    assert(count < 8);
    msg.field_12_num_valid_fields = count;

    LOG_INFO("GCL MSG write: 0x" << std::hex << msg.field_0_res_hash << " 0x" << msg.field_4_action_hash_or_ptr);

    return (LibGV_mesg_write_40B3ED(&msg) >= 0) - 1;
}
MGS_FUNC_IMPLEX(0x451A5E, Script_tbl_mesg_sub_451A5E, SCRIPT_IMPL);

char* CC Script_read_string_arg_40997B(BYTE* pScript)
{
    DWORD ret = 0;
    DWORD cmd = 0;
    gScriptExecuteRet_dword_78D7B4 = Script_GCL_Execute(pScript, &cmd, &ret);
    char* pStr = gScriptExecuteRet_dword_78D7B4 != nullptr ? reinterpret_cast<char*>(ret) : nullptr;
    LOG_INFO("Str= " << pStr);
    return pStr;
}
MGS_FUNC_IMPLEX(0x40997B, Script_read_string_arg_40997B, SCRIPT_IMPL);

int CC Script_Read3Words_409945(BYTE* pScript, WORD* pOut)
{
    DWORD ret = 0;
    DWORD cmd = 0;
    for (int i=0; i<3; i++)
    {
        pScript = Script_GCL_Execute(pScript, &cmd, &ret);
        pOut[i] = static_cast<WORD>(ret);
    }
    gScriptExecuteRet_dword_78D7B4 = pScript;
    return 0;
}
MGS_FUNC_IMPLEX(0x409945, Script_Read3Words_409945, SCRIPT_IMPL);

DWORD CC Script_Unknown8_409924(BYTE* pScript)
{
    DWORD ret = 0;
    DWORD cmd = 0;
    gScriptExecuteRet_dword_78D7B4 = Script_GCL_Execute(pScript, &cmd, &ret);
    return ret;
}
MGS_FUNC_IMPLEX(0x409924, Script_Unknown8_409924, SCRIPT_IMPL);

MGS_VAR(1, 0x723650, DWORD, gKillResOpen_723650, 0);
MGS_VAR(1, 0x66AFF0, DWORD, gResOpenCreated_dword_66AFF0, 1);

MGS_ARY(1, 0x722A44, char, 8, sLastStageName_722A44, {});

int CC Script_tbl_load_451BBF(BYTE* /*pScript*/)
{
    const char* scriptStringData = Script_read_string_arg_40997B(Script_GetReturnAddress());

    if (*scriptStringData)
    {
        if (!strcmp("dummy", scriptStringData) && !gResOpenCreated_dword_66AFF0)
        {
            gKillResOpen_723650 = 1;
        }

        if (Script_ParamExists('r'))
        {
            // Restart map
            if (Script_get_int())
            {
                // Soft restart?
                Stage_LoadRelated_44EB27(gGameStates_78E7E0.stage_name_hash_word_78E7EC, sLastStageName_722A44);
            }
            else
            {
                // Hard restart?
                strcpy(sLastStageName_722A44, File_StageName_44EB83());
                LibGV_Reset_System2_Memory_40B35E();
                LibGV_Init_FileCache_40A6AC();
                LibDG_Clear_Resident_Texture_Cache_Copy_4026E6();
                Stage_LoadRelated_44EB27(ResourceNameHash(scriptStringData), scriptStringData);
            }
            script_cancel_non_zero_dword_7227A0 = 1;
        }
        else
        {
            gGameStates_78E7E0.field_94 = gGameStates_78E7E0.stage_name_hash_word_78E7EC;
            gGameStates_78E7E0.stage_name_hash_word_78E7EC = ResourceNameHash(scriptStringData);
            Stage_LoadRelated_44EB27(gGameStates_78E7E0.stage_name_hash_word_78E7EC, scriptStringData);

            if (Script_ParamExists('m'))
            {
                // ?
                gGameStates_78E7E0.gScript_loader_param_m_78E7EE = static_cast<WORD>(Script_get_int());
            }

            if (Script_ParamExists('p'))
            {
                // Snakes starting position in the map?
                WORD pWordArray3[3] = {};
                Script_Read3Words_409945(Script_GetReturnAddress(), pWordArray3);
                gGameStates_78E7E0.gScript_loader_param_p_78E7F0[0] = pWordArray3[0];
                gGameStates_78E7E0.gScript_loader_param_p_78E7F0[1] = pWordArray3[1];
                gGameStates_78E7E0.gScript_loader_param_p_78E7F0[2] = pWordArray3[2];
            }

            if (Script_ParamExists('s'))
            {
                // ?
                script_cancel_non_zero_dword_7227A0 = Script_get_int();
                if (script_cancel_non_zero_dword_7227A0)
                {
                    script_cancel_non_zero_dword_7227A0 |= 0x80;
                }
            }
            else
            {
                script_cancel_non_zero_dword_7227A0 = 1;
            }

            if (!Script_ParamExists('n'))
            {
                // ?
                script_cancel_non_zero_dword_7227A0 |= 0x10;
            }
        }
    }
    else
    {
        script_cancel_non_zero_dword_7227A0 = 1;
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x451BBF, Script_tbl_load_451BBF, SCRIPT_IMPL);

void CC Script_Set_MainOrDemo_40908E(DWORD bMain)
{
    gScriptFileNameHashedToLoad_6BFBB4 = bMain != 1 ?
        HashFileName_40A58B(ResourceNameHash("scenerio"), 'g') :
        HashFileName_40A58B(ResourceNameHash("demo"), 'g');
}
MGS_FUNC_IMPLEX(0x0040908E, Script_Set_MainOrDemo_40908E, SCRIPT_IMPL);

MGS_FUNC_NOT_IMPL(0x45A497, void __cdecl(), Script_Clear_Named_Vars_45A497);
MGS_FUNC_NOT_IMPL(0x409433, void __cdecl(), sub_409433);

int CC Script_tbl_start_sub_451B0E(BYTE* /*pScript*/)
{
    if (Script_ParamExists('s'))
    {
        MenuTextureLoad_44DEB3();
    }

    if (Script_ParamExists('m'))
    {
        Menu_create_459891();
        printf("MENU INIT END\n");
    }

    if (Script_ParamExists('f'))
    {
        Font_load_fonts_45A5AF();
    }

    if (Script_ParamExists('v'))
    {
        SaveDataStructuresRelated_4093ED();
        Script_Clear_Named_Vars_45A497();
        gTotalFrameTime_dword_995344 = 0;
    }

    if (Script_ParamExists('d'))
    {
        Script_Set_MainOrDemo_40908E(Script_get_int());
    }

    if (Script_ParamExists('c'))
    {
        // Clear a save data bank and stage name stack?
        sub_409433();
        Script_Clear_Named_Vars_45A497();
        gTotalFrameTime_dword_995344 = 0;
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x451B0E, Script_tbl_start_sub_451B0E, SCRIPT_IMPL);

const SVECTOR gLightNormalVec_650128_tmp = { 0, 0, 4096, 0 };
MGS_VAR(1, 0x650128, SVECTOR, gLightNormalVec_650128, gLightNormalVec_650128_tmp);

void CC Script_light_vec_402144(int x, int y, int z)
{
    VECTOR vec1 = {};
    vec1.field_0_x = x;
    vec1.field_4_y = y;
    vec1.field_8_z = z;

    VECTOR vec1Normal = {};
    VectorNormal_44CAE0(&vec1, &vec1Normal);

    gLightNormalVec_650128.field_0_x = static_cast<short int>(vec1Normal.field_0_x);
    gLightNormalVec_650128.field_2_y = static_cast<short int>(vec1Normal.field_4_y);
    gLightNormalVec_650128.field_4_z = static_cast<short int>(vec1Normal.field_8_z);
}
MGS_FUNC_IMPLEX(0x00402144, Script_light_vec_402144, SCRIPT_IMPL);

const MATRIX3x3 gLightMatrix_650148_tmp =
{ 
    { 
        { 4096, 4096, 4096 },
        { 4096, 4096, 4096 },
        { 4096, 4096, 4096 } 
    } 
};
MGS_VAR(1, 0x650148, MATRIX3x3, gLightMatrix_650148, gLightMatrix_650148_tmp);

void CC Script_tbl_light_helper_param_c_40218B(__int16 x, __int16 y, __int16 z)
{
    gLightMatrix_650148.m[0][0] = 16 * x;
    gLightMatrix_650148.m[1][0] = 16 * y;
    gLightMatrix_650148.m[2][0] = 16 * z;
}
MGS_FUNC_IMPLEX(0x0040218B, Script_tbl_light_helper_param_c_40218B, SCRIPT_IMPL);

MGS_VAR(1, 0x6BEE70, WORD, light_r_word_6BEE70, 0);
MGS_VAR(1, 0x6BEE72, WORD, light_g_word_6BEE72, 0);
MGS_VAR(1, 0x6BEE74, WORD, light_b_word_6BEE74, 0);

MGS_VAR(1, 0x650168, DWORD, light_unknown_dword_650168, 0x3C808080);
MGS_VAR(1, 0x65016C, DWORD, light_unknown_dword_65016C, 0x3E808080);

void CC LightSettingsRelated_4020F5(int r, int g, int b)
{
    light_r_word_6BEE70 = static_cast<WORD>(r);
    light_g_word_6BEE72 = static_cast<WORD>(g);
    light_b_word_6BEE74 = static_cast<WORD>(b);
    gGte_background_colour_993E74.x = 16 * r;
    gGte_background_colour_993E74.y = 16 * g;
    gGte_background_colour_993E74.z = 16 * b;
    light_unknown_dword_650168 = 0x3CFFFFFF;
    light_unknown_dword_65016C = 0x3EFFFFFF;
}
MGS_FUNC_IMPLEX(0x004020F5, LightSettingsRelated_4020F5, SCRIPT_IMPL);

int CC Script_tbl_light_sub_451239(BYTE* /*pScript*/)
{
    SVECTOR vec = {};

    DWORD scriptParam = Script_ParamExists('d');
    if (scriptParam)
    {
        Script_Read3Words_409945((BYTE *)scriptParam, (WORD *)&vec.field_0_x);
        Script_light_vec_402144(vec.field_0_x, vec.field_2_y, vec.field_4_z);
    }
    scriptParam = Script_ParamExists('c');
    if (scriptParam)
    {
        Script_Read3Words_409945((BYTE *)scriptParam, (WORD *)&vec.field_0_x);
        Script_tbl_light_helper_param_c_40218B(vec.field_0_x, vec.field_2_y, vec.field_4_z);
    }
    scriptParam = Script_ParamExists('a');
    if (scriptParam)
    {
        Script_Read3Words_409945((BYTE *)scriptParam, (WORD *)&vec.field_0_x);
        LightSettingsRelated_4020F5(vec.field_0_x, vec.field_2_y, vec.field_4_z);
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x00451239, Script_tbl_light_sub_451239, SCRIPT_IMPL);

int CC Script_tbl_pad_452132(BYTE*)
{
    if (Script_ParamExists('m'))
    {
        g_lib_gv_stru_6BFEE0.gGv_dword_6C03A0_buttons_released = Script_get_int();
        game_state_dword_72279C.flags |= 0x8000000u;
    }
    if (Script_ParamExists('r'))
    {
        game_state_dword_72279C.flags |= 0x10000000u;
    }
    else if (Script_ParamExists('s'))
    {
        game_state_dword_72279C.flags &= 0xE7FFFF7F;
    }
    LibDG_Update2_helper_40A857();
    return 0;
}
MGS_FUNC_IMPLEX(0x00452132, Script_tbl_pad_452132, SCRIPT_IMPL);

MGS_FUNC_NOT_IMPL(0x00451688, int __cdecl(BYTE*), Script_tbl_ntrap_removeQ_451688);
MGS_FUNC_NOT_IMPL(0x00451673, int __cdecl(BYTE*), Script_tbl_hzd_related_sub_451673);
MGS_FUNC_NOT_IMPL(0x004512E5, int __cdecl(BYTE*), script_tbl_camera_sub_4512E5);
MGS_FUNC_NOT_IMPL(0x00451D5C, int __cdecl(BYTE*), Script_tbl_radio_sub_451D5C);
MGS_FUNC_NOT_IMPL(0x00451F22, int __cdecl(BYTE*), Script_tbl_str_status_sub_451F22);
MGS_FUNC_NOT_IMPL(0x00452064, int __cdecl(BYTE*), Script_tbl_demo_sub_452064);
MGS_FUNC_NOT_IMPL(0x00451778, int __cdecl(BYTE*), Script_tbl_ntrap_451778);
MGS_FUNC_NOT_IMPL(0x00451F89, int __cdecl(BYTE*), Script_tbl_varsave_sub_451F89);
MGS_FUNC_NOT_IMPL(0x00451FE3, int __cdecl(BYTE*), Script_tbl_system_sub_451FE3);
MGS_FUNC_NOT_IMPL(0x0045219B, int __cdecl(BYTE*), Script_tbl_sound_45219B);
MGS_FUNC_NOT_IMPL(0x004523C7, int __cdecl(BYTE*), Script_Tbl_rand_4523C7);
MGS_FUNC_NOT_IMPL(0x004523E7, int __cdecl(BYTE*), Script_tbl_sub_4523E7);
MGS_FUNC_NOT_IMPL(0x0045256B, int __cdecl(BYTE*), Script_tbl_print_sub_45256B);
MGS_FUNC_NOT_IMPL(0x004525D6, int __cdecl(BYTE*), Script_tbl_jimaku_4525D6);


static void TestCommonHashes()
{
    ASSERT_EQ(0x0d86, ResourceNameHash("if"));
    ASSERT_EQ(0x64C0, ResourceNameHash("eval"));
    ASSERT_EQ(0xCD3A, ResourceNameHash("return"));
    ASSERT_EQ(0x7636, ResourceNameHash("foreach"));
    ASSERT_EQ(0x22FF, ResourceNameHash("mesg"));
    ASSERT_EQ(0xD4CB, ResourceNameHash("trap"));
    ASSERT_EQ(0x9906, ResourceNameHash("chara"));
    ASSERT_EQ(0xC091, ResourceNameHash("map"));
    //ASSERT_EQ(0x7D50, ResourceNameHash("hzd"));
    ASSERT_EQ(0xEEE9, ResourceNameHash("camera"));
    ASSERT_EQ(0x306A, ResourceNameHash("light"));
    ASSERT_EQ(0xC8BB, ResourceNameHash("load"));
    ASSERT_EQ(0x9A1F, ResourceNameHash("start"));
    ASSERT_EQ(0x24E1, ResourceNameHash("radio"));
    ASSERT_EQ(0xE43C, ResourceNameHash("str_status"));
    ASSERT_EQ(0xA242, ResourceNameHash("demo"));
    ASSERT_EQ(0xDBAB, ResourceNameHash("ntrap"));
    ASSERT_EQ(0x430D, ResourceNameHash("delay"));
    ASSERT_EQ(0xCC85, ResourceNameHash("pad"));
    ASSERT_EQ(0x5C9E, ResourceNameHash("varsave"));
    ASSERT_EQ(0x4AD9, ResourceNameHash("system"));
    ASSERT_EQ(0x698D, ResourceNameHash("sound"));
    ASSERT_EQ(0x226D, ResourceNameHash("menu"));
    ASSERT_EQ(0x925E, ResourceNameHash("rand"));
    //ASSERT_EQ(0xE257, ResourceNameHash("stage"));
    ASSERT_EQ(0xB96E, ResourceNameHash("print"));
    ASSERT_EQ(0xEC9D, ResourceNameHash("jimaku"));
}

MGS_ARY(1, 0x66B000, proc_struct_sub, 24, script_funcs_tbl_66B000,
{
    { 0x22FF, 0x0, Script_tbl_mesg_sub_451A5E },
    { 0xD4CB, 0x0, Script_tbl_ntrap_removeQ_451688.Ptr() },
    { 0x9906, 0x0, Script_tbl_chara_451AC3 },
    { 0xC091, 0x0, Script_tbl_map_45151D },
    { 0x7D50, 0x0, Script_tbl_hzd_related_sub_451673.Ptr() },
    { 0xEEE9, 0x0, script_tbl_camera_sub_4512E5.Ptr() },
    { 0x306A, 0x0, Script_tbl_light_sub_451239 },
    { 0x9A1F, 0x0, Script_tbl_start_sub_451B0E },
    { 0xC8BB, 0x0, Script_tbl_load_451BBF },
    { 0x24E1, 0x0, Script_tbl_radio_sub_451D5C.Ptr() },
    { 0xE43C, 0x0, Script_tbl_str_status_sub_451F22.Ptr() },
    { 0xA242, 0x0, Script_tbl_demo_sub_452064.Ptr() },
    { 0xDBAB, 0x0, Script_tbl_ntrap_451778.Ptr() },
    { 0x430D, 0x0, Script_tbl_Delay_sub_4519C7 },
    { 0xCC85, 0x0, Script_tbl_pad_452132 },
    { 0x5C9E, 0x0, Script_tbl_varsave_sub_451F89.Ptr() },
    { 0x4AD9, 0x0, Script_tbl_system_sub_451FE3.Ptr() },
    { 0x698D, 0x0, Script_tbl_sound_45219B.Ptr() },
    { 0x226D, 0x0, Script_tbl_menu_sub_4521A7 },
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

//MGS_FUNC_NOT_IMPL(0x004090EA, int CC(BYTE*), Script_tbl_if_sub_4090EA);

int CC Script_tbl_if_sub_4090EA(BYTE* pScript)
{
    BYTE* pScriptCopy = nullptr;
    BYTE* execRet = nullptr;
    DWORD pScriptBytes;
    DWORD exec1 = 0;

    DWORD cmdRet = 0;
    pScriptCopy = pScript;

    pScriptCopy = Script_GCL_Execute(pScriptCopy, &cmdRet, &exec1);
    for(;;)
    {
        execRet = Script_GCL_Execute(pScriptCopy, &cmdRet, &pScriptBytes);
        if (exec1)
        {
            break;
        }

        if (!Script_GCL_Execute(execRet, &cmdRet, &exec1))
        {
            return 0;
        }

        const WORD cmdId = cmdRet >> 16;
        pScriptCopy = (BYTE *)exec1;
        if (cmdId != 'e') // else
        {
            if (cmdId != 'i') // elseif
            {
                return 0;
            }

            pScriptCopy = Script_GCL_Execute(pScriptCopy, &cmdRet, &exec1);
            continue;
        }
        exec1 = 1;
    }

    Script_Run((BYTE*)pScriptBytes, nullptr);

    return 0;
}
MGS_FUNC_IMPLEX(0x4090EA, Script_tbl_if_sub_4090EA, SCRIPT_IMPL);

MGS_FUNC_NOT_IMPL(0x00409178, int CC(BYTE*), Script_tbl_unknown_loop_sub_409178);
signed int script_tbl_nop_sub_4091F6(BYTE*)
{
    return 1;
}
//MSG_FUNC_IMPL(0x004091F6, script_tbl_nop_sub_4091F6); // Can't hook because func is too small


MGS_ARY(1, 0x6506E0, proc_struct_sub, 4, gEarlyScriptBinds_Tbl_6506E0,
{
    { 0x0d86, 0x0, Script_tbl_if_sub_4090EA },
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

MGS_PTR(1, 0x006BFC60, GCL_ProcInfo*, gProcInfos, nullptr);
MGS_PTR(1, 0x006BFC64, BYTE*, dword_6BFC64, nullptr);

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
    gProcInfos = reinterpret_cast<GCL_ProcInfo*>(pScript + 4);
    
    // Swap bytes in the table of script functions so they are the correct ordering
    dword_6BFC64 = Script_InitProcTables_sub_409C87(pScript + 4);

    // Get pointer to main script function start
    gScriptMainProc_dword_6BFC68 = (pScript + 4 + offset + 4);

    DWORD mainScriptLen = ToDWORD(gScriptMainProc_dword_6BFC68-4);
    BYTE* pEndOfScriptData = gScriptMainProc_dword_6BFC68 + mainScriptLen + 4;

    sub_45A6F6(2, pEndOfScriptData);

    return 0;
}
MGS_FUNC_IMPLEX(0x00409C19, Script_Init_sub_409C19, SCRIPT_IMPL);

signed int CC GV_gcx_file_handler_4090CF(void* pScript, TFileNameHash fileNameHashed)
{
    if (fileNameHashed == gScriptFileNameHashedToLoad_6BFBB4)
    {
        Script_Init_sub_409C19(reinterpret_cast<BYTE*>(pScript));
    }
    return 1;
}
MGS_FUNC_IMPLEX(0x004090CF, GV_gcx_file_handler_4090CF, SCRIPT_IMPL);

void CC ScriptEngineInit_4090A7()
{
    Script_sub_409A3A(); // Stack setup?
    SaveDataStructuresRelated_4093ED(); // Script vars memory?
    Script_sub_4091FA();
    LibGV_Set_FileExtHandler_40A68D('g', GV_gcx_file_handler_4090CF); // .gcx
    Script_Set_MainOrDemo_40908E(0);
}
MGS_FUNC_IMPLEX(0x004090A7, ScriptEngineInit_4090A7, SCRIPT_IMPL); // TODO

BYTE* CC Script_FindProc(WORD procId)
{
    for (GCL_ProcInfo* pProcInfo = gProcInfos; pProcInfo->mId; ++pProcInfo)
    {
        if (pProcInfo->mId == procId)
        {
            return dword_6BFC64 + pProcInfo->mOffset;
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

void CC Script_RunMainProc_409D49()
{
    static GCL_Proc_Arguments sMainProcArgs_6BFC70 = {};
    if (gScriptMainProc_dword_6BFC68)
    {
        if (*gScriptMainProc_dword_6BFC68 != 0x40)
        {
            printf("NOT SCRIPT DATA !!\n");
        }
        Script_Run(gScriptMainProc_dword_6BFC68 + 3, &sMainProcArgs_6BFC70);
    }
}
MGS_FUNC_IMPLEX(0x00409D49, Script_RunMainProc_409D49, SCRIPT_IMPL);

void CC Script_Push(BYTE *arg)
{
    (*gScriptStackPos_dword_6BFC3C) = arg;
    ++gScriptStackPos_dword_6BFC3C;
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
    LOG_INFO("Exec: " << cmdId);
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
            Script_Unknown6_409D77(pScript + 2, &dwRet /* &pScriptBytes*/);
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
    Test_Script_VarRead_4094DC();

    TestCommonHashes();
}
