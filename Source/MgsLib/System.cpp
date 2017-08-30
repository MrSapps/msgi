#include "stdafx.h"
#include "System.hpp"
#include <assert.h>


MSG_FUNC_NOT_IMPL(0x40ACB2, int __cdecl(int idx), System_sub_40ACB2);


//MSG_FUNC_NOT_IMPL(0x40B024, LibGV_MemoryAllocation *__cdecl(system_struct *pSystem, unsigned int alignedSize), System_FindMatchingAllocation_40B024);
//MSG_FUNC_NOT_IMPL(0x40B05B, LibGV_MemoryAllocation *__cdecl(system_struct *pSystem, LibGV_MemoryAllocation *pAlloc), System_sub_40B05B);

MGS_ARY(1, 0x78E980, system_struct, 3, gSystems_dword_78E980, {});

void SystemCpp_ForceLink() { }

static bool IsPowerOf2(int i)
{
    return !(i & (i - 1));
}

static DWORD RoundUpPowerOf2(DWORD numToRound, int multiple)
{
    assert(multiple && IsPowerOf2(multiple));
    return (numToRound + multiple - 1) & -multiple;
}

static DWORD RoundDownPowerOf2(DWORD numToRound, int multiple)
{
    assert(multiple && IsPowerOf2(multiple));
    return numToRound & -multiple;
}

system_struct* CC System_init_40AC6C(int index, int bIsDynamic, void* pMemory, int size)
{
    gSystems_dword_78E980[index].mFlags = bIsDynamic != 0;
    gSystems_dword_78E980[index].mStartAddr = (BYTE *)pMemory;
    
    BYTE* alignedEndPtr = (BYTE *)(pMemory) + RoundDownPowerOf2(size, 16);
    
    gSystems_dword_78E980[index].mUnitsCount = 1;
    gSystems_dword_78E980[index].mEndAddr = alignedEndPtr;
    
    gSystems_dword_78E980[index].mAllocs[0].mAllocType = LibGV_MemoryAllocation::eFree;
    gSystems_dword_78E980[index].mAllocs[0].mPDataStart = (BYTE *)pMemory;

    gSystems_dword_78E980[index].mAllocs[1].mPDataStart = alignedEndPtr;
    gSystems_dword_78E980[index].mAllocs[1].mAllocType = LibGV_MemoryAllocation::eUsed;
    return &gSystems_dword_78E980[index];
}
MSG_FUNC_IMPL(0x40AC6C, System_init_40AC6C);

void CC System_DeInit_Systems_0_to_2_sub_40AC52()
{
    for (int i = 0; i < 3; i++)
    {
        System_init_40AC6C(i, 0, nullptr, 0);
    }
}
MSG_FUNC_IMPL(0x40AC52, System_DeInit_Systems_0_to_2_sub_40AC52);

void CC System_Debug_sub_40ADEC(int index)
{
    system_struct* pSystem = &gSystems_dword_78E980[index];

    printf("system %d ( ", index);
    if (pSystem->mFlags & system_struct::eDynamic)
    {
        printf("dynamic ");
    }
    else
    {
        printf("static ");
    }

    if (pSystem->mFlags & system_struct::eVoided)
    {
        printf("voided ");
    }
    if (pSystem->mFlags & system_struct::eFailed)
    {
        printf("failed ");
    }
    printf(")\n");
    printf("  addr = %08x - %08x, units = %d\n", pSystem->mStartAddr, pSystem->mEndAddr, pSystem->mUnitsCount);

    int biggestFreeBlockSizeBytes = 0;
    int numFreeBytes = 0;
    int numVoidedBytes = 0;

    if (pSystem->mUnitsCount > 0)
    {
        LibGV_MemoryAllocation* ptr = pSystem->mAllocs;
        bool bEnd = false;
        int counter = pSystem->mUnitsCount;
        do
        {
            const int allocType = ptr->mAllocType;
            const int sizeBytes = ptr[1].mPDataStart - ptr->mPDataStart;
            if (allocType)
            {
                if (allocType == LibGV_MemoryAllocation::eVoid)
                {
                    numVoidedBytes += sizeBytes;
                }
            }
            else
            {
                numFreeBytes += sizeBytes;
                if (sizeBytes > biggestFreeBlockSizeBytes)
                {
                    biggestFreeBlockSizeBytes = sizeBytes;
                }
            }
            bEnd = counter-- == 1;
            ++ptr;
        } while (!bEnd);
    }

    const int numTotalBytes = pSystem->mEndAddr - pSystem->mStartAddr;
    printf("  free = %d / %d, voided = %d, max_free = %d\n",
        numFreeBytes,
        numTotalBytes,
        numVoidedBytes,
        biggestFreeBlockSizeBytes);
}
MSG_FUNC_IMPL(0x40ADEC, System_Debug_sub_40ADEC);

void CC System_Debug_sub_40AEC0(int idx)
{
    system_struct* pSystem = &gSystems_dword_78E980[idx];
    printf("system %d ( ", idx);
    if (pSystem->mFlags & system_struct::eDynamic)
    {
        printf("dynamic ");
    }
    else
    {
        printf("static ");
    }

    if (pSystem->mFlags & system_struct::eVoided)
    {
        printf("voided ");
    }

    if (pSystem->mFlags & system_struct::eFailed)
    {
        printf("failed ");
    }
    printf(")\n");

    if (pSystem->mUnitsCount > 0)
    {
        LibGV_MemoryAllocation* pAlloc = pSystem->mAllocs;
        LibGV_MemoryAllocation* pNextAlloc = nullptr;
        int unitCounter = pSystem->mUnitsCount;
        do
        {
            BYTE* ptr = pAlloc->mPDataStart;
            const int alloc_type = pAlloc->mAllocType;
            pNextAlloc = pAlloc + 1;
            const int allocSizeBytes = pAlloc[1].mPDataStart - pAlloc->mPDataStart;
            switch (alloc_type)
            {
            case LibGV_MemoryAllocation::eFree:
                printf("---- %8d bytes ( from %08x free )\n", allocSizeBytes, ptr);
                break;

            case LibGV_MemoryAllocation::eVoid:
                printf("==== %8d bytes ( from %08x void )\n", allocSizeBytes, ptr);
                break;

            case LibGV_MemoryAllocation::eUsed:
                printf("++++ %8d bytes ( from %08x used )\n", allocSizeBytes, ptr);
                break;

            default:
                printf("**** %8d bytes ( from %08x user %08x )\n", allocSizeBytes, ptr, alloc_type);
            }

            --unitCounter;
            pAlloc = pNextAlloc;
        } while (unitCounter);
    }
    printf("\n");
}
MSG_FUNC_IMPL(0x40AEC0, System_Debug_sub_40AEC0);

LibGV_MemoryAllocation* CC System_sub_40B05B(system_struct* pSystem, LibGV_MemoryAllocation* pAlloc)
{
    // pAlloc is an alloc that matches a size, its not yet marked as allocated
    // and the remaining total data is still bigger than size

    int idx = pSystem->mUnitsCount;
    const int allocIndex = pAlloc - pSystem->mAllocs;
    const int numAllocsBeforeThisOne = pSystem->mUnitsCount - allocIndex;

    if (numAllocsBeforeThisOne >= 0)
    {
        int counter = numAllocsBeforeThisOne + 1;
        do
        {
            pSystem->mAllocs[idx + 1].mPDataStart = pSystem->mAllocs[idx].mPDataStart;
            pSystem->mAllocs[idx + 1].mAllocType = pSystem->mAllocs[idx].mAllocType;
            --idx;
            --counter;
        } while (counter);
    }
    ++pSystem->mUnitsCount;
    return &pSystem->mAllocs[idx];
}
MSG_FUNC_IMPL(0x40B05B, System_sub_40B05B);

LibGV_MemoryAllocation* CC System_FindMatchingFreeAllocation_40B024(system_struct* pSystem, unsigned int requestedSize)
{
    BYTE* pStart = pSystem->mAllocs[0].mPDataStart;
    int unitCounter = pSystem->mUnitsCount;
    LibGV_MemoryAllocation* pAlloc = pSystem->mAllocs;
    while (unitCounter > 0)
    {
        const DWORD allocSize = pAlloc[1].mPDataStart - pStart; // Next alloc - this alloc
        if (allocSize >= requestedSize && pAlloc->mAllocType == LibGV_MemoryAllocation::eFree)
        {
            return pAlloc;
        }
       
        ++pAlloc;
        pStart = pAlloc->mPDataStart;

        --unitCounter;
    }
    return 0;
}
MSG_FUNC_IMPL(0x40B024, System_FindMatchingFreeAllocation_40B024);

void* CC System_mem_zerod_alloc_40AFA4(int idx, int size, void** alloc_type_or_ptr)
{
    system_struct* pSystem = &gSystems_dword_78E980[idx];
    if (pSystem->mUnitsCount >= 511)
    {
        return 0;
    }

    const unsigned int alignedSize = RoundUpPowerOf2(size, 16);
    LibGV_MemoryAllocation* pAlloc = System_FindMatchingFreeAllocation_40B024(pSystem, alignedSize);
    if (!pAlloc)
    {
        pSystem->mFlags |= system_struct::eFailed;
        return nullptr;
    }

    const unsigned int allocSize = pAlloc[1].mPDataStart - pAlloc->mPDataStart;
    if (allocSize > alignedSize)
    {
        System_sub_40B05B(pSystem, pAlloc); // TODO: Figure out this part
        pAlloc[1].mAllocType = LibGV_MemoryAllocation::eFree;
        pAlloc[1].mPDataStart = pAlloc->mPDataStart + alignedSize;
    }

    // Set the alloc type
    pAlloc->mAllocType = (DWORD)alloc_type_or_ptr;

    if (alloc_type_or_ptr != (void**)LibGV_MemoryAllocation::eUsed)
    {
        *alloc_type_or_ptr = pAlloc->mPDataStart;
    }

    memset(pAlloc->mPDataStart, 0, alignedSize);
    return pAlloc->mPDataStart;
}
MSG_FUNC_IMPL(0x40AFA4, System_mem_zerod_alloc_40AFA4);

void* CC System_2_zerod_allocate_memory_40B296(int size)
{
    return System_mem_zerod_alloc_40AFA4(2, size, (void**)LibGV_MemoryAllocation::eUsed);
}
MSG_FUNC_IMPL(0x40B296, System_2_zerod_allocate_memory_40B296);

