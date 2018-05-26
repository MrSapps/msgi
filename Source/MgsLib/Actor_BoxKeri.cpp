#include "stdafx.h"
#include "Actor_BoxKeri.hpp"
#include "ResourceNameHash.hpp"
#include "Control.hpp"
#include "Map.hpp"
#include "Menu.hpp"
#include "LibDG.hpp"

#define BOXKERI_IMPL true

MGS_VAR(1, 0x6BEF28, Res_Control, gControl_6BEF28, {});
MGS_VAR(1, 0x99534C, Res_Control*, gSnakeResControl_dword_99534C, nullptr);
MGS_VAR(1, 0x9942B0, SVECTOR, gSnakePos_stru_9942B0, {});

void CC Res_Enemy_boxkeri_shutdown_5B701F(Actor_boxkeri* pBox);
int CC Res_Enemy_boxkeri_loader_5B702E(Actor_boxkeri* pBox, PSX_MATRIX* pMtx, SVECTOR* pVec);
void CC Res_Enemy_boxkeri_update_5B6EF7(Actor_boxkeri* pBox);
void CC Res_Enemy_boxkeri_loader_mesg_5B711B();

Actor_boxkeri* CC Res_Enemy_boxkeri_create_5B6EA9(PSX_MATRIX* pMtx, SVECTOR* pVec)
{
    Actor_boxkeri* pBox = Actor_ResourceAllocT<Actor_boxkeri>(4);
    if (!pBox)
    {
        return nullptr;
    }

    Actor_Init_40A347(&pBox->mBase,
        reinterpret_cast<TActorFunction>(Res_Enemy_boxkeri_update_5B6EF7),
        reinterpret_cast<TActorFunction>(Res_Enemy_boxkeri_shutdown_5B701F),
        "C:\\mgs\\source\\Enemy\\boxkeri.c");

    if (Res_Enemy_boxkeri_loader_5B702E(pBox, pMtx, pVec) >= 0)
    {
        return pBox;
    }

    Actor_DestroyOnNextUpdate_40A3ED(&pBox->mBase);
    return nullptr;
}
MGS_FUNC_IMPLEX(0x5B6EA9, Res_Enemy_boxkeri_create_5B6EA9, BOXKERI_IMPL);

void CC Res_Enemy_boxkeri_shutdown_5B701F(Actor_boxkeri* pBox)
{
    Kmd_free_4500DD(&pBox->field_20_kmd);
}
MGS_FUNC_IMPLEX(0x5B701F, Res_Enemy_boxkeri_shutdown_5B701F, BOXKERI_IMPL);

int CC Res_Enemy_boxkeri_loader_5B702E(Actor_boxkeri* pBox, PSX_MATRIX* pMtx, SVECTOR* pVec)
{
    mapChangeFlagsOrScriptBinds_9942A0 = map_change_flags_dword_99535C;
    Kmd_Load_44FF7C(&pBox->field_20_kmd, ResourceNameHash("cb_box"), 109);
    Kmd_Set_Light_matrices_450109(&pBox->field_20_kmd, pBox->field_7C_set_on_kmd_light_matrix_ptrs);

    DataAfterStructure<Prim_Mesh_0x5C*>(&pBox->field_20_kmd.field_0_pObj->prim_48)->field_50_numObjTranslated = 500;

    pBox->field_44 = gControl_6BEF28.field_0_vec;
    pBox->field_4C = gControl_6BEF28.field_0_vec;

    pBox->field_78_pos2 = 0;
    pBox->field_7A_pos1 = 0;

    SVECTOR vec = {};
    Vector_subtract_40B4ED(pVec, &gSnakePos_stru_9942B0, &vec);
    const int v4 = Res_base_unknown_40B612(&vec);
    const int v5 = FixedSubtract_40B6BD(gSnakeResControl_dword_99534C->field_8_vec.field_2_y, static_cast<short>(v4));
    if (v5 < 512 || v5 > 3606)
    {
        pBox->field_76_state = 0;
    }
    else if (v5 >= 1536)
    {
        if (v5 >= 2584)
        {
            pBox->field_76_state = 3;
        }
        else
        {
            pBox->field_76_state = 2;
        }
    }
    else
    {
        pBox->field_76_state = 1;
    }
    memcpy(&pBox->field_54_mtx, pMtx, sizeof(pBox->field_54_mtx));
    pBox->field_74_ticks = 0;
    Res_Enemy_boxkeri_loader_mesg_5B711B();
    return 0;
}
MGS_FUNC_IMPLEX(0x5B702E, Res_Enemy_boxkeri_loader_5B702E, BOXKERI_IMPL);

void CC Res_Enemy_boxkeri_loader_mesg_5B711B()
{
    LibGV_Msg msg = {};
    msg.field_0_res_hash = ResourceNameHash("スネーク");
    msg.field_4_action_hash_or_ptr = ResourceNameHash("段ボール");
    msg.field_6_hash = 1;
    msg.field_12_num_valid_fields = 2;
    LibGV_mesg_write_40B3ED(&msg);
}
MGS_FUNC_IMPLEX(0x5B711B, Res_Enemy_boxkeri_loader_mesg_5B711B, BOXKERI_IMPL);

void CC Res_Enemy_boxkeri_update_5B6EF7(Actor_boxkeri* pBox)
{
    signed int ticks; // ecx
    Prim_unknown_0x48 *pKmd; // eax
    __int16 zPos; // ax
    __int16 xPos; // ax

    ticks = pBox->field_74_ticks;
    if (ticks > 40)
    {
        Actor_DestroyOnNextUpdate_40A3ED(&pBox->mBase);
        return;
    }
    if (ticks >= 9)
    {
        if (ticks >= 12)
        {
            if (ticks >= 15)
            {
                if (ticks >= 18)
                {
                    if (ticks >= 19)
                    {
                        pKmd = &pBox->field_20_kmd.field_0_pObj->prim_48;
                        if (ticks % 2)
                        {
                            pKmd->field_28_flags_or_type |= 0x80u;
                        }
                        else
                        {
                            pKmd->field_28_flags_or_type &= 0x7Fu;
                        }
                    }
                    else
                    {
                        pBox->field_78_pos2 += 32;
                    }
                }
                else
                {
                    pBox->field_78_pos2 -= 32;
                }
            }
            else
            {
                pBox->field_78_pos2 += 64;
            }
        }
        else
        {
            pBox->field_78_pos2 -= 128;
        }
    }
    else
    {
        pBox->field_7A_pos1 += 80;
        pBox->field_78_pos2 += 256;
    }

    switch (pBox->field_76_state)
    {
    case 0:
        if (ticks < 9)
        {
            pBox->field_7A_pos1 += 5;
        }
        pBox->field_4C.field_4_z = -pBox->field_7A_pos1;
        xPos = -pBox->field_78_pos2;
        goto LABEL_29;
    case 1:
        pBox->field_4C.field_0_x = -pBox->field_7A_pos1;
        zPos = pBox->field_78_pos2;
        goto LABEL_21;
    case 2:
        if (ticks < 9)
        {
            pBox->field_7A_pos1 += 100;
        }
        pBox->field_4C.field_4_z = pBox->field_7A_pos1;
        xPos = pBox->field_78_pos2;
    LABEL_29:
        pBox->field_44.field_0_x = xPos;
        break;
    case 3:
        pBox->field_4C.field_0_x = pBox->field_7A_pos1;
        zPos = -pBox->field_78_pos2;
    LABEL_21:
        pBox->field_44.field_4_z = zPos;
        break;
    }

    mapChangeFlagsOrScriptBinds_9942A0 = map_change_flags_dword_99535C;
    PsxSetRotationAndTranslation_407A8F(&pBox->field_54_mtx);
    Res_base_unknown_407B3D(&pBox->field_4C);
    Res_base_unknown_407B79(&pBox->field_44);
    PsxGetRotationAndTranslation_407BC1(&pBox->field_20_kmd.field_0_pObj->prim_48.field_0_matrix);
    Res_base_unknown_40241F(&gSnakePos_stru_9942B0, pBox->field_7C_set_on_kmd_light_matrix_ptrs);
    ++pBox->field_74_ticks;
}
MGS_FUNC_IMPLEX(0x5B6EF7, Res_Enemy_boxkeri_update_5B6EF7, BOXKERI_IMPL);
