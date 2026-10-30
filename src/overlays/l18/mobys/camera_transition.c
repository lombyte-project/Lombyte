/* Level 18 moby class camera_transition (0x284): moves the camera from one position and rotation to another for Drek's boss robot. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/game/usage_stats.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/level_spline.h"

#include "rnc/overlay/l18/mobys/camera_transition_globals.h"

#include "sda.h"

extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern float
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern int help_request(int, int) __asm__("FUN_L00_00203908");

extern float fast_add_rotations(float, float);
/* Ramps up, then interpolates the camera between the two placements, then ramps down. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002E09F8), where it is exact; names translated to the US level program. */

extern char  camera_secondary_mode_lui[] __asm__("D_001413D4") NOT_SDA;
extern float D_L18_0015F3FC_d __asm__("D_L18_0015F3FC");
extern int   help_request(int a, int b) __asm__("FUN_L00_00203908");
extern void  FUN_L00_00216f90_q(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void  FUN_L00_002ea9d8_q(void *) __asm__("FUN_L00_002ea9d8");
extern void  set_camera_rotation(void *) __asm__("FUN_L00_002eaa30");
extern void  FUN_L00_002eaaa0_q(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void  FUN_L00_002eac18_q(int) __asm__("FUN_L00_002eac18");

extern int                D_L18_00179E10[] NOT_SDA;
extern int D_L18_0017EF08 NOT_SDA;
void                      update_camera_transition(struct Moby *moby) __asm__("FUN_L18_002df608");

void update_camera_transition(struct Moby *moby) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_PRE_TRANSITION_RAMP_UP = 2,
        STATE_INTERPOLATE_TRANSITION = 3,
        STATE_INTERPOLATE_TRANSITION_WITH_RAMP_UP = 4,
        STATE_POST_TRANSITION_RAMP_DOWN = 5
    };
    char *data = (char *)moby->pvars;
    float transition_scratch_vectors[12];
    switch (moby->state) {
    case STATE_INITIALIZE:
        moby->state = 1;
        moby->update_dist = 0xFF;
        break;
    case STATE_PRE_TRANSITION_RAMP_UP:
        D_L18_0017EF08 = 1;
        approach_value((float *)(data + 0x6C), 1.0f, (*(float *)&frame_time) * 4.0f);
        {
            float t = *(float *)(data + 0x6C);
            if (D_L18_0015F3FC_d < t) {
                D_L18_0015F3FC_d = t;
            }
            if (1.0f <= t) {
                camera_transition_active = 1;
                clear_u64_value(transition_scratch_vectors);
                transition_scratch_vectors[2] = *(float *)(data + 0x4C);
                FUN_L00_00216f90_q(data + 0x40, transition_scratch_vectors, 0x72, 1);
                FUN_L00_002eaaa0_q(data, data + 0x20, 1, 0, 0);
                if (*(int *)(data + 0x78) == 1) {
                    help_request(0x4650, 0x85);
                    moby->state = 4;
                } else {
                    moby->state = 3;
                }
            }
        }
        break;
    case STATE_INTERPOLATE_TRANSITION:
        D_L18_0017EF08 = 1;
        if (fast_dec_timer((int *)(data + 0x74))) {
            advance_accelerated_scalar((float *)(data + 0x60),
                                       (float *)(data + 0x64),
                                       1.0f,
                                       *(float *)(data + 0x68),
                                       *(float *)(data + 0x68),
                                       *(float *)(data + 0x70));
        }
        lerp_vector(transition_scratch_vectors, data, data + 0x10, *(float *)(data + 0x60));
        clear_u64_value(transition_scratch_vectors + 4);
        transition_scratch_vectors[5] =
            fast_subtract_rotations(*(float *)(data + 0x34), *(float *)(data + 0x24)) *
            *(float *)(data + 0x60);
        transition_scratch_vectors[5] =
            fast_add_rotations(*(float *)(data + 0x24), transition_scratch_vectors[5]);
        transition_scratch_vectors[6] =
            fast_subtract_rotations(*(float *)(data + 0x38), *(float *)(data + 0x28)) *
            *(float *)(data + 0x60);
        transition_scratch_vectors[6] =
            fast_add_rotations(*(float *)(data + 0x28), transition_scratch_vectors[6]);
        FUN_L00_002ea9d8_q(transition_scratch_vectors);
        set_camera_rotation(transition_scratch_vectors + 4);
        approach_value((float *)(data + 0x6C), 0.0f, (*(float *)&frame_time) * 4.0f);
        {
            float transition_progress = *(float *)(data + 0x60);
            D_L18_0015F3FC_d = *(float *)(data + 0x6C);
            if (1.0f <= transition_progress) {
                camera_transition_active = 0;
                clear_u64_value(transition_scratch_vectors + 8);
                transition_scratch_vectors[10] = *(float *)(data + 0x4C);
                FUN_L00_00216f90_q(data + 0x40, transition_scratch_vectors + 8, 0, 0);
                FUN_L00_002eac18_q(2);
                moby->state = 5;
            } else if (*(int *)(camera_secondary_mode_lui) != 0x72) {
                clear_u64_value(transition_scratch_vectors + 8);
                transition_scratch_vectors[10] = *(float *)(data + 0x4C);
                FUN_L00_00216f90_q(data + 0x40, transition_scratch_vectors + 8, 0x72, 1);
                return;
            }
        }
        break;
    case STATE_INTERPOLATE_TRANSITION_WITH_RAMP_UP:
        if (fast_dec_timer((int *)(data + 0x74))) {
            advance_accelerated_scalar((float *)(data + 0x60),
                                       (float *)(data + 0x64),
                                       1.0f,
                                       *(float *)(data + 0x68),
                                       *(float *)(data + 0x68),
                                       *(float *)(data + 0x70));
        }
        lerp_vector(transition_scratch_vectors, data, data + 0x10, *(float *)(data + 0x60));
        clear_u64_value(transition_scratch_vectors + 4);
        transition_scratch_vectors[5] =
            fast_subtract_rotations(*(float *)(data + 0x34), *(float *)(data + 0x24)) *
            *(float *)(data + 0x60);
        transition_scratch_vectors[5] =
            fast_add_rotations(*(float *)(data + 0x24), transition_scratch_vectors[5]);
        transition_scratch_vectors[6] =
            fast_subtract_rotations(*(float *)(data + 0x38), *(float *)(data + 0x28)) *
            *(float *)(data + 0x60);
        transition_scratch_vectors[6] =
            fast_add_rotations(*(float *)(data + 0x28), transition_scratch_vectors[6]);
        FUN_L00_002ea9d8_q(transition_scratch_vectors);
        set_camera_rotation(transition_scratch_vectors + 4);
        if (1.0f <= *(float *)(data + 0x60) ||
            (0.2f < *(float *)(data + 0x60) && D_L18_00179E10[0] == 0 && D_L18_00179E10[9] == -1)) {
            float t;
            approach_value((float *)(data + 0x6C), 1.0f, (*(float *)&frame_time) * 4.0f);
            t = *(float *)(data + 0x6C);
            D_L18_0015F3FC_d = t;
            if (1.0f <= t) {
                camera_transition_active = 0;
                *(float *)(data + 0x60) = 1.0f;
                clear_u64_value(transition_scratch_vectors + 8);
                transition_scratch_vectors[10] = *(float *)(data + 0x4C);
                FUN_L00_00216f90_q(data + 0x40, transition_scratch_vectors + 8, 0, 0);
                FUN_L00_002eac18_q(3);
                moby->state = 5;
            }
        } else if (*(int *)(camera_secondary_mode_lui) != 0x72) {
            clear_u64_value(transition_scratch_vectors + 8);
            transition_scratch_vectors[10] = *(float *)(data + 0x4C);
            FUN_L00_00216f90_q(data + 0x40, transition_scratch_vectors + 8, 0x72, 1);
            return;
        } else {
            approach_value((float *)(data + 0x6C), 0.0f, (*(float *)&frame_time) * 4.0f);
            D_L18_0015F3FC_d = *(float *)(data + 0x6C);
        }
        break;
    case STATE_POST_TRANSITION_RAMP_DOWN:
        if (0.0f < *(float *)(data + 0x6C)) {
            approach_value((float *)(data + 0x6C), 0.0f, (*(float *)&frame_time) * 4.0f);
            D_L18_0015F3FC_d = *(float *)(data + 0x6C);
        } else {
            moby->state = 1;
        }
        break;
    }
}

#include "qcopy.h"

extern int scale_game_frames(int);

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002E0E90), where it is exact; names translated to the US level program. */

void start_camera_transition(struct Moby *moby,
                             void        *transition_start_position,
                             void        *transition_target_position,
                             void        *transition_start_rotation,
                             void        *transition_target_rotation,
                             void        *transition_setup_vector,
                             int          invulnerability_duration_frames,
                             int          transition_delay_frames,
                             int          transition_mode) __asm__("FUN_L18_002dfaa0");

void start_camera_transition(struct Moby *moby,
                             void        *transition_start_position,
                             void        *transition_target_position,
                             void        *transition_start_rotation,
                             void        *transition_target_rotation,
                             void        *transition_setup_vector,
                             int          invulnerability_duration_frames,
                             int          transition_delay_frames,

                             int transition_mode) {
    char *data = (char *)moby->pvars;
    *(int *)(data + 0x60) = 0;
    *(int *)(data + 0x64) = 0;
    qcopy(data, transition_start_position);
    qcopy(data + 0x10, transition_target_position);
    qcopy(data + 0x20, transition_start_rotation);
    qcopy(data + 0x30, transition_target_rotation);
    qcopy(data + 0x40, transition_setup_vector);
    moby->state = 2;
    *(float *)(data + 0x68) = frame_time_sq * 0.333f;
    *(float *)(data + 0x70) = 1.0f / (float)scale_game_frames(invulnerability_duration_frames);
    *(int *)(data + 0x74) = scale_game_frames(transition_delay_frames);
    *(int *)(data + 0x78) = transition_mode;
    hero.invulnerable_frames = scale_game_frames(invulnerability_duration_frames);
}

/* Read the vendor moby's current float value. */
/* Ported from rac1-decomp (src/overlays/l18/vendor_002A8400.c: func_L18_002E0F80), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x60];
    float value;
} Level18VendorFloatData;

typedef struct {
    char                    pad0[0x78];
    Level18VendorFloatData *data;
} Level18VendorFloatMoby;

float get_camera_transition_progress(Level18VendorFloatMoby *moby) __asm__("FUN_L18_002dfb90");

float get_camera_transition_progress(Level18VendorFloatMoby *moby) {
    return moby->data->value;
}

/* Maps states 2 and 3 to results 1 and 2; all other states return 0. */
s32 classify_moby_state_two_or_three(const u8 *moby) __asm__("FUN_L18_002dfba0");

s32 classify_moby_state_two_or_three(const u8 *moby) {
    u32 state = moby[0x20];
    s32 result = 2;

    if (state == 2) {
        return 1;
    }
    if (state != 3) {
        result = 0;
    }
    return result;
}
