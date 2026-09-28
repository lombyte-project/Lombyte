#ifndef RNC_VIDEO_DECODER_READ_MPEG_TYPES_H
#define RNC_VIDEO_DECODER_READ_MPEG_TYPES_H

#include "types.h"

struct M2c_D_0013C940 {
    u8 pad_0[0x1A0];
    s32 unk1A0;
    s32 unk1A4;
};

struct M2c_D_0013E550 {
    u8 pad_0[0x5C];
    s32 unk5C;
};

struct M2c_arg1 {
    u8 pad_0[0x50008];
    s32 unk50008;
};

#endif /* RNC_VIDEO_DECODER_READ_MPEG_TYPES_H */
