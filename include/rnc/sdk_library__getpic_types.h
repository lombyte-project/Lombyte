#ifndef RNC_SDK_LIBRARY__GETPIC_TYPES_H
#define RNC_SDK_LIBRARY__GETPIC_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x40];
    struct M2c_temp_16_12 * unk40;
};

struct M2c_temp_16_12 {
    s32 unk0;
    u8 pad_4[0x90];
    s32 unk94;
    s32 unk98;
    s32 unk9C;
    s32 unkA0;
    s32 unkA4;
    s32 unkA8;
    u8 pad_AC[0x28];
    s32 unkD4;
    s32 unkD8;
    u8 pad_DC[0x98];
    s32 unk174;
    u8 pad_178[0x6A8];
    s32 unk820;
    u8 pad_824[0x24];
    s32 unk848;
};

#endif /* RNC_SDK_LIBRARY__GETPIC_TYPES_H */
