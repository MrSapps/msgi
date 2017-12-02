#include "stdafx.h"
#include "Actor.hpp"
#include "Script.hpp"

void Actor_DelayCpp_ForceLink() { }

#define ACTOR_DELAY_IMPL true

struct Actor_Delay
{
    Actor mBase;
    DWORD mProcIdOrScriptPtr;
    GCL_Proc_Arguments mGclProcArgs;
    int mDelayCounter;
    DWORD mActive;
    DWORD field_34_args_array[8];
};
MGS_ASSERT_SIZEOF(Actor_Delay, 0x54);

void CC Actor_Delay_Update_455451(Actor_Delay* pDelay)
{
    if (pDelay->mActive || !gGameOverTimer_dword_7227A4)
    {
        if (--pDelay->mDelayCounter > 0)
        {
            return;
        }

        if (pDelay->mProcIdOrScriptPtr <= 0xFFFF)
        {
            // If less than MAX_WORD its a proc id
            Script_ProcCancelOrRun(static_cast<WORD>(pDelay->mProcIdOrScriptPtr), &pDelay->mGclProcArgs);
        }
        else
        {
            // Else its a script ip ptr
            Script_Run(reinterpret_cast<BYTE*>(pDelay->mProcIdOrScriptPtr), &pDelay->mGclProcArgs);
        }
    }
    Actor_DestroyOnNextUpdate(&pDelay->mBase);
}
MGS_FUNC_IMPLEX(0x00455451, Actor_Delay_Update_455451, ACTOR_DELAY_IMPL);

void CC Res_delay_84_sub_4553D1(DWORD scriptVar_p, GCL_Proc_Arguments* pArgs, int scriptVar_t)
{
    Actor_Delay* pDelay = Actor_ResourceAllocT<Actor_Delay>(6);
    if (pDelay)
    {
        if (pArgs)
        {
            pDelay->mGclProcArgs.mPArgs = pDelay->field_34_args_array;
            pDelay->mGclProcArgs.mNumArgs = pArgs->mNumArgs;
            for (int i = 0; i < pArgs->mNumArgs; i++)
            {
                pDelay->field_34_args_array[i] = pArgs->mPArgs[i];
            }
        }
        else
        {
            pDelay->mGclProcArgs.mNumArgs = 0;
        }

        int scriptVar_t2 = scriptVar_t;
        if (scriptVar_t >= 0)
        {
            pDelay->mActive = 0;
        }
        else
        {
            pDelay->mActive = 1;
            scriptVar_t2 = -scriptVar_t;
        }
        pDelay->mDelayCounter = scriptVar_t2;
        pDelay->mProcIdOrScriptPtr = scriptVar_p;
        Actor_Init(&pDelay->mBase, reinterpret_cast<TActorFunction>(Actor_Delay_Update_455451), nullptr, "C:\\mgs\\source\\Game\\delay.c");
    }
}
MGS_FUNC_IMPLEX(0x004553D1, Res_delay_84_sub_4553D1, ACTOR_DELAY_IMPL);

signed int CC Script_tbl_Delay_sub_4519C7(BYTE* /*pScript*/)
{
    int scriptVar_t = 0;
    int scriptVar_p = 0;
    if (Script_ParamExists('t'))
    {
        scriptVar_t = Script_get_int();
    }

    if (Script_ParamExists('p'))
    {
        scriptVar_p = Script_get_int();
    }

    if (Script_ParamExists('e'))
    {
        BYTE* pScriptAddress = Script_GetReturnAddress();
        DWORD notUsed = 0;
        DWORD execRet = 0;
        Script_GCL_Execute(pScriptAddress, &notUsed, &execRet);
        scriptVar_p = execRet;
    }

    if (Script_ParamExists('g'))
    {
        scriptVar_t = -scriptVar_t;
    }

    if (scriptVar_t && scriptVar_p)
    {
        Res_delay_84_sub_4553D1(scriptVar_p, 0, scriptVar_t);
        return 0;
    }
    return -1;
}
MGS_FUNC_IMPLEX(0x004519C7, Script_tbl_Delay_sub_4519C7, ACTOR_DELAY_IMPL);
