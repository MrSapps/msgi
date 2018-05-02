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
    PSX_MATRIX dword_6BC39C;
    s16 word_6BC3BC; // Camera number?
    s16 mTotalQueueSize;
    s16 mFreePrimCount;
    s16 mTotalObjectCount;
    Prim_Union** mQueue;
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
Prim_Union* CC PrimAlloc_405050(int maybeFlags, int numItems, __int16 gv_index, Prim_24b* size, void* field_3C);
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
    SVECTOR vecs_42[42];
    Psx_ScratchPad_Matrix field_2_Matrix;
};
MGS_ASSERT_SIZEOF(Psx_ScratchPad, 1024);

MGS_VAR_EXTERN(Psx_ScratchPad, gScratchPadMemory_991E40);

Prim_24b* CC LibGV_ProcessAndStoreInScratch_4045A5(Prim_24b* pIn, int count);
int CC LibGV_prim_buffer_allocate_texture_and_shade_40730A(Prim_Mesh_0x5C* pMeshObj, int activeBuffer, BYTE flags);
void CC LibGV_void_active_prim_buffer_4073E8(Prim_Mesh_0x5C* pMesh, int activeBuffer);
void CC MarkObjectQueueVoid_4018E0();

void CC LibGV_prim_buffer_set_shade_colour_4072B7(Prim_Mesh_0x5C* pMesh, int activeBuffer);

void DoDGTests();
