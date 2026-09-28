#ifndef RNC_SDK_LIBRARY_OUTPUTFRAME_TYPES_H
#define RNC_SDK_LIBRARY_OUTPUTFRAME_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0xF8];
    s32 unkF8;
    u8 pad_FC[0x54];
    s32 unk150;
    u8 pad_154[0x20];
    s32 unk174;
    u8 pad_178[0x40];
    s32 unk1B8;
    u8 pad_1BC[0x8];
    s32 unk1C4;
    s32 unk1C8;
    u8 pad_1CC[0x8];
    s32 unk1D4;
    s32 unk1D8;
    u8 pad_1DC[0x8];
    s32 unk1E4;
};

#endif /* RNC_SDK_LIBRARY_OUTPUTFRAME_TYPES_H */
