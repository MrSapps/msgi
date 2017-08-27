#include "stdafx.h"
#include "Actor.hpp"
#include "MgsFunction.hpp"

struct PauseKill
{
    WORD mPause;
    WORD mKill;
};
MSG_ASSERT_SIZEOF(PauseKill, 0x4);

MGS_ARY(1, 0x6507EC, PauseKill, 9, gPauseKills,
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
MGS_VAR(1, 0x791A0C, DWORD, flags_dword_791A0C, 0);
MGS_VAR(1, 0x9942A0, DWORD, dword_9942A0, 0);

void CC Actor_DumpActorSystem()
{
    printf("--DumpActorSystem--\n");
    ActorList* pActorList = gActorsList;
    for (int i = 9; i > 0; --i)
    {
        printf("Lv %d Pause %d Kill %d\n", i, pActorList->mPause, pActorList->mKill);
        Actor* pActor = &pActorList->first;
        while (pActor)
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
        }
        pActorList++;
    }
}
MSG_FUNC_IMPL(0x0040A0D4, Actor_DumpActorSystem);

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
MSG_FUNC_IMPL(0x0040A347, Actor_Init);

Actor* CC Actor_PushBack(int level, Actor* pActor, TActorFunction fnFree)
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
MSG_FUNC_IMPL(0x0040A2AF, Actor_PushBack);

void CC Actor_KillActorsAtLevel(signed int killLevel)
{
    ActorList* pActorList = gActorsList;
    for (int i = 9; i > 0; --i)
    {
        if (pActorList->mKill <= killLevel)       // only known to be called at level 4
        {
            Actor* pActor = &pActorList->first;
            while (pActor)
            {
                if (pActor->mFnUpdate || pActor->mFnShutdown)
                {
                    Actor_DestroyOnNextUpdate(pActor);
                }
                pActor = pActor->pNext;
            } 
        }
        pActorList++;
    }
}
MSG_FUNC_IMPL(0x0040A23D, Actor_KillActorsAtLevel);

void CC Actor_DestroyOnNextUpdate(Actor* pActor)
{
    pActor->mFnUpdate = Actor_Destroy;
}
MSG_FUNC_IMPL(0x0040A3ED, Actor_DestroyOnNextUpdate);

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
MSG_FUNC_IMPL(0x0040A37C, Actor_Destroy);

void CC Actor_UpdateActors()
{
    ActorList* pActorList = gActorsList;
    for (int i = 9; i > 0; --i)
    {
        if (!(flags_dword_791A0C & pActorList->mPause))
        {
            Actor* pActor = &pActorList->first;
            while (pActor)
            {
                if (pActor->mFnUpdate)
                {
                    pActor->mFnUpdate(pActor);
                }
                dword_9942A0 = 0;
                pActor = pActor->pNext;
            }
        }
        pActorList++;
    }
}
MSG_FUNC_IMPL(0x0040A1BF, Actor_UpdateActors);

void CC ActorList_Init()
{
    ActorList* pActorList = gActorsList;

    for (int i = 0; i < 9; i++)
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

    flags_dword_791A0C = 0;
}
MSG_FUNC_IMPL(0x0040A006, ActorList_Init);
