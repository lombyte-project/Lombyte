#ifndef LOMBYTE_RNC_GAMEPLAY_HERO_H
#define LOMBYTE_RNC_GAMEPLAY_HERO_H

#include "types.h"
#include "rnc/math/vector.h"

struct Moby;

/*
 * The player character's state at D_0013F350. Only the fields some matched
 * function in src/ or src/overlays reads or writes are listed; the struct is
 * larger. Field types follow the overlay views of this object (the G_8 and
 * P231ae0 views in rnc/overlay/hero.h and the per-file overlay Hero structs);
 * unkXXX fields have no known meaning yet. The level overlays read 0x2080 as
 * raw bytes; it stays a struct Moby * here.
 */
struct Hero {
    u8 pad_0[0x80];
    Vec4 pos;                      /* 0x80 */
    u8 pad_90[0x8];
    f32 unk98;                     /* 0x98 */
    u8 pad_9C[0x8C];
    f32 unk128;                    /* 0x128 */
    u8 pad_12C[0x6C];
    s32 unk198;                    /* 0x198 */
    u8 pad_19C[0x14];
    s16 unk1B0;                    /* 0x1B0 */
    u8 pad_1B2[0xE];
    s32 unk1C0;                    /* 0x1C0 */
    u8 pad_1C4[0x4C];
    Vec4 unk210;                   /* 0x210 */
    f32 unk220;                    /* 0x220 */
    f32 unk224;                    /* 0x224 */
    f32 unk228;                    /* 0x228 */
    f32 unk22C;                    /* 0x22C */
    f32 unk230;                    /* 0x230 */
    f32 unk234;                    /* 0x234 */
    f32 unk238;                    /* 0x238 */
    u8 pad_23C[0x1B];
    u8 unk257;                     /* 0x257 */
    u8 pad_258[0x38];
    Vec4 unk290;                   /* 0x290 */
    u8 pad_2A0[0x50];
    f32 height_threshold;          /* 0x2F0 */
    u8 pad_2F4[0x8];
    struct Moby *unk2FC;           /* 0x2FC */
    s32 unk300;                    /* 0x300 */
    u8 pad_304[0xA];
    s16 unk30E;                    /* 0x30E */
    u8 pad_310[0x10E];
    s16 unk41E;                    /* 0x41E */
    s32 unk420;                    /* 0x420 */
    u8 pad_424[0x10];
    f32 unk434;                    /* 0x434 */
    u8 pad_438[0x128];
    s32 unk560;                    /* 0x560 */
    u8 pad_564[0xC];
    s32 unk570;                    /* 0x570 */
    u8 pad_574[0x4A];
    s16 unk5BE;                    /* 0x5BE */
    u8 pad_5C0[0xAD0];
    struct Moby *secondary_moby;   /* 0x1090 */
    u8 pad_1094[0xC];
    s32 unk10A0;                   /* 0x10A0 */
    u8 pad_10A4[0x4];
    s16 unk10A8;                   /* 0x10A8 */
    u8 pad_10AA[0x2];
    u8 unk10AC;                    /* 0x10AC */
    u8 pad_10AD[0xB];
    s32 equipped_gadget;           /* 0x10B8 */
    u8 pad_10BC[0xC4];
    struct Moby *unk1180;          /* 0x1180 */
    struct Moby *unk1184;          /* 0x1184 */
    u8 pad_1188[0x1C];
    s32 unk11A4;                   /* 0x11A4 */
    s32 unk11A8;                   /* 0x11A8 */
    u8 pad_11AC[0x136];
    u8 unk12E2;                    /* 0x12E2 */
    u8 unk12E3;                    /* 0x12E3 */
    u8 base_condition;             /* 0x12E4 */
    u8 selector_1;                 /* 0x12E5 */
    u8 selector_3;                 /* 0x12E6 */
    u8 unk12E7;                    /* 0x12E7 */
    u8 unk12E8;                    /* 0x12E8 */
    u8 pad_12E9[0x1];
    u8 unk12EA;                    /* 0x12EA */
    u8 selector_11;                /* 0x12EB */
    u8 selector_13;                /* 0x12EC */
    u8 unk12ED;                    /* 0x12ED */
    u8 pad_12EE[0x326];
    s32 unk1614;                   /* 0x1614 */
    u8 pad_1618[0x28];
    Vec4 unk1640;                  /* 0x1640 */
    u8 pad_1650[0x9A6];
    u8 ammo_used;                  /* 0x1FF6 */
    u8 ammo_capacity;              /* 0x1FF7 */
    u8 pad_1FF8[0x88];
    struct Moby *moby;             /* 0x2080 */
    s32 secondary_mode;            /* 0x2084 */
    s32 unk2088;                   /* 0x2088 */
    s32 control_mode;              /* 0x208C */
    s32 unk2090;                   /* 0x2090 */
    s32 unk2094;                   /* 0x2094 */
    s32 unk2098;                   /* 0x2098 */
    s32 unk209C;                   /* 0x209C */
    u8 pad_20A0[0x4];
    u8 unk20A4;                    /* 0x20A4 */
    u8 unk20A5;                    /* 0x20A5 */
    u8 pad_20A6[0x1];
    u8 unk20A7;                    /* 0x20A7 */
    u8 unk20A8;                    /* 0x20A8 */
    u8 unk20A9;                    /* 0x20A9 */
    u8 unk20AA;                    /* 0x20AA */
    u8 pad_20AB[0x1];
    u8 unk20AC;                    /* 0x20AC */
    u8 pad_20AD[0x4];
    u8 unk20B1;                    /* 0x20B1 */
    u8 pad_20B2[0x1];
    u8 unk20B3;                    /* 0x20B3 */
    u8 pad_20B4[0x4];
    s32 unk20B8;                   /* 0x20B8 */
    u8 pad_20BC[0x164];
    s32 unk2220;                   /* 0x2220 */
    s32 unk2224;                   /* 0x2224 */
    s32 unk2228;                   /* 0x2228 */
    s32 unk222C;                   /* 0x222C */
    s32 unk2230;                   /* 0x2230 */
    s32 unk2234;                   /* 0x2234 */
    u8 pad_2238[0x38];
    s32 unk2270;                   /* 0x2270 */
    u8 pad_2274[0xC];
    struct Moby *unk2280;          /* 0x2280 */
    s32 unk2284;                   /* 0x2284 */
    u8 pad_2288[0x8];
    f32 unk2290;                   /* 0x2290 */
    s32 unk2294;                   /* 0x2294 */
    s32 unk2298;                   /* 0x2298 */
    f32 unk229C;                   /* 0x229C */
    s32 unk22A0;                   /* 0x22A0 */
    f32 unk22A4;                   /* 0x22A4 */
    s32 unk22A8;                   /* 0x22A8 */
    u8 pad_22AC[0x18];
    s32 unk22C4;                   /* 0x22C4 */
    s16 unk22C8;                   /* 0x22C8 */
    u8 unk22CA;                    /* 0x22CA */
    u8 pad_22CB[0x7];
    s16 unk22D2;                   /* 0x22D2 */
    s16 unk22D4;                   /* 0x22D4 */
    s16 unk22D6;                   /* 0x22D6 */
};

/* Compile-time layout checks: a wrong offset makes the array size negative. */
#define HERO_OFFSET_CHECK(field, off) \
    typedef char hero_offset_check_##field[ \
        ((unsigned long)&((struct Hero *)0)->field == (off)) ? 1 : -1]
HERO_OFFSET_CHECK(pos, 0x80);
HERO_OFFSET_CHECK(unk98, 0x98);
HERO_OFFSET_CHECK(unk128, 0x128);
HERO_OFFSET_CHECK(unk198, 0x198);
HERO_OFFSET_CHECK(unk1B0, 0x1B0);
HERO_OFFSET_CHECK(unk1C0, 0x1C0);
HERO_OFFSET_CHECK(unk210, 0x210);
HERO_OFFSET_CHECK(unk220, 0x220);
HERO_OFFSET_CHECK(unk224, 0x224);
HERO_OFFSET_CHECK(unk228, 0x228);
HERO_OFFSET_CHECK(unk22C, 0x22C);
HERO_OFFSET_CHECK(unk230, 0x230);
HERO_OFFSET_CHECK(unk234, 0x234);
HERO_OFFSET_CHECK(unk238, 0x238);
HERO_OFFSET_CHECK(unk257, 0x257);
HERO_OFFSET_CHECK(unk290, 0x290);
HERO_OFFSET_CHECK(height_threshold, 0x2F0);
HERO_OFFSET_CHECK(unk2FC, 0x2FC);
HERO_OFFSET_CHECK(unk300, 0x300);
HERO_OFFSET_CHECK(unk30E, 0x30E);
HERO_OFFSET_CHECK(unk41E, 0x41E);
HERO_OFFSET_CHECK(unk420, 0x420);
HERO_OFFSET_CHECK(unk434, 0x434);
HERO_OFFSET_CHECK(unk560, 0x560);
HERO_OFFSET_CHECK(unk570, 0x570);
HERO_OFFSET_CHECK(unk5BE, 0x5BE);
HERO_OFFSET_CHECK(secondary_moby, 0x1090);
HERO_OFFSET_CHECK(unk10A0, 0x10A0);
HERO_OFFSET_CHECK(unk10A8, 0x10A8);
HERO_OFFSET_CHECK(unk10AC, 0x10AC);
HERO_OFFSET_CHECK(equipped_gadget, 0x10B8);
HERO_OFFSET_CHECK(unk1180, 0x1180);
HERO_OFFSET_CHECK(unk1184, 0x1184);
HERO_OFFSET_CHECK(unk11A4, 0x11A4);
HERO_OFFSET_CHECK(unk11A8, 0x11A8);
HERO_OFFSET_CHECK(unk12E2, 0x12E2);
HERO_OFFSET_CHECK(unk12E3, 0x12E3);
HERO_OFFSET_CHECK(base_condition, 0x12E4);
HERO_OFFSET_CHECK(selector_1, 0x12E5);
HERO_OFFSET_CHECK(selector_3, 0x12E6);
HERO_OFFSET_CHECK(unk12E7, 0x12E7);
HERO_OFFSET_CHECK(unk12E8, 0x12E8);
HERO_OFFSET_CHECK(unk12EA, 0x12EA);
HERO_OFFSET_CHECK(selector_11, 0x12EB);
HERO_OFFSET_CHECK(selector_13, 0x12EC);
HERO_OFFSET_CHECK(unk12ED, 0x12ED);
HERO_OFFSET_CHECK(unk1614, 0x1614);
HERO_OFFSET_CHECK(unk1640, 0x1640);
HERO_OFFSET_CHECK(ammo_used, 0x1FF6);
HERO_OFFSET_CHECK(ammo_capacity, 0x1FF7);
HERO_OFFSET_CHECK(moby, 0x2080);
HERO_OFFSET_CHECK(secondary_mode, 0x2084);
HERO_OFFSET_CHECK(unk2088, 0x2088);
HERO_OFFSET_CHECK(control_mode, 0x208C);
HERO_OFFSET_CHECK(unk2090, 0x2090);
HERO_OFFSET_CHECK(unk2094, 0x2094);
HERO_OFFSET_CHECK(unk2098, 0x2098);
HERO_OFFSET_CHECK(unk209C, 0x209C);
HERO_OFFSET_CHECK(unk20A4, 0x20A4);
HERO_OFFSET_CHECK(unk20A5, 0x20A5);
HERO_OFFSET_CHECK(unk20A7, 0x20A7);
HERO_OFFSET_CHECK(unk20A8, 0x20A8);
HERO_OFFSET_CHECK(unk20A9, 0x20A9);
HERO_OFFSET_CHECK(unk20AA, 0x20AA);
HERO_OFFSET_CHECK(unk20AC, 0x20AC);
HERO_OFFSET_CHECK(unk20B1, 0x20B1);
HERO_OFFSET_CHECK(unk20B3, 0x20B3);
HERO_OFFSET_CHECK(unk20B8, 0x20B8);
HERO_OFFSET_CHECK(unk2220, 0x2220);
HERO_OFFSET_CHECK(unk2224, 0x2224);
HERO_OFFSET_CHECK(unk2228, 0x2228);
HERO_OFFSET_CHECK(unk222C, 0x222C);
HERO_OFFSET_CHECK(unk2230, 0x2230);
HERO_OFFSET_CHECK(unk2234, 0x2234);
HERO_OFFSET_CHECK(unk2270, 0x2270);
HERO_OFFSET_CHECK(unk2280, 0x2280);
HERO_OFFSET_CHECK(unk2284, 0x2284);
HERO_OFFSET_CHECK(unk2290, 0x2290);
HERO_OFFSET_CHECK(unk2294, 0x2294);
HERO_OFFSET_CHECK(unk2298, 0x2298);
HERO_OFFSET_CHECK(unk229C, 0x229C);
HERO_OFFSET_CHECK(unk22A0, 0x22A0);
HERO_OFFSET_CHECK(unk22A4, 0x22A4);
HERO_OFFSET_CHECK(unk22A8, 0x22A8);
HERO_OFFSET_CHECK(unk22C4, 0x22C4);
HERO_OFFSET_CHECK(unk22C8, 0x22C8);
HERO_OFFSET_CHECK(unk22CA, 0x22CA);
HERO_OFFSET_CHECK(unk22D2, 0x22D2);
HERO_OFFSET_CHECK(unk22D4, 0x22D4);
HERO_OFFSET_CHECK(unk22D6, 0x22D6);
#undef HERO_OFFSET_CHECK

extern struct Hero hero __asm__("D_0013F350");

#endif /* LOMBYTE_RNC_GAMEPLAY_HERO_H */
