/* Level 18 moby class camera_transition (0x284): moves the camera from one position and rotation to another for Drek's boss robot. */
#include "types.h"
#include "rnc/overlay/shared/camera/level_camera.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/shared/mobys/moby_anim.h"
#include "rnc/overlay/shared/mobys/entities.h"
#include "rnc/game/usage_stats.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/overlay/shared/world/level_spline.h"

#include "rnc/overlay/l18/mobys/camera_transition_globals.h"

#include "sda.h"
#include "rnc/math/interpolation/advance_accelerated_scalar.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"

enum {
    CAMERA_TRANSITION_STATE_PRE_TRANSITION_RAMP_UP = 2,
    CAMERA_TRANSITION_STATE_INTERPOLATE_TRANSITION = 3
};

extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
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

extern int                D_L18_00179E10[] NOT_SDA;
extern int D_L18_0017EF08 NOT_SDA;
void boss_camera_transition_update(struct Moby *moby) __asm__("FUN_L18_002df608");

typedef struct BossCameraTransitionVars {
    u8  pad_0[0x10];
    u8  end_position;
    u8  pad_11[0xF];
    u8  start_rotation;
    u8  pad_21[0x3];
    f32 start_rotation_y;
    f32 start_rotation_z;
    u8  pad_2C[0x4];
    f32 end_rotation[4];
    u8  hero_position; /* where FUN_L00_00216f90 places the hero */
    u8  pad_41[0xB];
    f32 hero_yaw;
    u8  pad_50[0x10];
    f32 transition_progress;
    s32 transition_progress_rate;
    f32 progress_acceleration;
    f32 ramp;
    f32 max_progress_rate;
    s32 timer_74;
    s32 transition_mode;
} BossCameraTransitionVars;

/* Moby class 644 */
void boss_camera_transition_update(struct Moby *moby) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_PRE_TRANSITION_RAMP_UP = 2,
        STATE_INTERPOLATE_TRANSITION = 3,
        STATE_INTERPOLATE_TRANSITION_WITH_RAMP_UP = 4,
        STATE_POST_TRANSITION_RAMP_DOWN = 5
    };
    enum {
        STATE_IDLE = 1
    };
    BossCameraTransitionVars *transition_private_vars = (BossCameraTransitionVars *)moby->pvars;
    float                     transition_scratch_vectors[12];
    switch (moby->state) {
    case STATE_INITIALIZE:
        moby->state = STATE_IDLE;
        moby->update_dist = 0xFF;
        break;
    case STATE_PRE_TRANSITION_RAMP_UP:
        D_L18_0017EF08 = 1;
        approach_value(&transition_private_vars->ramp, 1.0f, (*(float *)&frame_time) * 4.0f);
        {
            float t = transition_private_vars->ramp;
            if (D_L18_0015F3FC_d < t) {
                D_L18_0015F3FC_d = t;
            }
            if (1.0f <= t) {
                camera_transition_active = 1;
                clear_u64_value(transition_scratch_vectors);
                transition_scratch_vectors[2] = transition_private_vars->hero_yaw;
                FUN_L00_00216f90_q(
                    &transition_private_vars->hero_position, transition_scratch_vectors, 0x72, 1);
                FUN_L00_002eaaa0_q(
                    transition_private_vars, &transition_private_vars->start_rotation, 1, 0, 0);
                if (transition_private_vars->transition_mode == 1) {
                    help_request(0x4650, 0x85);
                    moby->state = STATE_INTERPOLATE_TRANSITION_WITH_RAMP_UP;
                } else {
                    moby->state = STATE_INTERPOLATE_TRANSITION;
                }
            }
        }
        break;
    case STATE_INTERPOLATE_TRANSITION:
        D_L18_0017EF08 = 1;
        if (fast_dec_timer(&transition_private_vars->timer_74)) {
            advance_accelerated_scalar(&transition_private_vars->transition_progress,
                                       (float *)&transition_private_vars->transition_progress_rate,
                                       1.0f,
                                       transition_private_vars->progress_acceleration,
                                       transition_private_vars->progress_acceleration,
                                       transition_private_vars->max_progress_rate);
        }
        lerp_vector(transition_scratch_vectors,
                    transition_private_vars,
                    &transition_private_vars->end_position,
                    transition_private_vars->transition_progress);
        clear_u64_value(transition_scratch_vectors + 4);
        transition_scratch_vectors[5] =
            fast_subtract_rotations(transition_private_vars->end_rotation[1],
                                    transition_private_vars->start_rotation_y) *
            transition_private_vars->transition_progress;
        transition_scratch_vectors[5] = fast_add_rotations(
            transition_private_vars->start_rotation_y, transition_scratch_vectors[5]);
        transition_scratch_vectors[6] =
            fast_subtract_rotations(transition_private_vars->end_rotation[2],
                                    transition_private_vars->start_rotation_z) *
            transition_private_vars->transition_progress;
        transition_scratch_vectors[6] = fast_add_rotations(
            transition_private_vars->start_rotation_z, transition_scratch_vectors[6]);
        FUN_L00_002ea9d8_q(transition_scratch_vectors);
        set_camera_rotation(transition_scratch_vectors + 4);
        approach_value(&transition_private_vars->ramp, 0.0f, (*(float *)&frame_time) * 4.0f);
        {
            float transition_progress = transition_private_vars->transition_progress;
            D_L18_0015F3FC_d = transition_private_vars->ramp;
            if (1.0f <= transition_progress) {
                camera_transition_active = 0;
                clear_u64_value(transition_scratch_vectors + 8);
                transition_scratch_vectors[10] = transition_private_vars->hero_yaw;
                FUN_L00_00216f90_q(
                    &transition_private_vars->hero_position, transition_scratch_vectors + 8, 0, 0);
                jet_end_mount(2);
                moby->state = STATE_POST_TRANSITION_RAMP_DOWN;
            } else if (*(int *)(camera_secondary_mode_lui) != 0x72) {
                clear_u64_value(transition_scratch_vectors + 8);
                transition_scratch_vectors[10] = transition_private_vars->hero_yaw;
                FUN_L00_00216f90_q(&transition_private_vars->hero_position,
                                   transition_scratch_vectors + 8,
                                   0x72,
                                   1);
                return;
            }
        }
        break;
    case STATE_INTERPOLATE_TRANSITION_WITH_RAMP_UP:
        if (fast_dec_timer(&transition_private_vars->timer_74)) {
            advance_accelerated_scalar(&transition_private_vars->transition_progress,
                                       (float *)&transition_private_vars->transition_progress_rate,
                                       1.0f,
                                       transition_private_vars->progress_acceleration,
                                       transition_private_vars->progress_acceleration,
                                       transition_private_vars->max_progress_rate);
        }
        lerp_vector(transition_scratch_vectors,
                    transition_private_vars,
                    &transition_private_vars->end_position,
                    transition_private_vars->transition_progress);
        clear_u64_value(transition_scratch_vectors + 4);
        transition_scratch_vectors[5] =
            fast_subtract_rotations(transition_private_vars->end_rotation[1],
                                    transition_private_vars->start_rotation_y) *
            transition_private_vars->transition_progress;
        transition_scratch_vectors[5] = fast_add_rotations(
            transition_private_vars->start_rotation_y, transition_scratch_vectors[5]);
        transition_scratch_vectors[6] =
            fast_subtract_rotations(transition_private_vars->end_rotation[2],
                                    transition_private_vars->start_rotation_z) *
            transition_private_vars->transition_progress;
        transition_scratch_vectors[6] = fast_add_rotations(
            transition_private_vars->start_rotation_z, transition_scratch_vectors[6]);
        FUN_L00_002ea9d8_q(transition_scratch_vectors);
        set_camera_rotation(transition_scratch_vectors + 4);
        if (1.0f <= transition_private_vars->transition_progress ||
            (0.2f < transition_private_vars->transition_progress && D_L18_00179E10[0] == 0 &&
             D_L18_00179E10[9] == -1)) {
            float t;
            approach_value(&transition_private_vars->ramp, 1.0f, (*(float *)&frame_time) * 4.0f);
            t = transition_private_vars->ramp;
            D_L18_0015F3FC_d = t;
            if (1.0f <= t) {
                camera_transition_active = 0;
                transition_private_vars->transition_progress = 1.0f;
                clear_u64_value(transition_scratch_vectors + 8);
                transition_scratch_vectors[10] = transition_private_vars->hero_yaw;
                FUN_L00_00216f90_q(
                    &transition_private_vars->hero_position, transition_scratch_vectors + 8, 0, 0);
                jet_end_mount(3);
                moby->state = STATE_POST_TRANSITION_RAMP_DOWN;
            }
        } else if (*(int *)(camera_secondary_mode_lui) != 0x72) {
            clear_u64_value(transition_scratch_vectors + 8);
            transition_scratch_vectors[10] = transition_private_vars->hero_yaw;
            FUN_L00_00216f90_q(
                &transition_private_vars->hero_position, transition_scratch_vectors + 8, 0x72, 1);
            return;
        } else {
            approach_value(&transition_private_vars->ramp, 0.0f, (*(float *)&frame_time) * 4.0f);
            D_L18_0015F3FC_d = transition_private_vars->ramp;
        }
        break;
    case STATE_POST_TRANSITION_RAMP_DOWN:
        if (0.0f < transition_private_vars->ramp) {
            approach_value(&transition_private_vars->ramp, 0.0f, (*(float *)&frame_time) * 4.0f);
            D_L18_0015F3FC_d = transition_private_vars->ramp;
        } else {
            moby->state = STATE_IDLE;
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
    BossCameraTransitionVars *position_rotation_transition_vars =
        (BossCameraTransitionVars *)moby->pvars;
    *(int *)&position_rotation_transition_vars->transition_progress = 0;
    position_rotation_transition_vars->transition_progress_rate = 0;
    qcopy(position_rotation_transition_vars, transition_start_position);
    qcopy(&position_rotation_transition_vars->end_position, transition_target_position);
    qcopy(&position_rotation_transition_vars->start_rotation, transition_start_rotation);
    qcopy(position_rotation_transition_vars->end_rotation, transition_target_rotation);
    qcopy(&position_rotation_transition_vars->hero_position, transition_setup_vector);
    moby->state = CAMERA_TRANSITION_STATE_PRE_TRANSITION_RAMP_UP;
    position_rotation_transition_vars->progress_acceleration = frame_time_sq * 0.333f;
    position_rotation_transition_vars->max_progress_rate =
        1.0f / (float)scale_game_frames(invulnerability_duration_frames);
    position_rotation_transition_vars->timer_74 = scale_game_frames(transition_delay_frames);
    position_rotation_transition_vars->transition_mode = transition_mode;
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
    u32 state = MOBY(moby)->state;
    s32 state_classification = 2;

    if (state == CAMERA_TRANSITION_STATE_PRE_TRANSITION_RAMP_UP) {
        return 1;
    }
    if (state != CAMERA_TRANSITION_STATE_INTERPOLATE_TRANSITION) {
        state_classification = 0;
    }
    return state_classification;
}
