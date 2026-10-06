#include "types.h"

typedef struct MpegPictureDecoder MpegPictureDecoder;
typedef struct MpegPictureState {
    u8 reserved_b0[0xB0];
    s32 decode_state;
    u8 reserved_b4[0x24];
    u32 output_address;
    u32 reserved_dc;
    u32 reserved_e0;
    s32 macroblock_count;
} MpegPictureState;

struct MpegPictureDecoder {
    u8 reserved[0x40];
    MpegPictureState *picture_state;
};

extern s32 DecodeMpegPicture(MpegPictureDecoder *decoder) __asm__("_getpic");

s32 sceMpegGetPicture(MpegPictureDecoder *decoder, void *picture_buffer,
                      s32 macroblock_count) __asm__("sceMpegGetPicture");

s32 sceMpegGetPicture(MpegPictureDecoder *decoder, void *picture_buffer, s32 macroblock_count) {
    MpegPictureState *picture_state = decoder->picture_state;

    picture_state->decode_state = 1;
    picture_state->output_address = ((u32)picture_buffer & 0x0FFFFFFFu) | 0x20000000u;
    picture_state->macroblock_count = macroblock_count;
    picture_state->reserved_e0 = 0;
    picture_state->reserved_dc = 0;
    return DecodeMpegPicture(decoder);
}
