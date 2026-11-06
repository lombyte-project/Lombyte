#ifndef LOMBYTE_RNC_OVERLAY_L04_LEG_RIG_H
#define LOMBYTE_RNC_OVERLAY_L04_LEG_RIG_H

#include "types.h"

/* Two-legged walker rig kept in a moby's pvars (Eudora: at pvars + 0x160).
 * Set up by FUN_L04_002d1418, refreshed by FUN_L04_00295de8. */

/* Attachment record handed to attach_manipulator. */
typedef struct {
    /* 0x00 */ u8            anim_joint;
    /* 0x01 */ u8            attached;
    /* 0x02 */ u8            pad2[0xE];
    /* 0x10 */ sceVu0FVECTOR rot; /* quaternion, turned by rig_animate */
    /* 0x20 */ u8            pad20[0x20];
} LegRigManip;

/* One leg: joint ids, yaw, angle limits and its two manipulators. */
typedef struct {
    /* 0x00 */ u8          joint;
    /* 0x01 */ u8          joint2;
    /* 0x02 */ u8          slot_a;
    /* 0x03 */ u8          slot_b;
    /* 0x04 */ u8          pad4[4];
    /* 0x08 */ f32         yaw;
    /* 0x0C */ f32         lower_turn; /* turns manip_a */
    /* 0x10 */ f32         lower_turn_speed;
    /* 0x14 */ f32         upper_turn; /* turns manip_b */
    /* 0x18 */ f32         upper_turn_speed;
    /* 0x1C */ f32         angle_min; /* set by FUN_L04_00292370 */
    /* 0x20 */ f32         angle_max;
    /* 0x24 */ u8          pad24[0xC];
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
    /* 0x00 */ s32         id;     /* animation sequence id */
    /* 0x04 */ f32         stride; /* distance covered per animation cycle */
    /* 0x08 */ f32         rate;   /* stride / multiply_global_factor_ed64(len * 2) */
    /* 0x0C */ f32         start;  /* animation_start(class slot, id) */
    /* 0x10 */ f32         len;    /* animation_end(class slot, id) - start */
    /* 0x14 */ u8          pad14[0xC];
    /* 0x20 */ LegRigPhase phase[2];
} LegRigAnim;

typedef struct {
    /* 0x000 */ f32         pts[6][4]; /* joint positions from transform_scaled_vertex_batch */
    /* 0x060 */ u8          pad60[0xC];
    /* 0x06C */ f32         heading; /* turn target while control_flags & 4 is set */
    /* 0x070 */ LegRigAnim *anims[13];
    /* 0x0A4 */ u8          padA4[0xC];
    /* 0x0B0 */ u8          unkB0;
    /* 0x0B1 */ u8          unkB1;
    /* 0x0B2 */ u8          unkB2;
    /* 0x0B3 */ u8          unkB3;
    /* 0x0B4 */ u8          jointB4;
    /* 0x0B5 */ u8          jointB5;
    /* 0x0B6 */ u8          mode;
    /* 0x0B7 */ u8          gait; /* D_L04_001CA938 gait id of the current animation, 0 for none */
    /* 0x0B8 */ s32         pitch_target;   /* f32 bits: spring_turn target for rot.y */
    /* 0x0BC */ s32         pitch_velocity; /* f32 bits */
    /* 0x0C0 */ f32         speed;
    /* 0x0C4 */ f32         max_speed_step; /* speed moves toward the gait speed by at most this */
    /* 0x0C8 */ s32         turn_velocity;  /* f32 bits: spring_turn velocity of rot.z */
    /* 0x0CC */ f32         turn_gain;
    /* 0x0D0 */ f32         turn_damping;
    /* 0x0D4 */ f32         max_turn_step;
    /* 0x0D8 */ s32         unkD8;
    /* 0x0DC */ f32         gravity_step;
    /* 0x0E0 */ f32         probe_lift; /* walker gait animations set probe_offset to twice this */
    /* 0x0E4 */ s32         unkE4;
    /* 0x0E8 */ s32         probe_offset; /* f32 bits: eases back to 0 */
    /* 0x0EC */ s16         unkEC;
    /* 0x0EE */ s16         control_flags; /* 4: steer to heading at speed instead of by the legs */
    /* 0x0F0 */ LegRigLeg   legs[2];
} LegRig;

#endif /* LOMBYTE_RNC_OVERLAY_L04_LEG_RIG_H */
