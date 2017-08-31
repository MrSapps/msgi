#include "stdafx.h"
#include "System.hpp"
#include <assert.h>
#include <gmock/gmock.h>

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

// Compacts free blocks
int CC System_sub_40B147(system_struct* pSystem, LibGV_MemoryAllocation* alloc, int numUnitsToMerge)
{
    LibGV_MemoryAllocation* pStartAlloc = alloc;
    LibGV_MemoryAllocation* pEndAlloc = alloc + numUnitsToMerge;

    const int allocIndex = alloc - pSystem->mAllocs;
    int numUnitsBeforeStartAlloc = pSystem->mUnitsCount - allocIndex;
    if (numUnitsBeforeStartAlloc >= 0)
    {
        ++numUnitsBeforeStartAlloc;
        do
        {
            pStartAlloc->mPDataStart = pEndAlloc->mPDataStart;
            pStartAlloc->mAllocType = pEndAlloc->mAllocType;
            ++pEndAlloc;
            ++pStartAlloc;
            --numUnitsBeforeStartAlloc;
        } while (numUnitsBeforeStartAlloc);
    }
    pSystem->mUnitsCount -= numUnitsToMerge;
    return numUnitsBeforeStartAlloc;
}
MSG_FUNC_IMPL(0x40B147, System_sub_40B147);


MSG_FUNC_NOT_IMPL(0x40B0F7, LibGV_MemoryAllocation *__cdecl (system_struct *, void *), System_FindAlloc_40B0F7);

// Frees a block
void CC System_Free_40B099(int idx, void *ptr)
{
    LibGV_MemoryAllocation* pAlloc = System_FindAlloc_40B0F7(&gSystems_dword_78E980[idx], ptr);
    if (pAlloc && pAlloc->mAllocType)
    {
        int numAllocsToMerge = 0;
        
        // Free the unit
        pAlloc->mAllocType = LibGV_MemoryAllocation::eFree;

        LibGV_MemoryAllocation* allocPtr = pAlloc;

        // If the unit we are freeing is the first unit or the unit before the one 
        // we are freeing is not free
        if (pAlloc == &gSystems_dword_78E980[idx].mAllocs[0] || pAlloc[-1].mAllocType)
        {
            // Compact from the next alloc
            allocPtr = pAlloc + 1;
        }
        else
        {
            // The block before is us free, so include it for merging
            numAllocsToMerge = 1;
        }

        if (pAlloc[1].mAllocType == LibGV_MemoryAllocation::eFree)
        {
            // The block after us is free so include it for merging
            ++numAllocsToMerge;
        }

        if (numAllocsToMerge)
        {
            System_sub_40B147(&gSystems_dword_78E980[idx], allocPtr, numAllocsToMerge);
        }
    }
}
MSG_FUNC_IMPL(0x40B099, System_Free_40B099);


//MSG_FUNC_NOT_IMPL(0x40B147, int __cdecl(system_struct*, LibGV_MemoryAllocation*, int), System_sub_40B147);
//MSG_FUNC_NOT_IMPL(0x40B099, void __cdecl(int, void*), System_Free_40B099);

void DoTestSystem()
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

    void* ptrs[8] = {};
    ptrs[0] = System_mem_zerod_alloc_40AFA4(0, 123, (void**)LibGV_MemoryAllocation::eUsed);
    ptrs[1] = System_mem_zerod_alloc_40AFA4(0, 55, (void**)LibGV_MemoryAllocation::eUsed);
    ptrs[2] = System_mem_zerod_alloc_40AFA4(0, 7, (void**)LibGV_MemoryAllocation::eUsed);
    ptrs[3] = System_mem_zerod_alloc_40AFA4(0, 700, (void**)LibGV_MemoryAllocation::eUsed);
    ptrs[4] = System_mem_zerod_alloc_40AFA4(0, 4477, (void**)LibGV_MemoryAllocation::eUsed);
    ptrs[5] = System_mem_zerod_alloc_40AFA4(0, 4477, (void**)LibGV_MemoryAllocation::eUsed);
    ptrs[6] = System_mem_zerod_alloc_40AFA4(0, 4477, (void**)LibGV_MemoryAllocation::eUsed);
    ptrs[7] = System_mem_zerod_alloc_40AFA4(0, 4477, (void**)LibGV_MemoryAllocation::eUsed);

    for (int i = 0; i < 8; i++)
    {
        ASSERT_NE(nullptr, ptrs[i]);
    }

    // Free the allocs so that we have [used][free][used][free] pattern
    System_Debug_sub_40ADEC(0);
    System_Debug_sub_40AEC0(0);

    System_Free_40B099(0, ptrs[1]);

    System_Debug_sub_40ADEC(0);
    System_Debug_sub_40AEC0(0);

    System_Free_40B099(0, ptrs[3]);

    System_Debug_sub_40ADEC(0);
    System_Debug_sub_40AEC0(0);

    System_Free_40B099(0, ptrs[5]);

    System_Debug_sub_40ADEC(0);
    System_Debug_sub_40AEC0(0);

    System_Free_40B099(0, ptrs[7]);

    System_Debug_sub_40ADEC(0);
    System_Debug_sub_40AEC0(0);

    // Free an alloc and check it gets merged
    System_Free_40B099(0, ptrs[0]);

    System_Debug_sub_40ADEC(0);
    System_Debug_sub_40AEC0(0);
}
