/* Level 12 moby class oscillating_target_link (class 1259): the oscillating link strips drawn from it toward its target. */
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"

#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/rendering/clip_volume.h"

/* Hoven vendor draw: builds the two strip matrices, then draws the four rings as quads lit by their phase. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00303CA8), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef struct Quad {
    float v[16];
    int   c[4];
    float uv[8];
    long  g[4];
} Quad;

typedef struct Data_303CA8 {
    float ring[8][70][4];
    int   phase[4]; /* 0x2300 */
    int   row;      /* 0x2310 */
    int   target;   /* 0x2314 */
    float angle;    /* 0x2318 */
    int   f231C;
    int   f2320;
    float dist;  /* 0x2324 */
    float turn;  /* 0x2328 */
    int   count; /* 0x232C */
    int   f2330;
} Data_303CA8;

extern Moby *D_L12_0015FFD8;
extern float oscillating_link_strip_quad_vertex_offsets[][4] __asm__("D_L12_001FB530");
extern float oscillating_link_strip_transform_rotations[][4] __asm__("D_L12_001FB5B0");

#include "rnc/overlay/l12/mobys/oscillating_target_link_data/draw_oscillating_target_link_strips.h"
#include "rnc/overlay/l12/mobys/oscillating_target_link_globals.h"
#include "rnc/overlay/level_path.h"

extern float oscillating_link_strip_segment_u_step_gp __asm__("D_L12_00161DF4")
    __attribute__((sda));
extern float oscillating_link_strip_scroll_phase_gp __asm__("D_L12_00161DF8") __attribute__((sda));
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s64   get_effect_texture(s32) __asm__("func_001F44B8");
extern void  draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
extern void  func_L00_001FD1D8_303CA8(void *, void *, int) __asm__("FUN_001f7d30");
extern float D_L12_001FB7D0_303CA8[] __asm__("D_L12_001FB510");

void draw_oscillating_target_link_strips(Moby *moby) __asm__("FUN_L12_003028c8");

void draw_oscillating_target_link_strips(Moby *moby) {
    Quad         quad[2];
    float        rotation_matrix[16];
    float        matrices[5][16];
    float        anchor[4];
    Data_303CA8 *target_link_strip_state;
    float       *pos;
    float       *translation;
    float        fade;
    float        scroll;
    int          i, segment, uv_index, vertex, pass, ring;

    target_link_strip_state = *(Data_303CA8 **)&moby->pvars;
    euler_rotation_matrix(rotation_matrix, ((float *)&moby->rot));
    pos = ((float *)&moby->pos);
    translation = &matrices[0][12];
    for (i = 0; i < 2; i++) {
        float *matrix = matrices[i];

        euler_rotation_matrix(matrix, oscillating_link_strip_transform_rotations[i]);
        multiply_matrix(matrix, rotation_matrix, matrix);
        if (!(i & 1) && target_link_strip_state->target != -1) {
            qcopy(anchor, ((float *)&D_L12_0015FFD8[target_link_strip_state->target].pos));
        } else {
            qcopy(anchor, pos);
        }
        fast_vec_add(translation, translation, anchor);
        translation += 16;
        matrix[15] = 1.0f;
    }
    quad[1].g[1] = get_effect_texture(0xE);
    quad[1].g[3] = (long)oscillating_link_strip_blend_minuend_color_selector |
                   ((long)oscillating_link_strip_blend_subtrahend_color_selector << 2) |
                   ((long)oscillating_link_strip_blend_alpha_factor_selector << 4) |
                   ((long)oscillating_link_strip_blend_addend_color_selector << 6) |
                   (0x8000L << 24);
    quad[1].g[2] = 0xFF9000000260UL;
    quad[1].g[0] = 0;
    {
        float *uv = &quad[1].uv[1];
        int    corner;

        for (corner = 0; corner < 4; corner++) {
            *uv = D_L12_001FB7D0_303CA8[corner * 2 + 1];
            uv += 2;
        }
    }
    for (ring = 0; ring < 4; ring++) {
        if (oscillating_link_strip_fade_pivot_ticks < target_link_strip_state->phase[ring]) {
            fade = convert_integer_to_float(target_link_strip_state->phase[ring] -
                                            oscillating_link_strip_fade_pivot_ticks) /
                   convert_integer_to_float(oscillating_link_strip_reset_ticks -
                                            oscillating_link_strip_fade_pivot_ticks);
        } else {
            fade = 1.0f - convert_integer_to_float(target_link_strip_state->phase[ring]) /
                              convert_integer_to_float(oscillating_link_strip_fade_pivot_ticks);
        }
        quad[1].c[0] = quad[1].c[1] = quad[1].c[2] = quad[1].c[3] =
            fast_tween_color(oscillating_link_strip_fade_source_color,
                             oscillating_link_strip_fade_target_color,
                             fade);
        scroll = 0.0f;
        for (segment = 0; segment < target_link_strip_state->count - 1; segment++) {
            scroll = scroll + oscillating_link_strip_segment_u_step_gp;
            if (1.0f < scroll) {
                scroll = scroll - 1.0f;
            }
            for (uv_index = 0; uv_index < 4; uv_index++) {
                quad[1].uv[uv_index * 2] = D_L12_001FB7D0_303CA8[uv_index * 2] + scroll +
                                           oscillating_link_strip_scroll_phase_gp;
            }
            for (vertex = 0; vertex < 4; vertex++) {
                fast_vec_add(&quad[1].v[vertex * 4],
                             oscillating_link_strip_quad_vertex_offsets[vertex],
                             target_link_strip_state->ring[ring][segment + (vertex >> 1)]);
            }
            for (pass = 0; pass < 2; pass++) {
                func_L00_001FD1D8_303CA8(&quad[1], matrices[pass], 0);
            }
        }
    }
}

extern void  random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern void  add_draw_callback(void *, void *) __asm__("FUN_001f4600");
extern float oscillating_link_strip_scroll_rate_gp __asm__("D_L12_00161DFC") __attribute__((sda));
extern float oscillating_link_strip_random_delta_magnitude_rate_gp __asm__("D_L12_00161E00")
    __attribute__((sda));
extern void draw_oscillating_target_link_strips(struct Moby *) __asm__("FUN_L12_003028c8");

void update_oscillating_target_link_strips(struct Moby *moby) __asm__("FUN_L12_00302c58");

void update_oscillating_target_link_strips(struct Moby *moby) {
    char *oscillating_link_strip_state = (char *)moby->pvars;
    float cycle =
        oscillating_link_strip_scroll_phase_gp + oscillating_link_strip_scroll_rate_gp * frame_time;
    int strip;
    oscillating_link_strip_scroll_phase_gp = cycle;
    if (cycle > 1.0f)
        oscillating_link_strip_scroll_phase_gp = cycle - 1.0f;
    for (strip = 0; strip < 4; strip++) {
        int    count;
        int    i;
        float *points;
        float *intermediate;
        if (fast_dec_timer((int *)(oscillating_link_strip_state + 0x2300 + strip * 4))) {
            int   vector_count = *(int *)(oscillating_link_strip_state + 0x232c);
            float intermediate_storage[vector_count][4];
            char *timers = oscillating_link_strip_state + 0x2300;
            intermediate = &intermediate_storage[0][0];
            i = 0;
            if (0 < *(int *)(oscillating_link_strip_state + 0x232c)) {
                points = (float *)(oscillating_link_strip_state + strip * 0x460);
                do {
                    float vector_count_float;
                    float length;
                    float index;
                    float rate;
                    clear_u64_value(points);
                    vector_count_float = (float)*(int *)(oscillating_link_strip_state + 0x232c);
                    length = *(float *)(oscillating_link_strip_state + 0x2324);
                    index = (float)i;
                    rate = oscillating_link_strip_random_delta_magnitude_rate_gp * frame_time;
                    points[3] = 1.0f;
                    points[0] = index * (length / vector_count_float);
                    random_vector(intermediate + i * 4, 0.0f, rate);
                    points += 4;
                    i++;
                } while (i < *(int *)(oscillating_link_strip_state + 0x232c));
            }
            count = 1;
            if (count < *(int *)(oscillating_link_strip_state + 0x232c) - 1) {
                points = (float *)(oscillating_link_strip_state + strip * 0x460 + 0x1190);
                do {
                    fast_vec_add(
                        points, intermediate_storage[count - 1], intermediate_storage[count]);
                    fast_vec_add(points, points, intermediate_storage[count + 1]);
                    fast_vec_scale(points, points, 0.333f);
                    count++;
                    points += 4;
                } while (count < *(int *)(oscillating_link_strip_state + 0x232c) - 1);
            }
            *(int *)(timers + (strip << 2)) = oscillating_link_strip_reset_ticks;
        } else {
            for (i = 1; i < *(int *)(oscillating_link_strip_state + 0x232c) - 1; i++) {
                float *point = (float *)(oscillating_link_strip_state + strip * 0x460 + i * 16);
                float *delta =
                    (float *)(oscillating_link_strip_state + strip * 0x460 + 0x1190 + i * 16 - 16);
                fast_vec_add(point, point, delta);
            }
        }
    }
    if (fast_vec_dist(&moby->pos, D_L12_00167240) < 48.0f)
        add_draw_callback(draw_oscillating_target_link_strips, moby);
}

/* Level 12 vendor moby update: steers toward its target, then calls the vendor helpers. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00304310), where it is exact; names translated to the US level program. */

typedef struct Data_304310 {
    char  pad0[0x2300];
    int   f2300;
    int   f2304;
    int   f2308;
    int   f230C;
    int   row;    /* 0x2310 */
    int   target; /* 0x2314 */
    float angle;  /* 0x2318 */
    int   f231C;
    int   f2320;
    float dist; /* 0x2324 */
    float turn; /* 0x2328 */
    int   f232C;
    int   f2330;
} Data_304310;

typedef struct Row {
    u128 v0;
    u128 v10;
    u128 v20;
} Row;

extern char  D_L12_00167240[];
extern float D_0015ED6C;
extern float D_L12_0015F580[] MACRO_ADDR;
extern int   D_0013F3D0; /* no foreign declaration */
extern int   collision_line() __asm__("FUN_001efa68");
extern int   FUN_L00_00259888();
void         interpolate_vector(f32 t, f32 *out, f32 *a, f32 *b) __asm__("FUN_L00_0025b6b8");
extern void  func_L00_0025C710_304310(void *, void *, void *, float) __asm__("FUN_L00_0025b6b8");
extern void  func_L12_00304038_304310(void *) __asm__("FUN_L12_00302c58");
extern void  make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern float D_0013F4D0_304310[] __asm__("D_0013F3D0");

void oscillating_target_link_update(Moby *moby) __asm__("FUN_L12_00302f30");

/* Moby class 1259 */
void oscillating_target_link_update(Moby *moby) {
    enum {
        TARGET_TRACKING_STATE_INITIALIZE = 0,
        TARGET_TRACKING_STATE_ACTIVE = 1
    };
    Vec4         start;
    Vec4         end;
    char         hit[0x30];
    Data_304310 *oscillating_target_link_state = *(Data_304310 **)&moby->pvars;
    Row         *row;
    float        swing;
    float        half;
    int          target;

    if (oscillating_target_link_state->target == -1) {
        return;
    }
    switch (moby->state) {
    case TARGET_TRACKING_STATE_INITIALIZE:
        oscillating_link_strip_fade_pivot_ticks = 0x16;
        oscillating_link_strip_reset_ticks = 0x2D;
        oscillating_target_link_state->f231C = -1;
        oscillating_target_link_state->f2304 = 0xB;
        oscillating_target_link_state->f2308 = 0x17;
        oscillating_target_link_state->f230C = 0x22;
        oscillating_target_link_state->f2300 = 0;
        moby->state = 1;
        break;
    case TARGET_TRACKING_STATE_ACTIVE:
        moby->rot.z =
            fast_arc_tan(D_L12_0015FFD8[oscillating_target_link_state->target].pos.x - moby->pos.x,
                         D_L12_0015FFD8[oscillating_target_link_state->target].pos.y - moby->pos.y);
        oscillating_target_link_state->dist = fast_vec_dist(
            ((float *)&moby->pos),
            (char *)D_L12_0015FFD8 + (oscillating_target_link_state->target << 8) + 0x10);
        oscillating_target_link_state->angle =
            fast_add_rotations(oscillating_target_link_state->angle,
                               oscillating_target_link_state->turn * 0.017453292f * D_0015ED6C);
        row = ((Row *)l12_paths[oscillating_target_link_state->row]);
        swing = fast_sin(oscillating_target_link_state->angle);
        if (1.5707964f < oscillating_target_link_state->angle &&
            oscillating_target_link_state->angle < 3.1415927f) {
            swing = 2.0f - swing;
        }
        if (-3.1415927f < oscillating_target_link_state->angle &&
            oscillating_target_link_state->angle < -1.5707964f) {
            swing = 2.0f - swing;
        }
        half = 0.5f;
        swing = swing * half;
        swing = swing + half;
        if (1.0f < swing) {
            swing = swing - 2.0f;
        }
        swing = absolute_float(swing);
        start.q = row->v10;
        end.q = row->v20;
        func_L00_0025C710_304310(((float *)&moby->pos), &start, &end, swing);
        if (vector_distance_xy(((float *)&moby->pos), D_0013F4D0_304310) > 64.0f) {
            return;
        }
        if (moby->visible == 0 &&
            D_L12_0015FFD8[oscillating_target_link_state->target].visible == 0) {
            fast_vec_add(&start,
                         ((float *)&moby->pos),
                         ((float *)&D_L12_0015FFD8[oscillating_target_link_state->target].pos));
            fast_vec_scale(&start, &start, half);
            start.f[3] = 16.0f;
            if (fast_bsphere_check(&start, 32.0f) == -1) {
                return;
            }
        }
        if (oscillating_target_link_state->f2330 != -1 &&
            is_point_inside_clip_volume(D_L12_00167240, oscillating_target_link_state->f2330) !=
                0) {
            return;
        }
        if (oscillating_target_link_state->f2320 == 0) {
            return;
        }
        func_L12_00304038_304310(moby);
        target = oscillating_target_link_state->target;
        if (target == -1) {
            return;
        }
        qcopy(&start, &moby->pos.z);
        qcopy(&end, ((float *)&D_L12_0015FFD8[target].pos));
        make_hit_descriptor(hit, moby, 0x10001, 1.0f, D_L12_0015F580);
        collision_line(&start, &end, 0, 0, hit);
        break;
    }
}
