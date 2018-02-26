#pragma once

#include "MgsFunction.hpp"
#include "Actor.hpp"
#include "Psx.hpp"
#include "Kmd.hpp"

void Actor_RankCPP_ForceLink();
void DoActor_RankTests();

struct Prim_unknown;
struct ButtonStates;

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

    int field_4B8_11_byte_structs_ary;
    int field_4BC;
    int field_4C0;
    int field_4C4;
    int field_4C8;
    int field_4CC;
    int field_4D0;
    int field_4D4;
    int field_4D8;
    int field_4DC;
    int field_4E0;
    int field_4E4;
    int field_4E8;
    int field_4EC;
    int field_4F0;
    int field_4F4;
    int field_4F8;
    int field_4FC;
    int field_500;
    int field_504;
    int field_508;
    int field_50C;
    int field_510;
    int field_514;
    int field_518;
    int field_51C;
    int field_520;
    int field_524;
    int field_528;
    int field_52C;
    int field_530;
    int field_534;
    int field_538;
    int field_53C;
    int field_540;
    int field_544;
    int field_548;
    int field_54C;
    int field_550;
    int field_554;
    int field_558;
    int field_55C;
    int field_560;
    int field_564;
    int field_568;
    int field_56C;
    int field_570;
    int field_574;
    int field_578;
    int field_57C;
    int field_580;
    int field_584;
    int field_588;
    int field_58C;
    int field_590;
    int field_594;
    int field_598;
    int field_59C;
    int field_5A0;
    int field_5A4;
    int field_5A8;
    int field_5AC;
    int field_5B0;
    int field_5B4;
    int field_5B8;
    int field_5BC;
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
