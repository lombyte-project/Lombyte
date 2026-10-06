#ifndef LOMBYTE_RNC_INPUT_PAD_CLEAR_PAD_INPUT_H
#define LOMBYTE_RNC_INPUT_PAD_CLEAR_PAD_INPUT_H

#include "types.h"

struct PAD {
    u8 pad_0[0x1A0];
    s32 unk1A0;
    s32 unk1A4;
    s32 unk1A8;
    u8 pad_1AC[0x4];
    s32 unk1B0;
    s32 unk1B4;
    s32 unk1B8;
    u8 pad_1BC[0x4];
    s32 unk1C0;
    s32 unk1C4;
    s32 unk1C8;
    u8 pad_1CC[0x4];
    s32 unk1D0;
    s32 unk1D4;
    s32 unk1D8;
};

struct WordCell {
    s32 unk0;
};

#endif /* LOMBYTE_RNC_INPUT_PAD_CLEAR_PAD_INPUT_H */
