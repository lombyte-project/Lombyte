#ifndef LOMBYTE_RNC_OVERLAY_HERO_H
#define LOMBYTE_RNC_OVERLAY_HERO_H

#include "types.h"

/* 128-bit value, for whole-quadword copies. */
typedef int OvlQuad __attribute__((mode(TI)));

typedef struct {
    char p0[0x44];
    int f44;
    int f48;
    char p1[0x10];
    float f5C;
    int f60;
    char p2[0xC];
} Rec70;

typedef struct {
    char p0[0x24];
    int f24;
    char p1[0x24];
} Rec4C;

typedef struct {
    u8 pad0[0x18];
    s32 i18;
    s32 i1C;
    union {
        OvlQuad q;
        f32 f[4];
    } v20;
    u8 pad30[0x10];
    f32 f40;
    f32 f44;
} HS;

typedef struct {
    u8 pad0[0x10];
    OvlQuad v10;
    u8 *p20;
    s32 i24;
    u8 b28;
    u8 pad29[3];
    f32 f2C;
    s32 i30;
    u8 *p34;
    u8 pad38[8];
} T_8;

typedef union {
    OvlQuad q;
    f32 f[4];
    s32 i[4];
} V_8;

/* Partial view of D_0013C940 (both older views merged). */
typedef struct {
    char p0[0x128] __attribute__((aligned(16)));
    float f128;
    float f12C;
    char p130[0x70];
    int f1A0;
    int f1A4;
    char p1A8[0x10];
    int f1B8;
    char p1BC[0x4];
    int held_buttons;    /* 0x1C0: named so in sound_options_menu.c's view */
    int pressed_buttons; /* 0x1C4: same; that menu tests 0xD00 and 0x10 on it */
    char p1C8[0xC];
    int f1D4;
    char p1D8[0x4];
    int f1DC;
} Pad;

/* One 0x2C-byte row of a per-level table (all three older views merged). */
typedef struct {
    char p0[4];
    int f4;
    char p8[4];
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    char p24[8];
} Rec2C;

/* Partial view of D_00141848 (both older views merged). */
typedef struct {
    char p0[0x58];
    unsigned short f58;
    unsigned short f5A;
    int f5C;
    char p60[0x30];
    unsigned short f90;
    unsigned short f92;
    int f94;
    char p98[0x60];
    unsigned short fF8;
    unsigned short fFA;
    int fFC;
} S141948;

#endif /* LOMBYTE_RNC_OVERLAY_HERO_H */
