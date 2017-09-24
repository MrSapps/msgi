#pragma once

#include "Actor.hpp"
#include "Psx.hpp"

struct Prim_unknown;

struct Actor_Debug
{
    Actor mBase;
    Prim_unknown* mPrimData;
    POLY_F4 mPolyF4;
};

void AddDebugActor();
