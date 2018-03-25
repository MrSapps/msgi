#pragma once

#include "Actor.hpp"

struct Actor_Movie
{
    Actor mBase;
    WORD word_7248F0_counter;
    WORD word_7248F2_11_param_i;
    short dword_7248F4_param_o;
    WORD field_26;
    int dword_7248F8_script_param_p;
    DWORD dword_7248FC_bIsEnding;
};
MGS_ASSERT_SIZEOF(Actor_Movie, 0x30);

void Force_Actor_Movie_Cpp_Link();
void DoMovie_Tests();

Actor* CC Res_movie_create_4561C6(DWORD scriptData, int scriptBinds, BYTE* pScript);
EXTERN_MGS_FUNC_NOT_IMPL(0x4561DF, Actor_Movie *__cdecl (int movieNameHashed, int bIsEnding), Res_movie_create_4561DF); // TODO
