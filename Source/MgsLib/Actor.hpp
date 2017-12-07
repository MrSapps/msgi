#pragma once

#include <windows.h>
#include "MgsFunction.hpp"
#include "types.hpp"

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
    s32 field_18;
    s32 field_1C;
};
MGS_ASSERT_SIZEOF(Actor, 0x20);

struct ActorList
{
    Actor first;
    Actor last;
    s16 mPause;
    s16 mKill;
};
MGS_ASSERT_SIZEOF(ActorList, 0x44);

MGS_VAR_EXTERN(u32, gActorPauseFlags_dword_791A0C);

// Helper to avoid casting all over the place
template<class T>
inline T* Actor_ResourceAllocT(int actor_system_idx)
{
    Actor* pActor = Actor_ResourceAlloc_40A30C(actor_system_idx, sizeof(T));
    return reinterpret_cast<T*>(pActor);
}

Actor* CC Actor_ResourceAlloc_40A30C(s32 actor_system_idx, s32 size);
Actor* CC Actor_Init_40A347(Actor* pActor, TActorFunction fnUpdate, TActorFunction fnShutDown, const char *srcFileName);
Actor* CC Actor_PushBack_40A2AF(s32 level, Actor* pActor, TActorFunction fnFree);
void CC Actor_KillActorsAtLevel_40A23D(s32 killLevel);
void CC Actor_DestroyOnNextUpdate_40A3ED(Actor* pActor);
void CC Actor_Destroy_40A37C(Actor* pActor);
void CC Actor_UpdateActors_40A1BF();
void CC ActorList_Init_40A006();
void CC Actor_DumpActorSystem_40A0D4();
ActorList* CC ActorList_Set_KillPause_40A0AA(s32 index, s16 pause, s16 kill);
void CC Actor_Remove_40A3FC(Actor* pActorToRemove);
