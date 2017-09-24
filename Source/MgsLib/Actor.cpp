#include "stdafx.h"
#include "Actor.hpp"
#include "MgsFunction.hpp"

#define ACTOR_IMPL true

struct PauseKill
{
    u16 mPause;
    u16 mKill;
};
MGS_ASSERT_SIZEOF(PauseKill, 0x4);

MGS_ARY(1, 0x6507EC, const PauseKill, 9, gPauseKills,
{ 
    { 0,  7 },
    { 0,  7 },
    { 9,  4 },
    { 9,  4 },
    { 15, 4 },
    { 15, 4 },
    { 15, 4 },
    { 9,  4 },
    { 0,  7 } 
});
MGS_ARY(1, 0x006BFC78, ActorList, 9, gActorsList, {});
MGS_VAR(1, 0x791A0C, u32, gActorPauseFlags_dword_791A0C, 0);
MGS_VAR(1, 0x9942A0, u32, dword_9942A0, 0);

template<class bShouldEnterList, class bShouldContinue>
static bool IterateActors(bShouldEnterList fnEnterList, bShouldContinue fnShouldContinue)
{
    ActorList* pActorList = gActorsList;
    for (s32 i = 9; i > 0; --i)
    {
        if (fnEnterList(pActorList))
        {
            Actor* pNext = nullptr;
            Actor* pActor = &pActorList->first;
            do
            {
                pNext = pActor->pNext;
                if (!fnShouldContinue(pActor))
                {
                    return true;
                }
                dword_9942A0 = 0;
                pActor = pNext;
            } while (pNext);
        }
        ++pActorList;
    }
    return false;
}

Actor* CC Actor_ResourceAlloc(s32 actor_system_idx, s32 size)
{
    UNREFERENCED_PARAMETER(actor_system_idx);
    UNREFERENCED_PARAMETER(size);
    return nullptr;
}
MGS_FUNC_IMPLEX(0x0040A30C, Actor_ResourceAlloc, false); // TODO: Implement me

void CC Actor_DumpActorSystem()
{
    printf("--DumpActorSystem--\n");

    s32 i = 0;
    auto fnEnterActorList = [&](ActorList* pActorList)
    {
        i++;
        printf("Lv %d Pause %d Kill %d\n", i, pActorList->mPause, pActorList->mKill);
        return true;
    };

    auto fnUpdateActor = [&](Actor* pActor)
    {
        if (pActor->mFnUpdate)
        {
            int unknown = 0;
            if (pActor->field_1C <= 0)
            {
                unknown = 0;
            }
            else
            {
                // TODO: I've yet to see this condition be hit - perhaps an unused feature of the actor system?
                unknown = 100 * pActor->field_18 / pActor->field_1C;
            }
            printf("Lv%d %04d.%02d %p %s\n", i, unknown / 100, unknown % 100, pActor->mFnUpdate, pActor->mName);
            pActor->field_1C = 0;
            pActor->field_18 = 0;
        }
        pActor = pActor->pNext;
        return true;
    };

    IterateActors(fnEnterActorList, fnUpdateActor);
}
MGS_FUNC_IMPLEX(0x0040A0D4, Actor_DumpActorSystem, ACTOR_IMPL);

Actor* CC Actor_Init(Actor* pActor, TActorFunction fnUpdate, TActorFunction fnShutDown, const char* srcFileName)
{
    if (srcFileName)
    {
        LOG_INFO("Actor init: " << srcFileName);
    }

    pActor->mFnUpdate = fnUpdate;
    pActor->mFnShutdown = fnShutDown;
    pActor->mName = srcFileName;
    pActor->field_1C = 0;
    pActor->field_18 = 0;
    return pActor;
}
MGS_FUNC_IMPLEX(0x0040A347, Actor_Init, ACTOR_IMPL);

Actor* CC Actor_PushBack(s32 level, Actor* pActor, TActorFunction fnFree)
{
    Actor* pLast = &gActorsList[level].last;
    Actor* pLastPrevious = pLast->pPrevious;

    pLast->pPrevious = pActor;
    pLastPrevious->pNext = pActor;

    pActor->pNext = pLast;
    pActor->pPrevious = pLastPrevious;

    pActor->mFnShutdown = nullptr;
    pActor->mFnUpdate = nullptr;
    pActor->mFreeFunc = fnFree;

    return pActor;
}
MGS_FUNC_IMPLEX(0x0040A2AF, Actor_PushBack, ACTOR_IMPL);

void CC Actor_KillActorsAtLevel(s32 killLevel)
{
    auto fnEnterActorList = [&](ActorList* pActorList)
    {
        return pActorList->mKill <= killLevel;
    };

    auto fnUpdateActor = [](Actor* pActor)
    {
        if (pActor->mFnUpdate || pActor->mFnShutdown)
        {
            Actor_DestroyOnNextUpdate(pActor);
        }
        return true;
    };

    IterateActors(fnEnterActorList, fnUpdateActor);
}
MGS_FUNC_IMPLEX(0x0040A23D, Actor_KillActorsAtLevel, ACTOR_IMPL);

void CC Actor_DestroyOnNextUpdate(Actor* pActor)
{
    pActor->mFnUpdate = Actor_Destroy;
}
MGS_FUNC_IMPLEX(0x0040A3ED, Actor_DestroyOnNextUpdate, ACTOR_IMPL);

void CC Actor_Destroy(Actor* pActor)
{
    if (!pActor)
    {
        return;
    }

    pActor->pNext->pPrevious = pActor->pPrevious;
    pActor->pPrevious->pNext = pActor->pNext;
    pActor->pPrevious = 0;
    pActor->pNext = 0;

    // Same as C++ destructor
    if (pActor->mFnShutdown)
    {
        pActor->mFnShutdown(pActor);
    }

    // Memory freeing function
    if (pActor->mFreeFunc)
    {
        pActor->mFreeFunc(pActor);
    }
}
MGS_FUNC_IMPLEX(0x0040A37C, Actor_Destroy, ACTOR_IMPL);

void CC Actor_UpdateActors()
{
    auto fnEnterActorList = [](ActorList* pActorList)
    {
        return !(gActorPauseFlags_dword_791A0C & pActorList->mPause);
    };

    auto fnUpdateActor = [](Actor* pActor)
    {
        if (pActor->mFnUpdate)
        {
            pActor->mFnUpdate(pActor);
        }
        return true;
    };

    IterateActors(fnEnterActorList, fnUpdateActor);
}
MGS_FUNC_IMPLEX(0x0040A1BF, Actor_UpdateActors, ACTOR_IMPL);

void CC ActorList_Init()
{
    ActorList* pActorList = gActorsList;

    for (s32 i = 0; i < 9; i++)
    {
        pActorList->first.pPrevious = 0;
        pActorList->first.pNext = &pActorList->last;
        pActorList->last.pPrevious = &pActorList->first;
        pActorList->last.pNext = 0;

        pActorList->first.mFreeFunc = 0;
        pActorList->first.mFnShutdown = 0;
        pActorList->last.mFreeFunc = 0;
        pActorList->last.mFnShutdown = 0;

        pActorList->mPause = gPauseKills[i].mPause;
        pActorList->mKill = gPauseKills[i].mKill;

        pActorList++;
    }

    gActorPauseFlags_dword_791A0C = 0;
}
MGS_FUNC_IMPLEX(0x0040A006, ActorList_Init, ACTOR_IMPL);

ActorList* CC ActorList_Set_KillPause(s32 index, s16 pause, s16 kill)
{
    gActorsList[index].mPause = pause;
    gActorsList[index].mKill = kill;
    return &gActorsList[index];
}
MGS_FUNC_IMPLEX(0x0040A0AA, ActorList_Set_KillPause, ACTOR_IMPL);

void CC Actor_Remove(Actor* pActorToRemove)
{
    auto fnEnterActorList = [](ActorList*)
    {
        return true;
    };

    auto fnUpdateActor = [&](Actor* pActor)
    {
        if (pActor == pActorToRemove)
        {
            Actor_DestroyOnNextUpdate(pActor);
            return false;
        }
        return true;
    };

    if (!IterateActors(fnEnterActorList, fnUpdateActor))
    {
        printf("#");
    }
}
MGS_FUNC_IMPLEX(0x0040A3FC, Actor_Remove, ACTOR_IMPL);
