#include "stdafx.h"

#include <gmock/gmock.h>
#include "System.hpp"

/*
void CC System_DeInit_Systems_0_to_2_sub_40AC52();
void CC System_Debug_sub_40ADEC(int index);
void CC System_Debug_sub_40AEC0(int idx);
LibGV_MemoryAllocation* CC System_sub_40B05B(system_struct* pSystem, LibGV_MemoryAllocation* pAlloc);
LibGV_MemoryAllocation* CC System_FindMatchingFreeAllocation_40B024(system_struct *pSystem, unsigned int requestedSize);
*/

TEST(System, System_init_40AC6C)
{
    BYTE heap50kb[1024 * 50] = {};
    system_struct* pSystem = System_init_40AC6C(0, 0, heap50kb, sizeof(heap50kb));
    ASSERT_EQ(0, pSystem->mFlags);
    ASSERT_EQ(1, pSystem->mUnitsCount);
    ASSERT_EQ(heap50kb, pSystem->mStartAddr);
    ASSERT_EQ(heap50kb + 1024 * 50, pSystem->mEndAddr);

    pSystem->mAllocs[0].mAllocType = LibGV_MemoryAllocation::eFree;
    pSystem->mAllocs[0].mAllocType = 0;

    pSystem->mAllocs[1].mAllocType = LibGV_MemoryAllocation::eUsed;
    pSystem->mAllocs[1].mAllocType = 0;



    System_Debug_sub_40ADEC(0);
    System_Debug_sub_40AEC0(0);
}
