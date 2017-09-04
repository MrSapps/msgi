#include "stdafx.h"
#include "LibGV.hpp"
#include "Actor.hpp"
#include <assert.h>
#include "LibDG.hpp"

#define REDIRECT_LIBGV_DATA 1
#define LIBGV_IMPL true

// TODO: Actually a SYSTEM alloc from lib DG
struct struct_8
{
    DWORD mId;
    DWORD* field_4;
};
MSG_ASSERT_SIZEOF(struct_8, 0x8);

struct struct_lib_gv
{
    Actor mBase;
    DWORD gRenderedFramesCount_dword_6BFF00;
    DWORD dword_6BFF04_time_related;
    struct_8* struct_8_ptr_6BFF08; // Seems to point to one of mStruct8_128Array_06BFF80
    GV_FnPtr field_6BFF0C_26_fn_ptrs[26];
    struct_8* struct_8_ptr_6BFF74;
    struct_8* struct_8_ptr_6BFF78;
    //DWORD pad_field_6BFF7C; // TODO: This isn't in ida, how can the rest of this struct be correct????
    struct_8 mStruct8_128Array_06BFF80[128];
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
MSG_ASSERT_SIZEOF(struct_lib_gv, 0x75C);

MGS_VAR(REDIRECT_LIBGV_DATA, 0x6BFEE0, struct_lib_gv, g_lib_gv_stru_6BFEE0, {});

// Other likely LibGvd funcs
MSG_FUNC_NOT_IMPL(0x40A72A, struct_8* CC(), LibGvd_sub_40A72A);
MSG_FUNC_NOT_IMPL(0x40A6CD, char* CC(), LibGvd_sub_40A6CD);
MSG_FUNC_NOT_IMPL(0x40A603, int CC(int), LibGvd_sub_40A603);

void LibGVCpp_ForceLink() { }

MSG_FUNC_NOT_IMPL(0x40B35E, void CC(), LibGV_Reset_System2_Memory_40B35E);
MSG_FUNC_NOT_IMPL(0x40A6AC, void CC(), LibGV_Init_Allocs_40A6AC);
MSG_FUNC_NOT_IMPL(0x40A4B1, void CC(), sub_40A4B1);

//MSG_FUNC_NOT_IMPL(0x4455A0, __int64 __cdecl(), TimingRelated_4455A0);

MGS_VAR(REDIRECT_LIBGV_DATA, 0x791A04, DWORD, dword_791A04, 0);
MGS_VAR_EXTERN(int, gActiveBuffer_dword_791A08);

void __cdecl LibGV_40B3BC()
{
    g_lib_gv_stru_6BFEE0.gGv_dword_6C03B0_array1[0] = 0;
    g_lib_gv_stru_6BFEE0.gGv_dword_6C04F4_array2[0] = 0;
    g_lib_gv_stru_6BFEE0.gGv_dword_6C0638_active_array_idx = 0;
}
MSG_FUNC_IMPLEX(0x40B3BC, LibGV_40B3BC, LIBGV_IMPL);

struct PerformanceFreqStruct
{
    LONGLONG mFreq60;
    LONGLONG mFreq30;
    LONGLONG mFreq24;
    LONGLONG mFreq23;
    LONGLONG mFreq22;
    LONGLONG mFreq21;
    LONGLONG mFreq20;
    LONGLONG mFreq25;
    LONGLONG mFreq15;
};
PerformanceFreqStruct gPerformanceFreq_995648;

LARGE_INTEGER CC TimerInitBaseLineAndGetCurrentTime_5201A6()
{
    LARGE_INTEGER currentCounter = {};
    static bool sDoneQuery_dword_77C920 = false;
    static LARGE_INTEGER sLargeInteger_stru_995690 = {};

    if (!sDoneQuery_dword_77C920)
    {
        QueryPerformanceFrequency(&sLargeInteger_stru_995690);
        gPerformanceFreq_995648.mFreq60 = sLargeInteger_stru_995690.QuadPart / 60;
        gPerformanceFreq_995648.mFreq30 = sLargeInteger_stru_995690.QuadPart / 30;
        gPerformanceFreq_995648.mFreq24 = sLargeInteger_stru_995690.QuadPart / 24;
        gPerformanceFreq_995648.mFreq23 = sLargeInteger_stru_995690.QuadPart / 23;
        gPerformanceFreq_995648.mFreq22 = sLargeInteger_stru_995690.QuadPart / 22;
        gPerformanceFreq_995648.mFreq21 = sLargeInteger_stru_995690.QuadPart / 21;
        gPerformanceFreq_995648.mFreq20 = sLargeInteger_stru_995690.QuadPart / 20;
        gPerformanceFreq_995648.mFreq25 = sLargeInteger_stru_995690.QuadPart / 25;
        gPerformanceFreq_995648.mFreq15 = sLargeInteger_stru_995690.QuadPart / 15;
        sDoneQuery_dword_77C920 = true;
    }
    QueryPerformanceCounter(&currentCounter);
    return currentCounter;
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
MSG_FUNC_IMPLEX(0x4455A0, TimeGetElapsed_4455A0, LIBGV_IMPL);

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
MSG_FUNC_IMPLEX(0x40A54E, LibGV_Update_40A54E, LIBGV_IMPL);

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
MSG_FUNC_IMPLEX(0x40A4F6, LibGv_Init_sub_40A4F6, LIBGV_IMPL);


void CC LibGv_ClearFunctionPointers_40A69D()
{
    memset(g_lib_gv_stru_6BFEE0.field_6BFF0C_26_fn_ptrs, 0, sizeof(g_lib_gv_stru_6BFEE0.field_6BFF0C_26_fn_ptrs));
}
MSG_FUNC_IMPLEX(0x40A69D, LibGv_ClearFunctionPointers_40A69D, LIBGV_IMPL);


void CC LibGV_SetFnPtr_sub_40A68D(char id, GV_FnPtr fn)
{
    // Convert the a-z index to 0-25
    const int idx = id - 'a';

    assert(idx < _countof(g_lib_gv_stru_6BFEE0.field_6BFF0C_26_fn_ptrs));
    g_lib_gv_stru_6BFEE0.field_6BFF0C_26_fn_ptrs[idx] = fn;
}
MSG_FUNC_IMPLEX(0x40A68D, LibGV_SetFnPtr_sub_40A68D, LIBGV_IMPL);
