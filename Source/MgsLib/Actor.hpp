#pragma once

#include <windows.h>
#include "MgsFunction.hpp"

struct Actor;
using TActorFunction = void(CC*)(Actor*);

struct Actor
{
    Actor* pPrevious;
    Actor* pNext;
    TActorFunction mFnUpdate;
    TActorFunction mFnShutdown;
    TActorFunction mFreeFunc;
    const char* mName;
    DWORD field_18;
    DWORD field_1C;
};
MSG_ASSERT_SIZEOF(Actor, 0x20);

struct ActorList
{
    Actor first;
    Actor last;
    WORD mPause;
    WORD mKill;
};
MSG_ASSERT_SIZEOF(ActorList, 0x44);

MGS_VAR_EXTERN(DWORD, gActorPauseFlags_dword_791A0C);

// Helper to avoid casting all over the place
template<class T>
inline T* Actor_ResourceAllocT(int actor_system_idx)
{
    Actor* pActor = Actor_ResourceAlloc(actor_system_idx, sizeof(T));
    return reinterpret_cast<T*>(pActor);
}

Actor* CC Actor_ResourceAlloc(int actor_system_idx, int size);
Actor* CC Actor_Init(Actor* pActor, TActorFunction fnUpdate, TActorFunction fnShutDown, const char *srcFileName);
Actor* CC Actor_PushBack(int level, Actor* pActor, TActorFunction fnFree);
void CC Actor_KillActorsAtLevel(signed int killLevel);
void CC Actor_DestroyOnNextUpdate(Actor* pActor);
void CC Actor_Destroy(Actor* pActor);
void CC Actor_UpdateActors();
void CC ActorList_Init();
void CC Actor_DumpActorSystem();
ActorList* CC ActorList_Set_KillPause(int index, __int16 pause, __int16 kill);
void CC Actor_Remove(Actor* pActorToRemove);
