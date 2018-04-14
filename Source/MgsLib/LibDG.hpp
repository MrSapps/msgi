#pragma once

#include "MgsFunction.hpp"
#include "Actor.hpp"
#include "Psx.hpp"
#include "LibGV.hpp"
#include "types.hpp"
#include "Kmd.hpp"

void LibDGCpp_ForceLink();

using ResInitFn = Actor*(CC *)(DWORD scriptData, int scriptBinds, BYTE* pScript);

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
MGS_VAR_EXTERN(PSX_MATRIX, gIdentity_matrix_6501F8);

void* CC MemClearUnknown_40B231(void* pMem, int size);


struct Prim_unknown
{
    PSX_MATRIX field_0_matrix;
    void* field_20;
    DWORD field_24_maybe_flags;
    WORD field_28_dword_9942A0;
    WORD field_2A_num_items;
    WORD field_2C_index;
    WORD field_2E_w_or_h;
    WORD field_30_size;
    WORD field_32;
    WORD field_34;
    WORD field_36;
    DWORD field_38_size24b;
    DWORD field_3C;
    BYTE* field_40_pDataStart[2];
    DWORD field_48; // Prim_unknown** ?
    DWORD field_4C;
    DWORD field_50; // field_68 == ?? // 0x54-0x68=0x14 20 bytes into first prim ?? 
    // field_84
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

    BYTE* mOrderingTables[2]; // 257 pointers? // One for each active buffer
    s16 word_6BC374_8;
    s16 word_6BC376_16;
    s16 word_6BC378_1;
    s16 word_6BC37A_0_1EC_size;
    PSX_MATRIX field_10_matrix;
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
    Prim_Union** gObjects_dword_6BC3C4;
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
MGS_VAR_EXTERN(struct_gv, gLibGvStruct2_6BC558);

MGS_ARY_EXTERN(DWORD, 2, dword_6BED18);


signed int CC GV_bin_file_handler_44E9D2(void* pData, TFileNameHash fileNameHash);
void CC LibDG_SetActiveResourceInitFuncPtrs_457B5B();
void CC LibDG_ClearActiveResourceFunctionPointerList_457B7C();
ResInitFn CC LibDG_GetResourceInitFuncPtr_457BAC(WORD hashedName);
void CC LibDg_Init_40111A();

signed int CC Script_tbl_chara_451AC3(BYTE* pScript);
signed int CC PrimAdd_401805(Prim_Union* pPrimBuffer);
void CC LibDG_ExecFnPtrs_40171C(int activeBuffer);
void CC LibDG_ClearTexturesCache_402487();
void CC LibDG_Restore_Textures_From_Resident_Memory_40274C();
void CC LibDG_Save_Texture_Hashes_To_Resident_Memory_4026F5();
void CC LibDG_Clear_Resident_Texture_Cache_Copy_4026E6();
signed int CC LibDG_SearchForTextureRecord_4024D2(signed int hashCode, Texture_Record** ppFreeItem);
void CC LibDG_SetRGB_401931(BYTE r, BYTE b, BYTE g);
Prim_Union* CC PrimAlloc_405050(int maybeFlags, int numItems, __int16 gv_index, void* size, void* field_3C);
Texture_Record* CC LibDG_FindTexture_4024A0(WORD hashCode);
void CC LibDG_4010A6();
void CC LibDG_Update2_helper_40A857();
void CC sub_40191F();


struct Psx_ScratchPad_Raw
{
    BYTE field_0[1024];
};
MGS_ASSERT_SIZEOF(Psx_ScratchPad_Raw, 1024);

struct Psx_ScratchPad_Matrix
{
    PSX_MATRIX mtx[32];
};

union Psx_ScratchPad
{
    Psx_ScratchPad_Raw field_0_raw;
    SVECTOR field_1_8Vecs[8];
    Psx_ScratchPad_Matrix field_2_Matrix;
};
MGS_ASSERT_SIZEOF(Psx_ScratchPad, 1024);

MGS_VAR_EXTERN(Psx_ScratchPad, gScratchPadMemory_991E40);
