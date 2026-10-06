#include "types.h"
struct MusicStreamState {
    u8 pad_0[0x40];
    s16 unk40;
    s16 unk42;
    u8 pad_44[0x18];
    s16 unk5C;
    s16 unk5E;
    u8 pad_60[0x18];
    s16 unk78;
    s16 unk7A;
};

extern struct MusicStreamState D_001516D0;
void music_pause(s32 arg0) {
    if (arg0 != 0) {
        D_001516D0.unk5C = -0x8000;
        D_001516D0.unk5E = 0;
    }
    D_001516D0.unk40 = -0x8000;
    D_001516D0.unk42 = 0;
    D_001516D0.unk78 = -0x8000;
    D_001516D0.unk7A = 0;
}

/* ACCEPTED: attempt-1 (sn-O2) direct 100/100/100; s16 fields at 0x40/0x42/0x5C/0x5E/0x78/0x7A, retail store order. */
