#pragma once

#include "MgsFunction.hpp"
#include "Actor.hpp"

void DoLibGv_Tests();
void LibGVCpp_ForceLink();

using TFileNameHash = DWORD; // only 24 bits are used
using GV_FnPtr = int(__cdecl*)(void*, TFileNameHash);

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

struct LibGV_FileRecord
{
    DWORD mId;
    void* mFileBuffer;
};
MGS_ASSERT_SIZEOF(LibGV_FileRecord, 0x8);

struct LibGv_Msg_Array
{
    DWORD mCount;
    LibGV_Msg mMessages[16];
};
MGS_ASSERT_SIZEOF(LibGv_Msg_Array, 0x144);

struct LibGv_Struct
{
    Actor mBase;
    DWORD gRenderedFramesCount_dword_6BFF00;
    DWORD dword_6BFF04_time_related;
    LibGV_FileRecord* dword_6BFF08_last_free_ptr; // Seems to point to one of mStruct8_128Array_06BFF80
    GV_FnPtr dword_6BFF0C_fn_ptrs[26];
    LibGV_FileRecord* dword_6BFF74_resident_top_alloc;
    int dword_6BFF78_count;
    DWORD mField_9C; // Padding?
    LibGV_FileRecord mFileCache_A0_06BFF80[128];
    DWORD gGv_dword_6C0380;
    DWORD gGv_dword_6C0384;
    DWORD gGv_dword_6C0388[6];
    DWORD gGv_dword_6C03A0;
    DWORD gGv_dword_6C03A4;
    DWORD gGv_dword_6C03A8;
    DWORD mDWORD_Pad2;
    LibGv_Msg_Array gGv_dword_6C03B0_mesg_array1;
    LibGv_Msg_Array gGv_dword_6C04F4_mesg_array2;
    DWORD gGv_dword_6C0638_active_mesg_array_idx;
};
MGS_ASSERT_SIZEOF(LibGv_Struct, 0x75C);

int CC LibGV_mesg_write_40B3ED(const LibGV_Msg* pSrcMsg);

void LibGv_Init_sub_40A4F6();
void CC LibGV_Set_FileExtHandler_40A68D(char id, GV_FnPtr fn);
void CC LibGv_ClearFunctionPointers_40A69D();
void CC LibGv_Set_Load_HZM_CallBack_40B725();
__int64 CC TimeGetElapsed_4455A0();
int CC LibGV_LoadFile_40A77F(void* fileData, signed int fileNameHash, int allocType);
void CC LibGV_Save_File_Cache_Records_To_Resident_Memory_40A6CD();
void CC LibGV_Restore_File_Cache_Records_From_Resident_Memory_40A72A();
void CC LibGV_Reset_System2_Memory_40B35E();
void CC LibGV_Init_FileCache_40A6AC();
void* CC LibGV_FindFile_40A603(int hash);

template<class T>
T CC LibGV_FindFile_40A603_T(int hash)
{
    return reinterpret_cast<T>(LibGV_FindFile_40A603(hash));
}

MGS_VAR_EXTERN(DWORD, gFrameTime_dword_791A04);
MGS_VAR_EXTERN(LibGv_Struct, g_lib_gv_stru_6BFEE0);
