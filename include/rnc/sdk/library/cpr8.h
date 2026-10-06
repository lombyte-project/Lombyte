#ifndef LOMBYTE_RNC_SDK_LIBRARY_CPR8_H
#define LOMBYTE_RNC_SDK_LIBRARY_CPR8_H

#include "types.h"

struct MpegDecoder {
    u8 pad_0[0xD8];
    s32 unkD8;
};

struct MpegCopyParams {
    s32 unk0;
    u8 pad_4[0x8];
    s32 unkC;
    s32 unk10;
};

struct MpegDecoderFrame {
    u8 pad_0[0xE0];
    s32 unkE0;
    s32 unkE4;
    u8 pad_E8[0x8C];
    s32 unk174;
};

#endif /* LOMBYTE_RNC_SDK_LIBRARY_CPR8_H */
