/* Level 05 moby class that moves and turns a manipulator when triggered. */
#include "types.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/mobys/attach_manipulator.h"

extern int   allocate_voice_for_target_entry() __asm__("FUN_0022da68");
extern short D_L05_0015FFD8 __asm__("D_L05_0015FFD8");
extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern float
approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");

extern void  build_axis_rotation_quaternion(void *, int, float) __asm__("FUN_L00_001fff28");
extern char *D_001413D0_lui __asm__("D_001413D0") NOT_SDA;

void triggered_motion_update(char *moby) __asm__("FUN_L05_00316258");

typedef struct TriggeredMotionVars {
    u8  pad_0[0x10];
    u8  manipulator_10;
    u8  pad_11[0xF];
    u8  manipulator_quat;
    u8  pad_21[0x2F];
    s32 upside_down;  /* nonzero: rot.x = pi and raised by 4 */
    s32 starts_open;  /* nonzero: starts shifted and waits to return */
    s32 trigger_moby; /* moby index whose command byte triggers it */
    f32 move_speed;
    f32 manipulator_angle;
    f32 manipulator_angular_velocity;
} TriggeredMotionVars;

/* Moby class 893 */
void triggered_motion_update(char *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_WAIT_FOR_TRIGGER = 1,
        MOBY_WAIT_TO_RETURN_TO_SAVED_POSITION = 2,
        MOBY_MOVE_TO_OFFSET_POSITION = 3,
        MOBY_RETURN_TO_SAVED_POSITION = 4,
        MOBY_SETTLE_AT_UPPER_MANIPULATOR_ANGLE = 5,
        MOBY_SETTLE_AT_ZERO_MANIPULATOR_ANGLE = 6
    };
    enum {
        MOBY_MOTION_SEQUENCE_COMPLETE = 7
    };
    TriggeredMotionVars *motion_and_manipulator_vars = (TriggeredMotionVars *)MOBY(moby)->pvars;
    float                target[4];
    float                delta[4];
    float                stopped;

    switch (MOBY(moby)->state) {
    case MOBY_INITIALIZE:
        if (motion_and_manipulator_vars->upside_down != 0) {
            MOBY(moby)->rot.x = 3.1415927f;
            MOBY(moby)->pos.z += 4.0f;
        }
        qcopy(motion_and_manipulator_vars, moby + 0x10);
        attach_manipulator(moby, 0, &motion_and_manipulator_vars->manipulator_10);
        if (motion_and_manipulator_vars->starts_open != 0) {
            motion_and_manipulator_vars->manipulator_angle = 0.0f;
            MOBY(moby)->state = MOBY_WAIT_TO_RETURN_TO_SAVED_POSITION;
            if (motion_and_manipulator_vars->upside_down != 0)
                fast_vec_scale(target, moby + 0xD0, 2.0f);
            else
                fast_vec_scale(target, moby + 0xD0, -2.0f);
            fast_vec_add(moby + 0x10, moby + 0x10, target);
        } else {
            MOBY(moby)->state = MOBY_WAIT_FOR_TRIGGER;
            motion_and_manipulator_vars->manipulator_angle = 0.5934119f;
            build_axis_rotation_quaternion(
                &motion_and_manipulator_vars->manipulator_quat, 0, 0.5934119f);
        }
        *(long long *)(moby + 0x38) = *(long long *)(D_001413D0_lui + 0x38);
        break;
    case MOBY_WAIT_FOR_TRIGGER:
    case MOBY_WAIT_TO_RETURN_TO_SAVED_POSITION: {
        u8 trigger =
            ((u8 *)*(int *)&D_L05_0015FFD8)[motion_and_manipulator_vars->trigger_moby * 256 + 0xBC];
        if (trigger == 0)
            return;
        if (MOBY(moby)->state == MOBY_WAIT_TO_RETURN_TO_SAVED_POSITION && trigger != 2) {
            MOBY(moby)->state = MOBY_RETURN_TO_SAVED_POSITION;
            break;
        }
        if (MOBY(moby)->state != MOBY_WAIT_FOR_TRIGGER)
            return;
        allocate_voice_for_target_entry(0, 0, moby);
        MOBY(moby)->state = MOBY_SETTLE_AT_ZERO_MANIPULATOR_ANGLE;
        return;
    }
    case MOBY_SETTLE_AT_UPPER_MANIPULATOR_ANGLE:
    case MOBY_SETTLE_AT_ZERO_MANIPULATOR_ANGLE: {
        float end;
        float approach;
        if (MOBY(moby)->state == MOBY_SETTLE_AT_UPPER_MANIPULATOR_ANGLE) {
            end = 0.5934119f;
            approach = 1.1868238f;
        } else {
            end = 0.0f;
            approach = -0.5934119f;
        }
        approach_angle(&motion_and_manipulator_vars->manipulator_angle,
                       &motion_and_manipulator_vars->manipulator_angular_velocity,
                       approach,
                       frame_time_sq * 4.1887903f,
                       frame_time_sq * 4.1887903f,
                       frame_time * 12.566371f);
        if (motion_and_manipulator_vars->manipulator_angle > 0.5934119f ||
            motion_and_manipulator_vars->manipulator_angle < 0.0f) {
            if (motion_and_manipulator_vars->manipulator_angle > 0.5934119f)
                motion_and_manipulator_vars->manipulator_angle = 0.5934119f;
            else if (motion_and_manipulator_vars->manipulator_angle < 0.0f)
                motion_and_manipulator_vars->manipulator_angle = 0.0f;
            if (MOBY(moby)->command == 0) {
                MOBY(moby)->command = 1;
                motion_and_manipulator_vars->manipulator_angular_velocity *= -0.33f;
            } else if (motion_and_manipulator_vars->manipulator_angle == end) {
                if (MOBY(moby)->state == MOBY_SETTLE_AT_UPPER_MANIPULATOR_ANGLE)
                    MOBY(moby)->state = MOBY_MOTION_SEQUENCE_COMPLETE;
                else
                    MOBY(moby)->state = MOBY_MOVE_TO_OFFSET_POSITION;
            }
        }
        build_axis_rotation_quaternion(&motion_and_manipulator_vars->manipulator_quat,
                                       0,
                                       motion_and_manipulator_vars->manipulator_angle);
    } break;
    case MOBY_MOVE_TO_OFFSET_POSITION:
    case MOBY_RETURN_TO_SAVED_POSITION:
        if (MOBY(moby)->state == MOBY_MOVE_TO_OFFSET_POSITION) {
            fast_vec_scale(target, moby + 0xD0, -1.9f);
            fast_vec_add(target, motion_and_manipulator_vars, target);
        } else {
            qcopy(target, motion_and_manipulator_vars);
        }
        fast_vec_sub(delta, target, moby + 0x10);
        stopped = 0.0f;
        advance_accelerated_scalar(fast_vec_length(delta),
                                   frame_time_sq * 6.0f,
                                   frame_time_sq * 12.0f,
                                   frame_time * 10.0f,
                                   &stopped,
                                   &motion_and_manipulator_vars->move_speed);
        fast_vec_sub(delta, target, moby + 0x10);
        fast_vec_normalize(delta, delta, motion_and_manipulator_vars->move_speed);
        fast_vec_add(moby + 0x10, moby + 0x10, delta);
        if (!(fast_vec_dist(moby + 0x10, target) < 0.0001f))
            return;
        if (MOBY(moby)->state == MOBY_MOVE_TO_OFFSET_POSITION) {
            allocate_voice_for_target_entry(1, 0, moby);
            MOBY(moby)->state = MOBY_MOTION_SEQUENCE_COMPLETE;
            return;
        }
        MOBY(moby)->state = MOBY_SETTLE_AT_UPPER_MANIPULATOR_ANGLE;
        break;
    }
}
