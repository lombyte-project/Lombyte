/* Level 06 moby class 0x40b: beam barrier. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "rnc/world/collision/collision_line.h"

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002F6790), where it is exact; names translated to the US level program. */
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");

/* angle from a moby to its owner, clamped to the gaps between obstacles in its list */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FAB28), where it is exact; names translated to the US level program. */
extern char *D_L06_0015FFD8;

#include "qcopy.h"

/* raise each listed moby's position z by a fixed amount and notify it */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FC4C0), where it is exact; names translated to the US level program. */
extern float random_float_between(float, float) __asm__("FUN_002132a8");

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FC6D8), where it is exact; names translated to the US level program. */
extern char D_L06_00167540[] NOT_SDA;

/* Steers a moby's camera-like orientation toward a target point, returns 1 when done. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002FCA98), where it is exact; names translated to the US level program. */
extern char D_L06_00167540[];
void        add_draw_callback(s32 arg0, s32 arg1) __asm__("FUN_001f4600");
extern int  voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");

/* Sparks thrown off the swinging hazard while the camera is near it. */
extern f32  random_float_between(f32, f32) __asm__("FUN_002132a8");
extern void random_vector(void *, f32, f32) __asm__("FUN_L00_00257d78");
extern void release_voice_slot(int) __asm__("FUN_0022d798");
extern int  help_request(int, int) __asm__("FUN_L00_00203908");

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002B5990.c: func_L06_002F78A0). */
#include "rnc/audio/voice_pool.h"
#include "rnc/ui/help/help_record.h"

typedef struct GbMoby         GbMoby;
extern GbMoby *D_L06_001745D8 NOT_SDA;
extern float                  D_L06_001DAFD0[];
extern float                  D_L06_001DB060[];
extern int                    D_L06_00161E40[] MACRO_ADDR;
extern int                    D_L06_00161E50;
extern int                    D_L06_00161E54;
extern f32                    D_L06_00161E08 __attribute__((sda));
extern float                  D_L06_00161E0C __attribute__((sda));
extern float beam_barrier_collision_z_offset_gp __asm__("D_L06_00161E18") __attribute__((sda));
extern int   beam_barrier_burst_color_from_gp __asm__("D_L06_00161E1C") __attribute__((sda));
extern int   beam_barrier_burst_color_to_gp __asm__("D_L06_00161E20") __attribute__((sda));
extern float beam_barrier_burst_scale_min_gp __asm__("D_L06_00161E24") __attribute__((sda));
extern float beam_barrier_burst_scale_max_gp __asm__("D_L06_00161E28") __attribute__((sda));
extern int beam_barrier_burst_lifetime_min_frames_gp __asm__("D_L06_00161E2C") __attribute__((sda));
extern int beam_barrier_burst_lifetime_max_frames_gp __asm__("D_L06_00161E30") __attribute__((sda));
extern float beam_barrier_burst_drift_min_speed_gp __asm__("D_L06_00161E34") __attribute__((sda));
extern float beam_barrier_burst_drift_max_speed_gp __asm__("D_L06_00161E38") __attribute__((sda));
extern float frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern int   current_level_index_macro __asm__("D_0015ED84") MACRO_ADDR;
extern int   D_0015EEA4_macro __asm__("D_0015EEA4") MACRO_ADDR;
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern unsigned int   fast_tween_color(unsigned int, unsigned int, float) __asm__("FUN_001fa6e0");
extern s32            random_integer_below(s32) __asm__("func_00213260");
extern unsigned char *spawn_moving_hero_relative_alpha_fade_particle(
    float *, char *, int, unsigned char, unsigned char, int, float) __asm__("FUN_L00_002730e0");
extern void make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern void record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
void        draw_beam_barrier_segments(char *moby) __asm__("FUN_L06_002f6dd0");
extern void draw_beam_barrier_segments(char *moby) __asm__("FUN_L06_002f6dd0");
void        animate_beam_barrier_strands(struct Moby *moby) __asm__("FUN_L06_002f7000");
extern void animate_beam_barrier_strands(struct Moby *moby) __asm__("FUN_L06_002f7000");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");

/* Moby vars of the barrier that FUN_L06_002f6470 updates. */
struct BarrierVars {
    int   links[4];    /* 0x00: moby-table indices of the mobys that open it (-1: none) */
    int   voice;       /* 0x10: looping voice slot (-1: none) */
    short touches;     /* 0x14: hero contacts and hits so far; 3 or more requests help */
    short touch_timer; /* 0x16: frames until the next touch counts */
    float fade;        /* 0x18: 1 while the hero is within 48 units; fades the beam colour */
};

/* Beam barrier: pushes the hero back and counts the touches; once every linked
   moby is done it bursts into sparks and removes itself. */
void update_beam_barrier(struct Moby *moby) __asm__("FUN_L06_002f6470");

void update_beam_barrier(struct Moby *moby) {
    enum {
        BEAM_BARRIER_INITIALIZE = 0,
        BEAM_BARRIER_ACTIVE = 1,
        BEAM_BARRIER_EMIT_SPARK_BURST = 2,
        BEAM_BARRIER_REMOVE = 3
    };
    struct BarrierVars *vars = (struct BarrierVars *)moby->pvars;
    unsigned char       state = moby->state;
    struct {
        float matrix[4][4];
        union {
            float local[4][4];
            struct {
                float rows[3][4];
                float delta[4];
            } collision;
        } shared;
        float point[4], offset[4], effect[4];
    } work;
    float         step;
    float         distance;
    int           segment;
    float        *column;
    int           next_segment;
    float        *pos;
    float        *cursor;
    float         yaw, dot;
    float         radius, blend, scale, lifetime, drift_step;
    int           color, lifetime_units;
    unsigned char life, random_byte;

    switch (state) {
    case BEAM_BARRIER_INITIALIZE: {
        D_L06_00161E54 = 0x16;
        D_L06_00161E50 = 0x2D;
        D_L06_00161E40[1] = 0xB;
        D_L06_00161E40[2] = 0x17;
        D_L06_00161E40[3] = 0x22;
        D_L06_00161E40[0] = 0;
        vars->voice = -1;
        moby->state = 1;
        moby->flags = moby->flags | 0x4000;
        return;
    }
    case BEAM_BARRIER_ACTIVE: {
        fast_dec_timer_s16(&vars->touch_timer);
        if (voice_is_playing(moby, vars->voice) == 0)
            vars->voice =
                ((int (*)(int, int, int))allocate_voice_for_target_entry)(0, 4, (int)moby);
        {
            cursor = (float *)&moby->pos;
            animate_beam_barrier_strands(moby);
            distance = fast_vec_dist(cursor, D_L06_00167540);
            pos = cursor;
        }
        if (distance < 48.0f) {
            step = frame_time_macro;
            cursor = &vars->fade;
            approach_value(cursor, 1.0f, step + step);
            work.matrix[2][0] = fast_cos(moby->rot.z) * 0.5f;
            work.matrix[2][1] = fast_sin(moby->rot.z) * 0.5f;
            work.matrix[2][2] = 0.0f;
            yaw = fast_add_rotations(moby->rot.z, 1.5707964f);
            work.matrix[3][0] = fast_cos(yaw) * 3.0f;
            yaw = fast_add_rotations(moby->rot.z, 1.5707964f);
            work.matrix[3][1] = fast_sin(yaw) * 3.0f;
            work.matrix[3][2] = 0.0f;
            qcopy(work.matrix[0], pos);
            work.matrix[0][2] = work.matrix[0][2] - beam_barrier_collision_z_offset_gp;
            qcopy(work.matrix[1], work.matrix[0]);
            fast_vec_add(work.matrix[0], work.matrix[0], work.matrix[2]);
            fast_vec_add(work.matrix[1], work.matrix[1], work.matrix[2]);
            fast_vec_add(work.matrix[0], work.matrix[0], work.matrix[3]);
            fast_vec_sub(work.matrix[1], work.matrix[1], work.matrix[3]);
            make_hit_descriptor(work.shared.local[0], moby, 0x10001, 5.0f, work.matrix[2]);
            collision_line(work.matrix[0], work.matrix[1], 0, moby, work.shared.local[0]);
            if (D_00141968[43].count == 0 && vars->touch_timer == 0 &&
                D_L06_001745D8 == (GbMoby *)hero.moby) {
                short touches = vars->touches + 1;
                vars->touches = touches;
                vars->touch_timer = scale_game_frames(30);
            }
            yaw = fast_add_rotations(moby->rot.z, 3.1415927f);
            work.matrix[2][0] = fast_cos(yaw) * 0.5f;
            yaw = fast_add_rotations(moby->rot.z, 3.1415927f);
            work.matrix[2][1] = fast_sin(yaw) * 0.5f;
            work.matrix[2][2] = 0.0f;
            yaw = fast_add_rotations(moby->rot.z, 1.5707964f);
            work.matrix[3][0] = fast_cos(yaw) * 3.0f;
            yaw = fast_add_rotations(moby->rot.z, 1.5707964f);
            work.matrix[3][1] = fast_sin(yaw) * 3.0f;
            work.matrix[3][2] = 0.0f;
            qcopy(work.matrix[0], pos);
            work.matrix[0][2] = work.matrix[0][2] - beam_barrier_collision_z_offset_gp;
            qcopy(work.matrix[1], work.matrix[0]);
            fast_vec_add(work.matrix[0], work.matrix[0], work.matrix[2]);
            fast_vec_add(work.matrix[1], work.matrix[1], work.matrix[2]);
            fast_vec_add(work.matrix[0], work.matrix[0], work.matrix[3]);
            fast_vec_sub(work.matrix[1], work.matrix[1], work.matrix[3]);
            make_hit_descriptor(work.shared.local[0], moby, 0x10001, 5.0f, work.matrix[2]);
            collision_line(work.matrix[0], work.matrix[1], 0, moby, work.shared.local[0]);
            if (D_00141968[43].count == 0 && vars->touch_timer == 0 &&
                D_L06_001745D8 == (GbMoby *)hero.moby) {
                short touches = vars->touches + 1;
                vars->touches = touches;
                vars->touch_timer = scale_game_frames(30);
            }
            if (hero.coll_hit_moby == (u8 *)moby) {
                fast_vec_sub(work.shared.collision.delta, &hero.motion.pos, pos);
                dot = fast_vec_dot(work.shared.collision.delta, &moby->rot_row0);
                if (0.0f < dot)
                    fast_vec_normalize(work.shared.local[0], &moby->rot_row0, 1.0f);
                else
                    fast_vec_normalize(work.shared.local[0], &moby->rot_row0, -1.0f);
                record_moby_hit(hero.moby, work.shared.local[0]);
                if (D_00141968[43].count == 0 && vars->touch_timer == 0) {
                    short touches = vars->touches + 1;
                    vars->touches = touches;
                    vars->touch_timer = scale_game_frames(30);
                }
            }
        } else {
            approach_value(&vars->fade, 0.0f, (step = frame_time_macro) + step);
        }
        if (vars->fade != 0.0f)
            add_draw_callback((s32)draw_beam_barrier_segments, (s32)moby);
        if (D_00141968[43].count == 0) {
            if (vars->touch_timer == 0) {
                char *hit = find_moby_hit(moby, 0x330000, 0);
                if (hit != 0) {
                    short touches = vars->touches + 1;
                    vars->touches = touches;
                    vars->touch_timer = scale_game_frames(30);
                }
            }
            moby->hit_slot = 0xFF;
            if (vars->touches >= 3)
                help_request(0x1774, 0x2B);
        }
        {
            int   all_done = 1;
            int   count_usage = 1;
            char *moby_table = D_L06_0015FFD8;
            int   link;
            for (link = 0; link < 4; link++) {
                int index = vars->links[link];
                if (index >= 0) {
                    struct Moby *linked = (struct Moby *)(moby_table + index * 256);
                    if (linked->oclass == 0x516) {
                        if (linked->state != 2)
                            all_done = 0;
                        else
                            count_usage = 0;
                    } else if (linked->command == 0) {
                        all_done = 0;
                    }
                }
            }
            if (!all_done)
                return;
            moby->collision = 0;
            moby->state = 2;
            {
                int voice = vars->voice;
                if (voice != -1) {
                    if ((struct Moby *)voice_pool.voices[voice].owner == moby &&
                        voice_pool.voices[voice].state != 0)
                        release_voice_slot(voice);
                }
                vars->voice = -1;
            }
            allocate_voice_for_target_entry(1, 0, (int)moby);
            if (count_usage) {
                unsigned short count = D_00141968[43].count;
                if (count <= 0xFFFE)
                    D_00141968[43].count = count + 1;
            }
            {
                int best = scale_game_frames(D_0015EEA4_macro) / 600;
                if ((int)D_00141968[43].best < best) {
                    int new_best = scale_game_frames(D_0015EEA4_macro) / 600;
                    D_00141968[43].best = new_best;
                }
                D_00141968[43].flags =
                    D_00141968[43].flags | (1 << current_level_index_macro) | 0x80000000;
            }
        }
        return;
    }
    case BEAM_BARRIER_EMIT_SPARK_BURST: {
        int spark;
        int segment_offset;
        euler_rotation_matrix(work.matrix, &moby->rot);
        set_scale_matrix(work.shared.local, D_L06_00161E0C);
        segment = 0;
        pos = (float *)&moby->pos;
        multiply_matrix(work.matrix, work.matrix, work.shared.local);
        column = work.shared.local[1];
        do {
            segment_offset = segment * 16;
            spark = 19;
            euler_rotation_matrix(work.shared.local, (char *)D_L06_001DB060 + segment_offset);
            next_segment = segment + 1;
            multiply_matrix(work.shared.local, work.matrix, work.shared.local);
            transform_vector(work.point, (char *)D_L06_001DAFD0 + segment_offset, work.matrix);
            fast_vec_add(work.shared.collision.delta, work.point, pos);
            work.shared.collision.delta[3] = 1.0f;
            do {
                radius = randf_sym(0.0f, D_L06_00161E08 * 0.5f);
                fast_vec_normalize(work.offset, column, radius);
                fast_vec_add(work.offset, work.offset, work.shared.collision.delta);
                blend = random_float_between(0.0f, 1.0f);
                color = fast_tween_color(
                    beam_barrier_burst_color_from_gp, beam_barrier_burst_color_to_gp, blend);
                scale = random_float_between(beam_barrier_burst_scale_min_gp,
                                             beam_barrier_burst_scale_max_gp);
                lifetime = random_float_between((float)beam_barrier_burst_lifetime_min_frames_gp,
                                                (float)beam_barrier_burst_lifetime_max_frames_gp);
                lifetime_units = truncate_float_to_s32(lifetime);
                drift_step = frame_time_macro;
                random_vector(work.effect,
                              beam_barrier_burst_drift_min_speed_gp * drift_step,
                              beam_barrier_burst_drift_max_speed_gp * drift_step);
                life = (unsigned char)scale_game_frames(lifetime_units);
                random_byte = (unsigned char)random_integer_below(0xFF);
                /* Called through a cast: retail evaluates the arguments in this order. */
                ((void (*)(float *, char *, int, float, unsigned char, unsigned char, int))
                     spawn_moving_hero_relative_alpha_fade_particle)(
                    work.offset, (char *)work.effect, color, scale, life, random_byte, 0);
                spark = spark - 1;
            } while (spark >= 0);
            segment = next_segment;
        } while (segment < 9);
        moby->state = 3;
        return;
    }
    case BEAM_BARRIER_REMOVE:
        mark_moby_for_removal(moby);
        return;
    }
}

extern int  get_effect_texture(int) __asm__("FUN_001f44b8");
extern void vu1_add_g_sregister(int, long) __asm__("FUN_00233980");
extern void FUN_L06_00216b88(int *, float *);

int D_L06_00161DD0 = 0;

int D_L06_00161DD4 = 2;

int D_L06_00161DD8 = 0;

int D_L06_00161DDC = 1;

int D_L06_00161DF0 = 0x80c04070;

int D_L06_00161DF4 = 0xff0000;

extern float beam_barrier_texture_u_step_gp __asm__("D_L06_00161DF8") __attribute__((sda));

float D_L06_00161DFC = 0;

Vec4 D_L06_001DB0F0[4][30] = {0};

void draw_beam_barrier_segments(char *moby) {
    float        base_matrix[16];
    float        yaw_matrix[16];
    float        segment_matrix[9][16];
    float        offset[4];
    int          params[9];
    char        *data = ((char *)MOBY(moby)->pvars);
    float       *matrix;
    float       *rotation;
    float       *offset_src;
    float       *translation;
    float       *first_matrix;
    int         *segment_params;
    unsigned int color;
    int          first, second, fourth;
    int         *points, *sequence;
    float        seventh, eighth;
    int          i;
    int          j;

    euler_rotation_matrix(base_matrix, moby + 0x40);
    set_scale_matrix(yaw_matrix, D_L06_00161E0C);
    multiply_matrix(base_matrix, base_matrix, yaw_matrix);
    vu1_add_g_sregister(6, get_effect_texture(14));
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x42,
                        (long)D_L06_00161DD0 | ((long)D_L06_00161DD4 << 2) |
                            ((long)D_L06_00161DD8 << 4) | ((long)D_L06_00161DDC << 6) |
                            0x8000000000L);
    font_queue_vu_state();
    first_matrix = segment_matrix[0];
    segment_params = params;
    rotation = D_L06_001DB060;
    offset_src = D_L06_001DAFD0;
    translation = segment_matrix[0] + 12;
    matrix = first_matrix;
    for (i = 8; i >= 0; i--) {
        euler_rotation_matrix(matrix, rotation);
        rotation += 4;
        multiply_matrix(matrix, base_matrix, matrix);
        transform_vector(offset, offset_src, base_matrix);
        offset_src += 4;
        fast_vec_add(translation, offset, moby + 0x10);
        matrix[15] = 1.0f;
        matrix += 16;
        translation += 16;
    }
    color = fast_tween_color(D_L06_00161DF0 & 0xFFFFFF, D_L06_00161DF0, *(float *)(data + 0x18));
    first = D_L06_00161E50;
    points = (int *)D_L06_001DB0F0;
    second = D_L06_00161E54;
    fourth = D_L06_00161DF4;
    seventh = beam_barrier_texture_u_step_gp;
    eighth = D_L06_00161DFC;
    sequence = D_L06_00161E40;
    params[0] = first;
    params[2] = (int)sequence;
    params[1] = second;
    params[3] = color;
    params[4] = fourth;
    *(float *)&params[7] = seventh;
    *(float *)&params[8] = eighth;
    params[5] = 0x1E;
    params[6] = (int)points;
    matrix = first_matrix;
    for (j = 8; j >= 0; j--) {
        FUN_L06_00216b88(segment_params, matrix);
        matrix += 16;
    }
}

extern f32 beam_barrier_rotation_x_speed_degrees_gp __asm__("D_L06_00161E10") __attribute__((sda));
extern f32 beam_barrier_render_phase_speed_gp __asm__("D_L06_00161E00") __attribute__((sda));
extern f32 beam_barrier_class_scale_multiplier_gp __asm__("D_L06_00161E14") __attribute__((sda));
extern f32 beam_barrier_strand_jitter_max_speed_gp __asm__("D_L06_00161E04") __attribute__((sda));
extern int D_L06_00161E40[4] MACRO_ADDR;

Vec4 D_L06_001DB870[4][30] = {0};

extern void FUN_L06_00217300(void *);

/* Spins the moby and animates four 30-point strands: a strand whose timer ran out is rebuilt with a smoothed random
   drift (each point averages three jitters) and its timer reset; otherwise every inner point moves by its drift. */
void animate_beam_barrier_strands(struct Moby *moby) {
    Vec4  jitter[30];
    float y;
    float step;
    float phase;
    int   i;
    int   j;
    int   k;

    moby->rot.x = fast_add_rotations(
        moby->rot.x, beam_barrier_rotation_x_speed_degrees_gp * DEG_TO_RAD * frame_time);
    phase = D_L06_00161DFC + beam_barrier_render_phase_speed_gp * frame_time;
    moby->scale = *(f32 *)((char *)moby->pclass + 0x24) * beam_barrier_class_scale_multiplier_gp;
    D_L06_00161DFC = phase;
    if (phase > 1.0f) {
        D_L06_00161DFC = phase - 1.0f;
    }
    step = D_L06_00161E08 / 30.0f;
    y = -(D_L06_00161E08 * 0.5f);
    for (i = 0; i < 4; i++) {
        if (D_L06_00161E40[i] == 0) {
            for (k = 0; k < 30; k++) {
                FUN_L06_00217300(&D_L06_001DB0F0[i][k]);
                D_L06_001DB0F0[i][k].f[1] = y;
                random_vector(
                    &jitter[k], 0.0f, beam_barrier_strand_jitter_max_speed_gp * frame_time);
                y += step;
            }
            for (k = 1; k < 29; k++) {
                fast_vec_add(&D_L06_001DB870[i][k], &jitter[k - 1], &jitter[k]);
                fast_vec_add(&D_L06_001DB870[i][k], &D_L06_001DB870[i][k], &jitter[k + 1]);
                fast_vec_scale(&D_L06_001DB870[i][k], &D_L06_001DB870[i][k], 0.333f);
            }
            D_L06_00161E40[i] = D_L06_00161E50;
        } else {
            for (j = 1; j < 29; j++) {
                fast_vec_add(&D_L06_001DB0F0[i][j], &D_L06_001DB0F0[i][j], &D_L06_001DB870[i][j]);
            }
            D_L06_00161E40[i]--;
        }
    }
}

#include "sda.h"

int D_L06_00161E40[4] = {0};

int D_L06_00161E50 = 0;

int D_L06_00161E54 = 0;

int beam_barrier_burst_color_from_gp __asm__("D_L06_00161E1C") = 0x80802020;
