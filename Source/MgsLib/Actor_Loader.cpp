#include "stdafx.h"
#include "Actor_Loader.hpp"
#include "Script.hpp"
#include "Actor_GameD.hpp"
#include "Timer.hpp"
#include "System.hpp"
#include "Sound.hpp"

#define ACTOR_LOADER_IMPL true

void Actor_LoaderCpp_ForceLink() { }

struct Actor_Loader_Impl
{
    int field_0;
    int field_4_str_pos;
    int field_8_unknown_state;
    char* field_C_c_str_ptr_field_2C;
    int field_10;
    int field_14;
    int field_18_state;
    int field_1C_c_str_data_cnf_sys_allocd;
    int field_20_c_str;
    s16 field_24_field_2C_char_state_resident_type;
    s16 field_26_padding_q;
    void* field_28_sys2_alloc_file_buffer;
    char field_2C_c_str[64];
    int field_6C_count_file_second_dword;
    int field_70_count_file_first_dword;
};
MGS_ASSERT_SIZEOF(Actor_Loader_Impl, 0x74);

void CC Res_loader_shutdown_helper_408E95(void* ptr)
{
    System_2_free_40B2A7(ptr);
}
MGS_FUNC_IMPLEX(0x408E95, Res_loader_shutdown_helper_408E95, ACTOR_LOADER_IMPL);

void CC Res_loader_48_end_457C95(Actor_Loader* pLoader)
{
    printf("LoadEnd\n");
    Res_loader_shutdown_helper_408E95(pLoader->field_20_pSystem_allocated_size_0x74);
    gLoaderState_dword_9942B8 = -1;
    Timer_30_1();
}
MGS_FUNC_IMPLEX(0x457C95, Res_loader_48_end_457C95, ACTOR_LOADER_IMPL);

MGS_VAR(1, 0x71D13C, DWORD, dword_71D13C, 0);
MGS_VAR(1, 0x6BFBA4, DWORD, dword_6BFBA4, 0);
MGS_VAR(1, 0x71D138, DWORD, dword_71D138, 0);
MGS_VAR(1, 0x78D7AC, DWORD, gFixupLibDg_Allocs_And_Hahses_dword_78D7AC, 0);
MGS_VAR(1, 0x650478, DWORD, dword_650478, 0);
MGS_VAR(1, 0x99533C, DWORD, dword_99533C, 0);


MGS_ARY(1, 0x6504C8, char, 256, gStage_Name_byte_6504C8, {});

void CC Stage_SetNameQ(const char* stageName)
{
    sprintf(gStage_Name_byte_6504C8, "stage/%s", stageName);
}
MGS_FUNC_IMPLEX(0x408ED8, Stage_SetNameQ, ACTOR_LOADER_IMPL);

MGS_FUNC_NOT_IMPL(0x00408EEF, signed int CC(char* fileName, void** buffer, signed int type), FS_LoadRequest);
MGS_FUNC_NOT_IMPL(0x0051D2ED, void CC(), File_HITEXT_INIT);

Actor_Loader_Impl* CC Stage_LoadRelated_DataCnf_Q2(const char* strStageNameParam)
{
    dword_6BFBA4 = 0;

    char strStageName[80] = {};
    strcpy(strStageName, strStageNameParam);
    if (!strstr(strStageName, "s11d") || dword_71D138)
    {
        dword_71D13C = 0;
        dword_71D138 = 0;
    }
    else
    {
        dword_71D13C = 1;
    }

    gFixupLibDg_Allocs_And_Hahses_dword_78D7AC = 0;

    Actor_Loader_Impl* pAllocated = System_2_zerod_allocate_memory_40B296_T<Actor_Loader_Impl>();
    pAllocated->field_18_state = 0;
    pAllocated->field_1C_c_str_data_cnf_sys_allocd = 0;
    pAllocated->field_8_unknown_state = -1;
    pAllocated->field_C_c_str_ptr_field_2C = pAllocated->field_2C_c_str;

    Stage_SetNameQ(strStageName);

    if (FS_LoadRequest("data.cnf", (void **)&pAllocated->field_1C_c_str_data_cnf_sys_allocd, 1) >= 0)
    {
        File_HITEXT_INIT();

        char strFullStageName[256] = {};
        sprintf(strFullStageName, "stage/%s", strStageName);

        if (dword_650478)
        {
            dword_650478 = 0;
        }
        else
        {
            Sound_CleanUpRelated();
        }

        Sound_LoadFxRelatedQ2(strFullStageName);
        Timer_30_1();
        return pAllocated;
    }
    return nullptr;
}
MGS_FUNC_IMPLEX(0x408918, Stage_LoadRelated_DataCnf_Q2, ACTOR_LOADER_IMPL);

MGS_FUNC_NOT_IMPL(0x00401F77, void CC(__int16 a1), Res_loader_tick_helper_401F77);
MGS_FUNC_NOT_IMPL(0x00408A73, signed int CC(Actor_Loader_Impl* pSystemStruct), Res_loader_help2_408A73);


signed int CC Res_loader_j_helper2_408A68(Actor_Loader_Impl* pSystemStruct)
{
    return Res_loader_help2_408A73(pSystemStruct);
}
MGS_FUNC_IMPLEX(0x408A68, Res_loader_j_helper2_408A68, ACTOR_LOADER_IMPL);

void CC Res_loader_48_Tick_457C4B(Actor_Loader* pLoader)
{
    ++pLoader->field_2C_counter;
    int flagsM2 = pLoader->field_24_proc_cancel_flags - 2;
    if (flagsM2)
    {
        if (flagsM2 == 1)
        {
            Res_loader_tick_helper_401F77(pLoader->field_2C_counter & 2);// Do a render tick to avoid not responding state?
            dword_99533C = 100;
        }
    }

    if (pLoader->mIsRunning)
    {
        if (!Res_loader_j_helper2_408A68(pLoader->field_20_pSystem_allocated_size_0x74))
        {
            pLoader->mIsRunning = 0;
        }
    }
    else
    {
        Actor_DestroyOnNextUpdate(&pLoader->mBase);
    }
}
MGS_FUNC_IMPLEX(0x457C4B, Res_loader_48_Tick_457C4B, ACTOR_LOADER_IMPL);

void CC Res_loader_Create_457BDD(const char* strStageName)
{
    Actor_Loader* pLoader = Actor_ResourceAllocT<Actor_Loader>(2);

    printf("LoadReq\n");

    Actor_Loader_Impl* pLoaderImpl = Stage_LoadRelated_DataCnf_Q2(strStageName);
    pLoader->field_20_pSystem_allocated_size_0x74 = pLoaderImpl;

    if (!pLoaderImpl)
    {
        printf("NOT FOUND STAGE %s\n", strStageName);
    }

    Actor_Init(
        &pLoader->mBase,
        reinterpret_cast<TActorFunction>(Res_loader_48_Tick_457C4B),
        reinterpret_cast<TActorFunction>(Res_loader_48_end_457C95),
        "C:\\mgs\\source\\Game\\loader.c");

    int flags = script_cancel_non_zero_dword_7227A0 & 0xF;
    pLoader->mIsRunning = 1;
    pLoader->field_24_proc_cancel_flags = flags;
    gLoaderState_dword_9942B8 = 0;
}
MGS_FUNC_IMPLEX(0x457BDD, Res_loader_Create_457BDD, ACTOR_LOADER_IMPL);
