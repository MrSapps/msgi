#include "stdafx.h"
#include "Actor_Loader.hpp"
#include "Script.hpp"
#include "Actor_GameD.hpp"
#include "Timer.hpp"
#include "System.hpp"
#include "Sound.hpp"
#include "ResourceNameHash.hpp"
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
    int field_4_data_cnf_line_count;
    int field_8_unknown_state;
    char* field_C_c_str_ptr_field_2C;
    int field_10;
    int field_14_load_ret;
    int field_18_state;
    char* field_1C_c_str_data_cnf_sys_allocd;
    char* field_20_c_str;
    s16 field_24_field_2C_char_state_resident_type;
    s16 field_26_padding_q;
    void* field_28_sys2_alloc_file_buffer;
    char field_2C_c_str[64];
    char* field_6C_pointer_file_second_dword;
    int field_70_count_file_first_dword;
};
MGS_ASSERT_SIZEOF(Actor_Loader_Impl, 0x74);

MGS_FUNC_NOT_IMPL(0x00408EEF, signed int CC(char* fileName, void** buffer, signed int type), FS_LoadRequest);
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

    if (FS_LoadRequest("data.cnf", (void**)&pAllocated->field_1C_c_str_data_cnf_sys_allocd, 1) >= 0)
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

int CC Res_loader_CountOfNonDotLines_408E67(const char* pInput)
{
    int count = 0;
    char line[64] = {};
    do
    {
        pInput = Res_loader_GetLine_408E1B(pInput, line);
        if (line[0] != '.')
        {
            count++;
        }
    } while (pInput);
    return count;
}
MGS_FUNC_IMPLEX(0x408E67, Res_loader_CountOfNonDotLines_408E67, ACTOR_LOADER_IMPL);

signed int CC Res_loader_help2_408A73(Actor_Loader_Impl* pSystemStruct)
{
    switch (pSystemStruct->field_18_state)
    {
    case 0:
        if (Res_loader_load_file_to_mem_408FAE() <= 0) // Returns 0 so always true
        {
            pSystemStruct->field_18_state = 1;
            pSystemStruct->field_20_c_str = pSystemStruct->field_1C_c_str_data_cnf_sys_allocd;
            pSystemStruct->field_24_field_2C_char_state_resident_type = 1;
            pSystemStruct->field_4_data_cnf_line_count = Res_loader_CountOfNonDotLines_408E67(pSystemStruct->field_1C_c_str_data_cnf_sys_allocd);
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
                pSystemStruct->field_6C_pointer_file_second_dword = (char*)(file_first_dword + 1);
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

        char* darDataPointer = pSystemStruct->field_6C_pointer_file_second_dword;
        int darItemCount = pSystemStruct->field_70_count_file_first_dword;
        do
        {
            pSystemStruct->field_6C_pointer_file_second_dword = darDataPointer;
            pSystemStruct->field_70_count_file_first_dword = darItemCount;
            
            // Copy the string from the file data into field_2C_c_str
            char* pFileName = pSystemStruct->field_2C_c_str;
            char fileDataChar = 0;
            do
            {
                *pFileName = *darDataPointer;
                fileDataChar = *pFileName++;
                ++darDataPointer;
            } while (fileDataChar);
            LOG_INFO("Processing DAR item: " << pSystemStruct->field_2C_c_str);

            char* pAfterFileNameData = RoundUpPowerOf2Ptr(darDataPointer, 4);

            DWORD darItemFileSize = *(DWORD*)pAfterFileNameData;
            char* darFileDataPointer = pAfterFileNameData + 4;

            if (strstr(pSystemStruct->field_2C_c_str, "pcx"))
            {
                Res_loader_EnableHiTex_51D1DB(pSystemStruct->field_2C_c_str);
            }

            const s16 resident_type = pSystemStruct->field_24_field_2C_char_state_resident_type;
            Str1_6BFBA0 = pSystemStruct->field_2C_c_str;
            const int maybe_id2 = sub_40A5C3(pSystemStruct->field_2C_c_str);
            const int libGvRet = LibGV_id_conflict_40A77F((int)darFileDataPointer, maybe_id2, resident_type);

            if (!libGvRet)
            {
                return 1;
            }

            if (libGvRet < 0)
            {
                printf("INIT_ERROR in %s !!\n", pSystemStruct->field_2C_c_str);
                return 0;
            }

            // Move to the next file in the DAR
            darDataPointer = &darFileDataPointer[darItemFileSize + 1];
            --darItemCount;
        } while (darItemCount > 0);

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

struct HiTexRecord
{
    DWORD field_0_id;
    DWORD field_4_is_in_use;
    char* field_8_name;
};
MGS_ASSERT_SIZEOF(HiTexRecord, 0xC);

MGS_VAR(1, 0x734A30, DWORD, gNum_HiTexs_dword_734A30, 0);
MGS_ARY(1, 0x9956A0, HiTexRecord, 8192, gHiText_recs_9956A0, {});

void CC File_HITEXT_INIT()
{
    // Free existing HiTex entries
    for (DWORD i = 0; i < gNum_HiTexs_dword_734A30; i++)
    {
        free(gHiText_recs_9956A0[i].field_8_name);
        gHiText_recs_9956A0[i].field_0_id = 0;
        const bool dword_6893D4_is_zero = dword_6893D4 == 0;
        if (!dword_6893D4_is_zero)
        {
            gHiText_recs_9956A0[i].field_4_is_in_use = 0;
        }
    }

    dword_6893D4 = 0;
    gNum_HiTexs_dword_734A30 = 0;

    FILE* hFile = fopen("hitex.dir", "rt");
    if (!hFile)
    {
        return;
    }

    char fileBuffer[256];
    for (;;)
    {
        memset(fileBuffer, 0, sizeof(fileBuffer));
        fgets(fileBuffer, 256, hFile);

        // Remove any trailing line feed
        char* newLinePos = strchr(fileBuffer, '\r');
        if (newLinePos)
        {
            *newLinePos = 0;
        }

        // Remove any trailing new line
        newLinePos = strchr(fileBuffer, '\n');
        if (newLinePos)
        {
            *newLinePos = 0;
        }

        // Bail if no more data
        if (!*fileBuffer)
        {
            break;
        }

        // Replace \\ with /
        char* tmp = fileBuffer;
        while (*tmp)
        {
            if (*tmp == '\\')
            {
                *tmp = '/';
            }
            tmp++;
        }

        gHiText_recs_9956A0[gNum_HiTexs_dword_734A30].field_8_name = _strdup(fileBuffer);
        const DWORD id = HiTexHash_51D47A(fileBuffer);
        const DWORD idx = gNum_HiTexs_dword_734A30;
        gHiText_recs_9956A0[idx].field_0_id = id;
        printf("HITEX_INIT: Id: %-5d Name: %s\n", id, gHiText_recs_9956A0[idx].field_8_name);
        gNum_HiTexs_dword_734A30++;
    }

    fclose(hFile);
}
MGS_FUNC_IMPLEX(0x0051D2ED, File_HITEXT_INIT, ACTOR_LOADER_IMPL);

const char* CC HITEX_NAME(DWORD id)
{
    for (DWORD i = 0; i < gNum_HiTexs_dword_734A30; i++)
    {
        if (gHiText_recs_9956A0[i].field_4_is_in_use && gHiText_recs_9956A0[i].field_0_id == id)
        {
            printf("HITEX_NAME: Id: %-5d Name: %s\n", id, gHiText_recs_9956A0[i].field_8_name);
            return gHiText_recs_9956A0[i].field_8_name;
        }
    }
    return nullptr;
}
MGS_FUNC_IMPLEX(0x0051D4BC, HITEX_NAME, ACTOR_LOADER_IMPL);

void CC Res_loader_EnableHiTex_51D1DB(const char* pcxName)
{
    for (DWORD i = 0; i < gNum_HiTexs_dword_734A30; i++)
    {
        const char* tgaName = strchr(gHiText_recs_9956A0[i].field_8_name, '/');
        if (tgaName)
        {
            // Consume the /
            tgaName++;
        }
        if (*tgaName)
        {
            if (strlen(tgaName) == strlen(pcxName))
            {
                if (strncmp(tgaName, pcxName, strlen(tgaName) - 3) == 0)
                {
                    gHiText_recs_9956A0[i].field_4_is_in_use = 1;
                    return;
                }
            }
        }
    }
}
MGS_FUNC_IMPLEX(0x51D1DB, Res_loader_EnableHiTex_51D1DB, ACTOR_LOADER_IMPL);

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
