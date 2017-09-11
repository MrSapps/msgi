#include "stdafx.h"
#include "Actor_Loader.hpp"
#include "Script.hpp"
#include "Actor_GameD.hpp"
#include "Timer.hpp"
#include "System.hpp"
#include "Sound.hpp"
#include <gmock/gmock.h>

#define ACTOR_LOADER_IMPL true

void DoTests();

void Actor_LoaderCpp_ForceLink() 
{
    DoTests();
}

struct Actor_Loader_Impl
{
    int field_0;
    int field_4_str_pos;
    int field_8_unknown_state;
    char* field_C_c_str_ptr_field_2C;
    int field_10;
    int field_14_load_ret;
    int field_18_state;
    void* field_1C_c_str_data_cnf_sys_allocd;
    char* field_20_c_str;
    s16 field_24_field_2C_char_state_resident_type;
    s16 field_26_padding_q;
    void* field_28_sys2_alloc_file_buffer;
    char field_2C_c_str[64];
    int field_6C_count_file_second_dword;
    int field_70_count_file_first_dword;
};
MGS_ASSERT_SIZEOF(Actor_Loader_Impl, 0x74);

MGS_FUNC_NOT_IMPL(0x00408EEF, signed int CC(char* fileName, void** buffer, signed int type), FS_LoadRequest);
MGS_FUNC_NOT_IMPL(0x0051D2ED, void CC(), File_HITEXT_INIT);
MGS_FUNC_NOT_IMPL(0x00401F77, void CC(__int16 a1), Res_loader_tick_helper_401F77);

MGS_VAR(1, 0x71D13C, DWORD, dword_71D13C, 0);
MGS_VAR(1, 0x6BFBA4, DWORD, dword_6BFBA4, 0);
MGS_VAR(1, 0x71D138, DWORD, dword_71D138, 0);
MGS_VAR(1, 0x78D7AC, DWORD, gFixupLibDg_Allocs_And_Hahses_dword_78D7AC, 0);
MGS_VAR(1, 0x650478, DWORD, dword_650478, 0);
MGS_VAR(1, 0x99533C, DWORD, dword_99533C, 0);

MGS_ARY(1, 0x6504C8, char, 256, gStage_Name_byte_6504C8, {});

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

void CC Stage_SetNameQ(const char* stageName)
{
    sprintf(gStage_Name_byte_6504C8, "stage/%s", stageName);
}
MGS_FUNC_IMPLEX(0x408ED8, Stage_SetNameQ, ACTOR_LOADER_IMPL);

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

    if (FS_LoadRequest("data.cnf", &pAllocated->field_1C_c_str_data_cnf_sys_allocd, 1) >= 0)
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


MGS_VAR(1, 0x6BFBA0, char*, Str1_6BFBA0, 0);
MGS_VAR(1, 0x78D7A8, DWORD, Actor_Loader_Impl_Field10_dword_78D7A8, 0);

MGS_FUNC_NOT_IMPL(0x00408D6C, signed int CC(Actor_Loader_Impl* pSystemStruct), Res_loader_408D6C);
MGS_FUNC_NOT_IMPL(0x00408FAE, int(), Res_loader_load_file_to_mem_408FAE);
MGS_FUNC_NOT_IMPL(0x0040A5C3, int CC(char* arg0), sub_40A5C3);
MGS_FUNC_NOT_IMPL(0x0040A77F, int CC(int sys2FileBuffer, signed int maybe_id, int resident_type), LibGV_id_conflict_40A77F);
MGS_FUNC_NOT_IMPL(0x0051D1DB, char* CC(char* a1), Res_loader_51D1DB);
MGS_FUNC_NOT_IMPL(0x00408E67, int CC(char* a1), Res_loader_408E67);

/*
signed int CC Res_loader_408D6C(Actor_Loader_Impl* pSystemStruct);
int Res_loader_load_file_to_mem_408FAE();
int CC sub_40A5C3(char* arg0);
int CC LibGV_id_conflict_40A77F(int sys2FileBuffer, signed int maybe_id, int resident_type);
char* CC Res_loader_51D1DB(char* a1);
int CC Res_loader_408E67(char* a1);
*/
bool CC Res_loader_Is_Extension_4088F2(const char* fileName, const char* extension)
{
    const char* dotPos = strchr(fileName, '.');
    if (dotPos)
    {
        dotPos++;
    }
    else
    {
        dotPos = fileName;
    }
    return strcmp(dotPos, extension) == 0;
}
MGS_FUNC_IMPLEX(0x4088F2, Res_loader_Is_Extension_4088F2, ACTOR_LOADER_IMPL);

signed int CC Res_loader_help2_408A73(Actor_Loader_Impl* pSystemStruct)
{
    switch (pSystemStruct->field_18_state)
    {
    case 0:
        if (Res_loader_load_file_to_mem_408FAE() <= 0) // Returns 0 so always true
        {
            pSystemStruct->field_18_state = 1;
            pSystemStruct->field_20_c_str = (char*)pSystemStruct->field_1C_c_str_data_cnf_sys_allocd;
            pSystemStruct->field_24_field_2C_char_state_resident_type = 1;
            pSystemStruct->field_4_str_pos = Res_loader_408E67((char *)pSystemStruct->field_1C_c_str_data_cnf_sys_allocd);
            pSystemStruct->field_8_unknown_state = 0;
        }
        break;

    case 1:
        if (Res_loader_408D6C(pSystemStruct) <= 0)
        {
            return 0;
        }
        pSystemStruct->field_18_state = 2;
        break;

    case 2:
        pSystemStruct->field_14_load_ret = Res_loader_load_file_to_mem_408FAE(); // Always returns 0
        if (pSystemStruct->field_14_load_ret <= 0) // Always true
        {
            if (Res_loader_Is_Extension_4088F2(pSystemStruct->field_C_c_str_ptr_field_2C, "dar"))
            {
                int* file_first_dword = (int *)pSystemStruct->field_28_sys2_alloc_file_buffer;
                pSystemStruct->field_70_count_file_first_dword = *file_first_dword;
                pSystemStruct->field_6C_count_file_second_dword = (int)(file_first_dword + 1);
                pSystemStruct->field_18_state = 3;
            }
            else
            {
                if (pSystemStruct->field_24_field_2C_char_state_resident_type == 3)
                {
                    // dead branch ??
                    //int v3 = pSystemStruct->field_26_padding_q;
                }
                else
                {
                    Actor_Loader_Impl_Field10_dword_78D7A8 = pSystemStruct->field_10;
                    s16 resident_type = pSystemStruct->field_24_field_2C_char_state_resident_type;
                    int maybe_id = sub_40A5C3(pSystemStruct->field_C_c_str_ptr_field_2C);
                    LibGV_id_conflict_40A77F((int)pSystemStruct->field_28_sys2_alloc_file_buffer, maybe_id, resident_type);
                    if (!pSystemStruct->field_24_field_2C_char_state_resident_type)
                    {
                        System_2_free_40B2A7(pSystemStruct->field_28_sys2_alloc_file_buffer);
                    }
                }
                pSystemStruct->field_18_state = 1;
            }
        }
        break;

    case 3:
        if (strstr(pSystemStruct->field_2C_c_str, ".dar"))
        {
            strstr(pSystemStruct->field_2C_c_str, "tex"); // ?? not used
        }
        int counter2 = pSystemStruct->field_6C_count_file_second_dword;
        int counter = pSystemStruct->field_70_count_file_first_dword;
        do
        {
            pSystemStruct->field_6C_count_file_second_dword = counter2;
            pSystemStruct->field_70_count_file_first_dword = counter;
            char* v12 = pSystemStruct->field_2C_c_str;
            char v7 = 0;
            do
            {
                *v12 = *(BYTE *)counter2;
                v7 = *v12++;
                ++counter2;
            } while (v7);

            int* v8 = (int *)(((4 - (counter2 & 3)) & 3) + counter2);
            int v13 = *v8;
            int counter2a = (int)(v8 + 1);

            if (strstr(pSystemStruct->field_2C_c_str, "pcx"))
            {
                Res_loader_51D1DB(pSystemStruct->field_2C_c_str);
            }

            s16 resident_type2 = pSystemStruct->field_24_field_2C_char_state_resident_type;
            Str1_6BFBA0 = pSystemStruct->field_2C_c_str;
            int maybe_id2 = sub_40A5C3(pSystemStruct->field_2C_c_str);
            int v17 = LibGV_id_conflict_40A77F(counter2a, maybe_id2, resident_type2);

            if (!v17)
            {
                return 1;
            }

            if (v17 < 0)
            {
                printf("INIT_ERROR in %s !!\n", pSystemStruct->field_2C_c_str);
                return 0;
            }
            counter2 = counter2a + v13 + 1;
            --counter;
        } while (counter > 0);

        if (!pSystemStruct->field_24_field_2C_char_state_resident_type)
        {
            System_2_free_40B2A7(pSystemStruct->field_28_sys2_alloc_file_buffer);
        }
        pSystemStruct->field_18_state = 1;
        break;
    }
    return 1;
}
MGS_FUNC_IMPLEX(0x408A73, Res_loader_help2_408A73, ACTOR_LOADER_IMPL);

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

const char* CC Res_loader_GetLine_408E1B(const char* pInput, char* pOutputLine)
{
    *pOutputLine = '\0';
        
    // Skip new lines
    while (*pInput == '\r' || *pInput == '\n')
    {
        pInput++;
    }

    // Bail if at end
    if (*pInput == '\0')
    {
        return nullptr;
    }

    // Keep going till new line or end
    while (*pInput != '\0')
    {
        if (*pInput == '\n' || *pInput == '\r')
        {
            pInput++;
            break;
        }

        // Copy to output
        *pOutputLine = *pInput;
        pOutputLine++;

        pInput++;
    }

    *pOutputLine = '\0';

    // Bail if at end
    if (*pInput == '\0')
    {
        return nullptr;
    }

    return pInput;
}
MGS_FUNC_IMPLEX(0x408E1B, Res_loader_GetLine_408E1B, ACTOR_LOADER_IMPL);

static void Res_loader_Is_Extension_4088F2_Test()
{
    ASSERT_EQ(true, Res_loader_Is_Extension_4088F2("blah.dar", "dar"));
    ASSERT_EQ(true, Res_loader_Is_Extension_4088F2("dar", "dar"));
    ASSERT_EQ(true, Res_loader_Is_Extension_4088F2(".dar", "dar"));
    ASSERT_EQ(false, Res_loader_Is_Extension_4088F2(".DAR", "dar"));
    ASSERT_EQ(false, Res_loader_Is_Extension_4088F2("dar.exe", "dar"));
}

static void Res_loader_408E1B_Test()
{
    const char* kInput = "Line1\r\r\n\nLine2\nblah3\rLOLS4";
    char buffer[40] = {};
    const char* ret = Res_loader_GetLine_408E1B(kInput, buffer);
    ASSERT_STREQ("\r\n\nLine2\nblah3\rLOLS4", ret);
    ASSERT_STREQ("Line1", buffer);

    ret = Res_loader_GetLine_408E1B(ret, buffer);
    ASSERT_STREQ("blah3\rLOLS4", ret);
    ASSERT_STREQ("Line2", buffer);

    ret = Res_loader_GetLine_408E1B(ret, buffer);
    ASSERT_STREQ("LOLS4", ret);
    ASSERT_STREQ("blah3", buffer);
    
    ret = Res_loader_GetLine_408E1B(ret, buffer);
    ASSERT_TRUE(ret == nullptr);
    ASSERT_STREQ("LOLS4", buffer);
}

void DoTests()
{
    Res_loader_Is_Extension_4088F2_Test();
    Res_loader_408E1B_Test();
}
