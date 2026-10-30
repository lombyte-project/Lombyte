/* Level 18 moby class trespasser_barrier (0x570): a glowing, humming barrier that fades out once its linked tresspasser_pad (0x267) reaches state 3. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/rendering/geometry_quad.h"
#include "rnc/rendering/effect_texture.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/ui/hud/hud_update_counter.h"
#include "rnc/overlay/shared/hero/update_hero_and_vehicle_mode_effects.h"
#include "rnc/overlay/shared/mobys/capture_linked_moby_relative_transforms.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"

extern float          approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern unsigned char *D_L18_0015FFD8;
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F2AE0.c: func_L18_002F3268), where it is exact; names translated to the US level program. */

extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern int FUN_0022da68(int, int, int);
void       draw_trespasser_barrier_quads(struct Moby *moby) __asm__("FUN_L18_002f1fd8");

extern void draw_trespasser_barrier_quads(struct Moby *) __asm__("FUN_L18_002f1fd8");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");

void update_trespasser_barrier(struct Moby *moby) __asm__("FUN_L18_002f1e68");

void update_trespasser_barrier(struct Moby *moby) {
    enum {
        GLOW_INITIALIZE = 0,
        GLOW_ACTIVE = 1,
        GLOW_FADE_OUT = 2
    };
    int *moby_vars = (int *)moby->pvars;
    switch (moby->state) {
    case GLOW_INITIALIZE:
        moby->state = 1;
        ((float *)moby_vars)[3] = 1.0f;
        break;
    case GLOW_ACTIVE:
        add_draw_callback((void (*)(void))draw_trespasser_barrier_quads, moby);
        if (voice_is_playing(moby, moby_vars[1]) == 0) {
            moby_vars[1] = FUN_0022da68(0, 4, (int)moby);
        }
        if (moby_vars[0] != -1) {
            unsigned char *linked_moby = D_L18_0015FFD8 + (moby_vars[0] << 8);
            if (MOBY(linked_moby)->oclass == 0x267 && linked_moby[0x20] >= 3) {
                moby->collision = 0;
                moby->state = 2;
                if (voice_is_playing(moby, moby_vars[1]) != 0) {
                    int i = moby_vars[1];
                    if (i != -1) {
                        char *voice_slot_entry = ((char *)&voice_pool) + i * 0x70;
                        if (*(unsigned char **)(voice_slot_entry + 0x88) == moby &&
                            *(unsigned char *)(voice_slot_entry + 0x74) != 0) {
                            release_voice_slot(i);
                        }
                    }
                    moby_vars[1] = -1;
                }
            }
        }
        break;
    case GLOW_FADE_OUT:
        approach_value((float *)(moby_vars + 3), 0.0f, frame_time + frame_time);
        break;
    }
}

extern float D_L18_001677C0[];
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

struct GlowVars {
    u8  pad0[8];
    f32 scroll;
    f32 max_alpha;
};

extern void  draw_quad_1fd8(void *, void *, int) __asm__("FUN_001f7d30");
extern float D_0015ED6C;

#include "rnc/overlay/l18/mobys/trespasser_barrier_globals.h"

extern f32 glow_moby_phase_rate_gp __asm__("D_L18_001622E0") __attribute__((sda));

#include "rnc/overlay/l18/mobys/trespasser_barrier_data/draw_trespasser_barrier_quads.h"

extern int glow_moby_phase_blend_start_color_gp __asm__("D_L18_001622C8") __attribute__((sda));
extern int glow_moby_phase_blend_end_color_gp __asm__("D_L18_001622CC") __attribute__((sda));
extern f32 glow_moby_layer_z_step_gp __asm__("D_L18_001622D0") __attribute__((sda));
extern f32 D_L18_001622D4 __attribute__((sda));

f32 D_L18_001DA350[4][2] = {
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {7.0f, 0.0f},
    {7.0f, 1.0f},
};

extern u128 glow_moby_quad_base_positions[4] __asm__("D_L18_001DA370");

void draw_trespasser_barrier_quads(struct Moby *moby) {
    struct GeometryQuad quads[4];
    float               matrix[4][4] __attribute__((aligned(16)));
    struct GlowVars    *vars;
    float               alpha;
    float               step;
    float               phase;
    int                 color;
    int                 i;
    int                 j;
    int                 pass;

    vars = (struct GlowVars *)moby->pvars;
    copy_matrix_rotation(matrix, &moby->rot_row0);
    qcopy(matrix[3], &moby->pos);
    matrix[3][3] = 1.0f;
    vars->scroll += glow_moby_phase_rate_gp * D_0015ED6C;
    if (1.0f < vars->scroll) {
        vars->scroll -= 1.0f;
    }
    alpha = (48.0f - fast_vec_dist(&moby->pos, D_L18_001677C0)) * 0.25f;
    if (vars->max_alpha < alpha) {
        alpha = vars->max_alpha;
    } else if (alpha < 0.0f) {
        alpha = 0.0f;
    }
    step = 0.25f;
    for (i = 0; i < 4; i++) {
        quads[i].texture = get_effect_texture(0xE);
        quads[i].reserved.value = 0;
        quads[i].texture_state = 0xFF9000000260ULL;
        quads[i].primitive = (u64)glow_moby_blend_minuend_selector_gp |
                             ((u64)glow_moby_blend_subtrahend_selector_gp << 2) |
                             ((u64)glow_moby_blend_factor_selector_gp << 4) |
                             ((u64)glow_moby_blend_addend_selector_gp << 6) | 0x8000000000ULL;
        phase = vars->scroll + i * step;
        phase = phase - (float)truncate_float_to_s32(phase);
        color = fast_tween_color(
            glow_moby_phase_blend_start_color_gp, glow_moby_phase_blend_end_color_gp, phase);
        color = fast_tween_color(color & 0xFFFFFF, color, alpha);
        for (j = 0; j < 4; j++) {
            quads[i].texture_coordinates[j][0] = D_L18_001DA350[j][0] + i * step;
            quads[i].texture_coordinates[j][1] = D_L18_001DA350[j][1];
            quads[i].colors[j] = color;
            qcopy_nc(&quads[i].positions[j], &glow_moby_quad_base_positions[j]);
            if (j & 1) {
                quads[i].positions[j].f[2] += phase * D_L18_001622D4;
            } else {
                quads[i].positions[j].f[2] -= phase * D_L18_001622D4;
            }
        }
    }
    for (pass = 2; pass >= 0; pass--) {
        matrix[3][2] += glow_moby_layer_z_step_gp;
        draw_quad_1fd8(&quads[0], matrix, 0);
        draw_quad_1fd8(&quads[1], matrix, 0);
        draw_quad_1fd8(&quads[2], matrix, 0);
        draw_quad_1fd8(&quads[3], matrix, 0);
    }
}
