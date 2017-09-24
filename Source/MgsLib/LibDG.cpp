#include "stdafx.h"
#include "LibDG.hpp"
#include "Script.hpp"
#include "LibGV.hpp"
#include "Actor_GameD.hpp"
#include <assert.h>

#define LIBDG_IMPL true

void LibDGCpp_ForceLink() { }

#define REDIRECT_LIBDG_DATA 1

MGS_VAR_EXTERN(int, gActiveBuffer_dword_791A08);

MGS_ARY(REDIRECT_LIBDG_DATA, 0x669AE0, Res_Init_Record, 512, gKnownResInitFuncs_669AE0,
{
    { 0x002A, 0, (ResInitFn)0x00519A61 },                       { 0x002F, 0, (ResInitFn)0x00519A9D },
    { 0x0030, 0, (ResInitFn)0x00600135 },                       { 0x0009, 0, (ResInitFn)0x005FFC1C },
    { 0x000D, 0, (ResInitFn)0x005FF35B },                       { 0x0048, 0, (ResInitFn)0x005C5666 },
    { 0x0047, 0, (ResInitFn)0x005C5111 },                       { 0x000A, 0, (ResInitFn)0x005FEFB9 },
    { 0x0025, 0, (ResInitFn)0x005F7EE2 },                       { 0x004A, 0, (ResInitFn)0x005683EF },
    { 0x000E, 0, (ResInitFn)0x0051B59B },                       { 0x0012, 0, (ResInitFn)0x005FECC3 },
    { 0x0013, 0, (ResInitFn)0x005FE85B },                       { 0x000B, 0, (ResInitFn)0x005FE69B },
    { 0x0033, 0, (ResInitFn)0x0051B1CF },                       { 0x003A, 0, (ResInitFn)0x005C4C4E },
    { 0x0040, 0, (ResInitFn)0x005C445E },                       { 0x0039, 0, (ResInitFn)0x005FD769 },
    { 0x0034, 0, (ResInitFn)0x0051B051 },                       { 0x0015, 0, (ResInitFn)0x00628ECC },
    { 0x0016, 0, (ResInitFn)0x005BB2D5 },                       { 0x0037, 0, (ResInitFn)0x005DF33A },
    { 0x0038, 0, (ResInitFn)0x005DF50D },                       { 0x001C, 0, (ResInitFn)0x00628A91 },
    { 0x0001, 0, (ResInitFn)0x0043EC38 },                       { 0x0003, 0, (ResInitFn)0x00441CF2 },
    { 0x0004, 0, (ResInitFn)0x00441CF2 },                       { 0x0045, 0, (ResInitFn)0x005C41C3 },
    { 0x0010, 0, (ResInitFn)0x005ABCB7 },                       { 0x0044, 0, (ResInitFn)0x005E7FC9 },
    { 0x0019, 0, (ResInitFn)0x0051AED6 },                       { 0x0042, 0, (ResInitFn)0x0061FFFF },
    { 0x0022, 0, (ResInitFn)0x005BB23E },                       { 0x0046, 0, (ResInitFn)0x006288F0 },
    { 0x003B, 0, (ResInitFn)0x005C3D78 },                       { 0x003F, 0, (ResInitFn)0x005C3580 },
    { 0x0017, 0, (ResInitFn)0x0062862E },                       { 0x0018, 0, (ResInitFn)0x005BB2D5 },
    { 0x0020, 0, (ResInitFn)0x005C31B7 },                       { 0x004B, 0, (ResInitFn)0x005FD23A },
    { 0x000C, 0, (ResInitFn)0x0051A97A },                       { 0x0028, 0, (ResInitFn)0x0062852B },
    { 0x0011, 0, (ResInitFn)0x0051AD2A },                       { 0x0049, 0, (ResInitFn)0x00627B86 },
    { 0x0041, 0, (ResInitFn)0x005C2454 },                       { 0x002D, 0, (ResInitFn)0x005ABC28 },
    { 0x0035, 0, (ResInitFn)0x0051AB83 },                       { 0x0036, 0, (ResInitFn)0x0051AC56 },
    { 0x001A, 0, (ResInitFn)0x005BAC0B },                       { 0x001B, 0, (ResInitFn)0x005BB0B2 },
    { 0x001D, 0, (ResInitFn)0x005E1D68 },                       { 0x0024, 0, (ResInitFn)0x005E1E8B },
    { 0x0007, 0, (ResInitFn)0x00519E54 },                       { 0x0029, 0, (ResInitFn)0x0063F7DF },
    { 0x0014, 0, (ResInitFn)0x005B9F52 },                       { 0x001F, 0, (ResInitFn)0x006282D3 },
    { 0x000F, 0, (ResInitFn)0x005AB3EA },                       { 0x0032, 0, (ResInitFn)0x005C22AC },
    { 0x002C, 0, (ResInitFn)0x004438CF },                       { 0x0008, 0, (ResInitFn)0x00519DEC },
    { 0x0002, 0, (ResInitFn)0x00519A25 },                       { 0x0031, 0, (ResInitFn)0x005FD006 },
    { 0x003C, 0, (ResInitFn)0x00627ECA },                       { 0x003D, 0, (ResInitFn)0x005FCA8F },
    { 0x003E, 0, (ResInitFn)0x00519504 },                       { 0x0026, 0, (ResInitFn)0x005197BC },
    { 0x0027, 0, (ResInitFn)0x00519838 },                       { 0x002E, 0, (ResInitFn)0x005FBEE0 },
    { 0x0043, 0, (ResInitFn)0x00627C02 },                       { 0x0006, 0, (ResInitFn)0x00519E54 },
    { 0x0005, 0, (ResInitFn)0x006154A7 },                       { 0x004C, 0, (ResInitFn)0x006153CF },
    { 0x0021, 0, (ResInitFn)0x00627BFF },                       { 0x0023, 0, (ResInitFn)0x005FF35B },
    { 0x002B, 0, (ResInitFn)0x005FF35B },                       { 0x001E, 0, (ResInitFn)0x00627715 },
    { 0x40E9, 0, (ResInitFn)0x005FBB0A },                       { 0xE068, 0, (ResInitFn)0x005FAF3E },
    { 0xD53C, 0, (ResInitFn)0x005FA06A },                       { 0x153E, 0, (ResInitFn)0x005F9170 },
    { 0x9FFD, 0, (ResInitFn)0x00457B50 },                       { 0x2D0A, 0, (ResInitFn)0x00457AC1 },
    { 0x566F, 0, (ResInitFn)0x004B3B01 },                       { 0x4974, 0, (ResInitFn)0x004B0578 },
    { 0x4975, 0, (ResInitFn)0x004AC173 },                       { 0x05EF, 0, (ResInitFn)0x005F8FFA },
    { 0x2A90, 0, (ResInitFn)0x00627435 },                       { 0x02C4, 0, (ResInitFn)0x005AB396 },
    { 0x92BC, 0, (ResInitFn)0x005AA507 },                       { 0x9988, 0, (ResInitFn)0x004ABBC9 },
    { 0xF59E, 0, (ResInitFn)0x00514A19 },                       { 0x6B6C, 0, (ResInitFn)0x005682F7 },
    { 0x8185, 0, (ResInitFn)0x005F8E18 },                       { 0x1158, 0, (ResInitFn)0x005F8573 },
    { 0x4E95, 0, (ResInitFn)0x005F820B },                       { 0x6A4C, 0, (ResInitFn)0x005F7EE8 },
    { 0x9093, 0, (ResInitFn)0x005F7EE5 },                       { 0xF74B, 0, (ResInitFn)0x005142ED },
    { 0x32FC, 0, (ResInitFn)0x006270F3 },                       { 0x6471, 0, (ResInitFn)0x00626FE4 },
    { 0x1A02, 0, (ResInitFn)0x005F781C },                       { 0x93B6, 0, (ResInitFn)0x004A2B6B },
    { 0x7ECA, 0, (ResInitFn)0x0049FC71 },                       { 0x6E90, 0, (ResInitFn)0x005A9076 },
    { 0x56CC, 0, (ResInitFn)0x005A816F },                       { 0xA9C5, 0, (ResInitFn)0x005F5F72 },
    { 0x7BC2, 0, (ResInitFn)0x00626EA8 },                       { 0xB99F, 0, (ResInitFn)0x0049EA8B },
    { 0x51C6, 0, (ResInitFn)0x00626C7D },                       { 0xF722, 0, (ResInitFn)0x0046BC36 },
    { 0x43A0, 0, (ResInitFn)0x006262A0 },                       { 0x788D, 0, (ResInitFn)0x00625DC1 },
    { 0x7A05, 0, (ResInitFn)0x00625D63 },                       { 0x3C0C, 0, (ResInitFn)0x005A04D6 },
    { 0xC6D7, 0, (ResInitFn)0x005A7099 },                       { 0xCC45, 0, (ResInitFn)0x005F2E0F },
    { 0x22C6, 0, (ResInitFn)0x005A6143 },                       { 0xECED, 0, (ResInitFn)0x004629FE },
    { 0xA3FB, 0, (ResInitFn)0x005F1A1D },                       { 0x8E60, 0, (ResInitFn)0x005F0B54 },
    { 0xBE79, 0, (ResInitFn)0x005F07A4 },                       { 0x6A98, 0, (ResInitFn)0x00513F69 },
    { 0x76BC, 0, (ResInitFn)0x0051368C },                       { 0xB4E6, 0, (ResInitFn)0x00457974 },
    { 0xE97E, 0, (ResInitFn)0x0056781A },                       { 0x3686, 0, (ResInitFn)0x0049B6AD },
    { 0x9F7D, 0, (ResInitFn)0x00511789 },                       { 0x9D00, 0, (ResInitFn)0x00624FDF },
    { 0x58F0, 0, (ResInitFn)0x00624A97 },                       { 0x6C0E, 0, (ResInitFn)0x005EA179 },
    { 0xB997, 0, (ResInitFn)0x0040BB8B },                       { 0x73F8, 0, (ResInitFn)0x00624208 },
    { 0xB58D, 0, (ResInitFn)0x00623B32 },                       { 0x4BE8, 0, (ResInitFn)0x00623714 },
    { 0x3303, 0, (ResInitFn)0x005E9A03 },                       { 0xAF6C, 0, (ResInitFn)0x0062345B },
    { 0xB103, 0, (ResInitFn)0x0062330F },                       { 0xE75A, 0, (ResInitFn)0x0062305A },
    { 0x107C, 0, (ResInitFn)0x00622EEF },                       { 0xAEF2, 0, (ResInitFn)0x00622D21 },
    { 0x2ABC, 0, (ResInitFn)0x00621BDD },                       { 0x9AB9, 0, (ResInitFn)0x005E80FE },
    { 0xE253, 0, (ResInitFn)0x00456960 },                       { 0x32E5, 0, (ResInitFn)0x0063F60E },
    { 0xA9DD, 0, (ResInitFn)0x0063F43D },                       { 0xB789, 0, (ResInitFn)0x0062190F },
    { 0x3F9A, 0, (ResInitFn)0x00621797 },                       { 0x76FE, 0, (ResInitFn)0x0062130E },
    { 0xA12E, 0, (ResInitFn)0x0044200E },                       { 0xC73E, 0, (ResInitFn)0x005E7FCF },
    { 0x5D64, 0, (ResInitFn)0x00620DA7 },                       { 0x118C, 0, (ResInitFn)0x00620978 },
    { 0xD6FB, 0, (ResInitFn)0x00620833 },                       { 0x84E1, 0, (ResInitFn)0x005115AE },
    { 0x0FAD, 0, (ResInitFn)0x00511497 },                       { 0xADD8, 0, (ResInitFn)0x0062052F },
    { 0x8D5A, 0, (ResInitFn)0x006203E0 },                       { 0x5A50, 0, (ResInitFn)0x0061FE1B },
    { 0x2477, 0, (ResInitFn)0x0061F9A8 },                       { 0xCCD3, 0, (ResInitFn)0x005E7FCC },
    { 0x8E70, 0, (ResInitFn)0x0061F2D5 },                       { 0xDB1F, 0, (ResInitFn)0x004DAA01 },
    { 0x9EB7, 0, (ResInitFn)0x00567203 },                       { 0xCB1F, 0, (ResInitFn)0x0056653D },
    { 0x5F5A, 0, (ResInitFn)0x0063F0EF },                       { 0xCBEB, 0, (ResInitFn)0x005E76AA },
    { 0x0D9B, 0, (ResInitFn)0x005E6DD7 },                       { 0x42E4, 0, (ResInitFn)0x005E6A41 },
    { 0xDD8B, 0, (ResInitFn)0x00511349 },                       { 0x8767, 0, (ResInitFn)0x0044D086 },
    { 0x6A48, 0, (ResInitFn)0x0061E1DD },                       { 0xA27E, 0, (ResInitFn)0x0061DDFA },
    { 0xB757, 0, (ResInitFn)0x00462532 },                       { 0x1EF9, 0, (ResInitFn)0x0050C653 },
    { 0x81C7, 0, (ResInitFn)0x0049B678 },                       { 0x4F02, 0, (ResInitFn)0x005B9F42 },
    { 0xC6AC, 0, (ResInitFn)0x005E66A1 },                       { 0x52BF, 0, (ResInitFn)0x005A60A2 },
    { 0xA6F6, 0, (ResInitFn)0x004421C2 },                       { 0x8B1A, 0, (ResInitFn)0x0050BF9B },
    { 0x711F, 0, (ResInitFn)0x0061DC48 },                       { 0x425F, 0, (ResInitFn)0x0061D52C },
    { 0x921B, 0, (ResInitFn)0x0061CCCC },                       { 0x7BF2, 0, (ResInitFn)0x005652F4 },
    { 0xCF72, 0, (ResInitFn)0x0049AD6B },                       { 0x53C7, 0, (ResInitFn)0x004606E2 },
    { 0x9D71, 0, (ResInitFn)0x005C0C8B },                       { 0xD5EC, 0, (ResInitFn)0x005BFEA1 },
    { 0xE448, 0, (ResInitFn)0x0061B8B9 },                       { 0x450F, 0, (ResInitFn)0x0050A2E4 },
    { 0xAF72, 0, (ResInitFn)0x00509679 },                       { 0xC755, 0, (ResInitFn)0x00565013 },
    { 0x5078, 0, (ResInitFn)0x005A53CA },                       { 0xE271, 0, (ResInitFn)0x005644F4 },
    { 0x4754, 0, (ResInitFn)0x005BE3F8 },                       { 0x954B, 0, (ResInitFn)0x0061B32D },
    { 0x1787, 0, (ResInitFn)0x005BD2D4 },                       { 0xC218, 0, (ResInitFn)0x0061AB21 },
    { 0xA2B5, 0, (ResInitFn)0x0045692F },                       { 0x7ACF, 0, (ResInitFn)0x005E61CC },
    { 0x6D78, 0, (ResInitFn)0x0061A71C },                       { 0x0065, 0, (ResInitFn)0x0061A683 },
    { 0xD4A5, 0, (ResInitFn)0x005E52D5 },                       { 0x3453, 0, (ResInitFn)0x00456860 },
    { 0x5345, 0, (ResInitFn)0x004561C6 },                       { 0xB98C, 0, (ResInitFn)0x005A4F47 },
    { 0xD3C0, 0, (ResInitFn)0x0050964C },                       { 0xF002, 0, (ResInitFn)0x00508856 },
    { 0x30BA, 0, (ResInitFn)0x00563A09 },                       { 0x4811, 0, (ResInitFn)0x005A473D },
    { 0xCF79, 0, (ResInitFn)0x00486BD4 },                       { 0x3AC3, 0, (ResInitFn)0x00482D1F },
    { 0xCFEF, 0, (ResInitFn)0x0047EE6E },                       { 0x976C, 0, (ResInitFn)0x0047A5E5 },
    { 0x8D31, 0, (ResInitFn)0x004792A6 },                       { 0xB916, 0, (ResInitFn)0x00477F58 },
    { 0xCBF8, 0, (ResInitFn)0x00456093 },                       { 0x3ED7, 0, (ResInitFn)0x0061A311 },
    { 0x720D, 0, (ResInitFn)0x00539051 },                       { 0xFED1, 0, (ResInitFn)0x00455F32 },
    { 0x20F2, 0, (ResInitFn)0x00619C50 },                       { 0xDD95, 0, (ResInitFn)0x006198AB },
    { 0x5F02, 0, (ResInitFn)0x006196C0 },                       { 0x30CE, 0, (ResInitFn)0x005E40BB },
    { 0xFDB6, 0, (ResInitFn)0x004612E8 },                       { 0x8FDC, 0, (ResInitFn)0x00462529 },
    { 0x6414, 0, (ResInitFn)0x00462395 },                       { 0xAA13, 0, (ResInitFn)0x00460D44 },
    { 0xCF00, 0, (ResInitFn)0x00460D1C },                       { 0x10FF, 0, (ResInitFn)0x00508209 },
    { 0xEDEB, 0, (ResInitFn)0x00505D37 },                       { 0x8427, 0, (ResInitFn)0x005E3D51 },
    { 0x17F5, 0, (ResInitFn)0x005E372A },                       { 0x1FA5, 0, (ResInitFn)0x0043EEFB },
    { 0xB032, 0, (ResInitFn)0x005E316B },                       { 0xB033, 0, (ResInitFn)0x005E2B67 },
    { 0xB7AE, 0, (ResInitFn)0x005E262D },                       { 0xBC76, 0, (ResInitFn)0x005E2053 },
    { 0x9C6C, 0, (ResInitFn)0x0061947E },                       { 0xB47A, 0, (ResInitFn)0x0061870B },
    { 0x9BC2, 0, (ResInitFn)0x005E1032 },                       { 0xFC0B, 0, (ResInitFn)0x005E0C21 },
    { 0x3AD2, 0, (ResInitFn)0x00505B87 },                       { 0xCC89, 0, (ResInitFn)0x005E01D2 },
    { 0x6D1B, 0, (ResInitFn)0x00476B30 },                       { 0xD8DD, 0, (ResInitFn)0x005054F4 },
    { 0xBF66, 0, (ResInitFn)0x00504B42 },                       { 0xA404, 0, (ResInitFn)0x005045B7 },
    { 0x27A2, 0, (ResInitFn)0x0050492D },                       { 0xA76F, 0, (ResInitFn)0x004E8181 },
    { 0xF4B0, 0, (ResInitFn)0x004FFA7A },                       { 0x710D, 0, (ResInitFn)0x004FEEC7 },
    { 0x91AA, 0, (ResInitFn)0x006183F2 },                       { 0x169C, 0, (ResInitFn)0x005DF74E },
    { 0x5147, 0, (ResInitFn)0x00455E18 },                       { 0x917B, 0, (ResInitFn)0x005DF51B }
});

// An zeroed array initially as the name suggests it is dynamically populated at run time.
MGS_ARY(REDIRECT_LIBDG_DATA, 0x994320, Res_Init_Record, 512, gDynamicResFuncs_word_994320, {} );

MGS_ARY(REDIRECT_LIBDG_DATA, 0x650040, Res_Init_Record, 3, gStaticResInitFuncs_stru_650040,
{
    { 0x21CA, 0, (ResInitFn)0x004E1701 },
    { 0x8767, 0, (ResInitFn)0x0044D086 },
    { 0xB997, 0, (ResInitFn)0x0040BB8B }
});

MGS_PTR(REDIRECT_LIBDG_DATA, 0x993F44, Res_Init_Record**, gpToDynamicResInitFuncs_dword_993F44, nullptr);

MGS_VAR(REDIRECT_LIBDG_DATA, 0x994304, DWORD, gSleep5000_after_res_funcs_1_dword_994304, 0);
MGS_VAR(REDIRECT_LIBDG_DATA, 0x99430C, DWORD, gSleep5000_after_res_funcs_2_dword_99430C, 0);

MGS_VAR(REDIRECT_LIBDG_DATA, 0x6BB910, Actor, gLibDGD_1_stru_6BB910, {});
MGS_VAR(REDIRECT_LIBDG_DATA, 0x6BB930, LibDG_Struct, gLibDG_2_stru_6BB930, {});

// TODO: Script_tbl_get_res_init_fn_457B9A - call GCL script to get res fn func ptr, then caller creates the resource
MGS_FUNC_NOT_IMPL(0x457B9A, ResInitFn CC(BYTE *pScript), Script_tbl_get_res_init_fn_457B9A);
MGS_VAR(REDIRECT_LIBDG_DATA, 0x722A40, DWORD, dword_722A40, 0);

MGS_VAR(1, 0x6BC180, struct_gv, gLibGvStruct0_6BC180, {}); // TODO: Probably an array of 3?
MGS_VAR(1, 0x6BC36C, struct_gv, gLibGVStruct1_6BC36C, {});
MGS_VAR(1, 0x6BC558, struct_gv, gLibGVStruct2_6BC558, {});

signed int CC Script_tbl_chara_sub_451AC3(BYTE* pScript)
{
    BYTE* scriptRet = Script_GetReturnAddress();
    ResInitFn pResFn = Script_tbl_get_res_init_fn_457B9A(scriptRet);
    if (pResFn)
    {
        BYTE* scriptRet2 = Script_GetReturnAddress();
        DWORD scriptUnknown = Script_Unknown8(scriptRet2);
        pResFn(scriptUnknown, dword_722A40, pScript);
        return 0;
    }
    return -1;
}
MGS_FUNC_IMPLEX(0x00451AC3, Script_tbl_chara_sub_451AC3, LIBDG_IMPL);

signed int CC LibDG_CHARA_44E9D2(void* pData, int fileNameHash)
{
    Res_Init_Record* pStartingRecord = reinterpret_cast<Res_Init_Record*>(pData);
    int dst_idx = 0;
    for (;;)
    {
        if (!pStartingRecord->mInitFunctionPointer)
        {
            break;
        }

        int src_idx = 0;
        while (gKnownResInitFuncs_669AE0[src_idx].mHashedName)
        {
            if (gKnownResInitFuncs_669AE0[src_idx].mHashedName == pStartingRecord->mHashedName)
            {
                gDynamicResFuncs_word_994320[dst_idx].mHashedName = pStartingRecord->mHashedName;
                gDynamicResFuncs_word_994320[dst_idx].mInitFunctionPointer = gKnownResInitFuncs_669AE0[src_idx].mInitFunctionPointer;
                break;
            }
            src_idx++;
        }
        printf("CHARA %d, %p\n", gDynamicResFuncs_word_994320[dst_idx].mHashedName, gDynamicResFuncs_word_994320[dst_idx].mInitFunctionPointer);

        ++dst_idx;
        ++pStartingRecord;
    }

    // Set last entry to 0 to terminate
    gDynamicResFuncs_word_994320[dst_idx].mHashedName = 0;
    gDynamicResFuncs_word_994320[dst_idx].mInitFunctionPointer = nullptr;
    
    if (gSleep5000_after_res_funcs_1_dword_994304 || gSleep5000_after_res_funcs_2_dword_99430C)
    {
        ::Sleep(5000);
    }

    gSleep5000_after_res_funcs_1_dword_994304 = 0;
    gSleep5000_after_res_funcs_2_dword_99430C = 0;

    return 1;
}
MGS_FUNC_IMPLEX(0x0044E9D2, LibDG_CHARA_44E9D2, LIBDG_IMPL);

void CC LibDG_SetActiveResourceInitFuncPtrs_457B5B()
{
    memcpy(gDynamicResFuncs_word_994320, gKnownResInitFuncs_669AE0, 4096u); // 4096u = sizeof(gResInitFuncs_669AE0)
    *gpToDynamicResInitFuncs_dword_993F44 = gDynamicResFuncs_word_994320;
}
MGS_FUNC_IMPLEX(0x457B5B, LibDG_SetActiveResourceInitFuncPtrs_457B5B, LIBDG_IMPL);

void CC LibDG_ClearActiveResourceFunctionPointerList_457B7C()
{
    memset(gDynamicResFuncs_word_994320, 0, 4096u); // 4096u = sizeof(gResInitFuncs_669AE0)
    *gpToDynamicResInitFuncs_dword_993F44 = gDynamicResFuncs_word_994320;
}
MGS_FUNC_IMPLEX(0x457B7C, LibDG_ClearActiveResourceFunctionPointerList_457B7C, LIBDG_IMPL);

static ResInitFn FindFnPtrByHash(Res_Init_Record* arrayToSearch, WORD hashName)
{
    Res_Init_Record* pRecord = arrayToSearch;
    while (pRecord->mInitFunctionPointer)
    {
        if (pRecord->mHashedName == hashName)
        {
            return pRecord->mInitFunctionPointer;
        }
        ++pRecord;
    }
    return nullptr;
}

ResInitFn CC LibDG_GetResourceInitFuncPtr_457BAC(WORD hashedName)
{
    // Search in the 3 "static" records
    ResInitFn result = FindFnPtrByHash(gStaticResInitFuncs_stru_650040, hashedName);
    if (result)
    {
        return result;
    }

    // Search in the 512 "dynamic" records
    result = FindFnPtrByHash(*gpToDynamicResInitFuncs_dword_993F44, hashedName);
    return result;
}
MGS_FUNC_IMPLEX(0x457BAC, LibDG_GetResourceInitFuncPtr_457BAC, LIBDG_IMPL);

MGS_FUNC_NOT_IMPL(0x401A4F, void CC(int ClipX1, __int16 clipY1, __int16 clipX2, __int16 clipY2, int a5), sub_401A4F);
MGS_FUNC_NOT_IMPL(0x4012F2, void CC(int k320), sub_4012F2);
MGS_FUNC_NOT_IMPL(0x4026E6, void CC(), LibDG_Reset_HashCounts_4026E6);
MGS_FUNC_NOT_IMPL(0x4010A6, void CC(), LibDG_4010A6);

signed int CC Returns1_402B1D(void*, int)
{
    return 1;
}
MGS_FUNC_IMPLEX(0x402B1D, Returns1_402B1D, LIBDG_IMPL);

MGS_FUNC_NOT_IMPL(0x402B25, int CC(void* fileData, int fileNameHash), MissionLog_Related1_402B25);
MGS_FUNC_NOT_IMPL(0x402796, int CC(void* fileData, int fileNameHash), sub_402796);
MGS_FUNC_NOT_IMPL(0x402A03, int CC(void* fileData, int fileNameHash), sub_402A03);
MGS_FUNC_NOT_IMPL(0x402A29, int CC(void* fileData, int fileNameHash), sub_402A29);
MGS_FUNC_NOT_IMPL(0x403290, int CC(void* fileData, int fileNameHash), sub_403290);
MGS_FUNC_NOT_IMPL(0x402A5F, int CC(void* fileData, int fileNameHash), sub_402A5F);
MGS_FUNC_NOT_IMPL(0x402AA9, int CC(void* fileData, int fileNameHash), sub_402AA9);

MGS_VAR(REDIRECT_LIBDG_DATA, 0x6BB908, DWORD, dword_6BB908, 0);

MGS_VAR(REDIRECT_LIBDG_DATA, 0x7919CE, DWORD, dword_7919CE, 0);
MGS_VAR(REDIRECT_LIBDG_DATA, 0x7919D4, DWORD, dword_7919D4, 0);
MGS_VAR(REDIRECT_LIBDG_DATA, 0x650090, DWORD, dword_650090, 0);


MGS_VAR(REDIRECT_LIBDG_DATA, 0x9942AB, BYTE, byte_9942AB, 0);



MGS_VAR_EXTERN(DWORD, game_state_dword_72279C);
MGS_FUNC_NOT_IMPL(0x40A857, void CC(), sub_40A857);
int CC Main_sub_401C02();

MGS_FUNC_NOT_IMPL(0x5200D2, signed __int64 CC(), sub_5200D2);

signed __int64 CC WaitFor_445580(int totalCount)
{
    __int64 result = 0;
    for (int i = 0; i < totalCount; i++)
    {
        result = sub_5200D2();
    }
    return result;
}
MGS_FUNC_IMPLEX(0x445580, WaitFor_445580, LIBDG_IMPL);

void CC LibDG_Update2_401234(Actor* pLibDg)
{
    static int sTimeStamp_dword_650094 = 0;

    dword_6BB908 = 0;

    gLibDG_2_stru_6BB930.dword_6BB954 = gLibDG_2_stru_6BB930.dword_6BB950_do_not_flip_buffers;

    if (BYTE3(game_state_dword_72279C) & 0x20)
    {
        int bDontFlipBuffers = gLibDG_2_stru_6BB930.dword_6BB950_do_not_flip_buffers;

        if (sTimeStamp_dword_650094 == -1)
        {
            sTimeStamp_dword_650094 = TimeGetElapsed_4455A0();
            bDontFlipBuffers = 0;
            gLibDG_2_stru_6BB930.dword_6BB950_do_not_flip_buffers = 0;
        }

        if (!bDontFlipBuffers)
        {
            WaitFor_445580(dword_650090);
        }

        // TODO: This is not 100% correct - but I don't think 
        // dword_6BB950_do_not_flip_buffers can ever be set to 1?
        //MGS_WARN_ONCE("Possibly wrong behavior here"); // TODO: Impl MGS_WARN_ONCE

        int elapsed = TimeGetElapsed_4455A0();
        if (elapsed - (sTimeStamp_dword_650094 + 2) < 0)
        {
            sTimeStamp_dword_650094 += 2;
        }
        gLibDG_2_stru_6BB930.dword_6BB950_do_not_flip_buffers = 0;
    }
    else
    {
        WaitFor_445580(dword_650090);
        sTimeStamp_dword_650094 = -1;
        gLibDG_2_stru_6BB930.dword_6BB950_do_not_flip_buffers = 0;
    }

    Main_sub_401C02();
    
    sub_40A857(); // Probably input related as input no longer works if not called?

    dword_995324 = (int)&dword_7919C0;

    if (byte_9942AB & 0x10)
    {
        if (HIWORD(dword_7919CE) | (WORD)dword_7919D4)
        {
            dword_995324 = 0x7919D0; // TODO: Is actually set to (int)&dword_7919CE + 2; ??
        }
    }
}
MGS_FUNC_IMPLEX(0x401234, LibDG_Update2_401234, LIBDG_IMPL);


MGS_VAR(1, 0x6BECE8, DWORD, gLibDG_ExecPtrs_6BECE8, 1);
MGS_ARY(1, 0x6BE4E8, DWORD*, 512, gUnkSize_1024_6BE4E8, {}); // TODO: Might just be 256?


MGS_FUNC_NOT_IMPL(0x40B231, unsigned int CC(void* pMem, int size), MemClearUnknown_40B231);

using TDG_FnPtr = void(CC*)(struct_gv* pGv, int activeBuffer);

MGS_FUNC_NOT_IMPL(0x4065AA, int CC(struct_gv* pGv, int activeBuffer), sub_4065AA);
MGS_FUNC_NOT_IMPL(0x4064B1, void CC(Prim_Object *pObject, int activeBuffer, char a3, int a4), sub_4064B1);
MGS_FUNC_NOT_IMPL(0x401DA8, int CC(PSX_RECT* pRect, int a2), sub_401DA8);

MGS_VAR(1, 0x78D32C, DWORD, dword_78D32C, 0);



void CC LibGV_4061E7(struct_gv* pGv, int activeBuffer)
{
    /*
    sub_401DA8((PSX_RECT*)&pGv->dword_6BC3C8_pStructure_rect, pGv->word_6BC3BC);

    Prim_Object **pObjects = pGv->gObjects_dword_6BC3C4;
    if (pGv->gObjectQueue_word_6BC3C2_0 > 0)
    {
        s16 objectQueueCount = pGv->gObjectQueue_word_6BC3C2_0;
        do
        {
            Prim_Object* pObject = *pObjects;
            ++pObjects;
            s16 v5 = 0;

            const int flags = pObject->field_28_flags;
            if (!(pObject->field_28_flags & 0x80) && (!pObject->field_2C || pObject->field_2C & dword_78D32C))
            {
                v5 = 2;
                if (flags & 0x20)
                {

                    *(QWORD *)&stru_993E40 = *(QWORD *)&pObject->field_68_92b_size;
                    *((QWORD *)&stru_993E40 + 1) = *(QWORD *)&pObject->field_70_pInners;
                    *((QWORD *)&stru_993E40 + 2) = *(QWORD *)&pObject->field_78;
                    *((QWORD *)&stru_993E40 + 3) = *(QWORD *)&pObject->field_80;
                    sub_4062CB(pObject->field_24 + 8);
                    v5 = sub_40640F();
                }
            }
            pObject->field_32_hasInners = v5;
            sub_4064B1(pObject, activeBuffer, static_cast<char>(flags), v5);
            --objectQueueCount;
        } while (objectQueueCount);
    }
    sub_4065AA(pGv, activeBuffer);
    */
}
MGS_FUNC_IMPLEX(0x4061E7, LibGV_4061E7, false); // TODO: Fully implement

MGS_FUNC_NOT_IMPL(0x405668, void CC(struct_gv* pGv, int activeBuffer), LibGV_405668);
MGS_FUNC_NOT_IMPL(0x405180, void CC(struct_gv* pGv, int activeBuffer), LibGV_405180);
MGS_FUNC_NOT_IMPL(0x4041A5, void CC(struct_gv* pGv, int activeBuffer), LibGV_4041A5);
MGS_FUNC_NOT_IMPL(0x403528, void CC(struct_gv* pGv, int activeBuffer), LibGV_403528);
//MGS_FUNC_NOT_IMPL(0x40340A, void CC(struct_gv* pGv, int activeBuffer), LibGV_40340A);

MGS_ARY(1, 0x991E40, int, 8, dword_991E40, {});


void CC OrderingTableAdd_4034C6(int pPrimDataStart, int count, int size)
{
    DWORD* dword_991E40_1_ot_ptr = (DWORD*)dword_991E40[1];
    const int dword_991E40_2_field_2E_w_or_h = dword_991E40[2];
    if (count - 1 >= 0)
    {
        int pData = pPrimDataStart;
        do
        {
            const int tag = *(WORD *)pData;
            if (tag > 0)
            {
                signed int v6 = tag - dword_991E40_2_field_2E_w_or_h;
                if (v6 < 0)
                {
                    v6 = 0;
                }

                const signed int maybe_z = v6 >> 8;

                // Start of prim points to next OT entry?
                *(DWORD *)pData &= 0xFF000000;
                *(DWORD *)pData |= dword_991E40_1_ot_ptr[maybe_z] & 0x00FFFFFF;

                // OT points to prim
                dword_991E40_1_ot_ptr[maybe_z] &= 0xFF000000;
                dword_991E40_1_ot_ptr[maybe_z] |= pData & 0x00FFFFFF;
            }
            pData += size;
            --count;
        } while (count);
    }
}
MGS_FUNC_IMPLEX(0x4034C6, OrderingTableAdd_4034C6, true); // TODO: Implement me

void __cdecl LibGV_40340A(struct_gv* pGv, int activeBuffer)
{
    int unkByte012Ptr; // edi@3

    dword_991E40[0] = (int)gUnkSize_1024_6BE4E8;  // 256 DWORD's
    int otPtr = *(&pGv->mOrderingTable1 + activeBuffer);
    dword_991E40[1] = otPtr + 4;
    DWORD** otrPtrNext = (DWORD**)dword_991E40[1];
    for (int i = 0; i < 256; i++)
    {
        DWORD* unkItem = gUnkSize_1024_6BE4E8[i];
        if (unkItem) // Only has ptrs for "3d objects" ?
        {
            do
            {
                DWORD unkByte3Idx = *unkItem >> 24; // Get single 3rd byte
                DWORD** otItemPtrPtr = &otrPtrNext[unkByte3Idx]; // Index into the OT using this byte
                unkByte012Ptr = *unkItem & 0xFFFFFF; // Other 3 bytes
                *unkItem = (unsigned int)*otItemPtrPtr | 0xC000000; // Set UNK to point to the OT
                *otItemPtrPtr = unkItem; // Set OT to point to the UNK
                unkItem = (DWORD *)unkByte012Ptr; // To next unk entry ?
            } while (unkByte012Ptr);
        }
    }

    unsigned __int16 v9 = dword_78D32C;
    
    const int primCount = pGv->g_PrimQueue1_word_6BC3BE_256 - pGv->gPrimQueue2_word_6BC3C0_256;
    for (int i = 0; i < primCount; i++)
    {
        Prim_unknown* p = pGv->gObjects_dword_6BC3C4[pGv->gPrimQueue2_word_6BC3C0_256 + i]; // 006bbd58
        if (!(BYTE1(p->field_24_maybe_flags) & 1) && (!p->field_28_dword_9942A0 || p->field_28_dword_9942A0 & v9))
        {
            dword_991E40[2] = p->field_2E_w_or_h;
            OrderingTableAdd_4034C6(
                (int)*(&p->field_40_pDataStart + activeBuffer),
                p->field_2A_num_items,
                p->field_30_size);
        }
    }
}
MGS_FUNC_IMPLEX(0x40340A, LibGV_40340A, true); // TODO: Implement me


MGS_ARY(1, 0x6500E0, TDG_FnPtr, 8, gLibDg_FuncPtrs_off_6500E0, 
{
    LibGV_4061E7,
    LibGV_405668.Ptr(),
    LibGV_405180.Ptr(),
    LibGV_4041A5.Ptr(),
    LibGV_403528.Ptr(),
    LibGV_40340A,
    nullptr
});

// Returns the old pointer because the calling code will use it to restore it later
TDG_FnPtr CC LibDG_SetFnPtr_4019FA(int idx, TDG_FnPtr fnPtr)
{
    if (idx == 7)
    {
        MGS_FATAL("LibDG_SetFnPtr_4019FA - should never be reached");
    }
    TDG_FnPtr old = gLibDg_FuncPtrs_off_6500E0[idx];
    gLibDg_FuncPtrs_off_6500E0[idx] = fnPtr;
    return old;
}
MGS_FUNC_IMPLEX(0x4019FA, LibDG_SetFnPtr_4019FA, LIBDG_IMPL);

void CC LibDG_Update1_4012ED(Actor* pActor)
{
    // This function is a jmp to a stub that does this
    LibDG_ExecFnPtrs_40171C(gActiveBuffer_dword_791A08);
}
MGS_FUNC_IMPLEX(0x4012ED, LibDG_Update1_4012ED, LIBDG_IMPL);

void CC LibDg_Init_40111A()
{
    //nullsub_8();
    //nullsub_7(DeadCode_4011F8);
    sub_401A4F(0, 0, 320, 240, 320);
    sub_4012F2(320);
    LibDG_Reset_HashCounts_4026E6();
    LibDG_4010A6();
    LibGV_Set_FileExtHandler_40A68D('p', MissionLog_Related1_402B25.Ptr()); // .pcc/.pcx
    LibGV_Set_FileExtHandler_40A68D('k', sub_402796.Ptr());                 // .kmd
    LibGV_Set_FileExtHandler_40A68D('l', Returns1_402B1D);                  // ??
    LibGV_Set_FileExtHandler_40A68D('n', sub_402A03.Ptr());                 // .n
    LibGV_Set_FileExtHandler_40A68D('o', sub_402A29.Ptr());                 // .oar
    LibGV_Set_FileExtHandler_40A68D('z', sub_403290.Ptr());                 // .zmd
    LibGV_Set_FileExtHandler_40A68D('i', sub_402A5F.Ptr());                 // .img
    LibGV_Set_FileExtHandler_40A68D('s', sub_402AA9.Ptr());                 // .sgt
    Actor_PushBack(0, &gLibDG_2_stru_6BB930.mBase, 0);// Handles 2D rendering?
    Actor_Init(&gLibDG_2_stru_6BB930.mBase, LibDG_Update2_401234, nullptr, "C:\\mgs\\source\\LibDG\\dgd.c");
    Actor_PushBack(8, &gLibDGD_1_stru_6BB910, 0); // Handles 3D rendering?
    Actor_Init(&gLibDGD_1_stru_6BB910, LibDG_Update1_4012ED, nullptr, "C:\\mgs\\source\\LibDG\\dgd.c");
}
MGS_FUNC_IMPLEX(0x40111A, LibDg_Init_40111A, LIBDG_IMPL);

signed int CC PrimAdd_401805(Prim_unknown* pPrimBuffer)
{
    struct_gv* pGv = &gLibGVStruct1_6BC36C;
    assert(pPrimBuffer->field_2C_r_index == 0);
    pGv = pGv + pPrimBuffer->field_2C_r_index; // Always 0?
    if (pGv->gPrimQueue2_word_6BC3C0_256 > pGv->gObjectQueue_word_6BC3C2_0)
    {
        const s16 newCount = pGv->gPrimQueue2_word_6BC3C0_256 - 1;
        pGv->gObjects_dword_6BC3C4[newCount] = pPrimBuffer; // PrimObject = Prim_unknown + extra ??
        pGv->gPrimQueue2_word_6BC3C0_256 = newCount;
        return 0;
    }
    return -1;
}
MGS_FUNC_IMPLEX(0x401805, PrimAdd_401805, LIBDG_IMPL);

void CC LibDG_ExecFnPtrs_40171C(int activeBuffer)
{
    if (!gLibDG_ExecPtrs_6BECE8)
    {
        MemClearUnknown_40B231(gUnkSize_1024_6BE4E8, 1024);

        int count = 7;
        // Check if extra function pointer slot is in use?
        // TODO: Seems like this is dead code as the condition can never be true?
        if (gLibDG_2_stru_6BB930.dword_6BB950_do_not_flip_buffers != 0)
        {
            count++;
            MGS_FATAL("LibDG_ExecFnPtrs_40171C - should never be reached");
        }

        for (int i = 0; i < count; i++)
        {
            gLibDg_FuncPtrs_off_6500E0[i](&gLibGVStruct1_6BC36C, activeBuffer);
        }
    }
}
MGS_FUNC_IMPLEX(0x40171C, LibDG_ExecFnPtrs_40171C, LIBDG_IMPL);
