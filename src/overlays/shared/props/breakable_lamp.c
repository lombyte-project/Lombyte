/* Breakable lamp (class 0x5E7, Wrench name lamp): glows, fades with camera distance and bursts into debris, leaving lamp_destroyed (0x5EA), when touched. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"

#include "sda.h"

#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"

/* Vars of the level 5 mine FUN_L05_0031c8e0 drives. */
struct MineVars_c8e0 {
    u8  pad0[0x60];
    s32 alpha;                  /* 0x60: glow alpha, faded out with camera distance */
    u8  glow_r, glow_g, glow_b; /* 0x64: moby glow colour */
    u8  pad67;
    s32 color;                     /* 0x68: spark colour */
    u8  spark_r, spark_g, spark_b; /* 0x6C */
    u8  max_alpha;                 /* 0x6F */
};

/* A spark effect FUN_L00_0026d000 returns. */
struct Spark_c8e0 {
    u8  pad0[3];
    u8  flags; /* 0x03 */
    u8  pad4[6];
    s16 life; /* 0x0A */
    u8  padC[0x14];
    struct SparkDraw_c8e0 {
        u8  pad0[4];
        s32 mode; /* 0x24 */
        u8  pad8[2];
        u8  fade; /* 0x2A */
        u8  life; /* 0x2B */
    } draw;
};

f32 breakable_lamp_tip_distance_gp __asm__("D_L05_00161FC8") = 4.0f;

f32         debris_axis_launch_speed_gp __asm__("D_L05_00161FD0") = 0.075f;
f32         debris_upward_launch_speed_gp __asm__("D_L05_00161FD4") = 0.08f;
extern void allocate_voice_for_target_entry(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern struct Moby *spawn_debris_c8e0(f32,
                                      struct Moby *,
                                      s32,
                                      void *,
                                      void *,
                                      s32,
                                      s32,
                                      void *,
                                      void *,
                                      void *) __asm__("FUN_L00_00263fd8");
extern void
spawn_trail_c8e0(void *, s32, f32, f32, s32, s32, s32, void *, f32) __asm__("FUN_L00_00272060");
extern struct Spark_c8e0       *
spawn_puff(void *, f32, f32, f32, s32, void *, f32, s32) __asm__("FUN_L00_0026d000");
char *spawn_moby_class_5ea_from_source(unsigned char *source_moby) __asm__("FUN_L05_0031d160");

extern char *spawn_moby_class_5ea_from_source(unsigned char *) __asm__("FUN_L05_0031d160");
void         draw_moby_glow_sprite(char *m) __asm__("FUN_L05_0031cf58");

/* Mine update: sets its glow, fades it out with camera distance and blows up into debris, smoke and sparks when something touches it. */
void breakable_lamp_update(struct Moby *moby) __asm__("FUN_L05_0031c8e0");

/* Moby class 1511 */
void breakable_lamp_update(struct Moby *moby) {
    struct MineVars_c8e0  *glow_and_spark_state = (struct MineVars_c8e0 *)moby->pvars;
    Vec4                   tip;
    Vec4                   debris_velocity;
    Vec4                   effect_motion;
    Vec4                   start;
    f32                    distance;
    f32                    fade;
    f32                    size;
    f32                    speed;
    f32                    yaw;
    f32                    pitch;
    char                  *hit;
    struct Moby           *debris;
    struct Spark_c8e0     *spark;
    struct SparkDraw_c8e0 *draw;
    s32                    sign;
    s32                    turn;
    s32                    i;
    s32                    frames;

    if (moby->state == 0) {
        moby->state = 1;
        moby->glow_color =
            (0x7F000000 | glow_and_spark_state->glow_b << 16 | glow_and_spark_state->glow_g << 8) |
            glow_and_spark_state->glow_r;
        glow_and_spark_state->color = glow_and_spark_state->spark_b << 16 |
                                      glow_and_spark_state->spark_g << 8 |
                                      glow_and_spark_state->spark_r;
    }
    distance = vector_distance_xy(D_L05_001671C0, &moby->pos);
    if (48.0f <= distance) {
        return;
    }
    if (32.0f <= distance) {
        fade = (distance - 32.0f) * 0.0625f;
        glow_and_spark_state->alpha = truncate_float_to_s32(
            convert_integer_to_float(glow_and_spark_state->max_alpha) * (1.0f - fade));
    } else {
        glow_and_spark_state->alpha = glow_and_spark_state->max_alpha;
    }
    add_draw_callback(draw_moby_glow_sprite, moby);
    hit = find_moby_hit(moby, 0x810000, 0);
    moby->hit_slot = 0xFF;
    if (hit == 0) {
        return;
    }
    fast_vec_scale(&tip, &moby->rot_row2, breakable_lamp_tip_distance_gp);
    fast_vec_add(&tip, &moby->pos, &tip);
    if (!(*(s32 *)(hit + 0x24) & 0x800000) && !(fast_vec_dist(&tip, hit) <= 0.8f)) {
        if ((*(struct Moby **)(hit + 0x20))->oclass != 0x47) {
            return;
        }
        if (!(fast_vec_dist(&tip, &(*(struct Moby **)(hit + 0x20))->pos) <= 0.8f)) {
            return;
        }
    }
    allocate_voice_for_target_entry(0, 0, moby);
    spawn_moby_class_5ea_from_source((unsigned char *)moby);
    debris = spawn_debris_c8e0(frame_time_sq * 12.0f,
                               moby,
                               0x5ED,
                               &moby->pos,
                               &moby->rot,
                               scale_game_frames(0x5A),
                               0,
                               D_L05_0015F580,
                               D_L05_0015F580,
                               D_L05_0015F580);
    if (debris != 0) {
        debris->spawn_frame = moby->spawn_frame;
    }
    spawn_proximity_gated_bolts(moby);
    fast_vec_scale(&debris_velocity, &moby->rot_row0, debris_axis_launch_speed_gp * frame_scale);
    debris_velocity.f[2] += debris_upward_launch_speed_gp * frame_scale;
    debris = spawn_debris_c8e0(frame_time_sq * 12.0f,
                               moby,
                               0x5EE,
                               &moby->pos,
                               &moby->rot,
                               scale_game_frames(0x5A),
                               0,
                               &debris_velocity,
                               D_L05_0015F580,
                               D_L05_0015F580);
    if (debris != 0) {
        debris->spawn_frame = moby->spawn_frame;
    }
    debris_velocity.f[0] = -debris_velocity.f[0];
    debris_velocity.f[1] = -debris_velocity.f[1];
    moby->rot.z = fast_add_rotations(moby->rot.z, 3.1415927f);
    debris = spawn_debris_c8e0(frame_time_sq * 12.0f,
                               moby,
                               0x5EE,
                               &moby->pos,
                               &moby->rot,
                               scale_game_frames(0x5A),
                               0,
                               &debris_velocity,
                               D_L05_0015F580,
                               D_L05_0015F580);
    if (debris != 0) {
        debris->spawn_frame = moby->spawn_frame;
    }
    for (i = 0; i < 24; i++) {
        size = random_float_between(0.3f, 4.0f);
        sign = random_integer_below(2);
        if (sign == 0) {
            sign--;
        }
        yaw = random_angle_radians();
        pitch = random_float_between(0.34906584f, 1.5358897f);
        build_spherical_offset(
            &effect_motion, random_float_between(0.05f, 0.1f) * frame_scale, yaw, pitch);
        qcopy(&start, &tip);
        start.f[0] += randf_sym(0.0f, 0.15f);
        start.f[1] += randf_sym(0.0f, 0.15f);
        start.f[2] += randf_sym(0.0f, 0.15f);
        frames = scale_game_frames(0x3C);
        spawn_trail_c8e0(&start,
                         frames,
                         size * 0.1f,
                         size,
                         glow_and_spark_state->color | 0x7F000000,
                         0,
                         sign,
                         &effect_motion,
                         0.003f);
        sign = -sign;
        frames = scale_game_frames(0x3C);
        spawn_trail_c8e0(
            &start, frames, size * 0.07f, size * 0.7f, 0x7F7F7F7F, 1, sign, &effect_motion, 0.003f);
        effect_motion.f[1] = 0.0f;
        effect_motion.f[0] = 0.0f;
        effect_motion.f[2] = random_float_between(0.01f, 0.05f) * frame_scale;
        speed = random_float_between(60000.0f, 200000.0f);
        turn = random_integer_below(2);
        spark = spawn_puff(&tip,
                           0.3f,
                           1.0f,
                           1.02f,
                           random_integer_below(2) ? -turn : turn,
                           &effect_motion,
                           speed,
                           0x407F7F7F);
        if (spark != 0) {
            spark->life = scale_game_frames(random_int_between(0x5A, 0x78));
            draw = &spark->draw;
            draw->mode = 2;
            draw->fade = random_int_between(0x40, 0x60);
            draw->life = spark->life;
            if (random_integer_below(2) != 0) {
                spark->flags = 0x44;
            }
        }
    }
    mark_moby_for_removal(moby);
}
/* Draws the moby's camera-facing glow sprite, flickering its alpha at random. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031E468), where it is exact; names translated to the US level program. */

typedef struct {
    float m[4][4];
    u32   col[4];
    float uv[8];
    u64   a, b, c, d;
} Pk;

extern float glow_sprite_local_vertices[4][4] __asm__("D_L05_00215C70");
extern void  draw_geometry_quad(void *, void *, int);

extern short glow_sprite_camera_offset_distance __asm__("D_L05_00161FCC") __attribute__((sda));
extern char  D_L05_001671C0_lui[] __asm__("D_L05_001671C0") NOT_SDA;

void draw_moby_glow_sprite(char *m) {
    char *d = *(char **)(m + 0x78);
    Pk    pk;
    float up[4];
    float M[4][4];
    float sprite_offset[4];
    float camera_facing_offset[4];
    u32   col;
    int   j;
    *(OvlQuad *)up = 0;
    up[2] = 1.0f;
    up[3] = 1.0f;
    pk.b = get_effect_texture(0xB);
    pk.d = 0x8000000048ULL;
    pk.c = 0x0000FF9000000260ULL;
    pk.a = 5;
    pk.uv[0] = 0.0f;
    pk.uv[1] = 0.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 1.0f;
    pk.uv[7] = 1.0f;
    fast_vec_scale(sprite_offset, m + 0xE0, *(float *)&breakable_lamp_tip_distance_gp);
    fast_vec_add(M[3], m + 0x10, sprite_offset);
    fast_vec_sub(M[0], D_L05_001671C0_lui, M[3]);
    fast_vec_normalize(M[0], M[0], 1.0f);
    fast_vec_cross(M[1], M[0], up);
    fast_vec_normalize(M[1], M[1], 1.0f);
    fast_vec_cross(M[2], M[1], M[0]);
    fast_vec_scale(camera_facing_offset, M[0], *(float *)&glow_sprite_camera_offset_distance);
    fast_vec_add(M[3], M[3], camera_facing_offset);
    if (((unsigned char *)d)[0x67] != 0 && random_integer_below(8) == 0) {
        *(int *)(d + 0x60) >>= 1;
    }
    col = (*(int *)(d + 0x60) << 24) | *(int *)(d + 0x68);
    pk.col[3] = col;
    pk.col[2] = col;
    pk.col[1] = col;
    pk.col[0] = col;
    for (j = 0; j < 4; j++) {
        fast_vec_scale(pk.m[j], glow_sprite_local_vertices[j], *(float *)(d + 0x70));
        transform_vector(pk.m[j], pk.m[j], M);
    }
    draw_geometry_quad(&pk, 0, 0);
}

char *spawn_moby_class_5ea_from_source(unsigned char *source_moby) {
    unsigned char *destination_moby = (unsigned char *)create_moby(0x5EA);
    if (destination_moby != 0) {
        destination_moby[0x30] = source_moby[0x30];
        MOBY(destination_moby)->draw_dist = ((unsigned short)MOBY(source_moby)->draw_dist);
        destination_moby[0x31] = 1;
        MOBY(destination_moby)->rot.x = MOBY(source_moby)->rot.x;
        MOBY(destination_moby)->rot.y = MOBY(source_moby)->rot.y;
        MOBY(destination_moby)->rot.z = MOBY(source_moby)->rot.z;
        MOBY(destination_moby)->spawn_frame = ((long)MOBY(source_moby)->spawn_frame);
        qcopy(destination_moby + 0x10, source_moby + 0x10);
        update_entity(destination_moby);
    }
    return (char *)destination_moby;
}

f32 D_L14_0016229C = 4.0f;
f32 D_L14_001622A4 = 0.075f;
f32 D_L14_001622A8 = 0.08f;

float D_L14_001F6200[4][4] = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};

short D_L14_001622A0 = -13107;

float glow_sprite_local_vertices[4][4] __asm__("D_L05_00215C70") = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};
