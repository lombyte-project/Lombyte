/* Level 12 moby classes: hoven_turret, mine, planetbuster_maximus_part, homing_mine_spawner. */
#include "rnc/hero/hero.h"
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/overlay/entities.h"

#include "rnc/math/vector.h"

#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_pool.h"
#include "sda.h"
#include "rnc/input/pad_globals.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/camera/follow_camera_update.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/vendor/set_camera_rotation.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/rendering/packets/emit_rgba_draw_packet.h"

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

#include "rnc/overlay/l12/mobys/hoven_turret_data/draw_oscillating_target_link_strips.h"
#include "rnc/overlay/l12/mobys/hoven_turret_globals.h"
#include "rnc/overlay/level_path.h"

extern float oscillating_link_strip_segment_u_step_gp __asm__("D_L12_00161DF4")
    __attribute__((sda));
extern float oscillating_link_strip_scroll_phase_gp __asm__("D_L12_00161DF8") __attribute__((sda));
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s64   get_effect_texture(s32) __asm__("func_001F44B8");
extern void  draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
extern int   func_001F4868_303CA8(int) __asm__("FUN_001f44b8");
extern void  func_L00_001FD1D8_303CA8(void *, void *, int) __asm__("FUN_001f7d30");
extern float D_L12_001FB7D0_303CA8[] __asm__("D_L12_001FB510");

void draw_oscillating_target_link_strips(Moby *moby) __asm__("FUN_L12_003028c8");

void draw_oscillating_target_link_strips(Moby *moby) {
    Quad         quad[2];
    float        rotation_matrix[16];
    float        matrices[5][16];
    float        anchor[4];
    Data_303CA8 *data;
    float       *pos;
    float       *translation;
    float        fade;
    float        scroll;
    int          i, segment, uv_index, vertex, pass, ring;

    data = *(Data_303CA8 **)&moby->pvars;
    euler_rotation_matrix(rotation_matrix, ((float *)&moby->rot));
    pos = ((float *)&moby->pos);
    translation = &matrices[0][12];
    for (i = 0; i < 2; i++) {
        float *matrix = matrices[i];

        euler_rotation_matrix(matrix, oscillating_link_strip_transform_rotations[i]);
        multiply_matrix(matrix, rotation_matrix, matrix);
        if (!(i & 1) && data->target != -1) {
            qcopy(anchor, ((float *)&D_L12_0015FFD8[data->target].pos));
        } else {
            qcopy(anchor, pos);
        }
        fast_vec_add(translation, translation, anchor);
        translation += 16;
        matrix[15] = 1.0f;
    }
    quad[1].g[1] = func_001F4868_303CA8(0xE);
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
        if (oscillating_link_strip_fade_pivot_ticks < data->phase[ring]) {
            fade = convert_integer_to_float(data->phase[ring] -
                                            oscillating_link_strip_fade_pivot_ticks) /
                   convert_integer_to_float(oscillating_link_strip_reset_ticks -
                                            oscillating_link_strip_fade_pivot_ticks);
        } else {
            fade = 1.0f - convert_integer_to_float(data->phase[ring]) /
                              convert_integer_to_float(oscillating_link_strip_fade_pivot_ticks);
        }
        quad[1].c[0] = quad[1].c[1] = quad[1].c[2] = quad[1].c[3] =
            fast_tween_color(oscillating_link_strip_fade_source_color,
                             oscillating_link_strip_fade_target_color,
                             fade);
        scroll = 0.0f;
        for (segment = 0; segment < data->count - 1; segment++) {
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
                             data->ring[ring][segment + (vertex >> 1)]);
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
    char *data = (char *)moby->pvars;
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
        if (fast_dec_timer((int *)(data + 0x2300 + strip * 4))) {
            int   vector_count = *(int *)(data + 0x232c);
            float intermediate_storage[vector_count][4];
            char *timers = data + 0x2300;
            intermediate = &intermediate_storage[0][0];
            i = 0;
            if (0 < *(int *)(data + 0x232c)) {
                points = (float *)(data + strip * 0x460);
                do {
                    float vector_count_float;
                    float length;
                    float index;
                    float rate;
                    clear_u64_value(points);
                    vector_count_float = (float)*(int *)(data + 0x232c);
                    length = *(float *)(data + 0x2324);
                    index = (float)i;
                    rate = oscillating_link_strip_random_delta_magnitude_rate_gp * frame_time;
                    points[3] = 1.0f;
                    points[0] = index * (length / vector_count_float);
                    random_vector(intermediate + i * 4, 0.0f, rate);
                    points += 4;
                    i++;
                } while (i < *(int *)(data + 0x232c));
            }
            count = 1;
            if (count < *(int *)(data + 0x232c) - 1) {
                points = (float *)(data + strip * 0x460 + 0x1190);
                do {
                    fast_vec_add(
                        points, intermediate_storage[count - 1], intermediate_storage[count]);
                    fast_vec_add(points, points, intermediate_storage[count + 1]);
                    fast_vec_scale(points, points, 0.333f);
                    count++;
                    points += 4;
                } while (count < *(int *)(data + 0x232c) - 1);
            }
            *(int *)(timers + (strip << 2)) = oscillating_link_strip_reset_ticks;
        } else {
            for (i = 1; i < *(int *)(data + 0x232c) - 1; i++) {
                float *point = (float *)(data + strip * 0x460 + i * 16);
                float *delta = (float *)(data + strip * 0x460 + 0x1190 + i * 16 - 16);
                fast_vec_add(point, point, delta);
            }
        }
    }
    if (fast_vec_dist(&moby->pos, D_L12_00167240) < 48.0f)
        add_draw_callback(draw_oscillating_target_link_strips, moby);
}

#include "rnc/math/vector.h"

#include "qcopy.h"

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

void update_oscillating_target_link(Moby *moby) __asm__("FUN_L12_00302f30");

void update_oscillating_target_link(Moby *moby) {
    enum {
        TARGET_TRACKING_STATE_INITIALIZE = 0,
        TARGET_TRACKING_STATE_ACTIVE = 1
    };
    Vec4         start;
    Vec4         end;
    char         hit[0x30];
    Data_304310 *data = *(Data_304310 **)&moby->pvars;
    Row         *row;
    float        swing;
    float        half;
    int          target;

    if (data->target == -1) {
        return;
    }
    switch (moby->state) {
    case TARGET_TRACKING_STATE_INITIALIZE:
        oscillating_link_strip_fade_pivot_ticks = 0x16;
        oscillating_link_strip_reset_ticks = 0x2D;
        data->f231C = -1;
        data->f2304 = 0xB;
        data->f2308 = 0x17;
        data->f230C = 0x22;
        data->f2300 = 0;
        moby->state = 1;
        break;
    case TARGET_TRACKING_STATE_ACTIVE:
        moby->rot.z = fast_arc_tan(D_L12_0015FFD8[data->target].pos.x - moby->pos.x,
                                   D_L12_0015FFD8[data->target].pos.y - moby->pos.y);
        data->dist = fast_vec_dist(((float *)&moby->pos),
                                   (char *)D_L12_0015FFD8 + (data->target << 8) + 0x10);
        data->angle = fast_add_rotations(data->angle, data->turn * 0.017453292f * D_0015ED6C);
        row = ((Row *)l12_paths[data->row]);
        swing = fast_sin(data->angle);
        if (1.5707964f < data->angle && data->angle < 3.1415927f) {
            swing = 2.0f - swing;
        }
        if (-3.1415927f < data->angle && data->angle < -1.5707964f) {
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
        if (moby->visible == 0 && D_L12_0015FFD8[data->target].visible == 0) {
            fast_vec_add(
                &start, ((float *)&moby->pos), ((float *)&D_L12_0015FFD8[data->target].pos));
            fast_vec_scale(&start, &start, half);
            start.f[3] = 16.0f;
            if (fast_bsphere_check(&start, 32.0f) == -1) {
                return;
            }
        }
        if (data->f2330 != -1 && is_point_inside_clip_volume(D_L12_00167240, data->f2330) != 0) {
            return;
        }
        if (data->f2320 == 0) {
            return;
        }
        func_L12_00304038_304310(moby);
        target = data->target;
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

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_003046C8), where it is exact; names translated to the US level program. */

extern void FUN_L08_00313ba8(void *);

extern char hoven_turret_camera_local_offset __asm__("D_L12_00161EF0") __attribute__((sda));
void        update_hoven_turret_camera(struct Moby *moby) __asm__("FUN_L12_003032e8");

void update_hoven_turret_camera(struct Moby *moby) {
    float position_and_rotation[8];
    char *data = (char *)moby->pvars;
    FUN_L08_00313ba8(moby);
    transform_vector_by_basis(
        position_and_rotation, &hoven_turret_camera_local_offset, &moby->rot_row0);
    fast_vec_add(position_and_rotation, position_and_rotation, &moby->pos);
    set_camera_position(position_and_rotation);
    position_and_rotation[4] = 0;
    position_and_rotation[5] = -*(float *)(data + 0x74);
    position_and_rotation[6] = fast_add_rotations(1.5707964f, moby->rot.z);
    set_camera_rotation(position_and_rotation + 4);
}

typedef struct {
    u8  pad0[0x40];
    s32 targets[10];
    u8  pad68[0x20];
    s32 timer;
} Data_303370;

/* Raises the timer while one of the ten targets is in the view cone, otherwise counts it down. */
void update_hoven_turret_target_alignment_timer(void *moby) __asm__("FUN_L12_00303370");

void update_hoven_turret_target_alignment_timer(void *moby) {
    Data_303370 *data = (Data_303370 *)((struct Moby *)moby)->pvars;
    int          found = 0;
    int          i;
    Vec4         position;

    for (i = 0; i < 10; i++) {
        if (data->targets[i] != -1) {
            Moby *target = &D_L12_0015FFD8[data->targets[i]];
            float yaw_limit;
            float pitch_limit;
            float yaw;
            float pitch;

            qcopy(&position, &target->pos);
            yaw_limit = 0.12217305f;
            pitch_limit = yaw_limit;
            if (target->oclass == 0x146)
                position.f[2] += 1.5f;
            if (target->oclass == 0x4FE) {
                yaw_limit = 0.034906585f;
                pitch_limit = 0.05235988f;
                position.f[2] += 1.5f;
            }
            yaw = fast_arc_tan(position.f[0] - l12_camera.pos.f[0],
                               position.f[1] - l12_camera.pos.f[1]);
            pitch = fast_arc_tan(vector_distance_xy(&l12_camera.pos, &position),
                                 position.f[2] - l12_camera.pos.f[2]);
            if (fast_difference_between_rotations(yaw, l12_camera.yaw) < yaw_limit &&
                fast_difference_between_rotations(pitch, -l12_camera.pitch) < pitch_limit)
                found = 1;
        }
    }
    if (!found) {
        fast_dec_timer(&data->timer);
    } else {
        data->timer += 2;
        if (scale_game_frames(30) < data->timer)
            data->timer = scale_game_frames(30);
    }
}

#include "sda.h"

/* Update of moby class 1267 (Hoven turret): waits for the hero, is manned and aimed with the stick, fires, and burns out when destroyed. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00304920), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef float Vec_304920[4] __attribute__((aligned(16)));

typedef struct {
    char          pad0;
    unsigned char b1;
    char          pad2[0xE];
    char          f10[0x60];
    short         f70;
    short         f72;
    float         f74;
    float         f78;
    float         f7C;
    int           f80;
    int           f84;
    int           f88;
    float         f8C;
    int           f90;
    int           f94;
    int           f98;
    int           f9C;
    float         fA0;
    float         fA4;
    int           fA8;
    short         fAC;
    short         fAE;
} Data_304920;

typedef struct {
    char         pad0[0x6C];
    struct Moby *f6C;
} Snd;

typedef struct {
    char  pad0[0x2C];
    float f2C;
} Hit_304920;

extern char *FUN_L00_0025a420(void *, int, int);
extern char *FUN_L00_0026dd20(void *, void *, int, int, float);
extern float D_0015ED6C_gp __asm__("D_0015ED6C") MACRO_ADDR;

#include "rnc/overlay/l12/mobys/hoven_turret_data/hoven_turret_update.h"

extern float              randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern float              approach_value(float *, float, float);
extern float              D_L12_0015F580_macro[] __asm__("D_L12_0015F580") MACRO_ADDR;
extern int                D_L12_0015F5C4_macro __asm__("D_L12_0015F5C4") MACRO_ADDR;
extern int D_L12_001619A0 MACRO_ADDR;
extern int                D_L12_00161F00 __attribute__((sda));
extern int                D_L12_00161F04 __attribute__((sda));
extern int D_L12_00161F20 MACRO_ADDR;
extern int D_L12_00161F24 MACRO_ADDR;
extern int                random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int                hero_set_state(int, int) __asm__("FUN_L12_002400d0");
extern void               FUN_L00_001fff28(void *, int, float);
extern void               hero_respawn(void) __asm__("FUN_L00_00211250");
extern void               set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void               moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void               jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void               spawn_explosion(void *,
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
extern void               FUN_L00_00262cf0(void *, void *, float, float, float);
extern void               start_scene(int) __asm__("FUN_L00_00298840");
extern void               queue_scene_movie_with_game_language(int) __asm__("FUN_L00_00299460");
extern void  create_moby_with_particles(void *, void *, void *, int) __asm__("FUN_L08_002e7eb0");
extern void  update_hoven_turret_target_alignment_timer(void *) __asm__("FUN_L12_00303370");
extern void  draw_hoven_turret_hud() __asm__("FUN_L12_00304218");
extern void  draw_hoven_turret_destruction_fade() __asm__("FUN_L12_00304d98");
extern void  allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void  attach_manipulator(void *, int, void *);
extern void  detach_manipulator(void *, void *);
extern void  memcard_save_data(s32, s32) __asm__("func_0020B178");
extern void  set_moby_animation(void *, s32, s32) __asm__("func_00212ED8");
extern char  D_0013E533[];
extern int   invert52_macro[] __asm__("D_0015EDDC") MACRO_ADDR;
extern Moby *D_L12_0015FFD8_gp __asm__("D_L12_0015FFD8") MACRO_ADDR;
extern int   func_0020BFC8_304920(int, int) __asm__("FUN_0020b178");

extern f32 frame_scale_macro __asm__("D_0015ED60") MACRO_ADDR;
extern f32 frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
void       hoven_turret_update(struct Moby *moby) __asm__("FUN_L12_00303540");

void hoven_turret_update(struct Moby *moby) {
    enum {
        HOVEN_TURRET_STATE_INITIALIZE = 0,
        HOVEN_TURRET_STATE_WAIT_FOR_BOARDING = 3,
        HOVEN_TURRET_STATE_MANNED = 4,
        HOVEN_TURRET_STATE_START_SCENE = 5,
        HOVEN_TURRET_STATE_INACTIVE = 6,
        HOVEN_TURRET_STATE_DESTRUCTION_EFFECTS = 7,
        HOVEN_TURRET_STATE_SAVE_DATA = 8
    };
    Data_304920 *data = *(Data_304920 **)&moby->pvars;

    ((Snd *)(D_0013E533 + 0x1D))->f6C = moby;
    switch (moby->state) {
    case HOVEN_TURRET_STATE_INITIALIZE:
        D_L12_00161F00 = 0;
        D_L12_00161F04 = 0;
        if (visited52[moby->mission + (current_level_index << 4)] == 0xFF) {
            moby->state = 6;
        } else {
            attach_manipulator(moby, 0, data);
            moby->state = 3;
            data->f84 = 0xC8;
            moby->rot.y = 0.0f;
            data->f70 = scale_game_frames(0x1E);
            data->f72 = 0;
            data->f78 = 0.0f;
            data->f7C = 0.0f;
            data->fA0 = moby->pos.z;
            data->f98 = -1;
            moby->rot.z = 3.1415927f;
            ((Snd *)(D_0013E533 + 0x1D))->f6C = moby;
        }
        break;
    case HOVEN_TURRET_STATE_WAIT_FOR_BOARDING: {
        {
            Vec_304920 out;

            moby_point_position(moby, 3, out);
        }
        if (vector_distance_xy(((float *)&moby->pos),
                               ((Hero *)(D_0013E533 + 0xE1D))->motion.pos.f) < 7.0f ||
            ((Hero *)(D_0013E533 + 0xE1D))->ground_moby == moby) {
            moby->pos.z = moby->pos.z + ((data->fA0 - 1.2f) - moby->pos.z) * 0.11f;
            data->fA4 =
                fast_add_rotations(
                    data->fA4,
                    fast_add_rotations(
                        fast_subtract_rotations(
                            fast_arc_tan(
                                ((Hero *)(D_0013E533 + 0xE1D))->motion.pos.f[0] - moby->pos.x,
                                ((Hero *)(D_0013E533 + 0xE1D))->motion.pos.f[1] - moby->pos.y),
                            moby->rot.z),
                        1.5707964f) *
                        0.01f) *
                0.9f;
            moby->rot.z = fast_add_rotations(moby->rot.z, data->fA4);
        } else {
            if (moby->pos.z < data->fA0) {
                moby->pos.z = moby->pos.z + D_0015ED6C_gp;
            } else {
                moby->pos.z = data->fA0;
            }
            if (((Hero *)(D_0013E533 + 0xE1D))->ground_moby != moby && data->f98 != -1) {
                allocate_voice_for_target_entry(2, 0, (int)moby);
            }
        }
        if (vector_distance_xy(((float *)&moby->pos),
                               ((float *)&((Hero *)(D_0013E533 + 0xE1D))->moby->pos)) < 1.3f) {
            if (scale_game_frames(0xF) < ((Hero *)(D_0013E533 + 0xE1D))->grounded_frames) {
                jet_mount(((float *)&moby->pos), ((float *)&moby->rot), 1, 0, 0);
                if (moby->prev_seq != 5) {
                    set_moby_animation(moby, 5, 0);
                }
                hero_set_state(0x32, 1);
                update_hoven_turret_camera((char *)moby);
                data->f8C = 200.0f;
                moby->hit_slot = 0xFF;
                D_L12_00161F20++;
                moby->state = 4;
            }
        }
        break;
    }
    case HOVEN_TURRET_STATE_MANNED:
        D_L12_00161F00 = 1;
        ((Hero *)(D_0013E533 + 0xE1D))->unk20A5 = 1;
        ((Hero *)(D_0013E533 + 0xE1D))->collision_off_frames = scale_game_frames(0x78);
        ((Hero *)(D_0013E533 + 0xE1D))->invulnerable_frames = scale_game_frames(0x78);
        if (D_L12_0015F5C4_macro != 2 && D_L12_001619A0 == 0) {
            add_draw_callback(draw_hoven_turret_hud, moby);
        }
        update_hoven_turret_target_alignment_timer(moby);
        if (fast_dec_timer(&data->f90)) {
            Hit_304920 *hit = (Hit_304920 *)FUN_L00_0025a420(moby, 0x10000, 0);
            if (hit != 0) {
                data->f90 = scale_game_frames(4);
                data->f8C = data->f8C - hit->f2C;
            }
            moby->hit_slot = 0xFF;
        }
        if (D_L12_00161F24 == 0) {
            if (((Hero *)(D_0013E533 + 0xE1D))->ship_flags & 1) {
                if (data->f8C > 100.0f) {
                    D_L12_00161F20--;
                }
                ((Hero *)(D_0013E533 + 0xE1D))->ship_flags = 0;
                hero_respawn();
            }
            if (data->f8C <= 0.0f) {
                D_L12_00161F04 = 1;
                data->fAC = scale_game_frames(0x78);
                moby->state = 7;
                break;
            }
        }
        if (data->f94 != -1 &&
            (signed char)((struct Moby *)((data->f94 << 8) + (*(int *)&D_L12_0015FFD8_gp)))->state <
                0) {
            if (D_L12_0015F5C4_macro == 2) {
                if (data->b1 != 0) {
                    detach_manipulator(moby, data);
                }
                if (moby->prev_seq != 0) {
                    set_moby_animation(moby, 0, 0);
                }
                moby->pos.z = data->fA0;
                moby->rot.z = 3.1415927f;
            } else {
                jet_clear_persistent(moby->mission);
                queue_scene_movie_with_game_language(0xD);
                unlock_planet(0xD);
                {
                    Vec_304920 cpos;
                    Vec_304920 crot;

                    *(u128 *)&cpos = 0;
                    *(u128 *)&crot = 0;
                    cpos[0] = 328.4f;
                    cpos[1] = 338.7f;
                    cpos[2] = 60.98f;
                    crot[2] = -1.7f;

                    D_L12_00161F00 = 0;
                    set_race_camera(cpos, crot, 0, 1);
                }
                moby->state = 5;
                break;
            }
        }
        if (D_L12_001619A0 != 0) {
            break;
        }
        {
            float joy[2];
            float y;
            float target_yaw_step;
            float target_pitch_step;

            joy[0] = ((Hero *)(D_0013E533 + 0xE1D))->stick_x;
            y = ((Hero *)(D_0013E533 + 0xE1D))->stick_y;
            if (invert52_macro[0] == 0) {
                y = -y;
            }
            joy[1] = y;
            target_yaw_step = hoven_turret_yaw_input_rate * 0.017453292f * D_0015ED6C_gp * -joy[0];
            if ((data->f7C >= 0.0f && data->f7C < target_yaw_step) ||
                (data->f7C <= 0.0f && target_yaw_step < data->f7C)) {
                approach_value(&data->f7C,
                               target_yaw_step,
                               hoven_turret_yaw_acceleration_rate * 0.017453292f *
                                   frame_time_sq_macro);
            } else {
                approach_value(&data->f7C,
                               target_yaw_step,
                               hoven_turret_yaw_braking_rate * 0.017453292f * frame_time_sq_macro);
            }
            moby->rot.z = fast_add_rotations(moby->rot.z, data->f7C);
            target_pitch_step =
                -(hoven_turret_pitch_input_rate * 0.017453292f * D_0015ED6C_gp) * -joy[1];
            if ((data->f78 >= 0.0f && data->f78 < target_pitch_step) ||
                (data->f78 <= 0.0f && target_pitch_step < data->f78)) {
                approach_value(&data->f78,
                               target_pitch_step,
                               hoven_turret_pitch_acceleration_rate * 0.017453292f *
                                   frame_time_sq_macro);
            } else {
                approach_value(&data->f78,
                               target_pitch_step,
                               hoven_turret_pitch_braking_rate * 0.017453292f *
                                   frame_time_sq_macro);
            }
            data->f74 = fast_add_rotations(data->f74, data->f78);
            if (data->f74 > hoven_turret_pitch_max) {
                data->f74 = hoven_turret_pitch_max;
            }
            if (data->f74 < hoven_turret_pitch_min) {
                data->f74 = hoven_turret_pitch_min;
            }
            FUN_L00_001fff28(data->f10, 1, -data->f74);
            if (moby->anim_flags & 2) {
                blend_moby_animation(moby, 5, 0, 1);
            }
            fast_dec_timer_s16(&data->f70);
            if ((pad_held[0] & 0xA0) && data->f70 == 0) {
                Vec_304920 firing_offset;
                Vec_304920 muz;
                float      yaw;
                int        i;

                blend_moby_animation(moby, data->f80 == 1 ? 4 : 3, 0, 1);
                yaw = fast_add_rotations(moby->rot.z, 1.5707964f);
                build_spherical_offset(
                    firing_offset, hoven_turret_firing_offset_rate * D_0015ED6C_gp, yaw, data->f74);
                if (data->f80 == 1) {
                    data->f80 = 0;
                    moby_point_position(moby, 2, muz);
                } else {
                    moby_point_position(moby, 1, muz);
                    data->f80 = 1;
                }
                fast_vec_add(muz, muz, firing_offset);
                fast_vec_add(muz, muz, firing_offset);
                fast_vec_add(muz, muz, firing_offset);
                create_moby_with_particles(moby, muz, firing_offset, 0);
                allocate_voice_for_target_entry(0, 0, (int)moby);
                l12_camera.shake_amount = 0.03f;
                l12_camera.shake_frames = scale_game_frames(0xA);
                l12_camera.shake2_amount = 0.03f;
                l12_camera.shake2_frames = scale_game_frames(7);
                data->f70 = scale_game_frames(0x14);
                for (i = 0; i < 5; i++) {
                    Vec_304920 randomized_firing_direction;
                    Vec_304920 rnd;

                    *(u128 *)rnd = 0;
                    rnd[0] = random_float_between(-1.0f, 1.0f);
                    rnd[1] = random_float_between(-1.0f, 1.0f);
                    rnd[2] = random_float_between(-1.0f, 1.0f);
                    *(u128 *)randomized_firing_direction = *(u128 *)rnd;
                    fast_vec_normalize(randomized_firing_direction,
                                       randomized_firing_direction,
                                       fast_vec_length(firing_offset) *
                                           (frame_scale_macro * (0.65f - 1.0f) + 1.0f));
                    fast_vec_add(
                        randomized_firing_direction, firing_offset, randomized_firing_direction);
                    fast_vec_normalize(
                        randomized_firing_direction,
                        randomized_firing_direction,
                        random_float_between(D_0015ED6C_gp * 3.0f, D_0015ED6C_gp * 6.0f));
                    FUN_L00_0026dd20(
                        muz,
                        randomized_firing_direction,
                        0x5F2F4F6F,
                        random_int_between(scale_game_frames(5), scale_game_frames(0xA)),
                        50000.0f);
                }
                fast_vec_normalize(firing_offset, firing_offset, D_0015ED6C_gp * 0.5f);
                FUN_L00_0026dd20(muz,
                                 firing_offset,
                                 0x2F4F7F7F,
                                 random_int_between(scale_game_frames(4), scale_game_frames(7)),
                                 500000.0f);
            }
            if (D_L12_001619A0 == 0) {
                update_hoven_turret_camera((char *)moby);
            }
        }
        break;
    case HOVEN_TURRET_STATE_DESTRUCTION_EFFECTS:
        add_draw_callback(draw_hoven_turret_destruction_fade, moby);
        if (fast_dec_timer_s16(&data->fAE)) {
            Vec_304920 explosion_position;
            float      randomized_view_offset_distance;
            float      randomized_view_yaw;
            float      randomized_view_pitch;
            float      t;

            randomized_view_offset_distance =
                random_float_between(hoven_turret_destruction_view_offset_distance_min,
                                     hoven_turret_destruction_view_offset_distance_max);
            randomized_view_yaw = fast_add_rotations(l12_camera.yaw, randf_sym(0.0f, 0.5235988f));
            randomized_view_pitch =
                fast_add_rotations(l12_camera.pitch, randf_sym(0.0f, 0.5235988f));
            qcopy(explosion_position, &l12_camera.pos);
            FUN_L00_00262cf0(explosion_position,
                             explosion_position,
                             randomized_view_offset_distance,
                             randomized_view_yaw,
                             randomized_view_pitch);
            t = random_float_between(0.5f, 1.3f);
            spawn_explosion(moby,
                            D_L12_0015F580_macro,
                            explosion_position,
                            0.0f,
                            0.0f,
                            7,
                            0xF,
                            0x19,
                            t,
                            t * 0.57f,
                            10.0f,
                            3,
                            0.7f,
                            0.0f,
                            0,
                            0,
                            -1,
                            0);
            data->fAE = scale_game_frames(random_int_between(5, 0xD));
        }
        if (fast_dec_timer_s16(&data->fAC)) {
            hero_respawn();
        }
        break;
    case HOVEN_TURRET_STATE_START_SCENE:
        if (D_L12_0015F5C4_macro != 2) {
            start_scene(1);
            moby->state = 8;
        }
        break;
    case HOVEN_TURRET_STATE_SAVE_DATA:
        if (D_L12_0015F5C4_macro != 2) {
            func_0020BFC8_304920(0, -1);
            moby->state = 6;
        }
        break;
    case HOVEN_TURRET_STATE_INACTIVE:
        if (moby->prev_seq != 0) {
            set_moby_animation(moby, 0, 0);
        }
        break;
    }
}
/* Draws the HUD gauge: a four-part frame, a 32-segment dial that fades with the data's charge, and two rotating needles. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_003055F8), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0x74];
    float f74;
    char  pad78[0x10];
    int   f88;
    float f8C;
} Data;

extern short hoven_turret_hud_gauge_center_x __asm__("D_L12_00161E14") __attribute__((sda));
extern short hoven_turret_hud_gauge_center_y __asm__("D_L12_00161E18") __attribute__((sda));
extern short hoven_turret_hud_frame_half_width __asm__("D_L12_00161E1C") __attribute__((sda));
extern short hoven_turret_hud_frame_half_height __asm__("D_L12_00161E20") __attribute__((sda));
extern short hoven_turret_hud_health_dial_radius_x __asm__("D_L12_00161E24") __attribute__((sda));
extern short hoven_turret_hud_health_dial_radius_y __asm__("D_L12_00161E28") __attribute__((sda));

#include "rnc/overlay/l12/mobys/hoven_turret_data/draw_hoven_turret_hud_2.h"

extern short hoven_turret_hud_orientation_indicator_radius_x __asm__("D_L12_00161E44")
    __attribute__((sda));
extern short hoven_turret_hud_orientation_indicator_radius_y __asm__("D_L12_00161E48")
    __attribute__((sda));

#include "rnc/overlay/l12/mobys/hoven_turret_data/draw_hoven_turret_hud.h"

extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
void        FUN_L02_0020bc88(long *m, int *b, int *a, long c, int flag);
extern long func_001F4868_3055F8(int) __asm__("FUN_001f44b8");
extern void func_L02_0020BF88_3055F8(void *, void *, void *, long, int) __asm__("FUN_L02_0020bc88");

void draw_hoven_turret_hud(Moby *moby) __asm__("FUN_L12_00304218");

void draw_hoven_turret_hud(Moby *moby) {
    int   uv[4];
    long  xy[4];
    int   colors[4];
    Data *data = *(Data **)&moby->pvars;
    long  texture;
    int   filled;
    int   color;
    int   dim_color;
    int   i;
    int   j;
    float angle;

    vu1_add_g_sregister(0x42, 0x8000000044L);
    uv[0] = hoven_turret_hud_frame_vertex_0_uv;
    uv[1] = hoven_turret_hud_frame_vertex_1_uv;
    uv[2] = hoven_turret_hud_frame_vertex_2_uv;
    uv[3] = hoven_turret_hud_frame_vertex_3_uv;
    colors[3] = hoven_turret_hud_frame_color;
    colors[2] = hoven_turret_hud_frame_color;
    colors[1] = hoven_turret_hud_frame_color;
    colors[0] = hoven_turret_hud_frame_color;
    texture = func_001F4868_3055F8(hoven_turret_hud_frame_texture_index_offset + 0x28);
    for (i = 0; i < 4; i++) {
        float sign_x;
        float sign_y;
        if (i & 1) {
            sign_x = -1.0f;
        } else {
            sign_x = 1.0f;
        }
        if (i >= 2) {
            sign_y = -1.0f;
        } else {
            sign_y = 1.0f;
        }
        xy[3] = 0xFFFFF000000000L;
        xy[2] = 0xFFFFF000000000L;
        xy[1] = 0xFFFFF000000000L;
        xy[0] = 0xFFFFF000000000L;
        xy[0] +=
            (u32)((truncate_float_to_s32((*(float *)&hoven_turret_hud_gauge_center_y +
                                          *(float *)&hoven_turret_hud_frame_half_height * sign_y) *
                                         16.0f)
                   << 16) -
                  (int)0x80000000);
        xy[0] += (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x +
                                        *(float *)&hoven_turret_hud_frame_half_width * sign_x)
                  << 4) +
                 0x8000;
        xy[1] += (u32)((truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_y * 16.0f)
                        << 16) -
                       (int)0x80000000);
        xy[1] += (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x +
                                        *(float *)&hoven_turret_hud_frame_half_width * sign_x)
                  << 4) +
                 0x8000;
        xy[2] +=
            (u32)((truncate_float_to_s32((*(float *)&hoven_turret_hud_gauge_center_y +
                                          *(float *)&hoven_turret_hud_frame_half_height * sign_y) *
                                         16.0f)
                   << 16) -
                  (int)0x80000000);
        xy[2] += (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x) << 4) + 0x8000;
        xy[3] += (u32)((truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_y * 16.0f)
                        << 16) -
                       (int)0x80000000);
        xy[3] += (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x) << 4) + 0x8000;
        func_L02_0020BF88_3055F8(xy, uv, colors, texture, 1);
    }
    texture = func_001F4868_3055F8(6);
    uv[0] = hoven_turret_hud_health_dial_vertex_0_uv;
    uv[1] = hoven_turret_hud_health_dial_vertex_1_uv;
    uv[2] = hoven_turret_hud_health_dial_vertex_2_uv;
    uv[3] = hoven_turret_hud_health_dial_vertex_3_uv;
    if (data->f8C > 100.0f) {
        color = fast_tween_color(0x6000FFFF, 0x6000FF00, (data->f8C - 100.0f) / 100.0f);
    } else {
        color = fast_tween_color(0x600000FF, 0x6000FFFF, data->f8C / 100.0f);
    }
    filled = truncate_float_to_s32((1.0f - data->f8C / 200.0f) * 32.0f);
    dim_color = (color & 0xFFFFFF) + 0x20000000;
    for (j = 0; j < 32; j++) {
        float start_angle, end_angle;
        int   x0, y0, x1, y1;
        if (j >= filled) {
            colors[3] = color;
            colors[2] = color;
            colors[1] = color;
            colors[0] = color;
        } else {
            colors[3] = dim_color;
            colors[2] = dim_color;
            colors[1] = dim_color;
            colors[0] = dim_color;
        }
        start_angle = fast_normalize_angle((float)j * 0.19634955f);
        end_angle = fast_add_rotations(start_angle, 0.19634955f);
        x0 = truncate_float_to_s32(*(float *)&hoven_turret_hud_health_dial_radius_x *
                                   fast_sin(start_angle));
        y0 = truncate_float_to_s32(*(float *)&hoven_turret_hud_health_dial_radius_y *
                                   -fast_cos(start_angle));
        x1 = truncate_float_to_s32(*(float *)&hoven_turret_hud_health_dial_radius_x *
                                   fast_sin(end_angle));
        y1 = truncate_float_to_s32(*(float *)&hoven_turret_hud_health_dial_radius_y *
                                   -fast_cos(end_angle));
        xy[3] = 0xFFFFF000000000L;
        xy[2] = 0xFFFFF000000000L;
        xy[1] = 0xFFFFF000000000L;
        xy[0] = 0xFFFFF000000000L;
        xy[0] += (u32)((truncate_float_to_s32(
                            (*(float *)&hoven_turret_hud_gauge_center_y + (float)y1) * 16.0f)
                        << 16) -
                       (int)0x80000000);
        xy[0] +=
            (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x + (float)x1) << 4) +
            0x8000;
        xy[1] += (u32)((truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_y * 16.0f)
                        << 16) -
                       (int)0x80000000);
        xy[1] += (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x) << 4) + 0x8000;
        xy[2] += (u32)((truncate_float_to_s32(
                            (*(float *)&hoven_turret_hud_gauge_center_y + (float)y0) * 16.0f)
                        << 16) -
                       (int)0x80000000);
        xy[2] +=
            (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x + (float)x0) << 4) +
            0x8000;
        xy[3] += (u32)((truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_y * 16.0f)
                        << 16) -
                       (int)0x80000000);
        xy[3] += (truncate_float_to_s32(*(float *)&hoven_turret_hud_gauge_center_x) << 4) + 0x8000;
        func_L02_0020BF88_3055F8(xy, uv, colors, texture, 1);
    }
    if (scale_game_frames(0xF) < data->f88) {
        color = fast_tween_color(0x5000FFFF,
                                 0x500000FF,
                                 convert_integer_to_float(data->f88 - scale_game_frames(0xF)) /
                                     (float)scale_game_frames(0xF));
    } else {
        color =
            fast_tween_color(0x5000FF00,
                             0x5000FFFF,
                             convert_integer_to_float(data->f88) / (float)scale_game_frames(0xF));
    }
    uv[0] = hoven_turret_hud_orientation_indicator_vertex_0_uv;
    uv[1] = hoven_turret_hud_orientation_indicator_vertex_1_uv;
    uv[2] = hoven_turret_hud_orientation_indicator_vertex_2_uv;
    uv[3] = hoven_turret_hud_orientation_indicator_vertex_3_uv;
    colors[3] = color;
    colors[2] = color;
    colors[1] = color;
    colors[0] = color;
    xy[3] = 0xFFFFF000000000L;
    xy[2] = 0xFFFFF000000000L;
    xy[1] = 0xFFFFF000000000L;
    xy[0] = 0xFFFFF000000000L;
    angle = fast_normalize_angle(fast_add_rotations(moby->rot.z * 4.0f, 0.0f));
    xy[0] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[0] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    angle = fast_normalize_angle(fast_add_rotations(moby->rot.z * 4.0f, 1.5707964f));
    xy[1] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[1] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    angle = fast_normalize_angle(fast_add_rotations(moby->rot.z * 4.0f, -1.5707964f));
    xy[2] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[2] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    angle = fast_normalize_angle(fast_add_rotations(moby->rot.z * 4.0f, 3.1415927f));
    xy[3] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[3] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    func_L02_0020BF88_3055F8(
        xy, uv, colors, get_effect_texture(hoven_turret_hud_yaw_indicator_texture_index), 1);
    xy[3] = 0xFFFFF000000000L;
    xy[2] = 0xFFFFF000000000L;
    xy[1] = 0xFFFFF000000000L;
    xy[0] = 0xFFFFF000000000L;
    angle = fast_normalize_angle(fast_add_rotations(data->f74 * 8.0f, 0.0f));
    xy[0] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[0] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    angle = fast_normalize_angle(fast_add_rotations(data->f74 * 8.0f, 1.5707964f));
    xy[1] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[1] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    angle = fast_normalize_angle(fast_add_rotations(data->f74 * 8.0f, -1.5707964f));
    xy[2] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[2] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    angle = fast_normalize_angle(fast_add_rotations(data->f74 * 8.0f, 3.1415927f));
    xy[3] += (u32)((truncate_float_to_s32(
                        fast_sin(angle) *
                        *(float *)&hoven_turret_hud_orientation_indicator_radius_y * 16.0f)
                    << 16) -
                   (int)0x80000000);
    xy[3] +=
        truncate_float_to_s32(fast_cos(angle) *
                              *(float *)&hoven_turret_hud_orientation_indicator_radius_x * 16.0f) +
        0x8000;
    func_L02_0020BF88_3055F8(
        xy, uv, colors, get_effect_texture(hoven_turret_hud_pitch_indicator_texture_index), 1);
}

/* Scale and play the vendor cue while its counter is below the limit. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00306178), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0xAC];
    short value;
} Level12VendorCounterData;

typedef struct {
    char                      pad0[0x78];
    Level12VendorCounterData *data;
} Level12VendorCounterMoby;

void draw_hoven_turret_destruction_fade(Level12VendorCounterMoby *moby) __asm__("FUN_L12_00304d98");

void draw_hoven_turret_destruction_fade(Level12VendorCounterMoby *moby) {
    Level12VendorCounterData *data = moby->data;
    int                       count = scale_game_frames(0x50);
    int                       value = data->value;
    if (value < count) {
        emit_rgba_draw_packet(0xAA, 0, 0, (count - value) * 0x50 / count);
    }
}

extern void  find_point_ahead(struct Moby *, void *, float) __asm__("FUN_L01_00274b78");
extern void *find_moby_hit(struct Moby *, int, int) __asm__("FUN_L00_0025a420");
extern int
fun_00213928(struct Moby *, void *, void *, int, int *, int, int, int) __asm__("FUN_00213928");
extern int
collect_mobies_in_sphere(void *, int, struct Moby *, void *, float) __asm__("FUN_L00_001f2868");
extern int  coll_sphere(void *, int, struct Moby *, float) __asm__("FUN_L00_001f0d60");
extern void vendor_spring(float *, float *, float, float, float, float) __asm__("FUN_L00_0025b8c0");
extern void spawn_explosion_304e00(struct Moby *,
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
extern void FUN_L00_00257470(struct Moby *, int, int);
extern void jet_animate(float, struct Moby *) __asm__("FUN_L00_00259fe8");

typedef struct {
    u8   pad0[0x20];
    s32  damage[0x10];
    Vec4 start;
    f32  speed;
    f32  bob;
    f32  bob_angle;
    f32  climb;
    f32  turn;
    s32  timer;
} Data_304e00;

typedef struct {
    u8           pad0[0x20];
    struct Moby *moby;
} HitSource;

extern struct Moby *hit_moby __asm__("D_L12_001742D8") NOT_SDA;
/* A homing mine: rises, then steers toward the point ahead and blows up near it or on contact. */
void update_homing_mine(struct Moby *moby) __asm__("FUN_L12_00304e00");

void update_homing_mine(struct Moby *moby) {
    enum {
        HOMING_MINE_STATE_INITIALIZE = 0,
        HOMING_MINE_STATE_HOMING = 1,
        HOMING_MINE_STATE_DETONATE = 2
    };
    Data_304e00 *data = (Data_304e00 *)moby->pvars;
    s32         *damage = data->damage;
    f32          target[20];
    f32          velocity[3];
    u8           hit_info[0x30];
    Vec4         direction;
    Vec4         probe;
    int          hits;
    HitSource   *source;
    float        angle;
    float        turn;
    float        height;
    float        difference;

    find_point_ahead(moby, target, 8.0f);
    source = find_moby_hit(moby, 0x230000, 0);
    fun_00213928(moby, source, damage, 0, &hits, 0, 0, 4);
    if (vector_distance_xy(&moby->pos, target) < 0.25f ||
        (hits >= 2 && source != NULL && source->moby->oclass != 0x4F5)) {
        spawn_explosion_304e00(moby,
                               D_L12_0015F580,
                               0,
                               0.0f,
                               0.0f,
                               3,
                               3,
                               5,
                               1.0f,
                               0.5f,
                               4.0f,
                               0.7f,
                               2,
                               7.0f,
                               0,
                               0,
                               -1,
                               0);
        FUN_L00_00257470(moby, 0, -1);
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case HOMING_MINE_STATE_INITIALIZE:
        moby->flags |= 0x1000;
        qcopy(&data->start, &moby->pos);
        moby->state = 1;
        data->turn = 0.6981317f;
        if (moby->parent != NULL && ((struct Moby *)moby->parent)->command != 0)
            data->turn = -data->turn;
        data->timer = 0;
        break;
    case HOMING_MINE_STATE_HOMING:
        data->timer++;
        if (data->timer < scale_game_frames(30))
            moby->pos.z +=
                fast_sin((f32)data->timer * 3.1415927f / (f32)scale_game_frames(30)) * 0.07f;
        if (scale_game_frames(15) < data->timer)
            data->speed += frame_time_sq * 30.0f;
        if (scale_game_frames(30) < data->timer) {
            if (moby->prev_seq == 0)
                blend_moby_animation(moby, 1, 0, 2);
            if (moby->prev_seq == 1 && (moby->anim_flags & 2))
                blend_moby_animation(moby, 2, 0, 2);
        }
        if (frame_time * 5.75f < data->speed)
            data->speed = frame_time * 5.75f;
        angle = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
        turn = angle;
        if (vector_distance_xy(&moby->pos, target) < 7.0f)
            turn = fast_add_rotations(angle, data->turn);
        velocity[0] = fast_cos(turn) * data->speed;
        velocity[1] = fast_sin(turn) * data->speed;
        velocity[2] = 0.0f;
        fast_vec_add(&moby->pos, &moby->pos, velocity);
        if (scale_game_frames(30) < data->timer) {
            data->bob_angle = fast_add_rotations(data->bob_angle, frame_time * 5.2359877f);
            moby->pos.z -= data->bob;
            data->bob = fast_sin(data->bob_angle) * 0.1f;
            moby->pos.z += data->bob;
        }
        if (scale_game_frames(30) < data->timer) {
            height = probe_ground_height(&moby->pos, 0, 0.5f);
            if (height < hero.ground_z.f)
                height = hero.ground_z.f;
            difference = height - moby->pos.z;
            if (difference < 0.5f && -3.0f < difference)
                vendor_spring(&moby->pos.z,
                              &data->climb,
                              height + 0.58f,
                              frame_scale_sq * 0.02f,
                              frame_scale_sq * 0.3f,
                              frame_time * 4.0f);
        }
        if (scale_game_frames(30) < data->timer) {
            direction.f[0] = fast_cos(angle) * 0.3f;
            direction.f[1] = fast_sin(angle) * 0.3f;
            direction.f[3] = 5627.925f;
            direction.f[2] = 1.2f;
            make_hit_descriptor(hit_info, moby, 0x10001, 1.0f, &direction);
            qcopy(&probe, &moby->pos);
            probe.f[2] += 0.4f;
            if (!collect_mobies_in_sphere(&probe, 0x10, moby, hit_info, 0.4f) || hit_moby == NULL ||
                hit_moby->oclass == 0x4F5 || hit_moby == moby->parent) {
                qcopy(&probe, &moby->pos);
                probe.f[2] += 0.35f;
                if (coll_sphere(&probe, 0, moby, 0.35f))
                    moby->state = 2;
            } else {
                moby->state = 2;
            }
        }
        break;
    case HOMING_MINE_STATE_DETONATE:
        spawn_explosion_304e00(moby,
                               D_L12_0015F580,
                               0,
                               0.0f,
                               0.0f,
                               5,
                               15,
                               25,
                               2.0f,
                               1.0f,
                               4.0f,
                               1.0f,
                               2,
                               7.0f,
                               0,
                               9,
                               -1,
                               0);
        mark_moby_for_removal(moby);
        return;
    }
    jet_animate(1.0f, moby);
}
