#include "stdafx.h"

#include <gmock/gmock.h>

#include "Actor.hpp"

TEST(Actor, ActorList_Init_40A006)
{
    ActorList_Init_40A006();
}

TEST(Actor, Actor_PushBack_40A2AF)
{
    Actor actor = {};
    actor.mName = "Test actor";
    // Actor_Init

    ActorList_Init_40A006();
    Actor_PushBack_40A2AF(0, &actor, nullptr);

    Actor_DestroyOnNextUpdate_40A3ED(&actor);

    Actor_UpdateActors_40A1BF();

    //Actor_Remove(&pActor);
    //Actor_KillActorsAtLevel(2);
    // Actor_DumpActorSystem
    // ActorList_Set_KillPause
} 
