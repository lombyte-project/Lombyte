/* Level 14 wind control moby class and its sparks. */
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
#include "rnc/math/random.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/overlay/l14/mobys/wind_control_globals.h"
#include "rnc/overlay/level_camera.h"

extern s64 get_effect_texture(s32) __asm__("func_001F44B8");

extern void draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");

extern void debug_print(const char *, short) __asm__("FUN_001e93b0");

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

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

extern struct {
    f32 v;
} D_L14_00162230, D_L14_00162234;

extern s32 D_L14_00162238;

extern f32 D_L14_00162240;

char D_L14_001FC980[] =
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

f32 wind_effect_strength_gp __asm__("D_L14_00161C08") = 2e+01f;

extern s32 wind_push_decay_frames_gp __asm__("D_L14_00161C04") __attribute__((sda));

void wind_control_update(struct Moby *wind_control_moby) __asm__("FUN_L14_002eaf88");

/* Moby class 610 */
void wind_control_update(struct Moby *wind_control_moby) {
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

extern int ftoi_k(float) __asm__("FUN_001fa6d0");

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

int wind_spark_speed_range_start_gp __asm__("D_L14_00161C30") = 15;

int wind_spark_speed_range_end_gp __asm__("D_L14_00161C34") = 40;

float wind_spark_size_range_start_gp __asm__("D_L14_00161C38") = 2.0f;

float wind_spark_size_range_end_gp __asm__("D_L14_00161C3C") = 6.0f;

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
        frame_time;
    lifetime_frames = scale_game_frames(ftoi_k(spark_vars->f14 * 2.0f / spark_vars->f440[i]));
    spark_vars->n320[i] = lifetime_frames;
    spark_vars->n380[i] = lifetime_frames;
    spark_vars->n3E0[i] = 0;
}

extern int spark_float_to_integer(float) __asm__("FUN_001fa6d0");

void draw_wind_sparks(struct Moby *m) __asm__("FUN_L14_002eb810");

extern int wind_spark_peak_alpha_gp __asm__("D_L14_00161C10") __attribute__((sda));

int wind_spark_fade_in_frames_gp __asm__("D_L14_00161C2C") = 30;

void update_wind_sparks(struct Moby *wind_control_moby) {
    Spk_k *spark_vars = (Spk_k *)wind_control_moby->pvars;
    float  spark_travel_direction[4];
    float  clip[4];
    float  spark_position_delta[4];
    float  ratio;
    float  spark_fade_fraction;
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
                spark_fade_fraction =
                    convert_integer_to_float(spark_vars->n380[i] - spark_vars->n320[i]);
                spark_fade_fraction =
                    spark_fade_fraction / convert_integer_to_float(wind_spark_fade_in_frames_gp);
            } else {
                spark_fade_fraction = convert_integer_to_float(spark_vars->n320[i]);
                spark_fade_fraction =
                    spark_fade_fraction /
                    convert_integer_to_float(spark_vars->n380[i] - wind_spark_fade_in_frames_gp);
            }
            {
                int spark_alpha =
                    spark_float_to_integer(((float)opacity - 0.0f) * spark_fade_fraction + 0.0f);
                *shade = spark_alpha;
                /* Fakematch: storing the shade again is a no-op that reload's cse drops; the
                 * extra uses rank `shade` above `m` for register allocation as in retail. */
                if (spark_alpha != 0) {
                    *shade = spark_alpha;
                }
            }
            fast_vec_scale(spark_position_delta, spark_travel_direction, spark_vars->f440[i]);
            fast_vec_add(pos, pos, spark_position_delta);
            if (in_clip != 0 && absolute_float(pos[0] - l14_camera.pos.f[0]) < 8.0f &&
                absolute_float(pos[1] - l14_camera.pos.f[1]) < 8.0f &&
                fast_difference_between_rotations(wind_control_moby->rot.z, l14_camera.yaw) >
                    2.3561945f) {
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

typedef unsigned int u128_m __attribute__((mode(TI)));

typedef struct {
    float         m[4][4];
    u32           col[4];
    float         uv[8];
    unsigned long a, b, c, d;
} Pk_m;

extern char D_L14_001674C0_m[] __asm__("D_L14_001674C0");

int wind_spark_base_color_gp __asm__("D_L14_00161C0C") = 0x7f7f7f;

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

extern float fast_sin(float);

float absolute_float(float input) __asm__("func_001F99C0");

s32 D_L14_00162238 = 0;

s32 wind_push_decay_frames_gp __asm__("D_L14_00161C04") = 0x1e;

int wind_spark_peak_alpha_gp __asm__("D_L14_00161C10") = 0x32;
