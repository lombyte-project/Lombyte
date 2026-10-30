/* Level 02 moby class rocket_door1. */
#include "types.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/level_labels.h"

extern float D_0015ED6C;

#include "sda.h"

#include "qcopy.h"

extern f32 frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;

#include "rnc/mobys/moby.h"

/* Pvars of a moby that turns from its spawn rotation to a goal rotation. */
typedef struct {
    Vec4 start; /* 0x00: spawn rotation, then its quaternion */
    Vec4 goal;  /* 0x10: goal angles in degrees, then their quaternion */
    f32  speed; /* 0x20 */
    f32  t;     /* 0x24: 0..1 along the turn */
} TurnerVars;

extern s32  D_0015ED84;
extern u8   D_0014C050[][16];
extern void FUN_L00_001fff28(void *, int, float);
extern float
approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");

/* Turns the moby from its spawn rotation to the goal angles once it is triggered (unkBC). */
void rocket_door1_update(struct Moby *m) __asm__("FUN_L02_002df948");

void rocket_door1_update(struct Moby *moby) {
    enum {
        ROCKET_DOOR1_INITIALIZE_ROTATION = 0,
        ROCKET_DOOR1_WAIT_FOR_COMMAND = 1,
        ROCKET_DOOR1_TURN_TO_GOAL_ROTATION = 2
    };
    Vec4        qx;
    Vec4        qy;
    Vec4        qz;
    Vec4        qxy;
    Vec4        rxy;
    TurnerVars *door_vars = (TurnerVars *)moby->pvars;

    if (level_game_mode == 2 && moby->command == 0) {
        moby->visible = 0;
        moby->flags |= 1;
    } else {
        moby->visible = 1;
        moby->flags &= ~1;
    }
    switch (moby->state) {
    case ROCKET_DOOR1_INITIALIZE_ROTATION:
        qcopy(&door_vars->start, &moby->rot);
        door_vars->start.f[3] = 0.0f;
        if (D_0014C050[D_0015ED84][moby->mission] == 0xFF) {
            moby->rot.x = door_vars->goal.f[0] * 0.017453292f;
            moby->rot.y = door_vars->goal.f[1] * 0.017453292f;
            moby->rot.z = door_vars->goal.f[2] * 0.017453292f;
            moby->state = 3;
        } else {
            moby->state = 1;
            moby->flags |= 0x100;
            FUN_L00_001fff28(&qx, 0, door_vars->goal.f[0] * 0.017453292f);
            FUN_L00_001fff28(&qy, 1, door_vars->goal.f[1] * 0.017453292f);
            FUN_L00_001fff28(&qz, 2, door_vars->goal.f[2] * 0.017453292f);
            multiply_quaternions(&qxy, &qx, &qy);
            multiply_quaternions(&door_vars->goal, &qxy, &qz);
            FUN_L00_001fff28(&qx, 0, moby->rot.x);
            FUN_L00_001fff28(&qy, 1, moby->rot.y);
            FUN_L00_001fff28(&qz, 2, moby->rot.z);
            multiply_quaternions(&rxy, &qx, &qy);
            multiply_quaternions(&door_vars->start, &rxy, &qz);
        }
        break;
    case ROCKET_DOOR1_WAIT_FOR_COMMAND:
        if (moby->command != 0) {
            moby->state = 2;
            door_vars->speed = 0.0f;
            door_vars->t = 0.0f;
        }
        break;
    case ROCKET_DOOR1_TURN_TO_GOAL_ROTATION:
        approach_angle(&door_vars->t,
                       &door_vars->speed,
                       1.0f,
                       frame_time_sq_macro * 0.5f,
                       frame_time_sq_macro * 0.5f,
                       D_0015ED6C);
        quaternion_nlerp(&qx, &door_vars->start, &door_vars->goal, door_vars->t);
        quaternion_to_rotation_basis(&qx, &moby->rot_row0);
        break;
    }
}
