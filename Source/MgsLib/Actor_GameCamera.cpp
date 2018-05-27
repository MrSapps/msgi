#include "stdafx.h"
#include "Actor_GameCamera.hpp"
#include "Actor.hpp"
#include "Psx.hpp"

#define GAME_CAMERA_IMPL true

struct struct_game_camera_data_1
{
    SVECTOR field_0_vec;
    SVECTOR field_8_vec;
    SVECTOR field_10_vec;
    int field_18_track;
    SVECTOR field_1C_vec;
    signed int field_24_ticks;
    SVECTOR field_28_vec;
};
MGS_ASSERT_SIZEOF(struct_game_camera_data_1, 0x30);


MGS_VAR(1, 0x724864, int, gCamera_Bits_dword_724864, 0);
MGS_VAR(1, 0x993FFC, int, gCamera_Track_mery172_dword_993FFC, 0);
MGS_VAR(1, 0x993F78, int, gCamera_Track_dword_993F78, 0);
MGS_VAR(1, 0x993FD0, int, gCamera_dword_993FD0, 0);
MGS_VAR(1, 0x993FA0, struct_game_camera_data_1, gCamera_unk_993FA0, {});
MGS_VAR(1, 0x994002, short, gCamera_word_994002, 0);
MGS_VAR(1, 0x724898, int, gCamera_dword_724898, 0);
MGS_VAR(1, 0x993FF8, int, gCameraFlags_dword_993FF8, 0);
MGS_VAR(1, 0x72489C, int, gCamera_dword_72489C, 0);
MGS_VAR(1, 0x99400C, SVECTOR, gCamera_99400C, {});
MGS_VAR(1, 0x994000, short, gCamera_zoom_level_word_994000, 0);
MGS_VAR(1, 0x993F7C, int, gCamera_zoom_copy_dword_993F7C, 0);
MGS_VAR(1, 0x66B718, int, gCamera_Track_dword_66B718, 10000);
const SVECTOR gCamera_66B720_value = { 640, 2048, 0, 0 };
MGS_VAR(1, 0x66B720, SVECTOR, gCamera_66B720, gCamera_66B720_value);



void CC Res_game_camera_update_453BC2(Actor* /*pActor*/);
MGS_FUNC_IMPLEX(0x453BC2, Res_game_camera_update_453BC2, GAME_CAMERA_IMPL);
void CC Res_game_camera_update_453BC2(Actor* pActor)
{
    Res_game_camera_update_453BC2_.Ptr()(pActor);
}

void CC Res_game_camera_create_453B11()
{
    Actor* pCamera = Actor_ResourceAlloc_40A30C(2, sizeof(Actor));
    if (pCamera)
    {
        Actor_Init_40A347(pCamera,
            Res_game_camera_update_453BC2,
            nullptr, 
            "C:\\mgs\\source\\Game\\camera.c");
    }

    gCamera_Bits_dword_724864 = 0;
    gCamera_Track_mery172_dword_993FFC = 10000;
    gCamera_Track_dword_993F78 = 10000;

    // Which camera from the HZD map is in use
    gCamera_dword_993FD0 = 0;

    // Distance to follow subject at
    gCamera_unk_993FA0.field_18_track = gCamera_Track_dword_66B718;

    // 3rd person camera pos?
    // x = up/down bound to the room
    // y = left/right bound to the room
    // z = no effect
    gCamera_unk_993FA0.field_10_vec = gCamera_66B720;

    // Causes over the shoulder camera enabled ?
    gCamera_word_994002 = 0;

    // Ditto but further away over the shoulder camera?
    gCamera_dword_724898 = 0;

    gCameraFlags_dword_993FF8 = 0;
    gCamera_dword_72489C = 0;
    gCamera_99400C.field_6_padding = 0;

    // FPS camera pos?
    // x = up/down
    // y = left/right
    // z = no effect
    gCamera_unk_993FA0.field_28_vec.field_0_x = 0;
    gCamera_unk_993FA0.field_28_vec.field_4_z = 0;
    gCamera_unk_993FA0.field_28_vec.field_2_y = 2048;

    // "Normal" zoom level
    gCamera_zoom_level_word_994000 = 320;

    // Zoom level for camera when menu is open?
    gCamera_zoom_copy_dword_993F7C = 320;
}
MGS_FUNC_IMPLEX(0x453B11, Res_game_camera_create_453B11, GAME_CAMERA_IMPL);
