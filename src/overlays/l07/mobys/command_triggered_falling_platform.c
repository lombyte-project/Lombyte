/* Level 07 moby class 0x42d: command triggered falling platform. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "qcopy.h"
#include "rnc/overlay/level_labels.h"

extern float random_angle_radians(void) __asm__("FUN_00213308");

/* steps a hit-effect emitter: orient, spawn a puff, and reseed the timer */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_00313690), where it is exact; names translated to the US level program. */
extern int   FUN_0022da68(int, int, int);
extern void  update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void  mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern f32   random_float_between(f32, f32) __asm__("FUN_002132a8");
void         FUN_L01_0026e090(int i, int v);
void         mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern int   rand(void) __asm__("FUN_001160d8");
extern s32   rand(void);
extern float step_angle_toward_target(float *, float, int, float) __asm__("FUN_L00_0025bc00");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern void
reorient_position_about_pivot(void *, void *, void *, void *, void *) __asm__("FUN_L07_00288968");
void update_command_triggered_falling_platform(char *moby) __asm__("FUN_L07_003112c8");

void update_command_triggered_falling_platform(char *moby) {
    enum {
        PLATFORM_INITIALIZE = 0,
        PLATFORM_COMMAND_GATED_SWAY = 1,
        PLATFORM_TIMED_SWAY_BEFORE_FALL = 2,
        PLATFORM_TILT_AND_FALL = 3,
        PLATFORM_PARENT_GATED_RECOVERY_SETUP = 4,
        PLATFORM_HEIGHT_RECOVERY_AND_SWAY = 5
    };
    char *data = ((char *)MOBY(moby)->pvars);
    char *parent = 0;
    char *parent_data = 0;
    float previous[4];
    float delta[4];
    float rotation[4];
    float scratch[4];
    float current_rotation[4];
    float current_position[4];
    float transformed[4];
    float target;
    float height;
    float sway;
    float roll;
    float speed;
    int   timer;

    qcopy(previous, moby + 0x10);
    fast_vec_scale(delta, moby + 0x10, -1.0f);
    qcopy(rotation, moby + 0x40);
    if (*(int *)(data + 0xA0) != -1) {
        parent = level_mobys + *(int *)(data + 0xA0) * 0x100;
        parent_data = *(char **)(parent + 0x78);
    }
    if (((unsigned char *)moby)[0xBC] == 2) {
        moby[0x20] = 2;
        moby[0xBC] = 3;
        *(int *)(data + 0xAC) = ((unsigned char *)data)[0xBD];
    }
    switch (((unsigned char *)moby)[0x20]) {
    case PLATFORM_INITIALIZE:
        *(int *)(data + 0x20) = 0;
        *(short *)(data + 0x24) = 0;
        data[0x28] = 4;
        *(short *)(data + 0x3E) = 13;
        timer = rand();
        if (timer & 1)
            MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, 3.1415927f);
        *(float *)(data + 0xA4) = MOBY(moby)->rot.x;
        *(float *)(data + 0xA8) = MOBY(moby)->rot.y;
        *(int *)(data + 0xC8) = -1;
        moby[0x20] = 1;
        moby[0xBC] = 0;
        if (((unsigned char *)moby)[0xB0] != 0xFF) {
            if (visited52[((unsigned char *)moby)[0xB0] + current_level_index * 16] != 0xFF)
                break;
            MOBY(moby)->pos.z -= 5.0f;
            if (((unsigned char *)data)[0xBC] == 0) {
                mark_moby_for_removal(moby);
                return;
            }
            moby[0x20] = 5;
        }
        break;
    case PLATFORM_COMMAND_GATED_SWAY:
        if (((unsigned char *)moby)[0xBC] != 1)
            break;
        if (*(int *)(data + 0xAC) == 0)
            *(int *)(data + 0xAC) = rand() % 10;
        if (((unsigned char *)data)[0xBE] != 0)
            FUN_L01_0026e090(((unsigned char *)moby)[0x21], 2);
        MOBY(moby)->rot.x = fast_add_rotations(
            *(float *)(data + 0xA4),
            fast_sin(normalize_angle(convert_integer_to_float(*(int *)(data + 0xAC)) * 0.75f)) *
                0.017453292f);
        MOBY(moby)->rot.y = fast_add_rotations(
            *(float *)(data + 0xA8),
            fast_cos(normalize_angle(convert_integer_to_float(*(int *)(data + 0xAC)) * 0.89f)) *
                0.017453292f);
        break;
    case PLATFORM_TIMED_SWAY_BEFORE_FALL:
        MOBY(moby)->rot.x = fast_add_rotations(
            *(float *)(data + 0xA4),
            fast_sin(normalize_angle(convert_integer_to_float(*(int *)(data + 0xAC)) * 0.75f)) *
                0.017453292f);
        MOBY(moby)->rot.y = fast_add_rotations(
            *(float *)(data + 0xA8),
            fast_cos(normalize_angle(convert_integer_to_float(*(int *)(data + 0xAC)) * 0.89f)) *
                0.017453292f);
        if (!fast_dec_timer((int *)(data + 0xAC)))
            break;
        if (((unsigned char *)data)[0xBE] != 0) {
            parent_data[0x19D] = data[0xBF];
            parent[0x20] = 12;
        }
        moby[0x20] = 3;
        break;
    case PLATFORM_TILT_AND_FALL:
        speed = 1.3962634f;
        speed -= MOBY(moby)->rot.x;
        speed *= 0.05f;
        MOBY(moby)->rot.x += speed;
        {
            /* Retail takes the sine of the rotation step. */
            float descent = fast_sin(speed) * 3.1f;
            float gravity = frame_time_sq;
            float drop = 15.0f;
            MOBY(moby)->pos.z -= descent;
            *(float *)(data + 0xB0) -= *(float *)(data + 0xB4) * gravity;
            MOBY(moby)->pos.z += *(float *)(data + 0xB0);
            if (!(MOBY(moby)->pos.z < *(float *)(data + 0xB8) - drop))
                break;
            if (((unsigned char *)data)[0xBC] != 0) {
                *(float *)(data + 0xB0) = 0.0f;
                moby[0x20] = 4;
            } else {
                mark_moby_for_removal(moby);
                return;
            }
        }
        break;
    case PLATFORM_PARENT_GATED_RECOVERY_SETUP:
        if (((unsigned char *)parent_data)[0xEF] < 2 && ((unsigned char *)parent)[0x20] > 11)
            break;
        *(float *)(data + 0xB0) = random_float_between(frame_time, frame_time * 4.0f);
        *(float *)(data + 0xC0) = random_angle_radians();
        *(float *)(data + 0xC4) = random_angle_radians();
        moby[0x20] = 5;
        MOBY(moby)->rot.x = 0.0f;
        MOBY(moby)->rot.y = 0.0f;
        *(int *)(data + 0xAC) = 0;
        break;
    case PLATFORM_HEIGHT_RECOVERY_AND_SWAY:
        target = MOBY(moby)->pos.z + *(float *)(data + 0xB0);
        height = *(float *)(data + 0xB8);
        if (hero.air_frames.s == 0 && (char *)hero.ground_moby == moby) {
            if (*(int *)(data + 0xAC) == 0)
                FUN_0022da68(0, 0, (int)moby);
            timer = scale_game_frames(90);
            if (*(int *)(data + 0xAC) < timer)
                *(int *)(data + 0xAC) = scale_game_frames(90);
        }
        if (*(int *)(data + 0xAC) != 0) {
            if (MOBY(moby)->pos.z > *(float *)(data + 0xB8) - 2.6f) {
                *(float *)(data + 0xB0) = -(frame_time * 0.25f);
            } else {
                *(float *)(data + 0xB0) = 0.0f;
            }
        } else {
            if (target < height) {
                float correction = (height - target) * 0.2f;
                if (correction > 0.9f)
                    correction = 0.9f;
                else if (correction < -0.9f)
                    correction = -0.9f;
                *(float *)(data + 0xB0) += correction;
            }
            {
                float velocity = *(float *)(data + 0xB0) - frame_time_sq * 9.8f;
                float cap = frame_time * 5.0f;
                *(float *)(data + 0xB0) = velocity;
                if (cap < velocity)
                    *(float *)(data + 0xB0) = cap;
            }
        }
        MOBY(moby)->pos.z += *(float *)(data + 0xB0);
        *(float *)(data + 0xC0) = fast_add_rotations(*(float *)(data + 0xC0), 0.01f);
        *(float *)(data + 0xC4) = fast_add_rotations(*(float *)(data + 0xC0), 0.017f);
        sway = fast_sin(*(float *)(data + 0xC0)) * 0.02f;
        roll = fast_sin(*(float *)(data + 0xC4)) * 0.02f;
        if (*(int *)(data + 0xAC) != 0) {
            float forward, side;
            fast_dec_timer((int *)(data + 0xAC));
            fast_vec_sub(scratch, &hero.motion.pos, moby + 0x10);
            forward = fast_vec_dot(scratch, moby + 0xC0);
            side = fast_vec_dot(scratch, moby + 0xD0) * -0.03f;
            forward *= 0.03f;
            sway = fast_add_rotations(sway, side);
            sway = fast_add_rotations(sway, forward);
            *(u128 *)current_rotation = *(u128 *)(moby + 0x40);
            *(u128 *)current_position = *(u128 *)(moby + 0x10);
            reorient_position_about_pivot(
                &hero.motion.pos, transformed, moby + 0x10, current_rotation, moby + 0x40);
            fast_vec_sub(&hero.motion.platform_delta, transformed, &hero.motion.pos);
            hero.motion.platform_delta.f[2] += MOBY(moby)->pos.z - current_position[2];
        }
        step_angle_toward_target((float *)&MOBY(moby)->rot, sway, 0, frame_time * 0.17453292f);
        step_angle_toward_target((float *)&MOBY(moby)->rot.y, roll, 0, frame_time * 0.17453292f);
        if (MOBY(moby)->rot.x > 0.5235988f)
            MOBY(moby)->rot.x = 0.5235988f;
        if (MOBY(moby)->rot.x < -0.5235988f)
            MOBY(moby)->rot.x = -0.5235988f;
        if (MOBY(moby)->rot.y > 0.5235988f)
            MOBY(moby)->rot.y = 0.5235988f;
        if (MOBY(moby)->rot.y < -0.5235988f)
            MOBY(moby)->rot.y = -0.5235988f;
        break;
    }
    if (((unsigned char *)moby)[0x20] == 5 && fast_dec_timer((int *)(data + 0xC8))) {
        float last = *(float *)(data + 0xCC);
        float height = *(float *)(data + 0xB8);
        if ((height <= last && MOBY(moby)->pos.z < height) ||
            (last <= height && height < MOBY(moby)->pos.z)) {
            FUN_0022da68(0, 0, (int)moby);
            *(int *)(data + 0xC8) = rand() % scale_game_frames(20) + scale_game_frames(20);
        }
    }
    *(float *)(data + 0xCC) = MOBY(moby)->pos.z;
    fast_vec_add(delta, delta, moby + 0x10);
    update_entity_matrix(data + 0x60, delta, rotation, moby + 0x40);
}
