/* Shared moby class escalator (and escalator_end, conveyor_belt). */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "qcopy.h"

typedef struct EscalatorVars {
    u8  pad0[0x40];
    f32 belt_velocity_x;
    f32 belt_velocity_y;
    s32 belt_velocity_z;
    u8  pad4C[0x14];
    u8  entity_matrix[0x3c];
    u32 entity_matrix_flags;
    s32 direction_timer;
    s32 constant_speed;
} EscalatorVars;

extern int escalator_direction_timer_frames_gp __asm__("D_L02_00161C18") __attribute__((sda));
float      escalator_target_speed_gp __asm__("D_L02_00161C14") = -2.5f;
float      escalator_acceleration_gp __asm__("D_L02_00161C1C") = 2.5f;

void escalator_update(char *moby) __asm__("FUN_L02_002dc6b0");

/* Moby classes 296, 652, 653 */
void escalator_update(char *moby) {
    enum {
        ESCALATOR_STATE_INITIALIZE = 0,
        ESCALATOR_STATE_TIMED_NEGATIVE_YAW_MOTION = 1,
        ESCALATOR_STATE_TIMED_POSITIVE_YAW_MOTION = 2,
        ESCALATOR_STATE_CONSTANT_SPEED_MOTION = 3
    };
    EscalatorVars *escalator_motion_state = (EscalatorVars *)MOBY(moby)->pvars;
    float          limit;
    float          y;
    int            state;

    if (escalator_motion_state == NULL)
        return;
    state = MOBY(moby)->state;
    if (state == ESCALATOR_STATE_TIMED_NEGATIVE_YAW_MOTION)
        goto state1;
    if (state < ESCALATOR_STATE_TIMED_POSITIVE_YAW_MOTION) {
        if (state == ESCALATOR_STATE_INITIALIZE)
            goto state0;
        goto move;
    }
    if (state == ESCALATOR_STATE_TIMED_POSITIVE_YAW_MOTION)
        goto state2;
    if (state == ESCALATOR_STATE_CONSTANT_SPEED_MOTION)
        goto state3;
    goto move;
state0:
    clear_u64_value(&escalator_motion_state->belt_velocity_x);
    escalator_motion_state->direction_timer =
        scale_game_frames(escalator_direction_timer_frames_gp);
    escalator_motion_state->entity_matrix_flags |= 4;
    if (escalator_motion_state->constant_speed != 0)
        MOBY(moby)->state = ESCALATOR_STATE_CONSTANT_SPEED_MOTION;
    else
        MOBY(moby)->state = ESCALATOR_STATE_TIMED_NEGATIVE_YAW_MOTION;
    goto move;
state1: {
    float speed;

    MOBY(moby)->anim_speed = 1.0f;
    if (fast_dec_timer((int *)(&escalator_motion_state->direction_timer))) {
        speed = fast_vec_length(&escalator_motion_state->belt_velocity_x) -
                escalator_acceleration_gp * frame_time_sq;
        if (speed <= 0.0f) {
            escalator_motion_state->direction_timer =
                scale_game_frames(escalator_direction_timer_frames_gp);
            MOBY(moby)->state = ESCALATOR_STATE_TIMED_POSITIVE_YAW_MOTION;
        } else {
            escalator_motion_state->belt_velocity_x = fast_cos(MOBY(moby)->rot.z) * -speed;
            y = fast_sin(MOBY(moby)->rot.z) * -speed;
            *(float *)&escalator_motion_state->belt_velocity_z = 0.0f;
            escalator_motion_state->belt_velocity_y = y;
        }
    } else {
        speed = fast_vec_length(&escalator_motion_state->belt_velocity_x);
        limit = absolute_float(escalator_target_speed_gp * frame_time);
        /* Retail loads frame_time_sq through its absolute EE address here. */
        if (speed < limit)
            speed += escalator_acceleration_gp * (*(float *)0x0015ED70);
        escalator_motion_state->belt_velocity_x = fast_cos(MOBY(moby)->rot.z) * -speed;
        escalator_motion_state->belt_velocity_y = fast_sin(MOBY(moby)->rot.z) * -speed;
        escalator_motion_state->belt_velocity_z = 0;
    }
    MOBY(moby)->anim_speed = speed / (escalator_target_speed_gp * frame_time);
    goto move;
}
state2: {
    float speed;

    MOBY(moby)->anim_speed = -1.0f;
    if (fast_dec_timer((int *)(&escalator_motion_state->direction_timer))) {
        speed = fast_vec_length(&escalator_motion_state->belt_velocity_x) -
                escalator_acceleration_gp * frame_time_sq;
        if (speed <= 0.0f) {
            escalator_motion_state->direction_timer =
                scale_game_frames(escalator_direction_timer_frames_gp);
            MOBY(moby)->state = ESCALATOR_STATE_TIMED_NEGATIVE_YAW_MOTION;
        } else {
            escalator_motion_state->belt_velocity_x = fast_cos(MOBY(moby)->rot.z) * speed;
            escalator_motion_state->belt_velocity_y = fast_sin(MOBY(moby)->rot.z) * speed;
            *(float *)&escalator_motion_state->belt_velocity_z = 0.0f;
        }
    } else {
        speed = fast_vec_length(&escalator_motion_state->belt_velocity_x);
        limit = absolute_float(escalator_target_speed_gp * frame_time);
        if (speed < limit)
            speed += escalator_acceleration_gp * (*(float *)0x0015ED70);
        escalator_motion_state->belt_velocity_x = fast_cos(MOBY(moby)->rot.z) * speed;
        escalator_motion_state->belt_velocity_y = fast_sin(MOBY(moby)->rot.z) * speed;
        escalator_motion_state->belt_velocity_z = 0;
    }
    MOBY(moby)->anim_speed = speed / -(escalator_target_speed_gp * frame_time);
    goto move;
}
state3:
    escalator_motion_state->belt_velocity_x =
        fast_cos(MOBY(moby)->rot.z) * -(escalator_target_speed_gp * frame_time);
    escalator_motion_state->belt_velocity_y =
        fast_sin(MOBY(moby)->rot.z) * -(escalator_target_speed_gp * frame_time);
    escalator_motion_state->belt_velocity_z = 0;
move:
    update_entity_matrix(&escalator_motion_state->entity_matrix,
                         &escalator_motion_state->belt_velocity_x,
                         moby + 0x40,
                         moby + 0x40);
}

int   D_L12_00161964 = 0xb4;
float D_L12_00161960 = -2.5f;
float D_L12_00161968 = 2.5f;

int escalator_direction_timer_frames_gp __asm__("D_L02_00161C18") = 0xb4;
