#ifndef RNC_SDK_LIBRARY__DECODEORSKIPFIELD_TYPES_H
#define RNC_SDK_LIBRARY__DECODEORSKIPFIELD_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x34];
    struct M2c_temp_16_13 * unk40;
};

struct M2c_temp_16_13 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 pad_C[0xA0];
    s32 unkAC;
    u8 pad_B0[0x24];
    s32 unkD4;
    u8 pad_D8[0x40];
    s32 unk118;
    u8 pad_11C[0x4];
    s32 unk120;
    u8 pad_124[0x50];
    s32 unk174;
};

#endif /* RNC_SDK_LIBRARY__DECODEORSKIPFIELD_TYPES_H */
