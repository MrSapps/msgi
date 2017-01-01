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

void ScriptCpp_ForceLink()
{

}

/*
int __cdecl Script_AnimQ(int a1, BYTE* pScript)
{
    int char_num = MAKEWORD(pScript[0], pScript[1]); // TODO: Check ordering
    int code_num = MAKEWORD(pScript[2], pScript[3]); // TODO: Check ordering

    int v4 = MAKEWORD(pScript[4], pScript[5]); // TODO: Check ordering


    signed int v6 = 1;

    if (v4 > 256)
    {
        v6 = 0;
        v4 -= 256;
    }

    const unsigned int idx = 20 * (char_num == 8650);
    word_995460[idx / 2] = 1;
    dword_995470[idx / 4] = a1; // off 16
    dword_995464[idx / 4] = char_num; // off 4
    dword_995468[idx / 4] = code_num; // off 8
    dword_99546C[idx / 4] = v4; // off 12
    word_995462[idx / 2] = v6; // off 2
    
    if (v6)
    {
        // Seems to get stuck here as whatever other thread is supposed to set the event never does..
        j_Task_WaitForSignalQ();
    }

    return printf("ANIME CHARA %d CODE %d\n", char_num, code_num);
}
MSG_FUNC_IMPL(0x0045CEC2, Script_AnimQ);
*/

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
