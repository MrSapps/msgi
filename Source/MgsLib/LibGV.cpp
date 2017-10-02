#include "stdafx.h"
#include "LibGV.hpp"
#include "Actor.hpp"
#include <assert.h>
#include "LibDG.hpp"
#include "Timer.hpp"
#include "Actor_Loader.hpp"
#include "ResourceNameHash.hpp"
#include <gmock/gmock.h>

#define REDIRECT_LIBGV_DATA 1
#define LIBGV_IMPL true


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
    LibGv_Msg_Array gGv_dword_6C03B0_mesg_array1;
    LibGv_Msg_Array gGv_dword_6C04F4_mesg_array2;
    DWORD gGv_dword_6C0638_active_mesg_array_idx;
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


void CC LibGV_mesg_init_40B3BC()
{
    g_lib_gv_stru_6BFEE0.gGv_dword_6C03B0_mesg_array1.mCount = 0;
    g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_mesg_array2.mCount = 0;
    g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_mesg_array_idx = 0;
}
MGS_FUNC_IMPLEX(0x40B3BC, LibGV_mesg_init_40B3BC, LIBGV_IMPL);

int CC LibGV_mesg_write_40B3ED(const LibGV_Msg* pSrcMsg)
{
    LibGv_Msg_Array* pArray = g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_mesg_array_idx ?
        &g_lib_gv_stru_6BFEE0.gGv_dword_6C03B0_mesg_array1 :
        &g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_mesg_array2;

    if (pArray->mCount >= 16)
    {
        return -1;
    }

    int messageCount = pArray->mCount;
    LibGV_Msg* pMsgIter = pArray->mMessages;
    WORD matching_count = 0;
    while (messageCount > 0)
    {
        // Find the last matching type
        if (pMsgIter->field_0_res_hash == pSrcMsg->field_0_res_hash)
        {
            // Check if there is a message before the last match
            LibGV_Msg* pCurMsg = &pMsgIter[messageCount];
            if (messageCount - 1 >= 0)
            {
                // There is so move everything back by 1 element so we can put the new message there
                LibGV_Msg* pDst = pCurMsg;
                LibGV_Msg* pSrc = pCurMsg - 1;
                for (int i = 0; i < messageCount; i++)
                {
                    memcpy(pDst, pSrc, sizeof(LibGV_Msg));
                    pDst--;
                    pSrc--;
                }
            }
            matching_count = pMsgIter->field_2_num_same_messages;
            break;
        }
        ++pMsgIter;
        --messageCount;
    }

    // Write in the new message and update the use count
    memcpy(pMsgIter, pSrcMsg, sizeof(LibGV_Msg));
    pMsgIter->field_2_num_same_messages = matching_count + 1;
    pArray->mCount++;

    return 0;
}
MGS_FUNC_IMPLEX(0x0040B3ED, LibGV_mesg_write_40B3ED, LIBGV_IMPL);

int CC LibGV_mesg_read_40B47C(int nameHash, LibGV_Msg** ppOut)
{
    if (!gActorPauseFlags_dword_791A0C)
    {
        LibGv_Msg_Array* pArray = g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_mesg_array_idx  ?
              &g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_mesg_array2 :
              &g_lib_gv_stru_6BFEE0.gGv_dword_6C03B0_mesg_array1;

        int count = pArray->mCount;
        if (pArray->mCount)
        {
            // TODO: Is 0x80000000 required? Can't see how it would ever be set
            for (LibGV_Msg* pMsg = pArray->mMessages; (--count & 0x80000000) == 0; ++pMsg)
            {
                if (pMsg->field_0_res_hash == nameHash)
                {
                    *ppOut = pMsg;
                    return pMsg->field_2_num_same_messages;
                }
            }
        }
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x0040B47C, LibGV_mesg_read_40B47C, LIBGV_IMPL);

int CC Res_otacom_update_helper_mesg_dr_gomon_lamp_on_off_504F25(int bOn)
{
    LibGV_Msg msg = {};

    msg.field_0_res_hash = ResourceNameHash("dr_gomon");
    msg.field_4_action_hash_or_ptr = ResourceNameHash("on");
    if (bOn)
    {
        msg.field_6_hash = ResourceNameHash("dr_lamp_on");
    }
    else
    {
        msg.field_6_hash = ResourceNameHash("dr_lamp_off");
    }
    msg.field_12_num_valid_fields = 2;
    return LibGV_mesg_write_40B3ED(&msg);
}
MGS_FUNC_IMPLEX(0x00504F25, Res_otacom_update_helper_mesg_dr_gomon_lamp_on_off_504F25, LIBGV_IMPL);

void Test_mesg()
{
    LibGV_mesg_init_40B3BC();

    for (WORD i = 0; i < 17; i++)
    {
        LibGV_Msg msg = {};

        if (i < 8)
        {
            msg.field_0_res_hash = ResourceNameHash("dr_gomon");
        }
        else if (i == 8)
        {
            msg.field_0_res_hash = ResourceNameHash("1234");
        }
        else if (i == 13)
        {
            msg.field_0_res_hash = ResourceNameHash("dr_gomon");
        }
        else
        {
            msg.field_0_res_hash = ResourceNameHash("45667");
        }

        msg.field_2_num_same_messages = i + 10;
        msg.field_4_action_hash_or_ptr = i + 20;
        msg.field_6_hash = i + 30;
        msg.field_8_min1 = i + 40;
        msg.field_A_min1 = i + 50;
        msg.field_C_min1 = i + 60;
        msg.field_E = i + 70;
        msg.field_10 = i + 80;
        msg.field_12_num_valid_fields = i + 90;
        if (i == 16)
        {
            ASSERT_EQ(-1, LibGV_mesg_write_40B3ED(&msg));
        }
        else
        {
            ASSERT_EQ(0, LibGV_mesg_write_40B3ED(&msg));
        }
    }

    ASSERT_EQ(16, g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_mesg_array2.mCount);

    for (int i = 0; i < 9; i++)
    {
        ASSERT_EQ(ResourceNameHash("dr_gomon"), g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_mesg_array2.mMessages[i].field_0_res_hash);
        ASSERT_EQ(9-i, g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_mesg_array2.mMessages[i].field_2_num_same_messages);
    }

    ASSERT_EQ(ResourceNameHash("1234"), g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_mesg_array2.mMessages[9].field_0_res_hash);
    ASSERT_EQ(1, g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_mesg_array2.mMessages[9].field_2_num_same_messages);

    int count = 0;
    for (int i = 10; i < 16; i++)
    {
        ASSERT_EQ(ResourceNameHash("45667"), g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_mesg_array2.mMessages[i].field_0_res_hash);
        ASSERT_EQ(6-count, g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_mesg_array2.mMessages[i].field_2_num_same_messages);
        count++;
    }

    const int kExpected[] = { 103, 97, 96, 95, 94, 93, 92, 91, 90, 98, 105, 104, 102, 101, 100, 99 };
    for (int i = 0; i < 16; i++)
    {
        ASSERT_EQ(kExpected[i], g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_mesg_array2.mMessages[i].field_12_num_valid_fields);
    }

    // Flip to looking in the correct array
    g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_mesg_array_idx = !g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_mesg_array_idx;

    {
        LibGV_Msg* out = nullptr;
        int numMatches = LibGV_mesg_read_40B47C(ResourceNameHash("dr_gomon"), &out);
        ASSERT_EQ(9, numMatches);

        for (int i = 0; i < numMatches; i++)
        {
            ASSERT_EQ(9-i, out->field_2_num_same_messages);
            out++;
        }
    }

    {
        LibGV_Msg* out = nullptr;
        int numMatches = LibGV_mesg_read_40B47C(ResourceNameHash("1234"), &out);
        ASSERT_EQ(1, numMatches);
        ASSERT_EQ(1, out->field_2_num_same_messages);
    }

    {
        LibGV_Msg* out = nullptr;
        int numMatches = LibGV_mesg_read_40B47C(ResourceNameHash("45667"), &out);
        ASSERT_EQ(6, numMatches);

        for (int i = 0; i < numMatches; i++)
        {
            ASSERT_EQ(6 - i, out->field_2_num_same_messages);
            out++;
        }
    }

    // Again to prove reading does not erase from source
    {
        LibGV_Msg* out = nullptr;
        int numMatches = LibGV_mesg_read_40B47C(ResourceNameHash("1234"), &out);
        ASSERT_EQ(1, numMatches);
        ASSERT_EQ(1, out->field_2_num_same_messages);
    }
}


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
        assert(g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_mesg_array_idx >=0 && g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_mesg_array_idx <= 1);
        g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_mesg_array_idx = 1 - g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_mesg_array_idx;

        if (g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_mesg_array_idx != 0)
        {
            g_lib_gv_stru_6BFEE0.gGv_dword_6C03B0_mesg_array1.mCount = 0;
        }
        else
        {
            g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_mesg_array2.mCount = 0;
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

void DoLibGv_Tests()
{
    Test_mesg();
}
