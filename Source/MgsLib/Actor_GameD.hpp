#pragma once

#include "MgsFunction.hpp"
#include "System.hpp"

void CC Init_Gamed_Create_44E12B();

void* CC ResidentTopAllocate_40B379(int size);
void CC Stage_LoadRelated_44EB27(WORD stageNameHashed, const char* pStageName);
const char* CC File_StageName_44EB83();


#pragma pack(push)
#pragma pack(push, 1)
struct ButtonStates
{
    WORD field_0_button_status;
    WORD field_2_button_pressed;
    WORD field_4_button_release;
    WORD field_8_button_quick;
    short field_8_dir;
    short field_A_analog;
    BYTE field_C_right_dx;
    BYTE field_D_right_dy;
    BYTE field_E_left_dx;
    BYTE field_F_left_dy;
};
MGS_ASSERT_SIZEOF(ButtonStates, 0x10);
#pragma pack(pop)

MGS_VAR_EXTERN(int, gCdId_78D7B0);
MGS_VAR_EXTERN(ButtonStates*, gpActiveButtons_995324);
MGS_ARY_EXTERN(ButtonStates, 4, gButtonsArray4_7919C0);

MGS_VAR_EXTERN(int, gLoaderState_dword_9942B8);
MGS_VAR_EXTERN(BYTE*, gResidentTop_dword_78E960);
MGS_VAR_EXTERN(BYTE*, gSavedTop_78E964);
MGS_VAR_EXTERN(WORD, stage_name_hash_word_78E7EC);
MGS_VAR_EXTERN(DWORD, dword_6893D4);

// Not really like this in the real game, but we need something like this
// so we have an end marker to allocate backwards
struct ResidentHeap
{
    BYTE mHeap[626178]; // This size is just a guess
    BYTE* AlignedEnd()
    {
        BYTE* p8BeforeEnd = &mHeap[_countof(mHeap) - 8];
        return (BYTE*)(RoundUpPowerOf2((DWORD)p8BeforeEnd, 4));
    }
};
MGS_VAR_EXTERN(ResidentHeap, gResidentHeap_81001F);
MGS_ARY_EXTERN(BYTE, 438272, gSystem2_memory_unk_8A8E20);
MGS_VAR_EXTERN(DWORD, gTotalFrameTime_dword_995344);
