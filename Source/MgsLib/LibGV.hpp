#pragma once

#include "MgsFunction.hpp"


void DoLibGv_Tests();
void LibGVCpp_ForceLink();

using GV_FnPtr = int(__cdecl*)(void*, int);

struct LibGV_Msg
{
    WORD field_0_res_hash;
    WORD field_2_num_same_messages;
    WORD field_4_action_hash_or_ptr;
    WORD field_6_hash;
    WORD field_8_min1;
    WORD field_A_min1;
    WORD field_C_min1;
    WORD field_E;
    WORD field_10;
    WORD field_12_num_valid_fields;
};
MGS_ASSERT_SIZEOF(LibGV_Msg, 0x14);
MGS_ARY_EXTERN(WORD, 16, word_7919C2);

struct LibGV_FileRecord
{
    DWORD mId;
    void* mFileBuffer;
};
MGS_ASSERT_SIZEOF(LibGV_FileRecord, 0x8);

int CC LibGV_mesg_write_40B3ED(const LibGV_Msg* pSrcMsg);

void LibGv_Init_sub_40A4F6();
void CC LibGV_Set_FileExtHandler_40A68D(char id, GV_FnPtr fn);
void CC LibGv_ClearFunctionPointers_40A69D();
void CC LibGv_Set_Load_HZM_CallBack_40B725();
__int64 CC TimeGetElapsed_4455A0();
int CC LibGV_LoadFile_40A77F(void* fileData, signed int fileNameHash, int allocType);
void CC LibGV_Save_File_Cache_Records_To_Resident_Memory_40A6CD();
void CC LibGV_Restore_File_Cache_Records_From_Resident_Memory_40A72A();
