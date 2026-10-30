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

void clear_pad_input(struct PAD *pad_state) __asm__("FUN_002172c0");

void clear_pad_input(struct PAD *pad_state) {
    s32 remaining;
    struct WordCell *slot;

    slot = ((u8 *)pad_state + (0x140));
    remaining = 0xF;
    pad_state->unk1A0 = 0;
    pad_state->unk1A4 = 0;
    pad_state->unk1A8 = 0;
    pad_state->unk1D0 = 1;
    pad_state->unk1B4 = 0;
    pad_state->unk1B8 = 0;
    pad_state->unk1C0 = 0;
    pad_state->unk1C4 = 0;
    pad_state->unk1C8 = 0;
    pad_state->unk1D8 = 0;
    pad_state->unk1D4 = 1;
    pad_state->unk1B0 = 0;
    do {
        *(s32 *)((u8 *)slot - 0x40) = 0;
        remaining -= 1;
        slot->unk0 = 0;
        slot += 1;
    } while (remaining >= 0);
}

extern void func_002172C0(struct PAD *pad_state) __attribute__((alias("FUN_002172c0")));