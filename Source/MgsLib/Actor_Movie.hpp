#pragma once

#include "Actor.hpp"

struct Actor_Movie
{
    Actor mBase;
    WORD word_7248F0_counter;
    WORD word_7248F2_11_param_i;
    short dword_7248F4_param_o;
    WORD field_26;
    int dword_7248F8_script_param_p;
    DWORD dword_7248FC_bIsEnding;
};
MGS_ASSERT_SIZEOF(Actor_Movie, 0x30);

struct Actor_Movie_Masher;
struct Actor_Movie_Masher_16Stru;
struct Actor_Movie_Masher_24Stru;
struct Actor_Movie_Data_SoundSettings;

struct Actor_Movie_Data
{
    Actor_Movie_Masher* field_0_masher_ptr;
    Actor_Movie_Masher_16Stru* field_4_ptr;
    Actor_Movie_Masher_24Stru* field_8;
    Actor_Movie_Data_SoundSettings* gAudioBufferSettings_dword_724A0C;
    DWORD gbMovieError_dword_724A10;
    void* gMovieBuffer_724A14;
    int field_18;
    int field_1C_read_ret;
    int field_20_sound_pos;
    int field_24_double_width;
    int field_28_double_height;
    int field_2C_audio_play_started;
    int sMovie_IO_Ptrs_Inited_dword_724A30;
};
MGS_ASSERT_SIZEOF(Actor_Movie_Data, 0x34);

struct Actor_Movie_Masher_16Stru
{
    int field_0;
    int field_4;
    int field_8;
    int field_C;
};
MGS_ASSERT_SIZEOF(Actor_Movie_Masher_16Stru, 16);

struct Actor_Movie_Masher_24Stru
{
    int field_0;
    int field_4_width;
    int field_8_height;
    int field_C;
    int field_10;
    int field_14;
};
MGS_ASSERT_SIZEOF(Actor_Movie_Masher_24Stru, 24);

struct Actor_Movie_Data_SoundSettings
{
    int field_0_flags;
    int field_4_samples_per_second;
    int field_8;
    int field_C_block_size;
    int field_10_num_buffers;
};
MGS_ASSERT_SIZEOF(Actor_Movie_Data_SoundSettings, 20);

struct Actor_Movie_Masher
{
    int field_0_file_handle;
    Actor_Movie_Masher_16Stru field_4_16_stru;
    Actor_Movie_Masher_24Stru field_14_24_struc;
    Actor_Movie_Data_SoundSettings field_2C_audio_settings;
    int field_40_frame_data;
    short *field_44_decoded_frame_data_buffer;
    int field_48_pField_80_buffer;
    void *field_4C_audio_buffer;
    int field_50_num_channels;
    int field_54_bits_per_sample;
    int field_58_macro_blocks_x;
    int field_5C_macro_blocks_y;
    char field_60;
    char field_61;
    __int16 field_62;
    int field_64_audio_frame_idx;
    int field_68;
    int field_6C;
    void* field_70_buffer;
    void* field_74_pfield_70_buffer;
    int field_78;
    int field_7C;
    void* field_80_buffer;
    int field_84;
    int field_88;
    void* field_8C_buffer;
    int field_90;
};
MGS_ASSERT_SIZEOF(Actor_Movie_Masher, 0x94);

void Force_Actor_Movie_Cpp_Link();
void DoMovie_Tests();

Actor* CC Res_movie_create_4561C6(DWORD scriptData, int scriptBinds, BYTE* pScript);
EXTERN_MGS_FUNC_NOT_IMPL(0x4561DF, Actor_Movie *__cdecl (int movieNameHashed, int bIsEnding), Res_movie_create_4561DF); // TODO
