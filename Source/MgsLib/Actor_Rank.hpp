#pragma once

#include "MgsFunction.hpp"
#include "Actor.hpp"
#include "Psx.hpp"
#include "Kmd.hpp"
#include "Font.hpp"

void Actor_RankCPP_ForceLink();
void DoActor_RankTests();

struct Prim_unknown;
struct ButtonStates;

enum DiffcultyLevels
{
    eVeryEasy = -1,
    eEasy = 0,
    eNormal = 1,
    eHard = 2,
    eExtreme = 3
};
MGS_VAR_EXTERN(short, gDiffcultyLevel_78E7E2);
MGS_VAR_EXTERN(WORD, gNumRations_word_78E88C);

struct Actor_Rank
{
    Actor mBase;
    ButtonStates* field_20_button_states;
    Prim_unknown_0x54* field_24_ptr_16_prims;
    Prim_unknown_0x54* field_28_ptr_9_prims;

    POLY_FT4 field_2C_back_l_type0_0x40_start;      // Back ground left half
    POLY_FT4 field_54_back_r_type0;                 // Back ground right half
    POLY_FT4 field_7C_rank_rank_type0;              // Score text background
    POLY_FT4 field_A4_rank_rank_type0;
    POLY_FT4 field_CC_rank_line1_type1;
    POLY_FT4 field_F4_rank_line1_type1;
    POLY_FT4 field_11C_rank_line2;
    POLY_FT4 field_144_rank_line2;
    POLY_FT4 field_16C_rank_line2;
    POLY_FT4 field_194_rank_line2;
    POLY_FT4 field_1BC_rank_line2;
    POLY_FT4 field_1E4_rank_line2;
    POLY_FT4 field_20C_rank_fox;
    POLY_FT4 field_234_rank_spe_camera;             // Camera text
    POLY_FT4 field_25C_rank_spe_stealth;
    POLY_FT4 field_284_rank_spe_mugen;
    POLY_FT4 field_2AC_cur_lu;
    POLY_FT4 field_2D4_cur_ru;
    POLY_FT4 field_2FC_cur_ld;
    POLY_FT4 field_324_cur_rd;
    POLY_FT4 field_34C_cur_u;
    POLY_FT4 field_374_cur_d;
    POLY_FT4 field_39C_cur_l;
    POLY_FT4 field_3C4_cur_r;
    POLY_FT4 field_3EC_cur_c;

    int field_414_script_e;
    DWORD *field_418_script_t;
    DWORD field_41C_16_prim_dst[16];
    DWORD field_45C_8_prim_dst[9];
    DWORD field_480_ticks;
    DWORD field_484_state;

    DWORD field_488_time;
    int field_48C_time_mins;
    int field_490_time_secs;

    int field_494_ranking;

    int field_498_mc_no;
    int field_49C_radar;
    int field_4A0_stealth;
    int field_4A4_mugen;

    int field_4A8_spe_no;
    int field_4AC_camera;
    int field_4B0_stealth;
    int field_4B4_mugen;

    Font field_4B8_font[6];
    int field_5C0;
    int field_5C4;
    int field_5C8;
    int field_5CC;
    int field_5D0;
    int field_5D4;
    int field_5D8;
    int field_5DC_112_structs_ary;
    int field_5E0_896;
    int field_5E4_256;
    int field_5E8_896;
    int field_5EC_276;
    int field_5F0;
    int field_5F4;
    int field_5F8_script_x;
};
MGS_ASSERT_SIZEOF(Actor_Rank, 0x5FC);

signed int CC Res_rank_Create_46EC0E(DWORD scriptUnknown, int a_dword_722A40, BYTE* pScript);
void CC Res_rank_prim_related_4767CE(Actor_Rank *pRank, WORD resourceNameHash, POLY_FT4* pData, __int16 x, __int16 y, __int16 w, __int16 h, int flagQ, int type);


MGS_VAR_EXTERN(WORD, gGameTime_word_78E896);
MGS_VAR_EXTERN(WORD, gGameTime_word_78E898);
