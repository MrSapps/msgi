#pragma once

#include "Actor.hpp"
#include "MgsFunction.hpp"

void Actor_LoaderCpp_ForceLink();

struct Actor_Loader_Impl;

struct Actor_Loader
{
    Actor mBase;
    Actor_Loader_Impl* field_20_pSystem_allocated_size_0x74;
    int field_24_proc_cancel_flags;
    int mIsRunning;
    int field_2C_counter;
    enum FileLoadModes
    {
        eNoCache = 0,
        eCache = 1,
        eResident = 2,
        eSound = 3
    };
};
MGS_ASSERT_SIZEOF(Actor_Loader, 0x30);

void CC Res_loader_Create_457BDD(const char* strStageName);
void CC Res_loader_EnableHiTex_51D1DB(const char* pcxName);
const char* CC Res_loader_GetLine_408E1B(const char* pInput, char* pOutputLine);

MGS_ARY_EXTERN(char, 256, gStage_Name_byte_6504C8);
MGS_VAR_EXTERN(DWORD, dword_99533C);
MGS_VAR_EXTERN(DWORD, gFixupLibDg_Allocs_And_Hahses_dword_78D7AC);
MGS_VAR_EXTERN(char*, Str1_6BFBA0);
