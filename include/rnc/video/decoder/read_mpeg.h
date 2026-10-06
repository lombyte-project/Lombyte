#ifndef LOMBYTE_RNC_VIDEO_DECODER_READ_MPEG_H
#define LOMBYTE_RNC_VIDEO_DECODER_READ_MPEG_H

#include "types.h"

struct PadState {
    u8 pad_0[0x1A0];
    s32 unk1A0;
    s32 unk1A4;
};

struct Globals_0013E550 {
    u8 pad_0[0x5C];
    s32 unk5C;
};

struct ReadBuf {
    u8 pad_0[0x50008];
    s32 unk50008;
};

#endif /* LOMBYTE_RNC_VIDEO_DECODER_READ_MPEG_H */
