#include "stdafx.h"
#include "Actor_GameD.hpp"
#include "Script.hpp"
#include "LibDG.hpp"
#include "LibGV.hpp"
#include "Actor_Debug.hpp"
#include "Actor_Loader.hpp"
#include "System.hpp"
#include "Actor_Rank.hpp" // gGameStates_78E7E0.gGameTime_word_78E896
#include "Fs.hpp"
#include "WinMain.hpp"
#include "Map.hpp"
#include "Menu.hpp"
#include "Renderer.hpp"
#include "Input.hpp"

#define ACTOR_GAMED_IMPL true

struct GameD_Struct
{
    Actor mBase;
    DWORD gamed_unk_722780;
    DWORD gamed_unk_722784;
};
MGS_ASSERT_SIZEOF(GameD_Struct, 0x28);

MGS_VAR(1, 0x78E7E0, GameState_0x180, gGameStates_78E7E0, {});

MGS_VAR(1, 0x722760, GameD_Struct, gGameD_stru_722760, {});
MGS_VAR(1, 0x995344, DWORD, gTotalFrameTime_dword_995344, 0);
MGS_VAR(1, 0x7227A0, DWORD, script_cancel_non_zero_dword_7227A0, 0);
MGS_VAR(1, 0x9942B8, int, gLoaderState_dword_9942B8, 0);

MGS_ARY(1, 0x7227C8, WORD, 8, gStageHashStack_7227C8, {});
MGS_VAR(1, 0x7227D8, WORD, gCurrentStageNameHashed_7227D8, 0);
MGS_ARY(1, 0x7227DC, char, 12, gStageName_7227DC, {});



MGS_VAR(1, 0x78D7B0, int, gCdId_78D7B0, 0);

MGS_VAR(1, 0x995324, ButtonStates*, gpActiveButtons_995324, 0);
MGS_ARY(1, 0x7919C0, ButtonStates, 4, gButtonsArray4_7919C0, {});

MGS_VAR(1, 0x78E960, BYTE*, gResidentTop_dword_78E960, 0);
MGS_VAR(1, 0x78E964, BYTE*, gSavedTop_78E964, 0);

MGS_VAR(1, 0x9942A4, DWORD, dword_9942A4, 0);
MGS_VAR(1, 0x791E08, DWORD, dword_791E08, 0);
MGS_VAR(1, 0x7227C4, DWORD, dword_7227C4, 0);
MGS_VAR(1, 0x722794, DWORD, dword_722794, 0);


MGS_FUNC_NOT_IMPL(0x0044E287, void __cdecl(), sub_44E287);

MGS_FUNC_NOT_IMPL(0x00521892, int CC(), sub_521892);
MGS_FUNC_NOT_IMPL(0x0044DEDE, int CC(), sub_44DEDE);
MGS_FUNC_NOT_IMPL(0x00453B11, void CC(), Res_game_camera_create_453B11);
MGS_FUNC_NOT_IMPL(0x00455299, void CC(), sub_455299);
MGS_FUNC_NOT_IMPL(0x0040945D, void CC(), sub_40945D);
MGS_FUNC_NOT_IMPL(0x004553C5, void CC(), sub_4553C5);
MGS_FUNC_NOT_IMPL(0x00455017, void CC(), GM_SoundStart_455017);
MGS_FUNC_NOT_IMPL(0x0044E8F1, void CC(), sub_44E8F1);
MGS_FUNC_NOT_IMPL(0x0044E969, void CC(), sub_44E969);
MGS_FUNC_NOT_IMPL(0x004583BB, void CC(), StreamActorStop_4583BB);
MGS_FUNC_NOT_IMPL(0x0044F28B, void CC(), GameD_update_helper_44F28B);
MGS_FUNC_NOT_IMPL(0x004552CB, void CC(), GameD_Sound_4552CB);
MGS_FUNC_NOT_IMPL(0x004502CD, Actor* CC(__int16 bShowContinue), Res_over_create_4502CD);
MGS_FUNC_NOT_IMPL(0x0044E932, void CC(), sub_44E932);


void CC Create_loader_44E226();
void CC LibDG_Clean_Texture_Cache_401110();
void CC LibGV_40A4BB();

MGS_ARY(1, 0x651DE4, const BYTE, 14, gKeyPsxBitIndex_byte_651DE4, { 4, 7, 5, 6, 2, 0, 3, 1, 8, 15, 13, 12, 14, 11 });

MGS_VAR(1, 0x71D188, DWORD, gCodecReturnKey_dword_71D188, 0);
MGS_VAR(1, 0x651D8C, DWORD, dword_651D8C, 1);

MGS_VAR(1, 0x651D90, DWORD, dword_651D90, 1);
MGS_VAR(1, 0x651D88, DWORD, dword_651D88, 1);
MGS_VAR(1, 0x71D160, DWORD, dword_71D160, 0);

MGS_VAR(1, 0x733E1C, DWORD, dword_733E1C, 0);
MGS_VAR(1, 0x657148, int, k65_unk_657148, 65);

MGS_VAR(1, 0x716E34, int, gMouseXBound_dword_716E34, 0);
MGS_VAR(1, 0x716E58, int, gMouseYBound_dword_716E58, 0);

MGS_VAR(1, 0x6FD1F4, int, gMouseY_dword_6FD1F4, 0);
MGS_VAR(1, 0x6FD1D0, int, gMouseX_dword_6FD1D0, 0);

MGS_VAR(1, 0x71D180, DWORD, gInputMouse_dword_71D180, 0);

MGS_VAR(1, 0x99562C, DWORD, gLastInputWasKeyBoard_dword_99562C, 0);
MGS_VAR(1, 0x71D17C, DWORD, gAllowMovement_dword_71D17C, 0);
MGS_VAR(1, 0x721E78, DWORD, gBgStateRunning_dword_721E78, 0);

// TODO: This is inited by the game config load, we force it to a known good state here for standalone binary
// which can't load configs yet
MGS_ARY(1, 0x716E1C, BYTE, 16, vKeys_byte_716E1C, { 0x58, 0x11, 0x20, 0x10, 0x51, 0x41, 0x57, 0x53, 0x09, 0x25 /*VK_LEFT?*/, 0x27, 0x26, 0x28, 0x1b, 0, 0 });

MGS_VAR(1, 0x71D144, DWORD, gbNothingPressedLastTime_71D144, 0);
MGS_VAR(1, 0x71D790, DWORD, gConfig_dword_71D790, 0);

// Local statics of GameD_Input_42C3C7
MGS_VAR(1, 0x6FD1BC, DWORD, sLastInputWasKeyBoard_dword_6FD1BC, 0);
MGS_VAR(1, 0x71D1B8, DWORD, sEscapeCounter_dword_71D1B8, 0);
MGS_VAR(1, 0x652B0C, DWORD, sButton_13_dword_652B0C, 1);
MGS_VAR(1, 0x652B10, DWORD, sInputLeft_dword_652B10, 1);
MGS_VAR(1, 0x652B1C, DWORD, sInputDown_dword_652B1C, 1);
MGS_VAR(1, 0x652B14, DWORD, sInputRight_dword_652B14, 1);
MGS_VAR(1, 0x652B20, DWORD, sInputSpace_dword_652B20, 1);
MGS_VAR(1, 0x652B18, DWORD, sInputUp_dword_652B18, 1);
MGS_VAR(1, 0x71D1BC, DWORD, sCounter_dword_71D1BC, 0);

void GiveAllItemsCheat()
{
    for (int n = 0; n < 24; n++)
    {
        if (n == Items::eIdCard)
        {
            gGameStates_78E7E0.gItem_states_word_78E82A[n] = 7;
        }
        else if (n != Items::eItemBomb)
        {
            gGameStates_78E7E0.gItem_states_word_78E82A[n] = 1;
        }
    }
}

DWORD CC GameD_Input_42C3C7()
{
    DWORD inputBits = 0;
    if (gKeys_9AD880[VK_ESCAPE] && game_state_dword_72279C.flags == 0x20000000 && strstr(gDest_78E7C0, "s19a"))
    {
        gKeys_9AD880[VK_ESCAPE] = 0;
        return 1 << gKeyPsxBitIndex_byte_651DE4[13];
    }

    if (gCodecReturnKey_dword_71D188)
    {
        if (gKeys_9AD880[VK_RETURN])
        {
            inputBits = 1 << gKeyPsxBitIndex_byte_651DE4[2];
        }
    }

    if (!gAllowMovement_dword_71D17C && sEscapeCounter_dword_71D1B8)
    {
        --sEscapeCounter_dword_71D1B8;
        gKeys_9AD880[VK_ESCAPE] = 0;
        return 0;
    }

    if (gAllowMovement_dword_71D17C)
    {
        sEscapeCounter_dword_71D1B8 = 15;
    }

    Input_Read_43BD6E();

    if (gbNothingPressedLastTime_71D144 && gInput_MouseZ_dword_71D67C)
    {
        bool bNothingPressed = true;
        for (int i = 0; i < 14; ++i)
        {
            if (gButtonStates_dword_71D30C[gButtonMappings_6571F4[i]])
            {
                bNothingPressed = false;
                break;
            }
        }

        if (bNothingPressed)
        {
            gbNothingPressedLastTime_71D144 = 0;
        }
        else
        {
            for (int j = 0; j < 14; ++j)
            {
                gButtonStates_dword_71D30C[gButtonMappings_6571F4[j]] = 0;
            }
        }
    }

    if (!gButtonStates_dword_71D30C[gButtonMappings_6571F4[13]])
    {
        sButton_13_dword_652B0C = 1;
    }

    if (sButton_13_dword_652B0C
        && (gButtonStates_dword_71D30C[gButtonMappings_6571F4[13]] && !gConfig_dword_71D790
            || gButtonStates_dword_71D30C[gButtonMappings_6571F4[13]]
            && gConfig_dword_71D790
            && gButtonStates_dword_71D30C[gInputShiftButton_dword_71D41C] == (gButtonStates2_unk_65714C[gButtonMappings_6571F4[13]] != 0))
        && gMouseMove_dword_717348 != 3)
    {
        if (game_state_dword_72279C.flags == 0x20000000 && strstr(gDest_78E7C0, "s19a"))
        {
            return 1 << gKeyPsxBitIndex_byte_651DE4[13];
        }
        gKeys_9AD880[VK_ESCAPE] = 1;
        gKeys_9AD880[VK_RETURN] = 0;
        sButton_13_dword_652B0C = 0;
        for (int i = 0; i < k65_unk_657148; ++i)
        {
            gButtonStates_dword_71D30C[i] = 0;
        }
    }

    if (gKeys_9AD880[VK_ESCAPE])
    {
        if (!gEscapePressed_NoMouseNoBgState_dword_717354 && !gMouseMove_dword_717348 && !gBgStateRunning_dword_721E78)
        {
            gEscapePressed_NoMouseNoBgState_dword_717354 = 1;
        }
    }

    if ((game_state_dword_72279C.flags & 0x20488000) != 0x20488000
        && (game_state_dword_72279C.flags & 0x80000 || (game_state_dword_72279C.flags & 0x104A2000) == 0x104A2000)
        || gAllowMovement_dword_71D17C
        || (game_state_dword_72279C.flags & 0x804A0000) == 0x804A0000
        || dword_733E1C)
    {
        if (gButtonStates_dword_71D30C[gButtonMappings_6571F4[9]] || gKeys_9AD880[VK_LEFT])
        {
            inputBits &= ~(1 << gKeyPsxBitIndex_byte_651DE4[9]);
        }
        else
        {
            sInputLeft_dword_652B10 = 1;
        }

        if (sInputLeft_dword_652B10)
        {
            if (gButtonStates_dword_71D30C[gButtonMappings_6571F4[9]])
            {
                gKeys_9AD880[VK_LEFT] = 1;
                sInputLeft_dword_652B10 = 0;
            }
        }

        if (gButtonStates_dword_71D30C[gButtonMappings_6571F4[10]] || gKeys_9AD880[VK_RIGHT])
        {
            inputBits &= ~(1 << gKeyPsxBitIndex_byte_651DE4[10]);
        }
        else
        {
            sInputRight_dword_652B14 = 1;
        }

        if (sInputRight_dword_652B14)
        {
            if (gButtonStates_dword_71D30C[gButtonMappings_6571F4[10]])
            {
                gKeys_9AD880[VK_RIGHT] = 1;
                sInputRight_dword_652B14 = 0;
            }
        }

        if (gButtonStates_dword_71D30C[gButtonMappings_6571F4[11]] || gKeys_9AD880[VK_UP])
        {
            inputBits &= ~(1 << gKeyPsxBitIndex_byte_651DE4[11]);
        }
        else
        {
            sInputUp_dword_652B18 = 1;
        }

        if (sInputUp_dword_652B18)
        {
            if (gButtonStates_dword_71D30C[gButtonMappings_6571F4[11]])
            {
                gKeys_9AD880[VK_UP] = 1;
                sInputUp_dword_652B18 = 0;
            }
        }

        if (gButtonStates_dword_71D30C[gButtonMappings_6571F4[12]] || gKeys_9AD880[VK_DOWN])
        {
            inputBits &= ~(1 << gKeyPsxBitIndex_byte_651DE4[12]);
        }
        else
        {
            sInputDown_dword_652B1C = 1;
        }

        if (sInputDown_dword_652B1C)
        {
            if (gButtonStates_dword_71D30C[gButtonMappings_6571F4[12]])
            {
                gKeys_9AD880[VK_DOWN] = 1;
                sInputDown_dword_652B1C = 0;
            }
        }

        if (!gButtonStates_dword_71D30C[gButtonMappings_6571F4[2]] && !gKeys_9AD880[VK_RETURN])
        {
            sInputSpace_dword_652B20 = 1;
        }

        if (sInputSpace_dword_652B20 && gButtonStates_dword_71D30C[gButtonMappings_6571F4[2]])
        {
            dword_71D160 = 1;
            if (gAllowMovement_dword_71D17C)
            {
                gKeys_9AD880[VK_RETURN] = 1;
            }
            sInputSpace_dword_652B20 = 0;
        }
        else
        {
            dword_71D160 = 0;
        }

        if (sInputLeft_dword_652B10)
        {
            if (gKeys_9AD880[VK_NUMPAD4] || gKeys_9AD880[VK_LEFT])
            {
                inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[9];
                sInputLeft_dword_652B10 = 0;
            }
        }

        if (sInputRight_dword_652B14)
        {
            if (gKeys_9AD880[VK_NUMPAD6] || gKeys_9AD880[VK_RIGHT])
            {
                inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[10];
                sInputRight_dword_652B14 = 0;
            }
        }

        if (sInputUp_dword_652B18)
        {
            if (gKeys_9AD880[VK_NUMPAD8] || gKeys_9AD880[VK_UP])
            {
                inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[11];
                sInputUp_dword_652B18 = 0;
            }
        }

        if (sInputDown_dword_652B1C)
        {
            if (gKeys_9AD880[VK_NUMPAD2] || gKeys_9AD880[VK_DOWN])
            {
                inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[12];
                sInputDown_dword_652B1C = 0;
            }
        }

        if (sInputSpace_dword_652B20 && (gKeys_9AD880[VK_RETURN] || gKeys_9AD880[VK_SPACE]))
        {
            sInputSpace_dword_652B20 = 0;
            if (dword_733E1C)
            {
                gKeys_9AD880[VK_SPACE] = 0;
                gKeys_9AD880[VK_RETURN] = 0;
                return (1 << gKeyPsxBitIndex_byte_651DE4[13]) | inputBits;
            }
            if (!gMouseMove_dword_717348 && !gAllowMovement_dword_71D17C)
            {
                gKeys_9AD880[VK_SPACE] = 0;
                gKeys_9AD880[VK_RETURN] = 0;
            }
            inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[2];
            if (game_state_dword_72279C.flags == 0x4A6000)
            {
                inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[13];
            }
        }

        if (gKeys_9AD880[VK_ESCAPE])
        {
            if ((game_state_dword_72279C.flags & 0x104A2000) != 0x104A2000
                && (game_state_dword_72279C.flags & 0x804A0000) != 0x804A0000
                && !gCodecReturnKey_dword_71D188)
            {
                gKeys_9AD880[VK_ESCAPE] = 0;
            }
            inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[3];
        }
        if (!gAllowMovement_dword_71D17C)
        {
            gKeys_9AD880[VK_UP] = 0;
            gKeys_9AD880[VK_DOWN] = 0;
            gKeys_9AD880[VK_LEFT] = 0;
            gKeys_9AD880[VK_RIGHT] = 0;
            gKeys_9AD880[VK_SPACE] = 0;
        }
    }
    else if (gKeys_9AD880[VK_PAUSE] || gKeys_9AD880[VK_F3])
    {
        gKeys_9AD880[VK_PAUSE] = 0;
        gKeys_9AD880[VK_F3] = 0;
        return (1 << gKeyPsxBitIndex_byte_651DE4[13]) | inputBits;
    }

    if (gKeys_9AD880[VK_ESCAPE] || gInput_MouseRightButton_734914 && dword_651D8C)
    {
        dword_651D8C = 0;
        if (dword_732E64 == 1 || dword_732E64 == 2 && gCodecReturnKey_dword_71D188)
        {
            gKeys_9AD880[VK_ESCAPE] = 0;
            inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[3];
            dword_732E64 = 2;
        }
        if (gMouseMove_dword_717348)
        {
            gKeys_9AD880[VK_ESCAPE] = 0;
            inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[3];
        }
    }

    if (gKeys_9AD880[VK_ESCAPE] || sCounter_dword_71D1BC)
    {
        if ((game_state_dword_72279C.flags & 0x408000) != 0x408000
            && (game_state_dword_72279C.flags == 0x10000020
                || game_state_dword_72279C.flags == 0x70001020
                || game_state_dword_72279C.flags & 0xC0440000))
        {
            if (++sCounter_dword_71D1BC == 4)
            {
                sCounter_dword_71D1BC = 0;
            }
            gKeys_9AD880[VK_ESCAPE] = 0;
            return (1 << gKeyPsxBitIndex_byte_651DE4[3]) | inputBits;
        }
        sCounter_dword_71D1BC = 0;
    }

    if ((game_state_dword_72279C.flags & 0x104A2000) != 0x104A2000)
    {
        for (int l = 0; l < 14; ++l)
        {
            if (gButtonMappings_6571F4[l] < 0 || gButtonMappings_6571F4[l] >= 0x20)
            {
                if (gButtonMappings_6571F4[l] != 0xFF)
                {
                    if (gButtonStates_dword_71D30C[gButtonMappings_6571F4[l]] & 0x80)
                    {
                        inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[l];
                    }
                }
            }
            else if (gConfig_dword_71D790)
            {
                if (gButtonMappings_6571F4[l] != gInputShiftButton_dword_71D41C)
                {
                    if (gButtonStates2_unk_65714C[l])
                    {
                        if (gButtonStates_dword_71D30C[gInputShiftButton_dword_71D41C] & 0x80)
                        {
                            if (gButtonStates_dword_71D30C[gButtonMappings_6571F4[l]] & 0x80)
                            {
                                inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[l];
                                gButtonStates_dword_71D30C[gButtonMappings_6571F4[l]] = 0;
                            }
                        }
                    }
                    else if (gButtonStates_dword_71D30C[gButtonMappings_6571F4[l]] & 0x80)
                    {
                        bool bFound = false;
                        for (int m = 0; m < 14; ++m)
                        {
                            if (m != l && gButtonMappings_6571F4[l] == gButtonMappings_6571F4[m])
                            {
                                bFound = true;
                                break;
                            }
                        }
                        if (bFound)
                        {
                            if (!(gButtonStates_dword_71D30C[gInputShiftButton_dword_71D41C] & 0x80))
                            {
                                inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[l];
                            }
                        }
                        else
                        {
                            inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[l];
                        }
                    }
                }
            }
            else if (gButtonStates_dword_71D30C[gButtonMappings_6571F4[l]] & 0x80)
            {
                inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[l];
            }
        }
    }

    gMouseY_dword_6FD1F4 += gInput_MouseY_dword_73490C;
    gMouseX_dword_6FD1D0 += gInput_MouseX_dword_734908;

    if (!gMouseMove_dword_717348)
    {
        if (gInputMouse_dword_71D180)
        {
            if (gMouseX_dword_6FD1D0 <= gMouseXBound_dword_716E34 - 100)
            {
                gMouseXBound_dword_716E34 = gMouseX_dword_6FD1D0;
                gInput_MouseX_dword_734908 = 0;
                inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[9];
            }

            if (gMouseX_dword_6FD1D0 >= gMouseXBound_dword_716E34 + 100)
            {
                gMouseXBound_dword_716E34 = gMouseX_dword_6FD1D0;
                gInput_MouseX_dword_734908 = 0;
                inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[10];
            }

            if (gMouseY_dword_6FD1F4 <= gMouseYBound_dword_716E58 - 100)
            {
                gMouseY_dword_6FD1F4 = 0;
                gMouseYBound_dword_716E58 = 0;
                gInput_MouseY_dword_73490C = 0;
                inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[11];
            }

            if (gMouseY_dword_6FD1F4 >= gMouseYBound_dword_716E58 + 100)
            {
                gMouseY_dword_6FD1F4 = 0;
                gMouseYBound_dword_716E58 = 0;
                gInput_MouseY_dword_73490C = 0;
                inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[12];
            }
        }
    }

    if (gCheatsEnabled_71687C)
    {
        if (gKeys_9AD880[VK_BACK])
        {
            if ((game_state_dword_72279C.flags & 0x104A2000) != 0x104A2000)
            {
                if (gKeys_9AD880['1'])
                {
                    gKeys_9AD880[VK_BACK] = 0;
                    gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 0;
                    gGameStates_78E7E0.gWeapon_states_word_78E802[0] = 999;
                    gGameStates_78E7E0.gWeaponCapacity_word_78E816[0] = 999;
                }

                if (gKeys_9AD880['2'])
                {
                    gKeys_9AD880[VK_BACK] = 0;
                    gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 1;
                    gGameStates_78E7E0.gWeapon_states_word_78E802[1] = 999;
                    gGameStates_78E7E0.gWeaponCapacity_word_78E816[1] = 999;
                }

                if (gKeys_9AD880['3'])
                {
                    gKeys_9AD880[VK_BACK] = 0;
                    gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 2;
                    gGameStates_78E7E0.gWeapon_states_word_78E802[2] = 999;
                    gGameStates_78E7E0.gWeaponCapacity_word_78E816[2] = 999;
                }

                if (gKeys_9AD880['4'])
                {
                    gKeys_9AD880[VK_BACK] = 0;
                    gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 3;
                    gGameStates_78E7E0.gWeapon_states_word_78E802[3] = 999;
                    gGameStates_78E7E0.gWeaponCapacity_word_78E816[3] = 999;
                }

                if (gKeys_9AD880['5'])
                {
                    gKeys_9AD880[VK_BACK] = 0;
                    gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 4;
                    gGameStates_78E7E0.gWeapon_states_word_78E802[4] = 999;
                    gGameStates_78E7E0.gWeaponCapacity_word_78E816[4] = 999;
                }

                if (gKeys_9AD880['6'])
                {
                    gKeys_9AD880[VK_BACK] = 0;
                    gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 5;
                    gGameStates_78E7E0.gWeapon_states_word_78E802[5] = 999;
                    gGameStates_78E7E0.gWeaponCapacity_word_78E816[5] = 999;
                }

                if (gKeys_9AD880['7'])
                {
                    gKeys_9AD880[VK_BACK] = 0;
                    gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 6;
                    gGameStates_78E7E0.gWeapon_states_word_78E802[6] = 999;
                    gGameStates_78E7E0.gWeaponCapacity_word_78E816[6] = 999;
                }

                if (gKeys_9AD880['8'])
                {
                    gKeys_9AD880[VK_BACK] = 0;
                    gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 7;
                    gGameStates_78E7E0.gWeapon_states_word_78E802[7] = 999;
                    gGameStates_78E7E0.gWeaponCapacity_word_78E816[7] = 999;
                }

                if (gKeys_9AD880['9'])
                {
                    gKeys_9AD880[VK_BACK] = 0;
                    gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 8;
                    gGameStates_78E7E0.gWeapon_states_word_78E802[8] = 999;
                    gGameStates_78E7E0.gWeaponCapacity_word_78E816[8] = 999;
                }

                if (gKeys_9AD880['0'])
                {
                    gKeys_9AD880[VK_BACK] = 0;
                    gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 9;
                    gGameStates_78E7E0.gWeapon_states_word_78E802[9] = 999;
                    gGameStates_78E7E0.gWeaponCapacity_word_78E816[9] = 999;
                }

                // Give all items cheat
                if (gKeys_9AD880[VK_ESCAPE])
                {
                    gKeys_9AD880[VK_ESCAPE] = 0;
                    gKeys_9AD880[VK_BACK] = 0;
                    GiveAllItemsCheat();
                }
            }
        }
    }
    else if (!gCodecReturnKey_dword_71D188 && !gAllowMovement_dword_71D17C && !(gActorPauseFlags_dword_791A0C & 2))
    {
        if (gKeys_9AD880['1'])
        {
            if (gGameStates_78E7E0.gWeapon_states_word_78E802[0] != -1
                && gGameStates_78E7E0.gWeaponCapacity_word_78E816[0] > 0
                && !(gGameStates_78E7E0.gWeapon_states_word_78E802[0] & 0x8000))
            {
                gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 0;
            }
        }

        if (gKeys_9AD880['2'])
        {
            if (gGameStates_78E7E0.gWeapon_states_word_78E802[1] != -1
                && gGameStates_78E7E0.gWeaponCapacity_word_78E816[1] > 0
                && !(gGameStates_78E7E0.gWeapon_states_word_78E802[1] & 0x8000))
            {
                gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 1;
            }
        }

        if (gKeys_9AD880['3'])
        {
            if (gGameStates_78E7E0.gWeapon_states_word_78E802[2] != -1
                && gGameStates_78E7E0.gWeaponCapacity_word_78E816[2] > 0
                && !(gGameStates_78E7E0.gWeapon_states_word_78E802[2] & 0x8000))
            {
                gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 2;
            }
        }

        if (gKeys_9AD880['4'])
        {
            if (gGameStates_78E7E0.gWeapon_states_word_78E802[3] != -1
                && gGameStates_78E7E0.gWeaponCapacity_word_78E816[3] > 0
                && !(gGameStates_78E7E0.gWeapon_states_word_78E802[3] & 0x8000))
            {
                gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 3;
            }
        }

        if (gKeys_9AD880['5'])
        {
            if (gGameStates_78E7E0.gWeapon_states_word_78E802[4] != -1
                && gGameStates_78E7E0.gWeaponCapacity_word_78E816[4] > 0
                && !(gGameStates_78E7E0.gWeapon_states_word_78E802[4] & 0x8000))
            {
                gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 4;
            }
        }

        if (gKeys_9AD880['6'])
        {
            if (gGameStates_78E7E0.gWeapon_states_word_78E802[5] != -1
                && gGameStates_78E7E0.gWeaponCapacity_word_78E816[5] > 0
                && !(gGameStates_78E7E0.gWeapon_states_word_78E802[5] & 0x8000))
            {
                gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 5;
            }
        }

        if (gKeys_9AD880['7'])
        {
            if (gGameStates_78E7E0.gWeapon_states_word_78E802[6] != -1
                && gGameStates_78E7E0.gWeaponCapacity_word_78E816[6] > 0
                && !(gGameStates_78E7E0.gWeapon_states_word_78E802[6] & 0x8000))
            {
                gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 6;
            }
        }

        if (gKeys_9AD880['8'])
        {
            if (gGameStates_78E7E0.gWeapon_states_word_78E802[7] != -1
                && gGameStates_78E7E0.gWeaponCapacity_word_78E816[7] > 0
                && !(gGameStates_78E7E0.gWeapon_states_word_78E802[7] & 0x8000))
            {
                gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 7;
            }
        }

        if (gKeys_9AD880['9'])
        {
            if (gGameStates_78E7E0.gWeapon_states_word_78E802[8] != -1
                && gGameStates_78E7E0.gWeaponCapacity_word_78E816[8] > 0
                && !(gGameStates_78E7E0.gWeapon_states_word_78E802[8] & 0x8000))
            {
                gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 8;
            }
        }

        if (gKeys_9AD880['0'])
        {
            if (gGameStates_78E7E0.gWeapon_states_word_78E802[9] != -1
                && gGameStates_78E7E0.gWeaponCapacity_word_78E816[9] > 0
                && !(gGameStates_78E7E0.gWeapon_states_word_78E802[9] & 0x8000))
            {
                gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = 9;
            }
        }
    }

    dword_651D90 = dword_651D90 != 0;

    if (!gInput_MouseLeftButton_734910 || dword_651D88)
    {
        dword_651D88 = 1;
    }

    if (!gInput_MouseRightButton_734914 || dword_651D8C)
    {
        dword_651D8C = 1;
    }

    if (game_state_dword_72279C.flags & 0x104A2000)
    {
        if (!(game_state_dword_72279C.flags & 0x8000) && !gAllowMovement_dword_71D17C)
        {
            if (gInput_MouseLeftButton_734910)
            {
                if (dword_651D88)
                {
                    dword_651D88 = 0;
                    dword_651D90 = 0;
                    gKeyBoardButtonStates_dword_71D204[2] = 0;
                    inputBits |= (1 << gKeyPsxBitIndex_byte_651DE4[1]) | (1 << gKeyPsxBitIndex_byte_651DE4[2]) | (1 << gKeyPsxBitIndex_byte_651DE4[8]) | (1 << gKeyPsxBitIndex_byte_651DE4[13]);
                }
            }
        }
    }

    if (gInput_MouseRightButton_734914 && dword_651D8C
        || gKeys_9AD880[VK_ESCAPE] && (game_state_dword_72279C.flags & 0x4A6000) == 0x4A6000)
    {
        dword_651D8C = 0;
        gKeys_9AD880[VK_ESCAPE] = 0;
        inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[3];
    }

    if (inputBits)
    {
        gLastInputWasKeyBoard_dword_99562C = 2;
    }


    const DWORD inputBitsCopy = inputBits;

    for (int i = 0; i < 13; ++i)
    {
        if (!gMouseMove_dword_717348 || i != 3 && vKeys_byte_716E1C[i] != 16)
        {
            const BYTE key = vKeys_byte_716E1C[i];
            if (key < 16u || key > 17u)
            {
                if (key > 17u && key == 255)
                {
                    continue;
                }

                inputBits |= gKeys_9AD880[vKeys_byte_716E1C[i]] << gKeyPsxBitIndex_byte_651DE4[i];
                if ((game_state_dword_72279C.flags & 0x104A2000) == 0x104A2000)
                {
                    if (gKeys_9AD880[vKeys_byte_716E1C[i]])
                    {
                        if (gMouseMove_dword_717348 != 4)
                        {
                            gKeys_9AD880[vKeys_byte_716E1C[i]] = 0;
                        }
                    }
                }
                continue;
            }
            if (GetAsyncKeyState(vKeys_byte_716E1C[i]) & 0x8000)
            {
                inputBits |= 1 << gKeyPsxBitIndex_byte_651DE4[i];
            }
        }
    }

    if (inputBitsCopy != inputBits)
    {
        gLastInputWasKeyBoard_dword_99562C = 1;
    }

    if (inputBits)
    {
        sLastInputWasKeyBoard_dword_6FD1BC = gLastInputWasKeyBoard_dword_99562C;
    }
    else
    {
        gLastInputWasKeyBoard_dword_99562C = sLastInputWasKeyBoard_dword_6FD1BC;
    }

    gKeys_9AD880[VK_ESCAPE] = 0;

    if (!(inputBits & 0x800))
    {
        return inputBits;
    }

    return inputBits & 0xFFF7FFFFu;
}
MGS_FUNC_IMPLEX(0x42C3C7, GameD_Input_42C3C7, true);

void CC Init_Menu_GV_DG_44E1F9()
{
    Menu_inits_459A48();
    LibGV_40A4B1();
    LibDG_4010A6();
    //nullsub_3();
}
MGS_FUNC_IMPLEX(0x44E1F9, Init_Menu_GV_DG_44E1F9, ACTOR_GAMED_IMPL);

void CC Reset_GV_DG_44E212()
{
    LibDG_Clean_Texture_Cache_401110();
    LibGV_40A4BB();
    LibDG_ClearActiveResourceFunctionPointerList_457B7C();
}
MGS_FUNC_IMPLEX(0x0044E212, Reset_GV_DG_44E212, ACTOR_GAMED_IMPL);

DWORD CC GameD_Input_445610()
{
    if (gFreeCameraCheat_77C934)
    {
        return 0;
    }
    return GameD_Input_42C3C7();
}
MGS_FUNC_IMPLEX(0x00445610, GameD_Input_445610, ACTOR_GAMED_IMPL);

DWORD CC GameD_Input_Wrapper_4455F0()
{
    if (gFreeCameraCheat_77C934)
    {
        return 0;
    }
    return GameD_Input_42C3C7();
}
MGS_FUNC_IMPLEX(0x004455F0, GameD_Input_Wrapper_4455F0, ACTOR_GAMED_IMPL);

static void GameD_Update_helper(DWORD buttons)
{
    //if (stateTest)
    {
        // 0x8000000 = disable input?
        // 0x80000000 = kill fmv?
        //game_state_dword_72279C = 0x10800000; // 0x10800000 no input, jamming
       // game_state_dword_72279C = 0x800000; // input, jamming
        //game_state_dword_72279C = 0xa46000;
    }

    if ((buttons & 0x90F) != 0x90F || dword_7227C4)
    {
        dword_722794 = 90;
    }
    else if (--dword_722794 < 0)
    {
        static char sPsxBinaryName_byte_722740[32] = {};
        //sprintf(sPsxBinaryName_byte_722740, "cdrom:\\MGS\\%s;1", (&gBinaries_off_650068)[4 * gBinaryIndex_dword_78D7B0]);
        SetDispMask(0);
        //nullsub_70();
        //nullsub_69();
        //Ret0_44AC80();
        //RetInput_6465B0(0);
        //nullsub_11(v6);
        //CloseEvents_40842A();
        Resetgraph_AndPrintPsxStructureSizes(3);
        //Ret0_6465E0();
        //Ret0_44D000();
        //Re0_6465D0();
        //Ret0_6465E0();
        //nullsub_18();
        //nullsub_17();
        for (;;)
        {
            printf("load %s\n", sPsxBinaryName_byte_722740);
            //Ret0_44CFD0();
        }
    }
    if (game_state_dword_72279C.flags & 0x80000000)
    {
        if (gpActiveButtons_995324[2].field_2_button_pressed & 0x840)
        {
            StreamActorStop_4583BB();
        }
    }
    if (GameD_Input_445610() & 0x20)
    {
        //nullsub_68();
        int v10 = 0;
        char v11[24];
        for (int i = 0; i < 24; ++i)
        {
            v10 = v11[i] & 1 | 2 * v10;
        }
    }

    if (!gActorPauseFlags_dword_791A0C)
    {
        sub_44E8F1();
    }
}


void CC GameD_update_44E381(GameD_Struct* pGameD)
{
    const DWORD buttons = GameD_Input_445610();

    gGameStates_78E7E0.gFlags_dword_78E7E4 |= 0x400u;


    if (!(gGameStates_78E7E0.gFlags_dword_78E7E4 & 0x2400)) // Dead code
    {
        //nullsub_10();
        int v14 = dword_99533C;
        if (dword_99533C > 255)
        {
            v14 = 255;
        }
        //nullsub_12();
    }

    dword_99533C = 0;
    dword_9942A4 = 0;

    if (gActorPauseFlags_dword_791A0C & 8)
    {
        if (dword_791E08 || sub_521892() == 3)
        {
            sub_44DEDE();
        }
        else
        {
            gActorPauseFlags_dword_791A0C = (gActorPauseFlags_dword_791A0C & 0xF7);
        }
    }
    else if (dword_791E08 || sub_521892() == 3)
    {
        gActorPauseFlags_dword_791A0C = (gActorPauseFlags_dword_791A0C | 8) & 0xFF;
    }

    if (!(gActorPauseFlags_dword_791A0C & 2))
    {
        gTotalFrameTime_dword_995344 += gFrameTime_dword_791A04;
        gGameStates_78E7E0.gGameTime_word_78E896 = static_cast<WORD>(gTotalFrameTime_dword_995344 / 60 / 3600); // TODO: Is DWORD?
        gGameStates_78E7E0.gGameTime_word_78E898 = static_cast<WORD>(gTotalFrameTime_dword_995344 / 60 % 3600); // TODO: Is DWORD?
    }

    const DWORD gamed_unk_722780 = pGameD->gamed_unk_722780;
    if (!gamed_unk_722780)
    {
        if (gLoaderState_dword_9942B8)            // -1 on LoadEnd, 0 on GameD/Loader created
        {
            gLoaderState_dword_9942B8 = 1;

            if (script_cancel_non_zero_dword_7227A0 & 0x80)
            {
                counter_dword_6BED20 = 0;
            }

            if (gFixupLibDg_Allocs_And_Hahses_dword_78D7AC)
            {
                LibGV_Save_File_Cache_Records_To_Resident_Memory_40A6CD();
                LibDG_Save_Texture_Hashes_To_Resident_Memory_4026F5();
            }

            Map_ResetMapCountAndKmdsCount_44F3F7();
            Res_game_camera_create_453B11();
            ImageMove_401A13();
            sub_455299();                             // reset array of data?
            
            if (script_cancel_non_zero_dword_7227A0 & 0x10)
            {
                sub_40945D();                           // reset save map + others?
            }

            printf("exec scenario\n");
            const int scriptDataCopy = script_cancel_non_zero_dword_7227A0;
            script_cancel_non_zero_dword_7227A0 = 0;
            if (scriptDataCopy & 0x20)
            {
                Script_ProcCancelOrRun(HIWORD(scriptDataCopy), 0);
            }
            else
            {
                Script_RunMainProc_409D49();
            }
            printf("end scenario\n");

            Menu_j_inventory_right_init_11_items_459A95();
            sub_4553C5();                             // call proc id idx 2
            GM_SoundStart_455017();
            pGameD->gamed_unk_722780 = 1;
        }
        return;
    }

    if (gamed_unk_722780 != 1)
    {
        return;
    }

    if (pGameD->gamed_unk_722784 > 0)
    {
        gActorPauseFlags_dword_791A0C = gActorPauseFlags_dword_791A0C & 0xF7;
        if (--pGameD->gamed_unk_722784 <= 0)
        {
            if (File_GetStreamState_45837C() == -1)
            {
                if (gActorPauseFlags_dword_791A0C & 5)
                {
                    if (script_cancel_non_zero_dword_7227A0 & 0x80)
                    {
                        counter_dword_6BED20 = 0;
                    }
                    pGameD->gamed_unk_722784 = 1;

                    if (!gActorPauseFlags_dword_791A0C)
                    {
                        sub_44E8F1();
                    }
                    return;
                }
                pGameD->gamed_unk_722780 = 0;
                pGameD->gamed_unk_722784 = 0;
                Map_FreeHzdItems_44F38D();
                Init_Menu_GV_DG_44E1F9();
                sub_44E287();
                if (!(script_cancel_non_zero_dword_7227A0 & 0x40))
                {
                    Reset_GV_DG_44E212();
                    Create_loader_44E226();
                }
                return;
            }
            pGameD->gamed_unk_722784 = 1;
        }

        if (!gActorPauseFlags_dword_791A0C)
        {
            sub_44E8F1();
        }
        return;
    }

    if (gGameOverTimer_dword_7227A4 > 0)
    {
        if (gGameOverTimer_dword_7227A4 == 1)
        {
            if (File_GetStreamState_45837C() == -1)
            {
                if (Res_over_create_4502CD(1))
                {
                    gGameOverTimer_dword_7227A4 = -1;
                }
                else
                {
                    Actor_KillActorsAtLevel_40A23D(4);
                }
            }
            else if (File_GetStreamState_45837C() == 1)
            {
                StreamActorStop_4583BB();
            }
        }
        else
        {
            --gGameOverTimer_dword_7227A4;
        }

        GameD_Update_helper(buttons);
        return;
    }

    if (!script_cancel_non_zero_dword_7227A0 || gActorPauseFlags_dword_791A0C & 2)
    {
        if (!gActorPauseFlags_dword_791A0C)
        {
            GameD_Sound_4552CB();
        }

        if (game_state_dword_72279C.flags & 0xD8004020)
        {
            if (gActorPauseFlags_dword_791A0C & 2)
            {
                sub_44E932();
            }
        }
        else
        {
            if (!(gActorPauseFlags_dword_791A0C & 0xFD))
            {
                if (gpActiveButtons_995324->field_2_button_pressed & 0x800)
                {
                    sub_44E969();
                }
            }
        }
        GameD_update_helper_44F28B();
        GameD_Update_helper(buttons);
        return;
    }
    
    if (script_cancel_non_zero_dword_7227A0 & 0x80)
    {
        counter_dword_6BED20 = 0x7FFF0000;
    }

    Actor_KillActorsAtLevel_40A23D(4);
    gActorPauseFlags_dword_791A0C = gActorPauseFlags_dword_791A0C & 0xF7;
    Map_FreeKmds_44F3B5();
    StreamActorStop_4583BB();
    pGameD->gamed_unk_722784 = 3;
    game_state_dword_72279C.flags |= 0x104A6000u;
}
MGS_FUNC_IMPLEX(0x0044E381, GameD_update_44E381, ACTOR_GAMED_IMPL);

void* CC ResidentTopAllocate_40B379(int size)
{
    const int alignedSize = RoundUpPowerOf2(size, 4);
    gResidentTop_dword_78E960 -= alignedSize; // Moving BACKWARDS into gResidentHeap_81001F
    return gResidentTop_dword_78E960;
}
MGS_FUNC_IMPLEX(0x0040B379, ResidentTopAllocate_40B379, ACTOR_GAMED_IMPL);

void CC Stage_GetNameHashStack_44EAED()
{
    memset(gStageHashStack_7227C8, 0, 0x10u);
}
MGS_FUNC_IMPLEX(0x0044EAED, Stage_GetNameHashStack_44EAED, ACTOR_GAMED_IMPL);

void CC Stage_CopyNameHashStack_44EAFF(void* pDst)
{
    memcpy(pDst, gStageHashStack_7227C8, 0x10u);
}
MGS_FUNC_IMPLEX(0x44EAFF, Stage_CopyNameHashStack_44EAFF, ACTOR_GAMED_IMPL);

void CC Stage_SetNameHashStack_44EB13(const void* pSrc)
{
    memcpy(gStageHashStack_7227C8, pSrc, 0x10u);
}
MGS_FUNC_IMPLEX(0x44EB13, Stage_SetNameHashStack_44EB13, ACTOR_GAMED_IMPL);

signed int CC Stage_NameHashIndex_44EB66(int toFind)
{
    signed int i = 0;
    for (i = 1; i < 8; i++)
    {
        if (gStageHashStack_7227C8[i] == toFind)
        {
            return i;
        }
    }
    return i;
}
MGS_FUNC_IMPLEX(0x0044EB66, Stage_NameHashIndex_44EB66, ACTOR_GAMED_IMPL);

WORD CC GetStageNameHashed_44EAE5()
{
    return gCurrentStageNameHashed_7227D8;
}
MGS_FUNC_IMPLEX(0x44EAE5, GetStageNameHashed_44EAE5, ACTOR_GAMED_IMPL);

const char* CC File_StageName_44EB83()
{
    return &gStageName_7227DC[0];
}
MGS_FUNC_IMPLEX(0x44EB83, File_StageName_44EB83, ACTOR_GAMED_IMPL);

void CC Stage_LoadRelated_44EB27(WORD stageNameHashed, const char* pStageName)
{
    LOG_INFO("Set stage name: " << pStageName);

    gCurrentStageNameHashed_7227D8 = stageNameHashed;
    strcpy(&gStageName_7227DC[0], pStageName);
    // Move everything "back" by 1 element
    
    WORD* pLastItem = &gStageHashStack_7227C8[7];
    do
    {
        *pLastItem = *(pLastItem - 1);
        --pLastItem;
    } while (pLastItem > &gStageHashStack_7227C8[0]);
    

    // Now overwrite the first element with the current stage hash
    gStageHashStack_7227C8[0] = gCurrentStageNameHashed_7227D8;
}
MGS_FUNC_IMPLEX(0x0044EB27, Stage_LoadRelated_44EB27, ACTOR_GAMED_IMPL);

void CC sub_44E1E0()
{
    gGameStates_78E7E0.gLoadItemFuncIdx_word_78E7FC = -1;
    gGameStates_78E7E0.gMenu_Selected_item_idx_word_78E7FE = -1;
}
MGS_FUNC_IMPLEX(0x0044E1E0, sub_44E1E0, ACTOR_GAMED_IMPL);

void CC SaveResidentTop_40B36E()
{
    gSavedTop_78E964 = gResidentTop_dword_78E960;
}
MGS_FUNC_IMPLEX(0x0040B36E, SaveResidentTop_40B36E, ACTOR_GAMED_IMPL);

MGS_VAR(1, 0x6893D4, DWORD, dword_6893D4, 0);

void CC Create_loader_44E226()
{
    static char sNoFolderName_669A88[13] = "nofoldername"; // 0x669A88
    const char* strStageName = "init";
    if (gGameStates_78E7E0.stage_name_hash_word_78E7EC)
    {
        strStageName = File_StageName_44EB83();
    }

    if (strcmp(strStageName, sNoFolderName_669A88)) // TODO: FIX MEReal game bug? this attempts to overwrite global data?
    {
        dword_6893D4 = 1;
        strcpy(sNoFolderName_669A88, strStageName);
    }
    Res_loader_Create_457BDD(strStageName);
}
MGS_FUNC_IMPLEX(0x0044E226, Create_loader_44E226, ACTOR_GAMED_IMPL);

void CC LibDG_Clean_Texture_Cache_401110()
{
    LibDG_ClearTexturesCache_402487();
    LibDG_Restore_Textures_From_Resident_Memory_40274C();
}
MGS_FUNC_IMPLEX(0x00401110, LibDG_Clean_Texture_Cache_401110, ACTOR_GAMED_IMPL);

MGS_VAR(1, 0x81001F, ResidentHeap, gResidentHeap_81001F, {});
MGS_ARY(1, 0x8A8E20, BYTE, 438272, gSystem2_memory_unk_8A8E20, {});
MGS_ARY(1, 0x913E20, BYTE, 192512, gSystem0_memory_unk_913E20, {});
MGS_ARY(1, 0x942E20, BYTE, 192512, gSystem1_memory_unk_942E20, {});

void CC System_Init_0_And_1_40A465()
{
    System_init_40AC6C(0, 1, &gSystem0_memory_unk_913E20[0], 192512);
    System_init_40AC6C(1, 1, &gSystem1_memory_unk_942E20[0], 192512);
}
MGS_FUNC_IMPLEX(0x0040A465, System_Init_0_And_1_40A465, ACTOR_GAMED_IMPL);

void CC LibGV_40A4BB()
{
    LibGV_Restore_File_Cache_Records_From_Resident_Memory_40A72A();
    System_DeInit_Systems_0_to_2_sub_40AC52();
    System_Init_0_And_1_40A465();
    System_init_40AC6C(2, 0, &gSystem2_memory_unk_8A8E20[0], 438272);
    printf("RESIDENT TOP %X\n", gResidentTop_dword_78E960);
}
MGS_FUNC_IMPLEX(0x0040A4BB, LibGV_40A4BB, ACTOR_GAMED_IMPL);

void CC Init_Gamed_Create_44E12B()
{
    gTotalFrameTime_dword_995344 = 0;
    gGameOverTimer_dword_7227A4 = 0;
    script_cancel_non_zero_dword_7227A0 = 0;
    gLoaderState_dword_9942B8 = 0;
    Res_MenuMan_create_459A9A();
    Stage_GetNameHashStack_44EAED();
    LibDG_SetActiveResourceInitFuncPtrs_457B5B();
    Script_BindInits_452610();
    LibGV_Set_FileExtHandler_40A68D('b', GV_bin_file_handler_44E9D2); // Handles loading ".bin" files which sets loadable objects table
    sub_44E1E0();
    Actor_PushBack_40A2AF(1, &gGameD_stru_722760.mBase, nullptr);
    Actor_Init_40A347(&gGameD_stru_722760.mBase, reinterpret_cast<TActorFunction>(GameD_update_44E381), nullptr, "C:\\mgs\\source\\Game\\gamed.c");
    Init_Menu_GV_DG_44E1F9();
    sub_44E287();
    Reset_GV_DG_44E212();
    gGameStates_78E7E0.gStartingCdId_78E7E8 = static_cast<WORD>(gCdId_78D7B0 + 1);
    gpActiveButtons_995324 = &gButtonsArray4_7919C0[0];
    SaveResidentTop_40B36E();
    gGameD_stru_722760.gamed_unk_722780 = 0;
    gGameD_stru_722760.gamed_unk_722784 = 0;

    Create_loader_44E226();
   // AddDebugActor();
}
MGS_FUNC_IMPLEX(0x44E12B, Init_Gamed_Create_44E12B, ACTOR_GAMED_IMPL);
