#ifndef LOMBYTE_RNC_OVERLAY_HERO_TABLES_H
#define LOMBYTE_RNC_OVERLAY_HERO_TABLES_H

#include "types.h"

/*
 * Per-level data tables the hero code reads (one copy per level overlay,
 * e.g. D_L00_00179890, D_L00_00179AC0, D_L00_0017BC28 in level 0).
 */

/*
 * 0x70-byte entry of D_L00_00179890, picked by FUN_L00_00229010(&index).
 * The hero stores the index, sets duration_frames from duration and then
 * either enters hero state 0x40 or calls FUN_L00_002323b8(unk48, 0, ...).
 */
typedef struct HeroTableEntry70 {
    u8 pad_0[0x44];
    s32 use_state_40;    /* nonzero: hero_set_state(0x40, 1) instead of FUN_L00_002323b8 */
    s32 unk48;           /* first argument of FUN_L00_002323b8 */
    u8 pad_4C[0x10];
    f32 duration;        /* seconds */
    s32 duration_frames; /* FUN_001f96b0(duration) * 60 */
    u8 pad_64[0xC];
} HeroTableEntry70;

/*
 * 0x4C-byte row of D_L00_00179AC0, indexed by the id from
 * FUN_L00_0020d498(0); the hero plays anim when its moby is not already in it.
 */
typedef struct HeroAnimRow {
    u8 pad_0[0x18];
    s32 unk18;           /* tested != 0 */
    u8 pad_1C[0x8];
    s32 anim;            /* animation id, compared with moby+0x53 */
    u8 pad_28[0x24];
} HeroAnimRow;

/*
 * 0x2C-byte row of D_L00_0017BC28, indexed by hero.unkA60. fC..f20 are
 * limits on the float hero.unkAA8 (fC is subtracted from it; f10, f14,
 * f18 and f1C..f20 are compared with it).
 */
typedef struct HeroTableRow2C {
    u8 pad_0[0x4];
    s32 kind;            /* 0 or 1; 1 adds a turn-direction test */
    u8 pad_8[0x4];
    s32 fC;
    s32 f10;
    s32 f14;
    s32 f18;
    s32 f1C;
    s32 f20;
    u8 pad_24[0x8];
} HeroTableRow2C;

#endif /* LOMBYTE_RNC_OVERLAY_HERO_TABLES_H */
