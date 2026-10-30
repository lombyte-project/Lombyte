/* Level 11 moby class seesaw_platform: the tilting platform. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/game/unlock_planet.h"

extern float fast_add_rotations(float, float);

#include "rnc/overlay/l11/mobys/seesaw_platform_globals.h"

extern float seesaw_hero_local_x_scale_gp __asm__("D_L11_00161F68") __attribute__((sda));
extern float seesaw_tilt_feedback_scale_gp __asm__("D_L11_00161F6C") __attribute__((sda));
extern float seesaw_spin_feedback_scale_gp __asm__("D_L11_00161F70") __attribute__((sda));
extern float seesaw_rider_spin_change_bias_degrees_gp __asm__("D_L11_00161F74")
    __attribute__((sda));
extern float seesaw_rider_spin_change_gain_degrees_gp __asm__("D_L11_00161F78")
    __attribute__((sda));
extern float seesaw_tilt_feedback_gain_degrees_gp __asm__("D_L11_00161F7C") __attribute__((sda));
extern float seesaw_spin_feedback_gain_degrees_gp __asm__("D_L11_00161F80") __attribute__((sda));
extern float seesaw_fall_tilt_threshold_degrees_gp __asm__("D_L11_00161F84") __attribute__((sda));
extern int   hero_set_state(int, int) __asm__("FUN_L11_0024db50");

/* Private variables of the tilting platform (seesaw_platform_update). */
struct L11TiltVars {
    u8  pad0[0x20];
    u8  unk20[0x3C]; /* 0x20: rider state for update_entity_matrix */
    s32 unk5C;       /* 0x5C */
    f32 spin;        /* 0x60: rot.y speed */
};

/* Tilting platform: rot.y springs back and is damped through its spin; the hero standing on it tilts it toward
   his side, and a tilt past D_L11_00161F84 degrees knocks him into hero state 6. */
void seesaw_platform_update(struct Moby *m) __asm__("FUN_L11_0030eb90");

void seesaw_platform_update(struct Moby *platform) {
    Vec4f               platform_translation_delta;
    Vec4f               rot;
    float               matrix[16] __attribute__((aligned(16)));
    Vec4f               hero_local_offset;
    struct L11TiltVars *platform_vars = (struct L11TiltVars *)platform->pvars;
    float               spin;
    float               change;
    float               scaled;

    fast_vec_scale(&platform_translation_delta, &platform->pos, -1.0f);
    qcopy(&rot, &platform->rot);
    platform_vars->unk5C = 1;
    spin = platform_vars->spin;
    /* two separate updates (not one sum): gives retail's FP register allocation */
    platform_vars->spin = spin + seesaw_tilt_feedback_gain_degrees_gp * 0.017453292f *
                                     frame_time_sq *
                                     (seesaw_tilt_feedback_scale_gp * platform->rot.y);
    platform_vars->spin = platform_vars->spin + seesaw_spin_feedback_gain_degrees_gp *
                                                    0.017453292f * frame_time_sq *
                                                    (seesaw_spin_feedback_scale_gp * spin);
    if (hero.ground_moby == platform && hero.air_frames.s == 0) {
        fast_vec_sub(&hero_local_offset, &hero.motion.pos, &platform->pos);
        transpose_rotation_matrix(matrix, &platform->rot_row0);
        transform_vector(&hero_local_offset, &hero_local_offset, matrix);
        scaled = seesaw_hero_local_x_scale_gp * hero_local_offset.x;
        change = seesaw_rider_spin_change_gain_degrees_gp * 0.017453292f * frame_time_sq * scaled;
        if (platform_vars->spin * change < 0.0f && platform->rot.y * change < 0.0f) {
            change += change;
        }
        if (change < 0.0f) {
            change -= seesaw_rider_spin_change_bias_degrees_gp * 0.017453292f * frame_time_sq;
        } else {
            change += seesaw_rider_spin_change_bias_degrees_gp * 0.017453292f * frame_time_sq;
        }
        platform_vars->spin = platform_vars->spin + change;
        if (absolute_float(platform->rot.y) >
                seesaw_fall_tilt_threshold_degrees_gp * 0.017453292f &&
            (u32)hero.state.control_mode < 2) {
            hero_set_state(HERO_STATE_FALL, 1);
        }
    }
    platform->rot.y = fast_add_rotations(platform->rot.y, platform_vars->spin);
    fast_vec_add(&platform_translation_delta, &platform_translation_delta, &platform->pos);
    update_entity_matrix(platform_vars->unk20, &platform_translation_delta, &rot, &platform->rot);
}
