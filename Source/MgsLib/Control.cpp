#include "stdafx.h"
#include "Control.hpp"
#include "LibDG.hpp"
#include "Map.hpp"

#define RES_CONTROL_IMPL true

MGS_VAR(1, 0x7227E8, Res_Control, gDefaultResControl_stru_7227E8, {});
MGS_ARY(1, 0x994120, Res_Control*, 96, gResControlArray_994120, {});
MGS_VAR(1, 0x99410C, int, gResControlCount_99410C, 0);

void ForceLinkControlCpp() { }

void CC Res_Control_Set_Default_44EB89()
{
    gResControlArray_994120[0] = &gDefaultResControl_stru_7227E8;
    gResControlCount_99410C = 1;
}
MGS_FUNC_IMPLEX(0x44EB89, Res_Control_Set_Default_44EB89, RES_CONTROL_IMPL);

void CC Res_init_control_loader_helper_444929(Res_Control_unknown* pCtrlUnknown, WORD scriptData)
{
    pCtrlUnknown->field_0_scriptData_orHashedName = scriptData;
    pCtrlUnknown->field_6_count = 0;
    pCtrlUnknown->field_4_trigger_Hash_Name_or_camera_w = 0;
    pCtrlUnknown->field_2_name_hash = 0;
    pCtrlUnknown->field_8_wordPtr = 0;
    pCtrlUnknown->field_C = 0;
    pCtrlUnknown->field_10 = 0;
    pCtrlUnknown->field_14_vec.field_4_z = 0;
    pCtrlUnknown->field_14_vec.field_2_y = 0;
    pCtrlUnknown->field_14_vec.field_0_x = 0;
}
MGS_FUNC_IMPLEX(0x444929, Res_init_control_loader_helper_444929, RES_CONTROL_IMPL);

signed int CC Res_Control_push_back_44EC38(Res_Control* pControl)
{
    // A special meaning that replaces the default/first control
    if (pControl->field_30_scriptData == 0x21CA)
    {
        gResControlArray_994120[0] = pControl;
    }
    else
    {
        // Fail if no more entries left
        if (gResControlCount_99410C >= 96)
        {
            return -1;
        }

        gResControlArray_994120[gResControlCount_99410C] = pControl;
        gResControlCount_99410C++;
    }
    return 0;
}
MGS_FUNC_IMPLEX(0x44EC38, Res_Control_push_back_44EC38, RES_CONTROL_IMPL);

signed int CC Res_Control_init_loader_44EB9E(Res_Control* pControl, DWORD scriptData, int scriptBinds)
{
    const DWORD mapId = scriptBinds ? scriptBinds : mapChangeFlagsOrScriptBinds_9942A0;
    mapChangeFlagsOrScriptBinds_9942A0 = mapId;

    MemClearUnknown_40B231(pControl, sizeof(Res_Control));

    map_record* pMapRec = Map_FromId_44F79E(mapId);
    pControl->field_2C_map = pMapRec;
    if (!pMapRec)
    {
        printf("InitControl : no map %X\n", mapId);
        return -1;
    }

    pControl->field_30_scriptData = static_cast<WORD>(scriptData);
    if (scriptData)
    {
        Res_init_control_loader_helper_444929(&pControl->field_10_pStruct_hzd_unknown, static_cast<WORD>(scriptData));
        if (Res_Control_push_back_44EC38(pControl) < 0)
        {
            return -1;
        }
    }

    pControl->field_32_height = 850;
    pControl->field_34 = -32767;
    pControl->field_38 = 450;
    pControl->field_36 = 450;
    pControl->field_59 = 2;
    pControl->field_55_flags = 2;
    pControl->field_78 = -32000;
    pControl->field_7A = 32000;

    return 0;
}
MGS_FUNC_IMPLEX(0x44EB9E, Res_Control_init_loader_44EB9E, RES_CONTROL_IMPL);

void CC Res_Control_remove_44F0AB(Res_Control* pControl)
{
    Res_Control** pControlIter = gResControlArray_994120;
    int count = gResControlCount_99410C;
    if (gResControlCount_99410C > 0)
    {
        // Find the position in the array
        while (*pControlIter != pControl)
        {
            ++pControlIter;
            if (--count <= 0)
            {
                return;
            }
        }

        // If the first item has been removed then reset it back to the default item
        if (pControlIter == gResControlArray_994120)
        {
            gResControlArray_994120[0] = &gDefaultResControl_stru_7227E8;
        }
        else
        {
            // Otherwise set the entry to remove to be the last item and
            // decrement the total count
            *pControlIter = gResControlArray_994120[gResControlCount_99410C-1];
            gResControlCount_99410C--;
        }
    }
}
MGS_FUNC_IMPLEX(0x44F0AB, Res_Control_remove_44F0AB, RES_CONTROL_IMPL);

void CC Res_Control_remove_if_scriptData_44F098(Res_Control* pControl)
{
    if (pControl->field_30_scriptData)
    {
        Res_Control_remove_44F0AB(pControl);
    }
}
MGS_FUNC_IMPLEX(0x44F098, Res_Control_remove_if_scriptData_44F098, RES_CONTROL_IMPL);

static void TestAddRemove()
{
    Res_Control test[10] = {};
    for (int i = 0; i < 10; i++)
    {
        test[i].field_32_height = static_cast<short>(i);
        ASSERT_EQ(0, Res_Control_push_back_44EC38(&test[i]));
    }

    for (int i = 0; i < 10; i++)
    {
        ASSERT_EQ(gResControlArray_994120[i], &test[i]);
    }

    ASSERT_EQ(gResControlCount_99410C, 10);
    Res_Control_remove_44F0AB(&test[4]);
    ASSERT_EQ(gResControlCount_99410C, 9);

    for (int i = 0; i < 4; i++)
    {
        ASSERT_EQ(gResControlArray_994120[i], &test[i]);
    }
    
    // Removed item has been replaced with the item at the end
    ASSERT_EQ(gResControlArray_994120[4], &test[9]);

    for (int i = 0; i < 4; i++)
    {
        ASSERT_EQ(gResControlArray_994120[i+5], &test[i+5]);
    }
}

void DoControlTests()
{
    TestAddRemove();
}
