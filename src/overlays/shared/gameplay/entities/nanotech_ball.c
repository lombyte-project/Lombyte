/* Shared moby classes: nanotech_ball, suckcannon, red_orb. */
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/overlay/quad.h"
typedef struct {
    u8 pad[0x20];
    u8 b20;
    u8 pad2[0x40 - 0x21];
    OvlQuad q40;
    u8 pad3[0x78 - 0x50];
    void *p78;
    u8 pad4[0xA6 - 0x7C];
    s16 hA6;
} O_002d7f88;
typedef struct {
    O_002d7f88 *o;
    f32 f4;
    u8 v8[4];
    f32 fC;
} D_002d7f88;
typedef struct {
    u8 pad[0x10];
    u8 v10[4];
    f32 f14;
    u8 pad2[0x23 - 0x18];
    u8 b23;
    u8 pad3[0x2C - 0x24];
    f32 f2C;
    u8 pad4[0x40 - 0x30];
    OvlQuad q40;
    u8 pad5[0x78 - 0x50];
    D_002d7f88 *p78;
} S_002d7f88;
extern f32 D_L00_001616DC;
s32 fast_dec_timer(void *) __asm__("FUN_001f9740");
void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
void clear_u64_value(void *) __asm__("FUN_001f99f8");
void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
void FUN_L00_002d7f88(S_002d7f88 *self) {
    D_002d7f88 *d = self->p78;
    O_002d7f88 *o;
    void *m;
    f32 t;
    d->f4 += (D_L00_001616DC + D_L00_001616DC) * frame_time;
    if (fast_dec_timer(d->v8) != 0 || D_L00_001616DC * 1.1f <= d->f4)
        goto kill;
    o = d->o;
    if (o == 0 || o->hA6 != 0xB9 || o->b20 == 0xFE || o->b20 == 0xFD) {
    kill:
        mark_moby_for_removal(self);
        return;
    }
    m = o->p78;
    clear_u64_value(self->v10);
    self->f14 = -d->f4;
    transform_vector_by_basis(self->v10, self->v10, (u8 *)d->o + 0xC0);
    fast_vec_add(self->v10, self->v10, m);
    qcopy(&self->q40, &d->o->q40);
    if (d->f4 > D_L00_001616DC) {
        self->b23 = truncate_float_to_s32((1.0f - (d->f4 - D_L00_001616DC) * 10.0f) * 127.0f);
    }
    t = d->fC;
    self->f2C = t * 0.1f + t * d->f4 / D_L00_001616DC;
}
/* True when the moby state byte is 1. */
int FUN_L00_002d8128(unsigned char *p) {
    return p[0x20] == 1;
}
typedef struct {
    char pad[0x10];
    OvlQuad q;
    signed char b20;
} P2d8138;
typedef struct {
    char pad[0xC];
    P2d8138 *p;
} T2d8138;
typedef struct {
    char pad[0x10];
    OvlQuad q;
    char b20;
    char pad2[0xF];
    char b30;
    char pad3[3];
    unsigned short h34;
    char pad4[0x42];
    T2d8138 *t78;
    char pad5[0x18];
    int f94;
} O2d8138;
O2d8138 *create_moby(int) __asm__("FUN_0020c4f8");
#include "qcopy.h"
/* Creates a class 0x326 moby linked to p at p's position; 0 when p's state is negative. */
O2d8138 *FUN_L00_002d8138(P2d8138 *p) {
    O2d8138 *o;
    if (p->b20 < 0)
        return 0;
    o = create_moby(0x326);
    if (o) {
        o->b30 = 0x40;
        o->b20 = 0;
        o->t78->p = p;
        o->f94 = 0;
        o->h34 |= 0x41;
        qcopy(&o->q, &p->q);
    }
    return o;
}
typedef unsigned int u128_2d81b8 __attribute__((mode(TI), aligned(16)));
typedef union {
    u128_2d81b8 q;
    float f[4];
} V_2d81b8;
typedef struct {
    char p0[0x10];
    float x, y, z;
} O_2d81b8;
typedef struct {
    char p0[0xA];
    short hA;
    char pC[4];
    float f10, f14, f18;
    char p1C[0x50 - 0x1C];
    float f50;
    char p54[0x70 - 0x54];
    float f70, f74, f78;
} D_2d81b8;
typedef struct {
    char p0[0x18];
    O_2d81b8 *o;
    char p1C[0x28 - 0x1C];
    float f28;
} G_2d81b8;
extern int D_L00_00173E40_lui[] __asm__("D_L00_00173E40") __attribute__((section(".data")));
extern float D_L00_00173E68_2d81b8 __asm__("D_L00_00173E68") __attribute__((section(".data")));
extern float frame_time_sq __asm__("D_0015ED70");
int FUN_001efa68_2d81b8(void *, void *, int, void *, int) __asm__("FUN_001efa68");
int FUN_L00_0025e3b8_2d81b8(void *) __asm__("FUN_L00_0025e3b8");
void FUN_L00_002d81b8(unsigned char *m) {
    V_2d81b8 a, b;
    D_2d81b8 *d = *(D_2d81b8 **)(m + 0x78);
    int hit = 0;
    float z, f;
    qcopy(&a, m + 0x10);
    a.f[2] += 0.5f;
    qcopy(&b, &a);
    b.f[2] -= 1.5f;
    if (FUN_001efa68_2d81b8(&a, &b, 2, m, 0)) {
        z = D_L00_00173E68_2d81b8;
        hit = 1;
    } else {
        z = b.f[2];
    }
    if (hit) {
        G_2d81b8 *g = &(*(G_2d81b8 *)D_L00_00173E40_lui);
        if (g->o) {
            if (FUN_L00_0025e3b8_2d81b8(g->o)) {
                *(float *)(m + 0x10) = d->f10 = g->o->x;
                *(float *)(m + 0x14) = d->f14 = g->o->y;
                {
                    float t = g->f28;
                    *(float *)(m + 0x18) = t;
                    d->f18 = t + 0.5f;
                }
            } else if (d->hA) {
                *(float *)(m + 0x10) = d->f10 = g->o->x + d->f70;
                *(float *)(m + 0x14) = d->f14 = g->o->y + d->f74;
                {
                    float t = g->o->z + d->f78;
                    *(float *)(m + 0x18) = t;
                    d->f18 = t + 0.5f;
                }
            } else {
                d->hA = 1;
                d->f70 = *(float *)(m + 0x10) - g->o->x;
                d->f74 = *(float *)(m + 0x14) - g->o->y;
                d->f78 = *(float *)(m + 0x18) - g->o->z;
            }
        }
    }
    f = *(float *)(m + 0x18);
    if (f <= z) {
        m[0xBC] = 0;
        if (d->hA)
            d->f78 += z - f;
        *(float *)(m + 0x18) = z;
        *(int *)&d->f50 = 0;
    } else {
        float c, acc;
        m[0xBC] = 1;
        acc = frame_time_sq * 9.8f;
        *(float *)(m + 0x18) = f - d->f50;
        d->f50 += acc;
        if (0.3f < d->f50)
            c = 0.3f;
        else
            c = d->f50;
        d->f50 = c;
        if (d->hA)
            d->f78 -= c;
    }
}
typedef struct {
    float x, y, z, w_002d8418;
} __attribute__((aligned(16))) Vec4_002d8418;
extern char g_debug_camera[] __asm__("D_L00_00166DC0");
void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
float FUN_002132a8_002d8418(float, float) __asm__("FUN_002132a8");
void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
float FUN_00213308_002d8418(void) __asm__("FUN_00213308");
void FUN_00214890_002d8418(Vec4_002d8418 *, Vec4_002d8418 *, Vec4_002d8418 *,
                           float) __asm__("FUN_00214890");
int FUN_00213260_002d8418(int) __asm__("FUN_00213260");
void FUN_L00_002730e0_002d8418(void *, Vec4_002d8418 *, int, int, int, int,
                               float) __asm__("FUN_L00_002730e0");
extern float D_L00_00161A60 __attribute__((sda));
extern float D_L00_00161A64 __attribute__((sda));
extern unsigned char D_L00_00161A68 __attribute__((sda));
extern int D_L00_00161A6C __attribute__((sda));
extern int D_L00_00161A70 __attribute__((sda));
extern float D_L00_00161A74 __attribute__((sda));
extern float D_L00_00161A78 __attribute__((sda));
void FUN_L00_002d8418(char *p) {
    Vec4_002d8418 d;
    Vec4_002d8418 r;
    Vec4_002d8418 q;
    char *pos = p + 0x10;
    fast_vec_sub(&d, g_debug_camera, pos);
    fast_vec_scale(&r, &hero.gravity_dir,
                          FUN_002132a8_002d8418(D_L00_00161A60 * frame_time,
                                                D_L00_00161A64 * frame_time));
    FUN_00214890_002d8418(&q, &r, &d, FUN_00213308_002d8418());
    FUN_L00_002730e0_002d8418(pos, &q, D_L00_00161A6C, D_L00_00161A68,
                              FUN_00213260_002d8418(0xFF) & 0xFF, 1, D_L00_00161A74);
    FUN_L00_002730e0_002d8418(pos, &q, D_L00_00161A70, D_L00_00161A68,
                              FUN_00213260_002d8418(0xFF) & 0xFF, 1, D_L00_00161A78);
}
typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) V_2d8510;
typedef struct {
    u8 p0[0x58];
    s16 h58[4];
    f32 f60[4];
} O_2d8510;
typedef struct {
    u8 p0[0x10];
    V_2d8510 v10;
    u8 p20[0x58];
    O_2d8510 *p78;
} M_2d8510;
typedef struct {
    u8 p0[0xF8];
    u8 *pF8;
} T_2d8510;
extern char g_debug_camera[] __asm__("D_L00_00166DC0") __attribute__((section(".data")));
extern T_2d8510 D_L00_001B2080_2d8510 __asm__("D_L00_001B2080") __attribute__((section(".data")));
extern s32 D_L00_00161A58_gp __asm__("D_L00_00161A58") __attribute__((sda));
void FUN_001f9a28_2d8510(void *, void *, void *) __asm__("FUN_001f9a28");
void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
s32 FUN_001f9770_2d8510(void *) __asm__("FUN_001f9770");
s32 FUN_001f96f8_2d8510(s32) __asm__("FUN_001f96f8");
f32 FUN_001fa6c0_2d8510(s32) __asm__("FUN_001fa6c0");
s32 FUN_001fa6d0_2d8510(f32) __asm__("FUN_001fa6d0");
f32 absolute_float(f32) __asm__("FUN_001f99c0");
s32 FUN_001fa6e0_2d8510(s32, s32, f32) __asm__("FUN_001fa6e0");
u8 *FUN_L00_00272f68_2d8510(void *, s32, u8, s32, s32, s32, s32, f32) __asm__("FUN_L00_00272f68");
extern s32 D_L00_00161A7C __attribute__((sda));
extern s32 D_L00_00161A80 __attribute__((sda));
extern s32 D_L00_00161A84 __attribute__((sda));
extern s32 D_L00_00161A88 __attribute__((sda));
extern f32 D_L00_00161A90[1] __attribute__((sda));
extern f32 D_L00_00161AA0[1] __attribute__((sda));
extern f32 D_L00_00161AB0[1] __attribute__((sda));
s32 FUN_L00_002d8510(M_2d8510 *m) {
    O_2d8510 *o;
    V_2d8510 a, b;
    s32 i, ret, c, hi, al;
    f32 z, t, f;
    u8 *p;
    o = m->p78;
    FUN_001f9a28_2d8510(&a, &(*(V_2d8510 *)g_debug_camera), &m->v10);
    ret = 0;
    fast_vec_normalize(&a, &a, -0.3f);
    fast_vec_normalize(&b, &a, 0.1f);
    fast_vec_add(&a, &a, &m->v10);
    z = a.z;
    for (i = 0; i < 4; i++) {
        if (FUN_001f9770_2d8510(&o->h58[i]))
            continue;
        if (o->h58[i] > FUN_001f96f8_2d8510(0x30))
            ret = 1;
        f = o->f60[i] += D_L00_00161A90[i];
        if (255.0f <= f)
            o->f60[i] = f - 255.0f;
        else if (f <= 0.0f)
            o->f60[i] = f + 255.0f;
        t = FUN_001fa6c0_2d8510(FUN_001f96f8_2d8510(0xFF) - o->h58[i]);
        t = t / (f32)FUN_001f96f8_2d8510(0xFF);
        c = FUN_001fa6e0_2d8510(D_L00_00161A7C, D_L00_00161A80,
                                absolute_float(0.5f - t));
        hi = c >> 24;
        t = 1.0f - o->h58[i] / FUN_001fa6c0_2d8510(FUN_001f96f8_2d8510(D_L00_00161A58_gp));
        al = FUN_001fa6d0_2d8510(FUN_001fa6c0_2d8510(hi) * (1.0f - t)) << 24;
        c = al | (c & 0xFFFFFF);
        a.z = z + D_L00_00161AB0[i];
        p = FUN_L00_00272f68_2d8510(&a, c, FUN_001fa6d0_2d8510(o->f60[i]), D_L00_00161A88, 0,
                                    2, 1, D_L00_00161AA0[i] * t);
        if (p)
            p[2] = *D_L00_001B2080_2d8510.pF8;
        p = FUN_L00_00272f68_2d8510(&a, al | 0xFFFFFF, FUN_001fa6d0_2d8510(o->f60[i]),
                                    D_L00_00161A88, D_L00_00161A84, 2, 1,
                                    D_L00_00161AA0[i] * (t * 0.7f));
        if (p)
            p[2] = *D_L00_001B2080_2d8510.pF8;
        fast_vec_add(&a, &a, &b);
    }
    return ret;
}
#include "eetypes.h"
typedef union {
    u128 q_002d8850;
    float f[4];
} Vec4_002d8850;
typedef struct {
    float f0_002d8850;
    char pad[0xc];
    u128 p10;
    Vec4_002d8850 p20;
    char *e[4];
} M_002d8850;
extern u128 D_L00_001E3CA0_002d8850[] __asm__("D_L00_001E3CA0");
void FUN_00214890_002d8850(void *, void *, void *, float) __asm__("FUN_00214890");
void FUN_001f9bf8_002d8850(void *, void *, float) __asm__("FUN_001f9bf8");
void FUN_001f9a10_002d8850(void *, void *, void *) __asm__("FUN_001f9a10");
void FUN_L00_002d8850(unsigned char *p, float *out, int i) {
    u128 t;
    M_002d8850 *m = *(M_002d8850 **)(p + 0x78);
    char *e;
    FUN_00214890_002d8850(&t, &D_L00_001E3CA0_002d8850[i], &m->p20, m->p20.f[3]);
    e = m->e[i];
    if (e && p[0x20] == 2)
        FUN_001f9bf8_002d8850(&t, &t, *(float *)(e + 0x28));
    FUN_001f9a10_002d8850(out, &m->p10, &t);
    out[2] += m->f0_002d8850;
}
typedef f32 V4[4] __attribute__((aligned(16)));
extern V4 D_L00_001E3CA0[];
extern V4 D_L00_001E3D20[];
extern f32 D_L00_001619F0 __attribute__((sda));
extern f32 D_L00_00161A14_gp __asm__("D_L00_00161A14") __attribute__((sda));
void build_look_at_matrix(V4 *, V4 *, V4 *, f32) __asm__("FUN_00214890");
void FUN_001f9bf8(V4 *, V4 *, f32);
/* Rebuilds the eight D_L00_001E3CA0 entries with build_look_at_matrix and normalizes them. */
void FUN_L00_002d8908(void) {
    s32 i;
    for (i = 0; i < 8; i++) {
        build_look_at_matrix(&D_L00_001E3CA0[i], &D_L00_001E3CA0[i], &D_L00_001E3D20[i],
                             D_L00_001619F0);
        FUN_001f9bf8(&D_L00_001E3CA0[i], &D_L00_001E3CA0[i], D_L00_00161A14_gp);
    }
}
#include "rnc/math/vector.h"
extern s32 D_001413D0_lui[] __asm__("D_001413D0") __attribute__((section(".data")));
#include "rnc/gameplay/entities/moby.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002D9EA0), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) V_2D9EA0;

typedef struct { float x, y, z; } V3_2D9EA0;

typedef struct Part_2D9EA0 {
    int f0;
    int f4;
    unsigned char b8;
    char p9[3];
    float fC;
    V_2D9EA0 pos;
} Part_2D9EA0;

typedef struct Ext_2D9EA0 {
    int f0;
    union {
        struct { struct Moby *owner; float scale; float ang; } s;
        V3_2D9EA0 a;
    } u;
    V3_2D9EA0 b;
    short h1C;
    short h1E;
} Ext_2D9EA0;

typedef struct D_2D9EA0 {
    float bob_offset;
    float bob_phase;
    short h8;
    short hA;
    struct Moby *owner;
    V_2D9EA0 pos;
    V_2D9EA0 v20;
    Part_2D9EA0 *slots[8];
    int f50;
    short h54;
    short h56;
    short h58[4];
    float f60[4];
    char p70[0xC];
    int f7C;
} D_2D9EA0;

extern Moby *D_L00_0015F63C;
extern char D_L00_00166DC0[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern int hero_max_health __asm__("D_0015EDA0");
extern unsigned char D_L00_001619F4 __attribute__((sda));
extern float D_L00_001619F8 __attribute__((sda));
extern float D_L00_001619FC __attribute__((sda));
extern int D_L00_00161A00 __attribute__((sda));
extern float D_L00_00161A0C __attribute__((sda));
extern float D_L00_00161A10_gp __asm__("D_L00_00161A10") __attribute__((sda));
extern float D_L00_00161A14_gp __asm__("D_L00_00161A14") __attribute__((sda));
extern int D_L00_00161A3C __attribute__((sda));
extern int D_L00_00161A40 __attribute__((sda));
extern int D_L00_00161A44 __attribute__((sda));
extern int D_L00_00161A48 __attribute__((sda));
extern float D_L00_00161A50 __attribute__((sda));
extern float D_L00_00161A54 __attribute__((sda));
extern int D_L00_00161A58_gp __asm__("D_L00_00161A58") __attribute__((sda));
extern float D_L00_00161A5C __attribute__((sda));
extern int fast_bsphere_check() __asm__("FUN_001fa728");
extern int free_effect_particle() __asm__("FUN_L00_00267a08");
extern s32 random_integer_below_c(s32) __asm__("func_00213260");
extern unsigned char D_0013F5E0[];
extern volatile unsigned char D_0015EDB6;
extern void add_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a10");
extern void build_look_at_matrix_c(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void enqueue_callback_list_1_alt(void (*)(void), int) __asm__("FUN_001f4600");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz_c(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a28");
extern void vu_euler_rotation_basis(void *, void *) __asm__("func_001FA030");
float FUN_L00_00257c48(float lo, float hi);
float cubic_hermite_scalar(float a, float b, float c, float d, float t) __asm__("FUN_L00_00257e20");
int FUN_L00_00257b90(int lo, int hi);
int trooper_play_sound(int i, int a1, int v, int k) __asm__("FUN_L00_0028dc90");
int FUN_L00_002d8980_c(char *a) __asm__("FUN_L00_002d8980");
s32 fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
s32 FUN_L00_002d8510_c(M_2d8510 *m) __asm__("FUN_L00_002d8510");
s32 scale_game_frames_alt_c(s32) __asm__("FUN_001f96f8");
unsigned char *FUN_L00_00273708(void *a, int b, int c);
void FUN_L00_002d81b8_c(unsigned char *m) __asm__("FUN_L00_002d81b8");
void FUN_L00_002d8418_c(char *p) __asm__("FUN_L00_002d8418");
void FUN_L00_002d8850_c(unsigned char *p, float *out, int i) __asm__("FUN_L00_002d8850");
void FUN_L00_002d8908_c(void) __asm__("FUN_L00_002d8908");
void FUN_L00_002d9810(Moby *moby);
extern V_2D9EA0 D_L00_001E4070_2D9EA0[] __asm__("D_L00_001E3D20");
extern float D_L00_001E17F0_2D9EA0[] __asm__("D_L00_001E14A0");
extern float func_002140F8_2D9EA0(float, float) __asm__("FUN_002132a8");
extern int func_002140B0_2D9EA0(int) __asm__("FUN_00213260");
extern int func_001F9850_2D9EA0(int) __asm__("FUN_001f96f8");
extern void func_001FA1F8_2D9EA0(void *, void *) __asm__("FUN_001fa030");
extern void func_L00_001FF4B0_2D9EA0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_001F9BD8_2D9EA0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_cross(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void func_001F9BF0_2D9EA0(void *, void *, void *) __asm__("FUN_001f9a28");
extern void func_00215C00_2D9EA0(void *, float, float, float) __asm__("FUN_00214db0");
extern float func_L00_00258C80_2D9EA0(float, float) __asm__("FUN_L00_00257c48");
extern float func_001F9FA8_2D9EA0(float) __asm__("FUN_001f9de0");
extern float func_001FA748_2D9EA0(float, float) __asm__("FUN_001fa580");
extern float func_001F9D10_2D9EA0(void *, void *) __asm__("FUN_001f9b48");
extern int func_L00_002D9E30_2D9EA0(void *) __asm__("FUN_L00_002d8980");
extern void func_001F9C30_2D9EA0(void *, void *, float) __asm__("FUN_001f9a68");
extern void func_002156E0_2D9EA0(void *, void *, float, void *) __asm__("FUN_00214890");
extern int func_L00_00258BC8_2D9EA0(int, int) __asm__("FUN_L00_00257b90");
extern void func_L00_002D9DB8_2D9EA0(void) __asm__("FUN_L00_002d8908");
extern int func_L00_00200290_2D9EA0(void *, float) __asm__("FUN_001fa728");
extern void func_001F49B0_2D9EA0(void (*)(void), int) __asm__("FUN_001f4600");
extern float func_001F9D48_2D9EA0(void *, void *) __asm__("FUN_001f9b80");
extern void func_L00_002D9D00_2D9EA0(void *, void *, int) __asm__("FUN_L00_002d8850");
extern Part_2D9EA0 *func_L00_002745A8_2D9EA0(void *, int, int) __asm__("FUN_L00_00273708");
extern void func_L00_002D98C8_2D9EA0(void *) __asm__("FUN_L00_002d8418");
extern int func_L00_002D99C0_2D9EA0(void *) __asm__("FUN_L00_002d8510");
extern void func_L00_002D9668_2D9EA0(void *) __asm__("FUN_L00_002d81b8");
extern int func_001F9938_2D9EA0(void *) __asm__("FUN_001f9770");
extern void func_L00_002688A8_2D9EA0(void *) __asm__("FUN_L00_00267a08");
extern void func_0020D678_2D9EA0(void *) __asm__("FUN_0020c828");
extern int func_L00_0028EF68_2D9EA0(int, int, void *, int) __asm__("FUN_L00_0028dc90");
extern float func_001FA888_2D9EA0(int) __asm__("FUN_001fa6c0");
extern float func_L00_00258E58_2D9EA0(float, float, float, float, float) __asm__("FUN_L00_00257e20");

void nanotech_ball_update(Moby *moby) __asm__("FUN_L00_002d89f0");
void nanotech_ball_update(Moby *moby) {
    enum {
        NANOTECH_BALL_STATE_INITIALIZE = 0,
        NANOTECH_BALL_STATE_FOLLOW_OWNER = 1,
        NANOTECH_BALL_STATE_DETACHED_PICKUP = 2,
        NANOTECH_BALL_STATE_COLLECTING = 3,
        NANOTECH_BALL_STATE_BASIS_ONLY = 4
    };
    V_2D9EA0 slot_rotation_basis_or_cull_position_or_start_tangent, rotated_slot_offset_or_particle_position, v20, collection_target_or_end_tangent, cross_product_or_particle_midpoint, half_displacement_or_spark_position, slot_curve_offset;
    D_2D9EA0 *data = (D_2D9EA0 *)moby->pvars;

    switch (moby->state) {
    case NANOTECH_BALL_STATE_INITIALIZE: {
        Moby *owner;
        int i;
        moby->state = 1;
        moby->flags |= 0x41;
        moby->collision = 0;
        moby->command = 0;
        owner = data->owner;
        qcopy(&moby->pos, &owner->pos);
        qcopy(&moby->rot, &owner->rot);
        qcopy(&data->pos, &moby->pos);
        *(int *)&data->bob_offset = 0;
        data->h8 = 0;
        data->pos.z += 0.5f;
        data->bob_phase = func_002140F8_2D9EA0(data->bob_offset, 3.1415927f);
        data->h56 = 0;
        data->hA = 0;
        for (i = 0; i < 4; i++) data->f60[i] = (float)func_002140B0_2D9EA0(0xFF);
        for (i = 0; i < 4; i++) data->h58[i] = func_001F9850_2D9EA0(D_L00_00161A58_gp);
        for (i = 7; i >= 0; i--) data->slots[i] = 0;
        if (D_L00_0015F63C == 0) {
            func_001FA1F8_2D9EA0(&moby->rot_row0, &moby->rot);
            D_L00_0015F63C = moby;
            moby->update_dist = 0xFF;
            data->f7C = 0;
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[0], &moby->rot_row0, D_L00_00161A14_gp);
            qcopy(&D_L00_001E4070_2D9EA0[0], &moby->rot_row2);
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[1], &moby->rot_row1, D_L00_00161A14_gp);
            qcopy(&D_L00_001E4070_2D9EA0[1], &moby->rot_row0);
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[2], &moby->rot_row2, D_L00_00161A14_gp);
            qcopy(&D_L00_001E4070_2D9EA0[2], &moby->rot_row1);
            func_001F9BD8_2D9EA0(&D_L00_001E3CA0[3], &moby->rot_row0, &moby->rot_row2);
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[3], &D_L00_001E3CA0[3], D_L00_00161A14_gp);
            fast_vec_cross(&D_L00_001E4070_2D9EA0[3], &D_L00_001E3CA0[3], &moby->rot_row1);
            func_001F9BD8_2D9EA0(&D_L00_001E3CA0[4], &moby->rot_row0, &moby->rot_row1);
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[4], &D_L00_001E3CA0[4], D_L00_00161A14_gp);
            fast_vec_cross(&D_L00_001E4070_2D9EA0[4], &D_L00_001E3CA0[4], &moby->rot_row2);
            func_001F9BF0_2D9EA0(&D_L00_001E3CA0[5], &moby->rot_row1, &moby->rot_row2);
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[5], &D_L00_001E3CA0[5], D_L00_00161A14_gp);
            fast_vec_cross(&D_L00_001E4070_2D9EA0[5], &D_L00_001E3CA0[5], &moby->rot_row0);
            func_001F9BF0_2D9EA0(&D_L00_001E3CA0[6], &moby->rot_row0, &moby->rot_row2);
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[6], &D_L00_001E3CA0[6], D_L00_00161A14_gp);
            fast_vec_cross(&D_L00_001E4070_2D9EA0[6], &D_L00_001E3CA0[6], &moby->rot_row1);
            func_001F9BF0_2D9EA0(&D_L00_001E3CA0[7], &moby->rot_row0, &moby->rot_row1);
            func_L00_001FF4B0_2D9EA0(&D_L00_001E3CA0[7], &D_L00_001E3CA0[7], D_L00_00161A14_gp);
            fast_vec_cross(&D_L00_001E4070_2D9EA0[7], &D_L00_001E3CA0[7], &moby->rot_row2);
        }
        {
            float yaw = func_002140F8_2D9EA0(0.0f, 6.2831855f);
            func_00215C00_2D9EA0(&data->v20, 1.0f, yaw, func_002140F8_2D9EA0(-1.5707964f, 1.5707964f));
        }
        data->v20.w = func_L00_00258C80_2D9EA0(0.785398006f, 1.5707964f);
    }
    case NANOTECH_BALL_STATE_FOLLOW_OWNER: {
        Moby *owner = data->owner;
        qcopy(&moby->pos, &owner->pos);
        qcopy(&data->pos, &owner->pos);
        data->pos.z += 0.5f;
        if ((signed char)owner->state < 0 || owner == moby) {
            moby->state = 2;
            data->f50 = 0;
        }
    }
    case NANOTECH_BALL_STATE_DETACHED_PICKUP:
        if (moby->state == 2) {
            int i;
            qcopy(&data->pos, &moby->pos);
            data->pos.z += 0.5f;
            func_L00_002D9668_2D9EA0(moby);
            for (i = 0; i < 8; i++) {
                if (data->slots[i] != 0) {
                    Ext_2D9EA0 *ext = (Ext_2D9EA0 *)(data->slots[i] + 1);
                    ext->u.s.scale = D_L00_00161A5C * func_001F9FA8_2D9EA0(ext->u.s.ang) + D_L00_00161A14_gp;
                    ext->u.s.ang = func_001FA748_2D9EA0(ext->u.s.ang, D_L00_001E17F0_2D9EA0[i]);
                }
            }
        }
        {
            Hero *player = &hero;
            int max_hp = hero_max_health;
            if (player->vehicle_mode == 1) max_hp = 4;
            if (moby->state != 1 && D_L00_0015F63C != 0 && ((D_2D9EA0 *)D_L00_0015F63C->pvars) != 0 && player->health.hp != 0
                && (player->health.hp + ((D_2D9EA0 *)D_L00_0015F63C->pvars)->f7C < max_hp || (D_0015EDB6 != 0 && player->invulnerable_frames == 0))
                && func_001F9D10_2D9EA0(&moby->pos, &player->motion.pos) < 10.0f
                && func_L00_002D9E30_2D9EA0(moby)) {
                int i;
                moby->state = 3;
                ((D_2D9EA0 *)D_L00_0015F63C->pvars)->f7C++;
                if (D_0015EDB6 != 0) player->invulnerable_frames = func_001F9850_2D9EA0(0x258);
                data->h54 = func_001F9850_2D9EA0(D_L00_00161A3C);
                func_001F9C30_2D9EA0(&collection_target_or_end_tangent, &player->gravity_dir, -D_L00_00161A54);
                func_001F9BD8_2D9EA0(&collection_target_or_end_tangent, &player->motion.pos, &collection_target_or_end_tangent);
                func_001F9C30_2D9EA0(&slot_rotation_basis_or_cull_position_or_start_tangent, &player->gravity_dir, -D_L00_00161A50);
                func_001F9BF0_2D9EA0(&v20, &collection_target_or_end_tangent, &moby->pos);
                v20.z -= 0.5f;
                fast_vec_cross(&cross_product_or_particle_midpoint, &v20, &slot_rotation_basis_or_cull_position_or_start_tangent);
                fast_vec_cross(&slot_rotation_basis_or_cull_position_or_start_tangent, &cross_product_or_particle_midpoint, &v20);
                func_002156E0_2D9EA0(&slot_rotation_basis_or_cull_position_or_start_tangent, &slot_rotation_basis_or_cull_position_or_start_tangent, -1.5707964f, &v20);
                func_L00_001FF4B0_2D9EA0(&slot_rotation_basis_or_cull_position_or_start_tangent, &slot_rotation_basis_or_cull_position_or_start_tangent, D_L00_00161A50);
                func_001F9C30_2D9EA0(&half_displacement_or_spark_position, &v20, 0.5f);
                for (i = 0; i < 8; i++) {
                    if (data->slots[i] != 0) {
                        Ext_2D9EA0 *ext = (Ext_2D9EA0 *)(data->slots[i] + 1);
                        func_002156E0_2D9EA0(&rotated_slot_offset_or_particle_position, &slot_rotation_basis_or_cull_position_or_start_tangent, (float)i * 0.448549628f, &v20);
                        func_001F9BD8_2D9EA0(&slot_curve_offset, &half_displacement_or_spark_position, &rotated_slot_offset_or_particle_position);
                        {
                            V3_2D9EA0 *dst = &ext->u.a;
                            dst->x = slot_curve_offset.x;
                            dst->y = slot_curve_offset.y;
                            dst->z = slot_curve_offset.z;
                        }
                        {
                            V3_2D9EA0 *dst = &ext->b;
                            V3_2D9EA0 *src = (V3_2D9EA0 *)&data->slots[i]->pos;
                            dst->x = src->x;
                            dst->y = src->y;
                            dst->z = src->z;
                        }
                        ext->h1E = ext->h1C = func_001F9850_2D9EA0(func_L00_00258BC8_2D9EA0(D_L00_00161A44, D_L00_00161A48));
                    }
                }
            }
        }
        data->bob_offset = D_L00_00161A10_gp * func_001F9FA8_2D9EA0(data->bob_phase);
        data->bob_phase = func_001FA748_2D9EA0(data->bob_phase, D_L00_00161A0C);
        if (D_L00_0015F63C == moby) func_L00_002D9DB8_2D9EA0();
        qcopy(&slot_rotation_basis_or_cull_position_or_start_tangent, &moby->pos);
        slot_rotation_basis_or_cull_position_or_start_tangent.z += 0.5f;
        slot_rotation_basis_or_cull_position_or_start_tangent.w = 1.0f;
        if (func_L00_00200290_2D9EA0(&slot_rotation_basis_or_cull_position_or_start_tangent, 64.0f) >= 0) {
            int i;
            int lifetime;
            func_001F49B0_2D9EA0((void (*)(void))FUN_L00_002d9810, (int)moby);
            if (10.0f < func_001F9D48_2D9EA0(&moby->pos, D_L00_00166DC0)) lifetime = func_001F9850_2D9EA0(5);
            else lifetime = func_001F9850_2D9EA0(40);
            for (i = 0; i < 8; i++) {
                Part_2D9EA0 *part;
                Part_2D9EA0 *slot;
                func_L00_002D9D00_2D9EA0(moby, &rotated_slot_offset_or_particle_position, i);
                part = func_L00_002745A8_2D9EA0(&rotated_slot_offset_or_particle_position, lifetime, 2);
                if (part != 0) {
                    Ext_2D9EA0 *ext = (Ext_2D9EA0 *)(part + 1);
                    ext->b.x = part->pos.x - moby->pos.x;
                    ext->b.y = part->pos.y - moby->pos.y;
                    ext->b.z = part->pos.z - moby->pos.z;
                    if (moby->state == 2) ext->u.s.owner = moby;
                    else ext->u.s.owner = data->owner;
                    part->f4 = D_L00_00161A00;
                    part->fC = D_L00_001619F8;
                }
                slot = data->slots[i];
                if (slot == 0) {
                    data->slots[i] = func_L00_002745A8_2D9EA0(&rotated_slot_offset_or_particle_position, 0, 1);
                    if (data->slots[i] != 0) {
                        Ext_2D9EA0 *ext = (Ext_2D9EA0 *)(data->slots[i] + 1);
                        ext->u.s.scale = D_L00_00161A14_gp;
                        ext->u.s.ang = 0.0f;
                        data->slots[i]->fC = D_L00_001619FC;
                        data->slots[i]->f4 = D_L00_00161A00;
                    }
                } else {
                    qcopy(&slot->pos, &rotated_slot_offset_or_particle_position);
                    slot->b8 += D_L00_001619F4;
                }
            }
        }
        break;
    case NANOTECH_BALL_STATE_COLLECTING:
        if (D_L00_0015F63C == moby) func_L00_002D9DB8_2D9EA0();
        if (data->h56 != 0) {
            func_001F9C30_2D9EA0(&moby->pos, D_0013F5E0, -D_L00_00161A54);
            func_001F9BD8_2D9EA0(&moby->pos, D_0013F5E0 - 0x210, &moby->pos);
            moby->rot.w = 0.52359879f;
            if (func_L00_002D99C0_2D9EA0(moby) != 0 && func_002140B0_2D9EA0(5) == 0) func_L00_002D98C8_2D9EA0(moby);
        }
        {
            Hero *player = &hero;
            int max_hp = hero_max_health;
            if (player->vehicle_mode == 1) max_hp = 4;
            if (player->health.hp == max_hp && player->invulnerable_frames != 0 && D_0015EDB6 != 0) {
                int duration = func_001F9850_2D9EA0(D_L00_00161A40);
                if (data->h54 < duration) {
                    int i;
                    data->h54 = duration;
                    for (i = 0; i < 4; i++) data->h58[i]++;
                }
            }
        }
        if (func_001F9938_2D9EA0(&data->h54)) {
            int i;
            for (i = 0; i < 8; i++) {
                if (data->slots[i] != 0) {
                    func_L00_002688A8_2D9EA0(data->slots[i]);
                    data->slots[i] = 0;
                }
            }
            if (D_L00_0015F63C == moby) moby->state = 4;
            else func_0020D678_2D9EA0(moby);
        } else {
            int i;
            func_001F9C30_2D9EA0(&v20, D_0013F5E0, -D_L00_00161A54);
            func_001F9BD8_2D9EA0(&v20, D_0013F5E0 - 0x210, &v20);
            for (i = 0; i < 8; i++) {
                Part_2D9EA0 *part = data->slots[i];
                Ext_2D9EA0 *ext;
                if (part == 0) continue;
                ext = (Ext_2D9EA0 *)(part + 1);
                if (func_001F9938_2D9EA0(&ext->h1E)) {
                    if (data->h56 == 0) {
                        Hero *player = &hero;
                        int max_hp = hero_max_health;
                        if (player->vehicle_mode == 1) max_hp = 4;
                        if (player->health.hp != 0) {
                            int new_hp = player->health.hp + 1;
                            player->health.hp = (max_hp < new_hp) ? max_hp : new_hp;
                        }
                        {
                            D_2D9EA0 *leader_data = (D_2D9EA0 *)D_L00_0015F63C->pvars;
                            leader_data->f7C--;
                            if (leader_data->f7C < 0) leader_data->f7C = 0;
                        }
                        func_L00_0028EF68_2D9EA0(1, 0, moby, 0x1F5);
                        data->h56 = 1;
                    }
                    func_L00_002688A8_2D9EA0(part);
                    data->slots[i] = 0;
                } else {
                    float progress;
                    float remaining;
                    Part_2D9EA0 *spark;
                    qcopy(&cross_product_or_particle_midpoint, &part->pos);
                    remaining = func_001FA888_2D9EA0(ext->h1C);
                    remaining = (float)ext->h1E / remaining;
                    {
                        V3_2D9EA0 *src = &ext->b;
                        rotated_slot_offset_or_particle_position.x = src->x;
                        rotated_slot_offset_or_particle_position.y = src->y;
                        rotated_slot_offset_or_particle_position.z = src->z;
                    }
                    rotated_slot_offset_or_particle_position.w = 0.0f;
                    progress = 1.0f - remaining;
                    slot_rotation_basis_or_cull_position_or_start_tangent.x = rotated_slot_offset_or_particle_position.x - ext->u.a.x;
                    slot_rotation_basis_or_cull_position_or_start_tangent.y = rotated_slot_offset_or_particle_position.y - ext->u.a.y;
                    slot_rotation_basis_or_cull_position_or_start_tangent.z = rotated_slot_offset_or_particle_position.z - ext->u.a.z;
                    collection_target_or_end_tangent.x = v20.x - ext->u.a.x;
                    collection_target_or_end_tangent.y = v20.y - ext->u.a.y;
                    collection_target_or_end_tangent.z = v20.z - ext->u.a.z;
                    part->pos.x = func_L00_00258E58_2D9EA0(slot_rotation_basis_or_cull_position_or_start_tangent.x, rotated_slot_offset_or_particle_position.x, v20.x, collection_target_or_end_tangent.x, progress);
                    part->pos.y = func_L00_00258E58_2D9EA0(slot_rotation_basis_or_cull_position_or_start_tangent.y, rotated_slot_offset_or_particle_position.y, v20.y, collection_target_or_end_tangent.y, progress);
                    part->pos.z = func_L00_00258E58_2D9EA0(slot_rotation_basis_or_cull_position_or_start_tangent.z, rotated_slot_offset_or_particle_position.z, v20.z, collection_target_or_end_tangent.z, progress);
                    half_displacement_or_spark_position.x = func_L00_00258C80_2D9EA0(0.0f, 0.00999999978f) + part->pos.x;
                    half_displacement_or_spark_position.y = func_L00_00258C80_2D9EA0(0.0f, 0.00999999978f) + part->pos.y;
                    half_displacement_or_spark_position.z = func_L00_00258C80_2D9EA0(0.0f, 0.00999999978f) + part->pos.z;
                    spark = func_L00_002745A8_2D9EA0(&half_displacement_or_spark_position, func_001F9850_2D9EA0(0x14), 0);
                    if (spark != 0) {
                        spark->f4 = D_L00_00161A00;
                        spark->fC = D_L00_001619F8;
                    }
                    func_001F9BD8_2D9EA0(&cross_product_or_particle_midpoint, &part->pos, &cross_product_or_particle_midpoint);
                    func_001F9C30_2D9EA0(&cross_product_or_particle_midpoint, &cross_product_or_particle_midpoint, 0.5f);
                    half_displacement_or_spark_position.x = func_L00_00258C80_2D9EA0(0.0f, 0.00999999978f) + cross_product_or_particle_midpoint.x;
                    half_displacement_or_spark_position.y = func_L00_00258C80_2D9EA0(0.0f, 0.00999999978f) + cross_product_or_particle_midpoint.y;
                    half_displacement_or_spark_position.z = func_L00_00258C80_2D9EA0(0.0f, 0.00999999978f) + cross_product_or_particle_midpoint.z;
                    spark = func_L00_002745A8_2D9EA0(&half_displacement_or_spark_position, func_001F9850_2D9EA0(0x14), 0);
                    if (spark != 0) {
                        spark->f4 = D_L00_00161A00;
                        spark->fC = D_L00_001619F8;
                    }
                }
            }
        }
        break;
    case NANOTECH_BALL_STATE_BASIS_ONLY:
        if (D_L00_0015F63C == moby) func_L00_002D9DB8_2D9EA0();
        break;
    }
}
/* Ported from rac1-decomp (nonmatching/shared/func_L00_002DACC0.c: func_L00_002DACC0), a near miss there; names translated to the US level program. */
typedef struct {
    u128 points[4];
    u32 colors[4];
    float texcoords[8];
    u64 texture[4];
} __attribute__((aligned(16))) GeometryQuad;
extern int D_L00_00161B10_macro __asm__("D_L00_00161B10") __attribute__((section(".sdata")));
extern int D_L00_00161AC0_gp __asm__("D_L00_00161AC0") __attribute__((sda));
extern int D_L00_00161AC4_gp __asm__("D_L00_00161AC4") __attribute__((sda));
extern int D_L00_00161AC8_gp __asm__("D_L00_00161AC8") __attribute__((sda));
extern int D_L00_00161ACC_gp __asm__("D_L00_00161ACC") __attribute__((sda));
extern int D_L00_00161AD0_gp __asm__("D_L00_00161AD0") __attribute__((sda));
extern int D_L00_00161AD4_gp __asm__("D_L00_00161AD4") __attribute__((sda));
extern u32 D_L00_00161AD8_gp __asm__("D_L00_00161AD8") __attribute__((sda));
extern float D_L00_00161ADC_gp __asm__("D_L00_00161ADC") __attribute__((sda));
extern u32 D_L00_00161AE0_gp __asm__("D_L00_00161AE0") __attribute__((sda));
extern int D_L00_00161AE4_gp __asm__("D_L00_00161AE4") __attribute__((sda));
extern float D_L00_00161AE8_gp __asm__("D_L00_00161AE8") __attribute__((sda));
extern float D_L00_00161AEC_gp __asm__("D_L00_00161AEC") __attribute__((sda));
extern char D_L00_00161AF0_macro[] __asm__("D_L00_00161AF0") __attribute__((section(".sdata")));
extern char D_L00_001E14C0[];
extern char D_L00_001E26E0[];
extern char D_L00_001E2FF0[];
extern char D_L00_001E3480[];
extern char D_L00_001E3A80[];
extern char D_L00_001E3B80[];
extern char D_L00_001E3C80[];
extern char D_L00_001E3DA0[];
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
void font_queue_vu_state(void) __asm__("FUN_001f76a0");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
void FUN_L00_001fde98(int, void *, void *, void *, int);
s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern void set_vector_length_xy(void *, void *, f32) __asm__("FUN_001f9c48");
float FUN_L00_00200260(float, float);

void FUN_L00_002d9810(Moby *moby) {
    GeometryQuad quad;
    float basis[4] __attribute__((aligned(16)));
    float *forward;
    float side[4] __attribute__((aligned(16)));
    float up[4] __attribute__((aligned(16)));
    float center[4] __attribute__((aligned(16)));
    float view_dir[4] __attribute__((aligned(16)));
    float to_camera[4] __attribute__((aligned(16)));
    float vertex[4] __attribute__((aligned(16)));
    D_2D9EA0 *vars = (D_2D9EA0 *)moby->pvars;
    Moby *owner;
    float pulse = vars->bob_offset / D_L00_00161A10_gp;
    int i, j, color;
    short *indices;
    short *row;
    short *rows;
    char *corner;
    char *outer, *inner;
    float *u, *v;
    float *v_base, *u_base;

    quad.texture[1] = get_effect_texture(D_L00_00161AD4_gp);
    quad.texture[2] = 0xff9000000260ULL;
    quad.texture[3] = (u64)D_L00_00161AC0_gp |
                      ((u64)D_L00_00161AC4_gp << 2) |
                      ((u64)D_L00_00161AC8_gp << 4) |
                      ((u64)D_L00_00161ACC_gp << 6) |
                      ((u64)D_L00_00161AD0_gp << 32);
    quad.texture[0] = 0;
    vu1_add_g_sregister(0x47, 0x53001);
    forward = basis;
    center[0] = moby->pos.x;
    center[1] = moby->pos.y;
    center[2] = moby->pos.z + 0.5f;
    center[2] += vars->bob_offset;
    center[3] = 1.0f;
    fast_vec_add(&center, &center, D_L00_00161AF0_macro);
    fast_vec_sub(&to_camera, g_debug_camera, &center);
    fast_vec_normalize(forward, &to_camera, 1.0f);
    owner = vars->owner;
    fast_vec_cross(&side, forward, &owner->rot_row2);
    fast_vec_normalize(&side, &side, 1.0f);
    fast_vec_cross(&up, &side, forward);
    quad.colors[3] = D_L00_00161AD8_gp;
    quad.colors[2] = D_L00_00161AD8_gp;
    quad.colors[1] = D_L00_00161AD8_gp;
    quad.colors[0] = D_L00_00161AD8_gp;
    vu1_add_g_sregister(6, get_effect_texture(D_L00_00161AD4_gp));
    vu1_add_g_sregister(0x42, 0x8000000044ULL);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xff9000000260ULL);
    vu1_add_g_sregister(0x4a, 0);
    font_queue_vu_state();
    for (j = 0; j < 0x122; j++) {
        transform_vector(&vertex, D_L00_001E14C0 + j * 16, basis);
        {
            float x = vertex[0], y = vertex[1], z = vertex[2];
            ((Vec3 *)D_L00_001E3DA0)[j].x = x;
            ((Vec3 *)D_L00_001E3DA0)[j].y = y;
            ((Vec3 *)D_L00_001E3DA0)[j].z = z;
        }
    }
    FUN_L00_001fde98(0x94, D_L00_001E3DA0,
                                D_L00_001E2FF0, D_L00_001E26E0, 1);
    FUN_L00_001fde98(0x6d, D_L00_001E3DA0 + 0x6d8,
                                D_L00_001E2FF0 + 0x248,
                                D_L00_001E26E0 + 0x490, 1);
    FUN_L00_001fde98(0x24, D_L00_001E3DA0 + 0xbe8,
                                D_L00_001E2FF0 + 0x3f8,
                                D_L00_001E26E0 + 0x7f0, 1);
    quad.texture[1] = get_effect_texture(0xb);
    quad.texture[3] = 0x8000000048ULL;
    color = truncate_float_to_s32(pulse * 64.0f + 128.0f);
    color = ((u32)(color + D_L00_00161B10_macro) << 24) | D_L00_00161AE0_gp;
    quad.colors[3] = 0;
    quad.colors[2] = 0;
    quad.colors[1] = color;
    quad.colors[0] = color;
    for (i = 0; i < 32; i++) {
        indices = (short *)(D_L00_001E3A80 + i * 8);
        j = 0;
        inner = (char *)quad.points;
        outer = (char *)quad.points;
        for (; j < 4; j++) {
            short index = *indices;
            if (j >= 2)
                fast_vec_scale(outer, D_L00_001E3480 + index * 16,
                             D_L00_00161ADC_gp * D_L00_00161AEC_gp);
            else
                fast_vec_scale(inner, D_L00_001E3480 + index * 16, D_L00_00161ADC_gp);
            inner += 16;
            outer += 16;
            indices++;
        }
        quad.texcoords[0] = 0.5f;
        quad.texcoords[1] = 0.5f;
        quad.texcoords[2] = 0.5f;
        quad.texcoords[3] = 0.5f;
        quad.texcoords[4] = 0.5f;
        quad.texcoords[5] = 1.0f;
        quad.texcoords[6] = 0.5f;
        quad.texcoords[7] = 1.0f;
        draw_geometry_quad(&quad, basis, 0);
    }
    if (FUN_L00_002d8128((u8 *)moby)) {
        quad.texture[1] = get_effect_texture(D_L00_00161AE4_gp);
        quad.colors[3] = 0x107f7f7f;
        quad.colors[2] = 0x107f7f7f;
        quad.colors[1] = 0x107f7f7f;
        quad.colors[0] = 0x107f7f7f;
        quad.texture[3] = 0x8000000044ULL;
        {
            Moby *owner = vars->owner;
            qcopy(basis, &owner->rot_row0);
            qcopy(&side, &owner->rot_row1);
            qcopy(&up, &owner->rot_row2);
        }
        center[2] = moby->pos.z;
        v_base = &quad.texcoords[0];
        u_base = &quad.texcoords[1];
        rows = (short *)D_L00_001E3C80;
        for (i = 0; i < 4; i++) {
            u = u_base;
            v = v_base;
            row = (short *)(i * 8 + (u32)rows);
            corner = (char *)quad.points;
            for (j = 3; j >= 0; j--) {
                transform_vector(corner, D_L00_001E3B80 + (*row++) * 16,
                                       basis);
                fast_vec_sub(view_dir, corner, g_debug_camera);
                corner += 16;
                view_dir[1] = view_dir[2];
                set_vector_length_xy(view_dir, view_dir, D_L00_00161AE8_gp);
                *v = view_dir[0] * 0.5f + 0.5f;
                *v = FUN_L00_00200260(*v, 8.0f);
                v += 2;
                *u = view_dir[1] * 0.5f + 0.5f;
                *u = FUN_L00_00200260(*u, 8.0f);
                u += 2;
            }
            draw_geometry_quad(&quad, 0, 0);
        }
    }
    vu1_add_g_sregister(0x47, 0x5360b);
}
int collision_line(Vec4 *, Vec4 *, int, int, int) __asm__("FUN_001efa68");
/* True when coll_line_fix returns 0 for the moby and (*(char * *)&D_001413D0_lui) positions, both raised by 0.5. */
int FUN_L00_002d8980(char *a) {
    Vec4 p, t;
    qcopy(&p, a + 0x10);
    p.f[2] += 0.5f;
    qcopy(&t, (*(char * *)&D_001413D0_lui) + 0x10);
    t.f[2] += 0.5f;
    return collision_line(&p, &t, 0x12, 0, 0) == 0;
}
/* False in modes 0, 1, 5, 12, and in modes 2 and 4 unless the state is 0x3C. */
int FUN_L00_002d9f78(void) {
    int r = 1;
    switch (hero.state.control_mode) {
    case 0:
    case 1:
    case 5:
    case 12:
        r = 0;
        break;
    case 2:
    case 4:
        if (hero.state.current != 0x3C)
            r = 0;
        break;
    }
    return r;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DB480), where it is exact; names translated to the US level program. */

typedef struct {
    int a[16];
} V __attribute__((aligned(16)));

extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void copy_matrix_rotation(void *, void *) __asm__("FUN_001fa298");
extern void copy_matrix3x4(void *, void *) __asm__("FUN_001fa2b8");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("FUN_0020e098");

/* Composes the 0xC0 matrix of a with the one built from c into b+0xC0 and flags b for update. */
void FUN_L00_002d9fd0(char *a, char *b, void *c) {
    V euler_rotation, source_rotation, composed_rotation;
    euler_rotation_matrix(&euler_rotation, c);
    copy_matrix_rotation(&source_rotation, a + 0xC0);
    multiply_matrix(&composed_rotation, &source_rotation, &euler_rotation);
    copy_matrix3x4(b + 0xC0, &composed_rotation);
    *(unsigned short *)(b + 0x34) |= 4;
    refresh_moby_spatial_bounds_from_basis(b);
}

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DB508), where it is exact; names translated to the US level program. */

typedef struct {
    u8 pad0[0x2078];
    s32 i2078;
    s32 i207C;
} G_2db508;

extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern unsigned char D_0013E529 __attribute__((section(".data")));
extern char *D_001413A0[];
extern G_2db508 D_0013F350;
extern G_2db508 D_0013F350_a __asm__("D_0013F350");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern void blend_moby_animation_alt(void *, int, int, int) __asm__("FUN_00212f90");

int FUN_L00_002da058(u8 *p, void *q, void *m_, void *r_, void *buf) {
    char *m = (char *)m_;
    char *r = (char *)r_;
    int ret = 0;
    int (*fn)(void *, void *, void *) =
        **(int (***)(void *, void *, void *))(*(char **)(m + 0x24) + 0x2C);
    char *x;
    int c;
    int n;
    int i;
    if (fn != 0) {
        ret = fn(m, buf, p);
        if (ret == 2 && *(short *)(r + 0x68) != 3) {

            n = D_0013F350_a.i2078 + D_0013F350_a.i207C;
            if (D_0013E529 == 0 ? n < 5 : n < 10) {
                x = *(char **)(r + 0x70);
                if (((unsigned char *)m)[0x53] != (unsigned char)x[3]) {
                    blend_moby_animation_alt(m, (unsigned char)x[3], 0, scale_game_frames_alt(10));
                }
                *(short *)(r + 0x68) = 3;
                *(int *)(r + 0x6C) = 0;
                *(float *)(m + 0x58) = 1.0f;
                *(float *)(r + 0x64) = vector_distance_xy(m + 0x10, buf);
                for (i = 0; i < (D_0013E529 == 0 ? 5 : 10); i++) {
                    G_2db508 *gg = &D_0013F350;
                    if (D_001413A0[i] == 0) {
                        D_001413A0[i] = m;
                        *(short *)(r + 0x6A) = i;
                        gg->i207C++;
                        break;
                    }
                }
            }
        }
    }
    return ret;
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DB690), where it is exact; names translated to the US level program. */

/* Calls the optional function reached through the moby pointer at +0x24 (slot 0xC); 1 if null. */
int FUN_L00_002da1e0(char *a) {
    int (*fn)(char *) = *(int (**)(char *))(*(char **)(*(char **)(a + 0x24) + 0x2C) + 0xC);
    if (fn != 0) {
        return fn(a);
    }
    return 1;
}
extern struct Hero hero_data __asm__("D_0013F350") __attribute__((section(".data")));
typedef struct {
    char p0[0x154];
    f32 f154;
    f32 f158;
    char p1[0x350 - 0x15C];
    char m350[0x40];
} C_002da250;
extern char D_L00_00166C80[] __attribute__((section(".data")));
typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) V_002da250;
void FUN_L00_0024f7c8_002da250(void *, s32, f32 *) __asm__("FUN_L00_0024f7c8");
void FUN_001f9cf8_002da250(V_002da250 *, V_002da250 *, void *) __asm__("FUN_001f9cf8");
void FUN_L00_002da250(u8 *a, f32 *b, f32 *c, f32 *d) {
    FUN_L00_0024f7c8_002da250(a, 0, d);
    if (hero_data.state.current == 1 || hero_data.state.current == 0x1E) {
        V_002da250 v;
        C_002da250 *cam = &(*(C_002da250 *)D_L00_00166C80);
        v.y = 0;
        v.x = 1.2f;
        *b = cam->f158;
        v.z = 0;
        *c = -cam->f154;
        FUN_001f9cf8_002da250(&v, &v, cam->m350);
        d[2] = v.z + *(f32 *)(a + 0x18);
        d[0] = v.x + hero_data.motion.pos.f[0];
        d[1] = v.y + hero_data.motion.pos.f[1];
    } else if (hero_data.unk22C8 == 1) {
        *b = hero_data.unk688;
        *c = *(f32 *)(a + 0x44);
    } else {
        *b = hero_data.unk688;
        *c = hero_data.unk684;
    }
}
typedef struct {
    f32 x, y, z, w;
} V_da360;
typedef struct M_da360 {
    u8 p0[0x10];
    V_da360 pos;
    u8 p20[4];
    u8 *p24;
    struct M_da360 *next;
    u8 p2c[4];
    u8 p30[4];
    u16 f34;
    u8 p36[0xa6 - 0x36];
    s16 a6;
} M_da360;
typedef struct {
    u8 p0[0x68];
    s16 h68;
    s16 h6a;
} R;
extern M_da360 *D_L00_0015FFE4;
typedef struct {
    u8 p[9];
    u8 b9;
} G1T;
typedef struct {
    u8 p[0x2078];
    s32 i2078;
} G2T;
extern G1T D_0013E520;
extern G2T D_0013F350_c __asm__("D_0013F350");
#define G1 (&D_0013E520)
#define G2 (&D_0013F350_c)
extern void FUN_L00_002db848(void);
extern void FUN_L00_002da250_c(void *, f32 *, f32 *, void *) __asm__("FUN_L00_002da250");
extern void FUN_L00_002ddf60(void *, void *);
extern R *FUN_L00_002db890_c(M_da360 *) __asm__("FUN_L00_002db890");
extern f32 vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern s32 FUN_L00_002d9f78_c(void) __asm__("FUN_L00_002d9f78");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern s32 FUN_001efa68_c(void *, V_da360 *, s32, M_da360 *, s32) __asm__("FUN_001efa68");
int FUN_L00_002da058(u8 *p, void *q, void *m_, void *r_, void *buf);
extern void FUN_L00_002da1e0_c(M_da360 *) __asm__("FUN_L00_002da1e0");
extern void FUN_L00_002df1e0_c(M_da360 *) __asm__("FUN_L00_002df1e0");
void FUN_L00_002da360(u8 *p, void *q) {
    u8 buf[0x10] __attribute__((aligned(16)));
    V_da360 v;
    f32 a, b;
    M_da360 *m;
    R *r;
    f32 d, thr, t;
    s32 n;

    FUN_L00_002da250_c(p, &a, &b, buf);
    FUN_L00_002ddf60(p, buf);
    for (m = D_L00_0015FFE4; m != 0; m = m->next) {
        if (m->a6 != 0 && (m->f34 & 0x1000) &&
            *(void **)*(u8 **)(m->p24 + 0x2c) != (void *)FUN_L00_002db848) {
            r = FUN_L00_002db890_c(m);
            if (r != 0 && r->h68 < 6) {
                d = vector_distance_xy(p + 0x10, &m->pos);
                if (d < 15.0f && !FUN_L00_002d9f78_c() &&
                    ((n = G2->i2078), G1->b9 == 0 ? n < 5 : n < 10)) {
                    if (r->h68 < 3) {
                        thr = 0.043864909559488297f;
                        if (d < 3.0f)
                            thr = 3.68587064743042f;
                        r->h6a = -1;
                        qcopy(&v, &m->pos);
                        v.z += 0.4f;
                        t = fast_difference_between_rotations(
                            a, fast_arc_tan(v.x - *(f32 *)(p + 0x10), v.y - *(f32 *)(p + 0x14)));
                        if (t * t < thr) {
                            t = fast_arc_tan(d, v.z - *(f32 *)(p + 0x18));
                            t = fast_difference_between_rotations(b, t);
                            if (t * t < thr) {
                                if (!FUN_001efa68_c(buf, &v, 2, m, 0))
                                    goto hit;
                                if (r->h68 > 0)
                                    FUN_L00_002da1e0_c(m);
                            } else if (r->h68 > 0)
                                FUN_L00_002da1e0_c(m);
                        } else if (r->h68 > 0)
                            FUN_L00_002da1e0_c(m);
                    } else if (r->h68 < 4) {
                    hit:
                        FUN_L00_002da058(p, q, m, r, buf);
                    }
                } else if (r->h68 != 0)
                    FUN_L00_002da1e0_c(m);
            }
        } else {
            FUN_L00_002df1e0_c(m);
        }
    }
}
typedef struct {
    u8 pad0[0x2050];
    u8 *slot[10];
    s32 i2078;
    s32 i207C;
} G_002da660;
extern G_002da660 D_0013F350_002da660 __asm__("D_0013F350") __attribute__((section(".data")));
typedef struct {
    u8 p[9];
    u8 b9;
} H_002da660;
extern H_002da660 item_text_variant __asm__("D_0013E520") __attribute__((section(".data")));
u8 *FUN_L00_002db890_002da660(u8 *) __asm__("FUN_L00_002db890");
void FUN_001e93b0_002da660(void *, s32, s32) __asm__("FUN_001e93b0");
extern char D_L00_001E9FE8[] __attribute__((section(".data")));
void FUN_L00_002da660(void) {
    s32 i;
    D_0013F350_002da660.i207C = 0;
    D_0013F350_002da660.i2078 = 0;
    for (i = 0; i < (item_text_variant.b9 == 0 ? 5 : 10); i++) {
        u8 *o;
        s32 k;
        if (D_0013F350_002da660.slot[i] == 0)
            continue;
        o = FUN_L00_002db890_002da660(D_0013F350_002da660.slot[i]);
        if (o == 0 || (k = *(s16 *)(o + 0x68)) < 3 || k >= 5 || *(s16 *)(o + 0x6A) == -1) {
            u8 *m = D_0013F350_002da660.slot[i];
            FUN_001e93b0_002da660(D_L00_001E9FE8, *(s16 *)(m + 0xB2), *(s16 *)(m + 0xA6));
            D_0013F350_002da660.slot[i] = 0;
        } else if (k == 3) {
            D_0013F350_002da660.i207C++;
        } else if (k == 4) {
            D_0013F350_002da660.i2078++;
        }
    }
}
/* Update the Suck Cannon: suction, aiming and firing, the stored mobys,
   target selection, usage statistics and cancelling the release input. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DBC40), where it is exact; names translated to the US level program. */
typedef unsigned int SuckQuad __attribute__((mode(TI)));

typedef union { struct { float x,y,z,w; } f; SuckQuad q; } SuckVec __attribute__((aligned(16)));

typedef struct { SuckVec row[3]; } SuckMatrix;

typedef struct { SuckVec row[4]; } SuckMatrix4;

typedef union {
    struct { SuckVec velocity,end; SuckMatrix rotation; SuckMatrix4 inverse; SuckVec point,offset; } aim;
    struct { SuckVec velocity; SuckMatrix rotation; SuckMatrix4 inverse; SuckVec launch,point,offset; } fire;
} SuckWork;

typedef struct SuckMoby SuckMoby;

typedef struct { u8 pad[8]; void (*fire)(SuckMoby *, SuckVec *, SuckMoby *, float); } SuckHandlers;

typedef struct SuckClass {
    u8 pad0[0x2C];
    SuckHandlers * handlers;
    u8 pad30[0x16];
    s16 category;
} SuckClass;

typedef struct SuckCannon {
    s32 unused;
    s32 sound;
    s32 cooldown;
    s32 blocked;
    SuckMoby * target;
    s32 target_timer;
    float target_height;
} SuckCannon;

struct SuckMoby {
    u8 pad0[0xC]; float radius; SuckVec pos;
    s8 state; u8 pad21[3]; SuckClass *cls; SuckMoby *next;
    u8 pad2C[8]; u16 flags; u8 pad36[0x1D];
    u8 anim_state; u8 pad54[0x1C]; u8 anim_flags; u8 pad71[7];
    SuckCannon *data; u8 pad7C[0x18]; s32 collision;
};

typedef struct SuckHero {
    u8 pad0[0x80];
    SuckVec pos;
    u8 pad90[0x70];
    SuckVec displacement;          /* 0x100: added to the launch velocity */
    u8 pad110[0x530];
    SuckMatrix basis;              /* 0x640: aims the shot outside first person */
    u8 pad670[0x14];
    float pitch;
    float yaw;
    u8 pad68C[0xA14];
    s32 fire_buttons;              /* 0x10A0: item slot 0 button mask */
    u8 pad10A4[0x8];
    u8 firing;                     /* 0x10AC: 2 while a shot is fired */
    u8 pad10AD[0x3];
    s32 ready_frames;              /* 0x10B0: must exceed 15 (22) frames to suck (fire) */
    s32 slot_state;                /* 0x10B4: 3 = released */
    u8 pad10B8[0x1F8];
    s32 aim_locked;
    u8 pad12B4[0x4];
    SuckMoby * aim_target;
    float aim_yaw;
    u8 pad12C0[0xD90];
    SuckMoby * stored[10];         /* 0x2050: sucked-in mobys, 5 slots (10 with the gold cannon) */
    s32 count;                     /* 0x2078: stored mobys ready to fire */
    s32 sucking;                   /* 0x207C: mobys still being pulled in */
    SuckMoby * moby;
    s32 state;                     /* 0x2084: 30 = first-person aim */
    u8 pad2088[0x4];
    s32 control_mode;
    u8 pad2090[0x17];
    u8 unk20A7;
    u8 pad20A8[0x4];
    u8 unk20AC;
} SuckHero;

typedef struct SuckInput {
    u8 pad0[0x1A0];
    s32 held;
    u8 pad1A4[0x4];
    s32 used;
} SuckInput;

typedef struct SuckVoice {
    u8 pad0[0x4];
    u8 active;
    u8 pad5[0x13];
    SuckMoby * owner;
    u8 pad1C[0x54];
} SuckVoice;

typedef struct SuckStats {
    u8 pad0[0x48];
    u16 count;
    u16 longest;
    s32 level_mask;
} SuckStats;

typedef struct SuckHit {
    u8 pad0[0x18];
    SuckMoby * moby;
    u8 pad1C[0x4];
    SuckVec pos;
} SuckHit;

typedef struct { u8 pad[9]; u8 gold; } SuckItemVariant; /* item 9 (Suck Cannon) is gold */

typedef struct { u8 pad[0x10]; float height; } SuckAimHeight;
extern char D_L00_00166C80[];
extern char D_L00_00166DC0[];
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32 approximate_arcsine(f32) __asm__("func_001F9DF8");
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 square_root_scalar(f32) __asm__("func_001F9988");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern float fast_difference_between_rotations(float, float) __asm__("func_001FA688");
extern int D_L00_00173E40_lui[];
extern int collision_segment_hits(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int allocate_voice_for_target_entry_alt(int,int,int) __asm__("FUN_0022da68");
extern int queue_animation_update_alt(int,int,int,int,int,int,int) __asm__("FUN_001ff308");
extern s32 D_0015EEA4;
extern s32 FUN_L00_002d9f78(void);
extern void transpose_rotation_matrix(void *,void *) __asm__("FUN_001fa2d8");
extern void FUN_L00_0020e4c8(void);
extern void FUN_L00_0020e698(void);
extern void FUN_L00_002223f8(int,int);
extern void FUN_L00_002377b8(void);
extern void FUN_L00_00237840(void);
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void FUN_L00_002d9fd0(char *,char *,void *);
extern void FUN_L00_002da360(u8 *,void *);
extern void FUN_L00_002da660(void);
extern void FUN_L00_002defe0(int);
extern void FUN_L00_002df048(int);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("func_0020E098");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
f32 compute_interpolated_record_value(void *anim) __asm__("FUN_0020c9e0");
float absolute_float(float input) __asm__("func_001F99C0");
s32 scale_game_frames_alt(s32) __asm__("FUN_001f96f8");
#include "rnc/gameplay/state/game_globals.h"
extern char D_0013E550[];
extern int func_001F9908_r(void *) __asm__("FUN_001f9740");
extern SuckHero suck_hero __asm__("D_0013F350");
extern SuckItemVariant suck_item_variant __asm__("D_0013E520");
extern SuckInput suck_input __asm__("D_0013C940");
extern SuckMoby *suck_moby_list __asm__("D_L00_0015FFE4");
extern char suck_fire_tuning[1] __asm__("D_L00_00161B30") __attribute__((sda));
extern void suck_delete_callback(void) __asm__("FUN_L00_00237798");
extern void suck_reset(SuckMoby *) __asm__("FUN_L00_002ddca0");
extern void suck_draw_reticle(int,int,float,float,int,int,int,float,int) __asm__("FUN_L00_001edf60");
extern SuckAimHeight *suck_get_aim_height(void *) __asm__("FUN_002141f8");
extern float D_L00_00173E68 __attribute__((section(".data")));
void suckcannon_update(SuckMoby *self) __asm__("FUN_L00_002da790");
void suckcannon_update(SuckMoby *self) {
    enum {
        SUCKCANNON_STATE_INITIALIZE = 0,
        SUCKCANNON_STATE_EMPTY_IDLE = 1,
        SUCKCANNON_STATE_SUCTION_STARTUP = 2,
        SUCKCANNON_STATE_SUCTION_ACTIVE = 3,
        SUCKCANNON_STATE_LOADED_AIM = 4,
        SUCKCANNON_STATE_FIRE_STORED_MOBY = 5
    };
    SuckWork work;
    SuckCannon *cannon;
    SuckStats *stats;
    int slot_released, i, sound, next_state;
    SuckMoby *stored;

    queue_animation_update_alt(4, 0x753F, (int)suck_delete_callback, (int)FUN_L00_002377b8,
                 (int)FUN_L00_00237840, (int)&suck_hero.count, 5);
    cannon = self->data;
    slot_released = suck_hero.slot_state == 3;
    if ((u8)self->state != 3 && (u8)self->state != 0) FUN_L00_002defe0((int)self);
    FUN_L00_002da660();
    switch ((u8)self->state) {
    case SUCKCANNON_STATE_INITIALIZE:
        for (i=0; i<(suck_item_variant.gold == 0 ? 5 : 10); ++i) {
            if (suck_hero.stored[i]) {
                suck_hero.stored[i]->collision = 0;
                suck_hero.stored[i]->flags &= ~0x1000;
            }
        }
        suck_hero.firing = 0;
        if (suck_hero.count > 0) {
            cannon->blocked = 1;
            blend_moby_animation(self, 6, 0, scale_game_frames_alt(4));
            self->state = 4;
        } else {
            cannon->cooldown = 0;
            self->state = 1;
        }
        suck_reset(self);
        break;
    case SUCKCANNON_STATE_EMPTY_IDLE:
        if (self->anim_flags & 2) blend_moby_animation(self,1,0,scale_game_frames_alt(0));
        func_001F9908_r(&cannon->cooldown);
        if (cannon->cooldown == 0) FUN_L00_0020e698();
        if (suck_hero.count > 0) {
            blend_moby_animation(self,6,0,scale_game_frames_alt(4));
            self->state = 4;
            break;
        }
        if ((suck_input.held & suck_hero.fire_buttons) && cannon->cooldown==0 && suck_hero.unk20A7==0 &&
            suck_hero.unk20AC==0 && scale_game_frames_alt(15) < suck_hero.ready_frames && FUN_L00_002d9f78()==0) {
            FUN_L00_0020e4c8();
            cannon->sound = allocate_voice_for_target_entry_alt(2,4,(int)self);
            cannon->cooldown = scale_game_frames_alt(10);
            stats = (SuckStats *)(((char *)&item_usage));
            if (stats->count < 65535) stats->count++;
            if (scale_game_frames_alt(D_0015EEA4)/600 > stats->longest)
                stats->longest = scale_game_frames_alt(D_0015EEA4)/600;
            stats->level_mask = (stats->level_mask | (1 << current_level_index)) | 0x80000000;
            self->state = 2;
        }
        break;
    case SUCKCANNON_STATE_SUCTION_STARTUP:
        if (self->anim_flags & 2) blend_moby_animation(self,3,0,scale_game_frames_alt(5));
        if (FUN_L00_002d9f78()!=0 || suck_hero.unk20AC || suck_hero.unk20A7) {
            sound = cannon->sound;
            if (sound!=-1) {
                SuckVoice *s=(SuckVoice *)(((char *)&D_0013E550))+sound;
                if (s[1].owner==self && s[1].active) release_voice_slot(sound);
            }
            cannon->sound=-1;
            FUN_L00_0020e698();
            suck_hero.firing=0;
            if (suck_hero.count>0) {
                blend_moby_animation(self,6,0,scale_game_frames_alt(4));
                next_state=4;
                goto changed;
            } else {
                cannon->cooldown=0;
                blend_moby_animation(self,1,0,scale_game_frames_alt(4));
                next_state=1;
                goto changed;
            }
        } else if (func_001F9908_r(&cannon->cooldown)) {
            cannon->cooldown=scale_game_frames_alt(20);
            next_state=3;
            goto changed;
        }
        break;
    case SUCKCANNON_STATE_SUCTION_ACTIVE:
        if (self->anim_state!=3 && (self->anim_state!=0 || (self->anim_flags&2)))
            blend_moby_animation(self,3,0,scale_game_frames_alt(5));
        func_001F9908_r(&cannon->cooldown);
        if (!(suck_hero.sucking || (suck_hero.count < (suck_item_variant.gold==0?5:10) &&
            FUN_L00_002d9f78()==0 && ((suck_input.held&suck_hero.fire_buttons) || cannon->cooldown) &&
            !suck_hero.unk20AC && !suck_hero.unk20A7))) {
            FUN_L00_0020e698();
            sound=cannon->sound;
            if(sound!=-1) {
                SuckVoice *s=(SuckVoice *)(((char *)&D_0013E550))+sound;
                if(s[1].owner==self && s[1].active) release_voice_slot(sound);
            }
            cannon->sound=-1;
            FUN_L00_002df048((int)self);
            suck_hero.firing=0;
            if(suck_hero.count>0) {
                blend_moby_animation(self,6,0,scale_game_frames_alt(4));
                cannon->cooldown=0;
                cannon->blocked=1;
                self->state=4;
                goto done;
            } else {
                cannon->cooldown=0;
                blend_moby_animation(self,1,0,scale_game_frames_alt(4));
                self->state=1;
                goto done;
            }
        } else {
            if (self->anim_flags&2) blend_moby_animation(self,3,0,scale_game_frames_alt(2));
            FUN_L00_002da360((u8 *)self,cannon);
        }
        break;
    case SUCKCANNON_STATE_LOADED_AIM:
        func_001F9908_r(&cannon->cooldown);
        if (!cannon->cooldown) FUN_L00_0020e698();
        if (self->anim_flags&2) {
            blend_moby_animation(self,6,0,scale_game_frames_alt(0));
            FUN_L00_0020e698();
        }
        if (!(suck_input.held&suck_hero.fire_buttons)) cannon->blocked=0;
        if (func_001F9908_r(&cannon->target_timer) ||
            (cannon->target && ((u8)cannon->target->state&0x80))) cannon->target=0;
        if (suck_hero.state==1) FUN_L00_002223f8(30,1);
        if (suck_hero.state==30 && (suck_hero.moby->flags&1) && !suck_hero.unk20AC) {
            char *camera=D_L00_00166C80;
            SuckVec *eye;
            SuckHit *hit;
            work.aim.velocity.q=0;
            work.aim.velocity.f.z=50.0f;
            copy_matrix3x4(&work.aim.rotation,camera);
            eye=(SuckVec *)(camera+0x140);
            transpose_rotation_matrix(&work.aim.inverse,&work.aim.rotation);
            transform_vector(&work.aim.velocity,&work.aim.velocity,&work.aim.inverse);
            add_vector_xyz(&work.aim.end,eye,&work.aim.velocity);
            if(collision_segment_hits(eye,&work.aim.end,0,(int)self,0)) {
                hit=(SuckHit *)D_L00_00173E40_lui;
                if(hit->moby && (hit->moby->flags&0x1000)) {
                    cannon->target=hit->moby;
                    cannon->target_timer=scale_game_frames_alt(20);
                    cannon->target_height=hit->pos.f.z-cannon->target->pos.f.z;
                }
            }
            if(cannon->target && !cannon->cooldown) {
                qcopy(&work.aim.point,&cannon->target->pos);
                work.aim.point.f.z+=cannon->target_height;
                suck_draw_reticle((int)self,0xFF0FFF0F,1.0f,0.0f,0,0x21,-1,90.0f,4);
            } else {
                suck_draw_reticle((int)self,0xFF0F0FFF,1.0f,0.0f,0,0x21,-1,90.0f,4);
            }
        }
        if(suck_hero.aim_locked) {
            cannon->target=suck_hero.aim_target;
            cannon->target_timer=scale_game_frames_alt(20);
            cannon->target_height=D_L00_00173E68-cannon->target->pos.f.z;
        }
        if(!cannon->blocked && suck_hero.count>0 && !cannon->cooldown &&
            (suck_input.held&suck_hero.fire_buttons) && suck_hero.control_mode!=7 && suck_hero.control_mode!=3 &&
            suck_hero.control_mode!=18 && suck_hero.control_mode!=17 && !suck_hero.unk20AC && !suck_hero.unk20A7 &&
            scale_game_frames_alt(22)<suck_hero.ready_frames) {
            FUN_L00_0020e4c8();
            blend_moby_animation(self,5,0,scale_game_frames_alt(2));
            suck_hero.firing=2;
            cannon->cooldown=scale_game_frames_alt(25);
            stats=(SuckStats *)(((char *)&item_usage));
            if(stats->count<65535) stats->count++;
            if(scale_game_frames_alt(D_0015EEA4)/600>stats->longest)
                stats->longest=scale_game_frames_alt(D_0015EEA4)/600;
            stats->level_mask = (stats->level_mask | (1 << current_level_index)) | 0x80000000;
            self->state=5;
        }
        break;
    case 6:
        break;
    case SUCKCANNON_STATE_FIRE_STORED_MOBY:
        stored=0;
        if (!(0.0f <= compute_interpolated_record_value(self))) break;
        for(i=0; i<(suck_item_variant.gold==0?5:10);i++) {
            if(suck_hero.stored[i]) {
                stored=suck_hero.stored[i];
                suck_hero.stored[i]=0;
                break;
            }
        }
        if(stored) {
            SuckMoby *best,*m;
            float yaw,pitch,limit;
            work.fire.velocity.q=0;
            copy_matrix3x4(&work.fire.rotation,D_L00_00166C80);
            transpose_rotation_matrix(&work.fire.inverse,&work.fire.rotation);
            if(suck_hero.state==30 && (suck_hero.moby->flags&1)) {
                work.fire.offset.q=0;
                work.fire.offset.f.z=frame_scale*0.4462f;
                work.fire.point.q=work.fire.offset.q;
                transform_vector(&work.fire.velocity,&work.fire.point,&work.fire.inverse);
                work.fire.offset.q=0;
                work.fire.offset.f.x=0.15f;
                work.fire.offset.f.y=0.3f;
                work.fire.offset.f.z=1.0f;
                transform_vector(&work.fire.launch,&work.fire.offset,&work.fire.inverse);
                add_vector_xyz(&work.fire.launch,&work.fire.launch,D_L00_00166C80+0x140);
            } else {
                work.fire.offset.q=0;
                work.fire.offset.f.x=frame_scale*0.4462f;
                work.fire.point.q=work.fire.offset.q;
                transform_vector_by_basis(&work.fire.velocity,&work.fire.point,&suck_hero.basis);
                FUN_L00_0024f7c8(self,0,&work.fire.launch);
            }
            qcopy(&stored->pos,&work.fire.launch);
            refresh_moby_spatial_bounds_from_basis(stored);
            add_vector_xyz(&work.fire.velocity,&work.fire.velocity,&suck_hero.displacement);
            if(suck_hero.state!=30 || !(suck_hero.moby->flags&1)) {
                limit=10000.0f;
                best=0;
                if(suck_hero.aim_locked) yaw=suck_hero.aim_yaw;
                else yaw=fast_arc_tan(work.fire.velocity.f.x,work.fire.velocity.f.y);
                pitch=-fast_arc_tan(vector_length_xy(&work.fire.velocity),work.fire.velocity.f.z);
                m=suck_moby_list;
                while(m) {
                    if(m->state>=0 && m!=stored) {
                        float tyaw,tpitch,dyaw,distance,offangle;
                        SuckAimHeight *r;
                        qcopy(&work.fire.point,&m->pos);
                        r=suck_get_aim_height(m);
                        if(r) work.fire.point.f.z+=r->height;
                        else work.fire.point.f.z+=0.5f;
                        if((m->flags&0x1000) && m && m->cls && m->cls->category==5) {
                            tyaw=fast_arc_tan(work.fire.point.f.x-work.fire.launch.f.x,work.fire.point.f.y-work.fire.launch.f.y);
                            tpitch=fast_arc_tan(vector_distance_xy(&work.fire.launch,&work.fire.point),work.fire.point.f.z-work.fire.launch.f.z);
                            dyaw=fast_difference_between_rotations(yaw,tyaw);
                            distance=distance_xyz(&work.fire.launch,&work.fire.point);
                            if(distance<2.5f && fast_difference_between_rotations(suck_hero.yaw,
                                fast_arc_tan(m->pos.f.x-suck_hero.pos.f.x,m->pos.f.y-suck_hero.pos.f.y))<1.0471976f &&
                                absolute_float(tpitch)<0.7853982f) {
                                best=m;
                                break;
                            }
                            if(!(limit<distance)) {
                                float delta;
                                build_spherical_offset(&work.fire.offset,distance,yaw,pitch);
                                add_vector_xyz(&work.fire.offset,&work.fire.offset,&work.fire.launch);
                                delta=distance_xyz(&work.fire.offset,&work.fire.point);
                                offangle=1.5707964f-approximate_arcsine(1.0f-delta*delta/((distance+distance)*distance));
                                if(0.4537856f<offangle) {
                                    float radius=m->radius*0.0009765625f;
                                    float adjustment=1.5707964f-approximate_arcsine(distance/square_root_scalar(distance*distance+radius*radius));
                                    if(radius<distance) offangle-=adjustment;
                                }
                                if(offangle<0.4537856f) {
                                    if(collision_segment_hits(D_L00_00166DC0,&work.fire.point,6,(int)self,0)) break;
                                    limit=distance;
                                    pitch=-tpitch;
                                    if(dyaw<0.17453292f) {
                                        yaw=tyaw;
                                        best=m;
                                    }
                                }
                            }
                        }
                    }
                    m=m->next;
                }
                if(best) cannon->target=best;
            }
            FUN_L00_002d9fd0((char *)self,(char *)stored,suck_fire_tuning);
            if(stored->cls->handlers->fire) stored->cls->handlers->fire(stored,&work.fire.velocity,cannon->target,cannon->target_height);
            suck_hero.count--;
            if(suck_hero.count>0) {
                suck_hero.firing=0;
                self->state=4;
                goto done;
            } else {
                cannon->cooldown=scale_game_frames_alt(20);
                next_state=1;
                suck_hero.firing=0;
                self->state=next_state;
                goto done;
            }
        }
        break;
    }
    goto done;
changed:
    self->state=next_state;
done:
    if(slot_released) {
        sound=cannon->sound;
        if(sound!=-1) {
            SuckVoice *s=(SuckVoice *)(((char *)&D_0013E550))+sound;
            if(s[1].owner==self && s[1].active) release_voice_slot(sound);
        }
        cannon->sound=-1;
        if(suck_input.held&5) suck_input.used |=5;
    }
}
typedef s32 (*Fn_db890)(void *);
typedef struct Cb_db890 {
    char pad[0x10];
    Fn_db890 f10;
} Cb_db890;
typedef struct Sub_db890 {
    char pad[0x2C];
    Cb_db890 *f2C;
} Sub_db890;
typedef struct Obj_db890 {
    char pad0[0x20];
    u8 f20;
    char pad1[3];
    Sub_db890 *f24;
    char pad2[0x50];
    s32 f78;
} Obj_db890;
/* Calls the class function at +0x10 for a moby not in state 0xFE/0xFD with data; else 0. */
s32 FUN_L00_002db890(Obj_db890 *o) {
    Fn_db890 f;
    if (o == 0 || o->f20 == 0xFE)
        return 0;
    if (o->f20 == 0xFD || o->f78 == 0 || o->f24 == 0)
        return 0;
    f = o->f24->f2C->f10;
    if (f)
        return f(o);
    return 0;
}
void debug_print_2db8f8(void *) __asm__("FUN_001e93b0");
u8 *FUN_L00_002db890_002db8f8(u8 *) __asm__("FUN_L00_002db890");
s32 scale_game_frames_2db8f8(s32) __asm__("FUN_001f96f8");
void blend_moby_animation_2db8f8(void *, s32, s32, s32) __asm__("FUN_00212f90");
f32 vector_distance_2db8f8(void *, void *) __asm__("FUN_001f9b80");
s32 FUN_L00_002db8f8(u8 *moby, u8 *position, u8 *target) {
    enum {
        SUCK_DISPATCH_ALIGNING_FOR_SUCTION = 0,
        SUCK_DISPATCH_RISING_FOR_SUCTION = 1,
        SUCK_DISPATCH_BEING_PULLED_IN = 2,
        SUCK_DISPATCH_STORED = 3,
        SUCK_DISPATCH_FLYING_PROJECTILE = 4,
        SUCK_DISPATCH_ROLLING_PROJECTILE = 5,
        SUCK_DISPATCH_FALLING_AFTER_CANCEL = 6
    };
    u8 *record = FUN_L00_002db890_002db8f8(moby);
    s16 state;
    if (record == 0)
        goto missing;
    if (moby == 0 || moby[0x20] == 0xFE || moby[0x20] == 0xFD || *(s16 *)(record + 0x68) == 8)
        goto fail;
    qcopy_nc(record, position);
    *(u8 **)(record + 0x60) = target;
    *(f32 *)(record + 0xC) = 1.0f;
    state = (s16)(*(u16 *)(record + 0x68) - 1);
    switch (state) {
    case SUCK_DISPATCH_ALIGNING_FOR_SUCTION:
        if (1.0f <= *(f32 *)(record + 0x6C)) {
            {
                u8 *seq = *(u8 **)(record + 0x70);
                if (moby[0x53] != seq[2]) {
                    blend_moby_animation_2db8f8(moby, seq[2], 0, scale_game_frames_2db8f8(10));
                }
            }
            *(s32 *)(record + 0x6C) = 0;
            *(s16 *)(record + 0x68) = 2;
            return 1;
        }
        {
            u8 *seq = *(u8 **)(record + 0x70);
            if (moby[0x53] != seq[1]) {
                blend_moby_animation_2db8f8(moby, seq[1], 0, scale_game_frames_2db8f8(10));
            }
        }
        return 1;
    case SUCK_DISPATCH_RISING_FOR_SUCTION:
        if (!(1.0f <= *(f32 *)(record + 0x6C)))
            return 1;
        return 2;
    case SUCK_DISPATCH_BEING_PULLED_IN:
        break;
    case SUCK_DISPATCH_STORED:
        return 3;
    case SUCK_DISPATCH_FLYING_PROJECTILE:
    case SUCK_DISPATCH_ROLLING_PROJECTILE:
    case SUCK_DISPATCH_FALLING_AFTER_CANCEL:
        {
            u8 *seq = *(u8 **)(record + 0x70);
            if (moby[0x53] != seq[3]) {
                blend_moby_animation_2db8f8(moby, seq[3], 0, scale_game_frames_2db8f8(10));
            }
        }
        *(s16 *)(record + 0x68) = 3;
        *(s32 *)(record + 0x6C) = 0;
        *(f32 *)(moby + 0x58) = 1.0f;
        *(f32 *)(record + 0x64) = vector_distance_2db8f8(moby + 0x10, target + 0x10);
        break;
    default:
        {
            u8 *seq = *(u8 **)(record + 0x70);
            if (moby[0x53] != seq[1]) {
                blend_moby_animation_2db8f8(moby, seq[1], 0, scale_game_frames_2db8f8(10));
            }
        }
        *(s32 *)(record + 0x6C) = 0;
        *(s16 *)(record + 0x68) = 1;
        return 1;
    }
    return 2;
missing:
    debug_print_2db8f8(D_L00_001EA030_002db8f8);
fail:
    return 0;
}
extern u8 *D_001403E0_002dbb20 __asm__("D_001403E0") __attribute__((section(".data")));
u8 *FUN_L00_002db890_002dbb20(u8 *) __asm__("FUN_L00_002db890");
void FUN_L00_002d9fd0_002dbb20(void *, void *, void *) __asm__("FUN_L00_002d9fd0");
void FUN_0020e098_002dbb20(void *) __asm__("FUN_0020e098");
void FUN_0022da68_002dbb20(s32, s32, void *) __asm__("FUN_0022da68");
void FUN_L00_00257470_002dbb20(void *, s32, s32) __asm__("FUN_L00_00257470");
s32 FUN_001f96f8_002dbb20(s32) __asm__("FUN_001f96f8");
void FUN_00212f90_002dbb20(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern u32 D_L00_00161B20[1] __attribute__((sda));
s32 FUN_L00_002dbb20(u8 *m, OvlQuad *v) {
    u8 *o = FUN_L00_002db890_002dbb20(m);
    u8 *g;
    if (o == 0)
        return 0;
    if (m != 0 && m[0x20] != 0xFE && m[0x20] != 0xFD && *(s16 *)(o + 0x68) != 8) {
        qcopy(m + 0x10, v);
        FUN_L00_002d9fd0_002dbb20(*(void **)(o + 0x60), m, D_L00_00161B20);
        FUN_0020e098_002dbb20(m);
        g = D_001403E0_002dbb20;
        if (g != 0 && *(s16 *)(g + 0xA6) == 0x351) {
            FUN_0022da68_002dbb20(5, 0, g);
        }
        FUN_L00_00257470_002dbb20(m, 0, -1);
        *(s16 *)(o + 0x68) = 4;
        if (m[0x53] != *(u8 *)(*(u8 **)(o + 0x70) + 4)) {
            u8 c = *(u8 *)(*(u8 **)(o + 0x70) + 4);
            FUN_00212f90_002dbb20(m, c, 0, FUN_001f96f8_002dbb20(10));
        }
        *(s32 *)(m + 0x2C) = 0;
        return 1;
    }
    return 0;
}
extern u8 D_0013F3D0_002dbc30[] __asm__("D_0013F3D0") __attribute__((section(".data")));
u8 *FUN_L00_002db890_002dbc30(u8 *) __asm__("FUN_L00_002db890");
u8 *FUN_002141f8_002dbc30(u8 *) __asm__("FUN_002141f8");
void FUN_0022da68_002dbc30(s32, s32, void *) __asm__("FUN_0022da68");
s32 FUN_001f96f8_002dbc30(s32) __asm__("FUN_001f96f8");
void FUN_00212f90_002dbc30(void *, s32, s32, s32) __asm__("FUN_00212f90");
f32 FUN_001f9b80_002dbc30(void *, void *) __asm__("FUN_001f9b80");
void FUN_001f9a10_002dbc30(void *, void *, void *) __asm__("FUN_001f9a10");
s32 FUN_L00_002dbc30(u8 *m, u8 *v, u8 *t, f32 f) {
    u8 *o = FUN_L00_002db890_002dbc30(m);
    u8 *x;
    u8 *d;
    if (o == 0)
        return 0;
    if (m == 0 || m[0x20] == 0xFE || m[0x20] == 0xFD || *(s16 *)(o + 0x68) == 8)
        goto fail;
    m[0x31] = 1;
    *(u16 *)(m + 0x34) &= 0xFFFC;
    if (*(f32 *)(v + 0xC) == 1.23456f) {
        if (m[0x53] != *(u8 *)(*(u8 **)(o + 0x70) + 6)) {
            u8 c = *(u8 *)(*(u8 **)(o + 0x70) + 6);
            FUN_00212f90_002dbc30(m, c, 0, FUN_001f96f8_002dbc30(0x14));
        }
        *(s32 *)(m + 0x98) = *(s32 *)(o + 0x88);
        *(s16 *)(o + 0x68) = 6;
    } else {
        if (m[0x53] != *(u8 *)(*(u8 **)(o + 0x70) + 5)) {
            u8 c = *(u8 *)(*(u8 **)(o + 0x70) + 5);
            FUN_00212f90_002dbc30(m, c, 0, FUN_001f96f8_002dbc30(0));
        }
        *(s16 *)(o + 0x68) = 5;
    }
    qcopy(o + 0x10, v);
    *(s32 *)(o + 0x94) = FUN_001f96f8_002dbc30(300);
    *(u8 **)(o + 0x74) = t;
    if (t != 0) {
        x = FUN_002141f8_002dbc30(t);
        if (x != 0)
            *(u16 *)(x + 0x1E) |= 0x80;
        qcopy(o + 0x40, t + 0x10);
        *(f32 *)(o + 0x90) = f;
        *(f32 *)(o + 0x48) += f;
    }
    if (FUN_001f9b80_002dbc30(m + 0x10, D_0013F3D0_002dbc30) < 1.0f) {
        FUN_001f9a10_002dbc30(m + 0x10, m + 0x10, o + 0x10);
    }
    if (*(s16 *)(o + 0x9C) >= 0) {
        FUN_0022da68_002dbc30(*(s16 *)(o + 0x9C), 0, m);
    }
    return 2;
fail:
    return 0;
}
typedef struct {
    u8 pad0[0x2050];
    s32 slot[11];
    s32 count;
} G;
extern G D_0013F350_u __asm__("D_0013F350");
u8 *FUN_L00_002db890_u(u8 *) __asm__("FUN_L00_002db890");
s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
s32 FUN_L00_002dbe20(u8 *m) {
    enum {
        SUCK_CANNON_INACTIVE = 0,
        SUCK_CANNON_ALIGNING_FOR_SUCTION = 1,
        SUCK_CANNON_RISING_FOR_SUCTION = 2,
        SUCK_CANNON_BEING_PULLED_IN = 3,
        SUCK_CANNON_STORED = 4,
        SUCK_CANNON_FLYING_PROJECTILE = 5,
        SUCK_CANNON_ROLLING_PROJECTILE = 6,
        SUCK_CANNON_FALLING_AFTER_CANCEL = 7
    };
    u8 *o = FUN_L00_002db890_u(m);
    s32 k;
    if (o == 0)
        return 0;
    if (m == 0 || m[0x20] == 0xFE || m[0x20] == 0xFD)
        return 0;
    k = *(s16 *)(o + 0x68);
    if (k == 8)
        return 0;
    switch (k) {
    case SUCK_CANNON_BEING_PULLED_IN:
        if (*(s16 *)(o + 0x6A) != -1) {
            D_0013F350_u.slot[*(s16 *)(o + 0x6A)] = 0;
            if (D_0013F350_u.count > 0)
                D_0013F350_u.count--;
        }
        /* fall through */
    case SUCK_CANNON_ALIGNING_FOR_SUCTION:
    case SUCK_CANNON_RISING_FOR_SUCTION: {
        if (m[0x53] != *(u8 *)(*(u8 **)(o + 0x70) + 7)) {
            u8 c = *(u8 *)(*(u8 **)(o + 0x70) + 7);
            blend_moby_animation(m, c, 0, scale_game_frames(10));
        }
        *(s16 *)(o + 0x68) = 7;
        return 0;
    }
    case SUCK_CANNON_STORED:
        return 1;
    case SUCK_CANNON_INACTIVE:
    case SUCK_CANNON_FLYING_PROJECTILE:
    case SUCK_CANNON_ROLLING_PROJECTILE:
    case SUCK_CANNON_FALLING_AFTER_CANCEL:
    case 9:
        return 0;
    }
    return 0;
}
/* Explosion burst for a moby: debris sprays, sparks, coloured flashes, camera shake and an optional damage area. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DD3D8), where it is exact; names translated to the US level program. */

typedef int u128_DD3D8 __attribute__((mode(TI)));

typedef struct { u128_DD3D8 v[5]; } B50_DD3D8;

typedef struct { int v[6]; } I6_DD3D8;

typedef struct { char p0[0x160]; float f160; int p164; int i168; } Cam_DD3D8;

extern B50_DD3D8 D_L00_001EA0B0;
extern I6_DD3D8 D_L00_001EA100;
extern I6_DD3D8 D_L00_001EA118;
extern char *FUN_L00_002af450(char *, char *, int, int, int);
extern char *mine_camera_blast(char *, char *, int, char *) __asm__("FUN_L00_002d3838");
extern f32 fast_vec_dot(void *a, void *b) __asm__("func_001F9AB0");
extern f32 fast_vec_length(void *) __asm__("func_001F9AF0");
extern float D_0015ED6C;
extern int *D_L00_00177F00[];
extern int collect_mobies_in_sphere() __asm__("FUN_L00_001f2868");
extern int FUN_L00_00257b90_c(int, int) __asm__("FUN_L00_00257b90");
extern s32 random_integer_below_c2(s32) __asm__("func_00213260");
extern s32 scale_game_frames_c(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32_c2(f32) __asm__("func_001FA6D0");
extern unsigned FUN_L00_0025c0e8(unsigned, int);
extern void FUN_L00_0025a9f8(void *, void *, void *, int, int, int, int, int, float, float, float);
extern void FUN_L00_0025c088(int *, int *, int *, int);
extern void FUN_L00_0026a9f0(void *, void *, int, int, float, int, int, int, int, float);
extern void clamp_vector_length(void *, void *, float) __asm__("FUN_001f9c90");
extern void fast_vec_normalize_c(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void fast_vec_scale_c(void *, void *, f32) __asm__("func_001F9A68");
extern void subtract_vector_xyz_c3(void *, void *, void *) __asm__("FUN_001f9a28");
unsigned char *FUN_L00_002df808(int owner, void *position, void *vector, int flags, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, float scale);
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");
extern char *func_L00_002E0CB8_DD3D8(float, void *, void *, void *, int, unsigned char, unsigned char, unsigned char, unsigned char) __asm__("FUN_L00_002df808");
extern char D_L00_00166C80[];
extern int func_L00_001F2BE8_DD3D8(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");

void FUN_L00_002dbf28(char *self, char *q) {
    B50_DD3D8 blk;
    float a[4];
    float b[4];
    float c[4];
    float d[4];
    float e[4];
    float g[4];
    I6_DD3D8 first_color_choices;
    I6_DD3D8 second_color_choices;
    int r0, g0, b0;
    int r1, g1, b1;
    float f22, dist, f24, sc, dt;
    int step, i, j, k;

    blk = D_L00_001EA0B0;
    *(u128_DD3D8 *)c = 0;
    c[2] = D_0015ED6C + D_0015ED6C;
    i = 0;
    *(u128_DD3D8 *)b = *(u128_DD3D8 *)c;
    *(u128_DD3D8 *)c = 0;
    c[2] = 1.0f;
    f22 = convert_integer_to_float((&D_0013E520)->b9) + 1.0f;
    step = 1;
    if ((&D_0013E520)->b9 == 0) step = 2;
    fast_vec_scale_c(b, b, f22);
    subtract_vector_xyz_c3(a, D_L00_00166DC0, self + 0x10);
    dist = fast_vec_length(a);
    for (; i < 10 / step; i++) {
        *(u128_DD3D8 *)g = 0;
        g[0] = random_float_between(-1.0f, 1.0f);
        g[1] = random_float_between(-1.0f, 1.0f);
        *(u128_DD3D8 *)e = *(u128_DD3D8 *)g;
        dt = fast_vec_dot(e, c);
        fast_vec_normalize_c(d, c, dt);
        subtract_vector_xyz_c3(e, e, d);
        fast_vec_normalize_c(d, c, random_float_between(0.0f, 1.0f));
        add_vector_xyz(e, e, d);
        fast_vec_normalize_c(e, e, f22 * random_float_between(3.5f, 6.5f) * D_0015ED6C);
        add_vector_xyz(e, e, b);
        {
            int h = scale_game_frames_c(60);
            FUN_L00_002af450(self + 0x10, (char *)e, FUN_L00_00257b90_c(h, scale_game_frames_c(120)), 0, (&D_0013E520)->b9);
        }
    }
    for (j = 0; j < 4 / step; j++) {
        *(u128_DD3D8 *)g = 0;
        g[0] = random_float_between(-1.0f, 1.0f);
        g[1] = random_float_between(-1.0f, 1.0f);
        *(u128_DD3D8 *)e = *(u128_DD3D8 *)g;
        dt = fast_vec_dot(e, c);
        fast_vec_normalize_c(d, c, dt);
        subtract_vector_xyz_c3(e, e, d);
        fast_vec_normalize_c(d, c, random_float_between(0.0f, 1.0f));
        add_vector_xyz(e, e, d);
        fast_vec_normalize_c(e, e, f22 * random_float_between(6.5f, 10.0f) * D_0015ED6C);
        add_vector_xyz(e, e, b);
        {
            int h = scale_game_frames_c(60);
            FUN_L00_002af450(self + 0x10, (char *)e, FUN_L00_00257b90_c(h, scale_game_frames_c(90)), 1, (&D_0013E520)->b9);
        }
    }
    *(u128_DD3D8 *)g = 0;
    g[0] = random_float_between(-1.0f, 1.0f);
    g[1] = random_float_between(-1.0f, 1.0f);
    g[2] = random_float_between(-1.0f, 1.0f);
    *(u128_DD3D8 *)e = *(u128_DD3D8 *)g;
    a[2] += dist * 0.5f;
    fast_vec_normalize_c(e, e, dist / 5.0f * D_0015ED6C);
    fast_vec_normalize_c(a, a, (dist + dist) * D_0015ED6C);
    add_vector_xyz(e, e, a);
    clamp_vector_length(e, e, D_0015ED6C * 10.0f);
    {
        int h = scale_game_frames_c(60);
        FUN_L00_002af450(self + 0x10, (char *)e, FUN_L00_00257b90_c(h, scale_game_frames_c(90)), 1, (&D_0013E520)->b9);
    }
    if (dist < 6.0f) {
        k = func_001FA898_r(dist) + 1;
    } else {
        k = 4;
    }
    f24 = 0.0f;
    if (dist < 7.0f) {
        f24 = 7.0f - dist;
    }
    if (k > 0) {
        int cnt = k;
        do {
            float sh = f22 * random_float_between(8.0f, 10.0f) * D_0015ED6C - f24 * D_0015ED6C;
            int A, B, r1, r2;
            float big;
            first_color_choices = D_L00_001EA100;
            second_color_choices = D_L00_001EA118;
            A = FUN_L00_0025c0e8(first_color_choices.v[random_integer_below_c2(6)], (&D_0013E520)->b9);
            B = FUN_L00_0025c0e8(second_color_choices.v[random_integer_below_c2(6)], (&D_0013E520)->b9);
            big = f22 * 400000.0f;
            r1 = FUN_L00_00257b90_c(scale_game_frames_c(15), scale_game_frames_c(20));
            r2 = FUN_L00_00257b90_c(scale_game_frames_c(25), scale_game_frames_c(30));
            FUN_L00_0026a9f0(self + 0x10, b, A, B, big, r1, r2, 0, 0, sh);
        } while (--cnt);
    }
    if (dist > 9.0f) {
        float size = f22 * 4.0f;
        func_L00_002E0CB8_DD3D8(size, self, self + 0x10, b, scale_game_frames_c(15), 0x7F, 0x7F, 0x7F, 0x20);
        r0 = 0x7F;
        g0 = 0x20;
        b0 = 0;
        FUN_L00_0025c088(&r0, &g0, &b0, (&D_0013E520)->b9);
        func_L00_002E0CB8_DD3D8(size, self, self + 0x10, b, scale_game_frames_c(24), r0, g0, b0, 0x20);
    }
    r1 = 0x7F;
    g1 = 0x3F;
    b1 = 0;
    FUN_L00_0025c088(&r1, &g1, &b1, (&D_0013E520)->b9);
    {
        float s = f22 * 4.0f;
        func_L00_002E0CB8_DD3D8(s, self, self + 0x10, b, scale_game_frames_c(20), r1, g1, b1, 0x30);
    }
    r1 = 0x60;
    g1 = 0x10;
    b1 = 0;
    FUN_L00_0025c088(&r1, &g1, &b1, (&D_0013E520)->b9);
    {
        float s = f22 * 3.5f;
        func_L00_002E0CB8_DD3D8(s, self, self + 0x10, b, scale_game_frames_c(27), r1, g1, b1, 0x40);
    }
    r1 = 0x20;
    g1 = 0;
    b1 = 0;
    FUN_L00_0025c088(&r1, &g1, &b1, (&D_0013E520)->b9);
    {
        float s = f22 * 3.0f;
        func_L00_002E0CB8_DD3D8(s, self, self + 0x10, b, scale_game_frames_c(29), r1, g1, b1, 0x20);
    }
    (*(Cam_DD3D8 *)D_L00_00166C80).f160 = (dist < 20.0f) ? 0.4f - dist * 0.0175f : 0.050000012f;
    (*(Cam_DD3D8 *)D_L00_00166C80).i168 = scale_game_frames_c(25);
    mine_camera_blast((char *)&blk, self + 0x10, 0, 0);
    if ((&D_0013E520)->b9) {
        k = func_L00_001F2BE8_DD3D8(3.0f, self + 0x10, 0x10, self, 0);
        *(u128_DD3D8 *)e = *(u128_DD3D8 *)(self + 0x10);
        FUN_L00_0025a9f8(self, e, D_L00_00177F00, k, 0, 0x830000, 2, 1, 2.0f, 1.0f, 1.0f);
    }
}
#include "qzero.h"
typedef struct {
    int v[16];
} __attribute__((aligned(16))) Mtx_002dc938;
void FUN_L00_002dbf28_002dc938(char *, char *) __asm__("FUN_L00_002dbf28");
void FUN_001fa050_002dc938(Mtx_002dc938 *, void *) __asm__("FUN_001fa050");
void FUN_001fa2b8_002dc938(void *, Mtx_002dc938 *) __asm__("FUN_001fa2b8");
void FUN_0020e098_002dc938(char *) __asm__("FUN_0020e098");
void FUN_0020c828_002dc938(char *) __asm__("FUN_0020c828");
void FUN_L00_002dc938(char *p, char *q, int flag) {
    Mtx_002dc938 m;
    void (*fn)(char *);
    if (flag)
        FUN_L00_002dbf28_002dc938(p, q);
    *(int *)(p + 0x98) = *(int *)(q + 0x84);
    qzero(p + 0x40);
    FUN_001fa050_002dc938(&m, p + 0x40);
    FUN_001fa2b8_002dc938(p + 0xC0, &m);
    FUN_0020e098_002dc938(p);
    *(unsigned short *)(p + 0x34) &= ~4;
    *(int *)(p + 0x98) = *(int *)(q + 0x84);
    *(float *)(p + 0x2C) = *(float *)(*(char **)(p + 0x24) + 0x24);
    *(short *)(q + 0x68) = 0;
    fn = *(void (**)(char *))(*(char **)(*(char **)(p + 0x24) + 0x2C) + 0x14);
    if (fn)
        fn(p);
    else
        FUN_0020c828_002dc938(p);
}
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DDEA0), where it is exact; names translated to the US level program. */

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) Vec_2DDEA0;

typedef struct { char pad0[0xC]; void (*fC)(void *); } Cb_2DDEA0;

typedef struct { char pad0[0x10]; int i10; char pad14[0x10]; float f24; char pad28[4]; Cb_2DDEA0 *f2C; char pad30[0x16]; short h46; } Cls_2DDEA0;

typedef struct {
    float x, y, f8, timer;
    Vec_2DDEA0 vel;
    Vec_2DDEA0 q20;
    Vec_2DDEA0 v30;
    Vec_2DDEA0 v40;
    Vec_2DDEA0 v50;
    struct Moby *owner;
    float f64;
    short state;
    short idx;
    float f6C;
    unsigned char *anims;
    struct Moby *target;
    float f78, f7C, f80;
    int i84, i88;
    float f8C, f90;
    int i94;
    float f98;
    short h9C, h9E;
} R_2DDEA0;

typedef struct { char pad0[0x18]; struct Moby *hit; char pad1C[4]; Vec_2DDEA0 v20; Vec_2DDEA0 v30; Vec_2DDEA0 v40; } Coll_2DDEA0;

typedef struct { Vec_2DDEA0 v; char pad10[8]; unsigned char b18; unsigned char b19; short h1A; int pad1C; } Hit_2DDEA0;

typedef union {
    Vec_2DDEA0 v[15];
    u128 q;
    struct { Vec_2DDEA0 pad; Hit_2DDEA0 hit; } h;
} Frame_2DDEA0;

extern char *FUN_L00_002db890_c2(char *) __asm__("FUN_L00_002db890");
extern f32 fast_add_rotations_c2(f32, f32) __asm__("func_001FA580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 square_root_scalar(f32) __asm__("func_001F9988");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 vector_length_xyz_c(void *) __asm__("FUN_001f9af0");
extern int D_0013E529_c __asm__("D_0013E529");
extern int D_001403FC; /* no foreign declaration */
extern int multiply_quaternions() __asm__("FUN_001fa3c0");
extern int quaternion_nlerp() __asm__("FUN_001fa400");
extern int quaternion_to_matrix() __asm__("FUN_001fa4f8");
extern int matrix_to_quaternion() __asm__("FUN_00214260");
extern int coll_sphere() __asm__("FUN_L00_001f0d60");
extern int collect_mobies_in_sphere() __asm__("FUN_L00_001f2868");
extern int FUN_L00_001ff430();
extern int FUN_L00_001ff660();
extern int FUN_L00_00259888();
extern int allocate_voice_for_target_entry_alt(int,int,int) __asm__("FUN_0022da68");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern void DebugPrint(char *, ...);
extern void transpose_rotation_matrix(void *,void *) __asm__("FUN_001fa2d8");
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void add_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a10");
extern void blend_moby_animation_c(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void build_quaternion_from_axis_angle(void *, void *, f32) __asm__("FUN_00214530");
extern void clear_u64_value_c(void *) __asm__("func_001F99F8");
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
extern void extract_matrix_rotation_quaternion(void *, void *) __asm__("func_002144D8");
extern void normalize_vector_xyz_c2(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void refresh_moby_spatial_bounds_from_basis(void *) __asm__("func_0020E098");
extern void scale_vector_xyz_c2(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz_c2(void *, void *, void *) __asm__("FUN_001f9a28");
extern void tick_countdown_32(s32 *) __asm__("func_001F9740");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
f32 normalize_angle(f32 a) __asm__("FUN_L00_0025e310");
float absolute_float_c(float input) __asm__("func_001F99C0");
int FUN_L00_002da218(char *a);
s32 scale_game_frames_alt_c2(s32) __asm__("FUN_001f96f8");
void FUN_L00_00258278(unsigned char *m, float *ptr, float t, float b, float c, float limit);
void FUN_L00_00258ad0(int a, int b, float x, float y, int c, int d);
void FUN_L00_0026c3d0(void *pos, void *mat);
void FUN_L00_002dc938_c(char *p, char *q, int flag) __asm__("FUN_L00_002dc938");
void no_op_main_callback(void *) __asm__("FUN_001e93e8");
extern float frame_scale __asm__("D_0015ED60") __attribute__((section(".sdata")));
extern float frame_scale_sq_macro __asm__("D_0015ED64") __attribute__((section(".sdata")));
extern float frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern unsigned char D_0013E629_2DDEA0[] __asm__("D_0013E529");
extern unsigned char D_001404FC_2DDEA0[] __asm__("D_001403FC");
extern char D_0013F450_2DDEA0[] __asm__("D_0013F350");
extern char D_0013F4D0_2DDEA0[] __asm__("D_0013F3D0");
extern char D_0013FA90_2DDEA0[] __asm__("D_0013F990");
extern int D_L00_00173E40_lui[];
extern char D_L00_001EA4B0_2DDEA0[] __asm__("D_L00_001EA130");
extern R_2DDEA0 *func_L00_002DCD40_2DDEA0(struct Moby *) __asm__("FUN_L00_002db890");
extern float func_L00_001FF860_2DDEA0(float, float) __asm__("FUN_001f9e90");
extern float func_001FA748_2DDEA0(float, float) __asm__("FUN_001fa580");
extern float func_001FA790_2DDEA0(float, float) __asm__("FUN_001fa5c8");
extern void func_L00_002592B0_2DDEA0(void *, void *, float, float, float, float) __asm__("FUN_L00_00258278");
extern void func_001F9BC0_2DDEA0(void *) __asm__("FUN_001f99f8");
extern int func_001F9850_2DDEA0(int) __asm__("FUN_001f96f8");
extern float func_001FA888_2DDEA0(int) __asm__("FUN_001fa6c0");
extern void func_00215328_2DDEA0(void *, void *) __asm__("FUN_002144d8");
extern void func_001FA218_2DDEA0(void *, void *) __asm__("FUN_001fa050");
extern void func_001FA460_2DDEA0(void *, void *) __asm__("FUN_001fa298");
extern void func_001FA540_2DDEA0(void *, void *, void *) __asm__("FUN_001fa378");
extern void func_002150B0_2DDEA0(void *, void *) __asm__("FUN_00214260");
extern void func_001FA5C8_2DDEA0(void *, void *, void *, float) __asm__("FUN_001fa400");
extern void func_001FA6C0_2DDEA0(void *, void *) __asm__("FUN_001fa4f8");
extern void func_001FA480_2DDEA0(void *, void *) __asm__("FUN_001fa2b8");
extern void func_L00_00250800_2DDEA0(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern float func_001F9D48_2DDEA0(void *, void *) __asm__("FUN_001f9b80");
extern void func_00213DE0_2DDEA0(void *, int, int, int) __asm__("FUN_00212f90");
extern int func_L00_002DB6C8_2DDEA0(void *, void *) __asm__("FUN_L00_002da218");
extern void func_001FA4A0_2DDEA0(void *, void *) __asm__("FUN_001fa2d8");
extern void func_001F9BF0_2DDEA0(void *, void *, void *) __asm__("FUN_001f9a28");
extern void func_001F9EE8_2DDEA0(void *, void *, void *) __asm__("FUN_001f9d20");
extern float func_001F9B88_2DDEA0(float) __asm__("FUN_001f99c0");
extern void func_001F9EC0_2DDEA0(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void func_001F9BD8_2DDEA0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void func_001E9768_2DDEA0(void *, void *) __asm__("FUN_001e93e8");
extern void func_L00_001FF4B0_2DDEA0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void func_0020EEE8_2DDEA0(void *) __asm__("FUN_0020e098");
extern void func_L00_002DDDE8_2DDEA0(void *, void *, int) __asm__("FUN_L00_002dc938");
extern float func_001F9CB8_2DDEA0(void *) __asm__("FUN_001f9af0");
extern float func_L00_001FF3E0_2DDEA0(void *) __asm__("FUN_L00_001ff430");
extern float func_001F9B50_2DDEA0(float) __asm__("FUN_001f9988");
extern void func_001F9C30_2DDEA0(void *, void *, float) __asm__("FUN_001f9a68");
extern void func_L00_0025A8C0_2DDEA0(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern void func_L00_001FF500_2DDEA0(void *, void *, float) __asm__("FUN_001f9c48");
extern int func_L00_001F2BE8_2DDEA0(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int func_L00_001F10E0_2DDEA0(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern float func_L00_0025F368_2DDEA0(float) __asm__("FUN_L00_0025e310");
extern void func_001F9CA0_2DDEA0(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void func_00215380_2DDEA0(void *, void *, float) __asm__("FUN_00214530");
extern void func_001FA588_2DDEA0(void *, void *, void *) __asm__("FUN_001fa3c0");
extern float func_001F9CE8_2DDEA0(void *) __asm__("FUN_001f9b20");
extern int func_0022ED80_2DDEA0(int, int, void *) __asm__("FUN_0022da68");
extern void func_L00_001FF610_2DDEA0(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern int func_001F9908_2DDEA0(void *) __asm__("FUN_001f9740");
extern void func_L00_0026D270_2DDEA0(void *, void *) __asm__("FUN_L00_0026c3d0");
extern int func_001FA898_2DDEA0(float) __asm__("FUN_001fa6d0");
extern int func_L00_00259B08_2DDEA0(void *, void *, float, int, float, int) __asm__("FUN_L00_00258ad0");
extern void func_001E9730_2DDEA0(void *) __asm__("FUN_001e93b0");

int FUN_L00_002dc9f0(struct Moby *moby, char *vars);
int FUN_L00_002dc9f0(struct Moby *moby, char *vars) {
    enum {
        SUCK_UPDATE_ALIGNING_FOR_SUCTION = 0,
        SUCK_UPDATE_RISING_FOR_SUCTION = 1,
        SUCK_UPDATE_BEING_PULLED_IN = 2,
        SUCK_UPDATE_STORED = 3,
        SUCK_UPDATE_FLYING_PROJECTILE = 4,
        SUCK_UPDATE_ROLLING_PROJECTILE = 5,
        SUCK_UPDATE_FALLING_AFTER_CANCEL = 6,
        SUCK_UPDATE_DEATH = 7
    };
    Frame_2DDEA0 work;
    R_2DDEA0 *record;

    if (moby == 0 || moby->state == 0xFE || moby->state == 0xFD) return 0;
    record = func_L00_002DCD40_2DDEA0(moby);
    if (record != 0) {
        *(u128 *)&record->v30 = *(u128 *)&moby->pos;
        switch ((short)(record->state - 1)) {
        case SUCK_UPDATE_ALIGNING_FOR_SUCTION: {
            float yaw, tolerance;
            tolerance = 0.01f;
            yaw = func_001FA748_2DDEA0(func_L00_001FF860_2DDEA0(record->x - moby->pos.x, record->y - moby->pos.y), 3.1415927f);
            func_L00_002592B0_2DDEA0(moby, vars + 0x44, yaw, frame_scale_sq_macro * 0.19634955f, frame_scale_sq_macro * 0.19634955f, frame_scale * 0.3927f);
            if (func_001FA790_2DDEA0(yaw, tolerance) < moby->rot.z && moby->rot.z < func_001FA748_2DDEA0(yaw, tolerance)) {
                record->state = 2;
                record->f6C = 0.0f;
            }
            func_001F9BC0_2DDEA0(&record->vel);
            break;
        }
        case SUCK_UPDATE_RISING_FOR_SUCTION:
            moby->anim_speed = record->f6C + 1.0f;
            record->f6C = record->f6C + 1.0f / func_001FA888_2DDEA0(func_001F9850_2DDEA0(1));
            if (1.0f < record->f6C) record->f6C = 1.0f;
            moby->pos.z += record->f6C / 100.0f * frame_scale;
            func_00215328_2DDEA0(&record->q20, &moby->rot_row0);
            func_001F9BC0_2DDEA0(&record->vel);
            break;
        case SUCK_UPDATE_BEING_PULLED_IN: {
            float one, distance, half;
            moby->visible = 1;
            moby->flags &= 0xFFFE;
            one = 1.0f;
            if (record->f6C < one) {
                record->f6C = record->f6C + one / func_001FA888_2DDEA0(func_001F9850_2DDEA0(30));
                work.q = 0;
                work.v[0].x = -1.5707964f;
                work.v[0].z = -1.5707964f;
                func_001FA218_2DDEA0(&work.v[11], &work.v[0]);
                func_001FA460_2DDEA0(&work.v[3], &record->owner->rot_row0);
                func_001FA540_2DDEA0(&work.v[7], &work.v[3], &work.v[11]);
                func_002150B0_2DDEA0(&work.v[1], &work.v[7]);
                func_001FA5C8_2DDEA0(&work.v[2], &record->q20, &work.v[1], record->f6C);
                func_001FA6C0_2DDEA0(&work.v[2], &work.v[3]);
                func_001FA480_2DDEA0(&moby->rot_row0, &work.v[3]);
                moby->flags = (moby->flags | 4) & 0x7FFF;
            } else {
                record->f6C = one;
            }
            func_L00_00250800_2DDEA0(record->owner, 0, &work.v[0]);
            half = 0.5f;
            distance = func_001F9D48_2DDEA0(&moby->pos, &work.v[0]);
            moby->scale = ((Cls_2DDEA0 *)moby->pclass)->f24 * (distance / (record->f64 + record->f64) + half);
            record->f98 = record->f98 + frame_scale_sq_macro * 0.11f;
            if (frame_scale * 0.75f < record->f98) record->f98 = frame_scale * 0.75f;
            if (distance < record->f98 * 70.0f) {
                func_00213DE0_2DDEA0(record->owner, 4, 0, func_001F9850_2DDEA0(2));
            }
            if (distance < 3.0f) {
                D_001404FC_2DDEA0[0] = 2;
                moby->collision = 0;
            }
            if (distance < record->f98) {
                ((struct Hero *)D_0013F450_2DDEA0)->unk207C--;
                ((struct Hero *)D_0013F450_2DDEA0)->unk2078++;
                func_L00_002DB6C8_2DDEA0(moby, &work.v[0]);
                ((struct Hero *)D_0013F450_2DDEA0)->unk2050[record->idx]->flags &= 0xEFFF;
            } else {
                func_001FA4A0_2DDEA0(&work.v[5], D_0013FA90_2DDEA0);
                func_001F9BF0_2DDEA0(&work.v[2], &moby->pos, &work.v[0]);
                func_001F9EE8_2DDEA0(&work.v[2], &work.v[2], &work.v[5]);
                if (work.v[2].x < 0.0f) {
                    if (0.0f < work.v[2].y) {
                        { float t = -work.v[2].x; float r; if (t < 2.0f) r = t; else r = 2.0f; work.v[3].y = r; }
                    } else {
                        work.v[3].y = (-2.0f < work.v[2].x) ? work.v[2].x : -2.0f;
                    }
                    work.v[3].x = -work.v[2].x * 0.5f;
                } else {
                    work.v[3].y = work.v[2].y * half;
                    work.v[3].x = (func_001F9B88_2DDEA0(work.v[2].y) < 2.0f) ? func_001F9B88_2DDEA0(work.v[2].y) : 2.0f;
                }
                work.v[3].z = 0.0f;
                func_001F9EC0_2DDEA0(&work.v[3], &work.v[3], D_0013FA90_2DDEA0);
                work.v[3].z = 0.0f;
                qcopy(&work.v[4], &work.v[0]);
                func_001F9BD8_2DDEA0(&work.v[0], &work.v[0], &work.v[3]);
                func_001E9768_2DDEA0(&work.v[4], &work.v[0]);
                func_001F9BF0_2DDEA0(&work.v[1], &work.v[0], &moby->pos);
                func_L00_001FF4B0_2DDEA0(&work.v[1], &work.v[1], record->f98);
                func_001F9BD8_2DDEA0(&moby->pos, &moby->pos, &work.v[1]);
                func_0020EEE8_2DDEA0(moby);
            }
            break;
        }
        case SUCK_UPDATE_STORED:
            moby->flags = (moby->flags | 3) & 0xFFFB;
            moby->visible = 0;
            moby->collision = 0;
            if (moby->anim_flags & 2) {
                if (moby->prev_seq != record->anims[4]) {
                    unsigned char anim = record->anims[4];
                    func_00213DE0_2DDEA0(moby, anim, 0, func_001F9850_2DDEA0(3));
                }
                moby->scale = ((Cls_2DDEA0 *)moby->pclass)->f24 * 0.3f;
                qzero(&moby->rot);
            }
            break;
        case SUCK_UPDATE_FLYING_PROJECTILE:
        case SUCK_UPDATE_ROLLING_PROJECTILE: {
            float spin, one;
            int hit_wall, surface;
            moby->visible = 1;
            moby->flags &= 0xFFFE;
            if (moby->pos.z < 2.0f) {
                func_L00_002DDDE8_2DDEA0(moby, record, 0);
                return 0;
            }
            work.q = *(u128 *)&record->vel;
            moby->scale = moby->scale + ((Cls_2DDEA0 *)moby->pclass)->f24 * (record->f7C * frame_scale);
            if (((Cls_2DDEA0 *)moby->pclass)->f24 < moby->scale) moby->scale = ((Cls_2DDEA0 *)moby->pclass)->f24;
            if (record->target != 0 && record->target->state != 0xFE && record->target->state != 0xFD) {
                int max_life = func_001F9850_2DDEA0(0x12C);
                if (record->i94 < max_life - func_001F9850_2DDEA0(5)) {
                    float speed, quad_a, quad_b, root, root1, root2, time;
                    qcopy(&work.v[3], &record->target->pos);
                    work.v[3].z = work.v[3].z + record->f90;
                    func_001F9BF0_2DDEA0(&work.v[2], &work.v[3], &record->v40);
                    func_001F9BF0_2DDEA0(&work.v[1], &work.v[3], &moby->pos);
                    qcopy(&record->v40, &work.v[3]);
                    speed = func_001F9CB8_2DDEA0(&record->vel);
                    quad_a = speed * speed - func_L00_001FF3E0_2DDEA0(&work.v[2]);
                    quad_b = (work.v[2].x * work.v[1].x + work.v[2].y * work.v[1].y + work.v[2].z * work.v[1].z) * -2.0f;
                    root = func_001F9B50_2DDEA0(quad_b * quad_b - quad_a * 4.0f * -func_L00_001FF3E0_2DDEA0(&work.v[1]));
                    root1 = (-quad_b + root) / (2.0f * quad_a);
                    root2 = (-quad_b - root) / (2.0f * quad_a);
                    if (0.0f < root1 && 0.0f < root2) {
                        time = (root2 < root1) ? root1 : root2;
                    } else if (0.0f < root1) {
                        time = root1;
                    } else if (0.0f < root2) {
                        time = root2;
                    } else {
                        time = -1.0f;
                    }
                    if (0.0f < time) {
                        func_001F9C30_2DDEA0(&work.v[4], &work.v[2], time);
                        func_001F9BD8_2DDEA0(&work.v[4], &work.v[4], &work.v[1]);
                        func_L00_001FF4B0_2DDEA0(&work.v[4], &work.v[4], frame_time_sq_macro * 90.0f);
                        func_001F9BD8_2DDEA0(&record->vel, &record->vel, &work.v[4]);
                        func_L00_001FF4B0_2DDEA0(&record->vel, &record->vel, speed);
                    } else {
                        func_L00_001FF4B0_2DDEA0(&work.v[1], &work.v[1], frame_time_sq_macro * 90.0f);
                        func_001F9BD8_2DDEA0(&record->vel, &record->vel, &work.v[1]);
                        func_L00_001FF4B0_2DDEA0(&record->vel, &record->vel, speed);
                    }
                }
            }
            one = 1.0f;
            func_001F9BD8_2DDEA0(&moby->pos, &moby->pos, &record->vel);
            func_L00_0025A8C0_2DDEA0(&work.v[1], moby, 0x430000, 2.0f, &record->vel);
            func_L00_001FF500_2DDEA0(&work.v[1], &work.v[1], one);
            work.h.hit.v.w = 5627.925f;
            work.h.hit.b18 = 1;
            work.h.hit.b19 = 3;
            work.h.hit.h1A = 0x351;
            work.h.hit.v.z = one;
            hit_wall = func_L00_001F2BE8_2DDEA0(record->f80, &moby->pos, 1, moby, &work.v[1]);
            hit_wall |= func_L00_001F10E0_2DDEA0(record->f80, &moby->pos, 0, moby);
            if (hit_wall != 0) {
                qcopy_nc(&moby->pos, &((Coll_2DDEA0 *)D_L00_00173E40_lui)->v30);
                func_001F9BF0_2DDEA0(&record->v50, &((Coll_2DDEA0 *)D_L00_00173E40_lui)->v20, &moby->pos);
            }
            if (((Coll_2DDEA0 *)D_L00_00173E40_lui)->hit != 0 && ((Coll_2DDEA0 *)D_L00_00173E40_lui)->hit->oclass == 0) hit_wall = 0;
            if (record->state == 6) {
                moby->flags &= 0x7FFF;
                moby->collision = ((Cls_2DDEA0 *)moby->pclass)->i10;
                moby->flags |= 4;
                record->v50.z = one;
                record->v50.x = 0.0f;
                record->v50.y = 0.0f;
                record->v50.w = 0.01f;
                spin = func_001F9CB8_2DDEA0(&work.v[0]) / record->f80;
                func_00215328_2DDEA0(&work.v[1], &moby->rot_row0);
                func_L00_001FF4B0_2DDEA0(&record->v50, &record->v50, one);
                func_L00_0025F368_2DDEA0(spin);
                func_L00_001FF4B0_2DDEA0(&work.v[4], &record->vel, -1.0f);
                func_001F9CA0_2DDEA0(&work.v[3], &work.v[4], &record->v50);
                func_L00_001FF4B0_2DDEA0(&work.v[3], &work.v[3], one);
                func_00215380_2DDEA0(&work.v[2], &work.v[3], spin);
                func_001FA588_2DDEA0(&work.v[5], &work.v[1], &work.v[2]);
                func_001FA6C0_2DDEA0(&work.v[5], &work.v[6]);
                func_L00_001FF4B0_2DDEA0(&moby->rot_row0, &work.v[6], one);
                func_L00_001FF4B0_2DDEA0(&moby->rot_row1, &work.v[7], one);
                func_L00_001FF4B0_2DDEA0(&moby->rot_row2, &work.v[8], one);
            }
            if (hit_wall != 0) {
                surface = 0;
                if (((Coll_2DDEA0 *)D_L00_00173E40_lui)->hit != 0 && ((Cls_2DDEA0 *)((Coll_2DDEA0 *)D_L00_00173E40_lui)->hit->pclass) != 0) {
                    surface = ((Cls_2DDEA0 *)((Coll_2DDEA0 *)D_L00_00173E40_lui)->hit->pclass)->h46;
                }
                func_L00_001FF4B0_2DDEA0(&((Coll_2DDEA0 *)D_L00_00173E40_lui)->v40, &((Coll_2DDEA0 *)D_L00_00173E40_lui)->v40, 1.0f);
                if (moby->prev_seq != record->anims[6]) {
                    unsigned char anim = record->anims[6];
                    func_00213DE0_2DDEA0(moby, anim, 0, func_001F9850_2DDEA0(0x14));
                }
                moby->collision_disable = record->i88;
                record->state = 6;
                if (0.87266463f < func_L00_001FF860_2DDEA0(((Coll_2DDEA0 *)D_L00_00173E40_lui)->v40.z, func_001F9CE8_2DDEA0(&((Coll_2DDEA0 *)D_L00_00173E40_lui)->v40))) {
                    if (record->h9E >= 0) func_0022ED80_2DDEA0(record->h9E, 0, moby);
                    func_L00_002DDDE8_2DDEA0(moby, record, 1);
                    return 0;
                }
                if (D_0013E629_2DDEA0[0] != 0 && surface >= 5) {
                    if (record->h9E >= 0) func_0022ED80_2DDEA0(record->h9E, 0, moby);
                    func_L00_002DDDE8_2DDEA0(moby, record, 1);
                }
                qzero(&work.v[1]);
                work.v[1].z = ((Coll_2DDEA0 *)D_L00_00173E40_lui)->v40.z;
                func_L00_001FF610_2DDEA0(&record->vel, &record->vel, &work.v[1]);
            }
            if (func_001F9908_2DDEA0(&record->i94) != 0) {
                func_L00_002DDDE8_2DDEA0(moby, record, 1);
                return 0;
            }
            if (60.0f < func_001F9D48_2DDEA0(&moby->pos, D_0013F4D0_2DDEA0)) {
                func_L00_002DDDE8_2DDEA0(moby, record, 1);
                return 0;
            }
            if (func_001F9CB8_2DDEA0(&record->vel) < frame_scale * 0.01f) {
                func_L00_002DDDE8_2DDEA0(moby, record, 1);
                return 0;
            }
            record->vel.z = record->vel.z - frame_scale_sq_macro * 0.00925f;
            func_L00_0026D270_2DDEA0(&moby->pos, &moby->rot_row0);
            break;
        }
        case SUCK_UPDATE_FALLING_AFTER_CANCEL: {
            int landed;
            moby->visible = 1;
            moby->flags &= 0xFFFE;
            record->vel.z = record->vel.z - frame_scale_sq_macro * 0.013f;
            if (moby->pos.z < 2.0f) {
                func_L00_002DDDE8_2DDEA0(moby, record, 1);
                return 0;
            }
            landed = func_L00_00259B08_2DDEA0(moby, &record->vel, 0.8f, func_001FA898_2DDEA0(record->f8C * 1024.0f), 0.0f, 0);
            moby->rot.x = moby->rot.x / 1.5f;
            moby->rot.y = moby->rot.y / 1.5f;
            if (landed != 0) {
                moby->rot.x = 0.0f;
                moby->rot.y = 0.0f;
                moby->anim_speed = 1.0f;
                return 1;
            }
            if (moby->anim_flags & 2) {
                moby->anim_speed = record->f78;
            }
            break;
        }
        case SUCK_UPDATE_DEATH:
            func_001E9730_2DDEA0(D_L00_001EA4B0_2DDEA0);
            break;
        }
    }
    if (moby->flags & 4) func_0020EEE8_2DDEA0(moby);
    if (record->state < 4) {
        if (record->timer <= 0.0f) {
            if (((Cls_2DDEA0 *)moby->pclass)->f2C->fC != 0) ((Cls_2DDEA0 *)moby->pclass)->f2C->fC(moby);
        }
        record->timer = record->timer - 1.0f;
    }
    return 0;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DED98), where it is exact; names translated to the US level program. */

typedef struct {
    float q[4][4];
    unsigned int col[4];
    float x0, y0, x1, y1, x2, y2, x3, y3;
    long word0, word1, word2, word3;
} Pkt;

extern char g_debug_camera[] __asm__("D_L00_00166DC0") __attribute__((section(".data")));
extern char D_L00_001E5EE0[] __attribute__((section(".data")));
extern char D_L00_001E6020[] __attribute__((section(".data")));
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern float D_L00_00161B48 __attribute__((sda));
extern float D_L00_00161B4C __attribute__((sda));
extern float D_L00_00161B50 __attribute__((sda));
extern short D_L00_001E6008[] __attribute__((section(".data")));
extern void fast_draw_quad_real(void *, void *, int);
extern void subtract_vector_xyz_c4(void *, void *, void *) __asm__("FUN_001f9a28");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");

void FUN_L00_002dd8e8(void) {
    Pkt s;
    float vc[4];
    float vd[4];
    float ve[4];
    float vf[4];
    float a, a2, am, b, b2, dot;
    float step;
    short *pv;
    int i, j, k, v;
    unsigned short y;
    s.word1 = get_effect_texture(0x15);
    s.word2 = 0x0000FF9000000260L;
    s.word3 = 0x8000000048L;
    s.word0 = 0;
    a = D_L00_00161B48;
    D_L00_00161B48 = a + 0.001f;
    if (D_L00_00161B48 > 7.0f) D_L00_00161B48 -= 7.0f;
    step = 0.3f;
    a += step;
    for (i = 1; i < 10; i++) {
        b = D_L00_00161B4C;
        D_L00_00161B4C = b + D_L00_00161B50;
        if (D_L00_00161B4C > 7.0f) D_L00_00161B4C -= 7.0f;
        a2 = a + step;
        for (j = 0; j < 20; j++) {
            qcopy(s.q[0], D_L00_001E6020 + (j * 16 + i * 0x140));
            qcopy(s.q[1], D_L00_001E6020 + ((j + 1) % 20 * 16 + i * 0x140));
            qcopy(s.q[2], D_L00_001E5EE0 + (j * 16 + i * 0x140));
            qcopy(s.q[3], D_L00_001E5EE0 + ((j + 1) % 20 * 16 + i * 0x140));
            subtract_vector_xyz_c4(vc, s.q[0], s.q[1]);
            subtract_vector_xyz_c4(vd, s.q[2], s.q[1]);
            cross_vectors_xyz(ve, vd, vc);
            subtract_vector_xyz_c4(vf, s.q[1], g_debug_camera);
            dot = fast_vec_dot(ve, vf);
            if (0.0f <= dot) {
                b += 0.05f;
            } else {
                b2 = b + 0.05f;
                am = a - step;
                y = D_L00_001E6008[i - 1];
                pv = &D_L00_001E6008[i];
                v = *pv;
                for (k = 0; k < 2; k++) {
                    if (v < 0) v = 0;
                    if (v > 0x40) v = 0x40;
                    v = (v << 24) | 0x404040;
                    s.col[k * 2] = v;
                    s.col[k * 2 + 1] = v;
                    v = (short)y;
                }
                if (i == 1) {
                    s.col[3] = 0x20404040;
                    s.col[2] = 0x20404040;
                } else if (i == 9) {
                    s.col[1] = 0x404040;
                    s.col[0] = 0x404040;
                }
                s.x0 = a;
                s.y0 = b;
                s.x1 = a;
                s.y1 = b2;
                s.x2 = am;
                s.y2 = b;
                s.x3 = am;
                s.y3 = b2;
                b = b2;
                fast_draw_quad_real(&s, 0, 0);
            }
        }
        a = a2;
    }
    vu1_add_g_sregister(0x47, 0x5360B);
}
extern int D_L00_00161B44_gp __asm__("D_L00_00161B44") __attribute__((sda));
extern int D_L00_00161B40_gp __asm__("D_L00_00161B40") __attribute__((sda));
extern short D_L00_001E6008[] __attribute__((section(".data")));
/* Sets D_L00_00161B40_gp and D_L00_00161B44_gp to 1 and clears D_L00_001E6008. */
void FUN_L00_002ddca0(void) {
    D_L00_00161B40_gp = 1;
    D_L00_00161B44_gp = 1;
    (*(short *)D_L00_001E6008) = 0;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002DF168), where it is exact; names translated to the US level program. */

typedef struct { char pad[0x2084]; int f; int pad2; int mode; } G_c;

typedef int u128_DF168 __attribute__((mode(TI)));

typedef struct { int pad[5]; int f14; } E04_DF168;

typedef struct { short idx; short angle; float f; float phase; float phase_step; float speed; char *p; } E04;

extern E04 D_L00_001E4C80[];
extern char D_L00_00161B60[] __attribute__((section(".sdata")));
extern char D_L00_00161B70[] __attribute__((section(".sdata")));
extern char D_L00_00166FD0[];
extern char D_L00_001E5F30[];
extern char D_L00_001E5F40[];
extern f32 random_angle_radians(void) __asm__("func_00213308");
extern float D_L00_00161B80 __attribute__((section(".sdata")));
extern float D_L00_00161B84 __attribute__((section(".sdata")));
extern int D_L00_00161B40 __attribute__((sda));
extern int D_L00_00161B44 __attribute__((sda));
extern int D_L00_00161B54 __attribute__((sda));
extern int D_L00_00161B58 __attribute__((sda));
extern short D_L00_001E6008[];
extern void build_look_at_matrix_c2(void *, void *, float, void *) __asm__("build_look_at_matrix");
extern void clear_u64_value_c2(void *) __asm__("func_001F99F8");
extern void fast_vec_normalize_c2(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void fast_vec_scale_c2(void *, void *, f32) __asm__("func_001F9A68");

void FUN_L00_002ddcb8(char *a, char *b) {
    u128_DF168 s[7];
    char *v;
    short *w;
    int i, st, j;
    if (D_L00_00161B40 == 0) return;
    D_L00_00161B84 = 1.7f;
    D_L00_00161B40 = 0;
    st = hero.state.current;
    if ((st == 0x1E || st == 1) && (hero.moby->flags & 1)) {
        qcopy((char *)s, D_L00_00166FD0);
        qcopy((char *)s + 0x10, D_L00_00166FD0 + 0x10);
        qcopy((char *)s + 0x20, D_L00_00166FD0 + 0x20);
        v = (char *)s + 0x20;
    } else {
        if (hero.unk22C8 == 1) {
            fast_vec_scale_c2(s, a + 0xD0, -1.0f);
            qcopy((char *)s + 0x10, a + 0xC0);
            qcopy((char *)s + 0x20, a + 0xE0);
            v = (char *)s + 0x20;
        } else {
            qcopy((char *)s, &hero.basis[0]);
            qcopy((char *)s + 0x10, &hero.basis[1]);
            qcopy((char *)s + 0x20, &hero.basis[2]);
            v = (char *)s + 0x20;
        }
    }
    fast_vec_normalize_c2((char *)s + 0x30, s, D_L00_00161B84);
    fast_vec_normalize_c2((char *)s + 0x40, v, 1.0f);
    qcopy(D_L00_001E5F40, b);
    D_L00_001E6008[0] = 0;
    for (i = 1; i < 10; i++) {
        add_vector_xyz(D_L00_001E5F40 + i * 0x10, D_L00_001E5F30 + i * 0x10, (char *)s + 0x30);
        D_L00_001E6008[i] = 0;
    }
    clear_u64_value_c2(D_L00_00161B60);
    s[5] = 0;
    s[6] = 0;
    *(float *)((char *)s + 0x58) = 0.1f;
    *(float *)((char *)s + 0x5C) = 1.0f;
    *(float *)((char *)s + 0x60) = 1.0f;
    *(float *)((char *)s + 0x6C) = 1.0f;
    {
        float f = random_angle_radians();
        build_look_at_matrix_c2(D_L00_00161B70, (char *)s + 0x50, f, (char *)s + 0x60);
        D_L00_00161B80 = f;
    }
    D_L00_00161B50 = 0.006f;
    D_L00_00161B54 = 0;
    D_L00_00161B58 = 0;
    if (D_L00_00161B44 != 0) {
        D_L00_00161B44 = 0;
        for (j = 199; j >= 0; j--) ((E04_DF168 *)D_L00_001E4C80)[j].f14 = 0;
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002ddf60.s", FUN_L00_002ddf60);
#else
/* Form the Suck Cannon's bending suction tube and update its particles. */
/* Ported from rac1-decomp (nonmatching/shared/func_L00_002DF410.c: func_L00_002DF410). */
typedef unsigned int Q41 __attribute__((mode(TI)));

typedef union { struct { float x,y,z,w; } f; Q41 q; } V41 __attribute__((aligned(16)));

typedef struct {
    u8 pad0[0x34]; u16 flags; u8 pad36[0x8A]; V41 basis[3];
} M41;

typedef struct {
    u8 pad0[0x640]; V41 matrix[3]; u8 pad670[0x1A10];
    M41 *moby; int camera_mode; u8 pad2088[0x240]; s16 special;
} Hero41;

typedef struct {
    float unknown0; int mode; u8 pad8[2]; u8 r,g; u8 padC[4];
    V41 velocity; float acceleration;
} Tail41;

typedef struct Particle41 {
    u8 pad0[0xA]; s16 life; float size; V41 pos; Tail41 tail;
} Particle41;



extern V41 D_L00_001E4B40[];
extern E04 D_L00_001E4C80_lui[] __asm__("D_L00_001E4C80");
extern char D_0013E533[] NOT_SDA;
extern char D_L00_00161B60[] MACRO_ADDR;
extern int D_L00_00161B54 __attribute__((sda));
extern int D_L00_00161B58_gp __asm__("D_L00_00161B58") __attribute__((sda));
extern char D_L00_00161B70[] MACRO_ADDR;
extern char D_L00_00166FD0[];
extern char D_L00_00173E60[];
extern char D_L00_001E5EE0[];
extern char D_L00_001E5F30[];
extern char D_L00_001E5F40[];
extern char D_L00_001E6020[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_arc_sin(f32) __asm__("func_001F9DF8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 fast_vec_dot(void *a, void *b) __asm__("func_001F9AB0");
extern f32 fast_vec_length(void *) __asm__("func_001F9AF0");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern f32 vector_distance(void *, s32 *) __asm__("FUN_001f9b80");
extern float D_0015ED60 MACRO_ADDR;
extern float D_L00_00161B84;
extern float D_L00_00161B80 MACRO_ADDR;
extern float D_L00_001E5FE0[];
extern float FUN_L00_001eb328(void *,float,float,float,float,float);
extern float fast_add_rotations(float, float) __asm__("func_001FA580");
extern int collision_line_ddf60(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern short D_L00_001E6008[];
extern void FUN_L00_0025b6b8(void *,void *,void *,float);
extern void FUN_L00_00267a08(void *);
extern void FUN_L00_002dd8e8(void);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void enqueue_callback_list_1_alt(void (*)(void),int) __asm__("FUN_001f4600");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void fast_vec_scale(void *, void *, f32) __asm__("func_001F9A68");
extern void lerp_vector(void *, void *, void *, f32) __asm__("func_001F9A40");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
float absolute_float(float input) __asm__("func_001F99C0");
s32 scale_frames_ddf60(s32) __asm__("FUN_001f96f8");
void FUN_L00_002ddcb8(char *a, char *b);
extern Particle41 *new_particle41(void *,float,float,float,int,void *,float,int) __asm__("FUN_L00_0026d000");

void FUN_L00_002ddf60(void *moby,void *position) {
    V41 step,up,orient[3],ray_end,start,end,axis,tmp_a,tmp_b,
        frame[3],tmp_c,motion,tmp_d,tmp_e,interp,cross_c,basis_c,tmp_f,unused_a,unused_b;
    V41 *orientation;
    int i,j,n,segment,next_ring;
    float t,angle,turn,delta,dot,length,radius,phase,amount,speed;
    Particle41 *p;
    V41 *anchor,*previous,*source;


    FUN_L00_002ddcb8(moby,position);
    if((((Hero41 *)&hero)->camera_mode==30 || ((Hero41 *)&hero)->camera_mode==1) && (((Hero41 *)&hero)->moby->flags&1)) {
        qcopy(&orient[0],D_L00_00166FD0);
        qcopy(&orient[1],D_L00_00166FD0+16);
        qcopy(&orient[2],D_L00_00166FD0+32);
        orientation=&orient[0];
    } else if(((Hero41 *)&hero)->special==1) {
        fast_vec_scale(&orient[0],&((M41 *)moby)->basis[1],-1.0f);
        { M41 *m=moby;
        qcopy_nc(&orient[1],&m->basis[0]);
        qcopy_nc(&orient[2],&m->basis[2]); }
        orientation=&orient[0];
    } else {
        qcopy(&orient[0],&((Hero41 *)&hero)->matrix[0]);
        qcopy(&orient[1],&((Hero41 *)&hero)->matrix[1]);
        qcopy(&orient[2],&((Hero41 *)&hero)->matrix[2]);
        orientation=&orient[0];
    }
    fast_vec_normalize(&step,orientation,D_L00_00161B84);
    fast_vec_normalize(&up,&orient[2],1.0f);
    qcopy(D_L00_001E5F40,position);
    fast_vec_normalize(&ray_end,orientation,D_L00_00161B84*10.0f);
    add_vector_xyz(&ray_end,&ray_end,position);
    if(collision_line_ddf60(position,&ray_end,20,(int)((Hero41 *)&hero)->moby,0)) {
        int n,i,k;
        n=truncate_float_to_s32(vector_distance(D_L00_00173E60,position)/D_L00_00161B84);
        for(i=n;i<10;i++) {
            if(D_L00_001E6008[i]>64)D_L00_001E6008[i]=64;
            D_L00_001E6008[i]=(u16)D_L00_001E6008[i]-4;
            if(D_L00_001E6008[i]<0)D_L00_001E6008[i]=0;
        }
        for(k=n-1;k>=0;k--)D_L00_001E6008[k]=D_L00_001E6008[k+1]+32;
    } else {
        int i;
        D_L00_001E6008[0]=(u16)D_L00_001E6008[0]+4;
        for(i=1;i<10;i++)D_L00_001E6008[i]=D_L00_001E6008[i-1]-32;
    }
    transform_vector_by_basis(&start,&(*(V41 *)D_L00_00161B60),orientation);
    add_vector_xyz(&start,&start,&step);
    transform_vector_by_basis(&end,&(*(V41 *)D_L00_00161B70),orientation);
    add_vector_xyz(&end,&end,&step);
    t=(float)D_L00_00161B54/convert_integer_to_float(scale_frames_ddf60(60));
    D_L00_00161B54++;
    step.f.x=start.f.x+(end.f.x-start.f.x)*t;
    step.f.y=start.f.y+(end.f.y-start.f.y)*t;
    step.f.z=start.f.z+(end.f.z-start.f.z)*t;
    if(D_L00_00161B54>=scale_frames_ddf60(60)) {
        qcopy(&(*(V41 *)D_L00_00161B60),&(*(V41 *)D_L00_00161B70));
        fast_vec_normalize(&axis,orientation,D_L00_00161B84);
        tmp_a.q=0; tmp_b.q=0;
        tmp_a.f.z=0.1f; tmp_a.f.w=1.0f;
        tmp_b.f.x=1.0f; tmp_b.f.w=1.0f;
        angle=fast_add_rotations(D_L00_00161B80,random_float_between(45.0f,315.0f)*0.0174532924f);
        func_002156E0_2D9EA0(&(*(V41 *)D_L00_00161B70),&tmp_a,angle,&tmp_b);
        D_L00_00161B80=angle;
        D_L00_00161B54=0;
    }
    (*(float *)&D_L00_00161B58_gp)=fast_add_rotations((*(float *)&D_L00_00161B58_gp),0.1f);
    qcopy(&axis,&orient[2]);
    qcopy(&frame[2],&axis);
    cross_vectors_xyz(&frame[1],&step,&frame[2]);
    fast_vec_normalize(&frame[1],&frame[1],1.0f);
    cross_vectors_xyz(&frame[0],&frame[2],&frame[1]);
    { int k;
    for(k=0;k<20;k++) {
        fast_vec_scale(&tmp_a,&D_L00_001E4B40[k],0.1f);
        transform_vector_by_basis(&tmp_a,&tmp_a,frame);
        add_vector_xyz(&((V41 *)D_L00_001E6020)[k],D_L00_001E5F40,&tmp_a);
    }
    }
    { int i;
    for(i=1;i<10;i++) {
        anchor=(V41 *)(D_L00_001E5F40+i*16);
        previous=(V41 *)(D_L00_001E5F30+i*16);
        subtract_vector_xyz(&tmp_a,anchor,previous);
        qcopy(&tmp_b,&step);
        cross_vectors_xyz(&tmp_c,&tmp_b,&tmp_a);
        dot=fast_vec_dot(&tmp_a,&tmp_b);
        { float target=1.5707964f-fast_arc_sin(dot/(fast_vec_length(&tmp_a)*D_L00_00161B84));
        turn=FUN_L00_001eb328(&D_L00_001E5FE0[i],0.0f,target,0.06f,0.2f,0.0f);
        delta=fast_subtract_rotations(target,turn); }
        if(absolute_float(delta)>0.31415927f)turn=fast_add_rotations(turn,fast_subtract_rotations(delta,0.31415927f));
        func_002156E0_2D9EA0(&motion,&tmp_a,turn,&tmp_c);
        fast_vec_normalize(&tmp_d,&motion,D_L00_00161B84);
        lerp_vector(&motion,&motion,&tmp_d,1.0f);
        { float len=fast_vec_length(&motion);
        if(len>2.0f)fast_vec_scale(&motion,&motion,2.0f/len); }
        if(i>=6)lerp_vector(&motion,&motion,&tmp_a,(float)i/convert_integer_to_float(10)+0.0f);
        add_vector_xyz(anchor,previous,&motion);
        qcopy_nc(&tmp_e,&step);
        qcopy_nc(&basis_c,&axis);
        fast_vec_normalize(&interp,&tmp_e,1.0f);
        cross_vectors_xyz(&cross_c,&interp,&basis_c);
        fast_vec_normalize(&cross_c,&cross_c,1.0f);
        cross_vectors_xyz(&basis_c,&cross_c,&interp);
        qcopy(&axis,&basis_c);
        radius=(float)i*0.3f;
        phase=(float)i*0.7f;
        { V41 *dst=((V41 *)D_L00_001E6020)+i*20; unsigned int j;
        for(j=0;j<20;j++) {
            angle=radius+0.1f;
            angle+=fast_sin(fast_add_rotations((*(float *)&D_L00_00161B58_gp)+phase,0.0f))*0.1f;
            fast_vec_scale(&tmp_f,(char *)D_L00_001E4B40+j*16,angle);
            transform_vector_by_basis(&tmp_f,&tmp_f,&interp);
            add_vector_xyz(dst+j,D_L00_001E5F40+i*16,&tmp_f);
        }
        }
        fast_vec_normalize(&step,&motion,D_L00_00161B84);
    }
    }
    enqueue_callback_list_1_alt(FUN_L00_002dd8e8,(int)moby);
    { int count,sparks;
    count=0; sparks=0;
    for(i=0;i<200;i++) {
        if(!D_L00_001E4C80_lui[i].p && count<5) {
            for(segment=9;segment>=0 && D_L00_001E6008[segment]<64;segment--);
            if(segment>0) {
                if(segment>=2 && sparks<2) {
                    { int k=random_integer_below(20);
                    amount=random_float_between(0.2f,1.0f);
                    k=(int)&((V41 *)D_L00_001E6020)[k];
                    lerp_vector(&tmp_a,(V41 *)k,(V41 *)k+segment*20,amount); }
                    lerp_vector(&tmp_c,D_L00_001E5F40,D_L00_001E5F40+segment*16,amount);
                    speed=random_float_between(60000.0f,180000.0f);
                    p=new_particle41(&tmp_a,0.0f,1.0f,1.03f,random_integer_below(5)+1,&tmp_b,speed,0x404040);
                    if(p) {
                        Tail41 *tail=&p->tail;
                        subtract_vector_xyz(&motion,&tmp_a,&tmp_c);
                        sparks++;
                        fast_vec_normalize(&tail->velocity,&motion,0.01f);
                        tail->velocity.f.z+=0.02f;
                    }
                }
                qcopy(&tmp_a,D_L00_001E5F40+segment*16);
                clear_u64_value(&tmp_b);
                speed=random_float_between(60000.0f,180000.0f);
                { Particle41 *main=new_particle41(&tmp_a,0.0f,1.0f,1.0f,6,&tmp_b,speed,0x7F404040);
                D_L00_001E4C80_lui[i].p=(char *)main;
                if(main) { Tail41 *tail=&main->tail; main->life=200; tail->mode=2; count++; tail->r=127; tail->g=200; } }
                D_L00_001E4C80_lui[i].idx=segment;
                D_L00_001E4C80_lui[i].f=0;
                D_L00_001E4C80_lui[i].phase=0;
                D_L00_001E4C80_lui[i].angle=random_integer_below(20);
                D_L00_001E4C80_lui[i].phase_step=random_float_between(0.1f,0.8f);
                D_L00_001E4C80_lui[i].speed=random_float_between(0.1f,0.4f);
            }
        }
        if(D_L00_001E4C80_lui[i].p) {

            subtract_vector_xyz(&tmp_a,D_L00_001E5F30+D_L00_001E4C80_lui[i].idx*16,D_L00_001E5F40+D_L00_001E4C80_lui[i].idx*16);
            length=fast_vec_length(&tmp_a);
            D_L00_001E4C80_lui[i].f+=D_L00_001E4C80_lui[i].speed;
            ((Particle41 *)D_L00_001E4C80_lui[i].p)->life=200;
            if(length<D_L00_001E4C80_lui[i].f) {
                D_L00_001E4C80_lui[i].idx=(u16)D_L00_001E4C80_lui[i].idx-1;
                if(D_L00_001E4C80_lui[i].idx<=0) { FUN_L00_00267a08(D_L00_001E4C80_lui[i].p); D_L00_001E4C80_lui[i].p=0; }
                else {
                    D_L00_001E4C80_lui[i].f-=length;
                    subtract_vector_xyz(&tmp_a,D_L00_001E5F30+D_L00_001E4C80_lui[i].idx*16,D_L00_001E5F40+D_L00_001E4C80_lui[i].idx*16);
                    length=fast_vec_length(&tmp_a);
                }
            }
                if(D_L00_001E4C80_lui[i].p) {

                amount=D_L00_001E4C80_lui[i].f/length;
                if(D_L00_001E4C80_lui[i].idx==2)((Particle41 *)D_L00_001E4C80_lui[i].p)->life=100;
                else if(D_L00_001E4C80_lui[i].idx<2) {
                    ((Particle41 *)D_L00_001E4C80_lui[i].p)->life=truncate_float_to_s32((1.0f-amount)*100.0f);
                    if(((Particle41 *)D_L00_001E4C80_lui[i].p)->life<61)((Particle41 *)D_L00_001E4C80_lui[i].p)->life=60;
                }

                if(D_L00_001E4C80_lui[i].idx==1) {
                    subtract_vector_xyz(&tmp_b,position,&((Particle41 *)D_L00_001E4C80_lui[i].p)->pos);
                    fast_vec_normalize(&tmp_b,&tmp_b,D_L00_001E4C80_lui[i].speed);
                    add_vector_xyz(&((Particle41 *)D_L00_001E4C80_lui[i].p)->pos,&((Particle41 *)D_L00_001E4C80_lui[i].p)->pos,&tmp_b);
                    ((Particle41 *)D_L00_001E4C80_lui[i].p)->size*=1.0f+D_0015ED60*-0.0500000119f;
                } else {
                    fast_vec_scale(&tmp_a,&tmp_a,amount);
                    add_vector_xyz(&((Particle41 *)D_L00_001E4C80_lui[i].p)->pos,D_L00_001E5F40+D_L00_001E4C80_lui[i].idx*16,&tmp_a);
                    j=D_L00_001E4C80_lui[i].angle;
                    subtract_vector_xyz(&tmp_b,(char *)((V41 *)D_L00_001E6020)+D_L00_001E4C80_lui[i].idx*320+j*16,D_L00_001E5F40+D_L00_001E4C80_lui[i].idx*16);
                    subtract_vector_xyz(&tmp_c,(char *)((V41 *)D_L00_001E5EE0)+D_L00_001E4C80_lui[i].idx*320+j*16,D_L00_001E5F30+D_L00_001E4C80_lui[i].idx*16);
                    FUN_L00_0025b6b8(&motion,&tmp_b,&tmp_c,amount);
                    j=(j+19)%20;
                    subtract_vector_xyz(&tmp_d,(char *)((V41 *)D_L00_001E6020)+D_L00_001E4C80_lui[i].idx*320+j*16,D_L00_001E5F40+D_L00_001E4C80_lui[i].idx*16);
                    subtract_vector_xyz(&tmp_e,(char *)((V41 *)D_L00_001E5EE0)+D_L00_001E4C80_lui[i].idx*320+j*16,D_L00_001E5F30+D_L00_001E4C80_lui[i].idx*16);
                    FUN_L00_0025b6b8(&interp,&tmp_d,&tmp_e,amount);
                    FUN_L00_0025b6b8(&motion,&motion,&interp,D_L00_001E4C80_lui[i].phase);
                    D_L00_001E4C80_lui[i].phase+=D_L00_001E4C80_lui[i].phase_step;
                    if(D_L00_001E4C80_lui[i].phase>=1.0f) { D_L00_001E4C80_lui[i].phase-=1.0f; D_L00_001E4C80_lui[i].angle=j; }
                    add_vector_xyz(&((Particle41 *)D_L00_001E4C80_lui[i].p)->pos,&((Particle41 *)D_L00_001E4C80_lui[i].p)->pos,&motion);
                }
            }
        }
    }
}
}
#endif
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002E0490), where it is exact; names translated to the US level program. */

extern short D_L00_001E6008[];
extern void FUN_L00_002dd8e8_u(void) __asm__("FUN_L00_002dd8e8");
void enqueue_callback_list_1(s32 arg0, s32 arg1) __asm__("FUN_001f4600");

/* When D_L00_001E6008[0] > 0: caps the ten counters at 0x40, lowers each by 4, queues 002dd8e8. */
void FUN_L00_002defe0(int arg0) {
    int i;
    if (D_L00_001E6008[0] > 0) {
        for (i = 0; i < 10; i++) {
            if (D_L00_001E6008[i] > 0x40)
                D_L00_001E6008[i] = 0x40;
            D_L00_001E6008[i] = (unsigned short)D_L00_001E6008[i] - 4;
        }
        enqueue_callback_list_1(FUN_L00_002dd8e8_u, arg0);
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* pull each active entry's marker toward its target, then update the rest */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002E04F8), where it is exact; names translated to the US level program. */


typedef float VQ[4] __attribute__((aligned(16)));

E04 D_L00_001E4C80[200] = {0};
E04 D_L01_001FF5E0[200] = {0};
E04 D_L02_001D7750[200] = {0};
E04 D_L03_001D6550[200] = {0};
E04 D_L04_001D7390[200] = {0};
E04 D_L05_0020BEC0[200] = {0};
E04 D_L06_001D8E40[200] = {0};
E04 D_L07_001DAE00[200] = {0};
E04 D_L08_001ED720[200] = {0};
E04 D_L09_00204100[200] = {0};
E04 D_L10_001D7F00[200] = {0};
E04 D_L11_001D7AD0[200] = {0};
E04 D_L12_001F94A0[200] = {0};
E04 D_L13_001D7B00[200] = {0};
E04 D_L14_001E3FB0[200] = {0};
E04 D_L15_001D74D0[200] = {0};
E04 D_L16_001D69F0[200] = {0};
E04 D_L17_001D77F0[200] = {0};
E04 D_L18_001D7B30[200] = {0};
extern char D_L00_001E5F30[];
extern char D_L00_001E5F40[];
extern float vector_length_xyz(void *);
extern s32 D_L00_00161B40_gp __asm__("D_L00_00161B40") __attribute__((sda));
extern s32 D_L00_00161B44_gp __asm__("D_L00_00161B44") __attribute__((sda));
extern f32 D_L00_00161B50_gp __asm__("D_L00_00161B50") __attribute__((sda));
extern void FUN_L00_002defe0(int);
extern void add_vector_xyz(void *, void *, void *);
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *, void *, void *);
void normalize_vector_xyz(void *, void *, f32);

void FUN_L00_002df048(int callback_context) {
    VQ a, b, c;
    int i;
    float len;
    char *p;

    *(int *)&D_L00_00161B40_gp = 1;
    *(float *)&D_L00_00161B50_gp = *(float *)&D_L00_00161B50_gp * 0.5f;
    *(int *)&D_L00_00161B44_gp = 1;
    for (i = 0; i < 200; i++) {
        p = D_L00_001E4C80[i].p;
        if (p != 0) {
            D_L00_001E4C80[i].p = 0;
            subtract_vector_xyz(a, D_L00_001E5F30 + D_L00_001E4C80[i].idx * 16,
                                D_L00_001E5F40 + D_L00_001E4C80[i].idx * 16);
            len = vector_length_xyz(a);
            scale_vector_xyz(a, a, D_L00_001E4C80[i].f / len);
            add_vector_xyz(b, D_L00_001E5F40 + D_L00_001E4C80[i].idx * 16, a);
            subtract_vector_xyz(c, p + 0x10, b);
            normalize_vector_xyz(c, c, 0.03f);
            scale_vector_xyz(a, a, 0.1f / len);
            add_vector_xyz(p + 0x30, c, a);
            *(short *)(p + 0xA) = 0x46;
        }
    }
    FUN_L00_002defe0(callback_context);
}
typedef struct {
    u8 pad[0x46];
    s16 h46;
} T_2df1e0;
typedef struct {
    u8 pad[0x10];
    u8 v10[0x10];
    u8 b20;
    u8 pad2[3];
    T_2df1e0 *p24;
} O_2df1e0;
extern short D_L00_001E6008[] __attribute__((section(".data")));
extern u8 D_L00_001E5F40_2df1e0[][16] __asm__("D_L00_001E5F40") __attribute__((section(".data")));
s32 random_integer_below(s32) __asm__("FUN_00213260");
s32 FUN_L00_0025e3f8(O_2df1e0 *);
f32 FUN_001f9b48(void *, void *);
void FUN_L00_002a7210(O_2df1e0 *, void *, f32, f32);
void FUN_L00_002c5be8(O_2df1e0 *);
void FUN_L00_002df1e0(O_2df1e0 *p) {
    s32 i;
    f32 f;
    if (p == 0)
        return;
    if (p->b20 == 0xFE)
        return;
    if (p->b20 == 0xFD)
        return;
    if (random_integer_below(10))
        return;
    if (!(p->p24 && p->p24->h46 == 0x13) && !FUN_L00_0025e3f8(p))
        return;
    if (!(FUN_001f9b48(p->v10, &hero.motion.pos) < 30.0f))
        return;
    for (i = 0; i < 10 && D_L00_001E6008[i] >= 8; i++) {
        f = i * 0.3f + 0.1f;
        if (f < 2.0f)
            f = 2.0f;
        if (FUN_001f9b48(p->v10, D_L00_001E5F40_2df1e0[i]) < f) {
            if (p && p->p24 && p->p24->h46 == 0x13) {
                FUN_L00_002a7210(p, hero.moby, 0.0f, 0.0f);
            } else {
                FUN_L00_002c5be8(p);
            }
        }
    }
}
int FUN_L00_002591d0(void *, s32, s32, s32);
int FUN_L00_002592b8(void *, s32, s32, s32);
void FUN_L00_00263618(void *, void *, f32, f32);
void FUN_L00_002df3d8(u8 *p) {
    u8 *it[2] __attribute__((aligned(16)));
    f32 s = 1.2f;
    if (current_level_index == 0 || current_level_index == 0x12) {
        s = 1.0f;
    }
    FUN_L00_002591d0(it, p[0x21], 0, 0);
    while (it[0] != 0) {
        u8 *q = it[0];
        if (q[0x31] != 0) {
            void **m = *(void ***)(q + 0x78);
            if (*(s16 *)(q + 0xA6) == *(s16 *)(p + 0xA6)) {
                FUN_L00_00263618(q + 0x10, m[1], s, 0.5f);
            }
        }
        FUN_L00_002592b8(it, it[0], 0, 0);
    }
}
extern s32 D_L00_0015F5CC;
typedef struct {
    char pad[8];
    s32 **p;
} S09;
typedef struct {
    char pad[0x78];
    S09 *s;
} M09;
typedef void (*CB09)(M09 *);
void FUN_L00_002df3d8_u(M09 *) __asm__("FUN_L00_002df3d8");
void FUN_001f4600(CB09, M09 *);
void FUN_L00_002df4a8(M09 *m) {
    u8 b;
    s32 v;
    S09 *s;
    b = m->pad[0x21];
    if (b == 0xFF)
        return;
    s = m->s;
    v = D_L00_0015F5CC;
    if (v == 0)
        return;
    if (v != *s->p) {
        *s->p = v;
        FUN_001f4600(FUN_L00_002df3d8_u, m);
    }
}
/* update for the pulsing colour object */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9438.c: func_L00_002E09A8), where it is exact; names translated to the US level program. */

extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
extern void FUN_L00_002df4a8_u(void *) __asm__("FUN_L00_002df4a8");

extern int D_L00_00161B90 __attribute__((sda));
extern int D_L00_00161B94 __attribute__((sda));
extern float D_L00_00161B98 __attribute__((sda));
void red_orb_update(char *m) __asm__("FUN_L00_002df4f8");
void red_orb_update(char *m) {
    enum {
        RED_ORB_STATE_INITIALIZE_PHASE = 0,
        RED_ORB_STATE_PULSE_COLOR = 1
    };
    char *p = *(char **)(m + 0x78);
    unsigned char s = m[0x20];
    float a;
    int c, hi, r, g, b, t;
    switch (s) {
    case RED_ORB_STATE_INITIALIZE_PHASE:
        *(float *)p = *(float *)p * DEG_TO_RAD;
        m[0x20] = 1;
        break;
    case RED_ORB_STATE_PULSE_COLOR:
        a = fast_add_rotations(*(float *)p,
                               D_L00_00161B98 * DEG_TO_RAD * frame_time);
        *(float *)p = a;
        a = fast_sin(a);
        c = fast_tween_color(D_L00_00161B90, D_L00_00161B94, a * 0.5f + 0.5f);
        b = (c >> 16) & 0xFF;
        g = (c >> 8) & 0xFF;
        r = c & 0xFF;
        hi = 0x38;
        if (current_level_index == 0 || current_level_index == 0x12)
            hi = 0x20;
        t = hi << 24 | b << 16;
        t |= g << 8;
        *(int *)(p + 4) = t | r;
        FUN_L00_002df4a8_u(m);
        set_moby_color(m, c & 0xFF, (c >> 16) & 0xFF, (unsigned)c >> 24);
        break;
    }
}

s16 D_L01_00200968[1] __attribute__((section(".data"))) = {0};
s16 D_L02_001D8AD8[1] __attribute__((section(".data"))) = {0};
s16 D_L03_001D78D8[1] __attribute__((section(".data"))) = {0};
s16 D_L04_001D8718[1] __attribute__((section(".data"))) = {0};
s16 D_L05_0020D248[1] __attribute__((section(".data"))) = {0};
s16 D_L06_001DA1C8[1] __attribute__((section(".data"))) = {0};
s16 D_L07_001DC188[1] __attribute__((section(".data"))) = {0};
s16 D_L08_001EEAA8[1] __attribute__((section(".data"))) = {0};
s16 D_L09_00205488[1] __attribute__((section(".data"))) = {0};
s16 D_L10_001D9288[1] __attribute__((section(".data"))) = {0};
s16 D_L11_001D8E58[1] __attribute__((section(".data"))) = {0};
s16 D_L12_001FA828[1] __attribute__((section(".data"))) = {0};
s16 D_L13_001D8E88[1] __attribute__((section(".data"))) = {0};
s16 D_L14_001E5338[1] __attribute__((section(".data"))) = {0};
s16 D_L15_001D8858[1] __attribute__((section(".data"))) = {0};
s16 D_L16_001D7D78[1] __attribute__((section(".data"))) = {0};
s16 D_L17_001D8B78[1] __attribute__((section(".data"))) = {0};
s16 D_L18_001D8EB8[1] __attribute__((section(".data"))) = {0};
