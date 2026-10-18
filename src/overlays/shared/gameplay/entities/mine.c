/* Shared moby classes: mine, nanotech_crate_fragment, gold_devastator_missile. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Mine (moby class 74) update: falls and bounces, sticks to what it hits, homes on a nearby target and explodes. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002AB910.c: func_L00_002AB910), where it is exact; names translated to the US level program. */


typedef float V910[4] __attribute__((aligned(16)));

typedef union {
    OvlQuad q;
    float f[4];
} U910;

typedef struct {
    OvlQuad q[5];
} B910;

typedef struct M910 M910;

typedef struct W910 W910;

struct W910 {
    U910 vel;
    char p10[0x20];
    M910 *owner;
    short h34;
    short h36;
    char p38[8];
    float f40;
    char p44[4];
    int i48;
    M910 *p4C;
    short h50;
    short h52;
    M910 *p54;
    int i58;
    int i5C;
    float f60;
    float f64;
    int i68;
    float f6C;
    float f70;
    int i74;
    int i78;
};

struct M910 {
    char p0[0x10];
    U910 pos;
    unsigned char state;
    char p21[3];
    char *p24;
    char p28[0xC];
    unsigned short flags;
    char p36[0xA];
    float rx;
    float ry;
    float rz;
    char p4C[6];
    unsigned char b52;
    unsigned char anim;
    char p54[0x1C];
    unsigned char b70;
    char p71[7];
    W910 *v;
    char p7C[0x18];
    int i94;
    char p98[0xC];
    unsigned char bA4;
    char pA5;
    short cls;
    char pA8[0x14];
    unsigned char bBC;
    char pBD[3];
    float mtx[4][4];
};

typedef struct {
    float dir[4];
    M910 *moby;
    int flags;
    unsigned char b8;
    unsigned char b9;
    unsigned short cls;
    float fC;
    int i10;
} __attribute__((aligned(16))) Qry910;
typedef struct {
    char p0[0x80];
    float pos[4];
    char p90[0x2FC - 0x90];
    unsigned char *p2FC;
    char p300[0x1090 - 0x300];
    M910 *hero;
    char p1094[0x2080 - 0x1094];
    M910 *cam;
    char p2084[0x20];
    unsigned char b20A4;
    unsigned char b20A5;
    char p20A6[9];
    unsigned char b20AF;
} G910;

typedef struct {
    char p0[0x11];
    unsigned char b11;
} E910;

typedef struct {
    char p0[0x18];
    M910 *moby;
    int i1C;
    U910 pos;
    char p30[0x10];
    U910 nrm;
} H910;

typedef struct {
    char p0[0x160];
    float f160;
    char p164[4];
    int i168;
} Cam910;

typedef struct {
    char p0[0x80];
    f32 x80;
    f32 x84;
    char p1[0x2E4 - 0x88];
    f32 f2E4;
    char p2[0x2080 - 0x2E8];
    char *cam;
} P;

typedef struct {
    char p0[0xB];
    u8 bB;
} E;

extern B910 D_L00_001E9830;
extern B910 D_L00_001E9880;
extern Cam910 D_L00_00166C80;
extern E D_0013E520;
extern M910 *D_L00_00173E58 __attribute__((section(".data")));
extern char *active_mobies[] __asm__("D_L00_001ABA00");
extern char *mine_debris(char *, char *, int, int, float, int, int) __asm__("FUN_L00_0026bed0");
extern char *mine_camera_blast(char *a, char *b, int c, char *d) __asm__("FUN_L00_002d3838");
extern char *FUN_L00_002d7e90(void *, float);
extern char D_L00_00173E70[];
extern s16 D_L00_001B07B0[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 FUN_001f9b48(void *, void *);
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float D_L00_00173E68[];
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float square_root(float) __asm__("FUN_001f9988");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float FUN_L00_00200510(float, float, float);
extern float FUN_L00_00259710(void *);
extern float dot_vectors_xyz(void *a, void *b);
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float random_float_between(float a, float b) __asm__("FUN_002132a8");
extern float vector_length_xyz(void *a);
extern int *D_L00_00177F00[];
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int collision_hit_material(void) __asm__("FUN_001f0b58");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int FUN_L00_001f0d60(float, void *, int, void *);
extern int collect_mobies_in_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern int mine_blend_color(int, int, float) __asm__("FUN_L00_002371e0");
extern int FUN_L00_00257b90(int, int);
extern int FUN_L00_00258490(void *, void *, void *, float, float, float, float, int);
extern int FUN_L00_00260400(void *, void *, void *, void *, void *, void *);
extern int random_integer_below_cf(void *, void *, float, float) __asm__("FUN_L00_0026f080");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int FUN_L00_002aa2a8(W910 *, M910 *, int);
extern int tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern s32 D_L00_00166DC0[];
extern s32 D_L00_00173E40[0x1C];
extern s32 FUN_L00_0025e3b8(s32);
extern s32 allocate_voice_for_target_entry(s32, s32, void *) __asm__("func_0022DA68");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern s32 D_L00_001612D0_gp __asm__("D_L00_001612D0") __attribute__((sda));
extern s32 D_L00_001612D8_gp __asm__("D_L00_001612D8") __attribute__((sda));
extern short D_L00_001613D0 __attribute__((sda));
extern short D_L00_001613D4 __attribute__((sda));
extern unsigned FUN_L00_0025c0e8(unsigned c, int mask);
extern unsigned char *D_L00_001B2080[];
extern unsigned char *FUN_002141f8_cf(unsigned char *) __asm__("FUN_L00_002603d0");
extern int D_L00_0015F5CC;
extern unsigned char *suck_get_aim_height(unsigned char *) __asm__("FUN_002141f8");
extern void *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void *FUN_L00_0026bb70(void *, void *, int, int, float, int, int, int, int);
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void FUN_L00_001ff660(void *, void *, void *);
extern void make_hit_descriptor(void *, void *, int, void *, float) __asm__("FUN_L00_00259888");
extern void jet_animate(unsigned char *, float) __asm__("FUN_L00_00259fe8");
extern void send_blast_hits(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float,
                             float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void mine_spawn_drops(void *, void *) __asm__("FUN_L00_0025f800");
extern void FUN_L00_00262b80(void *, char *, char *, float, float, float);
extern void FUN_L00_0026f548(void *, void *, int, int);
extern void FUN_L00_002738e8(void *, void *, int, float, float, float, int, int, int);
extern void pool_splash(int, int, int, float, float, float, float) __asm__("FUN_L00_002a3ec8");
extern void add_vector_xyz(void *, void *, void *);
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void clear_vector(void *);
extern void copy_matrix3x4(void *, void *);
extern void normalize_vector_xyz(void *, void *, float);
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void scale_vector_xyz(void *, void *, float);
extern void subtract_vector_xyz(void *dst, void *a, void *b);
f32 resolve_camera_surface_height(void *position, s32 optional_output) __asm__("FUN_002135f0");
float absolute_float(float input) __asm__("func_001F99C0");
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern char D_0013E550[];
extern E910 D_E910 __asm__("D_0013E520");
extern H910 coll_hit __asm__("D_L00_00173E40");

void mine_update(M910 *m) __asm__("FUN_L00_002aa670");
void mine_update(M910 *m) {
    B910 a = D_L00_001E9830;
    B910 b = D_L00_001E9880;
    U910 c;
    U910 old;
    U910 up;
    W910 *v;
    M910 *hd;
    int st;

    hd = 0;
    old.q = 0;
    up.q = 0;
    old.f[2] = frame_time + frame_time;
    up.f[2] = 1.0f;
    c.q = old.q;
    v = m->v;
    if ((hero.unk20A5 != 0 || hero.unk20AF != 0) && m->state == 0) {
        m->flags |= 0x41;
    } else {
        m->flags &= 0xFFBE;
    }
    if ((hero.items[0].moby == 0 || ((M910 *)hero.items[0].moby)->cls != 0xBE) && m->state == 0) {
        mark_moby_for_removal(m);
        return;
    }
    if (hero.vehicle_mode != 0 && m->state == 0) {
        mark_moby_for_removal(m);
        return;
    }
    if (v->owner != 0 && v->owner->state != 0xFE && v->owner->state != 0xFD &&
        v->owner->cls == 0xBE && v->h36 == 0 && 0.0f < vector_length_xyz(v)) {
        if (hero.ground_moby == 0 || FUN_002141f8_cf(hero.ground_moby) == 0) {
            FUN_L00_002aa2a8(v, m, 1);
        } else if (FUN_L00_002aa2a8(v, m, 0) == 0) {
            FUN_L00_002aa2a8(v, m, 2);
        }
    }
    st = m->state;
    if (st == 0) {
        if (hero.items[0].moby != 0 && ((M910 *)hero.items[0].moby)->cls == 0xBE) {
            float m1[4][4];
            float m2[4][4];
            U910 rot;
            {
                rot.q = 0;
                rot.f[0] = 3.1415927f;
                euler_rotation_matrix(m1, &rot);
                build_moby_bone_transform((char *)v->owner, 0, (char *)m2);
                multiply_matrix(m1, m2, m1);
                copy_matrix3x4(m->mtx, m1);
                m->i94 = 0;
                m->flags |= 0x100;
            }
            return;
        }
        mark_moby_for_removal(m);
        return;
    }
    {
        Qry910 q;
        {
            float k;
            float dist;
            float gz;

            m->flags &= 0xFEFF;
            qcopy(&old, &m->pos);
            if (!(st & 4)) {
                add_vector_xyz(&m->pos, &m->pos, v);
                if (v->h50 == 0) {
                    v->vel.f[2] -= frame_time_sq * 9.8f;
                } else {
                    v->vel.f[2] -= frame_time_sq * 9.8f / 10.0f;
                    if (random_integer_below(scale_game_frames(6) - 1) == 0) {
                        float wz = resolve_camera_surface_height((float *)&m->pos, &q);

                        scale_vector_xyz(&q, v, 0.25f);
                        random_integer_below_cf(&m->pos, &q, random_float_between(0.05f, 0.1f) * 210000.0f,
                                         wz);
                    }
                }
            }
            q.flags = 0x830000;
            q.fC = 3.0f;
            q.moby = m;
            q.i10 = 1;
            qcopy(&q, v);
            set_vector_length_xy(&q, &q, 1.0f);
            q.b8 = 2;
            q.dir[3] = 5627.925f;
            q.b9 = 1;
            q.cls = m->cls;
            m->pos.f[2] -= 0.4f;
            q.dir[2] = 1.0f;
            k = convert_integer_to_float(D_L00_0015F5CC - v->i74);
            k = k / multiply_global_factor_ed64(*(float *)&D_L00_001613D4);
            if (1.0f < k) {
                k = 1.0f;
            } else if (k < 0.0f) {
                k = 0.0f;
            }
            m->pos.f[2] -= v->f6C * fast_sin(v->f70) * k;
            if (!(m->state & 4) && collision_line(&old, &m->pos, 0x10, (int)m, (int)&q) != 0) {
                if (v->h50 == 0 && collision_hit_material() == 0 && v->vel.f[2] < 0.0f) {
                    V910 t;
                    V910 n;
                    {
                        int i;
                        char *p;

                        pool_splash(*(int *)&D_L00_001612D0_gp, *(int *)&D_L00_001612D8_gp, 1,
                                         m->pos.f[0], m->pos.f[1], 0.5f, -0.35f);
                        v->h50 = 1;
                        clear_vector((float *)v);
                        v->vel.f[2] = frame_time * -1.5f;
                        t[0] = m->pos.f[0];
                        t[1] = m->pos.f[1];
                        t[2] = D_L00_00173E68[0];
                        p = FUN_L00_002d7e90(t, 2.0f);
                        if (p != 0) {
                            p[0x23] = 0x70;
                        }
                        for (i = 0; i < 16; i++) {
                            float ang = random_angle_radians();
                            float rr = random_float_between(frame_time * 0.0f, frame_time * 3.0f);
                            int life;

                            n[0] = fast_cos(ang) * rr;
                            n[1] = fast_sin(ang) * rr;
                            n[2] = random_float_between(frame_time * 3.0f, frame_time * 6.5f);
                            life = FUN_L00_00257b90(0x5A, 0x78);
                            FUN_L00_0026f548(t, n, random_integer_below(2), life);
                        }
                    }
                } else if (v->h50 != 0 || collision_hit_material() != 0) {
                    V910 t;
                    V910 n;
                    V910 r;
                    V910 ov;
                    {
                        if (coll_hit.moby != 0 &&
                            (FUN_L00_0025e3b8((int)coll_hit.moby) != 0 ||
                             (coll_hit.moby->flags & 0x5000)) &&
                            coll_hit.moby->cls != m->cls && coll_hit.moby != hero.moby &&
                            coll_hit.moby != v->owner) {
                            m->bBC = 1;
                            if (m->state != 1) {
                                qcopy(&m->pos, &coll_hit.pos);
                                m->pos.f[2] = probe_ground_height(&m->pos, 0, 0.5f);
                            }
                        } else if (coll_hit.moby != 0 && coll_hit.moby->cls == m->cls) {
                            subtract_vector_xyz(&coll_hit.nrm, &m->pos, &coll_hit.moby->pos);
                            if (v->vel.f[2] < frame_time * -9.8f) {
                                m->bBC = 1;
                                qcopy(&m->pos, &coll_hit.pos);
                                FUN_L00_001ff660(&c, v, &coll_hit.nrm);
                                normalize_vector_xyz(&c, &c, frame_time + frame_time);
                                normalize_vector_xyz(&up, &coll_hit.nrm, 1.0f);
                            } else {
                                qcopy(&m->pos, &coll_hit.pos);
                                normalize_vector_xyz(t, &coll_hit.nrm, frame_time_sq * 9.7f);
                                add_vector_xyz(&m->pos, &m->pos, t);
                                FUN_L00_001ff660(v, v, &coll_hit.nrm);
                                scale_vector_xyz(v, v, 0.4f);
                                normalize_vector_xyz(t, &coll_hit.nrm, 1.0f);
                                dot_vectors_xyz(t, &up);
                                qcopy(n, &coll_hit.nrm);
                                goto orient;
                            }
                        } else if ((coll_hit.moby != 0 && coll_hit.moby != hero.moby &&
                                    coll_hit.moby != v->owner) ||
                                   coll_hit.i1C > 0) {
                            if (v->h50 != 0 || collision_hit_material() == 0 ||
                                v->vel.f[2] < frame_time * -9.8f) {
                                m->bBC = 1;
                                qcopy(&m->pos, &coll_hit.pos);
                                FUN_L00_001ff660(&c, v, &coll_hit.nrm);
                                normalize_vector_xyz(&c, &c, frame_time + frame_time);
                                normalize_vector_xyz(&up, &coll_hit.nrm, 1.0f);
                            } else {
                                if (coll_hit.moby != 0) {
                                    hd = (M910 *)FUN_002141f8_cf((unsigned char *)coll_hit.moby);
                                }
                                qcopy(&m->pos, &coll_hit.pos);
                                normalize_vector_xyz(t, &coll_hit.nrm, frame_time_sq * 9.7f);
                                add_vector_xyz(&m->pos, &m->pos, t);
                                *(OvlQuad *)ov = v->vel.q;
                                FUN_L00_001ff660(v, v, &coll_hit.nrm);
                                scale_vector_xyz(v, v, 0.6f);
                                v->vel.f[2] *= 0.5f;
                                if (v->f6C == 0.0f) {
                                    float ph;

                                    v->f6C = absolute_float(ov[2]) * 2.0f;
                                    ph = -2.3561945f;
                                    if (0.0f < ov[2]) {
                                        ph = 0.7853982f;
                                    }
                                    v->f70 = ph;
                                    v->i74 = D_L00_0015F5CC;
                                }
                                normalize_vector_xyz(t, &coll_hit.nrm, 1.0f);
                                if (0.707f < dot_vectors_xyz(t, &up) && ov[2] < 0.0f) {
                                    float fz = probe_ground_height(&m->pos, 0, 0.5f);

                                    if (m->pos.f[2] < fz) {
                                        m->pos.f[2] = fz;
                                    }
                                    v->vel.f[2] = -v->vel.f[2];
                                    if (vector_length_xyz(v) < frame_time * 0.5f || hd != 0) {
                                        clear_vector((float *)v);
                                        m->state |= 4;
                                        m->i94 = *(int *)(m->p24 + 0x10);
                                        if (coll_hit.moby != 0 && hd != 0) {
                                            v->p4C = coll_hit.moby;
                                        }
                                    }
                                }
                                qcopy(n, &coll_hit.nrm);
                            orient: {
                                r[0] = n[0] * fast_cos(m->rz) + n[1] * fast_sin(m->rz);
                                r[1] = n[1] * fast_cos(m->rz) - n[0] * fast_sin(m->rz);
                                r[2] = n[2];
                                m->rx = FUN_L00_00200510(
                                    m->rx,
                                    -fast_arc_tan(square_root(r[0] * r[0] + r[2] * r[2]), r[1]),
                                    0.05235988f);
                                m->ry =
                                    FUN_L00_00200510(m->ry, fast_arc_tan(r[2], r[0]), 0.05235988f);
                            }
                            }
                        }
                    }
                }
            } else if (m->state & 4) {
                float fz = probe_ground_height(&m->pos, 0, 0.5f);

                if (v->p4C != 0) {
                    if (!(v->p4C != 0 && v->p4C->state != 0xFE && v->p4C->state != 0xFD)) {
                        m->bBC = 1;
                        v->p4C = 0;
                    } else if (D_L00_00173E58 != 0) {
                        hd = (M910 *)FUN_002141f8_cf((unsigned char *)D_L00_00173E58);
                        if (hd != 0) {
                            FUN_L00_00260400(m, v->p4C, &m->pos, &m->rx, &m->pos, &m->rx);
                        }
                    }
                    if (hd == 0) {
                        int s;

                        m->state = 1;
                        s = v->i68;
                        if (s != -1) {
                            char *tab = ((char *)&D_0013E550);
                            char *e = tab + s * 0x70;

                            if (*(M910 **)(e + 0x88) == m && *(unsigned char *)(e + 0x74) != 0) {
                                release_voice_slot(s);
                            }
                        }
                        v->vel.f[2] = 0.0f;
                        v->i68 = -1;
                        v->h34 = scale_game_frames(0x78);
                        v->p4C = 0;
                    }
                } else if (!(m->state & 0x10)) {
                    if (m->pos.f[2] - fz < frame_time_sq * 40.0f) {
                        m->pos.f[2] = fz;
                        v->h34 = 0;
                        if (coll_hit.moby != 0 &&
                            FUN_002141f8_cf((unsigned char *)coll_hit.moby) != 0) {
                            v->p4C = coll_hit.moby;
                        }
                    } else {
                        int s;

                        m->state = 1;
                        s = v->i68;
                        if (s != -1) {
                            char *tab = ((char *)&D_0013E550);
                            char *e = tab + s * 0x70;

                            if (*(M910 **)(e + 0x88) == m && *(unsigned char *)(e + 0x74) != 0) {
                                release_voice_slot(s);
                            }
                        }
                        v->vel.f[2] = 0.0f;
                        v->i68 = -1;
                        v->h34 = scale_game_frames(0x78);
                    }
                }
                if ((v->p54 == 0 || v->p54->state == 0xFE || v->p54->state == 0xFD ||
                     !(v->p54->flags & 0x1000)) &&
                    (m->state & 0x10)) {
                    int s;

                    m->state = 1;
                    s = v->i68;
                    if (s != -1) {
                        char *tab = ((char *)&D_0013E550);
                        char *e = tab + s * 0x70;

                        if (*(M910 **)(e + 0x88) == m && *(unsigned char *)(e + 0x74) != 0) {
                            release_voice_slot(s);
                        }
                    }
                    v->vel.f[2] = 0.0f;
                    v->p54 = 0;
                    v->i68 = -1;
                    v->h34 = scale_game_frames(0x78);
                    if (m->anim != 0) {
                        blend_moby_animation(m, 0, 0, 10);
                    }
                }
                if (v->p54 != 0 && (m->state & 0x10)) {
                    V910 t;
                    V910 n;
                    Qry910 q2;
                    U910 probe;
                    U910 d;
                    {
                        float sp;
                        float lim;
                        int mode;

                        *(OvlQuad *)n = m->pos.q;
                        if ((m->b70 & 2) && m->b52 == 1 && m->anim != 2) {
                            blend_moby_animation(m, 2, 0, 10);
                        }
                        if (voice_is_playing(m, v->i68) == 0) {
                            v->i68 = allocate_voice_for_target_entry(1, 4, (int)m);
                        }
                        sp = v->f40 + frame_time_sq * 15.0f;
                        lim = frame_time * 4.0f;
                        v->f40 = sp;
                        if (lim < sp) {
                            v->f40 = lim;
                        }
                        mode = v->h52;
                        if (mode == 1) {
                            m->pos.f[2] += frame_scale * 0.05f;
                            if (tick_countdown_32_alt(&v->i48) != 0) {
                                v->i48 = scale_game_frames(7);
                                v->h52 = 2;
                            }
                        } else if (mode == 2) {
                            m->pos.f[2] -= frame_scale * 0.05f;
                            if (tick_countdown_32_alt(&v->i48) != 0) {
                                v->h52 = 0;
                            }
                        }
                        subtract_vector_xyz(t, &v->p54->pos, &m->pos);
                        normalize_vector_xyz(t, t, v->f40);
                        add_vector_xyz(&m->pos, &m->pos, t);
                        make_hit_descriptor(&q2, m, 0x10000, t, 1.0f);
                        qcopy(&probe, &m->pos);
                        probe.f[2] += 0.35f;
                        if (collect_mobies_in_sphere(&probe, 0x10, m, &q2, 0.35f) != 0 &&
                            D_L00_00173E58 != 0 && D_L00_00173E58->cls != 0x363 &&
                            D_L00_00173E58->cls != 0x4A && D_L00_00173E58->cls != 0x458 &&
                            D_L00_00173E58->cls != 0x365 && D_L00_00173E58->cls != 0x367) {
                            m->bBC = 1;
                        }
                        qcopy(&probe, &m->pos);
                        probe.f[2] += 0.25f;
                        if (FUN_L00_001f0d60(0.25f, &probe, 0, m) != 0) {
                            subtract_vector_xyz(&d, D_L00_00173E70, &probe);
                            add_vector_xyz(&m->pos, &m->pos, &d);
                        }
                        if (tick_countdown_32_alt(&v->i58) != 0) {
                            m->bBC = 1;
                        }
                    }
                }
                FUN_L00_00262b80(m, (char *)&v->f60, (char *)&v->f64, 0.5235988f,
                                 frame_time * 0.5235988f, frame_time * 0.29670596f);
                if (m->state & 4) {
                    V910 t;
                    {
                        subtract_vector_xyz(t, &m->pos, &old);
                        v->vel.q = *(OvlQuad *)t;
                    }
                }
            }
            if (!(m->state & 2) && fast_dec_timer_s16(&v->h34) != 0) {
                m->state |= 2;
                v->p54 = 0;
            }
            {
                char *res = find_moby_hit(m, 0x830000, 0);

                if (res != 0 && *(M910 **)(res + 0x20) != 0 &&
                    (*(M910 **)(res + 0x20))->cls != m->cls) {
                    m->bBC = 1;
                }
            }
            m->bA4 = 0xFF;
            if (m->state & 2) {
                if (!(m->state & 4) ||
                    (vector_distance(&((M910 *)hero.moby)->pos, &m->pos) < 0.75f &&
                     absolute_float(m->pos.f[2] - hero.motion.pos.f[2]) < 0.25f && (m->state & 0x10))) {
                    m->bBC = 1;
                } else if (FUN_001f9b48(&((M910 *)hero.moby)->pos, &m->pos) < 0.75f) {
                    V910 t;
                    V910 n;
                    V910 r;
                    {
                        subtract_vector_xyz(r, &m->pos, &((M910 *)hero.moby)->pos);
                        *(OvlQuad *)t = *(OvlQuad *)r;
                        normalize_vector_xyz(t, t, 0.75f);
                        add_vector_xyz(r, &((M910 *)hero.moby)->pos, t);
                        *(OvlQuad *)n = *(OvlQuad *)r;
                        FUN_L00_00258490(m, &m->pos, n, 0.4f, 0.25f, 0.25f, 0.5235988f, 0);
                    }
                } else {
                    M910 *best = 0;
                    float range =
                        (convert_integer_to_float(*((unsigned char *)&D_E910 + 0x11)) + 1.0f) * 4.0f;
                    int i;
                    M910 *o;

                    if (v->i78 != 0) {
                        range *= 3.0f;
                    }
                    i = 0;
                    o = (M910 *)active_mobies[i];
                    while (o != 0) {
                        float rg;
                        unsigned char *p;

                        if (30.0f < FUN_001f9b48(&m->pos, &o->pos)) {
                            goto next;
                        }
                        if (FUN_001f9b48(&m->pos, &o->pos) < 1.5f) {
                            m->bBC = 1;
                            break;
                        }
                        if (FUN_L00_0025e3b8((int)o) != 0) {
                            goto next;
                        }
                        rg = range;
                        p = suck_get_aim_height((unsigned char *)o);
                        if (p != 0) {
                            rg = range + convert_integer_to_float(p[0xA]) * 0.125f;
                        }
                        if (!(vector_distance(&m->pos, &o->pos) < rg)) {
                            goto next;
                        }
                        if (!(absolute_float(o->pos.f[2] - m->pos.f[2]) < 2.0f)) {
                            goto next;
                        }
                        if (v->p54 != 0 && v->p54->state != 0xFE && v->p54->state != 0xFD) {
                            goto next;
                        }
                        if (best != 0) {
                            float d1 = vector_distance(&m->pos, &o->pos);

                            if (!(d1 < vector_distance(&m->pos, &best->pos))) {
                                goto next;
                            }
                        }
                        m->state |= 0x10;
                        best = o;
                        blend_moby_animation(m, 1, 0, scale_game_frames(3));
                        allocate_voice_for_target_entry(0, 0, (int)m);
                        v->f40 = 0.0f;
                        v->i58 = scale_game_frames(600);
                        v->h52 = 1;
                        v->i48 = scale_game_frames(7);
                    next:
                        i++;
                        o = (M910 *)active_mobies[i];
                    }
                    if (best != 0) {
                        v->p54 = best;
                    }
                }
            }
            v->i78 = 0;
            if (v->i5C < 0 || *(int *)&D_L00_001613D0 != 0) {
                cart_burst(m, &m->pos, -1, 0.33f, 13.0f);
                mine_spawn_drops(m, D_L00_001B07B0);
                mark_moby_for_removal(m);
            }
            {
                float amp = v->f6C;

                if (0.05f < amp) {
                    float damped = frame_scale * -0.012000024f * amp + amp;

                    v->f6C = damped;
                    if (damped < 0.05f) {
                        v->f6C = 0.05f;
                    }
                }
            }
            k = convert_integer_to_float(D_L00_0015F5CC - v->i74);
            k = k / multiply_global_factor_ed64(*(float *)&D_L00_001613D4);
            if (1.0f < k) {
                k = 1.0f;
            } else if (k < 0.0f) {
                k = 0.0f;
            }
            v->f70 = fast_add_rotations(v->f70, frame_time * 6.2831855f);
            m->pos.f[2] = m->pos.f[2] + v->f6C * fast_sin(v->f70) * k + 0.4f;
            if (m->bBC != 0) {
                dist = vector_distance(&m->pos, D_L00_00166DC0);
                if (v->h50 == 0) {
                    V910 t;
                    {
                        float s = convert_integer_to_float(D_E910.b11) * 0.5f + 1.0f;
                        int hit = collect_mobies_in_sphere(&m->pos, 0x10, m, 0, s + s);

                        *(OvlQuad *)t = m->pos.q;
                        send_blast_hits(m, t, D_L00_00177F00, hit, 0, 0x810000, 4, 1, 3.0f, 0.25f,
                                         1.5f);
                    }
                }
                gz = FUN_L00_00259710(&m->pos);
                if (v->h50 != 0) {
                    V910 t;
                    V910 n;
                    V910 r;
                    {
                        float wz;
                        int i;
                        int j;
                        int l;
                        int cb;
                        int cg;
                        int cr;

                        *(OvlQuad *)t = m->pos.q;
                        t[2] += 3.5f;
                        wz = probe_ground_height(t, 0, 0.5f);
                        for (i = 0; i < 150; i++) {
                            *(OvlQuad *)r = 0;
                            r[0] = random_float_between(-1.0f, 1.0f);
                            r[1] = random_float_between(-1.0f, 1.0f);
                            r[2] = random_float_between(-0.5f, -2.0f);
                            *(OvlQuad *)n = *(OvlQuad *)r;
                            normalize_vector_xyz(
                                n, n, random_float_between(frame_time * 4.0f, frame_time * 8.0f));
                            random_integer_below_cf(&m->pos, n,
                                             random_float_between(0.05f, 0.1f) * 210000.0f, wz);
                        }
                        cr = 0x60;
                        cg = 0x60;
                        cb = 0x7F;
                        for (j = 0; j < 150; j++) {
                            float rr;
                            float ang;

                            *(OvlQuad *)r = 0;
                            r[0] = random_float_between(frame_time * -0.75f, frame_time * 0.75f);
                            r[1] = random_float_between(frame_time * -0.75f, frame_time * 0.75f);
                            r[2] = random_float_between(frame_time * 4.0f, frame_time * 16.0f);
                            *(OvlQuad *)n = *(OvlQuad *)r;
                            rr = random_float_between(0.0f, 0.35f);
                            ang = random_angle_radians();
                            r[0] = fast_cos(ang) * rr;
                            r[1] = fast_sin(ang) * rr;
                            r[2] = 0.0f;
                            add_vector_xyz(r, r, &m->pos);
                            r[2] = wz + 0.5f;
                            n[2] -= rr * frame_time * 8.0f;
                            FUN_L00_002738e8(r, n, 0, random_float_between(0.35f, 1.25f), wz,
                                             frame_time_sq * 20.0f,
                                             (0x16 << 24) | (cb << 16) | (cg << 8) | cr,
                                             (cb << 16) | (cg << 8) | cr, 1);
                        }
                        for (l = 0; l < 20; l++) {
                            int c1 = mine_blend_color(0x7F000000,
                                                      (0x7F << 24) | (cb << 16) | (cg << 8) | cr,
                                                      random_float_between(0.25f, 1.0f));
                            int c2 =
                                mine_blend_color(0, ((cb / 2) << 16) | ((cg / 2) << 8) | (cr / 2),
                                                 random_float_between(0.5f, 1.0f));
                            float sz;
                            int life;

                            r[0] = random_float_between(-3.0f, 3.0f);
                            r[1] = random_float_between(-3.0f, 3.0f);
                            r[2] = random_float_between(frame_time * 4.0f, frame_time * 14.0f);
                            normalize_vector_xyz(r, r,
                                                 random_float_between(1.5f, 3.0f) * frame_time);
                            r[2] = random_float_between(3.0f, 6.0f) * frame_time;
                            *(OvlQuad *)n = m->pos.q;
                            n[2] = wz;
                            c1 = FUN_L00_0025c0e8(c1, D_E910.b11);
                            c2 = FUN_L00_0025c0e8(c2, D_E910.b11);
                            sz = random_float_between(31500.002f, 52500.0f);
                            life =
                                FUN_L00_00257b90(scale_game_frames(0x5A), scale_game_frames(0x96));
                            FUN_L00_0026bb70(n, r, c1, c2, sz, life, 1, D_L00_001B2080[11][0], -1);
                        }
                    }
                } else if (gz + 2.0f < m->pos.f[2]) {
                    V910 t;
                    {
                        clear_vector(t);
                        spawn_explosion(m, t, 0, 0.0f, 0.0f, 3, 3, 5, 2.0f, 1.0f, 4.0f, 1.0f, 2,
                                         7.0f, 0, 1, -1, D_E910.b11);
                    }
                } else {
                    float sc = convert_integer_to_float(D_E910.b11) * 0.5f + 1.0f;
                    int cnt = truncate_float_to_s32(dist * 15.0f);
                    int i;

                    for (i = 0; i < cnt; i++) {
                        int kind = random_integer_below(4);
                        float tbl[4] = {frame_time * 14.0f, frame_time * 22.0f, frame_time * 12.0f,
                                        0.0f};
                        V910 n;
                        V910 r;
                        {
                            float rr;
                            float ang;

                            *(OvlQuad *)r = 0;
                            r[0] = random_float_between(frame_time * -0.5f, frame_time * 0.5f) * sc;
                            r[1] = random_float_between(frame_time * -0.5f, frame_time * 0.5f) * sc;
                            r[2] = random_float_between(frame_time * 4.0f, tbl[kind]) * sc;
                            *(OvlQuad *)n = *(OvlQuad *)r;
                            rr = random_float_between(0.0f, kind == 1 ? 0.5f : 0.25f) * sc;
                            ang = random_angle_radians();
                            r[0] = fast_cos(ang) * rr;
                            r[1] = fast_sin(ang) * rr;
                            r[2] = 0.0f;
                            add_vector_xyz(r, r, &m->pos);
                            r[2] -= rr * fast_cos(0.785398f);
                            n[2] -= rr * frame_time * 8.0f;
                            switch (kind) {
                            case 0: {
                                int c1 = FUN_L00_0025c0e8(0x1F101820, D_E910.b11);
                                float sz = random_float_between(200000.0f, 300000.0f) * sc;
                                int life = FUN_L00_00257b90(scale_game_frames(0xB4),
                                                            scale_game_frames(0xF0));

                                mine_debris((char *)r, (char *)n, c1, 0x101010, sz, life, 0);
                                break;
                            }
                            case 1: {
                                int c1 = FUN_L00_0025c0e8(0x3F081020, D_E910.b11);
                                int c2 = FUN_L00_0025c0e8(0x0F081020, D_E910.b11);
                                float sz = random_float_between(50000.0f, 100000.0f) * sc;

                                mine_debris((char *)r, (char *)n, c1, c2, sz,
                                                 scale_game_frames(0xB4), 1);
                                break;
                            }
                            case 2: {
                                int c1 = FUN_L00_0025c0e8(
                                    random_integer_below(100) < 0x28 ? 0x5FF8F8F8 : 0x2F486078,
                                    D_E910.b11);
                                int c2;
                                float sz;
                                int life;

                                c2 = FUN_L00_0025c0e8(0x0F000020, D_E910.b11);
                                sz = sc * 150000.0f;
                                life = FUN_L00_00257b90(scale_game_frames(0x1E),
                                                        scale_game_frames(0x2D));
                                mine_debris((char *)r, (char *)n, c1, c2, sz, life, 2);
                                break;
                            }
                            case 3: {
                                int c1 = mine_blend_color(0x7F000000, 0x7F182030,
                                                          random_float_between(0.25f, 1.0f));
                                int c2 =
                                    mine_blend_color(0, 0x5F5F5F, random_float_between(0.5f, 1.0f));
                                float sz;
                                int life;

                                c1 = FUN_L00_0025c0e8(c1, D_E910.b11);
                                c2 = FUN_L00_0025c0e8(c2, D_E910.b11);
                                build_spherical_offset(
                                    n, random_float_between(0.0f, 1.0f) * frame_time, ang,
                                    random_angle_radians());
                                sz = random_float_between(200000.0f, 300000.0f) * sc;
                                life = FUN_L00_00257b90(scale_game_frames(0xF0),
                                                        scale_game_frames(0x12C));
                                mine_debris((char *)r, (char *)n, c1, c2, sz, life, 3);
                                break;
                            }
                            }
                        }
                    }
                }
                m->bBC = 0;
                if (D_E910.b11 != 0) {
                    allocate_voice_for_target_entry(3, 0, (int)m);
                } else {
                    allocate_voice_for_target_entry(2, 0, (int)m);
                }
                D_L00_00166C80.f160 = dist < 20.0f ? 0.4f - dist * 0.0175f : 0.050000012f;
                D_L00_00166C80.i168 = scale_game_frames(0x19);
                FUN_001f9b48(&m->pos, hero.motion.pos.f);
                if (D_E910.b11 != 0) {
                    mine_camera_blast((char *)&b, (char *)&m->pos, 0, 0);
                } else {
                    mine_camera_blast((char *)&a, (char *)&m->pos, 0, 0);
                }
                mine_spawn_drops(m, D_L00_001B07B0);
                mark_moby_for_removal(m);
            } else {
                jet_animate((unsigned char *)m, 0.25f);
            }
        }
    }
}
#include "qcopy.h"
typedef unsigned int u128_2ac910 __attribute__((mode(TI), aligned(16)));
typedef union { u128_2ac910 q; float f[4]; } Q_2ac910;
typedef struct { u128_2ac910 p; void *o; int n; float f18; short h1c; short h1e; } V_2ac910;
unsigned char *FUN_0020c4f8_2ac910(int) __asm__("FUN_0020c4f8");
float FUN_002132a8_2ac910(float, float) __asm__("FUN_002132a8");
void FUN_L00_002502f0_2ac910(void *, int, int, int) __asm__("FUN_L00_002502f0");
int FUN_001f96f8_2ac910(int) __asm__("FUN_001f96f8");
float FUN_001fa6c0_2ac910(int) __asm__("FUN_001fa6c0");
void FUN_L00_00250df8_2ac910(void *) __asm__("FUN_L00_00250df8");
/* Spawns a particle moby with a random rotation, owned by `owner`, at `pos` with velocity `vel`, tinted red/green/blue
   and living `lifetime` frames; returns the new moby or NULL. */
unsigned char *spawn_glow_particle(void *owner, void *pos, void *vel, int lifetime, int red, int green, int blue, int alpha, float scale) __asm__("FUN_L00_002ac910");
unsigned char *spawn_glow_particle(void *owner, void *pos, void *vel, int lifetime, int red, int green, int blue, int alpha, float scale) {
    unsigned char *moby;
    red &= 0xFF; green &= 0xFF; blue &= 0xFF; alpha &= 0xFF;
    moby = FUN_0020c4f8_2ac910(0x70);
    if (moby) {
        Q_2ac910 rot_copy, rot;
        V_2ac910 *data = *(V_2ac910 **)(moby + 0x78);
        float elapsed;
        rot.q = 0;
        rot.f[0] = FUN_002132a8_2ac910(-3.1415927f, 3.1415927f);
        rot.f[1] = FUN_002132a8_2ac910(-3.1415927f, 3.1415927f);
        rot.f[2] = FUN_002132a8_2ac910(-3.1415927f, 3.1415927f);
        rot_copy.q = rot.q;
        moby[0x20] = 0;
        moby[0x23] = alpha;
        moby[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        moby[0x31] = 1;
        FUN_L00_002502f0_2ac910(moby, red, green, blue);
        data->f18 = *(float *)(moby + 0x2C) * scale;
        *(int *)(moby + 0x2C) = 0;
        data->o = owner;
        qcopy(moby + 0x10, pos);
        qcopy(data, vel);
        qcopy(moby + 0x40, &rot_copy);
        data->n = lifetime;
        data->h1c = *(unsigned short *)&data->n - FUN_001f96f8_2ac910(4);
        elapsed = FUN_001fa6c0_2ac910(data->n - data->h1c);
        *(float *)(moby + 0x2C) = elapsed * data->f18 / FUN_001fa6c0_2ac910(data->n);
        data->h1e = alpha;
        FUN_L00_00250df8_2ac910(moby);
    }
    return moby;
}
typedef struct {
    char pad[0x14];
    int n;
    float f18;
    s16 h1c;
    s16 h1e;
} V_2acad8;
typedef struct {
    char pad[0x23];
    u8 b23;
    char pad2[0x2C - 0x24];
    float f2c;
    char pad3[0x40 - 0x30];
    float r[3];
    char pad4[0x78 - 0x4C];
    V_2acad8 *v;
} M_2acad8;
extern float D_0015ED6C_2acad8 __asm__("D_0015ED6C");
extern float FUN_001fa6c0_2acad8(int) __asm__("FUN_001fa6c0");
extern float FUN_001fa580_2acad8(float, float) __asm__("FUN_001fa580");
extern int FUN_001f9770_2acad8(void *) __asm__("FUN_001f9770");
extern void FUN_0020c828_2acad8(void *) __asm__("FUN_0020c828");

void FUN_L00_002acad8(M_2acad8 *m) {
    V_2acad8 *v = m->v;
    float a, b;
    int n;
    a = FUN_001fa6c0_2acad8(v->n - v->h1c);
    b = FUN_001fa6c0_2acad8(v->n);
    m->f2c = a * v->f18 / b;
    m->r[0] = FUN_001fa580_2acad8(m->r[0], D_0015ED6C_2acad8 * 3.1415927f);
    m->r[1] = FUN_001fa580_2acad8(m->r[1], D_0015ED6C_2acad8 * 3.1415927f);
    m->r[2] = FUN_001fa580_2acad8(m->r[2], D_0015ED6C_2acad8 * 3.1415927f);
    if (FUN_001f9770_2acad8(&v->h1c)) {
        FUN_0020c828_2acad8(m);
        return;
    }
    n = v->n / 2;
    if (v->h1c < n) {
        m->b23 = v->h1c * v->h1e / n;
    }
}
#include "eetypes.h"
#include "qcopy.h"
typedef union {
    u128 q_002acbf0;
    f32 f[4];
} V_2acbf0_002acbf0;
typedef struct {
    V_2acbf0_002acbf0 corner[4];
    u32 color[4];
    f32 uv[8];
    s64 unk70;
    s64 tex;
    s64 unk80;
    s64 unk88;
} Q_2acbf0_002acbf0;
typedef struct {
    u8 pad0[0x10];
    V_2acbf0_002acbf0 pos;
    f32 f20, f24, f28;
} PV_2acbf0_002acbf0;
typedef struct {
    u8 pad0[0x78];
    PV_2acbf0_002acbf0 *pv;
} M_2acbf0_002acbf0;
extern s32 D_L00_0015F5C4_002acbf0 __asm__("D_L00_0015F5C4");
extern s32 D_L00_0015F5CC_002acbf0 __asm__("D_L00_0015F5CC");
extern f32 seg_sqrt(f32) __asm__("FUN_001f9988");
extern f32 atan2_2acbf0(f32, f32) __asm__("FUN_001f9e90");
extern f32 appr_2acbf0(f32, f32) __asm__("FUN_001fa580");
int FUN_001f96f8_002acbf0(int) __asm__("FUN_001f96f8");
float FUN_001fa6c0_002acbf0(int) __asm__("FUN_001fa6c0");
extern f32 i2f_2acbf0(f32) __asm__("FUN_001f96b0");
extern f32 fast_normalize_angle(f32) __asm__("FUN_001fa610");
extern void rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern void mmul_2acbf0(void *, void *, void *) __asm__("FUN_001fa378");
extern s64 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void draw_geometry_quad(void *, void *, s32) __asm__("FUN_001f7d30");
void FUN_L00_002acbf0(M_2acbf0_002acbf0 *m) {
    Q_2acbf0_002acbf0 quad;
    V_2acbf0_002acbf0 mat[4];
    V_2acbf0_002acbf0 mz[4];
    V_2acbf0_002acbf0 rot;
    V_2acbf0_002acbf0 rz;
    PV_2acbf0_002acbf0 *pv = m->pv;
    s32 i;
    s32 odd;
    f32 a, t;
    if (D_L00_0015F5C4_002acbf0 == 2) {
        return;
    }
    for (i = 0; i < 2; i++) {
        odd = i & 1;
        a = atan2_2acbf0(seg_sqrt(pv->f20 * pv->f20 + pv->f28 * pv->f28), pv->f24);
        rot.f[0] = appr_2acbf0(-a, 1.57079637f);
        rot.f[1] = atan2_2acbf0(pv->f28, pv->f20);
        rot.f[2] = 0.0f;
        rot.f[3] = 0.0f;
        rz.f[0] = 0.0f;
        t = FUN_001fa6c0_002acbf0(D_L00_0015F5CC_002acbf0 % FUN_001f96f8_002acbf0(180));
        {
            f32 d = i2f_2acbf0(180.0f);
            f32 r;
            t *= 6.28318548f;
            r = fast_normalize_angle(t / d);
            if (odd) {
                r = -r;
            }
            rz.f[1] = r;
        }
        rz.f[2] = 0.0f;
        rz.f[3] = 0.0f;
        rotation_matrix(mz, &rz);
        rotation_matrix(mat, &rot);
        mmul_2acbf0(mat, mat, mz);
        qcopy(&mat[3], &pv->pos);
        quad.color[0] = 0x80808080;
        quad.color[1] = 0x80808080;
        quad.color[2] = 0x80808080;
        quad.color[3] = 0x80808080;
        quad.tex = get_effect_texture(odd ? 0x12 : 0x11);
        quad.unk80 = 0xFF9000000260L;
        quad.unk88 = 0x8000000048L;
        quad.unk70 = 0;
        quad.corner[0].f[0] = -1.0f;
        quad.corner[0].f[1] = 0.0f;
        quad.corner[0].f[2] = 1.0f;
        quad.corner[0].f[3] = 1.0f;
        quad.corner[1].f[0] = 1.0f;
        quad.corner[1].f[1] = 0.0f;
        quad.corner[1].f[2] = 1.0f;
        quad.corner[1].f[3] = 1.0f;
        quad.corner[2].f[0] = -1.0f;
        quad.corner[2].f[1] = 0.0f;
        quad.corner[2].f[2] = -1.0f;
        quad.corner[2].f[3] = 1.0f;
        quad.corner[3].f[0] = 1.0f;
        quad.corner[3].f[1] = 0.0f;
        quad.corner[3].f[2] = -1.0f;
        quad.corner[3].f[3] = 1.0f;
        quad.uv[0] = 0.0f;
        quad.uv[1] = 1.0f;
        quad.uv[2] = 1.0f;
        quad.uv[3] = 1.0f;
        quad.uv[4] = 0.0f;
        quad.uv[5] = 0.0f;
        quad.uv[6] = 1.0f;
        quad.uv[7] = 0.0f;
        draw_geometry_quad(&quad, mat, 1);
    }
}
typedef unsigned int u128_2ace70 __attribute__((mode(TI), aligned(16)));
typedef union {
    u128_2ace70 q;
    float f[4];
} V_2ace70;
extern float D_0015ED6C_2ace70 __asm__("D_0015ED6C");
unsigned char *FUN_0020c4f8_2ace70(int) __asm__("FUN_0020c4f8");
float FUN_002132a8_2ace70(float, float) __asm__("FUN_002132a8");
float FUN_001f9e90_2ace70(float, float) __asm__("FUN_001f9e90");
void FUN_001f99f8_2ace70(void *) __asm__("FUN_001f99f8");
void FUN_L00_00250df8_2ace70(void *) __asm__("FUN_L00_00250df8");
unsigned char *spawn_bomb(int owner, void *pos) __asm__("FUN_L00_002ace70");
unsigned char *spawn_bomb(int owner, void *pos) {
    unsigned char *m = FUN_0020c4f8_2ace70(0x79);
    if (m) {
        V_2ace70 u, t;
        unsigned char *v = *(unsigned char **)(m + 0x78);
        struct Hero *g;
        float a;
        t.q = 0;
        t.f[0] = FUN_002132a8_2ace70(-3.1415927f, 3.1415927f);
        t.f[1] = FUN_002132a8_2ace70(-3.1415927f, 3.1415927f);
        t.f[2] = FUN_002132a8_2ace70(-3.1415927f, 3.1415927f);
        u.q = t.q;
        m[0x30] = 0xFF;
        *(short *)(m + 0x32) = 0xFF;
        m[0x31] = 1;
        m[0x20] = 0;
        *(int *)(v + 0x50) = owner;
        *(short *)(v + 0x56) = 0;
        *(short *)(v + 0x6A) = 0;
        *(int *)(v + 0x60) = 0;
        g = &hero;
        *(float *)(v + 0x6C) = FUN_001f9e90_2ace70(g->unk670, g->unk674);
        *(int *)(v + 0x64) = 0;
        qcopy(m + 0x10, pos);
        FUN_001f99f8_2ace70(v);
        qcopy(m + 0x40, &u);
        a = FUN_002132a8_2ace70(D_0015ED6C_2ace70 * 1.0471976f, D_0015ED6C_2ace70 * 3.1415927f);
        {
            float d = D_0015ED6C_2ace70;
            *(float *)(v + 0x58) = a;
            *(float *)(v + 0x5C) = FUN_002132a8_2ace70(d * 1.0471976f, d * 3.1415927f);
        }
        FUN_L00_00250df8_2ace70(m);
    }
    return m;
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002acfd8.s", FUN_L00_002acfd8);
#else
extern int D_0013CAE0_2acfd8 __asm__("D_0013CAE0") __attribute__((section(".data")));
extern float D_0015ED70_2acfd8 __asm__("D_0015ED70");
extern float D_L00_00173E60_2acfd8[4] __asm__("D_L00_00173E60");
extern short scale_game_frames_2acfd8(int) __asm__("FUN_001f96f8");
extern int FUN_L00_002603d0_2acfd8(unsigned char *) __asm__("FUN_L00_002603d0");
extern void add_vector_xyz_2acfd8(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz_2acfd8(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_vector_xyz_2acfd8(void *, void *, float) __asm__("FUN_001f9a68");
extern float dot_vectors_xyz_2acfd8(void *, void *) __asm__("FUN_001f9ab0");
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern void normalize_vector_xyz_2acfd8(void *, void *, float) __asm__("FUN_001f9bf8");
extern float fast_cos_2acfd8(float) __asm__("FUN_001f9dc8");
extern float fast_sin_2acfd8(float) __asm__("FUN_001f9de0");
extern float atan2_2acfd8(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations_2acfd8(float, float) __asm__("FUN_001fa688");
extern float convert_integer_to_float_2acfd8(int) __asm__("FUN_001fa6c0");
extern int FUN_001efa68_2acfd8(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void FUN_L00_002c2488_2acfd8(void *, void *, void *, float, float, float) __asm__("FUN_L00_002c2488");
void FUN_L00_002acfd8(float *pos, unsigned char *moby, float *velocity) {
    float *v = *(float **)(moby + 0x78);
    float difference[4] __attribute__((aligned(16)));
    float direction[4] __attribute__((aligned(16)));
    float result[4] __attribute__((aligned(16)));
    float working[4] __attribute__((aligned(16)));
    float desired[4] __attribute__((aligned(16)));
    float dot, length, distance, angle, height, total;

    *(short *)((char *)v + 0x6a) = scale_game_frames_2acfd8(30);
    moby[0x20] = 1;
    qcopy(v, velocity);
    if (hero.state.control_mode == 15) {
        add_vector_xyz_2acfd8(v, v, (char *)&hero + 0x100);
    } else if (hero.ground_moby != 0 && FUN_L00_002603d0_2acfd8(hero.ground_moby) != 0) {
        add_vector_xyz_2acfd8(v, v, (char *)&hero + 0x100);
    } else if (*(float **)((char *)v + 0x60) != 0) {
        short cls = *(short *)(*(char **)((char *)v + 0x60) + 0xa6);
        if (cls == 0x56e || cls == 0x58e) {
            add_vector_xyz_2acfd8(v, v, v + 0x10);
        } else if (*(int *)((char *)v + 0x64) != 0) {
            length = vector_length_xy(velocity);
            subtract_vector_xyz_2acfd8(difference, &hero.motion.pos, *(char **)((char *)v + 0x60) + 0x10);
            normalize_vector_xyz_2acfd8(difference, difference, 1.0f);
            dot = dot_vectors_xyz_2acfd8(difference, v + 0x10);
            total = dot + length;
            distance = vector_distance(pos, *(char **)((char *)v + 0x60) + 0x10) / total;
            if (dot > 0.0f) {
                scale_vector_xyz_2acfd8(direction, difference, distance * dot);
            } else {
                scale_vector_xyz_2acfd8(direction, v + 0x10, distance);
            }
            add_vector_xyz_2acfd8(working, direction, *(char **)((char *)v + 0x60) + 0x10);
            *(OvlQuad *)direction = *(OvlQuad *)working;
            {
                float heading = atan2_2acfd8(hero.unk670, hero.unk674);
                float toward = atan2_2acfd8(direction[0] - hero.motion.pos.f[0],
                                             direction[1] - hero.motion.pos.f[1]);
                angle = fast_difference_between_rotations_2acfd8(heading, toward);
            }
            if (angle > 1.5707964f) {
                working[0] = fast_cos_2acfd8(atan2_2acfd8(hero.unk670, hero.unk674)) *
                            vector_distance(&hero.motion.pos, direction) * 0.5f;
                angle = fast_sin_2acfd8(atan2_2acfd8(hero.unk670, hero.unk674));
                working[2] = 0.0f;
                working[1] = angle * vector_distance(&hero.motion.pos, direction) * 0.5f;
                add_vector_xyz_2acfd8(desired, working, &hero.motion.pos);
                qcopy(direction, desired);
            }
            total = D_0015ED70_2acfd8 * 11.0f;
            height = 8.5f;
            if (D_0013CAE0_2acfd8 & 5) {
                height = convert_integer_to_float_2acfd8(*((unsigned char *)&D_0013E520 + 0xa)) * 2.5f + 8.5f;
            }
            FUN_L00_002c2488_2acfd8(pos, direction, result, total,
                                       0.00001f, height);
            scale_vector_xyz_2acfd8(result, result, 0.65f);
            scale_vector_xyz_2acfd8(v, v, 0.35f);
            add_vector_xyz_2acfd8(working, v, result);
            *(OvlQuad *)v = *(OvlQuad *)working;
        }
    }
    *(short *)((char *)v + 0x54) = scale_game_frames_2acfd8(300);
    *(OvlQuad *)(moby + 0x10) = *(OvlQuad *)pos;
    qcopy_nc(difference, (char *)hero.moby + 0x10);
    difference[2] = pos[2];
    if (FUN_001efa68_2acfd8(difference, pos, 0, hero.moby, 0) != 0) {
        moby[0xbc] = 1;
        qcopy(moby + 0x10, D_L00_00173E60_2acfd8);
    }
    if (*((unsigned char *)&D_0013E520 + 0xa) != 0 && hero.state.current == HERO_STATE_LOOK) {
        float z = v[2];
        scale_vector_xyz_2acfd8(v, v, 1.5f);
        v[2] = z / 1.5f;
    }
}
#endif
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L00_002ad410.s", FUN_L00_002ad410);
#else
typedef struct {
    U910 vel;
    U910 landing;
    U910 normal;
    U910 previous_hit;
    U910 difference;
    M910 *holder;
    short timer;
    short holder_flag;
    char pad58[8];
    M910 *last_hit;
    int hit_count;
} LandingVars_2ad410;

extern float D_0015ED70_2ad410 __asm__("D_0015ED70");
extern U910 D_L00_00173E80_2ad410 __asm__("D_L00_00173E80");
extern int FUN_001f0b58_2ad410(void) __asm__("FUN_001f0b58");
extern float fast_cos_2ad410(float) __asm__("FUN_001f9dc8");
extern float fast_sin_2ad410(float) __asm__("FUN_001f9de0");
extern float fast_add_rotations_2ad410(float, float) __asm__("FUN_001fa580");
extern float atan2_2ad410(float, float) __asm__("FUN_001f9e90");
extern int FUN_001efa68_2ad410(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern int FUN_001fef20_2ad410(int) __asm__("FUN_001f96f8");
extern int FUN_001fef68_2ad410(int *) __asm__("FUN_001f9740");
extern int FUN_L00_002603d0_2ad410(M910 *) __asm__("FUN_L00_002603d0");
extern void FUN_001f93e0_2ad410(void *, void *) __asm__("FUN_001f4600");
extern void FUN_001ff2a8_2ad410(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_001ff278_2ad410(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001ff300_2ad410(float, void *, void *) __asm__("FUN_001f9a68");
extern void FUN_00259698_2ad410(void *, int, float) __asm__("FUN_00213508");

int FUN_L00_002ad410(LandingVars_2ad410 *v, M910 *m, int mode) {
    U910 position, velocity, previous, hero_position, offset, start;
    struct { int timer; int object_hit; } hit;
    int found = 0, i;
    hit.object_hit = 0;

    if (hero.state.current == 0x72)
        return 0;
    if (D_L00_0015F5C4_002acbf0 != 0)
        return 0;
    qcopy(&previous, &m->pos);
    if (m->state == 0) {
        position.f[0] = fast_cos_2ad410(fast_add_rotations_2ad410(-0.361968f,
                             atan2_2ad410(hero.unk670, hero.unk674))) * 0.859039f;
        position.f[1] = fast_sin_2ad410(fast_add_rotations_2ad410(-0.361968f,
                             atan2_2ad410(hero.unk670, hero.unk674))) * 0.859039f;
        position.f[2] = 0.0f;
        FUN_001ff278_2ad410(&position, &position, &hero.motion.pos);
        position.f[2] += 0.49082f;
        qcopy(&velocity, &v->vel);
        if (hero.state.control_mode == 15)
            FUN_001ff278_2ad410(&velocity, &velocity, &hero.motion.displacement);
        if (hero.ground_moby && FUN_L00_002603d0_2ad410((M910 *)hero.ground_moby) && mode)
            FUN_001ff278_2ad410(&velocity, &velocity, &hero.motion.displacement);
        hit.timer = FUN_001fef20_2ad410(300);
    } else {
        W910 *holder_vars = v->holder->v;
        int holder_flag;
        qcopy(&position, &m->pos);
        qcopy(&velocity, &v->vel);
        holder_flag = *(int *)((char *)holder_vars + 0x50);
        hit.timer = v->timer;
        if (holder_flag)
            v->holder_flag = 1;
    }
    {
        float z = position.f[2];
        hero_position.q = ((M910 *)hero.moby)->pos.q;
        hero_position.f[2] = z;
    }
    if (FUN_001efa68_2ad410(&hero_position, &position, 0x10, hero.moby, 0) &&
        (FUN_001f0b58_2ad410() || 0.0f < velocity.f[2])) {
        if (coll_hit.moby && coll_hit.i1C <= 0) {
            if ((coll_hit.moby != hero.moby && coll_hit.moby != v->holder) ||
                hit.timer < FUN_001fef20_2ad410(300) - FUN_001fef20_2ad410(10)) {
                offset.q = 0;
                offset.f[2] = 0.5f;
                {
                    H910 *h = &coll_hit;
                    if (v->last_hit != h->moby) {
                        v->hit_count = 0;
                        v->last_hit = h->moby;
                        v->previous_hit.q = h->moby->pos.q;
                    } else {
                        v->hit_count++;
                        FUN_001ff2a8_2ad410(&start, (char *)h->moby + 0x10, &v->previous_hit);
                        v->difference.q = start.q;
                        v->previous_hit.q = h->moby->pos.q;
                    }
                }
                hit.object_hit = 1;
                qcopy(&v->landing, (char *)coll_hit.moby + 0x10);
                found = 1;
                FUN_001ff278_2ad410(&offset, &offset, &v->landing);
                FUN_00259698_2ad410(&offset, 0, 0.5f);
            }
        } else if (coll_hit.i1C > 0) {
            qcopy(&v->landing, &coll_hit.pos);
            found = 1;
        }
    }
    if (*(unsigned char *)((char *)&D_E910 + 0xa) && hero.state.current == 1) {
        float z = velocity.f[2];
        FUN_001ff300_2ad410(1.5f, &velocity, &velocity);
        velocity.f[2] = z / 1.5f;
    }
    hero_position.q = position.q;
    while (!FUN_001fef68_2ad410(&hit.timer) && !found) {
        qcopy(&previous, &position);
        for (i = 9; i >= 0; i--) {
            FUN_001ff278_2ad410(&position, &position, &velocity);
            velocity.f[2] -= D_0015ED70_2ad410 * 11.0f;
        }
        if (!FUN_001efa68_2ad410(&previous, &position, 0x10, m, 0))
            continue;
        if (!FUN_001f0b58_2ad410() && 0.0f < velocity.f[2])
            continue;
        if (coll_hit.moby && coll_hit.i1C <= 0) {
            if (!(position.f[2] - hero_position.f[2] < 2.0f))
                goto check_surface_2ad410;
            if ((coll_hit.moby == hero.moby || coll_hit.moby == v->holder) &&
                hit.timer >= FUN_001fef20_2ad410(300) - FUN_001fef20_2ad410(10))
                continue;
            offset.q = 0;
            offset.f[2] = 0.5f;
            {
                H910 *h = &coll_hit;
                if (v->last_hit != h->moby) {
                    v->hit_count = 0;
                    v->last_hit = h->moby;
                    v->previous_hit.q = h->moby->pos.q;
                } else {
                    v->hit_count++;
                    FUN_001ff2a8_2ad410(&start, (char *)h->moby + 0x10, &v->previous_hit);
                    v->difference.q = start.q;
                    v->previous_hit.q = h->moby->pos.q;
                }
            }
            hit.object_hit = 1;
            qcopy(&v->landing, (char *)coll_hit.moby + 0x10);
            FUN_001ff278_2ad410(&offset, &offset, &v->landing);
            FUN_00259698_2ad410(&offset, 0, 0.5f);
            break;
        }
check_surface_2ad410:
        if (coll_hit.i1C > 0) {
            qcopy(&v->landing, &coll_hit.pos);
            break;
        }
    }
landing_done_2ad410:
    if (!hit.timer)
        goto return_zero_2ad410;
    if (!mode) {
        if (!coll_hit.moby)
            goto return_zero_2ad410;
        if (!FUN_L00_002603d0_2ad410(coll_hit.moby))
            goto return_zero_2ad410;
    }
    if (mode != 2 || !coll_hit.moby || !FUN_L00_002603d0_2ad410(coll_hit.moby))
        FUN_001f93e0_2ad410(FUN_L00_002acbf0, m);
    if (!hit.object_hit && v->last_hit) {
        if (v->last_hit->state == 0xfe) {
            v->hit_count = 0;
        } else if (v->last_hit->state == 0xfd) {
            v->hit_count = 0;
        } else if (!(v->last_hit->flags & 0x1000)) {
            v->hit_count = 0;
        } else {
            if (v->hit_count <= FUN_001fef20_2ad410(30))
                goto keep_hit_2ad410;
            v->hit_count = 0;
        }
        v->last_hit = 0;
    }
keep_hit_2ad410:
    v->normal.q = D_L00_00173E80_2ad410.q;
    FUN_001ff2a8_2ad410(&previous, &v->landing, D_L00_00166DC0);
    FUN_001ff300_2ad410(0.95f, &previous, &previous);
    FUN_001ff278_2ad410(&v->landing, &previous, D_L00_00166DC0);
    return 1;
return_zero_2ad410:
    return 0;
}
#endif
/* Update of moby class 121, a thrown bomb: it flies and spins, bounces once on the ground, explodes on a hit or when its timer runs out, then its blast ring grows for 15 frames. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002AB910.c: func_L00_002AEDD0), where it is exact; names translated to the US level program. */
typedef union {
    u128 q;
    float f[4];
} Vec_2adb30 __attribute__((aligned(16)));
typedef struct {
    int c[6];
} Colors6_2adb30;
typedef struct BombMoby_2adb30 BombMoby_2adb30;
typedef struct {
    Vec_2adb30 v;
    char pad10[0x40];
    M910 *owner;
    short h54;
    short h56;
    char pad58[0x10];
    short h68;
    short h6a;
} BombData_2adb30;
struct BombMoby_2adb30 {
    char p0[0x10];
    Vec_2adb30 pos;
    unsigned char state;
    char p21[0x13];
    unsigned short flags;
    char p36[0xA];
    float rx;
    float ry;
    char p48[0x2C];
    int f74;
    BombData_2adb30 *d;
    char p7C[0x14];
    int color;
    char p94[0x12];
    short cls;
    char pA8[0x14];
    unsigned char bBC;
};
typedef struct {
    Vec_2adb30 dir;
    BombMoby_2adb30 *moby;
    int flags;
    unsigned char kind;
    unsigned char on;
    short cls;
    float fDC;
    int iE0;
} BombHit_2adb30;
extern float D_0015ED6C_2adb30 __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern float frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern int D_L00_0015F5CC_2adb30 __asm__("D_L00_0015F5CC");
extern s32 D_L00_001612D0_gp __asm__("D_L00_001612D0") __attribute__((sda));
extern s32 D_L00_001612D8_gp __asm__("D_L00_001612D8") __attribute__((sda));
extern Cam910 player_camera __asm__("D_L00_00166C80");
extern s32 g_debug_camera[] __asm__("D_L00_00166DC0");
extern float D_L00_00173E68_2adb30[] __asm__("D_L00_00173E68");
extern int *D_L00_00177F00_2adb30[] __asm__("D_L00_00177F00");
extern unsigned char *D_L00_001B2080_2adb30[] __asm__("D_L00_001B2080");
extern G910 D_G910 __asm__("D_0013F350");
extern int FUN_001f96f8_2adb30(int) __asm__("FUN_001f96f8");
extern float FUN_001f96b0_2adb30(float) __asm__("FUN_001f96b0");
extern int FUN_001f9770_2adb30(void *) __asm__("FUN_001f9770");
extern void FUN_001f99f8_2adb30(float *) __asm__("FUN_001f99f8");
extern void FUN_001f9a10_2adb30(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_001f9a28_2adb30(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_001f9a68_2adb30(void *, void *, float) __asm__("FUN_001f9a68");
extern float FUN_001f9ab0_2adb30(void *, void *) __asm__("FUN_001f9ab0");
extern float FUN_001f9af0_2adb30(void *) __asm__("FUN_001f9af0");
extern f32 distance_xyz(void *, void *) __asm__("FUN_001f9b48");
extern float FUN_001f9dc8_2adb30(float) __asm__("FUN_001f9dc8");
extern float FUN_001f9de0_2adb30(float) __asm__("FUN_001f9de0");
extern float FUN_001fa580_2adb30(float, float) __asm__("FUN_001fa580");
extern float FUN_001fa6c0_2adb30(int) __asm__("FUN_001fa6c0");
extern int FUN_001fa6d0_2adb30(float) __asm__("FUN_001fa6d0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void FUN_0020c828_2adb30(void *) __asm__("FUN_0020c828");
extern int FUN_00213260_2adb30(int) __asm__("FUN_00213260");
extern float FUN_002132a8_2adb30(float, float) __asm__("FUN_002132a8");
extern float FUN_00213308_2adb30(void) __asm__("FUN_00213308");
extern float FUN_00213508_2adb30(void *, int, float) __asm__("FUN_00213508");
extern float FUN_002135f0_2adb30(float *, void *) __asm__("FUN_002135f0");
extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern int FUN_0022da68_2adb30(int, int, int) __asm__("FUN_0022da68");
extern int FUN_001efa68_2adb30(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int probe_world_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern int collision_hit_material(void) __asm__("FUN_001f0b58");
extern void FUN_001f9bf8_2adb30(void *, void *, float) __asm__("FUN_001f9bf8");
extern void trooper_scale(void *, void *, float) __asm__("FUN_001f9c48");
extern void FUN_001f9c90_2adb30(void *, void *, float) __asm__("FUN_001f9c90");
extern void FUN_L00_001ff660_2adb30(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern int mine_blend_color(int, int, float) __asm__("FUN_L00_002371e0");
extern int FUN_L00_00257b90_2adb30(int, int) __asm__("FUN_L00_00257b90");
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern float allocate_voice_for_target_entry_cf(void *) __asm__("FUN_L00_00259710");
extern void mine_blast_mobys(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
extern float FUN_L00_0025b6a8_2adb30(float, float, float) __asm__("FUN_L00_0025b6a8");
extern void FUN_L00_0025c088_2adb30(int *, int *, int *, int) __asm__("FUN_L00_0025c088");
extern unsigned FUN_L00_0025c0e8_2adb30(unsigned c, int mask) __asm__("FUN_L00_0025c0e8");
extern unsigned char *FUN_L00_002603d0_2adb30(unsigned char *) __asm__("FUN_L00_002603d0");
extern float normalize_angle(float) __asm__("FUN_L00_0025e310");
extern void FUN_L00_0026a9f0_2adb30(void *, void *, int, int, float, int, int, int, int, float) __asm__("FUN_L00_0026a9f0");
extern void *FUN_L00_0026bb70_2adb30(void *, void *, int, int, float, int, int, int, int) __asm__("FUN_L00_0026bb70");
extern int random_integer_below_cf(void *, void *, float, float) __asm__("FUN_L00_0026f080");
extern void FUN_L00_0026f548_2adb30(void *, void *, int, int) __asm__("FUN_L00_0026f548");
extern void FUN_L00_002738e8_2adb30(void *, void *, int, float, float, float, int, int, int) __asm__("FUN_L00_002738e8");
extern int trooper_play_sound(int i, int a1, int v, int k) __asm__("FUN_L00_0028dc90");
extern void FUN_L00_002a3ec8_2adb30(int, int, int, float, float, float, float) __asm__("FUN_L00_002a3ec8");
extern int FUN_L00_002ad410_2adb30(void *, void *, int) __asm__("FUN_L00_002ad410");
extern char *FUN_L00_002af450_2adb30(char *, char *, int, int, int) __asm__("FUN_L00_002af450");
extern char *mine_camera_blast(char *a, char *b, int c, char *d) __asm__("FUN_L00_002d3838");
extern char *FUN_L00_002d7e90_2adb30(void *, float) __asm__("FUN_L00_002d7e90");
extern char *FUN_L00_002df808_2adb30(void *, void *, void *, float, int, unsigned char, unsigned char, unsigned char, int) __asm__("FUN_L00_002df808");
void bomb_update(BombMoby_2adb30 *m) __asm__("FUN_L00_002adb30");
extern int D_L00_00160770;
extern short D_L00_001613F0 __attribute__((sda));
extern short D_L00_001613F4 __attribute__((sda));
extern short D_L00_001613F8 __attribute__((sda));
extern short D_L00_001613FC __attribute__((sda));
extern short D_L00_00161400 __attribute__((sda));
extern short D_L00_00161404 __attribute__((sda));
extern short D_L00_00161408 __attribute__((sda));
extern short D_L00_0016140C __attribute__((sda));
extern short D_L00_00161410 __attribute__((sda));
extern short D_L00_00161414 __attribute__((sda));
extern short D_L00_00161418 __attribute__((sda));
extern short D_L00_0016141C __attribute__((sda));
extern short D_L00_00161420 __attribute__((sda));
extern short D_L00_00161424 __attribute__((sda));
extern short D_L00_00161428 __attribute__((sda));
extern short D_L00_0016142C __attribute__((sda));
extern short D_L00_00161430 __attribute__((sda));
extern B910 D_L00_001E98D0;
extern Colors6_2adb30 D_L00_001E9920;
extern Colors6_2adb30 D_L00_001E9938;
void bomb_update(BombMoby_2adb30 *m) {
    B910 rec = D_L00_001E98D0;
    Vec_2adb30 delta;
    Vec_2adb30 old;
    Vec_2adb30 a;
    Vec_2adb30 b;
    Vec_2adb30 proj;
    BombData_2adb30 *d;
    b.q = 0;
    b.f[2] = D_0015ED6C_2adb30 * 2.0f;
    a.q = b.q;
    b.q = 0;
    b.f[2] = 1.0f;
    d = m->d;
    if ((D_G910.hero == 0 || D_G910.hero->cls != 0xC0) && m->state == 0) {
        FUN_0020c828_2adb30(m);
        return;
    }
    if (D_G910.b20A4 != 0 && m->state == 0) {
        FUN_0020c828_2adb30(m);
        return;
    }
    if ((D_G910.b20A5 != 0 || D_G910.b20AF != 0) && m->state == 0) {
        m->flags |= 0x41;
    } else {
        m->flags &= ~0x41;
    }
    if (d->owner->cls == 0xC0 && d->h56 == 0 && FUN_001f9af0_2adb30(d) > 0.0f) {
        if (D_G910.p2FC == 0 || FUN_L00_002603d0_2adb30(D_G910.p2FC) == 0) {
            FUN_L00_002ad410_2adb30(d, m, 1);
        } else if (FUN_L00_002ad410_2adb30(d, m, 0) == 0) {
            FUN_L00_002ad410_2adb30(d, m, 2);
        }
    }
    if (m->state == 1) {
        Vec_2adb30 c;
        float t;
        int cr;
        int cg;
        int cb;
        c.q = 0;
        c.f[0] = 128.0f;
        c.f[1] = 128.0f;
        c.f[2] = 128.0f;
        c.f[3] = 255.0f;
        FUN_001f9770_2adb30(&d->h6a);
        t = FUN_001fa6c0_2adb30(d->h6a) / FUN_001fa6c0_2adb30(FUN_001f96f8_2adb30(0x1E));
        cr = FUN_001fa6d0_2adb30((c.f[0] - 255.0f) * t + 255.0f);
        cg = FUN_001fa6d0_2adb30((c.f[1] - 64.0f) * t + 64.0f);
        cb = FUN_001fa6d0_2adb30((c.f[2] - 64.0f) * t + 64.0f);
        m->color = (cb << 16) | (0xFF << 24) | (cg << 8) | cr;
        if (d->h68 == 0) {
            if ((D_L00_0015F5CC_2adb30 & 7) == 0) {
                int i;
                for (i = 0; i < *(int *)&D_L00_0016142C; i++) {
                    Vec_2adb30 spark_velocity;
                    Vec_2adb30 v2;
                    Vec_2adb30 random_velocity_offset;
                    int p0;
                    int p1;
                    int n0;
                    int n1;
                    int n2;
                    FUN_001f9a68_2adb30(&spark_velocity, d, *(float *)&D_L00_001613F4);
                    random_vector(random_velocity_offset.f, 0.0f, *(float *)&D_L00_001613F8 * D_0015ED6C_2adb30);
                    FUN_001f9a10_2adb30(&spark_velocity, &random_velocity_offset, &spark_velocity);
                    FUN_001f99f8_2adb30(v2.f);
                    spark_velocity.f[3] = FUN_002132a8_2adb30(*(float *)&D_L00_0016140C, *(float *)&D_L00_00161410);
                    v2.f[3] = FUN_002132a8_2adb30(*(float *)&D_L00_00161414, *(float *)&D_L00_00161418);
                    p0 = fast_tween_color(*(int *)&D_L00_0016141C, *(int *)&D_L00_00161420, FUN_002132a8_2adb30(0.0f, 1.0f));
                    p1 = fast_tween_color(*(int *)&D_L00_00161424, *(int *)&D_L00_00161428, FUN_002132a8_2adb30(0.0f, 1.0f));
                    n0 = FUN_001fa6d0_2adb30(FUN_001f96b0_2adb30((float)*(int *)&D_L00_001613FC * FUN_002132a8_2adb30(0.0f, 1.0f) + 1.0f));
                    n1 = FUN_001fa6d0_2adb30(FUN_001f96b0_2adb30((float)*(int *)&D_L00_00161400 * (FUN_002132a8_2adb30(-*(float *)&D_L00_00161408, *(float *)&D_L00_00161408) + 1.0f)));
                    n2 = FUN_001fa6d0_2adb30(FUN_001f96b0_2adb30((float)*(int *)&D_L00_00161404 * (FUN_002132a8_2adb30(-*(float *)&D_L00_00161408, *(float *)&D_L00_00161408) + 1.0f)));
                    v2.f[2] -= *(float *)&D_L00_001613F0 * frame_time_sq_macro * (float)n1;
                    spawn_spark(&m->pos, &spark_velocity, &v2, p0, p1, n0, n1, n2, *(int *)&D_L00_00161430);
                }
            }
        }
        {
            BombHit_2adb30 q;
            m->rx = FUN_001fa580_2adb30(m->rx, D_0015ED6C_2adb30 * 2.0943952f);
            m->ry = FUN_001fa580_2adb30(m->ry, D_0015ED6C_2adb30 * 7.8539815f);
            qcopy_nc(&old, &m->pos);
            FUN_001f9a10_2adb30(&m->pos, &m->pos, d);
            if (d->h68 == 0) {
                d->v.f[2] -= frame_time_sq_macro * 11.0f;
            } else {
                d->v.f[2] -= frame_time_sq_macro * 11.0f / 10.0f;
                if (FUN_00213260_2adb30(FUN_001f96f8_2adb30(6) - 1) == 0) {
                    float h = FUN_002135f0_2adb30((float *)&m->pos, &q);
                    FUN_001f9a68_2adb30(&q, d, 0.25f);
                    random_integer_below_cf(&m->pos, &q, FUN_002132a8_2adb30(0.05f, 0.1f) * 210000.0f, h);
                }
            }
            q.moby = m;
            q.flags = 0x830000;
            if (d->h54 < FUN_001f96f8_2adb30(0x12C) - FUN_001f96f8_2adb30(0xA)) {
                q.flags |= 1;
            }
            q.iE0 = 1;
            q.fDC = 2.0f;
            qcopy(&q, d);
            trooper_scale(&q, &q, 1.0f);
            q.dir.f[2] = 1.0f;
            q.dir.f[3] = 5627.9248f;
            q.kind = 2;
            q.cls = m->cls;
            q.on = 1;
            if (d->h68 != 0) {
                d->h68++;
            }
            if (FUN_001efa68_2adb30(&old, &m->pos, 0x10, (int)m, (int)&q)) {
                if (d->h68 == 0 && collision_hit_material() == 0 && d->v.f[2] < 0.0f) {
                    Vec_2adb30 p;
                    Vec_2adb30 vel;
                    char *s;
                    int i;
                    FUN_L00_002a3ec8_2adb30(*(int *)&D_L00_001612D0_gp, *(int *)&D_L00_001612D8_gp, 1, m->pos.f[0], m->pos.f[1], 0.5f, -0.35f);
                    d->h68 = 1;
                    FUN_001f99f8_2adb30(d->v.f);
                    d->v.f[2] = D_0015ED6C_2adb30 * -1.5f;
                    p.f[0] = m->pos.f[0];
                    p.f[1] = m->pos.f[1];
                    p.f[2] = D_L00_00173E68_2adb30[0];
                    s = FUN_L00_002d7e90_2adb30(&p, 2.0f);
                    if (s != 0) {
                        s[0x23] = 0x70;
                    }
                    for (i = 0; i < 16; i++) {
                        float ang = FUN_00213308_2adb30();
                        float r = FUN_002132a8_2adb30(D_0015ED6C_2adb30 * 0.0f, D_0015ED6C_2adb30 * 3.0f);
                        int n;
                        vel.f[0] = FUN_001f9dc8_2adb30(ang) * r;
                        vel.f[1] = FUN_001f9de0_2adb30(ang) * r;
                        vel.f[2] = FUN_002132a8_2adb30(D_0015ED6C_2adb30 * 3.0f, D_0015ED6C_2adb30 * 6.5f);
                        n = FUN_L00_00257b90_2adb30(0x5A, 0x78);
                        FUN_L00_0026f548_2adb30(&p, &vel, FUN_00213260_2adb30(2), n);
                    }
                } else if (d->h68 != 0 || collision_hit_material() != 0) {
                    if (coll_hit.moby != 0) {
                        if ((coll_hit.moby != D_G910.cam && coll_hit.moby != d->owner) || d->h54 < FUN_001f96f8_2adb30(0x12C) - FUN_001f96f8_2adb30(0xA)) {
                            if (((BombMoby_2adb30 *)coll_hit.moby)->f74 != D_L00_00160770 || coll_hit.moby->cls == 0x336) {
                                H910 *h = &coll_hit;
                                m->bBC = 1;
                                qcopy(&m->pos, &h->pos);
                                FUN_L00_001ff660_2adb30(&a, d, &h->nrm);
                                FUN_001f9bf8_2adb30(&a, &a, D_0015ED6C_2adb30 * 2.0f);
                                FUN_001f9bf8_2adb30(&b, &h->nrm, 1.0f);
                            }
                        }
                    } else if (coll_hit.i1C > 0) {
                        H910 *h = &coll_hit;
                        m->bBC = 1;
                        qcopy(&m->pos, &h->pos);
                        FUN_L00_001ff660_2adb30(&a, d, &h->nrm);
                        FUN_001f9bf8_2adb30(&a, &a, D_0015ED6C_2adb30 * 2.0f);
                        FUN_001f9bf8_2adb30(&b, &h->nrm, 1.0f);
                    }
                }
            } else if (FUN_001f9770_2adb30(&d->h54)) {
                m->bBC = 1;
                FUN_001f9a10_2adb30(&a, &a, d);
            }
            if (m->bBC != 0) {
                float s = FUN_001fa6c0_2adb30((unsigned char)D_E910.p0[0xA]) + 1.0f;
                float dist;
                Vec_2adb30 v;
                int i;
                FUN_001f9a68_2adb30(&a, &a, s);
                FUN_001f9a28_2adb30(&delta, g_debug_camera, &m->pos);
                dist = FUN_001f9af0_2adb30(&delta);
                if (d->h68 < FUN_001f96f8_2adb30(0x14)) {
                    int n;
                    n = probe_world_sphere(&m->pos, 0x10, m, 0, s * 0.5f);
                    v.q = m->pos.q;
                    mine_blast_mobys(m, &v, D_L00_00177F00_2adb30, n, 0, 0x830000, 2, 1, 2.0f, 1.0f, 1.0f);
                    m->state = 2;
                }
                {
                    int k;
                    for (k = 0; k < 10; k++) {
                        Vec_2adb30 t;
                        float dp;
                        t.q = 0;
                        t.f[0] = FUN_002132a8_2adb30(-1.0f, 1.0f);
                        t.f[1] = FUN_002132a8_2adb30(-1.0f, 1.0f);
                        v.q = t.q;
                        dp = FUN_001f9ab0_2adb30(&v, &b);
                        FUN_001f9bf8_2adb30(&proj, &b, dp);
                        FUN_001f9a28_2adb30(&v, &v, &proj);
                        FUN_001f9bf8_2adb30(&proj, &b, FUN_002132a8_2adb30(0.0f, 1.0f));
                        FUN_001f9a10_2adb30(&v, &v, &proj);
                        FUN_001f9bf8_2adb30(&v, &v, s * FUN_002132a8_2adb30(3.5f, 6.5f) * D_0015ED6C_2adb30);
                        FUN_001f9a10_2adb30(&v, &v, &a);
                        FUN_L00_002af450_2adb30((char *)&m->pos, (char *)&v, FUN_L00_00257b90_2adb30(FUN_001f96f8_2adb30(0x3C), FUN_001f96f8_2adb30(0x78)), 0, (unsigned char)D_E910.p0[0xA]);
                    }
                }
                if (d->h68 < FUN_001f96f8_2adb30(0x14)) {
                    Vec_2adb30 t;
                    float g;
                    int r2;
                    int g2;
                    int b2;
                    int cnt;
                    {
                        int k;
                        for (k = 0; k < 4; k++) {
                            float dp;
                            t.q = 0;
                            t.f[0] = FUN_002132a8_2adb30(-1.0f, 1.0f);
                            t.f[1] = FUN_002132a8_2adb30(-1.0f, 1.0f);
                            v.q = t.q;
                            dp = FUN_001f9ab0_2adb30(&v, &b);
                            FUN_001f9bf8_2adb30(&proj, &b, dp);
                            FUN_001f9a28_2adb30(&v, &v, &proj);
                            FUN_001f9bf8_2adb30(&proj, &b, FUN_002132a8_2adb30(0.0f, 1.0f));
                            FUN_001f9a10_2adb30(&v, &v, &proj);
                            FUN_001f9bf8_2adb30(&v, &v, s * FUN_002132a8_2adb30(6.5f, 10.0f) * D_0015ED6C_2adb30);
                            FUN_001f9a10_2adb30(&v, &v, &a);
                            FUN_L00_002af450_2adb30((char *)&m->pos, (char *)&v, FUN_L00_00257b90_2adb30(FUN_001f96f8_2adb30(0x3C), FUN_001f96f8_2adb30(0x5A)), 1, (unsigned char)D_E910.p0[0xA]);
                        }
                    }
                    t.q = 0;
                    t.f[0] = FUN_002132a8_2adb30(-1.0f, 1.0f);
                    t.f[1] = FUN_002132a8_2adb30(-1.0f, 1.0f);
                    t.f[2] = FUN_002132a8_2adb30(-1.0f, 1.0f);
                    v.q = t.q;
                    delta.f[2] += dist * 0.5f;
                    FUN_001f9bf8_2adb30(&v, &v, dist / 5.0f * D_0015ED6C_2adb30);
                    FUN_001f9bf8_2adb30(&delta, &delta, dist * 2.0f * D_0015ED6C_2adb30);
                    FUN_001f9a10_2adb30(&v, &v, &delta);
                    FUN_001f9c90_2adb30(&v, &v, D_0015ED6C_2adb30 * 10.0f);
                    FUN_L00_002af450_2adb30((char *)&m->pos, (char *)&v, FUN_L00_00257b90_2adb30(FUN_001f96f8_2adb30(0x3C), FUN_001f96f8_2adb30(0x5A)), 1, (unsigned char)D_E910.p0[0xA]);
                    if (dist < 6.0f) {
                        cnt = FUN_001fa6d0_2adb30(dist) + 1;
                    } else {
                        cnt = 4;
                    }
                    g = 0.0f;
                    if (dist < 7.0f) {
                        g = 7.0f - dist;
                    }
                    for (i = 0; i < cnt; i++) {
                        float spd = s * FUN_002132a8_2adb30(8.0f, 10.0f) * D_0015ED6C_2adb30 - g * D_0015ED6C_2adb30;
                        Colors6_2adb30 c1 = D_L00_001E9920;
                        Colors6_2adb30 c2 = D_L00_001E9938;
                        FUN_L00_0026a9f0_2adb30(&m->pos, &a,
                                          FUN_L00_0025c0e8_2adb30(c1.c[FUN_00213260_2adb30(6)], (unsigned char)D_E910.p0[0xA]),
                                          FUN_L00_0025c0e8_2adb30(c2.c[FUN_00213260_2adb30(6)], (unsigned char)D_E910.p0[0xA]),
                                          s * 400000.0f,
                                          FUN_L00_00257b90_2adb30(FUN_001f96f8_2adb30(0xF), FUN_001f96f8_2adb30(0x14)),
                                          FUN_L00_00257b90_2adb30(FUN_001f96f8_2adb30(0x19), FUN_001f96f8_2adb30(0x1E)),
                                          0, 0, spd);
                    }
                    if (dist > 9.0f) {
                        int r;
                        int gg;
                        int bb;
                        FUN_L00_002df808_2adb30(m, &m->pos, &a, s * 4.0f, FUN_001f96f8_2adb30(0xF), 0x7F, 0x7F, 0x7F, 0x20);
                        r = 0x7F;
                        gg = 0x20;
                        bb = 0;
                        FUN_L00_0025c088_2adb30(&r, &gg, &bb, (unsigned char)D_E910.p0[0xA]);
                        FUN_L00_002df808_2adb30(m, &m->pos, &a, s * 4.0f, FUN_001f96f8_2adb30(0x18), r, gg, bb, 0x20);
                    }
                    r2 = 0x7F;
                    g2 = 0x3F;
                    b2 = 0;
                    FUN_L00_0025c088_2adb30(&r2, &g2, &b2, (unsigned char)D_E910.p0[0xA]);
                    FUN_L00_002df808_2adb30(m, &m->pos, &a, s * 4.0f, FUN_001f96f8_2adb30(0x14), r2, g2, b2, 0x30);
                    r2 = 0x60;
                    g2 = 0x10;
                    b2 = 0;
                    FUN_L00_0025c088_2adb30(&r2, &g2, &b2, (unsigned char)D_E910.p0[0xA]);
                    FUN_L00_002df808_2adb30(m, &m->pos, &a, s * 3.5f, FUN_001f96f8_2adb30(0x1B), r2, g2, b2, 0x40);
                    r2 = 0x20;
                    g2 = 0;
                    b2 = 0;
                    FUN_L00_0025c088_2adb30(&r2, &g2, &b2, (unsigned char)D_E910.p0[0xA]);
                    FUN_L00_002df808_2adb30(m, &m->pos, &a, s * 3.0f, FUN_001f96f8_2adb30(0x1D), r2, g2, b2, 0x20);
                }
                if (d->h68 >= FUN_001f96f8_2adb30(0x14)) {
                    float h;
                    m->pos.f[2] += 1.0f;
                    h = FUN_002135f0_2adb30((float *)&m->pos, &v);
                    m->pos.f[2] -= 1.0f;
                    {
                        int i;
                        for (i = 0; i < 150; i++) {
                            Vec_2adb30 u;
                            Vec_2adb30 t;
                            t.q = 0;
                            t.f[0] = FUN_002132a8_2adb30(-1.0f, 1.0f);
                            t.f[1] = FUN_002132a8_2adb30(-1.0f, 1.0f);
                            t.f[2] = FUN_002132a8_2adb30(-0.5f, 2.0f);
                            u.q = t.q;
                            FUN_001f9bf8_2adb30(&u, &u, s * FUN_002132a8_2adb30(D_0015ED6C_2adb30 * 4.0f, D_0015ED6C_2adb30 * 8.0f));
                            random_integer_below_cf(&m->pos, &u, FUN_002132a8_2adb30(0.05f, 0.1f) * 210000.0f, h);
                        }
                    }
                }
                d->v.f[0] = 0.0f;
                m->bBC = 0;
                if (d->h68 >= FUN_001f96f8_2adb30(0x14)) {
                    trooper_play_sound(0x16, 0, (int)m, 0);
                } else if ((unsigned char)D_E910.p0[0xA] != 0) {
                    FUN_0022da68_2adb30(1, 0, (int)m);
                } else {
                    FUN_0022da68_2adb30(0, 0, (int)m);
                }
                if (d->h68 != 0) {
                    float gz;
                    float wz;
                    int cr = 0x60;
                    int cg = 0x60;
                    int cb = 0x7F;
                    int delay = 0;
                    v.q = m->pos.q;
                    v.f[2] += 3.5f;
                    gz = FUN_00213508_2adb30(&v, 0, 0.5f);
                    wz = allocate_voice_for_target_entry_cf(&v);
                    if (gz != wz && gz - wz < 1.2f) {
                        Vec_2adb30 v2;
                        Vec_2adb30 w;
                        int i;
                        int j;
                        for (i = 0; i < 100; i++) {
                            float r;
                            float ang;
                            w.q = 0;
                            w.f[0] = FUN_002132a8_2adb30(D_0015ED6C_2adb30 * -0.75f, D_0015ED6C_2adb30 * 0.75f);
                            w.f[1] = FUN_002132a8_2adb30(D_0015ED6C_2adb30 * -0.75f, D_0015ED6C_2adb30 * 0.75f);
                            w.f[2] = FUN_002132a8_2adb30(D_0015ED6C_2adb30 * 4.0f, D_0015ED6C_2adb30 * 14.0f);
                            v2.q = w.q;
                            r = FUN_002132a8_2adb30(0.0f, 0.35f);
                            ang = FUN_00213308_2adb30();
                            w.f[0] = FUN_001f9dc8_2adb30(ang) * r;
                            w.f[1] = FUN_001f9de0_2adb30(ang) * r;
                            w.f[2] = 0.0f;
                            FUN_001f9a10_2adb30(&w, &w, &m->pos);
                            w.f[2] = gz + 0.5f;
                            v2.f[2] -= r * D_0015ED6C_2adb30 * 8.0f;
                            FUN_L00_002738e8_2adb30(&w, &v2, delay, FUN_002132a8_2adb30(0.25f, 1.0f), gz, frame_time_sq_macro * 20.0f, (0x16 << 24) | (cb << 16) | (cg << 8) | cr, (cb << 16) | (cg << 8) | cr, 1);
                        }
                        for (j = 0; j < 20; j++) {
                            int c0 = mine_blend_color(0x7F << 24, (0x7F << 24) | (cb << 16) | (cg << 8) | cr, FUN_002132a8_2adb30(0.25f, 1.0f));
                            int c1 = mine_blend_color(0, ((cb / 2) << 16) | ((cg / 2) << 8) | (cr / 2), FUN_002132a8_2adb30(0.5f, 1.0f));
                            float sz;
                            int life;
                            w.f[0] = FUN_002132a8_2adb30(-3.0f, 3.0f);
                            w.f[1] = FUN_002132a8_2adb30(-3.0f, 3.0f);
                            w.f[2] = FUN_002132a8_2adb30(D_0015ED6C_2adb30 * 4.0f, D_0015ED6C_2adb30 * 14.0f);
                            FUN_001f9bf8_2adb30(&w, &w, FUN_002132a8_2adb30(1.5f, 3.0f) * D_0015ED6C_2adb30);
                            w.f[2] = FUN_002132a8_2adb30(3.0f, 6.0f) * D_0015ED6C_2adb30;
                            v2.q = m->pos.q;
                            v2.f[2] = gz;
                            sz = FUN_002132a8_2adb30(31500.002f, 52500.0f);
                            life = FUN_L00_00257b90_2adb30(FUN_001f96f8_2adb30(0x5A), FUN_001f96f8_2adb30(0x96));
                            FUN_L00_0026bb70_2adb30(&v2, &w, c0, c1, sz, life, 1, D_L00_001B2080_2adb30[11][0], -1);
                        }
                    }
                }
                player_camera.f160 = dist < 20.0f ? 0.4f - dist * 0.0175f : 0.4f - 20.0f * 0.0175f;
                player_camera.i168 = FUN_001f96f8_2adb30(0x19);
                distance_xyz(&m->pos, D_G910.pos);
                mine_camera_blast((char *)&rec, (char *)&m->pos, 0, 0);
                if (m->state != 2) {
                    FUN_0020c828_2adb30(m);
                    return;
                }
            }
        }
    } else if (m->state != 2) {
        int v;
        d->h6a++;
        v = FUN_001fa6d0_2adb30(FUN_001f9de0_2adb30(normalize_angle((float)d->h6a * 0.068f)) * 96.0f) + 0x9F;
        m->color = (v << 16) | (0xFF << 24) | (v << 8) | v;
    }
    if (m->state == 2) {
        Vec_2adb30 p;
        float r;
        int n;
        m->flags |= 0x41;
        r = FUN_L00_0025b6a8_2adb30(0.5f, (FUN_001fa6c0_2adb30(*((unsigned char *)&D_E910 + 0xA)) + 1.0f) * 3.0f - 0.5f,
                              FUN_001fa6c0_2adb30(m->bBC) / (float)FUN_001f96f8_2adb30(0xF));
        n = probe_world_sphere(&m->pos, 0x10, m, 0, r);
        p.q = m->pos.q;
        mine_blast_mobys(m, &p, D_L00_00177F00_2adb30, n, 0, 0x830000, 2, 1, 2.0f, 1.0f, 1.0f);
        if (++m->bBC > FUN_001f96f8_2adb30(0xF)) {
            FUN_0020c828_2adb30(m);
        }
    }
}
extern float D_0015ED6C_2af450 __asm__("D_0015ED6C");
unsigned char *FUN_0020c4f8_2af450(int) __asm__("FUN_0020c4f8");
void FUN_L00_002502f0_2af450(void *, int, int, int) __asm__("FUN_L00_002502f0");
float FUN_002132a8_2af450(float, float) __asm__("FUN_002132a8");
void FUN_L00_00250df8_2af450(void *) __asm__("FUN_L00_00250df8");
unsigned char *spawn_debris(void *pos, void *vel, int n, int flag, int g) __asm__("FUN_L00_002af450");
unsigned char *spawn_debris(void *pos, void *vel, int n, int flag, int g) {
    unsigned char *m = FUN_0020c4f8_2af450(0x7A);
    if (m) {
        unsigned char *v;
        float a, d;
        v = *(unsigned char **)(m + 0x78);
        m[0x30] = 0xFF;
        *(short *)(m + 0x32) = 0xFF;
        m[0x31] = 1;
        m[0x23] = 0x80;
        FUN_L00_002502f0_2af450(m, 0x7F, 0x7F, 0x7F);
        qcopy(m + 0x10, pos);
        qcopy(v, vel);
        a = FUN_002132a8_2af450(D_0015ED6C_2af450 * 6.2831855f, D_0015ED6C_2af450 * 12.566371f);
        d = D_0015ED6C_2af450;
        *(float *)(v + 0x10) = a;
        *(float *)(v + 0x14) = FUN_002132a8_2af450(d * 6.2831855f, d * 12.566371f);
        *(short *)(v + 0x18) = n;
        *(short *)(v + 0x1A) = n;
        if (flag == 0) {
            *(float *)(m + 0x2C) *= FUN_002132a8_2af450(0.5f, 0.75f);
        }
        *(float *)(v + 0x1C) = *(float *)(m + 0x2C);
        m[0xBC] = flag | (g << 1);
        FUN_L00_00250df8_2af450(m);
    }
    return m;
}
typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2af5b8;
typedef struct {
    int c[4];
} C_2af5b8;
typedef struct {
    float x0, x4, x8, xc;
    float x10, x14;
    short x18, x1a;
    float x1c;
} W_2af5b8;
extern float D_0015ED6C_2af5b8 __asm__("D_0015ED6C");
extern float D_0015ED70_2af5b8 __asm__("D_0015ED70");
float rndf_2af5b8(float, float) __asm__("FUN_002132a8");
void scale_2af5b8(void *, void *, float) __asm__("FUN_001f9bf8");
int rndi_2af5b8(int) __asm__("FUN_00213260");
int FUN_L00_0025c0e8_c(int, int) __asm__("FUN_L00_0025c0e8");
int fr_2af5b8(int) __asm__("FUN_001f96f8");
void FUN_L00_00269958(void *, void *, int, int, int, int, int, int);
float app_2af5b8(float, float) __asm__("FUN_001fa580");
void add_2af5b8(void *, void *, void *) __asm__("FUN_001f9a10");
void kill_2af5b8(void *) __asm__("FUN_0020c828");
float itof_2af5b8(int) __asm__("FUN_001fa6c0");
int tick_2af5b8(void *) __asm__("FUN_001f9770");
void debris_update(unsigned char *m) __asm__("FUN_L00_002af5b8");
extern C_2af5b8 D_L00_00161438 __attribute__((section(".data")));
void debris_update(unsigned char *m) {
    W_2af5b8 *w = *(W_2af5b8 **)(m + 0x78);
    C_2af5b8 c;
    V_2af5b8 v;
    V_2af5b8 t;
    int i;
    float f;
    if (m[0xBC] & 1) {
        for (i = 0; i < 1; i++) {
            c = D_L00_00161438;
            *(OvlQuad *)&t = 0;
            t.f[0] = rndf_2af5b8(-1.0f, 1.0f);
            t.f[1] = rndf_2af5b8(-1.0f, 1.0f);
            t.f[2] = rndf_2af5b8(-1.0f, 1.0f);
            *(OvlQuad *)&v = *(OvlQuad *)&t;
            scale_2af5b8(&v, &v, D_0015ED6C_2af5b8);
            if (m[0xBC] & 0xFE) {
                int a = FUN_L00_0025c0e8_c(c.c[rndi_2af5b8(4)], m[0xBC] >> 1);
                int b = FUN_L00_0025c0e8_c(0x4FFF, m[0xBC] >> 1);
                FUN_L00_00269958(m + 0x10, &v, a, b, fr_2af5b8(0x2D), 0x32, 0x78, 1);
            } else {
                FUN_L00_00269958(m + 0x10, &v, c.c[rndi_2af5b8(4)], 0x4FFF, fr_2af5b8(0x2D), 0x32, 0x78,
                            1);
            }
        }
    }
    *(float *)(m + 0x40) = app_2af5b8(*(float *)(m + 0x40), w->x10);
    *(float *)(m + 0x44) = app_2af5b8(*(float *)(m + 0x44), w->x14);
    add_2af5b8(m + 0x10, m + 0x10, w);
    w->x8 -= D_0015ED70_2af5b8 * 14.6f;
    if (*(float *)(m + 0x10) < 0.0f || *(float *)(m + 0x14) < 0.0f || *(float *)(m + 0x18) < 0.0f) {
        kill_2af5b8(m);
        return;
    }
    if (w->x18 < w->x1a / 4) {
        f = itof_2af5b8(w->x18);
        *(float *)(m + 0x2C) = w->x1c * f / itof_2af5b8(w->x1a / 4);
    }
    if (tick_2af5b8(&w->x18))
        kill_2af5b8(m);
}
extern float D_0015ED6C_2af8b0 __asm__("D_0015ED6C");
typedef struct {
    void *o;
    float f4, f8;
    short hc, he;
    unsigned char p10[0x10];
    int i20;
    float f24, f28;
} V_2af8b0;
unsigned char *FUN_0020c4f8_2af8b0(int) __asm__("FUN_0020c4f8");
float FUN_00213308_2af8b0(void) __asm__("FUN_00213308");
int FUN_001f96f8_2af8b0(int) __asm__("FUN_001f96f8");
float FUN_L00_00257c48_2af8b0(float, float) __asm__("FUN_L00_00257c48");
float FUN_002132a8_2af8b0(float, float) __asm__("FUN_002132a8");
void FUN_L00_00250df8_2af8b0(void *) __asm__("FUN_L00_00250df8");
unsigned char *spawn_crate_debris(unsigned char *owner, void *vel, void *pos, void *rot, int type,
                                int n, float lo, float hi) __asm__("FUN_L00_002af8b0");
extern float g48_2afa48 __asm__("D_L00_00161448") __attribute__((sda));
extern float g4c_2afa48 __asm__("D_L00_0016144C") __attribute__((sda));
unsigned char *spawn_crate_debris(unsigned char *owner, void *vel, void *pos, void *rot, int type,
                                int n, float lo, float hi) {
    unsigned char *m = FUN_0020c4f8_2af8b0(type);
    if (m) {
        V_2af8b0 *v = *(V_2af8b0 **)(m + 0x78);
        float a;
        v->o = owner;
        m[0x30] = 0xFF;
        *(short *)(m + 0x32) = 0x7E;
        m[0x31] = 1;
        *(long *)(m + 0x38) = *(long *)(owner + 0x38);
        m[0x20] = 0;
        *(float *)(m + 0x40) = FUN_00213308_2af8b0();
        *(float *)(m + 0x44) = FUN_00213308_2af8b0();
        *(float *)(m + 0x48) = FUN_00213308_2af8b0();
        qcopy(m + 0x10, pos);
        qcopy(m + 0x40, rot);
        qcopy(v->p10, vel);
        v->f4 = *(float *)(m + 0x18);
        {
            short t = FUN_001f96f8_2af8b0(0x14);
            v->i20 = 0;
            v->he = t;
        }
        v->f24 = FUN_L00_00257c48_2af8b0(g48_2afa48, g4c_2afa48) *
                 DEG_TO_RAD * D_0015ED6C_2af8b0;
        v->f28 = FUN_L00_00257c48_2af8b0(g48_2afa48, g4c_2afa48) *
                 DEG_TO_RAD * D_0015ED6C_2af8b0;
        a = FUN_002132a8_2af8b0(lo, hi);
        *(float *)(m + 0x2C) *= a;
        v->hc = n;
        v->f8 = a * 0.25f;
        FUN_L00_00250df8_2af8b0(m);
    }
    return m;
}
#include "eetypes.h"
#include "qcopy.h"
typedef struct {
    u8 pad0[4];
    f32 f4;
    f32 f8;
    s16 hC;
    s16 hE;
    f32 vel[4] __attribute__((aligned(16)));
    f32 f20, f24, f28;
} PV_2afa48;
typedef struct {
    u8 pad0[0x10];
    f32 pos[4] __attribute__((aligned(16)));
    u8 b20, b21, b22, b23;
    u8 pad24[8];
    f32 f2c;
    u8 pad30[0x10];
    f32 f40, f44, f48;
    u8 pad4c[0x2C];
    PV_2afa48 *pv;
} M_2afa48;
extern f32 g70_2afa48 __asm__("D_L00_00161470") __attribute__((sda));
extern s32 g74_2afa48 __asm__("D_L00_00161474") __attribute__((sda));
extern u128 coll_hit_normal[] __asm__("D_L00_00173E80") __attribute__((section(".data")));
extern void vadd_2afa48(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32 appr_2afa48(f32, f32) __asm__("FUN_001fa580");
extern s32 tick_2afa48(void *) __asm__("FUN_001f9770");
extern s32 coll_sphere(void *, s32, s32, f32) __asm__("FUN_L00_001f0d60");
extern f32 dot_2afa48(void *, void *) __asm__("FUN_001f9ab0");
extern void refl_2afa48(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void vscl_2afa48(void *, void *, f32) __asm__("FUN_001f9a68");
extern f32 rnd_2afa48(f32, f32) __asm__("FUN_L00_00257c48");
extern void kill_2afa48(void *) __asm__("FUN_0020c828");
void nanotech_crate_fragment_update(M_2afa48 *m) __asm__("FUN_L00_002afa48");
void nanotech_crate_fragment_update(M_2afa48 *m) {
    PV_2afa48 *pv = m->pv;
    u128 old;
    u128 *t;
    qcopy(&old, m->pos);
    pv->vel[2] -= frame_time_sq * 20.0f;
    vadd_2afa48(m->pos, m->pos, pv->vel);
    m->f40 = appr_2afa48(m->f40, pv->f20);
    m->f44 = appr_2afa48(m->f44, pv->f24);
    m->f48 = appr_2afa48(m->f48, pv->f28);
    switch (m->b20) {
    case 0:
        if (m->pos[2] < pv->f4 - 3.0f) {
            m->b20 = 1;
        } else if (tick_2afa48(&pv->hE) && pv->hC != 0 && coll_sphere(m->pos, 2, 0, pv->f8)) {
            t = coll_hit_normal;
            if (dot_2afa48(pv->vel, t) < 0.0f) {
                qcopy(m->pos, t - 1);
                pv->hC--;
                refl_2afa48(pv->vel, pv->vel, t);
                vscl_2afa48(pv->vel, pv->vel, 0.75f);
                pv->f20 = 0.0f;
                pv->f24 = rnd_2afa48(g48_2afa48, g4c_2afa48) * DEG_TO_RAD * frame_time;
                pv->f28 = rnd_2afa48(g48_2afa48, g4c_2afa48) * DEG_TO_RAD * frame_time;
            }
        }
        break;
    case 1:
        m->f2c *= g70_2afa48;
        if (m->b23 < g74_2afa48) {
            kill_2afa48(m);
        } else {
            m->b23 -= g74_2afa48;
        }
        break;
    }
}
typedef union {
    OvlQuad q;
    float f[4];
} V_2afc70;
extern float D_0015ED6C_2afc70 __asm__("D_0015ED6C");
extern V_2afc70 hero_pos __asm__("D_0013F3D0") __attribute__((section(".data")));
extern unsigned char D_0013F490_2afc70[] __asm__("D_0013F490") __attribute__((section(".data")));
extern void *D_001413D0_2afc70 __asm__("D_001413D0") __attribute__((section(".data")));
extern unsigned char D_L00_00173E60_2afc70[] __asm__("D_L00_00173E60")
    __attribute__((section(".data")));
extern s16 D_L00_001B0770_lui[] __asm__("D_L00_001B0770") __attribute__((section(".data")));
extern unsigned char *FUN_0020c4f8_2afc70(int) __asm__("FUN_0020c4f8");
extern unsigned char *FUN_002141f8_2afc70(void *) __asm__("FUN_002141f8");
extern int FUN_001f96f8_2afc70(int) __asm__("FUN_001f96f8");
extern float FUN_001f9af0_2afc70(void *) __asm__("FUN_001f9af0");
extern int FUN_001efa68_2afc70(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void FUN_L00_00250df8_2afc70(void *) __asm__("FUN_L00_00250df8");
extern void FUN_L00_0025f780_2afc70(void *, void *, int) __asm__("FUN_L00_0025f780");

unsigned char *FUN_L00_002afc70(unsigned char *a, V_2afc70 *pos, unsigned char *tgt, float f12,
                                float f13, float f14, float f15, float f16) {
    V_2afc70 tmp;
    unsigned char *m;
    unsigned char *v;
    unsigned char *r;
    float t;
    int n;
    float x;

    m = FUN_0020c4f8_2afc70(0x99);
    if (m != 0) {
        v = *(unsigned char **)(m + 0x78);
        m[0x30] = 0xFF;
        *(short *)(m + 0x32) = 0xFF;
        m[0x31] = 1;
        *(float *)(m + 0x48) = f15;
        *(float *)(m + 0x44) = f16;
        m[0x20] = 0;
        *(unsigned char **)(v + 0x10) = a;
        qcopy(m + 0x10, pos);
        t = D_0015ED6C_2afc70 * 20.0f;
        *(int *)(v + 0x38) = 1;
        *(float *)(v + 0x48) = 70.0f;
        *(float *)(v + 0x34) = t;
        *(int *)(v + 0x24) = 0;
        *(int *)(v + 0x28) = 0;
        *(unsigned char **)(v + 0x18) = tgt;
        *(float *)(v + 0x20) = t / 3.0f;
        if (tgt != 0) {
            r = FUN_002141f8_2afc70(tgt);
            if (r != 0) {
                *(unsigned short *)(r + 0x1E) |= 0x80;
                if (r[0xB]) {
                    *(int *)(v + 0x38) = 0;
                }
                if (r[0xC]) {
                    *(float *)(v + 0x34) = (float)r[0xC] * D_0015ED6C_2afc70;
                }
                if (D_0015ED6C_2afc70 * 20.0f < *(float *)(v + 0x34)) {
                    *(float *)(v + 0x48) = 140.0f;
                }
            }
        }
        *(float *)(v + 0x2C) = f12;
        *(float *)(v + 0x30) = f13;
        *(int *)(v + 0x14) = FUN_001f96f8_2afc70(300);
        if (tgt != 0) {
            qcopy(v, tgt + 0x10);
            *(float *)(v + 0x1C) = f14;
            *(float *)(v + 8) += f14;
        }
        x = FUN_001f9af0_2afc70(D_0013F490_2afc70);
        n = FUN_001f96f8_2afc70(D_0015ED6C_2afc70 * 0.1f < x ? 0x3C : 0xF);
        *(short *)(v + 0x3E) = n;
        *(short *)(v + 0x3C) = n;
        if (a != 0 && a[0x20] != 0xFE && a[0x20] != 0xFD) {
            tmp.q = ((V_2afc70 *)(a + 0x10))->q;
        } else {
            qcopy(&tmp, &hero_pos);
            tmp.f[2] += 0.6f;
        }
        tmp.f[2] = pos->f[2];
        if (FUN_001efa68_2afc70(&tmp, pos, 0, D_001413D0_2afc70, 0)) {
            m[0xBC] = 1;
            qcopy(m + 0x10, D_L00_00173E60_2afc70);
        }
        FUN_L00_00250df8_2afc70(m);
        FUN_L00_0025f780_2afc70(m, D_L00_001B0770_lui, 0x1F);
        *(float *)(m + 0x2C) += *(float *)(m + 0x2C);
    }
    return m;
}
/* Sums n steps of a value easing toward 20 * D_0015ED6C by a tenth (times D_0015ED64) per step. */
float FUN_L00_002aff50(int n) {
    float sum = 0.0f, v = sum;
    if (n > 0) {
        float t = frame_time * 20.0f;
        do {
            v += (t - v) / 10.0f * frame_scale_sq;
            n--;
            sum += v;
        } while (n);
    }
    return sum;
}
typedef int ti_t __attribute__((mode(TI), aligned(16)));
typedef union {
    ti_t q;
    f32 f[4];
} VU;
typedef struct {
    VU v;
    s32 w10;
    s32 w14;
    char *owner;
    f32 f1C;
    s32 w20;
    s32 w24;
    s32 w28;
    f32 f2C;
    f32 f30;
    s32 w34;
    s32 w38;
} O;
typedef struct {
    char p0[0xB];
    u8 bB;
} E_u;

extern char *active_mobies[] __asm__("D_L00_001ABA00") __attribute__((section(".data")));
extern E_u D_0013E520_u __asm__("D_0013E520");
extern s32 D_L00_00166DC0[] __attribute__((section(".data")));
extern s32 D_L00_00173E40[0x1C] __attribute__((section(".data")));

extern f32 FUN_001f9b48(void *, void *);
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32 fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern f32 FUN_001f99c0(f32);
extern f32 fast_arc_sin(f32) __asm__("FUN_001f9df8");
extern f32 convert_integer_to_float(s32) __asm__("FUN_001fa6c0");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_00214db0(void *, f32, f32, f32);
extern s32 FUN_001efa68_u(void *, void *, s32, char *, void *) __asm__("FUN_001efa68");
extern s32 FUN_002141f8_u(char *) __asm__("FUN_002141f8");
extern s32 FUN_L00_0025f878(char *);

void FUN_L00_002affa8(char *self, O *out, char *skip) {
    VU v;
    VU lv;
    char *o;
    s32 p;
    s32 i;
    s32 flag;
    f32 best, yaw, pitch, dist, ang, z, angh, angv;
    char *spos;
    char *opos;
    struct Hero *g;

    i = 0;
    best = 10000.0f;
    yaw = *(f32 *)(self + 0x48);
    pitch = *(f32 *)(self + 0x44);
    o = active_mobies[i];
    if (o == 0)
        return;
    g = &hero;
    while (o != 0) {
        if (*(s8 *)(o + 0x20) < 0)
            goto next;
        if (o == skip)
            goto next;
        qcopy(&v, o + 0x10);
        spos = self + 0x10;
        opos = o + 0x10;
        if (40.0f < FUN_001f9b48(&v, spos))
            goto next;
        p = FUN_002141f8_u(o);
        if (p == 0)
            goto np;
        if (p <= 0x100000)
            goto np;
        if (0x3FFFFF < p)
            goto np;
        {
            f32 pf = *(f32 *)(p + 0x10);
            z = v.f[2] + pf;
        }
        goto zset;
    np:
        p = 0;
        z = v.f[2] + 0.5f;
    zset:
        v.f[2] = z;
        if (o == 0)
            goto next;
        if (*(char **)(o + 0x24) == 0)
            goto next;
        if (*(s16 *)(*(char **)(o + 0x24) + 0x46) != 5)
            goto next;
        angh = fast_arc_tan(v.f[0] - *(f32 *)(self + 0x10), v.f[1] - *(f32 *)(self + 0x14));
        angv = fast_arc_tan(vector_distance(spos, &v), v.f[2] - *(f32 *)(self + 0x18));
        dist = FUN_001f9b48(spos, &v);
        if (dist < 2.5f) {
            char *cam = *(char **)&g->moby;
            if (fast_difference_between_rotations(
                    *(f32 *)(cam + 0x48), fast_arc_tan(*(f32 *)(o + 0x10) - g->motion.pos.f[0],
                                                       *(f32 *)(o + 0x14) - g->motion.pos.f[1])) < 1.0471976f) {
                if (FUN_001f99c0(angv) < 0.7853982f)
                    return;
            }
        }
        if (best < dist)
            goto next;
        ang = convert_integer_to_float(D_0013E520_u.bB) * 0.6981317f;
        FUN_00214db0(&lv, dist, yaw, pitch + g->ground_pitch.f * 0.5f);
        fast_vec_add(&lv, &lv, spos);
        z = fast_arc_sin(FUN_001f9b48(&lv, &v) / (dist + dist));
        ang = ang + 1.5707964f;
        if (ang < z + z && p != 0) {
            ang = convert_integer_to_float(*(u8 *)(p + 0xA) << 3) * 0.125f;
            fast_arc_sin(ang / dist);
        }
        flag = 1;
        if (FUN_001efa68_u(D_L00_00166DC0, &v, 6, self, 0) != 0) {
            s32 hit = D_L00_00173E40[6];
            if (hit == 0 || hit != (s32)o)
                flag = 0;
        } else {
            best = dist;
            pitch = -angv;
            yaw = angh;
            if (FUN_L00_0025f878(o) != 0)
                goto next;
            z = 0.5f;
            out->owner = o;
            if (p != 0)
                z = *(f32 *)(p + 0x10);
            out->f2C = angh;
            out->w38 = flag;
            out->f30 = pitch;
            out->w24 = 0;
            out->w28 = 0;
            out->w20 = 0;
            qcopy(out, opos);
            out->f1C = z;
        }
    next:
        i++;
        o = active_mobies[i];
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002AB910.c: func_L00_002B1688), where it is exact; names translated to the US level program. */

typedef u32 VQ1688 __attribute__((mode(TI), aligned(16)));

typedef union {
    VQ1688 q;
    f32 f[4];
    s32 i[4];
} V1688;

typedef int Q1688 __attribute__((mode(TI)));

typedef struct {
    V1688 previous_target;
    M910 *owner;
    int life;
    M910 *target;
    float target_height;
    float speed;
    float yaw_velocity;
    float pitch_velocity;
    float yaw;
    float pitch;
    float desired_speed;
    int track_velocity;
    short turn_time;
    short turn_period;
    int bounce_count;
    int bounce_timer;
    float limit;
} Missile1688;

typedef struct { int colors[6]; } Colors1688;

typedef union {
    Qry910 collision;
    struct { V1688 direction; V1688 relative; V1688 target; } vectors;
} MissilePhase1688;

typedef struct { char pad0[0x100]; V1688 previous; char pad110[0x30]; V1688 current; } HeroMotion1688;

typedef struct { char pad0[0x18]; M910 *moby; int kind; V1688 position; char pad30[0x10]; V1688 normal, velocity_end, velocity_start; } Impact1688;

extern B910 D_L00_001E9950;
extern B910 D_L00_001E99A0;
extern Cam910 D_L00_00166C80;
extern Colors1688 D_L00_001E99F0;
extern Colors1688 D_L00_001E9A08;
extern E D_0013E520;
extern P D_0013F350;
extern V1688 D_L00_00160660 __attribute__((section(".sdata")));
extern V1688 D_L00_00160680 __attribute__((section(".sdata")));
extern char *FUN_L00_002af450(char *, char *, int, int, int);
extern char *mine_camera_blast(char *a, char *b, int c, char *d) __asm__("FUN_L00_002d3838");
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 random_angle_radians(void) __asm__("func_00213308");
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern f32 square_root_scalar(f32) __asm__("func_001F9988");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float D_0015ED6C;
extern float D_0015ED70;
extern float D_L00_0015F5D0;
extern float D_L00_0015F5D4;
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int *D_L00_00177F00[];
extern int D_L00_0015F5CC;
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int set_identity_matrix() __asm__("FUN_001f9fc8");
extern int collect_mobies_in_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern int FUN_L00_001ff290();
extern int FUN_L00_001ff2c0();
extern int FUN_L00_001ff430();
extern int FUN_L00_001ffd48();
extern int FUN_L00_001ffe20();
extern int FUN_L00_00257b90(int, int);
extern int FUN_L00_00258110();
unsigned char *spawn_glow_particle_u8(void *owner, void *pos, void *vel, float scale, int lifetime, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) __asm__("FUN_L00_002ac910");
extern int allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern int tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern s32 D_L00_00166DC0[];
extern s32 D_L00_00173E40[0x1C];
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern short D_L00_001B0770[];
extern unsigned FUN_L00_0025c0e8(unsigned c, int mask);
extern unsigned char *D_L00_001B20DC __attribute__((section(".data")));
extern void *FUN_L00_0026bb70(void *, void *, int, int, float, int, int, int, int);
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
extern void clamp_vector_length(void *, void *, float) __asm__("FUN_001f9c90");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void FUN_L00_001ff660(void *, void *, void *);
extern void FUN_L00_0025a9f8(void *, void *, void *, int, int, int, int, int, float, float, float);
extern void mine_spawn_drops(void *, void *) __asm__("FUN_L00_0025f800");
extern void FUN_L00_0026a9f0(void *, void *, int, int, float, int, int, int, int, float);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
extern void build_spherical_offset_c(void *, f32, f32, f32) __asm__("FUN_00214db0");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void transform_vector(void *, void *, void *) __asm__("func_001F9D20");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
void FUN_L00_0026a4c8(Q1688 *a, Q1688 *b, int c, int d, int n, float f);
void FUN_L00_0026cbb0(void *, void *, int, int, int, int, float);
void spawn_particle(void *, void *, int, int, int, int, float, float, float, float, float) __asm__("FUN_L00_002712b8");
void FUN_L00_002affa8(char *self, O *out, char *skip);
extern Impact1688 impact1688 __asm__("D_L00_00173E40");
extern HeroMotion1688 hero_motion1688 __asm__("D_0013F350");
extern float FUN_L00_001ff430_c(void *) __asm__("FUN_L00_001ff430");
extern void FUN_L00_001ff2c0_c(void *,void *,void *) __asm__("FUN_L00_001ff2c0");
extern float horizontal1688(void *) __asm__("FUN_001f9b20");
extern float chase1688(void *,float,float,float,float,float) __asm__("FUN_L00_00258110");
extern unsigned char *smoke1688(void *,void *,int,int,int,int,float,float,float,float,float) __asm__("FUN_L00_002712b8");
extern void rotate1688(void *,void *,void *) __asm__("FUN_001f9cf8");
extern float random_angle1688(void) __asm__("FUN_00213308");
extern void axis_rotate1688(void *,void *,void *,float) __asm__("FUN_00214890");
extern void FUN_L00_0026cbb0(void *,void *,int,int,int,int,float);
extern void FUN_L00_001ff290_c(void *,void *,void *) __asm__("FUN_L00_001ff290");
extern int collision1688(void *,void *,int,void *,void *) __asm__("FUN_001efa68");
extern void FUN_L00_001ff660(void *,void *,void *);
extern void FUN_L00_001ffd48_c(void *,void *,float) __asm__("FUN_L00_001ffd48");
extern void FUN_L00_001ffe20_c(void *,void *,void *) __asm__("FUN_L00_001ffe20");
extern void identity1688(void *) __asm__("FUN_001f9fc8");
extern void transform1688(void *,void *,void *) __asm__("FUN_001f9d20");
extern void FUN_L00_0026a9f0(void *,void *,int,int,float,int,int,int,int,float);
extern void FUN_L00_0026a4c8_c(void *,void *,int,int,float,int) __asm__("FUN_L00_0026a4c8");

void gold_devastator_missile_update(M910 *m) __asm__("FUN_L00_002b03a0");
void gold_devastator_missile_update(M910 *m) {
    B910 effect=D_L00_001E9950;
    B910 gold_effect=D_L00_001E99A0;
    V1688 camera_delta;
    V1688 previous;
    V1688 blast_velocity;
    V1688 normal;
    V1688 smoke_position;
    V1688 motion;
    V1688 surface_motion;
    V1688 hero_velocity;
    MissilePhase1688 phase;
    V1688 predicted;
    float basis[4][4];
    float basis_squared[4][4];
    float rotation[4][4];
    float diagonal[4][4];
    float rotation_part[4][4];
    M910 *skip=0;
    int reduced=0;
    Missile1688 *d;
    M910 *target;
    float size, yaw, pitch;
    normal.q=0;
    normal.f[2]=D_0015ED6C*8.0f;
    blast_velocity.q=normal.q;
    normal.q=0;
    normal.f[2]=1.0f;
    d=(Missile1688 *)m->v;
    target=d->target;
    if(D_L00_0015F5D0>0.9f || D_L00_0015F5D4>0.9f) reduced=1;
    d->speed=d->speed+(d->desired_speed-d->speed)/10.0f*frame_scale_sq;
    fast_dec_timer_s16(&d->turn_time);
    {
        float turn=convert_integer_to_float(d->turn_time);
        turn=turn/convert_integer_to_float(d->turn_period);
        add_vector_xyz(phase.collision.dir,&hero_motion1688.current,&hero_motion1688.previous);
        hero_velocity.q=phase.vectors.direction.q;
        scale_vector_xyz(&hero_velocity,&hero_velocity,turn);
    }
    size=convert_integer_to_float(D_0013E520.bB)+1.0f;
    if(target && d->life<scale_game_frames_alt(300)-scale_game_frames_alt(5) && (signed char)target->state>=0) {
        float time=0.0f;
        qcopy(&phase.vectors.target,&target->pos);
        phase.vectors.target.f[2]+=d->target_height;
        subtract_vector_xyz(phase.collision.dir,&phase.vectors.target,(&m->pos));
        if(d->track_velocity) {
            float b,a,root,intercept_time_plus_root,intercept_time_minus_root,denominator,negative_b;
            subtract_vector_xyz(&phase.vectors.relative,&phase.vectors.target,&d->previous_target);
            FUN_L00_001ff2c0_c(&predicted,&phase.vectors.relative,&hero_velocity);
            qcopy(&d->previous_target,&phase.vectors.target);
            a=FUN_L00_001ff430_c(&phase.vectors.relative);
            b=(phase.vectors.relative.f[0]*phase.collision.dir[0]+phase.vectors.relative.f[1]*phase.collision.dir[1]+phase.vectors.relative.f[2]*phase.collision.dir[2])*-2.0f;
            a=d->speed*d->speed-a;
            root=square_root_scalar(b*b-a*4.0f*-FUN_L00_001ff430_c(phase.collision.dir));
            negative_b=-b;
            denominator=a+a;
            intercept_time_plus_root=(negative_b+root)/denominator;
            intercept_time_minus_root=(negative_b-root)/denominator;
            if(intercept_time_plus_root>0.0f && intercept_time_minus_root>0.0f) {
                if(intercept_time_minus_root<intercept_time_plus_root) time=intercept_time_plus_root;
                else time=intercept_time_minus_root;
            } else {
                if(intercept_time_plus_root>0.0f) time=intercept_time_plus_root;
                else {
                    time=-1.0f;
                    if(intercept_time_minus_root>0.0f) time=intercept_time_minus_root;
                }
            }
        }
        if(time>0.0f && time<(float)scale_game_frames_alt(300)) {
                scale_vector_xyz(&predicted,&phase.vectors.relative,time);
                add_vector_xyz(&predicted,&predicted,phase.collision.dir);
                yaw=fast_arc_tan(predicted.f[0],predicted.f[1]);
                pitch=-fast_arc_tan(horizontal1688(&predicted),predicted.f[2]);
        } else {
            yaw=fast_arc_tan(phase.collision.dir[0],phase.collision.dir[1]);
            pitch=-fast_arc_tan(horizontal1688(phase.collision.dir),phase.collision.dir[2]);
        }
    } else {
        if(d->life<scale_game_frames_alt(300)-scale_game_frames_alt(5)) {
            yaw=d->yaw;
            pitch=d->pitch;
        } else {
            yaw=m->rz;
            pitch=m->ry;
        }
    }
    if(target) {
        m->rz=chase1688(&d->yaw_velocity,m->rz,yaw,D_0015ED70*6.2831855f,D_0015ED70*3.1415927f,D_0015ED6C*6.2831855f);
        m->ry=chase1688(&d->pitch_velocity,m->ry,pitch,D_0015ED70*6.2831855f,D_0015ED70*3.1415927f,D_0015ED6C*6.2831855f);
    } else {
        m->rz=chase1688(&d->yaw_velocity,m->rz,yaw,D_0015ED70*6.2831855f,D_0015ED70*3.1415927f,D_0015ED6C*4.71238899f);
        m->ry=chase1688(&d->pitch_velocity,m->ry,pitch,D_0015ED70*6.2831855f,D_0015ED70*3.1415927f,D_0015ED6C*4.71238899f);
    }
    build_spherical_offset_c(&motion,d->speed,m->rz,-m->ry);
    normalize_vector_xyz(&impact1688.normal,&motion,1.0f);
    scale_vector_xyz(&impact1688.normal,&impact1688.normal,-1.0f);
    if(!reduced || !(D_L00_0015F5CC&1)) {
        unsigned char *p;
        phase.vectors.relative.q=0;
        phase.vectors.relative.f[0]=random_float_between(-1.0f,1.0f);
        phase.vectors.relative.f[1]=random_float_between(-1.0f,1.0f);
        phase.vectors.relative.f[2]=random_float_between(-1.0f,1.0f);
        phase.vectors.direction.q=phase.vectors.relative.q;
        normalize_vector_xyz(phase.collision.dir,phase.collision.dir,random_float_between(0.1f,0.2f)*D_0015ED6C);
        normalize_vector_xyz(&smoke_position,&motion,-random_float_between(D_0015ED6C*0.1f,D_0015ED6C));
        add_vector_xyz(phase.collision.dir,phase.collision.dir,&smoke_position);
        normalize_vector_xyz(&smoke_position,&motion,random_float_between(0.0f,1.0f)*d->speed);
        add_vector_xyz(&smoke_position,&smoke_position,(&m->pos));
        p=smoke1688(&smoke_position,phase.collision.dir,scale_game_frames_alt(40),50,0x505050,3,40000.0f,1000.0f,1.0f,-0.0002f,0.0f);
        if(p) { unsigned char texture=*D_L00_001B20DC; p[3]=0x44; p[2]=texture; }
        smoke1688((&m->pos),phase.collision.dir,scale_game_frames_alt(7),127,0xB0B0B0,3,40000.0f,1000.0f,1.0f,-0.0004f,0.0f);
        phase.vectors.relative.q=0;
        phase.vectors.relative.f[2]=0.02f;
        rotate1688(&phase.vectors.relative,&phase.vectors.relative,m->mtx);
        axis_rotate1688(&phase.vectors.relative,&phase.vectors.relative,m->mtx,random_angle1688());
        {
            int color=FUN_L00_0025c0e8(0x4F007FFF,D_0013E520.bB);
            FUN_L00_0026cbb0(&smoke_position,&phase.vectors.relative,color,0x1FFFFFFF,scale_game_frames_alt(15),1,25000.0f);
        }
    }
    qcopy(&previous,(&m->pos));
    FUN_L00_001ff290_c(phase.collision.dir,&motion,&hero_velocity);
    add_vector_xyz((&m->pos),(&m->pos),&motion);
    if(m->pos.f[0]<0.0f || m->pos.f[1]<0.0f || m->pos.f[2]<0.0f) {
        mine_spawn_drops(m,D_L00_001B0770);
        mark_moby_for_removal(m);
        return;
    }
    phase.collision.moby=m;
    phase.collision.flags=0x830000;
    phase.collision.fC=3.0f;
    phase.collision.i10=1;
    qcopy(phase.collision.dir,&motion);
    set_vector_length_xy(phase.collision.dir,phase.collision.dir,1.0f);
    phase.collision.dir[2]=1.0f;
    phase.collision.dir[3]=5627.925f;
    phase.collision.b8=3;
    phase.collision.cls=m->cls;
    phase.collision.b9=1;
    if(collision1688(&previous,(&m->pos),0,D_0013F350.cam,&phase.collision)) {
        Impact1688 *hit=&impact1688;
        V1688 *hit_normal;
        if(hit->moby) {
            if(hit->moby!=(M910 *)D_0013F350.cam && hit->moby!=d->owner) {
                m->bBC=2;
                qcopy((&m->pos),&hit->position);
                subtract_vector_xyz(&surface_motion,&hit->velocity_end,&hit->velocity_start);
                hit_normal=&hit->normal;
                skip=m;
                normalize_vector_xyz(&surface_motion,&surface_motion,1.0f);
                FUN_L00_001ff660(&blast_velocity,&motion,hit_normal);
                normalize_vector_xyz(&blast_velocity,&blast_velocity,D_0015ED6C+D_0015ED6C);
                normalize_vector_xyz(&normal,hit_normal,1.0f);
            }
        } else if(hit->kind>0) {
            m->bBC=1;
            qcopy((&m->pos),&hit->position);
            subtract_vector_xyz(&surface_motion,&hit->velocity_end,&hit->velocity_start);
            hit_normal=&hit->normal;
            normalize_vector_xyz(&surface_motion,&surface_motion,1.0f);
            FUN_L00_001ff660(&blast_velocity,&motion,hit_normal);
            normalize_vector_xyz(&blast_velocity,&blast_velocity,D_0015ED6C+D_0015ED6C);
            normalize_vector_xyz(&normal,hit_normal,1.0f);
        }
    } else if(tick_countdown_32_alt(&d->life) || d->limit<vector_distance((&m->pos),&D_0013F350.x80)) {
        qcopy(&normal,&D_L00_00160680);
        qcopy(&surface_motion,&D_L00_00160660);
        rotate1688(&surface_motion,&surface_motion,m->mtx);
        rotate1688(&normal,&normal,m->mtx);
        if(!D_0013E520.bB) {
            mine_spawn_drops(m,D_L00_001B0770);
            mark_moby_for_removal(m);
            return;
        }
        m->bBC=1;
        add_vector_xyz(&blast_velocity,&blast_velocity,&motion);
    }
    if(m->bBC) {
        float distance, near_adjust;
        int particles;
        int particle_reduction;
        int i,j;
        int hit=collect_mobies_in_sphere((&m->pos),16,m,0,(size+size));
        particles=10;
        predicted.q=m->pos.q;
        FUN_L00_0025a9f8(m,&predicted,D_L00_00177F00,hit,(int)skip,0x830000,3,1,3.0f,1.0f,1.0f);
        particle_reduction=0;
        basis[0][0]=0.0f;
        basis[1][0]=-normal.f[2];
        basis[2][0]=normal.f[1];
        basis[3][0]=0.0f;
        basis[0][1]=normal.f[2];
        basis[1][1]=0.0f;
        basis[2][1]=-normal.f[0];
        basis[3][1]=0.0f;
        basis[0][2]=-normal.f[1];
        basis[1][2]=normal.f[0];
        basis[2][2]=0.0f;
        basis[3][2]=0.0f;
        basis[0][3]=0.0f;
        basis[1][3]=0.0f;
        basis[2][3]=0.0f;
        basis[3][3]=0.0f;
        multiply_matrix(basis_squared,basis,basis);
        if(reduced) { particle_reduction=1; particles/=3; }
        { int remaining;
        for(remaining=0;remaining<particles;remaining++) {
            if(m->bBC==1) {
                float a=random_angle1688();
                FUN_L00_001ffd48_c(rotation_part,basis_squared,1.0f-fast_cos(a));
                FUN_L00_001ffd48_c(diagonal,basis,fast_sin(a));
                FUN_L00_001ffe20_c(diagonal,diagonal,rotation_part);
                identity1688(rotation_part);
                FUN_L00_001ffe20_c(rotation,diagonal,rotation_part);
                transform1688(&predicted,&surface_motion,rotation);
                normalize_vector_xyz(&predicted,&predicted,random_float_between(8.5f,16.5f)*D_0015ED6C*size);
            } else {
                float speed=random_float_between(8.5f,16.5f)*D_0015ED6C*size;
                float a=random_angle1688();
                build_spherical_offset_c(&predicted,speed,a,random_angle1688());
            }
            predicted.f[2]+=D_0015ED6C*5.0f;
            add_vector_xyz(&predicted,&predicted,&blast_velocity);
            {
                int color=FUN_L00_0025c0e8(0x4F007FFF,D_0013E520.bB);
                int fade=FUN_L00_0025c0e8(0x1F00007F,D_0013E520.bB);
                int life;
                float particle_scale;
                if(vector_length_xyz(&predicted)>1.0f) normalize_vector_xyz(&predicted,&predicted,D_0015ED6C*8.0f);
                particle_scale=size*40000.0f;
                life=FUN_L00_00257b90(scale_game_frames_alt(60),scale_game_frames_alt(120))-particle_reduction*35;
                FUN_L00_0026bb70((&m->pos),&predicted,color,fade,particle_scale,life,1,-1,-1);
            }
        }
        }
        { V1688 random;
            random.q=0;
            random.f[0]=random_float_between(-1.0f,1.0f);
            random.f[1]=random_float_between(-1.0f,1.0f);
            random.f[2]=random_float_between(-1.0f,1.0f);
            predicted.q=random.q;
        }
        subtract_vector_xyz(&camera_delta,D_L00_00166DC0,(&m->pos));
        distance=vector_length_xyz(&camera_delta);
        camera_delta.f[2]+=distance*0.5f;
        normalize_vector_xyz(&predicted,&predicted,distance/5.0f*D_0015ED6C*size);
        normalize_vector_xyz(&camera_delta,&camera_delta,(distance+distance)*D_0015ED6C);
        add_vector_xyz(&predicted,&predicted,&camera_delta);
        clamp_vector_length(&predicted,&predicted,D_0015ED6C*10.0f);
        FUN_L00_002af450((char *)(&m->pos),(char *)&predicted,FUN_L00_00257b90(scale_game_frames_alt(60),scale_game_frames_alt(90)),0,0);
        if(distance<6.0f) j=truncate_float_to_s32(distance)/2;
        else j=3;
        near_adjust=0.0f;
        if(distance<7.0f) near_adjust=7.0f-distance;
        if(reduced) j/=2;
        for(i=0;i<j;i++) {
            float speed=random_float_between(8.0f,10.0f)*size*D_0015ED6C-near_adjust*D_0015ED6C;
            Colors1688 color=D_L00_001E99F0;
            Colors1688 fade=D_L00_001E9A08;
            int c=FUN_L00_0025c0e8(color.colors[random_integer_below(6)],D_0013E520.bB);
            int f=FUN_L00_0025c0e8(fade.colors[random_integer_below(6)],D_0013E520.bB);
            int life=FUN_L00_00257b90(scale_game_frames_alt(15),scale_game_frames_alt(20))-particle_reduction*7;
            int end=FUN_L00_00257b90(scale_game_frames_alt(25),scale_game_frames_alt(30))-particle_reduction*10;
            int white_life, white_end;
            FUN_L00_0026a9f0((&m->pos),&blast_velocity,c,f,400000.0f,life,end,0,0,speed);
            white_life=FUN_L00_00257b90(scale_game_frames_alt(5),scale_game_frames_alt(10))-particle_reduction*5;
            white_end=FUN_L00_00257b90(scale_game_frames_alt(15),scale_game_frames_alt(20))-particle_reduction*7;
            FUN_L00_0026a9f0((&m->pos),&blast_velocity,0x7FFFFFFF,0xFFFFFF,400000.0f,white_life,white_end,0,0,speed*0.5f);
        }
        for(i=0;i<particles;i++) {
            Colors1688 color=D_L00_001E99F0;
            Colors1688 fade=D_L00_001E9A08;
            V1688 v;
            int c,f,life;
            float debris_scale;
            { V1688 random;
            random.q=0;
            random.f[0]=random_float_between(-1.0f,1.0f);
            random.f[1]=random_float_between(-1.0f,1.0f);
            random.f[2]=random_float_between(-1.0f,1.0f);
                v.q=random.q;
            }
            normalize_vector_xyz(&v,&v,random_float_between(0.0f,3.0f)*D_0015ED6C*size);
            c=FUN_L00_0025c0e8(color.colors[random_integer_below(6)],D_0013E520.bB);
            f=FUN_L00_0025c0e8(fade.colors[random_integer_below(6)],D_0013E520.bB);
            debris_scale=size*200000.0f;
            life=FUN_L00_00257b90(scale_game_frames_alt(20),scale_game_frames_alt(35))-particle_reduction*10;
            FUN_L00_0026a4c8_c((&m->pos),&v,c,f,debris_scale,life);
        }
        if(D_L00_0015F5D0<0.95f && distance>9.0f) {
            int c=127,f=127;
            if(D_0013E520.bB) { c=60; f=60; }
            spawn_glow_particle_u8(m,(&m->pos),&blast_velocity,size*4.0f,scale_game_frames_alt(15),c,127,f,32);
            spawn_glow_particle_u8(m,(&m->pos),&blast_velocity,size*4.0f,scale_game_frames_alt(24),c,127,f,32);
        }
        { unsigned char color=127;
          int life;
          float flash_scale=size*4.0f;
          if(D_0013E520.bB) color=32;
          life=scale_game_frames_alt(20);
          spawn_glow_particle_u8(m,(&m->pos),&blast_velocity,flash_scale,life,color,127,0,48);
        }
        if(!reduced) spawn_glow_particle_u8(m,(&m->pos),&blast_velocity,(size+size),scale_game_frames_alt(19),255,255,255,32);
        motion.f[0]=0.0f;
        m->bBC=0;
        D_L00_00166C80.f160=distance<20.0f ? 0.4f-distance*0.0175f : 0.4f-20.0f*0.0175f;
        D_L00_00166C80.i168=scale_game_frames_alt(25);
        allocate_voice_for_target_entry_alt(0,0,(int)m);
        if(!reduced) {
            if(D_0013E520.bB) mine_camera_blast((char *)&gold_effect,(char *)(&m->pos),0,0);
            else mine_camera_blast((char *)&effect,(char *)(&m->pos),0,0);
        }
        d->bounce_count++;
        if(!D_0013E520.bB || d->bounce_count==3) {
            mine_spawn_drops(m,D_L00_001B0770);
            mark_moby_for_removal(m);
            return;
        }
        { V1688 bounce[2];
        d->target=0;
        m->bBC=0;
        normalize_vector_xyz(&impact1688.normal,&impact1688.normal,1.0f);
        FUN_L00_001ff660(&bounce[0],&motion,&impact1688.normal);
        m->rz=fast_arc_tan(bounce[0].f[0],bounce[0].f[1]);
        m->ry=-fast_arc_tan(horizontal1688(&bounce[0]),bounce[0].f[2]);
        d->yaw_velocity=0.0f; d->pitch_velocity=0.0f;
        normalize_vector_xyz(&bounce[1],&impact1688.normal,0.05f);
        add_vector_xyz((&m->pos),(&m->pos),&bounce[1]);
        if(impact1688.moby) d->owner=impact1688.moby;
        if(!d->bounce_timer) d->bounce_timer=scale_game_frames_alt(90);
        FUN_L00_002affa8((char *)m,(O *)d,(char *)impact1688.moby);
        allocate_voice_for_target_entry_alt(1,0,(int)m);
            }
    }
    if(tick_countdown_32_alt(&d->bounce_timer)) d->bounce_count=0;
}
typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2b1af0;
typedef struct {
    V_2b1af0 a;
    char pad[0x30];
    V_2b1af0 b;
} P_2b1af0;
void fast_subtract_rotations_cf(void *) __asm__("FUN_L00_001fefc8");
void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
void add_2b1af0(void *, void *, void *) __asm__("FUN_001f9a10");
void scale_2b1af0(void *, void *, float) __asm__("FUN_001f9a68");
float rndf_2b1af0(float, float) __asm__("FUN_002132a8");
void ff500_2b1af0(void *, void *, float) __asm__("FUN_001f9bf8");
float rnd_2b1af0(float, float) __asm__("FUN_L00_00257c48");
int trunc_2b1af0(float) __asm__("FUN_001fa6d0");
int fr_2b1af0(int) __asm__("FUN_001f96f8");
int rndi_2b1af0(int, int) __asm__("FUN_L00_00257b90");
void spawn_particle(void *, void *, int, int, int, int, float, float, float, float,
                  float) __asm__("FUN_L00_002712b8");
void spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern P_2b1af0 D_0013F450;
void FUN_L00_002b1af0(unsigned char *o) {
    V_2b1af0 b;
    V_2b1af0 a;
    V_2b1af0 c;
    V_2b1af0 d;
    int i, j;
    int t;
    if (o[0xBC] == 0)
        return;
    *(OvlQuad *)&a = 0;
    *(OvlQuad *)&b = 0;
    a.f[0] = 0.23f;
    a.f[1] = 0.18f;
    b.f[1] = 1.0f;
    b.f[3] = 1.0f;
    a.f[3] = 1.0f;
    fast_subtract_rotations_cf(o + 0xBC);
    transform_vector_by_basis(&a, &a, o + 0xC0);
    add_2b1af0(&c, o + 0x10, &a);
    transform_vector_by_basis(&b, &b, o + 0xC0);
    for (i = 1; i >= 0; i--) {
        ff500_2b1af0(&d, &b, rndf_2b1af0(0.01f, 0.06f));
        scale_2b1af0(&b, &D_0013F450.a, 0.7f);
        b.f[2] = 0.0f;
        add_2b1af0(&d, &d, &b);
        scale_2b1af0(&b, &D_0013F450.b, 0.7f);
        b.f[2] = 0.0f;
        add_2b1af0(&d, &d, &b);
        t = trunc_2b1af0(rnd_2b1af0(0.0f, 6.0f));
        spawn_particle(&c, &d, fr_2b1af0(0x1E), 0x7F, 0xFFFFFF, t, 25000.0f, 300.0f, 0.98f, 0.0f,
                     0.85f);
    }
    for (j = 0; j >= 0; j--) {
        ff500_2b1af0(&d, &b, rndf_2b1af0(0.01f, 0.06f));
        scale_2b1af0(&b, &D_0013F450.a, 0.55f);
        b.f[2] = 0.0f;
        add_2b1af0(&d, &d, &b);
        scale_2b1af0(&b, &D_0013F450.b, 0.55f);
        b.f[2] = 0.0f;
        add_2b1af0(&d, &d, &b);
        t = fr_2b1af0(0x14);
        spawn_burst(&c, &d, 0x4F007FFF, 0x1FFFFFFF, rndi_2b1af0(t, fr_2b1af0(0x28)), 1, 10000.0f);
    }
}
void build_look_at_matrix(void *, void *, void *, float) __asm__("FUN_00214890");
int count_2b1de0(float) __asm__("FUN_001fa6d0");
int random_bit_2b1de0(int) __asm__("FUN_00213260");

extern V_2b1af0 D_0013F9B0;
void FUN_L00_002b1de0(unsigned char *o) {
    V_2b1af0 c;
    V_2b1af0 a;
    V_2b1af0 b;
    V_2b1af0 d;
    int i;
    int j;
    int t;
    float angle;

    *(OvlQuad *)&a = 0;
    a.f[0] = 0.3f;
    a.f[1] = -1.2f;
    a.f[3] = 1.0f;
    transform_vector_by_basis(&a, &a, o + 0xC0);
    add_2b1af0(&b, o + 0x10, &a);
    for (i = 0; i < count_2b1de0(24.0f); i++) {
        angle = rnd_2b1af0(0.0f, 0.1308997f);
        build_look_at_matrix(&c, &D_0013F9B0, (char *)&D_0013F9B0 - 0x20,
                         (float)i * 0.2617994f + angle);
        ff500_2b1af0(&c, &c, rndf_2b1af0(0.05f, 0.1f));
        t = rndi_2b1af0(0, 3);
        if (random_bit_2b1de0(2) != 0)
            t = -t;
        if (*(int *)((char *)&D_0013F9B0 - 0x360) != 0) {
            scale_2b1af0(&d, (char *)&D_0013F9B0 - 0x560, 1.7f);
            add_2b1af0(&c, &c, &d);
        }
        {
            int low = fr_2b1af0(30);
            int high = fr_2b1af0(90);
            spawn_particle(&b, &c, rndi_2b1af0(low, high), 30, 0xFFFFFF, t,
                         60000.0f, 3000.0f, 0.85f, -0.001f, 0.0f);
        }
    }
    for (j = 0; j < count_2b1de0(6.0f); j++) {
        angle = rndf_2b1af0(0.0f, 0.7853982f);
        build_look_at_matrix(&c, &D_0013F9B0, (char *)&D_0013F9B0 - 0x20,
                         (float)j * 1.0471976f + angle);
        ff500_2b1af0(&c, &c, rndf_2b1af0(0.05f, 0.1f));
        {
            int low = fr_2b1af0(20);
            int high = fr_2b1af0(60);
            spawn_burst(&b, &c, 0x4F007FFF, 0x1FFFFFFF,
                         rndi_2b1af0(low, high), 1, 10000.0f);
        }
    }
    o[0xBC] = fr_2b1af0(5);
}
