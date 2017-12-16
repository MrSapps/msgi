#include "stdafx.h"
#include "LibDG.hpp"
#include "Script.hpp"
#include "LibGV.hpp"
#include "Actor_GameD.hpp"
#include <assert.h>
#include "Renderer.hpp"
#include "Actor_Loader.hpp"
#include "Fs.hpp"

#define LIBDG_IMPL true

void LibDGCpp_ForceLink() { }

#define REDIRECT_LIBDG_DATA 1

MGS_VAR_EXTERN(int, gActiveBuffer_dword_791A08);


MGS_FUNC_NOT_IMPL(0x62130E, Actor *__cdecl(int a1, signed int a2), Res_env_test_create_62130E);

struct Actor_Open
{
    Actor mBase;
};

Actor_Open* CC Res_open_create_486BD4(int a2, int a3) // DWORD scriptData, int scriptBinds, BYTE* pScript
{
    return (Actor_Open*)Res_env_test_create_62130E(a2, a3);
}
MGS_FUNC_IMPLEX(0x486BD4, Res_open_create_486BD4, false); // TODO


MGS_ARY(REDIRECT_LIBDG_DATA, 0x669AE0, Res_Init_Record, 512, gKnownResInitFuncs_669AE0,
{
    { 0x002A, 0, (ResInitFn)0x00519A61 }, /* Res_anime_pre_create_519A61 */              { 0x002F, 0, (ResInitFn)0x00519A9D }, /* Res_anime_pre_create_519A9D */
    { 0x0030, 0, (ResInitFn)0x00600135 }, /* Res_blst_ln_600135 */                       { 0x0009, 0, (ResInitFn)0x005FFC1C }, /* Res_blood_5FFC1C */
    { 0x000D, 0, (ResInitFn)0x005FF35B }, /* Res_d_bloods_5FF35B */                      { 0x0048, 0, (ResInitFn)0x005C5666 }, /* Res_blooddrp_create_5C5666 */
    { 0x0047, 0, (ResInitFn)0x005C5111 }, /* Res_bloodhzd_create_5C5111 */               { 0x000A, 0, (ResInitFn)0x005FEFB9 }, /* Res_splash_create_5FEFB9 */
    { 0x0025, 0, (ResInitFn)0x005F7EE2 }, /* Ret0_5F7EE2 */                              { 0x004A, 0, (ResInitFn)0x005683EF }, /* Res_liquid_bombled_create_5683EF */
    { 0x000E, 0, (ResInitFn)0x0051B59B }, /* Res_anime_create_breath */                  { 0x0012, 0, (ResInitFn)0x005FECC3 }, /* Res_bubble_t_create_5FECC3 */
    { 0x0013, 0, (ResInitFn)0x005FE85B }, /* Res_bubble_p_create_5FE85B */               { 0x000B, 0, (ResInitFn)0x005FE69B }, /* Res_bullet_create_5FE69B */
    { 0x0033, 0, (ResInitFn)0x0051B1CF }, /* Res_anime_pre_create_51B1CF */              { 0x003A, 0, (ResInitFn)0x005C4C4E }, /* Res_celofan2_create_5C4C4E */
    { 0x0040, 0, (ResInitFn)0x005C445E }, /* Res_celofan2_create_5C445E */               { 0x0039, 0, (ResInitFn)0x005FD769 }, /* Res_crsh_smk_create_5FD769 */
    { 0x0034, 0, (ResInitFn)0x0051B051 }, /* Res_anime_pre_create_51B051 */              { 0x0015, 0, (ResInitFn)0x00628ECC }, /* Res_goggle_create_628ECC */
    { 0x0016, 0, (ResInitFn)0x005BB2D5 }, /* Res_Equip_gglsight_create_5BB2D5 */         { 0x0037, 0, (ResInitFn)0x005DF33A }, /* Res_red_alr_create_5DF33A */
    { 0x0038, 0, (ResInitFn)0x005DF50D },                                                { 0x001C, 0, (ResInitFn)0x00628A91 }, /* Res_envmap3_create_628A91 */
    { 0x0001, 0, (ResInitFn)0x0043EC38 }, /* Res_blast_create_43EC38 */                  { 0x0003, 0, (ResInitFn)0x00441CF2 }, /* Res_fadeio_create_441CF2 */
    { 0x0004, 0, (ResInitFn)0x00441CF2 }, /* Res_fadeio_create_441CF2 */                 { 0x0045, 0, (ResInitFn)0x005C41C3 }, /* Res_famaslit_create_5C41C3 */
    { 0x0010, 0, (ResInitFn)0x005ABCB7 }, /* Res_demoasi_create_5ABCB7 */                { 0x0044, 0, (ResInitFn)0x005E7FC9 },
    { 0x0019, 0, (ResInitFn)0x0051AED6 }, /* Res_anime_pre_create_51AED6 */              { 0x0042, 0, (ResInitFn)0x0061FFFF }, /* Res_gas_efct_create_61FFFF */
    { 0x0022, 0, (ResInitFn)0x005BB23E }, /* Res_Equip_gmsight_create_5BB23E */          { 0x0046, 0, (ResInitFn)0x006288F0 }, /* Res_realshdw_create_6288F0 */
    { 0x003B, 0, (ResInitFn)0x005C3D78 }, /* Res_inverlt_create_5C3D78 */                { 0x003F, 0, (ResInitFn)0x005C3580 }, /* Res_inverlt2_create_5C3580 */
    { 0x0017, 0, (ResInitFn)0x0062862E }, /* Res_Takabe_goggleir_create_62862E */        { 0x0018, 0, (ResInitFn)0x005BB2D5 }, /* Res_Equip_gglsight_create_5BB2D5 */
    { 0x0020, 0, (ResInitFn)0x005C31B7 }, /* Res_Kojo_mgrexlzr_create_5C31B7 */          { 0x004B, 0, (ResInitFn)0x005FD23A }, /* Res_mg_room_create_5FD23A */
    { 0x000C, 0, (ResInitFn)0x0051A97A }, /* Res_anime_pre_create_51A97A */              { 0x0028, 0, (ResInitFn)0x0062852B }, /* Res_sepia_create_62852B */
    { 0x0011, 0, (ResInitFn)0x0051AD2A }, /* Res_anime_pre_create_51AD2A */              { 0x0049, 0, (ResInitFn)0x00627B86 }, /* Res_windcrcl_create_627B86 */
    { 0x0041, 0, (ResInitFn)0x005C2454 }, /* Res_ninjalzr_create_5C2454 */               { 0x002D, 0, (ResInitFn)0x005ABC28 }, /* Res_katana_create_5ABC28 */
    { 0x0035, 0, (ResInitFn)0x0051AB83 }, /* Res_anime_pre_create_51AB83 */              { 0x0036, 0, (ResInitFn)0x0051AC56 }, /* Res_anime_pre_create_51AC56 */
    { 0x001A, 0, (ResInitFn)0x005BAC0B }, /* Res_Equip_kogaku2_create_5BAC0B */          { 0x001B, 0, (ResInitFn)0x005BB0B2 }, /* Res_kogaku2_create_5BB0B2 */
    { 0x001D, 0, (ResInitFn)0x005E1D68 }, /* Res_plasma_create_5E1D68 */                 { 0x0024, 0, (ResInitFn)0x005E1E8B }, /* Res_plasma_create_5E1E8B */
    { 0x0007, 0, (ResInitFn)0x00519E54 }, /* Res_anime_pre_create_519E54 */              { 0x0029, 0, (ResInitFn)0x0063F7DF }, /* Res_rfsight_create_63F7DF */
    { 0x0014, 0, (ResInitFn)0x005B9F52 }, /* Res_Equip_scope_create_5B9F52 */            { 0x001F, 0, (ResInitFn)0x006282D3 }, /* Res_sepia_6282D3 */
    { 0x000F, 0, (ResInitFn)0x005AB3EA }, /* Res_Enemy_demokage_create_5AB3EA */         { 0x0032, 0, (ResInitFn)0x005C22AC }, /* Res_Kojo_m1e1smke_create_5C22AC */
    { 0x002C, 0, (ResInitFn)0x004438CF }, /* Res_sight_create_4438CF */                  { 0x0008, 0, (ResInitFn)0x00519DEC }, /* Res_anime_pre_create_519DEC */
    { 0x0002, 0, (ResInitFn)0x00519A25 }, /* Res_anime_pre_create_519A25 */              { 0x0031, 0, (ResInitFn)0x005FD006 }, /* Res_smke_ln_create_5FD006 */
    { 0x003C, 0, (ResInitFn)0x00627ECA }, /* Res_spark2_create_627ECA */                 { 0x003D, 0, (ResInitFn)0x005FCA8F }, /* Res_spark_create_5FCA8F */
    { 0x003E, 0, (ResInitFn)0x00519504 }, /* Res_anime_pre_create_519504 */              { 0x0026, 0, (ResInitFn)0x005197BC }, /* Res_anime_pre_create_5197BC */
    { 0x0027, 0, (ResInitFn)0x00519838 }, /* Res_anime_pre_create_519838 */              { 0x002E, 0, (ResInitFn)0x005FBEE0 }, /* Res_sub_room_create_5FBEE0 */
    { 0x0043, 0, (ResInitFn)0x00627C02 }, /* Res_sub_efct_create_627C02 */               { 0x0006, 0, (ResInitFn)0x00519E54 }, /* Res_anime_pre_create_519E54 */
    { 0x0005, 0, (ResInitFn)0x006154A7 }, /* Res_telop_create_6154A7 */                  { 0x004C, 0, (ResInitFn)0x006153CF }, /* Res_telop_create_6153CF */
    { 0x0021, 0, (ResInitFn)0x00627BFF }, /* Ret0_627BFF */                              { 0x0023, 0, (ResInitFn)0x005FF35B }, /* Res_d_bloods_create_5FF35B */
    { 0x002B, 0, (ResInitFn)0x005FF35B }, /* Res_d_bloods_create_5FF35B */               { 0x001E, 0, (ResInitFn)0x00627715 }, /* Res_windcrcl_create_627715 */
    { 0x40E9, 0, (ResInitFn)0x005FBB0A }, /* Res_10a_demo_create_5FBB0A */               { 0xE068, 0, (ResInitFn)0x005FAF3E }, /* Res_s11_objs_create_5FAF3E */
    { 0xD53C, 0, (ResInitFn)0x005FA06A }, /* Res_11b_demo_create_5FA06A */               { 0x153E, 0, (ResInitFn)0x005F9170 }, /* Res_11g_demo_create_5F9170 */
    { 0x9FFD, 0, (ResInitFn)0x00457B50 },                                                { 0x2D0A, 0, (ResInitFn)0x00457AC1 }, /* Res_second_457AC1 */
    { 0x566F, 0, (ResInitFn)0x004B3B01 }, /* Res_abst_create_4B3B01 */                   { 0x4974, 0, (ResInitFn)0x004B0578 }, /* Res_ab_demo1_create_4B0578 */
    { 0x4975, 0, (ResInitFn)0x004AC173 }, /* Res_ab_demo2_create_4AC173 */               { 0x05EF, 0, (ResInitFn)0x005F8FFA }, /* Res_a_shower_create_5F8FFA */
    { 0x2A90, 0, (ResInitFn)0x00627435 }, /* Res_gclarray_create_627435 */               { 0x02C4, 0, (ResInitFn)0x005AB396 }, /* Res_asiato_create_5AB396 */
    { 0x92BC, 0, (ResInitFn)0x005AA507 }, /* Res_asioto_create_5AA507 */                 { 0x9988, 0, (ResInitFn)0x004ABBC9 }, /* Res_at_create_4ABBC9 */
    { 0xF59E, 0, (ResInitFn)0x00514A19 }, /* Res_belong_create_514A19 */                 { 0x6B6C, 0, (ResInitFn)0x005682F7 }, /* Res_b_smoke_create_5682F7 */
    { 0x8185, 0, (ResInitFn)0x005F8E18 }, /* Res_blink_tx_create_5F8E18 */               { 0x1158, 0, (ResInitFn)0x005F8573 }, /* Res_mel10_create_5F8573 */
    { 0x4E95, 0, (ResInitFn)0x005F820B }, /* Res_blood_cl_create_5F820B */               { 0x6A4C, 0, (ResInitFn)0x005F7EE8 }, /* Res_blood_bl_create_5F7EE8 */
    { 0x9093, 0, (ResInitFn)0x005F7EE5 }, /* Ret0_5F7EE5 */                              { 0xF74B, 0, (ResInitFn)0x005142ED }, /* Res_boxall_5142ED */
    { 0x32FC, 0, (ResInitFn)0x006270F3 }, /* Res_breakobj_create_6270F3 */               { 0x6471, 0, (ResInitFn)0x00626FE4 }, /* Res_btn_chk_create_626FE4 */
    { 0x1A02, 0, (ResInitFn)0x005F781C }, /* Res_bubble_s_create_5F781C */               { 0x93B6, 0, (ResInitFn)0x004A2B6B }, /* Res_b_select_create_4A2B6B */
    { 0x7ECA, 0, (ResInitFn)0x0049FC71 }, /* Res_wire_create_49FC71 */                   { 0x6E90, 0, (ResInitFn)0x005A9076 }, /* Res_camera_5A9076 */
    { 0x56CC, 0, (ResInitFn)0x005A816F }, /* Res_camera2_create_5A816F */                { 0xA9C5, 0, (ResInitFn)0x005F5F72 }, /* Res_guncame_create_5F5F72 */
    { 0x7BC2, 0, (ResInitFn)0x00626EA8 }, /* Res_camshake_create_626EA8 */               { 0xB99F, 0, (ResInitFn)0x0049EA8B }, /* Res_cape_create_49EA8B */
    { 0x51C6, 0, (ResInitFn)0x00626C7D }, /* Res_cat_in_create_626C7D */                 { 0xF722, 0, (ResInitFn)0x0046BC36 }, /* Res_change_46BC36 */
    { 0x43A0, 0, (ResInitFn)0x006262A0 }, /* Res_ir_cens_create_6262A0 */                { 0x788D, 0, (ResInitFn)0x00625DC1 }, /* Res_chair_create_625DC1 */
    { 0x7A05, 0, (ResInitFn)0x00625D63 }, /* Res_cinema_create_625D63 */                 { 0x3C0C, 0, (ResInitFn)0x005A04D6 }, /* Res_jirai_5A04D6 */
    { 0xC6D7, 0, (ResInitFn)0x005A7099 }, /* Res_command_create_5A7099 */                { 0xCC45, 0, (ResInitFn)0x005F2E0F }, /* Res_dyncon_create_5F2E0F */
    { 0x22C6, 0, (ResInitFn)0x005A6143 }, /* Res_timer_create_5A6143 */                  { 0xECED, 0, (ResInitFn)0x004629FE },
    { 0xA3FB, 0, (ResInitFn)0x005F1A1D }, /* Res_crane_create_5F1A1D */                  { 0x8E60, 0, (ResInitFn)0x005F0B54 }, /* Res_crow_create_5F0B54 */
    { 0xBE79, 0, (ResInitFn)0x005F07A4 }, /* Res_death_sp_create_5F07A4 */               { 0x6A98, 0, (ResInitFn)0x00513F69 }, /* Res_dsmoke_513F69 */
    { 0x76BC, 0, (ResInitFn)0x0051368C }, /* Res_dsmoke2_51368C */                       { 0xB4E6, 0, (ResInitFn)0x00457974 }, /* Res_cancel_457974 */
    { 0xE97E, 0, (ResInitFn)0x0056781A }, /* Res_doll_create_56781A */                   { 0x3686, 0, (ResInitFn)0x0049B6AD }, /* Res_demosel_create_49B6AD */
    { 0x9F7D, 0, (ResInitFn)0x00511789 }, /* Res_display_create_511789 */                { 0x9D00, 0, (ResInitFn)0x00624FDF }, /* Res_dummy_fl_create_624FDF */
    { 0x58F0, 0, (ResInitFn)0x00624A97 }, /* Res_dummy_wl_624A97 */                      { 0x6C0E, 0, (ResInitFn)0x005EA179 }, /* Res_dog_create_5EA179 */
    { 0xB997, 0, (ResInitFn)0x0040BB8B }, /* Res_door_create_40BB8B */                   { 0x73F8, 0, (ResInitFn)0x00624208 }, /* Res_door2_create_624208 */
    { 0xB58D, 0, (ResInitFn)0x00623B32 }, /* Res_drum_create_623B32 */                   { 0x4BE8, 0, (ResInitFn)0x00623714 }, /* Res_drum2_create_623714 */
    { 0x3303, 0, (ResInitFn)0x005E9A03 }, /* Res_ductmous_create_5E9A03 */               { 0xAF6C, 0, (ResInitFn)0x0062345B }, /* Res_dymc_flr_create_62345B */
    { 0xB103, 0, (ResInitFn)0x0062330F }, /* Res_dymc_seg_create_62330F */               { 0xE75A, 0, (ResInitFn)0x0062305A }, /* Res_ed_telop_create_62305A */
    { 0x107C, 0, (ResInitFn)0x00622EEF }, /* Res_elc_damg_create_622EEF */               { 0xAEF2, 0, (ResInitFn)0x00622D21 }, /* Res_elc_flr_create_622D21 */
    { 0x2ABC, 0, (ResInitFn)0x00621BDD }, /* Res_elevator_create_621BDD */               { 0x9AB9, 0, (ResInitFn)0x005E80FE }, /* Res_valcan_ele_crow_create_5E80FE */
    { 0xE253, 0, (ResInitFn)0x00456960 }, /* Res_evpanel_create_456960 */                { 0x32E5, 0, (ResInitFn)0x0063F60E }, /* Res_emitter_create_63F60E */
    { 0xA9DD, 0, (ResInitFn)0x0063F43D }, /* Res_emitter2_create_63F43D */               { 0xB789, 0, (ResInitFn)0x0062190F }, /* Res_ending2_create_62190F */
    { 0x3F9A, 0, (ResInitFn)0x00621797 }, /* Res_env_snd_create_621797 */                { 0x76FE, 0, (ResInitFn)0x0062130E }, /* Res_env_test_create_62130E */
    { 0xA12E, 0, (ResInitFn)0x0044200E }, /* Res_fadeio_create_44200E */                 { 0xC73E, 0, (ResInitFn)0x005E7FCF }, /* Res_fall_spl_create_5E7FCF */
    { 0x5D64, 0, (ResInitFn)0x00620DA7 }, /* Res_o2_damge_create_620DA7 */               { 0x118C, 0, (ResInitFn)0x00620978 }, /* Res_findtrap_create_620978 */
    { 0xD6FB, 0, (ResInitFn)0x00620833 }, /* Res_fog_create_620833 */                    { 0x84E1, 0, (ResInitFn)0x005115AE }, /* Res_fonttext_5115AE */
    { 0x0FAD, 0, (ResInitFn)0x00511497 }, /* Res_motse_511497 */                         { 0xADD8, 0, (ResInitFn)0x0062052F }, /* Res_furnace_create_62052F */
    { 0x8D5A, 0, (ResInitFn)0x006203E0 }, /* Res_gasdamge_create_6203E0 */               { 0x5A50, 0, (ResInitFn)0x0061FE1B }, /* Res_gas_efct_create_61FE1B */
    { 0x2477, 0, (ResInitFn)0x0061F9A8 }, /* Res_jeep_emy_create_61F9A8 */               { 0xCCD3, 0, (ResInitFn)0x005E7FCC },
    { 0x8E70, 0, (ResInitFn)0x0061F2D5 }, /* Res_glass_create_61F2D5 */                  { 0xDB1F, 0, (ResInitFn)0x004DAA01 }, /* Res_goal_create_4DAA01 */
    { 0x9EB7, 0, (ResInitFn)0x00567203 }, /* Res_godzcom_create_567203 */                { 0xCB1F, 0, (ResInitFn)0x0056653D }, /* Res_godzila_56653D */
    { 0x5F5A, 0, (ResInitFn)0x0063F0EF }, /* Res_grdcam_63F0EF */                        { 0xCBEB, 0, (ResInitFn)0x005E76AA }, /* Res_bg_hex_create_5E76AA */
    { 0x0D9B, 0, (ResInitFn)0x005E6DD7 }, /* Res_bg_hex_s_loader_5E6DD7 */               { 0x42E4, 0, (ResInitFn)0x005E6A41 }, /* Res_hiyoko_create_5E6A41 */
    { 0xDD8B, 0, (ResInitFn)0x00511349 }, /* Res_intr_cam_511349 */                      { 0x8767, 0, (ResInitFn)0x0044D086 }, /* Res_item_44D086 */
    { 0x6A48, 0, (ResInitFn)0x0061E1DD }, /* Res_jeep_srl_create_61E1DD */               { 0xA27E, 0, (ResInitFn)0x0061DDFA }, /* Res_jdrum_create_61DDFA */
    { 0xB757, 0, (ResInitFn)0x00462532 },                                                { 0x1EF9, 0, (ResInitFn)0x0050C653 }, /* Res_johnny_create_50C653 */
    { 0x81C7, 0, (ResInitFn)0x0049B678 }, /* Res_TitleStageLoadRelated_49B678 */         { 0x4F02, 0, (ResInitFn)0x005B9F42 },
    { 0xC6AC, 0, (ResInitFn)0x005E66A1 }, /* Res_key_item_create_5E66A1 */               { 0x52BF, 0, (ResInitFn)0x005A60A2 }, /* Res_kiken_5A60A2 */
    { 0xA6F6, 0, (ResInitFn)0x004421C2 }, /* Res_Cache_Kmd_4421C2 */                     { 0x8B1A, 0, (ResInitFn)0x0050BF9B }, /* Res_landing_create_50BF9B */
    { 0x711F, 0, (ResInitFn)0x0061DC48 }, /* Res_life_up_61DC48 */                       { 0x425F, 0, (ResInitFn)0x0061D52C }, /* Res_lift_create_61D52C */
    { 0x921B, 0, (ResInitFn)0x0061CCCC }, /* Res_lift2_create_61CCCC */                  { 0x7BF2, 0, (ResInitFn)0x005652F4 }, /* Res_liquid_create_5652F4 */
    { 0xCF72, 0, (ResInitFn)0x0049AD6B }, /* Res_CdSwap_create_49AD6B */                 { 0x53C7, 0, (ResInitFn)0x004606E2 },
    { 0x9D71, 0, (ResInitFn)0x005C0C8B }, /* Res_m1e1_create_5C0C8B */                   { 0xD5EC, 0, (ResInitFn)0x005BFEA1 }, /* Res_m1e1came_create_5BFEA1 */
    { 0xE448, 0, (ResInitFn)0x0061B8B9 }, /* Res_doll2_61B8B9 */                         { 0x450F, 0, (ResInitFn)0x0050A2E4 }, /* Res_hair_m_create_50A2E4 */
    { 0xAF72, 0, (ResInitFn)0x00509679 }, /* Res_mempsy_create_509679 */                 { 0xC755, 0, (ResInitFn)0x00565013 }, /* Res_meryl3_565013 */
    { 0x5078, 0, (ResInitFn)0x005A53CA }, /* Res_meryl7_create_5A53CA */                 { 0xE271, 0, (ResInitFn)0x005644F4 }, /* Res_meryl72_create_5644F4 */
    { 0x4754, 0, (ResInitFn)0x005BE3F8 }, /* Res_mgrex_create_5BE3F8 */                  { 0x954B, 0, (ResInitFn)0x0061B32D }, /* Res_mg_demo1_create_61B32D */
    { 0x1787, 0, (ResInitFn)0x005BD2D4 }, /* Res_hind_create_5BD2D4 */                   { 0xC218, 0, (ResInitFn)0x0061AB21 }, /* Res_mirror_create_61AB21 */
    { 0xA2B5, 0, (ResInitFn)0x0045692F },                                                { 0x7ACF, 0, (ResInitFn)0x005E61CC }, /* Res_model_dt_create_5E61CC */
    { 0x6D78, 0, (ResInitFn)0x0061A71C }, /* Res_monitor1_create_61A71C */               { 0x0065, 0, (ResInitFn)0x0061A683 }, /* Res_mosaic_create_61A683 */
    { 0xD4A5, 0, (ResInitFn)0x005E52D5 }, /* Res_mouse_create_5E52D5 */                  { 0x3453, 0, (ResInitFn)0x00456860 }, /* Res_movie_456860 */
    { 0x5345, 0, (ResInitFn)0x004561C6 }, /* Res_movie_4561C6 */                         { 0xB98C, 0, (ResInitFn)0x005A4F47 }, /* Res_m_door_5A4F47 */
    { 0xD3C0, 0, (ResInitFn)0x0050964C },                                                { 0xF002, 0, (ResInitFn)0x00508856 }, /* Res_hair_n_create_508856 */
    { 0x30BA, 0, (ResInitFn)0x00563A09 }, /* Res_ninja_create_563A0 */                   { 0x4811, 0, (ResInitFn)0x005A473D }, /* Res_object_5A473D */
    { 0xCF79, 0, (ResInitFn)Res_open_create_486BD4 },                                    { 0x3AC3, 0, (ResInitFn)0x00482D1F }, /* Res_opena_create_482D1F */
    { 0xCFEF, 0, (ResInitFn)0x0047EE6E }, /* Res_openp_47EE6E */                         { 0x976C, 0, (ResInitFn)0x0047A5E5 }, /* Res_opt_create_47A5E5 */
    { 0x8D31, 0, (ResInitFn)0x004792A6 }, /* Res_opta_create_4792A6 */                   { 0xB916, 0, (ResInitFn)0x00477F58 }, /* Res_optp_create_477F58 */
    { 0xCBF8, 0, (ResInitFn)0x00456093 }, /* Res_pad_create_456093 */                    { 0x3ED7, 0, (ResInitFn)0x0061A311 }, /* Res_pad_demo_create_61A311 */
    { 0x720D, 0, (ResInitFn)0x00539051 }, /* Res_paddemo2_539051 */                      { 0xFED1, 0, (ResInitFn)0x00455F32 }, /* Res_vibrate_create_455F32 */
    { 0x20F2, 0, (ResInitFn)0x00619C50 }, /* Res_panel_create_619C50 */                  { 0xDD95, 0, (ResInitFn)0x006198AB }, /* Res_panel2_create_6198AB */
    { 0x5F02, 0, (ResInitFn)0x006196C0 }, /* Res_paper_create_6196C0 */                  { 0x30CE, 0, (ResInitFn)0x005E40BB }, /* Res_pato_lmp_5E40BB */
    { 0xFDB6, 0, (ResInitFn)0x004612E8 }, /* Res_pocket_create_4612E8 */                 { 0x8FDC, 0, (ResInitFn)0x00462529 }, /* Res_pocket_create_1_462529 */
    { 0x6414, 0, (ResInitFn)0x00462395 }, /* Res_pocket_create_0_462395 */               { 0xAA13, 0, (ResInitFn)0x00460D44 },
    { 0xCF00, 0, (ResInitFn)0x00460D1C },                                                { 0x10FF, 0, (ResInitFn)0x00508209 }, /* Res_perapict_create_508209 */
    { 0xEDEB, 0, (ResInitFn)0x00505D37 }, /* Res_photosel_505D37 */                      { 0x8427, 0, (ResInitFn)0x005E3D51 }, /* Res_scn_anim_create_5E3D51 */
    { 0x17F5, 0, (ResInitFn)0x005E372A }, /* Res_bg_sp_create_5E372A */                  { 0x1FA5, 0, (ResInitFn)0x0043EEFB }, /* Res_bg_stage_43EEFB */
    { 0xB032, 0, (ResInitFn)0x005E316B }, /* Res_bg_st1_create_5E316B */                 { 0xB033, 0, (ResInitFn)0x005E2B67 }, /* Res_bg_st2_create_5E2B67 */
    { 0xB7AE, 0, (ResInitFn)0x005E262D }, /* Res_bg_sp_cm_create_5E262D */               { 0xBC76, 0, (ResInitFn)0x005E2053 }, /* Res_bg_sp_st_create_5E2053 */
    { 0x9C6C, 0, (ResInitFn)0x0061947E }, /* Res_pk_gate_create_61947E */                { 0xB47A, 0, (ResInitFn)0x0061870B }, /* Res_pjeep_create_61870B */
    { 0x9BC2, 0, (ResInitFn)0x005E1032 }, /* Res_plasma_create_5E1032 */                 { 0xFC0B, 0, (ResInitFn)0x005E0C21 }, /* Res_flr_spa_create_5E0C21 */
    { 0x3AD2, 0, (ResInitFn)0x00505B87 }, /* Res_Ret0_505B87 */                          { 0xCC89, 0, (ResInitFn)0x005E01D2 }, /* Res_bg_hex_p_5E01D2 */
    { 0x6D1B, 0, (ResInitFn)0x00476B30 }, /* Res_preope_create_476B30 */                 { 0xD8DD, 0, (ResInitFn)0x005054F4 }, /* Res_ninja_5054F4 */
    { 0xBF66, 0, (ResInitFn)0x00504B42 }, /* Res_otacom_504B42 */                        { 0xA404, 0, (ResInitFn)0x005045B7 }, /* Res_sne_03c_5045B7 */
    { 0x27A2, 0, (ResInitFn)0x0050492D }, /* Res_sne_03c_50492D */                       { 0xA76F, 0, (ResInitFn)0x004E8181 }, /* Res_psyco_4E8181 */
    { 0xF4B0, 0, (ResInitFn)0x004FFA7A }, /* Res_meryl07b_create_4FFA7A */               { 0x710D, 0, (ResInitFn)0x004FEEC7 }, /* Res_psyobj_create_4FEEC7 */
    { 0x91AA, 0, (ResInitFn)0x006183F2 }, /* Res_puthzd_create_6183F2 */                 { 0x169C, 0, (ResInitFn)0x005DF74E }, /* Res_p_lamp_create_5DF74E */
    { 0x5147, 0, (ResInitFn)0x00455E18 }, /* Res_point_455E18 */                         { 0x917B, 0, (ResInitFn)0x005DF51B }  /* Res_item_dot_create_5DF51B */
});

// An zeroed array initially as the name suggests it is dynamically populated at run time.
MGS_ARY(REDIRECT_LIBDG_DATA, 0x994320, Res_Init_Record, 512, gDynamicResFuncs_word_994320, {} );

MGS_ARY(REDIRECT_LIBDG_DATA, 0x650040, Res_Init_Record, 3, gStaticResInitFuncs_stru_650040,
{
    { 0x21CA, 0, (ResInitFn)0x004E1701 }, // Res_sna_init_create_4E1701
    { 0x8767, 0, (ResInitFn)0x0044D086 }, // Res_item_44D086
    { 0xB997, 0, (ResInitFn)0x0040BB8B }  // Res_door_create_40BB8B
});

MGS_PTR(REDIRECT_LIBDG_DATA, 0x993F44, Res_Init_Record*, gpToDynamicResInitFuncs_dword_993F44, nullptr);

MGS_VAR(REDIRECT_LIBDG_DATA, 0x994304, DWORD, gSleep5000_after_res_funcs_1_dword_994304, 0);
MGS_VAR(REDIRECT_LIBDG_DATA, 0x99430C, DWORD, gSleep5000_after_res_funcs_2_dword_99430C, 0);

MGS_VAR(REDIRECT_LIBDG_DATA, 0x6BB910, Actor, gLibDGD_1_stru_6BB910, {});
MGS_VAR(REDIRECT_LIBDG_DATA, 0x6BB930, LibDG_Struct, gLibDG_2_stru_6BB930, {});

MGS_VAR(REDIRECT_LIBDG_DATA, 0x722A40, DWORD, gBinds_dword_722A40, 0);
MGS_ARY(1, 0x6BED18, DWORD, 2, dword_6BED18, {});

MGS_VAR(1, 0x6BC180, struct_gv, gLibGvStruct0_6BC180, {}); // TODO: Probably an array of 3?
MGS_VAR(1, 0x6BC36C, struct_gv, gLibGVStruct1_6BC36C, {});
MGS_VAR(1, 0x6BC558, struct_gv, gLibGVStruct2_6BC558, {});

MGS_VAR(1, 0x78D32C, DWORD, dword_78D32C, 0);

ResInitFn CC Script_tbl_get_res_init_fn_457B9A(BYTE *pScript)
{
    const WORD hashedName = static_cast<WORD>( Script_Unknown8_409924(pScript));
    LOG_INFO("Looking for " << hashedName << " chara function");
    return LibDG_GetResourceInitFuncPtr_457BAC(hashedName);
}
MGS_FUNC_IMPLEX(0x00457B9A, Script_tbl_get_res_init_fn_457B9A, LIBDG_IMPL);

signed int CC Script_tbl_chara_451AC3(BYTE* pScript)
{
    BYTE* scriptRet = Script_GetReturnAddress();
    ResInitFn pResFn = Script_tbl_get_res_init_fn_457B9A(scriptRet);
    if (pResFn)
    {
        BYTE* scriptRet2 = Script_GetReturnAddress();
        DWORD scriptUnknown =  Script_Unknown8_409924(scriptRet2);
        LOG_INFO("Exec chara fn: " << pResFn);
        pResFn(scriptUnknown, gBinds_dword_722A40, pScript);
        return 0;
    }
    return -1;
}
MGS_FUNC_IMPLEX(0x00451AC3, Script_tbl_chara_451AC3, LIBDG_IMPL);

signed int CC GV_bin_file_handler_44E9D2(void* pData, int /*fileNameHash*/)
{
    Res_Init_Record* pStartingRecord = reinterpret_cast<Res_Init_Record*>(pData);
    int dst_idx = 0;
    for (;;)
    {
        if (!pStartingRecord->mInitFunctionPointer)
        {
            break;
        }

        int src_idx = 0;
        while (gKnownResInitFuncs_669AE0[src_idx].mHashedName)
        {
            if (gKnownResInitFuncs_669AE0[src_idx].mHashedName == pStartingRecord->mHashedName)
            {
                gDynamicResFuncs_word_994320[dst_idx].mHashedName = pStartingRecord->mHashedName;
                gDynamicResFuncs_word_994320[dst_idx].mInitFunctionPointer = gKnownResInitFuncs_669AE0[src_idx].mInitFunctionPointer;
                break;
            }
            src_idx++;
        }
        printf("CHARA %d, %p\n", gDynamicResFuncs_word_994320[dst_idx].mHashedName, gDynamicResFuncs_word_994320[dst_idx].mInitFunctionPointer);

        ++dst_idx;
        ++pStartingRecord;
    }

    // Set last entry to 0 to terminate
    gDynamicResFuncs_word_994320[dst_idx].mHashedName = 0;
    gDynamicResFuncs_word_994320[dst_idx].mInitFunctionPointer = nullptr;
    
    if (gSleep5000_after_res_funcs_1_dword_994304 || gSleep5000_after_res_funcs_2_dword_99430C)
    {
        ::Sleep(5000);
    }

    gSleep5000_after_res_funcs_1_dword_994304 = 0;
    gSleep5000_after_res_funcs_2_dword_99430C = 0;

    return 1;
}
MGS_FUNC_IMPLEX(0x0044E9D2, GV_bin_file_handler_44E9D2, LIBDG_IMPL);

void CC LibDG_SetActiveResourceInitFuncPtrs_457B5B()
{
    memcpy(gDynamicResFuncs_word_994320, gKnownResInitFuncs_669AE0, 4096u); // 4096u = sizeof(gResInitFuncs_669AE0)
    gpToDynamicResInitFuncs_dword_993F44 = gDynamicResFuncs_word_994320;
}
MGS_FUNC_IMPLEX(0x457B5B, LibDG_SetActiveResourceInitFuncPtrs_457B5B, LIBDG_IMPL);

void CC LibDG_ClearActiveResourceFunctionPointerList_457B7C()
{
    memset(gDynamicResFuncs_word_994320, 0, 4096u); // 4096u = sizeof(gResInitFuncs_669AE0)
    gpToDynamicResInitFuncs_dword_993F44 = gDynamicResFuncs_word_994320;
}
MGS_FUNC_IMPLEX(0x457B7C, LibDG_ClearActiveResourceFunctionPointerList_457B7C, LIBDG_IMPL);

static ResInitFn FindFnPtrByHash(Res_Init_Record* arrayToSearch, WORD hashName)
{
    Res_Init_Record* pRecord = arrayToSearch;
    while (pRecord->mInitFunctionPointer)
    {
        if (pRecord->mHashedName == hashName)
        {
            return pRecord->mInitFunctionPointer;
        }
        ++pRecord;
    }
    return nullptr;
}

ResInitFn CC LibDG_GetResourceInitFuncPtr_457BAC(WORD hashedName)
{
    // Search in the 3 "static" records
    ResInitFn result = FindFnPtrByHash(gStaticResInitFuncs_stru_650040, hashedName);
    if (result)
    {
        return result;
    }

    // Search in the 512 "dynamic" records
    result = FindFnPtrByHash(gpToDynamicResInitFuncs_dword_993F44, hashedName);
    return result;
}
MGS_FUNC_IMPLEX(0x457BAC, LibDG_GetResourceInitFuncPtr_457BAC, LIBDG_IMPL);

MGS_ARY(1, 0x6BCB44, DWORD, 33, ot_gv0_0_6BCB44, {});
MGS_ARY(1, 0x6BCBC8, DWORD, 289, ot_gv0_1_6BCBC8, {});
MGS_ARY(1, 0x6BD04C, DWORD, 257, ot_gv1_0_6BD04C, {});
MGS_ARY(1, 0x6BD450, DWORD, 513, ot_gv1_1_6BD450, {});
MGS_ARY(1, 0x6BDC54, DWORD, 2, ot_gv2_0_6BDC54, {});
MGS_ARY(1, 0x6BDC5C, DWORD, 259, ot_gv2_1_6BDC5C, {});

MGS_VAR(REDIRECT_LIBDG_DATA, 0x6BE068, DR_ENV, stru_6BE068, {});
MGS_VAR(REDIRECT_LIBDG_DATA, 0x6BE0A8, DR_ENV, pPacked_6BE0A8, {});

MGS_ARY(1, 0x6BBD60, DWORD, 256, dg_dword_6BBD60_prim_ptrs, {});
MGS_VAR(REDIRECT_LIBDG_DATA, 0x6BBD5C, DWORD, dg_dword_6BBD5C_k320, 0);
MGS_ARY(1, 0x6BB95C, DWORD, 256, dg_dword_6BB95C_prim_ptrs, {});

void CC Pack_DRAWENV_40DDE0(DR_ENV* pPacked, DRAWENV* drawEnv)
{
    pPacked->code[0] =
        ((drawEnv->clip.y1 & 0x3FF) << 10) 
        | drawEnv->clip.x1 & 0x3FF 
        | 0xE3000000;

    pPacked->code[1] = 
        (((drawEnv->clip.y1 + drawEnv->clip.y2 - 1) & 0x3FF) << 10) 
        | (drawEnv->clip.x1 + drawEnv->clip.x2 - 1) & 0x3FF 
        | 0xE4000000;

    pPacked->code[2] = 
        ((drawEnv->offy & 0x3FF) << 11) 
        | drawEnv->offx & 0x7FF 
        | 0xE5000000;

    pPacked->code[3] = 
        32 * (((256 - drawEnv->textureWindow.y2) >> 3) & 0x1F) 
        | ((256 - drawEnv->textureWindow.x2) >> 3) & 0x1F 
        | (((drawEnv->textureWindow.y1 >> 3) & 0x1F) << 15) 
        | (((drawEnv->textureWindow.x1 >> 3) & 0x1F) << 10) 
        | 0xE2000000;

    pPacked->code[4] = 
        ((drawEnv->dtd != 0) << 9) 
        | ((drawEnv->dfe != 0) << 10) 
        | drawEnv->texturePage & 0x1FF 
        | 0xE1000000;

    pPacked->tag = pPacked->tag & 0xFFFFFF | 0x5000000;// 0x50 = gradated line?
}
MGS_FUNC_IMPLEX(0x40DDE0, Pack_DRAWENV_40DDE0, LIBDG_IMPL);

void CC sub_401570(struct_gv* gv, DRAWENV* pDrawEnv, int bNotBg)
{
    DRAWENV drawEnv = {};

    memcpy(&drawEnv, pDrawEnv, sizeof(drawEnv));
    const int xoff = gv->word_6BC378_1 != 0 ? dg_dword_6BBD5C_k320 : 0;
    gv->dword_6BC3D0_rect.x1 = drawEnv.clip.x1 - drawEnv.offx;
    gv->dword_6BC3D0_rect.y1 = drawEnv.clip.y1 - drawEnv.offy;
    gv->dword_6BC3D0_rect.x2 = drawEnv.clip.x2;
    gv->dword_6BC3D0_rect.y2 = drawEnv.clip.y2;
    Pack_DRAWENV_40DDE0(&gv->dword_6BC4D8_src_dr_env1, &drawEnv);
    drawEnv.clip.x1 += xoff;
    drawEnv.offx += xoff;
    Pack_DRAWENV_40DDE0(&gv->dword_6BC518_src_offsetted_dr_evn, &drawEnv);
    if (bNotBg)
    {
        drawEnv.isbg = 0;
        Pack_DRAWENV_40DDE0(&pPacked_6BE0A8, &drawEnv);
        drawEnv.clip.x1 -= xoff;
        drawEnv.offx -= xoff;
        Pack_DRAWENV_40DDE0(&stru_6BE068, &drawEnv);
    }
}
MGS_FUNC_IMPLEX(0x401570, sub_401570, LIBDG_IMPL);

void CC Gv3StructsInit_4012F2(int k320)
{
    DRAWENV drawEnv = {};
    //DrawSynCallback(NullDrawSyncCallBack);

    gLibGvStruct0_6BC180.word_6BC376_16 = -1;
    dg_dword_6BBD5C_k320 = k320;
    gLibGvStruct0_6BC180.mOrderingTables[0] = (int)ot_gv0_0_6BCB44;// ot start?
    gLibGvStruct0_6BC180.mOrderingTables[1] = (int)ot_gv0_1_6BCBC8;// also part of ot?
    gLibGvStruct0_6BC180.gPrimQueue2_word_6BC3C0_256 = 8;
    gLibGvStruct0_6BC180.g_PrimQueue1_word_6BC3BE_256 = 8;
    gLibGvStruct0_6BC180.gObjectQueue_word_6BC3C2_0 = 0;
    gLibGvStruct0_6BC180.gObjects_dword_6BC3C4 = (Prim_unknown **)dg_dword_6BBD60_prim_ptrs;// Might be 8 instead of 256?
    gLibGvStruct0_6BC180.word_6BC374_8 = 5;
    gLibGvStruct0_6BC180.word_6BC378_1 = 1;
    gLibGvStruct0_6BC180.word_6BC37A_0_1EC_size = 0;// used in other func
    
    Renderer_DRAWENV_Init_401888(&drawEnv, 0, 0, 320, 224);
    drawEnv.isbg = 1;
    
    sub_401570(&gLibGvStruct0_6BC180, &drawEnv, 1);
    
    memcpy(
        gLibGvStruct0_6BC180.dword_6BC3D8_dst,
        &gLibGvStruct0_6BC180.dword_6BC4D8_src_dr_env1,
        sizeof(gLibGvStruct0_6BC180.dword_6BC3D8_dst));
    
    gLibGvStruct0_6BC180.dword_6BC3C8_pStructure_rect = gLibGvStruct0_6BC180.dword_6BC3D0_rect;
    
    memcpy(
        gLibGvStruct0_6BC180.dword_6BC418_dst,
        &gLibGvStruct0_6BC180.dword_6BC518_src_offsetted_dr_evn,
        sizeof(gLibGvStruct0_6BC180.dword_6BC418_dst));

    gLibGVStruct1_6BC36C.mOrderingTables[0] = (int)&ot_gv1_0_6BD04C[0];
    gLibGVStruct1_6BC36C.mOrderingTables[1] = (int)&ot_gv1_1_6BD450[0];
    gLibGVStruct1_6BC36C.gPrimQueue2_word_6BC3C0_256 = 256;
    gLibGVStruct1_6BC36C.g_PrimQueue1_word_6BC3BE_256 = 256;
    gLibGVStruct1_6BC36C.gObjectQueue_word_6BC3C2_0 = 0;
    gLibGVStruct1_6BC36C.gObjects_dword_6BC3C4 = (Prim_unknown **)dg_dword_6BB95C_prim_ptrs;
    gLibGVStruct1_6BC36C.word_6BC374_8 = 8;
    gLibGVStruct1_6BC36C.word_6BC376_16 = 16;
    gLibGVStruct1_6BC36C.word_6BC378_1 = 1;
    gLibGVStruct1_6BC36C.word_6BC37A_0_1EC_size = 0;
    
    Renderer_DRAWENV_Init_401888(&drawEnv, 0, 0, 320, 224);
    drawEnv.offx = 160;
    drawEnv.offy = 112;
    sub_401570(&gLibGVStruct1_6BC36C, &drawEnv, 0);
    
    memcpy(
        gLibGVStruct1_6BC36C.dword_6BC3D8_dst,
        &gLibGVStruct1_6BC36C.dword_6BC4D8_src_dr_env1,
        sizeof(gLibGVStruct1_6BC36C.dword_6BC3D8_dst));
   
    gLibGVStruct1_6BC36C.dword_6BC3C8_pStructure_rect = gLibGVStruct1_6BC36C.dword_6BC3D0_rect;
   
    memcpy(
        gLibGVStruct1_6BC36C.dword_6BC418_dst,
        &gLibGVStruct1_6BC36C.dword_6BC518_src_offsetted_dr_evn,
        sizeof(gLibGVStruct1_6BC36C.dword_6BC418_dst));
    
    memcpy(gLibGVStruct1_6BC36C.dword_6BC458, &stru_6BE068, sizeof(gLibGVStruct1_6BC36C.dword_6BC458));
    memcpy(gLibGVStruct1_6BC36C.dword_6BC498, &pPacked_6BE0A8, sizeof(gLibGVStruct1_6BC36C.dword_6BC498));

    gLibGVStruct2_6BC558.mOrderingTables[0] = (int)&ot_gv2_0_6BDC54[0];
    gLibGVStruct2_6BC558.mOrderingTables[1] = (int)&ot_gv2_1_6BDC5C[0];
    gLibGVStruct2_6BC558.gPrimQueue2_word_6BC3C0_256 = 0;
    gLibGVStruct2_6BC558.g_PrimQueue1_word_6BC3BE_256 = 0;
    gLibGVStruct2_6BC558.gObjectQueue_word_6BC3C2_0 = 0;
    gLibGVStruct2_6BC558.gObjects_dword_6BC3C4 = 0;
    gLibGVStruct2_6BC558.word_6BC374_8 = 0;
    gLibGVStruct2_6BC558.word_6BC376_16 = 8;
    gLibGVStruct2_6BC558.word_6BC378_1 = 1;
    gLibGVStruct2_6BC558.word_6BC37A_0_1EC_size = 0;
    
    Renderer_DRAWENV_Init_401888(&drawEnv, 0, 0, 320, 224);
    sub_401570(&gLibGVStruct2_6BC558, &drawEnv, 0);
    
    memcpy(
        gLibGVStruct2_6BC558.dword_6BC3D8_dst,
        &gLibGVStruct2_6BC558.dword_6BC4D8_src_dr_env1,
        sizeof(gLibGVStruct2_6BC558.dword_6BC3D8_dst));

    gLibGVStruct2_6BC558.dword_6BC3C8_pStructure_rect = gLibGVStruct2_6BC558.dword_6BC3D0_rect;
    
    memcpy(
        gLibGVStruct2_6BC558.dword_6BC418_dst,
        &gLibGVStruct2_6BC558.dword_6BC518_src_offsetted_dr_evn,
        sizeof(gLibGVStruct2_6BC558.dword_6BC418_dst));

    memcpy(gLibGVStruct2_6BC558.dword_6BC458, &stru_6BE068, sizeof(gLibGVStruct2_6BC558.dword_6BC458));
    memcpy(gLibGVStruct2_6BC558.dword_6BC498, &pPacked_6BE0A8, sizeof(gLibGVStruct2_6BC558.dword_6BC498));
}
MGS_FUNC_IMPLEX(0x4012F2, Gv3StructsInit_4012F2, LIBDG_IMPL);

struct Texture_Record
{
    WORD mHashedName;
    BYTE mUsed;
    BYTE mNumColours;
    WORD mTPage;
    WORD mClut;
    BYTE u0;
    BYTE v0;
    BYTE u1;
    BYTE v1;
};
MGS_ASSERT_SIZEOF(Texture_Record, 0xC);

MGS_VAR(LIBDG_IMPL, 0x78D320, DWORD, gNumTexturesCopiedToResidentMemory_78D320, 0);
MGS_VAR(LIBDG_IMPL, 0x78BB08, Texture_Record*, gResidentTextureCacheCopy_78BB08, nullptr);
MGS_ARY(LIBDG_IMPL, 0x78BB20, Texture_Record, 512, g512Textures_unk_78BB20, {});

signed int CC LibDG_SearchForTextureRecord_4024D2(signed int hashCode, Texture_Record** ppFreeItem)
{
    const int kStartEndIdx = hashCode % 512;
    int idx = kStartEndIdx;
    do
    {
        // Not found case
        if (!g512Textures_unk_78BB20[idx].mHashedName)
        {
            *ppFreeItem = &g512Textures_unk_78BB20[idx];
            return 0;
        }

        // Found case
        if (g512Textures_unk_78BB20[idx].mHashedName == hashCode)
        {
            *ppFreeItem = &g512Textures_unk_78BB20[idx];
            return 1;
        }

        // Loop back around to the starting item
        if (++idx == 512)
        {
            idx = 0;
        }

    } while (idx != kStartEndIdx);

    *ppFreeItem = nullptr;

    return 0;
}
MGS_FUNC_IMPLEX(0x4024D2, LibDG_SearchForTextureRecord_4024D2, LIBDG_IMPL);

Texture_Record* CC LibDG_FindTexture_4024A0(WORD hashCode)
{
    Texture_Record* pFound = nullptr;
    if (!LibDG_SearchForTextureRecord_4024D2(hashCode, &pFound))
    {
        // Not found case
        static Texture_Record sLastFoundTexture_6BEE78 = {};
        LOG_ERROR("Texture " << hashCode << " was not found");
        return &sLastFoundTexture_6BEE78;
    }
    return pFound;
}
MGS_FUNC_IMPLEX(0x4024A0, LibDG_FindTexture_4024A0, LIBDG_IMPL);

void CC LibDG_ClearTexturesCache_402487()
{
    for (int i=0; i<512; i++)
    {
        g512Textures_unk_78BB20[i].mHashedName = 0;
        g512Textures_unk_78BB20[i].mUsed = FALSE;
    }
}
MGS_FUNC_IMPLEX(0x402487, LibDG_ClearTexturesCache_402487, LIBDG_IMPL);

void CC LibDG_Restore_Textures_From_Resident_Memory_40274C()
{
    if (gResidentTextureCacheCopy_78BB08 && gNumTexturesCopiedToResidentMemory_78D320 > 0)
    {
        for (DWORD i = 0; i < gNumTexturesCopiedToResidentMemory_78D320; i++)
        {
            // Record will not exist therefore pFree will be set to a pointer to the next free entry
            Texture_Record* pFree = nullptr;
            LibDG_SearchForTextureRecord_4024D2(gResidentTextureCacheCopy_78BB08[i].mHashedName, &pFree);

            // Which we then populate
            *pFree = gResidentTextureCacheCopy_78BB08[i];
        }
    }
}
MGS_FUNC_IMPLEX(0x40274C, LibDG_Restore_Textures_From_Resident_Memory_40274C, LIBDG_IMPL);

void CC LibDG_Save_Texture_Hashes_To_Resident_Memory_4026F5()
{
    int usedCount = 0;
    for (int i = 0; i < 512; i++)
    {
        if (g512Textures_unk_78BB20[i].mHashedName)
        {
            usedCount++;
        }
    }

    if (usedCount > 0)
    {
        gNumTexturesCopiedToResidentMemory_78D320 = usedCount;
        Texture_Record* pResidentAllocated = (Texture_Record *)ResidentTopAllocate_40B379(sizeof(Texture_Record) * usedCount);
        gResidentTextureCacheCopy_78BB08 = pResidentAllocated;

        for (int i = 0; i < 512; i++)
        {
            if (g512Textures_unk_78BB20[i].mHashedName)
            {
                *pResidentAllocated = g512Textures_unk_78BB20[i];
                ++pResidentAllocated;
            }
        }
    }
}
MGS_FUNC_IMPLEX(0x4026F5, LibDG_Save_Texture_Hashes_To_Resident_Memory_4026F5, LIBDG_IMPL);

void CC LibDG_Clear_Resident_Texture_Cache_Copy_4026E6()
{
    gNumTexturesCopiedToResidentMemory_78D320 = 0;
    gResidentTextureCacheCopy_78BB08 = nullptr;
}
MGS_FUNC_IMPLEX(0x4026E6, LibDG_Clear_Resident_Texture_Cache_Copy_4026E6, LIBDG_IMPL);


MGS_FUNC_NOT_IMPL(0x4020D8, void CC(), sub_4020D8);
MGS_FUNC_NOT_IMPL(0x401A31, void CC(), jImageMove_401A31);
MGS_FUNC_NOT_IMPL(0x40B9FF, void CC(), LibDG_40B9FF);

void CC SetRGB_40195A(char r, char g, char b)
{
    DRAWENV drawEnv = {};
    Renderer_DRAWENV_Init_401888(&drawEnv, 0, 0, 320, 224);
    drawEnv.r0 = r;
    drawEnv.g0 = g;
    drawEnv.b0 = b;
    drawEnv.isbg = 1;
    sub_401570(&gLibGvStruct0_6BC180, &drawEnv, 1);
    gLibGvStruct0_6BC180.word_6BC37A_0_1EC_size = 2;
}
MGS_FUNC_IMPLEX(0x40195A, SetRGB_40195A, LIBDG_IMPL);

void SetRGB000_4019AB()
{
    SetRGB_40195A(0, 0, 0);
}
MGS_FUNC_IMPLEX(0x4019AB, SetRGB000_4019AB, LIBDG_IMPL);

void CC LibDG_Set_RGB_FromDG_Actor_4019B9()
{
    SetRGB_40195A(gLibDG_2_stru_6BB930.gR_byte_6BB95A, gLibDG_2_stru_6BB930.gB_byte_6BB959, gLibDG_2_stru_6BB930.gG_byte_6BB958);
}
MGS_FUNC_IMPLEX(0x4019B9, LibDG_Set_RGB_FromDG_Actor_4019B9, LIBDG_IMPL);

void CC LibDG_SetRGB_401931(char r, char b, char g)
{
    gLibDG_2_stru_6BB930.gR_byte_6BB95A = r;
    gLibDG_2_stru_6BB930.gG_byte_6BB958 = g;
    gLibDG_2_stru_6BB930.gB_byte_6BB959 = b;
    SetRGB_40195A(r, b, g);
}
MGS_FUNC_IMPLEX(0x401931, LibDG_SetRGB_401931, LIBDG_IMPL);

void CC OT_Related_40162D(int activeBuffer); // WinMain.cpp

void CC LibDG_Init_OTs_ExecFnPtrs_401ABE()
{
    OT_Related_40162D(0);
    OT_Related_40162D(1);
    LibDG_ExecFnPtrs_40171C(0);
    LibDG_ExecFnPtrs_40171C(1);
}
MGS_FUNC_IMPLEX(0x401ABE, LibDG_Init_OTs_ExecFnPtrs_401ABE, LIBDG_IMPL);

void CC LibDG_4010A6()
{
    sub_4020D8();
    LibDG_Init_OTs_ExecFnPtrs_401ABE();
    dword_78D32C = 0;
    jImageMove_401A31();
    LibDG_40B9FF(); // Just sets var 6C0644 = 0
    LibDG_SetRGB_401931(0, 0, 0);
    printf("Object Queue %d\n", gLibGVStruct1_6BC36C.gObjectQueue_word_6BC3C2_0);
    printf("Primitive Queue %d\n", gLibGVStruct1_6BC36C.g_PrimQueue1_word_6BC3BE_256 - gLibGVStruct1_6BC36C.gPrimQueue2_word_6BC3C0_256);
    gLibGVStruct1_6BC36C.gObjectQueue_word_6BC3C2_0 = 0;
    gLibGVStruct1_6BC36C.gPrimQueue2_word_6BC3C0_256 = gLibGVStruct1_6BC36C.g_PrimQueue1_word_6BC3BE_256;
}
MGS_FUNC_IMPLEX(0x4010A6, LibDG_4010A6, LIBDG_IMPL);

void CC LibGvInitDispEnv_401A4F(int ClipX1, __int16 clipY1, __int16 clipX2, __int16 clipY2, int a320)
{
    gDispEnv_6BECF0.disp.y1 = clipY1;
    gDispEnv_6BECF0.disp.x2 = clipX2;
    gDispEnv_6BECF0.disp.y2 = clipY2;
    gDispEnv_6BECF0.screen.x1 = 0;
    gDispEnv_6BECF0.isinter = 0;
    gDispEnv_6BECF0.isrgb24 = 0;
    gDispEnv_6BECF0.disp.x1 = static_cast<s16>(ClipX1);
    dword_6BED18[0] = ClipX1;
    gDispEnv_6BECF0.screen.x2 = 256;
    gDispEnv_6BECF0.screen.y1 = 8;
    gDispEnv_6BECF0.screen.y2 = 224;
    dword_6BED18[1] = a320 + ClipX1;
}
MGS_FUNC_IMPLEX(0x401A4F, LibGvInitDispEnv_401A4F, LIBDG_IMPL);

signed int CC GV_lit_file_handler_402B1D(void*, int)
{
    return 1;
}
MGS_FUNC_IMPLEX(0x402B1D, GV_lit_file_handler_402B1D, LIBDG_IMPL);

const BYTE* CC GV_pcx_file_RLE_decompress_4bit_402F30(const BYTE* pInput, BYTE* pOutput, int count)
{
    do
    {
        BYTE inByte = *pInput++;
        if ((inByte & 0xC0) == 0xC0)
        {
            int runLength = inByte & 0x3F;
            BYTE runValue = *pInput++;
            count -= runLength;
            while (--runLength >= 0)
            {
                *pOutput++ = runValue;
            }
        }
        else
        {
            --count;
            *pOutput++ = inByte;
        }
    } while (count > 0);
    return pInput;
}
MGS_FUNC_IMPLEX(0x402F30, GV_pcx_file_RLE_decompress_4bit_402F30, LIBDG_IMPL);

void CC GV_pcx_file_pallete_convert_4031B9(const BYTE* pPal, WORD* pOutPal, int colourCount)
{
    for(;;)
    {
        colourCount--;
        if (colourCount < 0)
        {
            break;
        }

        BYTE r = pPal[0];
        BYTE g = pPal[1];
        BYTE b = pPal[2];
        WORD pixel16 = ((b | g | r) & 7) != 0 ? 0x20 : 0; // Top 3 bits = transparency
        if (r || g || b)
        {
            pixel16 = ((signed int)(unsigned __int8)r >> 3)
               | 32 * (((signed int)(unsigned __int8)g >> 3)
               | 32 * (((signed int)(unsigned __int8)b >> 3) | pixel16));
        }
        *pOutPal = pixel16;
        ++pOutPal;
        pPal += 3;
    }
}
MGS_FUNC_IMPLEX(0x4031B9, GV_pcx_file_pallete_convert_4031B9, LIBDG_IMPL);

struct pcx_header
{
    char field_0_magic;
    char field_1_version;
    char field_2_bRLE;
    char field_3_bitsPerPlane;
    WORD field_4_Xmin;
    WORD field_6_Ymin;
    WORD field_8_Xmax;
    WORD field_A_Ymax;
    WORD field_C_VertDPI;
    WORD field_E_HorzDPI;
    BYTE field_10_palette[48];
    char field_40_reserved;
    char field_41_colorPlanes;
    WORD field_42_bytesPerPlaneLine;
    WORD field_44_paltype;
    WORD field_46_hScrSize;
    WORD field_48_vScrSize;
    WORD field_4A_mgs_magic_3930;
    WORD field_4C_bpp_mgs;
    WORD field_4E_texX;
    WORD field_50_texY;
    WORD field_52_palX;
    WORD field_54_palY;
    WORD field_56_num_colours;
    char field_58[40];
};
MGS_ASSERT_SIZEOF(pcx_header, 128);

struct pcx_mgs_vram
{
    PSX_RECT field_0_vram_rect;
};
MGS_ASSERT_SIZEOF(pcx_mgs_vram, 8);

struct pcx_mgs
{
    WORD field_0_palx;
    WORD field_2_paly;
    WORD field_4_num_colours;
    WORD field_6_bUnknown;
    WORD field_8_256_pal[256];
    pcx_mgs_vram field_208;
};
MGS_ASSERT_SIZEOF(pcx_mgs, 0x210);

Texture_Record* CC sub_40252B(WORD hashedName, int bpp, __int16 bppShift0x30, PSX_RECT* pVramRect, pcx_mgs* pMgsPcx, BYTE numColours)
{
    Texture_Record* pTexture = nullptr;
    if (LibDG_SearchForTextureRecord_4024D2(hashedName, &pTexture))
    {
        if (pTexture->mUsed)
        {
            pTexture->mHashedName = 0;
        }
    }

    pTexture->mHashedName = hashedName;
    pTexture->mNumColours = numColours;
    pTexture->mUsed = 0;
    pTexture->mTPage = 32 * (bppShift0x30 | 4 * bpp) | (pVramRect->x1 / 64 + 16 * (pVramRect->y1 / 256));
    pTexture->mClut = (pMgsPcx->field_2_paly << 6) | (pMgsPcx->field_0_palx >> 4);

    const BYTE texturePageIndex = pVramRect->x1 % 64;

    BYTE u0 = 0;
    BYTE u1 = 0;
    if (bpp)
    {
        u0 = 2 * texturePageIndex;
        u1 = 2 * pVramRect->x2;
    }
    else
    {
        u0 = 4 * texturePageIndex;
        u1 = 4 * pVramRect->x2;
    }

    pTexture->u0 = u0;
    pTexture->u1 = u1 - 1;
    pTexture->v0 = pVramRect->y1 % 256;
    pTexture->v1 = pVramRect->y2 - 1;
    return pTexture;
}
MGS_FUNC_IMPLEX(0x40252B, sub_40252B, LIBDG_IMPL);

MGS_FUNC_NOT_IMPL(0x41C6B0, void __cdecl(PSX_RECT *pRect, BYTE *pPixelData), Render_sub_41C6B0);
MGS_FUNC_NOT_IMPL(0x41C640, WORD __cdecl(PSX_RECT *pRect, WORD *pallete, BYTE *pixelData, int surfaceType, int pTga, unsigned __int16 tga6, unsigned __int16 tga7), Render_sub_41C640);
MGS_FUNC_NOT_IMPL(0x402FB4, void __cdecl(const BYTE *pIn, BYTE *pOut, int bytesPerScanLine, signed int w, int h), GV_pcx_file_RLE_decompress_8bit_402FB4);

#pragma pack(push)
#pragma pack(1)
struct Tga_header
{
    BYTE field_0_idlength;
    BYTE field_1_colourmaptype;
    BYTE field_2_datatypecode;
    WORD field_3_colourmaporigin;
    WORD field_5_colourmaplength;
    BYTE field_7_colourmapdepth;
    WORD field_8_x_origin;
    WORD field_A_y_origin;
    WORD field_C_width;
    WORD field_E_height;
    BYTE field_10_bitsperpixel;
    BYTE field_11_imagedescriptor;
};
#pragma pack(pop)
MGS_ASSERT_SIZEOF(Tga_header, 0x12);

BYTE* CC jimGetTargetBuffer_42B6A6(WORD fileNameHash, WORD *pWidth, WORD *pHeight, DWORD** ppAllocated)
{
    const char* pHiTexName = HITEX_NAME_51D4BC(fileNameHash);
    if (!pHiTexName)
    {
        return nullptr;
    }

    AbstractedFileHandle* hReplacementTexture = File_LoadDirFile_51EE8F(pHiTexName, 0);
    if (!hReplacementTexture)
    {
        printf("*** $jim *** Error reading file %s.\n", pHiTexName);
        return nullptr;
    }

    const __int32 sizeToRead = File_GetPos_51F09E(hReplacementTexture, 0, 2);

    Tga_header* tgaBuffer = reinterpret_cast<Tga_header*>(malloc(sizeToRead));

    if (!tgaBuffer)
    {
        printf("*** $jim *** Error allocating mempry in jimGetTargetBuffer\n");
        File_Close_51F183(hReplacementTexture);
        return nullptr;
    }
    
    File_GetPos_51F09E(hReplacementTexture, 0, 0);

    if (File_NormalRead_51F0F5(hReplacementTexture, tgaBuffer, sizeToRead) != sizeToRead)
    {
        free(tgaBuffer);
        printf("*** $jim *** Error reading file %s.\n", pHiTexName);
        File_Close_51F183(hReplacementTexture);
        return nullptr;
    }

    File_Close_51F183(hReplacementTexture);

    if ((tgaBuffer->field_1_colourmaptype & 0xFF) > 1u       // Only 0 or 1 colour map type supported
        || tgaBuffer->field_2_datatypecode != 2              // Uncompressed, RGB images.
        || tgaBuffer->field_10_bitsperpixel != 16)          // 16 Bits per pixel
    {
        free(tgaBuffer);
        printf("*** $jim *** Error in Targa file %s or unsupported Targa format\n", pHiTexName);
        return 0;
    }

    DWORD colourMapSize = 0;
    if ((tgaBuffer->field_1_colourmaptype & 0xFF) == 1) // has colour map?
    {
        colourMapSize = (tgaBuffer->field_1_colourmaptype & 0xFF) * tgaBuffer->field_7_colourmapdepth / 8;
    }

    *ppAllocated = reinterpret_cast<DWORD*>(tgaBuffer);
    *pWidth = tgaBuffer->field_C_width;
    *pHeight = tgaBuffer->field_E_height;
    return reinterpret_cast<BYTE*>(tgaBuffer) + sizeof(Tga_header) + (colourMapSize + tgaBuffer->field_0_idlength);
}
MGS_FUNC_IMPLEX(0x42B6A6, jimGetTargetBuffer_42B6A6, LIBDG_IMPL);


MGS_VAR(1, 0x650D1A, WORD, word_650D1A, 0);

int CC GV_pcx_file_handler_402B25(void* fileData, int fileNameHash)
{
    pcx_header* pPcxFileData = reinterpret_cast<pcx_header*>(fileData);
    const WORD mgs_bpp = pPcxFileData->field_4C_bpp_mgs;
    WORD maxW = pPcxFileData->field_8_Xmax + 1 - pPcxFileData->field_4_Xmin;
    const WORD maxH = pPcxFileData->field_A_Ymax + 1 - pPcxFileData->field_6_Ymin;
    if (!(mgs_bpp & 1))
    {
        maxW /= 2;
    }
    const int size = maxH * maxW + sizeof(pcx_mgs);
    pcx_mgs* pAllocated = nullptr;
    if (!System_mem_zerod_alloc_40AFA4(gActiveBuffer_dword_791A08, size, (void**)&pAllocated))
    {
        return 0;
    }

    pAllocated->field_0_palx = pPcxFileData->field_52_palX;
    pAllocated->field_2_paly = pPcxFileData->field_54_palY;
    pAllocated->field_4_num_colours = pPcxFileData->field_56_num_colours;
    pAllocated->field_6_bUnknown = 1;

    pcx_mgs_vram* pRect = &pAllocated->field_208;
    pRect->field_0_vram_rect.x1 = pPcxFileData->field_4E_texX;
    pRect->field_0_vram_rect.y1 = pPcxFileData->field_50_texY;
    pRect->field_0_vram_rect.x2 = maxW / 2;
    pRect->field_0_vram_rect.y2 = maxH;

    const BYTE* pPal = nullptr;
    if (mgs_bpp & 1)
    {
        const BYTE* pRet = GV_pcx_file_RLE_decompress_4bit_402F30(
            (const BYTE*)&pPcxFileData[1].field_0_magic,
            (unsigned __int8 *)&pRect[1],
            maxH * maxW);
        pPal = pRet + 1;
    }
    else
    {
        GV_pcx_file_RLE_decompress_8bit_402FB4(
            (const BYTE*)&pPcxFileData[1].field_0_magic,
            (unsigned __int8 *)&pRect[1],
            pPcxFileData->field_42_bytesPerPlaneLine,
            maxW,
            maxH);
        pPal = pPcxFileData->field_10_palette;
    }
    GV_pcx_file_pallete_convert_4031B9(pPal, pAllocated->field_8_256_pal, pAllocated->field_4_num_colours);

    gStageIs_s11e_6FC778 = strcmp(File_StageName_44EB83(), "s11e") == 0;

    // NOTE: Software rendering branch has been pruned here

    WORD tgaWidth = 0;
    WORD tgaHeight = 0;
    DWORD* tgaBackingBuffer = nullptr;
    const BYTE* pTga16BitPixels = jimGetTargetBuffer_42B6A6(fileNameHash, &tgaWidth, &tgaHeight, &tgaBackingBuffer);
 
    word_650D1A = Render_sub_41C640(
        &pRect->field_0_vram_rect,
        pAllocated->field_8_256_pal,
       (BYTE*)&pRect[1],
        mgs_bpp & 1,
        (int)pTga16BitPixels,
        tgaWidth,
        tgaHeight);

    if (pTga16BitPixels)
    {
        free(tgaBackingBuffer);
    }

    Render_sub_41C6B0(&pRect->field_0_vram_rect, (BYTE*)&pRect[1]);

    System_VoidAllocation_40B187(gActiveBuffer_dword_791A08, (void**)&pAllocated);
    if (fileNameHash)
    {
        sub_40252B(
            fileNameHash,
            mgs_bpp & 1,
            (mgs_bpp & 0x30) >> 4,
            &pRect->field_0_vram_rect,
            pAllocated,
            static_cast<BYTE>(pAllocated->field_4_num_colours));
    }

    return 1;
}
MGS_FUNC_IMPLEX(0x402B25, GV_pcx_file_handler_402B25, LIBDG_IMPL);

// TODO: These are not implemented - just here to return 1 for running standalone
int CC GV_kmd_file_handler_402796(void* fileData, int fileNameHash)
{
    LOG_WARNING("KMD loader not impl");
    return 1;
}
MGS_FUNC_IMPLEX(0x402796, GV_kmd_file_handler_402796, false); // TODO

int CC GV_n_file_handler_402A03(void* fileData, int fileNameHash)
{
    LOG_WARNING("N loader not impl");
    return 1;
}
MGS_FUNC_IMPLEX(0x402A03, GV_n_file_handler_402A03, false); // TODO

int CC GV_oar_file_handler_402A29(void* fileData, int fileNameHash)
{
    LOG_WARNING("OAR loader not impl");
    return 1;
}
MGS_FUNC_IMPLEX(0x402A29, GV_oar_file_handler_402A29, false); // TODO

int CC GV_zmd_file_handler_403290(void* fileData, int fileNameHash)
{
    LOG_WARNING("ZMD loader not impl");
    return 1;
}
MGS_FUNC_IMPLEX(0x403290, GV_zmd_file_handler_403290, false); // TODO

int CC GV_img_file_handler_402A5F(void* fileData, int fileNameHash)
{
    LOG_WARNING("IMG loader not impl");
    return 1;
}
MGS_FUNC_IMPLEX(0x402A5F, GV_img_file_handler_402A5F, false); // TODO

int CC GV_sgt_file_handler_402AA9(void* fileData, int fileNameHash)
{
    LOG_WARNING("SGT loader not impl");
    return 1;
}
MGS_FUNC_IMPLEX(0x402AA9, GV_sgt_file_handler_402AA9, false); // TODO

MGS_VAR(REDIRECT_LIBDG_DATA, 0x6BB908, DWORD, dword_6BB908, 0);

MGS_VAR(REDIRECT_LIBDG_DATA, 0x7919CE, DWORD, dword_7919CE, 0);
MGS_VAR(REDIRECT_LIBDG_DATA, 0x7919D4, DWORD, dword_7919D4, 0);
MGS_VAR(REDIRECT_LIBDG_DATA, 0x650090, DWORD, dword_650090, 0);


MGS_VAR(REDIRECT_LIBDG_DATA, 0x9942AB, BYTE, byte_9942AB, 0);



MGS_VAR_EXTERN(DWORD, game_state_dword_72279C);
MGS_FUNC_NOT_IMPL(0x40A857, void CC(), sub_40A857);
void CC Main_sub_401C02();

MGS_FUNC_NOT_IMPL(0x5200D2, signed __int64 CC(), sub_5200D2);

signed __int64 CC WaitFor_445580(int totalCount)
{
    __int64 result = 0;
    for (int i = 0; i < totalCount; i++)
    {
        result = sub_5200D2();
    }
    return result;
}
MGS_FUNC_IMPLEX(0x445580, WaitFor_445580, LIBDG_IMPL);

void CC LibDG_Update2_401234(Actor* /*pLibDg*/)
{
    static int sTimeStamp_dword_650094 = 0;

    dword_6BB908 = 0;

    gLibDG_2_stru_6BB930.dword_6BB954 = gLibDG_2_stru_6BB930.dword_6BB950_do_not_flip_buffers;

    if (BYTE3(game_state_dword_72279C) & 0x20)
    {
        int bDontFlipBuffers = gLibDG_2_stru_6BB930.dword_6BB950_do_not_flip_buffers;

        if (sTimeStamp_dword_650094 == -1)
        {
            sTimeStamp_dword_650094 = TimeGetElapsed_4455A0();
            bDontFlipBuffers = 0;
            gLibDG_2_stru_6BB930.dword_6BB950_do_not_flip_buffers = 0;
        }

        if (!bDontFlipBuffers)
        {
            WaitFor_445580(dword_650090);
        }

        // TODO: This is not 100% correct - but I don't think 
        // dword_6BB950_do_not_flip_buffers can ever be set to 1?
        //MGS_WARN_ONCE("Possibly wrong behavior here"); // TODO: Impl MGS_WARN_ONCE

        int elapsed = TimeGetElapsed_4455A0();
        if (elapsed - (sTimeStamp_dword_650094 + 2) < 0)
        {
            sTimeStamp_dword_650094 += 2;
        }
        gLibDG_2_stru_6BB930.dword_6BB950_do_not_flip_buffers = 0;
    }
    else
    {
        WaitFor_445580(dword_650090);
        sTimeStamp_dword_650094 = -1;
        gLibDG_2_stru_6BB930.dword_6BB950_do_not_flip_buffers = 0;
    }

    Main_sub_401C02();
    
    sub_40A857(); // Probably input related as input no longer works if not called?

    dword_995324 = (int)&dword_7919C0;

    if (byte_9942AB & 0x10)
    {
        if (word_7919C2[7] | word_7919C2[9])
        {
            dword_995324 = (int)&word_7919C2[7];
        }
    }


    //p[0] |= 0xffff; // Shift / X held
    //p[1] |= 0xffff; // Tri/first person held
   // p[3] |= 0xffff; // 3rd person cam force ?
    //p[4] |= 0xffff; // can't dpad
   // p[6] |= 0xffff;

   // p[4] = 0x800; // Up 
    //p[4] = 0x400; // Right
   // p[4] = 0x1000; // Down
    //p[4] = 0x2000; // Down
    //p[4] = 0x4000; // Down
    //p[4] = 0x8000; // Nothing
    //p[4] = 0x1; // Down
    //p[4] = 0x2;  // Down
   // p[4] = 0x4;
   // p[4] = 0x8;
   // p[4] = 0x10;
   // p[4] = 0x20;
   // p[4] = 0x80; // down and right
   // p[4] = 0x100;
}
MGS_FUNC_IMPLEX(0x401234, LibDG_Update2_401234, LIBDG_IMPL);


MGS_VAR(1, 0x6BECE8, DWORD, gLibDG_ExecPtrs_6BECE8, 1);
MGS_ARY(1, 0x6BE4E8, DWORD*, 512, gUnkSize_1024_6BE4E8, {}); // TODO: Might just be 256?


MGS_FUNC_NOT_IMPL(0x40B231, unsigned int CC(void* pMem, int size), MemClearUnknown_40B231);

using TDG_FnPtr = void(CC*)(struct_gv* pGv, int activeBuffer);

MGS_FUNC_NOT_IMPL(0x4065AA, int CC(struct_gv* pGv, int activeBuffer), sub_4065AA);
MGS_FUNC_NOT_IMPL(0x4064B1, void CC(Prim_Object *pObject, int activeBuffer, char a3, int a4), sub_4064B1);
MGS_FUNC_NOT_IMPL(0x401DA8, int CC(PSX_RECT* pRect, int a2), sub_401DA8);




void CC LibGV_4061E7(struct_gv* pGv, int activeBuffer)
{
    /*
    sub_401DA8((PSX_RECT*)&pGv->dword_6BC3C8_pStructure_rect, pGv->word_6BC3BC);

    Prim_Object **pObjects = pGv->gObjects_dword_6BC3C4;
    if (pGv->gObjectQueue_word_6BC3C2_0 > 0)
    {
        s16 objectQueueCount = pGv->gObjectQueue_word_6BC3C2_0;
        do
        {
            Prim_Object* pObject = *pObjects;
            ++pObjects;
            s16 v5 = 0;

            const int flags = pObject->field_28_flags;
            if (!(pObject->field_28_flags & 0x80) && (!pObject->field_2C || pObject->field_2C & dword_78D32C))
            {
                v5 = 2;
                if (flags & 0x20)
                {

                    *(QWORD *)&stru_993E40 = *(QWORD *)&pObject->field_68_92b_size;
                    *((QWORD *)&stru_993E40 + 1) = *(QWORD *)&pObject->field_70_pInners;
                    *((QWORD *)&stru_993E40 + 2) = *(QWORD *)&pObject->field_78;
                    *((QWORD *)&stru_993E40 + 3) = *(QWORD *)&pObject->field_80;
                    sub_4062CB(pObject->field_24 + 8);
                    v5 = sub_40640F();
                }
            }
            pObject->field_32_hasInners = v5;
            sub_4064B1(pObject, activeBuffer, static_cast<char>(flags), v5);
            --objectQueueCount;
        } while (objectQueueCount);
    }
    sub_4065AA(pGv, activeBuffer);
    */
}
MGS_FUNC_IMPLEX(0x4061E7, LibGV_4061E7, false); // TODO: Fully implement

MGS_FUNC_NOT_IMPL(0x407122, void CC(struct_gv* pGv, int activeBuffer), LibGV_407122);
MGS_FUNC_NOT_IMPL(0x405668, void CC(struct_gv* pGv, int activeBuffer), LibGV_405668);
MGS_FUNC_NOT_IMPL(0x405180, void CC(struct_gv* pGv, int activeBuffer), LibGV_405180);
MGS_FUNC_NOT_IMPL(0x4041A5, void CC(struct_gv* pGv, int activeBuffer), LibGV_4041A5);
MGS_FUNC_NOT_IMPL(0x403528, void CC(struct_gv* pGv, int activeBuffer), LibGV_403528);
//MGS_FUNC_NOT_IMPL(0x40340A, void CC(struct_gv* pGv, int activeBuffer), LibGV_40340A);

MGS_ARY(1, 0x991E40, int, 8, dword_991E40, {});


void CC OrderingTableAdd_4034C6(int pPrimDataStart, int count, int size)
{
    DWORD* dword_991E40_1_ot_ptr = (DWORD*)dword_991E40[1];
    const int dword_991E40_2_field_2E_w_or_h = dword_991E40[2];
    if (count - 1 >= 0)
    {
        int pData = pPrimDataStart;
        do
        {
            const int tag = *(WORD *)pData;
            if (tag > 0)
            {
                signed int v6 = tag - dword_991E40_2_field_2E_w_or_h;
                if (v6 < 0)
                {
                    v6 = 0;
                }

                const signed int maybe_z = v6 >> 8;

                // Start of prim points to next OT entry?
                *(DWORD *)pData &= 0xFF000000;
                *(DWORD *)pData |= dword_991E40_1_ot_ptr[maybe_z] & 0x00FFFFFF;

                // OT points to prim
                dword_991E40_1_ot_ptr[maybe_z] &= 0xFF000000;
                dword_991E40_1_ot_ptr[maybe_z] |= pData & 0x00FFFFFF;
            }
            pData += size;
            --count;
        } while (count);
    }
}
MGS_FUNC_IMPLEX(0x4034C6, OrderingTableAdd_4034C6, true); // TODO: Implement me

void __cdecl LibGV_40340A(struct_gv* pGv, int activeBuffer)
{
    int unkByte012Ptr; // edi@3

    dword_991E40[0] = (int)gUnkSize_1024_6BE4E8;  // 256 DWORD's
    int otPtr = *(&pGv->mOrderingTables[activeBuffer]);
    dword_991E40[1] = otPtr + 4;
    DWORD** otrPtrNext = (DWORD**)dword_991E40[1];
    for (int i = 0; i < 256; i++)
    {
        DWORD* unkItem = gUnkSize_1024_6BE4E8[i];
        if (unkItem) // Only has ptrs for "3d objects" ?
        {
            do
            {
                DWORD unkByte3Idx = *unkItem >> 24; // Get single 3rd byte
                DWORD** otItemPtrPtr = &otrPtrNext[unkByte3Idx]; // Index into the OT using this byte
                unkByte012Ptr = *unkItem & 0xFFFFFF; // Other 3 bytes
                *unkItem = (unsigned int)*otItemPtrPtr | 0xC000000; // Set UNK to point to the OT
                *otItemPtrPtr = unkItem; // Set OT to point to the UNK
                unkItem = (DWORD *)unkByte012Ptr; // To next unk entry ?
            } while (unkByte012Ptr);
        }
    }

    DWORD v9 = dword_78D32C & 0xFFFF;
    
    const int primCount = pGv->g_PrimQueue1_word_6BC3BE_256 - pGv->gPrimQueue2_word_6BC3C0_256;
    for (int i = 0; i < primCount; i++)
    {
        Prim_unknown* p = pGv->gObjects_dword_6BC3C4[pGv->gPrimQueue2_word_6BC3C0_256 + i]; // 006bbd58
        if (!(BYTE1(p->field_24_maybe_flags) & 1) && (!p->field_28_dword_9942A0 || p->field_28_dword_9942A0 & v9))
        {
            dword_991E40[2] = p->field_2E_w_or_h;
            OrderingTableAdd_4034C6(
                (int)p->field_40_pDataStart[activeBuffer],
                p->field_2A_num_items,
                p->field_30_size);
        }
    }
}
MGS_FUNC_IMPLEX(0x40340A, LibGV_40340A, true); // TODO: Implement me


MGS_ARY(1, 0x6500E0, TDG_FnPtr, 8, gLibDg_FuncPtrs_off_6500E0, 
{
    LibGV_407122.Ptr(),
    LibGV_4061E7,
    LibGV_405668.Ptr(),
    LibGV_405180.Ptr(),
    LibGV_4041A5.Ptr(),
    LibGV_403528.Ptr(),
    LibGV_40340A,
    nullptr
});

// Returns the old pointer because the calling code will use it to restore it later
TDG_FnPtr CC LibDG_SetFnPtr_4019FA(int idx, TDG_FnPtr fnPtr)
{
    if (idx == 7)
    {
        MGS_FATAL("LibDG_SetFnPtr_4019FA - should never be reached");
    }
    TDG_FnPtr old = gLibDg_FuncPtrs_off_6500E0[idx];
    gLibDg_FuncPtrs_off_6500E0[idx] = fnPtr;
    return old;
}
MGS_FUNC_IMPLEX(0x4019FA, LibDG_SetFnPtr_4019FA, LIBDG_IMPL);

void CC LibDG_Update1_4012ED(Actor* /*pActor*/)
{
    // This function is a jmp to a stub that does this
    LibDG_ExecFnPtrs_40171C(gActiveBuffer_dword_791A08);
}
MGS_FUNC_IMPLEX(0x4012ED, LibDG_Update1_4012ED, LIBDG_IMPL);

void CC LibDg_Init_40111A()
{
    //nullsub_8();
    //nullsub_7(DeadCode_4011F8);
    LibGvInitDispEnv_401A4F(0, 0, 320, 240, 320);
    Gv3StructsInit_4012F2(320);
    LibDG_Clear_Resident_Texture_Cache_Copy_4026E6();
    LibDG_4010A6();
    LibGV_Set_FileExtHandler_40A68D('p', GV_pcx_file_handler_402B25); // .pcc/.pcx
    LibGV_Set_FileExtHandler_40A68D('k', GV_kmd_file_handler_402796);
    LibGV_Set_FileExtHandler_40A68D('l', GV_lit_file_handler_402B1D);
    LibGV_Set_FileExtHandler_40A68D('n', GV_n_file_handler_402A03);
    LibGV_Set_FileExtHandler_40A68D('o', GV_oar_file_handler_402A29);
    LibGV_Set_FileExtHandler_40A68D('z', GV_zmd_file_handler_403290);
    LibGV_Set_FileExtHandler_40A68D('i', GV_img_file_handler_402A5F);
    LibGV_Set_FileExtHandler_40A68D('s', GV_sgt_file_handler_402AA9);
    Actor_PushBack_40A2AF(0, &gLibDG_2_stru_6BB930.mBase, 0);// Handles 2D rendering?
    Actor_Init_40A347(&gLibDG_2_stru_6BB930.mBase, LibDG_Update2_401234, nullptr, "C:\\mgs\\source\\LibDG\\dgd.c");
    Actor_PushBack_40A2AF(8, &gLibDGD_1_stru_6BB910, 0); // Handles 3D rendering?
    Actor_Init_40A347(&gLibDGD_1_stru_6BB910, LibDG_Update1_4012ED, nullptr, "C:\\mgs\\source\\LibDG\\dgd.c");
}
MGS_FUNC_IMPLEX(0x40111A, LibDg_Init_40111A, LIBDG_IMPL);

signed int CC PrimAdd_401805(Prim_unknown* pPrimBuffer)
{
    struct_gv* pGv = &gLibGVStruct1_6BC36C;
    assert(pPrimBuffer->field_2C_r_index == 0);
    pGv = pGv + pPrimBuffer->field_2C_r_index; // Always 0?
    if (pGv->gPrimQueue2_word_6BC3C0_256 > pGv->gObjectQueue_word_6BC3C2_0)
    {
        const s16 newCount = pGv->gPrimQueue2_word_6BC3C0_256 - 1;
        pGv->gObjects_dword_6BC3C4[newCount] = pPrimBuffer; // PrimObject = Prim_unknown + extra ??
        pGv->gPrimQueue2_word_6BC3C0_256 = newCount;
        return 0;
    }
    return -1;
}
MGS_FUNC_IMPLEX(0x401805, PrimAdd_401805, LIBDG_IMPL);

void CC LibDG_ExecFnPtrs_40171C(int activeBuffer)
{
    if (!gLibDG_ExecPtrs_6BECE8)
    {
        MemClearUnknown_40B231(gUnkSize_1024_6BE4E8, 1024);

        int count = 7;
        // Check if extra function pointer slot is in use?
        // TODO: Seems like this is dead code as the condition can never be true?
        if (gLibDG_2_stru_6BB930.dword_6BB950_do_not_flip_buffers != 0)
        {
            count++;
            MGS_FATAL("LibDG_ExecFnPtrs_40171C - should never be reached");
        }

        for (int i = 0; i < count; i++)
        {
            gLibDg_FuncPtrs_off_6500E0[i](&gLibGVStruct1_6BC36C, activeBuffer);
        }
    }
}
MGS_FUNC_IMPLEX(0x40171C, LibDG_ExecFnPtrs_40171C, LIBDG_IMPL);
