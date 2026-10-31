/* Level 12 moby class water_reservoir: the reservoir's rising and falling water and its spray. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/overlay/collision.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/interpolation.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/rendering/effects/spawn_spark.h"

extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
void         draw_water_reservoir(struct Moby *moby) __asm__("FUN_L12_002ecac0");

extern void  draw_water_reservoir(struct Moby *) __asm__("FUN_L12_002ecac0");
extern float D_L12_0015F3FC;

#include "rnc/overlay/l12/mobys/water_reservoir_globals.h"

float water_reservoir_angle_rate_degrees_gp __asm__("D_L12_001619CC") = 1.8e+02f;
int   water_reservoir_transition_frames __asm__("D_L12_001619D0") = 105;
float water_reservoir_random_offset_radius_gp __asm__("D_L12_001619D4") = 0.25f;

#include "rnc/overlay/l12/mobys/water_reservoir_data/water_reservoir_update_3.h"

int D_L12_001619E8 = 1;

#include "rnc/overlay/l12/mobys/water_reservoir_data/water_reservoir_update_2.h"

int D_L12_001619F0 = 1;
int D_L12_001619F4 = 0x40404040;
int D_L12_001619F8 = 0x10022;

#include "rnc/overlay/l12/mobys/water_reservoir_data/water_reservoir_update.h"

float water_reservoir_height_approach_rate_gp __asm__("D_L12_00161A08") = 6.0f;

void water_reservoir_update(struct Moby *moby) __asm__("FUN_L12_002ec720");

/* Moby class 384 */
void water_reservoir_update(struct Moby *moby) {
    float *reservoir_state = (float *)moby->pvars;
    float  pos[4], offset0[4], offset1[4], pos_offset_delta[4], offset_pair_delta[4];
    float  fraction, frame_scale;
    int    scaled;
    if (moby->state != 0) {
        extern float D_L12_0015F3FC __attribute__((sda));
        frame_scale = D_L12_0015F3FC;
        if (moby->state == 1)
            goto active;
        goto update;
    }
    moby->state = 1;
    moby->command = 2;
    reservoir_state[0] = random_angle_radians();
    reservoir_state[2] = 5.0f;
    goto frame_active;
active:
    if (moby->command & 8) {
        moby->command = 1;
        *(int *)&reservoir_state[1] = scale_game_frames(water_reservoir_transition_frames);
    } else if (moby->command & 4) {
        moby->command = 2;
        *(int *)&reservoir_state[1] = scale_game_frames(water_reservoir_transition_frames);
    }
    reservoir_state[0] = fast_add_rotations(
        reservoir_state[0], water_reservoir_angle_rate_degrees_gp * 0.017453292f * frame_time);
frame_active:
    frame_scale = D_L12_0015F3FC;
update:
    if (frame_scale == 0.0f) {
        if (fast_dec_timer((int *)&reservoir_state[1]) == 0) {
            if (moby->command & 1) {
                fraction = 1.0f - convert_integer_to_float(*(int *)&reservoir_state[1]) /
                                      convert_integer_to_float(water_reservoir_transition_frames);
            } else {
                fraction = convert_integer_to_float(*(int *)&reservoir_state[1]) /
                           convert_integer_to_float(water_reservoir_transition_frames);
            }
            if (random_float_between(0.0f, 1.0f) < fraction) {
                random_vector(pos, 0.0f, water_reservoir_random_offset_radius_gp);
                random_vector(offset0, 0.0f, water_reservoir_random_offset_radius_gp);
                random_vector(offset1, 0.0f, water_reservoir_random_offset_radius_gp);
                fast_vec_sub(pos_offset_delta, pos, offset0);
                pos_offset_delta[2] += water_reservoir_effect_delta_z_reference_gp -
                                       water_reservoir_unmirrored_effect_spawn_z_offset_gp;
                fast_vec_sub(offset_pair_delta, offset0, offset1);
                offset_pair_delta[2] += water_reservoir_mirrored_effect_spawn_z_offset_gp -
                                        water_reservoir_effect_delta_z_reference_gp;
                scaled = water_reservoir_effect_delta_scale_divisor_gp;
                fast_vec_scale(pos_offset_delta, pos_offset_delta, 2.0f / (float)scaled);
                fast_vec_scale(offset_pair_delta, offset_pair_delta, 2.0f / (float)scaled);
                fast_vec_add(pos, pos, (float *)&moby->pos);
                if (moby->command & 1) {
                    pos[2] += water_reservoir_mirrored_effect_spawn_z_offset_gp;
                    pos_offset_delta[2] = -pos_offset_delta[2];
                    offset_pair_delta[2] = -offset_pair_delta[2];
                } else {
                    pos[2] += water_reservoir_unmirrored_effect_spawn_z_offset_gp;
                }
                pos_offset_delta[3] = random_float_between(
                    water_reservoir_effect_delta_fourth_component_range_start_gp,
                    water_reservoir_effect_delta_fourth_component_range_end_gp);
                offset_pair_delta[3] = random_float_between(
                    water_reservoir_effect_delta_fourth_component_range_start_gp,
                    water_reservoir_effect_delta_fourth_component_range_end_gp);
                spawn_spark(pos,
                            pos_offset_delta,
                            offset_pair_delta,
                            D_L12_001619F4,
                            D_L12_001619F4,
                            D_L12_001619E8,
                            scale_game_frames(water_reservoir_effect_delta_scale_divisor_gp),
                            D_L12_001619F0,
                            D_L12_001619F8);
            }
        }
        if ((moby->command & 1) && *(int *)&reservoir_state[1] == 0) {
            approach_value(
                -1.0f, water_reservoir_height_approach_rate_gp * frame_time, &reservoir_state[2]);
        } else if (moby->command & 2) {
            approach_value(
                3.0f, water_reservoir_height_approach_rate_gp * frame_time, &reservoir_state[2]);
        }
    }
    add_draw_callback(draw_water_reservoir, moby);
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_002EDE40), where it is exact; names translated to the US level program. */

typedef struct {
    float v[16];
    int   c[4];
    float uv[8];
    long  g[4];
} Quad;

extern char  water_reservoir_quad_vertex_template[] __asm__("D_L12_001F5B20");
extern float fast_cos(float);
extern float fast_sin(float);
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int   get_effect_texture(int) __asm__("FUN_001f44b8");
int          D_L12_00161A10 = 1;
int          D_L12_00161A14 = 0;
int          D_L12_00161A0C = 0;
int          D_L12_00161A18 = 1;
int          D_L12_00161A1C = 0x40602020;
int          D_L12_00161A20 = 0x40681818;
extern short water_reservoir_segment_cosine_scale_gp __asm__("D_L12_00161A24") __attribute__((sda));
extern void  draw_geometry_quad(void *, void *, int);

void draw_water_reservoir(struct Moby *moby) {
    char *water_reservoir_vars = (char *)moby->pvars;
    Quad  quad;
    float rotation_matrix[12];
    float pos[4];
    float segment_start_y, segment_end_y;
    int   i, col;
    copy_matrix_rotation(rotation_matrix, &moby->rot_row0);
    qcopy(pos, &moby->pos);
    pos[2] = pos[2] + *(float *)(water_reservoir_vars + 8);
    pos[3] = 1.0f;
    quad.g[1] = get_effect_texture(0x10);
    quad.g[0] = 0;
    quad.g[2] = 0xFF9000000260UL;
    quad.g[3] = (long)D_L12_00161A0C | ((long)D_L12_00161A10 << 2) | ((long)D_L12_00161A14 << 4) |
                ((long)D_L12_00161A18 << 6) | (0x8000L << 24);
    col = fast_tween_color(
        D_L12_00161A1C, D_L12_00161A20, (fast_sin(*(float *)water_reservoir_vars) + 1.0f) * 0.5f);
    for (i = 0; i < 4; i++) {
        quad.uv[i * 2] = 0.5f;
        *(&quad.uv[i * 2] + 1) = 0.5f;
        quad.c[i] = col;
        qcopy(&quad.v[i * 4], water_reservoir_quad_vertex_template + i * 16);
    }
    for (segment_start_y = -0.4f; segment_start_y < 0.36f; segment_start_y += 0.1f) {
        segment_end_y = segment_start_y + 0.1f;
        quad.v[9] = segment_start_y;
        quad.v[1] = segment_start_y;
        quad.v[13] = segment_end_y;
        quad.v[5] = segment_end_y;
        if (moby->command & 1) {
            quad.v[2] =
                -fast_cos(segment_start_y * *(float *)&water_reservoir_segment_cosine_scale_gp) +
                2.0f;
            quad.v[6] =
                -fast_cos(segment_end_y * *(float *)&water_reservoir_segment_cosine_scale_gp) +
                2.0f;
        } else {
            quad.v[2] =
                fast_cos(segment_start_y * *(float *)&water_reservoir_segment_cosine_scale_gp);
            quad.v[6] =
                fast_cos(segment_end_y * *(float *)&water_reservoir_segment_cosine_scale_gp);
        }
        draw_geometry_quad(&quad, rotation_matrix, 0);
    }
}
