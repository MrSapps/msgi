#include "stdafx.h"
#include "Actor.hpp"
#include "MgsFunction.hpp"

//actor_related_struct* gActors = (actor_related_struct*)0x006BFC78; // Array of 9 items, TODO: Check correct
MGS_ARY(1, 0x006BFC78, ActorList, 9, gActors, {});
MGS_VAR(1, 0x791A0C, DWORD, dword_791A0C, 0);
MGS_VAR(1, 0x9942A0, DWORD, dword_9942A0, 0);

//MSG_FUNC_NOT_IMPL(0x0040A0D4, int __cdecl(), Actor_DumpActorSystem);
int __cdecl Actor_DumpActorSystem()
{
    int result; // eax@1
    int v1; // [sp+0h] [bp-18h]@6
    Actor* pNextActor; // [sp+4h] [bp-14h]@4
    Actor* pActorCopy; // [sp+8h] [bp-10h]@3
    signed int i; // [sp+10h] [bp-8h]@1
    ActorList* pActor; // [sp+14h] [bp-4h]@1

    pActor = gActors;
    result = printf("--DumpActorSystem--\n");
    for (i = 0; i < 9; ++i)
    {
        printf("Lv %d Pause %d Kill %d\n", i, pActor->mPause, pActor->mKill);
        pActorCopy = &pActor->first;
        do
        {
            pNextActor = pActorCopy->pNext;
            if (pActorCopy->update)
            {
                if (pActorCopy->field_1C <= 0)
                    v1 = 0;
                else
                    v1 = 100 * pActorCopy->field_18 / pActorCopy->field_1C;
                printf("Lv%d %04d.%02d %p %s\n", i, v1 / 100, v1 % 100, pActorCopy->update, pActorCopy->mNamePtr);
                pActorCopy->field_1C = 0;
                pActorCopy->field_18 = 0;
            }
            pActorCopy = pNextActor;
        } while (pNextActor);
        ++pActor;
        result = i + 1;
    }
    return result;
}


//MSG_FUNC_NOT_IMPL(0x0040A347, Actor* __cdecl (Actor*, (void(__cdecl *)(Actor*)), (void(__cdecl *)(Actor*)), char*), Actor_Init);
Actor* __cdecl Actor_Init(Actor* a1, void(__cdecl *update)(Actor*), void(__cdecl *fn2)(Actor*), char *srcFileName)
{
    a1->update = update;
    a1->fnUnknown3 = fn2;
    a1->mNamePtr = srcFileName;
    a1->field_1C = 0;
    a1->field_18 = 0;
    return a1;
}
MSG_FUNC_IMPL(0x0040A347, Actor_Init);


//MSG_FUNC_NOT_IMPL(0x0040A2AF, Actor* __cdecl(int, Actor*, void(__cdecl *)(Actor*)), Actor_PushBack);
Actor* __cdecl Actor_PushBack(int a_nLvl, Actor* a_pActor, void(__cdecl *fn)(Actor*))
{
    Actor* pLast = &gActors[a_nLvl].last;
    Actor* pLastPrevious = pLast->pPrevious;
    pLast->pPrevious = a_pActor;
    pLastPrevious->pNext = a_pActor;
    a_pActor->pNext = pLast;
    a_pActor->pPrevious = pLastPrevious;
    a_pActor->fnUnknown3 = 0;
    a_pActor->update = 0;
    a_pActor->fnUnknown2 = fn;

    return a_pActor;
}

//MSG_FUNC_NOT_IMPL(0x0040A37C, void __cdecl (Actor*), Actor_Remove);
void __cdecl Actor_Remove(Actor* pActor)
{
    if (!pActor)
        return;

    pActor->pNext->pPrevious = pActor->pPrevious;
    pActor->pPrevious->pNext = pActor->pNext;
    pActor->pPrevious = 0;
    pActor->pNext = 0;

    if (pActor->fnUnknown3)
    {
        pActor->fnUnknown3(pActor);
    }

    if (pActor->fnUnknown2)
    {
        pActor->fnUnknown2(pActor);
    }
}


//MSG_FUNC_NOT_IMPL(0x0040A1BF, int __cdecl(), Actor_UpdateActors);
int __cdecl Actor_UpdateActors()
{
    int result; // eax@8
    Actor *v1; // [sp+0h] [bp-18h]@5
    Actor *v2; // [sp+4h] [bp-14h]@4
    signed int i; // [sp+10h] [bp-8h]@1
    ActorList *pActor; // [sp+14h] [bp-4h]@1

    pActor = gActors;


    for (i = 9; i > 0; --i)
    {
        if (!(dword_791A0C & pActor->mPause))
        {
            v2 = &pActor->first;
            do
            {
                v1 = v2->pNext;
                auto fn = v2->update;

                // bool isFamasFunc = fn == (void*)0x640CDC;

                if (fn)
                {
                    fn(v2);
                }
                dword_9942A0 = 0;
                v2 = v1;
            } while (v1);
        }
        ++pActor;
        result = i - 1;
    }
    return result;
}

struct PauseKill
{
    WORD mPause;
    WORD mKill;
};

MGS_ARY(1, 0x6507EC, PauseKill, 9, gPauseKills, { { 0, 7 },{ 0, 7 },{ 9, 4 },{ 9, 4 },{ 0xF, 4 },{ 0xF, 4 },{ 0xF, 4 },{ 9, 4 },{ 0, 7 } });

//MSG_FUNC_NOT_IMPL(0x0040A006, void __cdecl(), ActorList_Init);
void __cdecl ActorList_Init()
{
    ActorList* pActor = gActors;

    for (int i = 0; i < 9; i++)
    {
        pActor->first.pPrevious = 0;
        pActor->first.pNext = &pActor->last;
        pActor->last.pPrevious = &pActor->first;
        pActor->last.pNext = 0;

        pActor->first.fnUnknown2 = 0;
        pActor->first.fnUnknown3 = 0;
        pActor->last.fnUnknown2 = 0;
        pActor->last.fnUnknown3 = 0;

        pActor->mPause = gPauseKills[i].mPause;
        pActor->mKill = gPauseKills[i].mKill;

        pActor++;
    }

    dword_791A0C = 0;
}
