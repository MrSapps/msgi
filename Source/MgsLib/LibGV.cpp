#include "stdafx.h"
#include "LibGV.hpp"
#include "Actor.hpp"
#include <assert.h>

// TODO: Actually a SYSTEM alloc from lib DG
struct struct_8
{
    DWORD mId;
    DWORD* field_4;
};
static_assert(sizeof(struct_8) == 0x8, "struct_8 should be 0x8");
struct struct_lib_gv
{
    Actor mBase;
    DWORD gRenderedFramesCount_dword_6BFF00;
    DWORD field_6BFF04;
    struct_8* struct_8_ptr_6BFF08; // Seems to point to one of mStruct8_128Array_06BFF80
    GV_FnPtr field_6BFF0C_fn_ptrs[26];
    struct_8* struct_8_ptr_6BFF74;
    struct_8* struct_8_ptr_6BFF78;
    DWORD pad_field_6BFF7C;
    struct_8 mStruct8_128Array_06BFF80[128];
    DWORD field_6C0380;
    DWORD field_6C0384;
    DWORD field_6C0388[6];
    DWORD field_6C03A0;
    DWORD field_6C03A4;
    DWORD field_6C03B0[81];
};
// TODO: This is actually probably bigger!
static_assert(sizeof(struct_lib_gv) == 0x60C, "struct_lib_gvd should be 0x60C");

MGS_VAR(1, 0x6BFEE0, struct_lib_gv, g_lib_gv_stru_6BFEE0, {});

// Other likely LibGvd funcs
MSG_FUNC_NOT_IMPL(0x40A72A, struct_8 *__cdecl(), LibGvd_sub_40A72A);
MSG_FUNC_NOT_IMPL(0x40A6CD, char* __cdecl(), LibGvd_sub_40A6CD);
MSG_FUNC_NOT_IMPL(0x40A603, int __cdecl(int), LibGvd_sub_40A603);

void LibGVCppForceLink() { }

MSG_FUNC_NOT_IMPL(0x40B35E, void __cdecl(), LibGV_Reset_System2_Memory_40B35E);
MSG_FUNC_NOT_IMPL(0x40A6AC, void __cdecl(), LibGV_Init_Allocs_40A6AC);
MSG_FUNC_NOT_IMPL(0x40A4B1, void __cdecl(), sub_40A4B1);
MSG_FUNC_NOT_IMPL(0x40A54E, void __cdecl(Actor*), LibGV_Update_40A54E);

MGS_VAR_EXTERN(int, gActiveBuffer_dword_791A08);

void LibGv_Init_sub_40A4F6()
{
    ActorList_Init();
    LibGV_Reset_System2_Memory_40B35E();
    LibGv_ClearFunctionPointers_40A69D();
    LibGV_Init_Allocs_40A6AC();
    sub_40A4B1();
    Actor_PushBack(0, &g_lib_gv_stru_6BFEE0.mBase, nullptr);
    Actor_Init(&g_lib_gv_stru_6BFEE0.mBase, LibGV_Update_40A54E.Ptr(), nullptr, "C:\\mgs\\source\\LibGV\\gvd.c");
    gActiveBuffer_dword_791A08 = 0;
    g_lib_gv_stru_6BFEE0.gRenderedFramesCount_dword_6BFF00 = 0;
    //nullsub_6(nullsub_4);
}
MSG_FUNC_IMPL(0x40A4F6, LibGv_Init_sub_40A4F6);

void CC LibGv_ClearFunctionPointers_40A69D()
{
    memset(g_lib_gv_stru_6BFEE0.field_6BFF0C_fn_ptrs, 0, sizeof(g_lib_gv_stru_6BFEE0.field_6BFF0C_fn_ptrs));
}
MSG_FUNC_IMPL(0x40A69D, LibGv_ClearFunctionPointers_40A69D);


void __cdecl LibGV_SetFnPtr_sub_40A68D(char id, GV_FnPtr fn)
{
    // Convert the a-z index to 0-25
    const int idx = id - 'a';

    assert(idx < _countof(g_lib_gv_stru_6BFEE0.field_6BFF0C_fn_ptrs));
    g_lib_gv_stru_6BFEE0.field_6BFF0C_fn_ptrs[idx] = fn;
}
MSG_FUNC_IMPL(0x40A68D, LibGV_SetFnPtr_sub_40A68D);

struct_8 *__cdecl LibGvd_sub_40A618(DWORD id)
{
    signed int cnt;
    int v4;

    cnt = 128;
    v4 = 128 - id % 128;
    struct_8* result = &g_lib_gv_stru_6BFEE0.mStruct8_128Array_06BFF80[id % 128];

    while (result->mId & 0xFFFFFF)
    {
        if ((result->mId & 0xFFFFFF) == id) // 3 bytes are id
        {
            return result;
        }

        ++result;

        if (!--v4)
        {
            result = &g_lib_gv_stru_6BFEE0.mStruct8_128Array_06BFF80[0];
        }

        if (--cnt <= 0)
        {
            g_lib_gv_stru_6BFEE0.struct_8_ptr_6BFF08 = 0;
            return 0;
        }
    }
    g_lib_gv_stru_6BFEE0.struct_8_ptr_6BFF08 = result;
    return 0;
}
MSG_FUNC_IMPL(0x40A618, LibGvd_sub_40A618);

signed int __cdecl LibGvd_sub_40A662(int a1, DWORD* a2)
{
    if (!LibGvd_sub_40A618(a1) && g_lib_gv_stru_6BFEE0.struct_8_ptr_6BFF08 != 0)
    {
        g_lib_gv_stru_6BFEE0.struct_8_ptr_6BFF08->mId = a1;
        g_lib_gv_stru_6BFEE0.struct_8_ptr_6BFF08->field_4 = a2;
        return 0;
    }
    return -1;
}
MSG_FUNC_IMPL(0x40A662, LibGvd_sub_40A662);
