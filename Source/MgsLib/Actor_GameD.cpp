#include "stdafx.h"
#include "Actor_GameD.hpp"
#include "Script.hpp"
#include "LibDG.hpp"
#include "LibGV.hpp"
#include "Actor_Debug.hpp"

#define ACTOR_GAMED_IMPL true

MGS_VAR(1, 0x722760, Actor, g_gamed_722760, {}); // TODO: Will actually big an Actor + other data
MGS_VAR(1, 0x995344, DWORD, gFrameTime_dword_995344, 0);
MGS_VAR(1, 0x7227A0, DWORD, script_cancel_non_zero_dword_7227A0, 0);
MGS_VAR(1, 0x9942B8, DWORD, dword_9942B8, 0);
// TODO: Is a global that inherits from Actor
MGS_VAR(1, 0x725FC0, Actor, gMenuMan_stru_725FC0, {});
MGS_ARY(1, 0x7227C8, WORD, 5, word_7227C8, {}); // TODO: Struct?
MGS_VAR(1, 0x78E7FE, short, word_78E7FE, 0);
MGS_VAR(1, 0x78E7FC, short, gLoadItemFuncIdx_word_78E7FC, 0);



MGS_VAR(1, 0x78E7E8, WORD, word_78E7E8, 0);
MGS_VAR(1, 0x78D7B0, int, dword_78D7B0, 0);
MGS_VAR(1, 0x995324, DWORD, dword_995324, 0);
MGS_VAR(1, 0x7919C0, DWORD, dword_7919C0, 0);
MGS_VAR(1, 0x78E960, DWORD, gResidentTop_dword_78E960, 0);
MGS_VAR(1, 0x722780, DWORD, gamed_unk_722780, 0);
MGS_VAR(1, 0x722784, DWORD, gamed_unk_722784, 0);

MSG_FUNC_NOT_IMPL(0x0044E212, void* __cdecl(), sub_44E212);
MSG_FUNC_NOT_IMPL(0x0044E287, void __cdecl(), sub_44E287);
MSG_FUNC_NOT_IMPL(0x0044E1F9, int __cdecl(), sub_44E1F9); // Note: Not a CRT func!!
MSG_FUNC_NOT_IMPL(0x0044E381, void CC(Actor*), GameD_Update_44E381);


void CC Res_MenuMan_create_459A9A()
{
    Actor_PushBack(1, &gMenuMan_stru_725FC0, nullptr);
    Actor_Init(&gMenuMan_stru_725FC0, nullptr, nullptr, "C:\\mgs\\source\\Menu\\menuman.c");
}
MSG_FUNC_IMPLEX(0x00459A9A, Res_MenuMan_create_459A9A, ACTOR_GAMED_IMPL);

void* CC Stage_GetNameHashStack_44EAED()
{
    return memset(word_7227C8, 0, 0x10u);
}
MSG_FUNC_IMPLEX(0x0044EAED, Stage_GetNameHashStack_44EAED, ACTOR_GAMED_IMPL);

void CC sub_44E1E0()
{
    gLoadItemFuncIdx_word_78E7FC = -1;
    word_78E7FE = -1;
}
MSG_FUNC_IMPLEX(0x0044E1E0, sub_44E1E0, ACTOR_GAMED_IMPL);

int CC GetResidentTop()
{
    return gResidentTop_dword_78E960;
}
MSG_FUNC_IMPLEX(0x0040B36E, GetResidentTop, ACTOR_GAMED_IMPL);

MGS_VAR(1, 0x78E7EC, WORD, stage_name_hash_word_78E7EC, 0);
MGS_VAR(1, 0x6893D4, DWORD, dword_6893D4, 0);

MSG_FUNC_NOT_IMPL(0x0044EB83, char* CC(), File_StageName_44EB83);
MSG_FUNC_NOT_IMPL(0x00457BDD, void CC(const char*), Res_loader_Create_457BDD);

void CC Create_loader_44E226()
{
    static char sNoFolderName_669A88[13] = "nofoldername"; // 0x669A88
    const char* strStageName = "init";
    if (stage_name_hash_word_78E7EC)
    {
        strStageName = File_StageName_44EB83();
    }

    if (strcmp(strStageName, sNoFolderName_669A88)) // TODO: FIX MEReal game bug? this attempts to overwrite global data?
    {
        dword_6893D4 = 1;
        strcpy(sNoFolderName_669A88, strStageName);
    }
    Res_loader_Create_457BDD(strStageName);
}
MSG_FUNC_IMPLEX(0x0044E226, Create_loader_44E226, ACTOR_GAMED_IMPL);

void CC Init_Gamed_Create_44E12B()
{
    gFrameTime_dword_995344 = 0;
    dword_7227A4 = 0;
    script_cancel_non_zero_dword_7227A0 = 0;
    dword_9942B8 = 0;
    Res_MenuMan_create_459A9A();
    Stage_GetNameHashStack_44EAED();
    LibDG_SetActiveResourceInitFuncPtrs_457B5B();
    Script_BindInits_452610();
    LibGV_SetFnPtr_sub_40A68D('b', (GV_FnPtr)LibDG_CHARA_44E9D2);
    sub_44E1E0();
    Actor_PushBack(1, &g_gamed_722760, nullptr);
    Actor_Init(&g_gamed_722760, GameD_Update_44E381.Ptr(), nullptr, "C:\\mgs\\source\\Game\\gamed.c");

    sub_44E1F9();
    sub_44E287();
    sub_44E212();
    word_78E7E8 = (WORD)(dword_78D7B0 + 1);
    dword_995324 = (int)&dword_7919C0;
    GetResidentTop();
    gamed_unk_722780 = 0;
    gamed_unk_722784 = 0;

    Create_loader_44E226();
   // AddDebugActor();
}
MSG_FUNC_IMPLEX(0x44E12B, Init_Gamed_Create_44E12B, ACTOR_GAMED_IMPL);
