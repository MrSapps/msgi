#pragma once

#include "MgsFunction.hpp"
#include "Actor.hpp"
#include "Psx.hpp"

void LibDGCpp_ForceLink();

using ResInitFn = DWORD(CC *)(DWORD scriptData, int scriptBinds, BYTE* pScript);

struct Res_Init_Record
{
    WORD mHashedName;
    WORD mPadding;
    ResInitFn mInitFunctionPointer;
};
MGS_ASSERT_SIZEOF(Res_Init_Record, 0x8);

struct Texture_Record
{
    WORD mHashedName;
    BYTE mUsed;
    BYTE mNumColours;
    WORD mTPage;
    WORD mClut;
    BYTE u0;
    BYTE v0;
    BYTE u1;
    BYTE v1;
};
MGS_ASSERT_SIZEOF(Texture_Record, 0xC);

struct LibDG_Struct
{
    Actor mBase;
    DWORD dword_6BB950_do_not_flip_buffers;
    DWORD dword_6BB954;
    BYTE gG_byte_6BB958;
    BYTE gB_byte_6BB959;
    BYTE gR_byte_6BB95A;
    BYTE mPadding;
};
MGS_ASSERT_SIZEOF(LibDG_Struct, 0x2C);
MGS_VAR_EXTERN(LibDG_Struct, gLibDG_2_stru_6BB930);
MGS_VAR_EXTERN(DWORD, gBinds_dword_722A40);

EXTERN_MGS_FUNC_NOT_IMPL(0x40B231, unsigned int CC(void* pMem, int size), MemClearUnknown_40B231);


struct Prim_base
{
    DWORD field_0_ptr;
    DWORD field_4_ptr;
    DWORD field_8_ptr;
    DWORD field_C_ptr;
    DWORD field_10_countq;
    DWORD field_14;
    DWORD field_18;
    DWORD field_1C;
};
MGS_ASSERT_SIZEOF(Prim_base, 0x20);

struct Prim_unknown
{
    Prim_base mBase;
    DWORD field_20;
    DWORD field_24_maybe_flags;
    WORD field_28_dword_9942A0;
    WORD field_2A_num_items;
    WORD field_2C_r_index;
    WORD field_2E_w_or_h;
    WORD field_30_size;
    WORD field_32;
    WORD field_34;
    WORD field_36;
    DWORD field_38_g;
    DWORD field_3C_b;
    BYTE* field_40_pDataStart[2];
    DWORD field_48;
    DWORD field_4C;
    DWORD field_50;
};
MGS_ASSERT_SIZEOF(Prim_unknown, 0x54);


#pragma pack(push)
#pragma pack(2)
struct Prim_Object
{
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20_size0x20;
    int field_24;
    int field_28_flags;
    s16 field_2C;
    s16 field_2E_inner_count;
    s16 field_30;
    s16 field_32_hasInners;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48_92b_size;
    int field_4C;
    s16 field_50_fn_ptr;
    s16 field_52;
    s16 field_54;
    int field_56;
    s16 field_5A;  // This de-aligns the rest of the struct
    int field_5C;
    int field_60;
    int field_64;
    int field_68_92b_size;
    int field_6C;
    int field_70_pInners;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int field_9C_array_ptr;
    int field_A0;
};
MGS_ASSERT_SIZEOF(Prim_Object, 0xA4);

struct struct_gv
{
    // These vars are named to match how they look from gLibGVStruct1_6BC36C position, it was later discovered
    // that these are item 1 in an array of 3 struct_gvs

    int mOrderingTables[2]; // 257 pointers? // One for each active buffer
    s16 word_6BC374_8;
    s16 word_6BC376_16;
    s16 word_6BC378_1;
    s16 word_6BC37A_0_1EC_size;
    int dword_6BC37C_32byte_size;
    int dword_6BC380;
    int dword_6BC384;
    int dword_6BC388;
    int dword_6BC38C;
    int dword_6BC390;
    int dword_6BC394;
    int dword_6BC398;
    int dword_6BC39C;
    s16 word_6BC3A0;
    s16 word_6BC3A6;
    PSX_RECT rect;
    s16 word_6BC3AC;
    int align1;
    s16 align2;
    int dword_6BC3B4;
    int dword_6BC3B8;
    s16 word_6BC3BC;
    s16 g_PrimQueue1_word_6BC3BE_256;
    s16 gPrimQueue2_word_6BC3C0_256;
    s16 gObjectQueue_word_6BC3C2_0;
    Prim_unknown **gObjects_dword_6BC3C4;
    PSX_RECT dword_6BC3C8_pStructure_rect;
    PSX_RECT dword_6BC3D0_rect;
    int dword_6BC3D8_dst[16]; // One for each active buffer
    int dword_6BC418_dst[16];
    int dword_6BC458[16];
    int dword_6BC498[16];
    DR_ENV dword_6BC4D8_src_dr_env1;
    DR_ENV dword_6BC518_src_offsetted_dr_evn;
};
MGS_ASSERT_SIZEOF(struct_gv, 0x1EC);
#pragma pack(pop)

MGS_VAR_EXTERN(struct_gv, gLibGvStruct0_6BC180);
MGS_VAR_EXTERN(struct_gv, gLibGVStruct1_6BC36C);
MGS_VAR_EXTERN(struct_gv, gLibGVStruct2_6BC558);

MGS_ARY_EXTERN(DWORD, 2, dword_6BED18);


signed int CC GV_bin_file_handler_44E9D2(void* pData, WORD fileNameHash);
void CC LibDG_SetActiveResourceInitFuncPtrs_457B5B();
void CC LibDG_ClearActiveResourceFunctionPointerList_457B7C();
ResInitFn CC LibDG_GetResourceInitFuncPtr_457BAC(WORD hashedName);
void CC LibDg_Init_40111A();

signed int CC Script_tbl_chara_451AC3(BYTE* pScript);
signed int CC PrimAdd_401805(Prim_unknown* pPrimBuffer);
void CC LibDG_ExecFnPtrs_40171C(int activeBuffer);
void CC LibDG_ClearTexturesCache_402487();
void CC LibDG_Restore_Textures_From_Resident_Memory_40274C();
void CC LibDG_Save_Texture_Hashes_To_Resident_Memory_4026F5();
void CC LibDG_Clear_Resident_Texture_Cache_Copy_4026E6();
signed int CC LibDG_SearchForTextureRecord_4024D2(signed int hashCode, Texture_Record** ppFreeItem);
void CC LibDG_SetRGB_401931(BYTE r, BYTE b, BYTE g);
