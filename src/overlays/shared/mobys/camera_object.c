/* Shared moby classes: camera_object, vendor, bolt, wrench. */
#include "types.h"
#include "rnc/game/bolt_sources.h"
#include "rnc/mobys/moby_class_id.h"
#include "rnc/hero/hero.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "sda.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/debug/debug_menu.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_move_state_velocity.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/rotations/clip_negative_segment_endpoint_xyz.h"
#include "rnc/overlay/shared/math/vectors/random_vector.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/math/rotations/matrix_to_euler.h"
#include "rnc/overlay/shared/math/vectors/reflect_vector.h"
#include "rnc/overlay/shared/rendering/commands/set_moby_color.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/overlay/shared/ui/text/update_hud_counter_cap_and_visibility.h"
#include "rnc/ui/help/try_set_help_message.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/mobys/animation/update_moby_animation_state.h"
#include "rnc/overlay/shared/mobys/open_vendor_menu.h"
#include "rnc/overlay/shared/math/vectors/rotation_2D5FA8.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"

/* per-frame level ambience (wind/sway) state machine: picks a random target, then eases toward it */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A5B20), where it is exact; names translated to the US level program. */

struct Amb {
    float f0;
    float f4;
    float f8;
    short hC;
    short hE;
    float f10;
    float f14;
    float f18;
    short h1C;
    short h1E;
};

extern float      D_0015ED6C;
extern float      scale_time(float) __asm__("FUN_001f96b0");
extern struct Amb D_L00_001CB7C0;

void update_ambience_target_cycle(void) __asm__("FUN_L00_002a4890");

void update_ambience_target_cycle(void) {
    enum {
        AMBIENCE_WAIT_AND_SELECT_TARGET = 0,
        AMBIENCE_INTERPOLATE_TO_TARGET = 1
    };
    enum {
        AMBIENCE_WAIT_AND_SELECT_SECOND_TARGET = 2,
        AMBIENCE_INTERPOLATE_TO_SECOND_TARGET = 3
    };
    enum {
        AMBIENCE_HOLD_STATE = 1,
        AMBIENCE_SINGLE_TARGET_CYCLE = 2,
        AMBIENCE_TWO_TARGET_CYCLE = 3
    };
    struct Amb *ambience_state = &D_L00_001CB7C0;
    float       interpolation_duration, target_scalar, target_rotation, scalar_magnitude,
        scalar_clamp_value;
    switch (ambience_state->hC) {
    case AMBIENCE_HOLD_STATE:
        break;
    case AMBIENCE_SINGLE_TARGET_CYCLE:
        switch (ambience_state->hE) {
        case AMBIENCE_WAIT_AND_SELECT_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                interpolation_duration = convert_integer_to_float(ambience_state->h1C) *
                                         random_float_between(0.5f, 1.5f);
                target_rotation = fast_add_rotations(ambience_state->f0,
                                                     random_float_between(-1.0471976f, 1.0471976f));
                target_scalar =
                    random_float_between(ambience_state->f10 * 0.5f, ambience_state->f10 * 1.5f);
                ambience_state->f8 = fast_subtract_rotations(target_rotation, ambience_state->f4) /
                                     interpolation_duration;
                ambience_state->f18 =
                    (target_scalar - ambience_state->f14) / interpolation_duration;
                ambience_state->h1E = truncate_float_to_s32(interpolation_duration);
                ambience_state->hE = 1;
            }
            break;
        case AMBIENCE_INTERPOLATE_TO_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                ambience_state->h1E =
                    truncate_float_to_s32(convert_integer_to_float(ambience_state->h1C) *
                                          random_float_between(2.0f, 3.0f));
                ambience_state->hE = 0;
            } else {
                ambience_state->f4 = fast_add_rotations(ambience_state->f4, ambience_state->f8);
                ambience_state->f14 = ambience_state->f14 + ambience_state->f18;
                scalar_magnitude = absolute_float(ambience_state->f14);
                scalar_clamp_value = D_0015ED6C * 9.6f;
                if (scalar_clamp_value < scalar_magnitude) {
                    if (ambience_state->f14 < 0.0f)
                        scalar_clamp_value = -scalar_clamp_value;
                    ambience_state->f14 = scalar_clamp_value;
                }
            }
            break;
        }
        break;
    case AMBIENCE_TWO_TARGET_CYCLE:
        switch (ambience_state->hE) {
        case AMBIENCE_WAIT_AND_SELECT_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                interpolation_duration = scale_time(random_float_between(45.0f, 60.0f));
                target_rotation = fast_add_rotations(ambience_state->f0,
                                                     random_float_between(-1.5707964f, 1.5707964f));
                target_scalar = random_float_between(ambience_state->f10 * 1.5f,
                                                     ambience_state->f10 + ambience_state->f10);
                ambience_state->f8 = fast_subtract_rotations(target_rotation, ambience_state->f4) /
                                     interpolation_duration;
                ambience_state->f18 =
                    (target_scalar - ambience_state->f14) / interpolation_duration;
                ambience_state->h1E = truncate_float_to_s32(interpolation_duration);
                ambience_state->hE = 1;
            }
            break;
        case AMBIENCE_INTERPOLATE_TO_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                ambience_state->h1E =
                    truncate_float_to_s32(convert_integer_to_float(ambience_state->h1C) *
                                          random_float_between(1.0f, 1.5f));
                ambience_state->hE = 2;
            } else {
                ambience_state->f4 = fast_add_rotations(ambience_state->f4, ambience_state->f8);
                ambience_state->f14 = ambience_state->f14 + ambience_state->f18;
                scalar_magnitude = absolute_float(ambience_state->f14);
                scalar_clamp_value = D_0015ED6C * 9.6f;
                if (scalar_clamp_value < scalar_magnitude) {
                    if (ambience_state->f14 < 0.0f)
                        scalar_clamp_value = -scalar_clamp_value;
                    ambience_state->f14 = scalar_clamp_value;
                }
            }
            break;
        case AMBIENCE_WAIT_AND_SELECT_SECOND_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                interpolation_duration = scale_time(random_float_between(45.0f, 60.0f));
                target_rotation = fast_add_rotations(ambience_state->f0,
                                                     random_float_between(-0.785398f, 0.785398f));
                target_scalar =
                    random_float_between(ambience_state->f10 * 0.8f, ambience_state->f10 * 1.2f);
                ambience_state->f8 = fast_subtract_rotations(target_rotation, ambience_state->f4) /
                                     interpolation_duration;
                ambience_state->f18 =
                    (target_scalar - ambience_state->f14) / interpolation_duration;
                ambience_state->h1E = truncate_float_to_s32(interpolation_duration);
                ambience_state->hE = 3;
            }
            break;
        case AMBIENCE_INTERPOLATE_TO_SECOND_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                ambience_state->h1E =
                    truncate_float_to_s32(convert_integer_to_float(ambience_state->h1C) *
                                          random_float_between(3.0f, 5.0f));
                ambience_state->hE = 0;
            } else {
                ambience_state->f4 = fast_add_rotations(ambience_state->f4, ambience_state->f8);
                ambience_state->f14 = ambience_state->f14 + ambience_state->f18;
                scalar_magnitude = absolute_float(ambience_state->f14);
                scalar_clamp_value = D_0015ED6C * 9.6f;
                if (scalar_clamp_value < scalar_magnitude) {
                    if (ambience_state->f14 < 0.0f)
                        scalar_clamp_value = -scalar_clamp_value;
                    ambience_state->f14 = scalar_clamp_value;
                }
            }
            break;
        }
        break;
    }
}

#include "rnc/overlay/shared/mobys/camera_object_data/update_frame_timing_and_wrapped_value_2.h"
#include "rnc/overlay/shared/mobys/camera_object_globals.h"

extern int configured_unscaled_frame_count_gp __asm__("D_L00_00161368") __attribute__((sda));
extern int configured_reciprocal_frame_count_gp __asm__("D_L00_0016136C") __attribute__((sda));

#include "rnc/overlay/shared/mobys/camera_object_data/update_frame_timing_and_wrapped_value.h"

extern int   D_L00_0016137C_gp __asm__("D_L00_0016137C") __attribute__((sda));
extern int   D_L00_00161380 __attribute__((sda));
extern int   D_L00_00161388;
extern float D_L00_00161374_gp __asm__("D_L00_00161374") __attribute__((sda));
extern float wrapped_value_decrement_rate_gp __asm__("D_L00_00161378") __attribute__((sda));
extern float D_L00_00161384_gp __asm__("D_L00_00161384") __attribute__((sda));
extern float D_L00_0016138C_gp __asm__("D_L00_0016138C") __attribute__((sda));

#include "rnc/globals.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/overlay/hero_tables.h"

void update_frame_timing_and_wrapped_value(unsigned char *state_object) __asm__("FUN_L00_002a4cd8");

void update_frame_timing_and_wrapped_value(unsigned char *state_object) {
    enum {
        INITIALIZE_FRAME_TIMING = 0,
        UPDATE_WRAPPED_VALUE = 1
    };
    float one;
    int   scaled_frame_count;
    switch (state_object[0x20]) {
    case INITIALIZE_FRAME_TIMING:
        fetch_add_accumulator = 0;
        state_object[0x20] = 1;
        state_object[0x30] = 0xFF;
        one = 1.0f;
        D_L00_0016137C_gp = scale_game_frames(configured_unscaled_frame_count_gp);
        scaled_frame_count = scale_game_frames(configured_reciprocal_frame_count_gp);
        D_L00_00161380 = scaled_frame_count;
        D_L00_00161384_gp = one / convert_integer_to_float(scaled_frame_count);
        scaled_frame_count = scale_game_frames(configured_reciprocal_denominator_frames);
        D_L00_00161388 = scaled_frame_count;
        D_L00_0016138C_gp = one / (float)scaled_frame_count;
        break;
    case UPDATE_WRAPPED_VALUE:
        D_L00_00161374_gp -= wrapped_value_decrement_rate_gp * frame_scale;
        if (D_L00_00161374_gp <= -8.0f)
            D_L00_00161374_gp += 8.0f;
        break;
    }
}

int fetch_and_add_global_accumulator(int addend) __asm__("FUN_L00_002a4dc8");

int fetch_and_add_global_accumulator(int addend) {
    int previous_value = fetch_add_accumulator;
    fetch_add_accumulator = previous_value + addend;
    return previous_value;
}

#include "eetypes.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Creates a class 0x3EF moby at v and runs update_entity on it. */
unsigned char *create_camera_collision_moby(u128 *spawn_position) __asm__("FUN_L00_002a4de0");

unsigned char *create_camera_collision_moby(u128 *spawn_position) {
    unsigned char *spawned_moby = create_moby(0x3ef);
    if (spawned_moby) {
        spawned_moby[0x30] = 0xff;
        MOBY(spawned_moby)->flags |= 0x41;
        qcopy(spawned_moby + 0x10, spawn_position);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

typedef struct {
    char  p0[0x98];
    float f98;
    char  p1[0x164 - 0x9C];
    float f164;
    char  p2[0x2080 - 0x168];
    char *obj;
    char  p3[0x208C - 0x2084];
    int   w208C;
} G_2a4e48;

typedef struct {
    char  p0[0x158];
    float f158;
    char  p1[0x180 - 0x15C];
    char *c;
} C_2a4e48;

#include "rnc/overlay/level_camera.h"

extern LevelCamera l00_camera_lui __asm__("D_L00_00166C80") NOT_SDA;

void camera_object_update(char *o) __asm__("FUN_L00_002a4e48");

void camera_object_update(char *camera_object) {
    OvlQuad      difference_vector[1];
    OvlQuad      scaled_vector[1];
    LevelCamera *C = &l00_camera_lui;
    char        *object_position;
    char        *pos;
    G_2a4e48    *P;
    float        dot_product_or_threshold;

    if (C->unk180 != 0) {
        object_position = camera_object + 0x10;
        qcopy(object_position, (char *)C->unk180 + 0x30);
        pos = (char *)&hero.motion.pos;
        fast_vec_sub(difference_vector, object_position, pos);
        P = (G_2a4e48 *)(pos - 0x80);
        dot_product_or_threshold = fast_vec_dot(difference_vector, P->obj + 0xE0);
        fast_vec_scale(scaled_vector, P->obj + 0xE0, dot_product_or_threshold);
        fast_vec_sub(difference_vector, difference_vector, scaled_vector);
        if (absolute_float(dot_product_or_threshold) < 2.5f) {
            dot_product_or_threshold = 0.1f;
            if (fast_vec_length(difference_vector) < dot_product_or_threshold ||
                fast_difference_between_rotations(P->f98, C->yaw) < 2.3561945f ||
                P->f164 < frame_time * dot_product_or_threshold) {
                *(float *)(camera_object + 0x18) = 15.0f;
                *(float *)(camera_object + 0x14) = 15.0f;
                *(float *)(camera_object + 0x10) = 15.0f;
            } else {
                dot_product_or_threshold = fast_vec_dot(object_position, P->obj + 0xE0);
                fast_vec_scale(scaled_vector, P->obj + 0xE0, dot_product_or_threshold);
                fast_vec_sub(difference_vector, object_position, scaled_vector);
                dot_product_or_threshold = fast_vec_dot(pos, P->obj + 0xE0);
                if (P->w208C != 0x11) {
                    fast_vec_scale(scaled_vector, P->obj + 0xE0, dot_product_or_threshold);
                    fast_vec_add(object_position, difference_vector, scaled_vector);
                }
            }
        }
    } else {
        *(float *)(camera_object + 0x18) = 5.0f;
        *(float *)(camera_object + 0x14) = 5.0f;
        *(float *)(camera_object + 0x10) = 5.0f;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/bolts_and_wrench.c: func_L00_002A62D0). */

typedef unsigned int Q_2A62D0 __attribute__((mode(TI), aligned(16)));

typedef union {
    Q_2A62D0 q;
    float    f[4];
} V_2A62D0;

typedef struct {
    float v[4][4];
    int   col[4];
    float uv[4][2];
} Quad;

extern float D_L00_001D7360[][4];
extern float D_L00_001D73F0[][2];
extern int   vendor_sparkle_colors[] __asm__("D_L00_001D7438") NOT_SDA;
extern int   vendor_sparkle_quad_corners[][4] __asm__("D_L00_001D7460");
extern s64   get_effect_texture(s32) __asm__("FUN_001f44b8");
extern float vendor_sparkle_uv_scroll __asm__("D_L00_00161394") __attribute__((sda));
extern float vendor_glow_phase __asm__("D_L00_00161398") __attribute__((sda));
void        draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

void draw_vendor_effects(char *m) __asm__("FUN_L00_002a5040");

void draw_vendor_effects(char *m) {
    char    *p = *(char **)(m + 0x78);
    Quad     qd;
    long     pk[4];
    float    q[4][4];
    float    d0[4];
    float    mtx[3][4];
    V_2A62D0 M[4];
    V_2A62D0 T[4];
    float    zero;
    char    *g, *h;
    float(*uv_v)[2];
    float(*uvs)[2];
    int   izero;
    float off;
    /* Fakematch: the sparkle radius 1.1 lives in $f22, copied from the glow block's 1.1. */
    register float sparkle_radius __asm__("$f22");
    /* Fakematch: d0's address for the second euler_rotation_basis call. */
    register float *d0_arg __asm__("$5");
    int             k, i, i0, i1, i2, i3, col;

    h = (char *)&l00_camera_lui;
    d0[2] = *(float *)(h + 0x158);
    d0[0] = 0.0f;
    d0[1] = 0.0f;
    euler_rotation_basis(mtx, d0);

    vendor_sparkle_uv_scroll = vendor_sparkle_uv_scroll + 0.01f;
    if (1.0f < vendor_sparkle_uv_scroll) {
        vendor_sparkle_uv_scroll = vendor_sparkle_uv_scroll - 1.0f;
    }

    pk[2] = 1;
    pk[3] = 0x4000000044L;
    pk[0] = 0;
    pk[1] = get_effect_texture(0x18);

    for (k = 0; k < 4; k++) {
        transform_vector_by_basis(q[0], D_L00_001D7360[vendor_sparkle_quad_corners[k][0]], mtx);
        transform_vector_by_basis(q[1], D_L00_001D7360[vendor_sparkle_quad_corners[k][1]], mtx);
        transform_vector_by_basis(q[2], D_L00_001D7360[vendor_sparkle_quad_corners[k][2]], mtx);
        transform_vector_by_basis(q[3], D_L00_001D7360[vendor_sparkle_quad_corners[k][3]], mtx);

        qd.v[0][0] = *(float *)(m + 0x10) + q[0][0] * *(float *)(p + 0x90);
        qd.v[0][1] = *(float *)(m + 0x14) + q[0][1] * *(float *)(p + 0x90);
        qd.v[0][2] = *(float *)(m + 0x18) + q[0][2];
        qd.v[1][0] = *(float *)(m + 0x10) + q[1][0] * *(float *)(p + 0x90);
        qd.v[1][1] = *(float *)(m + 0x14) + q[1][1] * *(float *)(p + 0x90);
        qd.v[1][2] = *(float *)(m + 0x18) + q[1][2];
        qd.v[2][0] = *(float *)(m + 0x10) + q[2][0] * *(float *)(p + 0x90);
        qd.v[2][1] = *(float *)(m + 0x14) + q[2][1] * *(float *)(p + 0x90);
        qd.v[2][2] = *(float *)(m + 0x18) + q[2][2];
        qd.v[3][0] = *(float *)(m + 0x10) + q[3][0] * *(float *)(p + 0x90);
        qd.v[3][1] = *(float *)(m + 0x14) + q[3][1] * *(float *)(p + 0x90);
        qd.v[3][2] = *(float *)(m + 0x18) + q[3][2];

        i0 = vendor_sparkle_quad_corners[k][0];
        i1 = vendor_sparkle_quad_corners[k][1];
        i2 = vendor_sparkle_quad_corners[k][2];
        i3 = vendor_sparkle_quad_corners[k][3];
        off = vendor_sparkle_uv_scroll;
        qd.col[0] = vendor_sparkle_colors[i0];
        qd.col[1] = vendor_sparkle_colors[i1];
        qd.col[2] = vendor_sparkle_colors[i2];
        qd.col[3] = vendor_sparkle_colors[i3];
        uvs = D_L00_001D73F0;
        uv_v = (float(*)[2]) & uvs[0][1];
        qd.uv[0][0] = uvs[i0][0];
        qd.uv[0][1] = uv_v[i0][0] + off;
        qd.uv[1][0] = uvs[i1][0];
        qd.uv[1][1] = uv_v[i1][0] + off;
        qd.uv[2][0] = uvs[i2][0];
        qd.uv[2][1] = uv_v[i2][0] + off;
        qd.uv[3][0] = uvs[i3][0];
        qd.uv[3][1] = uv_v[i3][0] + off;
        draw_geometry_quad(&qd, 0, 1);
    }

    pk[1] = get_effect_texture(0x1B);

    vendor_glow_phase = vendor_glow_phase + 0.01f;
    if (1.0f < vendor_glow_phase) {
        vendor_glow_phase = 0.0f;
    }

    {
        float g = vendor_glow_phase;
        float n = -g * 1.2f;
        float pp = g * 1.2f;
        float z = g * 1.4f + 1.1f;
        float zz = z - 0.2f;
        sparkle_radius = 1.1f;

        T[0].q = 0;
        T[0].f[0] = n * *(float *)(p + 0x90);
        T[0].f[1] = n * *(float *)(p + 0x90);
        T[0].f[2] = z;
        T[1].q = 0;
        T[1].f[0] = pp * *(float *)(p + 0x90);
        T[1].f[1] = n * *(float *)(p + 0x90);
        T[1].f[2] = zz;
        T[2].q = 0;
        T[2].f[0] = n * *(float *)(p + 0x90);
        T[2].f[1] = pp * *(float *)(p + 0x90);
        T[2].f[2] = z;
        T[3].q = 0;
        T[3].f[0] = pp * *(float *)(p + 0x90);
        T[3].f[1] = pp * *(float *)(p + 0x90);
        T[3].f[2] = zz;
        M[0].q = T[0].q;
        T[0].q = M[0].q;
        M[1].q = T[1].q;
        T[1].q = M[1].q;
        M[2].q = T[2].q;
        M[3].q = T[3].q;
    }

    g = (char *)&l00_camera_lui;
    off = *(float *)(g + 0x158);
    d0[0] = 0.0f;
    d0[2] = off;
    d0[1] = 0.0f;
    /* Fakematch: d0_arg = d0 (frame slot 0xD0). The sparkle_radius input puts it after retail's mov.s $f22,$f5;
     * the $4 clobber keeps addiu $a0 (mtx) after it. */
    __asm__("addiu %0,$sp,0xD0" : "=r"(d0_arg) : "f"(sparkle_radius) : "$4");
    euler_rotation_basis(mtx, d0_arg);
    transform_vector_by_basis(q[0], M[0].f, mtx);
    transform_vector_by_basis(q[1], M[1].f, mtx);
    transform_vector_by_basis(q[2], M[2].f, mtx);
    transform_vector_by_basis(q[3], M[3].f, mtx);

    col = ((int)((1.0f - vendor_glow_phase) * 128.0f) << 24) | 0x808080;
    qd.v[0][0] = *(float *)(m + 0x10) + q[0][0];
    qd.v[0][1] = *(float *)(m + 0x14) + q[0][1];
    qd.v[0][2] = *(float *)(m + 0x18) + q[0][2];
    qd.v[1][0] = *(float *)(m + 0x10) + q[1][0];
    qd.v[1][1] = *(float *)(m + 0x14) + q[1][1];
    qd.v[1][2] = *(float *)(m + 0x18) + q[1][2];
    qd.v[2][0] = *(float *)(m + 0x10) + q[2][0];
    qd.v[2][1] = *(float *)(m + 0x14) + q[2][1];
    qd.v[2][2] = *(float *)(m + 0x18) + q[2][2];
    qd.v[3][0] = *(float *)(m + 0x10) + q[3][0];
    qd.v[3][1] = *(float *)(m + 0x14) + q[3][1];
    qd.v[3][2] = *(float *)(m + 0x18) + q[3][2];
    qd.col[0] = col;
    qd.col[1] = col;
    qd.col[2] = col;
    qd.col[3] = col;
    qd.uv[0][0] = 0.0f;
    qd.uv[0][1] = 0.0f;
    qd.uv[1][0] = 1.0f;
    qd.uv[1][1] = 0.0f;
    qd.uv[2][0] = 0.0f;
    qd.uv[2][1] = 1.0f;
    qd.uv[3][0] = 1.0f;
    qd.uv[3][1] = 1.0f;
    draw_geometry_quad(&qd, 0, 1);

    /* the sparkle vector reuses the first matrix row's slot in retail; its w is that row's 0 */
    izero = 0;
    zero = *(float *)&izero;
    for (i = 0; i < 4; i++) {
        float ang = (float)i * 3.14159f * 0.5f - 3.14159f;
        T[0].f[0] = fast_cos(ang) * sparkle_radius;
        T[0].f[1] = fast_sin(ang) * sparkle_radius;
        T[0].f[2] = 0.59f;
        transform_vector_by_basis(T[0].f, T[0].f, m + 0xC0);
        fast_vec_add(T[0].f, T[0].f, m + 0x10);
        draw_offset_textured_effect_quad(T[0].f, *(int *)(m + 0x90) & 0xFFFF0000, 0.1333f, zero);
    }
}

typedef struct {
    f32 f[4];
} V_2a57a8;

typedef struct {
    u8  p0[0x24];
    f32 f24;
} N_2a57a8;

typedef struct {
    u8        p0[0x10];
    f32       f10, f14, f18;
    u8        p1C[0x24 - 0x1C];
    N_2a57a8 *p24;
    u8        p28[4];
    f32       f2C;
    u8        p30[2];
    u16       h32;
    u16       h34;
    u8        p36[0x73 - 0x36];
    u8        b73;
} M_2a57a8;

typedef struct {
    s32       i0, i4;
    u8        p8[4];
    M_2a57a8 *mC;
    u8        p10[0x90 - 0x10];
    f32       f90;
} Q_2a57a8;

typedef struct {
    u8        p0[0x10];
    f32       f10, f14, f18;
    u8        p1C[4];
    u8        b20;
    u8        p21[0x52 - 0x21];
    u8        b52, b53;
    u8        p54[0x78 - 0x54];
    Q_2a57a8 *q;
    u8        p7C[0x90 - 0x7C];
    s32       i90;
} P_2a57a8;

typedef struct {
    u8  p0[0x80];
    f32 f80, f84;
    u8  p88[0x98 - 0x88];
    f32 f98;
    u8  p9C[0x2084 - 0x9C];
    s32 i2084;
    u8  p2088[4];
    s32 i208C;
    u8  p2090[0x20A4 - 0x2090];
    u8  b20A4;
} G_2a57a8;

extern V_2a57a8 hero_pos __asm__("D_0013F3D0") NOT_SDA;
extern s32      D_0013CAE4_lui __asm__("D_0013CAE4") NOT_SDA;
extern s32      D_L00_0015F5CC_2a57a8 __asm__("D_L00_0015F5CC");
void            FUN_L00_002a5040_2a57a8(void) __asm__("FUN_L00_002a5040");

#include "rnc/overlay/shared/mobys/camera_object_data/vendor_update.h"

void vendor_update(P_2a57a8 *p) __asm__("FUN_L00_002a57a8");

void vendor_update(P_2a57a8 *vendor_moby) {
    enum {
        VENDOR_INITIALIZE = 0,
        VENDOR_WAIT_FOR_HERO = 1,
        VENDOR_HERO_NEARBY = 2,
        VENDOR_ZERO_AUXILIARY_SCALE = 3
    };
    Q_2a57a8 *q = vendor_moby->q;
    s32       i, interaction_allowed, help_message_allowed, help_message_succeeded;
    u8       *v;
    f32       hero_distance, z;

    vendor_color_pulse_phase += 0.05f;
    if (vendor_color_pulse_phase > 3.14f)
        vendor_color_pulse_phase = -3.14f;
    vendor_moby->i90 =
        (((s32)(fast_sin(vendor_color_pulse_phase) * 48.0f) + 0x60) * 0x10101) | 0x80000000;
    switch (vendor_moby->b20) {
    case VENDOR_INITIALIZE:
        vendor_moby->b20 = 1;
        q->i0 = 0;
        q->i4 = 0;
        q->mC = create_moby(0x477);
        q->mC->h34 |= 2;
        q->mC->h32 = 0x40;
        q->mC->f10 = vendor_moby->f10;
        q->mC->f14 = vendor_moby->f14;
        q->mC->f18 = vendor_moby->f18 + 2.95f;
        q->mC->b73 = 0x20;
        q->f90 = 0.0f;
        for (i = 0; i < 2; i++) {
            attach_manipulator(q->mC, i, (u8 *)q + 0x10 + i * 0x40);
        }
        update_entity(q->mC);
        break;
    case VENDOR_WAIT_FOR_HERO:
        if (q->f90 > 0.0f)
            q->f90 -= 0.1f;
        if (q->f90 <= 0.0f)
            q->mC->h34 |= 1;
        if (!(D_L00_0015F5CC_2a57a8 & 7) &&
            vector_distance_xy(&vendor_moby->f10, &hero_pos) <= 16.0f &&
            absolute_float(vendor_moby->f18 - hero_pos.f[2]) <= 8.0f) {
            vendor_moby->b20 = 2;
            if (vendor_moby->b53 != 1)
                blend_moby_animation(vendor_moby, 1, 0, 10);
            q->mC->h34 &= 0xFFFE;
        }
        add_draw_callback(FUN_L00_002a5040_2a57a8, vendor_moby);
        rotation_2D5FA8((u8 *)q + 0x20, 2, vendor_manipulator_rotation_phase);
        rotation_2D5FA8((u8 *)q + 0x60, 2, vendor_manipulator_counter_rotation_phase);
        vendor_manipulator_rotation_phase =
            fast_add_rotations(vendor_manipulator_rotation_phase, 0.01f);
        vendor_manipulator_counter_rotation_phase =
            fast_add_rotations(vendor_manipulator_counter_rotation_phase, -0.02f);
        q->mC->f2C = q->f90 * q->mC->p24->f24 * 2.5f;
        update_entity(q->mC);
        break;
    case VENDOR_HERO_NEARBY:
        if (q->f90 < 1.0f)
            q->f90 += 0.1f;
        hero_distance = vector_distance_xy(&vendor_moby->f10, &hero_pos);
        z = absolute_float(vendor_moby->f18 - hero_pos.f[2]);
        if (!(D_L00_0015F5CC_2a57a8 & 7) && (hero_distance > 18.0f || z > 10.0f)) {
            vendor_moby->b20 = 1;
            if (vendor_moby->b53)
                blend_moby_animation(vendor_moby, 0, 0, 10);
            break;
        }
        add_draw_callback(FUN_L00_002a5040_2a57a8, vendor_moby);
        interaction_allowed = 1;
        help_message_allowed = 1;
        rotation_2D5FA8((u8 *)q + 0x20, 2, vendor_manipulator_rotation_phase);
        rotation_2D5FA8((u8 *)q + 0x60, 2, vendor_manipulator_counter_rotation_phase);
        vendor_manipulator_rotation_phase =
            fast_add_rotations(vendor_manipulator_rotation_phase, 0.01f);
        vendor_manipulator_counter_rotation_phase =
            fast_add_rotations(vendor_manipulator_counter_rotation_phase, -0.02f);
        q->mC->f2C = q->f90 * q->mC->p24->f24 * 2.5f;
        update_entity(q->mC);
        if (hero.state.control_mode != 0 && hero.state.current != 3 &&
            hero.state.control_mode != 1) {
            interaction_allowed = 0;
            help_message_allowed = 0;
        }
        if (hero.state.current == 0x1D || hero.state.current == 0x32) {
            interaction_allowed = 0;
            help_message_allowed = 0;
        }
        if (hero.vehicle_mode) {
            interaction_allowed = 0;
            help_message_allowed = 0;
        }
        if (vendor_moby->b52 != 1)
            interaction_allowed = 0;
        if (hero_distance > 4.0f || z > 2.0f) {
            interaction_allowed = 0;
            help_message_allowed = 0;
        }
        if (fast_difference_between_rotations(
                hero.motion.rot.f[2],
                fast_arc_tan(vendor_moby->f10 - hero.motion.pos.f[0],
                             vendor_moby->f14 - hero.motion.pos.f[1])) > 1.5707964f) {
            interaction_allowed = 0;
            help_message_allowed = 0;
        }
        help_message_succeeded = 0;
        if (help_message_allowed)
            help_message_succeeded = try_set_help_message(1, 0x53E3) != 0;
        if ((D_0013CAE4_lui & 0x10) && interaction_allowed && help_message_succeeded) {
            open_vendor_menu(vendor_moby);
            vendor_moby->b20 = 3;
        }
        break;
    case VENDOR_ZERO_AUXILIARY_SCALE:
        if (q->mC) {
            q->f90 = 0.0f;
            q->mC->f2C = q->mC->p24->f24 * q->f90 * 2.5f;
            update_entity(q->mC);
        }
        break;
    }
}

#include "qcopy.h"
#include "rnc/math/vector.h"
#include "rnc/hero/hero.h"
#include "rnc/overlay/collision.h"

typedef struct {
    u8  pad0[0x24];
    f32 scale; /* 0x24 */
} BoltClass;

struct Bolt;

typedef struct {
    Vec4         pos;  /* 0x00 */
    Vec4         rot;  /* 0x10 */
    Vec4         vel;  /* 0x20 */
    Vec4         spin; /* 0x30 */
    u8           pad40[8];
    f32          unk48;       /* 0x48 */
    f32          unk4C;       /* 0x4C */
    f32          unk50;       /* 0x50 */
    u8           landed;      /* 0x54 */
    u8           unk55;       /* 0x55 */
    u8           hop_frames;  /* 0x56 */
    u8           hop_left;    /* 0x57 */
    f32          unk58;       /* 0x58 */
    void        *ground_moby; /* 0x5C */
    struct Bolt *unk60;       /* 0x60 */
    f32          speed;       /* 0x64 */
    f32          ground_z;    /* 0x68 */
    s16          slot;        /* 0x6C */
    s16          slot_timer;  /* 0x6E */
} BoltVars;

typedef struct Bolt {
    u8         pad0[0x10];
    Vec4       pos;   /* 0x10 */
    u8         state; /* 0x20 */
    u8         pad21[3];
    BoltClass *pclass; /* 0x24 */
    u8         pad28[4];
    f32        scale; /* 0x2C */
    u8         unk30; /* 0x30 */
    u8         unk31; /* 0x31 */
    u16        unk32; /* 0x32 */
    u16        flags; /* 0x34 */
    u8         pad36[0x3D];
    u8         unk73; /* 0x73 */
    u8         pad74[4];
    BoltVars  *vars; /* 0x78 */
    u8         pad7C[0x2A];
    s16        oclass; /* 0xA6 */
    u8         padA8[8];
    u8         unkB0; /* 0xB0 */
    u8         padB1;
    s16        unkB2; /* 0xB2 */
    u8         padB4[0xC];
    Vec4       rot; /* 0xC0 */
    u8         padD0[0x10];
    Vec4       unkE0; /* 0xE0 */
} Bolt;

typedef struct {
    u8 pad0[0x454];
    u8 killed[1]; /* 0x454: indexed by the moby save id */
} MobyKillTable;

typedef struct {
    Vec4 pos;
    s16  pad10[2];
    s16  moby; /* 0x14 */
    u8   pad16[0xA];
} BoltTrailSlot;

extern s32                         D_L00_0015F5C4;
extern CollisionHit D_L00_00173E40 NOT_SDA;
extern Vec4          l00_bolt_glint_local_offsets_by_class[] __asm__("D_L00_001D73D0");
extern BoltTrailSlot glints[] __asm__("D_L00_0016EA40");
extern u8           *D_L00_0015FFDC;
extern s32           D_L00_0015FFBC;
extern u8           *moby_table_macro __asm__("D_L00_0015FFD8") MACRO_ADDR;
extern u8            mission_state_macro[] __asm__("D_L00_0015FC88") MACRO_ADDR;
extern s32           D_0015ED84;
extern MobyKillTable D_L00_001BA5D0 NOT_SDA;
extern MobyKillTable D_L00_001BB230 NOT_SDA;
extern u8                           D_0014C050[][16];
extern f32                          D_0015ED6C;

extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern s32  FUN_001efa68(void *, void *, s32, s32, s32);
extern void approach_value(void *, f32, f32) __asm__("FUN_00213ed8");
extern s32  FUN_L00_001f0d60(void *, s32, void *, f32);
extern s32  allocate_glint_slot(void *, s32, f32) __asm__("FUN_L00_001fe990");
extern s32  FUN_L00_002603d0(void *);
extern void convert_pose_to_reference_local(void *, void *, void *, void *, void *, void *) __asm__("FUN_L00_00260668");
extern s32  FUN_L00_00257b90(s32, s32);
extern void build_quaternion_from_axis_rotations(void *, void *) __asm__("FUN_L00_00259f50");
extern void FUN_L00_0025f730(void *, f32);
extern void FUN_L00_002a6de8_c(Bolt *, void *, s32) __asm__("FUN_L00_002a6de8");
extern void FUN_L00_002a6b70_c(Bolt *) __asm__("FUN_L00_002a6b70");
extern void FUN_L00_002a7210_c(Bolt *, void *, f32, f32) __asm__("FUN_L00_002a7210");

void bolt_update(Bolt *obj) __asm__("FUN_L00_002a5dd8");

void bolt_update(Bolt *bolt_moby) {
    enum {
        BOLT_INITIALIZE = 0x0,
        BOLT_FALLING = 0x1,
        BOLT_SETTLING = 0x2,
        BOLT_IDLE = 0x3,
        BOLT_MOVE_TO_HERO = 0x4
    };
    BoltVars *v = bolt_moby->vars;
    Vec4      geometry_scratch;
    Vec4      vector_scratch;
    Vec4      rotation_scratch;
    Vec4      collection_offset;
    Vec4      attraction_rotation_angles;
    Vec4      attraction_rotation_basis[3];
    s32       hit;
    f32       sc;
    f32       hr;
    f32       settling_inclination_limit;
    f32       k;
    f32       velocity_decay_step;
    f32       u;
    f32       settling_hop_phase;
    f32       hero_distance;
    f32       x;
    f32       rotation_blend_step;
    s32       flag;
    u8        i;

    if (D_L00_0015F5C4 == 2) {
        bolt_moby->unk31 = 0;
        bolt_moby->flags |= 1;
    } else {
        bolt_moby->unk31 = 1;
        bolt_moby->flags &= 0xFFFE;
    }
    switch (bolt_moby->state) {
    case BOLT_INITIALIZE:
        bolt_moby->state = 3;
        bolt_moby->unk73 = 0x14;
        bolt_moby->flags |= 0x100;
        bolt_moby->unk30 = 0x20;
        bolt_moby->unk32 = 0x20;
        if (random_integer_below(2) == 0) {
            v->unk55 |= 1;
        }
        fast_vec_normalize(&geometry_scratch, &bolt_moby->unkE0, 0.2f);
        fast_vec_add(&geometry_scratch, &geometry_scratch, &bolt_moby->pos);
        fast_vec_normalize(&vector_scratch, &bolt_moby->unkE0, -3.0f);
        fast_vec_add(&vector_scratch, &vector_scratch, &bolt_moby->pos);
        hit = FUN_001efa68(&geometry_scratch, &vector_scratch, 0x22, 0, 0);
        if (v->landed == 0 && hit != 0) {
            qcopy(&v->pos, &D_L00_00173E40.point);
            clear_u64_value(&v->rot);
            v->rot.f[2] = fast_arc_tan(D_L00_00173E40.normal_x, D_L00_00173E40.normal_y);
            v->rot.f[1] =
                fast_arc_tan(D_L00_00173E40.normal_z, vector_length_xy(&D_L00_00173E40.normal_x));
            if (D_L00_00173E40.moby != 0 && FUN_L00_002603d0((void *)D_L00_00173E40.moby) != 0) {
                void *gm = (void *)D_L00_00173E40.moby;
                v->ground_moby = gm;
                convert_pose_to_reference_local(bolt_moby, gm, v, &v->rot, v, &v->rot);
            }
        } else {
            qcopy(&v->pos, &bolt_moby->pos);
            clear_u64_value(&v->rot);
        }
        v->unk50 = random_angle_radians();
        v->unk58 = random_angle_radians();
        v->ground_z = bolt_moby->pos.f[2];
        v->slot_timer = FUN_L00_00257b90(0, scale_game_frames(600));
        v->slot = -1;
        switch (bolt_moby->oclass) {
        case BOLT_CLASS_ONE:
            bolt_moby->scale = bolt_moby->pclass->scale * 0.65f;
            break;
        case BOLT_CLASS_FIVE:
            bolt_moby->scale = bolt_moby->pclass->scale * 0.7f;
            break;
        case BOLT_CLASS_TWENTY:
            bolt_moby->scale = bolt_moby->pclass->scale * 0.75f;
            break;
        case BOLT_CLASS_FIFTY:
            bolt_moby->scale = bolt_moby->pclass->scale * 0.6f;
            break;
        }
        break;
    case BOLT_FALLING:
        v->vel.f[2] -= frame_time_sq * 10.8f;
        fast_vec_add(&bolt_moby->pos, &bolt_moby->pos, &v->vel);
        quaternion_to_rotation_basis(&v->spin, &rotation_scratch);
        multiply_matrix3x3(&bolt_moby->rot, &rotation_scratch, &bolt_moby->rot);
        switch (bolt_moby->oclass) {
        case BOLT_CLASS_ONE:
            hr = 0.15f;
            break;
        case BOLT_CLASS_FIVE:
            hr = 0.23f;
            break;
        case BOLT_CLASS_TWENTY:
            hr = 0.23f;
            break;
        default:
            hr = 0.25f;
            break;
        }
        if (bolt_moby->pos.f[2] < v->ground_z - 2.0f &&
            bolt_moby->pos.f[2] < hero.motion.pos.f[2] - 2.0f) {
            FUN_L00_002a7210_c(
                bolt_moby, hero.moby, random_float_between(-15.0f, 15.0f) * 0.017453292f, 0.0f);
            return;
        }
        if (FUN_L00_001f0d60(&bolt_moby->pos, 0x22, bolt_moby, hr) == 0) {
            return;
        }
        qcopy(&v->pos, &D_L00_00173E40.point);
        clear_u64_value(&v->rot);
        v->rot.f[2] = fast_arc_tan(D_L00_00173E40.normal_x, D_L00_00173E40.normal_y);
        v->rot.f[1] =
            fast_arc_tan(D_L00_00173E40.normal_z, vector_length_xy(&D_L00_00173E40.normal_x));
        fast_vec_sub(&geometry_scratch, &bolt_moby->pos, &D_L00_00173E40.point);
        reflect_vector(&v->vel, &v->vel, &geometry_scratch);
        if (vector_distance_xy(&bolt_moby->pos, &hero.motion.pos) < hero.reach_distance &&
            absolute_float(bolt_moby->pos.f[2] - hero.motion.pos.f[2]) < hero.reach_height) {
            if (hero.health.hp == 0) {
                return;
            }
            fast_vec_normalize(&v->vel, &v->vel, random_float_between(3.0f, 7.0f) * D_0015ED6C);
            FUN_L00_002a7210_c(bolt_moby, hero.moby, 0.0f, 0.0f);
            return;
        }
        settling_inclination_limit = 0.7853982f;
        if (!(fast_arc_tan(geometry_scratch.f[2], vector_length_xy(&geometry_scratch)) <
              settling_inclination_limit)) {
            return;
        }
        if (!(fast_arc_tan(D_L00_00173E40.normal_z, vector_length_xy(&D_L00_00173E40.normal_x)) <
              settling_inclination_limit)) {
            return;
        }
        bolt_moby->state = 2;
        v->hop_frames = v->hop_left =
            truncate_float_to_s32(scale_time(random_float_between(30.0f, 40.0f)));
        if (D_L00_00173E40.moby != 0 && FUN_L00_002603d0((void *)D_L00_00173E40.moby) != 0) {
            void *gm = (void *)D_L00_00173E40.moby;
            v->ground_moby = gm;
            convert_pose_to_reference_local(bolt_moby, gm, v, &v->rot, v, &v->rot);
        }
        matrix_to_euler(&bolt_moby->rot, &vector_scratch);
        build_quaternion_from_axis_rotations(&v->spin, &vector_scratch);
        return;
    case BOLT_SETTLING:
        FUN_L00_002a6de8_c(bolt_moby, &vector_scratch, 0);
        k = 0.25f;
        vector_scratch.f[2] += k;
        matrix_to_euler(&bolt_moby->rot, &geometry_scratch);
        build_quaternion_from_axis_rotations(&rotation_scratch, &geometry_scratch);
        rotation_blend_step = 1.0f / convert_integer_to_float(v->hop_left);
        settling_hop_phase =
            convert_integer_to_float(v->hop_left) / convert_integer_to_float(v->hop_frames);
        quaternion_nlerp(&v->spin, &v->spin, &rotation_scratch, rotation_blend_step);
        lerp_vector(&bolt_moby->pos, &bolt_moby->pos, &vector_scratch, settling_hop_phase);
        settling_hop_phase -= 0.5f;
        bolt_moby->pos.f[2] +=
            -(settling_hop_phase * settling_hop_phase - k) * ((f32)v->hop_frames * 0.025f);
        quaternion_to_rotation_basis(&v->spin, &bolt_moby->rot);
        if (--v->hop_left != 0) {
            return;
        }
        bolt_moby->state = 3;
        v->hop_frames = v->hop_left =
            truncate_float_to_s32(scale_time(random_float_between(10.0f, 20.0f)));
        return;
    case BOLT_IDLE:
        hero_distance = vector_distance_xy(&bolt_moby->pos, &hero.motion.pos);
        if (hero_distance < hero.reach_distance &&
            absolute_float(bolt_moby->pos.f[2] - hero.motion.pos.f[2]) < hero.reach_height) {
            FUN_L00_002a7210_c(bolt_moby, hero.moby, 0.0f, 0.0f);
            return;
        }
        if ((u8 *)bolt_moby > D_L00_0015FFDC && D_L00_0015FFBC < 200 && 48.0f < hero_distance &&
            bolt_moby->unk31 == 0) {
            mark_moby_for_removal(bolt_moby);
            return;
        }
        if (bolt_moby->unk31 != 0 || v->ground_moby != 0) {
            FUN_L00_002a6de8_c(bolt_moby, 0, 0);
            if (v->hop_left != 0) {
                u = convert_integer_to_float(v->hop_left) / convert_integer_to_float(v->hop_frames);
                u -= 0.5f;
                bolt_moby->pos.f[2] += -(u * u - 0.25f) * ((f32)v->hop_frames * 0.0125f);
                v->hop_left--;
            }
            if (v->slot >= 0) {
                transform_vector_by_basis(&geometry_scratch,
                                          &l00_bolt_glint_local_offsets_by_class[bolt_moby->oclass],
                                          &bolt_moby->rot);
                fast_vec_add(&glints[v->slot], &geometry_scratch, &bolt_moby->pos);
                if (glints[v->slot].moby != (((u8 *)bolt_moby - moby_table_macro) >> 8)) {
                    v->slot = -1;
                }
                return;
            }
            if (--v->slot_timer > 0) {
                return;
            }
            transform_vector_by_basis(&geometry_scratch,
                                      &l00_bolt_glint_local_offsets_by_class[bolt_moby->oclass],
                                      &bolt_moby->rot);
            fast_vec_add(&geometry_scratch, &geometry_scratch, &bolt_moby->pos);
            v->slot = allocate_glint_slot(
                &geometry_scratch, ((u8 *)bolt_moby - moby_table_macro) >> 8, 1.4f);
            v->slot_timer = FUN_L00_00257b90(scale_game_frames(300), scale_game_frames(600));
        } else if (v->hop_left != 0) {
            v->hop_left--;
        }
        return;
    case BOLT_MOVE_TO_HERO:
        flag = 0;
        if (v->unk60->oclass == 0) {
            fast_vec_sub(&geometry_scratch, &bolt_moby->pos, &hero.ground_point);
            transform_vector(&geometry_scratch, &geometry_scratch, hero.inv_rot_matrix);
            if (geometry_scratch.f[2] < 0.0f) {
                flag = 1;
            }
        }
        if (flag) {
            fast_vec_normalize(&geometry_scratch, &hero.rot_matrix[8], D_0015ED6C * 7.0f);
            fast_vec_sub(&vector_scratch, &geometry_scratch, &v->vel);
            FUN_L00_0025f730(&vector_scratch, frame_time_sq * 50.0f);
            fast_vec_add(&v->vel, &v->vel, &vector_scratch);
        } else {
            velocity_decay_step = frame_time_sq * 10.0f;
            if (velocity_decay_step < fast_vec_length(&v->vel)) {
                fast_vec_normalize(&geometry_scratch, &v->vel, velocity_decay_step);
                fast_vec_sub(&v->vel, &v->vel, &geometry_scratch);
            }
            approach_value(&v->speed, D_0015ED6C * 48.0f, frame_time_sq * 16.0f);
            x = D_0015ED6C * 8.0f;
            if (x < hero.motion.speed_xy) {
                fast_vec_normalize(
                    &vector_scratch, &hero.motion.displacement, (hero.motion.speed_xy - x) * 0.5f);
                fast_vec_add(&bolt_moby->pos, &bolt_moby->pos, &vector_scratch);
            }
        }
        quaternion_to_rotation_basis(&v->spin, &geometry_scratch);
        multiply_matrix3x3(&bolt_moby->rot, &geometry_scratch, &bolt_moby->rot);
        fast_vec_add(&bolt_moby->pos, &bolt_moby->pos, &v->vel);
        fast_vec_sub(&collection_offset, &hero.motion.body_center, &bolt_moby->pos);
        if (v->speed < fast_vec_length(&collection_offset)) {
            if (!(v->unk4C == 0.0f && v->unk48 == 0.0f)) {
                transform_vector_by_basis(
                    &collection_offset, &collection_offset, hero.inv_rot_matrix);
                clear_u64_value(&attraction_rotation_angles);
                attraction_rotation_angles.f[1] = v->unk48;
                attraction_rotation_angles.f[2] = v->unk4C;
                euler_rotation_basis(attraction_rotation_basis, &attraction_rotation_angles);
                transform_vector_by_basis(
                    &collection_offset, &collection_offset, attraction_rotation_basis);
                transform_vector_by_basis(&collection_offset, &collection_offset, hero.rot_matrix);
            }
            fast_vec_normalize(&collection_offset, &collection_offset, v->speed);
            fast_vec_add(&bolt_moby->pos, &bolt_moby->pos, &collection_offset);
            return;
        }
        FUN_L00_002a6b70_c(bolt_moby);
        if ((u8 *)bolt_moby < D_L00_0015FFDC) {
            D_L00_001BA5D0.killed[bolt_moby->unkB2] = bolt_moby->unkB0 + 2;
            if (bolt_moby->unkB0 == 0xFF || (mission_state_macro[bolt_moby->unkB0] != 0xFF &&
                                             D_0014C050[D_0015ED84][bolt_moby->unkB0] == 0xFF)) {
                D_L00_001BB230.killed[bolt_moby->unkB2] = bolt_moby->unkB0 + 2;
            }
        }
        mark_moby_for_removal(bolt_moby);
        return;
    }
}

/* What a bolt moby's unkB8 points at: byte 0xB1 is its source's per-level id. */
typedef struct {
    u8 pad_00[0xB1];
    u8 id; /* 0x80 set: no id */
} BoltSource;

extern s32 D_0015EE2C; /* bolts counted when the bolt's pvars[0x40] is set or it is an early moby */
/* second name for D_0015ED84: retail loads the level index again here */
extern s32   level_bolts[] __asm__("D_0013DF38"); /* bolts collected per level */
extern s32   D_L00_0015F598;
extern void *D_L00_0015FFDC_u __asm__("D_L00_0015FFDC");
extern f32   D_0015ED6C; /* frame-rate scale */
extern void
queue_animation_update(s32, s32, void *, void *, void *, void *, s32) __asm__("FUN_001ff308");
/* spawns a sparkle particle */
extern u8 *FUN_L00_00272060(
    void *pos, s32 life, f32 size0, f32 size1, s32 color, s32 mode, s32 spin, void *vel, f32 f2);

/*
 * CollectBolt: adds the bolt moby's value (1, or 5 / 20 / 50 for classes
 * 0xE / 0xF / 0x10) to the bolt count, the level's total and its source's
 * stats, shows the bolt counter and spawns two sparkles at the bolt.
 */
void collect_bolt(struct Moby *bolt) __asm__("FUN_L00_002a6b70");

void collect_bolt(struct Moby *bolt) {
    Vec4        sparkle_initial_velocity;
    Vec4        pos;
    s32         value;
    s32         spin;
    f32         k;
    f32         size;
    BoltSource *bolt_source;

    switch (bolt->oclass) {
    default:
    case 0:
        value = 1;
        break;
    case BOLT_CLASS_FIVE:
        value = 5;
        break;
    case BOLT_CLASS_TWENTY:
        value = 20;
        break;
    case BOLT_CLASS_FIFTY:
        value = 50;
        break;
    }

    bolt_source = bolt->parent;
    if (bolt_source && !(bolt_source->id & 0x80))
        bolt_source_stats[current_level_index][bolt_source->id].bolts += value;
    current_bolt_count += value;
    if (bolt->pvars[0x40] || (D_L00_0015F598 && (void *)bolt < D_L00_0015FFDC_u))
        D_0015EE2C += value;
    level_bolts[current_level_index] += value;

    k = 0.7f;
    queue_animation_update(2,
                           0x754E,
                           initialize_bolt_counter_hud,
                           update_bolt_counter_hud_animation_and_visibility,
                           draw_bolt_counter_hud_panel,
                           &current_bolt_count,
                           9999999);

    /* two sparkles 10 units out along a random direction, spinning opposite ways */
    random_vector(&sparkle_initial_velocity, D_0015ED6C * k, D_0015ED6C);
    fast_vec_scale(&pos, &sparkle_initial_velocity, 10.0f);
    fast_vec_add(&pos, &pos, &bolt->pos);
    spin = random_integer_below(2);
    if (spin == 0)
        spin--;
    size = random_float_between(0.4f, 0.5f);
    FUN_L00_00272060(&pos,
                     scale_game_frames(0x19),
                     size * 0.2f,
                     size,
                     0x7F207F7F,
                     0,
                     spin,
                     &sparkle_initial_velocity,
                     0.0f);
    spin = -spin;
    FUN_L00_00272060(&pos,
                     scale_game_frames(0x19),
                     size * 0.14f,
                     size * k,
                     0x7F7F7F7F,
                     1,
                     spin,
                     &sparkle_initial_velocity,
                     0.0f);
}

/* Steers a thrown moby along its path: builds its orientation from the data's angles and, when done, releases the held target. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A8088), where it is exact; names translated to the US level program. */

extern char *FUN_L00_002603d0_c(void *) __asm__("FUN_L00_002603d0");
extern int   coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int   transform_attachment_pose(
      int x, char *o, float *p, float *q, float *r, float *s) __asm__("FUN_L00_002604f0");

void update_bolt_grounded_pose(char *moby, void *out1, void *out2) __asm__("FUN_L00_002a6de8");

void update_bolt_grounded_pose(char *moby, void *out1, void *out2) {
    char *data = ((char *)MOBY(moby)->pvars);
    float local_rotation_basis[12];
    float base_rotation_basis[12];
    float local_rotation_angles[4];
    float rotated_position_offset[4];
    float A[4];
    float B[4];
    float D[4];
    float C[4];
    float rotation_angle_step, class_local_rotation_angle, base_axis_offset_distance,
        rotated_axis_offset_distance, ground_collision_radius;

    if (*(char **)(data + 0x5C) != 0) {
        transform_attachment_pose(
            (int)moby, *(char **)(data + 0x5C), (float *)data, (float *)(data + 0x10), A, B);
    } else {
        qcopy(A, data);
        qcopy(B, data + 0x10);
    }
    switch (MOBY(moby)->oclass) {
    case BOLT_CLASS_ONE:
        rotation_angle_step = 0.04f;
        class_local_rotation_angle = -0.25f;
        base_axis_offset_distance = 0.04875f;
        rotated_axis_offset_distance = 0.325f;
        ground_collision_radius = 0.15f;
        break;
    case BOLT_CLASS_FIVE:
        rotation_angle_step = 0.03f;
        class_local_rotation_angle = -0.5f;
        base_axis_offset_distance = 0.0245f;
        rotated_axis_offset_distance = 0.35f;
        ground_collision_radius = 0.23f;
        break;
    case BOLT_CLASS_TWENTY:
        rotation_angle_step = 0.025f;
        class_local_rotation_angle = -0.4f;
        base_axis_offset_distance = 0.05625f;
        rotated_axis_offset_distance = 0.375f;
        ground_collision_radius = 0.23f;
        break;
    default:
        rotation_angle_step = 0.0175f;
        class_local_rotation_angle = -0.35f;
        base_axis_offset_distance = 0.135f;
        rotated_axis_offset_distance = 0.3f;
        ground_collision_radius = 0.25f;
        break;
    }
    if (*(unsigned char *)(data + 0x55) & 1) {
        *(float *)(data + 0x50) = fast_add_rotations(*(float *)(data + 0x50), rotation_angle_step);
        *(float *)(data + 0x58) = fast_add_rotations(*(float *)(data + 0x58), -0.0125f);
    } else {
        *(float *)(data + 0x50) =
            fast_subtract_rotations(*(float *)(data + 0x50), rotation_angle_step);
        *(float *)(data + 0x58) = fast_subtract_rotations(*(float *)(data + 0x58), -0.0125f);
    }
    qcopy(local_rotation_angles, data + 0x50);
    local_rotation_angles[1] = class_local_rotation_angle;
    euler_rotation_basis(local_rotation_basis, local_rotation_angles);
    euler_rotation_basis(base_rotation_basis, B);
    multiply_matrix3x3(moby + 0xC0, base_rotation_basis, local_rotation_basis);
    fast_vec_scale(moby + 0x10, &base_rotation_basis[8], base_axis_offset_distance);
    fast_vec_scale(rotated_position_offset, moby + 0xC0, rotated_axis_offset_distance);
    fast_vec_add(moby + 0x10, moby + 0x10, rotated_position_offset);
    fast_vec_add(moby + 0x10, moby + 0x10, A);
    if (out1 != 0)
        qcopy(out1, A);
    if (out2 != 0)
        qcopy(out2, B);
    if (MOBY(moby)->state == 3) {
        char *ground_moby = *(char **)(data + 0x5C);
        if (ground_moby != 0 && *(unsigned char *)(data + 0x57) == 0) {
            int   should_detach_from_ground;
            char *x;
            if ((unsigned char)ground_moby[0x20] == 0xFE ||
                (*(unsigned char **)(data + 0x5C))[0x20] == 0xFD) {
                should_detach_from_ground = 1;
            } else {
                should_detach_from_ground =
                    coll_sphere(ground_collision_radius, moby + 0x10, 0x22, moby) == 0;
            }
            if (should_detach_from_ground) {
                moby[0x20] = 1;
                clear_u64_value(data + 0x30);
                *(float *)(data + 0x3C) = 1.0f;
                x = FUN_L00_002603d0_c(*(char **)(data + 0x5C));
                if (x != 0) {
                    qcopy(data + 0x20, x + 0x10);
                } else {
                    clear_u64_value(data + 0x20);
                }
                fast_vec_normalize(C, &base_rotation_basis[8], D_0015ED6C);
                fast_vec_normalize(D, (float *)&MOBY(moby)->rot_row0, D_0015ED6C);
                fast_vec_add(C, C, D);
                fast_vec_add(data + 0x20, data + 0x20, C);
                fast_vec_add(moby + 0x10, moby + 0x10, data + 0x20);
                *(char **)(data + 0x5C) = 0;
            }
        }
    }
}

extern int   D_15F5CC_2a7210 __asm__("D_L00_0015F5CC");
extern float D_15ED6C_2a7210 __asm__("D_0015ED6C");
extern void  build_quaternion_from_axis_angle(void *, void *, float) __asm__("FUN_00214530");

extern int   last_bolt_attraction_sound_frame_gp __asm__("D_L00_001613AC") __attribute__((sda));
extern float bolt_spin_angle_limit_degrees_gp __asm__("D_L00_001613A8") __attribute__((sda));
int          begin_bolt_attraction(void *ov,
                                   void *tv,
                                   float attraction_yaw_angle,
                                   float attraction_pitch_angle) __asm__("FUN_L00_002a7210");

int begin_bolt_attraction(void *ov,
                          void *tv,
                          float attraction_yaw_angle,

                          float attraction_pitch_angle) {
    char *bolt_moby = ov;
    char *target_moby = tv;
    char *bolt_vars = ((char *)MOBY(bolt_moby)->pvars);
    int   sound_cooldown_frames, last, now;
    float k;
    float attraction_geometry_scratch[3][4] __attribute__((aligned(16)));

    if (MOBY(bolt_moby)->state == 4) {
        return 0;
    }
    sound_cooldown_frames = scale_game_frames(3);
    last = last_bolt_attraction_sound_frame_gp;
    now = D_15F5CC_2a7210;
    if (last + sound_cooldown_frames < now || now < last) {
        last_bolt_attraction_sound_frame_gp = now;
        allocate_voice_for_target_entry(0, 0x20, bolt_moby);
    } else {
        if (attraction_yaw_angle == 0.0f)
            attraction_yaw_angle = random_float_between(-30.0f, 30.0f) * DEG_TO_RAD;
        if (attraction_pitch_angle == 0.0f)
            attraction_pitch_angle = random_float_between(0.0f, 30.0f) * DEG_TO_RAD;
    }
    if (MOBY(bolt_moby)->state == 3) {
        euler_rotation_basis(attraction_geometry_scratch, bolt_vars + 0x10);
        fast_vec_normalize(bolt_vars + 0x20,
                           attraction_geometry_scratch[2],
                           random_float_between(3.0f, 7.0f) * D_15ED6C_2a7210);
    }
    {
        *(float *)(bolt_vars + 0x4C) = attraction_yaw_angle;
        *(float *)(bolt_vars + 0x48) = attraction_pitch_angle;
        *(int *)(bolt_vars + 0x64) = 0;
        *(char **)(bolt_vars + 0x60) = target_moby;
        fast_vec_sub(attraction_geometry_scratch[1], target_moby + 0x10, bolt_moby + 0x10);
        fast_vec_cross(
            attraction_geometry_scratch[0], bolt_vars + 0x20, attraction_geometry_scratch[1]);
        fast_vec_normalize(bolt_vars + 0x30, attraction_geometry_scratch[0], 1.0f);
        k = random_float_between(bolt_spin_angle_limit_degrees_gp * 0.333000004f,
                                 bolt_spin_angle_limit_degrees_gp) *
            DEG_TO_RAD;
        build_quaternion_from_axis_angle(bolt_vars + 0x30, bolt_vars + 0x30, k * D_15ED6C_2a7210);
    }
    MOBY(bolt_moby)->state = 4;
    return 1;
}

/* Vars of a bolt moby (classes 0xD..0x10). */
typedef struct {
    u8  pad_00[0x20];
    f32 vel[4] __attribute__((aligned(16)));  /* 0x20 */
    f32 spin[4] __attribute__((aligned(16))); /* 0x30: per-frame rotation (quaternion) */
    u8  counted; /* 0x40: CollectBolt also adds the bolt to D_0015EE2C */
    u8  pad_41[0xF];
    f32 unk50; /* random angle */
    u8  pad_54;
    u8  unk55; /* bit 0 set at random */
    u8  pad_56[2];
    f32 unk58; /* random angle */
    u8  pad_5C[0xC];
    f32 start_z; /* 0x68 */
    s16 unk6C;
    s16 unk6E; /* random 0 .. 600 frames */
} BoltVars_u;

extern void *D_001413D0 NOT_SDA;
extern void             FUN_L00_001ff500(void *, void *, float);

/*
 * Spawns a bolt worth `value` (1, 5, 20 or 50) at `pos` with velocity `vel`,
 * coloured like `src`, spinning about vel x (src - pos). flags & 2 sends it
 * toward D_001413D0 (& 0x10 with a random angle). When no moby is free the
 * value goes straight into the bolt count.
 */
struct Moby *spawn_bolt_by_value(struct Moby *bolt_source,
                                 void        *pos,
                                 void        *vel,
                                 int          flags,
                                 int          value,
                                 int          counted) __asm__("FUN_L00_002a7438");

struct Moby *spawn_bolt_by_value(

    struct Moby *bolt_source, void *pos, void *vel, int flags, int value, int counted) {
    enum {
        BOLT_VALUE_ONE = 1,
        BOLT_VALUE_FIVE = 5,
        BOLT_VALUE_TWENTY = 20,
        BOLT_VALUE_FIFTY = 50
    };
    f32          spin_axis[4] __attribute__((aligned(16)));
    f32          bolt_to_source_offset[4] __attribute__((aligned(16)));
    struct Moby *spawned_bolt;
    BoltVars_u  *bolt_vars;
    struct Moby *root;
    u8          *color;
    float        scale;
    float        spin;
    float        angle;

    switch (value) {
    default:
    case BOLT_VALUE_ONE:
        spawned_bolt = (struct Moby *)create_moby(0xD);
        scale = 0.65f;
        break;
    case BOLT_VALUE_FIVE:
        spawned_bolt = (struct Moby *)create_moby(0xE);
        scale = 0.7f;
        break;
    case BOLT_VALUE_TWENTY:
        spawned_bolt = (struct Moby *)create_moby(0xF);
        scale = 0.75f;
        break;
    case BOLT_VALUE_FIFTY:
        spawned_bolt = (struct Moby *)create_moby(0x10);
        scale = 0.6f;
        break;
    }
    if (0 != spawned_bolt) {
        bolt_vars = (BoltVars_u *)spawned_bolt->pvars;
        bolt_vars->counted = counted;
        /* the bolt source is the root of src's unkB8 chain */
        root = bolt_source;
        if (bolt_source != 0) {
            while (root->parent != 0)
                root = root->parent;
        }
        spawned_bolt->parent = root;
        spawned_bolt->unk73 = 20;
        spawned_bolt->flags |= 0x100;
        spawned_bolt->update_dist = 0x40;
        spawned_bolt->draw_dist = 0x40;
        spawned_bolt->visible = 1;
        spawned_bolt->state = 1;
        spawned_bolt->scale = spawned_bolt->pclass->scale * scale;
        euler_rotation_basis(&spawned_bolt->rot_row0, &spawned_bolt->rot);
        if ((bolt_source->flags & 0x20) && (color = *(u8 **)(bolt_source->pvars + 0xC)) != 0)
            set_moby_color(spawned_bolt, color[4], color[5], color[6]);
        else
            spawned_bolt->spawn_frame = bolt_source->spawn_frame;
        bolt_vars->unk6E = random_int_between(0, scale_game_frames(600));
        bolt_vars->unk6C = -1;
        qcopy(&spawned_bolt->pos, pos);
        qcopy(bolt_vars->vel, vel);
        if (random_integer_below(2))
            bolt_vars->unk55 |= 1;
        bolt_vars->unk50 = random_angle_radians();
        bolt_vars->unk58 = random_angle_radians();
        bolt_vars->start_z = spawned_bolt->pos.z;
        spin = random_float_between(0.0f, bolt_spin_angle_limit_degrees_gp) * 0.017453292f *
               D_0015ED6C;
        fast_vec_sub(bolt_to_source_offset, &bolt_source->pos, &spawned_bolt->pos);
        fast_vec_cross(spin_axis, bolt_vars->vel, bolt_to_source_offset);
        FUN_L00_001ff500(bolt_vars->spin, spin_axis, 1.0f);
        build_quaternion_from_axis_angle(bolt_vars->spin, bolt_vars->spin, spin);
        if (flags & 2) {
            angle = 0.0f;
            if (flags & 0x10)
                angle = random_float_between(-30.0f, 30.0f) * 0.017453292f;
            begin_bolt_attraction(spawned_bolt, D_001413D0, angle, 0.0f);
        }
        update_entity((u8 *)spawned_bolt);
    } else {
        current_bolt_count += value;
    }
    return spawned_bolt;
}

#include "qcopy.h"

typedef int q_2a7780 __attribute__((mode(TI)));

typedef struct {
    s16       sel[4];
    u32       col[4][4];
    f32       f48;
    q_2a7780 *verts;
    s16 (*idx)[4][2];
    f32 *uv;
    s16  n;
    f32  f5C;
} E_2a7780;

typedef struct {
    q_2a7780 m[3];
    q_2a7780 m30;
    u8       pad[0x30];
    f32      f70;
    u8       pad2[4];
    s16      h78;
} R_2a7780;

typedef struct {
    u8        pad[0x78];
    R_2a7780 *r;
} O_2a7780;

typedef struct {
    u8  pad[0x2080];
    u8 *p2080;
} P_2a7780;

typedef struct {
    q_2a7780 pos[4];
    u32      col[4];
    f32      uv[4][2];
    u64      tag[4];
} V_2a7780;

extern P_2a7780 D_0013F350_2a7780 __asm__("D_0013F350");
extern int      game_frame __asm__("D_L00_0015F5CC");
extern int      FUN_001f44b8_2a7780(int) __asm__("FUN_001f44b8");
extern void     FUN_001f7d30_2a7780(void *, void *, int) __asm__("FUN_001f7d30");
#define P_2a7780_G D_0013F350_2a7780

extern E_2a7780 effects[] __asm__("D_L00_001D92E8");
extern int      last_frame __asm__("D_L00_001613C0");
extern f32      uv_scale __asm__("D_L00_001613BC") __attribute__((sda));
/* Draws the animated quad-strip effect attached to o; the effect set is chosen by state 0x17..0x19 / 0x2B. */
void draw_wrench_swing_effect(O_2a7780 *effect_owner) __asm__("FUN_L00_002a7780");

void draw_wrench_swing_effect(O_2a7780 *effect_owner) {
    R_2a7780 *r;
    E_2a7780 *effect_config;
    V_2a7780  quad;
    s32       sel0, sel1, sel2, sel3;
    u32       color;
    f32       scale, alpha;
    s32       effect_texture_id, idx, nv;
    s32       k;
    s32       i, j, elapsed_frames, current_frame;
    f32       effect_progress, uv_v_offset;

    r = effect_owner->r;
    if (r == 0)
        return;
    if (r->f70 == 0.0f)
        return;
    if (r->f70 == 1.0f) {
        if (P_2a7780_G.p2080[0x52] != P_2a7780_G.p2080[0x53]) {
            r->f70 = 0.0f;
            return;
        }
        if (P_2a7780_G.p2080[0x52] == 0xFF) {
            r->f70 = 0.0f;
            return;
        }
    }
    scale = 1.0f;
    effect_texture_id = 0x14;
    nv = 1;
    color = 0x808080;
    alpha = scale;
    if (!((u32)(P_2a7780_G.p2080[0x52] - 0x17) < 3 || P_2a7780_G.p2080[0x52] == 0x2B))
        k = r->h78;
    else
        k = P_2a7780_G.p2080[0x52];
    if (k == 0x17) {
        r->h78 = k;
        idx = 0;
    } else if (k == 0x18) {
        r->h78 = k;
        idx = 1;
    } else if (k == 0x19) {
        r->h78 = k;
        idx = 2;
    } else {
        r->h78 = 0x2B;
        alpha = 0.7f;
        idx = 3;
        scale = 1.25f;
        effect_texture_id = 0xC;
        nv = 2;
        color = 0x408080;
    }
    effect_progress = r->f70;
    effect_config = &effects[idx];
    sel0 = effect_config->sel[0];
    sel1 = effect_config->sel[1];
    sel2 = effect_config->sel[2];
    sel3 = effect_config->sel[3];
    if (effect_progress == 1.0f) {
        u8 *hero_moby = P_2a7780_G.p2080;
        qcopy(&r->m[0], hero_moby + 0xC0);
        qcopy(&r->m[1], hero_moby + 0xD0);
        qcopy(&r->m[2], hero_moby + 0xE0);
        last_frame = game_frame;
    }
    current_frame = game_frame;
    elapsed_frames = current_frame - last_frame;
    if (elapsed_frames <= 0)
        elapsed_frames = 1;
    r->f70 = effect_progress + (f32)elapsed_frames * effect_config->f48;
    last_frame = current_frame;
    if ((f32)(effect_config->n * 2) <= r->f70) {
        s32 k2 = P_2a7780_G.p2080[0x52];
        if ((u32)(k2 - 0x17) < 3)
            return;
        if (k2 != 0x2B)
            r->f70 = 0.0f;
        return;
    }
    quad.tag[1] = FUN_001f44b8_2a7780(effect_texture_id);
    quad.tag[2] = 0xFF9000000260LL;
    quad.tag[0] = 5;
    quad.tag[3] = (nv << 2) | 0x8000000040LL;
    qcopy(&r->m30, P_2a7780_G.p2080 + 0x10);
    for (i = 0; i < effect_config->n; i++) {
        for (j = 0; j < 4; j++) {
            s32  vi = effect_config->idx[i][j][0];
            f32 *uv;
            if (i == sel0)
                quad.col[j] = effect_config->col[0][j];
            else if (i == sel1)
                quad.col[j] = effect_config->col[1][j];
            else if (i == sel2)
                quad.col[j] = effect_config->col[2][j];
            else if (i == sel3)
                quad.col[j] = effect_config->col[3][j];
            else
                quad.col[j] = 0x50808080;
            quad.col[j] = (truncate_float_to_s32(((s32)quad.col[j] >> 24) * alpha) << 24) | color;
            fast_vec_scale(&quad.pos[j], &effect_config->verts[vi], scale);
            uv = (f32 *)(effect_config->idx[i][j][1] * 8 + (s32)effect_config->uv);
            uv_v_offset = 1.0f - r->f70 * effect_config->f5C;
            quad.uv[j][0] = (uv[0] - 0.5f) * uv_scale + 0.5f;
            quad.uv[j][1] = (uv[1] - 0.5f) * uv_scale + 0.5f + uv_v_offset;
        }
        FUN_001f7d30_2a7780(&quad, r, 0);
    }
}
#undef P_2a7780_G
typedef struct {
    char p0[0x50];
    u8   b50;
    u8   b51;
    u8   b52;
    u8   b53;
    f32  f54;
} O_002a7c70;

typedef struct {
    char        p0[0x198];
    s32         i198;
    char        p1[0x2080 - 0x19C];
    O_002a7c70 *p2080;
    s32         i2084;
    char        p2[8];
    s32         i2090;
} G_002a7c70;

void update_wrench_animation_from_hero_state(O_002a7c70 *wrench_moby) __asm__("FUN_L00_002a7c70");

void update_wrench_animation_from_hero_state(O_002a7c70 *wrench_moby) {
    struct Hero *hero_context = &hero;
    if (hero_context->state.current == 0x3B) {
        u8  hero_current_animation = ((O_002a7c70 *)hero_context->moby)->b53;
        s32 k = 0xB;
        if (hero_current_animation == 0x40)
            k = 0x13;
        if (hero_current_animation == 0x41)
            k = 0x12;
        if (k == 0x12 || k == 0x13) {
            wrench_moby->b52 = k;
            wrench_moby->b53 = k;
            wrench_moby->f54 = ((O_002a7c70 *)hero_context->moby)->f54;
            wrench_moby->b50 = ((O_002a7c70 *)hero_context->moby)->b50;
            wrench_moby->b51 = ((O_002a7c70 *)hero_context->moby)->b51;
            if (((O_002a7c70 *)hero_context->moby)->b52 != ((O_002a7c70 *)hero_context->moby)->b53)
                wrench_moby->b50 = wrench_moby->b51;
            update_moby_animation_state(wrench_moby);
        } else {
            blend_moby_animation(wrench_moby, k, 0, scale_game_frames(7));
        }
    } else if (hero_context->state.prev == 0x3B && hero_context->state_timer == 1) {
        blend_moby_animation(wrench_moby, 1, 0, scale_game_frames(0x11));
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A9030), where it is exact; names translated to the US level program. */

extern char *suck_get_aim_height(void *) __asm__("FUN_002141f8");

/* True when the record for coll_hit_moby (suck_get_aim_height) has byte 9 equal to 1. */
int get_wrench_hit_sound_variant(void) __asm__("FUN_L00_002a7d90");

int get_wrench_hit_sound_variant(void) {
    int   record_byte_9_is_one = 0;
    char *x = 0;
    if (coll_hit_moby != 0)
        x = suck_get_aim_height(coll_hit_moby);
    if (x != 0)
        record_byte_9_is_one = *(unsigned char *)(x + 9) == 1;
    return record_byte_9_is_one;
}

char *FUN_L00_0025c338(int);
/* True when FUN_L00_0025c338((int)coll_hit_moby) is set and has flag 2 at 0x1E. */
int should_trigger_wrench_hit_reaction(void) __asm__("FUN_L00_002a7de0");

int should_trigger_wrench_hit_reaction(void) {
    char *flag_record = FUN_L00_0025c338((int)coll_hit_moby);
    if (flag_record && (*(unsigned short *)(flag_record + 0x1e) & 2))
        return 1;
    return 0;
}

extern void
emit_2a7e20(float *, int, int, int, int, void *, float, float, float) __asm__("FUN_L00_00272060");

void emit_randomized_sparkle_pairs(float *sparkle_spawn_position,
                                   int    emission_pair_count) __asm__("FUN_L00_002a7e20");

void emit_randomized_sparkle_pairs(float *sparkle_spawn_position, int emission_pair_count) {
    OvlQuad sparkle_initial_velocity[1];
    OvlQuad old[1];
    int     i;
    for (i = 0; i < emission_pair_count; i++) {
        float base_sparkle_size = random_float_between(0.1f, 2.0f);
        int   k = random_integer_below(2);
        float azimuth_angle, elevation_angle;
        if (k == 0)
            k--;
        azimuth_angle = random_angle_radians();
        elevation_angle = random_float_between(0.34906584f, 1.53588974f);
        build_spherical_offset(sparkle_initial_velocity,
                               random_float_between(0.05f, 0.1f),
                               azimuth_angle,
                               elevation_angle);
        qcopy(old, sparkle_spawn_position);
        sparkle_spawn_position[0] += randf_sym(0.0f, 0.15f);
        sparkle_spawn_position[1] += randf_sym(0.0f, 0.15f);
        sparkle_spawn_position[2] += randf_sym(0.0f, 0.15f);
        emit_2a7e20(sparkle_spawn_position,
                    scale_game_frames(0x1E),
                    0x7F207F7F,
                    0,
                    k,
                    sparkle_initial_velocity,
                    base_sparkle_size * 0.1f,
                    base_sparkle_size,
                    0.003f);
        k = -k;
        emit_2a7e20(sparkle_spawn_position,
                    scale_game_frames(0x1E),
                    0x7F7F7F7F,
                    1,
                    k,
                    sparkle_initial_velocity,
                    base_sparkle_size * 0.07f,
                    base_sparkle_size * 0.7f,
                    0.003f);
    }
}

typedef union {
    u128 q;
    f32  f[4];
} V2a8028;

extern V2a8028 D_L00_00173E60_lui __asm__("D_L00_00173E60") NOT_SDA;
extern f32     D_L00_00173E40_lui[] __asm__("D_L00_00173E40") NOT_SDA;
extern void    spawn_two_point_effect_particle(void *, void *) __asm__("FUN_L00_0026c6e8");
extern void    spawn_burst(void *, void *, s32, s32, s32, s32, f32) __asm__("FUN_L00_0026cbb0");
extern void    FUN_L00_002a7e20_c(void *, s32) __asm__("FUN_L00_002a7e20");
void           spawn_wrench_impact_effects(u8 *wrench_moby) __asm__("FUN_L00_002a8028");

void spawn_wrench_impact_effects(u8 *wrench_moby) {
    V2a8028 effect_direction_scratch;
    V2a8028 effect_spawn_position;
    f32     pitch;
    f32     randomized_direction_angle;
    s32     i;
    s32     random_frame_lower_bound;
    fast_vec_sub(&effect_direction_scratch, &g_debug_camera, &D_L00_00173E60_lui);
    pitch =
        -fast_arc_tan(vector_length_xy(&effect_direction_scratch), effect_direction_scratch.f[2]);
    fast_arc_tan(effect_direction_scratch.f[0], effect_direction_scratch.f[1]);
    for (i = 19; i >= 0; i--) {
        randomized_direction_angle = fast_add_rotations(randf_sym(0.0f, 3.1415927f), pitch);
        build_spherical_offset(&effect_direction_scratch,
                               0.2f,
                               fast_add_rotations(randf_sym(0.0f, 3.1415927f), pitch),
                               randomized_direction_angle);
        fast_vec_scale(
            &effect_spawn_position, &effect_direction_scratch, random_float_between(1.0f, 3.0f));
        fast_vec_add(&effect_spawn_position, &effect_spawn_position, &D_L00_00173E60_lui);
        spawn_two_point_effect_particle(&effect_spawn_position, &effect_direction_scratch);
    }
    for (i = 9; i >= 0; i--) {
        effect_spawn_position.f[0] = D_L00_00173E40_lui[8] + randf_sym(0.0f, 0.1f);
        effect_spawn_position.f[1] = D_L00_00173E40_lui[9] + randf_sym(0.0f, 0.1f);
        effect_spawn_position.f[2] = D_L00_00173E40_lui[10] + randf_sym(0.0f, 0.1f);
        randomized_direction_angle = fast_add_rotations(randf_sym(0.0f, 3.1415927f), pitch);
        build_spherical_offset(&effect_direction_scratch,
                               0.05f,
                               fast_add_rotations(randf_sym(0.0f, 3.1415927f), pitch),
                               randomized_direction_angle);
        random_frame_lower_bound = scale_game_frames(10);
        spawn_burst(&effect_spawn_position,
                    &effect_direction_scratch,
                    0x4F007FFF,
                    0x1FFFFFFF,
                    random_int_between(random_frame_lower_bound, scale_game_frames(20)),
                    1,
                    20000.0f);
    }
    if (*(s16 *)(((u8 *)MOBY(wrench_moby)->pvars) + 0x78) == 0x2B) {
        FUN_L00_002a7e20_c(&D_L00_00173E60_lui, 10);
    }
}

typedef struct N_2a82c0 {
    char             p0[0x20];
    unsigned char    type;
    char             p1[3];
    char            *info;
    struct N_2a82c0 *next;
} N_2a82c0;

extern N_2a82c0 *D_L00_0015FFE4;
extern int       begin_bolt_attraction(void *, void *, float, float) __asm__("FUN_L00_002a7210");
extern int       is_moby_class_e2_cc_de_3ee_d6_e1_d5_or_df(void *) __asm__("FUN_L00_0025e3f8");
extern int       try_collect_ammo_pickup(void *) __asm__("FUN_L00_002c5be8");

void process_mobies_near_returning_wrench(unsigned char *wrench_moby) __asm__("FUN_L00_002a82c0");

void process_mobies_near_returning_wrench(unsigned char *wrench_moby) {
    N_2a82c0    *scanned_moby = D_L00_0015FFE4;
    struct Hero *hero_context;
    int          moby_processing_result;
    if (scanned_moby == 0)
        return;
    hero_context = &hero;
    do {
        if (scanned_moby->type == 0xFE)
            return;
        if (scanned_moby->type == 0xFD)
            return;
        if (scanned_moby && scanned_moby->info && *(short *)(scanned_moby->info + 0x46) == 0x13) {
            if (fast_vec_dist((char *)scanned_moby + 0x10, wrench_moby + 0x10) < 2.7f) {
                moby_processing_result =
                    begin_bolt_attraction(scanned_moby,
                                          ((void *)hero_context->moby),
                                          random_float_between(-30.0f, 30.0f) * DEG_TO_RAD,
                                          0.0f);
                goto check;
            }
        } else if (is_moby_class_e2_cc_de_3ee_d6_e1_d5_or_df(scanned_moby) &&
                   fast_vec_dist((char *)scanned_moby + 0x10, wrench_moby + 0x10) < 2.7f) {
            moby_processing_result = try_collect_ammo_pickup(scanned_moby);
        check:
            if (moby_processing_result)
                return;
        }
        scanned_moby = scanned_moby->next;
    } while (scanned_moby);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A96B8), where it is exact; names translated to the US level program. */

void scale_wrench_collision_vector_by_heading(float *in_out_vector) __asm__("FUN_L00_002a8418");

void scale_wrench_collision_vector_by_heading(float *in_out_vector) {
    float k = 1.8325957f;
    float vector_heading_angle = fast_arc_tan(in_out_vector[0], in_out_vector[1]);
    float clamped_heading_difference =
        fast_difference_between_rotations(hero.motion.rot.f[2], vector_heading_angle);
    if (clamped_heading_difference > k)
        clamped_heading_difference = k;
    fast_vec_normalize(in_out_vector,
                       in_out_vector,
                       fast_vec_length(in_out_vector) *
                           ((k - clamped_heading_difference) / k * 0.3f + 0.7f));
}

#include "rnc/mobys/moby.h"

#include "sda.h"

#include "qcopy.h"

/* Updates normal wrench swings and its thrown/returning collision sweep. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A9768), where it is exact; names translated to the US level program. */

typedef unsigned int WrenchQuad __attribute__((mode(TI)));

typedef float WrenchVec[4] __attribute__((aligned(16)));

typedef struct WrenchMoby WrenchMoby;

typedef struct {
    char  pad0[0x46];
    short kind;
} WrenchClass;

typedef struct {
    char      pad0[0x40];
    WrenchVec direction, target;
    float     speed, acceleration;
    int       pad68, timer;
    float     hit_blend;
    short     animation_count, collision, animation78, pad7A, previous_count, aim_hit;
} WrenchVars;

struct WrenchMoby {
    char           pad0[0x10];
    WrenchVec      position;
    unsigned char  state, pad21[3];
    WrenchClass   *model;
    char           pad28[0x28];
    unsigned char  frame, frame_end, animation, current_animation;
    float          animation_time, animation_scale;
    char           pad5C[0x14];
    unsigned char  animation_flags;
    char           pad71[7];
    WrenchVars    *data;
    char           pad7C[0x2A];
    unsigned short class_id;
    char           padA8[0x10];
    WrenchMoby    *parent;
};

typedef struct {
    char           pad0[0x80];
    WrenchVec      position;
    char           pad90[8];
    float          yaw;
    char           pad9C[0xA4];
    WrenchVec      velocity;
    char           pad150[0x48];
    int            state_timer;
    char           pad19C[0x854];
    WrenchVec      sweep_start, sweep_end, previous_start, previous_end;
    char           padA30[0x30];
    int            attack_index, attack_cooldown;
    float          reaction_yaw;
    char           padA6C[0x10];
    unsigned short hit_index;
    char           padA7E[0x12];
    float          animation_scale;
    char           padA94[8];
    int            blocked;
    char           padAA0[8];
    float          animation_frame;
    unsigned char  padAAC[4];
    int            selected_weapon, weapon_animation;
    char           padAB8[0x5F2];
    unsigned char  thrown, pad10AB, wrench_state;
    char           pad10AD[7];
    int            mode;
    char           pad10B8[0xFC8];
    WrenchMoby    *moby;
    int            state, pad2088, move_mode;
    char           pad2090[0x1E];
    unsigned char  alternate;
    char           pad20AF[4];
    unsigned char  no_ground_adjust;
    char           pad20B4[0x168];
    int            thrown_sound;
} WrenchHero;

typedef struct {
    char      pad0[0x140];
    WrenchVec position;
    float     pad150, pitch, yaw;
} PlayerCamera;

typedef struct {
    char        pad0[0x18];
    WrenchMoby *moby;
    int         surface;
    WrenchVec   position;
} WrenchHit;

typedef struct {
    char  pad0[0x14];
    short hit_index;
    char  pad16[6];
    short hit_count;
} MobyHitState;

typedef struct {
    char           pad0[8];
    float          force, range;
    char           pad10[8];
    unsigned char  flags, enabled;
    unsigned short class_id;
} HitQuery;

typedef union {
    WrenchVec  v[8];
    WrenchQuad q[8];
    HitQuery   query;
    struct {
        WrenchVec pad0;
        HitQuery  query;
    } shifted;
} WrenchScratch;

extern HeroTableRow2C attack_records[] __asm__("D_L00_0017BC28");
extern WrenchMoby    *D_L00_00177F00[];
extern char           D_0013E533[];
extern float          approach_value_alt(float *, float, float) __asm__("FUN_00213ed8");
extern float          probe_ground_height(void *, int, float) __asm__("func_00213508");
extern int            D_L00_00173E40_u[] __asm__("D_L00_00173E40");
extern int            FUN_001efa68(void *, void *, int, int, int);
extern int            collect_mobies_in_sphere() __asm__("FUN_L00_001f2868");
extern int            test_hero_animation_frame_window(float) __asm__("FUN_L00_00211870");
extern int
sweep_hit_segment(void *, void *, void *, void *, void *, void *, int) __asm__("FUN_L00_00259d08");
extern int is_nonnull_moby_outside_table_or_class_range_500_to_540_or_class_502(char *) __asm__("FUN_L00_0025e368");
extern int  allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern void hero_set_attack_ticks(int) __asm__("FUN_L00_00232640");
extern void
offset_hero_position_against_gravity(float *, float *, float) __asm__("FUN_L00_002336e8");
extern void interpolate_vector(void *, void *, void *, float) __asm__("FUN_L00_0025b6b8");
extern void compose_target_euler_rotation(char *, float, float, float) __asm__("FUN_L00_00262480");
extern void update_moby_transform_history(void *, int) __asm__("FUN_L00_00262608");
extern void clear_moby_transform_history(char *) __asm__("FUN_L00_00262840");
extern void FUN_L00_002a7780_cb(void) __asm__("FUN_L00_002a7780");
void        suck_draw_reticle(int   a,
                              int   b,
                              void *pos,
                              int   kind,
                              int   mode,
                              float x,
                              float y,
                              float z) __asm__("FUN_L00_001edf60");
void        FUN_L00_002a7c70_c(void *m) __asm__("FUN_L00_002a7c70");
void        emit_randomized_sparkle_pairs(float *pos, int n) __asm__("FUN_L00_002a7e20");
void        spawn_wrench_impact_effects(u8 *o) __asm__("FUN_L00_002a8028");
void        process_mobies_near_returning_wrench(unsigned char *m) __asm__("FUN_L00_002a82c0");
void        scale_wrench_collision_vector_by_heading(float *v) __asm__("FUN_L00_002a8418");

#include "rnc/input/pad_globals.h"

extern char      thrown_wrench_transform_history[] __asm__("D_00140B00");
extern char      wrench_return_position[] __asm__("D_001403C0");
extern WrenchHit collision_hit __asm__("D_L00_00173E40");
extern void
draw_target_reticle(int, int, float, float, int, int, int, float, int) __asm__("FUN_L00_001edf60");
extern int collide_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");

extern char D_0013F350[];

static __inline__ WrenchHero *wrench_hero(void) {
    return (WrenchHero *)D_0013F350;
}

static __inline__ float *sweep_end(void) {
    return (float *)(D_0013E533 + 0x181D);
}

static __inline__ unsigned long combo_step(int animation) {
    return (unsigned char)(animation - 15);
}

static __inline__ float *hero_velocity(void) {
    return (float *)(D_0013E533 + 0xF5D);
}

static __inline__ unsigned char *voice_slot(int slot) {
    return (unsigned char *)&voice_pool + slot * 0x70;
}

static __inline__ WrenchHero *sweep_hero(void) {
    return (WrenchHero *)((char *)sweep_end() - 0xA00);
}

void wrench_update(WrenchMoby *m) __asm__("FUN_L00_002a84c8");

void wrench_update(WrenchMoby *wrench_moby) {
    enum {
        WRENCH_NORMAL = 0,
        WRENCH_THROW_OUTBOUND = 10,
        WRENCH_THROW_RETURNING = 11
    };
    WrenchVars   *wrench_vars = wrench_moby->data;
    WrenchScratch work;
    WrenchHero   *player = wrench_hero();
    player->wrench_state = 0;
    if (player->mode == 3)
        return;
    switch (wrench_moby->state) {
    case WRENCH_NORMAL: {
        int             player_state;
        int             anim;
        WrenchMoby     *player_moby;
        HeroTableRow2C *attack;
        int             active;
        int             reaction;
        float           difference, yaw;
        wrench_moby->parent = player->moby;
        player_state = player->state;
        if (player_state == 1) {
            if (pad_held[0] & 0xA)
                draw_target_reticle(
                    (int)wrench_moby, 0xFF917267, 1.0f, 0.0f, 0, 0x25, -1, 90.0f, 4);
            wrench_vars->aim_hit = 0;
            build_spherical_offset(work.v[0], 10.0f, l00_camera.yaw, -l00_camera.pitch);
            fast_vec_add(work.v[0], work.v[0], l00_camera.pos.f);
            if (FUN_001efa68(l00_camera.pos.f, work.v[0], 4, (int)player->moby, 0)) {
                wrench_vars->aim_hit = player_state;
                qcopy(wrench_vars->target, collision_hit.position);
            }
        }
        add_draw_callback(FUN_L00_002a7780_cb, wrench_moby);
        anim = wrench_moby->current_animation;
        if ((unsigned char)anim != 0) {
            if ((unsigned char)anim == 14) {
                if (wrench_moby->animation_flags & 2)
                    blend_moby_animation(wrench_moby, 1, 0, scale_game_frames(5));
                return;
            } else if (wrench_hero()->alternate) {
                if (combo_step(anim) >= 2U)
                    blend_moby_animation(wrench_moby, 15, 0, scale_game_frames(4));
                else if ((wrench_moby->animation_flags & 2) && (unsigned char)anim == 15)
                    blend_moby_animation(wrench_moby, 16, 0, scale_game_frames(5));
                return;
            } else if (combo_step(anim) < 2U) {
                blend_moby_animation(wrench_moby, 14, 0, scale_game_frames(2));
                return;
            }
        }
        if ((wrench_moby->animation_flags & 2) && wrench_hero()->state != 0x3B &&
            wrench_moby->current_animation != 1)
            blend_moby_animation(wrench_moby, 1, 0, scale_game_frames(5));
        FUN_L00_002a7c70_c((char *)wrench_moby);
        {
            int current_state = wrench_hero()->state;
            if (current_state != 19 && current_state != 33 && current_state != 43 &&
                current_state != 112 && current_state != 20) {
                int current = wrench_moby->current_animation;
                if ((unsigned int)current >= 3 && wrench_moby->current_animation < 6)
                    blend_moby_animation(wrench_moby, 1, 0, scale_game_frames(4));
            }
        }
        player_moby = wrench_hero()->moby;
        if (player_moby->current_animation != player_moby->animation ||
            (int)player_moby->frame_end - player_moby->frame >= 3)
            wrench_vars->animation_count++;
        wrench_moby->animation_scale = wrench_hero()->animation_scale;
        attack = &attack_records[wrench_hero()->attack_index];
        if (wrench_hero()->move_mode != 6 && wrench_hero()->state != 43)
            break;
        active = 1;
        reaction = 0;
        if (wrench_hero()->blocked || wrench_hero()->animation_frame < (float)attack->f1C ||
            (float)attack->f20 < wrench_hero()->animation_frame)
            active = 0;
        {
            int reset = 0;
            if (wrench_vars->previous_count != wrench_vars->animation_count) {
                WrenchMoby *pm = wrench_hero()->moby;
                int         player_animation = pm->animation;
                if (player_animation == pm->current_animation) {
                    if (player_animation == 23)
                        reset = 0.0f <= wrench_hero()->animation_frame;
                    else if (player_animation == 24)
                        reset = 0.0f <= wrench_hero()->animation_frame;
                    else if (player_animation == 25)
                        reset = 12.0f <= wrench_hero()->animation_frame;
                    else if (player_animation == 43)
                        reset = 23.0f <= wrench_hero()->animation_frame;
                }
            }
            if (reset) {
                wrench_vars->hit_blend = 1.0f;
                wrench_vars->previous_count = wrench_vars->animation_count;
            }
        }
        difference = 0.0f;
        yaw = wrench_hero()->yaw;
        qcopy(wrench_hero()->previous_end, wrench_hero()->sweep_end);
        qcopy(wrench_hero()->previous_start, wrench_hero()->sweep_start);
        moby_point_position(wrench_moby, 1, wrench_hero()->sweep_end);
        moby_point_position(wrench_hero()->moby, 0, wrench_hero()->sweep_start);
        if (wrench_hero()->state == 19 || wrench_hero()->state == 112 ||
            wrench_hero()->state == 43) {
            yaw = fast_arc_tan(wrench_hero()->sweep_end[0] - wrench_hero()->position[0],
                               wrench_hero()->sweep_end[1] - wrench_hero()->position[1]);
            if (attack_records[wrench_hero()->attack_index].kind == 1)
                yaw = fast_add_rotations(yaw, -1.5707964f);
            else
                yaw = fast_add_rotations(yaw, 1.5707964f);
        }
        fast_vec_sub(work.v[0], sweep_end(), (sweep_end() - 4));
        fast_vec_normalize(work.v[0], work.v[0], fast_vec_length(work.v[0]) + 0.17f);
        if (sweep_hero()->move_mode != 15 && sweep_hero()->state != 112 &&
            sweep_hero()->state != 20) {
            difference = fast_difference_between_rotations(
                sweep_hero()->yaw,
                fast_arc_tan(sweep_hero()->sweep_end[0] - sweep_hero()->position[0],
                             sweep_hero()->sweep_end[1] - sweep_hero()->position[1]));
            if (difference > 1.5707964f)
                active = 0;
            scale_wrench_collision_vector_by_heading(work.v[0]);
        }
        fast_vec_add(sweep_end(), (sweep_end() - 4), work.v[0]);
        if (sweep_hero()->state != 43 && sweep_hero()->state != 112 && difference < 1.2217305f &&
            scale_game_frames(10) < sweep_hero()->state_timer) {
            float angle;
            qcopy(work.v[0], sweep_hero()->position);
            work.v[0][2] += 0.5f;
            angle = fast_arc_tan(sweep_hero()->sweep_end[0] - sweep_hero()->position[0],
                                 sweep_hero()->sweep_end[1] - sweep_hero()->position[1]);
            qcopy(work.v[1], sweep_hero()->position);
            work.v[1][2] += 0.5f;
            work.v[2][0] = fast_cos(angle) * 1.4f;
            work.v[2][1] = fast_sin(angle) * 1.4f;
            work.v[2][2] = 0.0f;
            scale_wrench_collision_vector_by_heading(work.v[2]);
            fast_vec_add(work.v[1], work.v[1], work.v[2]);
            if (!sweep_hero()->attack_cooldown && FUN_001efa68(work.v[0], work.v[1], 2, 0, 0) &&
                collision_hit_material()) {
                if (!collision_hit.moby ||
                    is_nonnull_moby_outside_table_or_class_range_500_to_540_or_class_502(
                        (char *)collision_hit.moby)) {
                    sweep_hero()->attack_cooldown = 1;
                    allocate_voice_for_target_entry_alt(2, 0, (int)wrench_moby);
                    spawn_wrench_impact_effects((char *)wrench_moby);
                    if (collision_hit.moby) {
                        work.v[3][0] = fast_cos(yaw);
                        work.v[3][1] = fast_sin(yaw);
                        work.v[3][2] = 0.0f;
                        spawn_nearby_effect(collision_hit.moby,
                                            (int)wrench_moby,
                                            0x10000,
                                            collision_hit.position,
                                            work.v[3],
                                            1.0f);
                    }
                }
            }
        }
        if (wrench_hero()->state == 20 && test_hero_animation_frame_window(27.0f)) {
            allocate_voice_for_target_entry_alt(2, 0, (int)wrench_moby);
            qcopy(work.v[0], wrench_hero()->sweep_end);
            qcopy(work.v[1], work.v[0]);
            offset_hero_position_against_gravity(work.v[0], work.v[0], 0.7f);
            offset_hero_position_against_gravity(work.v[1], work.v[1], -0.9f);
            if (FUN_001efa68(work.v[0], work.v[1], 2, 0, 0))
                spawn_wrench_impact_effects((char *)wrench_moby);
        }
        if (active) {
            int   count = 1;
            float scale = 1.0f, radius;
            if (wrench_hero()->state == 20) {
                scale = 1.55f;
                count = 2;
            }
            work.v[3][0] = fast_cos(yaw);
            work.v[3][1] = fast_sin(yaw);
            work.v[3][2] = 0.0f;
            make_hit_descriptor(work.v[0], wrench_moby, 0x10000, (float)count, work.v[3]);
            set_vector_length_xy(work.v[0], work.v[0], scale);
            work.query.force = 1.0f;
            work.query.range = 5627.925f;
            work.query.enabled = 1;
            work.query.class_id = wrench_moby->class_id;
            work.query.flags = 0;
            if (sweep_hit_segment(wrench_hero()->sweep_start,
                                  wrench_hero()->sweep_end,
                                  wrench_hero()->previous_start,
                                  wrench_hero()->previous_end,
                                  wrench_hero()->moby,
                                  &work,
                                  5) &&
                collision_hit.moby) {
                if (should_trigger_wrench_hit_reaction())
                    reaction = 1;
                else if (!wrench_hero()->attack_cooldown) {
                    if (!collision_hit.moby || !collision_hit.moby->model ||
                        collision_hit.moby->model->kind != 20) {
                        wrench_hero()->attack_cooldown = 1;
                        allocate_voice_for_target_entry_alt(
                            get_wrench_hit_sound_variant(), 0, (int)wrench_moby);
                        spawn_wrench_impact_effects((char *)wrench_moby);
                    }
                }
            }
            fast_vec_sub(work.v[5], sweep_end(), (sweep_end() - 4));
            fast_vec_normalize(work.v[5], work.v[5], fast_vec_length(work.v[5]) - 0.085f);
            fast_vec_add(work.v[4], work.v[5], (sweep_end() - 4));
            radius = 0.35f;
            if (sweep_hero()->move_mode == 15)
                radius = 0.7f;
            if (sweep_hero()->state == 20)
                radius = 0.47f;
            if (collide_sphere(work.v[4], radius, 0, sweep_hero()->moby, work.v[0]) &&
                (!D_L00_00177F00[0] || !D_L00_00177F00[0]->model ||
                 D_L00_00177F00[0]->model->kind != 18)) {
                if (should_trigger_wrench_hit_reaction())
                    reaction = 1;
                else if (!wrench_hero()->attack_cooldown) {
                    wrench_hero()->attack_cooldown = 1;
                    allocate_voice_for_target_entry_alt(
                        get_wrench_hit_sound_variant(), 0, (int)wrench_moby);
                    spawn_wrench_impact_effects((char *)wrench_moby);
                }
            }
        }
        if (reaction) {
            hero_set_state(33, 1);
            if (collision_hit.moby) {
                WrenchMoby *target = collision_hit.moby;
                wrench_hero()->reaction_yaw =
                    fast_arc_tan(wrench_hero()->position[0] - target->position[0],
                                 wrench_hero()->position[1] - target->position[1]);
            } else
                wrench_hero()->reaction_yaw = fast_add_rotations(wrench_hero()->yaw, 3.1415927f);
        }
        break;
    }
    case WRENCH_THROW_OUTBOUND:
    case WRENCH_THROW_RETURNING: {
        int    collided;
        float *position;
        float  radius;
        {
            char       *control = ((char *)&thrown_wrench_transform_history);
            WrenchHero *control_player = (WrenchHero *)(control - 0x17B0);
            update_moby_transform_history(control, 0);
            wrench_moby->parent = control_player->moby;
            control_player->wrench_state = 2;
            if (wrench_moby->state == 10 && control_player->thrown_sound == -1)
                control_player->thrown_sound =
                    allocate_voice_for_target_entry_alt(14, 4, (int)control_player->moby);
        }
        compose_target_euler_rotation((char *)wrench_moby, 0.0f, 0.0f, D_0015ED6C * 24.434608f);
        if (wrench_hero()->attack_cooldown && fast_dec_timer(&wrench_vars->timer))
            wrench_hero()->attack_cooldown = 0;
        position = wrench_moby->position;
        if (!wrench_hero()->no_ground_adjust) {
            float ground = probe_ground_height(position, 0, 0.5f);
            if (wrench_moby->position[2] - ground < 0.8f) {
                ground += 0.55f;
                if (wrench_moby->position[2] < ground)
                    approach_value_alt(&wrench_moby->position[2], ground, D_0015ED6C * 4.0f);
                else
                    approach_value_alt(&wrench_moby->position[2], ground, D_0015ED6C);
            }
        }
        qcopy(work.v[0], position);
        if (wrench_moby->state == 10) {
            qcopy(work.v[1], hero_velocity());
            fast_vec_scale(work.v[1], work.v[1], 0.7f);
            fast_vec_add(position, position, work.v[1]);
            fast_vec_normalize(work.v[1], wrench_vars->direction, wrench_vars->speed);
            fast_vec_add(position, position, work.v[1]);
            wrench_vars->acceleration += frame_time_cube * 170.0f;
            approach_value_alt(&wrench_vars->speed, 0.0f, wrench_vars->acceleration);
            if (wrench_vars->speed == 0.0f)
                wrench_moby->state = 11;
        } else {
            int   arrived = 0;
            float length, steps;
            process_mobies_near_returning_wrench((char *)wrench_moby);
            fast_vec_sub(work.v[1], ((char *)&wrench_return_position), position);
            wrench_vars->acceleration += frame_time_sq * 0.9f;
            wrench_vars->speed += wrench_vars->acceleration;
            length = fast_vec_length(work.v[1]);
            steps = wrench_vars->speed;
            steps = length / steps;
            if (steps < (float)scale_game_frames(5) && wrench_moby->current_animation != 1)
                blend_moby_animation(wrench_moby, 1, 0, scale_game_frames(5));
            if (length <= wrench_vars->speed) {
                wrench_vars->speed = length;
                arrived = 1;
            }
            fast_vec_normalize(work.v[1], work.v[1], wrench_vars->speed);
            fast_vec_add(position, position, work.v[1]);
            if (length / wrench_vars->speed < (float)scale_game_frames(4) &&
                wrench_hero()->selected_weapon != -1) {
                int sound;
                hero_set_attack_ticks(2);
                wrench_hero()->weapon_animation = 26;
                sound = wrench_hero()->thrown_sound;
                if (sound != -1) {
                    unsigned char *audio = voice_slot(sound);
                    if (*(WrenchMoby **)(audio + 0x88) == wrench_hero()->moby && audio[0x74])
                        release_voice_slot(sound);
                }
                wrench_hero()->thrown_sound = -1;
            }
            if (arrived) {
                int sound;
                allocate_hero_voice_for_entry(5, 0);
                sound = wrench_hero()->thrown_sound;
                if (sound != -1) {
                    unsigned char *audio = voice_slot(sound);
                    if (*(WrenchMoby **)(audio + 0x88) == wrench_hero()->moby && audio[0x74])
                        release_voice_slot(sound);
                }
                wrench_hero()->thrown_sound = -1;
                clear_moby_transform_history((char *)wrench_hero() + 0x17B0);
                wrench_hero()->thrown = 0;
                wrench_moby->state = 0;
            }
        }
        collided = 0;
        if (FUN_001efa68(work.v[0], position, 2, 0, 0) && collision_hit_material() &&
            (!collision_hit.moby || !is_moby_in_class_range_500_to_540((int)collision_hit.moby)))
            collided = collision_hit.surface != 0;
        if (collided || wrench_vars->collision) {
            wrench_vars->collision = 0;
            if (!wrench_hero()->attack_cooldown) {
                wrench_vars->timer = scale_game_frames(30);
                wrench_hero()->attack_cooldown = 1;
                allocate_voice_for_target_entry_alt(2, 0, (int)wrench_moby);
            }
            if (wrench_moby->state == 10) {
                spawn_wrench_impact_effects((char *)wrench_moby);
                wrench_vars->speed *= 0.5f;
                wrench_moby->state = 11;
            }
        }
        radius = 1.2f;
        if (wrench_moby->state == 11)
            radius = -0.37f;
        fast_vec_normalize(work.v[4], wrench_vars->direction, radius);
        make_hit_descriptor(work.v[1], wrench_moby, 0x10000, 1.0f, work.v[4]);
        work.shifted.query.force = 1.0f;
        work.shifted.query.range = 5627.925f;
        work.shifted.query.enabled = 1;
        work.shifted.query.class_id = wrench_hero()->hit_index + 1;
        work.shifted.query.flags = 0;
        moby_point_position(wrench_moby, 1, work.v[5]);
        if (collide_sphere(work.v[5], 0.4f, 0, wrench_hero()->moby, work.v[1]) ||
            (moby_point_position(wrench_moby, 0, work.v[5]),
             collide_sphere(work.v[5], 0.4f, 0, wrench_hero()->moby, work.v[1]))) {
            if (!wrench_hero()->attack_cooldown && collision_hit.moby &&
                (!collision_hit.moby->model || collision_hit.moby->model->kind != 20)) {
                MobyHitState *hit = (MobyHitState *)suck_get_aim_height(collision_hit.moby);
                if (!hit || hit->hit_count < 2 || hit->hit_index != work.shifted.query.class_id) {
                    work.q[7] = *(WrenchQuad *)collision_hit.moby->position;
                    interpolate_vector(work.v[6], work.v[5], work.v[7], 0.5f);
                    emit_randomized_sparkle_pairs(work.v[6], 5);
                    wrench_vars->timer = scale_game_frames(30);
                    wrench_hero()->attack_cooldown = 1;
                    allocate_voice_for_target_entry_alt(
                        get_wrench_hit_sound_variant(), 0, (int)wrench_moby);
                }
            }
        }
        break;
    }
    default:
        break;
    }
}
/* Spawns a homing bolt (class 0x100) from owner at pos, aimed at tgt or at whatever lies along its heading. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002AA998), where it is exact; names translated to the US level program. */

typedef float V[4] __attribute__((aligned(16)));

typedef int Q_2aa998 __attribute__((mode(TI)));

typedef union {
    Q_2aa998 q;
    float    f[4];
} BoltVector;

typedef struct {
    char       pad[0x10];
    BoltVector position;
} BoltOwner;

extern V     D_L00_00173E60;
extern char *select_auto_aim_target_moby(
    void *, void *, void *, float *, float *, float *, float) __asm__("FUN_L00_002630c8");
extern char  D_0013F490[];
extern void *D_001413D0;
extern float D_L00_001613C4 __attribute__((sda));
extern short D_L00_001B0770[];
extern void  append_unique_moby_index(void *, short *, int) __asm__("FUN_L00_0025f780");

char *spawn_homing_bolt(void *owner,
                        void *pos,
                        float a,
                        float b,
                        float c,
                        char *tgt,
                        float rz,
                        float ry) __asm__("FUN_L00_002a96f8");
char *spawn_homing_bolt(

    void *owner, void *pos, float a, float b, float c, char *tgt, float rz, float ry) {
    char      *m;
    char      *d;
    BoltVector t;
    BoltVector p;
    m = (char *)create_moby(0x100);
    if (m != 0) {
        ((unsigned char *)m)[0x30] = 0xFF;
        m[0x31] = 1;
        m[0x20] = 0;
        *(float *)(m + 0x2C) *= D_L00_001613C4;
        *(short *)(m + 0x32) = 0xFF;
        d = *(char **)(m + 0x78);
        *(float *)(m + 0x48) = rz;
        *(float *)(m + 0x44) = ry;
        if (tgt == 0) {
            build_spherical_offset(&t, D_0015ED6C * 20.0f, rz, ry);
            p.q = *(Q_2aa998 *)pos;
            tgt = select_auto_aim_target_moby(owner, &p, &t, &a, &b, &c, 0.34906584f);
        }
        *(void **)(d + 0x10) = owner;
        qcopy(m + 0x10, pos);
        *(float *)(d + 0x34) = D_0015ED6C * 100.0f;
        *(int *)(d + 0x38) = 1;
        *(int *)(d + 0x24) = 0;
        *(int *)(d + 0x28) = 0;
        *(int *)(d + 0x20) = 0;
        *(char **)(d + 0x18) = tgt;
        if (tgt != 0) {
            unsigned char *e = (unsigned char *)suck_get_aim_height(tgt);
            if (e != 0) {
                *(unsigned short *)(e + 0x1E) |= 0x80;
                if (e[0xB] != 0)
                    *(int *)(d + 0x38) = 0;
                if (e[0xC] != 0)
                    *(float *)(d + 0x34) = (float)e[0xC] * D_0015ED6C;
            }
        }
        *(float *)(d + 0x2C) = a;
        *(float *)(d + 0x30) = b;
        *(int *)(d + 0x14) = scale_game_frames(0x96);
        if (tgt != 0) {
            qcopy(d, tgt + 0x10);
            *(float *)(d + 8) += c;
            *(float *)(d + 0x1C) = c;
        }
        {
            int r;
            if (fast_vec_length((float *)(D_0013F490)) > D_0015ED6C * 0.1f) {
                r = scale_game_frames(0x3C);
                *(short *)(d + 0x3E) = r;
                *(short *)(d + 0x3C) = r;
            } else {
                r = scale_game_frames(1);
                *(short *)(d + 0x3E) = r;
                *(short *)(d + 0x3C) = r;
            }
        }
        t.q = ((BoltOwner *)owner)->position.q;
        t.f[2] = ((float *)pos)[2];
        if (FUN_001efa68(&t, pos, 0, *(int *)(((char *)&D_001413D0)), 0) != 0) {
            m[0xBC] = 1;
            qcopy(m + 0x10, &D_L00_00173E60);
        }
        update_entity(m);
        append_unique_moby_index(m, D_L00_001B0770, 0x1F);
        *(float *)(m + 0x2C) += *(float *)(m + 0x2C);
    }
    return m;
}
/* Creates a moby at a position with a scale, colour and a few data fields. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002AAC50), where it is exact; names translated to the US level program. */

void *spawn_class_593_moby(int   unused,
                           void *spawn_position,
                           int   initial_paired_value,
                           float scale,
                           float initial_private_scalar,
                           float b) __asm__("FUN_L00_002a99b0");

void *spawn_class_593_moby(int   unused,
                           void *spawn_position,
                           int   initial_paired_value,
                           float scale,
                           float initial_private_scalar,

                           float initial_private_value) {
    char *spawned_moby = create_moby(0x593);
    if (spawned_moby != 0) {
        char *moby_vars;
        float doubled_scale;
        qcopy(spawned_moby + 0x10, spawn_position);
        set_moby_color(spawned_moby, 0x80, 0x80, 0x70);
        moby_vars = ((char *)MOBY(spawned_moby)->pvars);
        doubled_scale = scale + scale;
        *(float *)moby_vars = initial_private_scalar;
        MOBY(spawned_moby)->scale = doubled_scale;
        MOBY(spawned_moby)->scale =
            doubled_scale * *(float *)(((char *)MOBY(spawned_moby)->pclass) + 0x24);
        *(int *)(moby_vars + 4) = initial_paired_value;
        *(int *)(moby_vars + 8) = initial_paired_value;
        *(float *)(moby_vars + 0xC) = initial_private_value;
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        MOBY(spawned_moby)->visible = 1;
        MOBY(spawned_moby)->state = 0;
    }
    return spawned_moby;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002AAD40), where it is exact; names translated to the US level program. */

extern int   D_L00_0015F320;
extern int   D_L00_0015F330;
extern int   allocate_packed_color_point_light_slot(float *pos,
                                                    float  radius,
                                                    float  intensity,
                                                    int    color) __asm__("FUN_L00_0023e5e0");
extern short D_L00_001613C8_gp __asm__("D_L00_001613C8") __attribute__((sda));

extern int  D_L00_0015F324;
extern int  D_L00_0015F334;
extern long D_L00_0015F328;
extern long D_L00_0015F338;
void *spawn_tracked_class_5f3_moby(int unused, float *spawn_position) __asm__("FUN_L00_002a9aa0");

void *spawn_tracked_class_5f3_moby(int unused, float *spawn_position) {
    char *spawned_moby = create_moby(0x5F3);
    if (spawned_moby != 0 && *(int *)&D_L00_001613C8_gp == 0) {
        char *moby_vars;
        spawned_moby[0x20] = 0;
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        moby_vars = ((char *)MOBY(spawned_moby)->pvars);
        *(OvlQuad *)&MOBY(spawned_moby)->pos = *(OvlQuad *)spawn_position;
        D_L00_0015F324 = 0xFF;
        D_L00_0015F334 = 0xFFFFFF;
        D_L00_0015F338 = 0x1F00000042L;
        *(int *)&D_L00_001613C8_gp = (int)spawned_moby;
        D_L00_0015F328 = 0;
        D_L00_0015F320 = 0;
        D_L00_0015F330 = 0;
        *(int *)(moby_vars + 0x10) = 0;
        *(short *)(moby_vars + 0x2C) =
            allocate_packed_color_point_light_slot(spawn_position, 0.0f, 0.0f, 0);
    }
    return spawned_moby;
}

extern short D_L00_001613C8 __attribute__((sda));
void         FUN_L00_002a9aa0_u(void *, OvlQuad *) __asm__("FUN_L00_002a9aa0");
void         set_tracked_class_5f3_moby_position(void    *unused_creation_arg,
                                                 OvlQuad *position) __asm__("FUN_L00_002a9b80");

void set_tracked_class_5f3_moby_position(void *unused_creation_arg, OvlQuad *position) {
    OvlQuad position_copy = *position;
    u8     *moby;
    u8     *moby_vars;
    u8     *tracked_moby;
    tracked_moby = *(u8 **)&D_L00_001613C8;
    if (tracked_moby == 0) {
        FUN_L00_002a9aa0_u(unused_creation_arg, &position_copy);
        tracked_moby = *(u8 **)&D_L00_001613C8;
        if (tracked_moby == 0)
            return;
    }
    moby = *(u8 **)&D_L00_001613C8;
    moby_vars = ((u8 *)MOBY(moby)->pvars);
    if (MOBY(moby)->oclass == 0x5F3) {
        *(OvlQuad *)&MOBY(moby)->pos = position_copy;
        *(s16 *)(moby_vars + 0x2E) = 1;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002AAE80), where it is exact; names translated to the US level program. */

void remove_tracked_class_5f3_moby(void) __asm__("FUN_L00_002a9be0");

void remove_tracked_class_5f3_moby(void) {
    struct Moby *tracked_moby = *(char **)&D_L00_001613C8_gp;
    if (tracked_moby != 0) {
        if (tracked_moby->oclass == 0x5F3) {
            char *data;
            int   point_light_slot_index;
            if (tracked_moby->state != 0)
                return;
            data = ((char *)tracked_moby->pvars);
            D_L00_0015F320 = 0;
            D_L00_0015F330 = 0;
            point_light_slot_index = *(short *)(data + 0x2C);
            if (point_light_slot_index != -1)
                release_point_light_slot(point_light_slot_index);
            mark_moby_for_removal(*(char **)&D_L00_001613C8_gp);
        }
        *(int *)&D_L00_001613C8_gp = 0;
    }
}

typedef union {
    u128 q;
    f32  f[4];
} V_2a9c50;

typedef struct {
    V_2a9c50 corner[4];
    u32      color[4];
    f32      uv[8];
    s64      unk70;
    s64      tex;
    s64      unk80;
    s64      unk88;
} Q_2a9c50;

typedef struct {
    u8       pad0[0x10];
    V_2a9c50 pos;
    f32      f20, f24, f28;
} PV;

typedef struct {
    u8  pad0[0x78];
    PV *pv;
} M_2a9c50;

extern s32 D_L00_0015F5CC;
/* Spin angles of the second quad: x is read through $gp, the rest as an
   array through the unsplit address macro (from rac1-decomp's form). */
extern f32  spin_angles_x __asm__("D_L00_001613E0") __attribute__((sda));
extern f32  spin_angles[] __asm__("D_L00_001613E0") MACRO_ADDR;
extern void draw_geometry_quad(void *, void *, s32) __asm__("FUN_001f7d30");
void        draw_mine_predicted_impact_marker(M_2a9c50 *moby) __asm__("FUN_L00_002a9c50");

void draw_mine_predicted_impact_marker(M_2a9c50 *moby) {
    Q_2a9c50 quad;
    V_2a9c50 quad_transform[4];
    V_2a9c50 mz[4];
    V_2a9c50 direction_rotation_angles;
    PV      *pv = moby->pv;
    s32      i;
    s32      odd;
    f32      direction_inclination, spin_angle_numerator;
    if (D_L00_0015F5C4 == 2) {
        return;
    }
    for (i = 0; i < 2; i++) {
        odd = i & 1;
        direction_inclination =
            fast_arc_tan(square_root(pv->f20 * pv->f20 + pv->f28 * pv->f28), pv->f24);
        direction_rotation_angles.f[0] = fast_add_rotations(-direction_inclination, 1.57079637f);
        direction_rotation_angles.f[1] = fast_arc_tan(pv->f28, pv->f20);
        direction_rotation_angles.f[2] = 0.0f;
        direction_rotation_angles.f[3] = 0.0f;
        spin_angles_x = 0.0f;
        spin_angle_numerator = convert_integer_to_float(D_L00_0015F5CC % scale_game_frames(180));
        {
            f32 spin_period = scale_time(180.0f);
            f32 quad_spin_angle;
            spin_angle_numerator *= 6.28318548f;
            quad_spin_angle = fast_normalize_angle(spin_angle_numerator / spin_period);
            if (odd) {
                quad_spin_angle = -quad_spin_angle;
            }
            spin_angles[1] = quad_spin_angle;
        }
        spin_angles[2] = 0.0f;
        spin_angles[3] = 0.0f;
        euler_rotation_matrix(mz, &spin_angles_x);
        euler_rotation_matrix(quad_transform, &direction_rotation_angles);
        multiply_matrix(quad_transform, quad_transform, mz);
        qcopy(&quad_transform[3], &pv->pos);
        quad.color[0] = 0x80808080;
        quad.color[1] = 0x80808080;
        quad.color[2] = 0x80808080;
        quad.color[3] = 0x80808080;
        quad.tex = get_effect_texture(odd ? 0x12 : 0x11);
        quad.unk80 = 0xFF9000000260L;
        quad.unk88 = 0x8000000048L;
        quad.unk70 = 0;
        quad.corner[0].f[0] = -1.0f;
        quad.corner[0].f[1] = 0.0f;
        quad.corner[0].f[2] = 1.0f;
        quad.corner[0].f[3] = 1.0f;
        quad.corner[1].f[0] = 1.0f;
        quad.corner[1].f[1] = 0.0f;
        quad.corner[1].f[2] = 1.0f;
        quad.corner[1].f[3] = 1.0f;
        quad.corner[2].f[0] = -1.0f;
        quad.corner[2].f[1] = 0.0f;
        quad.corner[2].f[2] = -1.0f;
        quad.corner[2].f[3] = 1.0f;
        quad.corner[3].f[0] = 1.0f;
        quad.corner[3].f[1] = 0.0f;
        quad.corner[3].f[2] = -1.0f;
        quad.corner[3].f[3] = 1.0f;
        quad.uv[0] = 0.0f;
        quad.uv[1] = 1.0f;
        quad.uv[2] = 1.0f;
        quad.uv[3] = 1.0f;
        quad.uv[4] = 0.0f;
        quad.uv[5] = 0.0f;
        quad.uv[6] = 1.0f;
        quad.uv[7] = 0.0f;
        draw_geometry_quad(&quad, quad_transform, 1);
    }
}

extern float   D_0015ED6C_2a9ed0 __asm__("D_0015ED6C");
extern int     D_L00_0015F5CC_2a9ed0 __asm__("D_L00_0015F5CC");
unsigned char *spawn_mine(int owner, OvlQuad *pos) __asm__("FUN_L00_002a9ed0");

unsigned char *spawn_mine(int owner, OvlQuad *spawn_position) {
    unsigned char *mine_moby = create_moby(0x4A);
    if (mine_moby) {
        unsigned char *mine_vars = ((unsigned char *)MOBY(mine_moby)->pvars);
        mine_moby[0x30] = 0xFF;
        MOBY(mine_moby)->draw_dist = 0x7F;
        mine_moby[0x31] = 1;
        MOBY(mine_moby)->scale = MOBY(mine_moby)->scale * 0.5f;
        mine_moby[0x20] = 0;
        *(int *)(mine_vars + 0x30) = owner;
        *(short *)(mine_vars + 0x36) = 0;
        *(int *)(mine_vars + 0x78) = 0;
        qcopy(mine_moby + 0x10, spawn_position);
        clear_u64_value(mine_vars);
        {
            float a = random_float_between(D_0015ED6C_2a9ed0 * 1.0471976f,
                                           D_0015ED6C_2a9ed0 * 3.1415927f);
            float frame_rate_scale = D_0015ED6C_2a9ed0;
            float frame_scaled_random_sample;
            int   x;
            float z;
            *(float *)(mine_vars + 0x38) = a;
            frame_scaled_random_sample =
                random_float_between(frame_rate_scale * 1.0471976f, frame_rate_scale * 3.1415927f);
            x = D_L00_0015F5CC_2a9ed0;
            z = 0.0f;
            *(float *)(mine_vars + 0x3C) = frame_scaled_random_sample;
            *(float *)(mine_vars + 0x64) = z;
            *(float *)(mine_vars + 0x60) = z;
            *(float *)(mine_vars + 0x70) = z;
            *(float *)(mine_vars + 0x6C) = z;
            *(int *)(mine_vars + 0x5C) = x;
            *(int *)(mine_vars + 0x74) = 0;
            *(int *)(mine_vars + 0x68) = -1;
            MOBY(mine_moby)->collision = 0;
        }
        update_entity(mine_moby);
    }
    return mine_moby;
}

typedef union {
    u128 q;
    f32  f[4];
} V_2aa008;

typedef struct {
    u8  pad[0x5C];
    s32 i5C;
} D_2aa008;

typedef struct {
    u8  pad[0x34];
    s16 h34;
} M_2aa008;

typedef struct N_2aa008 {
    u8               pad[0x10];
    V_2aa008         v10;
    u8               b20;
    u8               pad2[0x28 - 0x21];
    struct N_2aa008 *p28;
    f32              f2C;
    u8               pad3[0x78 - 0x30];
    void            *p78;
    u8               pad4[0xA6 - 0x7C];
    s16              hA6;
} N_2aa008;

typedef struct {
    u8       pad[0x10];
    V_2aa008 v10;
} Q_2aa008;

extern f32       D_0015ED6C_2aa008 __asm__("D_0015ED6C");
extern N_2aa008 *D_L00_0015FFE4_2aa008 __asm__("D_L00_0015FFE4");
extern V_2aa008  D_L00_00173E60_2aa008[] __asm__("D_L00_00173E60") NOT_SDA;
s32              FUN_L00_002603d0_2aa008(void *) __asm__("FUN_L00_002603d0");
s32              collision_line(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
void             initialize_moby_motion_at_position(V_2aa008 *requested_position,
                                                    N_2aa008 *self,
                                                    void     *base_motion_vector) __asm__("FUN_L00_002aa008");

void initialize_moby_motion_at_position(V_2aa008 *requested_position,
                                        N_2aa008 *self,

                                        void *base_motion_vector) {
    V_2aa008     random_offset;
    V_2aa008     vector_scratch;
    V_2aa008     collision_position_offset;
    M_2aa008    *moby_vars;
    struct Hero *P;
    N_2aa008    *scanned_moby;
    s32          count;
    N_2aa008    *best;
    s32          bestv;
    V_2aa008    *k;
    vector_scratch.q = 0;
    moby_vars = self->p78;
    vector_scratch.f[0] = random_float_between(-1.0f, 1.0f);
    vector_scratch.f[1] = random_float_between(-1.0f, 1.0f);
    vector_scratch.f[2] = random_float_between(-1.0f, 1.0f);
    self->b20 = 1;
    self->f2C = self->f2C / 0.65f;
    random_offset.q = vector_scratch.q;
    fast_vec_normalize(&random_offset,
                       &random_offset,
                       random_float_between(D_0015ED6C_2aa008 * 0.01f, D_0015ED6C_2aa008 * 0.1f));
    fast_vec_add(moby_vars, &random_offset, base_motion_vector);
    P = &hero;
    if (P->ground_moby && FUN_L00_002603d0_2aa008(P->ground_moby)) {
        fast_vec_add(moby_vars, moby_vars, &P->motion.displacement);
    }
    moby_vars->h34 = scale_game_frames(0x78);
    count = 0;
    best = 0;
    bestv = 0;
    for (scanned_moby = D_L00_0015FFE4_2aa008; scanned_moby; scanned_moby = scanned_moby->p28) {
        if (scanned_moby->b20 != 0xFE)
            if (scanned_moby->b20 != 0xFD)
                if (scanned_moby->hA6 == self->hA6)
                    if (scanned_moby != self) {
                        if (best == 0 || ((D_2aa008 *)scanned_moby->p78)->i5C < bestv) {
                            bestv = ((D_2aa008 *)scanned_moby->p78)->i5C;
                            best = scanned_moby;
                        }
                        count++;
                    }
    }
    if (count >= 8)
        ((D_2aa008 *)best->p78)->i5C = -1;
    qcopy(&self->v10, requested_position);
    vector_scratch.q = ((Q_2aa008 *)hero.moby)->v10.q;
    vector_scratch.f[2] = requested_position->f[2];
    if (collision_line(&vector_scratch, requested_position, 0, hero.moby, 0)) {
        k = D_L00_00173E60_2aa008;
        qcopy(&self->v10, &k[0]);
        fast_vec_normalize(&collision_position_offset, &k[2], frame_time_sq * 9.7f);
        fast_vec_add(&self->v10, &self->v10, &collision_position_offset);
        reflect_vector(moby_vars, moby_vars, &k[2]);
        fast_vec_scale(moby_vars, moby_vars, 0.6f);
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002AB548), where it is exact; names translated to the US level program. */

extern V     D_L00_00166DC0;
extern char *FUN_L00_002603d0_u(void *) __asm__("FUN_L00_002603d0");
extern int   D_L00_0015F5C4_c __asm__("D_L00_0015F5C4");
extern void  FUN_L00_002a9c50_c(void) __asm__("FUN_L00_002a9c50");

int predict_mine_impact(char *trajectory_state, char *moby, int mode) __asm__("FUN_L00_002aa2a8");

int predict_mine_impact(char *trajectory_state, char *moby, int mode) {
    float trajectory_position[4];
    float B[4];
    float C[4];
    float box[4];
    int   x;
    char *base = D_0013F350;
    int   i;
    char *owner_moby_vars;
    if (*(int *)(base + 0x2084) == 0x72)
        return 0;
    if (D_L00_0015F5C4_c != 0)
        return 0;
    qcopy(C, moby + 0x10);
    if (MOBY(moby)->state == 0) {
        trajectory_position[0] =
            fast_cos(fast_add_rotations(
                -0.36196801f, fast_arc_tan(*(float *)(base + 0x670), *(float *)(base + 0x674)))) *
            0.859039f;
        trajectory_position[1] =
            fast_sin(fast_add_rotations(
                -0.36196801f, fast_arc_tan(*(float *)(base + 0x670), *(float *)(base + 0x674)))) *
            0.859039f;
        *(int *)&trajectory_position[2] = 0;
        fast_vec_add(trajectory_position, trajectory_position, base + 0x80);
        trajectory_position[2] = trajectory_position[2] + 0.49082f;
        qcopy(B, trajectory_state);
        if (*(int *)(base + 0x2FC) != 0 &&
            FUN_L00_002603d0_u((void *)*(int *)(base + 0x2FC)) != 0 && mode != 0) {
            fast_vec_add(B, B, base + 0x100);
        }
        x = scale_game_frames(0x78);
    } else {
        owner_moby_vars = *(char **)(*(char **)(trajectory_state + 0x30) + 0x78);
        qcopy(trajectory_position, moby + 0x10);
        qcopy(B, trajectory_state);
        {
            short xs = *(short *)(trajectory_state + 0x34);
            int   owner_prediction_stop_signal = *(int *)(owner_moby_vars + 0x50);
            x = xs;
            if (owner_prediction_stop_signal != 0)
                *(short *)(trajectory_state + 0x36) = 1;
        }
    }
    {
        char *base2 = D_0013F350;
        char *hero_moby = *(char **)(base2 + 0x2080);
        *(u128 *)box = *(u128 *)(hero_moby + 0x10);
        box[2] = trajectory_position[2];
        if (FUN_001efa68(box, trajectory_position, 0x10, (int)hero_moby, 0) != 0) {
            if (collision_hit_material() != 0 && D_L00_00173E40_u[7] > 0) {
                qcopy(trajectory_state + 0x10, (char *)D_L00_00173E40_u + 0x20);
            }
        }
    }
    while (fast_dec_timer(&x) == 0) {
        qcopy(C, trajectory_position);
        i = 9;
        do {
            fast_vec_add(trajectory_position, trajectory_position, B);
            B[2] = B[2] - frame_time_sq * 9.8f;
        } while (--i >= 0);
        if (FUN_001efa68(C, trajectory_position, 0x12, (int)moby, 0) == 0)
            continue;
        if (collision_hit_material() == 0 && 0.0f < B[2])
            continue;
        if (D_L00_00173E40_u[7] > 0) {
            qcopy(trajectory_state + 0x10, (char *)D_L00_00173E40_u + 0x20);
            break;
        }
    }
    if (x != 0) {
        if (mode == 0) {
            if (coll_hit_moby == 0)
                return 0;
            if (FUN_L00_002603d0_u(coll_hit_moby) == 0)
                return 0;
        }
        if (mode != 2 || coll_hit_moby == 0 || FUN_L00_002603d0_u(coll_hit_moby) == 0) {
            add_draw_callback(FUN_L00_002a9c50_c, moby);
        }
        {
            char *camera_position = (char *)&D_L00_00166DC0;
            char *hit_position = trajectory_state + 0x10;
            *(u128 *)(trajectory_state + 0x20) = *(u128 *)&coll_hit_normal;
            fast_vec_sub(C, hit_position, camera_position);
            fast_vec_scale(C, C, 0.95f);
            fast_vec_add(hit_position, C, camera_position);
        }
        return 1;
    }
    return 0;
}

s32 D_L00_001D7460[1] __attribute__((section(".data"))) = {0};

s16 D_L01_001CBC4C[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001D78E0[1] __attribute__((section(".data"))) = {0};

s16 D_L02_001CBDCC[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001CBEE0[1] __attribute__((section(".data"))) = {0};

s16 D_L03_001CB8CC[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001CB9E0[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001CBA60[1] __attribute__((section(".data"))) = {0};

s16 D_L05_001CBC4C[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001CBD60[1] __attribute__((section(".data"))) = {0};

s16 D_L06_001CBF4C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001CC060[1] __attribute__((section(".data"))) = {0};

s16 D_L07_001CB84C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001CB960[1] __attribute__((section(".data"))) = {0};

s16 D_L08_001CBFCC[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001CC0E0[1] __attribute__((section(".data"))) = {0};

s16 D_L09_001CB94C[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001CBA60[1] __attribute__((section(".data"))) = {0};

s16 D_L10_001CBC4C[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001CBD60[1] __attribute__((section(".data"))) = {0};

s16 D_L11_001CC1CC[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001CC2E0[1] __attribute__((section(".data"))) = {0};

s16 D_L12_001CBC4C[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001CBD60[1] __attribute__((section(".data"))) = {0};

s16 D_L13_001CBACC[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001CBBE0[1] __attribute__((section(".data"))) = {0};

s16 D_L14_001CBECC[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001D87D0[1] __attribute__((section(".data"))) = {0};

s16 D_L15_001CBDCC[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001CBEE0[1] __attribute__((section(".data"))) = {0};

s16 D_L16_001CBC4C[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001CBD60[1] __attribute__((section(".data"))) = {0};

s16 D_L17_001CC0CC[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001CC1E0[1] __attribute__((section(".data"))) = {0};

s16 D_L18_001CC1CC[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001CC2E0[1] __attribute__((section(".data"))) = {0};

struct Amb D_L01_001CBC40 = {0};

int                  D_L01_00161368 = 0x3c;
int                  D_L01_0016136C = 0x3c;
int                  D_L01_0016137C = 0;
int                  D_L01_00161380 = 0;
int                  D_L01_00161388 = 0;
float                D_L01_00161374 = 0;
float                D_L01_00161378 = 0.0025f;
float                D_L01_00161384 = 0;
float                D_L01_0016138C = 0;
float D_L01_001D77E0 NOT_SDA = 0;
float D_L01_001D7870 NOT_SDA = 0;
float                D_L01_00161394 = 0;
float                D_L01_00161398 = 0;
s32                  D_L01_0015FFBC = 0;
int                  D_L01_001613AC = 0;
float                D_L01_001613A8 = 7.2e+02f;
int                  D_L01_001613C0 = 0;
f32                  D_L01_001613BC = 1.1f;
short D_L01_001B0BF0 NOT_SDA = 0;
f32                  D_L01_001613E0 = 0;
struct Amb           D_L02_001CBDC0 = {0};

float D_L02_001CBDE0 NOT_SDA = 0;
float D_L02_001CBE70 NOT_SDA = 0;
float                D_L02_00161368 = 0;
float                D_L02_0016136C = 0;
s32                  D_L02_0015FFBC = 0;
MobyKillTable        D_L02_001BABD0 = {0};

int                  D_L02_00161380 = 0;
float                D_L02_0016137C = 7.2e+02f;
int                  D_L02_00161394 = 0;
f32                  D_L02_00161390 = 1.1f;
void *D_L02_00174458 NOT_SDA = 0;
short D_L02_001B0D70 NOT_SDA = 0;
f32                  D_L02_001613B0 = 0;
struct Amb           D_L03_001CB8C0 = {0};

float D_L03_001CB8E0 NOT_SDA = 0;
float D_L03_001CB970 NOT_SDA = 0;
float                D_L03_00161370 = 0;
float                D_L03_00161374 = 0;
s32                  D_L03_0015FFBC = 0;
int                  D_L03_00161388 = 0;
float                D_L03_00161384 = 7.2e+02f;
int                  D_L03_0016139C = 0;
f32                  D_L03_00161398 = 1.1f;
short D_L03_001B0870 NOT_SDA = 0;
f32                  D_L03_001613C0 = 0;
float D_L04_001CB960 NOT_SDA = 0;
float D_L04_001CB9F0 NOT_SDA = 0;
float                D_L04_00161368 = 0;
float                D_L04_0016136C = 0;
s32                  D_L04_0015FFBC = 0;
MobyKillTable        D_L04_001BA750 = {0};

int                  D_L04_00161380 = 0;
float                D_L04_0016137C = 7.2e+02f;
int                  D_L04_00161394 = 0;
f32                  D_L04_00161390 = 1.1f;
float                D_L04_00161398 = 3.0f;
short D_L04_001B08F0 NOT_SDA = 0;
int                  D_L04_0015F320 = 0;
int                  D_L04_0015F330 = 0;
short                D_L04_0016139C = 0;
int                  D_L04_0015F324 = 0;
int                  D_L04_0015F334 = 0;

f32        D_L04_001613B0 = 0;
struct Amb D_L05_001CBC40 = {0};

float D_L05_001CBC60 NOT_SDA = 0;
float D_L05_001CBCF0 NOT_SDA = 0;
float                D_L05_00161368 = 0;
float                D_L05_0016136C = 0;
s32                  D_L05_0015FFBC = 0;
int                  D_L05_00161380 = 0;
float                D_L05_0016137C = 7.2e+02f;
int                  D_L05_00161398 = 0;
f32                  D_L05_00161394 = 1.1f;
short D_L05_001B0BF0 NOT_SDA = 0;
f32                  D_L05_001613C0 = 0;
struct Amb           D_L06_001CBF40 = {0};

float D_L06_001CBF60 NOT_SDA = 0;
float D_L06_001CBFF0 NOT_SDA = 0;
float                D_L06_00161368 = 0;
float                D_L06_0016136C = 0;
s32                  D_L06_0015FFBC = 0;
MobyKillTable        D_L06_001BAD50 = {0};

MobyKillTable D_L06_001BB9B0 = {0};

int                  D_L06_00161380 = 0;
float                D_L06_0016137C = 7.2e+02f;
int                  D_L06_00161394 = 0;
f32                  D_L06_00161390 = 1.1f;
short D_L06_001B0EF0 NOT_SDA = 0;
f32                  D_L06_001613B0 = 0;
struct Amb           D_L07_001CB840 = {0};

float D_L07_001CB860 NOT_SDA = 0;
float D_L07_001CB8F0 NOT_SDA = 0;
float                D_L07_00161368 = 0;
float                D_L07_0016136C = 0;
s32                  D_L07_0015FFBC = 0;
MobyKillTable        D_L07_001BA650 = {0};

MobyKillTable D_L07_001BB2B0 = {0};

int                  D_L07_00161380 = 0;
float                D_L07_0016137C = 7.2e+02f;
int                  D_L07_00161394 = 0;
f32                  D_L07_00161390 = 1.1f;
float                D_L07_00161398 = 3.0f;
short D_L07_001B07F0 NOT_SDA = 0;
int                  D_L07_0015F320 = 0;
int                  D_L07_0015F330 = 0;
short                D_L07_0016139C = 0;
int                  D_L07_0015F324 = 0;
int                  D_L07_0015F334 = 0;

f32        D_L07_001613B0 = 0;
struct Amb D_L08_001CBFC0 = {0};

float D_L08_001CBFE0 NOT_SDA = 0;
float D_L08_001CC070 NOT_SDA = 0;
float                D_L08_00161368 = 0;
float                D_L08_0016136C = 0;
int                  D_L08_00161380 = 0;
float                D_L08_0016137C = 7.2e+02f;
int                  D_L08_00161394 = 0;
f32                  D_L08_00161390 = 1.1f;
short D_L08_001B0F70 NOT_SDA = 0;
f32                  D_L08_001613B0 = 0;
struct Amb           D_L09_001CB940 = {0};

float D_L09_001CB960 NOT_SDA = 0;
float D_L09_001CB9F0 NOT_SDA = 0;
float                D_L09_001613D8 = 0;
float                D_L09_001613DC = 0;
s32                  D_L09_0015FFBC = 0;
MobyKillTable        D_L09_001BA750 = {0};

int                  D_L09_001613F0 = 0;
float                D_L09_001613EC = 7.2e+02f;
int                  D_L09_0016141C = 0;
f32                  D_L09_00161418 = 1.1f;
void *D_L09_00173FD8 NOT_SDA = 0;
float                D_L09_00161420 = 3.0f;
short D_L09_001B08F0 NOT_SDA = 0;
int                  D_L09_0015F320 = 0;
int                  D_L09_0015F330 = 0;
short                D_L09_00161424 = 0;
int                  D_L09_0015F324 = 0;
int                  D_L09_0015F334 = 0;

f32        D_L09_00161440 = 0;
struct Amb D_L10_001CBC40 = {0};

float D_L10_001CBC60 NOT_SDA = 0;
float D_L10_001CBCF0 NOT_SDA = 0;
float                D_L10_00161368 = 0;
float                D_L10_0016136C = 0;
s32                  D_L10_0015FFBC = 0;
int                  D_L10_00161380 = 0;
float                D_L10_0016137C = 7.2e+02f;
int                  D_L10_00161398 = 0;
f32                  D_L10_00161394 = 1.1f;
float                D_L10_0016139C = 3.0f;
short D_L10_001B0BF0 NOT_SDA = 0;
int                  D_L10_0015F320 = 0;
int                  D_L10_0015F330 = 0;
short                D_L10_001613A0 = 0;
int                  D_L10_0015F324 = 0;
int                  D_L10_0015F334 = 0;

f32        D_L10_001613C0 = 0;
struct Amb D_L11_001CC1C0 = {0};

float D_L11_001CC1E0 NOT_SDA = 0;
float D_L11_001CC270 NOT_SDA = 0;
float                D_L11_00161368 = 0;
float                D_L11_0016136C = 0;
int                  D_L11_00161380 = 0;
float                D_L11_0016137C = 7.2e+02f;
int                  D_L11_00161394 = 0;
f32                  D_L11_00161390 = 1.1f;
short D_L11_001B1170 NOT_SDA = 0;
f32                  D_L11_001613B0 = 0;
struct Amb           D_L12_001CBC40 = {0};

float D_L12_001CBC60 NOT_SDA = 0;
float D_L12_001CBCF0 NOT_SDA = 0;
float                D_L12_00161368 = 0;
float                D_L12_0016136C = 0;
s32                  D_L12_0015FFBC = 0;
MobyKillTable        D_L12_001BAA50 = {0};

MobyKillTable D_L12_001BB6B0 = {0};

int                  D_L12_00161380 = 0;
float                D_L12_0016137C = 7.2e+02f;
int                  D_L12_00161398 = 0;
f32                  D_L12_00161394 = 1.1f;
short D_L12_001B0BF0 NOT_SDA = 0;
f32                  D_L12_001613C0 = 0;
struct Amb           D_L13_001CBAC0 = {0};

float D_L13_001CBAE0 NOT_SDA = 0;
float D_L13_001CBB70 NOT_SDA = 0;
float                D_L13_00161368 = 0;
float                D_L13_0016136C = 0;
s32                  D_L13_0015FFBC = 0;
MobyKillTable        D_L13_001BA8D0 = {0};

MobyKillTable D_L13_001BB530 = {0};

int                  D_L13_00161380 = 0;
float                D_L13_0016137C = 7.2e+02f;
int                  D_L13_0016145C = 0;
f32                  D_L13_00161458 = 1.1f;
float                D_L13_00161460 = 3.0f;
short D_L13_001B0A70 NOT_SDA = 0;
int                  D_L13_0015F320 = 0;
int                  D_L13_0015F330 = 0;
short                D_L13_00161464 = 0;
int                  D_L13_0015F324 = 0;
int                  D_L13_0015F334 = 0;

f32        D_L13_00161480 = 0;
struct Amb D_L14_001CBEC0 = {0};

int                  D_L14_00161368 = 0x3c;
int                  D_L14_0016136C = 0x3c;
int                  D_L14_0016137C = 0;
int                  D_L14_00161380 = 0;
int                  D_L14_00161388 = 0;
float                D_L14_00161374 = 0;
float                D_L14_00161378 = 0.0025f;
float                D_L14_00161384 = 0;
float                D_L14_0016138C = 0;
float D_L14_001D86D0 NOT_SDA = 0;
float D_L14_001D8760 NOT_SDA = 0;
float                D_L14_00161488 = 0;
float                D_L14_0016148C = 0;
s32                  D_L14_0015FFBC = 0;
MobyKillTable        D_L14_001BACD0 = {0};

int                  D_L14_001614A0 = 0;
float                D_L14_0016149C = 7.2e+02f;
int                  D_L14_001615A0 = 0;
f32                  D_L14_0016159C = 1.1f;
short D_L14_001B0E70 NOT_SDA = 0;
f32                  D_L14_001615C0 = 0;
struct Amb           D_L15_001CBDC0 = {0};

float D_L15_001CBDE0 NOT_SDA = 0;
float D_L15_001CBE70 NOT_SDA = 0;
float                D_L15_00161368 = 0;
float                D_L15_0016136C = 0;
s32                  D_L15_0015FFBC = 0;
int                  D_L15_00161380 = 0;
float                D_L15_0016137C = 7.2e+02f;
int                  D_L15_0016147C = 0;
f32                  D_L15_00161478 = 1.1f;
float                D_L15_00161480 = 3.0f;
int                  D_L15_0015F324 = 0;
int                  D_L15_0015F334 = 0;

f32        D_L15_001614A0 = 0;
struct Amb D_L16_001CBC40 = {0};

float D_L16_001CBC60 NOT_SDA = 0;
float D_L16_001CBCF0 NOT_SDA = 0;
float                D_L16_00161368 = 0;
float                D_L16_0016136C = 0;
s32                  D_L16_0015FFBC = 0;
MobyKillTable        D_L16_001BAA50 = {0};

MobyKillTable D_L16_001BB6B0 = {0};

int                  D_L16_00161380 = 0;
float                D_L16_0016137C = 7.2e+02f;
int                  D_L16_0016144C = 0;
f32                  D_L16_00161448 = 1.1f;
short D_L16_001B0BF0 NOT_SDA = 0;
f32                  D_L16_00161470 = 0;
struct Amb           D_L17_001CC0C0 = {0};

float D_L17_001CC0E0 NOT_SDA = 0;
float D_L17_001CC170 NOT_SDA = 0;
float                D_L17_00161368 = 0;
float                D_L17_0016136C = 0;
s32                  D_L17_0015FFBC = 0;
MobyKillTable        D_L17_001BAED0 = {0};

int                  D_L17_00161380 = 0;
float                D_L17_0016137C = 7.2e+02f;
int                  D_L17_0016141C = 0;
f32                  D_L17_00161418 = 1.1f;
short D_L17_001B1070 NOT_SDA = 0;
f32                  D_L17_00161440 = 0;
struct Amb           D_L18_001CC1C0 = {0};

float D_L18_001CC1E0 NOT_SDA = 0;
float D_L18_001CC270 NOT_SDA = 0;
float                D_L18_00161368 = 0;
float                D_L18_0016136C = 0;
s32                  D_L18_0015FFBC = 0;
MobyKillTable        D_L18_001BAFD0 = {0};

MobyKillTable D_L18_001BBC30 = {0};

int                  D_L18_00161380 = 0;
float                D_L18_0016137C = 7.2e+02f;
int                  D_L18_00161434 = 0;
f32                  D_L18_00161430 = 1.1f;
float                D_L18_00161438 = 3.0f;
short D_L18_001B1170 NOT_SDA = 0;
int                  D_L18_0015F320 = 0;
int                  D_L18_0015F330 = 0;
short                D_L18_0016143C = 0;
int                  D_L18_0015F324 = 0;
int                  D_L18_0015F334 = 0;

f32 D_L18_00161450 = 0;

int configured_unscaled_frame_count_gp __asm__("D_L00_00161368") = 0x3c;

int last_frame __asm__("D_L00_001613C0") = 0;

int D_L00_0015F324 = 0;

int D_L00_0015F334 = 0;

short D_L00_001613C8_gp __asm__("D_L00_001613C8") = 0;
