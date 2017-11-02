#include "stdafx.h"
#include "Actor_GameD.hpp"
#include "Script.hpp"
#include "LibDG.hpp"
#include "LibGV.hpp"
#include "Actor_Debug.hpp"
#include "Actor_Loader.hpp"
#include "System.hpp"

#define ACTOR_GAMED_IMPL true

struct GameD_Struct
{
    Actor mBase;
    DWORD gamed_unk_722780;
    DWORD gamed_unk_722784;
};
MGS_ASSERT_SIZEOF(GameD_Struct, 0x28);

MGS_VAR(1, 0x722760, GameD_Struct, gGameD_stru_722760, {});
MGS_VAR(1, 0x995344, DWORD, gFrameTime_dword_995344, 0);
MGS_VAR(1, 0x7227A0, DWORD, script_cancel_non_zero_dword_7227A0, 0);
MGS_VAR(1, 0x9942B8, int, gLoaderState_dword_9942B8, 0);
// TODO: Is a global that inherits from Actor
MGS_VAR(1, 0x725FC0, Actor, gMenuMan_stru_725FC0, {});
MGS_ARY(1, 0x7227C8, WORD, 5, word_7227C8, {}); // TODO: Struct?
MGS_VAR(1, 0x78E7FE, short, word_78E7FE, 0);
MGS_VAR(1, 0x78E7FC, short, gLoadItemFuncIdx_word_78E7FC, 0);



MGS_VAR(1, 0x78E7E8, WORD, word_78E7E8, 0);
MGS_VAR(1, 0x78D7B0, int, dword_78D7B0, 0);
MGS_VAR(1, 0x995324, DWORD, dword_995324, 0);
MGS_VAR(1, 0x7919C0, DWORD, dword_7919C0, 0);
MGS_VAR(1, 0x78E960, BYTE*, gResidentTop_dword_78E960, 0);
MGS_VAR(1, 0x78E964, BYTE*, gSavedTop_78E964, 0);


MGS_FUNC_NOT_IMPL(0x0044E287, void __cdecl(), sub_44E287);
MGS_FUNC_NOT_IMPL(0x0044E1F9, int __cdecl(), sub_44E1F9); // Note: Not a CRT func!!
MGS_FUNC_NOT_IMPL(0x0044E381, void CC(Actor*), GameD_Update_44E381);

void* CC ResidentTopAllocate_40B379(int size)
{
    const int alignedSize = RoundUpPowerOf2(size, 4);
    gResidentTop_dword_78E960 -= alignedSize;
    return gResidentTop_dword_78E960;
}
MGS_FUNC_IMPLEX(0x0040B379, ResidentTopAllocate_40B379, ACTOR_GAMED_IMPL);

void CC Res_MenuMan_create_459A9A()
{
    Actor_PushBack(1, &gMenuMan_stru_725FC0, nullptr);
    Actor_Init(&gMenuMan_stru_725FC0, nullptr, nullptr, "C:\\mgs\\source\\Menu\\menuman.c");
}
MGS_FUNC_IMPLEX(0x00459A9A, Res_MenuMan_create_459A9A, ACTOR_GAMED_IMPL);

void* CC Stage_GetNameHashStack_44EAED()
{
    return memset(word_7227C8, 0, 0x10u);
}
MGS_FUNC_IMPLEX(0x0044EAED, Stage_GetNameHashStack_44EAED, ACTOR_GAMED_IMPL);

void CC sub_44E1E0()
{
    gLoadItemFuncIdx_word_78E7FC = -1;
    word_78E7FE = -1;
}
MGS_FUNC_IMPLEX(0x0044E1E0, sub_44E1E0, ACTOR_GAMED_IMPL);

void CC SaveResidentTop()
{
    gSavedTop_78E964 = gResidentTop_dword_78E960;
}
MGS_FUNC_IMPLEX(0x0040B36E, SaveResidentTop, ACTOR_GAMED_IMPL);

MGS_VAR(1, 0x78E7EC, WORD, stage_name_hash_word_78E7EC, 0);
MGS_VAR(1, 0x6893D4, DWORD, dword_6893D4, 0);

MGS_FUNC_NOT_IMPL(0x0044EB83, char* CC(), File_StageName_44EB83);

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
MGS_FUNC_IMPLEX(0x0044E226, Create_loader_44E226, ACTOR_GAMED_IMPL);

MGS_FUNC_NOT_IMPL(0x402487, void CC(), LibDG_ClearTexturesCache_402487);
MGS_FUNC_NOT_IMPL(0x40274C, void CC(), LibDG_Compact_Texture_Records_40274C);


void CC LibDG_Clean_Texture_Cache_401110()
{
    LibDG_ClearTexturesCache_402487();
    LibDG_Compact_Texture_Records_40274C();
}
MGS_FUNC_IMPLEX(0x00401110, LibDG_Clean_Texture_Cache_401110, ACTOR_GAMED_IMPL);

MGS_ARY(1, 0x913E20, BYTE, 192512, gSystem0_memory_unk_913E20, {});
MGS_ARY(1, 0x942E20, BYTE, 192512, gSystem1_memory_unk_942E20, {});

void CC System_Init_0_And_1_40A465()
{
    System_init_40AC6C(0, 1, &gSystem0_memory_unk_913E20[0], 192512);
    System_init_40AC6C(1, 1, &gSystem1_memory_unk_942E20[0], 192512);
}
MGS_FUNC_IMPLEX(0x0040A465, System_Init_0_And_1_40A465, ACTOR_GAMED_IMPL);

MGS_ARY(1, 0x8A8E20, BYTE, 438272, gSystem2_memory_unk_8A8E20, {});

void CC LibGV_40A4BB()
{
    LibGV_RestoreFileCacheFromResident_40A72A();
    System_DeInit_Systems_0_to_2_sub_40AC52();
    System_Init_0_And_1_40A465();
    System_init_40AC6C(2, 0, &gSystem2_memory_unk_8A8E20[0], 438272);
    printf("RESIDENT TOP %X\n", gResidentTop_dword_78E960);
}
MGS_FUNC_IMPLEX(0x0040A4BB, LibGV_40A4BB, ACTOR_GAMED_IMPL);

void CC Reset_GV_DG_44E212()
{
    LibDG_Clean_Texture_Cache_401110();
    LibGV_40A4BB();
    LibDG_ClearActiveResourceFunctionPointerList_457B7C();
}
MGS_FUNC_IMPLEX(0x0044E212, Reset_GV_DG_44E212, ACTOR_GAMED_IMPL);

void CC Init_Gamed_Create_44E12B()
{
    gFrameTime_dword_995344 = 0;
    dword_7227A4 = 0;
    script_cancel_non_zero_dword_7227A0 = 0;
    gLoaderState_dword_9942B8 = 0;
    Res_MenuMan_create_459A9A();
    Stage_GetNameHashStack_44EAED();
    LibDG_SetActiveResourceInitFuncPtrs_457B5B();
    Script_BindInits_452610();
    LibGV_Set_FileExtHandler_40A68D('b', GV_bin_file_handler_44E9D2); // Handles loading ".bin" files which sets loadable objects table
    sub_44E1E0();
    Actor_PushBack(1, &gGameD_stru_722760.mBase, nullptr);
    Actor_Init(&gGameD_stru_722760.mBase, GameD_Update_44E381.Ptr(), nullptr, "C:\\mgs\\source\\Game\\gamed.c");

    sub_44E1F9();
    sub_44E287();
    Reset_GV_DG_44E212();
    word_78E7E8 = (WORD)(dword_78D7B0 + 1);
    dword_995324 = (int)&dword_7919C0;
    SaveResidentTop();
    gGameD_stru_722760.gamed_unk_722780 = 0;
    gGameD_stru_722760.gamed_unk_722784 = 0;

    Create_loader_44E226();
   // AddDebugActor();
}
MGS_FUNC_IMPLEX(0x44E12B, Init_Gamed_Create_44E12B, ACTOR_GAMED_IMPL);
