#include "stdafx.h"
#include "Actor_GameD.hpp"
#include "Script.hpp"
#include "LibDG.hpp"
#include "LibGV.hpp"
#include "Actor_Debug.hpp"
#include "Actor_Loader.hpp"
#include "System.hpp"
#include "Actor_Rank.hpp" // gGameTime_word_78E896

#define ACTOR_GAMED_IMPL true

struct GameD_Struct
{
    Actor mBase;
    DWORD gamed_unk_722780;
    DWORD gamed_unk_722784;
};
MGS_ASSERT_SIZEOF(GameD_Struct, 0x28);

MGS_VAR(1, 0x722760, GameD_Struct, gGameD_stru_722760, {});
MGS_VAR(1, 0x995344, DWORD, gTotalFrameTime_dword_995344, 0);
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

MGS_VAR(1, 0x9942A4, DWORD, dword_9942A4, 0);
MGS_VAR(1, 0x791E08, DWORD, dword_791E08, 0);
MGS_VAR(1, 0x7227C4, DWORD, dword_7227C4, 0);
MGS_VAR(1, 0x722794, DWORD, dword_722794, 0);


MGS_FUNC_NOT_IMPL(0x0044E287, void __cdecl(), sub_44E287);
MGS_FUNC_NOT_IMPL(0x0044E1F9, int __cdecl(), sub_44E1F9); // Note: Not a CRT func!!

MGS_FUNC_NOT_IMPL(0x00445610, DWORD CC(), GameD_Input_445610);
MGS_FUNC_NOT_IMPL(0x00521892, int CC(), sub_521892);
MGS_FUNC_NOT_IMPL(0x0044DEDE, int CC(), sub_44DEDE);
MGS_FUNC_NOT_IMPL(0x0044F3F7, void CC(), Map_ResetMapCountAndKmdsCount_44F3F7);
MGS_FUNC_NOT_IMPL(0x00453B11, void CC(), Res_game_camera_create_453B11);
MGS_FUNC_NOT_IMPL(0x00401A13, int CC(), ImageMove_401A13);
MGS_FUNC_NOT_IMPL(0x00455299, void CC(), sub_455299);
MGS_FUNC_NOT_IMPL(0x0040945D, void CC(), sub_40945D);
MGS_FUNC_NOT_IMPL(0x00459A95, void CC(), sub_459A95);
MGS_FUNC_NOT_IMPL(0x004553C5, void CC(), sub_4553C5);
MGS_FUNC_NOT_IMPL(0x00455017, void CC(), GM_SoundStart_455017);
MGS_FUNC_NOT_IMPL(0x0045837C, signed int CC(), File_GetStreamState_45837C);
MGS_FUNC_NOT_IMPL(0x0044E8F1, void CC(), sub_44E8F1);
MGS_FUNC_NOT_IMPL(0x0044F38D, void CC(), Map_FreeHzdItems_44F38D);
MGS_FUNC_NOT_IMPL(0x0044F3B5, void CC(), Map_FreeKmds_44F3B5);
MGS_FUNC_NOT_IMPL(0x0044E969, void CC(), sub_44E969);
MGS_FUNC_NOT_IMPL(0x004583BB, void CC(), StreamActorStop_4583BB);
MGS_FUNC_NOT_IMPL(0x0044F28B, void CC(), GameD_update_helper_44F28B);
MGS_FUNC_NOT_IMPL(0x004552CB, void CC(), GameD_Sound_4552CB);
MGS_FUNC_NOT_IMPL(0x004502CD, Actor* CC(__int16 bShowContinue), Res_over_create_4502CD);
MGS_FUNC_NOT_IMPL(0x0044E932, void CC(), sub_44E932);


void CC Create_loader_44E226();

// WinMain.cpp
MGS_VAR_EXTERN(DWORD, gFlags_dword_78E7E4);
MGS_VAR_EXTERN(DWORD, counter_dword_6BED20);
MGS_VAR_EXTERN(DWORD, game_state_dword_72279C);


void CC LibDG_Clean_Texture_Cache_401110();
void CC LibGV_40A4BB();

void CC Reset_GV_DG_44E212()
{
    LibDG_Clean_Texture_Cache_401110();
    LibGV_40A4BB();
    LibDG_ClearActiveResourceFunctionPointerList_457B7C();
}
MGS_FUNC_IMPLEX(0x0044E212, Reset_GV_DG_44E212, ACTOR_GAMED_IMPL);

static void GameD_Update_helper(DWORD buttons)
{
    if ((buttons & 0x90F) != 0x90F || dword_7227C4)
    {
        dword_722794 = 90;
    }
    else if (--dword_722794 < 0)
    {
        static char sPsxBinaryName_byte_722740[32] = {};
        //sprintf(sPsxBinaryName_byte_722740, "cdrom:\\MGS\\%s;1", (&gBinaries_off_650068)[4 * gBinaryIndex_dword_78D7B0]);
        SetDispMask(0);
        //nullsub_70();
        //nullsub_69();
        //Ret0_44AC80();
        //RetInput_6465B0(0);
        //nullsub_11(v6);
        //CloseEvents_40842A();
        Resetgraph_AndPrintPsxStructureSizes(3);
        //Ret0_6465E0();
        //Ret0_44D000();
        //Re0_6465D0();
        //Ret0_6465E0();
        //nullsub_18();
        //nullsub_17();
        for (;;)
        {
            printf("load %s\n", sPsxBinaryName_byte_722740);
            //Ret0_44CFD0();
        }
    }
    if (game_state_dword_72279C & 0x80000000)
    {
        LOG_WARNING("Not implemented");
        //abort();
        /* TODO
        if (gArrayPtr_dword_995324[2].field_2 & 0x840)
        {
        StreamActorStop_4583BB();
        }*/
    }
    if (GameD_Input_445610() & 0x20)
    {
        //nullsub_68();
        int v10 = 0;
        char v11[24];
        for (int i = 0; i < 24; ++i)
        {
            v10 = v11[i] & 1 | 2 * v10;
        }
    }

    if (!gActorPauseFlags_dword_791A0C)
    {
        sub_44E8F1();
    }
}

void CC GameD_update_44E381(GameD_Struct* pGameD)
{
    const DWORD buttons = GameD_Input_445610();

    gFlags_dword_78E7E4 |= 0x400u;

    if (!(gFlags_dword_78E7E4 & 0x2400)) // Dead code
    {
        //nullsub_10();
        int v14 = dword_99533C;
        if (dword_99533C > 255)
        {
            v14 = 255;
        }
        //nullsub_12();
    }

    dword_99533C = 0;
    dword_9942A4 = 0;

    if (gActorPauseFlags_dword_791A0C & 8)
    {
        if (dword_791E08 || sub_521892() == 3)
        {
            sub_44DEDE();
        }
        else
        {
            gActorPauseFlags_dword_791A0C = (gActorPauseFlags_dword_791A0C & 0xF7);
        }
    }
    else if (dword_791E08 || sub_521892() == 3)
    {
        gActorPauseFlags_dword_791A0C = (gActorPauseFlags_dword_791A0C | 8) & 0xFF;
    }

    if (!(gActorPauseFlags_dword_791A0C & 2))
    {
        gTotalFrameTime_dword_995344 += gFrameTime_dword_791A04;
        gGameTime_word_78E896 = static_cast<WORD>(gTotalFrameTime_dword_995344 / 60 / 3600); // TODO: Is DWORD?
        gGameTime_word_78E898 = static_cast<WORD>(gTotalFrameTime_dword_995344 / 60 % 3600); // TODO: Is DWORD?
    }

    const DWORD gamed_unk_722780 = pGameD->gamed_unk_722780;
    if (!gamed_unk_722780)
    {
        if (gLoaderState_dword_9942B8)            // -1 on LoadEnd, 0 on GameD/Loader created
        {
            gLoaderState_dword_9942B8 = 1;

            if (script_cancel_non_zero_dword_7227A0 & 0x80)
            {
                counter_dword_6BED20 = 0;
            }

            if (gFixupLibDg_Allocs_And_Hahses_dword_78D7AC)
            {
                LibGV_Save_File_Cache_Records_To_Resident_Memory_40A6CD();
                LibDG_Save_Texture_Hashes_To_Resident_Memory_4026F5();
            }

            Map_ResetMapCountAndKmdsCount_44F3F7();
            Res_game_camera_create_453B11();
            ImageMove_401A13();
            sub_455299();                             // reset array of data?
            
            if (script_cancel_non_zero_dword_7227A0 & 0x10)
            {
                sub_40945D();                           // reset save map + others?
            }

            printf("exec scenario\n");
            const int scriptDataCopy = script_cancel_non_zero_dword_7227A0;
            script_cancel_non_zero_dword_7227A0 = 0;
            if (scriptDataCopy & 0x20)
            {
                Script_ProcCancelOrRun(HIWORD(scriptDataCopy), 0);
            }
            else
            {
                Script_RunMainProc_409D49();
            }
            printf("end scenario\n");

            sub_459A95();                             // init array 11?
            sub_4553C5();                             // call proc id idx 2
            GM_SoundStart_455017();
            pGameD->gamed_unk_722780 = 1;
        }
        return;
    }

    if (gamed_unk_722780 != 1)
    {
        return;
    }

    if (pGameD->gamed_unk_722784 > 0)
    {
        gActorPauseFlags_dword_791A0C = gActorPauseFlags_dword_791A0C & 0xF7;
        if (--pGameD->gamed_unk_722784 <= 0)
        {
            if (File_GetStreamState_45837C() == -1)
            {
                if (gActorPauseFlags_dword_791A0C & 5)
                {
                    if (script_cancel_non_zero_dword_7227A0 & 0x80)
                    {
                        counter_dword_6BED20 = 0;
                    }
                    pGameD->gamed_unk_722784 = 1;

                    if (!gActorPauseFlags_dword_791A0C)
                    {
                        sub_44E8F1();
                    }
                    return;
                }
                pGameD->gamed_unk_722780 = 0;
                pGameD->gamed_unk_722784 = 0;
                Map_FreeHzdItems_44F38D();
                sub_44E1F9();
                sub_44E287();
                if (!(script_cancel_non_zero_dword_7227A0 & 0x40))
                {
                    Reset_GV_DG_44E212();
                    Create_loader_44E226();
                }
                return;
            }
            pGameD->gamed_unk_722784 = 1;
        }

        if (!gActorPauseFlags_dword_791A0C)
        {
            sub_44E8F1();
        }
        return;
    }

    if (gGameOverTimer_dword_7227A4 > 0)
    {
        if (gGameOverTimer_dword_7227A4 == 1)
        {
            if (File_GetStreamState_45837C() == -1)
            {
                if (Res_over_create_4502CD(1))
                {
                    gGameOverTimer_dword_7227A4 = -1;
                }
                else
                {
                    Actor_KillActorsAtLevel(4);
                }
            }
            else if (File_GetStreamState_45837C() == 1)
            {
                StreamActorStop_4583BB();
            }
        }
        else
        {
            --gGameOverTimer_dword_7227A4;
        }

        GameD_Update_helper(buttons);
        return;
    }

    if (!script_cancel_non_zero_dword_7227A0 || gActorPauseFlags_dword_791A0C & 2)
    {
        if (!gActorPauseFlags_dword_791A0C)
        {
            GameD_Sound_4552CB();
        }

        if (game_state_dword_72279C & 0xD8004020)
        {
            if (gActorPauseFlags_dword_791A0C & 2)
            {
                sub_44E932();
            }
        }
        else
        {
            if (!(gActorPauseFlags_dword_791A0C & 0xFD))
            {
                LOG_WARNING("Not implemented");
                //abort();
                // TODO
                /*
                if (gArrayPtr_dword_995324->field_2 & 0x800)
                {
                    sub_44E969();
                }*/
            }
        }
        GameD_update_helper_44F28B();
        GameD_Update_helper(buttons);
        return;
    }
    
    if (script_cancel_non_zero_dword_7227A0 & 0x80)
    {
        counter_dword_6BED20 = 0x7FFF0000;
    }

    Actor_KillActorsAtLevel(4);
    gActorPauseFlags_dword_791A0C = gActorPauseFlags_dword_791A0C & 0xF7;
    Map_FreeKmds_44F3B5();
    StreamActorStop_4583BB();
    pGameD->gamed_unk_722784 = 3;
    game_state_dword_72279C |= 0x104A6000u;
}
MGS_FUNC_IMPLEX(0x0044E381, GameD_update_44E381, ACTOR_GAMED_IMPL);

void* CC ResidentTopAllocate_40B379(int size)
{
    const int alignedSize = RoundUpPowerOf2(size, 4);
    gResidentTop_dword_78E960 -= alignedSize; // Moving BACKWARDS into gResidentHeap_81001F
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

void CC LibDG_Clean_Texture_Cache_401110()
{
    LibDG_ClearTexturesCache_402487();
    LibDG_Restore_Textures_From_Resident_Memory_40274C();
}
MGS_FUNC_IMPLEX(0x00401110, LibDG_Clean_Texture_Cache_401110, ACTOR_GAMED_IMPL);

MGS_VAR(1, 0x81001F, ResidentHeap, gResidentHeap_81001F, {});
MGS_ARY(1, 0x8A8E20, BYTE, 438272, gSystem2_memory_unk_8A8E20, {});
MGS_ARY(1, 0x913E20, BYTE, 192512, gSystem0_memory_unk_913E20, {});
MGS_ARY(1, 0x942E20, BYTE, 192512, gSystem1_memory_unk_942E20, {});

void CC System_Init_0_And_1_40A465()
{
    System_init_40AC6C(0, 1, &gSystem0_memory_unk_913E20[0], 192512);
    System_init_40AC6C(1, 1, &gSystem1_memory_unk_942E20[0], 192512);
}
MGS_FUNC_IMPLEX(0x0040A465, System_Init_0_And_1_40A465, ACTOR_GAMED_IMPL);

void CC LibGV_40A4BB()
{
    LibGV_Restore_File_Cache_Records_From_Resident_Memory_40A72A();
    System_DeInit_Systems_0_to_2_sub_40AC52();
    System_Init_0_And_1_40A465();
    System_init_40AC6C(2, 0, &gSystem2_memory_unk_8A8E20[0], 438272);
    printf("RESIDENT TOP %X\n", gResidentTop_dword_78E960);
}
MGS_FUNC_IMPLEX(0x0040A4BB, LibGV_40A4BB, ACTOR_GAMED_IMPL);

void CC Init_Gamed_Create_44E12B()
{
    gTotalFrameTime_dword_995344 = 0;
    gGameOverTimer_dword_7227A4 = 0;
    script_cancel_non_zero_dword_7227A0 = 0;
    gLoaderState_dword_9942B8 = 0;
    Res_MenuMan_create_459A9A();
    Stage_GetNameHashStack_44EAED();
    LibDG_SetActiveResourceInitFuncPtrs_457B5B();
    Script_BindInits_452610();
    LibGV_Set_FileExtHandler_40A68D('b', GV_bin_file_handler_44E9D2); // Handles loading ".bin" files which sets loadable objects table
    sub_44E1E0();
    Actor_PushBack(1, &gGameD_stru_722760.mBase, nullptr);
    Actor_Init(&gGameD_stru_722760.mBase, reinterpret_cast<TActorFunction>(GameD_update_44E381), nullptr, "C:\\mgs\\source\\Game\\gamed.c");

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
