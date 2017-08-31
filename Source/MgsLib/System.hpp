#pragma once

#include "MgsFunction.hpp"

struct LibGV_MemoryAllocation
{
    BYTE* mPDataStart;
    enum eTypes
    {
        eFree = 0,
        eVoid = 1,
        eUsed = 2,
    };
    DWORD mAllocType;
};
MSG_ASSERT_SIZEOF(LibGV_MemoryAllocation, 8);

struct system_struct
{
    enum eFlags
    {
        eDynamic = 1,
        eVoided = 2,
        eFailed = 4,
    };
    DWORD mFlags;
    BYTE* mStartAddr;
    BYTE* mEndAddr;
    DWORD mUnitsCount;
    LibGV_MemoryAllocation mAllocs[512];
};
MSG_ASSERT_SIZEOF(system_struct, 0x1010);

system_struct* CC System_init_40AC6C(int index, int bIsDynamic, void* pMemory, int size);
void CC System_DeInit_Systems_0_to_2_sub_40AC52();
void CC System_Debug_sub_40ADEC(int index);
void CC System_Debug_sub_40AEC0(int idx);
LibGV_MemoryAllocation* CC System_sub_40B05B(system_struct* pSystem, LibGV_MemoryAllocation* pAlloc);
LibGV_MemoryAllocation* CC System_FindMatchingFreeAllocation_40B024(system_struct *pSystem, unsigned int requestedSize);
void* CC System_mem_zerod_alloc_40AFA4(int idx, int size, void** alloc_type_or_ptr);

void SystemCpp_ForceLink();
EXTERN_MSG_FUNC_NOT_IMPL(0x40ACB2, int __cdecl(int idx), System_sub_40ACB2);

void DoTestSystem();
