#pragma once

#include "MgsFunction.hpp"
#include "types.hpp"

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
    DWORD field_4_width;
    DWORD field_8_height;
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

class AudioDecompressor
{
public:

    s32 mUsedBits = 0;
    u32 mWorkBits = 0;
    s32 mAudioNumChannels = 0;
    u16* mAudioFrameDataPtr = nullptr;

    static u8 gSndTbl_byte_62EEB0[256];

    AudioDecompressor();
    static s32 GetSoundTableValue(s16 tblIndex);
    s16 sub_408F50(s16 a1);
    s32 ReadNextAudioWord(s32 value);
    s32 SndRelated_sub_409650();
    s16 NextSoundBits(u16 numBits);
    bool SampleMatches(s16& sample, s16 bits);
    template<class T>
    void decode_generic(T* outPtr, s32 numSamplesPerFrame, bool isLast);
    void decode_8bit_audio_frame(u8* outPtr, s32 numSamplesPerFrame, bool isLast);
    void decode_16bit_audio_frame(u16* outPtr, s32 numSamplesPerFrame, bool isLast);
    u16* SetupAudioDecodePtrs(u16 *rawFrameBuffer);
    void SetChannelCount(s32 channelCount);
    static void init_Snd_tbl();
};

// This is an actual C++ class.. perhaps the only class in the entire game
struct Actor_Movie_Masher
{
    FILE* field_0_file_handle;
    Actor_Movie_DDV_Header field_4_ddv_header;
    Actor_Movie_DDV_VideoHeader field_14_video_header;
    Actor_Movie_DDV_AudioHeader field_2C_audio_header;
    void* field_40_video_frame_to_decode;
    unsigned short *field_44_decoded_frame_data_buffer;
    int* field_48_sound_frame_to_decode;
    void *field_4C_decoded_audio_buffer;
    int field_50_num_channels;
    int field_54_bits_per_sample;
    int field_58_macro_blocks_x;
    int field_5C_macro_blocks_y;
    bool field_60_bHasAudio;
    bool field_61_bHasVideo;
    int field_64_audio_frame_idx;
    int field_68_frame_number;
    int field_6C_frame_num;
    int* field_70_frame_sizes_array;
    int* field_74_pCurrentFrameSize;
    int field_78;
    int field_7C;
    int* field_80_raw_frame_data;
    int field_84_max_frame_size;
    int field_88_audio_data_offset;
    void* field_8C_macro_block_buffer;
    int field_90_64_or_0;
};
MGS_ASSERT_SIZEOF(Actor_Movie_Masher, 0x94);

MGS_VAR_EXTERN(Actor_Movie_Data, gMovieData_724A00);

void CC jMovie_MMX_Decode_528985(Actor_Movie_Masher* pMasher, void* pDecodedFrame);
void CC File_Ptrs_Init_5289B3(int bAsync);
signed int __fastcall Masher_constructor_523FA0(Actor_Movie_Masher* pThis, void*, const char* movieFileName);
void __fastcall Masher_destructor_524214(Actor_Movie_Masher* pThis, void*);
int CC Res_movie_masher_read_frame_data_528973(Actor_Movie_Masher* pMasher);
void CC Res_movie_masher_set_channels_and_bits_per_sample_52B015(int numChannels, int bitsPerSample);
signed int CC Res_movie_masher_read_blocking_52897C(Actor_Movie_Masher* pMasher);
void* CC Res_movie_masher_sound_read_52899C(Actor_Movie_Masher* pMasher);
int __fastcall Res_movie_masher_read_frame_data_52427C(Actor_Movie_Masher* pThis, void*);
