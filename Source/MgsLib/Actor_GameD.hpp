#pragma once

#include "MgsFunction.hpp"

void CC Init_Gamed_Create_44E12B();

void* CC ResidentTopAllocate_40B379(int size);

MGS_VAR_EXTERN(int, dword_78D7B0);
MGS_VAR_EXTERN(DWORD, dword_995324);
MGS_VAR_EXTERN(DWORD, dword_7919C0);
MGS_VAR_EXTERN(int, gLoaderState_dword_9942B8);
MGS_VAR_EXTERN(DWORD, dword_6893D4);
MGS_VAR_EXTERN(BYTE*, gResidentTop_dword_78E960);
MGS_VAR_EXTERN(BYTE*, gSavedTop_78E964);

// Not really like this in the real game, but we need something like this
// so we have an end marker to allocate backwards
struct ResidentHeap
{
    BYTE mHeap[626178]; // This size is just a guess
    BYTE mPadding1;
    BYTE mPadding2;
    BYTE mHeapEnd; // This MUST be aligned to 4 TODO: Find a way to assert this at compile time or early run time
};
MGS_VAR_EXTERN(ResidentHeap, gResidentHeap_81001F);

MGS_ARY_EXTERN(BYTE, 438272, gSystem2_memory_unk_8A8E20);
