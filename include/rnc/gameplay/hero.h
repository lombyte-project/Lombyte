#ifndef LOMBYTE_RNC_GAMEPLAY_HERO_H
#define LOMBYTE_RNC_GAMEPLAY_HERO_H

#include "types.h"
#include "rnc/math/vector.h"

struct Moby;

/*
 * The player character's state at D_0013F350. Only the fields some matched
 * function in src/ or src/overlays reads or writes are listed; the struct is
 * larger. Field types follow the overlay views of this object
 * (rnc/overlay/hero.h and the per-file overlay Hero structs); 4-float
 * arrays are Vec4. unkXXX fields have no known meaning yet. Where the views
 * disagree the header keeps its type: the level overlays read 0x1090,
 * 0x1180, 0x1184 and 0x2080 as raw bytes (struct Moby * here) and one view
 * reads 0x560 as an int * (s32 here).
 */
struct Hero {
    u8 pad_0[0x40];
    f32 unk40[16];                 /* 0x40 */
    Vec4 pos;                      /* 0x80 */
    Vec4 rot;                      /* 0x90: f[2] is the yaw passed to fast_add_rotations/fast_difference_between_rotations */
    Vec4 unkA0;                    /* 0xA0 */
    u8 pad_B0[0x10];
    Vec4 unkC0;                    /* 0xC0 */
    u8 pad_D0[0x10];
    Vec4 velocity;                 /* 0xE0: f[2] clamped at -9*D_0015ED6C, length tested against 7*D_0015ED6C, cleared on stop */
    Vec4 unkF0;                    /* 0xF0 */
    Vec4 unk100;                   /* 0x100 */
    Vec4 unk110;                   /* 0x110 */
    Vec4 unk120;                   /* 0x120 */
    u8 pad_130[0x10];
    Vec4 unk140;                   /* 0x140 */
    Vec4 unk150;                   /* 0x150 */
    f32 unk160;                    /* 0x160 */
    f32 unk164;                    /* 0x164 */
    f32 unk168;                    /* 0x168 */
    f32 unk16C;                    /* 0x16C */
    Vec4 unk170;                   /* 0x170 */
    f32 unk180;                    /* 0x180 */
    f32 unk184;                    /* 0x184 */
    f32 unk188;                    /* 0x188 */
    u8 pad_18C[0x4];
    f32 unk190;                    /* 0x190 */
    f32 unk194;                    /* 0x194 */
    s32 state_timer;               /* 0x198: zeroed by the set-state function; compared with scale_game_frames(n) */
    u8 pad_19C[0x4];
    s32 unk1A0;                    /* 0x1A0 */
    s32 unk1A4;                    /* 0x1A4 */
    u8 pad_1A8[0x4];
    s32 unk1AC;                    /* 0x1AC */
    s16 unk1B0;                    /* 0x1B0 */
    s16 unk1B2;                    /* 0x1B2 */
    s32 unk1B4;                    /* 0x1B4 */
    s32 unk1B8;                    /* 0x1B8 */
    u8 pad_1BC[0x4];
    s32 unk1C0;                    /* 0x1C0 */
    s32 unk1C4;                    /* 0x1C4 */
    s16 unk1C8;                    /* 0x1C8 */
    u8 pad_1CA[0x6];
    s32 unk1D0;                    /* 0x1D0 */
    s32 unk1D4;                    /* 0x1D4 */
    s16 unk1D8;                    /* 0x1D8 */
    u8 pad_1DA[0x4];
    s16 unk1DE;                    /* 0x1DE */
    s16 unk1E0;                    /* 0x1E0 */
    s16 unk1E2;                    /* 0x1E2 */
    s16 unk1E4;                    /* 0x1E4 */
    u8 pad_1E6[0x2];
    s16 unk1E8;                    /* 0x1E8 */
    u8 pad_1EA[0x8];
    s16 unk1F2;                    /* 0x1F2 */
    s16 unk1F4;                    /* 0x1F4 */
    u8 pad_1F6[0x2];
    s16 unk1F8;                    /* 0x1F8 */
    u8 pad_1FA[0x16];
    Vec4 unk210;                   /* 0x210 */
    f32 unk220;                    /* 0x220 */
    f32 unk224;                    /* 0x224 */
    f32 unk228;                    /* 0x228 */
    f32 unk22C;                    /* 0x22C */
    f32 unk230;                    /* 0x230 */
    f32 unk234;                    /* 0x234 */
    f32 unk238;                    /* 0x238 */
    u8 *unk23C;                    /* 0x23C */
    u8 pad_240[0x8];
    f32 unk248;                    /* 0x248 */
    u8 pad_24C[0xB];
    u8 unk257;                     /* 0x257 */
    u8 pad_258[0x38];
    Vec4 unk290;                   /* 0x290 */
    Vec4 unk2A0;                   /* 0x2A0 */
    u8 pad_2B0[0x28];
    f32 unk2D8;                    /* 0x2D8 */
    f32 unk2DC;                    /* 0x2DC */
    f32 unk2E0;                    /* 0x2E0 */
    u8 pad_2E4[0x8];
    f32 unk2EC;                    /* 0x2EC */
    f32 height_threshold;          /* 0x2F0 */
    f32 unk2F4;                    /* 0x2F4 */
    s32 unk2F8;                    /* 0x2F8 */
    struct Moby *unk2FC;           /* 0x2FC */
    s32 unk300;                    /* 0x300 */
    f32 unk304;                    /* 0x304 */
    s16 unk308;                    /* 0x308 */
    s16 unk30A;                    /* 0x30A */
    s16 unk30C;                    /* 0x30C */
    s16 unk30E;                    /* 0x30E */
    u8 pad_310[0xA0];
    f32 unk3B0;                    /* 0x3B0 */
    u8 pad_3B4[0x4];
    s16 unk3B8;                    /* 0x3B8 */
    u8 pad_3BA[0x2];
    s16 unk3BC;                    /* 0x3BC */
    s16 unk3BE;                    /* 0x3BE */
    u8 pad_3C0[0x10];
    s32 unk3D0;                    /* 0x3D0 */
    s32 unk3D4;                    /* 0x3D4 */
    f32 unk3D8;                    /* 0x3D8 */
    u8 pad_3DC[0x4];
    s32 unk3E0;                    /* 0x3E0 */
    u8 pad_3E4[0x14];
    f32 unk3F8;                    /* 0x3F8 */
    u8 pad_3FC[0x4];
    Vec4 unk400;                   /* 0x400 */
    f32 unk410;                    /* 0x410 */
    u8 pad_414[0x4];
    s32 unk418;                    /* 0x418 */
    s16 unk41C;                    /* 0x41C */
    s16 unk41E;                    /* 0x41E */
    s32 unk420;                    /* 0x420 */
    f32 unk424;                    /* 0x424 */
    u8 pad_428[0x8];
    f32 unk430;                    /* 0x430 */
    f32 unk434;                    /* 0x434 */
    u8 pad_438[0x8];
    f32 unk440;                    /* 0x440 */
    f32 unk444;                    /* 0x444 */
    f32 unk448;                    /* 0x448 */
    s32 unk44C;                    /* 0x44C */
    u8 pad_450[0x30];
    f32 unk480;                    /* 0x480 */
    f32 unk484;                    /* 0x484 */
    f32 unk488;                    /* 0x488 */
    f32 unk48C;                    /* 0x48C */
    f32 unk490;                    /* 0x490 */
    f32 unk494;                    /* 0x494 */
    u8 pad_498[0x2];
    s16 unk49A;                    /* 0x49A */
    u8 pad_49C[0x4];
    f32 unk4A0;                    /* 0x4A0 */
    s16 unk4A4;                    /* 0x4A4 */
    s16 unk4A6;                    /* 0x4A6 */
    s16 unk4A8;                    /* 0x4A8 */
    s16 unk4AA;                    /* 0x4AA */
    u8 pad_4AC[0x2];
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
    u8 pad_4EC[0x4];
    f32 unk4F0;                    /* 0x4F0 */
    u8 pad_4F4[0xC];
    Vec4 unk500;                   /* 0x500 */
    u8 pad_510[0x30];
    Vec4 unk540;                   /* 0x540 */
    Vec4 unk550;                   /* 0x550 */
    s32 unk560;                    /* 0x560 */
    s32 unk564;                    /* 0x564 */
    f32 unk568;                    /* 0x568 */
    s32 unk56C;                    /* 0x56C */
    s32 unk570;                    /* 0x570 */
    f32 unk574;                    /* 0x574 */
    u8 pad_578[0x4];
    f32 unk57C;                    /* 0x57C */
    u8 pad_580[0x4];
    s32 unk584;                    /* 0x584 */
    u8 pad_588[0x4];
    s32 unk58C;                    /* 0x58C */
    s32 unk590;                    /* 0x590 */
    s32 unk594;                    /* 0x594 */
    s32 unk598;                    /* 0x598 */
    f32 unk59C;                    /* 0x59C */
    s32 unk5A0;                    /* 0x5A0 */
    s32 unk5A4;                    /* 0x5A4 */
    s32 unk5A8;                    /* 0x5A8 */
    u8 pad_5AC[0xC];
    s32 unk5B8;                    /* 0x5B8 */
    s16 unk5BC;                    /* 0x5BC */
    s16 unk5BE;                    /* 0x5BE */
    s32 unk5C0;                    /* 0x5C0 */
    s32 unk5C4;                    /* 0x5C4 */
    f32 unk5C8;                    /* 0x5C8 */
    u8 pad_5CC[0xC];
    struct Moby *unk5D8;           /* 0x5D8 */
    u8 pad_5DC[0x4];
    Vec4 unk5E0;                   /* 0x5E0 */
    u8 pad_5F0[0xC];
    s32 unk5FC;                    /* 0x5FC */
    s32 unk600;                    /* 0x600 */
    u8 pad_604[0x94];
    s32 unk698;                    /* 0x698 */
    u8 pad_69C[0x8];
    struct Moby *unk6A4;           /* 0x6A4 */
    u8 pad_6A8[0x18];
    u8 unk6C0[0x60];               /* 0x6C0 */
    u8 pad_720[0xB0];
    f32 unk7D0[0x20];              /* 0x7D0 */
    u8 pad_850[0x10];
    f32 unk860;                    /* 0x860 */
    u8 pad_864[0x8];
    u8 *unk86C;                    /* 0x86C */
    u8 pad_870[0x4];
    f32 unk874;                    /* 0x874 */
    u8 pad_878[0x10];
    s16 unk888;                    /* 0x888 */
    u8 pad_88A[0x12];
    s16 unk89C;                    /* 0x89C */
    s16 unk89E;                    /* 0x89E */
    f32 unk8A0;                    /* 0x8A0 */
    u8 pad_8A4[0x8];
    s16 unk8AC;                    /* 0x8AC */
    u8 pad_8AE[0x2];
    s16 unk8B0;                    /* 0x8B0 */
    s16 unk8B2;                    /* 0x8B2 */
    u8 pad_8B4[0x8];
    s16 unk8BC;                    /* 0x8BC */
    s16 unk8BE;                    /* 0x8BE */
    u8 pad_8C0[0x20];
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
    u8 pad_968[0x4];
    f32 unk96C;                    /* 0x96C */
    f32 unk970;                    /* 0x970 */
    s32 unk974;                    /* 0x974 */
    u8 pad_978[0x4];
    f32 unk97C;                    /* 0x97C */
    f32 unk980;                    /* 0x980 */
    u8 pad_984[0x4];
    s32 unk988;                    /* 0x988 */
    u8 pad_98C[0x8];
    struct Moby *unk994;           /* 0x994 */
    u8 pad_998[0x4];
    s16 unk99C;                    /* 0x99C */
    u8 pad_99E[0x2];
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
    u8 pad_9D0[0x4];
    f32 unk9D4;                    /* 0x9D4 */
    u8 pad_9D8[0xC];
    f32 unk9E4;                    /* 0x9E4 */
    f32 unk9E8;                    /* 0x9E8 */
    u8 pad_9EC[0x68];
    struct Moby *unkA54;           /* 0xA54 */
    s32 unkA58;                    /* 0xA58 */
    f32 unkA5C;                    /* 0xA5C */
    s32 unkA60;                    /* 0xA60 */
    u8 pad_A64[0x4];
    f32 unkA68;                    /* 0xA68 */
    f32 unkA6C;                    /* 0xA6C */
    f32 unkA70;                    /* 0xA70 */
    u8 pad_A74[0x1C];
    f32 unkA90;                    /* 0xA90 */
    u8 pad_A94[0x4];
    s32 unkA98;                    /* 0xA98 */
    s32 unkA9C;                    /* 0xA9C */
    u8 pad_AA0[0x8];
    f32 unkAA8;                    /* 0xAA8 */
    u8 pad_AAC[0x8];
    s32 unkAB4;                    /* 0xAB4 */
    u8 pad_AB8[0x250];
    s32 unkD08;                    /* 0xD08 */
    u8 pad_D0C[0x8];
    s32 unkD14;                    /* 0xD14 */
    u8 pad_D18[0x378];
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
    u8 unk12EE;                    /* 0x12EE */
    u8 pad_12EF[0x325];
    s32 unk1614;                   /* 0x1614 */
    u8 pad_1618[0x18];
    s32 unk1630;                   /* 0x1630 */
    u8 pad_1634[0x2];
    s16 unk1636;                   /* 0x1636 */
    u8 pad_1638[0x8];
    Vec4 unk1640;                  /* 0x1640 */
    s32 unk1650;                   /* 0x1650 */
    u8 pad_1654[0xC];
    s32 unk1660;                   /* 0x1660 */
    u8 pad_1664[0xC];
    Vec4 unk1670;                  /* 0x1670 */
    u8 pad_1680[0x976];
    u8 ammo_used;                  /* 0x1FF6 */
    u8 ammo_capacity;              /* 0x1FF7 */
    u8 pad_1FF8[0x88];
    struct Moby *moby;             /* 0x2080 */
    s32 state;                     /* 0x2084: state id; the state machines switch on it, FUN_L14_0022ff58 sets it */
    s32 state_step;                /* 0x2088: 0 on every state change; state code sets 1 and tests ==0/==1 */
    s32 control_mode;              /* 0x208C */
    s32 prev_state;                /* 0x2090: gets state on every state change */
    s32 prev_control_mode;         /* 0x2094: gets control_mode on every state change */
    s32 prev_state_timer;          /* 0x2098: gets state_timer on state change; copied back when the change is undone */
    s32 prev2_state;               /* 0x209C: gets prev_state on every state change */
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
    u8 pad_20BC[0x8];
    s32 unk20C4;                   /* 0x20C4 */
    u8 pad_20C8[0x158];
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
    s16 unk22D8;                   /* 0x22D8 */
    u8 pad_22DA[0x2];
    s16 unk22DC;                   /* 0x22DC */
    s16 unk22DE;                   /* 0x22DE */
    u8 pad_22E0[0x1C];
    s16 unk22FC;                   /* 0x22FC */
};

/* Compile-time layout checks: a wrong offset makes the array size negative. */
#define HERO_OFFSET_CHECK(field, off) \
    typedef char hero_offset_check_##field[ \
        ((unsigned long)&((struct Hero *)0)->field == (off)) ? 1 : -1]
HERO_OFFSET_CHECK(unk40, 0x40);
HERO_OFFSET_CHECK(pos, 0x80);
HERO_OFFSET_CHECK(rot, 0x90);
HERO_OFFSET_CHECK(unkA0, 0xA0);
HERO_OFFSET_CHECK(unkC0, 0xC0);
HERO_OFFSET_CHECK(velocity, 0xE0);
HERO_OFFSET_CHECK(unkF0, 0xF0);
HERO_OFFSET_CHECK(unk100, 0x100);
HERO_OFFSET_CHECK(unk110, 0x110);
HERO_OFFSET_CHECK(unk120, 0x120);
HERO_OFFSET_CHECK(unk140, 0x140);
HERO_OFFSET_CHECK(unk150, 0x150);
HERO_OFFSET_CHECK(unk160, 0x160);
HERO_OFFSET_CHECK(unk164, 0x164);
HERO_OFFSET_CHECK(unk168, 0x168);
HERO_OFFSET_CHECK(unk16C, 0x16C);
HERO_OFFSET_CHECK(unk170, 0x170);
HERO_OFFSET_CHECK(unk180, 0x180);
HERO_OFFSET_CHECK(unk184, 0x184);
HERO_OFFSET_CHECK(unk188, 0x188);
HERO_OFFSET_CHECK(unk190, 0x190);
HERO_OFFSET_CHECK(unk194, 0x194);
HERO_OFFSET_CHECK(state_timer, 0x198);
HERO_OFFSET_CHECK(unk1A0, 0x1A0);
HERO_OFFSET_CHECK(unk1A4, 0x1A4);
HERO_OFFSET_CHECK(unk1AC, 0x1AC);
HERO_OFFSET_CHECK(unk1B0, 0x1B0);
HERO_OFFSET_CHECK(unk1B2, 0x1B2);
HERO_OFFSET_CHECK(unk1B4, 0x1B4);
HERO_OFFSET_CHECK(unk1B8, 0x1B8);
HERO_OFFSET_CHECK(unk1C0, 0x1C0);
HERO_OFFSET_CHECK(unk1C4, 0x1C4);
HERO_OFFSET_CHECK(unk1C8, 0x1C8);
HERO_OFFSET_CHECK(unk1D0, 0x1D0);
HERO_OFFSET_CHECK(unk1D4, 0x1D4);
HERO_OFFSET_CHECK(unk1D8, 0x1D8);
HERO_OFFSET_CHECK(unk1DE, 0x1DE);
HERO_OFFSET_CHECK(unk1E0, 0x1E0);
HERO_OFFSET_CHECK(unk1E2, 0x1E2);
HERO_OFFSET_CHECK(unk1E4, 0x1E4);
HERO_OFFSET_CHECK(unk1E8, 0x1E8);
HERO_OFFSET_CHECK(unk1F2, 0x1F2);
HERO_OFFSET_CHECK(unk1F4, 0x1F4);
HERO_OFFSET_CHECK(unk1F8, 0x1F8);
HERO_OFFSET_CHECK(unk210, 0x210);
HERO_OFFSET_CHECK(unk220, 0x220);
HERO_OFFSET_CHECK(unk224, 0x224);
HERO_OFFSET_CHECK(unk228, 0x228);
HERO_OFFSET_CHECK(unk22C, 0x22C);
HERO_OFFSET_CHECK(unk230, 0x230);
HERO_OFFSET_CHECK(unk234, 0x234);
HERO_OFFSET_CHECK(unk238, 0x238);
HERO_OFFSET_CHECK(unk23C, 0x23C);
HERO_OFFSET_CHECK(unk248, 0x248);
HERO_OFFSET_CHECK(unk257, 0x257);
HERO_OFFSET_CHECK(unk290, 0x290);
HERO_OFFSET_CHECK(unk2A0, 0x2A0);
HERO_OFFSET_CHECK(unk2D8, 0x2D8);
HERO_OFFSET_CHECK(unk2DC, 0x2DC);
HERO_OFFSET_CHECK(unk2E0, 0x2E0);
HERO_OFFSET_CHECK(unk2EC, 0x2EC);
HERO_OFFSET_CHECK(height_threshold, 0x2F0);
HERO_OFFSET_CHECK(unk2F4, 0x2F4);
HERO_OFFSET_CHECK(unk2F8, 0x2F8);
HERO_OFFSET_CHECK(unk2FC, 0x2FC);
HERO_OFFSET_CHECK(unk300, 0x300);
HERO_OFFSET_CHECK(unk304, 0x304);
HERO_OFFSET_CHECK(unk308, 0x308);
HERO_OFFSET_CHECK(unk30A, 0x30A);
HERO_OFFSET_CHECK(unk30C, 0x30C);
HERO_OFFSET_CHECK(unk30E, 0x30E);
HERO_OFFSET_CHECK(unk3B0, 0x3B0);
HERO_OFFSET_CHECK(unk3B8, 0x3B8);
HERO_OFFSET_CHECK(unk3BC, 0x3BC);
HERO_OFFSET_CHECK(unk3BE, 0x3BE);
HERO_OFFSET_CHECK(unk3D0, 0x3D0);
HERO_OFFSET_CHECK(unk3D4, 0x3D4);
HERO_OFFSET_CHECK(unk3D8, 0x3D8);
HERO_OFFSET_CHECK(unk3E0, 0x3E0);
HERO_OFFSET_CHECK(unk3F8, 0x3F8);
HERO_OFFSET_CHECK(unk400, 0x400);
HERO_OFFSET_CHECK(unk410, 0x410);
HERO_OFFSET_CHECK(unk418, 0x418);
HERO_OFFSET_CHECK(unk41C, 0x41C);
HERO_OFFSET_CHECK(unk41E, 0x41E);
HERO_OFFSET_CHECK(unk420, 0x420);
HERO_OFFSET_CHECK(unk424, 0x424);
HERO_OFFSET_CHECK(unk430, 0x430);
HERO_OFFSET_CHECK(unk434, 0x434);
HERO_OFFSET_CHECK(unk440, 0x440);
HERO_OFFSET_CHECK(unk444, 0x444);
HERO_OFFSET_CHECK(unk448, 0x448);
HERO_OFFSET_CHECK(unk44C, 0x44C);
HERO_OFFSET_CHECK(unk480, 0x480);
HERO_OFFSET_CHECK(unk484, 0x484);
HERO_OFFSET_CHECK(unk488, 0x488);
HERO_OFFSET_CHECK(unk48C, 0x48C);
HERO_OFFSET_CHECK(unk490, 0x490);
HERO_OFFSET_CHECK(unk494, 0x494);
HERO_OFFSET_CHECK(unk49A, 0x49A);
HERO_OFFSET_CHECK(unk4A0, 0x4A0);
HERO_OFFSET_CHECK(unk4A4, 0x4A4);
HERO_OFFSET_CHECK(unk4A6, 0x4A6);
HERO_OFFSET_CHECK(unk4A8, 0x4A8);
HERO_OFFSET_CHECK(unk4AA, 0x4AA);
HERO_OFFSET_CHECK(unk4AE, 0x4AE);
HERO_OFFSET_CHECK(unk4AF, 0x4AF);
HERO_OFFSET_CHECK(unk4B0, 0x4B0);
HERO_OFFSET_CHECK(unk4B8, 0x4B8);
HERO_OFFSET_CHECK(unk4BC, 0x4BC);
HERO_OFFSET_CHECK(unk4C0, 0x4C0);
HERO_OFFSET_CHECK(unk4C4, 0x4C4);
HERO_OFFSET_CHECK(unk4D0, 0x4D0);
HERO_OFFSET_CHECK(unk4E4, 0x4E4);
HERO_OFFSET_CHECK(unk4E8, 0x4E8);
HERO_OFFSET_CHECK(unk4F0, 0x4F0);
HERO_OFFSET_CHECK(unk500, 0x500);
HERO_OFFSET_CHECK(unk540, 0x540);
HERO_OFFSET_CHECK(unk550, 0x550);
HERO_OFFSET_CHECK(unk560, 0x560);
HERO_OFFSET_CHECK(unk564, 0x564);
HERO_OFFSET_CHECK(unk568, 0x568);
HERO_OFFSET_CHECK(unk56C, 0x56C);
HERO_OFFSET_CHECK(unk570, 0x570);
HERO_OFFSET_CHECK(unk574, 0x574);
HERO_OFFSET_CHECK(unk57C, 0x57C);
HERO_OFFSET_CHECK(unk584, 0x584);
HERO_OFFSET_CHECK(unk58C, 0x58C);
HERO_OFFSET_CHECK(unk590, 0x590);
HERO_OFFSET_CHECK(unk594, 0x594);
HERO_OFFSET_CHECK(unk598, 0x598);
HERO_OFFSET_CHECK(unk59C, 0x59C);
HERO_OFFSET_CHECK(unk5A0, 0x5A0);
HERO_OFFSET_CHECK(unk5A4, 0x5A4);
HERO_OFFSET_CHECK(unk5A8, 0x5A8);
HERO_OFFSET_CHECK(unk5B8, 0x5B8);
HERO_OFFSET_CHECK(unk5BC, 0x5BC);
HERO_OFFSET_CHECK(unk5BE, 0x5BE);
HERO_OFFSET_CHECK(unk5C0, 0x5C0);
HERO_OFFSET_CHECK(unk5C4, 0x5C4);
HERO_OFFSET_CHECK(unk5C8, 0x5C8);
HERO_OFFSET_CHECK(unk5D8, 0x5D8);
HERO_OFFSET_CHECK(unk5E0, 0x5E0);
HERO_OFFSET_CHECK(unk5FC, 0x5FC);
HERO_OFFSET_CHECK(unk600, 0x600);
HERO_OFFSET_CHECK(unk698, 0x698);
HERO_OFFSET_CHECK(unk6A4, 0x6A4);
HERO_OFFSET_CHECK(unk6C0, 0x6C0);
HERO_OFFSET_CHECK(unk7D0, 0x7D0);
HERO_OFFSET_CHECK(unk860, 0x860);
HERO_OFFSET_CHECK(unk86C, 0x86C);
HERO_OFFSET_CHECK(unk874, 0x874);
HERO_OFFSET_CHECK(unk888, 0x888);
HERO_OFFSET_CHECK(unk89C, 0x89C);
HERO_OFFSET_CHECK(unk89E, 0x89E);
HERO_OFFSET_CHECK(unk8A0, 0x8A0);
HERO_OFFSET_CHECK(unk8AC, 0x8AC);
HERO_OFFSET_CHECK(unk8B0, 0x8B0);
HERO_OFFSET_CHECK(unk8B2, 0x8B2);
HERO_OFFSET_CHECK(unk8BC, 0x8BC);
HERO_OFFSET_CHECK(unk8BE, 0x8BE);
HERO_OFFSET_CHECK(unk8E0, 0x8E0);
HERO_OFFSET_CHECK(unk8E4, 0x8E4);
HERO_OFFSET_CHECK(unk8E8, 0x8E8);
HERO_OFFSET_CHECK(unk8EC, 0x8EC);
HERO_OFFSET_CHECK(unk8F0, 0x8F0);
HERO_OFFSET_CHECK(unk8F4, 0x8F4);
HERO_OFFSET_CHECK(unk8F8, 0x8F8);
HERO_OFFSET_CHECK(unk8FC, 0x8FC);
HERO_OFFSET_CHECK(unk900, 0x900);
HERO_OFFSET_CHECK(unk902, 0x902);
HERO_OFFSET_CHECK(unk90A, 0x90A);
HERO_OFFSET_CHECK(unk90C, 0x90C);
HERO_OFFSET_CHECK(unk90E, 0x90E);
HERO_OFFSET_CHECK(unk910, 0x910);
HERO_OFFSET_CHECK(unk914, 0x914);
HERO_OFFSET_CHECK(unk920, 0x920);
HERO_OFFSET_CHECK(unk930, 0x930);
HERO_OFFSET_CHECK(unk940, 0x940);
HERO_OFFSET_CHECK(unk944, 0x944);
HERO_OFFSET_CHECK(unk948, 0x948);
HERO_OFFSET_CHECK(unk94C, 0x94C);
HERO_OFFSET_CHECK(unk950, 0x950);
HERO_OFFSET_CHECK(unk954, 0x954);
HERO_OFFSET_CHECK(unk960, 0x960);
HERO_OFFSET_CHECK(unk964, 0x964);
HERO_OFFSET_CHECK(unk96C, 0x96C);
HERO_OFFSET_CHECK(unk970, 0x970);
HERO_OFFSET_CHECK(unk974, 0x974);
HERO_OFFSET_CHECK(unk97C, 0x97C);
HERO_OFFSET_CHECK(unk980, 0x980);
HERO_OFFSET_CHECK(unk988, 0x988);
HERO_OFFSET_CHECK(unk994, 0x994);
HERO_OFFSET_CHECK(unk99C, 0x99C);
HERO_OFFSET_CHECK(unk9A0, 0x9A0);
HERO_OFFSET_CHECK(unk9A4, 0x9A4);
HERO_OFFSET_CHECK(unk9A8, 0x9A8);
HERO_OFFSET_CHECK(unk9AC, 0x9AC);
HERO_OFFSET_CHECK(unk9B0, 0x9B0);
HERO_OFFSET_CHECK(unk9B4, 0x9B4);
HERO_OFFSET_CHECK(unk9B8, 0x9B8);
HERO_OFFSET_CHECK(unk9BC, 0x9BC);
HERO_OFFSET_CHECK(unk9BE, 0x9BE);
HERO_OFFSET_CHECK(unk9C4, 0x9C4);
HERO_OFFSET_CHECK(unk9C8, 0x9C8);
HERO_OFFSET_CHECK(unk9CC, 0x9CC);
HERO_OFFSET_CHECK(unk9D4, 0x9D4);
HERO_OFFSET_CHECK(unk9E4, 0x9E4);
HERO_OFFSET_CHECK(unk9E8, 0x9E8);
HERO_OFFSET_CHECK(unkA54, 0xA54);
HERO_OFFSET_CHECK(unkA58, 0xA58);
HERO_OFFSET_CHECK(unkA5C, 0xA5C);
HERO_OFFSET_CHECK(unkA60, 0xA60);
HERO_OFFSET_CHECK(unkA68, 0xA68);
HERO_OFFSET_CHECK(unkA6C, 0xA6C);
HERO_OFFSET_CHECK(unkA70, 0xA70);
HERO_OFFSET_CHECK(unkA90, 0xA90);
HERO_OFFSET_CHECK(unkA98, 0xA98);
HERO_OFFSET_CHECK(unkA9C, 0xA9C);
HERO_OFFSET_CHECK(unkAA8, 0xAA8);
HERO_OFFSET_CHECK(unkAB4, 0xAB4);
HERO_OFFSET_CHECK(unkD08, 0xD08);
HERO_OFFSET_CHECK(unkD14, 0xD14);
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
HERO_OFFSET_CHECK(unk12EE, 0x12EE);
HERO_OFFSET_CHECK(unk1614, 0x1614);
HERO_OFFSET_CHECK(unk1630, 0x1630);
HERO_OFFSET_CHECK(unk1636, 0x1636);
HERO_OFFSET_CHECK(unk1640, 0x1640);
HERO_OFFSET_CHECK(unk1650, 0x1650);
HERO_OFFSET_CHECK(unk1660, 0x1660);
HERO_OFFSET_CHECK(unk1670, 0x1670);
HERO_OFFSET_CHECK(ammo_used, 0x1FF6);
HERO_OFFSET_CHECK(ammo_capacity, 0x1FF7);
HERO_OFFSET_CHECK(moby, 0x2080);
HERO_OFFSET_CHECK(state, 0x2084);
HERO_OFFSET_CHECK(state_step, 0x2088);
HERO_OFFSET_CHECK(control_mode, 0x208C);
HERO_OFFSET_CHECK(prev_state, 0x2090);
HERO_OFFSET_CHECK(prev_control_mode, 0x2094);
HERO_OFFSET_CHECK(prev_state_timer, 0x2098);
HERO_OFFSET_CHECK(prev2_state, 0x209C);
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
HERO_OFFSET_CHECK(unk20C4, 0x20C4);
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
HERO_OFFSET_CHECK(unk22D8, 0x22D8);
HERO_OFFSET_CHECK(unk22DC, 0x22DC);
HERO_OFFSET_CHECK(unk22DE, 0x22DE);
HERO_OFFSET_CHECK(unk22FC, 0x22FC);
#undef HERO_OFFSET_CHECK

extern struct Hero hero __asm__("D_0013F350");

#endif /* LOMBYTE_RNC_GAMEPLAY_HERO_H */
