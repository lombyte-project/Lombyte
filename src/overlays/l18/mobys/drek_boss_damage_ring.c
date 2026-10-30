/* Level 18 class 0x274: the ring around Drek's boss robot that pushes and hurts the hero. */
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
/* UpdateMoby_628 */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002DD650), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern void
make_hit_descriptor(char *arg, int a, int b, void *src, float scale) __asm__("FUN_L00_00259888");
extern void
play_entity_sound(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern void record_moby_hit(int, void *) __asm__("FUN_L00_00259a88");
extern void draw_class_274_ring_effect() __asm__("FUN_L18_002dc4b8");
extern void add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");
void        mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

#include "rnc/overlay/l18/mobys/drek_boss_damage_ring_globals.h"

extern float class_274_ring_phase_rate_gp __asm__("D_L18_00161CA0") __attribute__((sda));
void         update_drek_boss_damage_ring(struct Moby *moby) __asm__("FUN_L18_002dc260");

void update_drek_boss_damage_ring(struct Moby *moby) {
    char   hit_descriptor[0x30];
    float  hit_direction_vector[4];
    float *moby_vars = (float *)moby->pvars;
    char  *target_position;
    char  *pos;
    int    flag;

    if (fast_dec_timer((int *)(moby_vars + 2))) {
        mark_moby_for_removal(moby);
        return;
    }
    pos = &moby->pos;
    target_position = D_0013E533 + 0xE9D;
    if (vector_distance_xy(pos, target_position) < moby_vars[0] + 0.5f) {
        fast_vec_sub(hit_direction_vector, target_position, pos);
        hit_direction_vector[2] = 0.0f;
        fast_vec_normalize(hit_direction_vector, hit_direction_vector, frame_time * 4.0f);
        hit_direction_vector[2] = frame_time * 4.0f;
        make_hit_descriptor(hit_descriptor, (int)moby, 0x10001, hit_direction_vector, 1.0f);
        record_moby_hit(*(int *)(target_position + 0x2000), hit_descriptor);
    }
    flag = game_frame & 4;
    moby_vars[1] = moby_vars[1] + class_274_ring_phase_rate_gp * frame_time;
    if (flag == 0) {
        play_entity_sound(*(int *)(moby_vars + 3), moby_vars[0], pos, 0x10000, 1.0f, 1.0f, 0, 1, 0);
    }
    add_draw_callback(draw_class_274_ring_effect, moby);
}

#include "qcopy.h"

extern void *create_moby() __asm__("FUN_0020c4f8");

/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002DD7D0), where it is exact; names translated to the US level program. */

char *
create_class_274_ring_effect(void *owner, void *vector, float value) __asm__("FUN_L18_002dc3e0");

char *create_class_274_ring_effect(void *owner, void *vector, float value) {
    char *moby = create_moby(0x274);
    if (moby != 0) {
        char *data = ((char *)MOBY(moby)->pvars);
        ((unsigned char *)moby)[0x30] = 0xFF;
        qcopy(moby + 0x10, vector);
        *(float *)data = value;
        *(int *)(data + 0x08) = 3;
        *(void **)(data + 0x0C) = owner;
    }
    return moby;
}

extern int scale_game_frames(int);
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002DD848), where it is exact; names translated to the US level program. */

extern s32 D_L18_00161C9C_gp __asm__("D_L18_00161C9C") __attribute__((sda));

void refresh_ring_effect(struct Moby *moby, void *pos, float radius) __asm__("FUN_L18_002dc458");

void refresh_ring_effect(struct Moby *moby, void *pos, float radius) {
    int *moby_vars = (int *)moby->pvars;
    int  countdown_limit_frames;
    qcopy(&moby->pos, pos);
    countdown_limit_frames = *(int *)&D_L18_00161C9C_gp;
    moby_vars[2] += 2;
    *(float *)moby_vars = radius;
    if (scale_game_frames(countdown_limit_frames) < moby_vars[2]) {
        moby_vars[2] = scale_game_frames(*(int *)&D_L18_00161C9C_gp);
    }
}

extern float fast_add_rotations(float, float);
extern float fast_sin(float);

/* Ported from rac1-decomp nonmatching/l18_veldin2/func_L18_002DD8A8.c (func_L18_002DD8A8) */
typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_dc4b8;

typedef struct {
    float u, v;
} UV;

typedef struct {
    V_dc4b8      corner[4];
    unsigned int color[4];
    UV           uv[4];
    long         unk70, tex, unk80, unk88;
} Q_dc4b8;

extern int   get_effect_texture(int) __asm__("FUN_001f44b8");
extern void  draw_quad_dc4b8(void *, void *, int) __asm__("FUN_001f7d30");
extern int   tween_color_dc4b8(int, int, float) __asm__("FUN_001fa6e0");
extern float class_274_ring_quad_uvs[] __asm__("D_L18_001D4330");

#include "rnc/overlay/l18/mobys/drek_boss_damage_ring_data/draw_class_274_ring_effect.h"

extern int   class_274_ring_color_start_gp __asm__("D_L18_00161CB8") __attribute__((sda));
extern int   class_274_ring_color_end_gp __asm__("D_L18_00161CBC") __attribute__((sda));
extern float class_274_ring_segment_angle_degrees_gp __asm__("D_L18_00161CC0") __attribute__((sda));
extern float class_274_ring_draw_height_offset_gp __asm__("D_L18_00161CC4") __attribute__((sda));
extern float D_L18_00161CC8 __attribute__((sda));
extern float class_274_ring_layer_radius_offset_gp __asm__("D_L18_00161CCC") __attribute__((sda));
extern float D_L18_00161CD4 __attribute__((sda));

void draw_class_274_ring_effect(char *moby) {
    Q_dc4b8 quad[4];
    V_dc4b8 quad_draw_transform[4];
    V_dc4b8 ring_segment_rotation_basis[3];
    V_dc4b8 ring_segment_rotation_angles;
    float  *moby_vars;
    int     i, j, k;
    int     layer_color;
    float   layer_phase, r, countdown_ticks_float, color_fade_weight, gi;

    moby_vars = ((float *)MOBY(moby)->pvars);
    set_identity_matrix(quad_draw_transform);
    qcopy(&quad_draw_transform[3], moby + 0x10);
    quad_draw_transform[3].f[2] =
        quad_draw_transform[3].f[2] + class_274_ring_draw_height_offset_gp;
    quad_draw_transform[3].f[3] = 1.0f;
    for (i = 0; i < 4; i++) {
        quad[i].tex = get_effect_texture(14);
        gi = class_274_ring_layer_radius_offset_gp * (float)i;
        quad[i].unk70 = 0;
        quad[i].unk80 = 0xFF9000000260L;
        quad[i].unk88 = (long)class_274_ring_blend_minuend_selector_gp |
                        ((long)class_274_ring_blend_subtrahend_selector_gp << 2) |
                        ((long)class_274_ring_blend_alpha_factor_selector_gp << 4) |
                        ((long)class_274_ring_blend_addend_selector_gp << 6) | 0x8000000000L;
        layer_phase = moby_vars[1] + (float)i * 0.25f;
        r = moby_vars[0] + gi;
        layer_phase = layer_phase - (float)truncate_float_to_s32(layer_phase);
        layer_color = tween_color_dc4b8(
            class_274_ring_color_start_gp, class_274_ring_color_end_gp, layer_phase);
        color_fade_weight = layer_phase * D_L18_00161CD4;
        if (color_fade_weight > 1.0f)
            color_fade_weight = 1.0f;
        layer_color = tween_color_dc4b8(layer_color & 0xFFFFFF, layer_color, color_fade_weight);
        countdown_ticks_float = convert_integer_to_float(((int *)moby_vars)[2]);
        color_fade_weight = countdown_ticks_float / (float)scale_game_frames(D_L18_00161C9C_gp);
        if (color_fade_weight > 1.0f) {
            color_fade_weight = 1.0f;
        } else if (color_fade_weight < 0.0f) {
            color_fade_weight = 0.0f;
        }
        layer_color = tween_color_dc4b8(layer_color & 0xFFFFFF, layer_color, color_fade_weight);
        for (j = 0; j < 4; j++) {
            float corner_angle;
            quad[i].uv[j].u = class_274_ring_quad_uvs[j * 2];
            quad[i].uv[j].v = class_274_ring_quad_uvs[j * 2 + 1];
            quad[i].corner[j].f[2] =
                (j & 1) ? layer_phase * D_L18_00161CC8 : -layer_phase * D_L18_00161CC8;
            corner_angle = fast_add_rotations(
                (float)(j >> 1) * (class_274_ring_segment_angle_degrees_gp * 0.017453292f) -
                    3.14159f,
                class_274_ring_segment_angle_degrees_gp * 0.25f * (float)i * 0.017453292f);
            quad[i].corner[j].f[0] = fast_cos(corner_angle) * r;
            quad[i].corner[j].f[1] = fast_sin(corner_angle) * r;
            quad[i].corner[j].f[3] = 1.0f;
            quad[i].color[j] = layer_color;
        }
    }
    clear_u64_value(&ring_segment_rotation_angles);
    ring_segment_rotation_angles.f[2] = class_274_ring_segment_angle_degrees_gp * 0.017453292f;
    euler_rotation_basis(&ring_segment_rotation_basis[0], &ring_segment_rotation_angles);
    for (k = 0; (float)k < 360.0f / class_274_ring_segment_angle_degrees_gp; k++) {
        draw_quad_dc4b8(&quad[0], quad_draw_transform, 0);
        draw_quad_dc4b8(&quad[1], quad_draw_transform, 0);
        draw_quad_dc4b8(&quad[2], quad_draw_transform, 0);
        draw_quad_dc4b8(&quad[3], quad_draw_transform, 0);
        multiply_matrix3x3(
            quad_draw_transform, quad_draw_transform, &ring_segment_rotation_basis[0]);
    }
}
