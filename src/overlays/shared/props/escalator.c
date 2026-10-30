/* Shared moby class escalator (and escalator_end, conveyor_belt). */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "qcopy.h"

extern int   escalator_direction_timer_frames_gp __asm__("D_L02_00161C18") __attribute__((sda));
extern float escalator_target_speed_gp __asm__("D_L02_00161C14") __attribute__((sda));
extern float escalator_acceleration_gp __asm__("D_L02_00161C1C") __attribute__((sda));

void escalator_update(char *moby) __asm__("FUN_L02_002dc6b0");

void escalator_update(char *moby) {
    char *data = ((char *)MOBY(moby)->pvars);
    float limit;
    float y;
    int   state;

    if (data == 0)
        return;
    state = MOBY(moby)->state;
    if (state == 1)
        goto state1;
    if (state < 2) {
        if (state == 0)
            goto state0;
        goto move;
    }
    if (state == 2)
        goto state2;
    if (state == 3)
        goto state3;
    goto move;
state0:
    clear_u64_value(data + 0x40);
    *(int *)(data + 0xa0) = scale_game_frames(escalator_direction_timer_frames_gp);
    *(u32 *)(data + 0x9c) |= 4;
    if (*(int *)(data + 0xa4) != 0)
        MOBY(moby)->state = 3;
    else
        MOBY(moby)->state = 1;
    goto move;
state1: {
    float speed;

    MOBY(moby)->anim_speed = 1.0f;
    if (fast_dec_timer((int *)(data + 0xa0))) {
        speed = fast_vec_length(data + 0x40) - escalator_acceleration_gp * frame_time_sq;
        if (speed <= 0.0f) {
            *(int *)(data + 0xa0) = scale_game_frames(escalator_direction_timer_frames_gp);
            MOBY(moby)->state = 2;
        } else {
            *(float *)(data + 0x40) = fast_cos(MOBY(moby)->rot.z) * -speed;
            y = fast_sin(MOBY(moby)->rot.z) * -speed;
            *(float *)(data + 0x48) = 0.0f;
            *(float *)(data + 0x44) = y;
        }
    } else {
        speed = fast_vec_length(data + 0x40);
        limit = absolute_float(escalator_target_speed_gp * frame_time);
        /* Retail loads frame_time_sq through its absolute EE address here. */
        if (speed < limit)
            speed += escalator_acceleration_gp * (*(float *)0x0015ED70);
        *(float *)(data + 0x40) = fast_cos(MOBY(moby)->rot.z) * -speed;
        *(float *)(data + 0x44) = fast_sin(MOBY(moby)->rot.z) * -speed;
        *(int *)(data + 0x48) = 0;
    }
    MOBY(moby)->anim_speed = speed / (escalator_target_speed_gp * frame_time);
    goto move;
}
state2: {
    float speed;

    MOBY(moby)->anim_speed = -1.0f;
    if (fast_dec_timer((int *)(data + 0xa0))) {
        speed = fast_vec_length(data + 0x40) - escalator_acceleration_gp * frame_time_sq;
        if (speed <= 0.0f) {
            *(int *)(data + 0xa0) = scale_game_frames(escalator_direction_timer_frames_gp);
            MOBY(moby)->state = 1;
        } else {
            *(float *)(data + 0x40) = fast_cos(MOBY(moby)->rot.z) * speed;
            *(float *)(data + 0x44) = fast_sin(MOBY(moby)->rot.z) * speed;
            *(float *)(data + 0x48) = 0.0f;
        }
    } else {
        speed = fast_vec_length(data + 0x40);
        limit = absolute_float(escalator_target_speed_gp * frame_time);
        if (speed < limit)
            speed += escalator_acceleration_gp * (*(float *)0x0015ED70);
        *(float *)(data + 0x40) = fast_cos(MOBY(moby)->rot.z) * speed;
        *(float *)(data + 0x44) = fast_sin(MOBY(moby)->rot.z) * speed;
        *(int *)(data + 0x48) = 0;
    }
    MOBY(moby)->anim_speed = speed / -(escalator_target_speed_gp * frame_time);
    goto move;
}
state3:
    *(float *)(data + 0x40) =
        fast_cos(MOBY(moby)->rot.z) * -(escalator_target_speed_gp * frame_time);
    *(float *)(data + 0x44) =
        fast_sin(MOBY(moby)->rot.z) * -(escalator_target_speed_gp * frame_time);
    *(int *)(data + 0x48) = 0;
move:
    update_entity_matrix(data + 0x60, data + 0x40, moby + 0x40, moby + 0x40);
}

int   D_L12_00161964 = 0xb4;
float D_L12_00161960 = -2.5f;
float D_L12_00161968 = 2.5f;

int escalator_direction_timer_frames_gp __asm__("D_L02_00161C18") = 0xb4;
