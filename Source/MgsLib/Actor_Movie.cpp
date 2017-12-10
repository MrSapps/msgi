#include "stdafx.h"
#include "Actor_Movie.hpp"
#include "MgsFunction.hpp"
#include "ResourceNameHash.hpp"
#include <gmock/gmock.h>

void Force_Actor_Movie_Cpp_Link() { }

#define MOVIE_IMPL true

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

void DoMovie_Tests()
{
    ASSERT_STREQ("./movie/null.ddv", Res_movie_GetName_4564F5(".", ResourceNameHash("lols")));
    ASSERT_STREQ("./movie/genbaku.ddv", Res_movie_GetName_4564F5(".", ResourceNameHash("genbaku")));
}
