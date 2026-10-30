/* Level 14 moby classes: shuttle, mine, elevator, taxi, qwark_oltanis, jet, rocket. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/audio/voice_pool.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/game/item_state.h"
#include "rnc/camera/follow_cam.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/002630b8.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/audio/streaming/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/jet_reset_camera.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/vectors/reflect_vector.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/overlay/shared/audio/streaming/set_moby_dialogue_record_value.h"
#include "rnc/overlay/shared/props/novalis_elevator.h"
#include "rnc/overlay/shared/mobys/spawn_type_52_particle_near_moby.h"
#include "rnc/overlay/shared/props/trackdrive_left.h"
#include "rnc/math/random.h"

/* Taxi stop selection shared by route helpers. */
enum TaxiStop {
    TAXI_STOP_RETURN_HUB = 0x0,
    TAXI_STOP_HUB_OUTBOUND_DESTINATION = 0x1,
    TAXI_STOP_RETURN_FEEDER_ORIGIN = 0x2,
    TAXI_STOP_RETURN_FEEDER_DESTINATION = 0x3
};

/* 0x002DF080, 180 bytes.  Initialises two tables of spaced vectors in the
 * moby's data block and clears four fields.
 * Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c:
 * func_L14_002E0480), where it is exact; names translated to the US level
 * program. */
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
void         draw_ribbon_effect(struct Moby *ribbon_moby) __asm__("FUN_L14_002df458");

void draw_ribbon_effect(struct Moby *) __asm__("FUN_L14_002df458");

/* 0x002DF138, 796 bytes.  Advances the three rotation phases and rebuilds
 * both vector tables: the first half of table 0 is a sine ripple, the second
 * half mirrors it (entry 5+n copies the Z of entry 4-n); table 1 is a cosine
 * sweep.  Then re-queues the update callback.
 *
 * Shape notes (ee-gcc 2.95.2 is sensitive to all of these):
 * - `j = 4` is set before the first loop so loop.c never sees it as the
 *   mirror pointer's initial value (the backward scan for biv inits stops at a
 *   label); update_equiv_regs then sinks the set-once/used-once constant to
 *   its single use, which yields retail's `li v0,4; sll; addiu 8; addu`.
 * - The mirror loop sits inside a `do { } while (0)` so flow's loop-depth
 *   weighting (REG_N_REFS += loop_depth) ranks the read pointer above the
 *   counter in global alloc ($a0/$a1 as in retail).  The pointers, counter and
 *   float constants are initialised before the dummy loop because loop.c
 *   treats a label-less loop as "phony" and would otherwise leave the hoisted
 *   insns after the LOOP_BEG note, where haifa turns the note into a
 *   scheduling barrier.
 * - rp is formed as an integer sum so fold keeps `d` as the last operand
 *   (`addu a0, v0, s1`), the way loop.c's giv init would have emitted it. */
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

/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002E0858), where it is exact; names translated to the US level program. */

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

/* Draws a two-layer ribbon along a path of points: each segment builds a basis from the segment direction and
 * the camera, sweeps two textured quads (an outer glow and an inner core) across it and, for the alpha-0x10
 * variant, brightens the first and last segments. The texture scrolls with the moby's pvars+0x1EC. */
#include "qcopy.h"

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

extern char  D_L14_001674C0[];
extern s64   get_effect_texture(s32) __asm__("func_001F44B8");
extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");
extern void  draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");

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

/* 0x002DFC58, 184 bytes.  If the moby's check passes, builds an effect
 * request on the stack and submits it.
 * Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c:
 * func_L14_002E1058), where it is exact; names translated to the US level
 * program. */
void register_hero_hit_in_clip_volume(struct Moby *moby) __asm__("FUN_L14_002dfc58");

void register_hero_hit_in_clip_volume(struct Moby *moby) {
    u8        *base = (u8 *)&hero.motion.pos;
    Level14Req hit_request;
    u8        *hero_base;
    if (is_point_inside_clip_volume(base, *(s32 *)(((u8 *)moby->pvars) + 0x200)) != 0) {
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
        hero_base = base - 0x80;
        *(s32 *)(hero_base + 0x22A8) = 0;
        record_moby_hit(*(s32 *)(hero_base + 0x2080), &hit_request);
    }
}

/* pvars of the lift that carries a rider moby and pushes the hero off its edge */
typedef struct {
    u8  pad0[0x74];
    s32 path; /* 0x74: path index into D_L14_001B0BB0 */
    u8  pad78[0x84];
    f32 height; /* 0xFC */
    u8  pad100[0x50];
    s32 rider;     /* 0x150: moby index carried along */
    s32 anchor;    /* 0x154: joint the rider follows, -1 for the moby position */
    s32 mode;      /* 0x158: 0x3039 when the rider is attached */
    s32 sound;     /* 0x15C */
    f32 offset[4]; /* 0x160 */
    u8  pad170[0x10];
    u8  spring[0x40]; /* 0x180 */
    u8  manip[0x10];  /* 0x1C0 */
    u8  wobble[0x30]; /* 0x1D0 */
    f32 angle;        /* 0x200 */
    s16 riding;       /* 0x204 */
    s16 h206;         /* 0x206 */
    u8  pad208[4];
    s16 timer; /* 0x20C */
    s16 pad20E;
    f32 timer_inv; /* 0x210 */
    f32 from;      /* 0x214 */
    f32 to;        /* 0x218 */
    s32 clip;      /* 0x21C */
} L14LiftVars;

extern struct Moby *D_L14_0015FFD8_lift __asm__("D_L14_0015FFD8");
extern char         shuttle_missing_target_error_format[] __asm__("D_L14_001FC840");

#include "rnc/overlay/l14/mobys/jet_globals.h"

extern s32 shuttle_enter_riding_transition_frames_gp __asm__("D_L14_00161B8C") __attribute__((sda));
extern s32 shuttle_exit_riding_transition_frames_gp __asm__("D_L14_00161B90") __attribute__((sda));
extern f32 shuttle_horizontal_rider_correction_limit_scale_gp __asm__("D_L14_00161B88")
    __attribute__((sda));
extern void debug_print(const char *, short) __asm__("FUN_001e93b0");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void FUN_L00_001fff28(void *, int, float);

void shuttle_update(struct Moby *m) __asm__("FUN_L14_002e7bd8");

void shuttle_update(struct Moby *shuttle_moby) {
    L14LiftVars *shuttle_vars;
    Vec4         old;
    Vec4         rot;
    Vec4         delta;
    Vec4         base;
    Vec4         rotated_rider_offset;
    f32          shuttle_rotation_matrix[16];
    struct Moby *rider;
    f32          horizontal_rider_correction_distance;
    f32          max;

    if (shuttle_moby == 0)
        return;
    shuttle_vars = (L14LiftVars *)shuttle_moby->pvars;
    if (shuttle_vars == 0)
        return;
    if (shuttle_vars->clip >= 0 &&
        is_point_inside_clip_volume(&hero.motion.pos, shuttle_vars->clip) != 0) {
        shuttle_moby->collision = 0;
        shuttle_moby->flags |= 0x41;
        if (shuttle_vars->rider >= 0) {
            struct Moby *r = &D_L14_0015FFD8_lift[shuttle_vars->rider];
            r->collision = 0;
            r->flags = (r->flags | 0x41) & 0xEFFF;
        }
        return;
    }
    if (shuttle_moby->collision == 0) {
        shuttle_moby->flags &= 0xFFBE;
        shuttle_moby->collision = shuttle_moby->pclass->collision;
        if (shuttle_vars->rider >= 0) {
            struct Moby *r = &D_L14_0015FFD8_lift[shuttle_vars->rider];
            r->flags &= 0xFFBE;
            r->collision = r->pclass->collision;
            r->flags |= 0x1000;
        }
    }
    qcopy(&old, &shuttle_moby->pos);
    qcopy(&rot, &shuttle_moby->rot);
    if (shuttle_moby->state == 0) {
        qcopy(&shuttle_moby->pos, ((char *)l14_paths[shuttle_vars->path]) + 0x10);
        shuttle_vars->angle = 0.0f;
        shuttle_vars->sound = -1;
        shuttle_vars->h206 = 0;
        attach_manipulator(shuttle_moby, 0, shuttle_vars->manip);
        if (shuttle_vars->rider == -1) {
            debug_print(shuttle_missing_target_error_format, shuttle_moby->save_id);
            mark_moby_for_removal(shuttle_moby);
            return;
        }
        shuttle_vars->riding = 0;
        shuttle_vars->from = shuttle_vars->height;
    }
    gunship_follow_path(shuttle_moby);
    rider = &D_L14_0015FFD8_lift[shuttle_vars->rider];
    if (shuttle_vars->mode == 0x3039) {
        if (shuttle_vars->anchor != -1)
            moby_point_position(shuttle_moby, shuttle_vars->anchor, &base);
        else
            qcopy(&base, &shuttle_moby->pos);
        euler_rotation_matrix(shuttle_rotation_matrix, &shuttle_moby->rot);
        transform_vector(&rotated_rider_offset, shuttle_vars->offset, shuttle_rotation_matrix);
        fast_vec_add(&rider->pos, &base, &rotated_rider_offset);
    }
    if (shuttle_vars->sound == -1 ||
        (voice_is_playing(shuttle_moby, shuttle_vars->sound) == 0 && shuttle_vars->sound == -1))
        shuttle_vars->sound = allocate_voice_for_target_entry(0, 4, shuttle_moby);
    shuttle_vars->angle = fast_add_rotations(shuttle_vars->angle, 0.17453292f);
    FUN_L00_001fff28(shuttle_vars->wobble, 0, shuttle_vars->angle);
    fast_vec_sub(&delta, &shuttle_moby->pos, &old);
    update_entity_matrix(shuttle_vars->spring, &delta, &rot, &shuttle_moby->rot);
    if (hero.state.control_mode == 13 && hero.grapple_target == rider) {
        if (shuttle_vars->riding == 0) {
            shuttle_vars->riding = 1;
            shuttle_vars->timer = scale_game_frames(shuttle_enter_riding_transition_frames_gp);
            shuttle_vars->timer_inv = 1.0f / convert_integer_to_float(shuttle_vars->timer);
        }
    } else {
        if (shuttle_vars->riding == 1) {
            if (hero.air_frames.s == 0 || hero.motion.pos.f[2] <= shuttle_moby->pos.z - 3.0f) {
                shuttle_vars->riding = 0;
                shuttle_vars->timer = scale_game_frames(shuttle_exit_riding_transition_frames_gp);
                shuttle_vars->timer_inv = 1.0f / convert_integer_to_float(shuttle_vars->timer);
                shuttle_vars->to = shuttle_vars->height;
            } else {
                qcopy(&base, &hero.motion.pos);
                fast_vec_sub(&delta, &rider->pos, &base);
                delta.f[2] = 0.0f;
                horizontal_rider_correction_distance = fast_vec_length(&delta);
                max = shuttle_horizontal_rider_correction_limit_scale_gp * frame_time;
                if (max < horizontal_rider_correction_distance)
                    horizontal_rider_correction_distance = max;
                fast_vec_normalize(
                    &hero.motion.platform_delta, &delta, horizontal_rider_correction_distance);
            }
        }
        if (shuttle_vars->riding == 0) {
            fast_dec_timer_s16(&shuttle_vars->timer);
            {
                f32 t = convert_integer_to_float(shuttle_vars->timer) * shuttle_vars->timer_inv;
                shuttle_vars->height =
                    shuttle_vars->from + (shuttle_vars->to - shuttle_vars->from) * t;
            }
            return;
        }
    }
    fast_dec_timer_s16(&shuttle_vars->timer);
    {
        f32 t = convert_integer_to_float(shuttle_vars->timer) * shuttle_vars->timer_inv;
        shuttle_vars->height = (shuttle_vars->from - 0.005f) * t + 0.005f;
    }
}

typedef struct {
    s32  clip_push; /* 0x0: clip volume that pushes the hero */
    char pad4[4];
    f32  push;       /* 0x8 */
    s16  push_timer; /* 0xC */
    s16  pushing;    /* 0xE */
    f32  push_inv;   /* 0x10 */
    f32  f14, f18, f1C;
    char pad20[0x4E0];
    s32  clip_area;    /* 0x500 */
    s32  point;        /* 0x504 */
    s16  period;       /* 0x508 */
    s16  period_timer; /* 0x50A */
    f32  period_inv;   /* 0x50C */
} L14WindVars;

#include "rnc/overlay/cuboid.h"

/* wind applied to the sparks: direction scaled by strength, and a weight */
extern struct {
    f32 v;
} D_L14_00162230, D_L14_00162234;

extern s32 D_L14_00162238;
extern f32 D_L14_00162240;
char       D_L14_001FC980[] =
    "*Error: Moby 610 (wind-control) with no cuboid index (wind pushing hero area). UID: %d - RAR\n";
char D_L14_001FC9E0[] =
    "*Error: Moby 610 (wind-control) with no fxArea cuboid (fx drawing area). UID: %d - RAR\n";
char D_L14_001FCA38[] =
    "*Error: Moby 610 (wind-control) with no fxAct cuboid (fx activation area). UID: %d - RAR\n";
void update_wind_sparks(struct Moby *wind_control_moby) __asm__("FUN_L14_002eb568");

void update_wind_sparks(struct Moby *m) __asm__("FUN_L14_002eb568");
void initialize_wind_spark(struct Moby *wind_control_moby,
                           int          i,
                           void        *spark_travel_direction) __asm__("FUN_L14_002eb388");

void initialize_wind_spark(struct Moby *m, int i, void *dir) __asm__("FUN_L14_002eb388");

extern f32 wind_effect_strength_gp __asm__("D_L14_00161C08") __attribute__((sda));
extern s32 wind_push_decay_frames_gp __asm__("D_L14_00161C04") __attribute__((sda));
void       update_wind_control(struct Moby *wind_control_moby) __asm__("FUN_L14_002eaf88");

void update_wind_control(struct Moby *wind_control_moby) {
    L14WindVars *wind_vars = (L14WindVars *)wind_control_moby->pvars;
    float        wind_direction_or_push_delta[4];
    char        *pos;
    struct Hero *hero_base;
    int          i;

    if (wind_control_moby->state == 0) {
        wind_control_moby->state = 1;
        if (wind_vars->clip_push < 0) {
            debug_print(D_L14_001FC980, wind_control_moby->save_id);
            mark_moby_for_removal(wind_control_moby);
            return;
        }
        if (wind_vars->point < 0) {
            debug_print(D_L14_001FC9E0, wind_control_moby->save_id);
            mark_moby_for_removal(wind_control_moby);
            return;
        }
        if (wind_vars->clip_area < 0) {
            debug_print(D_L14_001FCA38, wind_control_moby->save_id);
            mark_moby_for_removal(wind_control_moby);
            return;
        }
        wind_vars->pushing = 0;
        wind_vars->f14 = 2.0f * fast_vec_length((char *)l14_cuboids + (wind_vars->point << 7));
        wind_vars->f18 = fast_vec_length((char *)l14_cuboids + (wind_vars->point << 7) + 0x10);
        wind_vars->f1C = fast_vec_length((char *)l14_cuboids + (wind_vars->point << 7) + 0x20);
        wind_vars->period_timer = scale_game_frames(wind_vars->period);
        wind_vars->period_inv = 1.0f / convert_integer_to_float(wind_vars->period_timer);
        wind_direction_or_push_delta[0] = fast_cos(wind_control_moby->rot.z);
        wind_direction_or_push_delta[1] = fast_sin(wind_control_moby->rot.z);
        wind_direction_or_push_delta[2] = 0.0f;
        for (i = 0; i < 48; i++) {
            initialize_wind_spark(wind_control_moby, i, wind_direction_or_push_delta);
        }
    }
    pos = (char *)&hero.motion.pos;
    if (is_point_inside_clip_volume(pos, wind_vars->clip_area) != 0) {
        D_L14_00162230.v =
            fast_cos(wind_control_moby->rot.z) * (wind_effect_strength_gp * frame_time);
        D_L14_00162234.v =
            fast_sin(wind_control_moby->rot.z) * (wind_effect_strength_gp * frame_time);
        D_L14_00162238 = 0;
        D_L14_00162240 = 1.0f;
        /* Fakematch: storing the weight again is a no-op that reload's cse drops, but the
         * branch around it keeps the call's argument move out of the stores above, so it
         * lands in the call's delay slot as in retail. */
        if (wind_control_moby != 0) {
            D_L14_00162240 = 1.0f;
        }
        update_wind_sparks(wind_control_moby);
        hero_base = (struct Hero *)(pos - 0x80);
        if (hero_base->unk308 == 0 && hero_base->unk2284 != 13 &&
            (hero_base->on_ice_ground != 0 || hero_base->frames_off_ground.s != 0) &&
            is_point_inside_clip_volume(pos, wind_vars->clip_push) != 0) {
            float t;
            float k;
            fast_dec_timer_s16(&wind_vars->period_timer);
            t = convert_integer_to_float(wind_vars->period_timer) * wind_vars->period_inv;
            wind_vars->pushing = 1;
            k = frame_time;
            t = 1.0f - t;
            t = t * (wind_vars->push * k);
            wind_direction_or_push_delta[0] = fast_cos(wind_control_moby->rot.z) * t;
            wind_direction_or_push_delta[1] = fast_sin(wind_control_moby->rot.z) * t;
            wind_direction_or_push_delta[2] = 0.0f;
            fast_vec_add(pos + 0x70, pos + 0x70, wind_direction_or_push_delta);
            wind_vars->push_timer = scale_game_frames(wind_push_decay_frames_gp);
            wind_vars->push_inv = 1.0f / convert_integer_to_float(wind_vars->push_timer);
            return;
        }
    }
    if (wind_vars->pushing != 0 && wind_vars->push_timer != 0) {
        float t;
        if (fast_dec_timer_s16(&wind_vars->push_timer) != 0) {
            wind_vars->pushing = 0;
            wind_vars->period_timer = scale_game_frames(wind_vars->period);
            wind_vars->period_inv = 1.0f / convert_integer_to_float(wind_vars->period_timer);
        }
        if (hero.unk308 != 0 || hero.on_ice_ground == 0) {
            if (wind_vars->push_timer > scale_game_frames(15)) {
                wind_vars->push_timer = scale_game_frames(15);
                wind_vars->push_inv = 1.0f / (float)wind_vars->push_timer;
            }
        }
        t = (wind_vars->push * frame_time - 0.0f) *
                (convert_integer_to_float(wind_vars->push_timer) * wind_vars->push_inv) +
            0.0f;
        wind_direction_or_push_delta[0] = fast_cos(wind_control_moby->rot.z) * t;
        wind_direction_or_push_delta[1] = fast_sin(wind_control_moby->rot.z) * t;
        wind_direction_or_push_delta[2] = 0.0f;
        fast_vec_add(
            &hero.motion.platform_delta, &hero.motion.platform_delta, wind_direction_or_push_delta);
    }
}

/* Starts one spark: places it at a random spot around the emitter, picks its speed and its life. */
#include "qcopy.h"

extern float D_0015ED6C_k __asm__("D_0015ED6C");
extern int   ftoi_k(float) __asm__("FUN_001fa6d0");

typedef struct {
    char  pad0[0x14];
    float f14, f18, f1C;
    float pos[48][4];
    short n320[48];
    short n380[48];
    short n3E0[48];
    float f440[48];
    int   pad500;
    int   i504;
} Spk_k;

extern int   wind_spark_speed_range_start_gp __asm__("D_L14_00161C30") __attribute__((sda));
extern int   wind_spark_speed_range_end_gp __asm__("D_L14_00161C34") __attribute__((sda));
extern float wind_spark_size_range_start_gp __asm__("D_L14_00161C38") __attribute__((sda));
extern float wind_spark_size_range_end_gp __asm__("D_L14_00161C3C") __attribute__((sda));

void initialize_wind_spark(struct Moby *wind_control_moby, int i, void *spark_travel_direction) {
    Spk_k *spark_vars = (Spk_k *)wind_control_moby->pvars;
    float  fwd[4];
    float  side[4];
    float  up[4];
    float  position_offset[4];
    float  x, y, z;
    char  *spark_position;
    int    lifetime_frames;
    qcopy(fwd, spark_travel_direction);
    up[0] = 0.0f;
    up[2] = 1.0f;
    up[1] = 0.0f;
    up[3] = 0.0f;
    fast_vec_cross(side, fwd, up);
    qcopy(spark_vars->pos[i], &l14_cuboids[spark_vars->i504].pos);
    x = -spark_vars->f14;
    y = randf_sym(0.0f, spark_vars->f18);
    z = randf_sym(0.0f, spark_vars->f1C);
    fast_vec_scale(position_offset, fwd, x);
    spark_position = (char *)spark_vars->pos[i];
    fast_vec_add(spark_position, spark_position, position_offset);
    fast_vec_scale(position_offset, side, y);
    fast_vec_add(spark_position, spark_position, position_offset);
    fast_vec_scale(position_offset, up, z);
    fast_vec_add(spark_position, spark_position, position_offset);
    spark_vars->pos[i][3] =
        random_float_between(wind_spark_size_range_start_gp, wind_spark_size_range_end_gp);
    spark_vars->f440[i] =
        random_float_between(wind_spark_speed_range_start_gp, wind_spark_speed_range_end_gp) *
        D_0015ED6C_k;
    lifetime_frames = scale_game_frames(ftoi_k(spark_vars->f14 * 2.0f / spark_vars->f440[i]));
    spark_vars->n320[i] = lifetime_frames;
    spark_vars->n380[i] = lifetime_frames;
    spark_vars->n3E0[i] = 0;
}

extern int spark_float_to_integer(float) __asm__("FUN_001fa6d0");

struct L14SparkTarget {
    char  pad0[0x140];
    float x; /* 0x140 */
    float y; /* 0x144 */
    char  pad148[0x10];
    float yaw; /* 0x158 */
};

extern struct L14SparkTarget D_L14_00167380_spark __asm__("D_L14_00167380");
void                         draw_wind_sparks(struct Moby *m) __asm__("FUN_L14_002eb810");

extern int wind_spark_peak_alpha_gp __asm__("D_L14_00161C10") __attribute__((sda));
extern int wind_spark_fade_in_frames_gp __asm__("D_L14_00161C2C") __attribute__((sda));

void update_wind_sparks(struct Moby *wind_control_moby) {
    Spk_k *spark_vars = (Spk_k *)wind_control_moby->pvars;
    float  spark_travel_direction[4];
    float  clip[4];
    float  work[4];
    float  ratio;
    float  first;
    int    in_clip;
    int    i;
    int    opacity;
    short *shade;
    float *pos;
    spark_travel_direction[0] = fast_cos(wind_control_moby->rot.z);
    spark_travel_direction[1] = fast_sin(wind_control_moby->rot.z);
    spark_travel_direction[2] = 0.0f;
    qcopy(clip, &hero.motion.pos);
    clip[2] += 1.0f;
    in_clip = is_point_inside_clip_volume(clip, spark_vars->i504);
    i = 0;
    pos = spark_vars->pos[0];
    shade = spark_vars->n3E0;
    do {
        if (fast_dec_timer_s16(&spark_vars->n320[i]) != 0) {
            if (in_clip == 0 || i < 24) {
                initialize_wind_spark(wind_control_moby, i, spark_travel_direction);
            }
        } else {
            opacity = wind_spark_peak_alpha_gp;
            if (spark_vars->n380[i] - wind_spark_fade_in_frames_gp < spark_vars->n320[i]) {
                first = convert_integer_to_float(spark_vars->n380[i] - spark_vars->n320[i]);
                first = first / convert_integer_to_float(wind_spark_fade_in_frames_gp);
            } else {
                first = convert_integer_to_float(spark_vars->n320[i]);
                first = first / convert_integer_to_float(spark_vars->n380[i] -
                                                         wind_spark_fade_in_frames_gp);
            }
            {
                int spark_alpha = spark_float_to_integer(((float)opacity - 0.0f) * first + 0.0f);
                *shade = spark_alpha;
                /* Fakematch: storing the shade again is a no-op that reload's cse drops; the
                 * extra uses rank `shade` above `m` for register allocation as in retail. */
                if (spark_alpha != 0) {
                    *shade = spark_alpha;
                }
            }
            fast_vec_scale(work, spark_travel_direction, spark_vars->f440[i]);
            fast_vec_add(pos, pos, work);
            if (in_clip != 0 && absolute_float(pos[0] - D_L14_00167380_spark.x) < 8.0f &&
                absolute_float(pos[1] - D_L14_00167380_spark.y) < 8.0f &&
                fast_difference_between_rotations(wind_control_moby->rot.z,
                                                  D_L14_00167380_spark.yaw) > 2.3561945f) {
                if (spark_vars->n320[i] > scale_game_frames(20)) {
                    spark_vars->n320[i] = scale_game_frames(20);
                }
            }
        }
        pos += 4;
        shade++;
        i++;
    } while (i < 48);
    add_draw_callback(draw_wind_sparks, wind_control_moby);
}

/* Draws every live spark as a quad that faces the camera, faded by its age. */
typedef unsigned int u128_m __attribute__((mode(TI)));

typedef struct {
    float         m[4][4];
    u32           col[4];
    float         uv[8];
    unsigned long a, b, c, d;
} Pk_m;

extern char D_L14_001674C0_m[] __asm__("D_L14_001674C0");

extern int wind_spark_base_color_gp __asm__("D_L14_00161C0C") __attribute__((sda));

float D_L14_001DFD40[4][4] = {
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
};

void draw_wind_sparks(struct Moby *m) {
    Spk_k *d = (Spk_k *)m->pvars;
    Pk_m   pk;
    float  spark_quad_to_world[4][4];
    float  up[4];
    u32    col;
    int    i;
    int    k;
    pk.b = get_effect_texture(0x30);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    vu1_add_g_sregister(0x4A, 0);
    vu1_add_g_sregister(0x47, 0x51001);
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    for (i = 0; i < 48; i++) {
        if (d->n320[i] != 0) {
            col = wind_spark_base_color_gp | (d->n3E0[i] << 24);
            pk.col[3] = col;
            pk.col[2] = col;
            pk.col[1] = col;
            pk.col[0] = col;
            qcopy(spark_quad_to_world[3], d->pos[i]);
            *(u128_m *)up = 0;
            up[2] = 1.0f;
            up[3] = 1.0f;
            fast_vec_sub(spark_quad_to_world[0], d->pos[i], D_L14_001674C0_m);
            fast_vec_normalize(spark_quad_to_world[0], spark_quad_to_world[0], 1.0f);
            fast_vec_cross(spark_quad_to_world[1], spark_quad_to_world[0], up);
            fast_vec_normalize(spark_quad_to_world[1], spark_quad_to_world[1], 1.0f);
            fast_vec_cross(spark_quad_to_world[2], spark_quad_to_world[1], spark_quad_to_world[0]);
            for (k = 0; k < 4; k++) {
                fast_vec_scale(
                    pk.m[k], D_L14_001DFD40[k], ((Spk_k *)((char *)d + (i << 4)))->pos[0][3]);
                transform_vector(pk.m[k], pk.m[k], spark_quad_to_world);
            }
            draw_geometry_quad(&pk, 0, 0);
        }
    }
}

/* pvars of the drifting mine that follows a looped path */
typedef struct {
    u8  pad0[0x20];
    f32 health[0x10]; /* 0x20 */
    s32 path;         /* 0x60 */
    s32 segment;      /* 0x64 */
    f32 along;        /* 0x68 */
    f32 dir;          /* 0x6C */
    f32 vel[3];       /* 0x70 */
    f32 phase;        /* 0x7C */
    f32 spin[3];      /* 0x80 */
} L14MineVars;

typedef struct {
    s32 count;
    s32 pad4[3];
    struct {
        f32 pos[3];
        f32 length;
    } points[1];
} L14MinePath;

typedef struct {
    u8           pad0[0x20];
    struct Moby *moby; /* 0x20 */
} L14MineHit;

extern char  looping_path_moby_missing_path_error_format[] __asm__("D_L14_001FCA98");
extern char  looping_path_moby_empty_path_error_format[] __asm__("D_L14_001FCAD8");
extern f32   mine_path_step_rate_gp __asm__("D_L14_00161C44") __attribute__((sda));
extern f32   mine_glow_pulse_phase_step_gp __asm__("D_L14_00161C48") __attribute__((sda));
extern float FUN_L00_00257c48(float, float);
extern int
FUN_L00_0025d808_mine(void *, float *, int *, float *, int, float) __asm__("FUN_L00_0025d808");
extern float FUN_L00_0025b8c0(float *p, float *v, float t, float u1, float u2, float eps);
extern s32
fun_00213928(struct Moby *, void *, f32 *, s32, s32 *, s32, s32, s32) __asm__("FUN_00213928");
extern void mine_explode(void *,
                         void *,
                         void *,
                         float,
                         float,
                         int,
                         int,
                         int,
                         float,
                         float,
                         float,
                         float,
                         int,
                         float,
                         int,
                         int,
                         int,
                         int) __asm__("FUN_L00_0025e450");
extern int  mine_ftoi(float) __asm__("FUN_001fa6d0");

/* Mine update: drifts along its looped path while spinning and pulsing; blows up on a hit. */
void update_looping_path_moby(struct Moby *moby) __asm__("FUN_L14_002ec810");

void update_looping_path_moby(struct Moby *moby) {
    enum {
        MINE_STATE_INITIALIZE = 0,
        MINE_STATE_FOLLOW_PATH = 1
    };
    L14MineVars *mine_vars = (L14MineVars *)moby->pvars;
    float        path_target_position[4];
    Vec4         vel;
    s32          count;
    L14MinePath *path;
    L14MineHit  *hit;
    int          i;
    float        glow_pulse_sine;
    int          glow_red_intensity;
    int          glow_green_blue_intensity;

    switch (moby->state) {
    case MINE_STATE_INITIALIZE:
        if (mine_vars->path == -1) {
            debug_print(looping_path_moby_missing_path_error_format, moby->save_id);
            mark_moby_for_removal(moby);
            return;
        }
        path = ((L14MinePath *)l14_paths[mine_vars->path]);
        if (path->count == 0) {
            debug_print(looping_path_moby_empty_path_error_format, moby->save_id);
            mark_moby_for_removal(moby);
            return;
        }
        for (i = 0; i < path->count; i++) {
            f32 path_segment_length = fast_vec_dist(
                &((f32 *)path)[i * 4 + 4], &((f32 *)path)[((i + 1) % path->count) * 4 + 4]);
            char *pt = (char *)path + i * 16;
            *(f32 *)(pt + 0x1C) = path_segment_length;
        }
        mine_vars->segment = 0;
        mine_vars->along = 0;
        mine_vars->dir = random_integer_below(2) != 0 ? 1.0f : -1.0f;
        qcopy(&moby->pos, path->points[0].pos);
        moby->state = 1;
        mine_vars->spin[0] = random_float_between(0.05235988f, 0.10471976f);
        mine_vars->spin[1] = random_float_between(0.034906585f, 0.06981317f);
        mine_vars->spin[2] = random_float_between(0.017453292f, 0.034906585f);
        clear_u64_value(mine_vars->vel);
        mine_vars->phase = FUN_L00_00257c48(0.0f, 3.1415927f);
        break;
    case MINE_STATE_FOLLOW_PATH:
        FUN_L00_0025d808_mine(((L14MinePath *)l14_paths[mine_vars->path]),
                              path_target_position,
                              &mine_vars->segment,
                              &mine_vars->along,
                              1,
                              mine_path_step_rate_gp * frame_time * mine_vars->dir);
        FUN_L00_0025b8c0(
            &moby->pos.x, &mine_vars->vel[0], path_target_position[0], 0.02f, 0.2f, 0.0f);
        FUN_L00_0025b8c0(
            &moby->pos.y, &mine_vars->vel[1], path_target_position[1], 0.02f, 0.2f, 0.0f);
        FUN_L00_0025b8c0(
            &moby->pos.z, &mine_vars->vel[2], path_target_position[2], 0.02f, 0.2f, 0.0f);
        hit = find_moby_hit(moby, 0x230000, 0);
        fun_00213928(moby, hit, mine_vars->health, 0, &count, 0, 0, 4);
        if ((count >= 2 && hit != 0 && hit->moby->oclass != 0x283) ||
            hero.coll_hit_moby == (u8 *)moby) {
            clear_u64_value(&vel);
            allocate_voice_for_target_entry(0, 0, moby);
            if (hero.coll_hit_moby == (u8 *)moby)
                mine_explode(moby,
                             &vel,
                             &moby->pos,
                             3.0f,
                             1.0f,
                             10,
                             20,
                             60,
                             3.0f,
                             5.0f,
                             9.0f,
                             1.0f,
                             -1,
                             15.0f,
                             1,
                             1,
                             -1,
                             0);
            else
                mine_explode(moby,
                             &vel,
                             &moby->pos,
                             0.0f,
                             0.0f,
                             10,
                             20,
                             60,
                             3.0f,
                             5.0f,
                             9.0f,
                             1.0f,
                             -1,
                             15.0f,
                             1,
                             1,
                             -1,
                             0);
            mark_moby_for_removal(moby);
            return;
        }
        moby->rot.x = fast_add_rotations(moby->rot.x, mine_vars->spin[0]);
        moby->rot.y = fast_add_rotations(moby->rot.y, mine_vars->spin[1]);
        moby->rot.z = fast_add_rotations(moby->rot.z, mine_vars->spin[2]);
        mine_vars->phase = fast_add_rotations(mine_vars->phase, mine_glow_pulse_phase_step_gp);
        glow_pulse_sine = fast_sin(mine_vars->phase);
        glow_red_intensity = mine_ftoi(glow_pulse_sine * 48.0f) + 0xCF;
        glow_green_blue_intensity = mine_ftoi(glow_pulse_sine * 32.0f) + 0x30;
        moby->glow_color = glow_green_blue_intensity << 16 |
                           (0xFF000000 | glow_green_blue_intensity << 8) | glow_red_intensity;
        break;
    }
}

#include "qcopy.h"

/* Spawn a projectile moby at a, aimed along b, with two parameters. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002EE150), where it is exact; names translated to the US level program. */

extern char *create_moby(int);
extern char *create_moby(int) __asm__("FUN_0020c4f8");

char *spawn_flying_bolt(void  *spawn_position,
                        float *launch_vector,
                        int    collision_ignore_moby,
                        int    collision_mode) __asm__("FUN_L14_002eccf0");

char *spawn_flying_bolt(void  *spawn_position,
                        float *launch_vector,
                        int    collision_ignore_moby,

                        int collision_mode) {
    char *projectile_moby = create_moby(0x2A9);
    if (projectile_moby) {
        char *data;
        *(int *)(projectile_moby + 0x40) = 0;
        MOBY(projectile_moby)->update_dist = 0xFF;
        MOBY(projectile_moby)->draw_dist = 0x7F;
        projectile_moby[0x31] = 1;
        data = ((char *)MOBY(projectile_moby)->pvars);
        MOBY(projectile_moby)->rot.y =
            -fast_arc_tan(vector_length_xy(launch_vector), launch_vector[2]);
        MOBY(projectile_moby)->rot.z = fast_arc_tan(launch_vector[0], launch_vector[1]);
        qcopy(projectile_moby + 0x10, spawn_position);
        qcopy(data + 0x10, spawn_position);
        qcopy(data, launch_vector);
        blend_moby_animation(projectile_moby, 1, 0, scale_game_frames(5));
        MOBY(projectile_moby)->flags = 0x208;
        *(int *)(data + 0x20) = collision_ignore_moby;
        *(int *)(data + 0x24) = collision_mode;
        projectile_moby[0x23] = 0x10;
        update_entity(projectile_moby);
    }
    return projectile_moby;
}

/* Pvars of a flying bolt (same code as FUN_L07_0030b6a8, plus a sweep that skips class 0xFC mobys). */
typedef struct {
    Vec4  vel;    /* 0x00 */
    Vec4  origin; /* 0x10 */
    void *ignore; /* 0x20: collision owner to skip */
    s32   solid;  /* 0x24 */
} L14BoltVars;

/* Sweep request filled by FUN_L00_00259888 for the second collision test. */
typedef struct {
    u8 pad0[0x18];
    u8 flag18;
    u8 flag19;
    u8 pad1A[0x16];
} L14BoltSweep;

extern int   FUN_L00_00257b90(int, int);
extern void  FUN_L00_00259888(void *, struct Moby *, s32, void *, f32);
extern void  spawn_projectile_expiry_particle(void *, void *) __asm__("FUN_L07_0029a2a0");
extern char *FUN_L00_0026dd20(char *, char *, int c, int d, float f);

/* Result of the last collision line test. */
extern struct {
    u8           pad0[0x18];
    struct Moby *moby; /* 0x18 */
    u8           pad1C[4];
    Vec4         pos; /* 0x20 */
} D_L14_00174540;

extern char bolt_impact_reflection_normal[] __asm__("D_L14_00174580");

void update_flying_bolt(struct Moby *projectile_moby) __asm__("FUN_L14_002ece00");

void update_flying_bolt(struct Moby *projectile_moby) {
    L14BoltVars *bolt_vars = (L14BoltVars *)projectile_moby->pvars;
    Vec4         old;
    Vec4         target;
    L14BoltSweep sweep;
    Vec4         aim;
    Vec4         particle_launch_vector;
    Vec4         spark;
    void        *ignore;
    struct Moby *hit;
    int          pass;
    int          tries;
    int          i;
    int          j;

    qcopy(&old, &projectile_moby->pos);
    fast_vec_add(&projectile_moby->pos, &projectile_moby->pos, &bolt_vars->vel);
    qcopy(&target, &projectile_moby->pos);
    ignore = bolt_vars->ignore;
    qcopy(&aim, &bolt_vars->vel);
    aim.f[2] = 0.0f;
    set_vector_length_xy(&aim, &aim, 1.0f);
    tries = 0;
    aim.f[2] = 1.0f;
    FUN_L00_00259888(&sweep, projectile_moby, 0x10001, &aim, 1.0f);
    sweep.flag18 = 1;
    sweep.flag19 = 1;
    pass = bolt_vars->solid == 0;
    if (vector_distance_xy(&projectile_moby->pos, &bolt_vars->origin) > 30.0f) {
        if (projectile_moby->visible != 0) {
            for (i = 0; i < 3; i++) {
                spark.q = 0;
                spark.f[0] = random_float_between(-1.0f, 1.0f);
                spark.f[1] = random_float_between(-1.0f, 1.0f);
                spark.f[2] = random_float_between(-1.0f, 1.0f);
                particle_launch_vector.q = spark.q;
                fast_vec_normalize(&particle_launch_vector,
                                   &particle_launch_vector,
                                   fast_vec_length(&spark) * 0.1f);
                fast_vec_add(&particle_launch_vector, &spark, &particle_launch_vector);
                fast_vec_normalize(
                    &particle_launch_vector,
                    &particle_launch_vector,
                    random_float_between(frame_time + frame_time, frame_time * 4.0f));
                spawn_projectile_expiry_particle(&projectile_moby->pos, &particle_launch_vector);
            }
        }
        mark_moby_for_removal(projectile_moby);
        return;
    }
    while (tries < 3 && collision_line(&old, &target, pass, ignore, 0) != 0) {
        hit = D_L14_00174540.moby;
        tries++;
        if (hit != 0 && hit->oclass == 0xFC) {
            qcopy(&old, &D_L14_00174540.pos);
            ignore = hit;
            continue;
        }
        collision_line(&old, &target, pass, ignore, &sweep);
        qcopy(&projectile_moby->pos, &D_L14_00174540.pos);
        if (projectile_moby->visible != 0) {
            for (j = 0; j < 5; j++) {
                spark.q = 0;
                spark.f[0] = random_float_between(-1.0f, 1.0f);
                spark.f[1] = random_float_between(-1.0f, 1.0f);
                spark.f[2] = random_float_between(-1.0f, 1.0f);
                particle_launch_vector.q = spark.q;
                reflect_vector(&spark, &bolt_vars->vel, bolt_impact_reflection_normal);
                fast_vec_normalize(&particle_launch_vector,
                                   &particle_launch_vector,
                                   fast_vec_length(&spark) * 0.5f);
                fast_vec_add(&particle_launch_vector, &spark, &particle_launch_vector);
                fast_vec_normalize(&particle_launch_vector,
                                   &particle_launch_vector,
                                   random_float_between(frame_time * 3.0f, frame_time * 6.0f));
                FUN_L00_0026dd20((char *)&projectile_moby->pos,
                                 (char *)&particle_launch_vector,
                                 0x7F2F4F6F,
                                 FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(15)),
                                 30000.0f);
            }
        }
        allocate_voice_for_target_entry(0, 0, projectile_moby);
        mark_moby_for_removal(projectile_moby);
        return;
    }
    if (projectile_moby->pos.x < 2.0f || projectile_moby->pos.x > 1021.0f ||
        projectile_moby->pos.y < 2.0f || projectile_moby->pos.y > 1021.0f ||
        projectile_moby->pos.z < 2.0f || projectile_moby->pos.z > 1021.0f) {
        mark_moby_for_removal(projectile_moby);
    }
}

#include "qcopy.h"

/* Camera-intro controller: waits for the cutscene flag, then every few seconds moves the focus to a random
 * visible point of its path. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002EE6E0), where it is exact; names translated to the US level program. */

extern char  D_0013D24D[];
extern float D_L14_00161CC0[] MACRO_ADDR;
extern float D_L14_00161CD0[];
extern int   D_L14_0015F5C4;
extern short lightning_arc_interval_remaining_frames_gp __asm__("D_L14_00161C50")
    __attribute__((sda));
extern short lightning_arc_interval_frames_gp __asm__("D_L14_00161C54") __attribute__((sda));
extern short lightning_arc_initialized_gp __asm__("D_L14_00161C58") __attribute__((sda));
extern short lightning_displaced_endpoint_z_setting_gp __asm__("D_L14_00161C5C")
    __attribute__((sda));
extern short lightning_visibility_test_point_w_gp __asm__("D_L14_00161C60") __attribute__((sda));
extern short lightning_visibility_test_z_offset_gp __asm__("D_L14_00161C64") __attribute__((sda));
extern void  deactivate_recorded_moby(void) __asm__("FUN_L00_0028e990");
extern void  start_scene(int) __asm__("FUN_L00_00298840");
extern void  show_tracked_moby(void) __asm__("FUN_L01_002a2480");
extern void  update_branched_lightning_arc(char *) __asm__("FUN_L14_002ed530");
extern float D_L14_00161CD0_f __asm__("D_L14_00161CD0");

void update_path_lightning_controller(struct Moby *moby) __asm__("FUN_L14_002ed280");

void update_path_lightning_controller(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    int   visible_path_point_indices[5];
    float visibility_test_point[4];
    int   moby_state;
    if (MOBY(moby)->state == 0) {
        if (current_level_index == 0xE)
            hero.pack_disabled = 1;
        MOBY(moby)->state = 1;
        moby->update_dist = 0xFF;
        *(int *)&lightning_arc_initialized_gp = 0;
        *(int *)&lightning_arc_interval_remaining_frames_gp = 0;
    }
    moby_state = MOBY(moby)->state;
    if (moby_state == 1) {
        unsigned char *scene_start_guard_base;
        if (D_L14_0015F5C4 != 0)
            return;
        scene_start_guard_base = (unsigned char *)D_0013D24D + 0x13B;
        if (scene_start_guard_base[0x68] != 0)
            return;
        start_scene(0);
        scene_start_guard_base[0x68] = moby_state;
        MOBY(moby)->state = 2;
        deactivate_recorded_moby();
        return;
    }
    if (moby_state == 2) {
        if (D_L14_0015F5C4 == moby_state)
            return;
        MOBY(moby)->state = 3;
        show_tracked_moby();
        return;
    }
    if (fast_dec_timer(&lightning_arc_interval_remaining_frames_gp) == 0)
        return;
    if (*(int *)&lightning_arc_initialized_gp == 0) {
        int *path = ((int *)l14_paths[*(int *)moby_vars]);
        int  i;
        int  visible_point_count = 0;
        for (i = 0; i < path[0] && visible_point_count < 5; i++) {
            qcopy(visibility_test_point, (char *)path + i * 16 + 0x10);
            visibility_test_point[2] += *(float *)&lightning_visibility_test_z_offset_gp;
            visibility_test_point[3] = *(float *)&lightning_visibility_test_point_w_gp;
            if (fast_bsphere_check(visibility_test_point, 255.0f) > 0) {
                visible_path_point_indices[visible_point_count++] = i;
            }
        }
        if (visible_point_count > 0) {
            int k = random_integer_below(visible_point_count);
            qcopy(D_L14_00161CC0, (char *)path + visible_path_point_indices[k] * 16 + 0x10);
            lightning_path_endpoint_w = 1.0f;
            qcopy(D_L14_00161CD0, D_L14_00161CC0);
            lightning_displaced_endpoint_z = *(float *)&lightning_displaced_endpoint_z_setting_gp;
            D_L14_00161CD0_f += FUN_L00_00257c48(0.0f, 20.0f);
            lightning_displaced_endpoint_y += FUN_L00_00257c48(0.0f, 20.0f);
            qcopy(&moby->pos, D_L14_00161CC0);
        }
    }
    update_branched_lightning_arc(moby);
    if (lightning_arc_remaining_frames == 0) {
        *(int *)&lightning_arc_interval_remaining_frames_gp =
            scale_game_frames(*(int *)&lightning_arc_interval_frames_gp);
        *(int *)&lightning_arc_initialized_gp = 0;
    }
}

/* Arc state shared by FUN_L14_002ed530 and its draw callback FUN_L14_002edc18. */
extern f32 lightning_arc_alpha_pulse_angle_step_gp __asm__("D_L14_00161C68") __attribute__((sda));
extern f32 lightning_arc_alpha_pulse_sweep_gp __asm__("D_L14_00161C6C") __attribute__((sda));
extern s32 D_L14_00161C70_gp __asm__("D_L14_00161C70") __attribute__((sda));
extern f32 lightning_main_arc_point_spacing_gp __asm__("D_L14_00161C74") __attribute__((sda));

#include "rnc/overlay/l14/mobys/jet_data/update_branched_lightning_arc.h"

extern f32 lightning_main_arc_z_displacement_limit_gp __asm__("D_L14_00161C94")
    __attribute__((sda));
extern s32   lightning_arc_lifetime_range_start_gp __asm__("D_L14_00161C98") __attribute__((sda));
extern s32   lightning_arc_lifetime_range_end_gp __asm__("D_L14_00161C9C") __attribute__((sda));
extern u8    lightning_arc_alpha_fade __asm__("D_L14_00161CE0");
extern char *lightning_arc_owner_moby __asm__("D_L14_00161CE8");
extern s32   D_L14_00161CF8_macro[] __asm__("D_L14_00161CF8") MACRO_ADDR;
f32          lightning_main_arc_points[40][4] __asm__("D_L14_001DFD80") = {0};

extern s16 D_L14_001E0000[40];
f32        D_L14_001E0050_d530[3][16][4] __asm__("D_L14_001E0050") = {0};

u16 D_L14_001E0350_d530[3][16] __asm__("D_L14_001E0350") = {0};

extern int  arc_ftoi(float) __asm__("FUN_001fa6d0");
extern void allocate_voice_for_group_entry(int, int, void *) __asm__("FUN_0022dba0");
void        spawn_lightning_arc_particle_pairs(float *particle_spawn_position,
                                               int    spawn_pair_count) __asm__("FUN_L14_002ee6b0");

void         spawn_lightning_arc_particle_pairs(float *pos, int n) __asm__("FUN_L14_002ee6b0");
void         draw_branched_lightning_arc() __asm__("FUN_L14_002edc18");
extern float D_L14_00161CD0_macro[] __asm__("D_L14_00161CD0") MACRO_ADDR;

extern float lightning_main_arc_penultimate_point[] __asm__("D_L14_001DFFE0");

/* Lightning arc between the two camera points: builds the bolt and its three branches once, then ages and fades them. */
void update_branched_lightning_arc(char *moby) {
    Vec4   delta;
    float  arc_endpoint_distance;
    float  path_fraction;
    float  sign;
    float  branch_segment_angle;
    float  r;
    int    i;
    int    j;
    int    branch_age_index;
    float *main_arc_point;
    s32   *starts;
    u16   *main_arc_age;

    if (*(int *)&lightning_arc_initialized_gp == 0) {
        *(int *)&lightning_arc_initialized_gp = 1;
        lightning_arc_sound_pending = 1;
        lightning_arc_owner_moby = moby;
        i = 0;
        starts = D_L14_00161CF8_macro;
        *(int *)&lightning_arc_alpha_pulse_sweep_gp = 0;
        fast_vec_sub(&delta, D_L14_00161CD0_macro, D_L14_00161CC0);
        arc_endpoint_distance = fast_vec_length(&delta);
        lightning_main_arc_point_spacing_gp = arc_endpoint_distance / convert_integer_to_float(40);
        lightning_arc_alpha_fade = 0;
        for (; i < 40; i++) {
            main_arc_age = &D_L14_001E0000[i];
            main_arc_point = lightning_main_arc_points[i];
            main_arc_point[1] = main_arc_point[2] = 0.0f;
            main_arc_point[3] = 1.0f;
            main_arc_point[0] = lightning_main_arc_point_spacing_gp * (float)i;
            if (i < 39) {
                main_arc_point[2] =
                    main_arc_point[-2] + FUN_L00_00257c48(lightning_main_arc_z_step_range_start_gp,
                                                          lightning_main_arc_z_step_range_end_gp);
                if (lightning_main_arc_z_displacement_limit_gp < (*&main_arc_point)[2]) {
                    main_arc_point[2] = lightning_main_arc_z_displacement_limit_gp;
                } else if (main_arc_point[2] < -lightning_main_arc_z_displacement_limit_gp) {
                    main_arc_point[2] = -lightning_main_arc_z_displacement_limit_gp;
                }
            }
            path_fraction = convert_integer_to_float(i) / convert_integer_to_float(40);
            *main_arc_age =
                arc_ftoi((float)-scale_game_frames(D_L14_00161C70_gp) * path_fraction + 0.0f);
        }
        lerp_vector(lightning_main_arc_penultimate_point,
                    lightning_main_arc_penultimate_point - 4,
                    lightning_main_arc_penultimate_point + 4,
                    0.5f);
        sign = -1.0f;
        lightning_arc_remaining_frames = lightning_arc_initial_lifetime_frames =
            scale_game_frames(random_int_between(lightning_arc_lifetime_range_start_gp,
                                                 lightning_arc_lifetime_range_end_gp));
        lightning_arc_inverse_fade_duration =
            1.0f / convert_integer_to_float(lightning_arc_remaining_frames -
                                            scale_game_frames(D_L14_00161C70_gp));
        if (random_integer_below(2) != 0) {
            sign = 1.0f;
        }
        for (j = 0; j < 3; j++) {
            int k = random_int_between(5, 22);
            int branch_point_index;
            float(*row)[4] = D_L14_001E0050_d530[j];
            u16 *sq = D_L14_001E0350_d530[j];
            starts[j] = k;
            sq[0] = D_L14_001E0000[k];
            row[0][0] = 0.0f;
            row[0][2] = 0.0f;
            r = random_float_between(1.0f, 10.0f);
            branch_segment_angle = random_float_between(0.5235988f, 1.3962634f) * sign;
            row[0][0] = r * fast_cos(branch_segment_angle);
            row[0][2] = r * fast_sin(branch_segment_angle);
            for (branch_point_index = 1; branch_point_index < 16; branch_point_index++) {
                float angle_step;
                float na;
                r = random_float_between(1.0f, 10.0f);
                angle_step = FUN_L00_00257c48(0.0f, 40.0f) * 0.017453292f;
                na = fast_add_rotations(branch_segment_angle, angle_step);
                if (absolute_float(na) > 1.3962634f || (branch_segment_angle > 0.0f && na < 0.0f) ||
                    (branch_segment_angle < 0.0f && na > 0.0f)) {
                    branch_segment_angle = fast_add_rotations(branch_segment_angle, -angle_step);
                } else {
                    branch_segment_angle = na;
                }
                D_L14_001E0050_d530[j][branch_point_index][0] = r * fast_cos(branch_segment_angle);
                D_L14_001E0050_d530[j][branch_point_index][2] = r * fast_sin(branch_segment_angle);
                D_L14_001E0350_d530[j][branch_point_index] =
                    D_L14_001E0000[starts[j] + branch_point_index];
            }
            sign = -sign;
        }
    }
    if (moby == lightning_arc_owner_moby) {
        fast_dec_timer(&lightning_arc_remaining_frames);
        for (i = 0; i < 40; i++) {
            s16 v = ++D_L14_001E0000[i];
            if (scale_game_frames(D_L14_00161C70_gp) < v) {
                D_L14_001E0000[i] = scale_game_frames(D_L14_00161C70_gp);
            }
            if (i != 0) {
                if (i >= 35) {
                    float  k5 = 5.0f;
                    float *pp = lightning_main_arc_points[i];
                    pp[2] = pp[2] + (0.0f - pp[2]) * (convert_integer_to_float(i - 35) / k5);
                }
            }
        }
        scale_game_frames(D_L14_00161C70_gp);
        for (j = 0; j < 3; j++) {
            for (branch_age_index = 0; branch_age_index < 16; branch_age_index++) {
                s16 v = ++D_L14_001E0350_d530[j][branch_age_index];
                if (scale_game_frames(D_L14_00161C70_gp) < v) {
                    D_L14_001E0350_d530[j][branch_age_index] = scale_game_frames(D_L14_00161C70_gp);
                }
            }
        }
        if (lightning_arc_remaining_frames <
            lightning_arc_initial_lifetime_frames - scale_game_frames(D_L14_00161C70_gp)) {
            if (scale_game_frames(60) < lightning_arc_initial_lifetime_frames) {
                if (lightning_arc_alpha_pulse_sweep_gp < 6.2831855f) {
                    lightning_arc_alpha_pulse_offset =
                        arc_ftoi(fast_sin(lightning_arc_alpha_pulse_phase) * 128.0f);
                    lightning_arc_alpha_pulse_phase = fast_add_rotations(
                        lightning_arc_alpha_pulse_phase, lightning_arc_alpha_pulse_angle_step_gp);
                    lightning_arc_alpha_pulse_sweep_gp +=
                        lightning_arc_alpha_pulse_angle_step_gp * frame_scale;
                } else {
                    lightning_arc_alpha_pulse_offset = 0;
                }
            } else {
                lightning_arc_alpha_pulse_offset = 0;
            }
            lightning_arc_alpha_fade =
                (u8)lightning_arc_alpha_pulse_offset +
                arc_ftoi(convert_integer_to_float(lightning_arc_remaining_frames) *
                             lightning_arc_inverse_fade_duration * -255.0f +
                         255.0f);
            if (lightning_arc_sound_pending != 0) {
                allocate_voice_for_group_entry(0, 0, moby);
                lightning_arc_sound_pending = 0;
            }
            if (lightning_arc_alpha_fade < 50) {
                spawn_lightning_arc_particle_pairs(D_L14_00161CC0, 3);
            }
        } else {
            lightning_arc_alpha_fade = 0;
        }
        add_draw_callback(draw_branched_lightning_arc, moby);
    }
}

#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002EF078), where it is exact; names translated to the US level program. */

typedef int u128_2EF078 __attribute__((mode(TI)));

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) V_2EF078;

typedef struct {
    V_2EF078 v[4];
    int      col[4];
    float    uv[4][2];
    long     r70;
    long     tex;
    long     r80;
    long     r88;
} Q_2EF078;

extern V_2EF078 D_L14_001DFD80[];
extern V_2EF078 D_L14_001E0050[];
extern float    D_L14_00161CD0_c[] __asm__("D_L14_00161CD0") MACRO_ADDR;
extern int      D_L14_00161C70_gp __asm__("D_L14_00161C70") __attribute__((sda));

#include "rnc/overlay/l14/mobys/jet_data/draw_branched_lightning_arc.h"

extern float         D_L14_00161C88 __attribute__((sda));
extern int           D_L14_00161CF8_macro[] __asm__("D_L14_00161CF8") MACRO_ADDR;
extern s16           D_L14_001E0000[];
extern short         D_L14_001E0350[];
extern unsigned char D_L14_00161CE0[] MACRO_ADDR;
extern int           func_001FA898_r(float) __asm__("FUN_001fa6d0");

void draw_branched_lightning_arc(void) {
    Q_2EF078  layer_quads[2];
    V_2EF078  arc_to_world[4];
    V_2EF078  sz[2];
    V_2EF078  world_arc_points[2];
    V_2EF078  quad_to_world[4];
    V_2EF078  branch_position_scratch[3];
    int       outer_vertex_alpha;
    int       i;
    int       j;
    V_2EF078 *branch_offset_cursor;
    short    *branch_age_cursor;
    int       outer_vertex_color;
    V_2EF078 *nd;

    layer_quads[0].tex = get_effect_texture(0x2D);
    layer_quads[0].r88 = 0x8000000048L;
    layer_quads[0].r80 = 0xFF9000000260L;
    layer_quads[0].r70 = 0;
    outer_vertex_alpha =
        func_001FA898_r(convert_integer_to_float(D_L14_001E0000[0]) /
                        convert_integer_to_float(scale_game_frames(D_L14_00161C70_gp)) * 255.0f) -
        D_L14_00161CE0[0];
    if (outer_vertex_alpha < 0)
        outer_vertex_alpha = 0;
    if (outer_vertex_alpha > 0xFF)
        outer_vertex_alpha = 0xFF;
    outer_vertex_color = lightning_arc_outer_base_color_gp | (outer_vertex_alpha << 24);
    layer_quads[0].col[1] = outer_vertex_color;
    layer_quads[0].col[0] = outer_vertex_color;
    layer_quads[0].uv[0][0] = 0.0f;
    layer_quads[0].uv[0][1] = 0.0f;
    layer_quads[0].uv[1][0] = 0.0f;
    layer_quads[0].uv[1][1] = 1.0f;
    layer_quads[0].uv[2][0] = 1.0f;
    layer_quads[0].uv[2][1] = 0.0f;
    layer_quads[0].uv[3][0] = 1.0f;
    layer_quads[0].uv[3][1] = 1.0f;
    fast_mem_copy(&layer_quads[1], &layer_quads[0], 0x90);
    layer_quads[1].tex = get_effect_texture(0x2C);
    layer_quads[1].col[0] = layer_quads[1].col[1] =
        lightning_arc_inner_base_color_gp | ((outer_vertex_alpha >> 1) << 24);
    fast_vec_sub(&arc_to_world[0], D_L14_00161CC0, D_L14_00161CD0_c);
    fast_vec_normalize(&arc_to_world[0], &arc_to_world[0], 1.0f);
    arc_to_world[0].w = 0.0f;
    arc_to_world[2].x = 1.0f;
    arc_to_world[2].y = 0.0f;
    arc_to_world[2].z = 0.0f;
    arc_to_world[2].w = 0.0f;
    fast_vec_cross(&arc_to_world[1], &arc_to_world[0], &arc_to_world[2]);
    fast_vec_normalize(&arc_to_world[1], &arc_to_world[1], 1.0f);
    arc_to_world[1].w = 0.0f;
    fast_vec_cross(&arc_to_world[2], &arc_to_world[1], &arc_to_world[0]);
    arc_to_world[2].w = 0.0f;
    qcopy(&arc_to_world[3], D_L14_00161CD0_c);
    *(u128_2EF078 *)&sz[0] = 0;
    sz[0].z = lightning_arc_outer_half_width_start_gp;
    sz[0].w = 1.0f;
    *(u128_2EF078 *)&sz[1] = 0;
    sz[1].z = lightning_arc_outer_half_width_start_gp * D_L14_00161C88;
    sz[1].w = 1.0f;
    qcopy(&layer_quads[0].v[0], &sz[0]);
    fast_vec_scale(&layer_quads[0].v[1], &layer_quads[0].v[0], -1.0f);
    qcopy_nc(&layer_quads[1].v[0], &sz[1]);
    fast_vec_scale(&layer_quads[1].v[1], &layer_quads[1].v[0], -1.0f);
    nd = &D_L14_001DFD80[1];
    transform_vector(&world_arc_points[0], nd, &arc_to_world[0]);
    transform_vector(&world_arc_points[1], nd + 1, &arc_to_world[0]);
    qcopy(&quad_to_world[0], &arc_to_world[0]);
    fast_vec_sub(&quad_to_world[1], D_L14_001674C0, &world_arc_points[0]);
    fast_vec_normalize(&quad_to_world[1], &quad_to_world[1], 1.0f);
    fast_vec_cross(&quad_to_world[2], &quad_to_world[1], &quad_to_world[0]);
    qcopy(&quad_to_world[3], &world_arc_points[0]);
    transform_vector(&layer_quads[0].v[0], &layer_quads[0].v[0], &quad_to_world[0]);
    transform_vector(&layer_quads[0].v[1], &layer_quads[0].v[1], &quad_to_world[0]);
    transform_vector(&layer_quads[1].v[0], &layer_quads[1].v[0], &quad_to_world[0]);
    transform_vector(&layer_quads[1].v[1], &layer_quads[1].v[1], &quad_to_world[0]);
    for (i = 1; i < 0x28; i++) {
        int    main_arc_end_vertex_alpha;
        float *arc_endpoint = D_L14_00161CC0;
        qcopy(&layer_quads[0].v[2], &sz[0]);
        fast_vec_scale(&layer_quads[0].v[3], &layer_quads[0].v[2], -1.0f);
        qcopy(&layer_quads[1].v[2], &sz[1]);
        fast_vec_scale(&layer_quads[1].v[3], &layer_quads[1].v[2], -1.0f);
        if (i == 0x27) {
            qcopy(&world_arc_points[0], arc_endpoint);
        } else {
            transform_vector(&world_arc_points[0], &D_L14_001DFD80[i], &arc_to_world[0]);
        }
        qcopy(&quad_to_world[0], &arc_to_world[0]);
        fast_vec_sub(&quad_to_world[1], D_L14_001674C0, &world_arc_points[0]);
        fast_vec_normalize(&quad_to_world[1], &quad_to_world[1], 1.0f);
        fast_vec_cross(&quad_to_world[2], &quad_to_world[1], &quad_to_world[0]);
        qcopy(&quad_to_world[3], &world_arc_points[0]);
        transform_vector(&layer_quads[0].v[2], &layer_quads[0].v[2], &quad_to_world[0]);
        transform_vector(&layer_quads[0].v[3], &layer_quads[0].v[3], &quad_to_world[0]);
        transform_vector(&layer_quads[1].v[2], &layer_quads[1].v[2], &quad_to_world[0]);
        transform_vector(&layer_quads[1].v[3], &layer_quads[1].v[3], &quad_to_world[0]);
        if (i == 0x27) {
            main_arc_end_vertex_alpha = 0;
        } else {
            main_arc_end_vertex_alpha =
                func_001FA898_r(convert_integer_to_float(D_L14_001E0000[i]) /
                                convert_integer_to_float(scale_game_frames(D_L14_00161C70_gp)) *
                                255.0f) -
                D_L14_00161CE0[0];
            if (main_arc_end_vertex_alpha < 0)
                main_arc_end_vertex_alpha = 0;
            if (main_arc_end_vertex_alpha > 0xFF)
                main_arc_end_vertex_alpha = 0xFF;
        }
        layer_quads[0].col[2] = layer_quads[0].col[3] =
            lightning_arc_outer_base_color_gp | (main_arc_end_vertex_alpha << 24);
        layer_quads[1].col[2] = layer_quads[1].col[3] =
            lightning_arc_inner_base_color_gp | ((main_arc_end_vertex_alpha >> 1) << 24);
        draw_geometry_quad(&layer_quads[0], 0, 0);
        draw_geometry_quad(&layer_quads[1], 0, 0);
        layer_quads[0].col[0] = layer_quads[0].col[1] = layer_quads[0].col[2];
        layer_quads[1].col[0] = layer_quads[1].col[1] = layer_quads[1].col[2];
        qcopy(&layer_quads[0].v[0], &layer_quads[0].v[2]);
        qcopy(&layer_quads[0].v[1], &layer_quads[0].v[3]);
        qcopy(&layer_quads[1].v[0], &layer_quads[1].v[2]);
        qcopy(&layer_quads[1].v[1], &layer_quads[1].v[3]);
        sz[0].z =
            lightning_arc_outer_half_width_start_gp +
            (lightning_arc_outer_half_width_end_gp - lightning_arc_outer_half_width_start_gp) *
                cubic_hermite_scalar(-1.0f,
                                     0.0f,
                                     1.0f,
                                     0.0f,
                                     convert_integer_to_float(i) / convert_integer_to_float(0x28));
        sz[1].z = sz[0].z * D_L14_00161C88;
    }
    qcopy(&quad_to_world[0], &arc_to_world[0]);
    qcopy(&quad_to_world[1], &arc_to_world[1]);
    qcopy(&quad_to_world[2], &arc_to_world[2]);
    for (j = 0; j < 3; j++) {
        float     branch_age_fraction;
        short    *as = D_L14_001E0350;
        V_2EF078 *os = D_L14_001E0050;
        V_2EF078 *branch_offsets;

        branch_age_fraction = convert_integer_to_float(as[j * 16]) /
                              convert_integer_to_float(scale_game_frames(D_L14_00161C70_gp));
        outer_vertex_alpha = func_001FA898_r(branch_age_fraction * 255.0f) - D_L14_00161CE0[0];
        if (outer_vertex_alpha < 0)
            outer_vertex_alpha = 0;
        if (outer_vertex_alpha > 0xFF)
            outer_vertex_alpha = 0xFF;
        outer_vertex_color = lightning_arc_outer_base_color_gp | (outer_vertex_alpha << 24);
        layer_quads[0].col[1] = outer_vertex_color;
        layer_quads[0].col[0] = outer_vertex_color;
        layer_quads[1].col[0] = layer_quads[1].col[1] =
            lightning_arc_inner_base_color_gp | ((outer_vertex_alpha >> 1) << 24);
        sz[0].z =
            (lightning_arc_outer_half_width_start_gp +
             (lightning_arc_outer_half_width_end_gp - lightning_arc_outer_half_width_start_gp) *
                 cubic_hermite_scalar(-1.0f,
                                      0.0f,
                                      1.0f,
                                      0.0f,
                                      convert_integer_to_float(D_L14_00161CF8_macro[j]) /
                                          convert_integer_to_float(0x28))) *
            0.75f;
        sz[1].z = sz[0].z * 4.0f;
        qcopy(&layer_quads[0].v[0], &sz[0]);
        fast_vec_scale(&layer_quads[0].v[1], &layer_quads[0].v[0], -1.0f);
        qcopy(&layer_quads[1].v[0], &sz[1]);
        fast_vec_scale(&layer_quads[1].v[1], &layer_quads[1].v[0], -1.0f);
        qcopy(&branch_position_scratch[0], &D_L14_001DFD80[D_L14_00161CF8_macro[j]]);
        transform_vector(
            &branch_position_scratch[1], &branch_position_scratch[0], &arc_to_world[0]);
        branch_offsets = &os[j * 16];
        branch_position_scratch[0].x += branch_offsets->x;
        branch_position_scratch[0].z += branch_offsets->z;
        transform_vector(
            &branch_position_scratch[2], &branch_position_scratch[0], &arc_to_world[0]);
        qcopy(&quad_to_world[3], &branch_position_scratch[1]);
        transform_vector(&layer_quads[0].v[0], &layer_quads[0].v[0], &quad_to_world[0]);
        transform_vector(&layer_quads[0].v[1], &layer_quads[0].v[1], &quad_to_world[0]);
        transform_vector(&layer_quads[1].v[0], &layer_quads[1].v[0], &quad_to_world[0]);
        transform_vector(&layer_quads[1].v[1], &layer_quads[1].v[1], &quad_to_world[0]);
        branch_offset_cursor = branch_offsets + 1;
        branch_age_cursor = &as[j * 16 + 1];
        for (i = 1; i < 16; i++) {
            qcopy(&layer_quads[0].v[2], &sz[0]);
            fast_vec_scale(&layer_quads[0].v[3], &layer_quads[0].v[2], -1.0f);
            qcopy(&layer_quads[1].v[2], &sz[1]);
            fast_vec_scale(&layer_quads[1].v[3], &layer_quads[1].v[2], -1.0f);
            qcopy_nc(&branch_position_scratch[1], &branch_position_scratch[2]);
            branch_position_scratch[0].x += branch_offset_cursor->x;
            branch_position_scratch[0].z += branch_offset_cursor->z;
            transform_vector(
                &branch_position_scratch[2], &branch_position_scratch[0], &arc_to_world[0]);
            qcopy(&quad_to_world[3], &branch_position_scratch[1]);
            transform_vector(&layer_quads[0].v[2], &layer_quads[0].v[2], &quad_to_world[0]);
            transform_vector(&layer_quads[0].v[3], &layer_quads[0].v[3], &quad_to_world[0]);
            transform_vector(&layer_quads[1].v[2], &layer_quads[1].v[2], &quad_to_world[0]);
            transform_vector(&layer_quads[1].v[3], &layer_quads[1].v[3], &quad_to_world[0]);
            branch_age_fraction = convert_integer_to_float(*branch_age_cursor) /
                                  convert_integer_to_float(scale_game_frames(D_L14_00161C70_gp));
            outer_vertex_alpha = func_001FA898_r(branch_age_fraction * 255.0f) - D_L14_00161CE0[0];
            if (outer_vertex_alpha < 0)
                outer_vertex_alpha = 0;
            if (outer_vertex_alpha > 0xFF)
                outer_vertex_alpha = 0xFF;
            outer_vertex_color = lightning_arc_outer_base_color_gp | (outer_vertex_alpha << 24);
            layer_quads[0].col[3] = outer_vertex_color;
            layer_quads[0].col[2] = outer_vertex_color;
            layer_quads[1].col[2] = layer_quads[1].col[3] =
                lightning_arc_inner_base_color_gp | ((outer_vertex_alpha >> 1) << 24);
            draw_geometry_quad(&layer_quads[0], 0, 0);
            draw_geometry_quad(&layer_quads[1], 0, 0);
            layer_quads[0].col[0] = layer_quads[0].col[1] = layer_quads[0].col[2];
            layer_quads[1].col[0] = layer_quads[1].col[1] = layer_quads[1].col[2];
            qcopy(&layer_quads[0].v[0], &layer_quads[0].v[2]);
            qcopy(&layer_quads[0].v[1], &layer_quads[0].v[3]);
            qcopy(&layer_quads[1].v[0], &layer_quads[1].v[2]);
            qcopy(&layer_quads[1].v[1], &layer_quads[1].v[3]);
            sz[0].z *= 0.95f;
            sz[1].z = sz[0].z * 4.0f;
            branch_age_cursor++;
            branch_offset_cursor++;
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002EFB10), where it is exact; names translated to the US level program. */

extern unsigned char *FUN_L00_00272060(
    float *pos, int a, float f0, float f1, int col, int mode, int b, float *vel, float f2);

extern short lightning_arc_particle_speed_range_start_gp __asm__("D_L14_00161CA0")
    __attribute__((sda));
extern short lightning_arc_particle_speed_range_end_gp __asm__("D_L14_00161CA4")
    __attribute__((sda));
extern short D_L14_00161CA8 __attribute__((sda));
extern short lightning_arc_particle_scale_range_start_gp __asm__("D_L14_00161CAC")
    __attribute__((sda));
extern short lightning_arc_particle_scale_range_end_gp __asm__("D_L14_00161CB0")
    __attribute__((sda));

void spawn_lightning_arc_particle_pairs(float *particle_spawn_position, int spawn_pair_count) {
    float          initial_velocity[4];
    float          saved_position[4];
    float          particle_scale, velocity_horizontal_angle, velocity_vertical_angle;
    int            random_sign, r0, i, id;
    unsigned char *particle;
    if (spawn_pair_count > 0) {
        i = spawn_pair_count;
        do {
            particle_scale =
                random_float_between(*(float *)&lightning_arc_particle_scale_range_start_gp,
                                     *(float *)&lightning_arc_particle_scale_range_end_gp);
            random_sign = random_integer_below(2);
            if (random_sign == 0)
                random_sign--;
            velocity_horizontal_angle = random_angle_radians();
            velocity_vertical_angle = random_float_between(0.34906584f, 1.5358897f);
            build_spherical_offset(
                initial_velocity,
                random_float_between(*(float *)&lightning_arc_particle_speed_range_start_gp,
                                     *(float *)&lightning_arc_particle_speed_range_end_gp),
                velocity_horizontal_angle,
                velocity_vertical_angle);
            qcopy(saved_position, particle_spawn_position);
            particle_spawn_position[0] += FUN_L00_00257c48(0.0f, 0.15f);
            particle_spawn_position[1] += FUN_L00_00257c48(0.0f, 0.15f);
            particle_spawn_position[2] += FUN_L00_00257c48(0.0f, 0.15f);
            id = scale_game_frames(0x3C);
            particle = FUN_L00_00272060(particle_spawn_position,
                                        id,
                                        particle_scale * 0.1f,
                                        particle_scale,
                                        0x7F7F2020,
                                        0,
                                        random_sign,
                                        initial_velocity,
                                        *(float *)&D_L14_00161CA8);
            if (particle != 0) {
                particle[9] = func_001FA898_r(4.0f) + 0x70;
            }
            random_sign = -random_sign;
            particle = FUN_L00_00272060(particle_spawn_position,
                                        scale_game_frames(0x3C),
                                        particle_scale * 0.07f,
                                        particle_scale * 0.7f,
                                        0x7F7F7F7F,
                                        1,
                                        random_sign,
                                        initial_velocity,
                                        *(float *)&D_L14_00161CA8);
            if (particle != 0) {
                particle[9] = func_001FA898_r(4.0f) + 0x70;
            }
        } while (--i != 0);
    }
}

/* level save state: per-moby collected flags at 0x454 */
struct L14VendorLevel {
    u8 pad0[0x454];
    u8 collected[1];
};

extern struct L14VendorLevel D_L14_001BB930_v __asm__("D_L14_001BB930");
extern void                  vendor_volume(s32 voice, s32 volume) __asm__("FUN_L01_002a1968");
extern int                   vendor_ftoi(float) __asm__("FUN_001fa6d0");
extern int                   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern float vendor_spring(float *p, float t, float *v, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");
void initialize_elevator_path_state(struct Moby *moby) __asm__("FUN_L14_002eef60");

void initialize_elevator_path_state(struct Moby *m) __asm__("FUN_L14_002eef60");
int  advance_elevator_along_path(struct Moby *elevator_moby) __asm__("FUN_L14_002ef150");

int  advance_elevator_along_path(struct Moby *m) __asm__("FUN_L14_002ef150");
void settle_elevator_at_path_endpoint(struct Moby *elevator_moby) __asm__("FUN_L14_002ef3e0");

void settle_elevator_at_path_endpoint(struct Moby *m) __asm__("FUN_L14_002ef3e0");
void rebuild_elevator_world_path(u8 *moby) __asm__("FUN_L14_002ef630");

void update_elevator_idle_glow(struct Moby *moby) __asm__("FUN_L14_002ef6f0");

/* pvars of the vendor platform (shared with FUN_L14_002eef60/ef150/ef3e0/ef630) */
typedef struct {
    u8  pad0[0x60];
    u8  spring[0x40]; /* 0x60 */
    s32 path;         /* 0xA0 */
    s32 segment;      /* 0xA4 */
    f32 along;        /* 0xA8 */
    f32 dir;          /* 0xAC */
    s32 travelled;    /* 0xB0 */
    f32 lengthB4;     /* 0xB4 */
    s32 path2;        /* 0xB8 */
    f32 lengthBC;     /* 0xBC */
    f32 vel[4];       /* 0xC0 */
    f32 rot[4];       /* 0xD0 */
    u8  padE0[0x30];
    s32 item;         /* 0x110 */
    s32 target;       /* 0x114 */
    s32 clip_a;       /* 0x118 */
    s32 clip_b;       /* 0x11C */
    s32 clip_c;       /* 0x120 */
    s16 bought;       /* 0x124 */
    s16 level;        /* 0x126 */
    f32 level_vel;    /* 0x128 */
    s32 motor;        /* 0x12C */
    s32 motor_volume; /* 0x130 */
    s32 hum_volume;   /* 0x134 */
    s32 i138;         /* 0x138 */
} L14VendorVars;

/* Vendor platform update: rides its two rails between the trigger volumes, with hum and motor sounds. */
void elevator_update(struct Moby *m) __asm__("FUN_L14_002ee8d8");

void elevator_update(struct Moby *elevator_moby) {
    enum {
        ELEVATOR_STATE_INITIALIZE = 0,
        ELEVATOR_STATE_WAIT_FOR_RIDER = 1,
        ELEVATOR_STATE_FOLLOW_PATH = 2,
        ELEVATOR_STATE_SETTLE_AT_ENDPOINT = 3,
        ELEVATOR_STATE_WAIT_FOR_RIDER_EXIT = 4
    };
    L14VendorVars *elevator_vars = (L14VendorVars *)elevator_moby->pvars;
    Vec4           old;
    Vec4           rot;
    Vec4           delta;
    float          level;

    if (elevator_moby->state != 0 && elevator_moby->command == 2 && elevator_vars->dir < 0.0f) {
        int *path = (int *)l14_paths[elevator_vars->path2];
        elevator_vars->dir = -elevator_vars->dir;
        elevator_vars->segment = *path - 2;
        elevator_vars->along = *(float *)((char *)path + (elevator_vars->segment << 4) + 0x1C);
        elevator_moby->command = 1;
    }
    qcopy(&old, &elevator_moby->pos);
    qcopy(&rot, &elevator_moby->rot);
    switch (elevator_moby->state) {
    case ELEVATOR_STATE_INITIALIZE: {
        u16 id;
        elevator_moby->command = 0;
        elevator_moby->state = 1;
        initialize_elevator_path_state(elevator_moby);
        qcopy(&elevator_moby->pos, ((char *)l14_paths[elevator_vars->path]) + 0x10);
        qcopy(&elevator_moby->rot, elevator_vars->rot);
        id = elevator_moby->save_id;
        if (D_L14_001BB930_v.collected[(short)id] == 0 &&
            (((D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 0x1F)) ^ 1) & 1) &&
            visited52[elevator_vars->item + current_level_index * 16] == 0xFF)
            elevator_vars->bought = 1;
        else
            elevator_vars->bought = 0;
        elevator_moby->update_dist = 0xFF;
        elevator_moby->glow_color = 0x80303030;
        elevator_vars->motor = -1;
        elevator_vars->motor_volume = -1;
        elevator_vars->hum_volume = -1;
        elevator_vars->i138 = 0;
        break;
    }
    case ELEVATOR_STATE_WAIT_FOR_RIDER:
        update_elevator_idle_glow(elevator_moby);
        if (elevator_vars->clip_a >= 0 &&
            is_point_inside_clip_volume(&hero.motion.pos, elevator_vars->clip_a) != 0) {
            elevator_moby->command = 0;
            elevator_vars->dir = -1.0f;
            elevator_vars->segment = 0;
            elevator_vars->along = 0;
        }
        if (elevator_vars->clip_b >= 0 &&
            is_point_inside_clip_volume(&hero.motion.pos, elevator_vars->clip_b) != 0) {
            char **e;
            elevator_moby->command = 0;
            e = ((char **)&l14_paths[elevator_vars->path]);
            elevator_vars->segment = **(int **)e - 2;
            elevator_vars->along = *(float *)(*e + (elevator_vars->segment << 4) + 0x1C);
            elevator_vars->dir = 1.0f;
        }
        if (elevator_vars->clip_c >= 0 &&
            is_point_inside_clip_volume(&hero.motion.pos, elevator_vars->clip_c) != 0) {
            char **e;
            elevator_moby->command = 1;
            e = ((char **)&l14_paths[elevator_vars->path2]);
            elevator_vars->segment = **(int **)e - 2;
            elevator_vars->along = *(float *)(*e + (elevator_vars->segment << 4) + 0x1C);
            elevator_vars->dir = 1.0f;
        }
        if (hero.ground_moby == elevator_moby && hero.frames_off_ground.s == 0 &&
            vector_distance_xy(&hero.motion.pos, &elevator_moby->pos) < 1.0f) {
            int voice = allocate_voice_for_target_entry(1, 0, elevator_moby);
            if (voice >= 0) {
                if (elevator_vars->hum_volume < 0)
                    elevator_vars->hum_volume = *(int *)(((char *)&voice_pool) + 0xF0);
                vendor_volume(voice, elevator_vars->hum_volume);
            }
            if (elevator_vars->dir == -1.0f) {
                if (elevator_vars->bought != 0)
                    elevator_moby->command = 2;
                else
                    elevator_moby->command = 0;
            }
            elevator_moby->state = 2;
            elevator_vars->travelled = 0;
            elevator_vars->dir = -elevator_vars->dir;
        }
        break;
    case ELEVATOR_STATE_FOLLOW_PATH:
        if (voice_is_playing(elevator_moby, elevator_vars->motor) == 0) {
            elevator_vars->motor = allocate_voice_for_target_entry(0, 4, elevator_moby);
            if (elevator_vars->motor != 0) {
                if (elevator_vars->motor_volume < 0)
                    elevator_vars->motor_volume = *(int *)(((char *)&voice_pool) + 0x80);
                vendor_volume(elevator_vars->motor, elevator_vars->motor_volume);
            }
        }
        elevator_moby->glow_color = fast_tween_color(elevator_moby->glow_color, 0x80808040, 0.1f);
        if (advance_elevator_along_path(elevator_moby) != 0) {
            elevator_moby->state = 3;
            elevator_vars->level = elevator_vars->motor_volume;
        }
        rebuild_elevator_world_path((u8 *)elevator_moby);
        fast_vec_sub(&delta, &elevator_moby->pos, &old);
        update_entity_matrix(elevator_vars->spring, &delta, &rot, &elevator_moby->rot);
        break;
    case ELEVATOR_STATE_SETTLE_AT_ENDPOINT:
        if (fast_vec_length(elevator_vars->vel) >= 0.001f) {
            if (voice_is_playing(elevator_moby, elevator_vars->motor) == 0)
                elevator_vars->motor = allocate_voice_for_target_entry(0, 4, elevator_moby);
            level = convert_integer_to_float(elevator_vars->level);
            vendor_spring(&level,
                          convert_integer_to_float(elevator_vars->motor_volume) * 0.25f,
                          &elevator_vars->level_vel,
                          0.003f,
                          0.2f,
                          0.0f);
            elevator_vars->level = vendor_ftoi(level);
            if (elevator_vars->motor >= 0)
                vendor_volume(elevator_vars->motor, elevator_vars->level);
            rebuild_elevator_world_path((u8 *)elevator_moby);
        } else {
            int voice;
            update_elevator_idle_glow(elevator_moby);
            if (elevator_vars->motor != -1) {
                char *e = ((char *)&voice_pool) + elevator_vars->motor * 0x70;
                if (*(struct Moby **)(e + 0x88) == elevator_moby && ((u8 *)e)[0x74] != 0)
                    release_voice_slot(elevator_vars->motor);
            }
            elevator_vars->motor = -1;
            voice = allocate_voice_for_target_entry(1, 0, elevator_moby);
            if (voice >= 0)
                vendor_volume(voice, elevator_vars->hum_volume / 3);
            elevator_moby->state = 4;
            if (hero.ground_moby != elevator_moby)
                elevator_moby->state = 1;
        }
        break;
    case ELEVATOR_STATE_WAIT_FOR_RIDER_EXIT:
        update_elevator_idle_glow(elevator_moby);
        if (hero.ground_moby != elevator_moby)
            elevator_moby->state = 1;
        break;
    }
    if (elevator_moby->state != 2 && elevator_moby->state != 0) {
        settle_elevator_at_path_endpoint(elevator_moby);
        if (hero.ground_moby == elevator_moby) {
            fast_vec_sub(&delta, &elevator_moby->pos, &old);
            update_entity_matrix(elevator_vars->spring, &delta, &rot, &elevator_moby->rot);
        }
    }
}

void initialize_elevator_path_state(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    int  *path;
    int  *primary_path;
    int  *first;
    int   i;

    if (*(int *)(moby_vars + 0xA0) == -1) {
        mark_moby_for_removal(moby);
        return;
    }
    primary_path = (int *)l14_paths[*(int *)(moby_vars + 0xA0)];
    if (*primary_path == 0 || *(int *)(moby_vars + 0x114) == -1 ||
        *(int *)l14_paths[*(int *)(moby_vars + 0x114)] != 11) {
        mark_moby_for_removal(moby);
        return;
    }

    *(float *)(moby_vars + 0xB4) = 0.0f;
    path = primary_path;
    first = path + 4;
    for (i = 0; i < *path - 1; i++) {
        ((f32 *)path)[i * 4 + 7] = fast_vec_dist(&path[i * 4 + 4], &path[i * 4 + 8]);
        *(float *)(moby_vars + 0xB4) += ((f32 *)path)[i * 4 + 7];
    }
    {
        f32   len = fast_vec_dist(&((f32 *)primary_path)[i * 4 + 4], first);
        char *pt = (char *)primary_path + i * 16;
        *(f32 *)(pt + 0x1C) = len;
    }

    path = (int *)l14_paths[*(int *)(moby_vars + 0xB8)];
    *(float *)(moby_vars + 0xBC) = 0.0f;
    for (i = 0; i < *path - 1; i++) {
        ((f32 *)path)[i * 4 + 7] = fast_vec_dist(&path[i * 4 + 4], &path[i * 4 + 8]);
        *(float *)(moby_vars + 0xBC) += ((f32 *)path)[i * 4 + 7];
    }
    {
        f32   len = fast_vec_dist(&((f32 *)path)[i * 4 + 4], path + 4);
        char *pt = (char *)path + i * 16;
        *(f32 *)(pt + 0x1C) = len;
    }
    *(float *)(moby_vars + 0xAC) = -1.0f;
    *(int *)(moby_vars + 0xA8) = 0;
    *(int *)(moby_vars + 0xA4) = 0;
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002F05D8), where it is exact; names translated to the US level program. */

extern float D_0015ED6C_c __asm__("D_0015ED6C");
extern float interpolate_angle(float, float, float) __asm__("FUN_L00_0025b750");
extern int   sample_path() __asm__("FUN_L00_0025d808");
extern int
FUN_L00_0025d808_2f05d8(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");

extern short elevator_path_step_rate_gp __asm__("D_L14_00161D14") __attribute__((sda));

int advance_elevator_along_path(struct Moby *elevator_moby) {
    char *moby_vars = (char *)elevator_moby->pvars;
    float rot[4];
    float pos[4];
    char *selected_path;
    float total;
    float step;
    float rotation_blend;
    float x_rotation_blend;
    int   path_endpoint_reached;
    if (elevator_moby->command == 0) {
        selected_path = ((char *)l14_paths[*(int *)(moby_vars + 0xA0)]);
        total = *(float *)(moby_vars + 0xB4);
        qcopy(rot, moby_vars + 0xE0);
    } else {
        selected_path = ((char *)l14_paths[*(int *)(moby_vars + 0xB8)]);
        total = *(float *)(moby_vars + 0xBC);
        qcopy(rot, moby_vars + 0xF0);
    }
    step = *(float *)&elevator_path_step_rate_gp * D_0015ED6C_c;
    path_endpoint_reached = FUN_L00_0025d808_2f05d8(selected_path,
                                                    pos,
                                                    moby_vars + 0xA4,
                                                    moby_vars + 0xA8,
                                                    0,
                                                    step * *(float *)(moby_vars + 0xAC));
    FUN_L00_0025b8c0(
        &elevator_moby->pos.x, (float *)(moby_vars + 0xC0), pos[0], 0.003f, 0.2f, 0.0f);
    FUN_L00_0025b8c0(
        &elevator_moby->pos.y, (float *)(moby_vars + 0xC4), pos[1], 0.003f, 0.2f, 0.0f);
    FUN_L00_0025b8c0(
        &elevator_moby->pos.z, (float *)(moby_vars + 0xC8), pos[2], 0.003f, 0.2f, 0.0f);
    *(float *)(moby_vars + 0xB0) += step;
    rotation_blend = *(float *)(moby_vars + 0xB0) / total;
    if (1.0f < rotation_blend)
        rotation_blend = 1.0f;
    if (path_endpoint_reached != 0)
        rotation_blend = 1.0f;
    if (*(float *)(moby_vars + 0xAC) < 0.0f)
        rotation_blend = 1.0f - rotation_blend;
    if (0.0f < *(float *)(moby_vars + 0xAC)) {
        x_rotation_blend = 0.0f;
        if (0.5f < rotation_blend)
            x_rotation_blend = (rotation_blend - 0.5f) + (rotation_blend - 0.5f);
    } else {
        x_rotation_blend = 1.0f;
        if (rotation_blend <= 0.5f)
            x_rotation_blend = rotation_blend + rotation_blend;
    }
    if (1.0f < x_rotation_blend)
        x_rotation_blend = 1.0f;
    elevator_moby->rot.x =
        interpolate_angle(*(float *)(moby_vars + 0xD0), rot[0], x_rotation_blend);
    elevator_moby->rot.y = interpolate_angle(*(float *)(moby_vars + 0xD4), rot[1], rotation_blend);
    elevator_moby->rot.z = interpolate_angle(*(float *)(moby_vars + 0xD8), rot[2], rotation_blend);
    if (path_endpoint_reached != 0) {
        if (0.0f < *(float *)(moby_vars + 0xAC)) {
            int last_segment_index = *(int *)selected_path - 2;
            *(int *)(moby_vars + 0xA4) = last_segment_index;
            *(float *)(moby_vars + 0xA8) =
                *(float *)(selected_path + (last_segment_index << 4) + 0x1C);
        } else {
            *(int *)(moby_vars + 0xA4) = 0;
            *(float *)(moby_vars + 0xA8) = 0.0f;
        }
        return 1;
    }
    return 0;
}
/* Springs the moby's position (and, while moving, its rotation) toward its current path point. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002F0868), where it is exact; names translated to the US level program. */

void settle_elevator_at_path_endpoint(struct Moby *elevator_moby) {
    char *moby_vars = (char *)elevator_moby->pvars;
    float pos[4];
    float rot[4];
    char *selected_path;
    if (elevator_moby->command == 0) {
        selected_path = ((char *)l14_paths[*(int *)(moby_vars + 0xA0)]);
        qcopy(rot, moby_vars + 0xE0);
    } else {
        selected_path = ((char *)l14_paths[*(int *)(moby_vars + 0xB8)]);
        qcopy(rot, moby_vars + 0xF0);
    }
    if (*(float *)(moby_vars + 0xAC) > 0.0f) {
        qcopy(pos, selected_path + *(int *)selected_path * 16);
    } else {
        qcopy(pos, selected_path + 0x10);
    }
    FUN_L00_0025b8c0(
        &elevator_moby->pos.x, (float *)(moby_vars + 0xC0), pos[0], 0.003f, 0.2f, 0.0f);
    FUN_L00_0025b8c0(
        &elevator_moby->pos.y, (float *)(moby_vars + 0xC4), pos[1], 0.003f, 0.2f, 0.0f);
    FUN_L00_0025b8c0(
        &elevator_moby->pos.z, (float *)(moby_vars + 0xC8), pos[2], 0.003f, 0.2f, 0.0f);
    if (0.0f < *(float *)(moby_vars + 0xAC)) {
        FUN_L00_0025b8c0(
            &elevator_moby->rot.x, (float *)(moby_vars + 0x100), rot[0], 0.003f, 0.2f, 0.0f);
        FUN_L00_0025b8c0(
            &elevator_moby->rot.y, (float *)(moby_vars + 0x104), rot[1], 0.003f, 0.2f, 0.0f);
        FUN_L00_0025b8c0(
            &elevator_moby->rot.z, (float *)(moby_vars + 0x108), rot[2], 0.003f, 0.2f, 0.0f);
    }
}

/* Select the vendor moby's state from its value and mark it initialized. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002F0A00), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0xAC];
    float value;
    char  padB0[0x74];
    short field124;
} Level14VendorData;

typedef struct {
    char               pad0[0x78];
    Level14VendorData *data;
    char               pad7C[0x40];
    unsigned char      state;
} Level14VendorMoby;

/* Picks state 2 when the data value is negative, else 1, and marks the data active. */
void enable_elevator_alternate_route(Level14VendorMoby *moby) __asm__("FUN_L14_002ef578");

void enable_elevator_alternate_route(Level14VendorMoby *moby) {
    Level14VendorData *data = moby->data;
    moby->state = data->value < 0.0f ? 2 : 1;
    data->field124 = 1;
}
/* Clears the vendor moby's flags and marks its id as collected in both bit tables. */

extern int D_0015ED84_m __asm__("D_0015ED84");
extern int D_L14_001BABD0[];

void mark_elevator_collected(Level14VendorMoby *vendor_moby) __asm__("FUN_L14_002ef5a8");

void mark_elevator_collected(Level14VendorMoby *vendor_moby) {
    vendor_moby->state = 0;
    vendor_moby->data->field124 = 0;
    D_0014C190_257218[D_0015ED84_m][(short)*(unsigned short *)((char *)vendor_moby + 0xB2) >> 5] |=
        1 << (*(unsigned short *)((char *)vendor_moby + 0xB2) & 0x1F);
    D_L14_001BABD0[(short)*(unsigned short *)((char *)vendor_moby + 0xB2) >> 5] |=
        1 << (*(unsigned short *)((char *)vendor_moby + 0xB2) & 0x1F);
}

extern u8 elevator_local_path_points[] __asm__("D_L14_001E03B0");

/* 0x002EF630, 192 bytes.  Rebuilds the 11 vectors of a table entry from the
 * moby's matrix and position.
 * Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c:
 * func_L14_002F0AB8), where it is exact; names translated to the US level
 * program. */
void rebuild_elevator_world_path(u8 *moby) {
    u8          *data = ((u8 *)MOBY(moby)->pvars);
    u8          *local_path_point_cursor = elevator_local_path_points;
    u8          *world_path_point_cursor = ((u8 *)l14_paths[*(s32 *)(data + 0x114)]) + 0x10;
    s32          i = 10;
    f32          one = 1.0f;
    struct Hero *x;
    do {
        transform_vector_by_basis(world_path_point_cursor, local_path_point_cursor, moby + 0xC0);
        local_path_point_cursor += 0x10;
        i--;
        fast_vec_add(world_path_point_cursor, world_path_point_cursor, moby + 0x10);
        *(f32 *)(world_path_point_cursor + 0xC) = one;
        world_path_point_cursor += 0x10;
    } while (i >= 0);
    x = &hero;
    x->path_constrain_pending = *(u16 *)(data + 0x114);
}

/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002F0B78), where it is exact; names translated to the US level program. */

extern float absolute_float(float) __asm__("func_001F99C0");
extern float fast_sin(float);
extern short D_0015EE6C_s __asm__("D_0015ED6C");

void update_elevator_idle_glow(struct Moby *moby) {
    char        *data = (char *)moby->pvars;
    struct Hero *x = &hero;
    if (absolute_float(moby->pos.x - x->motion.pos.f[0]) > 40.0f ||
        absolute_float(moby->pos.y - x->motion.pos.f[1]) > 40.0f) {
        moby->glow_color = fast_tween_color(moby->glow_color, 0x80303030, 0.1f);
    } else {
        int   i, col;
        float a =
            fast_add_rotations(*(float *)(data + 0x138), *(float *)&D_0015EE6C_s * 6.2831855f);
        *(float *)(data + 0x138) = a;
        i = func_001FA898_r((fast_sin(a) * 4.0f - 3.0f) * 128.0f);
        if (i > 0x80)
            i = 0x80;
        else if (i < 0x20)
            i = 0x20;
        col = (i << 16) | 0x80000000 | (i << 8) | i;
        moby->glow_color = fast_tween_color(moby->glow_color, col, 0.1f);
    }
}

/* pvars of the taxi ship (same layout as FUN_L14_002f1040/f1140/f13f8 use) */
typedef struct {
    u8   pad0[0x20];
    u8   spring[0x40]; /* 0x20 */
    f32  pos[4];       /* 0x60 */
    f32  rot[4];       /* 0x70 */
    s32  points[5];    /* 0x80 */
    u8   pad94[0x18];
    s32 *path;   /* 0xAC: camera path */
    s16  hB0;    /* 0xB0 */
    s16  sound;  /* 0xB2 */
    s16  picked; /* 0xB4 */
    s16  target; /* 0xB6 */
    s32  item_a; /* 0xB8 */
    s32  item_b; /* 0xBC */
    f32  t;      /* 0xC0 */
    f32  tv;     /* 0xC4 */
    u8   padC8[0x18];
    f32  save_pos[4]; /* 0xE0 */
    f32  save_rot[4]; /* 0xF0 */
    s32  clip[3];     /* 0x100 */
} L14TaxiVars;

extern s32  D_L14_0015F5CC;
extern s32  D_L14_0015F594;
extern char D_0013CAE4[];
extern float
advance_accelerated_scalar(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern int  advance_moby_path(char *) __asm__("FUN_L05_003195f8");
extern void reset_selected_moby_to_current_placement(void *) __asm__("FUN_L05_00319510");
extern void apply_moby_position_and_rotation_wobble(char *) __asm__("FUN_L05_00319740");
extern void transform_moby_vector_table_to_world_space(char *) __asm__("FUN_L05_003198e8");
extern void taxi_sample(float, s32 *, int, void *, void *, int) __asm__("FUN_00214e58");
void        initialize_taxi_stop_from_position(struct Moby *moby) __asm__("FUN_L14_002f1040");

void prepare_taxi_route(struct Moby *moby) __asm__("FUN_L14_002f1140");

struct Moby *find_group_moby_by_tag(struct Moby *moby, int tag) __asm__("FUN_L14_002f12f0");

void request_taxi_stop_prompt(struct Moby *moby) __asm__("FUN_L14_002f13f8");

extern void taxi_sample_f(s32 *, int, void *, void *, int, float) __asm__("FUN_00214e58");
/* Ported from rac1-decomp src/overlays/l14_oltanis/vendor_002E0538.c (func_L14_002F19C0) */
void taxi_update(struct Moby *moby) __asm__("FUN_L14_002f0538");

void taxi_update(struct Moby *moby) {
    enum {
        TAXI_STATE_INITIALIZE = 0,
        TAXI_STATE_WAIT_AT_STOP = 1,
        TAXI_STATE_BLEND_STOP_TO_PATH_START = 3,
        TAXI_STATE_FOLLOW_PATH = 4,
        TAXI_STATE_BLEND_PATH_END_TO_STOP = 5,
        TAXI_STATE_WAIT_FOR_ITEM_GATE = 6,
        TAXI_STATE_BLEND_SAVED_POSE_TO_PATH_START = 7
    };
    L14TaxiVars *taxi_vars = (L14TaxiVars *)moby->pvars;
    float        previous_position[4], previous_rotation[4], position_delta[4],
        blend_from_position_or_rotation_matrix[4], blend_to_position[4], blend_from_rotation[4],
        blend_to_rotation[4], hero_relative_position[4];
    float taxi_inverse_rotation_matrix[16];
    int   ticks = 10;
    int   button = 0x10;
    int   wrap = 0;
    int   flags = 0;
    qcopy(previous_position, &moby->pos);
    qcopy(previous_rotation, &moby->rot);
    set_moby_dialogue_record_value(moby, (moby->flags ^ 1) & 1);
    switch (moby->state) {
    case TAXI_STATE_INITIALIZE:
        taxi_vars->hB0 = -1;
        taxi_vars->sound = -1;
        initialize_taxi_stop_from_position(moby);
        break;
    case TAXI_STATE_WAIT_AT_STOP: {
        struct Hero *g = &hero;
        if (g->ground_moby == moby && g->air_frames.s == 0) {
            transpose_rotation_matrix(blend_from_position_or_rotation_matrix, &moby->rot_row0);
            fast_vec_sub(hero_relative_position, &g->motion.pos, &moby->pos);
            hero_relative_position[3] = 0;
            transform_vector(hero_relative_position,
                             hero_relative_position,
                             blend_from_position_or_rotation_matrix);
            if (absolute_float(hero_relative_position[1]) < 1.2f) {
                prepare_taxi_route(moby);
                request_taxi_stop_prompt(moby);
                if ((*(int *)D_0013CAE4 & button) && D_L14_0015F594 == 4) {
                    struct Moby *m;
                    moby->state = 3;
                    m = find_group_moby_by_tag(moby, taxi_vars->target);
                    if (m != 0) {
                        m->state = 2;
                        m->flags |= 1;
                        m->visible = 0;
                        m->collision = 0;
                    }
                    taxi_vars->hB0 = -1;
                }
            }
        } else {
            if (taxi_vars->clip[0] >= 0 && taxi_vars->picked != 0 &&
                is_point_inside_clip_volume(&hero.motion.pos, taxi_vars->clip[0])) {
                if (D_L14_0015F5CC < scale_game_frames(ticks)) {
                    taxi_vars->target = 0;
                    taxi_vars->picked = 0;
                    qcopy_nc(&moby->pos, &l14_cuboids[taxi_vars->points[taxi_vars->picked]].pos);
                    qcopy_nc(&moby->rot, &l14_cuboids[taxi_vars->points[taxi_vars->picked]].rot);
                    moby->state = 1;
                    qcopy_nc(taxi_vars->pos,
                             &l14_cuboids[taxi_vars->points[taxi_vars->picked]].pos);
                    qcopy_nc(taxi_vars->rot,
                             &l14_cuboids[taxi_vars->points[taxi_vars->picked]].rot);
                } else {
                    if (taxi_vars->picked == 2) {
                        taxi_vars->picked = 3;
                    } else {
                        taxi_vars->picked = 1;
                    }
                    prepare_taxi_route(moby);
                    moby->state = 3;
                }
            }
            if (taxi_vars->clip[1] >= 0 && taxi_vars->picked != 1 &&
                is_point_inside_clip_volume(&hero.motion.pos, taxi_vars->clip[1])) {
                if (D_L14_0015F5CC < scale_game_frames(ticks)) {
                    taxi_vars->target = 1;
                    taxi_vars->picked = 1;
                    qcopy_nc(&moby->pos, &l14_cuboids[taxi_vars->points[taxi_vars->picked]].pos);
                    qcopy_nc(&moby->rot, &l14_cuboids[taxi_vars->points[taxi_vars->picked]].rot);
                    moby->state = 1;
                    qcopy_nc(taxi_vars->pos,
                             &l14_cuboids[taxi_vars->points[taxi_vars->picked]].pos);
                    qcopy_nc(taxi_vars->rot,
                             &l14_cuboids[taxi_vars->points[taxi_vars->picked]].rot);
                } else {
                    taxi_vars->picked = 0;
                    prepare_taxi_route(moby);
                    moby->state = 3;
                }
            }
            if (taxi_vars->clip[2] >= 0 && taxi_vars->picked < 2 &&
                is_point_inside_clip_volume(&hero.motion.pos, taxi_vars->clip[2])) {
                if (D_L14_0015F5CC < scale_game_frames(ticks)) {
                    taxi_vars->target = 2;
                    taxi_vars->picked = 2;
                    qcopy_nc(&moby->pos, &l14_cuboids[taxi_vars->points[taxi_vars->picked]].pos);
                    qcopy_nc(&moby->rot, &l14_cuboids[taxi_vars->points[taxi_vars->picked]].rot);
                    moby->state = 1;
                    qcopy_nc(taxi_vars->pos,
                             &l14_cuboids[taxi_vars->points[taxi_vars->picked]].pos);
                    qcopy_nc(taxi_vars->rot,
                             &l14_cuboids[taxi_vars->points[taxi_vars->picked]].rot);
                } else {
                    taxi_vars->picked = 2;
                    prepare_taxi_route(moby);
                    moby->state = 3;
                }
            }
        }
        break;
    }
    case TAXI_STATE_BLEND_STOP_TO_PATH_START: {
        struct Hero *g = &hero;
        if (g->ground_moby == moby && g->air_frames.s == 0) {
            follow_camera_set_distance(0, 7.0f, 0.002f);
        }
        advance_accelerated_scalar(
            1.0f, frame_time_sq, frame_time_sq, frame_time * 0.5f, &taxi_vars->t, &taxi_vars->tv);
        qcopy_nc(blend_from_position_or_rotation_matrix,
                 &l14_cuboids[taxi_vars->points[taxi_vars->picked]].pos);
        qcopy_nc(blend_from_rotation, &l14_cuboids[taxi_vars->points[taxi_vars->picked]].rot);
        taxi_sample(0.0f, taxi_vars->path, wrap, blend_to_position, blend_to_rotation, flags);
        lerp_vector(taxi_vars->pos,
                    blend_from_position_or_rotation_matrix,
                    blend_to_position,
                    taxi_vars->t);
        taxi_vars->rot[1] = fast_add_rotations(
            fast_subtract_rotations(blend_to_rotation[1], blend_from_rotation[1]) * taxi_vars->t,
            blend_from_rotation[1]);
        taxi_vars->rot[2] = fast_add_rotations(
            fast_subtract_rotations(blend_to_rotation[2], blend_from_rotation[2]) * taxi_vars->t,
            blend_from_rotation[2]);
        if (1.0f <= taxi_vars->t) {
            taxi_vars->t = 0.0f;
            taxi_vars->tv = 0.0f;
            moby->state = 4;
        }
        break;
    }
    case TAXI_STATE_BLEND_SAVED_POSE_TO_PATH_START: {
        float hermite_blend_fraction;
        advance_accelerated_scalar(
            1.0f, frame_time_sq, frame_time_sq, frame_time * 0.5f, &taxi_vars->t, &taxi_vars->tv);
        qcopy(blend_from_position_or_rotation_matrix, taxi_vars->save_pos);
        qcopy(blend_from_rotation, taxi_vars->save_rot);
        taxi_sample_f(taxi_vars->path, wrap, blend_to_position, blend_to_rotation, flags, 0.0f);
        hermite_blend_fraction = cubic_hermite_scalar(1.0f, 0.0f, 1.0f, 0.0f, taxi_vars->t);
        lerp_vector(taxi_vars->pos,
                    blend_from_position_or_rotation_matrix,
                    blend_to_position,
                    hermite_blend_fraction);
        taxi_vars->rot[1] = fast_add_rotations(
            fast_subtract_rotations(blend_to_rotation[1], blend_from_rotation[1]) *
                hermite_blend_fraction,
            blend_from_rotation[1]);
        taxi_vars->rot[2] = fast_add_rotations(
            fast_subtract_rotations(blend_to_rotation[2], blend_from_rotation[2]) *
                hermite_blend_fraction,
            blend_from_rotation[2]);
        if (1.0f <= taxi_vars->t) {
            taxi_vars->t = 0.0f;
            taxi_vars->tv = 0.0f;
            moby->state = 4;
        }
        break;
    }
    case TAXI_STATE_FOLLOW_PATH: {
        struct Hero *g = &hero;
        if (g->ground_moby == moby && g->air_frames.s == 0) {
            follow_camera_set_distance(0, 7.0f, 0.003f);
        }
        if (advance_moby_path((char *)moby)) {
            taxi_vars->t = 0.0f;
            taxi_vars->tv = 0.0f;
            moby->state = 5;
        }
        break;
    }
    case TAXI_STATE_BLEND_PATH_END_TO_STOP: {
        struct Hero *g;
        advance_accelerated_scalar(
            1.0f, frame_time_sq, frame_time_sq, frame_time * 0.25f, &taxi_vars->t, &taxi_vars->tv);
        qcopy_nc(blend_to_position, &l14_cuboids[taxi_vars->points[taxi_vars->target]].pos);
        qcopy_nc(blend_to_rotation, &l14_cuboids[taxi_vars->points[taxi_vars->target]].rot);
        taxi_sample_f(taxi_vars->path,
                      wrap,
                      blend_from_position_or_rotation_matrix,
                      blend_from_rotation,
                      flags,
                      (float)*taxi_vars->path);
        lerp_vector(taxi_vars->pos,
                    blend_from_position_or_rotation_matrix,
                    blend_to_position,
                    taxi_vars->t);
        taxi_vars->rot[1] = fast_add_rotations(
            fast_subtract_rotations(blend_to_rotation[1], blend_from_rotation[1]) * taxi_vars->t,
            blend_from_rotation[1]);
        taxi_vars->rot[2] = fast_add_rotations(
            fast_subtract_rotations(blend_to_rotation[2], blend_from_rotation[2]) * taxi_vars->t,
            blend_from_rotation[2]);
        if (1.0f <= taxi_vars->t) {
            taxi_vars->t = 0.0f;
            taxi_vars->tv = 0.0f;
            moby->state = 1;
            reset_selected_moby_to_current_placement(moby);
        }
        g = &hero;
        if (g->ground_moby == moby && g->air_frames.s == 0) {
            transpose_rotation_matrix(taxi_inverse_rotation_matrix, &moby->rot_row0);
            fast_vec_sub(hero_relative_position, &g->motion.pos, &moby->pos);
            hero_relative_position[3] = 0;
            transform_vector(
                hero_relative_position, hero_relative_position, taxi_inverse_rotation_matrix);
            if (absolute_float(hero_relative_position[1]) < 1.2f) {
                request_taxi_stop_prompt(moby);
                if (*(int *)D_0013CAE4 & button) {
                    qcopy(taxi_vars->save_pos, taxi_vars->pos);
                    qcopy(taxi_vars->save_rot, taxi_vars->rot);
                    initialize_taxi_stop_from_position(moby);
                    taxi_vars->picked = taxi_vars->target;
                    prepare_taxi_route(moby);
                    moby->state = 7;
                    qcopy(taxi_vars->pos, taxi_vars->save_pos);
                    qcopy(taxi_vars->rot, taxi_vars->save_rot);
                    {
                        struct Moby *m = find_group_moby_by_tag(moby, taxi_vars->target);
                        if (m != 0) {
                            m->state = 2;
                            m->flags |= 1;
                            m->visible = 0;
                            m->collision = 0;
                        }
                    }
                }
            }
        }
        break;
    }
    case TAXI_STATE_WAIT_FOR_ITEM_GATE: {
        unsigned char *t = visited52;
        int            k = current_level_index * 16;
        if (t[taxi_vars->item_a + k] == 0xFF || t[taxi_vars->item_b + k] == 0xFF) {
            unsigned short bits = moby->flags;
            int            value = moby->pclass->collision;
            moby->visible = 1;
            moby->collision = value;
            moby->flags = bits & 0xFFFE;
            moby->state = 1;
        }
        break;
    }
    }
    apply_moby_position_and_rotation_wobble((char *)moby);
    if (moby->state >= 3 && moby->state <= 4) {
        struct Hero *g = &hero;
        if (g->ground_moby == moby && g->air_frames.s == 0) {
            g->jump_lock_timer = 2;
            g->unk1F4 = 2;
            transform_moby_vector_table_to_world_space((char *)moby);
        }
        if (voice_is_playing(moby, taxi_vars->sound) == 0) {
            taxi_vars->sound = allocate_voice_for_target_entry(0, 4, moby);
        }
    } else if (voice_is_playing(moby, taxi_vars->sound) != 0) {
        int voice_slot = taxi_vars->sound;
        if (voice_slot != -1) {
            char *voice_entry = ((char *)&voice_pool) + voice_slot * 0x70;
            if (*(struct Moby **)(voice_entry + 0x88) == moby &&
                *(unsigned char *)(voice_entry + 0x74) != 0) {
                release_voice_slot(voice_slot);
            }
        }
        taxi_vars->sound = -1;
    }
    fast_vec_sub(position_delta, &moby->pos, previous_position);
    update_entity_matrix(taxi_vars->spring, position_delta, previous_rotation, &moby->rot);
}

struct L14PickVars {
    char  pad0[0x60];
    float pos[4];
    float rot[4];
    int   points[5];
    char  pad94[0x20];
    short picked;
};

void initialize_taxi_stop_from_position(struct Moby *moby) {
    struct L14PickVars *taxi_vars = (struct L14PickVars *)moby->pvars;
    int                 i;
    taxi_vars->picked = -1;
    for (i = 0; i < 5; i++) {
        if (fast_vec_dist(&moby->pos, &l14_cuboids[taxi_vars->points[i]].pos) < 1.0f) {
            taxi_vars->picked = i;
            break;
        }
    }
    moby->state = 1;
    {
        int          k = taxi_vars->points[taxi_vars->picked];
        LevelCuboid *taxi_stops = l14_cuboids;
        qcopy(taxi_vars->pos, &taxi_stops[k].pos);
        qcopy(taxi_vars->rot, &taxi_stops[k].rot);
    }
}

/* Picks the path for the current point and sets the travel speeds; same source as FUN_L16_002e4a58 with five points. */
extern float D_0015ED6C_u __asm__("D_0015ED6C");

void prepare_taxi_route(struct Moby *moby) {
    char *taxi_vars = (char *)moby->pvars;
    char *selected_path;
    float inv, travel_step_limit, travel_acceleration_step, first_segment_length;
    switch (*(short *)(taxi_vars + 0xB4)) {
    case TAXI_STOP_RETURN_HUB:
        *(char **)(taxi_vars + 0xAC) = ((char *)l14_paths[*(int *)(taxi_vars + 0x94)]);
        *(short *)(taxi_vars + 0xB6) = 1;
        break;
    case TAXI_STOP_HUB_OUTBOUND_DESTINATION:
        *(char **)(taxi_vars + 0xAC) = ((char *)l14_paths[*(int *)(taxi_vars + 0x98)]);
        *(short *)(taxi_vars + 0xB6) = 0;
        break;
    case TAXI_STOP_RETURN_FEEDER_ORIGIN:
        *(char **)(taxi_vars + 0xAC) = ((char *)l14_paths[*(int *)(taxi_vars + 0x9C)]);
        *(short *)(taxi_vars + 0xB6) = 3;
        break;
    case TAXI_STOP_RETURN_FEEDER_DESTINATION:
        *(char **)(taxi_vars + 0xAC) = ((char *)l14_paths[*(int *)(taxi_vars + 0xA0)]);
        *(short *)(taxi_vars + 0xB6) = 0;
        break;
    case 4:
        break;
    }
    selected_path = *(char **)(taxi_vars + 0xAC);
    first_segment_length = fast_vec_dist(selected_path + 0x10, selected_path + 0x20);
    travel_step_limit = D_0015ED6C_u * 5.0f;
    travel_acceleration_step = frame_time_sq * 2.5f;
    inv = 1.0f / (first_segment_length * (float)**(int **)(taxi_vars + 0xAC));
    *(int *)(taxi_vars + 0xC4) = 0;
    *(int *)(taxi_vars + 0xC0) = 0;
    *(float *)(taxi_vars + 0xD4) = travel_step_limit * inv;
    *(float *)(taxi_vars + 0xD8) = travel_acceleration_step * inv;
}

/* Finds the moby in this moby's group list whose vars tag at 0xB4 matches, or returns null. */
struct TaggedVars {
    u8  pad0[0xB4];
    s16 tag; /* 0xB4 */
};

extern u16         *D_L14_001ABF40[];
extern struct Moby *D_L14_0015FFD8_12f0 __asm__("D_L14_0015FFD8");

struct Moby *find_group_moby_by_tag(struct Moby *moby, int tag) {
    u16         *group_entry_cursor;
    struct Moby *pool;
    int          group_moby_index;

    if (moby->group < moby_group_index_limit &&
        (group_entry_cursor = D_L14_001ABF40[moby->group]) != 0) {
        pool = D_L14_0015FFD8_12f0;
        do {
            group_moby_index = *group_entry_cursor & 0x7FFF;
            if (((struct TaggedVars *)pool[group_moby_index].pvars)->tag == tag) {
                goto found;
            }
        } while ((s16)*group_entry_cursor++ >= 0);
    }
    return 0;
found:
    return &D_L14_0015FFD8_12f0[group_moby_index];
}

extern void try_set_help_message(int, int) __asm__("FUN_00215130");

void request_taxi_stop_prompt(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int   taxi_stop_index = *(short *)(data + 0xB4);
    if (moby->state == 5) {
        taxi_stop_index = *(short *)(data + 0xB6);
    }
    switch (taxi_stop_index) {
    case TAXI_STOP_RETURN_HUB:
        try_set_help_message(4, 0x53ED);
        break;
    case TAXI_STOP_HUB_OUTBOUND_DESTINATION:
        try_set_help_message(4, 0x53EE);
        break;
    case TAXI_STOP_RETURN_FEEDER_ORIGIN:
        try_set_help_message(4, 0x53EE);
        break;
    case TAXI_STOP_RETURN_FEEDER_DESTINATION:
        try_set_help_message(4, 0x53EE);
        break;
    case 4:
        try_set_help_message(4, 0x1399);
        break;
    default:
        try_set_help_message(4, 0x139B);
        break;
    }
}

extern unsigned char D_0015EDB0;

typedef struct {
    float target[4];
    float eye[4];
    float delta[4];
} L14WatchScratch;

typedef struct {
    char           pad00[0x10];
    float          position[4];
    unsigned char  state;
    char           pad21[0xF];
    unsigned char  opacity;
    unsigned char  near_flag;
    char           pad32[0x16];
    float          yaw;
    char           pad4C[7];
    unsigned char  animation;
    char           pad54[0x24];
    char          *data;
    char           pad7C[3];
    unsigned char  flag7F;
    char           pad80[0x30];
    unsigned char  bB0;
    char           padB1;
    unsigned short hB2;
} L14WatchMoby;

typedef struct {
    char           pad00[4];
    unsigned short sub;
    char           pad06[2];
    unsigned char  b8;
    char           pad09[3];
    float          fC;
    char           pad10[0x10];
    void          *p20;
    char           pad24[0x1C];
    int            i40;
    int            i44;
    int            i48;
    char           pad4C[0x34];
    char           body[0x64];
    float          pitch;
    float          yaw;
    char           padEC[4];
    float          height;
    char           padF4[0xC];
    char           head[0x68];
    float          head_yaw;
    char           pad16C[0x14];
    float          target[4];
    char           pad190[4];
    int            moving_timer;
    int            target_timer;
} L14WatchData;

typedef struct {
    char  pad00[0xD0];
    float aim[4];
} L14Player;

extern unsigned char D_0015EDB0_b __asm__("D_0015EDB0");
extern float         scale_time(float) __asm__("FUN_001f96b0");
extern int           FUN_001fa6d0(float);
extern void          FUN_L00_0025a120(void *);
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");

typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} L14LevelState;

extern L14LevelState  D_L14_001BB930;
extern char           captain_qwark_name[] __asm__("D_L14_00161F20");
extern unsigned char *D_L14_0015FFD8;
extern void           initialize_npc_dialogue(void *, void *) __asm__("FUN_L00_002668a0");
extern void           begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
void                  mark_elevator_collected(Level14VendorMoby *m) __asm__("FUN_L14_002ef5a8");
extern void           FUN_L00_00263d40(int, int);
extern void           memcard_save_data(int, int) __asm__("FUN_0020b178");

/* Watcher moby update: gated by its collectable and trigger, then turns its head toward the hero or a random target. */
void qwark_oltanis_update(L14WatchMoby *m) __asm__("FUN_L14_002fba20");

void qwark_oltanis_update(L14WatchMoby *moby) {
    enum {
        QWARK_OLTANIS_STATE_INITIALIZE_DIALOGUE = 0,
        QWARK_OLTANIS_STATE_UPDATE_DIALOGUE = 1,
        QWARK_OLTANIS_STATE_POST_DIALOGUE = 2,
        QWARK_OLTANIS_STATE_REMOVE = 3
    };
    L14WatchScratch scratch;
    L14WatchData   *moby_vars = (L14WatchData *)moby->data;
    float           rate;
    float           head_rate;
    int             tracking;
    attach_big_head_cheat_manipulator_to_matching_moby(moby);
    if (moby->near_flag) {
        if (fast_vec_dist(moby->position, D_L14_001674C0) < 28.0f) {
            FUN_L00_0025a120(moby);
            moby->flag7F = 0x16;
        }
    }
    switch (moby->state) {
    case QWARK_OLTANIS_STATE_INITIALIZE_DIALOGUE: {
        unsigned short id = moby->hB2;
        if (D_L14_001BB930.collected[(short)id] != 0 ||
            (D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 0x1F)) & 1) {
            mark_moby_for_removal(moby);
            return;
        }
        if (moby_vars->i44 == -1 || moby_vars->i48 == -1) {
            mark_moby_for_removal(moby);
            return;
        }
        moby_vars->p20 = (void *)captain_qwark_name;
        initialize_npc_dialogue(moby, moby_vars);
        moby_vars->b8 = 1;
        moby->state = 1;
        break;
    }
    case QWARK_OLTANIS_STATE_UPDATE_DIALOGUE:
        if (moby_vars->b8 == 1 && is_point_inside_clip_volume(&hero.motion.pos, moby_vars->i40)) {
            if (visited52[moby->bB0 + current_level_index * 16] != 0xFF) {
                char *camera_point;
                jet_clear_persistent(moby->bB0);
                camera_point = (char *)l14_cuboids + moby_vars->i48 * 128;
                jet_reset_camera(camera_point + 0x30, camera_point + 0x70);
            }
            enable_elevator_alternate_route(D_L14_0015FFD8 + moby_vars->i44 * 256);
            moby_vars->fC = 255.0f;
        } else {
            moby_vars->fC = 1.7f;
        }
        if (update_npc_dialogue(moby, moby_vars)) {
            begin_dialogue_camera_transition(moby, 3.0f);
            moby->state = 2;
        }
        break;
    case QWARK_OLTANIS_STATE_POST_DIALOGUE:
        if (D_L14_0015F5C4 != 2) {
            moby->state = 1;
            if ((unsigned)(moby_vars->sub - 3) < 2) {
                if ((short)moby_vars->sub == 4)
                    FUN_L00_00263d40(0x53DD, -1);
                else
                    FUN_L00_00263d40(0x36B3, -1);
                grant_inventory_item(0x20, 1);
                mark_elevator_collected(D_L14_0015FFD8 + moby_vars->i44 * 256);
                D_0014C190_257218[D_0015ED84_m][(short)moby->hB2 >> 5] |= 1 << (moby->hB2 & 0x1F);
                D_L14_001BABD0[(short)moby->hB2 >> 5] |= 1 << (moby->hB2 & 0x1F);
                memcard_save_data(0, -1);
                mark_moby_for_removal(moby);
                return;
            }
        }
        break;
    case QWARK_OLTANIS_STATE_REMOVE:
        mark_moby_for_removal(moby);
        return;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 0) {
        char      *hero_position = (char *)&hero.motion.pos;
        L14Player *hero_data;
        tracking = 1;
        if (vector_distance_xy(moby->position, hero_position) < 8.0f &&
            (hero_data = (L14Player *)(hero_position - 0x80),
             fast_difference_between_rotations(
                 moby->yaw,
                 fast_arc_tan(hero_data->aim[0] - moby->position[0],
                              hero_data->aim[1] - moby->position[1]))) < 1.5707964f) {
            if (fast_vec_length(hero_position + 0x80) > 0.01f)
                moby_vars->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&moby_vars->moving_timer);
        } else if (moby_vars->moving_timer) {
            moby_vars->moving_timer = 0;
            qcopy(moby_vars->target, hero.motion.body_center.f);
        }
        if (fast_dec_timer(&moby_vars->target_timer)) {
            float heading;
            moby_vars->target_timer =
                FUN_001fa6d0(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(
                moby_vars->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(moby_vars->target, moby_vars->target, moby->position);
        }
        if (moby_vars->moving_timer) {
            qcopy(scratch.target, hero.motion.body_center.f);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, moby_vars->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, moby->position);
        scratch.eye[2] += 1.0f;
        fast_vec_sub(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(fast_arc_tan(scratch.delta[0], scratch.delta[1]), moby->yaw);
        pitch = -fast_arc_tan(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        moby_vars->pitch = pitch;
        moby_vars->yaw = yaw * 0.6f;
        moby_vars->head_yaw = yaw * 0.4f;
    }
    if (D_0015EDB0_b)
        moby_vars->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_vars->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_vars->head, 1);
}

extern void cart_burst(float, float, struct Moby *, void *, int) __asm__("FUN_L00_0025f090");
extern void allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");
void        initialize_flight_path_state(struct Moby *moby) __asm__("FUN_L14_002fc890");

extern void initialize_flight_path_state(struct Moby *) __asm__("FUN_L14_002fc890");
void        advance_enemy_fighter_along_flight_path(struct Moby *moby) __asm__("FUN_L14_002fc9a0");

extern void advance_enemy_fighter_along_flight_path(struct Moby *) __asm__("FUN_L14_002fc9a0");
extern void spawn_enemy_fighter_exhaust(struct Moby *) __asm__("FUN_L14_002fc3e8");

extern struct {
    char          pad[0x18];
    unsigned char flag;
} D_0013D408;

extern s32 enemy_fighter_respawn_delay_frames_gp __asm__("D_L14_00161F5C") __attribute__((sda));
char       D_L14_001FCE10[] = "Enemy Fighter (moby908) with no flyPath. UID: %d - RAR\n";
char       D_L14_001FCE48[] =
    "Enemy Fighter (moby908) with a flyPath that has no points. UID: %d - RAR\n";

typedef struct {
    u8  pad0[0x60];
    s32 segment; /* 0x60 */
    s32 along;   /* 0x64 */
    s32 path;    /* 0x68 */
    s32 sound;   /* 0x6C */
    f32 f70;     /* 0x70 */
    f32 f74;     /* 0x74 */
    f32 f78;     /* 0x78 */
    s32 timer;   /* 0x7C */
    f32 f80;     /* 0x80 */
    f32 f84;     /* 0x84 */
    f32 f88;     /* 0x88 */
} L14CartVars;

/* Cart update: follows its path, bursts when shot and respawns at the path start after a delay. */
void update_enemy_fighter(struct Moby *moby) __asm__("FUN_L14_002fc0f0");

void update_enemy_fighter(struct Moby *moby) {
    enum {
        ENEMY_FIGHTER_STATE_INITIALIZE = 0,
        ENEMY_FIGHTER_STATE_FOLLOW_PATH = 1,
        ENEMY_FIGHTER_STATE_WAIT_FOR_RESPAWN = 2
    };
    L14CartVars *data = (L14CartVars *)moby->pvars;
    int          hit = find_moby_hit(moby, 0x800000, 0);
    int          index;
    int          path_index;
    char        *path;
    moby->hit_slot = 0xff;
    if (hit && moby->state != 2) {
        if (current_level_index == 14) {
            enemy_fighter_destruction_count = enemy_fighter_destruction_count + 1;
            if (enemy_fighter_destruction_count > 2 && D_0013D408.flag == 0) {
                D_0013D408.flag = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53d6, -1);
            }
        }
        moby->state = 2;
        cart_burst(5.0f, 13.0f, moby, &moby->pos, -1);
        moby->collision = 0;
        moby->flags |= 0x41;
        index = data->sound;
        if (index != -1) {
            char *entry = ((char *)&voice_pool) + index * 0x70;
            if (*(struct Moby **)(entry + 0x88) == moby && *(unsigned char *)(entry + 0x74) != 0)
                release_voice_slot(index);
        }
        data->sound = -1;
    }
    switch (moby->state) {
    case ENEMY_FIGHTER_STATE_INITIALIZE:
        path_index = data->path;
        if (path_index == -1) {
            debug_print(D_L14_001FCE10, moby->save_id);
            mark_moby_for_removal(moby);
            return;
        }
        if (*(int *)l14_paths[path_index] == 0) {
            debug_print(D_L14_001FCE48, moby->save_id);
            mark_moby_for_removal(moby);
            return;
        }
        initialize_flight_path_state(moby);
        moby->state = 1;
        moby->update_dist = 0xff;
        moby->draw_dist = 0xff;
        data->sound = -1;
        data->timer = scale_game_frames(enemy_fighter_respawn_delay_frames_gp);
        break;
    case ENEMY_FIGHTER_STATE_FOLLOW_PATH:
        advance_enemy_fighter_along_flight_path(moby);
        break;
    case ENEMY_FIGHTER_STATE_WAIT_FOR_RESPAWN:
        if (fast_dec_timer(&data->timer)) {
            path = ((char *)l14_paths[data->path]);
            moby->flags &= 0xffbe;
            moby->collision = moby->pclass->collision;
            data->timer = scale_game_frames(enemy_fighter_respawn_delay_frames_gp);
            moby->state = 1;
            data->segment = 0;
            data->along = 0;
            qcopy(&moby->pos, path + 0x10);
            moby->rot.z = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                       *(float *)(path + 0x24) - *(float *)(path + 0x14));
            data->f80 = data->f84 = data->f88 = data->f70 = data->f74 = data->f78 = 0.0f;
        }
        break;
    }
    spawn_enemy_fighter_exhaust(moby);
    if (moby->state != 0 && moby->state != 2 && voice_is_playing(moby, data->sound) == 0)
        data->sound = allocate_voice_for_target_entry(0, 4, moby);
}

/* A smoke puff from FUN_L00_0026d000. */
typedef struct {
    u8  pad0[4];
    s32 mode; /* 0x04 */
    u8  pad8[2];
    u8  tint; /* 0x0A */
    u8  fade; /* 0x0B */
} L14PuffTail;

typedef struct {
    u8          pad0[3];
    u8          alpha; /* 0x03 */
    u8          pad4[4];
    u8          shade; /* 0x08 */
    s8          frame; /* 0x09 */
    s16         life;  /* 0x0A */
    u8          padC[0x14];
    L14PuffTail tail; /* 0x20 */
} L14Puff;

#include "rnc/overlay/l14/mobys/jet_data/spawn_enemy_fighter_exhaust.h"

extern char D_L14_0015F580[] MACRO_ADDR;
extern int  exhaust_ftoi(float) __asm__("FUN_001fa6d0");
extern L14Puff *
spawn_puff(void *, f32, f32, f32, int, void *, f32, int) __asm__("FUN_L00_0026d000");

/* Engine exhaust: two smoke puffs and three fading rings on each side of the moby. */
void spawn_enemy_fighter_exhaust(struct Moby *emitter_moby) {
    Vec4     step;
    Vec4     pos;
    Vec4     base;
    L14Puff *puff;
    int      i;
    int      j;
    int      r;
    int      spin;
    int      life;
    float    size;

    if (emitter_moby->state == 0 || emitter_moby->state == 2 || emitter_moby->visible == 0)
        return;
    fast_vec_scale(&step, &emitter_moby->rot_row0, enemy_fighter_exhaust_base_row0_offset_gp);
    fast_vec_add(&pos, &emitter_moby->pos, &step);
    fast_vec_scale(&step, &emitter_moby->rot_row2, enemy_fighter_exhaust_base_row2_offset_gp);
    fast_vec_add(&pos, &pos, &step);
    qcopy(&base, &pos);
    fast_vec_scale(&step, &emitter_moby->rot_row1, enemy_fighter_exhaust_paired_emitter_offset_gp);
    fast_vec_add(&pos, &pos, &step);
    for (j = 0; j < 2; j++) {
        spin = random_integer_below(16);
        puff = spawn_puff(&pos,
                          0.2f,
                          1.0f,
                          0.9f,
                          random_integer_below(2) == 0 ? spin : -spin,
                          D_L14_0015F580,
                          200000.0f,
                          enemy_fighter_exhaust_paired_puff_color_gp);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->life = scale_game_frames(12);
            puff->frame = (emitter_moby->draw_dist >> 5 << 4) + exhaust_ftoi(4.0f);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
    }
    spin = 16;
    size = 100000.0f;
    fast_vec_scale(&step, &emitter_moby->rot_row0, enemy_fighter_exhaust_ring_row0_offset_gp);
    fast_vec_add(&pos, &pos, &step);
    life = scale_game_frames(2);
    for (i = 0; i < 3; i++) {
        puff = spawn_puff(&pos, 0.05f, 1.0f, 1.0f, spin, D_L14_0015F580, size, 0x7FFFFFFF);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->frame = (emitter_moby->draw_dist >> 5 << 4) + exhaust_ftoi(4.0f);
            puff->life = life;
            puff->shade = random_integer_below(0xFF);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
        spin = -spin;
        life *= 2;
        size -= 20000.0f;
    }
    fast_vec_scale(&step, &emitter_moby->rot_row1, enemy_fighter_exhaust_paired_emitter_offset_gp);
    fast_vec_sub(&pos, &base, &step);
    for (j = 0; j < 2; j++) {
        spin = random_integer_below(16);
        puff = spawn_puff(&pos,
                          0.2f,
                          1.0f,
                          0.9f,
                          random_integer_below(2) == 0 ? spin : -spin,
                          D_L14_0015F580,
                          200000.0f,
                          enemy_fighter_exhaust_paired_puff_color_gp);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->frame = (emitter_moby->draw_dist >> 5 << 4) + exhaust_ftoi(4.0f);
            puff->life = scale_game_frames(12);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
    }
    spin = 16;
    size = 100000.0f;
    fast_vec_scale(&step, &emitter_moby->rot_row0, enemy_fighter_exhaust_ring_row0_offset_gp);
    fast_vec_add(&pos, &pos, &step);
    life = scale_game_frames(2);
    for (i = 0; i < 3; i++) {
        puff = spawn_puff(&pos, 0.05f, 1.0f, 1.0f, spin, D_L14_0015F580, size, 0x7FFFFFFF);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->frame = (emitter_moby->draw_dist >> 5 << 4) + exhaust_ftoi(4.0f);
            puff->life = life;
            puff->shade = random_integer_below(0xFF);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
        spin = -spin;
        life *= 2;
        size -= 20000.0f;
    }
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002FDD18), where it is exact; names translated to the US level program. */

void initialize_flight_path_state(struct Moby *moby) {
    char  *data = (char *)moby->pvars;
    float *path = (float *)l14_paths[*(int *)(data + 0x68)];
    int    i;
    for (i = 0; i < *(int *)path - 1; i++) {
        path[i * 4 + 7] = fast_vec_dist(&path[i * 4 + 4], &path[i * 4 + 8]);
    }
    *(int *)(data + 0x60) = 0;
    *(int *)(data + 0x64) = 0;
    qcopy(&moby->pos, path + 4);
    moby->rot.z = fast_arc_tan(path[8] - path[4], path[9] - path[5]);
    *(float *)(data + 0x80) = *(float *)(data + 0x84) = *(float *)(data + 0x88) =
        *(float *)(data + 0x70) = *(float *)(data + 0x74) = *(float *)(data + 0x78) = 0.0f;
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002FDE28), where it is exact; names translated to the US level program. */

extern float D_0015ED6C_c2 __asm__("D_0015ED6C");
extern void  FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void
FUN_L00_0025d808_c(int a, char *pos, char *b, char *c, int d, float f) __asm__("FUN_L00_0025d808");
float absolute_float(float input) __asm__("func_001F99C0");

extern short enemy_fighter_path_step_rate_gp __asm__("D_L14_00161F48") __attribute__((sda));
extern short enemy_fighter_flight_spring_strength_gp __asm__("D_L14_00161F4C") __attribute__((sda));
extern short enemy_fighter_flight_spring_damping_gp __asm__("D_L14_00161F50") __attribute__((sda));
extern short enemy_fighter_bank_angle_scale_gp __asm__("D_L14_00161F54") __attribute__((sda));
extern short enemy_fighter_bank_yaw_error_scale_gp __asm__("D_L14_00161F58") __attribute__((sda));

void advance_enemy_fighter_along_flight_path(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    float path_target_position[3];
    float path_position_delta[3];
    float target_yaw, yaw_error_factor, path_vertical_angle;
    FUN_L00_0025d808_c((int)((char *)l14_paths[*(int *)(moby_vars + 0x68)]),
                       (char *)path_target_position,
                       moby_vars + 0x60,
                       moby_vars + 0x64,
                       1,
                       (*(float *)&enemy_fighter_path_step_rate_gp) * D_0015ED6C_c2);
    FUN_L00_0025b8c0(&moby->pos.x,
                     (float *)(moby_vars + 0x70),
                     path_target_position[0],
                     (*(float *)&enemy_fighter_flight_spring_strength_gp),
                     (*(float *)&enemy_fighter_flight_spring_damping_gp),
                     0.0f);
    FUN_L00_0025b8c0(&moby->pos.y,
                     (float *)(moby_vars + 0x74),
                     path_target_position[1],
                     (*(float *)&enemy_fighter_flight_spring_strength_gp),
                     (*(float *)&enemy_fighter_flight_spring_damping_gp),
                     0.0f);
    FUN_L00_0025b8c0(&moby->pos.z,
                     (float *)(moby_vars + 0x78),
                     path_target_position[2],
                     (*(float *)&enemy_fighter_flight_spring_strength_gp),
                     (*(float *)&enemy_fighter_flight_spring_damping_gp),
                     0.0f);
    fast_vec_sub(path_position_delta, path_target_position, &moby->pos);
    if (absolute_float(path_position_delta[0]) > 0.01f) {
        if (absolute_float(path_position_delta[1]) > 0.01f) {
            target_yaw = fast_arc_tan(path_position_delta[0], path_position_delta[1]);
            yaw_error_factor = fast_subtract_rotations(moby->rot.z, target_yaw);
            FUN_L00_00258278(moby,
                             (float *)(moby_vars + 0x88),
                             target_yaw,
                             (*(float *)&enemy_fighter_flight_spring_strength_gp),
                             (*(float *)&enemy_fighter_flight_spring_damping_gp),
                             0.0f);
            path_vertical_angle =
                fast_arc_tan(vector_length_xy(path_position_delta), path_position_delta[2]);
            FUN_L00_0025b8c0(&moby->rot.y,
                             (float *)(moby_vars + 0x84),
                             -path_vertical_angle,
                             (*(float *)&enemy_fighter_flight_spring_strength_gp),
                             (*(float *)&enemy_fighter_flight_spring_damping_gp),
                             0.0f);
            yaw_error_factor =
                yaw_error_factor / (*(float *)&enemy_fighter_bank_yaw_error_scale_gp);
            if (yaw_error_factor > 1.0f)
                yaw_error_factor = 1.0f;
            if (yaw_error_factor < -1.0f)
                yaw_error_factor = -1.0f;
            FUN_L00_0025b8c0(&moby->rot.x,
                             (float *)(moby_vars + 0x80),
                             yaw_error_factor * (*(float *)&enemy_fighter_bank_angle_scale_gp),
                             0.003f,
                             (*(float *)&enemy_fighter_flight_spring_damping_gp),
                             0.0f);
        }
    }
}

/* Camera target of the level (D_L14_00167380): position and view angles. */
struct L14ViewTarget {
    u8  pad0[0x140];
    f32 pos[3]; /* 0x140 */
    u8  pad14C[8];
    f32 pitch; /* 0x154 */
    f32 yaw;   /* 0x158 */
};

extern struct L14ViewTarget D_L14_00167380_v __asm__("D_L14_00167380");
extern struct Moby         *D_L14_0015FFD8_t __asm__("D_L14_0015FFD8");

struct L14TramLevel {
    u8 pad0[0x454];
    u8 collected[1];
};

extern struct L14TramLevel D_L14_001BB930_t __asm__("D_L14_001BB930");
extern unsigned char       D_0013D408_t[] __asm__("D_0013D408");
extern char enemy_fighter_921_missing_flight_path_error_format[] __asm__("D_L14_001FCE98");
extern char enemy_fighter_921_empty_flight_path_error_format[] __asm__("D_L14_001FCED0");
extern s32 enemy_fighter_921_respawn_delay_frames_gp __asm__("D_L14_00161F8C") __attribute__((sda));
extern void mark_moby_and_linked_moby_for_removal(char *moby) __asm__("FUN_L14_002fe238");
extern void set_moby_state_two(unsigned char *moby) __asm__("FUN_L06_00303150");
extern void FUN_L00_0025f090(void *, void *, int, float, float);
void        spawn_moby_exhaust_puffs(struct Moby *m) __asm__("FUN_L14_002fd310");
int         advance_moby_along_flight_path(struct Moby *moby) __asm__("FUN_L14_002fd918");

int advance_moby_along_flight_path(struct Moby *m) __asm__("FUN_L14_002fd918");

/* pvars of the tram (FUN_L14_002fcbb0 and its helpers fc890/fd918 read the same layout) */
typedef struct {
    u8  pad0[0x60];
    s32 segment; /* 0x60 */
    s32 along;   /* 0x64 */
    s32 path;    /* 0x68 */
    s32 sound;   /* 0x6C */
    f32 f70;     /* 0x70 */
    f32 f74;     /* 0x74 */
    f32 f78;     /* 0x78 */
    s32 timer;   /* 0x7C */
    f32 f80;     /* 0x80 */
    f32 f84;     /* 0x84 */
    f32 f88;     /* 0x88 */
    s32 carrier; /* 0x8C */
    s32 i90;     /* 0x90 */
    s32 stage;   /* 0x94 */
    s32 item[3]; /* 0x98 */
    s32 clip;    /* 0xA4 */
} L14TramVars;

/* Tram update: carries its rider moby, unlocks once the three items are bought and the camera looks at it. */
void update_enemy_fighter_921(struct Moby *moby) __asm__("FUN_L14_002fcbb0");

void update_enemy_fighter_921(struct Moby *moby) {
    enum {
        TRAM_STATE_INITIALIZE = 0,
        TRAM_STATE_WAIT_FOR_UNLOCK = 1,
        TRAM_STATE_FOLLOW_PATH = 2,
        TRAM_STATE_WAIT_FOR_RESPAWN = 3
    };
    L14TramVars *moby_vars = (L14TramVars *)moby->pvars;
    Vec4         rider_view_target_or_carrier_offset;
    Vec4         carrier_position;

    if (moby->state == 0) {
        u16 id = moby->save_id;
        if (D_L14_001BB930_t.collected[(short)id] != 0 ||
            (D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 0x1F)) & 1) {
            mark_moby_and_linked_moby_for_removal((char *)&D_L14_0015FFD8_t[moby_vars->carrier]);
            mark_moby_for_removal(moby);
            return;
        }
    }
    {
        void *hit = find_moby_hit(moby, 0x800000, 0);
        moby->hit_slot = 0xFF;
        if (hit != 0 && moby_vars->carrier == -1 && moby->state != 3) {
            if (current_level_index == 14) {
                enemy_fighter_destruction_count = enemy_fighter_destruction_count + 1;
                if (enemy_fighter_destruction_count >= 3 && D_0013D408_t[0x18] == 0) {
                    D_0013D408_t[0x18] = 1;
                    allocate_voice_for_bank_entry(1, 0, 0);
                    FUN_L00_00263d40(0x53D6, -1);
                }
            }
            moby->state = 3;
            FUN_L00_0025f090(moby, &moby->pos, -1, 5.0f, 13.0f);
            moby->collision = 0;
            moby->flags |= 0x41;
            if (moby_vars->sound != -1) {
                char *voice_entry = ((char *)&voice_pool) + moby_vars->sound * 0x70;
                if (*(struct Moby **)(voice_entry + 0x88) == moby && ((u8 *)voice_entry)[0x74] != 0)
                    release_voice_slot(moby_vars->sound);
            }
            moby_vars->sound = -1;
        }
    }
    switch (moby->state) {
    case TRAM_STATE_INITIALIZE:
        if (moby_vars->path == -1) {
            debug_print(enemy_fighter_921_missing_flight_path_error_format, moby->save_id);
            mark_moby_for_removal(moby);
            return;
        }
        if (*(int *)l14_paths[moby_vars->path] == 0) {
            debug_print(enemy_fighter_921_empty_flight_path_error_format, moby->save_id);
            mark_moby_for_removal(moby);
            return;
        }
        if (moby_vars->item[0] < 0 || moby_vars->item[1] < 0 || moby_vars->item[2] < 0) {
            mark_moby_for_removal(moby);
            return;
        }
        initialize_flight_path_state(moby);
        if (moby_vars->carrier != 0) {
            struct Moby *c;
            moby->state = 1;
            moby->update_dist = 0xFF;
            moby->flags |= 0x41;
            moby->collision = 0;
            c = &D_L14_0015FFD8_t[moby_vars->carrier];
            c->flags |= 0x41;
            moby_vars->stage = 0;
        } else {
            moby->state = 2;
            moby->update_dist = 0x80;
        }
        moby->draw_dist = 0x80;
        moby_vars->sound = -1;
        moby_vars->timer = scale_game_frames(enemy_fighter_921_respawn_delay_frames_gp);
        break;
    case TRAM_STATE_WAIT_FOR_UNLOCK: {
        int lvl = current_level_index * 16;
        u8  first_item_status = visited52[moby_vars->item[0] + lvl];
        if (first_item_status == 0xFF) {
            u8 second_item_status = visited52[moby_vars->item[1] + lvl];
            if (second_item_status == first_item_status) {
                u8 x = visited52[moby_vars->item[2] + lvl];
                if (x == second_item_status && hero.frames_off_ground.s == 0 &&
                    hero.ground_moby == 0 &&
                    is_point_inside_clip_volume(&hero.motion.pos, moby_vars->clip) != 0) {
                    struct Moby *carrier = &D_L14_0015FFD8_t[moby_vars->carrier];
                    struct Moby *rider = &D_L14_0015FFD8_t[((int *)carrier->pvars)[1]];
                    float        pitch;
                    qcopy(&rider_view_target_or_carrier_offset, &rider->pos);
                    rider_view_target_or_carrier_offset.f[2] += 4.0f;
                    pitch = fast_difference_between_rotations(
                        D_L14_00167380_v.pitch,
                        -fast_arc_tan(vector_distance_xy(D_L14_00167380_v.pos,
                                                         &rider_view_target_or_carrier_offset),
                                      rider_view_target_or_carrier_offset.f[2] -
                                          D_L14_00167380_v.pos[2]));
                    if (fast_difference_between_rotations(
                            D_L14_00167380_v.yaw,
                            fast_arc_tan(rider->pos.x - D_L14_00167380_v.pos[0],
                                         rider->pos.y - D_L14_00167380_v.pos[1])) < 0.4886922f &&
                        pitch < 0.34906584f &&
                        collision_line(D_L14_00167380_v.pos,
                                       &rider_view_target_or_carrier_offset,
                                       0x12,
                                       rider,
                                       0) == 0) {
                        D_0014C190_257218[current_level_index][(short)moby->save_id >> 5] |=
                            1 << (moby->save_id & 0x1F);
                        D_L14_001BABD0[(short)moby->save_id >> 5] |= 1 << (moby->save_id & 0x1F);
                        moby->flags &= 0xFFBE;
                        moby->collision = moby->pclass->collision;
                        carrier->flags &= 0xFFBE;
                        moby->state = 2;
                        activate_descriptor(moby_vars->i90);
                        rider->draw_dist = 0xFF;
                        rider->update_dist = 0xFF;
                    }
                }
            }
        }
        break;
    }
    case TRAM_STATE_FOLLOW_PATH:
        if (advance_moby_along_flight_path(moby) != 0)
            return;
        if (moby_vars->carrier != 0 && moby_vars->stage < 2) {
            struct Moby *c;
            if (moby_vars->stage == 0 && moby_vars->segment >= 0x38)
                moby_vars->stage = 1;
            fast_vec_scale(&carrier_position, &moby->rot_row0, 0.21f);
            fast_vec_scale(&rider_view_target_or_carrier_offset, &moby->rot_row2, -1.24f);
            fast_vec_add(
                &carrier_position, &carrier_position, &rider_view_target_or_carrier_offset);
            fast_vec_add(&carrier_position, &moby->pos, &carrier_position);
            c = &D_L14_0015FFD8_t[moby_vars->carrier];
            qcopy(&c->pos, &carrier_position);
            qcopy(&c->rot, &moby->rot);
            if (moby_vars->stage == 1) {
                set_moby_state_two((unsigned char *)c);
                moby_vars->stage = 2;
            }
        }
        break;
    case TRAM_STATE_WAIT_FOR_RESPAWN:
        if (fast_dec_timer(&moby_vars->timer) != 0) {
            char *path = ((char *)l14_paths[moby_vars->path]);
            moby->flags &= 0xFFBE;
            moby->collision = moby->pclass->collision;
            moby_vars->timer = scale_game_frames(enemy_fighter_921_respawn_delay_frames_gp);
            moby->state = 2;
            moby_vars->segment = 0;
            moby_vars->along = 0;
            qcopy(&moby->pos, path + 0x10);
            moby->rot.z = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                       *(float *)(path + 0x24) - *(float *)(path + 0x14));
            moby_vars->f80 = moby_vars->f84 = moby_vars->f88 = moby_vars->f70 = moby_vars->f74 =
                moby_vars->f78 = 0.0f;
        }
        break;
    }
    spawn_moby_exhaust_puffs(moby);
    if (moby->state != 0 && moby->state != 3 && moby->state != 1 &&
        voice_is_playing(moby, moby_vars->sound) == 0)
        moby_vars->sound = allocate_voice_for_target_entry(0, 4, moby);
}

extern f32 moby_exhaust_base_row0_offset_gp __asm__("D_L14_00161F60") __attribute__((sda));
extern f32 moby_exhaust_paired_emitter_offset_gp __asm__("D_L14_00161F64") __attribute__((sda));
extern f32 moby_exhaust_base_row2_offset_gp __asm__("D_L14_00161F68") __attribute__((sda));
extern f32 moby_exhaust_ring_row0_offset_gp __asm__("D_L14_00161F6C") __attribute__((sda));
extern f32 moby_exhaust_grey_puff_row0_offset_gp __asm__("D_L14_00161F70") __attribute__((sda));

#include "rnc/overlay/l14/mobys/jet_data/spawn_moby_exhaust_puffs.h"

/* Exhaust of the second engine moby: puffs and rings on both sides plus a grey puff in the middle. */
void spawn_moby_exhaust_puffs(struct Moby *moby) {
    Vec4     step;
    Vec4     pos;
    Vec4     base;
    L14Puff *puff;
    int      i;
    int      j;
    int      spin;
    int      life;
    int      grey;
    float    size;

    if (moby->state == 0 || moby->state == 3 || moby->state == 1)
        return;
    fast_vec_scale(&step, &moby->rot_row0, moby_exhaust_base_row0_offset_gp);
    fast_vec_add(&pos, &moby->pos, &step);
    fast_vec_scale(&step, &moby->rot_row2, moby_exhaust_base_row2_offset_gp);
    fast_vec_add(&pos, &pos, &step);
    qcopy(&base, &pos);
    fast_vec_scale(&step, &moby->rot_row1, moby_exhaust_paired_emitter_offset_gp);
    fast_vec_add(&pos, &pos, &step);
    for (j = 0; j < 2; j++) {
        spin = random_integer_below(16);
        puff = spawn_puff(&pos,
                          0.2f,
                          1.0f,
                          0.9f,
                          random_integer_below(2) == 0 ? spin : -spin,
                          D_L14_0015F580,
                          200000.0f,
                          moby_exhaust_paired_puff_color_gp);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->life = scale_game_frames(12);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
    }
    size = 100000.0f;
    fast_vec_scale(&step, &moby->rot_row0, moby_exhaust_ring_row0_offset_gp);
    fast_vec_add(&pos, &pos, &step);
    spin = 16;
    life = scale_game_frames(2);
    for (i = 0; i < 3; i++) {
        puff = spawn_puff(&pos, 0.05f, 1.0f, 1.0f, spin, D_L14_0015F580, size, 0x7FFFFFFF);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->life = life;
            puff->shade = random_integer_below(0xFF);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
        spin = -spin;
        life *= 2;
        size -= 20000.0f;
    }
    size = 300000.0f;
    fast_vec_scale(&step, &moby->rot_row0, moby_exhaust_grey_puff_row0_offset_gp);
    fast_vec_add(&pos, &base, &step);
    grey = FUN_L00_00257b90(0x40, 0x80);
    grey = 0x7F000000 | grey << 16 | grey << 8 | grey;
    if (random_integer_below(3) != 0) {
        L14Puff *grey_puff;
        spin = random_integer_below(4);
        grey_puff = spawn_puff(&pos,
                               0.05f,
                               1.0f,
                               1.0f,
                               random_integer_below(2) == 0 ? spin : -spin,
                               D_L14_0015F580,
                               size,
                               grey);
        if (grey_puff != 0) {
            L14PuffTail *tail = &grey_puff->tail;
            grey_puff->life = scale_game_frames(60);
            if (random_integer_below(2) != 0)
                grey_puff->alpha = 0x44;
            tail->tint = 0x40;
            tail->mode = 2;
            tail->fade = scale_game_frames(60);
        }
    }
    fast_vec_scale(&step, &moby->rot_row1, moby_exhaust_paired_emitter_offset_gp);
    fast_vec_sub(&pos, &base, &step);
    for (j = 0; j < 2; j++) {
        spin = random_integer_below(16);
        puff = spawn_puff(&pos,
                          0.2f,
                          1.0f,
                          0.9f,
                          random_integer_below(2) == 0 ? spin : -spin,
                          D_L14_0015F580,
                          200000.0f,
                          moby_exhaust_paired_puff_color_gp);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->life = scale_game_frames(12);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
    }
    size = 100000.0f;
    fast_vec_scale(&step, &moby->rot_row0, moby_exhaust_ring_row0_offset_gp);
    fast_vec_add(&pos, &pos, &step);
    spin = 16;
    life = scale_game_frames(2);
    for (i = 0; i < 3; i++) {
        puff = spawn_puff(&pos, 0.05f, 1.0f, 1.0f, spin, D_L14_0015F580, size, 0x7FFFFFFF);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->life = life;
            puff->shade = random_integer_below(0xFF);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
        spin = -spin;
        life *= 2;
        size -= 20000.0f;
    }
}

/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002FEDA0), where it is exact; names translated to the US level program. */

extern u8 *D_L14_00167500 NOT_SDA;
extern float              D_0015ED6C_f2 __asm__("D_0015ED6C");
extern float              vector_length_xy(void *);
extern float flight_path_spring_strength_gp __asm__("D_L14_00161F7C") __attribute__((sda));
extern float flight_path_spring_damping_gp __asm__("D_L14_00161F80") __attribute__((sda));
extern float D_L14_00161F84 __attribute__((sda));
extern float flight_path_bank_yaw_error_scale_gp __asm__("D_L14_00161F88") __attribute__((sda));
extern float flight_path_step_rate_gp __asm__("D_L14_00161F78") __attribute__((sda));
extern void  flag_vendor_item(int) __asm__("FUN_L13_00316f48");
extern int
FUN_L00_0025d808_2f05d8(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");

int advance_moby_along_flight_path(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    float path_target_position[4];
    float path_position_delta[4];
    float yaw;
    float yaw_error_factor;
    int   path_boundary_reached;
    path_boundary_reached = FUN_L00_0025d808_2f05d8(((int *)l14_paths[*(int *)(moby_vars + 0x68)]),
                                                    path_target_position,
                                                    moby_vars + 0x60,
                                                    moby_vars + 0x64,
                                                    (unsigned int)*(int *)(moby_vars + 0x8C) >> 31,
                                                    flight_path_step_rate_gp * D_0015ED6C_f2);
    if (*(int *)(moby_vars + 0x8C) != 0 &&
        (path_boundary_reached != 0 || *(short *)(D_L14_00167500 + 0x86) != 0x13)) {
        flag_vendor_item(*(int *)(moby_vars + 0x90));
        if (*(int *)(moby_vars + 0x6C) != -1) {
            char *voice_entry = ((char *)&voice_pool) + *(int *)(moby_vars + 0x6C) * 0x70;
            if (*(char **)(voice_entry + 0x88) == moby &&
                ((unsigned char *)voice_entry)[0x74] != 0) {
                release_voice_slot(*(int *)(moby_vars + 0x6C));
            }
        }
        *(int *)(moby_vars + 0x6C) = -1;
        mark_moby_for_removal(moby);
        return 1;
    }
    FUN_L00_0025b8c0(&moby->pos.x,
                     (float *)(moby_vars + 0x70),
                     path_target_position[0],
                     flight_path_spring_strength_gp,
                     flight_path_spring_damping_gp,
                     0.0f);
    FUN_L00_0025b8c0(&moby->pos.y,
                     (float *)(moby_vars + 0x74),
                     path_target_position[1],
                     flight_path_spring_strength_gp,
                     flight_path_spring_damping_gp,
                     0.0f);
    FUN_L00_0025b8c0(&moby->pos.z,
                     (float *)(moby_vars + 0x78),
                     path_target_position[2],
                     flight_path_spring_strength_gp,
                     flight_path_spring_damping_gp,
                     0.0f);
    fast_vec_sub(path_position_delta, path_target_position, &moby->pos);
    if (0.01f < absolute_float(path_position_delta[0])) {
        if (0.01f < absolute_float(path_position_delta[1])) {
            yaw = fast_arc_tan(path_position_delta[0], path_position_delta[1]);
            yaw_error_factor = fast_subtract_rotations(moby->rot.z, yaw);
            FUN_L00_00258278(moby,
                             (float *)(moby_vars + 0x88),
                             yaw,
                             flight_path_spring_strength_gp,
                             flight_path_spring_damping_gp,
                             0.0f);
            yaw = fast_arc_tan(vector_length_xy(path_position_delta), path_position_delta[2]);
            FUN_L00_0025b8c0(&moby->rot.y,
                             (float *)(moby_vars + 0x84),
                             -yaw,
                             flight_path_spring_strength_gp,
                             flight_path_spring_damping_gp,
                             0.0f);
            yaw_error_factor = yaw_error_factor / flight_path_bank_yaw_error_scale_gp;
            if (1.0f < yaw_error_factor)
                yaw_error_factor = 1.0f;
            if (yaw_error_factor < -1.0f)
                yaw_error_factor = -1.0f;
            FUN_L00_0025b8c0(&moby->rot.x,
                             (float *)(moby_vars + 0x80),
                             yaw_error_factor * D_L14_00161F84,
                             0.003f,
                             flight_path_spring_damping_gp,
                             0.0f);
        }
    }
    return 0;
}

#include "rnc/overlay/quad.h"

/* pvars of the flyer rocket_update moves to a path point */
typedef struct {
    s32 point;  /* 0x0: path point index into l14_cuboids (0x80 each, position at 0x30) */
    s32 target; /* 0x4: moby index removed on arrival */
    s32 sound;  /* 0x8: voice slot, -1 when none */
    s32 timer;  /* 0xC */
} FlyerVars;

extern s32  rocket_initial_timer_frames_gp __asm__("D_L14_00161FB0") __attribute__((sda));
extern f32  rocket_travel_speed_gp __asm__("D_L14_00161FA8") __attribute__((sda));
extern f32  rocket_steering_blend_fraction_gp __asm__("D_L14_00161FAC") __attribute__((sda));
extern void spawn_explosion(void *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            int,
                            float,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
extern void spawn_moby_offset_puffs(struct Moby *) __asm__("FUN_L14_002fded0");

void rocket_update(struct Moby *moby) __asm__("FUN_L14_002fdbb8");

void rocket_update(struct Moby *moby) {
    enum {
        ROCKET_STATE_INITIALIZE = 0,
        ROCKET_STATE_IDLE = 1,
        ROCKET_STATE_FLY_TO_POINT = 2
    };
    FlyerVars   *vars = (FlyerVars *)moby->pvars;
    Vec4f        destination_delta;
    OvlVec4      step;
    f32          distance_to_destination;
    f32          speed;
    char        *slot;
    struct Moby *target;

    switch (moby->state) {
    case ROCKET_STATE_INITIALIZE:
        if (vars->point < 0 || vars->target < 0) {
            mark_moby_for_removal(moby);
            return;
        }
        moby->state = 1;
        moby->update_dist = 0xFF;
        moby->pos.z += 1.5f;
        vars->sound = -1;
        vars->timer = scale_game_frames(rocket_initial_timer_frames_gp);
        return;
    case ROCKET_STATE_IDLE:
        return;
    case ROCKET_STATE_FLY_TO_POINT:
        fast_vec_sub(
            &destination_delta, (char *)l14_cuboids + vars->point * 0x80 + 0x30, &moby->pos);
        distance_to_destination = fast_vec_length(&destination_delta);
        fast_dec_timer(&vars->timer);
        speed = rocket_travel_speed_gp * frame_time;
        if (distance_to_destination < speed || distance_to_destination <= 0.0f) {
            if (vars->sound != -1) {
                slot = ((char *)&voice_pool) + vars->sound * 0x70;
                if (*(struct Moby **)(slot + 0x88) == moby && ((u8 *)slot)[0x74] != 0) {
                    release_voice_slot(vars->sound);
                }
            }
            target = (struct Moby *)(D_L14_0015FFD8 + vars->target * 0x100);
            vars->sound = -1;
            if (target != 0) {
                mark_moby_for_removal(target);
            }
            qcopy(&moby->pos, (char *)l14_cuboids + vars->point * 0x80 + 0x30);
            qcopy(&step, &moby->rot_row0);
            step.f[2] = 1.0f;
            spawn_explosion(moby,
                            &step,
                            &moby->pos,
                            0.0f,
                            0.0f,
                            0x14,
                            0xA,
                            0xC,
                            10.0f,
                            6.0f,
                            9.0f,
                            0,
                            1.0f,
                            30.0f,
                            1,
                            0xA,
                            -1,
                            0);
            mark_moby_for_removal(moby);
            return;
        }
        moby->rot.x = fast_add_rotations(moby->rot.x, 2.0f * DEG_TO_RAD);
        moby->rot.y = interpolate_angle(
            moby->rot.y,
            -fast_arc_tan(vector_length_xy(&destination_delta), destination_delta.z),
            rocket_steering_blend_fraction_gp);
        moby->rot.z = interpolate_angle(moby->rot.z,
                                        fast_arc_tan(destination_delta.x, destination_delta.y),
                                        rocket_steering_blend_fraction_gp);
        fast_vec_scale(&step, &moby->rot_row0, speed);
        fast_vec_add((u8 *)&moby->pos, (u8 *)&moby->pos, (u8 *)&step);
        spawn_moby_offset_puffs(moby);
        if (voice_is_playing(moby, vars->sound) == 0) {
            vars->sound = allocate_voice_for_target_entry(1, 4, (int)moby);
        }
        return;
    }
}

s32 shuttle_enter_riding_transition_frames_gp __asm__("D_L14_00161B8C") = 0x3c;
s32 shuttle_exit_riding_transition_frames_gp __asm__("D_L14_00161B90") = 0x5a;

s32 D_L14_00162238 = 0;

s32 wind_push_decay_frames_gp __asm__("D_L14_00161C04") = 0x1e;

int wind_spark_peak_alpha_gp __asm__("D_L14_00161C10") = 0x32;

short lightning_arc_interval_frames_gp __asm__("D_L14_00161C54") = 0xb4;

s32 lightning_arc_lifetime_range_start_gp __asm__("D_L14_00161C98") = 0x38;

u8 lightning_arc_alpha_fade __asm__("D_L14_00161CE0") = 0;

char *lightning_arc_owner_moby __asm__("D_L14_00161CE8") = 0;
