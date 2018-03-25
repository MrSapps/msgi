#include "stdafx.h"
#include "Actor_Movie.hpp"
#include "MgsFunction.hpp"
#include "ResourceNameHash.hpp"
#include "Actor.hpp"
#include "Script.hpp"
#include <gmock/gmock.h>

void Force_Actor_Movie_Cpp_Link() { }

#define MOVIE_IMPL true

struct Actor_Movie
{
    Actor mBase;
    WORD word_7248F0_counter;
    WORD word_7248F2_11_param_i;
    WORD dword_7248F4_param_o;
    WORD field_26;
    int dword_7248F8_script_param_p;
    DWORD dword_7248FC_bIsEnding;
};
MGS_ASSERT_SIZEOF(Actor_Movie, 0x30);

char* CC Res_movie_GetName_4564F5(char* currentDir, WORD movieNameHashed)
{
    static const char* sMovieNames_66B9D8[] =
    {
        "genbaku",
        "kasou",
        "police",
        "kaitai",
        "alaska",
        "wangan",
        "idenshi",
        "inuzori",
        "e399",
        "null"
    };

    static char sMoviePath_724900[256] = {};

    int i = 0;
    for (i = 0; i < _countof(sMovieNames_66B9D8)-1; i++)
    {
        if (ResourceNameHash(sMovieNames_66B9D8[i]) == movieNameHashed)
        {
            break;
        }
    }
    sprintf(sMoviePath_724900, "%s/movie/%s.ddv", currentDir, sMovieNames_66B9D8[i]);
    return sMoviePath_724900;
}
MGS_FUNC_IMPLEX(0x4564F5, Res_movie_GetName_4564F5, MOVIE_IMPL);

MGS_FUNC_NOT_IMPL(0x4561DF, Actor_Movie *__cdecl (int movieNameHashed, int bIsEnding), Res_movie_create_4561DF); // TODO

Actor_Movie* CC Res_movie_create_456860(int movieNameHashed)
{
    Actor_Movie* pActorMovie = Res_movie_create_4561DF(movieNameHashed, 0);
    if (pActorMovie)
    {
        if (Script_ParamExists('i'))
        {
            pActorMovie->word_7248F2_11_param_i += static_cast<WORD>(Script_Unknown8_409924(Script_GetReturnAddress()));
        }

        if (Script_ParamExists('o'))
        {
            pActorMovie->dword_7248F4_param_o -= static_cast<WORD>(Script_Unknown8_409924(Script_GetReturnAddress()));
        }

        if (Script_ParamExists('p'))
        {
            pActorMovie->dword_7248F8_script_param_p = static_cast<WORD>(Script_Unknown8_409924(Script_GetReturnAddress()));
        }
        return pActorMovie;
    }

    if (Script_ParamExists('p'))
    {
        Script_ProcCancelOrRun(static_cast<WORD>(Script_Unknown8_409924(Script_GetReturnAddress())), 0);
    }
    return nullptr;
}
MGS_FUNC_IMPLEX(0x456860, Res_movie_create_456860, MOVIE_IMPL);

Actor* CC Res_movie_create_4561C6(DWORD scriptData, int /*scriptBinds*/, BYTE* /*pScript*/)
{
    return &Res_movie_create_456860(scriptData)->mBase;
}
MGS_FUNC_IMPLEX(0x4561C6, Res_movie_create_4561C6, MOVIE_IMPL);

void DoMovie_Tests()
{
    ASSERT_STREQ("./movie/null.ddv", Res_movie_GetName_4564F5(".", ResourceNameHash("lols")));
    ASSERT_STREQ("./movie/genbaku.ddv", Res_movie_GetName_4564F5(".", ResourceNameHash("genbaku")));
}
