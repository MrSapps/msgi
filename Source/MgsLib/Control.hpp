#pragma once

#include "MgsFunction.hpp"
#include "Psx.hpp"

void ForceLinkControlCpp();
void DoControlTests();

struct Res_Control_unknown
{
    __int16 field_0_scriptData_orHashedName;
    __int16 field_2_name_hash;
    __int16 field_4_trigger_Hash_Name_or_camera_w;
    __int16 field_6_count;
    int field_8_wordPtr;
    int field_C;
    int field_10;
    SVECTOR field_14_vec;
};
MGS_ASSERT_SIZEOF(Res_Control_unknown, 0x1c);

struct map_record;

struct Res_Control
{
    SVECTOR field_0_vec;
    SVECTOR field_8_vec;
    Res_Control_unknown field_10_pStruct_hzd_unknown;
    map_record *field_2C_map;
    __int16 field_30_scriptData;
    __int16 field_32_height;
    __int16 field_34;
    __int16 field_36;
    __int16 field_38;
    __int16 field_3A;
    __int16 field_3C;
    __int16 field_3E;
    __int16 field_40;
    __int16 field_42;
    SVECTOR field_44_vec;
    SVECTOR field_4C_turn_vec;
    char field_54;
    char field_55_flags;
    char field_56;
    char field_57;
    char field_58;
    char field_59;
    __int16 field_5A;
    int field_5C_mesg;
    SVECTOR field_60_vecs_ary[2];
    int field_70;
    int field_74;
    __int16 field_78;
    __int16 field_7A;
};
MGS_ASSERT_SIZEOF(Res_Control, 0x7c);
