#pragma once

#include "Actor.hpp"

struct Actor_Movie
{
    Actor mBase;
    WORD word_7248F0_counter;
    WORD word_7248F2_11_param_i;
    short dword_7248F4_param_o;
    WORD field_26_pad;
    int dword_7248F8_script_param_p;
    DWORD dword_7248FC_bIsEnding;
};
MGS_ASSERT_SIZEOF(Actor_Movie, 0x30);

struct Actor_Movie_Masher;
struct Actor_Movie_DDV_Header;
struct Actor_Movie_DDV_VideoHeader;
struct Actor_Movie_DDV_AudioHeader;

struct Actor_Movie_Data
{
    Actor_Movie_Masher* field_0_masher_ptr;
    Actor_Movie_DDV_Header* field_4_ddv_header;
    Actor_Movie_DDV_VideoHeader* field_8_video_header;
    Actor_Movie_DDV_AudioHeader* field_C_audio_header;
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

struct Actor_Movie_DDV_Header
{
    int field_0_ddv_version;
    int field_4_contains;
    int field_8_frame_rate;
    int field_C_number_of_frames;
};
MGS_ASSERT_SIZEOF(Actor_Movie_DDV_Header, 16);

struct Actor_Movie_DDV_VideoHeader
{
    int field_0_unknown;
    int field_4_width;
    int field_8_height;
    int field_C_max_audio_frame_size;
    int field_10_max_video_frame_size;
    int field_14_key_frame_rate;
};
MGS_ASSERT_SIZEOF(Actor_Movie_DDV_VideoHeader, 24);

struct Actor_Movie_DDV_AudioHeader
{
    int field_0_audio_format;
    int field_4_samples_per_second;
    int field_8_max_audio_frame_size;
    int field_C_single_audio_frame_size;
    int field_10_num_frames_interleave;
};
MGS_ASSERT_SIZEOF(Actor_Movie_DDV_AudioHeader, 20);

struct Actor_Movie_Masher
{
    int field_0_file_handle;
    Actor_Movie_DDV_Header field_4_ddv_header;
    Actor_Movie_DDV_VideoHeader field_14_video_header;
    Actor_Movie_DDV_AudioHeader field_2C_audio_header;
    int field_40_frame_data;
    short *field_44_decoded_frame_data_buffer;
    int field_48_pField_80_buffer;
    void *field_4C_audio_buffer;
    int field_50_num_channels;
    int field_54_bits_per_sample;
    int field_58_macro_blocks_x;
    int field_5C_macro_blocks_y;
    char field_60_bHasAudio;
    char field_61_bHasVideo;
    __int16 field_62_pad;
    int field_64_audio_frame_idx;
    int field_68;
    int field_6C_frame_num;
    void* field_70_frame_sizes_array;
    void* field_74_pCurrentFrameSize;
    int field_78;
    int field_7C;
    void* field_80_audio_frame_buffer;
    int field_84_max_audio_frame;
    int field_88;
    void* field_8C_macro_block_buffer;
    int field_90_64_or_0;
};
MGS_ASSERT_SIZEOF(Actor_Movie_Masher, 0x94);

void Force_Actor_Movie_Cpp_Link();
void DoMovie_Tests();

Actor* CC Res_movie_create_4561C6(DWORD scriptData, int scriptBinds, BYTE* pScript);
Actor_Movie* CC Res_movie_create_4561DF(int movieNameHashed, int bIsEnding);
