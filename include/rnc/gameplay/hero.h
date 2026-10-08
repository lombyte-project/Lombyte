#ifndef LOMBYTE_RNC_GAMEPLAY_HERO_H
#define LOMBYTE_RNC_GAMEPLAY_HERO_H

#include "types.h"
#include "rnc/math/vector.h"

struct Moby;

/*
 * The player character's state at D_0013F350. Only the fields some matched
 * function in src/ reads or writes are listed; the struct is larger.
 */
struct Hero {
    u8 pad_0[0x80];
    Vec4 pos;                    /* 0x80 */
    u8 pad_90[0x8];
    f32 unk98;                   /* 0x98 */
    u8 pad_9C[0x1F4];
    Vec4 unk290;                 /* 0x290 */
    u8 pad_2A0[0x50];
    f32 height_threshold;        /* 0x2F0 */
    u8 pad_2F4[0x8];
    struct Moby *unk2FC;         /* 0x2FC */
    u8 pad_300[0x260];
    s32 unk560;                  /* 0x560 */
    u8 pad_564[0xC];
    s32 unk570;                  /* 0x570 */
    u8 pad_574[0xB1C];
    struct Moby *secondary_moby; /* 0x1090 */
    u8 pad_1094[0x24];
    s32 equipped_gadget;         /* 0x10B8 */
    u8 pad_10BC[0x228];
    u8 base_condition;           /* 0x12E4 */
    u8 selector_1;               /* 0x12E5 */
    u8 selector_3;               /* 0x12E6 */
    u8 pad_12E7[0x4];
    u8 selector_11;              /* 0x12EB */
    u8 selector_13;              /* 0x12EC */
    u8 pad_12ED[0xD09];
    u8 ammo_used;                /* 0x1FF6 */
    u8 ammo_capacity;            /* 0x1FF7 */
    u8 pad_1FF8[0x88];
    struct Moby *moby;           /* 0x2080 */
    s32 secondary_mode;          /* 0x2084 */
    u8 pad_2088[0x4];
    s32 control_mode;            /* 0x208C */
    u8 pad_2090[0x21];
    u8 unk20B1;                  /* 0x20B1 */
    u8 pad_20B2[0x1D2];
    s32 unk2284;                 /* 0x2284 */
};

extern struct Hero hero __asm__("D_0013F350");

#endif /* LOMBYTE_RNC_GAMEPLAY_HERO_H */
