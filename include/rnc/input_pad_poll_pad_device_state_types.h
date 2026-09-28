#ifndef RNC_INPUT_PAD_POLL_PAD_DEVICE_STATE_TYPES_H
#define RNC_INPUT_PAD_POLL_PAD_DEVICE_STATE_TYPES_H

#include "types.h"

struct M2c_arg0 {
    u8 pad_0[0x188];
    u8 unk188;
    s32 unk189;
    u8 pad_18D[0x7];
    s32 unk194;
    s32 unk198;
    s32 unk19C;
    s32 unk1A0;
    u8 pad_1A4[0x8];
    s32 unk1AC;
    s32 unk1B0;
    u8 pad_1B4[0x8];
    s32 unk1BC;
    u8 pad_1C0[0x1C];
    s32 unk1DC;
    u8 pad_1E0[0x168];
    s32 unk348;
};

#endif /* RNC_INPUT_PAD_POLL_PAD_DEVICE_STATE_TYPES_H */
