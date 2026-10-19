/* Level 14 moby classes: shuttle, mine, elevator, taxi, qwark_oltanis, jet, rocket. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/game_globals.h"
#define MOBY(p) ((struct Moby *)(p))

/* 0x002DF080, 180 bytes.  Initialises two tables of spaced vectors in the
 * moby's data block and clears four fields.
 * Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c:
 * func_L14_002E0480), where it is exact; names translated to the US level
 * program. */
void FUN_L14_002df080(u8 *moby) {
    f32 *d = *(f32 **)(moby + 0x78);
    f32 *p = d;
    s32 i;
    for (i = 0; i < 10; i++) {
        p[0] = d[0x1F0 / 4] * (f32)i;
        p[1] = p[2] = 0.0f;
        p[3] = 1.0f;
        p += 4;
    }
    i = 0;
    p = d;
    for (; i < 20; i++) {
        p[0xA0 / 4] = (f32)i * 0.39f;
        p[0xA4 / 4] = p[0xA8 / 4] = 0.0f;
        p[0xAC / 4] = 1.0f;
        p += 4;
    }
    d[0x1E0 / 4] = 0.0f;
    d[0x1E4 / 4] = 0.0f;
    d[0x1E8 / 4] = 0.0f;
    d[0x1EC / 4] = 0.0f;
}
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float integer_to_float_l14(int) __asm__("FUN_001fa6c0");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern float spread_l14(float, float) __asm__("FUN_L00_00257c48");
extern void enqueue_callback_l14(void *, void *) __asm__("FUN_001f4600");
void FUN_L14_002df458(struct Moby *);

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
void FUN_L14_002df138(u8 *moby) {
    f32 *d = *(f32 **)(moby + 0x78);
    f32 step;
    f32 a;
    f32 b;
    f32 t;
    f32 u;
    f32 c;
    s32 i;
    s32 j;
    s32 k;
    f32 *rp;
    f32 *wp;
    f32 sixteen;
    f32 one;
    j = 4;
    d[0x1E0 / 4] = fast_add_rotations(d[0x1E0 / 4], 0.17453292f);
    d[0x1E4 / 4] = fast_subtract_rotations(d[0x1E4 / 4], 0.80285144f);
    d[0x1E8 / 4] = fast_add_rotations(d[0x1E8 / 4], 0.05236f);
    step = (360.0f / (integer_to_float_l14(10) * 0.5f)) * 0.017453292f;
    for (i = 0; i < 5; i++) {
        t = integer_to_float_l14(i) / integer_to_float_l14(10);
        if (i == 0) {
            b = 0.0f;
        } else {
            a = (f32)i;
            b = fast_sin(normalize_angle(d[0x1E0 / 4] + (a * step))) * 0.1f;
            b += spread_l14(0.0f, 0.2f);
        }
        d[(i * 4) + 2] = b;
        d[(i * 4) + 1] = 0.0f;
        d[(i * 4) + 0] = (d[0x1F0 / 4] * ((f32)i)) + 1.6f;
        d[(i * 4) + 3] = 1.0f;
    }
    k = 5;
    rp = (f32 *)((j * 16) + (u32)(d + 2));
    wp = &d[20];
    sixteen = 1.6f;
    one = 1.0f;
    /* Fakematch: the extra loop level rematerialises the 4 and ranks the read
       pointer above the counter in register allocation, as in retail. */
    do {
        for (; k < 10; k++) {
            wp[2] = *rp;
            wp[1] = 0.0f;
            wp[0] = (d[0x1F0 / 4] * ((f32)k)) + sixteen;
            wp[3] = one;
            wp += 4;
            rp -= 4;
        }
    } while (0);
    for (i = 0; i < 19; i++) {
        c = (f32)i;
        d[(i * 4) + (0xA8 / 4)] = 0.0f;
        t = normalize_angle(d[0x1E4 / 4] + (c * 0.99483764f));
        u = fast_sin(normalize_angle(d[0x1E8 / 4] + (c * 0.20944f))) * 0.75f;
        d[(i * 4) + (0xA8 / 4)] += (u * fast_sin(t)) + spread_l14(0.0f, 0.2f);
        d[(i * 4) + (0xA4 / 4)] = fast_cos(t) * 0.5f;
        d[(i * 4) + (0xA0 / 4)] = c * 0.39f;
        d[(i * 4) + (0xAC / 4)] = 1.0f;
    }
    enqueue_callback_l14(FUN_L14_002df458, moby);
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002E0858), where it is exact; names translated to the US level program. */

extern void FUN_L14_002df510(struct Moby *, void *, int, int, int);
void vu1_add_g_sregister(s32 a0, s64 a1) __asm__("FUN_00233980");

void FUN_L14_002df458(struct Moby *m) {
    char *d = (char *)m->pvars;
    float v = *(float *)(d + 0x1EC) - frame_scale * 0.2f;
    *(float *)(d + 0x1EC) = v;
    if (v <= -8.0f) {
        *(float *)(d + 0x1EC) = v + 8.0f;
    }
    vu1_add_g_sregister(0x47, 0x51001);
    FUN_L14_002df510(m, d, 0x80, 0x40, 10);
    FUN_L14_002df510(m, d + 0xA0, 0x30, 0x10, 0x14);
}
/* Draws a two-layer ribbon along a path of points: each segment builds a basis from the segment direction and
 * the camera, sweeps two textured quads (an outer glow and an inner core) across it and, for the alpha-0x10
 * variant, brightens the first and last segments. The texture scrolls with the moby's pvars+0x1EC. */
#include "qcopy.h"
typedef int u128_df510 __attribute__((mode(TI)));
typedef struct { float x, y, z, w; } __attribute__((aligned(16))) V_DF510;
typedef struct {
    V_DF510 v[4];
    int col[4];
    float uv[4][2];
    long r70;
    long tex;
    long r80;
    long r88;
} Q_DF510;
extern char D_L14_001674C0[];
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern f32 fast_cos_df510(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin_df510(f32) __asm__("FUN_001f9de0");
extern void euler_matrix_df510(void *, void *) __asm__("FUN_001fa050");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");

void FUN_L14_002df510(struct Moby *m, void *path, int outer_alpha, int inner_alpha, int n) {
    V_DF510 *pts = (V_DF510 *)path;
    Q_DF510 q[2];
    V_DF510 w[2];
    V_DF510 t;
    V_DF510 mat[4];
    V_DF510 wm[4];
    V_DF510 p[2];
    char *d = (char *)m->pvars;
    int c;
    int i;

    q[0].tex = get_effect_texture(0x2D);
    q[0].r88 = 0x8000000048L;
    q[0].r80 = 0xFF9000000260L;
    q[0].r70 = 0;
    q[0].col[0] = q[0].col[1] = q[0].col[2] = q[0].col[3] = (outer_alpha << 24) | 0x7F7F7F;
    q[1].tex = get_effect_texture(0x2C);
    c = (inner_alpha << 24) | 0x7F4040;
    q[1].col[3] = c;
    q[1].col[2] = c;
    q[1].col[1] = c;
    q[1].col[0] = c;
    q[1].r88 = q[0].r88;
    q[1].r80 = q[0].r80;
    q[1].r70 = q[0].r70;
    q[1].uv[0][0] = 1.0f;
    q[1].uv[0][1] = 0.0f;
    q[1].uv[1][0] = 1.0f;
    q[1].uv[1][1] = 1.0f;
    q[1].uv[2][0] = 0.0f;
    q[1].uv[2][1] = 0.0f;
    q[1].uv[3][0] = 0.0f;
    q[1].uv[3][1] = 1.0f;
    q[0].uv[0][0] = *(float *)(d + 0x1EC) + 0.0f;
    q[0].uv[0][1] = 0.0f;
    q[0].uv[1][0] = *(float *)(d + 0x1EC) + 0.0f;
    q[0].uv[1][1] = 1.0f;
    q[0].uv[2][0] = *(float *)(d + 0x1EC) + 1.0f;
    q[0].uv[2][1] = 0.0f;
    q[0].uv[3][0] = *(float *)(d + 0x1EC) + 1.0f;
    q[0].uv[3][1] = 1.0f;
    *(u128_df510 *)&w[0] = 0;
    w[0].w = 1.0f;
    *(u128_df510 *)&w[1] = 0;
    w[0].z = 0.2f;
    w[1].z = 0.4f;
    w[1].w = 1.0f;
    t.x = fast_cos_df510(m->rot.z);
    t.y = fast_sin_df510(m->rot.z);
    t.z = 0.0f;
    qcopy(&mat[0], &t);
    subtract_vector_xyz_c2(&mat[1], D_L14_001674C0, &m->pos);
    scale_vector_xyz(&mat[1], &mat[1], 1.0f);
    cross_vectors_xyz(&mat[2], &mat[1], &mat[0]);
    normalize_vector_xyz(&mat[2], &mat[2], 1.0f);
    cross_vectors_xyz(&mat[0], &mat[2], &mat[1]);
    qcopy(&mat[3], &m->pos);
    euler_matrix_df510(&wm[0], &m->rot);
    qcopy(&wm[3], &m->pos);
    qcopy(&q[0].v[0], &w[0]);
    scale_vector_xyz(&q[0].v[1], &q[0].v[0], -1.0f);
    qcopy(&q[1].v[0], &w[1]);
    scale_vector_xyz(&q[1].v[1], &q[1].v[0], -1.0f);
    transform_vector(&p[0], &pts[0], &wm[0]);
    transform_vector(&p[1], &pts[1], &wm[0]);
    subtract_vector_xyz_c2(&mat[0], &p[1], &p[0]);
    normalize_vector_xyz(&mat[0], &mat[0], 1.0f);
    subtract_vector_xyz_c2(&mat[1], D_L14_001674C0, &p[0]);
    cross_vectors_xyz(&mat[2], &mat[1], &mat[0]);
    normalize_vector_xyz(&mat[2], &mat[2], 1.0f);
    cross_vectors_xyz(&mat[1], &mat[0], &mat[2]);
    qcopy(&mat[3], &p[0]);
    transform_vector(&q[0].v[0], &q[0].v[0], &mat[0]);
    transform_vector(&q[0].v[1], &q[0].v[1], &mat[0]);
    transform_vector(&q[1].v[0], &q[1].v[0], &mat[0]);
    transform_vector(&q[1].v[1], &q[1].v[1], &mat[0]);
    for (i = 1; i < n; i++) {
        float f;
        qcopy(&q[0].v[2], &w[0]);
        scale_vector_xyz(&q[0].v[3], &q[0].v[2], -1.0f);
        qcopy(&q[1].v[2], &w[1]);
        scale_vector_xyz(&q[1].v[3], &q[1].v[2], -1.0f);
        if (i == n - 1) {
            transform_vector(&mat[3], &pts[i], &wm[0]);
        } else {
            transform_vector(&p[0], &pts[i], &wm[0]);
            transform_vector(&p[1], &pts[i + 1], &wm[0]);
            subtract_vector_xyz_c2(&mat[0], &p[1], &p[0]);
            normalize_vector_xyz(&mat[0], &mat[0], 1.0f);
            subtract_vector_xyz_c2(&mat[1], D_L14_001674C0, &p[0]);
            cross_vectors_xyz(&mat[2], &mat[1], &mat[0]);
            normalize_vector_xyz(&mat[2], &mat[2], 1.0f);
            cross_vectors_xyz(&mat[1], &mat[0], &mat[2]);
            qcopy(&mat[3], &p[0]);
        }
        transform_vector(&q[0].v[2], &q[0].v[2], &mat[0]);
        transform_vector(&q[0].v[3], &q[0].v[3], &mat[0]);
        transform_vector(&q[1].v[2], &q[1].v[2], &mat[0]);
        transform_vector(&q[1].v[3], &q[1].v[3], &mat[0]);
        if (i == 1 && inner_alpha == 0x10) {
            int c0 = q[0].col[0];
            int c1 = q[1].col[0];
            q[0].col[0] = q[0].col[1] = 0xFFFFFF;
            q[1].col[0] = q[1].col[1] = 0x7F4040;
            draw_geometry_quad(&q[0], 0, 0);
            draw_geometry_quad(&q[1], 0, 0);
            q[0].col[3] = c0;
            q[0].col[2] = c0;
            q[0].col[1] = c0;
            q[0].col[0] = c0;
            q[1].col[3] = c1;
            q[1].col[2] = c1;
            q[1].col[1] = c1;
            q[1].col[0] = c1;
        } else if (i == n - 2 && inner_alpha == 0x10) {
            int c0 = q[0].col[2];
            int c1 = q[1].col[2];
            q[0].col[2] = q[0].col[3] = 0xFFFFFF;
            q[1].col[2] = q[1].col[3] = 0x7F4040;
            draw_geometry_quad(&q[0], 0, 0);
            draw_geometry_quad(&q[1], 0, 0);
            q[0].col[3] = c0;
            q[0].col[2] = c0;
            q[0].col[1] = c0;
            q[0].col[0] = c0;
            q[1].col[3] = c1;
            q[1].col[2] = c1;
            q[1].col[1] = c1;
            q[1].col[0] = c1;
        } else {
            draw_geometry_quad(&q[0], 0, 0);
            draw_geometry_quad(&q[1], 0, 0);
        }
        qcopy(&q[0].v[0], &q[0].v[2]);
        qcopy(&q[0].v[1], &q[0].v[3]);
        qcopy(&q[1].v[0], &q[1].v[2]);
        qcopy(&q[1].v[1], &q[1].v[3]);
        f = cubic_hermite_scalar(-1.0f, 0.0f, 1.0f, 0.0f, convert_integer_to_float(i) / convert_integer_to_float(n)) * 0.0f + 0.2f;
        w[0].z = f;
        w[1].z = 2.0f * f;
        q[0].uv[0][0] = *(float *)(d + 0x1EC) + 0.0f;
        q[0].uv[0][1] = 0.0f;
        q[0].uv[1][0] = *(float *)(d + 0x1EC) + 0.0f;
        q[0].uv[1][1] = 1.0f;
        q[0].uv[2][0] = *(float *)(d + 0x1EC) + 1.0f;
        q[0].uv[2][1] = 0.0f;
        q[0].uv[3][0] = *(float *)(d + 0x1EC) + 1.0f;
        q[0].uv[3][1] = 1.0f;
    }
}
extern s32 is_point_inside_clip_volume(u8 *base, s32 arg) __asm__("FUN_00214720");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void FUN_L00_00259a88(s32, void *);

typedef struct {
    f32 v[4];
    u8 *moby;
    s32 flags;
    s8 a;
    s8 b;
    s16 c;
    f32 range;
    s32 one;
} Level14Req;

/* 0x002DFC58, 184 bytes.  If the moby's check passes, builds an effect
 * request on the stack and submits it.
 * Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c:
 * func_L14_002E1058), where it is exact; names translated to the US level
 * program. */
void FUN_L14_002dfc58(u8 *moby) {
    u8 *base = (u8 *)&hero.motion.pos;
    Level14Req r;
    u8 *q;
    if (is_point_inside_clip_volume(base, *(s32 *)(*(u8 **)(moby + 0x78) + 0x200)) != 0) {
        r.flags = 0x200001;
        r.range = 100.0f;
        r.one = 1;
        r.moby = moby;
        clear_u64_value(&r);
        r.v[2] = 1.0f;
        r.v[3] = 5627.925f;
        r.a = 5;
        r.b = 4;
        r.c = *(u16 *)(moby + 0xA6);
        q = base - 0x80;
        *(s32 *)(q + 0x22A8) = 0;
        FUN_L00_00259a88(*(s32 *)(q + 0x2080), &r);
    }
}
/* pvars of the lift that carries a rider moby and pushes the hero off its edge */
typedef struct {
    u8 pad0[0x74];
    s32 path;          /* 0x74: path index into D_L14_001B0BB0 */
    u8 pad78[0x84];
    f32 height;        /* 0xFC */
    u8 pad100[0x50];
    s32 rider;         /* 0x150: moby index carried along */
    s32 anchor;        /* 0x154: joint the rider follows, -1 for the moby position */
    s32 mode;          /* 0x158: 0x3039 when the rider is attached */
    s32 sound;         /* 0x15C */
    f32 offset[4];     /* 0x160 */
    u8 pad170[0x10];
    u8 spring[0x40];   /* 0x180 */
    u8 manip[0x10];    /* 0x1C0 */
    u8 wobble[0x30];   /* 0x1D0 */
    f32 angle;         /* 0x200 */
    s16 riding;        /* 0x204 */
    s16 h206;          /* 0x206 */
    u8 pad208[4];
    s16 timer;         /* 0x20C */
    s16 pad20E;
    f32 timer_inv;     /* 0x210 */
    f32 from;          /* 0x214 */
    f32 to;            /* 0x218 */
    s32 clip;          /* 0x21C */
} L14LiftVars;

extern struct Moby *D_L14_0015FFD8_lift __asm__("D_L14_0015FFD8");
extern char *D_L14_001B0BB0_lift[] __asm__("D_L14_001B0BB0");
extern char D_L14_001FC840[];
extern s32 D_L14_00161B8C __attribute__((sda));
extern s32 D_L14_00161B90 __attribute__((sda));
extern f32 D_L14_00161B88 __attribute__((sda));
extern f32 D_0015ED6C_lift __asm__("D_0015ED6C");
extern int lift_inside(void *, int) __asm__("FUN_00214720");
extern void attach_manipulator(void *arg0, s32 arg1, void *arg2) __asm__("FUN_0020cb10");
extern void lift_print(const char *, short) __asm__("FUN_001e93b0");
extern void lift_remove(void *) __asm__("FUN_0020c828");
extern void gunship_follow_path(void *) __asm__("FUN_L01_002f5168");
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void lift_xform(void *, void *, void *) __asm__("FUN_001f9d20");
extern void lift_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void lift_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int lift_sound(int, int, void *) __asm__("FUN_0022da68");
extern float lift_add_rot(float, float) __asm__("FUN_001fa580");
extern void FUN_L00_001fff28(void *, int, float);
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern int lift_frames(int) __asm__("FUN_001f96f8");
extern float lift_itof(int) __asm__("FUN_001fa6c0");
extern int lift_tick(short *) __asm__("FUN_001f9770");
extern float lift_length(void *) __asm__("FUN_001f9af0");
extern void lift_normalize(void *, void *, float) __asm__("FUN_001f9bf8");

void shuttle_update(struct Moby *m) __asm__("FUN_L14_002e7bd8");
void shuttle_update(struct Moby *m) {
    L14LiftVars *d;
    Vec4 old;
    Vec4 rot;
    Vec4 delta;
    Vec4 base;
    Vec4 off;
    f32 mat[16];
    struct Moby *rider;
    f32 len;
    f32 max;

    if (m == 0)
        return;
    d = (L14LiftVars *)m->pvars;
    if (d == 0)
        return;
    if (d->clip >= 0 && lift_inside(&hero.motion.pos, d->clip) != 0) {
        m->collision = 0;
        m->flags |= 0x41;
        if (d->rider >= 0) {
            struct Moby *r = &D_L14_0015FFD8_lift[d->rider];
            r->collision = 0;
            r->flags = (r->flags | 0x41) & 0xEFFF;
        }
        return;
    }
    if (m->collision == 0) {
        m->flags &= 0xFFBE;
        m->collision = m->pclass->collision;
        if (d->rider >= 0) {
            struct Moby *r = &D_L14_0015FFD8_lift[d->rider];
            r->flags &= 0xFFBE;
            r->collision = r->pclass->collision;
            r->flags |= 0x1000;
        }
    }
    qcopy(&old, &m->pos);
    qcopy(&rot, &m->rot);
    if (m->state == 0) {
        qcopy(&m->pos, D_L14_001B0BB0_lift[d->path] + 0x10);
        d->angle = 0.0f;
        d->sound = -1;
        d->h206 = 0;
        attach_manipulator(m, 0, d->manip);
        if (d->rider == -1) {
            lift_print(D_L14_001FC840, m->save_id);
            lift_remove(m);
            return;
        }
        d->riding = 0;
        d->from = d->height;
    }
    gunship_follow_path(m);
    rider = &D_L14_0015FFD8_lift[d->rider];
    if (d->mode == 0x3039) {
        if (d->anchor != -1)
            FUN_L00_0024f7c8(m, d->anchor, &base);
        else
            qcopy(&base, &m->pos);
        euler_rotation_matrix(mat, &m->rot);
        lift_xform(&off, d->offset, mat);
        lift_add(&rider->pos, &base, &off);
    }
    if (d->sound == -1 || (voice_is_playing(m, d->sound) == 0 && d->sound == -1))
        d->sound = lift_sound(0, 4, m);
    d->angle = lift_add_rot(d->angle, 0.17453292f);
    FUN_L00_001fff28(d->wobble, 0, d->angle);
    lift_sub(&delta, &m->pos, &old);
    update_entity_matrix(d->spring, &delta, &rot, &m->rot);
    if (hero.state.control_mode == 13 && hero.unk964 == rider) {
        if (d->riding == 0) {
            d->riding = 1;
            d->timer = lift_frames(D_L14_00161B8C);
            d->timer_inv = 1.0f / lift_itof(d->timer);
        }
    } else {
        if (d->riding == 1) {
            if (hero.air_frames.s == 0 || hero.motion.pos.f[2] <= m->pos.z - 3.0f) {
                d->riding = 0;
                d->timer = lift_frames(D_L14_00161B90);
                d->timer_inv = 1.0f / lift_itof(d->timer);
                d->to = d->height;
            } else {
                qcopy(&base, &hero.motion.pos);
                lift_sub(&delta, &rider->pos, &base);
                delta.f[2] = 0.0f;
                len = lift_length(&delta);
                max = D_L14_00161B88 * D_0015ED6C_lift;
                if (max < len)
                    len = max;
                lift_normalize(&hero.motion.platform_delta, &delta, len);
            }
        }
        if (d->riding == 0) {
            lift_tick(&d->timer);
            {
                f32 t = lift_itof(d->timer) * d->timer_inv;
                d->height = d->from + (d->to - d->from) * t;
            }
            return;
        }
    }
    lift_tick(&d->timer);
    {
        f32 t = lift_itof(d->timer) * d->timer_inv;
        d->height = (d->from - 0.005f) * t + 0.005f;
    }
}
typedef struct {
    s32 clip_push;   /* 0x0: clip volume that pushes the hero */
    char pad4[4];
    f32 push;        /* 0x8 */
    s16 push_timer;  /* 0xC */
    s16 pushing;     /* 0xE */
    f32 push_inv;    /* 0x10 */
    f32 f14, f18, f1C;
    char pad20[0x4E0];
    s32 clip_area;   /* 0x500 */
    s32 point;       /* 0x504 */
    s16 period;      /* 0x508 */
    s16 period_timer; /* 0x50A */
    f32 period_inv;  /* 0x50C */
} L14WindVars;
extern char *D_L14_001600EC_w __asm__("D_L14_001600EC");
extern f32 D_0015ED6C_w __asm__("D_0015ED6C");
/* wind applied to the sparks: direction scaled by strength, and a weight */
extern struct { f32 v; } D_L14_00162230, D_L14_00162234;
extern s32 D_L14_00162238;
extern f32 D_L14_00162240;
char D_L14_001FC980[] = "*Error: Moby 610 (wind-control) with no cuboid index (wind pushing hero area). UID: %d - RAR\n";
char D_L14_001FC9E0[] = "*Error: Moby 610 (wind-control) with no fxArea cuboid (fx drawing area). UID: %d - RAR\n";
char D_L14_001FCA38[] = "*Error: Moby 610 (wind-control) with no fxAct cuboid (fx activation area). UID: %d - RAR\n";
extern void wind_print(const char *, short) __asm__("FUN_001e93b0");
extern void wind_remove(struct Moby *) __asm__("FUN_0020c828");
extern float wind_length(void *) __asm__("FUN_001f9af0");
extern int wind_frames(int) __asm__("FUN_001f96f8");
extern float wind_itof(int) __asm__("FUN_001fa6c0");
extern float wind_cos(float) __asm__("FUN_001f9dc8");
extern float wind_sin(float) __asm__("FUN_001f9de0");
extern int wind_inside(void *, int) __asm__("FUN_00214720");
extern int wind_tick(short *) __asm__("FUN_001f9770");
extern void wind_add(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_L14_002eb568(struct Moby *m);
void FUN_L14_002eb388(struct Moby *m, int i, void *dir);

extern f32 D_L14_00161C08 __attribute__((sda));
extern s32 D_L14_00161C04 __attribute__((sda));
void FUN_L14_002eaf88(struct Moby *m) {
    L14WindVars *d = (L14WindVars *)m->pvars;
    float dir[4];
    char *pos;
    struct Hero *h;
    int i;

    if (m->state == 0) {
        m->state = 1;
        if (d->clip_push < 0) {
            wind_print(D_L14_001FC980, m->save_id);
            wind_remove(m);
            return;
        }
        if (d->point < 0) {
            wind_print(D_L14_001FC9E0, m->save_id);
            wind_remove(m);
            return;
        }
        if (d->clip_area < 0) {
            wind_print(D_L14_001FCA38, m->save_id);
            wind_remove(m);
            return;
        }
        d->pushing = 0;
        d->f14 = 2.0f * wind_length(D_L14_001600EC_w + (d->point << 7));
        d->f18 = wind_length(D_L14_001600EC_w + (d->point << 7) + 0x10);
        d->f1C = wind_length(D_L14_001600EC_w + (d->point << 7) + 0x20);
        d->period_timer = wind_frames(d->period);
        d->period_inv = 1.0f / wind_itof(d->period_timer);
        dir[0] = wind_cos(m->rot.z);
        dir[1] = wind_sin(m->rot.z);
        dir[2] = 0.0f;
        for (i = 0; i < 48; i++) {
            FUN_L14_002eb388(m, i, dir);
        }
    }
    pos = (char *)&hero.motion.pos;
    if (wind_inside(pos, d->clip_area) != 0) {
        D_L14_00162230.v = wind_cos(m->rot.z) * (D_L14_00161C08 * D_0015ED6C_w);
        D_L14_00162234.v = wind_sin(m->rot.z) * (D_L14_00161C08 * D_0015ED6C_w);
        D_L14_00162238 = 0;
        D_L14_00162240 = 1.0f;
        /* Fakematch: storing the weight again is a no-op that reload's cse drops, but the
         * branch around it keeps the call's argument move out of the stores above, so it
         * lands in the call's delay slot as in retail. */
        if (m != 0) {
            D_L14_00162240 = 1.0f;
        }
        FUN_L14_002eb568(m);
        h = (struct Hero *)(pos - 0x80);
        if (h->unk308 == 0 && h->unk2284 != 13 && (h->unk12E2 != 0 || h->frames_off_ground.s != 0) &&
            wind_inside(pos, d->clip_push) != 0) {
            float t;
            float k;
            wind_tick(&d->period_timer);
            t = wind_itof(d->period_timer) * d->period_inv;
            d->pushing = 1;
            k = D_0015ED6C_w;
            t = 1.0f - t;
            t = t * (d->push * k);
            dir[0] = wind_cos(m->rot.z) * t;
            dir[1] = wind_sin(m->rot.z) * t;
            dir[2] = 0.0f;
            wind_add(pos + 0x70, pos + 0x70, dir);
            d->push_timer = wind_frames(D_L14_00161C04);
            d->push_inv = 1.0f / wind_itof(d->push_timer);
            return;
        }
    }
    if (d->pushing != 0 && d->push_timer != 0) {
        float t;
        if (wind_tick(&d->push_timer) != 0) {
            d->pushing = 0;
            d->period_timer = wind_frames(d->period);
            d->period_inv = 1.0f / wind_itof(d->period_timer);
        }
        if (hero.unk308 != 0 || hero.unk12E2 == 0) {
            if (d->push_timer > wind_frames(15)) {
                d->push_timer = wind_frames(15);
                d->push_inv = 1.0f / (float)d->push_timer;
            }
        }
        t = (d->push * D_0015ED6C_w - 0.0f) * (wind_itof(d->push_timer) * d->push_inv) + 0.0f;
        dir[0] = wind_cos(m->rot.z) * t;
        dir[1] = wind_sin(m->rot.z) * t;
        dir[2] = 0.0f;
        wind_add(&hero.motion.platform_delta, &hero.motion.platform_delta, dir);
    }
}
/* Starts one spark: places it at a random spot around the emitter, picks its speed and its life. */
#include "qcopy.h"
extern struct { char pad[0x30]; float v[4]; char rest[0x40]; } *D_L14_001600EC_k __asm__("D_L14_001600EC");
extern float D_0015ED6C_k __asm__("D_0015ED6C");
extern void cross_k(void *, void *, void *) __asm__("FUN_001f9ad8");
extern float spread_k(float, float) __asm__("FUN_L00_00257c48");
extern void scale_k(void *, void *, float) __asm__("FUN_001f9a68");
extern void add_k(void *, void *, void *) __asm__("FUN_001f9a10");
extern float rand_k(float, float) __asm__("FUN_002132a8");
extern int ftoi_k(float) __asm__("FUN_001fa6d0");
extern int frames_k(int) __asm__("FUN_001f96f8");

typedef struct {
    char pad0[0x14];
    float f14, f18, f1C;
    float pos[48][4];
    short n320[48];
    short n380[48];
    short n3E0[48];
    float f440[48];
    int pad500;
    int i504;
} Spk_k;

extern int D_L14_00161C30 __attribute__((sda));
extern int D_L14_00161C34 __attribute__((sda));
extern float D_L14_00161C38 __attribute__((sda));
extern float D_L14_00161C3C __attribute__((sda));
void FUN_L14_002eb388(struct Moby *m, int i, void *dir) {
    Spk_k *d = (Spk_k *)m->pvars;
    float fwd[4];
    float side[4];
    float up[4];
    float t[4];
    float x, y, z;
    char *p;
    int n;
    qcopy(fwd, dir);
    up[0] = 0.0f;
    up[2] = 1.0f;
    up[1] = 0.0f;
    up[3] = 0.0f;
    cross_k(side, fwd, up);
    qcopy(d->pos[i], D_L14_001600EC_k[d->i504].v);
    x = -d->f14;
    y = spread_k(0.0f, d->f18);
    z = spread_k(0.0f, d->f1C);
    scale_k(t, fwd, x);
    p = (char *)d->pos[i];
    add_k(p, p, t);
    scale_k(t, side, y);
    add_k(p, p, t);
    scale_k(t, up, z);
    add_k(p, p, t);
    d->pos[i][3] = rand_k(D_L14_00161C38, D_L14_00161C3C);
    d->f440[i] = rand_k(D_L14_00161C30, D_L14_00161C34) * D_0015ED6C_k;
    n = frames_k(ftoi_k(d->f14 * 2.0f / d->f440[i]));
    d->n320[i] = n;
    d->n380[i] = n;
    d->n3E0[i] = 0;
}
extern float fast_cos_spark(float) __asm__("FUN_001f9dc8");
extern float fast_sin_spark(float) __asm__("FUN_001f9de0");
extern int tick_spark_timer(short *) __asm__("FUN_001f9770");
extern float spark_integer_to_float(int) __asm__("FUN_001fa6c0");
extern int spark_float_to_integer(float) __asm__("FUN_001fa6d0");
extern float spark_absolute_float(float) __asm__("FUN_001f99c0");
extern float spark_angle_difference(float, float) __asm__("FUN_001fa688");
extern int spark_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern void spark_scale_vector(void *, void *, float) __asm__("FUN_001f9a68");
extern void spark_add_vector(void *, void *, void *) __asm__("FUN_001f9a10");
extern int spark_scale_frames(int) __asm__("FUN_001f96f8");
extern void spark_enqueue_callback(void (*)(struct Moby *), struct Moby *) __asm__("FUN_001f4600");
struct L14SparkTarget {
    char pad0[0x140];
    float x;      /* 0x140 */
    float y;      /* 0x144 */
    char pad148[0x10];
    float yaw;    /* 0x158 */
};
extern struct L14SparkTarget D_L14_00167380_spark __asm__("D_L14_00167380");
void FUN_L14_002eb810(struct Moby *m);

extern int D_L14_00161C10 __attribute__((sda));
extern int D_L14_00161C2C __attribute__((sda));
void FUN_L14_002eb568(struct Moby *m) {
    Spk_k *d = (Spk_k *)m->pvars;
    float dir[4];
    float clip[4];
    float work[4];
    float ratio;
    float first;
    int in_clip;
    int i;
    int opacity;
    short *shade;
    float *pos;
    dir[0] = fast_cos_spark(m->rot.z);
    dir[1] = fast_sin_spark(m->rot.z);
    dir[2] = 0.0f;
    qcopy(clip, &hero.motion.pos);
    clip[2] += 1.0f;
    in_clip = spark_inside_clip_volume(clip, d->i504);
    i = 0;
    pos = d->pos[0];
    shade = d->n3E0;
    do {
        if (tick_spark_timer(&d->n320[i]) != 0) {
            if (in_clip == 0 || i < 24) {
                FUN_L14_002eb388(m, i, dir);
            }
        } else {
            opacity = D_L14_00161C10;
            if (d->n380[i] - D_L14_00161C2C < d->n320[i]) {
                first = spark_integer_to_float(d->n380[i] - d->n320[i]);
                first = first / spark_integer_to_float(D_L14_00161C2C);
            } else {
                first = spark_integer_to_float(d->n320[i]);
                first = first / spark_integer_to_float(d->n380[i] - D_L14_00161C2C);
            }
            {
                int s = spark_float_to_integer(((float)opacity - 0.0f) * first + 0.0f);
                *shade = s;
                /* Fakematch: storing the shade again is a no-op that reload's cse drops; the
                 * extra uses rank `shade` above `m` for register allocation as in retail. */
                if (s != 0) {
                    *shade = s;
                }
            }
            spark_scale_vector(work, dir, d->f440[i]);
            spark_add_vector(pos, pos, work);
            if (in_clip != 0 &&
                spark_absolute_float(pos[0] - D_L14_00167380_spark.x) < 8.0f &&
                spark_absolute_float(pos[1] - D_L14_00167380_spark.y) < 8.0f &&
                spark_angle_difference(m->rot.z, D_L14_00167380_spark.yaw) > 2.3561945f) {
                if (d->n320[i] > spark_scale_frames(20)) {
                    d->n320[i] = spark_scale_frames(20);
                }
            }
        }
        pos += 4;
        shade++;
        i++;
    } while (i < 48);
    spark_enqueue_callback(FUN_L14_002eb810, m);
}
/* Draws every live spark as a quad that faces the camera, faded by its age. */
typedef unsigned int u128_m __attribute__((mode(TI)));
typedef struct {
    float m[4][4];
    u32 col[4];
    float uv[8];
    unsigned long a, b, c, d;
} Pk_m;
extern char D_L14_001674C0_m[] __asm__("D_L14_001674C0");
extern unsigned long tex_m(s32) __asm__("FUN_001f44b8");
extern void sreg_m(s32, s64) __asm__("FUN_00233980");
extern void sub_m(void *, void *, void *) __asm__("FUN_001f9a28");
extern void norm_m(void *, void *, float) __asm__("FUN_001f9bf8");
extern void cross_m(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void scale_m(void *, void *, float) __asm__("FUN_001f9a68");
extern void xform_m(void *, void *, void *) __asm__("FUN_001f9d20");
extern void draw_m(void *, int, int) __asm__("FUN_001f7d30");

extern int D_L14_00161C0C __attribute__((sda));
float D_L14_001DFD40[4][4] = {
    {0.0f, 1.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, -1.0f, 1.0f},
    {0.0f, -1.0f, 1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
};
void FUN_L14_002eb810(struct Moby *m) {
    Spk_k *d = (Spk_k *)m->pvars;
    Pk_m pk;
    float mat[4][4];
    float up[4];
    u32 col;
    int i;
    int k;
    pk.b = tex_m(0x30);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    sreg_m(0x4A, 0);
    sreg_m(0x47, 0x51001);
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
            col = D_L14_00161C0C | (d->n3E0[i] << 24);
            pk.col[3] = col;
            pk.col[2] = col;
            pk.col[1] = col;
            pk.col[0] = col;
            qcopy(mat[3], d->pos[i]);
            *(u128_m *)up = 0;
            up[2] = 1.0f;
            up[3] = 1.0f;
            sub_m(mat[0], d->pos[i], D_L14_001674C0_m);
            norm_m(mat[0], mat[0], 1.0f);
            cross_m(mat[1], mat[0], up);
            norm_m(mat[1], mat[1], 1.0f);
            cross_m(mat[2], mat[1], mat[0]);
            for (k = 0; k < 4; k++) {
                scale_m(pk.m[k], D_L14_001DFD40[k], ((Spk_k *)((char *)d + (i << 4)))->pos[0][3]);
                xform_m(pk.m[k], pk.m[k], mat);
            }
            draw_m(&pk, 0, 0);
        }
    }
}
/* pvars of the drifting mine that follows a looped path */
typedef struct {
    u8 pad0[0x20];
    f32 health[0x10];  /* 0x20 */
    s32 path;          /* 0x60 */
    s32 segment;       /* 0x64 */
    f32 along;         /* 0x68 */
    f32 dir;           /* 0x6C */
    f32 vel[3];        /* 0x70 */
    f32 phase;         /* 0x7C */
    f32 spin[3];       /* 0x80 */
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
    u8 pad0[0x20];
    struct Moby *moby;  /* 0x20 */
} L14MineHit;

extern L14MinePath *D_L14_001B0BB0_mine[] __asm__("D_L14_001B0BB0");
extern char D_L14_001FCA98[];
extern char D_L14_001FCAD8[];
extern f32 D_L14_00161C44 __attribute__((sda));
extern f32 D_L14_00161C48 __attribute__((sda));
extern f32 D_0015ED6C_mine __asm__("D_0015ED6C");
extern void mine_print(const char *, short) __asm__("FUN_001e93b0");
extern void mine_remove(void *) __asm__("FUN_0020c828");
extern float mine_distance(void *, void *) __asm__("FUN_001f9b48");
extern int mine_rand(int) __asm__("FUN_00213260");
extern float mine_random(float, float) __asm__("FUN_002132a8");
extern void mine_clear(void *) __asm__("FUN_001f99f8");
extern float FUN_L00_00257c48(float, float);
extern int FUN_L00_0025d808_mine(void *, float *, int *, float *, int, float) __asm__("FUN_L00_0025d808");
extern float FUN_L00_0025b8c0(float *p, float *v, float t, float u1, float u2, float eps);
extern L14MineHit *mine_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern s32 fun_00213928(struct Moby *, void *, f32 *, s32, s32 *, s32, s32, s32) __asm__("FUN_00213928");
extern int mine_sound(int, int, void *) __asm__("FUN_0022da68");
extern void mine_explode(void *, void *, void *, float, float, int, int, int, float, float, float, float,
                         int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern float mine_add_rot(float, float) __asm__("FUN_001fa580");
extern float mine_sin(float) __asm__("FUN_001f9de0");
extern int mine_ftoi(float) __asm__("FUN_001fa6d0");

/* Mine update: drifts along its looped path while spinning and pulsing; blows up on a hit. */
void FUN_L14_002ec810(struct Moby *m) {
    enum {
        MINE_STATE_INITIALIZE = 0,
        MINE_STATE_FOLLOW_PATH = 1
    };
    L14MineVars *d = (L14MineVars *)m->pvars;
    float v[4];
    Vec4 vel;
    s32 count;
    L14MinePath *path;
    L14MineHit *hit;
    int i;
    float s;
    int r;
    int g;

    switch (m->state) {
    case MINE_STATE_INITIALIZE:
        if (d->path == -1) {
            mine_print(D_L14_001FCA98, m->save_id);
            mine_remove(m);
            return;
        }
        path = D_L14_001B0BB0_mine[d->path];
        if (path->count == 0) {
            mine_print(D_L14_001FCAD8, m->save_id);
            mine_remove(m);
            return;
        }
        for (i = 0; i < path->count; i++) {
            f32 len = mine_distance(&((f32 *)path)[i * 4 + 4], &((f32 *)path)[((i + 1) % path->count) * 4 + 4]);
            char *pt = (char *)path + i * 16;
            *(f32 *)(pt + 0x1C) = len;
        }
        d->segment = 0;
        d->along = 0;
        d->dir = mine_rand(2) != 0 ? 1.0f : -1.0f;
        qcopy(&m->pos, path->points[0].pos);
        m->state = 1;
        d->spin[0] = mine_random(0.05235988f, 0.10471976f);
        d->spin[1] = mine_random(0.034906585f, 0.06981317f);
        d->spin[2] = mine_random(0.017453292f, 0.034906585f);
        mine_clear(d->vel);
        d->phase = FUN_L00_00257c48(0.0f, 3.1415927f);
        break;
    case MINE_STATE_FOLLOW_PATH:
        FUN_L00_0025d808_mine(D_L14_001B0BB0_mine[d->path], v, &d->segment, &d->along, 1,
                              D_L14_00161C44 * D_0015ED6C_mine * d->dir);
        FUN_L00_0025b8c0(&m->pos.x, &d->vel[0], v[0], 0.02f, 0.2f, 0.0f);
        FUN_L00_0025b8c0(&m->pos.y, &d->vel[1], v[1], 0.02f, 0.2f, 0.0f);
        FUN_L00_0025b8c0(&m->pos.z, &d->vel[2], v[2], 0.02f, 0.2f, 0.0f);
        hit = mine_hit(m, 0x230000, 0);
        fun_00213928(m, hit, d->health, 0, &count, 0, 0, 4);
        if ((count >= 2 && hit != 0 && hit->moby->oclass != 0x283) || hero.coll_hit_moby == (u8 *)m) {
            mine_clear(&vel);
            mine_sound(0, 0, m);
            if (hero.coll_hit_moby == (u8 *)m)
                mine_explode(m, &vel, &m->pos, 3.0f, 1.0f, 10, 20, 60, 3.0f, 5.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
            else
                mine_explode(m, &vel, &m->pos, 0.0f, 0.0f, 10, 20, 60, 3.0f, 5.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
            mine_remove(m);
            return;
        }
        m->rot.x = mine_add_rot(m->rot.x, d->spin[0]);
        m->rot.y = mine_add_rot(m->rot.y, d->spin[1]);
        m->rot.z = mine_add_rot(m->rot.z, d->spin[2]);
        d->phase = mine_add_rot(d->phase, D_L14_00161C48);
        s = mine_sin(d->phase);
        r = mine_ftoi(s * 48.0f) + 0xCF;
        g = mine_ftoi(s * 32.0f) + 0x30;
        m->glow_color = g << 16 | (0xFF000000 | g << 8) | r;
        break;
    }
}
#include "qcopy.h"

/* Spawn a projectile moby at a, aimed along b, with two parameters. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002EE150), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void FUN_L00_00250df8(void *);
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");
extern char *func_0020D348_m(int) __asm__("FUN_0020c4f8");

char *FUN_L14_002eccf0(void *a, float *b, int c, int d) {
    char *m = func_0020D348_m(0x2A9);
    if (m) {
        char *data;
        *(int *)(m + 0x40) = 0;
        *(unsigned char *)(m + 0x30) = 0xFF;
        *(short *)(m + 0x32) = 0x7F;
        m[0x31] = 1;
        data = *(char **)(m + 0x78);
        *(float *)(m + 0x44) = -fast_arc_tan(vector_length_xy(b), b[2]);
        *(float *)(m + 0x48) = fast_arc_tan(b[0], b[1]);
        qcopy(m + 0x10, a);
        qcopy(data + 0x10, a);
        qcopy(data, b);
        blend_moby_animation(m, 1, 0, scale_game_frames(5));
        *(short *)(m + 0x34) = 0x208;
        *(int *)(data + 0x20) = c;
        *(int *)(data + 0x24) = d;
        m[0x23] = 0x10;
        FUN_L00_00250df8(m);
    }
    return m;
}
/* Pvars of a flying bolt (same code as FUN_L07_0030b6a8, plus a sweep that skips class 0xFC mobys). */
typedef struct {
    Vec4 vel;        /* 0x00 */
    Vec4 origin;     /* 0x10 */
    void *ignore;    /* 0x20: collision owner to skip */
    s32 solid;       /* 0x24 */
} L14BoltVars;

/* Sweep request filled by FUN_L00_00259888 for the second collision test. */
typedef struct {
    u8 pad0[0x18];
    u8 flag18;
    u8 flag19;
    u8 pad1A[0x16];
} L14BoltSweep;

extern void bolt_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern float bolt_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 bolt_length(void *) __asm__("FUN_001f9af0");
extern void bolt_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void bolt_normalize_xy(void *, void *, f32) __asm__("FUN_001f9c48");
extern f32 bolt_random(f32, f32) __asm__("FUN_002132a8");
extern int FUN_L00_00257b90(int, int);
extern s32 bolt_frames(s32) __asm__("FUN_001f96f8");
extern int bolt_coll_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void FUN_L00_00259888(void *, struct Moby *, s32, void *, f32);
extern void FUN_L07_0029a2a0(void *, void *);
extern void FUN_L00_001ff660(void *, void *, void *);
extern char *FUN_L00_0026dd20(char *, char *, int c, int d, float f);
extern int bolt_sound(int, int, void *) __asm__("FUN_0022da68");
extern void bolt_remove(void *) __asm__("FUN_0020c828");
/* Result of the last collision line test. */
extern struct {
    u8 pad0[0x18];
    struct Moby *moby;   /* 0x18 */
    u8 pad1C[4];
    Vec4 pos;            /* 0x20 */
} D_L14_00174540;
extern char D_L14_00174580[];

void FUN_L14_002ece00(struct Moby *m) {
    L14BoltVars *v = (L14BoltVars *)m->pvars;
    Vec4 old;
    Vec4 target;
    L14BoltSweep sweep;
    Vec4 aim;
    Vec4 dir;
    Vec4 spark;
    void *ignore;
    struct Moby *hit;
    int pass;
    int tries;
    int i;
    int j;

    qcopy(&old, &m->pos);
    bolt_add(&m->pos, &m->pos, &v->vel);
    qcopy(&target, &m->pos);
    ignore = v->ignore;
    qcopy(&aim, &v->vel);
    aim.f[2] = 0.0f;
    bolt_normalize_xy(&aim, &aim, 1.0f);
    tries = 0;
    aim.f[2] = 1.0f;
    FUN_L00_00259888(&sweep, m, 0x10001, &aim, 1.0f);
    sweep.flag18 = 1;
    sweep.flag19 = 1;
    pass = v->solid == 0;
    if (bolt_distance(&m->pos, &v->origin) > 30.0f) {
        if (m->visible != 0) {
            for (i = 0; i < 3; i++) {
                spark.q = 0;
                spark.f[0] = bolt_random(-1.0f, 1.0f);
                spark.f[1] = bolt_random(-1.0f, 1.0f);
                spark.f[2] = bolt_random(-1.0f, 1.0f);
                dir.q = spark.q;
                bolt_normalize(&dir, &dir, bolt_length(&spark) * 0.1f);
                bolt_add(&dir, &spark, &dir);
                bolt_normalize(&dir, &dir, bolt_random(frame_time + frame_time, frame_time * 4.0f));
                FUN_L07_0029a2a0(&m->pos, &dir);
            }
        }
        bolt_remove(m);
        return;
    }
    while (tries < 3 && bolt_coll_line(&old, &target, pass, ignore, 0) != 0) {
        hit = D_L14_00174540.moby;
        tries++;
        if (hit != 0 && hit->oclass == 0xFC) {
            qcopy(&old, &D_L14_00174540.pos);
            ignore = hit;
            continue;
        }
        bolt_coll_line(&old, &target, pass, ignore, &sweep);
        qcopy(&m->pos, &D_L14_00174540.pos);
        if (m->visible != 0) {
            for (j = 0; j < 5; j++) {
                spark.q = 0;
                spark.f[0] = bolt_random(-1.0f, 1.0f);
                spark.f[1] = bolt_random(-1.0f, 1.0f);
                spark.f[2] = bolt_random(-1.0f, 1.0f);
                dir.q = spark.q;
                FUN_L00_001ff660(&spark, &v->vel, D_L14_00174580);
                bolt_normalize(&dir, &dir, bolt_length(&spark) * 0.5f);
                bolt_add(&dir, &spark, &dir);
                bolt_normalize(&dir, &dir, bolt_random(frame_time * 3.0f, frame_time * 6.0f));
                FUN_L00_0026dd20((char *)&m->pos, (char *)&dir, 0x7F2F4F6F,
                                 FUN_L00_00257b90(bolt_frames(10), bolt_frames(15)), 30000.0f);
            }
        }
        bolt_sound(0, 0, m);
        bolt_remove(m);
        return;
    }
    if (m->pos.x < 2.0f || m->pos.x > 1021.0f || m->pos.y < 2.0f || m->pos.y > 1021.0f ||
        m->pos.z < 2.0f || m->pos.z > 1021.0f) {
        bolt_remove(m);
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Camera-intro controller: waits for the cutscene flag, then every few seconds moves the focus to a random
 * visible point of its path. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002EE6E0), where it is exact; names translated to the US level program. */

extern char D_0013D24D[];
extern float D_L14_00161CC0[] __attribute__((section(".sdata")));
extern float D_L14_00161CD0[];
extern float FUN_L00_00257c48(float, float);
extern int D_L14_0015F5C4;
extern int D_L14_00161CB4;
extern int fast_bsphere_check(void *, float) __asm__("FUN_001fa728");
extern int tick_countdown_32_alt(void *) __asm__("FUN_001f9740");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern short D_L14_00161C50 __attribute__((sda));
extern short D_L14_00161C54 __attribute__((sda));
extern short D_L14_00161C58 __attribute__((sda));
extern short D_L14_00161C5C __attribute__((sda));
extern short D_L14_00161C60 __attribute__((sda));
extern short D_L14_00161C64 __attribute__((sda));
extern void FUN_L00_0028e990(void);
extern void start_scene(int) __asm__("FUN_L00_00298840");
extern void FUN_L01_002a2480(void);
extern void FUN_L14_002ed530(char *);
extern int *D_L14_001B0F30_ee[] __asm__("D_L14_001B0BB0");
extern float D_L14_00161CD0_f __asm__("D_L14_00161CD0");
extern float D_L14_00161CD4;
extern float D_L14_00161CD8;

extern float D_L14_00161CCC;
void FUN_L14_002ed280(struct Moby *moby) {
    char *d = (char *)moby->pvars;
    int idx[5];
    float v[4];
    int st;
    if (MOBY(moby)->state == 0) {
        if (current_level_index == 0xE) hero.unk22D8 = 1;
        MOBY(moby)->state = 1;
        moby->update_dist = 0xFF;
        *(int *)&D_L14_00161C58 = 0;
        *(int *)&D_L14_00161C50 = 0;
    }
    st = MOBY(moby)->state;
    if (st == 1) {
        unsigned char *p;
        if (D_L14_0015F5C4 != 0) return;
        p = (unsigned char *)D_0013D24D + 0x13B;
        if (p[0x68] != 0) return;
        start_scene(0);
        p[0x68] = st;
        MOBY(moby)->state = 2;
        FUN_L00_0028e990();
        return;
    }
    if (st == 2) {
        if (D_L14_0015F5C4 == st) return;
        MOBY(moby)->state = 3;
        FUN_L01_002a2480();
        return;
    }
    if (tick_countdown_32_alt(&D_L14_00161C50) == 0) return;
    if (*(int *)&D_L14_00161C58 == 0) {
        int *path = D_L14_001B0F30_ee[*(int *)d];
        int i;
        int n = 0;
        for (i = 0; i < path[0] && n < 5; i++) {
            qcopy(v, (char *)path + i * 16 + 0x10);
            v[2] += *(float *)&D_L14_00161C64;
            v[3] = *(float *)&D_L14_00161C60;
            if (fast_bsphere_check(v, 255.0f) > 0) {
                idx[n++] = i;
            }
        }
        if (n > 0) {
            int k = random_integer_below(n);
            qcopy(D_L14_00161CC0, (char *)path + idx[k] * 16 + 0x10);
            D_L14_00161CCC = 1.0f;
            qcopy(D_L14_00161CD0, D_L14_00161CC0);
            D_L14_00161CD8 = *(float *)&D_L14_00161C5C;
            D_L14_00161CD0_f += FUN_L00_00257c48(0.0f, 20.0f);
            D_L14_00161CD4 += FUN_L00_00257c48(0.0f, 20.0f);
            qcopy(&moby->pos, D_L14_00161CC0);
        }
    }
    FUN_L14_002ed530(moby);
    if (D_L14_00161CB4 == 0) {
        *(int *)&D_L14_00161C50 = scale_game_frames(*(int *)&D_L14_00161C54);
        *(int *)&D_L14_00161C58 = 0;
    }
}
/* Arc state shared by FUN_L14_002ed530 and its draw callback FUN_L14_002edc18. */
extern f32 D_L14_00161C68 __attribute__((sda));
extern f32 D_L14_00161C6C __attribute__((sda));
extern s32 D_L14_00161C70_gp __asm__("D_L14_00161C70") __attribute__((sda));
extern f32 D_L14_00161C74 __attribute__((sda));
extern f32 D_L14_00161C8C __attribute__((sda));
extern f32 D_L14_00161C90 __attribute__((sda));
extern f32 D_L14_00161C94 __attribute__((sda));
extern s32 D_L14_00161C98 __attribute__((sda));
extern s32 D_L14_00161C9C __attribute__((sda));
extern u8 D_L14_00161CE0_d530 __asm__("D_L14_00161CE0");
extern s16 D_L14_00161CE2;
extern f32 D_L14_00161CE4;
extern char *D_L14_00161CE8;
extern s32 D_L14_00161CEC;
extern f32 D_L14_00161CF0;
extern s32 D_L14_00161CF4;
extern s32 D_L14_00161CF8_macro[] __asm__("D_L14_00161CF8") __attribute__((section(".sdata")));
extern f32 D_L14_001DFD80_d530[40][4] __asm__("D_L14_001DFD80");
extern s16 D_L14_001E0000[40];
f32 D_L14_001E0050_d530[3][16][4] __asm__("D_L14_001E0050") = {0};
u16 D_L14_001E0350_d530[3][16] __asm__("D_L14_001E0350") = {0};
extern void arc_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern float arc_length(void *) __asm__("FUN_001f9af0");
extern float arc_itof(int) __asm__("FUN_001fa6c0");
extern int arc_ftoi(float) __asm__("FUN_001fa6d0");
extern int arc_frames(int) __asm__("FUN_001f96f8");
extern void arc_lerp(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern int FUN_L00_00257b90_d530(int, int) __asm__("FUN_L00_00257b90");
extern float arc_random(float, float) __asm__("FUN_002132a8");
extern int arc_rand(int) __asm__("FUN_00213260");
extern float arc_cos(float) __asm__("FUN_001f9dc8");
extern float arc_sin(float) __asm__("FUN_001f9de0");
extern float arc_add_rot(float, float) __asm__("FUN_001fa580");
extern float arc_fabs(float) __asm__("FUN_001f99c0");
extern int arc_tick(void *) __asm__("FUN_001f9740");
extern void allocate_voice_for_group_entry(int, int, void *) __asm__("FUN_0022dba0");
extern void arc_enqueue(void (*)(char *), char *) __asm__("FUN_001f4600");
void FUN_L14_002ee6b0(float *pos, int n);
void FUN_L14_002edc18();
extern float D_L14_00161CD0_d530[] __asm__("D_L14_00161CD0") __attribute__((section(".sdata")));

extern float D_L14_001DFFE0[];
/* Lightning arc between the two camera points: builds the bolt and its three branches once, then ages and fades them. */
void FUN_L14_002ed530(char *moby) {
    Vec4 delta;
    float len;
    float t;
    float sign;
    float ang;
    float r;
    int i;
    int j;
    int n;
    float *p;
    s32 *starts;
    u16 *e;

    if (*(int *)&D_L14_00161C58 == 0) {
        *(int *)&D_L14_00161C58 = 1;
        D_L14_00161CF4 = 1;
        D_L14_00161CE8 = moby;
        i = 0;
        starts = D_L14_00161CF8_macro;
        *(int *)&D_L14_00161C6C = 0;
        arc_sub(&delta, D_L14_00161CD0_d530, D_L14_00161CC0);
        len = arc_length(&delta);
        D_L14_00161C74 = len / arc_itof(40);
        D_L14_00161CE0_d530 = 0;
        for (; i < 40; i++) {
            e = &D_L14_001E0000[i];
            p = D_L14_001DFD80_d530[i];
            p[1] = p[2] = 0.0f;
            p[3] = 1.0f;
            p[0] = D_L14_00161C74 * (float)i;
            if (i < 39) {
                p[2] = p[-2] + FUN_L00_00257c48(D_L14_00161C8C, D_L14_00161C90);
                if (D_L14_00161C94 < (*&p)[2]) {
                    p[2] = D_L14_00161C94;
                } else if (p[2] < -D_L14_00161C94) {
                    p[2] = -D_L14_00161C94;
                }
            }
            t = arc_itof(i) / arc_itof(40);
            *e = arc_ftoi((float)-arc_frames(D_L14_00161C70_gp) * t + 0.0f);
        }
        arc_lerp(D_L14_001DFFE0, D_L14_001DFFE0 - 4, D_L14_001DFFE0 + 4, 0.5f);
        sign = -1.0f;
        D_L14_00161CB4 = D_L14_00161CEC = arc_frames(FUN_L00_00257b90_d530(D_L14_00161C98, D_L14_00161C9C));
        D_L14_00161CF0 = 1.0f / arc_itof(D_L14_00161CB4 - arc_frames(D_L14_00161C70_gp));
        if (arc_rand(2) != 0) {
            sign = 1.0f;
        }
        for (j = 0; j < 3; j++) {
            int k = FUN_L00_00257b90_d530(5, 22);
            int m;
            float (*row)[4] = D_L14_001E0050_d530[j];
            u16 *sq = D_L14_001E0350_d530[j];
            starts[j] = k;
            sq[0] = D_L14_001E0000[k];
            row[0][0] = 0.0f;
            row[0][2] = 0.0f;
            r = arc_random(1.0f, 10.0f);
            ang = arc_random(0.5235988f, 1.3962634f) * sign;
            row[0][0] = r * arc_cos(ang);
            row[0][2] = r * arc_sin(ang);
            for (m = 1; m < 16; m++) {
                float d;
                float na;
                r = arc_random(1.0f, 10.0f);
                d = FUN_L00_00257c48(0.0f, 40.0f) * 0.017453292f;
                na = arc_add_rot(ang, d);
                if (arc_fabs(na) > 1.3962634f || (ang > 0.0f && na < 0.0f) || (ang < 0.0f && na > 0.0f)) {
                    ang = arc_add_rot(ang, -d);
                } else {
                    ang = na;
                }
                D_L14_001E0050_d530[j][m][0] = r * arc_cos(ang);
                D_L14_001E0050_d530[j][m][2] = r * arc_sin(ang);
                D_L14_001E0350_d530[j][m] = D_L14_001E0000[starts[j] + m];
            }
            sign = -sign;
        }
    }
    if (moby == D_L14_00161CE8) {
        arc_tick(&D_L14_00161CB4);
        for (i = 0; i < 40; i++) {
            s16 v = ++D_L14_001E0000[i];
            if (arc_frames(D_L14_00161C70_gp) < v) {
                D_L14_001E0000[i] = arc_frames(D_L14_00161C70_gp);
            }
            if (i != 0) {
                if (i >= 35) {
                    float k5 = 5.0f;
                    float *pp = D_L14_001DFD80_d530[i];
                    pp[2] = pp[2] + (0.0f - pp[2]) * (arc_itof(i - 35) / k5);
                }
            }
        }
        arc_frames(D_L14_00161C70_gp);
        for (j = 0; j < 3; j++) {
            for (n = 0; n < 16; n++) {
                s16 v = ++D_L14_001E0350_d530[j][n];
                if (arc_frames(D_L14_00161C70_gp) < v) {
                    D_L14_001E0350_d530[j][n] = arc_frames(D_L14_00161C70_gp);
                }
            }
        }
        if (D_L14_00161CB4 < D_L14_00161CEC - arc_frames(D_L14_00161C70_gp)) {
            if (arc_frames(60) < D_L14_00161CEC) {
                if (D_L14_00161C6C < 6.2831855f) {
                    D_L14_00161CE2 = arc_ftoi(arc_sin(D_L14_00161CE4) * 128.0f);
                    D_L14_00161CE4 = arc_add_rot(D_L14_00161CE4, D_L14_00161C68);
                    D_L14_00161C6C += D_L14_00161C68 * frame_scale;
                } else {
                    D_L14_00161CE2 = 0;
                }
            } else {
                D_L14_00161CE2 = 0;
            }
            D_L14_00161CE0_d530 = (u8)D_L14_00161CE2 + arc_ftoi(arc_itof(D_L14_00161CB4) * D_L14_00161CF0 * -255.0f + 255.0f);
            if (D_L14_00161CF4 != 0) {
                allocate_voice_for_group_entry(0, 0, moby);
                D_L14_00161CF4 = 0;
            }
            if (D_L14_00161CE0_d530 < 50) {
                FUN_L14_002ee6b0(D_L14_00161CC0, 3);
            }
        } else {
            D_L14_00161CE0_d530 = 0;
        }
        arc_enqueue(FUN_L14_002edc18, moby);
    }
}
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002EF078), where it is exact; names translated to the US level program. */

typedef int u128_2EF078 __attribute__((mode(TI)));

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) V_2EF078;

typedef struct {
    V_2EF078 v[4];
    int col[4];
    float uv[4][2];
    long r70;
    long tex;
    long r80;
    long r88;
} Q_2EF078;

extern V_2EF078 D_L14_001DFD80[];
extern V_2EF078 D_L14_001E0050[];
extern char D_L14_001674C0[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float D_L14_00161CD0_c[] __asm__("D_L14_00161CD0") __attribute__((section(".sdata")));
extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");
extern int D_L14_00161C70_gp __asm__("D_L14_00161C70") __attribute__((sda));
extern int D_L14_00161C78 __attribute__((sda));
extern int D_L14_00161C7C __attribute__((sda));
extern float D_L14_00161C80 __attribute__((sda));
extern float D_L14_00161C84 __attribute__((sda));
extern float D_L14_00161C88 __attribute__((sda));
extern int D_L14_00161CF8_macro[]  __asm__("D_L14_00161CF8")__attribute__((section(".sdata")));
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 truncate_float_to_s32_c2(f32) __asm__("func_001FA6D0");
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern s16 D_L14_001E0000[];
extern short D_L14_001E0350[];
extern unsigned char D_L14_00161CE0[] __attribute__((section(".sdata")));
extern void copy_blocks_16_forward(void *dst, void *src, s32 size) __asm__("func_001F98D0");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void FUN_L14_002edc18(void) {
    Q_2EF078 q[2];
    V_2EF078 m[4];
    V_2EF078 sz[2];
    V_2EF078 p[2];
    V_2EF078 m2[4];
    V_2EF078 r[3];
    int a;
    int i;
    int j;
    V_2EF078 *n;
    short *s;
    int c;
    V_2EF078 *nd;

    q[0].tex = get_effect_texture(0x2D);
    q[0].r88 = 0x8000000048L;
    q[0].r80 = 0xFF9000000260L;
    q[0].r70 = 0;
    a = func_001FA898_r(convert_integer_to_float(D_L14_001E0000[0]) / convert_integer_to_float(scale_game_frames_alt(D_L14_00161C70_gp)) * 255.0f) - D_L14_00161CE0[0];
    if (a < 0) a = 0;
    if (a > 0xFF) a = 0xFF;
    c = D_L14_00161C78 | (a << 24);
    q[0].col[1] = c;
    q[0].col[0] = c;
    q[0].uv[0][0] = 0.0f;
    q[0].uv[0][1] = 0.0f;
    q[0].uv[1][0] = 0.0f;
    q[0].uv[1][1] = 1.0f;
    q[0].uv[2][0] = 1.0f;
    q[0].uv[2][1] = 0.0f;
    q[0].uv[3][0] = 1.0f;
    q[0].uv[3][1] = 1.0f;
    copy_blocks_16_forward(&q[1], &q[0], 0x90);
    q[1].tex = get_effect_texture(0x2C);
    q[1].col[0] = q[1].col[1] = D_L14_00161C7C | ((a >> 1) << 24);
    subtract_vector_xyz_c2(&m[0], D_L14_00161CC0, D_L14_00161CD0_c);
    normalize_vector_xyz(&m[0], &m[0], 1.0f);
    m[0].w = 0.0f;
    m[2].x = 1.0f;
    m[2].y = 0.0f;
    m[2].z = 0.0f;
    m[2].w = 0.0f;
    cross_vectors_xyz(&m[1], &m[0], &m[2]);
    normalize_vector_xyz(&m[1], &m[1], 1.0f);
    m[1].w = 0.0f;
    cross_vectors_xyz(&m[2], &m[1], &m[0]);
    m[2].w = 0.0f;
    qcopy(&m[3], D_L14_00161CD0_c);
    *(u128_2EF078 *)&sz[0] = 0;
    sz[0].z = D_L14_00161C84;
    sz[0].w = 1.0f;
    *(u128_2EF078 *)&sz[1] = 0;
    sz[1].z = D_L14_00161C84 * D_L14_00161C88;
    sz[1].w = 1.0f;
    qcopy(&q[0].v[0], &sz[0]);
    scale_vector_xyz(&q[0].v[1], &q[0].v[0], -1.0f);
    qcopy_nc(&q[1].v[0], &sz[1]);
    scale_vector_xyz(&q[1].v[1], &q[1].v[0], -1.0f);
    nd = &D_L14_001DFD80[1];
    transform_vector(&p[0], nd, &m[0]);
    transform_vector(&p[1], nd + 1, &m[0]);
    qcopy(&m2[0], &m[0]);
    subtract_vector_xyz_c2(&m2[1], D_L14_001674C0, &p[0]);
    normalize_vector_xyz(&m2[1], &m2[1], 1.0f);
    cross_vectors_xyz(&m2[2], &m2[1], &m2[0]);
    qcopy(&m2[3], &p[0]);
    transform_vector(&q[0].v[0], &q[0].v[0], &m2[0]);
    transform_vector(&q[0].v[1], &q[0].v[1], &m2[0]);
    transform_vector(&q[1].v[0], &q[1].v[0], &m2[0]);
    transform_vector(&q[1].v[1], &q[1].v[1], &m2[0]);
    for (i = 1; i < 0x28; i++) {
        int b;
        float *e = D_L14_00161CC0;
        qcopy(&q[0].v[2], &sz[0]);
        scale_vector_xyz(&q[0].v[3], &q[0].v[2], -1.0f);
        qcopy(&q[1].v[2], &sz[1]);
        scale_vector_xyz(&q[1].v[3], &q[1].v[2], -1.0f);
        if (i == 0x27) {
            qcopy(&p[0], e);
        } else {
            transform_vector(&p[0], &D_L14_001DFD80[i], &m[0]);
        }
        qcopy(&m2[0], &m[0]);
        subtract_vector_xyz_c2(&m2[1], D_L14_001674C0, &p[0]);
        normalize_vector_xyz(&m2[1], &m2[1], 1.0f);
        cross_vectors_xyz(&m2[2], &m2[1], &m2[0]);
        qcopy(&m2[3], &p[0]);
        transform_vector(&q[0].v[2], &q[0].v[2], &m2[0]);
        transform_vector(&q[0].v[3], &q[0].v[3], &m2[0]);
        transform_vector(&q[1].v[2], &q[1].v[2], &m2[0]);
        transform_vector(&q[1].v[3], &q[1].v[3], &m2[0]);
        if (i == 0x27) {
            b = 0;
        } else {
            b = func_001FA898_r(convert_integer_to_float(D_L14_001E0000[i]) / convert_integer_to_float(scale_game_frames_alt(D_L14_00161C70_gp)) * 255.0f) - D_L14_00161CE0[0];
            if (b < 0) b = 0;
            if (b > 0xFF) b = 0xFF;
        }
        q[0].col[2] = q[0].col[3] = D_L14_00161C78 | (b << 24);
        q[1].col[2] = q[1].col[3] = D_L14_00161C7C | ((b >> 1) << 24);
        draw_geometry_quad(&q[0], 0, 0);
        draw_geometry_quad(&q[1], 0, 0);
        q[0].col[0] = q[0].col[1] = q[0].col[2];
        q[1].col[0] = q[1].col[1] = q[1].col[2];
        qcopy(&q[0].v[0], &q[0].v[2]);
        qcopy(&q[0].v[1], &q[0].v[3]);
        qcopy(&q[1].v[0], &q[1].v[2]);
        qcopy(&q[1].v[1], &q[1].v[3]);
        sz[0].z = D_L14_00161C84 + (D_L14_00161C80 - D_L14_00161C84) * cubic_hermite_scalar(-1.0f, 0.0f, 1.0f, 0.0f, convert_integer_to_float(i) / convert_integer_to_float(0x28));
        sz[1].z = sz[0].z * D_L14_00161C88;
    }
    qcopy(&m2[0], &m[0]);
    qcopy(&m2[1], &m[1]);
    qcopy(&m2[2], &m[2]);
    for (j = 0; j < 3; j++) {
        float t;
        short *as = D_L14_001E0350;
        V_2EF078 *os = D_L14_001E0050;
        V_2EF078 *o;

        t = convert_integer_to_float(as[j * 16]) / convert_integer_to_float(scale_game_frames_alt(D_L14_00161C70_gp));
        a = func_001FA898_r(t * 255.0f) - D_L14_00161CE0[0];
        if (a < 0) a = 0;
        if (a > 0xFF) a = 0xFF;
        c = D_L14_00161C78 | (a << 24);
        q[0].col[1] = c;
        q[0].col[0] = c;
        q[1].col[0] = q[1].col[1] = D_L14_00161C7C | ((a >> 1) << 24);
        sz[0].z = (D_L14_00161C84 + (D_L14_00161C80 - D_L14_00161C84) * cubic_hermite_scalar(-1.0f, 0.0f, 1.0f, 0.0f, convert_integer_to_float(D_L14_00161CF8_macro[j]) / convert_integer_to_float(0x28))) * 0.75f;
        sz[1].z = sz[0].z * 4.0f;
        qcopy(&q[0].v[0], &sz[0]);
        scale_vector_xyz(&q[0].v[1], &q[0].v[0], -1.0f);
        qcopy(&q[1].v[0], &sz[1]);
        scale_vector_xyz(&q[1].v[1], &q[1].v[0], -1.0f);
        qcopy(&r[0], &D_L14_001DFD80[D_L14_00161CF8_macro[j]]);
        transform_vector(&r[1], &r[0], &m[0]);
        o = &os[j * 16];
        r[0].x += o->x;
        r[0].z += o->z;
        transform_vector(&r[2], &r[0], &m[0]);
        qcopy(&m2[3], &r[1]);
        transform_vector(&q[0].v[0], &q[0].v[0], &m2[0]);
        transform_vector(&q[0].v[1], &q[0].v[1], &m2[0]);
        transform_vector(&q[1].v[0], &q[1].v[0], &m2[0]);
        transform_vector(&q[1].v[1], &q[1].v[1], &m2[0]);
        n = o + 1;
        s = &as[j * 16 + 1];
        for (i = 1; i < 16; i++) {
            qcopy(&q[0].v[2], &sz[0]);
            scale_vector_xyz(&q[0].v[3], &q[0].v[2], -1.0f);
            qcopy(&q[1].v[2], &sz[1]);
            scale_vector_xyz(&q[1].v[3], &q[1].v[2], -1.0f);
            qcopy_nc(&r[1], &r[2]);
            r[0].x += n->x;
            r[0].z += n->z;
            transform_vector(&r[2], &r[0], &m[0]);
            qcopy(&m2[3], &r[1]);
            transform_vector(&q[0].v[2], &q[0].v[2], &m2[0]);
            transform_vector(&q[0].v[3], &q[0].v[3], &m2[0]);
            transform_vector(&q[1].v[2], &q[1].v[2], &m2[0]);
            transform_vector(&q[1].v[3], &q[1].v[3], &m2[0]);
            t = convert_integer_to_float(*s) / convert_integer_to_float(scale_game_frames_alt(D_L14_00161C70_gp));
            a = func_001FA898_r(t * 255.0f) - D_L14_00161CE0[0];
            if (a < 0) a = 0;
            if (a > 0xFF) a = 0xFF;
            c = D_L14_00161C78 | (a << 24);
            q[0].col[3] = c;
            q[0].col[2] = c;
            q[1].col[2] = q[1].col[3] = D_L14_00161C7C | ((a >> 1) << 24);
            draw_geometry_quad(&q[0], 0, 0);
            draw_geometry_quad(&q[1], 0, 0);
            q[0].col[0] = q[0].col[1] = q[0].col[2];
            q[1].col[0] = q[1].col[1] = q[1].col[2];
            qcopy(&q[0].v[0], &q[0].v[2]);
            qcopy(&q[0].v[1], &q[0].v[3]);
            qcopy(&q[1].v[0], &q[1].v[2]);
            qcopy(&q[1].v[1], &q[1].v[3]);
            sz[0].z *= 0.95f;
            sz[1].z = sz[0].z * 4.0f;
            s++;
            n++;
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002EFB10), where it is exact; names translated to the US level program. */

extern f32 random_angle_radians(void) __asm__("func_00213308");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern float FUN_L00_00257c48(float, float);
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 scale_game_frames_c(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern unsigned char * FUN_L00_00272060(float *pos, int a, float f0, float f1, int col, int mode, int b, float *vel, float f2);
extern void build_spherical_offset_c(void *, f32, f32, f32) __asm__("func_00214DB0");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

extern short D_L14_00161CA0 __attribute__((sda));
extern short D_L14_00161CA4 __attribute__((sda));
extern short D_L14_00161CA8 __attribute__((sda));
extern short D_L14_00161CAC __attribute__((sda));
extern short D_L14_00161CB0 __attribute__((sda));
void FUN_L14_002ee6b0(float *pos, int n) {
    float v[4];
    float p2[4];
    float a, f21, f20;
    int s, r0, i, id;
    unsigned char *r;
    if (n > 0) {
    i = n;
    do {
        a = random_float_between_c(*(float *)&D_L14_00161CAC, *(float *)&D_L14_00161CB0);
        s = random_integer_below(2);
        if (s == 0) s--;
        f21 = random_angle_radians();
        f20 = random_float_between_c(0.34906584f, 1.5358897f);
        build_spherical_offset_c(v, random_float_between_c(*(float *)&D_L14_00161CA0, *(float *)&D_L14_00161CA4), f21, f20);
        qcopy(p2, pos);
        pos[0] += FUN_L00_00257c48(0.0f, 0.15f);
        pos[1] += FUN_L00_00257c48(0.0f, 0.15f);
        pos[2] += FUN_L00_00257c48(0.0f, 0.15f);
        id = scale_game_frames_c(0x3C);
        r = FUN_L00_00272060(pos, id, a * 0.1f, a, 0x7F7F2020, 0, s, v, *(float *)&D_L14_00161CA8);
        if (r != 0) {
            r[9] = func_001FA898_r(4.0f) + 0x70;
        }
        s = -s;
        r = FUN_L00_00272060(pos, scale_game_frames_c(0x3C), a * 0.07f, a * 0.7f, 0x7F7F7F7F, 1, s, v, *(float *)&D_L14_00161CA8);
        if (r != 0) {
            r[9] = func_001FA898_r(4.0f) + 0x70;
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
extern char *D_L14_001B0BB0_v[] __asm__("D_L14_001B0BB0");
extern char D_0013E550[];
extern int vendor_inside(void *, int) __asm__("FUN_00214720");
extern int vendor_sound(int, int, void *) __asm__("FUN_0022da68");
extern void vendor_volume(s32 voice, s32 volume) __asm__("FUN_L01_002a1968");
extern void vendor_release(s32) __asm__("FUN_0022d798");
extern float vendor_distance(void *, void *) __asm__("FUN_001f9b80");
extern float vendor_length(void *) __asm__("FUN_001f9af0");
extern float vendor_itof(int) __asm__("FUN_001fa6c0");
extern int vendor_ftoi(float) __asm__("FUN_001fa6d0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void vendor_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern float vendor_spring(float *p, float t, float *v, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");
void FUN_L14_002eef60(struct Moby *m);
int FUN_L14_002ef150(struct Moby *m);
void FUN_L14_002ef3e0(struct Moby *m);
void FUN_L14_002ef630(u8 *moby);
void FUN_L14_002ef6f0(struct Moby *moby);

/* pvars of the vendor platform (shared with FUN_L14_002eef60/ef150/ef3e0/ef630) */
typedef struct {
    u8 pad0[0x60];
    u8 spring[0x40];   /* 0x60 */
    s32 path;          /* 0xA0 */
    s32 segment;       /* 0xA4 */
    f32 along;         /* 0xA8 */
    f32 dir;           /* 0xAC */
    s32 travelled;     /* 0xB0 */
    f32 lengthB4;      /* 0xB4 */
    s32 path2;         /* 0xB8 */
    f32 lengthBC;      /* 0xBC */
    f32 vel[4];        /* 0xC0 */
    f32 rot[4];        /* 0xD0 */
    u8 padE0[0x30];
    s32 item;          /* 0x110 */
    s32 target;        /* 0x114 */
    s32 clip_a;        /* 0x118 */
    s32 clip_b;        /* 0x11C */
    s32 clip_c;        /* 0x120 */
    s16 bought;        /* 0x124 */
    s16 level;         /* 0x126 */
    f32 level_vel;     /* 0x128 */
    s32 motor;         /* 0x12C */
    s32 motor_volume;  /* 0x130 */
    s32 hum_volume;    /* 0x134 */
    s32 i138;          /* 0x138 */
} L14VendorVars;
extern s32 D_0014C190_257218[][64] __asm__("D_0014C190");
/* Vendor platform update: rides its two rails between the trigger volumes, with hum and motor sounds. */
void elevator_update(struct Moby *m) __asm__("FUN_L14_002ee8d8");
void elevator_update(struct Moby *m) {
    enum {
        ELEVATOR_STATE_INITIALIZE = 0,
        ELEVATOR_STATE_WAIT_FOR_RIDER = 1,
        ELEVATOR_STATE_FOLLOW_PATH = 2,
        ELEVATOR_STATE_SETTLE_AT_ENDPOINT = 3,
        ELEVATOR_STATE_WAIT_FOR_RIDER_EXIT = 4
    };
    L14VendorVars *d = (L14VendorVars *)m->pvars;
    Vec4 old;
    Vec4 rot;
    Vec4 delta;
    float level;

    if (m->state != 0 && m->command == 2 && d->dir < 0.0f) {
        int *path = (int *)D_L14_001B0BB0_v[d->path2];
        d->dir = -d->dir;
        d->segment = *path - 2;
        d->along = *(float *)((char *)path + (d->segment << 4) + 0x1C);
        m->command = 1;
    }
    qcopy(&old, &m->pos);
    qcopy(&rot, &m->rot);
    switch (m->state) {
    case ELEVATOR_STATE_INITIALIZE: {
        u16 id;
        m->command = 0;
        m->state = 1;
        FUN_L14_002eef60(m);
        qcopy(&m->pos, D_L14_001B0BB0_v[d->path] + 0x10);
        qcopy(&m->rot, d->rot);
        id = m->save_id;
        if (D_L14_001BB930_v.collected[(short)id] == 0 &&
            (((D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 0x1F)) ^ 1) & 1) &&
            visited52[d->item + current_level_index * 16] == 0xFF)
            d->bought = 1;
        else
            d->bought = 0;
        m->update_dist = 0xFF;
        m->glow_color = 0x80303030;
        d->motor = -1;
        d->motor_volume = -1;
        d->hum_volume = -1;
        d->i138 = 0;
        break;
    }
    case ELEVATOR_STATE_WAIT_FOR_RIDER:
        FUN_L14_002ef6f0(m);
        if (d->clip_a >= 0 && vendor_inside(&hero.motion.pos, d->clip_a) != 0) {
            m->command = 0;
            d->dir = -1.0f;
            d->segment = 0;
            d->along = 0;
        }
        if (d->clip_b >= 0 && vendor_inside(&hero.motion.pos, d->clip_b) != 0) {
            char **e;
            m->command = 0;
            e = &D_L14_001B0BB0_v[d->path];
            d->segment = **(int **)e - 2;
            d->along = *(float *)(*e + (d->segment << 4) + 0x1C);
            d->dir = 1.0f;
        }
        if (d->clip_c >= 0 && vendor_inside(&hero.motion.pos, d->clip_c) != 0) {
            char **e;
            m->command = 1;
            e = &D_L14_001B0BB0_v[d->path2];
            d->segment = **(int **)e - 2;
            d->along = *(float *)(*e + (d->segment << 4) + 0x1C);
            d->dir = 1.0f;
        }
        if (hero.ground_moby == m && hero.frames_off_ground.s == 0 &&
            vendor_distance(&hero.motion.pos, &m->pos) < 1.0f) {
            int voice = vendor_sound(1, 0, m);
            if (voice >= 0) {
                if (d->hum_volume < 0)
                    d->hum_volume = *(int *)(D_0013E550 + 0xF0);
                vendor_volume(voice, d->hum_volume);
            }
            if (d->dir == -1.0f) {
                if (d->bought != 0)
                    m->command = 2;
                else
                    m->command = 0;
            }
            m->state = 2;
            d->travelled = 0;
            d->dir = -d->dir;
        }
        break;
    case ELEVATOR_STATE_FOLLOW_PATH:
        if (voice_is_playing(m, d->motor) == 0) {
            d->motor = vendor_sound(0, 4, m);
            if (d->motor != 0) {
                if (d->motor_volume < 0)
                    d->motor_volume = *(int *)(D_0013E550 + 0x80);
                vendor_volume(d->motor, d->motor_volume);
            }
        }
        m->glow_color = fast_tween_color(m->glow_color, 0x80808040, 0.1f);
        if (FUN_L14_002ef150(m) != 0) {
            m->state = 3;
            d->level = d->motor_volume;
        }
        FUN_L14_002ef630((u8 *)m);
        vendor_sub(&delta, &m->pos, &old);
        update_entity_matrix(d->spring, &delta, &rot, &m->rot);
        break;
    case ELEVATOR_STATE_SETTLE_AT_ENDPOINT:
        if (vendor_length(d->vel) >= 0.001f) {
            if (voice_is_playing(m, d->motor) == 0)
                d->motor = vendor_sound(0, 4, m);
            level = vendor_itof(d->level);
            vendor_spring(&level, vendor_itof(d->motor_volume) * 0.25f, &d->level_vel, 0.003f, 0.2f, 0.0f);
            d->level = vendor_ftoi(level);
            if (d->motor >= 0)
                vendor_volume(d->motor, d->level);
            FUN_L14_002ef630((u8 *)m);
        } else {
            int voice;
            FUN_L14_002ef6f0(m);
            if (d->motor != -1) {
                char *e = D_0013E550 + d->motor * 0x70;
                if (*(struct Moby **)(e + 0x88) == m && ((u8 *)e)[0x74] != 0)
                    vendor_release(d->motor);
            }
            d->motor = -1;
            voice = vendor_sound(1, 0, m);
            if (voice >= 0)
                vendor_volume(voice, d->hum_volume / 3);
            m->state = 4;
            if (hero.ground_moby != m)
                m->state = 1;
        }
        break;
    case ELEVATOR_STATE_WAIT_FOR_RIDER_EXIT:
        FUN_L14_002ef6f0(m);
        if (hero.ground_moby != m)
            m->state = 1;
        break;
    }
    if (m->state != 2 && m->state != 0) {
        FUN_L14_002ef3e0(m);
        if (hero.ground_moby == m) {
            vendor_sub(&delta, &m->pos, &old);
            update_entity_matrix(d->spring, &delta, &rot, &m->rot);
        }
    }
}
extern char *D_L14_001B0BB0_rail[] __asm__("D_L14_001B0BB0");
extern float FUN_001f9b48(void *, void *);
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

void FUN_L14_002eef60(struct Moby *m) {
    char *d = (char *)m->pvars;
    int *path;
    int *e;
    int *first;
    int i;

    if (*(int *)(d + 0xA0) == -1) {
        mark_moby_for_removal(m);
        return;
    }
    e = (int *)D_L14_001B0BB0_rail[*(int *)(d + 0xA0)];
    if (*e == 0 || *(int *)(d + 0x114) == -1 ||
        *(int *)D_L14_001B0BB0_rail[*(int *)(d + 0x114)] != 11) {
        mark_moby_for_removal(m);
        return;
    }

    *(float *)(d + 0xB4) = 0.0f;
    path = e;
    first = path + 4;
    for (i = 0; i < *path - 1; i++) {
        ((f32 *)path)[i * 4 + 7] = FUN_001f9b48(&path[i * 4 + 4], &path[i * 4 + 8]);
        *(float *)(d + 0xB4) += ((f32 *)path)[i * 4 + 7];
    }
    {
        f32 len = FUN_001f9b48(&((f32 *)e)[i * 4 + 4], first);
        char *pt = (char *)e + i * 16;
        *(f32 *)(pt + 0x1C) = len;
    }

    path = (int *)D_L14_001B0BB0_rail[*(int *)(d + 0xB8)];
    *(float *)(d + 0xBC) = 0.0f;
    for (i = 0; i < *path - 1; i++) {
        ((f32 *)path)[i * 4 + 7] = FUN_001f9b48(&path[i * 4 + 4], &path[i * 4 + 8]);
        *(float *)(d + 0xBC) += ((f32 *)path)[i * 4 + 7];
    }
    {
        f32 len = FUN_001f9b48(&((f32 *)path)[i * 4 + 4], path + 4);
        char *pt = (char *)path + i * 16;
        *(f32 *)(pt + 0x1C) = len;
    }
    *(float *)(d + 0xAC) = -1.0f;
    *(int *)(d + 0xA8) = 0;
    *(int *)(d + 0xA4) = 0;
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002F05D8), where it is exact; names translated to the US level program. */

extern char *D_L14_001B0BB0_c3[] __asm__("D_L14_001B0BB0");
extern float D_0015ED6C_c __asm__("D_0015ED6C");
extern float FUN_L00_0025b750(float, float, float);
extern float FUN_L00_0025b8c0(float *p, float *v, float t, float u1, float u2, float eps);
extern int sample_path() __asm__("FUN_L00_0025d808");
extern int FUN_L00_0025d808_2f05d8(void *, void *, void *, void *, int,
                                    float) __asm__("FUN_L00_0025d808");

extern short D_L14_00161D14 __attribute__((sda));
int FUN_L14_002ef150(struct Moby *m) {
    char *d = (char *)m->pvars;
    float rot[4];
    float pos[4];
    char *e;
    float total;
    float step;
    float t;
    float s;
    int r;
    if (m->command == 0) {
        e = D_L14_001B0BB0_c3[*(int *)(d + 0xA0)];
        total = *(float *)(d + 0xB4);
        qcopy(rot, d + 0xE0);
    } else {
        e = D_L14_001B0BB0_c3[*(int *)(d + 0xB8)];
        total = *(float *)(d + 0xBC);
        qcopy(rot, d + 0xF0);
    }
    step = *(float *)&D_L14_00161D14 * D_0015ED6C_c;
    r = FUN_L00_0025d808_2f05d8(e, pos, d + 0xA4, d + 0xA8, 0, step * *(float *)(d + 0xAC));
    FUN_L00_0025b8c0(&m->pos.x, (float *)(d + 0xC0), pos[0], 0.003f, 0.2f, 0.0f);
    FUN_L00_0025b8c0(&m->pos.y, (float *)(d + 0xC4), pos[1], 0.003f, 0.2f, 0.0f);
    FUN_L00_0025b8c0(&m->pos.z, (float *)(d + 0xC8), pos[2], 0.003f, 0.2f, 0.0f);
    *(float *)(d + 0xB0) += step;
    t = *(float *)(d + 0xB0) / total;
    if (1.0f < t)
        t = 1.0f;
    if (r != 0)
        t = 1.0f;
    if (*(float *)(d + 0xAC) < 0.0f)
        t = 1.0f - t;
    if (0.0f < *(float *)(d + 0xAC)) {
        s = 0.0f;
        if (0.5f < t)
            s = (t - 0.5f) + (t - 0.5f);
    } else {
        s = 1.0f;
        if (t <= 0.5f)
            s = t + t;
    }
    if (1.0f < s)
        s = 1.0f;
    m->rot.x = FUN_L00_0025b750(*(float *)(d + 0xD0), rot[0], s);
    m->rot.y = FUN_L00_0025b750(*(float *)(d + 0xD4), rot[1], t);
    m->rot.z = FUN_L00_0025b750(*(float *)(d + 0xD8), rot[2], t);
    if (r != 0) {
        if (0.0f < *(float *)(d + 0xAC)) {
            int n = *(int *)e - 2;
            *(int *)(d + 0xA4) = n;
            *(float *)(d + 0xA8) = *(float *)(e + (n << 4) + 0x1C);
        } else {
            *(int *)(d + 0xA4) = 0;
            *(float *)(d + 0xA8) = 0.0f;
        }
        return 1;
    }
    return 0;
}
/* Springs the moby's position (and, while moving, its rotation) toward its current path point. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002F0868), where it is exact; names translated to the US level program. */

extern char *D_L14_001B0BB0_c4[] __asm__("D_L14_001B0BB0");

void FUN_L14_002ef3e0(struct Moby *m) {
    char *d = (char *)m->pvars;
    float pos[4];
    float rot[4];
    char *e;
    if (m->command == 0) {
        e = D_L14_001B0BB0_c4[*(int *)(d + 0xA0)];
        qcopy(rot, d + 0xE0);
    } else {
        e = D_L14_001B0BB0_c4[*(int *)(d + 0xB8)];
        qcopy(rot, d + 0xF0);
    }
    if (*(float *)(d + 0xAC) > 0.0f) {
        qcopy(pos, e + *(int *)e * 16);
    } else {
        qcopy(pos, e + 0x10);
    }
    FUN_L00_0025b8c0(&m->pos.x, (float *)(d + 0xC0), pos[0], 0.003f, 0.2f, 0.0f);
    FUN_L00_0025b8c0(&m->pos.y, (float *)(d + 0xC4), pos[1], 0.003f, 0.2f, 0.0f);
    FUN_L00_0025b8c0(&m->pos.z, (float *)(d + 0xC8), pos[2], 0.003f, 0.2f, 0.0f);
    if (0.0f < *(float *)(d + 0xAC)) {
        FUN_L00_0025b8c0(&m->rot.x, (float *)(d + 0x100), rot[0], 0.003f, 0.2f, 0.0f);
        FUN_L00_0025b8c0(&m->rot.y, (float *)(d + 0x104), rot[1], 0.003f, 0.2f, 0.0f);
        FUN_L00_0025b8c0(&m->rot.z, (float *)(d + 0x108), rot[2], 0.003f, 0.2f, 0.0f);
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Select the vendor moby's state from its value and mark it initialized. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002F0A00), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0xAC];
    float value;
    char padB0[0x74];
    short field124;
} Level14VendorData;

typedef struct {
    char pad0[0x78];
    Level14VendorData *data;
    char pad7C[0x40];
    unsigned char state;
} Level14VendorMoby;

/* Picks state 2 when the data value is negative, else 1, and marks the data active. */
void FUN_L14_002ef578(Level14VendorMoby *moby) {
    Level14VendorData *data = moby->data;
    moby->state = data->value < 0.0f ? 2 : 1;
    data->field124 = 1;
}
/* Clears the vendor moby's flags and marks its id as collected in both bit tables. */

extern int D_0015ED84_m __asm__("D_0015ED84");
extern int D_L14_001BABD0[];

void FUN_L14_002ef5a8(Level14VendorMoby *m) {
    m->state = 0;
    m->data->field124 = 0;
    D_0014C190_257218[D_0015ED84_m][(short)*(unsigned short *)((char *)m + 0xB2) >> 5] |= 1 << (*(unsigned short *)((char *)m + 0xB2) & 0x1F);
    D_L14_001BABD0[(short)*(unsigned short *)((char *)m + 0xB2) >> 5] |= 1 << (*(unsigned short *)((char *)m + 0xB2) & 0x1F);
}
extern u8 D_L14_001E03B0[];
extern u8 *D_L14_001B0BB0_u[] __asm__("D_L14_001B0BB0");
extern void transform_vector_by_basis(u8 *, u8 *, u8 *) __asm__("FUN_001f9cf8");
extern void add_vector_xyz(u8 *, u8 *, u8 *) __asm__("FUN_001f9a10");

/* 0x002EF630, 192 bytes.  Rebuilds the 11 vectors of a table entry from the
 * moby's matrix and position.
 * Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c:
 * func_L14_002F0AB8), where it is exact; names translated to the US level
 * program. */
void FUN_L14_002ef630(u8 *moby) {
    u8 *data = *(u8 **)(moby + 0x78);
    u8 *src = D_L14_001E03B0;
    u8 *dst = D_L14_001B0BB0_u[*(s32 *)(data + 0x114)] + 0x10;
    s32 i = 10;
    f32 one = 1.0f;
    struct Hero *x;
    do {
        transform_vector_by_basis(dst, src, moby + 0xC0);
        src += 0x10;
        i--;
        add_vector_xyz(dst, dst, moby + 0x10);
        *(f32 *)(dst + 0xC) = one;
        dst += 0x10;
    } while (i >= 0);
    x = &hero;
    x->unk22DA = *(u16 *)(data + 0x114);
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002F0B78), where it is exact; names translated to the US level program. */

extern float absolute_float(float) __asm__("func_001F99C0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_sin(float);
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int truncate_float_to_s32();
extern short D_0015EE6C_s __asm__("D_0015ED6C");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void FUN_L14_002ef6f0(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    struct Hero *x = &hero;
    if (absolute_float(moby->pos.x - x->motion.pos.f[0]) > 40.0f ||
        absolute_float(moby->pos.y - x->motion.pos.f[1]) > 40.0f) {
        moby->glow_color = fast_tween_color(moby->glow_color, 0x80303030, 0.1f);
    } else {
        int i, col;
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
/* A stop of the taxi route (D_L14_001600EC, 0x80 bytes each). */
struct L14TaxiStop {
    u8 pad0[0x30];
    f32 pos[4];      /* 0x30 */
    u8 pad40[0x30];
    f32 rot[4];      /* 0x70 */
};
/* pvars of the taxi ship (same layout as FUN_L14_002f1040/f1140/f13f8 use) */
typedef struct {
    u8 pad0[0x20];
    u8 spring[0x40];  /* 0x20 */
    f32 pos[4];       /* 0x60 */
    f32 rot[4];       /* 0x70 */
    s32 points[5];    /* 0x80 */
    u8 pad94[0x18];
    s32 *path;        /* 0xAC: camera path */
    s16 hB0;          /* 0xB0 */
    s16 sound;        /* 0xB2 */
    s16 picked;       /* 0xB4 */
    s16 target;       /* 0xB6 */
    s32 item_a;       /* 0xB8 */
    s32 item_b;       /* 0xBC */
    f32 t;            /* 0xC0 */
    f32 tv;           /* 0xC4 */
    u8 padC8[0x18];
    f32 save_pos[4];  /* 0xE0 */
    f32 save_rot[4];  /* 0xF0 */
    s32 clip[3];      /* 0x100 */
} L14TaxiVars;
extern struct L14TaxiStop *D_L14_001600EC_taxi __asm__("D_L14_001600EC");
extern s32 D_L14_0015F5CC;
extern s32 D_L14_0015F594;
extern char D_0013CAE4[];
extern char D_0013E550[];
extern float advance_accelerated_scalar_c(float, float, float, float, float *, float *) __asm__("FUN_00213f38");
extern f32 taxi_add_rot(f32 a, f32 b) __asm__("FUN_001fa580");
extern f32 taxi_sub_rot(f32, f32) __asm__("FUN_001fa5c8");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int FUN_L05_003195f8(char *);
extern void lerp_vector(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void transpose_rotation_matrix(void *, void *) __asm__("FUN_001fa2d8");
extern void update_entity_matrix(void *a, void *b, void *c, void *d) __asm__("FUN_L00_00260738");
extern void FUN_L00_00266858(void *, int);
extern void FUN_L00_002e8450(int, float, float);
extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");
extern void FUN_L05_00319510(void *);
extern void FUN_L05_00319740(char *);
extern void FUN_L05_003198e8(char *);
extern void taxi_release(s32) __asm__("FUN_0022d798");
extern void taxi_sample(float, s32 *, int, void *, void *, int) __asm__("FUN_00214e58");
extern void taxi_sub(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern float taxi_fabs(float) __asm__("FUN_001f99c0");
extern int taxi_frames(int) __asm__("FUN_001f96f8");
extern int taxi_inside(void *, int) __asm__("FUN_00214720");
extern int taxi_sound(int, int, void *) __asm__("FUN_0022da68");
void FUN_L14_002f1040(struct Moby *moby);
void FUN_L14_002f1140(struct Moby *moby);
struct Moby *FUN_L14_002f12f0(struct Moby *moby, int tag);
void FUN_L14_002f13f8(struct Moby *moby);

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
    L14TaxiVars *d = (L14TaxiVars *)moby->pvars;
    float previous_position[4], previous_rotation[4], position_delta[4], blend_from_position_or_rotation_matrix[4], blend_to_position[4], blend_from_rotation[4], blend_to_rotation[4], hero_relative_position[4];
    float m80[16];
    int ticks = 10;
    int button = 0x10;
    int wrap = 0;
    int flags = 0;
    qcopy(previous_position, &moby->pos);
    qcopy(previous_rotation, &moby->rot);
    FUN_L00_00266858(moby, (moby->flags ^ 1) & 1);
    switch (moby->state) {
    case TAXI_STATE_INITIALIZE:
        d->hB0 = -1;
        d->sound = -1;
        FUN_L14_002f1040(moby);
        break;
    case TAXI_STATE_WAIT_AT_STOP: {
        struct Hero *g = &hero;
        if (g->ground_moby == moby && g->air_frames.s == 0) {
            transpose_rotation_matrix(blend_from_position_or_rotation_matrix, &moby->rot_row0);
            taxi_sub(hero_relative_position, &g->motion.pos, &moby->pos);
            hero_relative_position[3] = 0;
            transform_vector(hero_relative_position, hero_relative_position, blend_from_position_or_rotation_matrix);
            if (taxi_fabs(hero_relative_position[1]) < 1.2f) {
                FUN_L14_002f1140(moby);
                FUN_L14_002f13f8(moby);
                if ((*(int *)D_0013CAE4 & button) && D_L14_0015F594 == 4) {
                    struct Moby *m;
                    moby->state = 3;
                    m = FUN_L14_002f12f0(moby, d->target);
                    if (m != 0) {
                        m->state = 2;
                        m->flags |= 1;
                        m->visible = 0;
                        m->collision = 0;
                    }
                    d->hB0 = -1;
                }
            }
        } else {
            if (d->clip[0] >= 0 && d->picked != 0 && taxi_inside(&hero.motion.pos, d->clip[0])) {
                if (D_L14_0015F5CC < taxi_frames(ticks)) {
                    d->target = 0;
                    d->picked = 0;
                    qcopy_nc(&moby->pos, D_L14_001600EC_taxi[d->points[d->picked]].pos);
                    qcopy_nc(&moby->rot, D_L14_001600EC_taxi[d->points[d->picked]].rot);
                    moby->state = 1;
                    qcopy_nc(d->pos, D_L14_001600EC_taxi[d->points[d->picked]].pos);
                    qcopy_nc(d->rot, D_L14_001600EC_taxi[d->points[d->picked]].rot);
                } else {
                    if (d->picked == 2) {
                        d->picked = 3;
                    } else {
                        d->picked = 1;
                    }
                    FUN_L14_002f1140(moby);
                    moby->state = 3;
                }
            }
            if (d->clip[1] >= 0 && d->picked != 1 && taxi_inside(&hero.motion.pos, d->clip[1])) {
                if (D_L14_0015F5CC < taxi_frames(ticks)) {
                    d->target = 1;
                    d->picked = 1;
                    qcopy_nc(&moby->pos, D_L14_001600EC_taxi[d->points[d->picked]].pos);
                    qcopy_nc(&moby->rot, D_L14_001600EC_taxi[d->points[d->picked]].rot);
                    moby->state = 1;
                    qcopy_nc(d->pos, D_L14_001600EC_taxi[d->points[d->picked]].pos);
                    qcopy_nc(d->rot, D_L14_001600EC_taxi[d->points[d->picked]].rot);
                } else {
                    d->picked = 0;
                    FUN_L14_002f1140(moby);
                    moby->state = 3;
                }
            }
            if (d->clip[2] >= 0 && d->picked < 2 && taxi_inside(&hero.motion.pos, d->clip[2])) {
                if (D_L14_0015F5CC < taxi_frames(ticks)) {
                    d->target = 2;
                    d->picked = 2;
                    qcopy_nc(&moby->pos, D_L14_001600EC_taxi[d->points[d->picked]].pos);
                    qcopy_nc(&moby->rot, D_L14_001600EC_taxi[d->points[d->picked]].rot);
                    moby->state = 1;
                    qcopy_nc(d->pos, D_L14_001600EC_taxi[d->points[d->picked]].pos);
                    qcopy_nc(d->rot, D_L14_001600EC_taxi[d->points[d->picked]].rot);
                } else {
                    d->picked = 2;
                    FUN_L14_002f1140(moby);
                    moby->state = 3;
                }
            }
        }
        break;
    }
    case TAXI_STATE_BLEND_STOP_TO_PATH_START: {
        struct Hero *g = &hero;
        if (g->ground_moby == moby && g->air_frames.s == 0) {
            FUN_L00_002e8450(0, 7.0f, 0.002f);
        }
        advance_accelerated_scalar_c(1.0f, frame_time_sq, frame_time_sq, frame_time * 0.5f, &d->t, &d->tv);
        qcopy_nc(blend_from_position_or_rotation_matrix, D_L14_001600EC_taxi[d->points[d->picked]].pos);
        qcopy_nc(blend_from_rotation, D_L14_001600EC_taxi[d->points[d->picked]].rot);
        taxi_sample(0.0f, d->path, wrap, blend_to_position, blend_to_rotation, flags);
        lerp_vector(d->pos, blend_from_position_or_rotation_matrix, blend_to_position, d->t);
        d->rot[1] = taxi_add_rot(taxi_sub_rot(blend_to_rotation[1], blend_from_rotation[1]) * d->t, blend_from_rotation[1]);
        d->rot[2] = taxi_add_rot(taxi_sub_rot(blend_to_rotation[2], blend_from_rotation[2]) * d->t, blend_from_rotation[2]);
        if (1.0f <= d->t) {
            d->t = 0.0f;
            d->tv = 0.0f;
            moby->state = 4;
        }
        break;
    }
    case TAXI_STATE_BLEND_SAVED_POSE_TO_PATH_START: {
        float f;
        advance_accelerated_scalar_c(1.0f, frame_time_sq, frame_time_sq, frame_time * 0.5f, &d->t, &d->tv);
        qcopy(blend_from_position_or_rotation_matrix, d->save_pos);
        qcopy(blend_from_rotation, d->save_rot);
        taxi_sample_f(d->path, wrap, blend_to_position, blend_to_rotation, flags, 0.0f);
        f = cubic_hermite_scalar(1.0f, 0.0f, 1.0f, 0.0f, d->t);
        lerp_vector(d->pos, blend_from_position_or_rotation_matrix, blend_to_position, f);
        d->rot[1] = taxi_add_rot(taxi_sub_rot(blend_to_rotation[1], blend_from_rotation[1]) * f, blend_from_rotation[1]);
        d->rot[2] = taxi_add_rot(taxi_sub_rot(blend_to_rotation[2], blend_from_rotation[2]) * f, blend_from_rotation[2]);
        if (1.0f <= d->t) {
            d->t = 0.0f;
            d->tv = 0.0f;
            moby->state = 4;
        }
        break;
    }
    case TAXI_STATE_FOLLOW_PATH: {
        struct Hero *g = &hero;
        if (g->ground_moby == moby && g->air_frames.s == 0) {
            FUN_L00_002e8450(0, 7.0f, 0.003f);
        }
        if (FUN_L05_003195f8((char *)moby)) {
            d->t = 0.0f;
            d->tv = 0.0f;
            moby->state = 5;
        }
        break;
    }
    case TAXI_STATE_BLEND_PATH_END_TO_STOP: {
        struct Hero *g;
        advance_accelerated_scalar_c(1.0f, frame_time_sq, frame_time_sq, frame_time * 0.25f, &d->t, &d->tv);
        qcopy_nc(blend_to_position, D_L14_001600EC_taxi[d->points[d->target]].pos);
        qcopy_nc(blend_to_rotation, D_L14_001600EC_taxi[d->points[d->target]].rot);
        taxi_sample_f(d->path, wrap, blend_from_position_or_rotation_matrix, blend_from_rotation, flags, (float)*d->path);
        lerp_vector(d->pos, blend_from_position_or_rotation_matrix, blend_to_position, d->t);
        d->rot[1] = taxi_add_rot(taxi_sub_rot(blend_to_rotation[1], blend_from_rotation[1]) * d->t, blend_from_rotation[1]);
        d->rot[2] = taxi_add_rot(taxi_sub_rot(blend_to_rotation[2], blend_from_rotation[2]) * d->t, blend_from_rotation[2]);
        if (1.0f <= d->t) {
            d->t = 0.0f;
            d->tv = 0.0f;
            moby->state = 1;
            FUN_L05_00319510(moby);
        }
        g = &hero;
        if (g->ground_moby == moby && g->air_frames.s == 0) {
            transpose_rotation_matrix(m80, &moby->rot_row0);
            taxi_sub(hero_relative_position, &g->motion.pos, &moby->pos);
            hero_relative_position[3] = 0;
            transform_vector(hero_relative_position, hero_relative_position, m80);
            if (taxi_fabs(hero_relative_position[1]) < 1.2f) {
                FUN_L14_002f13f8(moby);
                if (*(int *)D_0013CAE4 & button) {
                    qcopy(d->save_pos, d->pos);
                    qcopy(d->save_rot, d->rot);
                    FUN_L14_002f1040(moby);
                    d->picked = d->target;
                    FUN_L14_002f1140(moby);
                    moby->state = 7;
                    qcopy(d->pos, d->save_pos);
                    qcopy(d->rot, d->save_rot);
                    {
                        struct Moby *m = FUN_L14_002f12f0(moby, d->target);
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
        int k = current_level_index * 16;
        if (t[d->item_a + k] == 0xFF || t[d->item_b + k] == 0xFF) {
            unsigned short bits = moby->flags;
            int value = moby->pclass->collision;
            moby->visible = 1;
            moby->collision = value;
            moby->flags = bits & 0xFFFE;
            moby->state = 1;
        }
        break;
    }
    }
    FUN_L05_00319740((char *)moby);
    if (moby->state >= 3 && moby->state <= 4) {
        struct Hero *g = &hero;
        if (g->ground_moby == moby && g->air_frames.s == 0) {
            g->unk1F2 = 2;
            g->unk1F4 = 2;
            FUN_L05_003198e8((char *)moby);
        }
        if (voice_is_playing(moby, d->sound) == 0) {
            d->sound = taxi_sound(0, 4, moby);
        }
    } else if (voice_is_playing(moby, d->sound) != 0) {
        int h = d->sound;
        if (h != -1) {
            char *e = D_0013E550 + h * 0x70;
            if (*(struct Moby **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                taxi_release(h);
            }
        }
        d->sound = -1;
    }
    taxi_sub(position_delta, &moby->pos, previous_position);
    update_entity_matrix(d->spring, position_delta, previous_rotation, &moby->rot);
}
extern float FUN_001f9b48_2f1040(void *, void *) __asm__("FUN_001f9b48");
struct L14PathPoint {
    char pad0[0x30];
    float pos[4];
    char pad40[0x30];
    float rot[4];
};
extern struct L14PathPoint *D_L14_001600EC_2f1040 __asm__("D_L14_001600EC");
struct L14PickVars {
    char pad0[0x60];
    float pos[4];
    float rot[4];
    int points[5];
    char pad94[0x20];
    short picked;
};

void FUN_L14_002f1040(struct Moby *moby) {
    struct L14PickVars *d = (struct L14PickVars *)moby->pvars;
    int i;
    d->picked = -1;
    for (i = 0; i < 5; i++) {
        if (FUN_001f9b48_2f1040(&moby->pos, D_L14_001600EC_2f1040[d->points[i]].pos) < 1.0f) {
            d->picked = i;
            break;
        }
    }
    moby->state = 1;
    {
        int k = d->points[d->picked];
        struct L14PathPoint *t = D_L14_001600EC_2f1040;
        qcopy(d->pos, t[k].pos);
        qcopy(d->rot, t[k].rot);
    }
}
/* Picks the path for the current point and sets the travel speeds; same source as FUN_L16_002e4a58 with five points. */
extern char *D_L14_001B0BB0_c2[] __asm__("D_L14_001B0BB0");
extern float D_0015ED6C_u __asm__("D_0015ED6C");
extern float FUN_001f9b48(void *, void *);

void FUN_L14_002f1140(struct Moby *moby) {
    enum {
        TAXI_STOP_RETURN_HUB = 0,
        TAXI_STOP_HUB_OUTBOUND_DESTINATION = 1,
        TAXI_STOP_RETURN_FEEDER_ORIGIN = 2,
        TAXI_STOP_RETURN_FEEDER_DESTINATION = 3
    };
    char *d = (char *)moby->pvars;
    char *p;
    float inv, a, b, r;
    switch (*(short *)(d + 0xB4)) {
    case TAXI_STOP_RETURN_HUB:
        *(char **)(d + 0xAC) = D_L14_001B0BB0_c2[*(int *)(d + 0x94)];
        *(short *)(d + 0xB6) = 1;
        break;
    case TAXI_STOP_HUB_OUTBOUND_DESTINATION:
        *(char **)(d + 0xAC) = D_L14_001B0BB0_c2[*(int *)(d + 0x98)];
        *(short *)(d + 0xB6) = 0;
        break;
    case TAXI_STOP_RETURN_FEEDER_ORIGIN:
        *(char **)(d + 0xAC) = D_L14_001B0BB0_c2[*(int *)(d + 0x9C)];
        *(short *)(d + 0xB6) = 3;
        break;
    case TAXI_STOP_RETURN_FEEDER_DESTINATION:
        *(char **)(d + 0xAC) = D_L14_001B0BB0_c2[*(int *)(d + 0xA0)];
        *(short *)(d + 0xB6) = 0;
        break;
    case 4:
        break;
    }
    p = *(char **)(d + 0xAC);
    r = FUN_001f9b48(p + 0x10, p + 0x20);
    a = D_0015ED6C_u * 5.0f;
    b = frame_time_sq * 2.5f;
    inv = 1.0f / (r * (float)**(int **)(d + 0xAC));
    *(int *)(d + 0xC4) = 0;
    *(int *)(d + 0xC0) = 0;
    *(float *)(d + 0xD4) = a * inv;
    *(float *)(d + 0xD8) = b * inv;
}
/* Finds the moby in this moby's group list whose vars tag at 0xB4 matches, or returns null. */
struct TaggedVars {
    u8 pad0[0xB4];
    s16 tag;   /* 0xB4 */
};

extern s32 D_L14_0015FFF4;
extern u16 *D_L14_001ABF40[];
extern struct Moby *D_L14_0015FFD8_12f0 __asm__("D_L14_0015FFD8");

struct Moby *FUN_L14_002f12f0(struct Moby *moby, int tag) {
    u16 *p;
    struct Moby *pool;
    int idx;

    if (moby->group < D_L14_0015FFF4 && (p = D_L14_001ABF40[moby->group]) != 0) {
        pool = D_L14_0015FFD8_12f0;
        do {
            idx = *p & 0x7FFF;
            if (((struct TaggedVars *)pool[idx].pvars)->tag == tag) {
                goto found;
            }
        } while ((s16)*p++ >= 0);
    }
    return 0;
found:
    return &D_L14_0015FFD8_12f0[idx];
}
#ifndef NOT_SDA
#define NOT_SDA __attribute__((section(".data")))
#endif
#ifndef MACRO_ADDR
#define MACRO_ADDR __attribute__((section(".sdata")))
#endif
extern void func_00215F80_f13f8(int, int) __asm__("FUN_00215130");
void FUN_L14_002f13f8(struct Moby *moby) {
    enum {
        TAXI_STOP_RETURN_HUB = 0,
        TAXI_STOP_HUB_OUTBOUND_DESTINATION = 1,
        TAXI_STOP_RETURN_FEEDER_ORIGIN = 2,
        TAXI_STOP_RETURN_FEEDER_DESTINATION = 3
    };
    char *data = (char *)moby->pvars;
    int v = *(short *)(data + 0xB4);
    if (moby->state == 5) {
        v = *(short *)(data + 0xB6);
    }
    switch (v) {
    case TAXI_STOP_RETURN_HUB:
        func_00215F80_f13f8(4, 0x53ED);
        break;
    case TAXI_STOP_HUB_OUTBOUND_DESTINATION:
        func_00215F80_f13f8(4, 0x53EE);
        break;
    case TAXI_STOP_RETURN_FEEDER_ORIGIN:
        func_00215F80_f13f8(4, 0x53EE);
        break;
    case TAXI_STOP_RETURN_FEEDER_DESTINATION:
        func_00215F80_f13f8(4, 0x53EE);
        break;
    case 4:
        func_00215F80_f13f8(4, 0x1399);
        break;
    default:
        func_00215F80_f13f8(4, 0x139B);
        break;
    }
}
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void FUN_001f9a10_c(void *, void *, void *) __asm__("FUN_001f9a10");
#include "sda.h"
extern unsigned char D_0015EDB0;
typedef struct {
    float target[4];
    float eye[4];
    float delta[4];
} L14WatchScratch;
typedef struct {
    char pad00[0x10];
    float position[4];
    unsigned char state;
    char pad21[0xF];
    unsigned char opacity;
    unsigned char near_flag;
    char pad32[0x16];
    float yaw;
    char pad4C[7];
    unsigned char animation;
    char pad54[0x24];
    char *data;
    char pad7C[3];
    unsigned char flag7F;
    char pad80[0x30];
    unsigned char bB0;
    char padB1;
    unsigned short hB2;
} L14WatchMoby;
typedef struct {
    char pad00[4];
    unsigned short sub;
    char pad06[2];
    unsigned char b8;
    char pad09[3];
    float fC;
    char pad10[0x10];
    void *p20;
    char pad24[0x1C];
    int i40;
    int i44;
    int i48;
    char pad4C[0x34];
    char body[0x64];
    float pitch;
    float yaw;
    char padEC[4];
    float height;
    char padF4[0xC];
    char head[0x68];
    float head_yaw;
    char pad16C[0x14];
    float target[4];
    char pad190[4];
    int moving_timer;
    int target_timer;
} L14WatchData;
typedef struct {
    char pad00[0xD0];
    float aim[4];
} L14Player;
extern unsigned char D_0015EDB0_b __asm__("D_0015EDB0");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int fast_dec_timer(int *) __asm__("FUN_001f9740");
extern int FUN_001fa6d0(float);
extern int FUN_00214720_c(void *, int) __asm__("FUN_00214720");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void FUN_L00_0025a120(void *);
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");

typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} L14LevelState;
extern L14LevelState D_L14_001BB930;
extern int D_0015ED84_m __asm__("D_0015ED84");
extern int D_L14_001BABD0[];
extern char D_L14_00161F20[];
extern char D_L14_001674C0[];
extern char *D_L14_001600EC;
extern unsigned char *D_L14_0015FFD8;
extern int D_L14_0015F5C4;
extern void FUN_L02_002e0cd8(void *);
extern void initialize_npc_dialogue(void *, void *) __asm__("FUN_L00_002668a0");
extern int update_npc_dialogue(void *, void *) __asm__("FUN_L00_00266448");
extern void begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
extern void jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void FUN_L14_002ef578_c(void *) __asm__("FUN_L14_002ef578");
void FUN_L14_002ef5a8(Level14VendorMoby *m);
extern void FUN_L00_00263d40(int, int);
extern void FUN_L00_00260860(int, int);
extern void memcard_save_data(int, int) __asm__("FUN_0020b178");

/* Watcher moby update: gated by its collectable and trigger, then turns its head toward the hero or a random target. */
void qwark_oltanis_update(L14WatchMoby *m) __asm__("FUN_L14_002fba20");
void qwark_oltanis_update(L14WatchMoby *m) {
    enum {
        QWARK_OLTANIS_STATE_INITIALIZE_DIALOGUE = 0,
        QWARK_OLTANIS_STATE_UPDATE_DIALOGUE = 1,
        QWARK_OLTANIS_STATE_POST_DIALOGUE = 2,
        QWARK_OLTANIS_STATE_REMOVE = 3
    };
    L14WatchScratch scratch;
    L14WatchData *d = (L14WatchData *)m->data;
    float rate;
    float head_rate;
    int tracking;
    FUN_L02_002e0cd8(m);
    if (m->near_flag) {
        if (FUN_001f9b48(m->position, D_L14_001674C0) < 28.0f) {
            FUN_L00_0025a120(m);
            m->flag7F = 0x16;
        }
    }
    switch (m->state) {
    case QWARK_OLTANIS_STATE_INITIALIZE_DIALOGUE: {
        unsigned short id = m->hB2;
        if (D_L14_001BB930.collected[(short)id] != 0 ||
            (D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 0x1F)) & 1) {
            mark_moby_for_removal(m);
            return;
        }
        if (d->i44 == -1 || d->i48 == -1) {
            mark_moby_for_removal(m);
            return;
        }
        d->p20 = (void *)D_L14_00161F20;
        initialize_npc_dialogue(m, d);
        d->b8 = 1;
        m->state = 1;
        break;
    }
    case QWARK_OLTANIS_STATE_UPDATE_DIALOGUE:
        if (d->b8 == 1 && FUN_00214720_c(&hero.motion.pos, d->i40)) {
            if (visited52[m->bB0 + current_level_index * 16] != 0xFF) {
                char *t;
                jet_clear_persistent(m->bB0);
                t = D_L14_001600EC + d->i48 * 128;
                jet_reset_camera(t + 0x30, t + 0x70);
            }
            FUN_L14_002ef578_c(D_L14_0015FFD8 + d->i44 * 256);
            d->fC = 255.0f;
        } else {
            d->fC = 1.7f;
        }
        if (update_npc_dialogue(m, d)) {
            begin_dialogue_camera_transition(m, 3.0f);
            m->state = 2;
        }
        break;
    case QWARK_OLTANIS_STATE_POST_DIALOGUE:
        if (D_L14_0015F5C4 != 2) {
            m->state = 1;
            if ((unsigned)(d->sub - 3) < 2) {
                if ((short)d->sub == 4)
                    FUN_L00_00263d40(0x53DD, -1);
                else
                    FUN_L00_00263d40(0x36B3, -1);
                FUN_L00_00260860(0x20, 1);
                FUN_L14_002ef5a8(D_L14_0015FFD8 + d->i44 * 256);
                D_0014C190_257218[D_0015ED84_m][(short)m->hB2 >> 5] |= 1 << (m->hB2 & 0x1F);
                D_L14_001BABD0[(short)m->hB2 >> 5] |= 1 << (m->hB2 & 0x1F);
                memcard_save_data(0, -1);
                mark_moby_for_removal(m);
                return;
            }
        }
        break;
    case QWARK_OLTANIS_STATE_REMOVE:
        mark_moby_for_removal(m);
        return;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (m->animation == 0) {
        char *g = (char *)&hero.motion.pos;
        L14Player *q;
        tracking = 1;
        if (vector_distance(m->position, g) < 8.0f &&
            (q = (L14Player *)(g - 0x80),
             fast_difference_between_rotations(
                 m->yaw, fast_arc_tan(q->aim[0] - m->position[0], q->aim[1] - m->position[1]))) <
                1.5707964f) {
            if (fast_vec_length(g + 0x80) > 0.01f)
                d->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&d->moving_timer);
        } else if (d->moving_timer) {
            d->moving_timer = 0;
            qcopy(d->target, hero.motion.unkD0.f);
        }
        if (fast_dec_timer(&d->target_timer)) {
            float heading;
            d->target_timer = FUN_001fa6d0(multiply_global_factor_ed64(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(m->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(d->target, 6.0f, heading,
                                   random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            FUN_001f9a10_c(d->target, d->target, m->position);
        }
        if (d->moving_timer) {
            qcopy(scratch.target, hero.motion.unkD0.f);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, d->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, m->position);
        scratch.eye[2] += 1.0f;
        fast_vec_sub(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(fast_arc_tan(scratch.delta[0], scratch.delta[1]), m->yaw);
        pitch = -fast_arc_tan(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        d->pitch = pitch;
        d->yaw = yaw * 0.6f;
        d->head_yaw = yaw * 0.4f;
    }
    if (D_0015EDB0_b)
        d->height = 2.75f;
    update_joint_animation(rate * frame_scale_sq, head_rate * frame_scale_sq, m, d->body, 0);
    update_joint_animation(rate * frame_scale_sq, head_rate * frame_scale_sq, m, d->head, 1);
}
extern int cart_hit(struct Moby *, int, int) __asm__("FUN_L00_0025a420");
extern void cart_burst(float, float, struct Moby *, void *, int) __asm__("FUN_L00_0025f090");
extern void cart_bank_voice(int, int, int) __asm__("FUN_0022db10");
extern void cart_dialog(int, int) __asm__("FUN_L00_00263d40");
extern void cart_release(int) __asm__("FUN_0022d798");
extern void FUN_L14_002fc890(struct Moby *);
extern void FUN_L14_002fc9a0(struct Moby *);
extern void FUN_L14_002fc3e8(struct Moby *);
extern void cart_print(const char *, short) __asm__("FUN_001e93b0");
extern void cart_remove(struct Moby *) __asm__("FUN_0020c828");
extern int cart_frames(int) __asm__("FUN_001f96f8");
extern int cart_tick(void *) __asm__("FUN_001f9740");
extern float cart_atan2(float, float) __asm__("FUN_001f9e90");
extern int cart_voice_alive(struct Moby *, int) __asm__("FUN_L00_0028d8c0");
extern int cart_sound(int, int, struct Moby *) __asm__("FUN_0022da68");
extern int D_0015EE08;
extern struct { char pad[0x18]; unsigned char flag; } D_0013D408;
extern s32 D_L14_00161F5C __attribute__((sda));
extern char D_0013E550[];
char D_L14_001FCE10[] = "Enemy Fighter (moby908) with no flyPath. UID: %d - RAR\n";
char D_L14_001FCE48[] = "Enemy Fighter (moby908) with a flyPath that has no points. UID: %d - RAR\n";
extern char *D_L14_001B0BB0_cart[] __asm__("D_L14_001B0BB0");

typedef struct {
    u8 pad0[0x60];
    s32 segment;     /* 0x60 */
    s32 along;       /* 0x64 */
    s32 path;        /* 0x68 */
    s32 sound;       /* 0x6C */
    f32 f70;         /* 0x70 */
    f32 f74;         /* 0x74 */
    f32 f78;         /* 0x78 */
    s32 timer;       /* 0x7C */
    f32 f80;         /* 0x80 */
    f32 f84;         /* 0x84 */
    f32 f88;         /* 0x88 */
} L14CartVars;

/* Cart update: follows its path, bursts when shot and respawns at the path start after a delay. */
void FUN_L14_002fc0f0(struct Moby *moby) {
    enum {
        ENEMY_FIGHTER_STATE_INITIALIZE = 0,
        ENEMY_FIGHTER_STATE_FOLLOW_PATH = 1,
        ENEMY_FIGHTER_STATE_WAIT_FOR_RESPAWN = 2
    };
    L14CartVars *data = (L14CartVars *)moby->pvars;
    int hit = cart_hit(moby, 0x800000, 0);
    int index;
    int path_index;
    char *path;
    moby->hit_slot = 0xff;
    if (hit && moby->state != 2) {
        if (current_level_index == 14) {
            D_0015EE08 = D_0015EE08 + 1;
            if (D_0015EE08 > 2 && D_0013D408.flag == 0) {
                D_0013D408.flag = 1;
                cart_bank_voice(1, 0, 0);
                cart_dialog(0x53d6, -1);
            }
        }
        moby->state = 2;
        cart_burst(5.0f, 13.0f, moby, &moby->pos, -1);
        moby->collision = 0;
        moby->flags |= 0x41;
        index = data->sound;
        if (index != -1) {
            char *entry = D_0013E550 + index * 0x70;
            if (*(struct Moby **)(entry + 0x88) == moby && *(unsigned char *)(entry + 0x74) != 0)
                cart_release(index);
        }
        data->sound = -1;
    }
    switch (moby->state) {
    case ENEMY_FIGHTER_STATE_INITIALIZE:
        path_index = data->path;
        if (path_index == -1) {
            cart_print(D_L14_001FCE10, moby->save_id);
            cart_remove(moby);
            return;
        }
        if (*(int *)D_L14_001B0BB0_cart[path_index] == 0) {
            cart_print(D_L14_001FCE48, moby->save_id);
            cart_remove(moby);
            return;
        }
        FUN_L14_002fc890(moby);
        moby->state = 1;
        moby->update_dist = 0xff;
        moby->draw_dist = 0xff;
        data->sound = -1;
        data->timer = cart_frames(D_L14_00161F5C);
        break;
    case ENEMY_FIGHTER_STATE_FOLLOW_PATH:
        FUN_L14_002fc9a0(moby);
        break;
    case ENEMY_FIGHTER_STATE_WAIT_FOR_RESPAWN:
      if (cart_tick(&data->timer)) {
        path = D_L14_001B0BB0_cart[data->path];
        moby->flags &= 0xffbe;
        moby->collision = moby->pclass->collision;
        data->timer = cart_frames(D_L14_00161F5C);
        moby->state = 1;
        data->segment = 0;
        data->along = 0;
        qcopy(&moby->pos, path + 0x10);
        moby->rot.z = cart_atan2(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                           *(float *)(path + 0x24) - *(float *)(path + 0x14));
        data->f80 = data->f84 =
            data->f88 = data->f70 =
            data->f74 = data->f78 = 0.0f;
      }
      break;
    }
    FUN_L14_002fc3e8(moby);
    if (moby->state != 0 && moby->state != 2 &&
        cart_voice_alive(moby, data->sound) == 0)
        data->sound = cart_sound(0, 4, moby);
}
/* A smoke puff from FUN_L00_0026d000. */
typedef struct {
    u8 pad0[4];
    s32 mode;        /* 0x04 */
    u8 pad8[2];
    u8 tint;         /* 0x0A */
    u8 fade;         /* 0x0B */
} L14PuffTail;

typedef struct {
    u8 pad0[3];
    u8 alpha;        /* 0x03 */
    u8 pad4[4];
    u8 shade;        /* 0x08 */
    s8 frame;        /* 0x09 */
    s16 life;        /* 0x0A */
    u8 padC[0x14];
    L14PuffTail tail; /* 0x20 */
} L14Puff;

extern f32 D_L14_00161F30 __attribute__((sda));
extern f32 D_L14_00161F34 __attribute__((sda));
extern f32 D_L14_00161F38 __attribute__((sda));
extern f32 D_L14_00161F3C __attribute__((sda));
extern s32 D_L14_00161F44 __attribute__((sda));
extern char D_L14_0015F580[] __attribute__((section(".sdata")));
extern void exhaust_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern void exhaust_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void exhaust_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern int exhaust_rand(int) __asm__("FUN_00213260");
extern s32 exhaust_frames(s32) __asm__("FUN_001f96f8");
extern int exhaust_ftoi(float) __asm__("FUN_001fa6d0");
extern L14Puff *exhaust_puff(void *, f32, f32, f32, int, void *, f32, int) __asm__("FUN_L00_0026d000");

/* Engine exhaust: two smoke puffs and three fading rings on each side of the moby. */
void FUN_L14_002fc3e8(struct Moby *m) {
    Vec4 step;
    Vec4 pos;
    Vec4 base;
    L14Puff *puff;
    int i;
    int j;
    int r;
    int spin;
    int life;
    float size;

    if (m->state == 0 || m->state == 2 || m->visible == 0)
        return;
    exhaust_scale(&step, &m->rot_row0, D_L14_00161F30);
    exhaust_add(&pos, &m->pos, &step);
    exhaust_scale(&step, &m->rot_row2, D_L14_00161F38);
    exhaust_add(&pos, &pos, &step);
    qcopy(&base, &pos);
    exhaust_scale(&step, &m->rot_row1, D_L14_00161F34);
    exhaust_add(&pos, &pos, &step);
    for (j = 0; j < 2; j++) {
        spin = exhaust_rand(16);
        puff = exhaust_puff(&pos, 0.2f, 1.0f, 0.9f, exhaust_rand(2) == 0 ? spin : -spin, D_L14_0015F580, 200000.0f, D_L14_00161F44);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->life = exhaust_frames(12);
            puff->frame = (m->draw_dist >> 5 << 4) + exhaust_ftoi(4.0f);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
    }
    spin = 16;
    size = 100000.0f;
    exhaust_scale(&step, &m->rot_row0, D_L14_00161F3C);
    exhaust_add(&pos, &pos, &step);
    life = exhaust_frames(2);
    for (i = 0; i < 3; i++) {
        puff = exhaust_puff(&pos, 0.05f, 1.0f, 1.0f, spin, D_L14_0015F580, size, 0x7FFFFFFF);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->frame = (m->draw_dist >> 5 << 4) + exhaust_ftoi(4.0f);
            puff->life = life;
            puff->shade = exhaust_rand(0xFF);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
        spin = -spin;
        life *= 2;
        size -= 20000.0f;
    }
    exhaust_scale(&step, &m->rot_row1, D_L14_00161F34);
    exhaust_sub(&pos, &base, &step);
    for (j = 0; j < 2; j++) {
        spin = exhaust_rand(16);
        puff = exhaust_puff(&pos, 0.2f, 1.0f, 0.9f, exhaust_rand(2) == 0 ? spin : -spin, D_L14_0015F580, 200000.0f, D_L14_00161F44);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->frame = (m->draw_dist >> 5 << 4) + exhaust_ftoi(4.0f);
            puff->life = exhaust_frames(12);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
    }
    spin = 16;
    size = 100000.0f;
    exhaust_scale(&step, &m->rot_row0, D_L14_00161F3C);
    exhaust_add(&pos, &pos, &step);
    life = exhaust_frames(2);
    for (i = 0; i < 3; i++) {
        puff = exhaust_puff(&pos, 0.05f, 1.0f, 1.0f, spin, D_L14_0015F580, size, 0x7FFFFFFF);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->frame = (m->draw_dist >> 5 << 4) + exhaust_ftoi(4.0f);
            puff->life = life;
            puff->shade = exhaust_rand(0xFF);
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

extern char *D_L14_001B0BB0_c[] __asm__("D_L14_001B0BB0");
extern float FUN_001f9b48(void *, void *);

void FUN_L14_002fc890(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float *path = (float *)D_L14_001B0BB0_c[*(int *)(data + 0x68)];
    int i;
    for (i = 0; i < *(int *)path - 1; i++) {
        path[i * 4 + 7] = FUN_001f9b48(&path[i * 4 + 4], &path[i * 4 + 8]);
    }
    *(int *)(data + 0x60) = 0;
    *(int *)(data + 0x64) = 0;
    qcopy(&moby->pos, path + 4);
    moby->rot.z = fast_arc_tan(path[8] - path[4], path[9] - path[5]);
    *(float *)(data + 0x80) = *(float *)(data + 0x84) = *(float *)(data + 0x88) =
        *(float *)(data + 0x70) = *(float *)(data + 0x74) = *(float *)(data + 0x78) = 0.0f;
}
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002FDE28), where it is exact; names translated to the US level program. */

extern char *D_L14_001B0BB0_c5[] __asm__("D_L14_001B0BB0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 vector_length_xy_c(void *) __asm__("FUN_001f9b20");
extern float D_0015ED6C_c2 __asm__("D_0015ED6C");
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void FUN_L00_0025d808_c(int a, char *pos, char *b, char *c, int d, float f) __asm__("FUN_L00_0025d808");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");

extern short D_L14_00161F48 __attribute__((sda));
extern short D_L14_00161F4C __attribute__((sda));
extern short D_L14_00161F50 __attribute__((sda));
extern short D_L14_00161F54 __attribute__((sda));
extern short D_L14_00161F58 __attribute__((sda));
void FUN_L14_002fc9a0(struct Moby *moby) {
    char *d = (char *)moby->pvars;
    float v[3];
    float t[3];
    float a, b, c;
    FUN_L00_0025d808_c((int)D_L14_001B0BB0_c5[*(int *)(d + 0x68)], (char *)v, d + 0x60, d + 0x64, 1, (*(float *)&D_L14_00161F48) * D_0015ED6C_c2);
    FUN_L00_0025b8c0(&moby->pos.x, (float *)(d + 0x70), v[0], (*(float *)&D_L14_00161F4C), (*(float *)&D_L14_00161F50), 0.0f);
    FUN_L00_0025b8c0(&moby->pos.y, (float *)(d + 0x74), v[1], (*(float *)&D_L14_00161F4C), (*(float *)&D_L14_00161F50), 0.0f);
    FUN_L00_0025b8c0(&moby->pos.z, (float *)(d + 0x78), v[2], (*(float *)&D_L14_00161F4C), (*(float *)&D_L14_00161F50), 0.0f);
    subtract_vector_xyz_c(t, v, &moby->pos);
    if (absolute_float(t[0]) > 0.01f) {
        if (absolute_float(t[1]) > 0.01f) {
            a = fast_arc_tan(t[0], t[1]);
            b = fast_subtract_rotations(moby->rot.z, a);
            FUN_L00_00258278(moby, (float *)(d + 0x88), a, (*(float *)&D_L14_00161F4C), (*(float *)&D_L14_00161F50), 0.0f);
            c = fast_arc_tan(vector_length_xy_c(t), t[2]);
            FUN_L00_0025b8c0(&moby->rot.y, (float *)(d + 0x84), -c, (*(float *)&D_L14_00161F4C), (*(float *)&D_L14_00161F50), 0.0f);
            b = b / (*(float *)&D_L14_00161F58);
            if (b > 1.0f) b = 1.0f;
            if (b < -1.0f) b = -1.0f;
            FUN_L00_0025b8c0(&moby->rot.x, (float *)(d + 0x80), b * (*(float *)&D_L14_00161F54), 0.003f, (*(float *)&D_L14_00161F50), 0.0f);
        }
    }
}
/* Camera target of the level (D_L14_00167380): position and view angles. */
struct L14ViewTarget {
    u8 pad0[0x140];
    f32 pos[3];      /* 0x140 */
    u8 pad14C[8];
    f32 pitch;       /* 0x154 */
    f32 yaw;         /* 0x158 */
};
extern struct L14ViewTarget D_L14_00167380_v __asm__("D_L14_00167380");
extern struct Moby *D_L14_0015FFD8_t __asm__("D_L14_0015FFD8");
extern char *D_L14_001B0BB0_t[] __asm__("D_L14_001B0BB0");
struct L14TramLevel {
    u8 pad0[0x454];
    u8 collected[1];
};
extern struct L14TramLevel D_L14_001BB930_t __asm__("D_L14_001BB930");
extern char D_0013E550[];
extern int tram_frames(int) __asm__("FUN_001f96f8");
extern void tram_release(s32) __asm__("FUN_0022d798");
extern int tram_inside(void *, int) __asm__("FUN_00214720");
extern int tram_sound(int, int, void *) __asm__("FUN_0022da68");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int D_L14_001BABD0[];
extern unsigned char D_0013D408_t[] __asm__("D_0013D408");
extern char D_L14_001FCE98[];
extern char D_L14_001FCED0[];
extern s32 D_L14_00161F8C __attribute__((sda));
extern void FUN_L14_002fe238(char *moby);
extern void FUN_L06_00303150(unsigned char *moby);
extern void FUN_L10_002f5a50(int);
extern void *tram_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");
extern void FUN_L00_00263d40(int, int);
extern void FUN_L00_0025f090(void *, void *, int, float, float);
extern void tram_print(const char *, short) __asm__("FUN_001e93b0");
extern void tram_remove(void *) __asm__("FUN_0020c828");
extern float tram_distance(void *, void *) __asm__("FUN_001f9b80");
extern float tram_atan2(float, float) __asm__("FUN_001f9e90");
extern float tram_angle_diff(float, float) __asm__("FUN_001fa688");
extern int tram_coll_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int tram_tick(void *) __asm__("FUN_001f9740");
extern void tram_scale(void *, void *, f32) __asm__("FUN_001f9a68");
extern void tram_add(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_L14_002fc890(struct Moby *moby);
void FUN_L14_002fd310(struct Moby *m);
int FUN_L14_002fd918(struct Moby *m);

/* pvars of the tram (FUN_L14_002fcbb0 and its helpers fc890/fd918 read the same layout) */
typedef struct {
    u8 pad0[0x60];
    s32 segment;     /* 0x60 */
    s32 along;       /* 0x64 */
    s32 path;        /* 0x68 */
    s32 sound;       /* 0x6C */
    f32 f70;         /* 0x70 */
    f32 f74;         /* 0x74 */
    f32 f78;         /* 0x78 */
    s32 timer;       /* 0x7C */
    f32 f80;         /* 0x80 */
    f32 f84;         /* 0x84 */
    f32 f88;         /* 0x88 */
    s32 carrier;     /* 0x8C */
    s32 i90;         /* 0x90 */
    s32 stage;       /* 0x94 */
    s32 item[3];     /* 0x98 */
    s32 clip;        /* 0xA4 */
} L14TramVars;
/* Tram update: carries its rider moby, unlocks once the three items are bought and the camera looks at it. */
void FUN_L14_002fcbb0(struct Moby *m) {
    enum {
        TRAM_STATE_INITIALIZE = 0,
        TRAM_STATE_WAIT_FOR_UNLOCK = 1,
        TRAM_STATE_FOLLOW_PATH = 2,
        TRAM_STATE_WAIT_FOR_RESPAWN = 3
    };
    L14TramVars *d = (L14TramVars *)m->pvars;
    Vec4 b;
    Vec4 a;

    if (m->state == 0) {
        u16 id = m->save_id;
        if (D_L14_001BB930_t.collected[(short)id] != 0 ||
            (D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 0x1F)) & 1) {
            FUN_L14_002fe238((char *)&D_L14_0015FFD8_t[d->carrier]);
            tram_remove(m);
            return;
        }
    }
    {
        void *hit = tram_hit(m, 0x800000, 0);
        m->hit_slot = 0xFF;
        if (hit != 0 && d->carrier == -1 && m->state != 3) {
            if (current_level_index == 14) {
                D_0015EE08 = D_0015EE08 + 1;
                if (D_0015EE08 >= 3 && D_0013D408_t[0x18] == 0) {
                    D_0013D408_t[0x18] = 1;
                    allocate_voice_for_bank_entry(1, 0, 0);
                    FUN_L00_00263d40(0x53D6, -1);
                }
            }
            m->state = 3;
            FUN_L00_0025f090(m, &m->pos, -1, 5.0f, 13.0f);
            m->collision = 0;
            m->flags |= 0x41;
            if (d->sound != -1) {
                char *e = D_0013E550 + d->sound * 0x70;
                if (*(struct Moby **)(e + 0x88) == m && ((u8 *)e)[0x74] != 0)
                    tram_release(d->sound);
            }
            d->sound = -1;
        }
    }
    switch (m->state) {
    case TRAM_STATE_INITIALIZE:
        if (d->path == -1) {
            tram_print(D_L14_001FCE98, m->save_id);
            tram_remove(m);
            return;
        }
        if (*(int *)D_L14_001B0BB0_t[d->path] == 0) {
            tram_print(D_L14_001FCED0, m->save_id);
            tram_remove(m);
            return;
        }
        if (d->item[0] < 0 || d->item[1] < 0 || d->item[2] < 0) {
            tram_remove(m);
            return;
        }
        FUN_L14_002fc890(m);
        if (d->carrier != 0) {
            struct Moby *c;
            m->state = 1;
            m->update_dist = 0xFF;
            m->flags |= 0x41;
            m->collision = 0;
            c = &D_L14_0015FFD8_t[d->carrier];
            c->flags |= 0x41;
            d->stage = 0;
        } else {
            m->state = 2;
            m->update_dist = 0x80;
        }
        m->draw_dist = 0x80;
        d->sound = -1;
        d->timer = tram_frames(D_L14_00161F8C);
        break;
    case TRAM_STATE_WAIT_FOR_UNLOCK: {
        int lvl = current_level_index * 16;
        u8 v = visited52[d->item[0] + lvl];
        if (v == 0xFF) {
            u8 w = visited52[d->item[1] + lvl];
            if (w == v) {
                u8 x = visited52[d->item[2] + lvl];
                if (x == w && hero.frames_off_ground.s == 0 && hero.ground_moby == 0 &&
                    tram_inside(&hero.motion.pos, d->clip) != 0) {
                    struct Moby *carrier = &D_L14_0015FFD8_t[d->carrier];
                    struct Moby *rider = &D_L14_0015FFD8_t[((int *)carrier->pvars)[1]];
                    float pitch;
                    qcopy(&b, &rider->pos);
                    b.f[2] += 4.0f;
                    pitch = tram_angle_diff(D_L14_00167380_v.pitch,
                                            -tram_atan2(tram_distance(D_L14_00167380_v.pos, &b), b.f[2] - D_L14_00167380_v.pos[2]));
                    if (tram_angle_diff(D_L14_00167380_v.yaw, tram_atan2(rider->pos.x - D_L14_00167380_v.pos[0],
                                                                         rider->pos.y - D_L14_00167380_v.pos[1])) < 0.4886922f &&
                        pitch < 0.34906584f &&
                        tram_coll_line(D_L14_00167380_v.pos, &b, 0x12, rider, 0) == 0) {
                        D_0014C190_257218[current_level_index][(short)m->save_id >> 5] |= 1 << (m->save_id & 0x1F);
                        D_L14_001BABD0[(short)m->save_id >> 5] |= 1 << (m->save_id & 0x1F);
                        m->flags &= 0xFFBE;
                        m->collision = m->pclass->collision;
                        carrier->flags &= 0xFFBE;
                        m->state = 2;
                        FUN_L10_002f5a50(d->i90);
                        rider->draw_dist = 0xFF;
                        rider->update_dist = 0xFF;
                    }
                }
            }
        }
        break;
    }
    case TRAM_STATE_FOLLOW_PATH:
        if (FUN_L14_002fd918(m) != 0)
            return;
        if (d->carrier != 0 && d->stage < 2) {
            struct Moby *c;
            if (d->stage == 0 && d->segment >= 0x38)
                d->stage = 1;
            tram_scale(&a, &m->rot_row0, 0.21f);
            tram_scale(&b, &m->rot_row2, -1.24f);
            tram_add(&a, &a, &b);
            tram_add(&a, &m->pos, &a);
            c = &D_L14_0015FFD8_t[d->carrier];
            qcopy(&c->pos, &a);
            qcopy(&c->rot, &m->rot);
            if (d->stage == 1) {
                FUN_L06_00303150((unsigned char *)c);
                d->stage = 2;
            }
        }
        break;
    case TRAM_STATE_WAIT_FOR_RESPAWN:
        if (tram_tick(&d->timer) != 0) {
            char *path = D_L14_001B0BB0_t[d->path];
            m->flags &= 0xFFBE;
            m->collision = m->pclass->collision;
            d->timer = tram_frames(D_L14_00161F8C);
            m->state = 2;
            d->segment = 0;
            d->along = 0;
            qcopy(&m->pos, path + 0x10);
            m->rot.z = tram_atan2(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                  *(float *)(path + 0x24) - *(float *)(path + 0x14));
            d->f80 = d->f84 = d->f88 = d->f70 = d->f74 = d->f78 = 0.0f;
        }
        break;
    }
    FUN_L14_002fd310(m);
    if (m->state != 0 && m->state != 3 && m->state != 1 && voice_is_playing(m, d->sound) == 0)
        d->sound = tram_sound(0, 4, m);
}
extern f32 D_L14_00161F60 __attribute__((sda));
extern f32 D_L14_00161F64 __attribute__((sda));
extern f32 D_L14_00161F68 __attribute__((sda));
extern f32 D_L14_00161F6C __attribute__((sda));
extern f32 D_L14_00161F70 __attribute__((sda));
extern s32 D_L14_00161F74 __attribute__((sda));
extern int FUN_L00_00257b90(int, int);

/* Exhaust of the second engine moby: puffs and rings on both sides plus a grey puff in the middle. */
void FUN_L14_002fd310(struct Moby *m) {
    Vec4 step;
    Vec4 pos;
    Vec4 base;
    L14Puff *puff;
    int i;
    int j;
    int spin;
    int life;
    int grey;
    float size;

    if (m->state == 0 || m->state == 3 || m->state == 1)
        return;
    exhaust_scale(&step, &m->rot_row0, D_L14_00161F60);
    exhaust_add(&pos, &m->pos, &step);
    exhaust_scale(&step, &m->rot_row2, D_L14_00161F68);
    exhaust_add(&pos, &pos, &step);
    qcopy(&base, &pos);
    exhaust_scale(&step, &m->rot_row1, D_L14_00161F64);
    exhaust_add(&pos, &pos, &step);
    for (j = 0; j < 2; j++) {
        spin = exhaust_rand(16);
        puff = exhaust_puff(&pos, 0.2f, 1.0f, 0.9f, exhaust_rand(2) == 0 ? spin : -spin, D_L14_0015F580, 200000.0f, D_L14_00161F74);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->life = exhaust_frames(12);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
    }
    size = 100000.0f;
    exhaust_scale(&step, &m->rot_row0, D_L14_00161F6C);
    exhaust_add(&pos, &pos, &step);
    spin = 16;
    life = exhaust_frames(2);
    for (i = 0; i < 3; i++) {
        puff = exhaust_puff(&pos, 0.05f, 1.0f, 1.0f, spin, D_L14_0015F580, size, 0x7FFFFFFF);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->life = life;
            puff->shade = exhaust_rand(0xFF);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
        spin = -spin;
        life *= 2;
        size -= 20000.0f;
    }
    size = 300000.0f;
    exhaust_scale(&step, &m->rot_row0, D_L14_00161F70);
    exhaust_add(&pos, &base, &step);
    grey = FUN_L00_00257b90(0x40, 0x80);
    grey = 0x7F000000 | grey << 16 | grey << 8 | grey;
    if (exhaust_rand(3) != 0) {
        L14Puff *p;
        spin = exhaust_rand(4);
        p = exhaust_puff(&pos, 0.05f, 1.0f, 1.0f, exhaust_rand(2) == 0 ? spin : -spin, D_L14_0015F580, size, grey);
        if (p != 0) {
            L14PuffTail *tail = &p->tail;
            p->life = exhaust_frames(60);
            if (exhaust_rand(2) != 0)
                p->alpha = 0x44;
            tail->tint = 0x40;
            tail->mode = 2;
            tail->fade = exhaust_frames(60);
        }
    }
    exhaust_scale(&step, &m->rot_row1, D_L14_00161F64);
    exhaust_sub(&pos, &base, &step);
    for (j = 0; j < 2; j++) {
        spin = exhaust_rand(16);
        puff = exhaust_puff(&pos, 0.2f, 1.0f, 0.9f, exhaust_rand(2) == 0 ? spin : -spin, D_L14_0015F580, 200000.0f, D_L14_00161F74);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->life = exhaust_frames(12);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
    }
    size = 100000.0f;
    exhaust_scale(&step, &m->rot_row0, D_L14_00161F6C);
    exhaust_add(&pos, &pos, &step);
    spin = 16;
    life = exhaust_frames(2);
    for (i = 0; i < 3; i++) {
        puff = exhaust_puff(&pos, 0.05f, 1.0f, 1.0f, spin, D_L14_0015F580, size, 0x7FFFFFFF);
        if (puff != 0) {
            L14PuffTail *tail = &puff->tail;
            puff->life = life;
            puff->shade = exhaust_rand(0xFF);
            tail->mode = 2;
            tail->tint = 0x7F;
            tail->fade = puff->life;
        }
        spin = -spin;
        life *= 2;
        size -= 20000.0f;
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002E0538.c: func_L14_002FEDA0), where it is exact; names translated to the US level program. */

extern u8 *D_L14_00167500 __attribute__((section(".data")));
extern char D_0013E550[];
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern float D_0015ED6C_f2 __asm__("D_0015ED6C");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float FUN_L00_0025b8c0(float *p, float *v, float t, float u1, float u2, float eps);
extern float vector_length_xy(void *);
extern int sample_path() __asm__("FUN_L00_0025d808");
extern float D_L14_00161F7C __attribute__((sda));
extern float D_L14_00161F80 __attribute__((sda));
extern float D_L14_00161F84 __attribute__((sda));
extern float D_L14_00161F88 __attribute__((sda));
extern float D_L14_00161F78 __attribute__((sda));
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void FUN_L13_00316f48(int);
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void subtract_vector_xyz(void *dst, void *a, void *b);
float absolute_float(float input) __asm__("func_001F99C0");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern int FUN_L00_0025d808_2f05d8(void *, void *, void *, void *, int,
                                    float) __asm__("FUN_L00_0025d808");
extern int *D_L14_001B0BB0[];

int FUN_L14_002fd918(struct Moby *m) {
    char *d = (char *)m->pvars;
    float v[4];
    float w[4];
    float yaw;
    float t;
    int r;
    r = FUN_L00_0025d808_2f05d8(D_L14_001B0BB0[*(int *)(d + 0x68)], v, d + 0x60, d + 0x64,
                                 (unsigned int)*(int *)(d + 0x8C) >> 31,
                                 D_L14_00161F78 * D_0015ED6C_f2);
    if (*(int *)(d + 0x8C) != 0 && (r != 0 || *(short *)(D_L14_00167500 + 0x86) != 0x13)) {
        FUN_L13_00316f48(*(int *)(d + 0x90));
        if (*(int *)(d + 0x6C) != -1) {
            char *e = D_0013E550 + *(int *)(d + 0x6C) * 0x70;
            if (*(char **)(e + 0x88) == m && ((unsigned char *)e)[0x74] != 0) {
                release_voice_slot(*(int *)(d + 0x6C));
            }
        }
        *(int *)(d + 0x6C) = -1;
        mark_moby_for_removal(m);
        return 1;
    }
    FUN_L00_0025b8c0(&m->pos.x, (float *)(d + 0x70), v[0], D_L14_00161F7C, D_L14_00161F80,
                     0.0f);
    FUN_L00_0025b8c0(&m->pos.y, (float *)(d + 0x74), v[1], D_L14_00161F7C, D_L14_00161F80,
                     0.0f);
    FUN_L00_0025b8c0(&m->pos.z, (float *)(d + 0x78), v[2], D_L14_00161F7C, D_L14_00161F80,
                     0.0f);
    subtract_vector_xyz(w, v, &m->pos);
    if (0.01f < absolute_float(w[0])) {
        if (0.01f < absolute_float(w[1])) {
            yaw = fast_arc_tan(w[0], w[1]);
            t = fast_subtract_rotations(m->rot.z, yaw);
            FUN_L00_00258278(m, (float *)(d + 0x88), yaw, D_L14_00161F7C, D_L14_00161F80, 0.0f);
            yaw = fast_arc_tan(vector_length_xy(w), w[2]);
            FUN_L00_0025b8c0(&m->rot.y, (float *)(d + 0x84), -yaw, D_L14_00161F7C,
                             D_L14_00161F80, 0.0f);
            t = t / D_L14_00161F88;
            if (1.0f < t)
                t = 1.0f;
            if (t < -1.0f)
                t = -1.0f;
            FUN_L00_0025b8c0(&m->rot.x, (float *)(d + 0x80), t * D_L14_00161F84, 0.003f,
                             D_L14_00161F80, 0.0f);
        }
    }
    return 0;
}
#include "rnc/overlay/quad.h"
/* pvars of the flyer rocket_update moves to a path point */
typedef struct {
    s32 point;   /* 0x0: path point index into D_L14_001600EC (0x80 each, position at 0x30) */
    s32 target;  /* 0x4: moby index removed on arrival */
    s32 sound;   /* 0x8: voice slot, -1 when none */
    s32 timer;   /* 0xC */
} FlyerVars;

extern s32 D_L14_00161FB0 __attribute__((sda));
extern f32 D_L14_00161FA8 __attribute__((sda));
extern f32 D_L14_00161FAC __attribute__((sda));
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float,
                             float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern void FUN_L14_002fded0(struct Moby *);
extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");

void rocket_update(struct Moby *moby) __asm__("FUN_L14_002fdbb8");
void rocket_update(struct Moby *moby) {
    enum {
        ROCKET_STATE_INITIALIZE = 0,
        ROCKET_STATE_IDLE = 1,
        ROCKET_STATE_FLY_TO_POINT = 2
    };
    FlyerVars *vars = (FlyerVars *)moby->pvars;
    Vec4f diff;
    OvlVec4 step;
    f32 dist;
    f32 speed;
    char *slot;
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
        vars->timer = scale_game_frames(D_L14_00161FB0);
        return;
    case ROCKET_STATE_IDLE:
        return;
    case ROCKET_STATE_FLY_TO_POINT:
        fast_vec_sub(&diff, D_L14_001600EC + vars->point * 0x80 + 0x30, &moby->pos);
        dist = fast_vec_length(&diff);
        fast_dec_timer(&vars->timer);
        speed = D_L14_00161FA8 * frame_time;
        if (dist < speed || dist <= 0.0f) {
            if (vars->sound != -1) {
                slot = D_0013E550 + vars->sound * 0x70;
                if (*(struct Moby **)(slot + 0x88) == moby && ((u8 *)slot)[0x74] != 0) {
                    release_voice_slot(vars->sound);
                }
            }
            target = (struct Moby *)(D_L14_0015FFD8 + vars->target * 0x100);
            vars->sound = -1;
            if (target != 0) {
                mark_moby_for_removal(target);
            }
            qcopy(&moby->pos, D_L14_001600EC + vars->point * 0x80 + 0x30);
            qcopy(&step, &moby->rot_row0);
            step.f[2] = 1.0f;
            spawn_explosion(moby, &step, &moby->pos, 0.0f, 0.0f, 0x14, 0xA, 0xC, 10.0f, 6.0f,
                             9.0f, 0, 1.0f, 30.0f, 1, 0xA, -1, 0);
            mark_moby_for_removal(moby);
            return;
        }
        moby->rot.x = fast_add_rotations(moby->rot.x, 2.0f * DEG_TO_RAD);
        moby->rot.y = FUN_L00_0025b750(moby->rot.y, -fast_arc_tan(vector_length_xy(&diff), diff.z), D_L14_00161FAC);
        moby->rot.z = FUN_L00_0025b750(moby->rot.z, fast_arc_tan(diff.x, diff.y), D_L14_00161FAC);
        fast_vec_scale(&step, &moby->rot_row0, speed);
        add_vector_xyz((u8 *)&moby->pos, (u8 *)&moby->pos, (u8 *)&step);
        FUN_L14_002fded0(moby);
        if (voice_is_playing(moby, vars->sound) == 0) {
            vars->sound = allocate_voice_for_target_entry(1, 4, (int)moby);
        }
        return;
    }
}

