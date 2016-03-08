#include "stdafx.h"
#include "MgsFunction.hpp"
#include "logger.hpp"

// Strangely.. never called!
int __cdecl Script_Operator_Evaluate(int operation, int v1, int v2)
{
    abort();

    int result = 0;

    /*
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
        break;
    case 19:
        result = v1 && v2;
        break;
    default:
        result = 0;
        break;
    }
    */
    return result;
}
MSG_FUNC_IMPL(0x00409E7C, Script_Operator_Evaluate);
