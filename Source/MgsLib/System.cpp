#include "stdafx.h"
#include "System.hpp"
#include <assert.h>
#include <gmock/gmock.h>

#define SYSTEM_IMPL true

MGS_ARY(1, 0x78E980, system_struct, 3, gSystems_dword_78E980, {});

MGS_FUNC_NOT_IMPL(0x40AD58, LibGV_MemoryAllocation* CC(system_struct* pSystem), System_sub_40AD58);
MGS_FUNC_NOT_IMPL(0x40ACF4, int CC (system_struct* pSystem), System_sub_40ACF4);

void SystemCpp_ForceLink() { }

static bool IsPowerOf2(int i)
{
    return !(i & (i - 1));
}

DWORD RoundUpPowerOf2(DWORD numToRound, int multiple)
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
MGS_FUNC_IMPLEX(0x40AC6C, System_init_40AC6C, SYSTEM_IMPL);

system_struct* CC System_Get_Max_Allocatable_Element_Count_4036A7(int index, DWORD* maxAllocatableElements, unsigned int elementSizeInBytes)
{
    int totalElementCount = 0;  
    LibGV_MemoryAllocation* pAllocIter = gSystems_dword_78E980[index].mAllocs;
    for (DWORD i = 0; i < gSystems_dword_78E980[index].mUnitsCount; i++)
    {
        if (!pAllocIter->mAllocType)
        {
            totalElementCount += (pAllocIter[1].mPDataStart - pAllocIter->mPDataStart) / elementSizeInBytes;
        }
        ++pAllocIter;
    }

    *maxAllocatableElements = totalElementCount;
    return &gSystems_dword_78E980[index];
}
MGS_FUNC_IMPLEX(0x4036A7, System_Get_Max_Allocatable_Element_Count_4036A7, SYSTEM_IMPL);

BYTE* CC System_allocate_with_hint_4036ED(system_struct* pSystem, LibGV_MemoryAllocation** pAllocated, DWORD* pAllocatedSizeBytes)
{
    LibGV_MemoryAllocation* pAlloc = nullptr;

    if (*pAllocated)
    {
        // Use after the passed in alloc
        pAlloc = (*pAllocated) + 1;
    }
    else
    {
        // Start from the first alloc
        pAlloc = pSystem->mAllocs;
    }

    char* startPtr = reinterpret_cast<char*>(pAlloc);
    char* endPtr = reinterpret_cast<char*>(pSystem);

    const int totalUnitsCount = pSystem->mUnitsCount - ((startPtr - endPtr - offsetof(system_struct, mUnitsCount)) / sizeof(LibGV_MemoryAllocation));
    for (int i=0; i<totalUnitsCount; i++)
    {
        if (!pAlloc[i].mAllocType)
        {
            // Update the allocation pointer, the size and return the raw allocation pointer
            *pAllocated = &pAlloc[i];
            *pAllocatedSizeBytes = pAlloc[i+1].mPDataStart - pAlloc[i].mPDataStart;
            return pAlloc[i].mPDataStart;
        }
    }

    // Alloc failure
    *pAllocatedSizeBytes = 0;
    return nullptr;
}
MGS_FUNC_IMPLEX(0x4036ED, System_allocate_with_hint_4036ED, SYSTEM_IMPL);

void CC System_DeInit_Systems_0_to_2_sub_40AC52()
{
    for (int i = 0; i < 3; i++)
    {
        System_init_40AC6C(i, 0, nullptr, 0);
    }
}
MGS_FUNC_IMPLEX(0x40AC52, System_DeInit_Systems_0_to_2_sub_40AC52, SYSTEM_IMPL);

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
MGS_FUNC_IMPLEX(0x40ADEC, System_Debug_sub_40ADEC, SYSTEM_IMPL);

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
MGS_FUNC_IMPLEX(0x40AEC0, System_Debug_sub_40AEC0, SYSTEM_IMPL);

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
MGS_FUNC_IMPLEX(0x40B05B, System_sub_40B05B, SYSTEM_IMPL);

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
MGS_FUNC_IMPLEX(0x40B024, System_FindMatchingFreeAllocation_40B024, SYSTEM_IMPL);


void CC System_VoidAllocation_40B187(int idx, void** pMem)
{
    LibGV_MemoryAllocation* pFound = System_FindAlloc_40B0F7(&gSystems_dword_78E980[idx], *pMem);
    if (pFound)
    {
        pFound->mAllocType = LibGV_MemoryAllocation::eVoid;
        gSystems_dword_78E980[idx].mFlags |= system_struct::eVoided;
    }
}
MGS_FUNC_IMPLEX(0x40B187, System_VoidAllocation_40B187, SYSTEM_IMPL);

void CC System_2_VoidAllocation_40B2B5(void *ptr)
{
    System_VoidAllocation_40B187(2, &ptr);
}
MGS_FUNC_IMPLEX(0x40B2B5, System_2_VoidAllocation_40B2B5, SYSTEM_IMPL);

void CC Safe_System_2_VoidAllocation_40513B(void *ptr)
{
    if (ptr)
    {
        System_2_VoidAllocation_40B2B5(ptr);
    }
}
MGS_FUNC_IMPLEX(0x40513B, Safe_System_2_VoidAllocation_40513B, SYSTEM_IMPL);

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
MGS_FUNC_IMPLEX(0x40AFA4, System_mem_zerod_alloc_40AFA4, SYSTEM_IMPL);

void* CC System_2_zerod_allocate_memory_40B296(int size)
{
    void* ptr = System_mem_zerod_alloc_40AFA4(2, size, (void**)LibGV_MemoryAllocation::eUsed);;
    assert(ptr != nullptr);
    return ptr;
}
MGS_FUNC_IMPLEX(0x40B296, System_2_zerod_allocate_memory_40B296, SYSTEM_IMPL);

void* CC System_mem_alloc_40AF91(int idx, int memSize)
{
    return System_mem_zerod_alloc_40AFA4(idx, memSize, (void**)LibGV_MemoryAllocation::eUsed);
}
MGS_FUNC_IMPLEX(0x40AF91, System_mem_alloc_40AF91, SYSTEM_IMPL);

void CC System_2_free_40B2A7(void* pAlloc)
{
    System_Free_40B099(2, pAlloc);
}
MGS_FUNC_IMPLEX(0x40B2A7, System_2_free_40B2A7, SYSTEM_IMPL);

// Compacts free blocks
int CC System_EraseContiguousBlocks_40B147(system_struct* pSystem, LibGV_MemoryAllocation* pEraseFrom, int numBlocksToErase)
{
    const int startAllocIndex = pEraseFrom - pSystem->mAllocs;
    int numMovesCount = pSystem->mUnitsCount - startAllocIndex - numBlocksToErase;
    if (numMovesCount >= 0)
    {
        ++numMovesCount;
        for (int i = 0; i < numMovesCount; i++)
        {
            pSystem->mAllocs[startAllocIndex + i].mPDataStart = pSystem->mAllocs[startAllocIndex + numBlocksToErase + i].mPDataStart;
            pSystem->mAllocs[startAllocIndex + i].mAllocType = pSystem->mAllocs[startAllocIndex + numBlocksToErase + i].mAllocType;
        }
    }
    pSystem->mUnitsCount -= numBlocksToErase;
    return numMovesCount;
}
MGS_FUNC_IMPLEX(0x40B147, System_EraseContiguousBlocks_40B147, SYSTEM_IMPL);

void CC System_HouseKeeping_40ACB2(int idx)
{
    system_struct* pSystem = &gSystems_dword_78E980[idx];
    const int flags = pSystem->mFlags;
    if (pSystem->mFlags & (system_struct::eVoided | system_struct::eFailed))
    {
        if (flags & system_struct::eFailed)
        {
            if (flags & system_struct::eDynamic)
            {
                System_sub_40AD58(&gSystems_dword_78E980[idx]);
                pSystem->mFlags &= ~(system_struct::eVoided | system_struct::eFailed);
            }
        }

        if (flags & system_struct::eVoided)
        {
            System_sub_40ACF4(pSystem);
            pSystem->mFlags &= ~system_struct::eVoided;
        }
    }
    pSystem->mFlags &= ~(system_struct::eVoided | system_struct::eFailed);
}
MGS_FUNC_IMPLEX(0x40ACB2, System_HouseKeeping_40ACB2, SYSTEM_IMPL);

// Finds a block by doing a binary search
LibGV_MemoryAllocation* CC System_FindAlloc_40B0F7(system_struct* pSystem, void* pFindMe)
{
    // First see if the pointer is within the heap range
    if (pFindMe < pSystem->mStartAddr || pFindMe >= pSystem->mEndAddr)
    {
        return nullptr;
    }

    // TODO: The real function does a binary search here, I just do a slower brute force search..
    for (DWORD i = 0; i < pSystem->mUnitsCount; i++)
    {
        if (pSystem->mAllocs[i].mPDataStart == pFindMe)
        {
            return &pSystem->mAllocs[i];
        }
    }
    return nullptr;
}
MGS_FUNC_IMPLEX(0x40B0F7, System_FindAlloc_40B0F7, SYSTEM_IMPL);

// Frees a block
void CC System_Free_40B099(int idx, void *ptr)
{
    LibGV_MemoryAllocation* pAlloc = System_FindAlloc_40B0F7(&gSystems_dword_78E980[idx], ptr);
    if (pAlloc && pAlloc->mAllocType)
    {
        int numToErase = 0;
        
        // Free the unit
        pAlloc->mAllocType = LibGV_MemoryAllocation::eFree;

        LibGV_MemoryAllocation* pEraseFrom = pAlloc;

        // If the unit we are freeing is the first unit or the unit before the one 
        // we are freeing is not free
        if (pAlloc == &gSystems_dword_78E980[idx].mAllocs[0] || pAlloc[-1].mAllocType)
        {
            pEraseFrom = pAlloc + 1;
        }
        else
        {
            // The block before is us free, so include it for merging
            numToErase = 1;
        }

        if (pAlloc[1].mAllocType == LibGV_MemoryAllocation::eFree)
        {
            // The block after us is free so include it for merging
            ++numToErase;
        }

        if (numToErase > 0)
        {
            System_EraseContiguousBlocks_40B147(&gSystems_dword_78E980[idx], pEraseFrom, numToErase);
        }
    }
}
MGS_FUNC_IMPLEX(0x40B099, System_Free_40B099, SYSTEM_IMPL);

void TestInitAndInterleavedAllocFree()
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

    // TODO: Need to verify the actual allocs that exist at each free

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

    System_Free_40B099(0, ptrs[2]);

    System_Debug_sub_40ADEC(0);
    System_Debug_sub_40AEC0(0);

    System_Free_40B099(0, ptrs[6]);

    System_Debug_sub_40ADEC(0);
    System_Debug_sub_40AEC0(0);

    System_Free_40B099(0, ptrs[4]);

    System_Debug_sub_40ADEC(0);
    System_Debug_sub_40AEC0(0);
}

void DoTestSystem()
{
    TestInitAndInterleavedAllocFree();
}
