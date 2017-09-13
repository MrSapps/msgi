#pragma once

#include "MgsFunction.hpp"

void LibGVCpp_ForceLink();

using GV_FnPtr = int(__cdecl*)(void*, int);


void LibGv_Init_sub_40A4F6();
void CC LibGV_Set_FileExtHandler_40A68D(char id, GV_FnPtr fn);
void CC LibGv_ClearFunctionPointers_40A69D();
void CC LibGv_Set_Load_HZM_CallBack_40B725();
__int64 CC TimeGetElapsed_4455A0();
int CC LibGV_LoadFile_40A77F(void* fileData, signed int fileNameHash, int allocType);
