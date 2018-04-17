#include "stdafx.h"
#include "Map.hpp"
#include "MgsFunction.hpp"
#include "System.hpp"
#include "Script.hpp"
#include "WinMain.hpp"
#include "LibDG.hpp"
#include "ResourceNameHash.hpp"

#define MAP_IMPL true

struct map_record
{
    DWORD field_0_map_index_bit;
    WORD field_4_map_num;
    WORD field_6_bUsed;
    void* field_8_hzd;
    DWORD field_C_l_file;
    DWORD field_10_script_z_param_bit_index_zone_id;
};
MGS_ASSERT_SIZEOF(map_record, 0x14);

MGS_ARY(1, 0x722870, map_record, 16, gMap_20_array_722870, {});
MGS_VAR(1, 0x7229B4, DWORD, gMap_count_dword_7229B4, 0);
MGS_VAR(1, 0x7229B0, DWORD, gMapsChanged_dword_7229B0, 0);


map_record* CC Map_FindByNum_44F7CB(int toFind)
{
    for (DWORD i=0; i<gMap_count_dword_7229B4; i++)
    {
        if (gMap_20_array_722870[i].field_4_map_num == toFind)
        {
            return &gMap_20_array_722870[i];
        }
    }
    return nullptr;
}
MGS_FUNC_IMPLEX(0x44F7CB, Map_FindByNum_44F7CB, MAP_IMPL);

map_record* CC Map_FindByZoneId_44F867(int id)
{
    for (DWORD i = 0; i < gMap_count_dword_7229B4; i++)
    {
        if (gMap_20_array_722870[i].field_10_script_z_param_bit_index_zone_id & id)
        {
            return &gMap_20_array_722870[i];
        }
    }
    return nullptr;
}
MGS_FUNC_IMPLEX(0x44F867, Map_FindByZoneId_44F867, MAP_IMPL);

signed int CC Map_MarkNotUsed_44F70E(int mapId)
{
    for (DWORD i = 0; i < gMap_count_dword_7229B4; i++)
    {
        if (gMap_20_array_722870[i].field_4_map_num == mapId)
        {
            printf("del map %d\n", gMap_20_array_722870[i].field_0_map_index_bit);
            gMap_20_array_722870[i].field_6_bUsed = 0;
            gMapsChanged_dword_7229B0 = 1;
            return 1;
        }
    }
    printf("delmap: not found map %d\n", mapId);
    return 0;
}
MGS_FUNC_IMPLEX(0x44F70E, Map_MarkNotUsed_44F70E, MAP_IMPL);


signed int CC Map_MarkUsed_44F6BF(int mapId)
{
    for (DWORD i = 0; i < gMap_count_dword_7229B4; i++)
    {
        if (gMap_20_array_722870[i].field_4_map_num == mapId)
        {
            printf("add map %d\n", gMap_20_array_722870[i].field_0_map_index_bit);
            gMap_20_array_722870[i].field_6_bUsed = 1;
            gMapsChanged_dword_7229B0 = 1;
            return 1;
        }
    }
    printf("addmap : not found map %d\n", mapId);
    return 0;
}
MGS_FUNC_IMPLEX(0x44F6BF, Map_MarkUsed_44F6BF, MAP_IMPL);

map_record* CC Map_GetNextFreeRecord_44F505(__int16 mapNum)
{
    DWORD indexBitNum = gMap_count_dword_7229B4;
    map_record* pRec = gMap_20_array_722870;
    if (gMap_count_dword_7229B4 > 0)
    {
        pRec = &gMap_20_array_722870[gMap_count_dword_7229B4];
    }
    pRec->field_6_bUsed = 0;
    pRec->field_4_map_num = mapNum;
    pRec->field_0_map_index_bit = 1 << indexBitNum;
    gMap_count_dword_7229B4 = indexBitNum + 1;
    return pRec;
}
MGS_FUNC_IMPLEX(0x44F505, Map_GetNextFreeRecord_44F505, MAP_IMPL);

void CC Map_HZD_safe_free_40B893(void* pAlloc)
{
    if (pAlloc)
    {
        System_2_free_40B2A7(pAlloc);
    }
}
MGS_FUNC_IMPLEX(0x40B893, Map_HZD_safe_free_40B893, MAP_IMPL);

void CC Map_FreeHzdItems_44F38D()
{
    for (DWORD i = 0; i < gMap_count_dword_7229B4; i++)
    {
        if (gMap_20_array_722870[i].field_8_hzd)
        {
            Map_HZD_safe_free_40B893(gMap_20_array_722870[i].field_8_hzd);
        }
    }
}
MGS_FUNC_IMPLEX(0x44F38D, Map_FreeHzdItems_44F38D, MAP_IMPL);

signed int CC Map_IsInUse_44F7ED(DWORD mapNum)
{
    for (DWORD i = 0; i < gMap_count_dword_7229B4; i++)
    {
        if (gMap_20_array_722870[i].field_0_map_index_bit == mapNum && gMap_20_array_722870[i].field_6_bUsed)
        {
            return 1;
        }
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x44F7ED, Map_IsInUse_44F7ED, MAP_IMPL);

int CC Map_ScriptReloadMaps_44F75E(int zeroOrOne)
{
    for (DWORD i = 0; i < gMap_count_dword_7229B4; i++)
    {
        gMap_20_array_722870[i].field_6_bUsed = 0;
    }

    while (Script_GetReturnAddress())
    {
        Map_MarkUsed_44F6BF(Script_get_int());
    }

    gMapsChanged_dword_7229B0 = zeroOrOne + 1;

    return 0;
}
MGS_FUNC_IMPLEX(0x44F75E, Map_ScriptReloadMaps_44F75E, MAP_IMPL);

MGS_FUNC_NOT_IMPL(0x44F5AF, void* CC(int resource_name_hashed, int default_0, __int16 bitIndex, int default_48, int default_24), Map_HZD_Load_44F5AF)
MGS_FUNC_NOT_IMPL(0x44F53B, int CC(int resourceNameHashed, map_record *pMapStruct), Map_LitLoad_44F53B)

void CC Map_LoadMapData_44F66F(int mapNum, int resourceNameHashed)
{
    printf("set map %d\n", mapNum);
    map_record* pMap = Map_GetNextFreeRecord_44F505(static_cast<short>(mapNum));
    pMap->field_8_hzd = Map_HZD_Load_44F5AF(resourceNameHashed, 0, pMap->field_0_map_index_bit, 48, 24);
    DWORD fileNameHashed = HashFileName_40A58B(resourceNameHashed, 'l');
    pMap->field_C_l_file = (DWORD)LibGV_FindFile_40A603(fileNameHashed);
    Map_LitLoad_44F53B(resourceNameHashed, pMap);
}
MGS_FUNC_IMPLEX(0x44F66F, Map_LoadMapData_44F66F, MAP_IMPL);

map_record* CC Map_ScriptLoadMapBlocks_44F640()
{
    while (Script_GetReturnAddress())
    {
        const int mapNum = Script_get_int();
        const int nameHashed = Script_get_int();
        Map_LoadMapData_44F66F(mapNum, nameHashed);
    }
    return gMap_20_array_722870;
}
MGS_FUNC_IMPLEX(0x44F640, Map_ScriptLoadMapBlocks_44F640, MAP_IMPL);

signed int CC Script_tbl_map_45151D(BYTE* /*pScript*/)
{
    if (Script_ParamExists('d'))
    {
        Map_ScriptLoadMapBlocks_44F640();
    }

    if (Script_ParamExists('s'))
    {
        Map_ScriptReloadMaps_44F75E(1);
        if (!(game_state_dword_72279C.flags & 0x80000000))
        {
            counter_dword_6BED20 = 4;
        }
    }

    if (Script_ParamExists('c'))
    {
        Map_ScriptReloadMaps_44F75E(0);
        if (!(game_state_dword_72279C.flags & 0x80000000))
        {
            if (Script_ParamExists('u'))
            {
                counter_dword_6BED20 = 4;
            }
            else
            {
                counter_dword_6BED20 = 0;
            }
        }
    }

    if (Script_ParamExists('a'))
    {
        gBinds_dword_722A40 = 0;
        while (Script_GetReturnAddress())
        {
            map_record* pMapRecord = Map_FindByNum_44F7CB(Script_get_int());
            if (!pMapRecord)
            {
                return -1;
            }
            gBinds_dword_722A40 |= pMapRecord->field_0_map_index_bit;
        }
    }

    if (Script_ParamExists('p'))
    {
        while (Script_GetReturnAddress())
        {
            Map_MarkUsed_44F6BF(Script_get_int());
        }
    }

    if (Script_ParamExists('m'))
    {
        while (Script_GetReturnAddress())
        {
            Map_MarkNotUsed_44F70E(Script_get_int());
        }
    }

    if (Script_ParamExists('b'))
    {
        WORD param_b[3];
        Script_Read3Words_409945(Script_GetReturnAddress(), param_b);
        LibDG_SetRGB_401931(static_cast<BYTE>(param_b[0]), static_cast<BYTE>(param_b[1]), static_cast<BYTE>(param_b[2]));
    }

    return 0;
}
MGS_FUNC_IMPLEX(0x45151D, Script_tbl_map_45151D, MAP_IMPL);

MGS_ARY(1, 0x7229B8, Prim_unknown_0x48*, 32, gMapOrKmd_Array_dword_7229B8, {});
MGS_VAR(1, 0x722868, DWORD, gMapOrKmdCount_dword_722868, 0);

void CC Map_ResetMapCountAndKmdsCount_44F3F7()
{
    gMapOrKmdCount_dword_722868 = 0;
    memset(gMapOrKmd_Array_dword_7229B8, 0, 32 * sizeof(Prim_unknown_0x48*));
    gMap_count_dword_7229B4 = 0;
    gMapsChanged_dword_7229B0 = 0;
}
MGS_FUNC_IMPLEX(0x44F3F7, Map_ResetMapCountAndKmdsCount_44F3F7, MAP_IMPL);

void CC Map_FreeKmds_44F3B5()
{
    for (DWORD i = 0; i < gMapOrKmdCount_dword_722868; i++)
    {
        Object_Remove_4017C3(gMapOrKmd_Array_dword_7229B8[i]);
        Prim_free_colour_buffer_443FCB(gMapOrKmd_Array_dword_7229B8[i]);
        Prim_void_and_free_4440BE(gMapOrKmd_Array_dword_7229B8[i]);
        gMapOrKmd_Array_dword_7229B8[i] = nullptr;
    }
    gMapOrKmdCount_dword_722868 = 0;
}
MGS_FUNC_IMPLEX(0x44F3B5, Map_FreeKmds_44F3B5, MAP_IMPL);

MGS_FUNC_NOT_IMPL(0x44F888, void __cdecl (Prim_unknown_0x48 *pKmd), Map_Reshade_44F888) // TODO

void CC Map_Reshade_all_44F8C3()
{
    for (DWORD i = 0; i < gMapOrKmdCount_dword_722868; i++)
    {
        Map_Reshade_44F888(gMapOrKmd_Array_dword_7229B8[i]);
    }
}
MGS_FUNC_IMPLEX(0x44F8C3, Map_Reshade_all_44F8C3, MAP_IMPL);
