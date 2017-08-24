#pragma once

#include <windows.h>

struct Actor
{
    Actor* pPrevious;
    Actor* pNext;
    void(__cdecl *update)(Actor*);
    void(__cdecl *fnUnknown3)(Actor*);
    void(__cdecl *fnUnknown2)(Actor*);
    char* mNamePtr;
    DWORD field_18;
    DWORD field_1C;
};
static_assert(sizeof(Actor) == 0x20, "Actor should be 0x20");

struct ActorList
{
    Actor first;
    Actor last;
    WORD mPause;
    WORD mKill;
};
static_assert(sizeof(ActorList) == 0x44, "ActorList should be 0x44");

Actor* __cdecl Actor_Init(Actor* a1, void(__cdecl *update)(Actor*), void(__cdecl *fn2)(Actor*), char *srcFileName);
Actor* __cdecl Actor_PushBack(int a_nLvl, Actor* a_pActor, void(__cdecl *fn)(Actor*));
void __cdecl Actor_Remove(Actor* pActor);
int __cdecl Actor_UpdateActors();
void __cdecl ActorList_Init();
int __cdecl Actor_DumpActorSystem();
