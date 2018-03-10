#pragma once

#include "MgsFunction.hpp"
#include "System.hpp"
#include "types.hpp"

void CC Init_Gamed_Create_44E12B();

void* CC ResidentTopAllocate_40B379(int size);
void CC Stage_LoadRelated_44EB27(WORD stageNameHashed, const char* pStageName);
const char* CC File_StageName_44EB83();

enum PsxButtonBits : u32
{
    eL2 = 1 << 0,
    eR2 = 1 << 1,
    eL1 = 1 << 2,
    eR1 = 1 << 3,
    eTriangle = 1 << 4,
    eCircle = 1 << 5,
    eCross = 1 << 6,
    eSquare = 1 << 7,
    eSelect = 1 << 8,
    // As seen in LibEtc.h of PSYQ.. don't think these can ever be used.
    // PADi 9 ?
    // PADj 10 ?
    eStart = 1 << 11,
    eDPadUp = 1 << 12,
    eDPadRight = 1 << 13,
    eDPadDown = 1 << 14,
    eDPadLeft = 1 << 15,
};

#pragma pack(push)
#pragma pack(push, 1)
struct ButtonStates
{
    WORD field_0_button_status;
    WORD field_2_button_pressed;
    WORD field_4_button_release;
    WORD field_6_button_quick;
    short field_8_dir;
    short field_A_analog;
    BYTE field_C_right_dx;
    BYTE field_D_right_dy;
    BYTE field_E_left_dx;
    BYTE field_F_left_dy;
};
MGS_ASSERT_SIZEOF(ButtonStates, 0x10);
#pragma pack(pop)


struct GameState_0x180
{
    WORD field_0;
    short gDiffcultyLevel_78E7E2;
    DWORD gFlags_dword_78E7E4;
    WORD gStartingCdId_78E7E8;
    WORD gScripTbl_generic_78E7EA;
    WORD stage_name_hash_word_78E7EC;
    WORD gScript_loader_param_m_78E7EE;
    WORD gScript_loader_param_p_78E7F0[3];
    WORD gSnakeCurrentHealth_78E7F6;
    WORD gSnakeMaxHealth_78E7F8;
    BYTE gSnakeFlags_byte_78E7FA;
    BYTE field_1B;
    short gLoadItemFuncIdx_word_78E7FC;
    short gMenu_Selected_item_idx_word_78E7FE;
    WORD field_20;
    WORD gWeapon_states_word_78E802[10];
    WORD gWeaponCapacity_word_78E816[10];
    short gItem_states_word_78E82A[24];
    WORD gEnd1_unk_78E85A[3];
    WORD field_80;
    WORD gMenu_PAL_card_icon_idx_word_78E862;
    WORD s_word_78E864;
    WORD s_word_78E866;
    WORD s_word_78E868;
    WORD gItemsAreFrozen_word_78E86A;
    WORD field_8C;
    WORD gFreezeItemsTimer_word_78E86E;
    WORD field_90;
    short gSnakeShakeDelay_word_78E872;
    WORD field_94;
    short gSnakeColdTimer_word_78E876;
    WORD field_98;
    short gSnakeCold_word_78E87A;
    WORD gTimesSpotted_word_78E87C;
    WORD gNumEnemiesKilled_word_78E87E;
    DWORD field_A0[3];
    WORD gNumRations_word_78E88C;
    WORD gContinues_word_78E88E;
    WORD gNumSaves_word_78E890;
    BYTE field_B2[2];
    WORD field_B4;
    WORD gGameTime_word_78E896;
    WORD gGameTime_word_78E898;
    WORD field_BA;
    WORD field_BC;
    WORD field_BE;
    WORD save_data_192_word_78E8A0[96];
};
MGS_ASSERT_SIZEOF(GameState_0x180, 0x180);

MGS_VAR_EXTERN(GameState_0x180, gGameStates_78E7E0);

MGS_VAR_EXTERN(int, gCdId_78D7B0);
MGS_VAR_EXTERN(ButtonStates*, gpActiveButtons_995324);
MGS_ARY_EXTERN(ButtonStates, 4, gButtonsArray4_7919C0);

MGS_VAR_EXTERN(int, gLoaderState_dword_9942B8);
MGS_VAR_EXTERN(BYTE*, gResidentTop_dword_78E960);
MGS_VAR_EXTERN(BYTE*, gSavedTop_78E964);
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
