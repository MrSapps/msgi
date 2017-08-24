#include "stdafx.h"
#include "System.hpp"

struct LibGV_MemoryAllocation
{
    BYTE* mPDataStart;
    DWORD mAllocType;
};
static_assert(sizeof(LibGV_MemoryAllocation) == 8, "size must be 8");

struct system_struct
{
    DWORD mFlags;
    BYTE* mStartAddr;
    BYTE* mEndAddr;
    DWORD mUnitsCount;
    BYTE* mMemoryPool;
    DWORD field_14;
    LibGV_MemoryAllocation mAllocs[511];

    //DWORD pUnknownStruct2; // This and below is the allocs memory data?
    //DWORD mArray[1021]; 
};
static_assert(sizeof(system_struct) == 0x1010, "size must be 0x1010");

MSG_FUNC_NOT_IMPL(0x40ACB2, int __cdecl(int idx), System_sub_40ACB2);

MSG_FUNC_NOT_IMPL(0x40B024, LibGV_MemoryAllocation *__cdecl(system_struct *pSystem, unsigned int alignedSize), System_FindMatchingAllocation_40B024);
MSG_FUNC_NOT_IMPL(0x40B05B, LibGV_MemoryAllocation *__cdecl(system_struct *pSystem, LibGV_MemoryAllocation *pAlloc), System_sub_40B05B);

MGS_ARY(1, 0x78E980, system_struct, 3, gSystems_dword_78E980, {});

void SystemCpp_ForceLink() { }

system_struct *__cdecl System_init_40AC6C(int index, int bIsDynamic, void* pMemory, int size)
{
    gSystems_dword_78E980[index].mFlags = bIsDynamic != 0;
    gSystems_dword_78E980[index].mStartAddr = (BYTE*)pMemory;
    BYTE* endPtr = ((BYTE*)pMemory + (size & 0xFFFFFFF0));
    gSystems_dword_78E980[index].mUnitsCount = 1;
    gSystems_dword_78E980[index].mEndAddr = endPtr;
    gSystems_dword_78E980[index].field_14 = 0;
    gSystems_dword_78E980[index].mMemoryPool = (BYTE*)pMemory;
    gSystems_dword_78E980[index].mAllocs[0].mPDataStart = endPtr;
    gSystems_dword_78E980[index].mAllocs[0].mAllocType = 2;
    return &gSystems_dword_78E980[index];
}
MSG_FUNC_IMPL(0x40AC6C, System_init_40AC6C);

/*
void* __cdecl System_mem_zerod_alloc_40AFA4(int idx, int size, void** alloc_type_or_ptr)
{
    system_struct *pSystem;
    unsigned int alignedSize;
    LibGV_MemoryAllocation *pAlloc;
    LibGV_MemoryAllocation *ptr;

    pSystem = &gSystems_dword_78E980[idx];
    if (pSystem->mUnitsCount >= 511)
    {
        return 0;
    }

    alignedSize = (size + 15) & 0xFFFFFFF0;
    
    pAlloc = System_FindMatchingAllocation_40B024(pSystem, alignedSize);
    if (!pAlloc)
    {
        pSystem->mFlags |= 4u; // mark as failed
        return 0;
    }

    ptr = reinterpret_cast<LibGV_MemoryAllocation*>(pAlloc->mPDataStart);

    const unsigned int allocSize = pAlloc[1].mPDataStart - pAlloc->mPDataStart;
    if (allocSize > alignedSize)
    {
        System_sub_40B05B(pSystem, pAlloc);
        pAlloc[1].mAllocType = 0;
        pAlloc[1].mPDataStart = reinterpret_cast<BYTE*>(ptr + alignedSize);
    }
    pAlloc->mAllocType = (DWORD)alloc_type_or_ptr;

    if (alloc_type_or_ptr != (void**)2)
    {
        *alloc_type_or_ptr = ptr;
    }

    memset(ptr, 0, alignedSize);
    return ptr;
}
MSG_FUNC_IMPL(0x40AFA4, System_mem_zerod_alloc_40AFA4);

void *__cdecl System_2_zerod_allocate_memory_40B296(int size)
{
    return System_mem_zerod_alloc_40AFA4(2, size, (void**)2);
}
MSG_FUNC_IMPL(0x40B296, System_2_zerod_allocate_memory_40B296);
*/
