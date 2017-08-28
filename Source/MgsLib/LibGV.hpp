#pragma once

#include "MgsFunction.hpp"

void LibGVCppForceLink();

using GV_FnPtr = int(__cdecl*)(DWORD*);


void LibGv_Init_sub_40A4F6();
void CC LibGV_SetFnPtr_sub_40A68D(char id, GV_FnPtr fn);
void CC LibGv_ClearFunctionPointers_40A69D();
