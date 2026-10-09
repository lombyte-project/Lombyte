#ifndef LOMBYTE_RNC_OVERLAY_LEG_RIG_H
#define LOMBYTE_RNC_OVERLAY_LEG_RIG_H

#include "types.h"

/* Two-legged walker rig kept in a moby's pvars (Eudora: at pvars + 0x160).
 * Set up by FUN_L04_002d1418, refreshed by FUN_L04_00295de8. */

/* Attachment record handed to attach_manipulator (FUN_0020cb10). */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 attached;
    /* 0x02 */ u8 pad2[0x3E];
} LegRigManip;

/* One leg: joint ids, yaw, angle limits and its two manipulators. */
typedef struct {
    /* 0x00 */ u8 joint;
    /* 0x01 */ u8 joint2;
    /* 0x02 */ u8 slot_a;
    /* 0x03 */ u8 slot_b;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ f32 yaw;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ f32 angle_min;         /* set by FUN_L04_00292370 */
    /* 0x20 */ f32 angle_max;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ LegRigManip manip_a;
    /* 0x70 */ LegRigManip manip_b;
} LegRigLeg;

/* Step timing of one foot pair within an animation: frame spans between marks. */
typedef struct {
    /* 0x00 */ f32 span[2];
    /* 0x08 */ f32 from[2];
    /* 0x10 */ f32 to[2];
} LegRigPhase;

/* Per-animation-sequence entry (FUN_L04_002923b8 fills the timing). */
typedef struct {
    /* 0x00 */ s32 id;                /* animation sequence id */
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 rate;              /* unk4 / FUN_001f96b0(len * 2) */
    /* 0x0C */ f32 start;             /* FUN_L04_002418b0(class slot, id) */
    /* 0x10 */ f32 len;               /* FUN_L04_00241910(class slot, id) - start */
    /* 0x14 */ u8 pad14[0xC];
    /* 0x20 */ LegRigPhase phase[2];
} LegRigAnim;

typedef struct {
    /* 0x000 */ f32 pts[6][4];        /* joint positions from FUN_0020cd48 */
    /* 0x060 */ u8 pad60[0x10];
    /* 0x070 */ LegRigAnim *anims[13];
    /* 0x0A4 */ u8 padA4[0xC];
    /* 0x0B0 */ u8 unkB0;
    /* 0x0B1 */ u8 unkB1;
    /* 0x0B2 */ u8 unkB2;
    /* 0x0B3 */ u8 unkB3;
    /* 0x0B4 */ u8 jointB4;
    /* 0x0B5 */ u8 jointB5;
    /* 0x0B6 */ u8 mode;
    /* 0x0B7 */ u8 unkB7;
    /* 0x0B8 */ s32 unkB8;
    /* 0x0BC */ s32 unkBC;
    /* 0x0C0 */ f32 speed;
    /* 0x0C4 */ f32 unkC4;
    /* 0x0C8 */ s32 unkC8;
    /* 0x0CC */ f32 unkCC;
    /* 0x0D0 */ f32 unkD0;
    /* 0x0D4 */ f32 unkD4;
    /* 0x0D8 */ s32 unkD8;
    /* 0x0DC */ f32 unkDC;
    /* 0x0E0 */ f32 unkE0;
    /* 0x0E4 */ s32 unkE4;
    /* 0x0E8 */ s32 unkE8;
    /* 0x0EC */ s16 unkEC;
    /* 0x0EE */ s16 unkEE;
    /* 0x0F0 */ LegRigLeg legs[2];
} LegRig;

#endif /* LOMBYTE_RNC_OVERLAY_LEG_RIG_H */
