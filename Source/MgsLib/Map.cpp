#include "stdafx.h"
#include "Map.hpp"
#include "MgsFunction.hpp"
#include "System.hpp"
#include "Script.hpp"
#include "WinMain.hpp"
#include "LibDG.hpp"
#include "ResourceNameHash.hpp"
#include "Menu.hpp"
#include "Actor_GameD.hpp"

#define MAP_IMPL true

void CC Map_LoadMapData_44F66F(int mapNum, int resourceNameHashed);

struct LitHeader
{
    DWORD field_0_num_lights;
};
MGS_ASSERT_SIZEOF(LitHeader, 0x4);

struct map_record
{
    DWORD field_0_map_index_bit;
    WORD field_4_map_num;
    WORD field_6_bUsed;
    struct HzdMap* field_8_hzd;
    LitHeader* field_C_l_file;
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

void CC Map_LitLoad_44F53B(int resourceNameHashed, const map_record* pMapStruct)
{
    KmdHeader* pFileData = (KmdHeader *)LibGV_FindFile_40A603(HashFileName_40A58B(resourceNameHashed, 'k'));
    Prim_Union* pPrim = Obj_Alloc_443FEC(pFileData, 87, 0);
    PsxSetRotationAndTranslation_407A8F(&gIdentity_matrix_6501F8);
    PsxGetRotationAndTranslation_407BC1(&pPrim->prim_48.field_0_matrix);
    LitHeader* p_l_file = pMapStruct->field_C_l_file;
    if (p_l_file)
    {
        PrimObjRelated_443A4E(&pPrim->prim_48, DataAfterStructure<Light*>(p_l_file), p_l_file->field_0_num_lights);
    }
    else
    {
        PrimObjRelated_443A4E(&pPrim->prim_48, nullptr, 0);
    }
    Object_Add_40178F(pPrim);
    pPrim->prim_48.field_2C_index = static_cast<WORD>(pMapStruct->field_0_map_index_bit);
    gMapOrKmd_Array_dword_7229B8[gMapOrKmdCount_dword_722868] = &pPrim->prim_48;
    gMapOrKmdCount_dword_722868++;
}
MGS_FUNC_IMPLEX(0x44F53B, Map_LitLoad_44F53B, MAP_IMPL);

map_record* CC Map_FromId_44F79E(signed int mapChangeFlags)
{
    map_record* result = nullptr;
    signed int bitNMask = mapChangeFlags;
    for (DWORD i = 0; i < gMap_count_dword_7229B4; i++)
    {
        if (!bitNMask)
        {
            break;
        }

        if (bitNMask & 1)
        {
            result = &gMap_20_array_722870[i];
        }

        bitNMask >>= 1;
    }
    return result;
}
MGS_FUNC_IMPLEX(0x44F79E, Map_FromId_44F79E, MAP_IMPL);

void CC Map_Reshade_44F888(Prim_unknown_0x48* pObj)
{
    map_record* pMap = Map_FromId_44F79E(pObj->field_2C_index);
    if (!pMap)
    {
        printf("Reshade NULL map\n");
    }

    LitHeader* p_l_file = pMap->field_C_l_file;
    if (p_l_file)
    {
        PrimObjRelated_443A4E(pObj, DataAfterStructure<Light*>(p_l_file), p_l_file->field_0_num_lights);
    }
}
MGS_FUNC_IMPLEX(0x44F888, Map_Reshade_44F888, MAP_IMPL);

void CC Map_Reshade_all_44F8C3()
{
    for (DWORD i = 0; i < gMapOrKmdCount_dword_722868; i++)
    {
        Map_Reshade_44F888(gMapOrKmd_Array_dword_7229B8[i]);
    }
}
MGS_FUNC_IMPLEX(0x44F8C3, Map_Reshade_all_44F8C3, MAP_IMPL);

struct hzm_vec4
{
    short field_0_x;
    short field_2_y;
    short field_4_z;
    short field_6_w;
};
MGS_ASSERT_SIZEOF(hzm_vec4, 0x8);

struct hzm_vec3
{
    short field_0_x;
    short field_2_y;
    short field_4_z;
};
MGS_ASSERT_SIZEOF(hzm_vec3, 0x6);

struct hzm_camera
{
    hzm_vec4 field_0_trigger_pos;
    hzm_vec4 field_8_camera_pos;
    hzm_vec4 field_10_orientation_pos;
};
MGS_ASSERT_SIZEOF(hzm_camera, 0x18);

struct hzm_trigger
{
    hzm_vec4 field_0_trigger_pos;
    char field_8_tag_name[12];
    char field_14_id1;
    char field_15_id2;
    short field_16_name_hashed; // Hash of field_8_tag_name generated at run time
};
MGS_ASSERT_SIZEOF(hzm_trigger, 0x18);

union hzm_trigger_or_camera
{
    hzm_trigger trigger;
    hzm_camera camera;
};
MGS_ASSERT_SIZEOF(hzm_trigger_or_camera, 0x18);

struct hzm_positioned_camera_or_trigger
{
    hzm_vec4 field_0_area_pos;
    hzm_trigger_or_camera any;
};
MGS_ASSERT_SIZEOF(hzm_positioned_camera_or_trigger, 0x20);

struct hzm_path_record
{
    hzm_vec3 field_0_vector;
    short animationCode;
};
MGS_ASSERT_SIZEOF(hzm_path_record, 8);

struct hzm_pathfinding_record
{
    int field_0; // Order ?
    hzm_path_record* field_4_pRecord;
};
MGS_ASSERT_SIZEOF(hzm_pathfinding_record, 0x8);

struct hzm_wall
{
    hzm_vec3 init_coord;
    short unknown1;
    hzm_vec3 end_coord;
    short unknown2;
};
MGS_ASSERT_SIZEOF(hzm_wall, 16);

struct hzm_altimetry
{
    hzm_vec3 init_coord;
    short unknown1;
    hzm_vec3 end_coord;
    short unknown2;
    short unknown3[16];
};
MGS_ASSERT_SIZEOF(hzm_altimetry, 48);

struct hzm_table_record
{
    short field_0_n_cameras_and_triggers;
    short field_2_n_walls;
    short field_4_n_heights;
    short field_6_n_unknown;
    hzm_wall* field_8_wall_offset;
    hzm_altimetry* field_C_height_offset;
    hzm_positioned_camera_or_trigger* field_10_cameras_and_triggers;
    DWORD* field_14_wall_config_offset; // bit flags array
};
MGS_ASSERT_SIZEOF(hzm_table_record, 0x18);

struct hzm_nav_mesh
{
    hzm_vec3 field_0_vec;
    short width;
    short height;
    short unknown[7]; // TODO: Type recovery
};
MGS_ASSERT_SIZEOF(hzm_table_record, 24);

struct hzm_header_data
{
    short field_0_num_navmeshes;
    short field_2_num_pathfindings;
    hzm_table_record* field_4_offset_tables;
    hzm_nav_mesh* field_8_offset_navmeshes;
    hzm_pathfinding_record* field_C_offset_pathfindings;
};
MGS_ASSERT_SIZEOF(hzm_header_data, 0x10);

union hzm_version_and_nav_mesh_ptr
{
    BYTE* pNavMesh; // Some sort of ordering table into the nav meshes?
    short version;
};
MGS_ASSERT_SIZEOF(hzm_version_and_nav_mesh_ptr, 4);

struct hzm_header
{
    hzm_version_and_nav_mesh_ptr field_0_version_and_ptr_to_nav_meshes;
    int field_4;
    short field_8;
    short field_A_table_count;
    hzm_header_data field_C_pSub;
};
MGS_ASSERT_SIZEOF(hzm_header, 0x1C);

void CC HZM_Process_TableFlagIfTriggers_40B7A3(hzm_positioned_camera_or_trigger* pFlags, int flagsCount)
{
    hzm_positioned_camera_or_trigger* pUnion = pFlags;
    for (int i = 0; i < flagsCount; i++)
    {
        // If its a camera then skip
        if (pUnion[i].any.trigger.field_15_id2 == -1)
        {
            break;
        }
        else
        {
            // Otherwise find the final space in the string
            for (char& hzmChar : pUnion[i].any.trigger.field_8_tag_name)
            {
                if (hzmChar == ' ')
                {
                    // Null terminate it so we can use it as a C-string
                    hzmChar = 0;
                    break;
                }
            }

            // And calculate the hashed name
            pUnion[i].any.trigger.field_16_name_hashed = ResourceNameHash(pUnion[i].any.trigger.field_8_tag_name);
        }
    }
}
MGS_FUNC_IMPLEX(0x40B7A3, HZM_Process_TableFlagIfTriggers_40B7A3, MAP_IMPL);

int CC Gv_hzm_file_handler_40B734(void* pFileData, TFileNameHash)
{
    hzm_header* pHzm = reinterpret_cast<hzm_header*>(pFileData);

    if (pHzm->field_0_version_and_ptr_to_nav_meshes.version < 2)
    {
        printf("Warning:old version hzm\n");
    }

    pHzm->field_0_version_and_ptr_to_nav_meshes.pNavMesh = nullptr; // For some reason first 4 bytes get re-used as a pointer.. 
    OffsetToPointer(pHzm, &pHzm->field_C_pSub.field_4_offset_tables);
    OffsetToPointer(pHzm, &pHzm->field_C_pSub.field_8_offset_navmeshes);
    OffsetToPointer(pHzm, &pHzm->field_C_pSub.field_C_offset_pathfindings);

    hzm_pathfinding_record* pPathFindingRecord = pHzm->field_C_pSub.field_C_offset_pathfindings;
    for (int i = 0; i < pHzm->field_C_pSub.field_2_num_pathfindings; i++)
    {
        OffsetToPointer(pHzm, &pPathFindingRecord[i].field_4_pRecord);
    }

    hzm_table_record* pTableRecord = pHzm->field_C_pSub.field_4_offset_tables;
    for (int i = 0; i < pHzm->field_A_table_count; i++)
    {
        OffsetToPointer(pHzm, &pTableRecord[i].field_10_cameras_and_triggers);
        OffsetToPointer(pHzm, &pTableRecord[i].field_8_wall_offset);
        OffsetToPointer(pHzm, &pTableRecord[i].field_C_height_offset);
        OffsetToPointer(pHzm, &pTableRecord[i].field_14_wall_config_offset);
        HZM_Process_TableFlagIfTriggers_40B7A3(pTableRecord[i].field_10_cameras_and_triggers, pTableRecord[i].field_0_n_cameras_and_triggers);
    }
    return 1;
}
MGS_FUNC_IMPLEX(0x40B734, Gv_hzm_file_handler_40B734, MAP_IMPL);


struct HzdMap
{
    hzm_header* field_0_pHzmHeader;
    hzm_table_record* field_4_pTableRec;
    short field_8_bitIdx;
    short field_A;
    short field_C;
    short field_E_trigger_count; // Num items in field_18_pTriggers
    short field_10_24Size; // field_1C_pAfterStructure_24 size
    short field_12_48size; // field_20_pAfterStructure_48 size
    BYTE* field_14_pnav_meshes; // Ordering/index of meshes?
    hzm_positioned_camera_or_trigger* field_18_pTriggers;
    void* field_1C_pAfterStructure_24; // TODO: Types
    void* field_20_pAfterStructure_48; // TODO: Types
    void* field_24_pAfterStructure48_End; // TODO: Types
};
MGS_ASSERT_SIZEOF(HzdMap, 0x28);

void CC Map_HZD_NavMesh_40B8A5(hzm_header* pHzm, BYTE* pOut)
{
    // TODO: Implement
    MGS_FORCE_ENOUGH_SPACE_FOR_A_DETOUR;
}
MGS_FUNC_IMPLEX(0x40B8A5, Map_HZD_NavMesh_40B8A5, false); // TODO

HzdMap* CC Map_HZD_Load_Helper_40B7E0(hzm_header* pHzdData, int default_0_flags_index, int default_48, int default_24)
{
    if (pHzdData->field_0_version_and_ptr_to_nav_meshes.pNavMesh == nullptr)
    {
        if (pHzdData->field_C_pSub.field_0_num_navmeshes > 1)
        {
            const signed int numNavMeshes = pHzdData->field_C_pSub.field_0_num_navmeshes;
            BYTE* pNavMeshes = reinterpret_cast<BYTE*>(System_2_zerod_allocate_memory_40B296((numNavMeshes - 1) * (numNavMeshes - 2) / 2 + numNavMeshes - 1));
            Map_HZD_NavMesh_40B8A5(pHzdData, pNavMeshes);
            pHzdData->field_0_version_and_ptr_to_nav_meshes.pNavMesh = pNavMeshes;
        }
    }

    HzdMap* pHzdMap = reinterpret_cast<HzdMap*>(System_2_zerod_allocate_memory_40B296((6 * default_48) + (4 * default_24) + sizeof(HzdMap)));
    if (pHzdMap)
    {
        pHzdMap->field_12_48size = static_cast<short>(default_48);
        pHzdMap->field_10_24Size = static_cast<short>(default_24);

        pHzdMap->field_1C_pAfterStructure_24 = (void*)&pHzdMap[1]; // TODO: Types

        DWORD* p24 = (DWORD*)&pHzdMap[1] + default_24; // TODO: Types
        pHzdMap->field_0_pHzmHeader = pHzdData;
        pHzdMap->field_20_pAfterStructure_48 = (void*)p24; // TODO: Types
        pHzdMap->field_24_pAfterStructure48_End = &p24[default_48]; // TODO: Types

        pHzdMap->field_A = 0;
        pHzdMap->field_C = 0;

        pHzdMap->field_4_pTableRec = &pHzdData->field_C_pSub.field_4_offset_tables[default_0_flags_index];
        pHzdMap->field_14_pnav_meshes = pHzdData->field_0_version_and_ptr_to_nav_meshes.pNavMesh;

        hzm_positioned_camera_or_trigger* pCamerasAndTriggers = pHzdMap->field_4_pTableRec->field_10_cameras_and_triggers;

        // Find the last item from the end that is not a camera, thus yielding the count and pointer to the triggers
        short int triggerCount = pHzdMap->field_4_pTableRec->field_0_n_cameras_and_triggers;
        while (triggerCount > 0 && pCamerasAndTriggers->any.trigger.field_15_id2 != -1)
        {
            pCamerasAndTriggers++;
            triggerCount--;
        }
        pHzdMap->field_E_trigger_count = triggerCount;
        pHzdMap->field_18_pTriggers = pCamerasAndTriggers;
    }
    return pHzdMap;
}
MGS_FUNC_IMPLEX(0x40B7E0, Map_HZD_Load_Helper_40B7E0, MAP_IMPL);


HzdMap* CC Map_HZD_Load_44F5AF(DWORD resource_name_hashed, int flagsIndex, __int16 bitIndex, int default_48, int default_24)
{
    hzm_header* pFileData = reinterpret_cast<hzm_header*>(LibGV_FindFile_40A603(HashFileName_40A58B(resource_name_hashed, 'h')));
    if (!pFileData)
    {
        // Not found use null
        pFileData = reinterpret_cast<hzm_header*>(LibGV_FindFile_40A603(HashFileName_40A58B(ResourceNameHash("null"), 'h')));
        if (!pFileData)
        {
            // Still not found, use abst.hzd if the current stage is abst
            const WORD abst_hashed = ResourceNameHash("abst");
            if (GetStageNameHashed_44EAE5() == abst_hashed)
            {
                pFileData = reinterpret_cast<hzm_header*>(LibGV_FindFile_40A603(HashFileName_40A58B(abst_hashed, 'h')));
            }
        }
    }
    HzdMap* hzdMap = Map_HZD_Load_Helper_40B7E0(pFileData, flagsIndex, default_48, default_24);
    hzdMap->field_8_bitIdx = bitIndex;
    return hzdMap;
}
MGS_FUNC_IMPLEX(0x44F5AF, Map_HZD_Load_44F5AF, MAP_IMPL);

void CC Map_LoadMapData_44F66F(int mapNum, int resourceNameHashed)
{
    printf("set map %d\n", mapNum);
    map_record* pMap = Map_GetNextFreeRecord_44F505(static_cast<short>(mapNum));
    pMap->field_8_hzd = Map_HZD_Load_44F5AF(resourceNameHashed, 0, static_cast<short>(pMap->field_0_map_index_bit), 48, 24);
    pMap->field_C_l_file = reinterpret_cast<LitHeader*>(LibGV_FindFile_40A603(HashFileName_40A58B(resourceNameHashed, 'l')));
    Map_LitLoad_44F53B(resourceNameHashed, pMap);
}
MGS_FUNC_IMPLEX(0x44F66F, Map_LoadMapData_44F66F, MAP_IMPL);

MGS_FUNC_NOT_IMPL(0x444903, void __cdecl (unsigned __int16 flags), Binds_444903);

MGS_VAR(1, 0x99535C, DWORD, map_change_flags_dword_99535C, 0);
MGS_VAR(1, 0x791A10, DWORD, gMapUseTableIndicesBits_791A10, 0);


MGS_FUNC_NOT_IMPL(0x4021B3, void __cdecl (), LibDG_Init_Lights_4021B3);
MGS_FUNC_NOT_IMPL(0x4021C9, void __cdecl (Light *pLights, int lightCount), Light_add_4021C9);

map_record* CC Script_tbl_hzd_impl_44F415()
{
    // Allocate a map record
    map_record* pMapStruct = Map_GetNextFreeRecord_44F505(static_cast<short>(Script_get_int()));
    int hzd_param4 = 0;
    int hzd_param5 = 0;

    if (Script_ParamExists('d'))
    {
        hzd_param4 = Script_get_int();
        hzd_param5 = Script_get_int();
    }
    else
    {
        hzd_param4 = 48;
        hzd_param5 = 24;
    }

    if (!Script_ParamExists('h'))
    {
        printf("no hzd\n");
        return nullptr;
    }

    // Load the HZD map
    const DWORD nameHashed = Script_get_int();
    const DWORD flagsIndex = Script_get_int();
    pMapStruct->field_8_hzd = Map_HZD_Load_44F5AF(
        nameHashed,
        flagsIndex,
        static_cast<short>(pMapStruct->field_0_map_index_bit),
        hzd_param4,
        hzd_param5);

    // Light file
    if (Script_ParamExists('l'))
    {
        pMapStruct->field_C_l_file = reinterpret_cast<LitHeader*>(LibGV_FindFile_40A603(HashFileName_40A58B(Script_get_int(), 'l')));
    }
    else
    {
        pMapStruct->field_C_l_file = nullptr;
    }

    // Lights
    if (Script_ParamExists('k'))
    {
        while (Script_GetReturnAddress())
        {
            Map_LitLoad_44F53B(Script_get_int(), pMapStruct);
        }
    }

    // Zones
    pMapStruct->field_10_script_z_param_bit_index_zone_id = 0;
    if (Script_ParamExists('z'))
    {
        while (Script_GetReturnAddress())
        {
            pMapStruct->field_10_script_z_param_bit_index_zone_id |= 1 << Script_get_int();
        }
    }

    return pMapStruct;
}
MGS_FUNC_IMPLEX(0x44F415, Script_tbl_hzd_impl_44F415, MAP_IMPL);

void CC Map_update_44F2A4(int bLightUpdate)
{
    DWORD usedTableIndicesAsBits = 0;
    DWORD usedMapBits = 0;

    LibDG_Init_Lights_4021B3();

    for (DWORD i = 0; i < gMap_count_dword_7229B4; i++)
    {
        if (gMap_20_array_722870[i].field_6_bUsed)
        {
            usedMapBits |= gMap_20_array_722870[i].field_0_map_index_bit;
            for (DWORD j = 0; j < gMapOrKmdCount_dword_722868; j++)
            {
                if (gMapOrKmd_Array_dword_7229B8[j]->field_2C_index & gMap_20_array_722870[i].field_0_map_index_bit)
                {
                    LitHeader* pLitFile = gMap_20_array_722870[i].field_C_l_file;
                    if (pLitFile->field_0_num_lights)
                    {
                        Light_add_4021C9(DataAfterStructure<Light*>(pLitFile), pLitFile->field_0_num_lights);
                        if (bLightUpdate)
                        {
                            PrimObjRelated_443A4E(gMapOrKmd_Array_dword_7229B8[j], DataAfterStructure<Light*>(pLitFile), pLitFile->field_0_num_lights);
                        }
                    }
                    else
                    {
                        Light_add_4021C9(nullptr, 0);
                        if (bLightUpdate)
                        {
                            PrimObjRelated_443A4E(gMapOrKmd_Array_dword_7229B8[j], 0, 0);
                        }
                    }
                }
            }
            hzm_table_record* pFirstTable = gMap_20_array_722870[i].field_8_hzd->field_0_pHzmHeader->field_C_pSub.field_4_offset_tables;
            hzm_table_record* pCurrentTable = gMap_20_array_722870[i].field_8_hzd->field_4_pTableRec;
            const DWORD tableIndex = pCurrentTable - pFirstTable;
            usedTableIndicesAsBits |= 1 << (tableIndex);
        }
    }
    map_change_flags_dword_99535C = usedMapBits;
    gMapUseTableIndicesBits_791A10 = usedTableIndicesAsBits;
    dword_78D32C = usedMapBits;
    Binds_444903(static_cast<WORD>(usedMapBits));
}
MGS_FUNC_IMPLEX(0x44F2A4, Map_update_44F2A4, true);

struct TestHzm
{
    hzm_header mHeader;
    hzm_table_record mTables[32];
    hzm_positioned_camera_or_trigger mTriggers[8];
};

static void Test_Map_update_44F2A4()
{
    gMapOrKmdCount_dword_722868 = 0;
    gMapUseTableIndicesBits_791A10 = 0;

    TestHzm testHzm = {};
    testHzm.mHeader.field_A_table_count = 2;
    testHzm.mHeader.field_C_pSub.field_4_offset_tables = testHzm.mTables;

    testHzm.mTables[0].field_0_n_cameras_and_triggers = 5;
    testHzm.mTables[0].field_10_cameras_and_triggers = &testHzm.mTriggers[0];

    for (int i = 0; i < 32; i++)
    {
        HzdMap hzd[2] = {};
        hzd[0].field_0_pHzmHeader = &testHzm.mHeader;
        hzd[0].field_18_pTriggers = &testHzm.mTriggers[0];
        hzd[0].field_4_pTableRec = &testHzm.mTables[i];
        hzd[0].field_E_trigger_count = 5;
        gMap_20_array_722870[0].field_6_bUsed = 2;
        gMap_20_array_722870[0].field_8_hzd = &hzd[0];

        gMap_count_dword_7229B4 = 1;

        Map_update_44F2A4(0);

        ASSERT_EQ(gMapUseTableIndicesBits_791A10, 1 << i);
    }
}

void CC Map_update_if_changed_44F28B()
{
    if (gMapsChanged_dword_7229B0)
    {
        Map_update_44F2A4(gMapsChanged_dword_7229B0 - 1);
        gMapsChanged_dword_7229B0 = 0;
    }
}
MGS_FUNC_IMPLEX(0x44F28B, Map_update_if_changed_44F28B, MAP_IMPL);

void DoMapTests()
{
    Test_Map_update_44F2A4();
}
