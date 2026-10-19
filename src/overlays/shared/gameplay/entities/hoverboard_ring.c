/* Shared moby classes: hoverboard_ring, lamp. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"

#include "sda.h"

#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031C0F0), where it is exact; names translated to the US level program. */

extern float random_angle_radians(void) __asm__("FUN_00213308");
extern int fast_bsphere_check(char *, float) __asm__("FUN_001fa728");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern unsigned char *FUN_L00_002743a8(float f0, float f1, int a0, void *pos, int timer, int flag,
                                       int col, float *vel);

void hoverboard_ring_update(char *moby) __asm__("FUN_L05_0031abe0");
void hoverboard_ring_update(char *moby) {
    enum {
        HOVERBOARD_RING_STATE_INIT = 0,
        HOVERBOARD_RING_STATE_ACTIVE = 1
    };
    switch (((unsigned char *)moby)[0x20]) {
    case HOVERBOARD_RING_STATE_INIT:
        ((unsigned char *)moby)[0x30] = 0x80;
        *(unsigned short *)(moby + 0x34) |= 0x41;
        moby[0x20] = 1;
        break;
    case HOVERBOARD_RING_STATE_ACTIVE: {
        float v[4];
        int i;
        qcopy(v, moby + 0x10);
        v[3] = 8.0f;
        if (fast_bsphere_check((char *)v, 128.0f) != -1) {
            if (hero.state.control_mode == 0x16) {
                for (i = 0; i < 3; i++) {
                    float a = random_angle_radians();
                    float b = random_angle_radians();
                    FUN_L00_002743a8(a, b, (int)moby, D_L05_0015F580, scale_game_frames(0x28), 1,
                                     0x7F, 0);
                }
            }
        }
        break;
    }
    }
}
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math/vector.h"

/* Vars of the level 5 mine FUN_L05_0031c8e0 drives. */
struct MineVars_c8e0 {
    u8 pad0[0x60];
    s32 alpha;                     /* 0x60: glow alpha, faded out with camera distance */
    u8 glow_r, glow_g, glow_b;     /* 0x64: moby glow colour */
    u8 pad67;
    s32 color;                     /* 0x68: spark colour */
    u8 spark_r, spark_g, spark_b;  /* 0x6C */
    u8 max_alpha;                  /* 0x6F */
};

/* A spark effect FUN_L00_0026d000 returns. */
struct Spark_c8e0 {
    u8 pad0[3];
    u8 flags;                      /* 0x03 */
    u8 pad4[6];
    s16 life;                      /* 0x0A */
    u8 padC[0x14];
    struct SparkDraw_c8e0 {
        u8 pad0[4];
        s32 mode;                  /* 0x24 */
        u8 pad8[2];
        u8 fade;                   /* 0x2A */
        u8 life;                   /* 0x2B */
    } draw;
};

extern f32 D_L05_00161FC8_gp __asm__("D_L05_00161FC8") __attribute__((sda));
extern f32 D_L05_00161FD0 __attribute__((sda));
extern f32 D_L05_00161FD4 __attribute__((sda));
extern f32 vector_distance_c8e0(void *, void *) __asm__("FUN_001f9b80");
extern f32 distance_xyz_c8e0(void *, void *) __asm__("FUN_001f9b48");
extern f32 int_to_float_c8e0(s32) __asm__("FUN_001fa6c0");
extern s32 float_to_int_c8e0(f32) __asm__("FUN_001fa6d0");
extern void enqueue_callback_c8e0(void (*)(char *), struct Moby *) __asm__("FUN_001f4600");
extern char *find_moby_hit_c8e0(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern void scale_vector_xyz_c8e0(void *, void *, f32) __asm__("FUN_001f9a68");
extern void add_vector_xyz_c8e0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void play_sound_c8e0(s32, s32, struct Moby *) __asm__("FUN_0022da68");
extern s32 scale_game_frames_c8e0(s32) __asm__("FUN_001f96f8");
extern struct Moby *spawn_debris_c8e0(f32, struct Moby *, s32, void *, void *, s32, s32, void *, void *, void *) __asm__("FUN_L00_00263fd8");
extern void FUN_L01_002787a0_c8e0(struct Moby *) __asm__("FUN_L01_002787a0");
extern f32 fast_add_rotations_c8e0(f32, f32) __asm__("FUN_001fa580");
extern f32 random_float_between_c8e0(f32, f32) __asm__("FUN_002132a8");
extern s32 random_integer_below_c8e0(s32) __asm__("FUN_00213260");
extern f32 random_angle_c8e0(void) __asm__("FUN_00213308");
extern void build_spherical_offset_c8e0(void *, f32, f32, f32) __asm__("FUN_00214db0");
extern f32 random_float_c8e0(f32, f32) __asm__("FUN_L00_00257c48");
extern void spawn_trail_c8e0(void *, s32, f32, f32, s32, s32, s32, void *, f32) __asm__("FUN_L00_00272060");
extern struct Spark_c8e0 *spawn_spark_c8e0(void *, f32, f32, f32, s32, void *, f32, s32) __asm__("FUN_L00_0026d000");
extern s32 random_int_between_c8e0(s32, s32) __asm__("FUN_L00_00257b90");
extern void mark_moby_for_removal_c8e0(struct Moby *) __asm__("FUN_0020c828");
extern char *FUN_L05_0031d160(unsigned char *);
void FUN_L05_0031cf58(char *m);

/* Mine update: sets its glow, fades it out with camera distance and blows up into debris, smoke and sparks when something touches it. */
void FUN_L05_0031c8e0(struct Moby *moby);
void FUN_L05_0031c8e0(struct Moby *moby) {
    struct MineVars_c8e0 *vars = (struct MineVars_c8e0 *)moby->pvars;
    Vec4 tip;
    Vec4 vel;
    Vec4 dir;
    Vec4 start;
    f32 distance;
    f32 fade;
    f32 size;
    f32 speed;
    f32 yaw;
    f32 pitch;
    char *hit;
    struct Moby *debris;
    struct Spark_c8e0 *spark;
    struct SparkDraw_c8e0 *draw;
    s32 sign;
    s32 turn;
    s32 i;
    s32 frames;

    if (moby->state == 0) {
        moby->state = 1;
        moby->glow_color = (0x7F000000 | vars->glow_b << 16 | vars->glow_g << 8) | vars->glow_r;
        vars->color = vars->spark_b << 16 | vars->spark_g << 8 | vars->spark_r;
    }
    distance = vector_distance_c8e0(D_L05_001671C0, &moby->pos);
    if (48.0f <= distance) {
        return;
    }
    if (32.0f <= distance) {
        fade = (distance - 32.0f) * 0.0625f;
        vars->alpha = float_to_int_c8e0(int_to_float_c8e0(vars->max_alpha) * (1.0f - fade));
    } else {
        vars->alpha = vars->max_alpha;
    }
    enqueue_callback_c8e0(FUN_L05_0031cf58, moby);
    hit = find_moby_hit_c8e0(moby, 0x810000, 0);
    moby->hit_slot = 0xFF;
    if (hit == 0) {
        return;
    }
    scale_vector_xyz_c8e0(&tip, &moby->rot_row2, D_L05_00161FC8_gp);
    add_vector_xyz_c8e0(&tip, &moby->pos, &tip);
    if (!(*(s32 *)(hit + 0x24) & 0x800000) && !(distance_xyz_c8e0(&tip, hit) <= 0.8f)) {
        if ((*(struct Moby **)(hit + 0x20))->oclass != 0x47) {
            return;
        }
        if (!(distance_xyz_c8e0(&tip, &(*(struct Moby **)(hit + 0x20))->pos) <= 0.8f)) {
            return;
        }
    }
    play_sound_c8e0(0, 0, moby);
    FUN_L05_0031d160((unsigned char *)moby);
    debris = spawn_debris_c8e0(frame_time_sq * 12.0f, moby, 0x5ED, &moby->pos, &moby->rot, scale_game_frames_c8e0(0x5A), 0,
                               D_L05_0015F580, D_L05_0015F580, D_L05_0015F580);
    if (debris != 0) {
        debris->spawn_frame = moby->spawn_frame;
    }
    FUN_L01_002787a0_c8e0(moby);
    scale_vector_xyz_c8e0(&vel, &moby->rot_row0, D_L05_00161FD0 * frame_scale);
    vel.f[2] += D_L05_00161FD4 * frame_scale;
    debris = spawn_debris_c8e0(frame_time_sq * 12.0f, moby, 0x5EE, &moby->pos, &moby->rot, scale_game_frames_c8e0(0x5A), 0,
                               &vel, D_L05_0015F580, D_L05_0015F580);
    if (debris != 0) {
        debris->spawn_frame = moby->spawn_frame;
    }
    vel.f[0] = -vel.f[0];
    vel.f[1] = -vel.f[1];
    moby->rot.z = fast_add_rotations_c8e0(moby->rot.z, 3.1415927f);
    debris = spawn_debris_c8e0(frame_time_sq * 12.0f, moby, 0x5EE, &moby->pos, &moby->rot, scale_game_frames_c8e0(0x5A), 0,
                               &vel, D_L05_0015F580, D_L05_0015F580);
    if (debris != 0) {
        debris->spawn_frame = moby->spawn_frame;
    }
    for (i = 0; i < 24; i++) {
        size = random_float_between_c8e0(0.3f, 4.0f);
        sign = random_integer_below_c8e0(2);
        if (sign == 0) {
            sign--;
        }
        yaw = random_angle_c8e0();
        pitch = random_float_between_c8e0(0.34906584f, 1.5358897f);
        build_spherical_offset_c8e0(&dir, random_float_between_c8e0(0.05f, 0.1f) * frame_scale, yaw, pitch);
        qcopy(&start, &tip);
        start.f[0] += random_float_c8e0(0.0f, 0.15f);
        start.f[1] += random_float_c8e0(0.0f, 0.15f);
        start.f[2] += random_float_c8e0(0.0f, 0.15f);
        frames = scale_game_frames_c8e0(0x3C);
        spawn_trail_c8e0(&start, frames, size * 0.1f, size, vars->color | 0x7F000000, 0, sign, &dir, 0.003f);
        sign = -sign;
        frames = scale_game_frames_c8e0(0x3C);
        spawn_trail_c8e0(&start, frames, size * 0.07f, size * 0.7f, 0x7F7F7F7F, 1, sign, &dir, 0.003f);
        dir.f[1] = 0.0f;
        dir.f[0] = 0.0f;
        dir.f[2] = random_float_between_c8e0(0.01f, 0.05f) * frame_scale;
        speed = random_float_between_c8e0(60000.0f, 200000.0f);
        turn = random_integer_below_c8e0(2);
        spark = spawn_spark_c8e0(&tip, 0.3f, 1.0f, 1.02f, random_integer_below_c8e0(2) ? -turn : turn, &dir, speed, 0x407F7F7F);
        if (spark != 0) {
            spark->life = scale_game_frames_c8e0(random_int_between_c8e0(0x5A, 0x78));
            draw = &spark->draw;
            draw->mode = 2;
            draw->fade = random_int_between_c8e0(0x40, 0x60);
            draw->life = spark->life;
            if (random_integer_below_c8e0(2) != 0) {
                spark->flags = 0x44;
            }
        }
    }
    mark_moby_for_removal_c8e0(moby);
}
/* Draws the moby's camera-facing glow sprite, flickering its alpha at random. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0031E468), where it is exact; names translated to the US level program. */


typedef struct {
    float m[4][4];
    u32 col[4];
    float uv[8];
    u64 a, b, c, d;
} Pk_31e468;

extern float D_L05_00215C70[4][4];
extern int random_integer_below(int) __asm__("FUN_00213260");
extern f32 D_L05_00161FC8_gp __asm__("D_L05_00161FC8") __attribute__((sda));
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void add_vector_xyz(void *, void *, void *);
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void draw_geometry_quad(void *, void *, int);
extern void normalize_vector_xyz(void *, void *, float);
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
u64 get_effect_texture_alt(s32) __asm__("FUN_001f44b8");

extern short D_L05_00161FCC __attribute__((sda));
extern char D_L05_001671C0_lui[] __asm__("D_L05_001671C0") __attribute__((section(".data")));
void FUN_L05_0031cf58(char *m) {
    char *d = *(char **)(m + 0x78);
    Pk_31e468 pk;
    float up[4];
    float M[4][4];
    float t[4];
    float w[4];
    u32 col;
    int j;
    *(OvlQuad *)up = 0;
    up[2] = 1.0f;
    up[3] = 1.0f;
    pk.b = get_effect_texture_alt(0xB);
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
    scale_vector_xyz(t, m + 0xE0, *(float *)&D_L05_00161FC8_gp);
    add_vector_xyz(M[3], m + 0x10, t);
    subtract_vector_xyz(M[0], D_L05_001671C0_lui, M[3]);
    normalize_vector_xyz(M[0], M[0], 1.0f);
    cross_vectors_xyz(M[1], M[0], up);
    normalize_vector_xyz(M[1], M[1], 1.0f);
    cross_vectors_xyz(M[2], M[1], M[0]);
    scale_vector_xyz(w, M[0], *(float *)&D_L05_00161FCC);
    add_vector_xyz(M[3], M[3], w);
    if (((unsigned char *)d)[0x67] != 0 && random_integer_below(8) == 0) {
        *(int *)(d + 0x60) >>= 1;
    }
    col = (*(int *)(d + 0x60) << 24) | *(int *)(d + 0x68);
    pk.col[3] = col;
    pk.col[2] = col;
    pk.col[1] = col;
    pk.col[0] = col;
    for (j = 0; j < 4; j++) {
        scale_vector_xyz(pk.m[j], D_L05_00215C70[j], *(float *)(d + 0x70));
        transform_vector(pk.m[j], pk.m[j], M);
    }
    draw_geometry_quad(&pk, 0, 0);
}
extern char *FUN_0020c4f8_d160(int) __asm__("FUN_0020c4f8");
extern void FUN_L00_00250df8_d160(void *) __asm__("FUN_L00_00250df8");

char *FUN_L05_0031d160(unsigned char *source_moby) {
    unsigned char *m = (unsigned char *)FUN_0020c4f8_d160(0x5EA);
    if (m != 0) {
        m[0x30] = source_moby[0x30];
        *(unsigned short *)(m + 0x32) = *(unsigned short *)(source_moby + 0x32);
        m[0x31] = 1;
        *(float *)(m + 0x40) = *(float *)(source_moby + 0x40);
        *(float *)(m + 0x44) = *(float *)(source_moby + 0x44);
        *(float *)(m + 0x48) = *(float *)(source_moby + 0x48);
        *(long *)(m + 0x38) = *(long *)(source_moby + 0x38);
        qcopy(m + 0x10, source_moby + 0x10);
        FUN_L00_00250df8_d160(m);
    }
    return (char *)m;
}
/* Sets up a moby's vector from the level table and clamps its height for the current state. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0032A868), where it is exact; names translated to the US level program. */


extern float vector_distance(float *, float *) __asm__("FUN_001f9b80");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");

void FUN_L05_00329358(char *moby) {
    float v[4];
    char *d = *(char **)(moby + 0x70);
    float *p = (float *)(d + 0x80);
    *(OvlQuad *)v = 0;
    v[2] = 0.7f;
    transform_vector(v, v, &hero);
    add_vector_xyz(p, &hero.motion.pos, v);
    qcopy(d + 0x1D0, p);
    if (current_level_index == 5) {
        float w[4];
        *(OvlQuad *)w = 0;
        w[0] = 286.16f;
        w[1] = 447.82f;
        w[2] = 63.37f;
        w[3] = 1.0f;
        if (vector_distance(w, p) < 48.0f) {
            if (p[2] < 62.4f) {
                p[2] = 62.4f;
            }
        } else {
            if (p[2] < 67.2f) {
                p[2] = 67.2f;
            }
        }
    } else if (current_level_index == 0x10) {
        if (p[2] < 77.5f) {
            p[2] = 77.5f;
        }
    }
    normalize_vector_xyz(p + 8, &hero.gravity_dir, -1.0f);
}
extern char *D_L05_00167200 __attribute__((section(".data")));
void FUN_L05_003294c0(float a, float b, float c, float d, float e, float f, float g, float h) {
    char *o = D_L05_00167200;
    char *p = *(char **)(o + 0x70);

    *(float *)(p + 0x184) = a;
    *(float *)(p + 0x188) = b;
    *(float *)(p + 0x180) = c;
    *(float *)(p + 0x190) = d;
    *(float *)(p + 0x194) = e;
    *(float *)(p + 0x18C) = f;
    p = *(char **)(o + 0x70);
    if (g < 0.0f) {
        *(float *)(p + 0x10) = 0.01f;
    } else {
        *(float *)(p + 0x10) = g;
    }
    if (h < 0.0f) {
        *(float *)(p + 0x14) = 0.2f;
    } else {
        *(float *)(p + 0x14) = h;
    }
}
/* Places the camera moby relative to the hero's target and builds its look-at basis. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0032AA50), where it is exact; names translated to the US level program. */

extern void FUN_L05_00329358_c(void *) __asm__("FUN_L05_00329358");

void FUN_L05_00329540(char *m) {
    char *d = *(char **)(m + 0x70);
    char *at = m + 0x30;
    char *up = m + 0x10;
    char *side = m + 0x20;
    struct Hero *g;
    char *a;
    char *b;
    float target_height_offset[4];
    float look_direction[4];
    float look_target[4];
    float camera_offset[4];
    float gravity_up[4];
    FUN_L05_00329358_c(m);
    g = &hero;
    a = d + 0x80;
    b = d + 0xE0;
    normalize_vector_xyz(camera_offset, ((char *)g->moby) + 0xC0, 1.0f);
    normalize_vector_xyz(camera_offset, camera_offset, *(float *)(d + 0xE0));
    subtract_vector_xyz(at, a, camera_offset);
    normalize_vector_xyz(gravity_up, &g->gravity_dir, -1.0f);
    normalize_vector_xyz(camera_offset, gravity_up, *(float *)(b + 0x10));
    add_vector_xyz(at, at, camera_offset);
    normalize_vector_xyz(target_height_offset, gravity_up, *(float *)(a + 0x4C));
    add_vector_xyz(look_target, target_height_offset, a);
    subtract_vector_xyz(look_direction, look_target, at);
    normalize_vector_xyz(m, look_direction, 1.0f);
    cross_vectors_xyz(up, m, gravity_up);
    cross_vectors_xyz(side, up, m);
    normalize_vector_xyz(up, up, 1.0f);
    normalize_vector_xyz(side, side, 1.0f);
    qcopy(m + 0x40, m);
}
#include "qcopy.h"
#include "rnc/gameplay/entities/moby.h"

typedef struct {
    Vec4 pos;           /* 0x00 */
    u8 pad10[0x20];
    Vec4 side;          /* 0x30 */
    s32 state;          /* 0x40: 5 after a preset reset */
    s32 unk44;          /* 0x44 */
    f32 max_angle;      /* 0x48 */
    f32 dist;           /* 0x4C */
    u8 pad50[8];
    f32 unk58;          /* 0x58 */
    f32 unk5C;          /* 0x5C */
} CameraFocus;

/* View block at orbit + 0x30. */
typedef struct {
    Vec4 vel;           /* 0x00 */
    u8 pad10[0x10];
    f32 unk20;          /* 0x20 */
    f32 unk24;          /* 0x24 */
    s32 unk28;          /* 0x28 */
    s16 unk2C;          /* 0x2C */
    u8 pad2E[0x12];
    f32 aspect;         /* 0x40 */
    u8 pad44[0xC];
} CameraView;

#include "rnc/gameplay/camera/camera_spring.h"

typedef struct {
    Vec4 vel;           /* 0x00 */
    f32 unk10;          /* 0x10 */
    u8 pad14[0xC];
    Vec4 accel;         /* 0x20 */
    u8 pad30[4];
    f32 unk34;          /* 0x34 */
    f32 unk38;          /* 0x38 */
    s32 unk3C;          /* 0x3C */
    s32 unk40;          /* 0x40 */
    u8 pad44[0xC];
} CameraTrack;

/* Orbit state the camera keeps while it circles the hero. */
typedef struct {
    Vec4 axis;          /* 0x00 */
    f32 unk10;          /* 0x10: 0.01 unless the preset gives one (FUN_L05_003294c0) */
    f32 unk14;          /* 0x14: 0.2 unless the preset gives one */
    s32 unk18;          /* 0x18 */
    s16 mode;           /* 0x1C: 0 waiting, 1 orbiting */
    u8 pad1E[0x12];
    CameraView view;    /* 0x30 */
    CameraFocus focus;  /* 0x80 */
    CameraSpring spring; /* 0xE0 */
    CameraTrack track;  /* 0x100 */
    f32 goal[3];        /* 0x150: yaw, pitch, distance */
    f32 vel[3];         /* 0x15C */
    f32 accel[3];       /* 0x168 */
    f32 damp[3];        /* 0x174 */
    f32 dist;           /* 0x180 */
    f32 spring_dist;    /* 0x184 */
    f32 spring_height;  /* 0x188 */
    f32 unk18C;         /* 0x18C */
    f32 unk190;         /* 0x190 */
    f32 unk194;         /* 0x194 */
    u8 pad198[0x18];
    Vec4 unk1B0;        /* 0x1B0 */
    Vec4 unk1C0;        /* 0x1C0 */
} CameraOrbit;

typedef struct {
    u8 pad0[0x30];
    Vec4 pos;           /* 0x30 */
    u8 pad40[0x10];
    f32 yaw;            /* 0x50 */
    f32 pitch;          /* 0x54 */
    f32 dist;           /* 0x58 */
    u8 pad5C[0x14];
    CameraOrbit *orbit; /* 0x70 */
    u8 pad74[0xA];
    s16 unk7E;          /* 0x7E: cleared after a preset reset */
    u8 pad80[4];
    s16 preset;         /* 0x84: row of D_L05_0015EF50 */
} Camera;

/* Camera preset: distances, angle limits and damping. */
typedef struct {
    f32 unk0;           /* 0x00 */
    u8 pad4[0x1C];
    f32 unk20;          /* 0x20 */
    f32 unk24;          /* 0x24 */
    f32 spring_dist;    /* 0x28 */
    f32 spring_height;  /* 0x2C */
    f32 dist;           /* 0x30 */
    f32 unk34;          /* 0x34 */
    f32 unk38;          /* 0x38 */
    f32 unk3C;          /* 0x3C */
    f32 unk40;          /* 0x40 */
    f32 unk44;          /* 0x44 */
    s32 unk48;          /* 0x48 */
} CameraPreset;

typedef struct {
    u8 pad0[0x1C];
    CameraPreset *preset; /* 0x1C */
} CameraPresetRow;

extern CameraPresetRow *D_L05_0015EF50;
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");

/* Resets the camera from its preset (row cam->preset of D_L05_0015EF50): distances,
   angle limits and damping, cleared velocities, then places it. */
void FUN_L05_003296e0(Camera *cam) {
    CameraPreset *s = D_L05_0015EF50[cam->preset].preset;
    CameraOrbit *d;
    CameraFocus *focus;
    CameraSpring *spring;
    CameraOrbit *orbit;
    CameraTrack *track;
    CameraView *view;

    s->unk48 = 0;
    d = cam->orbit;
    d->spring_dist = s->spring_dist;
    d->spring_height = s->spring_height;
    d->dist = s->dist;
    d->unk190 = s->unk34;
    d->unk194 = s->unk38;
    d->unk18C = s->unk3C;
    clear_u64_value(&d->unk1B0);
    clear_u64_value(&d->unk1C0);
    focus = &cam->orbit->focus;
    focus->state = 5;
    focus->unk44 = D_001413D0[0];
    focus->dist = d->dist;
    focus->max_angle = 0.209439516f;
    focus->unk58 = 0.00174532924f;
    focus->unk5C = -0.0043633231f;
    spring = &cam->orbit->spring;
    spring->dist = d->spring_dist;
    spring->unk4 = 0.5f;
    spring->height = d->spring_height;
    orbit = cam->orbit;
    orbit->unk10 = 0.01f;
    orbit->unk14 = 0.2f;
    orbit->unk18 = 0;
    orbit->mode = 0;
    clear_u64_value(orbit);
    track = &cam->orbit->track;
    clear_u64_value(track);
    clear_u64_value(&track->accel);
    track->unk40 = 0;
    track->unk10 = s->unk0;
    track->unk34 = s->unk20;
    track->unk38 = s->unk24;
    track->unk3C = 0;
    view = &cam->orbit->view;
    view->unk20 = 0.8f;
    view->unk24 = 0.3f;
    view->aspect = 1.3333334f;
    view->unk28 = 0;
    view->unk2C = 0;
    clear_u64_value(view);
    FUN_L05_003294c0(s->spring_dist, s->spring_height, s->dist, s->unk34,
                     s->unk38, s->unk3C, s->unk40, s->unk44);
    FUN_L05_00329540((char *)cam);
    cam->unk7E = 0;
}

extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
extern f32 fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float fast_arc_sin(float) __asm__("FUN_001f9df8");
extern void rotate_about_axis(void *out, void *v, void *axis, f32 ang) __asm__("FUN_00214890");
extern float rope_spring_turn(void *, float, float, float, float, float) __asm__("FUN_L00_001eb328");
extern float FUN_001ebd78(void *, float, float, float, float, float);

/* Camera that orbits the hero while hero.unk88E is set, easing yaw, pitch and distance toward their goals. */
void FUN_L05_003298d8(Camera *cam) {
    Vec4 up;
    Vec4 side;
    Vec4 fwd;
    Vec4 rel;
    Vec4 along;
    Vec4 flat;
    Vec4 unit;
    Vec4 rot;
    CameraOrbit *o = cam->orbit;
    CameraFocus *focus = &o->focus;
    f32 *spring = o->goal;
    f32 dot;
    f32 len;
    f32 ang;
    f32 pitch;
    f32 tilt;

    normalize_vector_xyz(&up, &hero.gravity_dir, -1.0f);
    if (o->mode == 0 && hero.unk88E != 0) {
        o->mode = 1;
        cross_vectors_xyz(&side, &hero.moby->rot_row2, &up);
        normalize_vector_xyz(&side, &side, 1.0f);
        cross_vectors_xyz(&fwd, &up, &side);
        normalize_vector_xyz(&fwd, &fwd, 1.0f);
        subtract_vector_xyz(&rel, &cam->pos, focus);
        dot = fast_vec_dot(&rel, &up);
        normalize_vector_xyz(&along, &up, dot);
        subtract_vector_xyz(&flat, &rel, &along);
        dot = fast_vec_dot(&fwd, &flat);
        len = vector_length_xyz(&flat);
        if (len == 0.0f) {
            len = 0.0001f;
        }
        ang = 1.5707964f - fast_arc_sin(dot / len);
        normalize_vector_xyz(&unit, &flat, 1.0f);
        if (fast_vec_dot(&side, &unit) < 0.0f) {
            ang = -ang;
        }
        cam->yaw = ang;
        rotate_about_axis(&rot, &fwd, &up, ang);
        dot = fast_vec_dot(&rot, &rel);
        len = vector_length_xyz(&rel);
        if (len == 0.0f) {
            len = 0.0001f;
        }
        tilt = 1.5707964f - fast_arc_sin(dot / len);
        normalize_vector_xyz(&unit, &rel, 1.0f);
        pitch = -tilt;
        if (fast_vec_dot(&up, &unit) < 0.0f) {
            pitch = tilt;
        }
        cam->pitch = pitch;
        cam->dist = vector_length_xyz(&rel);
        qcopy(&o->focus.side, &fwd);
        spring[0] = 3.1415927f;
        spring[1] = -1.5358897f;
        spring[2] = 5.0f;
        spring[3] = 0.0f;
        spring[4] = 0.0f;
        spring[5] = 0.0f;
        spring[6] = 0.005f;
        spring[9] = 0.2f;
        spring[7] = 0.005f;
        spring[10] = 0.2f;
        spring[8] = 0.005f;
        spring[11] = 0.2f;
    }
    if (o->mode == 1) {
        normalize_vector_xyz(&side, &focus->side, cam->dist);
        rotate_about_axis(&side, &side, &up, cam->yaw);
        cross_vectors_xyz(&fwd, &side, &up);
        normalize_vector_xyz(&fwd, &fwd, 1.0f);
        rotate_about_axis(&side, &side, &fwd, cam->pitch);
        add_vector_xyz(&cam->pos, focus, &side);
        cam->yaw = rope_spring_turn(&spring[3], cam->yaw, spring[0], spring[6], spring[9], 0.0f);
        cam->pitch = rope_spring_turn(&spring[4], cam->pitch, spring[1], spring[7], spring[10], 0.0f);
        cam->dist = FUN_001ebd78(&spring[5], cam->dist, spring[2], spring[8], spring[11], 0.0f);
        if (hero.unk88E == 0) {
            o->mode = 0;
            scale_vector_xyz(o, o, -1.0f);
        }
    }
}
/* Camera follow: springs the position toward the focus, keeps clear of walls, eases distance and height, then rebuilds the basis. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0032B1F8), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) V_32B1F8;

typedef struct {
    V_32B1F8 pos;
    char pad10[0x10];
    V_32B1F8 axis;
    char pad30[0x1C];
    float dist;
} Focus_32B1F8;

typedef struct {
    float dist;
    char pad4[0xC];
    float height;
} Spring_32B1F8;

typedef struct {
    V_32B1F8 vel;
    char pad10[0x10];
    float state[4];
    char pad30[4];
    float k34, k38, k3C;
    float blend;
} Track_32B1F8;

typedef struct {
    float vel[4];
    float k10, k14, k18;
    short mode;
    char pad1E[0x62];
    Focus_32B1F8 focus;
    char padD0[0x10];
    Spring_32B1F8 spring;
    char padF4[0xC];
    Track_32B1F8 track;
    char pad150[0x30];
    float dist;
    float spring_dist;
    float spring_height;
    float focus_dist;
    float spring_dist2;
    float spring_height2;
    char pad198[8];
    V_32B1F8 aim;
    V_32B1F8 bias;
    float bias_vel[4];
    V_32B1F8 next;
} Orbit_32B1F8;

typedef struct {
    V_32B1F8 fwd;
    V_32B1F8 up;
    V_32B1F8 side;
    V_32B1F8 pos;
    char pad40[0x24];
    float vel[3];
    Orbit_32B1F8 *orbit;
    char pad74[0x10];
    short preset;
} Camera_32B1F8;

typedef struct { char pad0[0x48]; float vel; } Preset_32B1F8;

typedef struct { char pad0[0x1C]; Preset_32B1F8 *preset; } Row_32B1F8;

extern f32 dot_vectors_xyz(void *, void *) __asm__("func_001F9AB0");
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
f32 cam_interp_values(void *, f32, f32, f32, f32, f32) __asm__("FUN_001ebd78");
float absolute_float(float input) __asm__("func_001F99C0");
extern char D_0013E633_32B1F8[] __asm__("D_0013F5E0");
extern Row_32B1F8 *D_L05_0015F050_32B1F8 __asm__("D_L05_0015EF50") __attribute__((section(".sdata")));
extern float D_L05_00162268_32B1F8 __asm__("D_L05_001621A8") __attribute__((sda));
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern float spring_32B1F8(float, float, float *, float, float, float) __asm__("FUN_001ebd78");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern int collide_32B1F8(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern float fabs_32B1F8(float) __asm__("FUN_001f99c0");
extern void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void FUN_L05_003298d8_c(Camera_32B1F8 *) __asm__("FUN_L05_003298d8");

extern float D_L05_00174278 __attribute__((section(".data")));
void FUN_L05_00329ce8(Camera_32B1F8 *camera) {
    V_32B1F8 down;
    V_32B1F8 focus;
    V_32B1F8 rel;
    V_32B1F8 lateral;
    V_32B1F8 radial;
    V_32B1F8 base;
    V_32B1F8 goal;
    V_32B1F8 offset;
    V_32B1F8 probe;
    V_32B1F8 step;
    Orbit_32B1F8 *orbit = camera->orbit;
    char *up_axis = D_0013E633_32B1F8;
    Preset_32B1F8 *preset;
    Focus_32B1F8 *focus_state;
    Track_32B1F8 *track;
    float focus_distance;
    Spring_32B1F8 *spring;
    float *vel;
    float along;
    V_32B1F8 *pos;
    int probe_step;
    int orbiting = 0;

    preset = D_L05_0015F050_32B1F8[camera->preset].preset;
    track = &orbit->track;
    focus_distance = orbit->dist;
    focus_state = &orbit->focus;
    spring = &orbit->spring;
    fast_vec_normalize(&down, up_axis, -1.0f);
    qcopy(&focus, &focus_state->pos);
    if (orbit->mode == 0) {
        fast_vec_sub(&rel, pos = &camera->pos, &focus);
        probe_step = 0;
        along = fast_vec_dot(&rel, &focus_state->axis);
        fast_vec_normalize(&offset, &focus_state->axis, along);
        fast_vec_sub(&lateral, &rel, &offset);
        fast_vec_normalize(&radial, &lateral, spring->dist);
        fast_vec_add(&base, &focus, &radial);
        fast_vec_normalize(&offset, &focus_state->axis, spring->height);
        fast_vec_add(&goal, &base, &offset);
        fast_vec_add(&goal, &goal, &orbit->bias);
        camera->pos.x = spring_32B1F8(camera->pos.x, goal.x, &orbit->vel[0], orbit->k10, orbit->k14, orbit->k18);
        camera->pos.y = spring_32B1F8(camera->pos.y, goal.y, &orbit->vel[1], orbit->k10, orbit->k14, orbit->k18);
        camera->pos.z = spring_32B1F8(camera->pos.z, goal.z, &orbit->vel[2], orbit->k10, orbit->k14, orbit->k18);
        vel = camera->vel;
        probe.x = vel[0];
        probe.y = vel[1];
        probe.z = vel[2];
        *(int *)&probe.w = 0;
        fast_vec_sub(&step, pos, &probe);
        fast_vec_scale(&step, &step, 0.16666667f);
        while (probe_step < 6) {
            if (collide_32B1F8(0.5f, &probe, 18, 0) != 0) {
                spring->height = spring_32B1F8(spring->height, D_L05_00174278 - focus.z, &preset->vel, 0.05f, 0.2f, 0.0f);
                break;
            }
            probe_step++;
            fast_vec_add(&probe, &probe, &step);
        }
        if (probe_step == 6) {
            orbit->bias.x = spring_32B1F8(orbit->bias.x, 0.0f, &orbit->bias_vel[0], 0.02f, 0.2f, 0.05f);
            orbit->bias.y = spring_32B1F8(orbit->bias.y, 0.0f, &orbit->bias_vel[1], 0.02f, 0.2f, 0.05f);
            orbit->bias.z = spring_32B1F8(orbit->bias.z, 0.0f, &orbit->bias_vel[2], 0.02f, 0.2f, 0.05f);
            spring->height = spring_32B1F8(spring->height, orbit->spring_height, &preset->vel, 0.005f, 0.2f, 0.0f);
        }
        if (((Hero *)(D_0013E633_32B1F8 - 0x290))->air_frames.s != 0 && 2.0f < ((Hero *)(D_0013E633_32B1F8 - 0x290))->ground_distance) {
            if (fabs_32B1F8(spring->dist - orbit->spring_dist2) < 0.015f) spring->dist = orbit->spring_dist2;
            else if (spring->dist < orbit->spring_dist2) spring->dist = spring->dist + 0.015f;
            else spring->dist = spring->dist - 0.015f;
            if (probe_step == 6) {
                if (fabs_32B1F8(spring->height - orbit->spring_height2) < 0.05f) spring->height = orbit->spring_height2;
                else if (spring->height < orbit->spring_height2) spring->height = spring->height + 0.05f;
                else spring->height = spring->height - 0.05f;
            }
            if (fabs_32B1F8(focus_state->dist - orbit->focus_dist) < 0.02f) focus_state->dist = orbit->focus_dist;
            else if (focus_state->dist < orbit->focus_dist) focus_state->dist = focus_state->dist + 0.02f;
            else focus_state->dist = focus_state->dist - 0.02f;
        } else {
            if (fabs_32B1F8(spring->dist - orbit->spring_dist) < 0.03f) spring->dist = orbit->spring_dist;
            else if (spring->dist < orbit->spring_dist) spring->dist = spring->dist + 0.03f;
            else spring->dist = spring->dist - 0.03f;
            if (fabs_32B1F8(spring->height - orbit->spring_height) < 0.1f) spring->height = orbit->spring_height;
            else if (spring->height < orbit->spring_height) spring->height = spring->height + 0.1f;
            else spring->height = spring->height - 0.1f;
            if (fabs_32B1F8(focus_state->dist - focus_distance) < 0.04f) focus_state->dist = focus_distance;
            else if (focus_state->dist < focus_distance) focus_state->dist = focus_state->dist + 0.04f;
            else focus_state->dist = focus_state->dist - 0.04f;
        }
        orbiting = 1;
    } else {
        qcopy(&orbit->aim, *(char **)(up_axis + 0x1DF0) + 0xE0);
        if (fabs_32B1F8(spring->dist - orbit->spring_dist) < 0.015f) spring->dist = orbit->spring_dist;
        else if (spring->dist < orbit->spring_dist) spring->dist = spring->dist + 0.015f;
        else spring->dist = spring->dist - 0.015f;
        if (fabs_32B1F8(spring->height - orbit->spring_height) < 0.05f) spring->height = orbit->spring_height;
        else if (spring->height < orbit->spring_height) spring->height = spring->height + 0.05f;
        else spring->height = spring->height - 0.05f;
        if (fabs_32B1F8(focus_state->dist - focus_distance) < 0.02f) focus_state->dist = focus_distance;
        else if (focus_state->dist < focus_distance) focus_state->dist = focus_state->dist + 0.02f;
        else focus_state->dist = focus_state->dist - 0.02f;
    }
    FUN_L05_003298d8_c(camera);
    qcopy(&focus_state->pos, &orbit->next);
    qcopy(&focus, &focus_state->pos);
    fast_vec_normalize(&offset, &focus_state->axis, focus_state->dist);
    fast_vec_add(&goal, &focus, &offset);
    fast_vec_sub(&rel, &goal, &camera->pos);
    fast_vec_normalize(&rel, &rel, 1.0f);
    along = fast_vec_dot(&camera->fwd, &rel);
    fast_vec_cross(&camera->up, &rel, &down);
    fast_vec_normalize(&camera->up, &camera->up, 1.0f);
    fast_vec_cross(&camera->side, &camera->up, &rel);
    if (orbiting == 0 || fabs_32B1F8(along) < 0.5f || (vel = (float *)(D_0013E633_32B1F8 - 0x290), ((Hero *)vel)->unk88D != 0)) {
        track->blend = spring_32B1F8(track->blend, 0.0f, track->state, track->k34, track->k38, track->k3C);
    } else {
        float tilt;
        fast_vec_normalize(&probe, ((char *)((Hero *)vel)->moby) + 0xC0, 1.0f);
        tilt = fast_vec_dot(&probe, &camera->side);
        fast_vec_normalize(&step, &camera->side, tilt);
        fast_vec_sub(&probe, &probe, &step);
        tilt = fast_vec_dot(&probe, &camera->up);
        track->blend = spring_32B1F8(track->blend, tilt * D_L05_00162268_32B1F8, track->state, track->k34, track->k38, track->k3C);
    }
    if (0.00001f < fabs_32B1F8(track->blend)) {
        fast_vec_normalize(&probe, &camera->up, track->blend);
        fast_vec_add(&goal, &goal, &probe);
    }
    fast_vec_sub(&rel, &goal, &camera->pos);
    fast_vec_normalize(&camera->fwd, &rel, 1.0f);
    fast_vec_cross(&camera->up, &camera->fwd, &down);
    fast_vec_normalize(&camera->up, &camera->up, 1.0f);
    fast_vec_cross(&camera->side, &camera->up, &camera->fwd);
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002CF2C0.c: func_L05_0032BBF0), where it is exact; names translated to the US level program. */

extern void extract_matrix_rotation_quaternion(void *, void *) __asm__("FUN_002144d8");
extern void FUN_L05_003296e0(Camera *);

extern char *D_L05_00167204 __attribute__((section(".data")));
void FUN_L05_0032a6e0(char *out) {
    char *src = D_L05_00167204;
    qcopy(out + 0x30, src + 0x30);
    qcopy(out, src);
    qcopy(out + 0x10, src + 0x10);
    qcopy(out + 0x20, src + 0x20);
    extract_matrix_rotation_quaternion(out + 0x40, out);
    FUN_L05_003296e0((Camera *)out);
}
