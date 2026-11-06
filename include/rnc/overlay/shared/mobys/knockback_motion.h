#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_KNOCKBACK_MOTION_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_KNOCKBACK_MOTION_H

#include "types.h"

/* Ballistic motion of a knocked-back enemy (usually at pvars + 0x70).
 * trooper_start_knockback (FUN_L00_0025c558) launches it and
 * step_moby_motion (FUN_L00_0025c698) moves the moby along it each frame. */
struct KnockbackMotion {
    f32 velocity[4];              /* 0x00: added to pos each step; [2] loses gravity */
    f32 gravity;                  /* 0x10: per step; scaled by 0.75 after each bounce */
    f32 speed_approach_step;      /* 0x14: horizontal speed moves toward target_speed by it */
    f32 speed;                    /* 0x18: horizontal launch speed */
    f32 vertical_speed;           /* 0x1C: launch vertical speed, also the upward cap */
    s32 collision_radius_1024ths; /* 0x20 */
    s32 flags;                    /* 0x24: 2 blend ex, 4 no turn, 0x10, 0x20 no collision */
    f32 collision_z_offset;       /* 0x28: collision sphere height above pos */
    s16 timer;                    /* 0x2C: 300 frames at launch */
    u8  particle_mode;            /* 0x2E: 1 spawns shape point particles */
    u8  particle_count;           /* 0x2F */
    f32 restitution;              /* 0x30: bounce off steep walls */
    u8  pad34[8];
    u8  active;                  /* 0x3C: set at launch */
    u8  unk3D;                   /* cleared when the hit reaction is set up */
    s16 phase;                   /* 0x3E: 0 rising, 1 falling, 2 grounded */
    f32 turn_target;             /* 0x40: launch angle + pi, spring_turn target */
    f32 turn_velocity;           /* 0x44 */
    f32 max_turn_step;           /* 0x48 */
    f32 target_speed;            /* 0x4C: horizontal speed while airborne */
    f32 apex_animation_frame;    /* 0x50: -1 for none */
    f32 landing_animation_frame; /* 0x54: -1 for none */
    u8  pad58[4];
    f32 ground_bounces; /* 0x5C: counts bounces after first touching the ground */
};

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_KNOCKBACK_MOTION_H */
