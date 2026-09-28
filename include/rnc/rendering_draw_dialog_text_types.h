#ifndef RNC_RENDERING_DRAW_DIALOG_TEXT_TYPES_H
#define RNC_RENDERING_DRAW_DIALOG_TEXT_TYPES_H

#include "types.h"

struct Blob18 {
    u8 data[0x18];
};

struct M2c_D_0013F350 {
    u8 pad_0[0x894];
    s32 unk894;
    u8 pad_898[0x2];
    s16 unk89A;
    u8 pad_89C[0xC];
    s32 unk8A8;
    u8 pad_8AC[0x14];
    s32 unk8C0;
};

struct M2c_D_00193300 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad_14[0x8];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
};

struct M2c_temp_18_388 {
    /* the 0x18-byte text box descriptor at sp+0x200 / sp+0x220 */
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u16 unk12;
    u8 pad_14[0x4];
    u64 unk18;
};

struct M2c_Frame {
    u8 buf[0x200];              /* 0x000 */
    struct M2c_temp_18_388 box; /* 0x200 */
    struct M2c_temp_18_388 box2;/* 0x220 */
    u8 buf2[0x40];              /* 0x240 */
    s32 sp280;                  /* 0x280 */
};

#endif /* RNC_RENDERING_DRAW_DIALOG_TEXT_TYPES_H */
