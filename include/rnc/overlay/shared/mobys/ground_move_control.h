#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_GROUND_MOVE_CONTROL_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_GROUND_MOVE_CONTROL_H

#include "types.h"

/* Movement control block of a moby walking on the ground toward a target.
 * init_ground_move_control (FUN_L01_0026d930) sets the defaults,
 * step_mover (FUN_L00_00259028) turns the moby and moves it with
 * move_moby_with_ground_and_collision_checks (FUN_L00_00258b50). */
struct GroundMoveControl {
    s32 collision_radius_1024ths; /* 0x00: radius in 1/1024 units for the collision move */
    f32 step_height;              /* 0x04 */
    f32 max_drop;                 /* 0x08: move undone when the ground drops more */
    f32 max_climb;                /* 0x0C: move undone when the ground rises more */
    f32 speed;                    /* 0x10: along the heading; accelerated, braked, clamped */
    f32 turn_velocity;            /* 0x14: spring_turn velocity of rot.z */
    f32 vertical_velocity;        /* 0x18: minus gravity each step */
    f32 acceleration;             /* 0x1C */
    f32 brake;                    /* 0x20: fraction of speed removed near the target */
    f32 max_speed;                /* 0x24 */
    f32 heading_tolerance;        /* 0x28: moves only while the heading error is smaller */
    f32 arrive_distance;          /* 0x2C: result flag 4 when the target is closer */
    f32 ground_probe_depth;       /* 0x30: ground probe goes this far below pos.z */
    u8  pad_34[0x4];
    s32 movement_option_flags; /* 0x38: 1 no gravity, 4 use move_heading, 0x20 probe flag */
    f32 turn_spring;           /* 0x3C: spring_turn spring strength */
    f32 turn_damping;          /* 0x40 */
    f32 max_turn_step;         /* 0x44 */
    s32 move_timer;            /* 0x48: frames left to move; refilled while facing the target */
    f32 move_heading;          /* 0x4C: heading used with movement option 4 */
};

void init_ground_move_control(struct GroundMoveControl *) __asm__("FUN_L01_0026d930");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_GROUND_MOVE_CONTROL_H */
