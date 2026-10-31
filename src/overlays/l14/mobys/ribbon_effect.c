/* Level 14 ribbon effect moby class: state, path animation, drawing and hero hits. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/level_spline.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "sda.h"

/* Updates a gem-lock sentry: picks up a sound slot and fires when the player is near. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002E0228), where it is exact; names translated to the US level program. */

extern char D_0013E633[] __asm__("D_0013F3D0");
extern int  trooper_play_sound(int i, int voice_flags, int moby, int k) __asm__("FUN_L00_0028dc90");
void        initialize_ribbon_effect_state(struct Moby *moby) __asm__("FUN_L14_002df080");
void        animate_ribbon_effect_paths(struct Moby *moby) __asm__("FUN_L14_002df138");
void        register_hero_hit_in_clip_volume(struct Moby *moby) __asm__("FUN_L14_002dfc58");

void ribbon_effect_moby_update(struct Moby *moby) __asm__("FUN_L14_002dee28");

/* Moby class 386 */
void ribbon_effect_moby_update(struct Moby *moby) {
    enum {
        MOBY_STATE_INITIALIZE = 0,
        MOBY_STATE_UPDATE = 1
    };
    char        *ribbon_effect_state = (char *)moby->pvars;
    struct Hero *x;
    float        forward_point[4];
    float        bounding_sphere[4];
    int          voice_slot_index;
    switch (moby->state) {
    case MOBY_STATE_INITIALIZE:
        if (*(int *)(ribbon_effect_state + 0x1F4) == -1 ||
            *(int *)(ribbon_effect_state + 0x1F8) == -1 ||
            *(int *)(ribbon_effect_state + 0x1FC) == -1 ||
            *(int *)(ribbon_effect_state + 0x200) < 0) {
            mark_moby_for_removal(moby);
            return;
        }
        initialize_ribbon_effect_state(moby);
        moby->state = 1;
        *(int *)(ribbon_effect_state + 0x204) = -1;
        break;
    case MOBY_STATE_UPDATE:
        x = &hero;
        if (x->state.control_mode != 0xF ||
            (x->rail_spline != (int)l14_splines[*(int *)(ribbon_effect_state + 0x1F4)].path &&
             x->rail_spline != (int)l14_splines[*(int *)(ribbon_effect_state + 0x1F8)].path &&
             x->rail_spline != (int)l14_splines[*(int *)(ribbon_effect_state + 0x1FC)].path)) {
            if (16.0f < absolute_float(x->motion.pos.f[0] - moby->pos.x))
                goto end;
            if (16.0f < absolute_float(x->motion.pos.f[1] - moby->pos.y))
                goto end;
        }
        if (voice_is_playing(moby, *(int *)(ribbon_effect_state + 0x204)) == 0) {
            *(int *)(ribbon_effect_state + 0x204) = trooper_play_sound(6, 4, (int)moby, 0xD3);
        }
        register_hero_hit_in_clip_volume(moby);
        forward_point[0] = fast_cos(moby->rot.z) * 3.125f;
        forward_point[1] = fast_sin(moby->rot.z) * 3.125f;
        forward_point[2] = 0.0f;
        fast_vec_add(forward_point, &moby->pos, forward_point);
        qcopy(bounding_sphere, forward_point);
        bounding_sphere[3] = 4.5f;
        if (fast_bsphere_check((char *)bounding_sphere, 64.0f) >= 0) {
            animate_ribbon_effect_paths(moby);
        }
        return;
    }
end:
    voice_slot_index = *(int *)(ribbon_effect_state + 0x204);
    if (voice_slot_index >= 0) {
        if (voice_slot_index != -1) {
            char *voice_slot_entry = D_0013E633 - 0xE80 + voice_slot_index * 0x70;
            if (*(char **)(voice_slot_entry + 0x88) == moby &&
                *(unsigned char *)(voice_slot_entry + 0x74) != 0) {
                release_voice_slot(voice_slot_index);
            }
        }
        *(int *)(ribbon_effect_state + 0x204) = -1;
    }
}

void initialize_ribbon_effect_state(struct Moby *moby) __asm__("FUN_L14_002df080");

void initialize_ribbon_effect_state(struct Moby *moby) {
    f32 *moby_vars = ((f32 *)moby->pvars);
    f32 *vector_cursor = moby_vars;
    s32  i;
    for (i = 0; i < 10; i++) {
        vector_cursor[0] = moby_vars[0x1F0 / 4] * (f32)i;
        vector_cursor[1] = vector_cursor[2] = 0.0f;
        vector_cursor[3] = 1.0f;
        vector_cursor += 4;
    }
    i = 0;
    vector_cursor = moby_vars;
    for (; i < 20; i++) {
        vector_cursor[0xA0 / 4] = (f32)i * 0.39f;
        vector_cursor[0xA4 / 4] = vector_cursor[0xA8 / 4] = 0.0f;
        vector_cursor[0xAC / 4] = 1.0f;
        vector_cursor += 4;
    }
    moby_vars[0x1E0 / 4] = 0.0f;
    moby_vars[0x1E4 / 4] = 0.0f;
    moby_vars[0x1E8 / 4] = 0.0f;
    moby_vars[0x1EC / 4] = 0.0f;
}

extern float normalize_angle(float) __asm__("FUN_L00_0025e310");

void draw_ribbon_effect(struct Moby *ribbon_moby) __asm__("FUN_L14_002df458");

void draw_ribbon_effect(struct Moby *) __asm__("FUN_L14_002df458");

void animate_ribbon_effect_paths(struct Moby *moby) __asm__("FUN_L14_002df138");

void animate_ribbon_effect_paths(struct Moby *moby) {
    f32 *moby_vars = ((f32 *)moby->pvars);
    f32  step;
    f32  ripple_point_index;
    f32  ripple_height;
    f32  point_fraction_or_sweep_angle;
    f32  sweep_height_amplitude;
    f32  sweep_point_index;
    s32  i;
    s32  j;
    s32  k;
    f32 *rp;
    f32 *wp;
    f32  sixteen;
    f32  one;
    j = 4;
    moby_vars[0x1E0 / 4] = fast_add_rotations(moby_vars[0x1E0 / 4], 0.17453292f);
    moby_vars[0x1E4 / 4] = fast_subtract_rotations(moby_vars[0x1E4 / 4], 0.80285144f);
    moby_vars[0x1E8 / 4] = fast_add_rotations(moby_vars[0x1E8 / 4], 0.05236f);
    step = (360.0f / (convert_integer_to_float(10) * 0.5f)) * 0.017453292f;
    for (i = 0; i < 5; i++) {
        point_fraction_or_sweep_angle = convert_integer_to_float(i) / convert_integer_to_float(10);
        if (i == 0) {
            ripple_height = 0.0f;
        } else {
            ripple_point_index = (f32)i;
            ripple_height =
                fast_sin(normalize_angle(moby_vars[0x1E0 / 4] + (ripple_point_index * step))) *
                0.1f;
            ripple_height += randf_sym(0.0f, 0.2f);
        }
        moby_vars[(i * 4) + 2] = ripple_height;
        moby_vars[(i * 4) + 1] = 0.0f;
        moby_vars[(i * 4) + 0] = (moby_vars[0x1F0 / 4] * ((f32)i)) + 1.6f;
        moby_vars[(i * 4) + 3] = 1.0f;
    }
    k = 5;
    rp = (f32 *)((j * 16) + (u32)(moby_vars + 2));
    wp = &moby_vars[20];
    sixteen = 1.6f;
    one = 1.0f;
    /* Fakematch: the extra loop level rematerialises the 4 and ranks the read
       pointer above the counter in register allocation, as in retail. */
    do {
        for (; k < 10; k++) {
            wp[2] = *rp;
            wp[1] = 0.0f;
            wp[0] = (moby_vars[0x1F0 / 4] * ((f32)k)) + sixteen;
            wp[3] = one;
            wp += 4;
            rp -= 4;
        }
    } while (0);
    for (i = 0; i < 19; i++) {
        sweep_point_index = (f32)i;
        moby_vars[(i * 4) + (0xA8 / 4)] = 0.0f;
        point_fraction_or_sweep_angle =
            normalize_angle(moby_vars[0x1E4 / 4] + (sweep_point_index * 0.99483764f));
        sweep_height_amplitude =
            fast_sin(normalize_angle(moby_vars[0x1E8 / 4] + (sweep_point_index * 0.20944f))) *
            0.75f;
        moby_vars[(i * 4) + (0xA8 / 4)] +=
            (sweep_height_amplitude * fast_sin(point_fraction_or_sweep_angle)) +
            randf_sym(0.0f, 0.2f);
        moby_vars[(i * 4) + (0xA4 / 4)] = fast_cos(point_fraction_or_sweep_angle) * 0.5f;
        moby_vars[(i * 4) + (0xA0 / 4)] = sweep_point_index * 0.39f;
        moby_vars[(i * 4) + (0xAC / 4)] = 1.0f;
    }
    add_draw_callback(draw_ribbon_effect, moby);
}

extern void
draw_ribbon_effect_path(struct Moby *, void *, int, int, int) __asm__("FUN_L14_002df510");

void draw_ribbon_effect(struct Moby *ribbon_moby) {
    char *moby_vars = (char *)ribbon_moby->pvars;
    float updated_texture_u_offset = *(float *)(moby_vars + 0x1EC) - frame_scale * 0.2f;
    *(float *)(moby_vars + 0x1EC) = updated_texture_u_offset;
    if (updated_texture_u_offset <= -8.0f) {
        *(float *)(moby_vars + 0x1EC) = updated_texture_u_offset + 8.0f;
    }
    vu1_add_g_sregister(0x47, 0x51001);
    draw_ribbon_effect_path(ribbon_moby, moby_vars, 0x80, 0x40, 10);
    draw_ribbon_effect_path(ribbon_moby, moby_vars + 0xA0, 0x30, 0x10, 0x14);
}

typedef int u128_df510 __attribute__((mode(TI)));

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_DF510;

typedef struct {
    V_DF510 v[4];
    int     col[4];
    float   uv[4][2];
    long    r70;
    long    tex;
    long    r80;
    long    r88;
} Q_DF510;

extern char D_L14_001674C0[];

extern s64 get_effect_texture(s32) __asm__("func_001F44B8");

extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");

extern void draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");

void draw_ribbon_effect_path(

    struct Moby *ribbon_moby, void *path, int outer_alpha, int inner_alpha, int point_count) {
    V_DF510 *pts = (V_DF510 *)path;
    Q_DF510  layer_quads[2];
    V_DF510  w[2];
    V_DF510  heading_vector;
    V_DF510  quad_to_world[4];
    V_DF510  wm[4];
    V_DF510  world_path_points[2];
    char    *moby_vars = (char *)ribbon_moby->pvars;
    int      inner_layer_color;
    int      i;

    layer_quads[0].tex = get_effect_texture(0x2D);
    layer_quads[0].r88 = 0x8000000048L;
    layer_quads[0].r80 = 0xFF9000000260L;
    layer_quads[0].r70 = 0;
    layer_quads[0].col[0] = layer_quads[0].col[1] = layer_quads[0].col[2] = layer_quads[0].col[3] =
        (outer_alpha << 24) | 0x7F7F7F;
    layer_quads[1].tex = get_effect_texture(0x2C);
    inner_layer_color = (inner_alpha << 24) | 0x7F4040;
    layer_quads[1].col[3] = inner_layer_color;
    layer_quads[1].col[2] = inner_layer_color;
    layer_quads[1].col[1] = inner_layer_color;
    layer_quads[1].col[0] = inner_layer_color;
    layer_quads[1].r88 = layer_quads[0].r88;
    layer_quads[1].r80 = layer_quads[0].r80;
    layer_quads[1].r70 = layer_quads[0].r70;
    layer_quads[1].uv[0][0] = 1.0f;
    layer_quads[1].uv[0][1] = 0.0f;
    layer_quads[1].uv[1][0] = 1.0f;
    layer_quads[1].uv[1][1] = 1.0f;
    layer_quads[1].uv[2][0] = 0.0f;
    layer_quads[1].uv[2][1] = 0.0f;
    layer_quads[1].uv[3][0] = 0.0f;
    layer_quads[1].uv[3][1] = 1.0f;
    layer_quads[0].uv[0][0] = *(float *)(moby_vars + 0x1EC) + 0.0f;
    layer_quads[0].uv[0][1] = 0.0f;
    layer_quads[0].uv[1][0] = *(float *)(moby_vars + 0x1EC) + 0.0f;
    layer_quads[0].uv[1][1] = 1.0f;
    layer_quads[0].uv[2][0] = *(float *)(moby_vars + 0x1EC) + 1.0f;
    layer_quads[0].uv[2][1] = 0.0f;
    layer_quads[0].uv[3][0] = *(float *)(moby_vars + 0x1EC) + 1.0f;
    layer_quads[0].uv[3][1] = 1.0f;
    *(u128_df510 *)&w[0] = 0;
    w[0].w = 1.0f;
    *(u128_df510 *)&w[1] = 0;
    w[0].z = 0.2f;
    w[1].z = 0.4f;
    w[1].w = 1.0f;
    heading_vector.x = fast_cos(ribbon_moby->rot.z);
    heading_vector.y = fast_sin(ribbon_moby->rot.z);
    heading_vector.z = 0.0f;
    qcopy(&quad_to_world[0], &heading_vector);
    fast_vec_sub(&quad_to_world[1], D_L14_001674C0, &ribbon_moby->pos);
    fast_vec_scale(&quad_to_world[1], &quad_to_world[1], 1.0f);
    fast_vec_cross(&quad_to_world[2], &quad_to_world[1], &quad_to_world[0]);
    fast_vec_normalize(&quad_to_world[2], &quad_to_world[2], 1.0f);
    fast_vec_cross(&quad_to_world[0], &quad_to_world[2], &quad_to_world[1]);
    qcopy(&quad_to_world[3], &ribbon_moby->pos);
    euler_rotation_matrix(&wm[0], &ribbon_moby->rot);
    qcopy(&wm[3], &ribbon_moby->pos);
    qcopy(&layer_quads[0].v[0], &w[0]);
    fast_vec_scale(&layer_quads[0].v[1], &layer_quads[0].v[0], -1.0f);
    qcopy(&layer_quads[1].v[0], &w[1]);
    fast_vec_scale(&layer_quads[1].v[1], &layer_quads[1].v[0], -1.0f);
    transform_vector(&world_path_points[0], &pts[0], &wm[0]);
    transform_vector(&world_path_points[1], &pts[1], &wm[0]);
    fast_vec_sub(&quad_to_world[0], &world_path_points[1], &world_path_points[0]);
    fast_vec_normalize(&quad_to_world[0], &quad_to_world[0], 1.0f);
    fast_vec_sub(&quad_to_world[1], D_L14_001674C0, &world_path_points[0]);
    fast_vec_cross(&quad_to_world[2], &quad_to_world[1], &quad_to_world[0]);
    fast_vec_normalize(&quad_to_world[2], &quad_to_world[2], 1.0f);
    fast_vec_cross(&quad_to_world[1], &quad_to_world[0], &quad_to_world[2]);
    qcopy(&quad_to_world[3], &world_path_points[0]);
    transform_vector(&layer_quads[0].v[0], &layer_quads[0].v[0], &quad_to_world[0]);
    transform_vector(&layer_quads[0].v[1], &layer_quads[0].v[1], &quad_to_world[0]);
    transform_vector(&layer_quads[1].v[0], &layer_quads[1].v[0], &quad_to_world[0]);
    transform_vector(&layer_quads[1].v[1], &layer_quads[1].v[1], &quad_to_world[0]);
    for (i = 1; i < point_count; i++) {
        float outer_half_width;
        qcopy(&layer_quads[0].v[2], &w[0]);
        fast_vec_scale(&layer_quads[0].v[3], &layer_quads[0].v[2], -1.0f);
        qcopy(&layer_quads[1].v[2], &w[1]);
        fast_vec_scale(&layer_quads[1].v[3], &layer_quads[1].v[2], -1.0f);
        if (i == point_count - 1) {
            transform_vector(&quad_to_world[3], &pts[i], &wm[0]);
        } else {
            transform_vector(&world_path_points[0], &pts[i], &wm[0]);
            transform_vector(&world_path_points[1], &pts[i + 1], &wm[0]);
            fast_vec_sub(&quad_to_world[0], &world_path_points[1], &world_path_points[0]);
            fast_vec_normalize(&quad_to_world[0], &quad_to_world[0], 1.0f);
            fast_vec_sub(&quad_to_world[1], D_L14_001674C0, &world_path_points[0]);
            fast_vec_cross(&quad_to_world[2], &quad_to_world[1], &quad_to_world[0]);
            fast_vec_normalize(&quad_to_world[2], &quad_to_world[2], 1.0f);
            fast_vec_cross(&quad_to_world[1], &quad_to_world[0], &quad_to_world[2]);
            qcopy(&quad_to_world[3], &world_path_points[0]);
        }
        transform_vector(&layer_quads[0].v[2], &layer_quads[0].v[2], &quad_to_world[0]);
        transform_vector(&layer_quads[0].v[3], &layer_quads[0].v[3], &quad_to_world[0]);
        transform_vector(&layer_quads[1].v[2], &layer_quads[1].v[2], &quad_to_world[0]);
        transform_vector(&layer_quads[1].v[3], &layer_quads[1].v[3], &quad_to_world[0]);
        if (i == 1 && inner_alpha == 0x10) {
            int c0 = layer_quads[0].col[0];
            int c1 = layer_quads[1].col[0];
            layer_quads[0].col[0] = layer_quads[0].col[1] = 0xFFFFFF;
            layer_quads[1].col[0] = layer_quads[1].col[1] = 0x7F4040;
            draw_geometry_quad(&layer_quads[0], 0, 0);
            draw_geometry_quad(&layer_quads[1], 0, 0);
            layer_quads[0].col[3] = c0;
            layer_quads[0].col[2] = c0;
            layer_quads[0].col[1] = c0;
            layer_quads[0].col[0] = c0;
            layer_quads[1].col[3] = c1;
            layer_quads[1].col[2] = c1;
            layer_quads[1].col[1] = c1;
            layer_quads[1].col[0] = c1;
        } else if (i == point_count - 2 && inner_alpha == 0x10) {
            int c0 = layer_quads[0].col[2];
            int c1 = layer_quads[1].col[2];
            layer_quads[0].col[2] = layer_quads[0].col[3] = 0xFFFFFF;
            layer_quads[1].col[2] = layer_quads[1].col[3] = 0x7F4040;
            draw_geometry_quad(&layer_quads[0], 0, 0);
            draw_geometry_quad(&layer_quads[1], 0, 0);
            layer_quads[0].col[3] = c0;
            layer_quads[0].col[2] = c0;
            layer_quads[0].col[1] = c0;
            layer_quads[0].col[0] = c0;
            layer_quads[1].col[3] = c1;
            layer_quads[1].col[2] = c1;
            layer_quads[1].col[1] = c1;
            layer_quads[1].col[0] = c1;
        } else {
            draw_geometry_quad(&layer_quads[0], 0, 0);
            draw_geometry_quad(&layer_quads[1], 0, 0);
        }
        qcopy(&layer_quads[0].v[0], &layer_quads[0].v[2]);
        qcopy(&layer_quads[0].v[1], &layer_quads[0].v[3]);
        qcopy(&layer_quads[1].v[0], &layer_quads[1].v[2]);
        qcopy(&layer_quads[1].v[1], &layer_quads[1].v[3]);
        outer_half_width = cubic_hermite_scalar(-1.0f,
                                                0.0f,
                                                1.0f,
                                                0.0f,
                                                convert_integer_to_float(i) /
                                                    convert_integer_to_float(point_count)) *
                               0.0f +
                           0.2f;
        w[0].z = outer_half_width;
        w[1].z = 2.0f * outer_half_width;
        layer_quads[0].uv[0][0] = *(float *)(moby_vars + 0x1EC) + 0.0f;
        layer_quads[0].uv[0][1] = 0.0f;
        layer_quads[0].uv[1][0] = *(float *)(moby_vars + 0x1EC) + 0.0f;
        layer_quads[0].uv[1][1] = 1.0f;
        layer_quads[0].uv[2][0] = *(float *)(moby_vars + 0x1EC) + 1.0f;
        layer_quads[0].uv[2][1] = 0.0f;
        layer_quads[0].uv[3][0] = *(float *)(moby_vars + 0x1EC) + 1.0f;
        layer_quads[0].uv[3][1] = 1.0f;
    }
}

extern void record_moby_hit(s32, void *) __asm__("FUN_L00_00259a88");

typedef struct {
    f32 v[4];
    u8 *moby;
    s32 flags;
    s8  a;
    s8  b;
    s16 c;
    f32 range;
    s32 one;
} Level14Req;

void register_hero_hit_in_clip_volume(struct Moby *moby) __asm__("FUN_L14_002dfc58");

void register_hero_hit_in_clip_volume(struct Moby *moby) {
    u8        *hero_position = (u8 *)&hero.motion.pos;
    Level14Req hit_request;
    u8        *hero_base;
    if (is_point_inside_clip_volume(hero_position, *(s32 *)(((u8 *)moby->pvars) + 0x200)) != 0) {
        hit_request.flags = 0x200001;
        hit_request.range = 100.0f;
        hit_request.one = 1;
        hit_request.moby = moby;
        clear_u64_value(&hit_request);
        hit_request.v[2] = 1.0f;
        hit_request.v[3] = 5627.925f;
        hit_request.a = 5;
        hit_request.b = 4;
        hit_request.c = ((u16)moby->oclass);
        hero_base = hero_position - 0x80;
        *(s32 *)(hero_base + 0x22A8) = 0;
        record_moby_hit(*(s32 *)(hero_base + 0x2080), &hit_request);
    }
}

extern float fast_sin(float);
