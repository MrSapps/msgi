#include "stdafx.h"
#include "LibGV.hpp"
#include "Actor.hpp"
#include <assert.h>
#include "LibDG.hpp"
#include "Timer.hpp"
#include "Actor_Loader.hpp"

#define REDIRECT_LIBGV_DATA 1
#define LIBGV_IMPL true


struct LibGV_FileRecord
{
    DWORD mId;
    void* mFileBuffer;
};
MGS_ASSERT_SIZEOF(LibGV_FileRecord, 0x8);

struct LibGv_Struct
{
    Actor mBase;
    DWORD gRenderedFramesCount_dword_6BFF00;
    DWORD dword_6BFF04_time_related;
    LibGV_FileRecord* dword_6BFF08_last_free_ptr; // Seems to point to one of mStruct8_128Array_06BFF80
    GV_FnPtr dword_6BFF0C_fn_ptrs[26];
    LibGV_FileRecord* dword_6BFF74_resident_top_alloc;
    LibGV_FileRecord* dword_6BFF78_count;
    LibGV_FileRecord mStruct8_128Array_06BFF80[128];
    DWORD mDWORD_Pad1;
    DWORD gGv_dword_6C0380;
    DWORD gGv_dword_6C0384;
    DWORD gGv_dword_6C0388[6];
    DWORD gGv_dword_6C03A0;
    DWORD gGv_dword_6C03A4;
    DWORD gGv_dword_6C03A8;
    DWORD mDWORD_Pad2;
    DWORD gGv_dword_6C03B0_array1[81];
    DWORD gGv_dword_6C04F4_array2[81];
    DWORD gGv_dword_6C0638_active_array_idx;
};
MGS_ASSERT_SIZEOF(LibGv_Struct, 0x75C);

MGS_VAR(REDIRECT_LIBGV_DATA, 0x6BFEE0, LibGv_Struct, g_lib_gv_stru_6BFEE0, {});

// Other likely LibGvd funcs
MGS_FUNC_NOT_IMPL(0x40A72A, LibGV_FileRecord* CC(), LibGvd_sub_40A72A);
MGS_FUNC_NOT_IMPL(0x40A6CD, char* CC(), LibGvd_sub_40A6CD);

void LibGVCpp_ForceLink() { }

MGS_FUNC_NOT_IMPL(0x40B35E, void CC(), LibGV_Reset_System2_Memory_40B35E);
MGS_FUNC_NOT_IMPL(0x40A6AC, void CC(), LibGV_Init_Allocs_40A6AC);
MGS_FUNC_NOT_IMPL(0x40A4B1, void CC(), sub_40A4B1);
MGS_FUNC_NOT_IMPL(0x40B734, int CC(void*, int), Hzm_load_40B734);

MGS_FUNC_NOT_IMPL(0x40A618, LibGV_FileRecord* CC(int resHash), LibGV_Find_Item_40A618);

MGS_VAR(REDIRECT_LIBGV_DATA, 0x791A04, DWORD, dword_791A04, 0);
MGS_VAR_EXTERN(int, gActiveBuffer_dword_791A08);

void* CC LibGV_FindFile_40A603(int hash)
{
    LibGV_FileRecord* pFound = LibGV_Find_Item_40A618(hash);
    if (pFound)
    {
        return pFound->mFileBuffer;
    }
    return nullptr;
}
MGS_FUNC_IMPLEX(0x40A603, LibGV_FindFile_40A603, LIBGV_IMPL);

int CC LibGV_LoadFile_40A77F(void* fileData, signed int fileNameHash, int allocType)
{
    if (allocType == Actor_Loader::eNoCache)
    {
        auto fnFileLoader = g_lib_gv_stru_6BFEE0.dword_6BFF0C_fn_ptrs[fileNameHash >> 16];
        if (fnFileLoader)
        {
            const int loadFileResult = fnFileLoader((DWORD*)fileData, fileNameHash);
            if (loadFileResult > 0)
            {
                return 1;
            }
            return loadFileResult;
        }
        return 1;
    }
    else
    {
        if (LibGV_Find_Item_40A618(fileNameHash) || (g_lib_gv_stru_6BFEE0.dword_6BFF08_last_free_ptr == 0))
        {
            printf("id conflict\n");
            return -1;
        }

        int hashWithResidentOrSoundFlag = fileNameHash;
        if (allocType != Actor_Loader::eCache) // Isn't cache or no cache, so must be resident or sound
        {
            hashWithResidentOrSoundFlag = fileNameHash | 0x1000000;
        }

        g_lib_gv_stru_6BFEE0.dword_6BFF08_last_free_ptr->mId = hashWithResidentOrSoundFlag;
        g_lib_gv_stru_6BFEE0.dword_6BFF08_last_free_ptr->mFileBuffer = fileData;

        // The first WORD of the hash is related to the file extension
        auto fnFileLoader = g_lib_gv_stru_6BFEE0.dword_6BFF0C_fn_ptrs[fileNameHash >> 16];
        if (fnFileLoader)
        {
            const int loadFileResult = fnFileLoader((DWORD*)fileData, fileNameHash);
            if (loadFileResult <= 0)
            {
                g_lib_gv_stru_6BFEE0.dword_6BFF08_last_free_ptr->mId = 0;
                return loadFileResult;
            }
        }
        return 1;
    }
}
MGS_FUNC_IMPLEX(0x0040A77F, LibGV_LoadFile_40A77F, LIBGV_IMPL);

void CC LibGV_40B3BC()
{
    g_lib_gv_stru_6BFEE0.gGv_dword_6C03B0_array1[0] = 0;
    g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_array2[0] = 0;
    g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_array_idx = 0;
}
MGS_FUNC_IMPLEX(0x40B3BC, LibGV_40B3BC, LIBGV_IMPL);

__int64 CC TimeGetElapsed_4455A0()
{
    static LARGE_INTEGER sCurrentPeftCounter_qword_665508 = {};

    if ((sCurrentPeftCounter_qword_665508.HighPart &  sCurrentPeftCounter_qword_665508.LowPart) == -1)
    {
        sCurrentPeftCounter_qword_665508.QuadPart = TimerInitBaseLineAndGetCurrentTime_5201A6().QuadPart;
    }

    return (TimerInitBaseLineAndGetCurrentTime_5201A6().QuadPart - sCurrentPeftCounter_qword_665508.QuadPart) / gPerformanceFreq_995648.mFreq60;
}
MGS_FUNC_IMPLEX(0x4455A0, TimeGetElapsed_4455A0, LIBGV_IMPL);

void CC LibGV_Update_40A54E(Actor* pActor)
{
    ++g_lib_gv_stru_6BFEE0.gRenderedFramesCount_dword_6BFF00;

    int currentTime = TimeGetElapsed_4455A0(); // TODO: Truncation?
    int timeDiff = currentTime - g_lib_gv_stru_6BFEE0.dword_6BFF04_time_related;
    g_lib_gv_stru_6BFEE0.dword_6BFF04_time_related = currentTime;
    dword_791A04 = timeDiff;

    if (!gLibDG_2_stru_6BB930.dword_6BB950_do_not_flip_buffers)
    {
        // flip active buffer
        gActiveBuffer_dword_791A08 = !gActiveBuffer_dword_791A08;
    }

    if (!gActorPauseFlags_dword_791A0C)
    {
        assert(g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_array_idx >=0 && g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_array_idx <= 1);
        g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_array_idx = 1 - g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_array_idx;

        if (g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_array_idx != 0)
        {
            g_lib_gv_stru_6BFEE0.gGv_dword_6C03B0_array1[0] = 0;
        }
        else
        {
            g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_array2[0] = 0;
        }
    } 
}
MGS_FUNC_IMPLEX(0x40A54E, LibGV_Update_40A54E, LIBGV_IMPL);

void LibGv_Init_sub_40A4F6()
{
    ActorList_Init();
    LibGV_Reset_System2_Memory_40B35E();
    LibGv_ClearFunctionPointers_40A69D();
    LibGV_Init_Allocs_40A6AC();
    sub_40A4B1();
    Actor_PushBack(0, &g_lib_gv_stru_6BFEE0.mBase, nullptr);
    Actor_Init(&g_lib_gv_stru_6BFEE0.mBase, LibGV_Update_40A54E, nullptr, "C:\\mgs\\source\\LibGV\\gvd.c");
    gActiveBuffer_dword_791A08 = 0;
    g_lib_gv_stru_6BFEE0.gRenderedFramesCount_dword_6BFF00 = 0;
    //nullsub_6(nullsub_4);
}
MGS_FUNC_IMPLEX(0x40A4F6, LibGv_Init_sub_40A4F6, LIBGV_IMPL);


void CC LibGv_ClearFunctionPointers_40A69D()
{
    memset(g_lib_gv_stru_6BFEE0.dword_6BFF0C_fn_ptrs, 0, sizeof(g_lib_gv_stru_6BFEE0.dword_6BFF0C_fn_ptrs));
}
MGS_FUNC_IMPLEX(0x40A69D, LibGv_ClearFunctionPointers_40A69D, LIBGV_IMPL);

void CC LibGv_Set_Load_HZM_CallBack_40B725()
{
    LibGV_Set_FileExtHandler_40A68D('h', Hzm_load_40B734.Ptr()); // .hzm
}
MGS_FUNC_IMPLEX(0x40B725, LibGv_Set_Load_HZM_CallBack_40B725, LIBGV_IMPL);

void CC LibGV_Set_FileExtHandler_40A68D(char id, GV_FnPtr fn)
{
    // Convert the a-z index to 0-25
    const int idx = id - 'a';

    assert(idx < _countof(g_lib_gv_stru_6BFEE0.dword_6BFF0C_fn_ptrs));
    g_lib_gv_stru_6BFEE0.dword_6BFF0C_fn_ptrs[idx] = fn;
}
MGS_FUNC_IMPLEX(0x40A68D, LibGV_Set_FileExtHandler_40A68D, LIBGV_IMPL);
