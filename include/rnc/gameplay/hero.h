#ifndef LOMBYTE_RNC_GAMEPLAY_HERO_H
#define LOMBYTE_RNC_GAMEPLAY_HERO_H

#include "types.h"
#include "rnc/math/vector.h"

struct Moby;

/*
 * A trail of moby copies following a source moby (shared/gameplay/state/00261968.c):
 * FUN_L00_00262500 sets the source, FUN_L00_00262528 adds a copy,
 * FUN_L00_00262608 records the source's last 8 positions and places each copy
 * `delay` frames behind, FUN_L00_00262840 removes the copies.
 */
struct MobyTrail {
    Vec4 pos[8];                   /* 0x00: ring of source positions */
    Vec4 rot[8];                   /* 0x80: ring of source rotations */
    s32 copy_fade[4];              /* 0x100: start of copy moby byte 0x23; all 0 ends the trail */
    s32 delay[4];                  /* 0x110: frames each copy lags behind */
    struct Moby *copies[4];        /* 0x120 */
    s16 head;                      /* 0x130: next ring slot */
    s16 count;                     /* 0x132: ring entries filled (max 8) */
    struct Moby *source;           /* 0x134 */
    s32 copy_count;                /* 0x138: copies in use (max 4) */
    s32 active;                    /* 0x13C */
};

/*
 * One of the hero's four item slots (hero.items[i], 0x1090 + i * 0x50); slot 0
 * holds the equipped gadget (load_hand_gadget). FUN_L00_0020fca8(i, v) releases
 * slot i: stores v at +0x14, sets state 3 (calling the moby's +0x74 callback
 * once), clears state and item and removes both mobys (slot 3 keeps moby2).
 * FUN_L00_0020d460 / FUN_L00_0020d498 return a ready slot's moby / item id.
 * A slot change clears +0x1A, sets +0x20 to 3 and blends the moby (2, 0, 2).
 */
struct HeroItemSlot {
    struct Moby *moby;             /* 0x00: item moby */
    struct Moby *moby2;            /* 0x04: second item moby */
    u8 pad_08[0x8];
    s32 button_mask;               /* 0x10: pad buttons that hold the gadget states */
    s32 unk14;                     /* 0x14: set on release */
    s16 timer;                     /* 0x18: counts down; a slot change waits for 0 */
    u8 unk1A;                      /* 0x1A: cleared on a slot change */
    u8 pad_1B[0x1];
    u8 unk1C;                      /* 0x1C: 2 blocks a slot change, 1 reloads timer */
    u8 timer_reload;               /* 0x1D: timer reload value */
    u8 pad_1E[0x2];
    s32 unk20;                     /* 0x20 */
    s32 state;                     /* 0x24: 0 empty, 2 ready, 3 released */
    s32 item_id;                   /* 0x28: item in the slot */
    u8 pad_2C[0x24];
};

/* Position, rotation and velocities (hero + 0x80) */
struct HeroMotion {
    Vec4 pos;                      /* 0x80: world position */
    Vec4 rot;                      /* 0x90: rotation; z is the yaw */
    Vec4 unkA0;                    /* 0xA0 */
    Vec4 unkB0;                    /* 0xB0 */
    Vec4 unkC0;                    /* 0xC0 */
    Vec4 unkD0;                    /* 0xD0 */
    Vec4 velocity;                 /* 0xE0: per-frame velocity */
    Vec4 unkF0;                    /* 0xF0 */
    Vec4 unk100;                   /* 0x100 */
    Vec4 unk110;                   /* 0x110 */
    Vec4 unk120;                   /* 0x120 */
    Vec4 unk130;                   /* 0x130 */
    Vec4 unk140;                   /* 0x140 */
    Vec4 unk150;                   /* 0x150: copied from unk110/unk100 on hero_set_state */
    f32 unk160;                    /* 0x160 */
    f32 unk164;                    /* 0x164 */
    f32 unk168;                    /* 0x168 */
    f32 unk16C;                    /* 0x16C */
    Vec4 unk170;                   /* 0x170 */
    f32 unk180;                    /* 0x180: angle, compared with rot.z */
    f32 unk184;                    /* 0x184 */
    f32 unk188;                    /* 0x188 */
    u8 pad_18C[0x4];
};

/* Random-interval timer run by FUN_L00_00205600 (hero + 0xFF0) */
struct HeroRandTimer {
    s32 fired;                     /* 0xFF0: 1 once the deadline passed */
    s32 deadline;                  /* 0xFF4: frame count to wait for */
    s32 range;                     /* 0xFF8: random span; 0 holds the timer off */
};

/* State machine bookkeeping (hero + 0x2084) */
struct HeroState {
    s32 current;                   /* 0x2084: state id; hero_set_state sets it */
    s32 step;                      /* 0x2088: sub-step, 0 on state change */
    s32 control_mode;              /* 0x208C */
    s32 prev;                      /* 0x2090: state before the last change */
    s32 prev_control_mode;         /* 0x2094: control_mode before the last change */
    s32 prev_timer;                /* 0x2098: state_timer before the last change */
    s32 prev2;                     /* 0x209C: state before prev */
    s32 prev2_control_mode;        /* 0x20A0: control mode before prev_control_mode */
};

/* Hit points (hero + 0x22A8) */
struct HeroHealth {
    s32 hp;                        /* 0x22A8: hit points; 0 = dead */
    s32 unk22AC;                   /* 0x22AC: swapped with health.hp */
    s16 unk22B0;                   /* 0x22B0: s16 copy of health.hp */
    s16 unk22B2;                   /* 0x22B2 */
};

/*
 * The player character at D_0013F350 (sizeof 0x2300). Only fields that matched
 * code in src/ or src/overlays uses are listed; unkXXX = meaning not known yet,
 * pad_XXX = no named access. Some level overlays read the item-slot mobys and
 * 0x2080 as raw bytes, and one view reads 0x560 as an int *.
 * Notes on unknown fields:
 *   unk200 / unk210 - copied from the collision hit (FUN_L00_002133a8)
 *   unk458 / unk45C - speeds eased in and scaled into velocity (FUN_L02_00223450)
 *   unk22D2         - set on a pad bit-0x10 press (no C reader yet)
 *   unk22DA         - nonzero makes FUN_L00_002133a8 call FUN_L00_002347c0
 * Item switching (FUN_L00_0020fea0): a nonzero pending_item[i] that differs
 * moves into selected_item[i] and changes slot i; saved_item[i] keeps the
 * slot's item while 0x1C, 0x1D, 6 or 4 is forced in (0x26 when it was empty).
 * hero_set_state copies state.current/control_mode/state_timer into the prev
 * fields, zeroes state.step and state_timer and clears trail.
 */
struct Hero {
    u8 pad_0[0x40];
    f32 unk40[16];                 /* 0x40 */
    struct HeroMotion motion;      /* 0x80 */
    f32 unk190;                    /* 0x190 */
    f32 unk194;                    /* 0x194 */
    s32 state_timer;               /* 0x198: frames in the current state */
    s32 unk19C;                    /* 0x19C */
    s32 unk1A0;                    /* 0x1A0 */
    s32 unk1A4;                    /* 0x1A4 */
    s32 unk1A8;                    /* 0x1A8 */
    s32 unk1AC;                    /* 0x1AC */
    s16 unk1B0;                    /* 0x1B0 */
    s16 unk1B2;                    /* 0x1B2 */
    s32 unk1B4;                    /* 0x1B4 */
    s32 unk1B8;                    /* 0x1B8 */
    s32 unk1BC;                    /* 0x1BC */
    s32 unk1C0;                    /* 0x1C0 */
    s32 unk1C4;                    /* 0x1C4 */
    s16 unk1C8;                    /* 0x1C8 */
    s16 unk1CA;                    /* 0x1CA */
    s32 unk1CC;                    /* 0x1CC */
    s32 unk1D0;                    /* 0x1D0 */
    s32 unk1D4;                    /* 0x1D4 */
    s16 unk1D8;                    /* 0x1D8 */
    s16 unk1DA;                    /* 0x1DA */
    s16 unk1DC;                    /* 0x1DC */
    s16 unk1DE;                    /* 0x1DE */
    s16 unk1E0;                    /* 0x1E0 */
    s16 unk1E2;                    /* 0x1E2 */
    s16 unk1E4;                    /* 0x1E4 */
    s16 unk1E6;                    /* 0x1E6 */
    s16 unk1E8;                    /* 0x1E8 */
    s16 unk1EA;                    /* 0x1EA */
    s16 unk1EC;                    /* 0x1EC */
    s16 unk1EE;                    /* 0x1EE */
    s16 unk1F0;                    /* 0x1F0 */
    s16 unk1F2;                    /* 0x1F2 */
    s16 unk1F4;                    /* 0x1F4 */
    s16 unk1F6;                    /* 0x1F6 */
    s16 unk1F8;                    /* 0x1F8 */
    u8 pad_1FA[0x6];
    Vec4 unk200;                   /* 0x200 */
    Vec4 unk210;                   /* 0x210 */
    f32 unk220;                    /* 0x220 */
    f32 unk224;                    /* 0x224 */
    f32 unk228;                    /* 0x228 */
    f32 unk22C;                    /* 0x22C */
    f32 unk230;                    /* 0x230 */
    f32 unk234;                    /* 0x234 */
    u8 pad_238[0x4];
    u8 *coll_hit_moby;             /* 0x23C: moby of the last push-out collision hit */
    s32 unk240;                    /* 0x240 */
    u8 pad_244[0x4];
    f32 unk248;                    /* 0x248 */
    u8 pad_24C[0xB];
    u8 unk257;                     /* 0x257 */
    u8 pad_258[0x18];
    Vec4 unk270;                   /* 0x270 */
    Vec4 unk280;                   /* 0x280 */
    Vec4 unk290;                   /* 0x290 */
    Vec4 unk2A0;                   /* 0x2A0 */
    Vec4 unk2B0;                   /* 0x2B0 */
    union { f32 f[2]; s32 i[2]; } unk2C0; /* 0x2C0: written as floats and zeroed as ints */
    union { f32 f[2]; s32 i[2]; } unk2C8; /* 0x2C8: written as floats and zeroed as ints */
    union { f32 f[2]; s32 i[2]; } unk2D0; /* 0x2D0: written as floats and zeroed as ints */
    union { f32 f; s32 i; } unk2D8; /* 0x2D8: written as a float and zeroed as an int */
    f32 unk2DC;                    /* 0x2DC */
    union { f32 f; s32 i; } unk2E0; /* 0x2E0: written as a float and zeroed as an int */
    union { f32 f; s32 i; } unk2E4; /* 0x2E4: written as a float and zeroed as an int */
    union { f32 f; s32 i; } unk2E8; /* 0x2E8: written as a float and zeroed as an int */
    f32 unk2EC;                    /* 0x2EC */
    f32 height_threshold;          /* 0x2F0 */
    f32 unk2F4;                    /* 0x2F4 */
    s32 unk2F8;                    /* 0x2F8 */
    struct Moby *unk2FC;           /* 0x2FC */
    s32 unk300;                    /* 0x300 */
    f32 unk304;                    /* 0x304 */
    s16 unk308;                    /* 0x308 */
    s16 unk30A;                    /* 0x30A */
    union { s16 s; u16 u; } unk30C; /* 0x30C: read signed, incremented unsigned */
    union { s16 s; u16 u; } unk30E; /* 0x30E: read signed, incremented unsigned */
    u8 pad_310[0xA0];
    f32 unk3B0;                    /* 0x3B0 */
    s32 unk3B4;                    /* 0x3B4 */
    s16 unk3B8;                    /* 0x3B8 */
    u8 pad_3BA[0x2];
    s16 unk3BC;                    /* 0x3BC */
    s16 unk3BE;                    /* 0x3BE */
    u8 pad_3C0[0x10];
    s32 unk3D0;                    /* 0x3D0 */
    s32 unk3D4;                    /* 0x3D4 */
    f32 unk3D8;                    /* 0x3D8 */
    f32 unk3DC;                    /* 0x3DC */
    s32 unk3E0;                    /* 0x3E0 */
    u8 *unk3E4;                    /* 0x3E4 */
    s32 unk3E8;                    /* 0x3E8 */
    u8 pad_3EC[0x8];
    f32 unk3F4;                    /* 0x3F4 */
    f32 unk3F8;                    /* 0x3F8 */
    u8 pad_3FC[0x4];
    Vec4 unk400;                   /* 0x400 */
    f32 unk410;                    /* 0x410 */
    f32 unk414;                    /* 0x414 */
    s32 unk418;                    /* 0x418: set when velocity.z clamps to unk500.z, counts up */
    s16 unk41C;                    /* 0x41C */
    s16 unk41E;                    /* 0x41E: set to 1 when FUN_L00_00233a78(velocity) < 0.001 */
    s32 unk420;                    /* 0x420: per-state scale_game_frames(N), compared with state_timer */
    f32 unk424;                    /* 0x424 */
    f32 unk428;                    /* 0x428 */
    f32 unk42C;                    /* 0x42C */
    f32 unk430;                    /* 0x430 */
    f32 unk434;                    /* 0x434 */
    f32 unk438;                    /* 0x438 */
    f32 unk43C;                    /* 0x43C */
    f32 unk440;                    /* 0x440 */
    f32 unk444;                    /* 0x444 */
    f32 unk448;                    /* 0x448 */
    s32 unk44C;                    /* 0x44C */
    s32 unk450;                    /* 0x450 */
    f32 unk454;                    /* 0x454 */
    f32 unk458;                    /* 0x458 */
    f32 unk45C;                    /* 0x45C */
    Vec4 unk460;                   /* 0x460 */
    Vec4 unk470;                   /* 0x470 */
    f32 unk480;                    /* 0x480 */
    f32 unk484;                    /* 0x484 */
    f32 unk488;                    /* 0x488 */
    f32 unk48C;                    /* 0x48C */
    f32 unk490;                    /* 0x490 */
    f32 unk494;                    /* 0x494 */
    s16 unk498;                    /* 0x498 */
    s16 unk49A;                    /* 0x49A */
    s16 unk49C;                    /* 0x49C */
    s16 unk49E;                    /* 0x49E */
    f32 unk4A0;                    /* 0x4A0 */
    s16 unk4A4;                    /* 0x4A4 */
    s16 unk4A6;                    /* 0x4A6 */
    s16 unk4A8;                    /* 0x4A8 */
    s16 unk4AA;                    /* 0x4AA */
    s16 unk4AC;                    /* 0x4AC */
    u8 unk4AE;                     /* 0x4AE */
    u8 unk4AF;                     /* 0x4AF */
    f32 unk4B0;                    /* 0x4B0 */
    u8 pad_4B4[0x4];
    f32 unk4B8;                    /* 0x4B8 */
    s32 unk4BC;                    /* 0x4BC */
    f32 unk4C0;                    /* 0x4C0 */
    s32 unk4C4;                    /* 0x4C4 */
    u8 pad_4C8[0x8];
    Vec4 unk4D0;                   /* 0x4D0 */
    u8 pad_4E0[0x4];
    f32 unk4E4;                    /* 0x4E4 */
    s32 unk4E8;                    /* 0x4E8 */
    f32 unk4EC;                    /* 0x4EC */
    f32 unk4F0;                    /* 0x4F0 */
    u8 pad_4F4[0x4];
    s32 unk4F8;                    /* 0x4F8: first argument of FUN_L00_00233f80 (FUN_L01_00233de0) */
    u8 pad_4FC[0x4];
    Vec4 unk500;                   /* 0x500 */
    Vec4 unk510;                   /* 0x510 */
    Vec4 unk520;                   /* 0x520 */
    Vec4 unk530;                   /* 0x530 */
    Vec4 unk540;                   /* 0x540 */
    Vec4 unk550;                   /* 0x550 */
    s32 unk560;                    /* 0x560 */
    s32 unk564;                    /* 0x564 */
    f32 unk568;                    /* 0x568 */
    s32 unk56C;                    /* 0x56C */
    s32 unk570;                    /* 0x570 */
    f32 unk574;                    /* 0x574 */
    s32 unk578;                    /* 0x578 */
    f32 unk57C;                    /* 0x57C */
    f32 unk580;                    /* 0x580 */
    s32 unk584;                    /* 0x584 */
    f32 unk588;                    /* 0x588 */
    s32 unk58C;                    /* 0x58C */
    s32 unk590;                    /* 0x590 */
    s32 unk594;                    /* 0x594 */
    s32 unk598;                    /* 0x598 */
    f32 unk59C;                    /* 0x59C */
    s32 unk5A0;                    /* 0x5A0 */
    s32 unk5A4;                    /* 0x5A4 */
    s32 unk5A8;                    /* 0x5A8 */
    union { f32 f; s32 i; } unk5AC; /* 0x5AC: float approached toward a speed, zeroed as an int */
    f32 unk5B0;                    /* 0x5B0 */
    s32 unk5B4;                    /* 0x5B4 */
    s32 unk5B8;                    /* 0x5B8 */
    s16 unk5BC;                    /* 0x5BC */
    s16 unk5BE;                    /* 0x5BE */
    s32 unk5C0;                    /* 0x5C0 */
    s32 unk5C4;                    /* 0x5C4 */
    f32 unk5C8;                    /* 0x5C8 */
    f32 unk5CC;                    /* 0x5CC */
    f32 unk5D0;                    /* 0x5D0 */
    f32 unk5D4;                    /* 0x5D4 */
    struct Moby *unk5D8;           /* 0x5D8 */
    f32 unk5DC;                    /* 0x5DC */
    Vec4 unk5E0;                   /* 0x5E0 */
    s32 *unk5F0;                   /* 0x5F0 */
    s32 unk5F4;                    /* 0x5F4 */
    f32 unk5F8;                    /* 0x5F8 */
    s32 unk5FC;                    /* 0x5FC */
    s32 unk600;                    /* 0x600 */
    f32 unk604;                    /* 0x604 */
    u8 pad_608[0x4];
    s32 unk60C;                    /* 0x60C */
    f32 unk610;                    /* 0x610 */
    f32 unk614;                    /* 0x614 */
    union { f32 f; s32 i; } unk618; /* 0x618: float distance clamp, zeroed as an int */
    u8 pad_61C[0x54];
    f32 unk670;                    /* 0x670 */
    f32 unk674;                    /* 0x674 */
    u8 pad_678[0x10];
    f32 unk688;                    /* 0x688 */
    u8 pad_68C[0x4];
    f32 unk690;                    /* 0x690 */
    f32 unk694;                    /* 0x694 */
    s32 unk698;                    /* 0x698 */
    f32 unk69C;                    /* 0x69C */
    s32 unk6A0;                    /* 0x6A0 */
    struct Moby *unk6A4;           /* 0x6A4 */
    u8 pad_6A8[0x18];
    Vec4 unk6C0;                   /* 0x6C0: 0x60 bytes cleared by FillTransferWords */
    f32 unk6D0;                    /* 0x6D0 */
    f32 unk6D4;                    /* 0x6D4 */
    u8 pad_6D8[0x48];
    Vec4 unk720;                   /* 0x720 */
    u8 pad_730[0x10];
    Vec4 unk740;                   /* 0x740 */
    Vec4 unk750;                   /* 0x750 */
    Vec4 unk760;                   /* 0x760 */
    Vec4 unk770;                   /* 0x770 */
    Vec4 unk780;                   /* 0x780 */
    Vec4 unk790;                   /* 0x790 */
    u8 pad_7A0[0x30];
    f32 unk7D0[0x20];              /* 0x7D0 */
    u8 pad_850[0x4];
    f32 unk854;                    /* 0x854 */
    f32 unk858;                    /* 0x858 */
    f32 unk85C;                    /* 0x85C */
    f32 unk860;                    /* 0x860 */
    s32 unk864;                    /* 0x864 */
    f32 unk868;                    /* 0x868 */
    u8 *unk86C;                    /* 0x86C */
    s32 unk870;                    /* 0x870 */
    f32 unk874;                    /* 0x874 */
    f32 unk878;                    /* 0x878 */
    f32 unk87C;                    /* 0x87C */
    u8 pad_880[0x4];
    s16 unk884;                    /* 0x884 */
    s16 unk886;                    /* 0x886 */
    s16 unk888;                    /* 0x888 */
    s16 unk88A;                    /* 0x88A */
    u8 unk88C;                     /* 0x88C */
    u8 unk88D;                     /* 0x88D */
    u8 unk88E;                     /* 0x88E */
    u8 unk88F;                     /* 0x88F */
    u8 pad_890[0x4];
    s32 unk894;                    /* 0x894: frame counter shown as m:ss.hh on the HUD (FUN_L05_00266710) */
    s16 unk898;                    /* 0x898 */
    s16 unk89A;                    /* 0x89A: shown + 1 (capped at 3) on the HUD (FUN_L05_00266320) */
    s16 unk89C;                    /* 0x89C */
    s16 unk89E;                    /* 0x89E */
    f32 unk8A0;                    /* 0x8A0 */
    f32 unk8A4;                    /* 0x8A4 */
    s32 unk8A8;                    /* 0x8A8: count shown on the HUD (FUN_L05_00266710) */
    s16 unk8AC;                    /* 0x8AC */
    u8 pad_8AE[0x1];
    u8 unk8AF;                     /* 0x8AF */
    s16 unk8B0;                    /* 0x8B0 */
    s16 unk8B2;                    /* 0x8B2 */
    void *unk8B4;                  /* 0x8B4 */
    union { f32 f; s32 i; } unk8B8; /* 0x8B8: read as a float, zeroed as an int */
    s16 unk8BC;                    /* 0x8BC */
    s16 unk8BE;                    /* 0x8BE */
    s32 unk8C0;                    /* 0x8C0: 1..3 picks the HUD rank string (FUN_L05_00266320) */
    s32 unk8C4;                    /* 0x8C4 */
    u8 pad_8C8[0x4];
    s16 unk8CC;                    /* 0x8CC */
    u8 unk8CE;                     /* 0x8CE */
    u8 pad_8CF[0x1];
    Vec4 unk8D0;                   /* 0x8D0 */
    s32 unk8E0;                    /* 0x8E0 */
    f32 unk8E4;                    /* 0x8E4 */
    f32 unk8E8;                    /* 0x8E8 */
    f32 unk8EC;                    /* 0x8EC */
    s32 unk8F0;                    /* 0x8F0 */
    s32 unk8F4;                    /* 0x8F4 */
    s16 unk8F8;                    /* 0x8F8 */
    u8 pad_8FA[0x2];
    f32 unk8FC;                    /* 0x8FC */
    s16 unk900;                    /* 0x900 */
    s16 unk902;                    /* 0x902 */
    u8 pad_904[0x6];
    s16 unk90A;                    /* 0x90A */
    s16 unk90C;                    /* 0x90C */
    s16 unk90E;                    /* 0x90E */
    s32 unk910;                    /* 0x910 */
    s32 unk914;                    /* 0x914 */
    u8 pad_918[0x8];
    Vec4 unk920;                   /* 0x920 */
    f32 unk930;                    /* 0x930 */
    u8 pad_934[0xC];
    f32 unk940;                    /* 0x940 */
    f32 unk944;                    /* 0x944 */
    f32 unk948;                    /* 0x948 */
    f32 unk94C;                    /* 0x94C */
    f32 unk950;                    /* 0x950 */
    f32 unk954;                    /* 0x954 */
    u8 pad_958[0x8];
    f32 unk960;                    /* 0x960 */
    struct Moby *unk964;           /* 0x964 */
    s32 unk968;                    /* 0x968 */
    f32 unk96C;                    /* 0x96C */
    f32 unk970;                    /* 0x970 */
    s32 unk974;                    /* 0x974 */
    u8 pad_978[0x4];
    f32 unk97C;                    /* 0x97C */
    f32 unk980;                    /* 0x980 */
    f32 unk984;                    /* 0x984 */
    s32 unk988;                    /* 0x988 */
    u8 pad_98C[0x4];
    struct Moby *unk990;           /* 0x990 */
    struct Moby *unk994;           /* 0x994 */
    f32 unk998;                    /* 0x998 */
    s16 unk99C;                    /* 0x99C */
    s16 unk99E;                    /* 0x99E */
    f32 unk9A0;                    /* 0x9A0 */
    f32 unk9A4;                    /* 0x9A4 */
    f32 unk9A8;                    /* 0x9A8 */
    f32 unk9AC;                    /* 0x9AC */
    f32 unk9B0;                    /* 0x9B0 */
    f32 unk9B4;                    /* 0x9B4 */
    f32 unk9B8;                    /* 0x9B8 */
    s16 unk9BC;                    /* 0x9BC */
    s16 unk9BE;                    /* 0x9BE */
    u8 pad_9C0[0x4];
    f32 unk9C4;                    /* 0x9C4 */
    f32 unk9C8;                    /* 0x9C8 */
    f32 unk9CC;                    /* 0x9CC */
    f32 unk9D0;                    /* 0x9D0: rail yaw (FUN_L01_002f6328) */
    f32 unk9D4;                    /* 0x9D4 */
    u8 pad_9D8[0x4];
    f32 unk9DC;                    /* 0x9DC: rail speed, eased toward the rail's speed */
    f32 unk9E0;                    /* 0x9E0: pull toward the rail, capped at the distance */
    f32 unk9E4;                    /* 0x9E4 */
    f32 unk9E8;                    /* 0x9E8 */
    u8 pad_9EC[0x64];
    u8 *unkA50;                    /* 0xA50 */
    struct Moby *unkA54;           /* 0xA54 */
    s32 unkA58;                    /* 0xA58 */
    f32 unkA5C;                    /* 0xA5C */
    s32 unkA60;                    /* 0xA60 */
    s32 unkA64;                    /* 0xA64 */
    f32 unkA68;                    /* 0xA68 */
    f32 unkA6C;                    /* 0xA6C */
    f32 unkA70;                    /* 0xA70 */
    u8 pad_A74[0x8];
    s32 unkA7C;                    /* 0xA7C */
    s32 unkA80;                    /* 0xA80 */
    u8 *unkA84;                    /* 0xA84 */
    u8 *unkA88;                    /* 0xA88 */
    void *unkA8C;                  /* 0xA8C */
    f32 unkA90;                    /* 0xA90 */
    f32 unkA94;                    /* 0xA94 */
    s32 unkA98;                    /* 0xA98 */
    s32 unkA9C;                    /* 0xA9C */
    s32 unkAA0;                    /* 0xAA0 */
    s32 unkAA4;                    /* 0xAA4 */
    f32 unkAA8;                    /* 0xAA8 */
    u8 pad_AAC[0x8];
    s32 unkAB4;                    /* 0xAB4 */
    u8 pad_AB8[0x248];
    u8*unkD00;                     /* 0xD00 */
    u8*unkD04;                     /* 0xD04 */
    u8 *unkD08[2];                 /* 0xD08: per item slot 0/1: moby from FUN_L00_0024f028 */
    u8 *unkD10;                    /* 0xD10 */
    s32 unkD14;                    /* 0xD14 */
    u8 pad_D18[0x2D8];
    struct HeroRandTimer rand_timer;/* 0xFF0 */
    u8 pad_FFC[0x14];
    s32 unk1010;                   /* 0x1010 */
    u8 pad_1014[0x7C];
    struct HeroItemSlot items[4];  /* 0x1090: item slots; slot 0 is the equipped gadget */
    u8 pad_11D0[0x110];
    s16 unk12E0;                   /* 0x12E0 */
    u8 unk12E2;                    /* 0x12E2 */
    u8 unk12E3;                    /* 0x12E3 */
    u8 base_condition;             /* 0x12E4 */
    u8 selector_1;                 /* 0x12E5 */
    u8 selector_3;                 /* 0x12E6 */
    u8 unk12E7;                    /* 0x12E7 */
    u8 unk12E8;                    /* 0x12E8 */
    u8 unk12E9;                    /* 0x12E9 */
    u8 unk12EA;                    /* 0x12EA */
    u8 selector_11;                /* 0x12EB */
    u8 selector_13;                /* 0x12EC */
    u8 unk12ED;                    /* 0x12ED */
    u8 unk12EE;                    /* 0x12EE */
    u8 pad_12EF[0x301];
    struct Moby *ship_moby;        /* 0x15F0: the ship the hero is flying (l17 FUN_L17_002ed018) */
    s16 ship_oclass;               /* 0x15F4: its oclass, -1 when none */
    u8 ship_ammo;                  /* 0x15F6: shots left in the space levels; firing needs one and takes it (FUN_L11_003126f8) */
    u8 ship_ammo_max;              /* 0x15F7: ammo pips the ship HUD draws, lit while below ship_ammo */
    u8 unk15F8;                    /* 0x15F8 */
    u8 unk15F9;                    /* 0x15F9 */
    u8 unk15FA;                    /* 0x15FA */
    u8 unk15FB;                    /* 0x15FB */
    f32 ship_hp;                   /* 0x15FC: ship health; l17 subtracts collision damage and explodes the ship below 0 */
    f32 ship_hp_max;               /* 0x1600: full ship_hp; the HUD bar shows ship_hp over it */
    f32 unk1604;                   /* 0x1604 */
    s32 ship_hp_percent;           /* 0x1608: ship_hp as a percentage for the HUD */
    s16 unk160C;                   /* 0x160C */
    u8 unk160E;                    /* 0x160E */
    u8 ship_flags;                 /* 0x160F: 2 = being steered back from the edge or height limit of the space arena (FUN_L11_00313f60) */
    u8 pad_1610[0x4];
    s32 unk1614;                   /* 0x1614 */
    u8 pad_1618[0x8];
    void *unk1620;                 /* 0x1620 */
    void *unk1624;                 /* 0x1624 */
    u8 pad_1628[0x8];
    s32 unk1630;                   /* 0x1630 */
    s16 unk1634;                   /* 0x1634 */
    s16 unk1636;                   /* 0x1636 */
    u8 pad_1638[0x8];
    Vec4 unk1640;                  /* 0x1640 */
    s32 unk1650;                   /* 0x1650 */
    u8 pad_1654[0xC];
    s32 unk1660;                   /* 0x1660 */
    u8 pad_1664[0xC];
    struct MobyTrail trail;        /* 0x1670: trail of moby copies following hero.moby */
    u8 pad_17B0[0x350];
    Vec4 unk1B00[32];              /* 0x1B00: ring of 32 quads indexed by unk21B0 */
    u8 pad_1D00[0x20];
    f32 unk1D20;                   /* 0x1D20 */
    f32 unk1D24;                   /* 0x1D24 */
    u8 pad_1D28[0x238];
    u8 unk1F60[0x40];              /* 0x1F60: 0x40-byte buffer cleared by FUN_L00_002b58d8 */
    u8 unk1FA0[0x40];              /* 0x1FA0: 0x40-byte buffer cleared by FUN_L00_002b58d8 */
    u8 *unk1FE0;                   /* 0x1FE0: moby set by FUN_L00_002b58d8 */
    u8 pad_1FE4[0x11];
    u8 unk1FF5;                    /* 0x1FF5 */
    u8 ammo_used;                  /* 0x1FF6 */
    u8 ammo_capacity;              /* 0x1FF7 */
    u8 pad_1FF8[0x48];
    struct Moby *unk2040;          /* 0x2040: nearest oclass 0x25D moby (FUN_L00_002d2ee8) */
    f32 unk2044;                   /* 0x2044: distance to unk2040; 100000 when none */
    s32 unk2048;                   /* 0x2048 */
    u8 pad_204C[0x34];
    struct Moby *moby;             /* 0x2080: the hero's moby */
    struct HeroState state;        /* 0x2084 */
    u8 unk20A4;                    /* 0x20A4 */
    u8 unk20A5;                    /* 0x20A5 */
    u8 unk20A6;                    /* 0x20A6: forces slot 0 to item 8 */
    u8 unk20A7;                    /* 0x20A7 */
    u8 unk20A8;                    /* 0x20A8 */
    u8 unk20A9;                    /* 0x20A9 */
    u8 unk20AA;                    /* 0x20AA */
    u8 unk20AB;                    /* 0x20AB */
    u8 unk20AC;                    /* 0x20AC */
    u8 unk20AD;                    /* 0x20AD */
    u8 unk20AE;                    /* 0x20AE */
    u8 unk20AF;                    /* 0x20AF */
    u8 pad_20B0[0x1];
    u8 unk20B1;                    /* 0x20B1 */
    u8 unk20B2;                    /* 0x20B2 */
    u8 unk20B3;                    /* 0x20B3 */
    u8 unk20B4;                    /* 0x20B4 */
    u8 pad_20B5[0x3];
    s32 pending_item[7];           /* 0x20B8: per slot: item to switch to */
    s32 selected_item[7];          /* 0x20D4: per slot: item after a switch */
    s32 saved_item[7];             /* 0x20F0: per slot: item kept while another is forced in */
    s32 restore_item[7];           /* 0x210C: per slot: 1 puts saved_item back */
    u8 pad_2128[0x88];
    s32 unk21B0;                   /* 0x21B0: unk1B00 ring index */
    s32 unk21B4;                   /* 0x21B4: unk1B00 ring count */
    u8 pad_21B8[0x60];
    s32 unk2218;                   /* 0x2218 */
    u8 pad_221C[0x4];
    s32 unk2220;                   /* 0x2220 */
    s32 unk2224;                   /* 0x2224 */
    s32 unk2228;                   /* 0x2228 */
    s32 unk222C;                   /* 0x222C */
    s32 unk2230;                   /* 0x2230 */
    s32 unk2234;                   /* 0x2234 */
    u8 pad_2238[0x38];
    s32 unk2270;                   /* 0x2270 */
    s32 unk2274;                   /* 0x2274 */
    u8 pad_2278[0x8];
    struct Moby *unk2280;          /* 0x2280 */
    s32 unk2284;                   /* 0x2284 */
    f32 unk2288;                   /* 0x2288 */
    f32 unk228C;                   /* 0x228C */
    f32 unk2290;                   /* 0x2290 */
    s32 unk2294;                   /* 0x2294 */
    s32 unk2298;                   /* 0x2298 */
    f32 unk229C;                   /* 0x229C */
    s32 unk22A0;                   /* 0x22A0 */
    f32 unk22A4;                   /* 0x22A4 */
    struct HeroHealth health;      /* 0x22A8 */
    s32 unk22B4;                   /* 0x22B4 */
    f32 unk22B8;                   /* 0x22B8: distance to the nearest carrying path this frame (FUN_L01_002f3120 keeps the minimum) */
    u8 pad_22BC[0x8];
    s32 unk22C4;                   /* 0x22C4 */
    s16 unk22C8;                   /* 0x22C8 */
    u8 unk22CA;                    /* 0x22CA */
    u8 unk22CB;                    /* 0x22CB */
    s16 unk22CC;                   /* 0x22CC */
    s16 unk22CE;                   /* 0x22CE */
    u8 pad_22D0[0x2];
    s16 unk22D2;                   /* 0x22D2 */
    s16 unk22D4;                   /* 0x22D4: countdown set on pad bit 0x10 press */
    s16 unk22D6;                   /* 0x22D6: countdown; nonzero with unk22D4 switches pending_item[0] */
    s16 unk22D8;                   /* 0x22D8 */
    s16 unk22DA;                   /* 0x22DA */
    s16 unk22DC;                   /* 0x22DC */
    s16 unk22DE;                   /* 0x22DE */
    s16 unk22E0;                   /* 0x22E0 */
    u8 pad_22E2[0x12];
    s32 unk22F4;                   /* 0x22F4 */
    s32 unk22F8;                   /* 0x22F8 */
    s16 unk22FC;                   /* 0x22FC */
};

/* Offsets are checked in src/check/hero_layout_check.c (make check). */
extern struct Hero hero __asm__("D_0013F350");

#endif /* LOMBYTE_RNC_GAMEPLAY_HERO_H */
