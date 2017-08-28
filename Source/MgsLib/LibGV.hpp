#pragma once

#include "MgsFunction.hpp"

void LibGVCppForceLink();

using GV_FnPtr = int(__cdecl*)(DWORD*);

void __cdecl LibGV_SetFnPtr_sub_40A68D(char id, GV_FnPtr fn);
