#include "stdafx.h"

#include <gmock/gmock.h>

#include "Actor.hpp"

TEST(Actor, ActorList_Init)
{
    ActorList_Init();
}

TEST(Actor, Actor_PushBack)
{
    Actor actor = {};
    actor.mName = "Test actor";
    // Actor_Init

    ActorList_Init();
    Actor_PushBack(0, &actor, nullptr);

    Actor_DestroyOnNextUpdate(&actor);

    Actor_UpdateActors();

    //Actor_Remove(&pActor);
    //Actor_KillActorsAtLevel(2);
    // Actor_DumpActorSystem
    // ActorList_Set_KillPause
} 
