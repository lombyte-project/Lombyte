/* Level 08 moby classes: bomb, bomber_tank, blarg_bombthrower, shuttle, rocket. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/gameplay/entities/moby.h"

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002B9438), where it is exact; names translated to the US level program. */

extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int FUN_L08_00279f00(char *);
extern void FUN_L00_0025d1b8(void *);

extern float D_L08_00161468 __attribute__((sda));
extern float D_L08_0016146C __attribute__((sda));
void FUN_L08_002b8228(struct Moby *moby) {
    float a = D_L08_00161468;
    float b = D_L08_0016146C;
    char *d;
    moby->rot.x = 0.0f;
    moby->rot.y = 0.5235988f;
    moby->rot.z = 0.0f;
    d = (char *)moby->pvars;
    *(float *)(d + 0xC) = random_float_between(a, b);
    *(int *)(d + 0x28) = 0;
    *(float *)(d + 8) = moby->pos.z;
    FUN_L00_0025d1b8(moby);
    *(int *)(d + 0x24) = FUN_L08_00279f00(moby);
}
/* Ported from rac1-decomp src/overlays/l08_batalia/vendor_002B9438.c (func_L08_002B94B0) */

#include "qcopy.h"

/* Level 8 moby update (class 152): state machine that steers the moby toward the hero and fires its event. */

typedef struct Moby Moby;

typedef struct Target_2B94B0 {
    char pad0[0xA];
    short fA;
} Target_2B94B0;

typedef struct MobyData_2B94B0 {
    char pad0[8];
    float f8;
    float fC;
    float vel[4];
    int f20;
    Target_2B94B0 *target;
    int ticks;
    int f2C;
} MobyData_2B94B0;

typedef struct Moby_2B94B0 {
    char pad0[0x10];
    float pos[4];
    unsigned char state;
    char pad21[0x27];
    float f48;
    char pad4C[0x2C];
    MobyData_2B94B0 *data;
} Moby_2B94B0;

typedef struct Hero_2B94B0 {
    char pad0[0x80];
    float pos[4];
    char pad90[0x480];
    float v510[4];
    float v520[4];
    float v530[4];
    char pad540[0x1B4C];
    int f208C;
    char pad2090[0x1F8];
    float f2288;
    float f228C;
} Hero_2B94B0;

extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float FUN_001f9b80(void *, void *);
extern float D_0015ED70;
extern int D_0015ED84;
extern char *D_L08_0015FFD8;
extern float D_L08_0016144C __attribute__((sda));
extern float D_L08_00161458 __attribute__((sda));
extern float D_L08_0016145C __attribute__((sda));
extern int D_L08_00161460 __attribute__((sda));
extern float D_L08_00161464 __attribute__((sda));
extern int D_L08_00174658;
extern int D_L08_0015F5CC;
extern int FUN_L00_001ff660();
extern int FUN_L08_002025d0(void*,int,int,void*,int);
extern int allocate_voice_for_target_entry(int,int,void*) __asm__("FUN_0022da68");
extern s32 queue_animation_update() __asm__("func_001FF308");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void clear_u64_value(void *) __asm__("func_001F99F8");
extern void mark_moby_for_removal(void *) __asm__("func_0020C828");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");
void FUN_L08_002b8228_c(struct Moby *moby) __asm__("FUN_L08_002b8228");
extern float fast_vec_length(void *a) __asm__("FUN_001f9af0");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern int func_L00_001F39B0_2B94B0(void *, void *, int, int, int) __asm__("FUN_L08_002025d0");
extern void func_L00_001FF610_2B94B0(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern float func_001FA748_2B94B0(float, float) __asm__("FUN_001fa580");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int func_001FFB38_2B94B0(int, int, void *, void *, void *, void *, int) __asm__("FUN_001ff308");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern float func_001F9FA8_2B94B0(float) __asm__("FUN_001f9de0");
extern void func_L08_002B9438_2B94B0(void *moby) __asm__("FUN_L08_002b8228");
extern void func_L00_0023A658_2B94B0(void) __asm__("FUN_L00_00239cc8");
extern void func_L00_0023A690_2B94B0(void) __asm__("FUN_L00_00239d00");
extern void func_L00_0023A788_2B94B0(void) __asm__("FUN_L00_00239df8");
extern char D_L08_001746D8_2B94B0[] __asm__("D_L08_00174658");
extern int D_L08_001746D8_2B94B0i __asm__("D_L08_00174658") __attribute__((section(".data")));
extern char D_L08_00174700_2B94B0[] __asm__("D_L08_00174680");
extern s32 D_0013DF38_arr[] __asm__("D_0013DF38");

void FUN_L08_002b82a0(Moby_2B94B0 *m)
{
    MobyData_2B94B0 *d = m->data;
    float A[4], F[4], B[4], C[4], P[4], E[4];
    float f20, t, f;
    int hit, ok, n;

    d->ticks++;
    if (m->state != 0) m->pos[2] = d->f8;
    switch (m->state) {
    case 0:
        func_L08_002B9438_2B94B0(m);
        if (d->f2C != 0) m->state = 1;
        else m->state = 2;
        break;
    case 1:
        qcopy(P, m->pos);
        d->vel[2] = d->vel[2] - D_0015ED70 * 9.8f;
        f20 = fast_vec_length(d->vel);
        if (D_L08_0016144C * 1.8f * frame_scale < f20) {
            fast_vec_normalize(d->vel, d->vel, D_L08_0016144C * 1.8f * frame_scale);
            f20 = D_L08_0016144C * 1.8f * frame_scale;
        }
        add_vector_xyz(A, P, d->vel);
        fast_vec_normalize(B, d->vel, D_L08_0016144C);
        subtract_vector_xyz(B, P, B);
        fast_vec_normalize(C, d->vel, f20 + D_L08_0016144C);
        add_vector_xyz(C, C, P);
        if (D_L08_0015F5CC % 2 == (((char *)m - D_L08_0015FFD8) >> 8) % 2
            && func_L00_001F39B0_2B94B0(B, C, 2, 0, 0) != 0) {
            ok = 1;
            if (D_L08_001746D8_2B94B0i == 0 || d->ticks >= 10) {
                if (f20 < D_L08_00161458) {
                    ok = 0;
                } else {
                    subtract_vector_xyz(E, P, (D_L08_001746D8_2B94B0 + 8));
                    fast_vec_normalize(E, E, D_L08_0016144C);
                    add_vector_xyz(P, (D_L08_001746D8_2B94B0 + 8), E);
                }
                if (ok != 0) {
                    func_L00_001FF610_2B94B0(F, d->vel, D_L08_00174700_2B94B0);
                    f = fast_vec_length(F) - D_L08_0016145C;
                    fast_vec_normalize(d->vel, F, f < 0.0f ? 0.0f : f);
                } else {
                    clear_u64_value(d->vel);
                    qcopy(P, (D_L08_001746D8_2B94B0 + 8));
                    P[2] = P[2] + D_L08_0016144C;
                    if (*(int *)D_L08_001746D8_2B94B0 == 0) {
                        clear_u64_value(d->vel);
                        m->state = 2;
                    }
                }
            } else {
                qcopy(P, A);
            }
        } else {
            qcopy(P, A);
        }
        qcopy(m->pos, P);
    case 2:
        hit = 0;
        m->f48 = func_001FA748_2B94B0(m->f48, d->fC);
        if (((Hero_2B94B0 *)((char *)&hero))->f208C == 15) {
            if (vector_distance(((Hero_2B94B0 *)((char *)&hero))->v510, m->pos) < 0.5f) hit = 1;
            if (vector_distance(((Hero_2B94B0 *)((char *)&hero))->v520, m->pos) < 0.5f) hit = 1;
            if (vector_distance(((Hero_2B94B0 *)((char *)&hero))->v530, m->pos) < 0.5f) hit = 1;
        } else if (vector_distance(m->pos, ((Hero_2B94B0 *)((char *)&hero))->pos) < ((Hero_2B94B0 *)((char *)&hero))->f2288) {
            if (absolute_float(m->pos[2] - ((Hero_2B94B0 *)((char *)&hero))->pos[2]) < ((Hero_2B94B0 *)((char *)&hero))->f228C) hit = 1;
        }
        if (hit == 0) break;
        if (d->target != 0) d->target->fA = 0;
        current_bolt_count = current_bolt_count + d->f20;
        ((int *)D_0013DF38_arr)[D_0015ED84] = ((int *)D_0013DF38_arr)[D_0015ED84] + d->f20;
        allocate_voice_for_target_entry(0, 0, m);
        func_001FFB38_2B94B0(2, 0x754E, func_L00_0023A658_2B94B0, func_L00_0023A690_2B94B0,
                             func_L00_0023A788_2B94B0, &current_bolt_count, 0x98967F);
        mark_moby_for_removal(m);
        return;
    case 3:
        break;
    case 4:
        if (d->target != 0) d->target->fA = 0;
        mark_moby_for_removal(m);
        return;
    }
    n = D_L08_00161460;
    t = (float)(d->ticks % n) / (float)(n - 1) * 6.2831855f - 3.1415927f;
    f = func_001F9FA8_2B94B0(t);
    d->f8 = m->pos[2];
    m->pos[2] = m->pos[2] + D_L08_00161464 * f;
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Spawns a moby of class 0xF8 at a position and facing derived from a vector. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D35E8), where it is exact; names translated to the US level program. */

extern char *func_0020D348_m(int);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void FUN_L00_00250df8(void *);

char *FUN_L08_002d22e0(int arg, char *pos, char *vec) {
    char *m = func_0020D348_m(0xF8);
    if (m != 0) {
        char *d = *(char **)(m + 0x78);
        *(unsigned char *)(m + 0x30) = 0xFF;
        *(short *)(m + 0x32) = 0xFF;
        m[0x31] = 1;
        m[0x20] = 1;
        *(int *)(d + 0x30) = arg;
        *(int *)(d + 0x20) = scale_game_frames(300);
        *(float *)(d + 0x2C) = 1.0f;
        *(int *)(d + 0x24) = 0;
        qcopy(m + 0x10, pos);
        qcopy(d + 0x10, vec);
        *(float *)(m + 0x40) = random_float_between(-180.0f, 180.0f) * DEG_TO_RAD;
        *(float *)(m + 0x48) = fast_arc_tan(*(float *)vec, *(float *)(vec + 4)) + 1.5707964f;
        FUN_L00_00250df8(m);
    }
    return m;
}
/* Bomb (class 248) update: fuse, bounce and blast state machine on the moby's state byte */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D36F8), where it is exact; names translated to the US level program. */

typedef int v128_36F8 __attribute__((mode(TI)));

typedef struct { char b[16]; } blk16;

struct Probe_36F8 { float v[4]; void *moby; int flags; int pad; float f; int st; };

struct Ray_36F8 { float start[4]; float end[4]; struct Probe_36F8 x; };

union Coll_36F8 { struct Ray_36F8 r; struct Probe_36F8 p; };

typedef int OvlQuad_c __attribute__((mode(TI)));

typedef struct {
    u8 pad0[0x18];
    s32 moby; /* 0x18: passed to FUN_002141f8 (reads moby+0x34, moby+0x78); 0 when no moby was hit */
    s32 unk1C; /* 0x1C: > 0 gates copying point and the normal into the hero (shared/ui/help/00226f10.c) */
    union {
        OvlQuad_c q;
        f32 f[4];
    } point; /* 0x20: f[2] is read as the ground height */
    u8 pad30[0x10]; /* 0x30: position after push-out (copied into hero.motion.pos) */
    f32 normal_x; /* 0x40: start of the surface normal; its angle is tested against 50 degrees (0.87266463) */
    f32 normal_y; /* 0x44: FUN_001f9e90 (atan2) of normal_x, normal_y is stored in the hero */
    f32 normal_z; /* 0x48: ledge probes compare atan2(normal_z, xy length) with 20 degrees */
} CollisionHit_2d23f0;

#include "rnc/overlay/collision.h"
extern CollisionHit D_L08_00174640;
extern blk16 D_L08_00161868;
extern float D_0015ED6C __attribute__((section(".sdata")));
extern float D_0015ED70_gp __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern f32 fast_vec_length_c(void *) __asm__("func_001F9AF0");
extern f32 random_float_between_c5(f32, f32) __asm__("func_002132A8");
extern float fast_add_rotations_c(float, float) __asm__("func_001FA580");
extern float D_L08_00174660[];
extern int FUN_001efa68(void *, void *, int, void *, int);
extern int FUN_L00_001f0d60(float, void *, int, void *);
extern int FUN_L00_001f2868(void *, float, int, void *, void *);
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern int tick_countdown_32_alt(void *) __asm__("FUN_001f9740");
extern s32 random_integer_below_c3(s32) __asm__("func_00213260");
extern s32 scale_game_frames_c(s32) __asm__("func_001F96F8");
extern float D_L08_00161860 __attribute__((sda));
extern void FUN_L00_001ff660_c(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void FUN_L00_00259fe8(void *, float);
extern void FUN_L00_0025d458_c(void *, short *) __asm__("FUN_L00_0025d458");
extern void FUN_L00_0025d538_c(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_0025e450_c3(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_0025f090_c(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void FUN_L00_00269958_c(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("func_001F9AD8");
extern void fast_vec_normalize_c(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void fast_vec_scale_c(void *, void *, f32) __asm__("func_001F9A68");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern float D_0015ED70_gp __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern float D_0015ED6C __attribute__((section(".sdata")));
extern float D_L08_0015F580_macro[] __asm__("D_L08_0015F580") __attribute__((section(".sdata")));
extern float D_L08_00174660[];

void FUN_L08_002d23f0(char *moby) {
    union Coll_36F8 c;
    blk16 t70;
    float t80[4];
    float t90[4];
    char *d;
    int st;
    int two;
    float f20;

    d = *(char **)(moby + 0x78);
    if (*(unsigned char *)(moby + 0x20) != 0x37) {
        if (FUN_L00_001f0d60(0.5f, moby + 0x10, 0x11, moby)) {
            int g = D_L08_001746D8_2B94B0i;
            if (g != 0 && (g != *(int *)(d + 0x30) || *(int *)(d + 0x24) != 0)) {
                if (!(*(unsigned char *)(moby + 0xBC) & 2)) *(unsigned char *)(moby + 0x20) = 0x62; else *(unsigned char *)(moby + 0x20) = 0x63;
                qcopy((moby + 0x10), ((char *)D_L08_00174660));
                FUN_L00_001ff660_c(d + 0x10, d + 0x10, ((char *)D_L08_00174660) + 0x20);
                fast_vec_normalize(d + 0x10, d + 0x10, D_0015ED6C + D_0015ED6C);
            }
        }
    }

    st = *(unsigned char *)(moby + 0x20);
    two = 2;
    switch (st) {
    case 1: {
        float t;
        t = fast_vec_length_c((d + 0x10));
        *(float *)(moby + 0x40) = fast_add_rotations_c(*(float *)(moby + 0x40), t * 0.5f);
        if (*(int *)(d + 0x24) == 0) {
            float lo = -1.0f;
            float hi = 1.0f;
            int *q;
            t70 = D_L08_00161868;
            *(v128_36F8 *)t90 = 0;
            t90[0] = random_float_between_c5(lo, hi);
            t90[1] = random_float_between_c5(lo, hi);
            t90[2] = random_float_between_c5(lo, hi);
            *(v128_36F8 *)t80 = *(v128_36F8 *)t90;
            fast_vec_normalize(t80, t80, D_0015ED6C);
            q = (int *)t70.b + random_integer_below_c3(4);
            FUN_L00_00269958_c((moby + 0x10), t80, *q, 0x4FFF, scale_game_frames_c(0x2D), 0x32, 0x78, 1);
        }
        qcopy(c.r.start, moby + 0x10);
        f20 = 0.16f;
        c.r.start[2] -= f20;
        add_vector_xyz((moby + 0x10), (moby + 0x10), (d + 0x10));
        *(float *)(d + 0x18) -= D_L08_00161860 * D_0015ED70_gp;
        qcopy(c.r.end, moby + 0x10);
        c.r.end[2] -= f20;
        c.r.x.moby = moby;
        c.r.x.flags = 0x810000;
        c.r.x.st = st;
        c.r.x.f = 1.0f;
        qcopy(c.r.x.v, d + 0x10);
        if (FUN_001efa68(c.r.start, c.r.end, 0, moby, 0)) {
            char *tbl = (char *)&D_L08_00174640;
            int v = *(int *)(tbl + 0x18);
            if (v != 0) {
                if (v != *(int *)(d + 0x30) || *(int *)(d + 0x24) != 0) {
                    if (!(*(unsigned char *)(moby + 0xBC) & 2)) *(unsigned char *)(moby + 0x20) = 0x62; else *(unsigned char *)(moby + 0x20) = 0x63;
                    qcopy((moby + 0x10), ((char *)D_L08_00174660));
                    {
                        char *v = d + 0x10;
                        FUN_L00_001ff660_c(v, v, ((char *)D_L08_00174660) + 0x20);
                        fast_vec_normalize(v, v, D_0015ED6C + D_0015ED6C);
                    }
                }
            } else if (*(int *)(tbl + 0x1C) > 0) {
                if (*(unsigned char *)(moby + 0xBC) & 1) {
                    float k;
                    allocate_voice_for_target_entry(1, 0, moby);
                    qcopy((moby + 0x10), tbl + 0x20);
                    *(float *)(moby + 0x18) += f20;
                    FUN_L00_001ff660_c((d + 0x10), (d + 0x10), tbl + 0x40);
                    cross_vectors_xyz(&t70, (d + 0x10), tbl + 0x40);
                    k = D_0015ED6C * 1.5f;
                    fast_vec_normalize(&t70, &t70, random_float_between_c5(-k, k));
                    add_vector_xyz((d + 0x10), (d + 0x10), &t70);
                    fast_vec_scale_c((d + 0x10), (d + 0x10), 0.4f);
                    *(int *)(d + 0x24) += 1;
                    *(int *)(d + 0x20) = scale_game_frames_c(0xB4);
                    if (fast_vec_length_c((d + 0x10)) < D_0015ED6C) {
                        int r;
                        *(unsigned char *)(moby + 0x20) = two;
                        r = scale_game_frames_c(0x78);
                        *(int *)(d + 0x20) = r;
                        *(float *)(d + 0x28) = 254.0f / (float)r;
                        clear_u64_value((d + 0x10));
                    }
                } else {
                    *(unsigned char *)(moby + 0x20) = 0x62;
                    qcopy((moby + 0x10), tbl + 0x20);
                    FUN_L00_001ff660_c((d + 0x10), (d + 0x10), tbl + 0x40);
                    fast_vec_normalize((d + 0x10), (d + 0x10), D_0015ED6C + D_0015ED6C);
                }
            }
        } else if (tick_countdown_32_alt(d + 0x20)) {
            if (*(unsigned char *)(moby + 0xBC) & 2) {
                *(unsigned char *)(moby + 0x20) = 0x63;
            } else {
                *(unsigned char *)(moby + 0x20) = 0x62;
            }
        }
        break;
    }
    case 2:
        if (tick_countdown_32_alt(d + 0x20)) {
            *(unsigned char *)(moby + 0x20) = 0x63;
        } else if (*(int *)(d + 0x20) == scale_game_frames_c(0x3C)
                   || *(int *)(d + 0x20) == scale_game_frames_c(0x28)
                   || *(int *)(d + 0x20) == scale_game_frames_c(0x14)) {
            *(unsigned char *)(d + 7) = 0xFA;
            FUN_L00_0025d458_c(moby, (short *)d);
        }
        FUN_L00_0025d538_c(moby, d);
        break;
    case 0x62:
        allocate_voice_for_target_entry(0, 0, moby);
        c.p.moby = moby;
        c.p.flags = 0x810001;
        c.p.f = 1.0f;
        clear_u64_value(&c.p);
        c.p.st = 0;
        FUN_L00_001f2868(moby + 0x10, 1.0f, 0x10, moby, &c.p);
        FUN_L00_0025e450_c3(moby, D_L08_0015F580_macro, 0, 0.0f, 0.0f, 7, 10, 20, 3.0f, 1.7f, 4.0f, 1.0f, -1, 7.0f, 0, 7, -1, 0);
        mark_moby_for_removal(moby);
        return;
    case 0x63: {
        char *p;
        allocate_voice_for_target_entry(0, 0, moby);
        p = moby + 0x10;
        c.p.moby = moby;
        c.p.flags = 0x810001;
        c.p.f = 1.0f;
        clear_u64_value(&c.p);
        c.p.st = 0;
        FUN_L00_001f2868(p, 1.0f, 0x10, moby, &c.p);
        FUN_L00_0025f090_c(moby, p, -1, 0.5f, 13.0f);
        mark_moby_for_removal(moby);
        return;
    }
    }
    FUN_L00_00259fe8(moby, 0.2f);
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L08_002d42d8.s", FUN_L08_002d42d8);
#else
extern int D_L08_001B0CB0[];
extern s32 D_001413D0[];
extern char D_0013D388[] __attribute__((section(".data")));
extern unsigned short D_00141C38 __attribute__((section(".data")));
extern unsigned char D_0013D4CB;
extern float D_0015ED6C;
extern float D_L08_001618A8 __attribute__((sda));
extern float D_L08_001618CC __attribute__((sda));
extern float D_L08_001618D0 __attribute__((sda));
extern struct Moby *create_moby(int) __asm__("FUN_0020c4f8");
extern void attach_manipulator(void *, int, void *) __asm__("FUN_0020cb10");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern float moby_animation_frame(void *) __asm__("FUN_0020c9e0");
extern int moby_animation_crossed_frame(void *, float) __asm__("FUN_00214cc8");
extern int FUN_L00_0024f7c8();
extern void FUN_L00_00260738(void *, void *, void *, void *);
extern void FUN_L00_00203908(int, int);
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
void FUN_L08_002d4ca0(struct Moby *moby);
void FUN_L08_002d55c8(struct Moby *moby, int path_id);
void FUN_L08_002d5950(char *moby, int joint, int count);
void FUN_L08_002d5d08_u(struct Moby *moby) __asm__("FUN_L08_002d5d08");
void FUN_L08_002d6450(struct Moby *moby);
char *FUN_L08_002dc8a0(char *src, char *pos, char *vec);

void FUN_L08_002d42d8(struct Moby *moby) {
    float old_position[4], old_rotation[4], velocity[4], joint_pos[4];
    char *data = (char *)moby->pvars;
    void *position = &moby->pos;
    struct Moby *target;
    struct Moby *other;
    long state;
    float angle, speed, frame;
    qcopy(old_position, position);
    qcopy(old_rotation, &moby->rot);
    if (*(unsigned char *)(data + 0x2e) == 2) {
        if (*(void **)(data + 0x160)) mark_moby_for_removal(*(void **)(data + 0x160));
        if (*(void **)(data + 0x164)) mark_moby_for_removal(*(void **)(data + 0x164));
        mark_moby_for_removal(moby);
        return;
    }
    FUN_L08_002d4ca0(moby);
    target = *(struct Moby **)(data + 0xf0);
    state = moby->state;
    if (state == 11) goto state11;
    if (state < 12) {
        if (state == 3) goto state3;
        if (state < 4) {
            if (state == 0) goto state0;
        } else if (state == 6) goto state6;
    } else {
        if (state == 14) goto state14;
        if (state < 15) goto finish;
        if (state != 99) goto finish;
        goto state99;
    }
    goto finish;
    state0:
        if (*(struct Moby **)(data + 0x160) == 0) {
            (*(struct Moby **)(data + 0x160) = create_moby(0x14b))->update_dist = 0x40;
            (*(struct Moby **)(data + 0x160))->draw_dist = 0x40;
            (*(struct Moby **)(data + 0x160))->visible = 1;
            (*(struct Moby **)(data + 0x160))->spawn_frame = (*(struct Moby **)D_001413D0)->spawn_frame;
            (*(struct Moby **)(data + 0x160))->flags = 0x4000;
            other = *(struct Moby **)(data + 0x160);
            qcopy(&other->pos, position);
            other->collision = 0;
            blend_moby_animation(*(void **)(data + 0x160), 0, 0, 0);
            FUN_L00_00250df8(*(void **)(data + 0x160));
        }
        if (*(struct Moby **)(data + 0x164) == 0) {
            (*(struct Moby **)(data + 0x164) = create_moby(0x14b))->update_dist = 0x40;
            (*(struct Moby **)(data + 0x164))->draw_dist = 0x40;
            (*(struct Moby **)(data + 0x164))->visible = 1;
            (*(struct Moby **)(data + 0x164))->spawn_frame = (*(struct Moby **)D_001413D0)->spawn_frame;
            (*(struct Moby **)(data + 0x164))->flags = 0xc000;
            other = *(struct Moby **)(data + 0x164);
            qcopy(&other->pos, position);
            other->collision = 0;
            blend_moby_animation(*(void **)(data + 0x164), 0, 0, 0);
            FUN_L00_00250df8(*(void **)(data + 0x164));
        }
        attach_manipulator(moby, 0, data + 0x100);
        moby->flags |= 0x1000;
        *(float *)(data + 0x16c) = D_0015ED6C + D_0015ED6C;
        if (*(int *)(data + 0x140) > 0) {
            moby->state = 11;
            moby->flags = (moby->flags & 0xefff) | 0x41;
            (*(struct Moby **)(data + 0x160))->flags |= 0x41;
            (*(struct Moby **)(data + 0x164))->flags |= 0x41;
            moby->collision = 0;
            (*(struct Moby **)(data + 0x160))->collision = 0;
            (*(struct Moby **)(data + 0x164))->collision = 0;
            if (moby->prev_seq != 0) blend_moby_animation(moby, 0, 0, 0);
        } else if (*(int *)(data + 0x144) > 0) {
            if (((unsigned char *)D_0013D388)[0x2e] == 0) {
                moby->state = 14;
                moby->flags |= 0x41;
            (*(struct Moby **)(data + 0x160))->flags |= 0x41;
            (*(struct Moby **)(data + 0x164))->flags |= 0x41;
            moby->collision = 0;
            (*(struct Moby **)(data + 0x160))->collision = 0;
            (*(struct Moby **)(data + 0x164))->collision = 0;
            if (moby->prev_seq != 0) blend_moby_animation(moby, 0, 0, 0);
            } else {
                FUN_L08_002d6450(moby);
            qcopy(&moby->pos, (char *)D_L08_001B0CB0[*(int *)(data + 0x150)] + 0x10);
            moby->state = 6;
            if (moby->prev_seq != 1) blend_moby_animation(moby, 1, 0, 0);
            }
        } else {
            qcopy(&moby->pos, (char *)D_L08_001B0CB0[*(int *)(data + 0x150)] + 0x10);
            moby->state = 6;
            if (moby->prev_seq != 1) blend_moby_animation(moby, 1, 0, 0);
        }
 initialize:
        clear_u64_value(data + 0x40);
        *(unsigned short *)(data + 0x28) = 3;
        *(float *)(data + 0x30) = 3.0f;
        data[0x2a] = 12;
        data[0x2e] = 1;
        *(float *)(data + 0x20) = 9.0f;
        *(unsigned short *)(data + 0x3e) |= 2;
        clear_u64_value(data + 0x40);
        *(float *)(data + 0x178) = 0;
        speed = D_L08_001618CC;
        angle = moby->rot.z;
        *(float *)(data + 0x170) = angle;
        data[0x58] = truncate_float_to_s32(speed);
        data[0x5a] = truncate_float_to_s32(D_L08_001618D0 * 8.0f);
        goto finish;
    state6:
        FUN_L08_002d55c8(moby, *(int *)(data + 0x150));
        if (*(int *)(data + 0xf4) == 2) goto finish;
        if (!(vector_distance(&moby->pos, &target->pos) < 20.0f)) goto finish;
        if (D_00141C38 == 0 && D_0013D4CB == 0) FUN_L00_00203908(0x1f44, 0x5a);
        moby->state = 3;
        if (moby->prev_seq != 2) blend_moby_animation(moby, 2, 0, scale_game_frames(10));
        goto finish;
    state3:
        FUN_L08_002d55c8(moby, *(int *)(data + 0x150));
        if (*(int *)(data + 0xf4) == 2 || vector_distance(&moby->pos, &target->pos) > 22.0f) {
            moby->state = 6;
            if (moby->prev_seq != 1) blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        }
        if (moby_animation_crossed_frame(moby, 4.0f) || moby_animation_crossed_frame(moby, 10.0f)) {
            frame = moby_animation_frame(moby);
            if (frame == 4.0f) FUN_L00_0024f7c8(moby, 1, joint_pos);
            else FUN_L00_0024f7c8(moby, 2, joint_pos);
            frame = D_L08_001618A8;
            frame *= D_0015ED6C;
            angle = fast_cos(fast_add_rotations(*(float *)(data + 0x170), moby->rot.z));
            angle *= (fast_cos(*(float *)(data + 0x178)) * frame);
            velocity[0] = angle;
            angle = fast_sin(fast_add_rotations(*(float *)(data + 0x170), moby->rot.z));
            angle *= (fast_cos(*(float *)(data + 0x178)) * frame);
            velocity[2] = 0.0f;
            velocity[1] = angle;
            velocity[2] = -fast_sin(*(float *)(data + 0x178)) * frame;
            {
                struct Moby *other = (struct Moby *)FUN_L08_002dc8a0((char *)moby, (char *)joint_pos, (char *)velocity);
                if (other) other->state = 1;
            }
        }
        goto finish;
    state11:
        if (**(float **)(D_L08_0015FFD8 + *(int *)(data + 0x140) * 0x100 + 0x78) != 1.0f) goto finish;
        qcopy(&moby->pos, (char *)D_L08_001B0CB0[*(int *)(data + 0x14c)] + 0x10);
        qcopy(&(*(struct Moby **)(data + 0x160))->pos, &moby->pos);
        qcopy(&(*(struct Moby **)(data + 0x164))->pos, &moby->pos);
        moby->flags &= 0xffbe;
        (*(struct Moby **)(data + 0x160))->flags &= 0xffbe;
        (*(struct Moby **)(data + 0x164))->flags &= 0xffbe;
        goto restore_collision;
    state14:
        if (*(int *)(data + 0xf4) == 2) goto finish;
        FUN_L08_002d5d08_u(moby);
        target = (struct Moby *)D_0013D388;
        if (((unsigned char *)target)[0x2e] != 0) allocate_voice_for_target_entry(6, 0, moby);
        ((unsigned char *)target)[0x2e] = 1;
 restore_parts:
        qcopy(&(*(struct Moby **)(data + 0x160))->pos, &moby->pos);
        qcopy(&(*(struct Moby **)(data + 0x164))->pos, &moby->pos);
        moby->flags &= 0xffbe;
        (*(struct Moby **)(data + 0x160))->flags &= 0xffbe;
        (*(struct Moby **)(data + 0x164))->flags &= 0xffbe;
 restore_collision:
        moby->collision = moby->pclass->collision;
        other = *(struct Moby **)(data + 0x160);
        other->collision = other->pclass->collision;
        other = *(struct Moby **)(data + 0x164);
        other->collision = other->pclass->collision;
        moby->flags |= 0x1000;
        if (*(int *)(data + 0xf4) == 2) goto finish;
        moby->state = 6;
        if (moby->prev_seq != 1) blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        goto finish;
    state99:
        FUN_L08_002d55c8(moby, *(int *)(data + 0x150));
        frame = moby_animation_frame(moby);
        if ((59.0f <= frame && frame <= 60.0f) || (119.0f <= frame && frame <= 120.0f))
            FUN_L08_002d5950((char *)moby, 4, 10);
        else if ((29.0f <= frame && frame <= 30.0f) || (89.0f <= frame && frame <= 90.0f))
            FUN_L08_002d5950((char *)moby, 5, 10);
        goto finish;
finish:
    subtract_vector_xyz(velocity, &moby->pos, old_position);
    FUN_L00_00260738(data + 0x60, velocity, &moby->rot, old_rotation);
}
#endif
/* Per-frame update of a batalia turret moby: damage, its death state and its two helper parts. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D5FA8), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x20];
    float health;
    char pad24[2];
    short timer;
    char pad28[0x16];
    unsigned short flags;
    float f40[4];
    char pad50[0x50];
    short anim0;
    short anim2;
    char padA4[3];
    unsigned char tint;
    char padA8[8];
    float target[4];
    char padC0[0x30];
    int fF0;
    int mode;
    char padF8[0x18];
    float f110[4];
    float f120[4];
    char pad130[0x24];
    int path;
    char pad158[8];
    Moby *partA;
    Moby *partB;
    char pad168[8];
    float yaw;
    float yaw_speed;
    float pitch;
    float pitch_speed;
    char pad180[4];
    int part_id;
} Data_2D5FA8;

typedef struct { char pad0[0x20]; Moby *moby; } Hit_2D5FA8;

typedef struct { int count; char pad4[0xC]; char list[1]; } Path_2D5FA8;

void *FUN_L05_002899f8(void *a, void *b, void *c, void *d);
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float D_0015ED6C;
extern int D_L08_001B0CB0[];
extern int multiply_quaternions() __asm__("FUN_001fa3c0");
extern s32 fun_00213928(void *, void *, void *, u32, s32 *, f32 *, s32 *, s32) __asm__("FUN_00213928");
extern int FUN_L00_001fff28();
extern int allocate_voice_for_target_entry_alt(int,int,void*) __asm__("FUN_0022da68");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern void *FUN_L00_0025a420(void*,int,int);
extern void FUN_L00_0025e450(void *,void *,void *,float,float,int,int,int,float,float,float,float,int,float,int,int,int,int);
extern void FUN_L00_00263d40(int, int);
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
float FUN_L00_0025be00(float *cur, float *vel, float target, float lim, float accel, float maxv);
int FUN_L00_0025ff38(float best, char *a, char *o, int *excluded, int excluded_count, void *region, int region_count);
s32 allocate_voice_for_bank_entry(s32 entry_index, s32 flags, s32 owner) __asm__("FUN_0022db10");
void *FUN_L00_00263fd8(char *src, int cls, float *pos, void *mat, int a8, int a9, float *v10, float *v11, float scale, float *v12);
void FUN_L00_00257470(unsigned char *m, int flags, int arg);
void FUN_L00_0025d458(void *a, void *s);
void FUN_L00_0025d538(s32 id, void *s);
extern int D_0015ED84;
extern float D_0015ED70;
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern int D_L08_0015F694_2D5FA8 __asm__("D_L08_0015F5B0");
extern int D_L08_0015F698_2D5FA8 __asm__("D_L08_0015F5B4") __attribute__((sda));
extern int D_L08_0015F6A0_2D5FA8 __asm__("D_L08_0015F5BC") __attribute__((sda));
extern Path_2D5FA8 *D_L08_001B0FB0_2D5FA8[] __asm__("D_L08_001B0CB0");
extern s32 D_001413D0[];
extern unsigned char D_0013D50F_2D5FA8[] __asm__("D_0013D408");
extern Hit_2D5FA8 *find_2D5FA8(Moby *, int, int) __asm__("FUN_L00_0025a420");
extern Hit_2D5FA8 *pick_2D5FA8(Hit_2D5FA8 *, Hit_2D5FA8 *, Hit_2D5FA8 *, int) __asm__("FUN_L05_002899f8");
extern int damage_2D5FA8(Moby *, Hit_2D5FA8 *, float *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern int ticks_2D5FA8(int) __asm__("FUN_001f96f8");
extern void animate_2D5FA8(Moby *, int, int, int) __asm__("FUN_00212f90");
extern void zero_2D5FA8(void *) __asm__("FUN_001f99f8");
extern void burst_2D5FA8(Moby *, float *, float *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void *spawn_2D5FA8(Moby *, int, float *, float *, int, int, float, float *, float *, float *) __asm__("FUN_L00_00263fd8");
extern int sound_2D5FA8(int, int, int) __asm__("FUN_0022db10");
extern void voice_2D5FA8(int, int) __asm__("FUN_L00_00263d40");
extern void explode_2D5FA8(Moby *, int, int) __asm__("FUN_L00_00257470");
extern void delete_2D5FA8(Moby *) __asm__("FUN_0020c828");
extern int event_2D5FA8(int, int, Moby *) __asm__("FUN_0022da68");
extern void effects_start_2D5FA8(Moby *, short *) __asm__("FUN_L00_0025d458");
extern int follow_2D5FA8(float, Moby *, float *, int, int, char *, int) __asm__("FUN_L00_0025ff38");
extern void fast_vec_sub(float *, float *, float *) __asm__("FUN_001f9a28");
extern float angle_2D5FA8(float, float) __asm__("FUN_001f9e90");
extern float length_2D5FA8(float *) __asm__("FUN_001f9b20");
extern float angle_delta_2D5FA8(float, float) __asm__("FUN_001fa5c8");
extern float approach_2D5FA8(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");
extern void rotation_2D5FA8(float *, int, float) __asm__("FUN_L00_001fff28");
extern void matmul_2D5FA8(float *, float *, float *) __asm__("FUN_001fa3c0");
extern void effects_update_2D5FA8(Moby *, short *) __asm__("FUN_L00_0025d538");

extern int D_L08_0015F5B8 __attribute__((sda));
void FUN_L08_002d4ca0(Moby *moby) {
    Data_2D5FA8 *data = (Data_2D5FA8 *)moby->pvars;
    Hit_2D5FA8 *source;
    Hit_2D5FA8 *part_a_source;
    Hit_2D5FA8 *part_b_source;
    float vec[4];
    int hit;
    float damage = 0.0f;
    int one;

    source = find_2D5FA8(moby, 0x330000, 0);
    if (data->partA != 0) {
        part_a_source = find_2D5FA8(data->partA, 0x210000, 0);
    } else {
        part_a_source = 0;
    }
    if (data->partB != 0) {
        part_b_source = find_2D5FA8(data->partB, 0x210000, 0);
    } else {
        part_b_source = 0;
    }
    source = pick_2D5FA8(source, part_a_source, part_b_source, 0);
    if (source != 0 && source->moby != 0) {
        short source_class = source->moby->oclass;
        if (source_class == 0x1A9 || source_class == 0x47) source = 0;
    }
    damage_2D5FA8(moby, source, &data->health, 0, &hit, &damage, 0, 4);
    one = 1;
    if (hit != one && moby->state != 0) {
        data->health -= damage;
        if (damage != 0.0f) event_2D5FA8(4, 0, moby);
        if (data->health > 0.0f) {
            if (damage < 4.0f) {
                data->tint = 0x78;
                if ((unsigned short)(source->moby->oclass - 0xB0) < 2) {
                    data->timer = ticks_2D5FA8(0x3C);
                } else {
                    data->timer = ticks_2D5FA8(0x1E);
                }
            } else if (damage < 6.0f) {
                data->tint = 0xC8;
                data->timer = ticks_2D5FA8(0x1E);
            } else {
                data->tint = 0xE6;
                data->timer = ticks_2D5FA8(0x1E);
            }
            if (moby->state == 0xC && moby->prev_seq != 1) {
                animate_2D5FA8(moby, 1, 0, 0);
            }
        } else {
            int dead = 0x63;
            if (moby->state != dead) {
                qcopy(vec, &moby->pos);
                vec[2] += 3.0f;
                data->tint = 0xFA;
                zero_2D5FA8(data->f120);
                burst_2D5FA8(moby, data->f40, vec, 0.0f, 0.0f, 20, 8, 20, 4.0f, 2.0f, 9.0f, 1.0f, -1, 15.0f, 1, one, -1, 0);
                spawn_2D5FA8(moby, 0x6C0, (float *)&moby->pos, (float *)&moby->rot, 0, 0, 0.0f, D_L08_0015F580_macro, D_L08_0015F580_macro, D_L08_0015F580_macro);
                spawn_2D5FA8(moby, 0x6C1, (float *)&moby->pos, (float *)&moby->rot, 0, 0, 0.0f, D_L08_0015F580_macro, D_L08_0015F580_macro, D_L08_0015F580_macro);
                spawn_2D5FA8(moby, 0x6C2, (float *)&moby->pos, (float *)&moby->rot, 0, 0, 0.0f, D_L08_0015F580_macro, D_L08_0015F580_macro, D_L08_0015F580_macro);
                spawn_2D5FA8(moby, 0x6C3, (float *)&moby->pos, (float *)&moby->rot, 0, 0, 0.0f, D_L08_0015F580_macro, D_L08_0015F580_macro, D_L08_0015F580_macro);
                spawn_2D5FA8(moby, 0x6C4, (float *)&moby->pos, (float *)&moby->rot, 0, 0, 0.0f, D_L08_0015F580_macro, D_L08_0015F580_macro, D_L08_0015F580_macro);
                if (moby->prev_seq != one) animate_2D5FA8(moby, 1, 0, 3);
                moby->state = dead;
                data->health = 6.0f;
                data->flags &= 0xFFFD;
            } else {
                if (D_0015ED84 == 8) {
                    int part = data->part_id;
                    if (part != 0 && source != 0 && source->moby != 0 && source->moby->oclass == 0x661) {
                        if (part == one) D_L08_0015F694_2D5FA8 = one;
                        if (part == 2) D_L08_0015F698_2D5FA8 = one;
                        if (part == 3) D_L08_0015F5B8 = one;
                        if (part == 4) D_L08_0015F6A0_2D5FA8 = one;
                        if (D_L08_0015F694_2D5FA8 != 0 && D_L08_0015F698_2D5FA8 != 0) {
                            unsigned char *flags = D_0013D50F_2D5FA8;
                            if (flags[0xD] == 0) {
                                flags[0xD] = one;
                                sound_2D5FA8(1, 0, 0);
                                voice_2D5FA8(0x53D6, -1);
                            }
                        }
                    }
                }
                qcopy(vec, &moby->pos);
                vec[2] += 1.0f;
                explode_2D5FA8(moby, 0, -1);
                event_2D5FA8(2, 0, moby);
                burst_2D5FA8(moby, data->f40, vec, 0.0f, 0.0f, 20, 8, 20, 4.0f, 2.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
                spawn_2D5FA8(moby, 0x6FE, (float *)&moby->pos, (float *)&moby->rot, 0, 0, 0.0f, D_L08_0015F580_macro, D_L08_0015F580_macro, D_L08_0015F580_macro);
                spawn_2D5FA8(moby, 0x6FE, (float *)&moby->pos, (float *)&moby->rot, 0, 0, 0.0f, D_L08_0015F580_macro, D_L08_0015F580_macro, D_L08_0015F580_macro);
                spawn_2D5FA8(moby, 0x6FF, (float *)&moby->pos, (float *)&moby->rot, 0, 0, 0.0f, D_L08_0015F580_macro, D_L08_0015F580_macro, D_L08_0015F580_macro);
                spawn_2D5FA8(moby, 0x6FF, (float *)&moby->pos, (float *)&moby->rot, 0, 0, 0.0f, D_L08_0015F580_macro, D_L08_0015F580_macro, D_L08_0015F580_macro);
                spawn_2D5FA8(moby, 0x6FF, (float *)&moby->pos, (float *)&moby->rot, 0, 0, 0.0f, D_L08_0015F580_macro, D_L08_0015F580_macro, D_L08_0015F580_macro);
                if (data->partA != 0) delete_2D5FA8(data->partA);
                if (data->partB != 0) delete_2D5FA8(data->partB);
                delete_2D5FA8(moby);
                return;
            }
        }
        effects_start_2D5FA8(moby, &data->anim0);
    }

    moby->hit_slot = 0xFF;
    if (data->partA != 0) data->partA->hit_slot = 0xFF;
    if (data->partB != 0) data->partB->hit_slot = 0xFF;

    follow_2D5FA8(100.0f, moby, data->target, 0, 0, D_L08_001B0FB0_2D5FA8[data->path]->list, D_L08_001B0FB0_2D5FA8[data->path]->count);
    if (data->fF0 == 0) data->fF0 = *(int *)D_001413D0;
    if (data->mode != 2) {
        float yaw, distance, pitch;
        fast_vec_sub(vec, data->target, (float *)&moby->pos);
        vec[2] -= 3.0f;
        yaw = angle_2D5FA8(vec[0], vec[1]);
        distance = length_2D5FA8(vec);
        pitch = -angle_2D5FA8(distance, vec[2]);
        if (0.34906585f < pitch) {
            pitch = 0.34906585f;
        } else if (pitch < -0.7853982f) {
            pitch = -0.7853982f;
        }
        yaw = angle_delta_2D5FA8(yaw, moby->rot.z);
        approach_2D5FA8(&data->yaw, yaw, &data->yaw_speed, D_0015ED70 * 12.566371f, D_0015ED70 * 12.566371f, D_0015ED6C_lui * 9.424778f);
        approach_2D5FA8(&data->pitch, pitch, &data->pitch_speed, D_0015ED70 * 12.566371f, D_0015ED70 * 12.566371f, D_0015ED6C_lui * 9.424778f);
    } else {
        approach_2D5FA8(&data->yaw, 0.0f, &data->yaw_speed, D_0015ED70 * 12.566371f, D_0015ED70 * 12.566371f, D_0015ED6C_lui * 9.424778f);
        approach_2D5FA8(&data->pitch, 0.0f, &data->pitch_speed, D_0015ED70 * 12.566371f, D_0015ED70 * 12.566371f, D_0015ED6C_lui * 9.424778f);
    }
    rotation_2D5FA8(vec, 1, data->pitch);
    rotation_2D5FA8(data->f110, 2, data->yaw);
    matmul_2D5FA8(data->f110, vec, data->f110);
    {
        short saved_anim0 = data->anim0;
        short saved_anim2 = data->anim2;
        effects_update_2D5FA8(moby, &data->anim0);
        if (data->partA != 0) {
            data->anim0 = saved_anim0;
            data->anim2 = saved_anim2;
            effects_update_2D5FA8(data->partA, &data->anim0);
        }
        if (data->partB != 0) {
            data->anim0 = saved_anim0;
            data->anim2 = saved_anim2;
            effects_update_2D5FA8(data->partB, &data->anim0);
        }
    }
}
extern int D_L08_001B0CB0[];
extern char D_0013E550[];
extern float D_L08_001618A0 __attribute__((sda));
extern float fast_atan2_l08(float, float) __asm__("FUN_001f9e90");
extern float approach_angle_l08(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern void approach_value_l08(float *, float, float) __asm__("FUN_00213ed8");
extern float fast_cos_l08(float) __asm__("FUN_001f9dc8");
extern float fast_sin_l08(float) __asm__("FUN_001f9de0");
extern void add_vector_l08(void *, void *, void *) __asm__("FUN_001f9a10");
extern float ground_height_l08(void *, int, float) __asm__("FUN_00213508");
extern float distance_vector_l08(void *, void *) __asm__("FUN_001f9b80");
extern float length_vector_l08(void *) __asm__("FUN_001f9af0");
extern int sound_valid_l08(void *, int) __asm__("FUN_L00_0028d8c0");
extern int play_sound_l08(int, int, void *) __asm__("FUN_0022da68");
extern void set_sound_volume_l08(int, int) __asm__("FUN_L01_002a1968");
extern void stop_sound_l08(int) __asm__("FUN_0022d798");
extern int scale_game_frames_l08(int) __asm__("FUN_001f96f8");

typedef struct {
    u8 pad0[0x80];
    int volume;
} SoundSlot_2d55c8;

#define SOUND_SLOT_2d55c8(id) ((SoundSlot_2d55c8 *)(D_0013E550 + (id) * 0x70))

typedef struct {
    u8 pad0[0x40];
    float velocity[4];
    u8 pad50[0x108];
    float turn_speed;
    int sound;
    void *linked[2];
    int point;
    float target_speed;
    u8 pad170[0x10];
    float speed;
} PathMoverVars_2d55c8;

void FUN_L08_002d55c8(struct Moby *moby, int path_id) {
    PathMoverVars_2d55c8 *vars = (PathMoverVars_2d55c8 *)moby->pvars;
    int *path = (int *)D_L08_001B0CB0[path_id];
    float point[4];
    float step[3];
    float scale;
    float angle;
    float speed;
    float *heading;

    qcopy(point, path + 4 + vars->point * 4);
    heading = &moby->rot.z;
    angle = fast_atan2_l08(point[0] - moby->pos.x, point[1] - moby->pos.y);
    approach_angle_l08(heading, &vars->turn_speed, angle,
        frame_time_sq * 4.712389f, frame_time_sq * 4.712389f, frame_time * 3.1415927f);
    approach_value_l08(&vars->speed, vars->target_speed, frame_time_sq + frame_time_sq);
    step[0] = fast_cos_l08(moby->rot.z) * vars->speed;
    step[1] = fast_sin_l08(moby->rot.z) * vars->speed;
    step[2] = 0.0f;
    add_vector_l08(&moby->pos, &moby->pos, step);
    moby->pos.z = ground_height_l08(&moby->pos, 0, 0.5f);
    if (distance_vector_l08(&moby->pos, point) < 0.5f) {
        vars->point = (vars->point + 1) % *path;
    }
    scale = vars->speed / (D_L08_001618A0 * frame_time);
    if (scale > 2.0f) {
        scale = 2.0f;
    } else if (scale < 0.0f) {
        scale = 0.0f;
    }
    if (vars->linked[0] != 0) {
        char *linked;

        *(float *)((char *)vars->linked[0] + 0x58) = scale;
        linked = vars->linked[0];
        qcopy(linked + 0x40, (char *)moby + 0x40);
        qcopy(linked + 0x10, &moby->pos);
    }
    if (vars->linked[1] != 0) {
        char *linked;

        *(float *)((char *)vars->linked[1] + 0x58) = scale;
        linked = vars->linked[1];
        qcopy(linked + 0x40, (char *)moby + 0x40);
        qcopy(linked + 0x10, &moby->pos);
    }
    speed = length_vector_l08(vars->velocity);
    if (frame_time * 0.1f < speed) {
        if (sound_valid_l08(moby, vars->sound) == 0) {
            vars->sound = play_sound_l08(1, 4, moby);
            set_sound_volume_l08(vars->sound, 1);
        } else if (SOUND_SLOT_2d55c8(vars->sound)->volume < 0x400) {
            set_sound_volume_l08(vars->sound, SOUND_SLOT_2d55c8(vars->sound)->volume + 0x400 / scale_game_frames_l08(60));
        }
    } else if (sound_valid_l08(moby, vars->sound) != 0) {
        if (SOUND_SLOT_2d55c8(vars->sound)->volume < 0x100) {
            stop_sound_l08(vars->sound);
            vars->sound = -1;
        } else {
            set_sound_volume_l08(vars->sound, SOUND_SLOT_2d55c8(vars->sound)->volume - 0x400 / scale_game_frames_l08(60));
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D6C58), where it is exact; names translated to the US level program. */

extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern f32 fast_cos_c(f32) __asm__("func_001F9DC8");
extern f32 random_angle_radians_c(void) __asm__("func_00213308");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern float D_0015ED6C;
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern int D_L08_001618D4 __attribute__((sda));
extern int D_L08_001618D8 __attribute__((sda));
extern float D_L08_001618DC __attribute__((sda));
extern float D_L08_001618E0 __attribute__((sda));
extern float D_L08_001618E8 __attribute__((sda));
extern int D_L08_001618EC __attribute__((sda));
extern int D_L08_001618F0 __attribute__((sda));
extern int D_L08_001618F4 __attribute__((sda));
extern float D_L08_001618F8 __attribute__((sda));
extern float D_L08_001618FC __attribute__((sda));
extern int FUN_L00_0024f7c8();
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void scale_vector_xyz_c(void *, void *, float) __asm__("FUN_001f9a68");
extern void func_L00_00250800_2D6C58(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void func_0020DAF8_2D6C58(char *, int, char *) __asm__("FUN_0020cca8");
extern float func_00214158_2D6C58(void) __asm__("FUN_00213308");
extern float func_002140F8_2D6C58(float, float) __asm__("FUN_002132a8");
extern void func_L00_001FF4B0_2D6C58(void *, void *, float) __asm__("FUN_001f9bf8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern float func_001F9F90_2D6C58(float) __asm__("FUN_001f9dc8");
extern float func_001F9FA8_2D6C58(float) __asm__("FUN_001f9de0");
extern float func_001F9878_2D6C58(float) __asm__("FUN_001f96b0");
extern int func_001FA898_2D6C58(float) __asm__("FUN_001fa6d0");
extern char *func_00219780_2D6C58(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");

void FUN_L08_002d5950(char *moby, int joint, int count) {
    char *data = *(char **)(moby + 0x78);
    char t10[32];
    float t30[8];
    float joint_pos[4];
    float velocity[4];
    float t70[4];
    float jitter[4];
    float spawn_pos[4];
    int i;
    int frames1, frames2, frames3;
    float random_angle, offset;

    func_L00_00250800_2D6C58(moby, joint, joint_pos);
    func_0020DAF8_2D6C58(moby, joint, t10);
    for (i = 0; i < count; i++) {
        float *spawn_ptr = spawn_pos;

        random_angle = func_00214158_2D6C58();
        qcopy(spawn_ptr, joint_pos);
        spawn_pos[0] += func_002140F8_2D6C58(-0.05f, 0.05f);
        spawn_pos[1] += func_002140F8_2D6C58(-0.05f, 0.05f);
        spawn_pos[2] += func_002140F8_2D6C58(-0.05f, 0.05f);
        qcopy(velocity, data + 0x40);
        qcopy(t70, data + 0x40);
        if (count / 3 < i) {
            func_L00_001FF4B0_2D6C58(jitter, t30, func_002140F8_2D6C58(D_L08_001618DC, D_L08_001618DC * 1.2f) * D_0015ED6C);
            fast_vec_add(velocity, velocity, jitter);
            fast_vec_scale(t70, t70, 0.5f);
            offset = func_001F9F90_2D6C58(random_angle);
            offset = offset * (func_002140F8_2D6C58(0.0f, D_L08_001618E8) * D_0015ED6C);
            t70[0] = t70[0] + offset;
            offset = func_001F9FA8_2D6C58(random_angle);
            offset = offset * (func_002140F8_2D6C58(0.0f, D_L08_001618E8) * D_0015ED6C);
            t70[1] = t70[1] + offset;
            t70[2] = D_L08_001618E0 * D_0015ED6C;
        } else {
            func_L00_001FF4B0_2D6C58(jitter, t30, func_002140F8_2D6C58(0.0f, D_L08_001618DC) * D_0015ED6C);
            fast_vec_add(velocity, velocity, jitter);
            fast_vec_scale(t70, t70, 0.5f);
            offset = func_001F9F90_2D6C58(random_angle);
            offset = offset * (func_002140F8_2D6C58(0.0f, D_L08_001618E8 * 0.25f) * D_0015ED6C);
            t70[0] = t70[0] + offset;
            offset = func_001F9FA8_2D6C58(random_angle);
            offset = offset * (func_002140F8_2D6C58(0.0f, D_L08_001618E8 * 0.25f) * D_0015ED6C);
            t70[1] = t70[1] + offset;
            t70[2] = D_L08_001618E0 * D_0015ED6C;
        }
        velocity[3] = D_L08_001618F8;
        t70[3] = D_L08_001618FC;
        frames1 = func_001FA898_2D6C58(func_001F9878_2D6C58(func_002140F8_2D6C58((float)D_L08_001618EC, (float)D_L08_001618EC * 1.2f)));
        frames2 = func_001FA898_2D6C58(func_001F9878_2D6C58(func_002140F8_2D6C58((float)D_L08_001618F0, (float)D_L08_001618F0 * 1.2f)));
        frames3 = func_001FA898_2D6C58(func_001F9878_2D6C58(func_002140F8_2D6C58((float)D_L08_001618F4, (float)D_L08_001618F4 * 1.5f)));
        func_00219780_2D6C58(joint_pos, velocity, t70, D_L08_001618D4, D_L08_001618D8, frames1, frames2, frames3, -1);
    }
}
/* Spawns debris mobys while the level counter allows, then scatters or bursts the group's other mobys. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D7010), where it is exact; names translated to the US level program. */

typedef int u128_7010 __attribute__((mode(TI)));

typedef struct { char pad0[0x21]; unsigned char group; char pad22[0xDE]; } Blk_7010;

typedef struct { char pad0[0x140]; float x,y,z,w; } Camera46;

extern Camera46 D_L08_00167480;
extern char *D_L08_0015FFD8;
extern char *D_L08_001600EC;
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern f32 fast_arc_tan_c(f32, f32) __asm__("func_001F9E90");
extern f32 fast_cos_c3(f32) __asm__("func_001F9DC8");
extern f32 random_angle_radians_c4(void) __asm__("func_00213308");
extern f32 random_float_between_c4(f32, f32) __asm__("func_002132A8");
extern float fast_difference_between_rotations_c2(float, float) __asm__("func_001FA688");
extern int D_L08_0015FFBC;
extern s32 random_integer_below_c2(s32) __asm__("func_00213260");
extern short *D_L08_001AC040[];
extern void FUN_L00_0025e450_c2(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L05_00318648(void*,int,void*,void*,int);
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");

void FUN_L08_002d5d08(char *moby) {
    float a[4];
    float b[4];
    float c[4];
    char *d = *(char **)(moby + 0x78);
    short *list = D_L08_001AC040[((Blk_7010 *)D_L08_0015FFD8 + *(int *)(d + 0x144))->group];
    char *other;
    unsigned short index;
    int i;

    if (list == 0) return;
    index = *list & 0x7FFF;
    other = (char *)((Blk_7010 *)D_L08_0015FFD8 + index);
    for (i = 0; i < 40 && D_L08_0015FFBC >= 50; i++) {
        char *m;
        if (random_integer_below_c2(0xFF) & 1) m = func_0020D348_m(0x310); else m = func_0020D348_m(0x311);
        if (m != 0) {
            float ang;
            *(short *)(m + 0x32) = 0x40;
            *(unsigned char *)(m + 0x30) = 0xFF;
            m[0x31] = 1;
            *(float *)(m + 0x40) = func_00214158_2D6C58();
            *(float *)(m + 0x44) = func_00214158_2D6C58();
            *(float *)(m + 0x48) = func_00214158_2D6C58();
            a[0] = random_float_between_c4(-1.0f, 1.0f);
            a[1] = random_float_between_c4(-1.0f, 1.0f);
            a[2] = random_float_between_c4(-1.0f, 1.0f);
            a[3] = 1.0f;
            transform_vector(a, a, D_L08_001600EC + (*(int *)(d + 0x148) << 7));
            qcopy(m + 0x10, a);
            ang = random_float_between_c4(-0.2617994f, 0.2617994f);
            clear_u64_value(b);
            c[0] = fast_cos_c3(ang) * (D_0015ED6C_lui * 15.0f);
            c[1] = fast_sin(ang) * (D_0015ED6C_lui * 15.0f);
            c[2] = 0.0f;
            c[2] = random_float_between_c4(D_0015ED6C_lui * 8.0f, D_0015ED6C_lui * 12.0f);
            FUN_L05_00318648(m, (int)moby, c, b, func_001FA898_2D6C58(random_float_between_c4(60.0f, 90.0f)));
        }
    }
    while (1) {
        if (*(unsigned char *)(other + 0x20) < 0x7F && other != moby) {
            unsigned short cls = *(unsigned short *)(other + 0xA6);
            if ((unsigned short)(cls - 0x24C) < 0xB) {
                float ang;
                float lo, hi;
                ang = random_float_between_c4(-0.2617994f, 0.2617994f);
                a[0] = fast_cos_c3(ang) * (D_0015ED6C_lui * 15.0f);
                a[1] = fast_sin(ang) * (D_0015ED6C_lui * 15.0f);
                a[2] = 0.0f;
                a[2] = random_float_between_c4(D_0015ED6C_lui * 8.0f, D_0015ED6C_lui * 12.0f);
                switch ((short)(*(unsigned short *)(other + 0xA6) - 0x24C)) {
                case 0:
                    *(u128_7010 *)c = 0; c[0] = 1.629f; c[1] = -1.341f; c[2] = 11.48f;
                    qcopy(b, c); break;
                case 1:
                    *(u128_7010 *)c = 0; c[0] = 1.299f; c[1] = -5.558f; c[2] = 11.48f;
                    qcopy(b, c); break;
                case 2:
                    *(u128_7010 *)c = 0; c[0] = 0.629f; c[1] = -2.527f; c[2] = 3.409f;
                    qcopy(b, c); break;
                case 3:
                    *(u128_7010 *)c = 0; c[0] = 0.019f; c[1] = -6.19f; c[2] = 3.409f;
                    qcopy(b, c); break;
                case 4:
                    *(u128_7010 *)c = 0; c[0] = 3.196f; c[1] = -2.105f; c[2] = 7.784f;
                    qcopy(b, c); break;
                case 5:
                    *(u128_7010 *)c = 0; c[0] = 2.11f; c[1] = -3.013f; c[2] = 2.625f;
                    qcopy(b, c); break;
                case 6:
                    *(u128_7010 *)c = 0; c[0] = 4.33f; c[1] = -6.449f; c[2] = 7.397f;
                    qcopy(b, c); break;
                case 7:
                    *(u128_7010 *)c = 0; c[0] = 3.286f; c[1] = -6.925f; c[2] = 2.96f;
                    qcopy(b, c); break;
                case 8:
                    *(u128_7010 *)c = 0; c[0] = 1.539f; c[1] = -1.071f; c[2] = 13.144f;
                    qcopy(b, c); break;
                case 9:
                    *(u128_7010 *)c = 0; c[0] = 1.596f; c[1] = -2.512f; c[2] = 8.359f;
                    qcopy(b, c); break;
                case 10:
                    *(u128_7010 *)c = 0; c[0] = 0.885f; c[1] = -5.824f; c[2] = 8.224f;
                    qcopy(b, c); break;
                }
                if (fast_difference_between_rotations_c2(fast_arc_tan_c(a[0], a[1]),
                                  fast_arc_tan_c(D_L08_00167480.x - *(float *)(moby + 0x10),
                                                    D_L08_00167480.y - *(float *)(moby + 0x14))) < 0.2617994f) {
                    FUN_L05_00318648(other, (int)moby, a, b, func_001FA898_2D6C58(random_float_between_c4(10.0f, 20.0f)));
                } else {
                    FUN_L05_00318648(other, (int)moby, a, b, func_001FA898_2D6C58(random_float_between_c4(30.0f, 45.0f)));
                }
            } else if ((short)cls == 0x1D8) {
                clear_u64_value(a);
                FUN_L00_0025e450_c2(other, a, other + 0x10, 0.0f, 0.0f, 0x32, 0x3C, 0x3C, 12.0f, 8.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
                mark_moby_for_removal(other);
            }
        }
        if (*list++ < -1) break;
        index = *list & 0x7FFF;
        other = (char *)((Blk_7010 *)D_L08_0015FFD8 + index);
    }
}
extern char *D_L08_0015FFD8;
extern short *D_L08_001AC040[];

/* Remove active mobys of the selected classes from this moby's linked group. */
void FUN_L08_002d6450(struct Moby *moby) {
    short *list = D_L08_001AC040[(((struct Moby *)D_L08_0015FFD8) + *(s32 *)(moby->pvars + 0x144))->group];
    struct Moby *other;
    u16 index;

    if (list != 0) {
        index = *list & 0x7fff;
        other = (struct Moby *)D_L08_0015FFD8 + index;
        while (1) {
            if (other->state < 0x7f && other != moby) {
                u16 class_id = other->oclass;
                if ((u32)(class_id - 0x24c) < 0xb) {
                    mark_moby_for_removal(other);
                } else if ((s16)class_id == 0x1d8) {
                    mark_moby_for_removal(other);
                }
            }
            if (*list++ < -1)
                break;
            index = *list & 0x7fff;
            other = (struct Moby *)D_L08_0015FFD8 + index;
        }
    }
}

/* Splashes the hero into the water: once inits the water plane, queues the
 * surface draw, and when the hero breaks the surface spawns spray and ripples. */

extern int D_L08_00161970 __attribute__((sda));
extern float D_L08_001D7060[4];
extern f32 D_L08_0015F580[] __attribute__((section(".sdata")));
extern void FUN_L02_002a40d0(void *, float);
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void FUN_L08_002da0d0(void *);
extern int FUN_L00_0028d8c0(struct Moby *, int);
extern void release_voice_slot(int) __asm__("FUN_0022d798");
extern char *FUN_L00_002d7e90(void *, float);
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern int FUN_L00_00257b90(int, int);
extern int random_integer_below(int) __asm__("FUN_00213260");
extern void FUN_L00_0026f548(void *, void *, int, int);
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern unsigned char *FUN_L00_002718d0(void *, void *, void *, float, float);
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");

void FUN_L08_002da0f0(struct Moby *moby) {
    Vec4 pos;
    Vec4 v;
    char *r;
    int i;
    int n;
    float sc, sp;

    if (D_L08_00161970 == 0) {
        D_L08_00161970 = 1;
        FUN_L02_002a40d0(D_L08_001D7060, 0.6666667f);
    }
    enqueue_callback_list_1(FUN_L08_002da0d0, moby);
    if (current_level_index != 8)
        return;
    if (!(hero.motion.pos.f[2] < D_L08_001D7060[2]))
        return;
    if (!(D_L08_001D7060[2] <= hero.motion.pos.f[2] - hero.motion.displacement.f[2]))
        return;
    if (FUN_L00_0028d8c0(hero.moby, hero.health.unk22B2)) {
        release_voice_slot(hero.health.unk22B2);
        hero.health.unk22B2 = -1;
    }
    qcopy(&pos, &hero.motion.pos);
    pos.f[2] = D_L08_001D7060[2];
    r = FUN_L00_002d7e90(&pos, 3.0f);
    if (r)
        r[0x23] = 0x70;
    for (i = 0; i < 16; i++) {
        float ang = random_angle_radians();
        float k = random_float_between(frame_time * 0.0f, frame_time * 3.0f);
        v.f[0] = fast_cos(ang) * k;
        v.f[1] = fast_sin(ang) * k;
        v.f[2] = random_float_between(frame_time * 3.0f, frame_time * 6.5f);
        n = FUN_L00_00257b90(0x5A, 0x78);
        FUN_L00_0026f548(&pos, &v, random_integer_below(2), n);
    }
    for (i = 0; i < 16; i++) {
        unsigned char *q;
        random_vector(v.f, 1.0f, 1.0f);
        add_vector_xyz(&v, &v, &pos);
        v.f[2] = D_L08_001D7060[2] + 0.05f;
        sc = random_float_between(0.7f, 1.0f);
        sp = i == 0 ? 2.0f : -2.0f;
        q = FUN_L00_002718d0(&v, D_L08_0015F580, &D_L08_001D7060[2], sc, sp);
        if (q)
            *(short *)(q + 0xA) = truncate_float_to_s32(multiply_global_factor_ed64(random_float_between(30.0f, 60.0f)));
    }
}
extern float D_L08_00161998 __attribute__((sda));
extern float D_L08_001619A0 __attribute__((sda));
extern u16 D_L08_001619A8 __attribute__((sda));
extern void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern unsigned char *FUN_L00_0026d000(void *, float, float, float, int, void *, float, int);
extern char *FUN_L00_0026cbb0(void *, void *, int, int, int, int, float);

/* Strings count glowing puffs from one point to another, growing from size0
 * to size1, then bursts a spark shower at each end. */
void FUN_L08_002da3f0(struct Moby *moby, void *from, void *to, int count, f32 size0, f32 size1) {
    Vec4 step;
    Vec4 pos;
    Vec4 vel;
    f32 grow;
    f32 size;
    unsigned char *puff;
    unsigned char *tail;
    int alpha, grey;
    int i;

    subtract_vector_xyz(&step, to, from);
    scale_vector_xyz(&step, &step, 1.0f / (f32)count);
    grow = (size1 - size0) / (f32)count;
    vel.q = 0;
    vel.f[3] = 1.0f;
    vel.f[2] = 0.01f;
    for (i = 0; i < count; i++) {
        scale_vector_xyz(&pos, &step, convert_integer_to_float(i));
        add_vector_xyz(&pos, from, &pos);
        vel.f[0] = randf_sym(0.0f, 0.005f);
        vel.f[1] = randf_sym(0.0f, 0.005f);
        vel.f[2] = random_float_between(D_L08_00161998 * 0.1f, D_L08_00161998);
        alpha = FUN_L00_00257b90(0x30, 0x70);
        grey = FUN_L00_00257b90(0x30, 0x7F);
        grey |= (grey << 16) | (grey << 8);
        size = random_float_between(1.0f, 1.02f);
        puff = FUN_L00_0026d000(&pos, D_L08_001619A0, 1.0f, size, FUN_L00_00257b90(-2, 2), &vel,
                                grow + size0 * 210000.0f, (alpha << 24) | grey);
        if (puff) {
            tail = puff + 0x20;
            if (random_integer_below(2)) {
                puff[3] = 0x44;
                grey = FUN_L00_00257b90(0x60, 0xE0);
                grey |= (grey << 16) | (grey << 8);
                *(u32 *)(puff + 4) = (alpha << 24) | grey;
            }
            *(u16 *)(puff + 0xA) = D_L08_001619A8;
            *(s32 *)(tail + 4) = 2;
            tail[0xA] = alpha;
            tail[0xB] = D_L08_001619A8;
        }
    }
    vel.f[0] = fast_cos(random_angle_radians()) * 0.05f;
    vel.f[1] = fast_sin(random_angle_radians()) * 0.05f;
    vel.f[2] = 0.0f;
    vel.f[2] = random_float_between(0.01f, 0.03f);
    FUN_L00_0026cbb0(from, &vel, 0x4F007FFF, 0x1FFFFFFF,
                     FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(20)), 1, 10000.0f);
    vel.f[0] = fast_cos(random_angle_radians()) * 0.05f;
    vel.f[1] = fast_sin(random_angle_radians()) * 0.05f;
    vel.f[2] = 0.0f;
    vel.f[2] = random_float_between(0.01f, 0.03f);
    FUN_L00_0026cbb0(to, &vel, 0x4F007FFF, 0x1FFFFFFF,
                     FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(20)), 1, 20000.0f);
}
/* Picks the best candidate moby from the moby's list by heading and distance.
 * Exact since tools/ps2eeas_nops.py puts the hazard nop after the shared bc1fl's label. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DBD88), where it is exact; names translated to the US level program. */

extern char *D_L08_0015FFD8;
extern float FUN_001f9b48(void *, void *);
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");

char *FUN_L08_002daa10(struct Moby *moby) {
    short *t = D_L08_001AC040[moby->group];
    char *best;
    char *p;
    float bestD, bestA;
    int idx;
    struct Hero *base;
    if (t == 0)
        return 0;
    best = 0;
    idx = *(unsigned short *)t & 0x7FFF;
    p = D_L08_0015FFD8 + (idx << 8);
    bestA = 0.0f;
    bestD = bestA;
    for (;;) {
        if (*(short *)(p + 0xA6) != 0x14D && *(unsigned char *)(p + 0xBC) == 0) {
            float ang, d, a;
            a = fast_arc_tan(*(float *)(p + 0x10) - moby->pos.x,
                             *(float *)(p + 0x14) - moby->pos.y);
            base = &hero;
            ang = fast_difference_between_rotations(
                a, fast_arc_tan(base->motion.pos.f[0] - moby->pos.x,
                                base->motion.pos.f[1] - moby->pos.y));
            d = FUN_001f9b48(&moby->pos, p + 0x10);
            if (ang < 1.5707964f) {
                if (bestA < ang) {
                    best = p;
                    bestD = d;
                    bestA = ang;
                }
            } else if ((d >= 16.0f && (d < bestD || bestD < 16.0f)) || (d < 16.0f && bestD < d)) {
                best = p;
                bestD = d;
                bestA = ang;
            }
        }
        if (*t < -1)
            return best;
        t++;
        idx = *(unsigned short *)t & 0x7FFF;
        p = D_L08_0015FFD8 + (idx << 8);
    }
}
/* Update function of the blarg bomb thrower: hit reactions, then a state machine that walks to its bomb crate, picks up and throws bombs at the hero. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DBF68), where it is exact; names translated to the US level program. */

typedef struct { char pad0[0x20]; struct Moby *moby; } Hit_2D5FA8_c;

typedef struct { char pad0[0x140]; float x, y; char pad148[0x10]; float f158; } Cam_DBF68;

typedef struct { char pad0[0x40]; struct Moby *target; int result; } Look_DBF68;

typedef struct {
    char pad0[0x20];
    float health;
    char pad24[5];
    unsigned char b29;
    char pad2A[0xE];
    int i38;
    char pad3C[4];
    float v40[4];
    char pad50[8];
    unsigned char b58;
    char pad59;
    unsigned char b5A;
    char pad5B[5];
    short anim[3];
    char pad66;
    unsigned char tint;
    char pad68[8];
    char blk70[0x10];
    float f80, f84, f88, f8C;
    int pad90;
    int i94;
    char pad98[0x15];
    unsigned char bAD;
    char padAE[0x12];
    float fC0, fC4;
    char padC8[8];
    char blkD0[8];
    float fD8, fDC;
    char padE0[0x14];
    float fF4;
    char padF8[0x14];
    float f10C, f110, f114;
    char pad118[8];
    struct Moby *waypoint;
    struct Moby *p124;
    int i128;
    char blk12C[4];
    short s130, s132;
    int i134;
    char pad138[0x28];
    char blk160[0x10];
} Data_DBF68;

typedef struct { char pad0[0x140]; float x,y,z,w; } Camera46_c;

char *FUN_L08_002d22e0_c(int arg, char *pos, char *vec) __asm__("FUN_L08_002d22e0");
char *FUN_L08_002daa10_c(struct Moby *moby) __asm__("FUN_L08_002daa10");
extern f32 convert_integer_to_float_c(s32) __asm__("func_001FA6C0");
extern f32 fast_arc_tan_c2(f32, f32) __asm__("func_001F9E90");
extern f32 fast_cos_c4(f32) __asm__("func_001F9DC8");
extern f32 fast_vec_length_c2(void *) __asm__("func_001F9AF0");
extern float D_L08_00161978_gp __asm__("D_L08_00161978") __attribute__((sda));
extern float D_L08_0016197C_gp __asm__("D_L08_0016197C") __attribute__((sda));
extern float D_L08_00161988_gp __asm__("D_L08_00161988") __attribute__((sda));
extern float D_L08_0016198C_gp __asm__("D_L08_0016198C") __attribute__((sda));
extern float D_L08_00161990_gp __asm__("D_L08_00161990") __attribute__((sda));
extern float D_L08_001675C0[];
extern float FUN_001f96b0(float);
extern float FUN_L00_0025abf0(void *, void *, void *, float, float);
extern float fast_difference_between_rotations_c3(float, float) __asm__("func_001FA688");
extern float fast_vec_dist(void *, void *);
extern float vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern int D_L08_001619AC_gp __asm__("D_L08_001619AC") __attribute__((sda));
extern int D_L08_001619D0_gp __asm__("D_L08_001619D0") __attribute__((sda));
extern int FUN_L00_00259028(char *m, char *p, float *pos, char *v);
extern int FUN_L00_0025c698(void *, void *);
extern int FUN_L08_0026a3d0(void *, float, void *);
extern s32 scale_game_frames_c2(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32_c4(f32) __asm__("func_001FA6D0");
extern void FUN_L00_00258278(void*,void*,float,float,float,float);
extern void FUN_L00_0025a120(void *);
extern void FUN_L00_0025c558(void *, void *, float, int, int, int);
extern void FUN_L00_0025d458_c2(void *, short *) __asm__("FUN_L00_0025d458");
extern void FUN_L00_0025d538_c2(void *, void *) __asm__("FUN_L00_0025d538");
extern void FUN_L00_0025f730(void*,float);
extern void FUN_L01_0026d930(char *p);
extern void FUN_L04_002c46d0(void *);
extern void set_vector_length_xy(void *, void *, f32) __asm__("FUN_001f9c48");
f32 compute_interpolated_record_value(void *anim) __asm__("FUN_0020c9e0");
void FUN_L00_00257470(unsigned char *m, int flags, int arg);
void FUN_L00_00263ac8(float x, int a, int b, unsigned char *p);
extern Hit_2D5FA8_c *find_2D5FA8_c(struct Moby *, int, int) __asm__("FUN_L00_0025a420");
extern int FUN_00213928(struct Moby *, Hit_2D5FA8_c *, float *, int, int *, float *, int, int);
extern void FUN_00212f90(struct Moby *, int, int, int);
extern void explode_2D5FA8_c(struct Moby *, int, int) __asm__("FUN_L00_00257470");
extern void effects_start_2D5FA8_c(struct Moby *, short *) __asm__("FUN_L00_0025d458");
extern void effects_update_2D5FA8_c(struct Moby *, short *) __asm__("FUN_L00_0025d538");
extern void func_L00_00264B40_x(struct Moby *, int, float, char *) __asm__("FUN_L00_00263ac8");
extern float FUN_0020c9e0(struct Moby *);
extern void func_L00_002592B0_x(float, void *, void *, float, float, float) __asm__("FUN_L00_00258278");
extern char camera_state_bytes[] __asm__("D_L08_00167480");

void FUN_L08_002dabf0(struct Moby *moby) {
    Data_DBF68 *vars = (Data_DBF68 *)moby->pvars;
    Look_DBF68 search;
    float point[4];
    float velocity[4];
    float aim[4];
    int hit;
    float damage;
    float scratch;
    struct Moby *target;
    struct Moby *old_waypoint;

    damage = 0.0f;
    func_L00_00264B40_x(moby, D_L08_001619AC_gp, 3.0f, vars->blk160);
    if (moby->visible != 0) {
        if (fast_vec_dist(&moby->pos, D_L08_001675C0) < 29.0f) {
            FUN_L00_0025a120(moby);
            moby->shadow_range = 0x17;
        }
    }
    if (moby->state == 3) {
        moby->flags &= 0xFBFF;
    } else {
        moby->flags |= 0x400;
    }
    if (moby->state == 100) {
        float duration = convert_integer_to_float_c(D_L08_001619D0_gp);
        int start_alpha = vars->s132;
        moby->alpha = func_001FA898_2D6C58((float)start_alpha + (float)-start_alpha * ((float)(D_L08_001619D0_gp - vars->s130) / duration));
        FUN_L04_002c46d0(moby);
        return;
    }
    FUN_00213928(moby, find_2D5FA8_c(moby, 0x330000, 0), &vars->health, 0, &hit, &damage, 0, 4);
    if (hit != 1 && moby->state != 0x63) {
        moby->flags |= 0x1000;
        vars->health -= damage;
        if (0.0f < vars->health) {
            if (damage < 1.0f) {
                vars->tint = 100;
            } else {
                char *hero_bytes;
                vars->f80 = frame_time_sq * 30.0f;
                vars->f88 = frame_time * 5.0f;
                vars->f8C = frame_time * 7.0f;
                vars->i94 = 9;
                vars->f84 = 0.0005f;
                vars->bAD = 0;
                hero_bytes = (char *)&hero;
                FUN_L00_0025c558(moby, vars->blk70,
                                  fast_arc_tan_c2(moby->pos.x - *(float *)(hero_bytes + 0x80),
                                                    moby->pos.y - *(float *)(hero_bytes + 0x84)),
                                  6, 1, 0);
                vars->fC0 = 6.0f;
                vars->fC4 = 10.0f;
                vars->tint = 0x78;
                moby->state = 0x5A;
            }
        } else {
            char *hero_bytes;
            vars->i94 = 9;
            vars->f84 = 0.0005f;
            vars->f80 = frame_time_sq * 30.0f;
            vars->f88 = D_L08_0016198C_gp * frame_time;
            vars->f8C = D_L08_00161990_gp * frame_time;
            vars->bAD = 0;
            hero_bytes = (char *)&hero;
            FUN_L00_0025c558(moby, vars->blk70,
                              fast_arc_tan_c2(moby->pos.x - *(float *)(hero_bytes + 0x80),
                                                moby->pos.y - *(float *)(hero_bytes + 0x84)),
                              9, 1, 0);
            vars->fC0 = 6.0f;
            vars->fC4 = 10.0f;
            explode_2D5FA8_c(moby, 0, -1);
            vars->tint = 0xFA;
            moby->state = 0x63;
            moby->flags &= 0xEFFF;
            vars->waypoint->command = 0;
        }
        effects_start_2D5FA8_c(moby, vars->anim);
    }
    moby->hit_slot = 0xFF;
    search.result = FUN_L08_0026a3d0(moby, 16.0f, &search);
    if (search.target == 0) {
        search.target = *(struct Moby **)&D_001413D0;
    }
    target = search.target;
    switch (moby->state) {
    case 0:
        moby->flags |= 0x1000;
        vars->b29 = 1;
        vars->health = 2.0f;
        FUN_L01_0026d930(vars->blkD0);
        vars->fDC = 2.0f;
        vars->fD8 = 2.0f;
        vars->f10C = 0.03f;
        vars->f110 = 0.3f;
        vars->f114 = 0.25f;
        vars->fF4 = D_L08_00161978_gp * frame_time;
        vars->b58 = func_001FA898_2D6C58(17.0f);
        vars->b5A = func_001FA898_2D6C58(10.4f);
        clear_u64_value(vars->v40);
        vars->waypoint = (struct Moby *)FUN_L08_002daa10_c((char *)moby);
        vars->waypoint->command = 1;
        vars->i128 = scale_game_frames_c2(0xB4);
        if (moby->prev_seq != 4) {
            FUN_00212f90(moby, 4, 0, 0);
        }
        moby->state = 1;
        vars->i134 = scale_game_frames_c2(0xF0);
        break;
    case 1:
        FUN_L00_00259028((char *)moby, vars->blkD0, (float *)&vars->waypoint->pos, (char *)vars->v40);
        if (fast_vec_dist(&moby->pos, &vars->waypoint->pos) < 3.0f) {
            if (fast_difference_between_rotations_c3(moby->rot.z,
                              fast_arc_tan_c2(vars->waypoint->pos.x - moby->pos.x,
                                                vars->waypoint->pos.y - moby->pos.y)) < 0.2617994f) {
                if (6.0f < vector_distance_xy_alt(&moby->pos, &target->pos)) {
                    if (moby->prev_seq != 5) {
                        FUN_00212f90(moby, 5, 0, 10);
                    }
                    moby->state = 6;
                    break;
                }
                vars->i134 = scale_game_frames_c2(0xF0);
                {
                    struct Moby *old_waypoint = vars->waypoint;
                    struct Moby *next_waypoint = (struct Moby *)FUN_L08_002daa10_c((char *)moby);
                    vars->waypoint = next_waypoint;
                    if (next_waypoint == 0) {
                        vars->waypoint = old_waypoint;
                    }
                    old_waypoint->command = 0;
                    vars->waypoint->command = 1;
                }
                break;
            }
        }
        if (tick_countdown_32_alt(&vars->i134) != 0) {
            old_waypoint = vars->waypoint;
            if ((vars->waypoint = (struct Moby *)FUN_L08_002daa10_c((char *)moby)) == 0) {
                vars->waypoint = old_waypoint;
            }
            old_waypoint->command = 0;
            vars->waypoint->command = 1;
            vars->i134 = scale_game_frames_c2(0xF0);
        }
        break;
    case 6:
        FUN_L00_00259028((char *)moby, vars->blkD0, (float *)&vars->waypoint->pos, (char *)vars->v40);
        if (moby->anim_flags & 2) {
            FUN_00212f90(vars->waypoint, 1, 0, scale_game_frames_c2(6));
            qcopy(&moby->pos, &vars->waypoint->pos);
            moby->pos.z -= 0.1f;
            if (moby->prev_seq != 1) {
                FUN_00212f90(moby, 1, 0, 10);
            }
            moby->state = 3;
            moby->flags &= 0xEFFF;
            moby->rot.z = fast_arc_tan_c2(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
            vars->i128 = scale_game_frames_c2(0x78);
            moby->collision = 0;
        }
        break;
    case 3: {
        float dist = vector_distance_xy_alt(&moby->pos, &target->pos);
        func_L00_002592B0_x(fast_arc_tan_c2(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                            moby, vars->blk12C, 0.02f, 0.3f, 0.2f);
        if (dist < 20.0f && 8.0f < dist && tick_countdown_32_alt(&vars->i128) != 0) {
            Cam_DBF68 *cam = (Cam_DBF68 *)camera_state_bytes;
            if (fast_difference_between_rotations_c3(cam->f158, fast_arc_tan_c2(moby->pos.x - cam->x, moby->pos.y - cam->y)) < 0.55850536f) {
                goto attack;
            }
        }
        if (vars->i38 == 0 || !(0.5f < dist)) {
            break;
        }
    attack:
        FUN_00212f90(vars->waypoint, 1, 0, scale_game_frames_c2(6));
        if (moby->prev_seq != 2) {
            FUN_00212f90(moby, 2, 0, scale_game_frames_c2(7));
        }
        moby->state = 4;
        moby->flags |= 0x1000;
        moby->collision = moby->pclass->collision;
        break;
    }
    case 4:
        point[0] = fast_cos_c4(moby->rot.z) * 2.0f;
        point[1] = fast_sin(moby->rot.z) * 2.0f;
        point[2] = 0.0f;
        add_vector_xyz(point, point, &vars->waypoint->pos);
        if (0.2f < vector_distance_xy_alt(&moby->pos, point)) {
            FUN_L00_00259028((char *)moby, vars->blkD0, point, (char *)vars->v40);
        }
        if (moby->anim_flags & 2) {
            Cam_DBF68 *cam;
            if (vector_distance_xy_alt(&moby->pos, &target->pos) < 6.0f
                || (cam = (Cam_DBF68 *)camera_state_bytes,
                    0.55850536f < fast_difference_between_rotations_c3(cam->f158, fast_arc_tan_c2(moby->pos.x - cam->x, moby->pos.y - cam->y)))) {
                old_waypoint = vars->waypoint;
                if ((vars->waypoint = (struct Moby *)FUN_L08_002daa10_c((char *)moby)) == 0) {
                    vars->waypoint = old_waypoint;
                }
                old_waypoint->command = 0;
                vars->waypoint->command = 1;
                if (moby->prev_seq != 4) {
                    FUN_00212f90(moby, 4, 0, scale_game_frames_c2(7));
                }
                moby->state = 1;
                vars->i134 = scale_game_frames_c2(0xF0);
            } else {
                if (moby->prev_seq != 3) {
                    FUN_00212f90(moby, 3, 0, scale_game_frames_c2(6));
                }
                moby->state = 5;
            }
        }
        break;
    case 5:
        func_L00_002592B0_x(fast_arc_tan_c2(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                            moby, vars->blk12C, 0.02f, 0.3f, 0.2f);
        if (moby->seq == moby->prev_seq && FUN_0020c9e0(moby) == 19.0f) {
            float distance, time;
            char *next;
            subtract_vector_xyz(aim, &target->pos, &moby->pos);
            aim[2] = 0.0f;
            distance = fast_vec_length_c2(aim);
            time = distance / FUN_001f96b0(D_L08_0016197C_gp);
            distance = distance - (D_L08_00161988_gp * time + 2.0f);
            if (10.0f < distance) {
                distance = 10.0f;
            } else if (distance < 3.0f) {
                distance = 3.0f;
            }
            FUN_L00_0025f730(aim, distance);
            add_vector_xyz(aim, aim, &moby->pos);
            func_L00_00250800_2D6C58(moby, 9, point);
            subtract_vector_xyz(velocity, &target->pos, point);
            set_vector_length_xy(velocity, velocity, time);
            velocity[2] = FUN_L00_0025abf0(point, aim, &scratch, time, -(frame_time_sq * 15.0f));
            next = FUN_L08_002d22e0_c((int)moby, (char *)point, (char *)velocity);
            vars->p124 = (struct Moby *)next;
            if (next != 0) {
                char *next_vars = *(char **)(next + 0x78);
                *(int *)(next_vars + 0x20) = scale_game_frames_c2(0xB4);
                *(short *)(next_vars + 0xC) = scale_game_frames_c2(10);
                *(short *)(next_vars + 0xE) = scale_game_frames_c2(10);
                vars->p124->state = 1;
                vars->p124->command |= 3;
                vars->i128 = scale_game_frames_c2(0xB4);
            }
            break;
        }
        if (moby->anim_flags & 2) {
            moby->state = 1;
            if (moby->prev_seq != 4) {
                FUN_00212f90(moby, 4, 0, 3);
            }
            vars->i134 = scale_game_frames_c2(0xF0);
        }
        break;
    case 0x5B:
        if (moby->anim_flags & 2) {
            if (moby->prev_seq != 7) {
                FUN_00212f90(moby, 7, 0, 3);
            }
            moby->state = 0x5D;
        }
        break;
    case 0x5A:
    case 0x5C:
    case 0x5D:
        if (FUN_L00_0025c698(moby, vars->blk70) & 0x40) {
            old_waypoint = vars->waypoint;
            if ((vars->waypoint = (struct Moby *)FUN_L08_002daa10_c((char *)moby)) == 0) {
                vars->waypoint = old_waypoint;
            } else {
                old_waypoint->command = 0;
                vars->waypoint->command = 1;
            }
            if (moby->prev_seq != 4) {
                FUN_00212f90(moby, 4, 0, 10);
            }
            moby->state = 1;
            vars->i134 = scale_game_frames_c2(0xF0);
        }
        if (moby->pos.z < 5.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case 0x63:
        if (FUN_L00_0025c698(moby, vars->blk70) & 0x40) {
            moby->anim_speed = 0.0f;
            moby->state = 100;
            moby->collision = 0;
            vars->s132 = 0x28;
            vars->s130 = D_L08_001619D0_gp;
            FUN_L04_002c46d0(moby);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    effects_update_2D5FA8_c(moby, vars->anim);
    vars->i38 = 0;
}
/* Pvars of the hero-carrying lift. */
typedef struct {
    u8 pad0[0x20];
    u8 motion[0x40];     /* 0x20: carried-moby motion state for FUN_L00_00260738 */
    Vec4 home_pos;       /* 0x60 */
    Vec4 home_rot;       /* 0x70 */
    s32 path;            /* 0x80: -1 when it has no path */
    u8 pad84[8];
    s32 stop;            /* 0x8C: -1 when no stop is selected */
    u8 pad90[0x10];
    u8 manip;            /* 0xA0 */
    u8 manip_set;        /* 0xA1 */
    u8 padA2[0xE];
    u8 spin_state[0x30]; /* 0xB0 */
    s16 unkE0;           /* 0xE0 */
    u8 padE2[0x1E];
    s32 unk100;          /* 0x100 */
    u8 pad104[0xC];
    Vec4 spin;           /* 0x110: x is the wheel angle */
    s32 active;          /* 0x120 */
    s32 ridable;         /* 0x124 */
} LiftVars;

typedef struct {
    s32 busy;
    u8 pad4[0x20];
    s32 owner;           /* 0x24: -1 when free */
} LiftLock;

extern s32 D_L08_0015F5C4;
extern s32 D_L08_0015F594 __attribute__((sda));
extern LiftLock D_L08_00179C10;
extern s32 D_0013CAE4[];
extern void attach_manipulator(void *, s32, void *) __asm__("FUN_0020cb10");
extern void FUN_L00_00259f50(void *, void *);
extern void FUN_L00_00261d78(s32, float, void *, void *);
extern f32 distance_xyz(void *, void *) __asm__("FUN_001f9b48");
extern void FUN_L00_00260738(void *, void *, void *, void *);
extern void FUN_L08_002dc4a0(struct Moby *);
void FUN_L08_002dc0c8(struct Moby *moby);
void FUN_L08_002dc180(struct Moby *moby);
int FUN_L08_002dc648(struct Moby *m);
void FUN_L08_002dc7f0(struct Moby *moby);

/* Lift update: runs the lift's state machine (0 setup, 2 carry the hero
 * when riding, 3 travel to the selected stop) and moves anything riding it
 * by how far it moved this frame. */
void FUN_L08_002dbdb0(struct Moby *moby) {
    LiftVars *vars = (LiftVars *)moby->pvars;
    Vec4 old_pos;
    Vec4 old_rot;
    Vec4 delta;
    Vec4 p;

    qcopy(&old_pos, &moby->pos);
    qcopy(&old_rot, &moby->rot);
    if (D_L08_0015F5C4 != 2 && vars->ridable != 0) {
        moby->visible = 1;
        moby->flags &= ~1;
    } else {
        moby->visible = 0;
        moby->flags |= 1;
    }
    if (vars->manip_set == 0)
        attach_manipulator(moby, 0, &vars->manip);
    vars->spin.f[0] = fast_add_rotations(vars->spin.f[0], frame_time * 1.5707964f);
    FUN_L00_00259f50(vars->spin_state, &vars->spin);
    switch (moby->state) {
    case 0:
        clear_u64_value(&vars->spin);
        if (vars->path == -1) {
            qcopy(&vars->home_pos, &moby->pos);
            qcopy(&vars->home_rot, &moby->rot);
            moby->state = 4;
            vars->ridable = 1;
        } else {
            vars->unk100 = -1;
            vars->unkE0 = -1;
            FUN_L08_002dc180((char *)moby);
            moby->state = 2;
            moby->update_dist = 0xFF;
        }
        break;
    case 2:
        if (hero.ground_moby == moby) {
            if (D_L08_00179C10.busy == 0 && D_L08_00179C10.owner == -1) {
                qcopy(&p, &hero.motion.pos);
                FUN_L00_00261d78(vars->stop, 0.5f, &p, &p);
                if (distance_xyz(&hero.motion.pos, &p) < 0.001f) {
                    FUN_L08_002dc7f0((char *)moby);
                    if ((D_0013CAE4[0] & 0x10) && hero.air_frames.s == 0 && D_L08_0015F594 == 10)
                        moby->state = 3;
                }
            }
        } else {
            FUN_L08_002dc180((char *)moby);
        }
        break;
    case 3:
        hero.unk1F2 = 5;
        hero.unk1F4 = 5;
        hero.unk22DA = vars->stop;
        if (FUN_L08_002dc648((char *)moby)) {
            FUN_L08_002dc180((char *)moby);
            moby->state = 2;
        }
        break;
    }
    FUN_L08_002dc4a0(moby);
    if (vars->stop != -1)
        FUN_L08_002dc0c8((char *)moby);
    subtract_vector_xyz(&delta, &moby->pos, &old_pos);
    FUN_L00_00260738(vars->motion, &delta, &old_rot, &moby->rot);
}
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DD440), where it is exact; names translated to the US level program. */

extern char D_L08_001DB280[];
extern int D_L08_001B0CB0[];
extern void FUN_001f9cf8(void *, void *, void *);

void FUN_L08_002dc0c8(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int *t = (int *)D_L08_001B0CB0[*(int *)(data + 0x8C)];
    if (*t == 8) {
        char *q = D_L08_001DB280;
        char *mat = &moby->rot_row0;
        char *pos = &moby->pos;
        char *p = (char *)t + 0x10;
        int i;
        for (i = 7; i >= 0; i--) {
            FUN_001f9cf8(p, q, mat);
            q += 0x10;
            add_vector_xyz(p, p, pos);
            p += 0x10;
        }
    }
}

#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DD4F8), where it is exact; names translated to the US level program. */

typedef struct {
    int count;
    int pad[3];
    float e[1][4];
} Route_2dd4f8;

extern float FUN_001f9b80(void *, void *);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int D_L08_001B0CB0[];
#include "rnc/gameplay/state/game_globals.h"
extern int D_L08_0015F5CC; /* no foreign declaration */
extern Route_2dd4f8 *D_L08_001B0FB0_r[] __asm__("D_L08_001B0CB0");

void FUN_L08_002dc180(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    char *pl = (char *)&hero.motion.pos;
    Route_2dd4f8 *a = D_L08_001B0FB0_r[*(int *)(data + 0x88)];
    Route_2dd4f8 *b = D_L08_001B0FB0_r[*(int *)(data + 0x80)];
    Route_2dd4f8 *c = D_L08_001B0FB0_r[*(int *)(data + 0x84)];
    float d0 = distance_xyz(pl, (char *)a + (a->count << 4));
    float d1 = distance_xyz(pl, b->e[0]);
    float d2 = distance_xyz(pl, c->e[0]);
    int sel = 0;
    float y;
    if (d1 < d0) {
        d0 = d1;
        sel = 1;
    }
    if (d2 < d0) sel = 3;
    if (*(int *)(data + 0x100) == sel) return;
    *(int *)(data + 0x100) = sel;
    *(int *)(data + 0xE4) = 0;
    *(float *)(data + 0x104) = 1.0f;
    moby->visible = 1;
    moby->flags &= 0xFFFE;
    *(int *)(data + 0x124) = 1;
    moby->collision = moby->pclass->collision;
    switch (sel) {
    case 0: {
        int n = a->count;
        unsigned char *t;
        int f94, f98;
        int fr;
        qcopy(data + 0x60, (char *)((n << 4) + (int)a));
        y = fast_arc_tan(*(float *)((char *)a + ((n - 2) << 4) + 0x10) - *(float *)(data + 0x60),
                              *(float *)((char *)a + ((n - 2) << 4) + 0x14) - *(float *)(data + 0x64));
        fr = current_level_index << 4;
        t = visited52;
        *(float *)(data + 0x78) = y;
        *(float *)(data + 0xE4) = 1.0f;
        *(int *)(data + 0x104) = 0;
        f94 = t[*(int *)(data + 0x94) + fr] == 0xFF;
        f98 = t[*(int *)(data + 0x98) + fr] == 0xFF;
        if (f98) {
            *(Route_2dd4f8 **)(data + 0x120) = c;
        } else if (f94) {
            *(Route_2dd4f8 **)(data + 0x120) = b;
        } else if (32.0f < FUN_001f9b80(&moby->pos, pl) || D_L08_0015F5CC < 5) {
            moby->visible = 0;
            moby->flags |= 1;
            *(int *)(data + 0x124) = 0;
            moby->collision = 0;
        }
        return;
    }
    case 1:
        qcopy(data + 0x60, b->e[0]);
        y = fast_arc_tan(b->e[1][0] - *(float *)(data + 0x60), b->e[1][1] - *(float *)(data + 0x64));
        *(Route_2dd4f8 **)(data + 0x120) = b;
        break;
    case 2:
        qcopy(data + 0x60, a->e[0]);
        y = fast_arc_tan(a->e[1][0] - *(float *)(data + 0x60), a->e[1][1] - *(float *)(data + 0x64));
        *(Route_2dd4f8 **)(data + 0x120) = a;
        break;
    case 3:
        qcopy(data + 0x60, c->e[0]);
        y = fast_arc_tan(c->e[1][0] - *(float *)(data + 0x60), c->e[1][1] - *(float *)(data + 0x64));
        *(Route_2dd4f8 **)(data + 0x120) = c;
        break;
    default:
        return;
    }
    *(float *)(data + 0x78) = y;
}
/* Per-frame update: ease the angle and position fields toward their targets. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DD9C0), where it is exact; names translated to the US level program. */

extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("FUN_00213f38");
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern float D_0015ED70;
extern void sample_camera_path(void *, s32, void *, void *, s32, f32) __asm__("FUN_00214e58");

int FUN_L08_002dc648(struct Moby *m) {
    char *d = (char *)m->pvars;
    int *path;
    float a = D_0015ED70 * 0.1f;
    float b = D_0015ED6C_lui * 0.08f;
    path = *(int **)(d + 0x120);
    advance_accelerated_scalar((float *)(d + 0xE4), (float *)(d + 0xE8), *(float *)(d + 0x104), a,
                                 a, b);
    *(float *)(d + 0x110) =
        fast_add_rotations(*(float *)(d + 0x110), *(float *)(d + 0xE8) * 50.0f);
    sample_camera_path(path, 0, d + 0x60, (float *)(d + 0x70), 0,
                         *(float *)(d + 0xE4) * (float)(*path - 1));
    *(int *)(d + 0x74) = 0;
    if (absolute_float(*(float *)(d + 0xE4) - *(float *)(d + 0x104)) < 0.2f) {
        advance_accelerated_scalar((float *)(d + 0x108), (float *)(d + 0x10C), 3.14159f,
                                     D_0015ED70 * 2.0943952f, D_0015ED70 * 2.0943952f,
                                     D_0015ED6C_lui * 6.2831855f);
    } else {
        *(int *)(d + 0x108) = 0;
    }
    if (*(float *)(d + 0x104) == 0.0f) {
        *(float *)(d + 0x78) = fast_add_rotations(*(float *)(d + 0x78), 3.14159f);
    }
    *(float *)(d + 0x78) = fast_add_rotations(*(float *)(d + 0x78), *(float *)(d + 0x108));
    return *(float *)(d + 0xE4) == *(float *)(d + 0x104);
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DDB68), where it is exact; names translated to the US level program. */

extern int D_L08_001B0CB0[];

void FUN_L08_002dc7f0(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    if (*(float *)(data + 0x104) > 0.9f) {
        try_set_help_message(0xA, 0x1F4D);
    } else if (*(int *)(data + 0x120) == D_L08_001B0CB0[*(int *)(data + 0x80)]) {
        try_set_help_message(0xA, 0x1F50);
    } else if (*(int *)(data + 0x120) == D_L08_001B0CB0[*(int *)(data + 0x88)]) {
        try_set_help_message(0xA, 0x1F4F);
    } else {
        try_set_help_message(0xA, 0x1F4E);
    }
}
/* Spawns a moby of class 0x1A9 at a position, with a random spin and a heading from a vector. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DDC18), where it is exact; names translated to the US level program. */

char *FUN_L08_002dc8a0(char *src, char *pos, char *vec) {
    char *moby = func_0020D348_m(0x1A9);
    char *data;
    if (moby) {
        data = *(char **)(moby + 0x78);
        *(unsigned char *)(moby + 0x30) = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 1;
        *(char **)(data + 0x20) = src;
        *(int *)(data + 0x10) = scale_game_frames(0x12C);
        *(float *)(data + 0x1C) = 1.0f;
        *(int *)(data + 0x14) = 0;
        qcopy(moby + 0x10, pos);
        qcopy(data, vec);
        *(float *)(moby + 0x40) = random_float_between(-180.0f, 180.0f) * DEG_TO_RAD;
        *(float *)(moby + 0x48) = fast_arc_tan(*(float *)vec, *(float *)(vec + 4)) + 1.5707964f;
        FUN_L00_00250df8(moby);
    }
    return moby;
}
extern int FUN_L08_002dc9a8_collision(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern int FUN_L08_002dc9a8_trace(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void FUN_L08_002dc9a8_transform(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_L08_002dc9a8_setup(void *) __asm__("FUN_001f99f8");
extern int FUN_L08_002dc9a8_test(void *) __asm__("FUN_001f9740");
extern void FUN_L08_002dc9a8_add(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void FUN_L08_002dc9a8_scale(float, void *, void *) __asm__("FUN_001f9bf8");
extern float FUN_L08_002dc9a8_random(float, float) __asm__("FUN_002132a8");
extern float FUN_L08_002dc9a8_random_word(void) __asm__("FUN_00213308");
extern float FUN_L08_002dc9a8_sin(float) __asm__("FUN_001f9dc8");
extern float FUN_L08_002dc9a8_cos(float) __asm__("FUN_001f9de0");
extern int FUN_L08_002dc9a8_random_below(int) __asm__("FUN_00213260");
extern int FUN_L08_002dc9a8_random_color(int) __asm__("FUN_001f96f8");
extern int FUN_L08_002dc9a8_blend(int, int) __asm__("FUN_L00_00257b90");
extern int FUN_L08_002dc9a8_color(float, int, int) __asm__("FUN_L08_00264930");
extern void FUN_L08_002dc9a8_emit(void *, void *, int, int, float, int, int) __asm__("FUN_L00_0026bed0");
extern void FUN_L08_002dc9a8_special(void *, float, float, float) __asm__("FUN_00214db0");
extern void FUN_L08_002dc9a8_start(int, int, void *) __asm__("FUN_0022da68");
extern void FUN_L08_002dc9a8_finish(void *) __asm__("FUN_0020c828");
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern float D_0015ED70;
#include "rnc/overlay/collision.h"
extern int D_L08_00174658_2DC9A8 __asm__("D_L08_00174658") __attribute__((section(".data")));

typedef struct __attribute__((packed)) EffectVectorCopy_2DC9A8 {
    unsigned long long values[2];
} EffectVectorCopy_2DC9A8;

typedef struct EffectRecord_2DC9A8 {
    float pos[4];
    float velocity[4];
    void *owner;
    int flags;
    int unknown;
    float scale;
    int state;
} EffectRecord_2DC9A8;

extern CollisionHit D_L08_00174640;
void FUN_L08_002dc9a8(struct Moby *moby) {
    char *m = (char *)moby;
    char *data = *(char **)(m + 0x78);
    float original[4];
    EffectRecord_2DC9A8 record;
    float *position;
    float effect[4];
    float velocity[4];
    float frame_time;
    float radius;
    int i, axis;
    float angle;

    if (FUN_L08_002dc9a8_collision(m + 0x10, 0.5f, 0, m, 0) != 0 &&
        D_L08_00174658_2DC9A8 != 0 &&
        (D_L08_00174658_2DC9A8 != *(int *)(data + 0x20) || *(int *)(data + 0x14) != 0)) {
        m[0x20] = 99;
    }
    switch ((unsigned char)m[0x20]) {
    case 1: {
        position = (float *)(m + 0x10);
        qcopy(original, position);
        FUN_L08_002dc9a8_transform(position, position, data);
        *(float *)(data + 8) -= D_0015ED70 * 10.8f;
        qcopy(record.pos, position);
        record.owner = m;
        record.flags = 0x810000;
        record.state = 1;
        record.scale = 1.0f;
        qcopy(record.velocity, data);
        if (FUN_L08_002dc9a8_trace(original, &record, 0, m, 0) != 0) {
            if (*(int *)(data + 0x20) != 0 &&
                D_L08_00174640.moby != *(int *)(data + 0x20)) {
                m[0x20] = 99;
                qcopy(position, D_L08_00174640.point.f);
                FUN_L08_002dc9a8_add(data, data, &D_L08_00174640.normal_x);
                FUN_L08_002dc9a8_scale(D_0015ED6C_lui + D_0015ED6C_lui, data, data);
            }
        } else {
            if (FUN_L08_002dc9a8_test(data + 0x10) != 0) m[0x20] = 99;
        }
    } break;
    case 99: {
        record.pos[0] = D_0015ED6C_lui * 14.0f;
        record.pos[1] = D_0015ED6C_lui * 22.0f;
        record.pos[2] = D_0015ED6C_lui * 12.0f;
        record.pos[3] = 0.0f;
        *(EffectVectorCopy_2DC9A8 *)original = *(EffectVectorCopy_2DC9A8 *)record.pos;
        record.owner = m;
        record.flags = 0x810001;
        record.scale = 1.0f;
        FUN_L08_002dc9a8_setup(record.velocity);
        record.state = 0;
        FUN_L08_002dc9a8_collision(m + 0x10, 1.0f, 9, m, record.velocity);
        FUN_L08_002dc9a8_start(0, 0, m);
        for (i = 0; i < 50; i++) {
            axis = i % 4;
            *(u128 *)velocity = 0;
            velocity[0] = FUN_L08_002dc9a8_random(D_0015ED6C_lui * -0.5f,
                                                    D_0015ED6C_lui * 0.5f);
            velocity[1] = FUN_L08_002dc9a8_random(D_0015ED6C_lui * -0.5f,
                                                    D_0015ED6C_lui * 0.5f);
            velocity[2] = FUN_L08_002dc9a8_random(D_0015ED6C_lui * 4.0f,
                                                    original[axis]);
            *(u128 *)effect = *(u128 *)velocity;
            radius = FUN_L08_002dc9a8_random(0.0f, 0.25f);
            angle = FUN_L08_002dc9a8_random_word();
            record.pos[0] = FUN_L08_002dc9a8_sin(angle) * radius;
            record.pos[1] = FUN_L08_002dc9a8_cos(angle) * radius;
            record.pos[2] = 0.0f;
            FUN_L08_002dc9a8_transform(record.pos, record.pos, m + 0x10);
            frame_time = radius * D_0015ED6C_lui;
            radius *= 0.717f;
            record.pos[2] -= radius;
            effect[2] -= frame_time * 8.0f;
            position = effect;
            switch (axis) {
            case 0: {
                int color, color2, alpha;
                float size = FUN_L08_002dc9a8_random(200000.0f, 300000.0f);
                color = FUN_L08_002dc9a8_random_color(60);
                color2 = FUN_L08_002dc9a8_random_color(120);
                alpha = FUN_L08_002dc9a8_blend(color, color2);
                FUN_L08_002dc9a8_emit(record.pos, position, 0x1f101820, 0x101010, size, alpha, 0);
            } break;
            case 2: {
                int color, color2, alpha;
                color = FUN_L08_002dc9a8_random_below(100) < 40 ? 0x5ff8f8f8 : 0x2f486078;
                color2 = FUN_L08_002dc9a8_random_color(30);
                alpha = FUN_L08_002dc9a8_random_color(45);
                FUN_L08_002dc9a8_emit(record.pos, position, color, 0x0f000020, 150000.0f,
                                      FUN_L08_002dc9a8_blend(color2, alpha), 2);
            } break;
            case 3: {
                int color, color2, alpha;
                color = FUN_L08_002dc9a8_color(FUN_L08_002dc9a8_random(0.25f, 1.0f),
                                                  0x7f000000, 0x7f182030);
                color2 = FUN_L08_002dc9a8_color(FUN_L08_002dc9a8_random(0.5f, 1.0f),
                                                   0, 0x5f5f5f);
                radius = FUN_L08_002dc9a8_random(0.0f, 1.0f) * D_0015ED6C_lui;
                FUN_L08_002dc9a8_special(position, radius, angle,
                                          FUN_L08_002dc9a8_random_word());
                radius = FUN_L08_002dc9a8_random(150000.0f, 200000.0f);
                alpha = FUN_L08_002dc9a8_random_color(60);
                alpha = FUN_L08_002dc9a8_blend(alpha, FUN_L08_002dc9a8_random_color(120));
                FUN_L08_002dc9a8_emit(record.pos, position, color, color2, radius,
                                      alpha, 3);
            } break;
            case 1: break;
            }
        }
        FUN_L08_002dc9a8_finish(m);
    } break;
    }
}
#include "sda.h"

/* Rocket guidance, route traversal, launch effects, and destruction. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DE738), where it is exact; names translated to the US level program. */

typedef struct {
    int count;
    int pad[3];
    float e[1][4];
} Route_2dd4f8_2dd3c0;

typedef unsigned int Q46 __attribute__((mode(TI)));

typedef float V46[4] __attribute__((aligned(16)));

typedef struct M46 M46;

typedef struct D46 D46;

typedef struct { char pad0[0x10]; int animation; char pad14[0x10]; float scale; } Model46;

struct M46 {
    char pad0[0xC]; float distance; V46 position;
    unsigned char state,group,pad22[2]; Model46 *model;
    float pad28,scale; unsigned char pad30[2]; unsigned short flags;
    char pad34[0xC]; V46 rotation; char pad50[0x28]; D46 *data;
    char pad7C[0x18]; int animation; char pad98[0xE]; unsigned short class_id;
    char padA8[0x18]; V46 matrix[4];
};

struct D46 {
    char pad0[0x20]; V46 velocity; char pad30[0x10];
    float pitch,yaw,roll; M46 *target;
    int waypoint; signed char direction; char pad55[3]; int health; int pad5C;
    Route_2dd4f8_2dd3c0 *path; int path_index; char pad68[0x18];
    float yaw_velocity; int pad84,effect_path; float activation_range;
    int persistent,timer; float speed;
};

typedef struct { int waypoint; signed char direction; char pad5[0xB]; Route_2dd4f8_2dd3c0 *path; } RouteState46;

typedef struct { char pad0[0x2C]; unsigned char active; } Persistent46;


extern Camera46 D_L08_00167480;
extern char *D_L08_0015FFD8_macro __asm__("D_L08_0015FFD8") __attribute__((section(".sdata")));
extern char D_0013D388[] __attribute__((section(".data")));
extern char D_0013F3D0[] __attribute__((section(".data")));
extern f32 distance_xyz_c(void *, void *) __asm__("func_001F9B48");
extern f32 fast_cos_c2(f32) __asm__("func_001F9DC8");
extern f32 random_angle_radians_c2(void) __asm__("func_00213308");
extern f32 random_float_between_c2(f32, f32) __asm__("func_002132A8");
extern float D_0015ED6C __attribute__((section(".sdata")));
extern float frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern float D_L08_00161A00 __attribute__((sda));
extern float D_L08_00161A04 __attribute__((sda));
extern float D_L08_00161A08 __attribute__((sda));
extern float D_L08_00161A0C __attribute__((sda));
extern float D_L08_00161A14 __attribute__((sda));
extern float D_L08_00161A24 __attribute__((sda));
extern float D_L08_00161A28 __attribute__((sda));
extern float FUN_L00_0025bc00(float*,int,float,float);
extern float approach_value(float*,float,float);
extern float fast_difference_between_rotations_c(float, float) __asm__("func_001FA688");
extern int D_L08_0015F5C4_macro __asm__("D_L08_0015F5C4") __attribute__((section(".sdata")));
extern int D_L08_00161A20 __attribute__((sda));
extern int D_L08_00161A2C __attribute__((sda));
extern int D_L08_00161A30 __attribute__((sda));
extern s32 truncate_float_to_s32_c2(f32) __asm__("func_001FA6D0");
extern unsigned int D_L08_0015FFDC __attribute__((section(".sdata")));
extern void FUN_L00_00258278(void*,void*,float,float,float,float);
extern void FUN_L00_0025f090(void*,void*,int,float,float);
extern void FUN_L00_0025f730(void*,float);
extern void FUN_L00_00262cf0(void*,void*,float,float,float);
extern void FUN_L00_00269958(void*,void*,int,int,int,int,int,int);
extern void FUN_L05_00318648(void*,int,void*,void*,int);
extern void FUN_L08_002de528_u(void*,int,int,int) __asm__("FUN_L08_002de528");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void tick_countdown_32(s32 *) __asm__("func_001F9740");
extern Route_2dd4f8_2dd3c0 *D_L08_001B0FB0_r_c[] __asm__("D_L08_001B0CB0");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");
extern int timer46(int*) __asm__("FUN_001f9740");

void FUN_L08_002dd3c0(M46 *moby) {
    V46 target,delta,accel,effect,aux;
    D46 *data=moby->data;
    if (FUN_L00_0025a420(moby,0x210000,0) && moby->state!=4) {
        allocate_voice_for_target_entry_alt(0,0,moby);
        FUN_L00_0025f090(moby,moby->position,-1,0.5f,13.0f);
        mark_moby_for_removal(moby);
        return;
    }
    switch(moby->state) {
    case 0: {
        char *persistence_base=D_0013D388;
        Persistent46 *state=(Persistent46*)(persistence_base+data->persistent);
        if (state->active) {
            short *link=D_L08_001AC040[moby->group];
            do { mark_moby_for_removal(D_L08_0015FFD8_macro+((*(unsigned short*)link&0x7FFF)<<8)); } while (*link++>=-1);
            return;
        }
        if (!data->persistent) data->timer=scale_game_frames_alt(10);
        data->path=D_L08_001B0FB0_r_c[data->path_index];
        qcopy(moby->position,data->path->e[0]);
        data->waypoint=1; moby->state=3; moby->animation=0; moby->flags=0;
        return;
    }
    case 1: {
        D46 *target_data=data->target->data;
        int life,size0,size1;
        float yaw,pitch,radius,angle;
        moby->rotation[0]=fast_add_rotations(moby->rotation[0],D_0015ED6C*6.2831855f);
        qcopy(target,data->target->position); target[2]+=5.0f;
        yaw=fast_arc_tan(moby->position[0]-target[0],moby->position[1]-target[1]);
        pitch=fast_arc_tan(FUN_001f9b80(target,moby->position),moby->position[2]-target[2]);
        FUN_L00_00262cf0(target,target,data->roll,data->yaw,data->pitch);
        subtract_vector_xyz(delta,target,moby->position);
        FUN_L00_0025f730(delta,target_data->speed*D_0015ED6C);
        subtract_vector_xyz(accel,delta,data->velocity);
        FUN_L00_0025f730(accel,D_L08_00161A04*frame_time_sq_macro);
        add_vector_xyz(data->velocity,data->velocity,accel);
        add_vector_xyz(moby->position,moby->position,data->velocity);
        moby->rotation[1]=-fast_arc_tan(vector_length_xy(data->velocity),data->velocity[2]);
        moby->rotation[2]=fast_arc_tan(data->velocity[0],data->velocity[1]);
        if (fast_difference_between_rotations_c(yaw,data->yaw)<0.08726646f && fast_difference_between_rotations_c(pitch,data->pitch)<0.08726646f) moby->state=2;
        radius=random_float_between_c2(0.0f,D_L08_00161A14)*D_0015ED6C;
        angle=random_angle_radians_c2();
        normalize_vector_xyz(effect,moby->matrix[2],radius*fast_cos_c2(angle));
        normalize_vector_xyz(aux,moby->matrix[1],radius*fast_sin(angle));
        add_vector_xyz(effect,effect,aux); qcopy(aux,aux);
        effect[3]=D_L08_00161A24; aux[3]=D_L08_00161A28;
        life=scale_game_frames_alt(D_L08_00161A20);
        size0=func_001FA898_r(D_L08_00161A24); size1=func_001FA898_r(D_L08_00161A28);
        FUN_L00_00269958(moby->position,effect,D_L08_00161A2C,D_L08_00161A30,life,size0,size1,1);
        return;
    }
    case 2: {
        D46 *target_data=data->target->data;
        int life,size0,size1;
        float scale,radius,angle;
        moby->rotation[0]=fast_add_rotations(moby->rotation[0],D_0015ED6C*6.2831855f);
        scale=moby->model->scale*3.0f;
        approach_value(&moby->scale,scale,moby->model->scale/(float)scale_game_frames_alt(180));
        qcopy(target,data->target->position); target[2]+=5.0f;
        subtract_vector_xyz(delta,target,moby->position);
        FUN_L00_0025f730(delta,target_data->speed*D_0015ED6C);
        subtract_vector_xyz(accel,delta,data->velocity);
        FUN_L00_0025f730(accel,D_L08_00161A04*4.0f*frame_time_sq_macro);
        add_vector_xyz(data->velocity,data->velocity,accel);
        add_vector_xyz(moby->position,moby->position,data->velocity);
        moby->rotation[1]=-fast_arc_tan(vector_length_xy(data->velocity),data->velocity[2]);
        moby->rotation[2]=fast_arc_tan(data->velocity[0],data->velocity[1]);
        if (distance_xyz_c(moby->position,data->target->position)<D_L08_00161A0C) {
            target_data->health-=60; clear_u64_value(data->velocity); allocate_voice_for_target_entry_alt(0,0,moby); moby->state=6;
        }
        radius=random_float_between_c2(0.0f,D_L08_00161A14)*D_0015ED6C;
        angle=random_angle_radians_c2();
        normalize_vector_xyz(effect,moby->matrix[2],radius*fast_cos_c2(angle));
        normalize_vector_xyz(aux,moby->matrix[1],radius*fast_sin(angle));
        add_vector_xyz(effect,effect,aux); qcopy(aux,aux);
        effect[3]=D_L08_00161A24; aux[3]=D_L08_00161A28;
        life=scale_game_frames_alt(D_L08_00161A20);
        size0=func_001FA898_r(D_L08_00161A24); size1=func_001FA898_r(D_L08_00161A28);
        FUN_L00_00269958(moby->position,effect,D_L08_00161A2C,D_L08_00161A30,life,size0,size1,1);
        return;
    }
    case 3:
        if (D_L08_0015F5C4_macro) return;
        if (distance_xyz_c(D_0013F3D0,(char*)data->path+(data->path->count<<4))<data->activation_range && timer46(&data->timer)) {
            moby->flags=0x40; moby->state=4; moby->animation=moby->model->animation;
            moby->rotation[2]=fast_arc_tan(data->path->e[1][0]-moby->position[0],data->path->e[1][1]-moby->position[1]);
            allocate_voice_for_target_entry_alt(1,0,moby);
        }
        return;
    case 4: {
        RouteState46 *route;
        int next;
        float yaw;
        if (D_L08_0015F5C4_macro) return;
        route=(RouteState46*)&data->waypoint;
        next=(route->waypoint+route->path->count+route->direction)%route->path->count;
        yaw=fast_arc_tan(*(float*)((char*)route->path+(next<<4)+0x10)-moby->position[0],*(float*)((char*)route->path+(next<<4)+0x14)-moby->position[1]);
        FUN_L00_00258278(moby,&data->yaw_velocity,yaw,0.1f,1.3f,1.0f);
        moby->rotation[1]=fast_arc_tan(FUN_001f9b80(route->path->e[next],moby->position),moby->position[2]-route->path->e[next][2]);
        qcopy_nc(target,route->path->e[next]);
        if (route->waypoint!=route->path->count-1) {
            int life0,life1,size1;
            subtract_vector_xyz(delta,target,moby->position);
            normalize_vector_xyz(delta,delta,D_L08_00161A08*D_0015ED6C);
            add_vector_xyz(moby->position,moby->position,delta);
            if (distance_xyz_c(moby->position,target)<0.5f) route->waypoint=next;
            *(Q46*)effect=(Q46)0;
            effect[0]=random_float_between_c2(-1.0f,1.0f); effect[1]=random_float_between_c2(-1.0f,1.0f); effect[2]=random_float_between_c2(-1.0f,1.0f);
            *(Q46*)accel=*(Q46*)effect; FUN_L00_0025f730(accel,D_0015ED6C+D_0015ED6C);
            life0=scale_game_frames_alt(FUN_L00_00257b90(30,45));
            size1=FUN_L00_00257b90(40,70);
            FUN_L00_00269958(moby->position,accel,0x6F00AFFF,0xFF,life0,40,size1,1);
            *(Q46*)aux=(Q46)0;
            aux[0]=random_float_between_c2(-1.0f,1.0f); aux[1]=random_float_between_c2(-1.0f,1.0f); aux[2]=random_float_between_c2(-1.0f,1.0f);
            *(Q46*)effect=*(Q46*)aux; FUN_L00_0025f730(effect,D_0015ED6C+D_0015ED6C);
            life1=scale_game_frames_alt(FUN_L00_00257b90(60,120));
            size1=FUN_L00_00257b90(100,150);
            FUN_L00_00269958(moby->position,effect,0x1FFFFFFF,0x4F4F4F,life1,40,size1,0);
            return;
        } else {
            short *link=D_L08_001AC040[moby->group];
            long special=0;
            int index;
            M46 *other;
            if (!link) return;
            index=*(unsigned short*)link&0x7FFF;
            other=(M46*)(D_L08_0015FFD8_macro+(index<<8));
            ((Persistent46*)(D_0013D388+data->persistent))->active=1;
            linked_object:
                if (other->state<0x7F && other!=moby) {
                    unsigned short cls=other->class_id;
                    if ((unsigned int)(cls-0x223)<6) {
                        clear_u64_value(accel);
                        switch ((short)(other->class_id-0x223)) {
                        case 0: *(Q46*)aux=(Q46)0; aux[0]=-3.331f; aux[1]=3.331f; aux[2]=0.275f; qcopy(effect,aux); break;
                        case 1: *(Q46*)aux=(Q46)0; aux[0]=-6.0f; aux[1]=6.0f; aux[2]=-1.772f; qcopy(effect,aux); break;
                        case 2: *(Q46*)aux=(Q46)0; aux[0]=-2.0f; aux[1]=-1.152f; aux[2]=-1.772f; qcopy(effect,aux); break;
                        case 3: *(Q46*)aux=(Q46)0; aux[2]=0.975f; qcopy(effect,aux); break;
                        case 4: *(Q46*)aux=(Q46)0; aux[1]=-1.665f; aux[2]=0.642f; qcopy(effect,aux); break;
                        case 5: *(Q46*)aux=(Q46)0; qcopy(effect,aux); break;
                        }
                        if (other->distance>1500.0f) mark_moby_for_removal(other);
                        else {
                            float launch_yaw,center_yaw,min_frames,max_frames,angle_delta;
                            float *launch_offset;
                            subtract_vector_xyz(accel,other->position,moby->position); accel[2]=0.0f;
                            normalize_vector_xyz(accel,accel,D_0015ED6C*12.0f);
                            accel[2]=random_float_between_c2(D_0015ED6C*8.0f,D_0015ED6C*12.0f);
                            other->animation=0;
                            launch_yaw=fast_arc_tan(accel[0],accel[1]);
                            center_yaw=fast_arc_tan(D_L08_00167480.x-moby->position[0],D_L08_00167480.y-moby->position[1]);
                            angle_delta=fast_difference_between_rotations_c(launch_yaw,center_yaw);
                            launch_offset=effect;
                            if (angle_delta<0.7853982f) { min_frames=10.0f; max_frames=20.0f; }
                            else { min_frames=60.0f; max_frames=90.0f; }
                            FUN_L05_00318648(other,(int)moby,accel,launch_offset,func_001FA898_r(random_float_between_c2(min_frames,max_frames)));
                        }
                    } else if ((short)cls==0x1A8) {
                        special=1; clear_u64_value(accel);
                        FUN_L00_0025e450(other,accel,other->position,0.0f,0.0f,50,60,60,12.0f,8.0f,9.0f,1.0f,-1,15.0f,1,1,-1,0);
                        mark_moby_for_removal(other);
                    }
                }
                if (*link++>=-1) {
                    index=*(unsigned short*)link&0x7FFF;
                    other=(M46*)(D_L08_0015FFD8_macro+(index<<8));
                    goto linked_object;
                }
            if (special) FUN_L08_002de528_u(moby,data->effect_path,30,1);
            else FUN_L08_002de528_u(moby,data->effect_path,30,0);
            allocate_voice_for_target_entry_alt(2,0,moby); moby->state=6;
        }
        return;
    }
    case 5:
        FUN_L00_0025bc00(&moby->rotation[2],0,data->yaw,D_0015ED6C*0.5235988f);
        FUN_L00_0025bc00(&moby->rotation[1],0,-data->pitch,D_0015ED6C*0.5235988f);
        moby->rotation[0]=fast_add_rotations(moby->rotation[0],D_0015ED6C*6.2831855f);
        build_spherical_offset(data->velocity,(D_L08_00161A00+D_L08_00161A00)*D_0015ED6C,moby->rotation[2],-moby->rotation[1]);
        add_vector_xyz(moby->position,moby->position,data->velocity);
        if (FUN_L08_002025d0(moby->position,0x400,2,moby,0) || moby->position[0]<5.0f || moby->position[0]>500.0f || moby->position[1]<5.0f || moby->position[1]>500.0f || moby->position[2]<5.0f || moby->position[2]>500.0f || distance_xyz_c(D_0013F3D0,moby->position)<16.0f) moby->state=6;
        return;
    case 6:
        if (D_L08_0015FFDC<(unsigned int)moby) FUN_L00_0025e450(moby,data->velocity,moby->position,0.0f,0.0f,15,10,10,3.0f,2.0f,9.0f,1.0f,-1,15.0f,1,1,-1,0);
        mark_moby_for_removal(moby);
        break;
    default: break;
    }
}
extern struct Moby *create_moby(s32) __asm__("FUN_0020c4f8");
extern float D_L08_001675C0[];
extern float FUN_001f9b80(void *, void *);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern void FUN_L00_002502f0(void *, int, int, int);
extern int D_L08_00161A34 __attribute__((sda));
extern int D_L08_00161A38 __attribute__((sda));
extern int D_L08_00161A3C __attribute__((sda));
extern float D_L08_00161A40 __attribute__((sda));

struct Moby *FUN_L08_002de3e0(void *pos, struct Moby *target, void *velocity, float pitch, float yaw) {
    struct Moby *moby = create_moby(0x1B3);
    char *data;
    float *reference;
    float distance;
    float scale;
    struct Moby *target_copy = target;
    void *velocity_copy = velocity;
    if (moby != 0) {
        data = (char *)moby->pvars;
        qcopy(&moby->pos, pos);
        reference = D_L08_001675C0;
        distance = FUN_001f9b80(reference, &moby->pos);
        reference = (float *)((char *)reference - 0x140);
        moby->rot.y = -fast_arc_tan(distance, reference[82] - moby->pos.z);
        *(volatile float *)&moby->rot.z = fast_arc_tan(reference[80] - moby->pos.x,
                                                        reference[81] - moby->pos.y);
        scale = moby->pclass->scale;
        scale *= D_L08_00161A40;
        moby->update_dist = 0xFF;
        moby->visible = 1;
        *(volatile u8 *)&moby->state = 1;
        moby->draw_dist = 0xFF;
        moby->scale = scale;
        FUN_L00_002502f0(moby, D_L08_00161A34, D_L08_00161A38, D_L08_00161A3C);
        *(float *)(data + 0x40) = pitch;
        *(float *)(data + 0x48) = 70.0f;
        *(float *)(data + 0x44) = yaw;
        *(struct Moby **)(data + 0x4C) = target_copy;
        qcopy(data + 0x20, velocity_copy);
        FUN_L00_00250df8(moby);
    }
    return moby;
}
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DF8A0), where it is exact; names translated to the US level program. */

extern char *D_L08_001600EC;
extern float D_0015ED6C_lui __asm__("D_0015ED6C");
extern f32 random_angle_radians_c3(void) __asm__("func_00213308");
extern f32 random_float_between_c3(f32, f32) __asm__("func_002132A8");
extern s32 random_integer_below_c(s32) __asm__("func_00213260");
extern s32 truncate_float_to_s32_c3(f32) __asm__("func_001FA6D0");
extern void FUN_L00_0025e450_c(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void set_vector_length_xy(void *, void *, f32) __asm__("FUN_001f9c48");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");

void FUN_L08_002de528(char *moby, int a, int n, int flag) {
    float vec[4];
    float vec2[4];
    float vec3[4];
    char *data = *(char **)(moby + 0x78);
    int i;

    if (flag) {
        FUN_L00_0025e450_c(moby, data + 0x20, moby + 0x10, 0.0f, 0.0f, 0x1E, 0x14, 0x14, 6.0f, 4.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
    } else {
        FUN_L00_0025e450_c(moby, data + 0x20, moby + 0x10, 0.0f, 0.0f, 0x2D, 0x1E, 0x1E, 10.0f, 6.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
    }
    for (i = 0; i < n; i++) {
        {
            char *m;
            if (flag) {
                if (random_integer_below_c(0xFF) & 1) m = func_0020D348_m(0x30E); else m = func_0020D348_m(0x30F);
            } else {
                if (random_integer_below_c(0xFF) & 1) m = func_0020D348_m(0x310); else m = func_0020D348_m(0x311);
            }
            *(short *)(m + 0x32) = 0x40;
            m[0x31] = 1;
            m[0x30] = 0x40;
            *(long *)(m + 0x38) = *(long *)(moby + 0x38);
            *(float *)(m + 0x40) = random_angle_radians_c3();
            *(float *)(m + 0x44) = random_angle_radians_c3();
            *(float *)(m + 0x48) = random_angle_radians_c3();
            vec[0] = random_float_between_c3(-1.0f, 1.0f);
            vec[1] = random_float_between_c3(-1.0f, 1.0f);
            vec[2] = random_float_between_c3(-1.0f, 1.0f);
            vec[3] = 1.0f;
            transform_vector(vec, vec, D_L08_001600EC + (a << 7));
            qcopy(m + 0x10, vec);
            clear_u64_value(vec2);
            subtract_vector_xyz(vec3, vec, moby + 0x10);
            set_vector_length_xy(vec3, vec3, random_float_between_c3(D_0015ED6C_lui * 12.0f, D_0015ED6C_lui * 15.0f));
            vec3[2] = random_float_between_c3(D_0015ED6C_lui * 6.0f, D_0015ED6C_lui * 18.0f);
            FUN_L05_00318648(m, (int)moby, vec3, vec2, func_001FA898_r(random_float_between_c3(60.0f, 90.0f)));
        }
    }
}
/* Ported from rac1-decomp src/overlays/l08_batalia/vendor_002B9438.c (func_L08_002DFBC0) */

/* Update function: switch on the state byte at +0x20. State 0 reads the table D_L08_001B0FB0[*data] (deleting
 * the moby if the index is -1), 1 steps along it, 2 falls under gravity and deletes the moby below z = 10.0
 * or on a hit. */

extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float D_0015ED6C;
extern float D_0015ED70;
extern int D_001413D4 __attribute__((section(".data")));
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern float D_L08_00161A44 __attribute__((sda));
extern unsigned char D_0013D408[];
extern void *find_moby_hit(void*,int,int) __asm__("FUN_L00_0025a420");
extern void FUN_L00_0025e450(void *,void *,void *,float,float,int,int,int,float,float,float,float,int,float,int,int,int,int);
extern void FUN_L00_00263d40(int, int);
extern void FUN_L08_002dec90_c(char *m, char *pos) __asm__("FUN_L08_002dec90");
s32 allocate_voice_for_bank_entry(s32 entry_index, s32 flags, s32 owner) __asm__("FUN_0022db10");

void FUN_L08_002de848(char *m) {
    float a[4];
    float b[4];
    char *d = *(char **)(m + 0x78);
    char *pos = m + 0x10;
    int *t;
    qcopy(a, pos);
    t = (int *)D_L08_001B0CB0[*(int *)d];
    switch (*(unsigned char *)(m + 0x20)) {
    case 0:
        if (*(int *)d == -1) {
            mark_moby_for_removal(m);
            return;
        }
        {
            float l0 = distance_xyz((char *)t + 0x10, (char *)t + 0x20);
            distance_xyz((char *)t + 0x10, (char *)t + (*t << 4));
            *(float *)(d + 8) = D_L08_00161A44 * D_0015ED6C / l0;
            *(float *)(d + 4) = *(float *)(d + 4) * (float)*t;
            *(float *)(d + 0xC) = randf_sym(0.0f, 5.0f);
        }
        m[0x20] = 1;
        *(unsigned char *)(m + 0x30) = 0xFF;
    case 1: {
        float f;
        sample_camera_path(t, 1, m + 0x10, (float *)(m + 0x40), 0, *(float *)(d + 4));
        *(float *)(m + 0x44) = *(float *)(m + 0x44) * 0.5f;
        *(float *)(m + 0x18) = *(float *)(m + 0x18) + *(float *)(d + 0xC);
        *(float *)(d + 4) = *(float *)(d + 4) + *(float *)(d + 8);
        f = (float)*t;
        if (f < *(float *)(d + 4)) {
            *(float *)(d + 4) = *(float *)(d + 4) - f;
        }
        break;
    }
    case 2: {
        float c15 = 15.0f;
        float c10 = 10.0f;
        float dd;
        float g;
        char *v = d + 0x10;
        g = D_0015ED6C * 7.3303828f;
        *(float *)(d + 0x18) = *(float *)(d + 0x18) - D_0015ED70 * c15;
        dd = fast_add_rotations(*(float *)(m + 0x40), g);
        *(float *)(m + 0x40) = *(float *)(m + 0x40) + dd;
        *(float *)(m + 0x44) = -fast_arc_tan(vector_length_xy(v), *(float *)(d + 0x18));
        add_vector_xyz(pos, pos, v);
        if (*(float *)(m + 0x18) < c10) {
            mark_moby_for_removal(m);
            return;
        }
        if (coll_sphere(1.25f, pos, 0, m)) {
            FUN_L00_0025e450(m, v, pos, 0.0f, 0.0f, 0x14, 0x28, 0x10, c10, 5.0f, 9.0f, 1.0f, -1, c15, 1, 1, -1, 0);
            mark_moby_for_removal(m);
            return;
        }
        break;
    }
    default:
        break;
    }
    if (*(float *)(m + 0x10) < 4.0f || *(float *)(m + 0x14) < 4.0f || *(float *)(m + 0x18) < 4.0f) {
        *(int *)(m + 0x94) = 0;
    } else {
        *(int *)(m + 0x94) = *(int *)(*(char **)(m + 0x24) + 0x10);
    }
    if (find_moby_hit(m, 0x10000, 0) != 0 && *(unsigned char *)(m + 0x20) != 2) {
        if (D_001413D4 == 0x32 && D_0013D408[0xC] == 0) {
            D_0013D408[0xC] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            FUN_L00_00263d40(0x53D6, -1);
        }
        subtract_vector_xyz(b, m + 0x10, a);
        qcopy(d + 0x10, b);
        *(float *)(d + 0x18) = *(float *)(d + 0x18) + (D_0015ED6C + D_0015ED6C);
        FUN_L00_0025e450(m, b, m + 0x10, 0.0f, 0.0f, 0x14, 0x28, 0x10, 10.0f, 5.0f, 9.0f, 1.0f, -1, 15.0f, 1, 1, -1, 0);
        m[0x20] = 2;
    }
    *(unsigned char *)(m + 0xA4) = 0xFF;
    if (*(unsigned char *)(m + 0x31) != 0) {
        FUN_L08_002dec90_c(m, (char *)a);
    }
}
/* Spawns a burst of particles at an offset from the moby, with random sizes and colours from level tuning values. */

extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern float D_L08_00161A48 __attribute__((sda));
extern float D_L08_00161A4C __attribute__((sda));
extern float D_L08_00161A50 __attribute__((sda));
extern int D_L08_00161A54 __attribute__((sda));
extern int D_L08_00161A58 __attribute__((sda));
extern int D_L08_00161A5C __attribute__((sda));
extern float D_L08_00161A60 __attribute__((sda));
extern float D_L08_00161A64 __attribute__((sda));
extern float D_L08_00161A68 __attribute__((sda));
extern float D_L08_00161A6C __attribute__((sda));
extern float D_L08_00161A70 __attribute__((sda));
extern int D_L08_00161A74 __attribute__((sda));
extern int D_L08_00161A78 __attribute__((sda));
extern int D_L08_00161A7C __attribute__((sda));
extern int D_L08_00161A80 __attribute__((sda));
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void FUN_L08_002dec90(struct Moby *m, char *pos) {
    float v10[4];
    float v20[4];
    float v30[4];
    float v40[4];
    float v50[4];
    float v60[4];
    int a, b, c, d, e;
    char *r;
    subtract_vector_xyz(v30, &m->pos, pos);
    *(OvlQuad *)v60 = 0;
    v60[0] = -2.3f;
    v60[2] = 0.3f;
    transform_vector_by_basis(v50, v60, &m->rot_row0);
    add_vector_xyz(v50, v50, &m->pos);
    scale_vector_xyz(v10, v30, D_L08_00161A48);
    scale_vector_xyz(v20, v30, D_L08_00161A4C);
    random_vector(v40, 0.0f, D_L08_00161A50 * frame_time);
    add_vector_xyz(v20, v20, v40);
    v10[3] = random_float_between(D_L08_00161A64, D_L08_00161A68);
    v20[3] = random_float_between(D_L08_00161A6C, D_L08_00161A70);
    a = fast_tween_color(D_L08_00161A74, D_L08_00161A78,
                     random_float_between(0.0f, 1.0f));
    b = fast_tween_color(D_L08_00161A7C, D_L08_00161A80,
                     random_float_between(0.0f, 1.0f));
    c = func_001FA898_r(
        multiply_global_factor_ed64((float)D_L08_00161A54 * random_float_between(0.0f, 1.0f) + 1.0f));
    d = func_001FA898_r(multiply_global_factor_ed64(
        (float)D_L08_00161A58 *
        (random_float_between(-D_L08_00161A60, D_L08_00161A60) + 1.0f)));
    e = func_001FA898_r(multiply_global_factor_ed64(
        (float)D_L08_00161A5C *
        (random_float_between(-D_L08_00161A60, D_L08_00161A60) + 1.0f)));
    r = spawn_spark(v50, v10, v20, a, b, c, d, e, -1);
    if (r != 0) {
        r[9] = func_001FA898_r(8.0f) - 0x70;
    }
}
