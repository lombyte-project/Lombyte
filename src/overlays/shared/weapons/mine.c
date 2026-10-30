/* Shared moby classes: mine, nanotech_crate_fragment, gold_devastator_missile. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/weapons/suck_aim.h"
#include "rnc/overlay/shared/vehicles/veldin_ship.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"

/* Mine (moby class 74) update: falls and bounces, sticks to what it hits, homes on a nearby target and explodes. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002AB910.c: func_L00_002AB910), where it is exact; names translated to the US level program. */

typedef float V910[4] __attribute__((aligned(16)));

typedef union {
    OvlQuad q;
    float   f[4];
} U910;

typedef struct {
    OvlQuad q[5];
} B910;

typedef struct M910 M910;

typedef struct W910 W910;

struct W910 {
    U910  vel;
    char  p10[0x20];
    M910 *owner;
    short h34;
    short h36;
    char  p38[8];
    float f40;
    char  p44[4];
    int   i48;
    M910 *p4C;
    short h50;
    short h52;
    M910 *p54;
    int   i58;
    int   i5C;
    float f60;
    float f64;
    int   i68;
    float f6C;
    float f70;
    int   i74;
    int   i78;
};

struct M910 {
    char           p0[0x10];
    U910           pos;
    unsigned char  state;
    char           p21[3];
    char          *p24;
    char           p28[0xC];
    unsigned short flags;
    char           p36[0xA];
    float          rx;
    float          ry;
    float          rz;
    char           p4C[6];
    unsigned char  b52;
    unsigned char  anim;
    char           p54[0x1C];
    unsigned char  b70;
    char           p71[7];
    W910          *v;
    char           p7C[0x18];
    int            i94;
    char           p98[0xC];
    unsigned char  bA4;
    char           pA5;
    short          cls;
    char           pA8[0x14];
    unsigned char  bBC;
    char           pBD[3];
    float          mtx[4][4];
};

typedef struct {
    float          dir[4];
    M910          *moby;
    int            flags;
    unsigned char  b8;
    unsigned char  b9;
    unsigned short cls;
    float          fC;
    int            i10;
} __attribute__((aligned(16))) Qry910;

typedef struct {
    char           p0[0x80];
    float          pos[4];
    char           p90[0x2FC - 0x90];
    unsigned char *p2FC;
    char           p300[0x1090 - 0x300];
    M910          *hero;
    char           p1094[0x2080 - 0x1094];
    M910          *cam;
    char           p2084[0x20];
    unsigned char  b20A4;
    unsigned char  b20A5;
    char           p20A6[9];
    unsigned char  b20AF;
} G910;

typedef struct {
    char          p0[0x11];
    unsigned char b11;
} E910;

typedef struct {
    char  p0[0x18];
    M910 *moby;
    int   i1C;
    U910  pos;
    char  p30[0x10];
    U910  nrm;
} H910;

typedef struct {
    char  p0[0x160];
    float f160;
    char  p164[4];
    int   i168;
} Cam910;

typedef struct {
    char  p0[0x80];
    f32   x80;
    f32   x84;
    char  p1[0x2E4 - 0x88];
    f32   f2E4;
    char  p2[0x2080 - 0x2E8];
    char *cam;
} P;

typedef struct {
    char p0[0xA];
    u8   bA;
    u8   bB;
} E;

extern B910 l00_mine_camera_blast_parameters __asm__("D_L00_001E9830");
extern B910 l00_mine_alternate_camera_blast_parameters __asm__("D_L00_001E9880");

#include "rnc/overlay/level_camera.h"

extern E D_0013E520;

#include "rnc/overlay/collision.h"

extern char *active_mobies[] __asm__("D_L00_001ABA00");
extern char *mine_debris(char *, char *, int, int, float, int, int) __asm__("FUN_L00_0026bed0");
extern s16   D_L00_001B07B0[];
extern float scale_time(float) __asm__("FUN_001f96b0");
extern float step_rotation_toward_target(float, float, float) __asm__("FUN_L00_00200510");
extern float FUN_L00_00259710(void *);
extern float fast_vec_dot(void *a, void *b) __asm__("FUN_001f9ab0");
extern float fast_vec_length(void *a) __asm__("FUN_001f9af0");
extern int  *D_L00_00177F00[];
extern int   collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int   collision_hit_material(void) __asm__("FUN_001f0b58");
extern int   FUN_L00_001f0d60(float, void *, int, void *);
extern int collect_mobies_in_sphere(void *, int, void *, void *, float) __asm__("FUN_L00_001f2868");
extern int mine_blend_color(int, int, float) __asm__("FUN_L00_002371e0");
extern int FUN_L00_00257b90(int, int);
extern int FUN_L00_00258490(void *, void *, void *, float, float, float, float, int);
extern int FUN_L00_00260400(void *, void *, void *, void *, void *, void *);
extern int random_integer_below_cf(void *, void *, float, float) __asm__("FUN_L00_0026f080");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int predict_mine_impact(W910 *, M910 *, int) __asm__("FUN_L00_002aa2a8");
extern s32 D_L00_00166DC0[];
extern s32 D_L00_00173E40[0x1C];
extern s32 is_moby_in_class_range_500_to_540(s32) __asm__("FUN_L00_0025e3b8");
extern s32 D_L00_001612D0_gp __asm__("D_L00_001612D0") __attribute__((sda));
extern s32 D_L00_001612D8_gp __asm__("D_L00_001612D8") __attribute__((sda));
extern short          mine_force_burst __asm__("D_L00_001613D0") __attribute__((sda));
extern short          mine_bounce_ramp_duration __asm__("D_L00_001613D4") __attribute__((sda));
extern unsigned       swap_packed_color_channels(unsigned c, int mask) __asm__("FUN_L00_0025c0e8");
extern unsigned char *D_L00_001B2080[];
extern unsigned char *FUN_002141f8_cf(unsigned char *) __asm__("FUN_L00_002603d0");
extern int            D_L00_0015F5CC;
extern void          *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void          *spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
             void *, void *, int, int, float, int, int, int, int) __asm__("FUN_L00_0026bb70");
extern void reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void make_hit_descriptor(void *, void *, int, void *, float) __asm__("FUN_L00_00259888");
extern void jet_animate(unsigned char *, float) __asm__("FUN_L00_00259fe8");
extern void
send_blast_hits(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__("FUN_L00_0025a9f8");
extern void spawn_explosion(void *,
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
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void mine_spawn_drops(void *, void *) __asm__("FUN_L00_0025f800");
extern void update_moby_rotation_oscillation(void *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");
extern void spawn_debris_chunk(void *, void *, int, int) __asm__("FUN_L00_0026f548");
extern void spawn_gravity_floor_burst_particle(
    void *, void *, int, float, float, float, int, int, int) __asm__("FUN_L00_002738e8");
extern void pool_splash(int, int, int, float, float, float, float) __asm__("FUN_L00_002a3ec8");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void build_moby_bone_transform(int, int, void *) __asm__("func_0020CCA8");
extern void copy_matrix3x4(void *, void *);
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_sub(void *dst, void *a, void *b) __asm__("FUN_001f9a28");
f32   resolve_camera_surface_height(void *position, s32 optional_output) __asm__("FUN_002135f0");
float absolute_float(float input) __asm__("func_001F99C0");
extern E910 D_E910 __asm__("D_0013E520");
extern H910 coll_hit __asm__("D_L00_00173E40");

void mine_update(M910 *m) __asm__("FUN_L00_002aa670");

void mine_update(M910 *mine_moby) {
    enum {
        MINE_DEBRIS_FIXED_SECONDARY_COLOR = 0,
        MINE_DEBRIS_FIXED_LIFETIME = 1,
        MINE_DEBRIS_SHORT_LIVED = 2,
        MINE_DEBRIS_SPHERICAL_VELOCITY = 3
    };
    B910  camera_blast_parameters = l00_mine_camera_blast_parameters;
    B910  alternate_camera_blast_parameters = l00_mine_alternate_camera_blast_parameters;
    U910  collision_response_velocity;
    U910  old;
    U910  up;
    W910 *v;
    M910 *hd;
    int   mine_state_snapshot;

    hd = 0;
    old.q = 0;
    up.q = 0;
    old.f[2] = frame_time + frame_time;
    up.f[2] = 1.0f;
    collision_response_velocity.q = old.q;
    v = mine_moby->v;
    if ((hero.unk20A5 != 0 || hero.unk20AF != 0) && mine_moby->state == 0) {
        mine_moby->flags |= 0x41;
    } else {
        mine_moby->flags &= 0xFFBE;
    }
    if ((hero.items[0].moby == 0 || ((M910 *)hero.items[0].moby)->cls != 0xBE) &&
        mine_moby->state == 0) {
        mark_moby_for_removal(mine_moby);
        return;
    }
    if (hero.vehicle_mode != 0 && mine_moby->state == 0) {
        mark_moby_for_removal(mine_moby);
        return;
    }
    if (v->owner != 0 && v->owner->state != 0xFE && v->owner->state != 0xFD &&
        v->owner->cls == 0xBE && v->h36 == 0 && 0.0f < fast_vec_length(v)) {
        if (hero.ground_moby == 0 || FUN_002141f8_cf(hero.ground_moby) == 0) {
            predict_mine_impact(v, mine_moby, 1);
        } else if (predict_mine_impact(v, mine_moby, 0) == 0) {
            predict_mine_impact(v, mine_moby, 2);
        }
    }
    mine_state_snapshot = mine_moby->state;
    if (mine_state_snapshot == 0) {
        if (hero.items[0].moby != 0 && ((M910 *)hero.items[0].moby)->cls == 0xBE) {
            float mine_attachment_transform[4][4];
            float owner_bone_transform[4][4];
            U910  attachment_rotation_angles;
            {
                attachment_rotation_angles.q = 0;
                attachment_rotation_angles.f[0] = 3.1415927f;
                euler_rotation_matrix(mine_attachment_transform, &attachment_rotation_angles);
                build_moby_bone_transform((char *)v->owner, 0, (char *)owner_bone_transform);
                multiply_matrix(
                    mine_attachment_transform, owner_bone_transform, mine_attachment_transform);
                copy_matrix3x4(mine_moby->mtx, mine_attachment_transform);
                mine_moby->i94 = 0;
                mine_moby->flags |= 0x100;
            }
            return;
        }
        mark_moby_for_removal(mine_moby);
        return;
    }
    {
        Qry910 q;
        {
            float k;
            float explosion_camera_distance;
            float gz;

            mine_moby->flags &= 0xFEFF;
            qcopy(&old, &mine_moby->pos);
            if (!(mine_state_snapshot & 4)) {
                fast_vec_add(&mine_moby->pos, &mine_moby->pos, v);
                if (v->h50 == 0) {
                    v->vel.f[2] -= frame_time_sq * 9.8f;
                } else {
                    v->vel.f[2] -= frame_time_sq * 9.8f / 10.0f;
                    if (random_integer_below(scale_game_frames(6) - 1) == 0) {
                        float wz = resolve_camera_surface_height((float *)&mine_moby->pos, &q);

                        scale_vector_xyz(&q, v, 0.25f);
                        random_integer_below_cf(
                            &mine_moby->pos, &q, random_float_between(0.05f, 0.1f) * 210000.0f, wz);
                    }
                }
            }
            q.flags = 0x830000;
            q.fC = 3.0f;
            q.moby = mine_moby;
            q.i10 = 1;
            qcopy(&q, v);
            set_vector_length_xy(&q, &q, 1.0f);
            q.b8 = 2;
            q.dir[3] = 5627.925f;
            q.b9 = 1;
            q.cls = mine_moby->cls;
            mine_moby->pos.f[2] -= 0.4f;
            q.dir[2] = 1.0f;
            k = convert_integer_to_float(D_L00_0015F5CC - v->i74);
            k = k / scale_time(*(float *)&mine_bounce_ramp_duration);
            if (1.0f < k) {
                k = 1.0f;
            } else if (k < 0.0f) {
                k = 0.0f;
            }
            mine_moby->pos.f[2] -= v->f6C * fast_sin(v->f70) * k;
            if (!(mine_moby->state & 4) &&
                collision_line(&old, &mine_moby->pos, 0x10, (int)mine_moby, (int)&q) != 0) {
                if (v->h50 == 0 && collision_hit_material() == 0 && v->vel.f[2] < 0.0f) {
                    V910 t;
                    V910 n;
                    {
                        int   i;
                        char *p;

                        pool_splash(*(int *)&D_L00_001612D0_gp,
                                    *(int *)&D_L00_001612D8_gp,
                                    1,
                                    mine_moby->pos.f[0],
                                    mine_moby->pos.f[1],
                                    0.5f,
                                    -0.35f);
                        v->h50 = 1;
                        clear_u64_value((float *)v);
                        v->vel.f[2] = frame_time * -1.5f;
                        t[0] = mine_moby->pos.f[0];
                        t[1] = mine_moby->pos.f[1];
                        t[2] = coll_hit.pos.f[2];
                        p = spawn_burst_moby(t, 2.0f);
                        if (p != 0) {
                            p[0x23] = 0x70;
                        }
                        for (i = 0; i < 16; i++) {
                            float ang = random_angle_radians();
                            float rr = random_float_between(frame_time * 0.0f, frame_time * 3.0f);
                            int   life;

                            n[0] = fast_cos(ang) * rr;
                            n[1] = fast_sin(ang) * rr;
                            n[2] = random_float_between(frame_time * 3.0f, frame_time * 6.5f);
                            life = FUN_L00_00257b90(0x5A, 0x78);
                            spawn_debris_chunk(t, n, random_integer_below(2), life);
                        }
                    }
                } else if (v->h50 != 0 || collision_hit_material() != 0) {
                    V910 t;
                    V910 n;
                    V910 r;
                    V910 ov;
                    {
                        if (coll_hit.moby != 0 &&
                            (is_moby_in_class_range_500_to_540((int)coll_hit.moby) != 0 ||
                             (coll_hit.moby->flags & 0x5000)) &&
                            coll_hit.moby->cls != mine_moby->cls && coll_hit.moby != hero.moby &&
                            coll_hit.moby != v->owner) {
                            mine_moby->bBC = 1;
                            if (mine_moby->state != 1) {
                                qcopy(&mine_moby->pos, &coll_hit.pos);
                                mine_moby->pos.f[2] = probe_ground_height(&mine_moby->pos, 0, 0.5f);
                            }
                        } else if (coll_hit.moby != 0 && coll_hit.moby->cls == mine_moby->cls) {
                            fast_vec_sub(&coll_hit.nrm, &mine_moby->pos, &coll_hit.moby->pos);
                            if (v->vel.f[2] < frame_time * -9.8f) {
                                mine_moby->bBC = 1;
                                qcopy(&mine_moby->pos, &coll_hit.pos);
                                reflect_vector(&collision_response_velocity, v, &coll_hit.nrm);
                                fast_vec_normalize(&collision_response_velocity,
                                                   &collision_response_velocity,
                                                   frame_time + frame_time);
                                fast_vec_normalize(&up, &coll_hit.nrm, 1.0f);
                            } else {
                                qcopy(&mine_moby->pos, &coll_hit.pos);
                                fast_vec_normalize(t, &coll_hit.nrm, frame_time_sq * 9.7f);
                                fast_vec_add(&mine_moby->pos, &mine_moby->pos, t);
                                reflect_vector(v, v, &coll_hit.nrm);
                                scale_vector_xyz(v, v, 0.4f);
                                fast_vec_normalize(t, &coll_hit.nrm, 1.0f);
                                fast_vec_dot(t, &up);
                                qcopy(n, &coll_hit.nrm);
                                goto orient;
                            }
                        } else if ((coll_hit.moby != 0 && coll_hit.moby != hero.moby &&
                                    coll_hit.moby != v->owner) ||
                                   coll_hit.i1C > 0) {
                            if (v->h50 != 0 || collision_hit_material() == 0 ||
                                v->vel.f[2] < frame_time * -9.8f) {
                                mine_moby->bBC = 1;
                                qcopy(&mine_moby->pos, &coll_hit.pos);
                                reflect_vector(&collision_response_velocity, v, &coll_hit.nrm);
                                fast_vec_normalize(&collision_response_velocity,
                                                   &collision_response_velocity,
                                                   frame_time + frame_time);
                                fast_vec_normalize(&up, &coll_hit.nrm, 1.0f);
                            } else {
                                if (coll_hit.moby != 0) {
                                    hd = (M910 *)FUN_002141f8_cf((unsigned char *)coll_hit.moby);
                                }
                                qcopy(&mine_moby->pos, &coll_hit.pos);
                                fast_vec_normalize(t, &coll_hit.nrm, frame_time_sq * 9.7f);
                                fast_vec_add(&mine_moby->pos, &mine_moby->pos, t);
                                *(OvlQuad *)ov = v->vel.q;
                                reflect_vector(v, v, &coll_hit.nrm);
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
                                fast_vec_normalize(t, &coll_hit.nrm, 1.0f);
                                if (0.707f < fast_vec_dot(t, &up) && ov[2] < 0.0f) {
                                    float fz = probe_ground_height(&mine_moby->pos, 0, 0.5f);

                                    if (mine_moby->pos.f[2] < fz) {
                                        mine_moby->pos.f[2] = fz;
                                    }
                                    v->vel.f[2] = -v->vel.f[2];
                                    if (fast_vec_length(v) < frame_time * 0.5f || hd != 0) {
                                        clear_u64_value((float *)v);
                                        mine_moby->state |= 4;
                                        mine_moby->i94 = *(int *)(mine_moby->p24 + 0x10);
                                        if (coll_hit.moby != 0 && hd != 0) {
                                            v->p4C = coll_hit.moby;
                                        }
                                    }
                                }
                                qcopy(n, &coll_hit.nrm);
                            orient: {
                                r[0] =
                                    n[0] * fast_cos(mine_moby->rz) + n[1] * fast_sin(mine_moby->rz);
                                r[1] =
                                    n[1] * fast_cos(mine_moby->rz) - n[0] * fast_sin(mine_moby->rz);
                                r[2] = n[2];
                                mine_moby->rx = step_rotation_toward_target(
                                    mine_moby->rx,
                                    -fast_arc_tan(square_root(r[0] * r[0] + r[2] * r[2]), r[1]),
                                    0.05235988f);
                                mine_moby->ry = step_rotation_toward_target(
                                    mine_moby->ry, fast_arc_tan(r[2], r[0]), 0.05235988f);
                            }
                            }
                        }
                    }
                }
            } else if (mine_moby->state & 4) {
                float fz = probe_ground_height(&mine_moby->pos, 0, 0.5f);

                if (v->p4C != 0) {
                    if (!(v->p4C != 0 && v->p4C->state != 0xFE && v->p4C->state != 0xFD)) {
                        mine_moby->bBC = 1;
                        v->p4C = 0;
                    } else if (coll_hit_moby != 0) {
                        hd = (M910 *)FUN_002141f8_cf((unsigned char *)coll_hit_moby);
                        if (hd != 0) {
                            FUN_L00_00260400(mine_moby,
                                             v->p4C,
                                             &mine_moby->pos,
                                             &mine_moby->rx,
                                             &mine_moby->pos,
                                             &mine_moby->rx);
                        }
                    }
                    if (hd == 0) {
                        int s;

                        mine_moby->state = 1;
                        s = v->i68;
                        if (s != -1) {
                            char *tab = ((char *)&voice_pool);
                            char *e = tab + s * 0x70;

                            if (*(M910 **)(e + 0x88) == mine_moby &&
                                *(unsigned char *)(e + 0x74) != 0) {
                                release_voice_slot(s);
                            }
                        }
                        v->vel.f[2] = 0.0f;
                        v->i68 = -1;
                        v->h34 = scale_game_frames(0x78);
                        v->p4C = 0;
                    }
                } else if (!(mine_moby->state & 0x10)) {
                    if (mine_moby->pos.f[2] - fz < frame_time_sq * 40.0f) {
                        mine_moby->pos.f[2] = fz;
                        v->h34 = 0;
                        if (coll_hit.moby != 0 &&
                            FUN_002141f8_cf((unsigned char *)coll_hit.moby) != 0) {
                            v->p4C = coll_hit.moby;
                        }
                    } else {
                        int s;

                        mine_moby->state = 1;
                        s = v->i68;
                        if (s != -1) {
                            char *tab = ((char *)&voice_pool);
                            char *e = tab + s * 0x70;

                            if (*(M910 **)(e + 0x88) == mine_moby &&
                                *(unsigned char *)(e + 0x74) != 0) {
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
                    (mine_moby->state & 0x10)) {
                    int s;

                    mine_moby->state = 1;
                    s = v->i68;
                    if (s != -1) {
                        char *tab = ((char *)&voice_pool);
                        char *e = tab + s * 0x70;

                        if (*(M910 **)(e + 0x88) == mine_moby &&
                            *(unsigned char *)(e + 0x74) != 0) {
                            release_voice_slot(s);
                        }
                    }
                    v->vel.f[2] = 0.0f;
                    v->p54 = 0;
                    v->i68 = -1;
                    v->h34 = scale_game_frames(0x78);
                    if (mine_moby->anim != 0) {
                        blend_moby_animation(mine_moby, 0, 0, 10);
                    }
                }
                if (v->p54 != 0 && (mine_moby->state & 0x10)) {
                    V910   t;
                    V910   n;
                    Qry910 homing_hit_descriptor;
                    U910   probe;
                    U910   collision_correction;
                    {
                        float sp;
                        float max_homing_step;
                        int   mode;

                        *(OvlQuad *)n = mine_moby->pos.q;
                        if ((mine_moby->b70 & 2) && mine_moby->b52 == 1 && mine_moby->anim != 2) {
                            blend_moby_animation(mine_moby, 2, 0, 10);
                        }
                        if (voice_is_playing(mine_moby, v->i68) == 0) {
                            v->i68 = allocate_voice_for_target_entry(1, 4, (int)mine_moby);
                        }
                        sp = v->f40 + frame_time_sq * 15.0f;
                        max_homing_step = frame_time * 4.0f;
                        v->f40 = sp;
                        if (max_homing_step < sp) {
                            v->f40 = max_homing_step;
                        }
                        mode = v->h52;
                        if (mode == 1) {
                            mine_moby->pos.f[2] += frame_scale * 0.05f;
                            if (fast_dec_timer(&v->i48) != 0) {
                                v->i48 = scale_game_frames(7);
                                v->h52 = 2;
                            }
                        } else if (mode == 2) {
                            mine_moby->pos.f[2] -= frame_scale * 0.05f;
                            if (fast_dec_timer(&v->i48) != 0) {
                                v->h52 = 0;
                            }
                        }
                        fast_vec_sub(t, &v->p54->pos, &mine_moby->pos);
                        fast_vec_normalize(t, t, v->f40);
                        fast_vec_add(&mine_moby->pos, &mine_moby->pos, t);
                        make_hit_descriptor(&homing_hit_descriptor, mine_moby, 0x10000, t, 1.0f);
                        qcopy(&probe, &mine_moby->pos);
                        probe.f[2] += 0.35f;
                        if (collect_mobies_in_sphere(
                                &probe, 0x10, mine_moby, &homing_hit_descriptor, 0.35f) != 0 &&
                            coll_hit_moby != 0 && ((M910 *)coll_hit_moby)->cls != 0x363 &&
                            ((M910 *)coll_hit_moby)->cls != 0x4A &&
                            ((M910 *)coll_hit_moby)->cls != 0x458 &&
                            ((M910 *)coll_hit_moby)->cls != 0x365 &&
                            ((M910 *)coll_hit_moby)->cls != 0x367) {
                            mine_moby->bBC = 1;
                        }
                        qcopy(&probe, &mine_moby->pos);
                        probe.f[2] += 0.25f;
                        if (FUN_L00_001f0d60(0.25f, &probe, 0, mine_moby) != 0) {
                            fast_vec_sub(&collision_correction, &coll_hit_position, &probe);
                            fast_vec_add(&mine_moby->pos, &mine_moby->pos, &collision_correction);
                        }
                        if (fast_dec_timer(&v->i58) != 0) {
                            mine_moby->bBC = 1;
                        }
                    }
                }
                update_moby_rotation_oscillation(mine_moby,
                                                 (char *)&v->f60,
                                                 (char *)&v->f64,
                                                 0.5235988f,
                                                 frame_time * 0.5235988f,
                                                 frame_time * 0.29670596f);
                if (mine_moby->state & 4) {
                    V910 t;
                    {
                        fast_vec_sub(t, &mine_moby->pos, &old);
                        v->vel.q = *(OvlQuad *)t;
                    }
                }
            }
            if (!(mine_moby->state & 2) && fast_dec_timer_s16(&v->h34) != 0) {
                mine_moby->state |= 2;
                v->p54 = 0;
            }
            {
                char *moby_hit_record = find_moby_hit(mine_moby, 0x830000, 0);

                if (moby_hit_record != 0 && *(M910 **)(moby_hit_record + 0x20) != 0 &&
                    (*(M910 **)(moby_hit_record + 0x20))->cls != mine_moby->cls) {
                    mine_moby->bBC = 1;
                }
            }
            mine_moby->bA4 = 0xFF;
            if (mine_moby->state & 2) {
                if (!(mine_moby->state & 4) ||
                    (vector_distance_xy(&((M910 *)hero.moby)->pos, &mine_moby->pos) < 0.75f &&
                     absolute_float(mine_moby->pos.f[2] - hero.motion.pos.f[2]) < 0.25f &&
                     (mine_moby->state & 0x10))) {
                    mine_moby->bBC = 1;
                } else if (fast_vec_dist(&((M910 *)hero.moby)->pos, &mine_moby->pos) < 0.75f) {
                    V910 t;
                    V910 n;
                    V910 r;
                    {
                        fast_vec_sub(r, &mine_moby->pos, &((M910 *)hero.moby)->pos);
                        *(OvlQuad *)t = *(OvlQuad *)r;
                        fast_vec_normalize(t, t, 0.75f);
                        fast_vec_add(r, &((M910 *)hero.moby)->pos, t);
                        *(OvlQuad *)n = *(OvlQuad *)r;
                        FUN_L00_00258490(
                            mine_moby, &mine_moby->pos, n, 0.4f, 0.25f, 0.25f, 0.5235988f, 0);
                    }
                } else {
                    M910 *best = 0;
                    float range =
                        (convert_integer_to_float(*((unsigned char *)&D_E910 + 0x11)) + 1.0f) *
                        4.0f;
                    int   i;
                    M910 *candidate_moby;

                    if (v->i78 != 0) {
                        range *= 3.0f;
                    }
                    i = 0;
                    candidate_moby = (M910 *)active_mobies[i];
                    while (candidate_moby != 0) {
                        float          rg;
                        unsigned char *p;

                        if (30.0f < fast_vec_dist(&mine_moby->pos, &candidate_moby->pos)) {
                            goto next;
                        }
                        if (fast_vec_dist(&mine_moby->pos, &candidate_moby->pos) < 1.5f) {
                            mine_moby->bBC = 1;
                            break;
                        }
                        if (is_moby_in_class_range_500_to_540((int)candidate_moby) != 0) {
                            goto next;
                        }
                        rg = range;
                        p = suck_get_aim_height((unsigned char *)candidate_moby);
                        if (p != 0) {
                            rg = range + convert_integer_to_float(p[0xA]) * 0.125f;
                        }
                        if (!(vector_distance_xy(&mine_moby->pos, &candidate_moby->pos) < rg)) {
                            goto next;
                        }
                        if (!(absolute_float(candidate_moby->pos.f[2] - mine_moby->pos.f[2]) <
                              2.0f)) {
                            goto next;
                        }
                        if (v->p54 != 0 && v->p54->state != 0xFE && v->p54->state != 0xFD) {
                            goto next;
                        }
                        if (best != 0) {
                            float candidate_distance =
                                vector_distance_xy(&mine_moby->pos, &candidate_moby->pos);

                            if (!(candidate_distance <
                                  vector_distance_xy(&mine_moby->pos, &best->pos))) {
                                goto next;
                            }
                        }
                        mine_moby->state |= 0x10;
                        best = candidate_moby;
                        blend_moby_animation(mine_moby, 1, 0, scale_game_frames(3));
                        allocate_voice_for_target_entry(0, 0, (int)mine_moby);
                        v->f40 = 0.0f;
                        v->i58 = scale_game_frames(600);
                        v->h52 = 1;
                        v->i48 = scale_game_frames(7);
                    next:
                        i++;
                        candidate_moby = (M910 *)active_mobies[i];
                    }
                    if (best != 0) {
                        v->p54 = best;
                    }
                }
            }
            v->i78 = 0;
            if (v->i5C < 0 || *(int *)&mine_force_burst != 0) {
                cart_burst(mine_moby, &mine_moby->pos, -1, 0.33f, 13.0f);
                mine_spawn_drops(mine_moby, D_L00_001B07B0);
                mark_moby_for_removal(mine_moby);
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
            k = k / scale_time(*(float *)&mine_bounce_ramp_duration);
            if (1.0f < k) {
                k = 1.0f;
            } else if (k < 0.0f) {
                k = 0.0f;
            }
            v->f70 = fast_add_rotations(v->f70, frame_time * 6.2831855f);
            mine_moby->pos.f[2] = mine_moby->pos.f[2] + v->f6C * fast_sin(v->f70) * k + 0.4f;
            if (mine_moby->bBC != 0) {
                explosion_camera_distance = vector_distance_xy(&mine_moby->pos, D_L00_00166DC0);
                if (v->h50 == 0) {
                    V910 t;
                    {
                        float s = convert_integer_to_float(D_E910.b11) * 0.5f + 1.0f;
                        int   hit =
                            collect_mobies_in_sphere(&mine_moby->pos, 0x10, mine_moby, 0, s + s);

                        *(OvlQuad *)t = mine_moby->pos.q;
                        send_blast_hits(mine_moby,
                                        t,
                                        D_L00_00177F00,
                                        hit,
                                        0,
                                        0x810000,
                                        4,
                                        1,
                                        3.0f,
                                        0.25f,
                                        1.5f);
                    }
                }
                gz = FUN_L00_00259710(&mine_moby->pos);
                if (v->h50 != 0) {
                    V910 t;
                    V910 n;
                    V910 r;
                    {
                        float wz;
                        int   i;
                        int   j;
                        int   l;
                        int   cb;
                        int   cg;
                        int   cr;

                        *(OvlQuad *)t = mine_moby->pos.q;
                        t[2] += 3.5f;
                        wz = probe_ground_height(t, 0, 0.5f);
                        for (i = 0; i < 150; i++) {
                            *(OvlQuad *)r = 0;
                            r[0] = random_float_between(-1.0f, 1.0f);
                            r[1] = random_float_between(-1.0f, 1.0f);
                            r[2] = random_float_between(-0.5f, -2.0f);
                            *(OvlQuad *)n = *(OvlQuad *)r;
                            fast_vec_normalize(
                                n, n, random_float_between(frame_time * 4.0f, frame_time * 8.0f));
                            random_integer_below_cf(&mine_moby->pos,
                                                    n,
                                                    random_float_between(0.05f, 0.1f) * 210000.0f,
                                                    wz);
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
                            fast_vec_add(r, r, &mine_moby->pos);
                            r[2] = wz + 0.5f;
                            n[2] -= rr * frame_time * 8.0f;
                            spawn_gravity_floor_burst_particle(r,
                                                               n,
                                                               0,
                                                               random_float_between(0.35f, 1.25f),
                                                               wz,
                                                               frame_time_sq * 20.0f,
                                                               (0x16 << 24) | (cb << 16) |
                                                                   (cg << 8) | cr,
                                                               (cb << 16) | (cg << 8) | cr,
                                                               1);
                        }
                        for (l = 0; l < 20; l++) {
                            int   c1 = mine_blend_color(0x7F000000,
                                                      (0x7F << 24) | (cb << 16) | (cg << 8) | cr,
                                                      random_float_between(0.25f, 1.0f));
                            int   c2 = mine_blend_color(0,
                                                      ((cb / 2) << 16) | ((cg / 2) << 8) | (cr / 2),
                                                      random_float_between(0.5f, 1.0f));
                            float sz;
                            int   life;

                            r[0] = random_float_between(-3.0f, 3.0f);
                            r[1] = random_float_between(-3.0f, 3.0f);
                            r[2] = random_float_between(frame_time * 4.0f, frame_time * 14.0f);
                            fast_vec_normalize(r, r, random_float_between(1.5f, 3.0f) * frame_time);
                            r[2] = random_float_between(3.0f, 6.0f) * frame_time;
                            *(OvlQuad *)n = mine_moby->pos.q;
                            n[2] = wz;
                            c1 = swap_packed_color_channels(c1, D_E910.b11);
                            c2 = swap_packed_color_channels(c2, D_E910.b11);
                            sz = random_float_between(31500.002f, 52500.0f);
                            life =
                                FUN_L00_00257b90(scale_game_frames(0x5A), scale_game_frames(0x96));
                            spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
                                n, r, c1, c2, sz, life, 1, D_L00_001B2080[11][0], -1);
                        }
                    }
                } else if (gz + 2.0f < mine_moby->pos.f[2]) {
                    V910 t;
                    {
                        clear_u64_value(t);
                        spawn_explosion(mine_moby,
                                        t,
                                        0,
                                        0.0f,
                                        0.0f,
                                        3,
                                        3,
                                        5,
                                        2.0f,
                                        1.0f,
                                        4.0f,
                                        1.0f,
                                        2,
                                        7.0f,
                                        0,
                                        1,
                                        -1,
                                        D_E910.b11);
                    }
                } else {
                    float sc = convert_integer_to_float(D_E910.b11) * 0.5f + 1.0f;
                    int   explosion_debris_count =
                        truncate_float_to_s32(explosion_camera_distance * 15.0f);
                    int i;

                    for (i = 0; i < explosion_debris_count; i++) {
                        int   kind = random_integer_below(4);
                        float tbl[4] = {
                            frame_time * 14.0f, frame_time * 22.0f, frame_time * 12.0f, 0.0f};
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
                            fast_vec_add(r, r, &mine_moby->pos);
                            r[2] -= rr * fast_cos(0.785398f);
                            n[2] -= rr * frame_time * 8.0f;
                            switch (kind) {
                            case MINE_DEBRIS_FIXED_SECONDARY_COLOR: {
                                int   c1 = swap_packed_color_channels(0x1F101820, D_E910.b11);
                                float sz = random_float_between(200000.0f, 300000.0f) * sc;
                                int   life = FUN_L00_00257b90(scale_game_frames(0xB4),
                                                            scale_game_frames(0xF0));

                                mine_debris((char *)r, (char *)n, c1, 0x101010, sz, life, 0);
                                break;
                            }
                            case MINE_DEBRIS_FIXED_LIFETIME: {
                                int   c1 = swap_packed_color_channels(0x3F081020, D_E910.b11);
                                int   c2 = swap_packed_color_channels(0x0F081020, D_E910.b11);
                                float sz = random_float_between(50000.0f, 100000.0f) * sc;

                                mine_debris(
                                    (char *)r, (char *)n, c1, c2, sz, scale_game_frames(0xB4), 1);
                                break;
                            }
                            case MINE_DEBRIS_SHORT_LIVED: {
                                int c1 = swap_packed_color_channels(
                                    random_integer_below(100) < 0x28 ? 0x5FF8F8F8 : 0x2F486078,
                                    D_E910.b11);
                                int   c2;
                                float sz;
                                int   life;

                                c2 = swap_packed_color_channels(0x0F000020, D_E910.b11);
                                sz = sc * 150000.0f;
                                life = FUN_L00_00257b90(scale_game_frames(0x1E),
                                                        scale_game_frames(0x2D));
                                mine_debris((char *)r, (char *)n, c1, c2, sz, life, 2);
                                break;
                            }
                            case MINE_DEBRIS_SPHERICAL_VELOCITY: {
                                int c1 = mine_blend_color(
                                    0x7F000000, 0x7F182030, random_float_between(0.25f, 1.0f));
                                int c2 =
                                    mine_blend_color(0, 0x5F5F5F, random_float_between(0.5f, 1.0f));
                                float sz;
                                int   life;

                                c1 = swap_packed_color_channels(c1, D_E910.b11);
                                c2 = swap_packed_color_channels(c2, D_E910.b11);
                                build_spherical_offset(n,
                                                       random_float_between(0.0f, 1.0f) *
                                                           frame_time,
                                                       ang,
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
                mine_moby->bBC = 0;
                if (D_E910.b11 != 0) {
                    allocate_voice_for_target_entry(3, 0, (int)mine_moby);
                } else {
                    allocate_voice_for_target_entry(2, 0, (int)mine_moby);
                }
                l00_camera.shake_amount = explosion_camera_distance < 20.0f
                                              ? 0.4f - explosion_camera_distance * 0.0175f
                                              : 0.050000012f;
                l00_camera.shake_frames = scale_game_frames(0x19);
                fast_vec_dist(&mine_moby->pos, hero.motion.pos.f);
                if (D_E910.b11 != 0) {
                    mine_camera_blast(
                        (char *)&alternate_camera_blast_parameters, (char *)&mine_moby->pos, 0, 0);
                } else {
                    mine_camera_blast(
                        (char *)&camera_blast_parameters, (char *)&mine_moby->pos, 0, 0);
                }
                mine_spawn_drops(mine_moby, D_L00_001B07B0);
                mark_moby_for_removal(mine_moby);
            } else {
                jet_animate((unsigned char *)mine_moby, 0.25f);
            }
        }
    }
}

#include "qcopy.h"

typedef unsigned int u128_2ac910 __attribute__((mode(TI), aligned(16)));

typedef union {
    u128_2ac910 q;
    float       f[4];
} Q_2ac910;

typedef struct {
    u128_2ac910 p;
    void       *o;
    int         n;
    float       f18;
    short       h1c;
    short       h1e;
} V_2ac910;

void set_moby_color(void *, int, int, int) __asm__("FUN_L00_002502f0");
int  FUN_001f96f8_2ac910(int) __asm__("FUN_001f96f8");
void update_entity(void *) __asm__("FUN_L00_00250df8");
/* Spawns a particle moby with a random rotation, owned by `owner`, at `pos` with velocity `vel`, tinted red/green/blue
   and living `lifetime` frames; returns the new moby or NULL. */
unsigned char *spawn_glow_particle(void *owner,
                                   void *pos,
                                   void *vel,
                                   int   lifetime,
                                   int   red,
                                   int   green,
                                   int   blue,
                                   int   alpha,
                                   float scale) __asm__("FUN_L00_002ac910");
unsigned char *spawn_glow_particle(void *owner,
                                   void *spawn_position,
                                   void *initial_velocity,
                                   int   lifetime,
                                   int   red,
                                   int   green,
                                   int   blue,
                                   int   alpha,

                                   float scale) {
    unsigned char *moby;
    red &= 0xFF;
    green &= 0xFF;
    blue &= 0xFF;
    alpha &= 0xFF;
    moby = create_moby(0x70);
    if (moby) {
        Q_2ac910  rot_copy, random_initial_rotation;
        V_2ac910 *data = ((V_2ac910 *)MOBY(moby)->pvars);
        float     elapsed;
        random_initial_rotation.q = 0;
        random_initial_rotation.f[0] = random_float_between(-3.1415927f, 3.1415927f);
        random_initial_rotation.f[1] = random_float_between(-3.1415927f, 3.1415927f);
        random_initial_rotation.f[2] = random_float_between(-3.1415927f, 3.1415927f);
        rot_copy.q = random_initial_rotation.q;
        moby[0x20] = 0;
        moby[0x23] = alpha;
        moby[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        set_moby_color(moby, red, green, blue);
        data->f18 = MOBY(moby)->scale * scale;
        *(int *)(moby + 0x2C) = 0;
        data->o = owner;
        qcopy(moby + 0x10, spawn_position);
        qcopy(data, initial_velocity);
        qcopy(moby + 0x40, &rot_copy);
        data->n = lifetime;
        data->h1c = *(unsigned short *)&data->n - FUN_001f96f8_2ac910(4);
        elapsed = convert_integer_to_float(data->n - data->h1c);
        MOBY(moby)->scale = elapsed * data->f18 / convert_integer_to_float(data->n);
        data->h1e = alpha;
        update_entity(moby);
    }
    return moby;
}

typedef struct {
    char  pad[0x14];
    int   n;
    float f18;
    s16   h1c;
    s16   h1e;
} V_2acad8;

typedef struct {
    char      pad[0x23];
    u8        b23;
    char      pad2[0x2C - 0x24];
    float     f2c;
    char      pad3[0x40 - 0x30];
    float     r[3];
    char      pad4[0x78 - 0x4C];
    V_2acad8 *v;
} M_2acad8;

extern float D_0015ED6C_2acad8 __asm__("D_0015ED6C");

void update_glow_particle(M_2acad8 *particle_moby) __asm__("FUN_L00_002acad8");

void update_glow_particle(M_2acad8 *particle_moby) {
    V_2acad8 *v = particle_moby->v;
    float     elapsed_lifetime_frames, total_lifetime_frames;
    int       n;
    elapsed_lifetime_frames = convert_integer_to_float(v->n - v->h1c);
    total_lifetime_frames = convert_integer_to_float(v->n);
    particle_moby->f2c = elapsed_lifetime_frames * v->f18 / total_lifetime_frames;
    particle_moby->r[0] = fast_add_rotations(particle_moby->r[0], D_0015ED6C_2acad8 * 3.1415927f);
    particle_moby->r[1] = fast_add_rotations(particle_moby->r[1], D_0015ED6C_2acad8 * 3.1415927f);
    particle_moby->r[2] = fast_add_rotations(particle_moby->r[2], D_0015ED6C_2acad8 * 3.1415927f);
    if (fast_dec_timer_s16(&v->h1c)) {
        mark_moby_for_removal(particle_moby);
        return;
    }
    n = v->n / 2;
    if (v->h1c < n) {
        particle_moby->b23 = v->h1c * v->h1e / n;
    }
}

#include "eetypes.h"
#include "qcopy.h"

typedef union {
    u128 q_002acbf0;
    f32  f[4];
} V_2acbf0_002acbf0;

typedef struct {
    V_2acbf0_002acbf0 corner[4];
    u32               color[4];
    f32               uv[8];
    s64               unk70;
    s64               tex;
    s64               unk80;
    s64               unk88;
} Q_2acbf0_002acbf0;

typedef struct {
    u8                pad0[0x10];
    V_2acbf0_002acbf0 pos;
    f32               f20, f24, f28;
} PV_2acbf0_002acbf0;

typedef struct {
    u8                  pad0[0x78];
    PV_2acbf0_002acbf0 *pv;
} M_2acbf0_002acbf0;

extern s32  D_L00_0015F5C4_002acbf0 __asm__("D_L00_0015F5C4");
extern s32  D_L00_0015F5CC_002acbf0 __asm__("D_L00_0015F5CC");
int         FUN_001f96f8_002acbf0(int) __asm__("FUN_001f96f8");
extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern s64  get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void draw_geometry_quad(void *, void *, s32) __asm__("FUN_001f7d30");
void        draw_bomb_landing_marker(M_2acbf0_002acbf0 *moby) __asm__("FUN_L00_002acbf0");

void draw_bomb_landing_marker(M_2acbf0_002acbf0 *moby) {
    Q_2acbf0_002acbf0   quad;
    V_2acbf0_002acbf0   quad_transform[4];
    V_2acbf0_002acbf0   mz[4];
    V_2acbf0_002acbf0   quad_alignment_angles;
    V_2acbf0_002acbf0   rz;
    PV_2acbf0_002acbf0 *pv = moby->pv;
    s32                 i;
    s32                 odd;
    f32                 polar_angle, rotation_angle_numerator;
    if (D_L00_0015F5C4_002acbf0 == 2) {
        return;
    }
    for (i = 0; i < 2; i++) {
        odd = i & 1;
        polar_angle = fast_arc_tan(square_root(pv->f20 * pv->f20 + pv->f28 * pv->f28), pv->f24);
        quad_alignment_angles.f[0] = fast_add_rotations(-polar_angle, 1.57079637f);
        quad_alignment_angles.f[1] = fast_arc_tan(pv->f28, pv->f20);
        quad_alignment_angles.f[2] = 0.0f;
        quad_alignment_angles.f[3] = 0.0f;
        rz.f[0] = 0.0f;
        rotation_angle_numerator =
            convert_integer_to_float(D_L00_0015F5CC_002acbf0 % FUN_001f96f8_002acbf0(180));
        {
            f32 scaled_rotation_period = scale_time(180.0f);
            f32 rotation_angle;
            rotation_angle_numerator *= 6.28318548f;
            rotation_angle =
                fast_normalize_angle(rotation_angle_numerator / scaled_rotation_period);
            if (odd) {
                rotation_angle = -rotation_angle;
            }
            rz.f[1] = rotation_angle;
        }
        rz.f[2] = 0.0f;
        rz.f[3] = 0.0f;
        euler_rotation_matrix(mz, &rz);
        euler_rotation_matrix(quad_transform, &quad_alignment_angles);
        multiply_matrix(quad_transform, quad_transform, mz);
        qcopy(&quad_transform[3], &pv->pos);
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
        draw_geometry_quad(&quad, quad_transform, 1);
    }
}

typedef unsigned int u128_2ace70 __attribute__((mode(TI), aligned(16)));

typedef union {
    u128_2ace70 q;
    float       f[4];
} V_2ace70;

extern float   D_0015ED6C_2ace70 __asm__("D_0015ED6C");
unsigned char *spawn_bomb(int owner, void *pos) __asm__("FUN_L00_002ace70");

unsigned char *spawn_bomb(int owner, void *spawn_position) {
    unsigned char *bomb_moby = create_moby(0x79);
    if (bomb_moby) {
        V_2ace70       u, random_rotation;
        unsigned char *v = ((unsigned char *)MOBY(bomb_moby)->pvars);
        struct Hero   *hero_context;
        float          frame_scaled_random_angle;
        random_rotation.q = 0;
        random_rotation.f[0] = random_float_between(-3.1415927f, 3.1415927f);
        random_rotation.f[1] = random_float_between(-3.1415927f, 3.1415927f);
        random_rotation.f[2] = random_float_between(-3.1415927f, 3.1415927f);
        u.q = random_rotation.q;
        bomb_moby[0x30] = 0xFF;
        MOBY(bomb_moby)->draw_dist = 0xFF;
        bomb_moby[0x31] = 1;
        bomb_moby[0x20] = 0;
        *(int *)(v + 0x50) = owner;
        *(short *)(v + 0x56) = 0;
        *(short *)(v + 0x6A) = 0;
        *(int *)(v + 0x60) = 0;
        hero_context = &hero;
        *(float *)(v + 0x6C) = fast_arc_tan(hero_context->facing_x, hero_context->facing_y);
        *(int *)(v + 0x64) = 0;
        qcopy(bomb_moby + 0x10, spawn_position);
        clear_u64_value(v);
        qcopy(bomb_moby + 0x40, &u);
        frame_scaled_random_angle =
            random_float_between(D_0015ED6C_2ace70 * 1.0471976f, D_0015ED6C_2ace70 * 3.1415927f);
        {
            float frame_time = D_0015ED6C_2ace70;
            *(float *)(v + 0x58) = frame_scaled_random_angle;
            *(float *)(v + 0x5C) =
                random_float_between(frame_time * 1.0471976f, frame_time * 3.1415927f);
        }
        update_entity(bomb_moby);
    }
    return bomb_moby;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002AB910.c: func_L00_002AE278). */
extern f32  fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern void aim_throw_arc(void *, void *, void *, f32, f32, f32) __asm__("FUN_L00_002c2488");

#include "rnc/input/pad_globals.h"

void launch_bomb(float *pos, unsigned char *moby, float *velocity) __asm__("FUN_L00_002acfd8");

void launch_bomb(float *pos, unsigned char *moby, float *velocity) {
    float         *vars = ((float *)MOBY(moby)->pvars);
    float          difference[4] __attribute__((aligned(16)));
    float          direction[4] __attribute__((aligned(16)));
    float          result[4] __attribute__((aligned(16)));
    float          working[4] __attribute__((aligned(16)));
    float          desired[4] __attribute__((aligned(16)));
    unsigned char *mp;
    float         *rp;
    float          dot, length, distance, angle, height, total;

    *(short *)((char *)vars + 0x6a) = scale_game_frames(30);
    moby[0x20] = 1;
    qcopy(vars, velocity);
    if (hero.state.control_mode == 15) {
        fast_vec_add(vars, vars, &hero.motion.displacement);
    } else if (hero.ground_moby != 0 && FUN_002141f8_cf((unsigned char *)hero.ground_moby) != 0) {
        fast_vec_add(vars, vars, &hero.motion.displacement);
    } else if (*(float **)((char *)vars + 0x60) != 0) {
        short cls = *(short *)(*(char **)((char *)vars + 0x60) + 0xa6);
        if (cls == 0x56e || cls == 0x58e) {
            fast_vec_add(vars, vars, vars + 0x10);
        } else if (*(int *)((char *)vars + 0x64) != 0) {
            length = vector_length_xy(velocity);
            fast_vec_sub(difference, &hero.motion.pos, *(char **)((char *)vars + 0x60) + 0x10);
            fast_vec_normalize(difference, difference, 1.0f);
            dot = fast_vec_dot(difference, vars + 0x10);
            total = dot + length;
            distance = vector_distance_xy(pos, *(char **)((char *)vars + 0x60) + 0x10) / total;
            if (dot > 0.0f) {
                scale_vector_xyz(direction, difference, distance * dot);
                fast_vec_add(working, direction, *(char **)((char *)vars + 0x60) + 0x10);
                *(OvlQuad *)direction = *(OvlQuad *)working;
            } else {
                scale_vector_xyz(direction, vars + 0x10, distance);
                fast_vec_add(working, direction, *(char **)((char *)vars + 0x60) + 0x10);
                *(OvlQuad *)direction = *(OvlQuad *)working;
            }
            {
                float heading = fast_arc_tan(hero.facing_x, hero.facing_y);
                float toward = fast_arc_tan(direction[0] - hero.motion.pos.f[0],
                                            direction[1] - hero.motion.pos.f[1]);
                angle = fast_difference_between_rotations(heading, toward);
            }
            if (angle > 1.5707964f) {
                working[0] = fast_cos(fast_arc_tan(hero.facing_x, hero.facing_y)) *
                             vector_distance_xy(&hero.motion.pos, direction) * 0.5f;
                angle = fast_sin(fast_arc_tan(hero.facing_x, hero.facing_y));
                working[1] = angle * vector_distance_xy(&hero.motion.pos, direction) * 0.5f;
                working[2] = 0.0f;
                fast_vec_add(desired, working, &hero.motion.pos);
                *(OvlQuad *)direction = *(OvlQuad *)desired;
            }
            total = frame_time_sq * 11.0f;
            rp = result;
            if (pad_held[0] & 5) {
                height = convert_integer_to_float(D_0013E520.bA) * 2.5f + 8.5f;
            } else {
                height = 8.5f;
            }
            aim_throw_arc(pos, direction, rp, total, 0.00001f, height);
            scale_vector_xyz(result, result, 0.65f);
            scale_vector_xyz(vars, vars, 0.35f);
            fast_vec_add(working, vars, result);
            *(OvlQuad *)vars = *(OvlQuad *)working;
        }
    }
    mp = moby + 0x10;
    *(short *)((char *)vars + 0x54) = scale_game_frames(300);
    qcopy(mp, pos);
    *(OvlQuad *)difference = *(OvlQuad *)((char *)hero.moby + 0x10);
    difference[2] = pos[2];
    if (collision_line(difference, pos, 0, (int)hero.moby, 0) != 0) {
        moby[0xbc] = 1;
        qcopy(mp, &coll_hit.pos);
    }
    if (D_0013E520.bA != 0 && hero.state.current == HERO_STATE_LOOK) {
        float z = vars[2];
        scale_vector_xyz(vars, vars, 1.5f);
        vars[2] = z / 1.5f;
    }
}

typedef struct {
    U910  vel;
    U910  landing;
    U910  normal;
    U910  previous_hit;
    U910  difference;
    M910 *holder;
    short timer;
    short holder_flag;
    char  pad58[8];
    M910 *last_hit;
    int   hit_count;
} LandingVars;

extern float D_0015ED70_2ad410 __asm__("D_0015ED70");
extern int   ray_hits(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern int   FUN_L00_002603d0_2ad410(M910 *) __asm__("FUN_L00_002603d0");
extern void  add_draw_callback(void *, void *) __asm__("FUN_001f4600");

int predict_throw_landing(LandingVars *v, M910 *thrown_moby, int mode) __asm__("FUN_L00_002ad410");

int predict_throw_landing(LandingVars *v, M910 *thrown_moby, int mode) {
    U910 position, velocity, previous, hero_position, offset, start;
    int  timer;
    int  object_hit;
    int  found = 0, i;
    object_hit = 0;

    if (hero.state.current == 0x72)
        return 0;
    if (D_L00_0015F5C4_002acbf0 != 0)
        return 0;
    qcopy(&previous, &thrown_moby->pos);
    if (thrown_moby->state == 0) {
        position.f[0] =
            fast_cos(fast_add_rotations(-0.361968f, fast_arc_tan(hero.facing_x, hero.facing_y))) *
            0.859039f;
        position.f[1] =
            fast_sin(fast_add_rotations(-0.361968f, fast_arc_tan(hero.facing_x, hero.facing_y))) *
            0.859039f;
        position.f[2] = 0.0f;
        fast_vec_add(&position, &position, &hero.motion.pos);
        position.f[2] += 0.49082f;
        qcopy(&velocity, &v->vel);
        if (hero.state.control_mode == 15)
            fast_vec_add(&velocity, &velocity, &hero.motion.displacement);
        if (hero.ground_moby && FUN_L00_002603d0_2ad410((M910 *)hero.ground_moby) && mode)
            fast_vec_add(&velocity, &velocity, &hero.motion.displacement);
        timer = scale_game_frames(300);
    } else {
        W910 *holder_vars = v->holder->v;
        int   holder_flag;
        qcopy(&position, &thrown_moby->pos);
        qcopy(&velocity, &v->vel);
        holder_flag = *(int *)((char *)holder_vars + 0x50);
        timer = v->timer;
        if (holder_flag)
            v->holder_flag = 1;
    }
    {
        float z = position.f[2];
        hero_position.q = ((M910 *)hero.moby)->pos.q;
        hero_position.f[2] = z;
    }
    if (ray_hits(&hero_position, &position, 0x10, hero.moby, 0) &&
        (collision_hit_material() || 0.0f < velocity.f[2])) {
        if (coll_hit.moby && coll_hit.i1C <= 0) {
            if ((coll_hit.moby != hero.moby && coll_hit.moby != v->holder) ||
                timer < scale_game_frames(300) - scale_game_frames(10)) {
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
                        fast_vec_sub(&start, (char *)h->moby + 0x10, &v->previous_hit);
                        v->difference.q = start.q;
                        v->previous_hit.q = h->moby->pos.q;
                    }
                }
                object_hit = 1;
                qcopy(&v->landing, (char *)coll_hit.moby + 0x10);
                found = 1;
                fast_vec_add(&offset, &offset, &v->landing);
                probe_ground_height(&offset, 0, 0.5f);
            }
        } else if (coll_hit.i1C > 0) {
            qcopy(&v->landing, &coll_hit.pos);
            found = 1;
        }
    }
    if (*(unsigned char *)((char *)&D_E910 + 0xa) && hero.state.current == 1) {
        float z = velocity.f[2];
        fast_vec_scale(&velocity, &velocity, 1.5f);
        velocity.f[2] = z / 1.5f;
    }
    hero_position.q = position.q;
    while (!fast_dec_timer(&timer) && !found) {
        qcopy(&previous, &position);
        for (i = 9; i >= 0; i--) {
            fast_vec_add(&position, &position, &velocity);
            velocity.f[2] -= D_0015ED70_2ad410 * 11.0f;
        }
        if (!ray_hits(&previous, &position, 0x10, thrown_moby, 0))
            continue;
        if (!collision_hit_material() && 0.0f < velocity.f[2])
            continue;
        if (coll_hit.moby && coll_hit.i1C <= 0) {
            if (!(position.f[2] - hero_position.f[2] < 2.0f))
                goto check_surface;
            if ((coll_hit.moby == hero.moby || coll_hit.moby == v->holder) &&
                timer >= scale_game_frames(300) - scale_game_frames(10))
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
                    fast_vec_sub(&start, (char *)h->moby + 0x10, &v->previous_hit);
                    v->difference.q = start.q;
                    v->previous_hit.q = h->moby->pos.q;
                }
            }
            object_hit = 1;
            qcopy(&v->landing, (char *)coll_hit.moby + 0x10);
            fast_vec_add(&offset, &offset, &v->landing);
            probe_ground_height(&offset, 0, 0.5f);
            break;
        }
    check_surface:
        if (coll_hit.i1C > 0) {
            qcopy(&v->landing, &coll_hit.pos);
            break;
        }
    }
    if (!timer)
        goto return_zero;
    if (!mode) {
        if (!coll_hit.moby)
            goto return_zero;
        if (!FUN_L00_002603d0_2ad410(coll_hit.moby))
            goto return_zero;
    }
    if (mode != 2 || !coll_hit.moby || !FUN_L00_002603d0_2ad410(coll_hit.moby))
        add_draw_callback(draw_bomb_landing_marker, thrown_moby);
    if (!object_hit && v->last_hit) {
        if (v->last_hit->state == 0xfe) {
            v->hit_count = 0;
        } else if (v->last_hit->state == 0xfd) {
            v->hit_count = 0;
        } else if (!(v->last_hit->flags & 0x1000)) {
            v->hit_count = 0;
        } else {
            if (v->hit_count <= scale_game_frames(30))
                goto keep_hit;
            v->hit_count = 0;
        }
        v->last_hit = 0;
    }
keep_hit:
    v->normal.q = coll_hit_normal.q;
    fast_vec_sub(&previous, &v->landing, D_L00_00166DC0);
    fast_vec_scale(&previous, &previous, 0.95f);
    fast_vec_add(&v->landing, &previous, D_L00_00166DC0);
    return 1;
return_zero:
    return 0;
}

/* Update of moby class 121, a thrown bomb: it flies and spins, bounces once on the ground, explodes on a hit or when its timer runs out, then its blast ring grows for 15 frames. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002AB910.c: func_L00_002AEDD0), where it is exact; names translated to the US level program. */
typedef union {
    u128  q;
    float f[4];
} Vec_2adb30 __attribute__((aligned(16)));

typedef struct {
    int c[6];
} Colors6;

typedef struct BombMoby BombMoby;

typedef struct {
    Vec_2adb30 v;
    char       pad10[0x40];
    M910      *owner;
    short      h54;
    short      h56;
    char       pad58[0x10];
    short      h68;
    short      h6a;
} BombData;

struct BombMoby {
    char           p0[0x10];
    Vec_2adb30     pos;
    unsigned char  state;
    char           p21[0x13];
    unsigned short flags;
    char           p36[0xA];
    float          rx;
    float          ry;
    char           p48[0x2C];
    int            f74;
    BombData      *d;
    char           p7C[0x14];
    int            color;
    char           p94[0x12];
    short          cls;
    char           pA8[0x14];
    unsigned char  bBC;
};

typedef struct {
    Vec_2adb30    dir;
    BombMoby     *moby;
    int           flags;
    unsigned char kind;
    unsigned char on;
    short         cls;
    float         fDC;
    int           iE0;
} BombHit;

extern float          frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
extern float          frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;
extern int            D_L00_0015F5CC_2adb30 __asm__("D_L00_0015F5CC");
extern s32            g_debug_camera[] __asm__("D_L00_00166DC0");
extern int           *D_L00_00177F00_2adb30[] __asm__("D_L00_00177F00");
extern unsigned char *D_L00_001B2080_2adb30[] __asm__("D_L00_001B2080");
extern G910           D_G910 __asm__("D_0013F350");
extern int            FUN_001f96f8_2adb30(int) __asm__("FUN_001f96f8");
extern float          FUN_001f96b0_2adb30(float) __asm__("FUN_001f96b0");
extern void           FUN_001f9a28_2adb30(void *, void *, void *) __asm__("FUN_001f9a28");
extern void           FUN_001f9a68_2adb30(void *, void *, float) __asm__("FUN_001f9a68");
extern float          FUN_001f9ab0_2adb30(void *, void *) __asm__("FUN_001f9ab0");
extern float          FUN_001f9af0_2adb30(void *) __asm__("FUN_001f9af0");
extern float          FUN_001f9dc8_2adb30(float) __asm__("FUN_001f9dc8");
extern float          FUN_001f9de0_2adb30(float) __asm__("FUN_001f9de0");
extern int            fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern float          FUN_002135f0_2adb30(float *, void *) __asm__("FUN_002135f0");
extern char *
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern void  FUN_001f9bf8_2adb30(void *, void *, float) __asm__("FUN_001f9bf8");
extern int   random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void  random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern float allocate_voice_for_target_entry_cf(void *) __asm__("FUN_L00_00259710");
extern float interpolate_scalar_linear(float, float, float) __asm__("FUN_L00_0025b6a8");
extern void  swap_color_channel_words(int *, int *, int *, int) __asm__("FUN_L00_0025c088");
extern unsigned char *FUN_L00_002603d0_2adb30(unsigned char *) __asm__("FUN_L00_002603d0");
extern float          normalize_angle(float) __asm__("FUN_L00_0025e310");
extern void           FUN_L00_0026a9f0_2adb30(
              void *, void *, int, int, float, int, int, int, int, float) __asm__("FUN_L00_0026a9f0");
extern int trooper_play_sound(int i, int voice_flags, int moby, int k) __asm__("FUN_L00_0028dc90");
extern int FUN_L00_002ad410_2adb30(void *, void *, int) __asm__("FUN_L00_002ad410");
extern char   *FUN_L00_002af450_2adb30(char *, char *, int, int, int) __asm__("FUN_L00_002af450");
extern char   *spawn_randomly_rotated_colored_moby(void *,
                                                   void *,
                                                   void *,
                                                   float,
                                                   int,
                                                   unsigned char,
                                                   unsigned char,
                                                   unsigned char,
                                                   int) __asm__("FUN_L00_002df808");
void           bomb_update(BombMoby *m) __asm__("FUN_L00_002adb30");
extern int     D_L00_00160770;
extern short   bomb_spark_vertical_acceleration __asm__("D_L00_001613F0") __attribute__((sda));
extern short   bomb_spark_velocity_scale __asm__("D_L00_001613F4") __attribute__((sda));
extern short   bomb_spark_random_velocity_radius __asm__("D_L00_001613F8") __attribute__((sda));
extern short   bomb_spark_random_frame_count_range __asm__("D_L00_001613FC") __attribute__((sda));
extern short   bomb_spark_vertical_offset_frame_base __asm__("D_L00_00161400") __attribute__((sda));
extern short   bomb_spark_jittered_frame_count_base __asm__("D_L00_00161404") __attribute__((sda));
extern short   bomb_spark_frame_count_jitter __asm__("D_L00_00161408") __attribute__((sda));
extern short   D_L00_0016140C __attribute__((sda));
extern short   D_L00_00161410 __attribute__((sda));
extern short   D_L00_00161414 __attribute__((sda));
extern short   D_L00_00161418 __attribute__((sda));
extern short   bomb_spark_primary_color_start __asm__("D_L00_0016141C") __attribute__((sda));
extern short   bomb_spark_primary_color_end __asm__("D_L00_00161420") __attribute__((sda));
extern short   bomb_spark_secondary_color_start __asm__("D_L00_00161424") __attribute__((sda));
extern short   bomb_spark_secondary_color_end __asm__("D_L00_00161428") __attribute__((sda));
extern short   bomb_sparks_per_emission __asm__("D_L00_0016142C") __attribute__((sda));
extern short   bomb_spark_texture_selector __asm__("D_L00_00161430") __attribute__((sda));
extern B910    l00_bomb_camera_blast_parameters __asm__("D_L00_001E98D0");
extern Colors6 l00_bomb_blast_effect_primary_palette __asm__("D_L00_001E9920");
extern Colors6 l00_bomb_blast_effect_secondary_palette __asm__("D_L00_001E9938");

void bomb_update(BombMoby *bomb_moby) {
    B910       rec = l00_bomb_camera_blast_parameters;
    Vec_2adb30 delta;
    Vec_2adb30 old;
    Vec_2adb30 blast_velocity;
    Vec_2adb30 b;
    Vec_2adb30 proj;
    BombData  *d;
    b.q = 0;
    b.f[2] = frame_time_macro * 2.0f;
    blast_velocity.q = b.q;
    b.q = 0;
    b.f[2] = 1.0f;
    d = bomb_moby->d;
    if ((D_G910.hero == 0 || D_G910.hero->cls != 0xC0) && bomb_moby->state == 0) {
        mark_moby_for_removal(bomb_moby);
        return;
    }
    if (D_G910.b20A4 != 0 && bomb_moby->state == 0) {
        mark_moby_for_removal(bomb_moby);
        return;
    }
    if ((D_G910.b20A5 != 0 || D_G910.b20AF != 0) && bomb_moby->state == 0) {
        bomb_moby->flags |= 0x41;
    } else {
        bomb_moby->flags &= ~0x41;
    }
    if (d->owner->cls == 0xC0 && d->h56 == 0 && FUN_001f9af0_2adb30(d) > 0.0f) {
        if (D_G910.p2FC == 0 || FUN_L00_002603d0_2adb30(D_G910.p2FC) == 0) {
            FUN_L00_002ad410_2adb30(d, bomb_moby, 1);
        } else if (FUN_L00_002ad410_2adb30(d, bomb_moby, 0) == 0) {
            FUN_L00_002ad410_2adb30(d, bomb_moby, 2);
        }
    }
    if (bomb_moby->state == 1) {
        Vec_2adb30 c;
        float      t;
        int        cr;
        int        cg;
        int        cb;
        c.q = 0;
        c.f[0] = 128.0f;
        c.f[1] = 128.0f;
        c.f[2] = 128.0f;
        c.f[3] = 255.0f;
        fast_dec_timer_s16(&d->h6a);
        t = convert_integer_to_float(d->h6a) / convert_integer_to_float(FUN_001f96f8_2adb30(0x1E));
        cr = truncate_float_to_s32((c.f[0] - 255.0f) * t + 255.0f);
        cg = truncate_float_to_s32((c.f[1] - 64.0f) * t + 64.0f);
        cb = truncate_float_to_s32((c.f[2] - 64.0f) * t + 64.0f);
        bomb_moby->color = (cb << 16) | (0xFF << 24) | (cg << 8) | cr;
        if (d->h68 == 0) {
            if ((D_L00_0015F5CC_2adb30 & 7) == 0) {
                int i;
                for (i = 0; i < *(int *)&bomb_sparks_per_emission; i++) {
                    Vec_2adb30 spark_velocity;
                    Vec_2adb30 v2;
                    Vec_2adb30 random_velocity_offset;
                    int        p0;
                    int        spark_secondary_color;
                    int        spark_random_frame_count;
                    int        spark_vertical_offset_scale;
                    int        spark_jittered_frame_count;
                    FUN_001f9a68_2adb30(&spark_velocity, d, *(float *)&bomb_spark_velocity_scale);
                    random_vector(random_velocity_offset.f,
                                  0.0f,
                                  *(float *)&bomb_spark_random_velocity_radius * frame_time_macro);
                    fast_vec_add(&spark_velocity, &random_velocity_offset, &spark_velocity);
                    clear_u64_value(v2.f);
                    spark_velocity.f[3] =
                        random_float_between(*(float *)&D_L00_0016140C, *(float *)&D_L00_00161410);
                    v2.f[3] =
                        random_float_between(*(float *)&D_L00_00161414, *(float *)&D_L00_00161418);
                    p0 = fast_tween_color(*(int *)&bomb_spark_primary_color_start,
                                          *(int *)&bomb_spark_primary_color_end,
                                          random_float_between(0.0f, 1.0f));
                    spark_secondary_color =
                        fast_tween_color(*(int *)&bomb_spark_secondary_color_start,
                                         *(int *)&bomb_spark_secondary_color_end,
                                         random_float_between(0.0f, 1.0f));
                    spark_random_frame_count = truncate_float_to_s32(
                        FUN_001f96b0_2adb30((float)*(int *)&bomb_spark_random_frame_count_range *
                                                random_float_between(0.0f, 1.0f) +
                                            1.0f));
                    spark_vertical_offset_scale = truncate_float_to_s32(FUN_001f96b0_2adb30(
                        (float)*(int *)&bomb_spark_vertical_offset_frame_base *
                        (random_float_between(-*(float *)&bomb_spark_frame_count_jitter,
                                              *(float *)&bomb_spark_frame_count_jitter) +
                         1.0f)));
                    spark_jittered_frame_count = truncate_float_to_s32(FUN_001f96b0_2adb30(
                        (float)*(int *)&bomb_spark_jittered_frame_count_base *
                        (random_float_between(-*(float *)&bomb_spark_frame_count_jitter,
                                              *(float *)&bomb_spark_frame_count_jitter) +
                         1.0f)));
                    v2.f[2] -= *(float *)&bomb_spark_vertical_acceleration * frame_time_sq_macro *
                               (float)spark_vertical_offset_scale;
                    spawn_spark(&bomb_moby->pos,
                                &spark_velocity,
                                &v2,
                                p0,
                                spark_secondary_color,
                                spark_random_frame_count,
                                spark_vertical_offset_scale,
                                spark_jittered_frame_count,
                                *(int *)&bomb_spark_texture_selector);
                }
            }
        }
        {
            BombHit q;
            bomb_moby->rx = fast_add_rotations(bomb_moby->rx, frame_time_macro * 2.0943952f);
            bomb_moby->ry = fast_add_rotations(bomb_moby->ry, frame_time_macro * 7.8539815f);
            qcopy_nc(&old, &bomb_moby->pos);
            fast_vec_add(&bomb_moby->pos, &bomb_moby->pos, d);
            if (d->h68 == 0) {
                d->v.f[2] -= frame_time_sq_macro * 11.0f;
            } else {
                d->v.f[2] -= frame_time_sq_macro * 11.0f / 10.0f;
                if (random_integer_below(FUN_001f96f8_2adb30(6) - 1) == 0) {
                    float h = FUN_002135f0_2adb30((float *)&bomb_moby->pos, &q);
                    FUN_001f9a68_2adb30(&q, d, 0.25f);
                    random_integer_below_cf(
                        &bomb_moby->pos, &q, random_float_between(0.05f, 0.1f) * 210000.0f, h);
                }
            }
            q.moby = bomb_moby;
            q.flags = 0x830000;
            if (d->h54 < FUN_001f96f8_2adb30(0x12C) - FUN_001f96f8_2adb30(0xA)) {
                q.flags |= 1;
            }
            q.iE0 = 1;
            q.fDC = 2.0f;
            qcopy(&q, d);
            set_vector_length_xy(&q, &q, 1.0f);
            q.dir.f[2] = 1.0f;
            q.dir.f[3] = 5627.9248f;
            q.kind = 2;
            q.cls = bomb_moby->cls;
            q.on = 1;
            if (d->h68 != 0) {
                d->h68++;
            }
            if (collision_line(&old, &bomb_moby->pos, 0x10, (int)bomb_moby, (int)&q)) {
                if (d->h68 == 0 && collision_hit_material() == 0 && d->v.f[2] < 0.0f) {
                    Vec_2adb30 p;
                    Vec_2adb30 splash_particle_velocity;
                    char      *s;
                    int        i;
                    pool_splash(*(int *)&D_L00_001612D0_gp,
                                *(int *)&D_L00_001612D8_gp,
                                1,
                                bomb_moby->pos.f[0],
                                bomb_moby->pos.f[1],
                                0.5f,
                                -0.35f);
                    d->h68 = 1;
                    clear_u64_value(d->v.f);
                    d->v.f[2] = frame_time_macro * -1.5f;
                    p.f[0] = bomb_moby->pos.f[0];
                    p.f[1] = bomb_moby->pos.f[1];
                    p.f[2] = coll_hit.pos.f[2];
                    s = spawn_burst_moby(&p, 2.0f);
                    if (s != 0) {
                        s[0x23] = 0x70;
                    }
                    for (i = 0; i < 16; i++) {
                        float ang = random_angle_radians();
                        float r =
                            random_float_between(frame_time_macro * 0.0f, frame_time_macro * 3.0f);
                        int n;
                        splash_particle_velocity.f[0] = FUN_001f9dc8_2adb30(ang) * r;
                        splash_particle_velocity.f[1] = FUN_001f9de0_2adb30(ang) * r;
                        splash_particle_velocity.f[2] =
                            random_float_between(frame_time_macro * 3.0f, frame_time_macro * 6.5f);
                        n = random_int_between(0x5A, 0x78);
                        spawn_debris_chunk(
                            &p, &splash_particle_velocity, random_integer_below(2), n);
                    }
                } else if (d->h68 != 0 || collision_hit_material() != 0) {
                    if (coll_hit.moby != 0) {
                        if ((coll_hit.moby != D_G910.cam && coll_hit.moby != d->owner) ||
                            d->h54 < FUN_001f96f8_2adb30(0x12C) - FUN_001f96f8_2adb30(0xA)) {
                            if (((BombMoby *)coll_hit.moby)->f74 != D_L00_00160770 ||
                                coll_hit.moby->cls == 0x336) {
                                H910 *h = &coll_hit;
                                bomb_moby->bBC = 1;
                                qcopy(&bomb_moby->pos, &h->pos);
                                reflect_vector(&blast_velocity, d, &h->nrm);
                                FUN_001f9bf8_2adb30(
                                    &blast_velocity, &blast_velocity, frame_time_macro * 2.0f);
                                FUN_001f9bf8_2adb30(&b, &h->nrm, 1.0f);
                            }
                        }
                    } else if (coll_hit.i1C > 0) {
                        H910 *h = &coll_hit;
                        bomb_moby->bBC = 1;
                        qcopy(&bomb_moby->pos, &h->pos);
                        reflect_vector(&blast_velocity, d, &h->nrm);
                        FUN_001f9bf8_2adb30(
                            &blast_velocity, &blast_velocity, frame_time_macro * 2.0f);
                        FUN_001f9bf8_2adb30(&b, &h->nrm, 1.0f);
                    }
                }
            } else if (fast_dec_timer_s16(&d->h54)) {
                bomb_moby->bBC = 1;
                fast_vec_add(&blast_velocity, &blast_velocity, d);
            }
            if (bomb_moby->bBC != 0) {
                float      s = convert_integer_to_float((unsigned char)D_E910.p0[0xA]) + 1.0f;
                float      camera_distance;
                Vec_2adb30 v;
                int        i;
                FUN_001f9a68_2adb30(&blast_velocity, &blast_velocity, s);
                FUN_001f9a28_2adb30(&delta, g_debug_camera, &bomb_moby->pos);
                camera_distance = FUN_001f9af0_2adb30(&delta);
                if (d->h68 < FUN_001f96f8_2adb30(0x14)) {
                    int n;
                    n = collect_mobies_in_sphere(&bomb_moby->pos, 0x10, bomb_moby, 0, s * 0.5f);
                    v.q = bomb_moby->pos.q;
                    send_blast_hits(bomb_moby,
                                    &v,
                                    D_L00_00177F00_2adb30,
                                    n,
                                    0,
                                    0x830000,
                                    2,
                                    1,
                                    2.0f,
                                    1.0f,
                                    1.0f);
                    bomb_moby->state = 2;
                }
                {
                    int k;
                    for (k = 0; k < 10; k++) {
                        Vec_2adb30 t;
                        float      dp;
                        t.q = 0;
                        t.f[0] = random_float_between(-1.0f, 1.0f);
                        t.f[1] = random_float_between(-1.0f, 1.0f);
                        v.q = t.q;
                        dp = FUN_001f9ab0_2adb30(&v, &b);
                        FUN_001f9bf8_2adb30(&proj, &b, dp);
                        FUN_001f9a28_2adb30(&v, &v, &proj);
                        FUN_001f9bf8_2adb30(&proj, &b, random_float_between(0.0f, 1.0f));
                        fast_vec_add(&v, &v, &proj);
                        FUN_001f9bf8_2adb30(
                            &v, &v, s * random_float_between(3.5f, 6.5f) * frame_time_macro);
                        fast_vec_add(&v, &v, &blast_velocity);
                        FUN_L00_002af450_2adb30((char *)&bomb_moby->pos,
                                                (char *)&v,
                                                random_int_between(FUN_001f96f8_2adb30(0x3C),
                                                                   FUN_001f96f8_2adb30(0x78)),
                                                0,
                                                (unsigned char)D_E910.p0[0xA]);
                    }
                }
                if (d->h68 < FUN_001f96f8_2adb30(0x14)) {
                    Vec_2adb30 t;
                    float      g;
                    int        r2;
                    int        g2;
                    int        b2;
                    int        blast_effect_count;
                    {
                        int k;
                        for (k = 0; k < 4; k++) {
                            float dp;
                            t.q = 0;
                            t.f[0] = random_float_between(-1.0f, 1.0f);
                            t.f[1] = random_float_between(-1.0f, 1.0f);
                            v.q = t.q;
                            dp = FUN_001f9ab0_2adb30(&v, &b);
                            FUN_001f9bf8_2adb30(&proj, &b, dp);
                            FUN_001f9a28_2adb30(&v, &v, &proj);
                            FUN_001f9bf8_2adb30(&proj, &b, random_float_between(0.0f, 1.0f));
                            fast_vec_add(&v, &v, &proj);
                            FUN_001f9bf8_2adb30(
                                &v, &v, s * random_float_between(6.5f, 10.0f) * frame_time_macro);
                            fast_vec_add(&v, &v, &blast_velocity);
                            FUN_L00_002af450_2adb30((char *)&bomb_moby->pos,
                                                    (char *)&v,
                                                    random_int_between(FUN_001f96f8_2adb30(0x3C),
                                                                       FUN_001f96f8_2adb30(0x5A)),
                                                    1,
                                                    (unsigned char)D_E910.p0[0xA]);
                        }
                    }
                    t.q = 0;
                    t.f[0] = random_float_between(-1.0f, 1.0f);
                    t.f[1] = random_float_between(-1.0f, 1.0f);
                    t.f[2] = random_float_between(-1.0f, 1.0f);
                    v.q = t.q;
                    delta.f[2] += camera_distance * 0.5f;
                    FUN_001f9bf8_2adb30(&v, &v, camera_distance / 5.0f * frame_time_macro);
                    FUN_001f9bf8_2adb30(&delta, &delta, camera_distance * 2.0f * frame_time_macro);
                    fast_vec_add(&v, &v, &delta);
                    clamp_vector_length(&v, &v, frame_time_macro * 10.0f);
                    FUN_L00_002af450_2adb30(
                        (char *)&bomb_moby->pos,
                        (char *)&v,
                        random_int_between(FUN_001f96f8_2adb30(0x3C), FUN_001f96f8_2adb30(0x5A)),
                        1,
                        (unsigned char)D_E910.p0[0xA]);
                    if (camera_distance < 6.0f) {
                        blast_effect_count = truncate_float_to_s32(camera_distance) + 1;
                    } else {
                        blast_effect_count = 4;
                    }
                    g = 0.0f;
                    if (camera_distance < 7.0f) {
                        g = 7.0f - camera_distance;
                    }
                    for (i = 0; i < blast_effect_count; i++) {
                        float spd = s * random_float_between(8.0f, 10.0f) * frame_time_macro -
                                    g * frame_time_macro;
                        Colors6 c1 = l00_bomb_blast_effect_primary_palette;
                        Colors6 blast_effect_secondary_palette =
                            l00_bomb_blast_effect_secondary_palette;
                        FUN_L00_0026a9f0_2adb30(
                            &bomb_moby->pos,
                            &blast_velocity,
                            swap_packed_color_channels(c1.c[random_integer_below(6)],
                                                       (unsigned char)D_E910.p0[0xA]),
                            swap_packed_color_channels(
                                blast_effect_secondary_palette.c[random_integer_below(6)],
                                (unsigned char)D_E910.p0[0xA]),
                            s * 400000.0f,
                            random_int_between(FUN_001f96f8_2adb30(0xF), FUN_001f96f8_2adb30(0x14)),
                            random_int_between(FUN_001f96f8_2adb30(0x19),
                                               FUN_001f96f8_2adb30(0x1E)),
                            0,
                            0,
                            spd);
                    }
                    if (camera_distance > 9.0f) {
                        int r;
                        int gg;
                        int bb;
                        spawn_randomly_rotated_colored_moby(bomb_moby,
                                                            &bomb_moby->pos,
                                                            &blast_velocity,
                                                            s * 4.0f,
                                                            FUN_001f96f8_2adb30(0xF),
                                                            0x7F,
                                                            0x7F,
                                                            0x7F,
                                                            0x20);
                        r = 0x7F;
                        gg = 0x20;
                        bb = 0;
                        swap_color_channel_words(&r, &gg, &bb, (unsigned char)D_E910.p0[0xA]);
                        spawn_randomly_rotated_colored_moby(bomb_moby,
                                                            &bomb_moby->pos,
                                                            &blast_velocity,
                                                            s * 4.0f,
                                                            FUN_001f96f8_2adb30(0x18),
                                                            r,
                                                            gg,
                                                            bb,
                                                            0x20);
                    }
                    r2 = 0x7F;
                    g2 = 0x3F;
                    b2 = 0;
                    swap_color_channel_words(&r2, &g2, &b2, (unsigned char)D_E910.p0[0xA]);
                    spawn_randomly_rotated_colored_moby(bomb_moby,
                                                        &bomb_moby->pos,
                                                        &blast_velocity,
                                                        s * 4.0f,
                                                        FUN_001f96f8_2adb30(0x14),
                                                        r2,
                                                        g2,
                                                        b2,
                                                        0x30);
                    r2 = 0x60;
                    g2 = 0x10;
                    b2 = 0;
                    swap_color_channel_words(&r2, &g2, &b2, (unsigned char)D_E910.p0[0xA]);
                    spawn_randomly_rotated_colored_moby(bomb_moby,
                                                        &bomb_moby->pos,
                                                        &blast_velocity,
                                                        s * 3.5f,
                                                        FUN_001f96f8_2adb30(0x1B),
                                                        r2,
                                                        g2,
                                                        b2,
                                                        0x40);
                    r2 = 0x20;
                    g2 = 0;
                    b2 = 0;
                    swap_color_channel_words(&r2, &g2, &b2, (unsigned char)D_E910.p0[0xA]);
                    spawn_randomly_rotated_colored_moby(bomb_moby,
                                                        &bomb_moby->pos,
                                                        &blast_velocity,
                                                        s * 3.0f,
                                                        FUN_001f96f8_2adb30(0x1D),
                                                        r2,
                                                        g2,
                                                        b2,
                                                        0x20);
                }
                if (d->h68 >= FUN_001f96f8_2adb30(0x14)) {
                    float h;
                    bomb_moby->pos.f[2] += 1.0f;
                    h = FUN_002135f0_2adb30((float *)&bomb_moby->pos, &v);
                    bomb_moby->pos.f[2] -= 1.0f;
                    {
                        int i;
                        for (i = 0; i < 150; i++) {
                            Vec_2adb30 u;
                            Vec_2adb30 t;
                            t.q = 0;
                            t.f[0] = random_float_between(-1.0f, 1.0f);
                            t.f[1] = random_float_between(-1.0f, 1.0f);
                            t.f[2] = random_float_between(-0.5f, 2.0f);
                            u.q = t.q;
                            FUN_001f9bf8_2adb30(&u,
                                                &u,
                                                s * random_float_between(frame_time_macro * 4.0f,
                                                                         frame_time_macro * 8.0f));
                            random_integer_below_cf(&bomb_moby->pos,
                                                    &u,
                                                    random_float_between(0.05f, 0.1f) * 210000.0f,
                                                    h);
                        }
                    }
                }
                d->v.f[0] = 0.0f;
                bomb_moby->bBC = 0;
                if (d->h68 >= FUN_001f96f8_2adb30(0x14)) {
                    trooper_play_sound(0x16, 0, (int)bomb_moby, 0);
                } else if ((unsigned char)D_E910.p0[0xA] != 0) {
                    allocate_voice_for_target_entry(1, 0, (int)bomb_moby);
                } else {
                    allocate_voice_for_target_entry(0, 0, (int)bomb_moby);
                }
                if (d->h68 != 0) {
                    float gz;
                    float wz;
                    int   cr = 0x60;
                    int   cg = 0x60;
                    int   cb = 0x7F;
                    int   delay = 0;
                    v.q = bomb_moby->pos.q;
                    v.f[2] += 3.5f;
                    gz = probe_ground_height(&v, 0, 0.5f);
                    wz = allocate_voice_for_target_entry_cf(&v);
                    if (gz != wz && gz - wz < 1.2f) {
                        Vec_2adb30 v2;
                        Vec_2adb30 particle_work_vector;
                        int        i;
                        int        j;
                        for (i = 0; i < 100; i++) {
                            float r;
                            float ang;
                            particle_work_vector.q = 0;
                            particle_work_vector.f[0] = random_float_between(
                                frame_time_macro * -0.75f, frame_time_macro * 0.75f);
                            particle_work_vector.f[1] = random_float_between(
                                frame_time_macro * -0.75f, frame_time_macro * 0.75f);
                            particle_work_vector.f[2] = random_float_between(
                                frame_time_macro * 4.0f, frame_time_macro * 14.0f);
                            v2.q = particle_work_vector.q;
                            r = random_float_between(0.0f, 0.35f);
                            ang = random_angle_radians();
                            particle_work_vector.f[0] = FUN_001f9dc8_2adb30(ang) * r;
                            particle_work_vector.f[1] = FUN_001f9de0_2adb30(ang) * r;
                            particle_work_vector.f[2] = 0.0f;
                            fast_vec_add(
                                &particle_work_vector, &particle_work_vector, &bomb_moby->pos);
                            particle_work_vector.f[2] = gz + 0.5f;
                            v2.f[2] -= r * frame_time_macro * 8.0f;
                            spawn_gravity_floor_burst_particle(&particle_work_vector,
                                                               &v2,
                                                               delay,
                                                               random_float_between(0.25f, 1.0f),
                                                               gz,
                                                               frame_time_sq_macro * 20.0f,
                                                               (0x16 << 24) | (cb << 16) |
                                                                   (cg << 8) | cr,
                                                               (cb << 16) | (cg << 8) | cr,
                                                               1);
                        }
                        for (j = 0; j < 20; j++) {
                            int shrinking_particle_primary_color =
                                mine_blend_color(0x7F << 24,
                                                 (0x7F << 24) | (cb << 16) | (cg << 8) | cr,
                                                 random_float_between(0.25f, 1.0f));
                            int   c1 = mine_blend_color(0,
                                                      ((cb / 2) << 16) | ((cg / 2) << 8) | (cr / 2),
                                                      random_float_between(0.5f, 1.0f));
                            float sz;
                            int   life;
                            particle_work_vector.f[0] = random_float_between(-3.0f, 3.0f);
                            particle_work_vector.f[1] = random_float_between(-3.0f, 3.0f);
                            particle_work_vector.f[2] = random_float_between(
                                frame_time_macro * 4.0f, frame_time_macro * 14.0f);
                            FUN_001f9bf8_2adb30(&particle_work_vector,
                                                &particle_work_vector,
                                                random_float_between(1.5f, 3.0f) *
                                                    frame_time_macro);
                            particle_work_vector.f[2] =
                                random_float_between(3.0f, 6.0f) * frame_time_macro;
                            v2.q = bomb_moby->pos.q;
                            v2.f[2] = gz;
                            sz = random_float_between(31500.002f, 52500.0f);
                            life = random_int_between(FUN_001f96f8_2adb30(0x5A),
                                                      FUN_001f96f8_2adb30(0x96));
                            spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
                                &v2,
                                &particle_work_vector,
                                shrinking_particle_primary_color,
                                c1,
                                sz,
                                life,
                                1,
                                D_L00_001B2080_2adb30[11][0],
                                -1);
                        }
                    }
                }
                l00_camera.shake_amount = camera_distance < 20.0f ? 0.4f - camera_distance * 0.0175f
                                                                  : 0.4f - 20.0f * 0.0175f;
                l00_camera.shake_frames = FUN_001f96f8_2adb30(0x19);
                fast_vec_dist(&bomb_moby->pos, D_G910.pos);
                mine_camera_blast((char *)&rec, (char *)&bomb_moby->pos, 0, 0);
                if (bomb_moby->state != 2) {
                    mark_moby_for_removal(bomb_moby);
                    return;
                }
            }
        }
    } else if (bomb_moby->state != 2) {
        int v;
        d->h6a++;
        v = truncate_float_to_s32(FUN_001f9de0_2adb30(normalize_angle((float)d->h6a * 0.068f)) *
                                  96.0f) +
            0x9F;
        bomb_moby->color = (v << 16) | (0xFF << 24) | (v << 8) | v;
    }
    if (bomb_moby->state == 2) {
        Vec_2adb30 p;
        float      r;
        int        n;
        bomb_moby->flags |= 0x41;
        r = interpolate_scalar_linear(
            0.5f,
            (convert_integer_to_float(*((unsigned char *)&D_E910 + 0xA)) + 1.0f) * 3.0f - 0.5f,
            convert_integer_to_float(bomb_moby->bBC) / (float)FUN_001f96f8_2adb30(0xF));
        n = collect_mobies_in_sphere(&bomb_moby->pos, 0x10, bomb_moby, 0, r);
        p.q = bomb_moby->pos.q;
        send_blast_hits(
            bomb_moby, &p, D_L00_00177F00_2adb30, n, 0, 0x830000, 2, 1, 2.0f, 1.0f, 1.0f);
        if (++bomb_moby->bBC > FUN_001f96f8_2adb30(0xF)) {
            mark_moby_for_removal(bomb_moby);
        }
    }
}

extern float D_0015ED6C_2af450 __asm__("D_0015ED6C");
void         FUN_L00_00250df8_2af450(void *) __asm__("FUN_L00_00250df8");
unsigned char *
spawn_debris(void *pos, void *vel, int n, int flag, int g) __asm__("FUN_L00_002af450");
unsigned char *spawn_debris(void *spawn_position,
                            void *initial_velocity,
                            int   lifetime_frames,
                            int   flag,

                            int trail_color_mask) {
    unsigned char *debris_moby = create_moby(0x7A);
    if (debris_moby) {
        unsigned char *debris_data;
        float          rotation_x_increment, frame_time;
        debris_data = ((unsigned char *)MOBY(debris_moby)->pvars);
        debris_moby[0x30] = 0xFF;
        MOBY(debris_moby)->draw_dist = 0xFF;
        debris_moby[0x31] = 1;
        debris_moby[0x23] = 0x80;
        set_moby_color(debris_moby, 0x7F, 0x7F, 0x7F);
        qcopy(debris_moby + 0x10, spawn_position);
        qcopy(debris_data, initial_velocity);
        rotation_x_increment =
            random_float_between(D_0015ED6C_2af450 * 6.2831855f, D_0015ED6C_2af450 * 12.566371f);
        frame_time = D_0015ED6C_2af450;
        *(float *)(debris_data + 0x10) = rotation_x_increment;
        *(float *)(debris_data + 0x14) =
            random_float_between(frame_time * 6.2831855f, frame_time * 12.566371f);
        *(short *)(debris_data + 0x18) = lifetime_frames;
        *(short *)(debris_data + 0x1A) = lifetime_frames;
        if (flag == 0) {
            MOBY(debris_moby)->scale *= random_float_between(0.5f, 0.75f);
        }
        *(float *)(debris_data + 0x1C) = MOBY(debris_moby)->scale;
        debris_moby[0xBC] = flag | (trail_color_mask << 1);
        FUN_L00_00250df8_2af450(debris_moby);
    }
    return debris_moby;
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
void         fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
int          FUN_L00_0025c0e8_c(int, int) __asm__("FUN_L00_0025c0e8");
void         spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
void         debris_update(unsigned char *m) __asm__("FUN_L00_002af5b8");
extern C_2af5b8 D_L00_00161438 NOT_SDA;

void debris_update(unsigned char *debris_moby) {
    W_2af5b8 *debris_data = ((W_2af5b8 *)MOBY(debris_moby)->pvars);
    C_2af5b8  c;
    V_2af5b8  emission_direction;
    V_2af5b8  random_direction;
    int       i;
    float     f;
    if (debris_moby[0xBC] & 1) {
        for (i = 0; i < 1; i++) {
            c = D_L00_00161438;
            *(OvlQuad *)&random_direction = 0;
            random_direction.f[0] = random_float_between(-1.0f, 1.0f);
            random_direction.f[1] = random_float_between(-1.0f, 1.0f);
            random_direction.f[2] = random_float_between(-1.0f, 1.0f);
            *(OvlQuad *)&emission_direction = *(OvlQuad *)&random_direction;
            fast_vec_normalize(&emission_direction, &emission_direction, D_0015ED6C_2af5b8);
            if (debris_moby[0xBC] & 0xFE) {
                int adjusted_color =
                    FUN_L00_0025c0e8_c(c.c[random_integer_below(4)], debris_moby[0xBC] >> 1);
                int adjusted_secondary_color = FUN_L00_0025c0e8_c(0x4FFF, debris_moby[0xBC] >> 1);
                spawn_effect(debris_moby + 0x10,
                             &emission_direction,
                             adjusted_color,
                             adjusted_secondary_color,
                             scale_game_frames(0x2D),
                             0x32,
                             0x78,
                             1);
            } else {
                spawn_effect(debris_moby + 0x10,
                             &emission_direction,
                             c.c[random_integer_below(4)],
                             0x4FFF,
                             scale_game_frames(0x2D),
                             0x32,
                             0x78,
                             1);
            }
        }
    }
    MOBY(debris_moby)->rot.x = fast_add_rotations(MOBY(debris_moby)->rot.x, debris_data->x10);
    MOBY(debris_moby)->rot.y = fast_add_rotations(MOBY(debris_moby)->rot.y, debris_data->x14);
    fast_vec_add(debris_moby + 0x10, debris_moby + 0x10, debris_data);
    debris_data->x8 -= D_0015ED70_2af5b8 * 14.6f;
    if (MOBY(debris_moby)->pos.x < 0.0f || MOBY(debris_moby)->pos.y < 0.0f ||
        MOBY(debris_moby)->pos.z < 0.0f) {
        mark_moby_for_removal(debris_moby);
        return;
    }
    if (debris_data->x18 < debris_data->x1a / 4) {
        f = convert_integer_to_float(debris_data->x18);
        MOBY(debris_moby)->scale =
            debris_data->x1c * f / convert_integer_to_float(debris_data->x1a / 4);
    }
    if (fast_dec_timer_s16(&debris_data->x18))
        mark_moby_for_removal(debris_moby);
}

extern float D_0015ED6C_2af8b0 __asm__("D_0015ED6C");

typedef struct {
    void         *o;
    float         f4, f8;
    short         hc, he;
    unsigned char p10[0x10];
    int           i20;
    float         f24, f28;
} V_2af8b0;

int            FUN_001f96f8_2af8b0(int) __asm__("FUN_001f96f8");
float          randf_sym(float, float) __asm__("FUN_L00_00257c48");
void           FUN_L00_00250df8_2af8b0(void *) __asm__("FUN_L00_00250df8");
unsigned char *spawn_crate_debris(unsigned char *owner,
                                  void          *vel,
                                  void          *pos,
                                  void          *rot,
                                  int            type,
                                  int            n,
                                  float          lo,
                                  float          hi) __asm__("FUN_L00_002af8b0");
extern float   D_L00_00161448_gp __asm__("D_L00_00161448") __attribute__((sda));
extern float   D_L00_0016144C_gp __asm__("D_L00_0016144C") __attribute__((sda));
unsigned char *spawn_crate_debris(unsigned char *owner,
                                  void          *initial_velocity,
                                  void          *spawn_position,
                                  void          *initial_rotation,
                                  int            type,
                                  int            initial_bounce_count,
                                  float          lo,

                                  float hi) {
    unsigned char *debris_moby = create_moby(type);
    if (debris_moby) {
        V_2af8b0 *debris_data = ((V_2af8b0 *)MOBY(debris_moby)->pvars);
        float     scale_multiplier;
        debris_data->o = owner;
        debris_moby[0x30] = 0xFF;
        MOBY(debris_moby)->draw_dist = 0x7E;
        debris_moby[0x31] = 1;
        MOBY(debris_moby)->spawn_frame = *(long *)(owner + 0x38);
        debris_moby[0x20] = 0;
        MOBY(debris_moby)->rot.x = random_angle_radians();
        MOBY(debris_moby)->rot.y = random_angle_radians();
        MOBY(debris_moby)->rot.z = random_angle_radians();
        qcopy(debris_moby + 0x10, spawn_position);
        qcopy(debris_moby + 0x40, initial_rotation);
        qcopy(debris_data->p10, initial_velocity);
        debris_data->f4 = MOBY(debris_moby)->pos.z;
        {
            short t = FUN_001f96f8_2af8b0(0x14);
            debris_data->i20 = 0;
            debris_data->he = t;
        }
        debris_data->f24 =
            randf_sym(D_L00_00161448_gp, D_L00_0016144C_gp) * DEG_TO_RAD * D_0015ED6C_2af8b0;
        debris_data->f28 =
            randf_sym(D_L00_00161448_gp, D_L00_0016144C_gp) * DEG_TO_RAD * D_0015ED6C_2af8b0;
        scale_multiplier = random_float_between(lo, hi);
        MOBY(debris_moby)->scale *= scale_multiplier;
        debris_data->hc = initial_bounce_count;
        debris_data->f8 = scale_multiplier * 0.25f;
        FUN_L00_00250df8_2af8b0(debris_moby);
    }
    return debris_moby;
}

#include "eetypes.h"
#include "qcopy.h"

typedef struct {
    u8  pad0[4];
    f32 f4;
    f32 f8;
    s16 hC;
    s16 hE;
    f32 vel[4] __attribute__((aligned(16)));
    f32 f20, f24, f28;
} PV;

typedef struct {
    u8  pad0[0x10];
    f32 pos[4] __attribute__((aligned(16)));
    u8  b20, b21, b22, b23;
    u8  pad24[8];
    f32 f2c;
    u8  pad30[0x10];
    f32 f40, f44, f48;
    u8  pad4c[0x2C];
    PV *pv;
} M_2afa48;

extern f32  D_L00_00161470_gp __asm__("D_L00_00161470") __attribute__((sda));
extern s32  D_L00_00161474_gp __asm__("D_L00_00161474") __attribute__((sda));
extern s32  coll_sphere(void *, s32, s32, f32) __asm__("FUN_L00_001f0d60");
extern f32  fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern void vscl_2afa48(void *, void *, f32) __asm__("FUN_001f9a68");
extern f32  randf_sym(f32, f32) __asm__("FUN_L00_00257c48");
void        nanotech_crate_fragment_update(M_2afa48 *m) __asm__("FUN_L00_002afa48");

void nanotech_crate_fragment_update(M_2afa48 *fragment_moby) {
    enum {
        NANOTECH_CRATE_FRAGMENT_BOUNCING = 0,
        NANOTECH_CRATE_FRAGMENT_FADING = 1
    };
    PV   *pv = fragment_moby->pv;
    u128  old;
    u128 *hit_normal;
    qcopy(&old, fragment_moby->pos);
    pv->vel[2] -= frame_time_sq * 20.0f;
    fast_vec_add(fragment_moby->pos, fragment_moby->pos, pv->vel);
    fragment_moby->f40 = fast_add_rotations(fragment_moby->f40, pv->f20);
    fragment_moby->f44 = fast_add_rotations(fragment_moby->f44, pv->f24);
    fragment_moby->f48 = fast_add_rotations(fragment_moby->f48, pv->f28);
    switch (fragment_moby->b20) {
    case NANOTECH_CRATE_FRAGMENT_BOUNCING:
        if (fragment_moby->pos[2] < pv->f4 - 3.0f) {
            fragment_moby->b20 = 1;
        } else if (fast_dec_timer_s16(&pv->hE) && pv->hC != 0 &&
                   coll_sphere(fragment_moby->pos, 2, 0, pv->f8)) {
            hit_normal = (u128 *)&coll_hit_normal;
            if (fast_vec_dot(pv->vel, hit_normal) < 0.0f) {
                qcopy(fragment_moby->pos, hit_normal - 1);
                pv->hC--;
                reflect_vector(pv->vel, pv->vel, hit_normal);
                vscl_2afa48(pv->vel, pv->vel, 0.75f);
                pv->f20 = 0.0f;
                pv->f24 = randf_sym(D_L00_00161448_gp, D_L00_0016144C_gp) * DEG_TO_RAD * frame_time;
                pv->f28 = randf_sym(D_L00_00161448_gp, D_L00_0016144C_gp) * DEG_TO_RAD * frame_time;
            }
        }
        break;
    case NANOTECH_CRATE_FRAGMENT_FADING:
        fragment_moby->f2c *= D_L00_00161470_gp;
        if (fragment_moby->b23 < D_L00_00161474_gp) {
            mark_moby_for_removal(fragment_moby);
        } else {
            fragment_moby->b23 -= D_L00_00161474_gp;
        }
        break;
    }
}

typedef union {
    OvlQuad q;
    float   f[4];
} V_2afc70;

extern float         D_0015ED6C_2afc70 __asm__("D_0015ED6C");
extern V_2afc70      hero_pos __asm__("D_0013F3D0") NOT_SDA;
extern unsigned char D_0013F490_lui[] __asm__("D_0013F490") NOT_SDA;
extern void         *D_001413D0_lui __asm__("D_001413D0") NOT_SDA;
extern unsigned char D_L00_00173E60_2afc70[] __asm__("D_L00_00173E60")
    __attribute__((section(".data")));
extern s16   D_L00_001B0770_lui[] __asm__("D_L00_001B0770") NOT_SDA;
extern int   FUN_001f96f8_2afc70(int) __asm__("FUN_001f96f8");
extern float FUN_001f9af0_2afc70(void *) __asm__("FUN_001f9af0");
extern int   FUN_001efa68_2afc70(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern void  FUN_L00_00250df8_2afc70(void *) __asm__("FUN_L00_00250df8");
extern void  append_unique_moby_index(void *, void *, int) __asm__("FUN_L00_0025f780");

unsigned char *spawn_devastator_missile(unsigned char *owner,
                                        V_2afc70      *spawn_position,
                                        unsigned char *target_moby,
                                        float          guidance_yaw,
                                        float          guidance_pitch,
                                        float          target_height_offset,
                                        float          initial_yaw,
                                        float          initial_pitch) __asm__("FUN_L00_002afc70");

unsigned char *spawn_devastator_missile(unsigned char *owner,
                                        V_2afc70      *spawn_position,
                                        unsigned char *target_moby,
                                        float          guidance_yaw,
                                        float          guidance_pitch,
                                        float          target_height_offset,
                                        float          initial_yaw,

                                        float initial_pitch) {
    V_2afc70       collision_start;
    unsigned char *missile_moby;
    unsigned char *missile_data;
    unsigned char *target_aim_record;
    float          default_desired_speed;
    int            turn_period;
    float          x;

    missile_moby = create_moby(0x99);
    if (missile_moby != 0) {
        missile_data = ((unsigned char *)MOBY(missile_moby)->pvars);
        missile_moby[0x30] = 0xFF;
        MOBY(missile_moby)->draw_dist = 0xFF;
        missile_moby[0x31] = 1;
        MOBY(missile_moby)->rot.z = initial_yaw;
        MOBY(missile_moby)->rot.y = initial_pitch;
        missile_moby[0x20] = 0;
        *(unsigned char **)(missile_data + 0x10) = owner;
        qcopy(missile_moby + 0x10, spawn_position);
        default_desired_speed = D_0015ED6C_2afc70 * 20.0f;
        *(int *)(missile_data + 0x38) = 1;
        *(float *)(missile_data + 0x48) = 70.0f;
        *(float *)(missile_data + 0x34) = default_desired_speed;
        *(int *)(missile_data + 0x24) = 0;
        *(int *)(missile_data + 0x28) = 0;
        *(unsigned char **)(missile_data + 0x18) = target_moby;
        *(float *)(missile_data + 0x20) = default_desired_speed / 3.0f;
        if (target_moby != 0) {
            target_aim_record = suck_get_aim_height(target_moby);
            if (target_aim_record != 0) {
                *(unsigned short *)(target_aim_record + 0x1E) |= 0x80;
                if (target_aim_record[0xB]) {
                    *(int *)(missile_data + 0x38) = 0;
                }
                if (target_aim_record[0xC]) {
                    *(float *)(missile_data + 0x34) =
                        (float)target_aim_record[0xC] * D_0015ED6C_2afc70;
                }
                if (D_0015ED6C_2afc70 * 20.0f < *(float *)(missile_data + 0x34)) {
                    *(float *)(missile_data + 0x48) = 140.0f;
                }
            }
        }
        *(float *)(missile_data + 0x2C) = guidance_yaw;
        *(float *)(missile_data + 0x30) = guidance_pitch;
        *(int *)(missile_data + 0x14) = FUN_001f96f8_2afc70(300);
        if (target_moby != 0) {
            qcopy(missile_data, target_moby + 0x10);
            *(float *)(missile_data + 0x1C) = target_height_offset;
            *(float *)(missile_data + 8) += target_height_offset;
        }
        x = FUN_001f9af0_2afc70(D_0013F490_lui);
        turn_period = FUN_001f96f8_2afc70(D_0015ED6C_2afc70 * 0.1f < x ? 0x3C : 0xF);
        *(short *)(missile_data + 0x3E) = turn_period;
        *(short *)(missile_data + 0x3C) = turn_period;
        if (owner != 0 && owner[0x20] != 0xFE && owner[0x20] != 0xFD) {
            collision_start.q = ((V_2afc70 *)(owner + 0x10))->q;
        } else {
            qcopy(&collision_start, &hero_pos);
            collision_start.f[2] += 0.6f;
        }
        collision_start.f[2] = spawn_position->f[2];
        if (FUN_001efa68_2afc70(&collision_start, spawn_position, 0, D_001413D0_lui, 0)) {
            missile_moby[0xBC] = 1;
            qcopy(missile_moby + 0x10, D_L00_00173E60_2afc70);
        }
        FUN_L00_00250df8_2afc70(missile_moby);
        append_unique_moby_index(missile_moby, D_L00_001B0770_lui, 0x1F);
        MOBY(missile_moby)->scale += MOBY(missile_moby)->scale;
    }
    return missile_moby;
}

/* Sums n steps of a value easing toward 20 * D_0015ED6C by a tenth (times D_0015ED64) per step. */
float calculate_eased_step_sum(int step_count) __asm__("FUN_L00_002aff50");

float calculate_eased_step_sum(int step_count) {
    float sum = 0.0f, eased_value = sum;
    if (step_count > 0) {
        float target_value = frame_time * 20.0f;
        do {
            eased_value += (target_value - eased_value) / 10.0f * frame_scale_sq;
            step_count--;
            sum += eased_value;
        } while (step_count);
    }
    return sum;
}

typedef int ti_t __attribute__((mode(TI), aligned(16)));

typedef union {
    ti_t q;
    f32  f[4];
} VU;

typedef struct {
    VU    v;
    s32   w10;
    s32   w14;
    char *owner;
    f32   f1C;
    s32   w20;
    s32   w24;
    s32   w28;
    f32   f2C;
    f32   f30;
    s32   w34;
    s32   w38;
} O;

typedef struct {
    char p0[0xB];
    u8   bB;
} E_u;

extern char *active_mobies[] __asm__("D_L00_001ABA00") NOT_SDA;
extern E_u   D_0013E520_u __asm__("D_0013E520");
extern s32   D_L00_00166DC0[] NOT_SDA;
extern s32   D_L00_00173E40[0x1C] NOT_SDA;

extern f32  FUN_001f99c0(f32);
extern void FUN_00214db0(void *, f32, f32, f32);
extern s32  FUN_001efa68_u(void *, void *, s32, char *, void *) __asm__("FUN_001efa68");
extern s32  find_listed_moby_by_data_id(char *) __asm__("FUN_L00_0025f878");

void select_gold_devastator_missile_target(char *self,
                                           O    *out,
                                           char *skip) __asm__("FUN_L00_002affa8");

void select_gold_devastator_missile_target(char *self, O *out, char *skip) {
    VU    candidate_aim_position;
    VU    lv;
    char *candidate_moby;
    s32   candidate_aim_record_address;
    s32   i;
    s32   flag;
    f32 best, yaw, pitch, candidate_aim_distance, aim_angle_limit_or_asin_numerator, z, angh, angv;
    char        *spos;
    char        *opos;
    struct Hero *hero_context;

    i = 0;
    best = 10000.0f;
    yaw = *(f32 *)(self + 0x48);
    pitch = *(f32 *)(self + 0x44);
    candidate_moby = active_mobies[i];
    if (candidate_moby == 0)
        return;
    hero_context = &hero;
    while (candidate_moby != 0) {
        if (((s8)MOBY(candidate_moby)->state) < 0)
            goto next;
        if (candidate_moby == skip)
            goto next;
        qcopy(&candidate_aim_position, candidate_moby + 0x10);
        spos = self + 0x10;
        opos = candidate_moby + 0x10;
        if (40.0f < fast_vec_dist(&candidate_aim_position, spos))
            goto next;
        candidate_aim_record_address = suck_get_aim_height(candidate_moby);
        if (candidate_aim_record_address == 0)
            goto np;
        if (candidate_aim_record_address <= 0x100000)
            goto np;
        if (0x3FFFFF < candidate_aim_record_address)
            goto np;
        {
            f32 pf = *(f32 *)(candidate_aim_record_address + 0x10);
            z = candidate_aim_position.f[2] + pf;
        }
        goto zset;
    np:
        candidate_aim_record_address = 0;
        z = candidate_aim_position.f[2] + 0.5f;
    zset:
        candidate_aim_position.f[2] = z;
        if (candidate_moby == 0)
            goto next;
        if (((char *)MOBY(candidate_moby)->pclass) == 0)
            goto next;
        if (*(s16 *)(((char *)MOBY(candidate_moby)->pclass) + 0x46) != 5)
            goto next;
        angh = fast_arc_tan(candidate_aim_position.f[0] - *(f32 *)(self + 0x10),
                            candidate_aim_position.f[1] - *(f32 *)(self + 0x14));
        angv = fast_arc_tan(vector_distance_xy(spos, &candidate_aim_position),
                            candidate_aim_position.f[2] - *(f32 *)(self + 0x18));
        candidate_aim_distance = fast_vec_dist(spos, &candidate_aim_position);
        if (candidate_aim_distance < 2.5f) {
            char *hero_moby = *(char **)&hero_context->moby;
            if (fast_difference_between_rotations(
                    MOBY(hero_moby)->rot.z,
                    fast_arc_tan(MOBY(candidate_moby)->pos.x - hero_context->motion.pos.f[0],
                                 MOBY(candidate_moby)->pos.y - hero_context->motion.pos.f[1])) <
                1.0471976f) {
                if (FUN_001f99c0(angv) < 0.7853982f)
                    return;
            }
        }
        if (best < candidate_aim_distance)
            goto next;
        aim_angle_limit_or_asin_numerator = convert_integer_to_float(D_0013E520_u.bB) * 0.6981317f;
        FUN_00214db0(&lv, candidate_aim_distance, yaw, pitch + hero_context->ground_pitch.f * 0.5f);
        fast_vec_add(&lv, &lv, spos);
        z = fast_arc_sin(fast_vec_dist(&lv, &candidate_aim_position) /
                         (candidate_aim_distance + candidate_aim_distance));
        aim_angle_limit_or_asin_numerator = aim_angle_limit_or_asin_numerator + 1.5707964f;
        if (aim_angle_limit_or_asin_numerator < z + z && candidate_aim_record_address != 0) {
            aim_angle_limit_or_asin_numerator =
                convert_integer_to_float(*(u8 *)(candidate_aim_record_address + 0xA) << 3) * 0.125f;
            fast_arc_sin(aim_angle_limit_or_asin_numerator / candidate_aim_distance);
        }
        flag = 1;
        if (FUN_001efa68_u(D_L00_00166DC0, &candidate_aim_position, 6, self, 0) != 0) {
            s32 hit = D_L00_00173E40[6];
            if (hit == 0 || hit != (s32)candidate_moby)
                flag = 0;
        } else {
            best = candidate_aim_distance;
            pitch = -angv;
            yaw = angh;
            if (find_listed_moby_by_data_id(candidate_moby) != 0)
                goto next;
            z = 0.5f;
            out->owner = candidate_moby;
            if (candidate_aim_record_address != 0)
                z = *(f32 *)(candidate_aim_record_address + 0x10);
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
        candidate_moby = active_mobies[i];
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002AB910.c: func_L00_002B1688), where it is exact; names translated to the US level program. */

typedef u32 VQ1688 __attribute__((mode(TI), aligned(16)));

typedef union {
    VQ1688 q;
    f32    f[4];
    s32    i[4];
} V1688;

typedef int Q1688 __attribute__((mode(TI)));

typedef struct {
    V1688 previous_target;
    M910 *owner;
    int   life;
    M910 *target;
    float target_height;
    float speed;
    float yaw_velocity;
    float pitch_velocity;
    float yaw;
    float pitch;
    float desired_speed;
    int   track_velocity;
    short turn_time;
    short turn_period;
    int   bounce_count;
    int   bounce_timer;
    float limit;
} Missile1688;

typedef struct {
    int colors[6];
} Colors1688;

typedef union {
    Qry910 collision;
    struct {
        V1688 direction;
        V1688 relative;
        V1688 target;
    } vectors;
} MissilePhase1688;

typedef struct {
    char  pad0[0x100];
    V1688 previous;
    char  pad110[0x30];
    V1688 current;
} HeroMotion1688;

typedef struct {
    char  pad0[0x18];
    M910 *moby;
    int   kind;
    V1688 position;
    char  pad30[0x10];
    V1688 normal, velocity_end, velocity_start;
} Impact1688;

extern B910       l00_devastator_missile_camera_blast_parameters __asm__("D_L00_001E9950");
extern B910       l00_gold_devastator_missile_camera_blast_parameters __asm__("D_L00_001E99A0");
extern Colors1688 l00_devastator_missile_impact_color_palette __asm__("D_L00_001E99F0");
extern Colors1688 l00_devastator_missile_impact_fade_palette __asm__("D_L00_001E9A08");
extern P          D_0013F350;
extern V1688 D_L00_00160660          MACRO_ADDR;
extern V1688 D_L00_00160680          MACRO_ADDR;
extern char                         *FUN_L00_002af450(char *, char *, int, int, int);
extern f32                           fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float                         D_0015ED6C;
extern float                         D_0015ED70;
extern float                         D_L00_0015F5D0;
extern float                         D_L00_0015F5D4;
extern int                           add_vectors() __asm__("FUN_L00_001ff290");
extern int                           FUN_L00_001ff2c0();
extern int                           FUN_L00_001ff430();
extern int                           FUN_L00_001ffd48();
extern int                           FUN_L00_001ffe20();
extern int                           FUN_L00_00258110();
unsigned char                       *spawn_glow_particle_u8(void         *owner,
                                                            void         *pos,
                                                            void         *vel,
                                                            float         scale,
                                                            int           lifetime,
                                                            unsigned char red,
                                                            unsigned char green,
                                                            unsigned char blue,
                                                            unsigned char alpha) __asm__("FUN_L00_002ac910");
extern short                         D_L00_001B0770[];
extern unsigned char *D_L00_001B20DC NOT_SDA;
extern void FUN_L00_0025a9f8(void *, void *, void *, int, int, int, int, int, float, float, float);
extern void FUN_L00_0026a9f0(void *, void *, int, int, float, int, int, int, int, float);
extern void build_look_at_matrix(void *out, void *dir, void *axis, f32 ang) __asm__("FUN_00214890");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
void        FUN_L00_0026a4c8(Q1688 *a, Q1688 *b, int c, int d, int n, float f);
void        spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
void spawn_particle(void *, void *, int, int, int, int, float, float, float, float, float) __asm__("FUN_L00_002712b8");
extern Impact1688     impact1688 __asm__("D_L00_00173E40");
extern HeroMotion1688 hero_motion1688 __asm__("D_0013F350");
extern float          FUN_L00_001ff430_c(void *) __asm__("FUN_L00_001ff430");
extern void           FUN_L00_001ff2c0_c(void *, void *, void *) __asm__("FUN_L00_001ff2c0");
extern float spring_turn(void *, float, float, float, float, float) __asm__("FUN_L00_00258110");
extern unsigned char *
smoke1688(void *, void *, int, int, int, int, float, float, float, float, float) __asm__("FUN_L00_002712b8");
extern void build_look_at_matrix(void *, void *, void *, float) __asm__("FUN_00214890");
extern void spawn_burst(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern void FUN_L00_001ff290_c(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern int  collision1688(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void reflect_vector(void *, void *, void *) __asm__("FUN_L00_001ff660");
extern void FUN_L00_001ffd48_c(void *, void *, float) __asm__("FUN_L00_001ffd48");
extern void FUN_L00_001ffe20_c(void *, void *, void *) __asm__("FUN_L00_001ffe20");
extern void FUN_L00_0026a9f0(void *, void *, int, int, float, int, int, int, int, float);
extern void FUN_L00_0026a4c8_c(void *, void *, int, int, float, int) __asm__("FUN_L00_0026a4c8");

void gold_devastator_missile_update(M910 *m) __asm__("FUN_L00_002b03a0");

void gold_devastator_missile_update(M910 *missile_moby) {
    B910             effect = l00_devastator_missile_camera_blast_parameters;
    B910             gold_effect = l00_gold_devastator_missile_camera_blast_parameters;
    V1688            camera_delta;
    V1688            previous;
    V1688            blast_velocity;
    V1688            normal;
    V1688            smoke_position;
    V1688            motion;
    V1688            surface_motion;
    V1688            hero_velocity;
    MissilePhase1688 phase;
    V1688            predicted;
    float            basis[4][4];
    float            basis_squared[4][4];
    float            rotation[4][4];
    float            diagonal[4][4];
    float            rotation_part[4][4];
    M910            *skip = 0;
    int              reduced = 0;
    Missile1688     *missile_data;
    M910            *target;
    float            size, yaw, pitch;
    normal.q = 0;
    normal.f[2] = D_0015ED6C * 8.0f;
    blast_velocity.q = normal.q;
    normal.q = 0;
    normal.f[2] = 1.0f;
    missile_data = (Missile1688 *)missile_moby->v;
    target = missile_data->target;
    if (D_L00_0015F5D0 > 0.9f || D_L00_0015F5D4 > 0.9f)
        reduced = 1;
    missile_data->speed =
        missile_data->speed +
        (missile_data->desired_speed - missile_data->speed) / 10.0f * frame_scale_sq;
    fast_dec_timer_s16(&missile_data->turn_time);
    {
        float turn = convert_integer_to_float(missile_data->turn_time);
        turn = turn / convert_integer_to_float(missile_data->turn_period);
        fast_vec_add(phase.collision.dir, &hero_motion1688.current, &hero_motion1688.previous);
        hero_velocity.q = phase.vectors.direction.q;
        scale_vector_xyz(&hero_velocity, &hero_velocity, turn);
    }
    size = convert_integer_to_float(D_0013E520.bB) + 1.0f;
    if (target && missile_data->life < scale_game_frames(300) - scale_game_frames(5) &&
        (signed char)target->state >= 0) {
        float time = 0.0f;
        qcopy(&phase.vectors.target, &target->pos);
        phase.vectors.target.f[2] += missile_data->target_height;
        fast_vec_sub(phase.collision.dir, &phase.vectors.target, (&missile_moby->pos));
        if (missile_data->track_velocity) {
            float intercept_linear_coefficient, a, root, intercept_time_plus_root,
                intercept_time_minus_root, denominator, negative_b;
            fast_vec_sub(
                &phase.vectors.relative, &phase.vectors.target, &missile_data->previous_target);
            FUN_L00_001ff2c0_c(&predicted, &phase.vectors.relative, &hero_velocity);
            qcopy(&missile_data->previous_target, &phase.vectors.target);
            a = FUN_L00_001ff430_c(&phase.vectors.relative);
            intercept_linear_coefficient = (phase.vectors.relative.f[0] * phase.collision.dir[0] +
                                            phase.vectors.relative.f[1] * phase.collision.dir[1] +
                                            phase.vectors.relative.f[2] * phase.collision.dir[2]) *
                                           -2.0f;
            a = missile_data->speed * missile_data->speed - a;
            root = square_root(intercept_linear_coefficient * intercept_linear_coefficient -
                               a * 4.0f * -FUN_L00_001ff430_c(phase.collision.dir));
            negative_b = -intercept_linear_coefficient;
            denominator = a + a;
            intercept_time_plus_root = (negative_b + root) / denominator;
            intercept_time_minus_root = (negative_b - root) / denominator;
            if (intercept_time_plus_root > 0.0f && intercept_time_minus_root > 0.0f) {
                if (intercept_time_minus_root < intercept_time_plus_root)
                    time = intercept_time_plus_root;
                else
                    time = intercept_time_minus_root;
            } else {
                if (intercept_time_plus_root > 0.0f)
                    time = intercept_time_plus_root;
                else {
                    time = -1.0f;
                    if (intercept_time_minus_root > 0.0f)
                        time = intercept_time_minus_root;
                }
            }
        }
        if (time > 0.0f && time < (float)scale_game_frames(300)) {
            scale_vector_xyz(&predicted, &phase.vectors.relative, time);
            fast_vec_add(&predicted, &predicted, phase.collision.dir);
            yaw = fast_arc_tan(predicted.f[0], predicted.f[1]);
            pitch = -fast_arc_tan(vector_length_xy(&predicted), predicted.f[2]);
        } else {
            yaw = fast_arc_tan(phase.collision.dir[0], phase.collision.dir[1]);
            pitch = -fast_arc_tan(vector_length_xy(phase.collision.dir), phase.collision.dir[2]);
        }
    } else {
        if (missile_data->life < scale_game_frames(300) - scale_game_frames(5)) {
            yaw = missile_data->yaw;
            pitch = missile_data->pitch;
        } else {
            yaw = missile_moby->rz;
            pitch = missile_moby->ry;
        }
    }
    if (target) {
        missile_moby->rz = spring_turn(&missile_data->yaw_velocity,
                                       missile_moby->rz,
                                       yaw,
                                       D_0015ED70 * 6.2831855f,
                                       D_0015ED70 * 3.1415927f,
                                       D_0015ED6C * 6.2831855f);
        missile_moby->ry = spring_turn(&missile_data->pitch_velocity,
                                       missile_moby->ry,
                                       pitch,
                                       D_0015ED70 * 6.2831855f,
                                       D_0015ED70 * 3.1415927f,
                                       D_0015ED6C * 6.2831855f);
    } else {
        missile_moby->rz = spring_turn(&missile_data->yaw_velocity,
                                       missile_moby->rz,
                                       yaw,
                                       D_0015ED70 * 6.2831855f,
                                       D_0015ED70 * 3.1415927f,
                                       D_0015ED6C * 4.71238899f);
        missile_moby->ry = spring_turn(&missile_data->pitch_velocity,
                                       missile_moby->ry,
                                       pitch,
                                       D_0015ED70 * 6.2831855f,
                                       D_0015ED70 * 3.1415927f,
                                       D_0015ED6C * 4.71238899f);
    }
    build_spherical_offset(&motion, missile_data->speed, missile_moby->rz, -missile_moby->ry);
    fast_vec_normalize(&impact1688.normal, &motion, 1.0f);
    scale_vector_xyz(&impact1688.normal, &impact1688.normal, -1.0f);
    if (!reduced || !(D_L00_0015F5CC & 1)) {
        unsigned char *smoke_particle;
        phase.vectors.relative.q = 0;
        phase.vectors.relative.f[0] = random_float_between(-1.0f, 1.0f);
        phase.vectors.relative.f[1] = random_float_between(-1.0f, 1.0f);
        phase.vectors.relative.f[2] = random_float_between(-1.0f, 1.0f);
        phase.vectors.direction.q = phase.vectors.relative.q;
        fast_vec_normalize(phase.collision.dir,
                           phase.collision.dir,
                           random_float_between(0.1f, 0.2f) * D_0015ED6C);
        fast_vec_normalize(
            &smoke_position, &motion, -random_float_between(D_0015ED6C * 0.1f, D_0015ED6C));
        fast_vec_add(phase.collision.dir, phase.collision.dir, &smoke_position);
        fast_vec_normalize(
            &smoke_position, &motion, random_float_between(0.0f, 1.0f) * missile_data->speed);
        fast_vec_add(&smoke_position, &smoke_position, (&missile_moby->pos));
        smoke_particle = smoke1688(&smoke_position,
                                   phase.collision.dir,
                                   scale_game_frames(40),
                                   50,
                                   0x505050,
                                   3,
                                   40000.0f,
                                   1000.0f,
                                   1.0f,
                                   -0.0002f,
                                   0.0f);
        if (smoke_particle) {
            unsigned char texture = *D_L00_001B20DC;
            smoke_particle[3] = 0x44;
            smoke_particle[2] = texture;
        }
        smoke1688((&missile_moby->pos),
                  phase.collision.dir,
                  scale_game_frames(7),
                  127,
                  0xB0B0B0,
                  3,
                  40000.0f,
                  1000.0f,
                  1.0f,
                  -0.0004f,
                  0.0f);
        phase.vectors.relative.q = 0;
        phase.vectors.relative.f[2] = 0.02f;
        transform_vector_by_basis(
            &phase.vectors.relative, &phase.vectors.relative, missile_moby->mtx);
        build_look_at_matrix(&phase.vectors.relative,
                             &phase.vectors.relative,
                             missile_moby->mtx,
                             random_angle_radians());
        {
            int color = swap_packed_color_channels(0x4F007FFF, D_0013E520.bB);
            spawn_burst(&smoke_position,
                        &phase.vectors.relative,
                        color,
                        0x1FFFFFFF,
                        scale_game_frames(15),
                        1,
                        25000.0f);
        }
    }
    qcopy(&previous, (&missile_moby->pos));
    FUN_L00_001ff290_c(phase.collision.dir, &motion, &hero_velocity);
    fast_vec_add((&missile_moby->pos), (&missile_moby->pos), &motion);
    if (missile_moby->pos.f[0] < 0.0f || missile_moby->pos.f[1] < 0.0f ||
        missile_moby->pos.f[2] < 0.0f) {
        mine_spawn_drops(missile_moby, D_L00_001B0770);
        mark_moby_for_removal(missile_moby);
        return;
    }
    phase.collision.moby = missile_moby;
    phase.collision.flags = 0x830000;
    phase.collision.fC = 3.0f;
    phase.collision.i10 = 1;
    qcopy(phase.collision.dir, &motion);
    set_vector_length_xy(phase.collision.dir, phase.collision.dir, 1.0f);
    phase.collision.dir[2] = 1.0f;
    phase.collision.dir[3] = 5627.925f;
    phase.collision.b8 = 3;
    phase.collision.cls = missile_moby->cls;
    phase.collision.b9 = 1;
    if (collision1688(&previous, (&missile_moby->pos), 0, D_0013F350.cam, &phase.collision)) {
        Impact1688 *hit = &impact1688;
        V1688      *hit_normal;
        if (hit->moby) {
            if (hit->moby != (M910 *)D_0013F350.cam && hit->moby != missile_data->owner) {
                missile_moby->bBC = 2;
                qcopy((&missile_moby->pos), &hit->position);
                fast_vec_sub(&surface_motion, &hit->velocity_end, &hit->velocity_start);
                hit_normal = &hit->normal;
                skip = missile_moby;
                fast_vec_normalize(&surface_motion, &surface_motion, 1.0f);
                reflect_vector(&blast_velocity, &motion, hit_normal);
                fast_vec_normalize(&blast_velocity, &blast_velocity, D_0015ED6C + D_0015ED6C);
                fast_vec_normalize(&normal, hit_normal, 1.0f);
            }
        } else if (hit->kind > 0) {
            missile_moby->bBC = 1;
            qcopy((&missile_moby->pos), &hit->position);
            fast_vec_sub(&surface_motion, &hit->velocity_end, &hit->velocity_start);
            hit_normal = &hit->normal;
            fast_vec_normalize(&surface_motion, &surface_motion, 1.0f);
            reflect_vector(&blast_velocity, &motion, hit_normal);
            fast_vec_normalize(&blast_velocity, &blast_velocity, D_0015ED6C + D_0015ED6C);
            fast_vec_normalize(&normal, hit_normal, 1.0f);
        }
    } else if (fast_dec_timer(&missile_data->life) ||
               missile_data->limit < vector_distance_xy((&missile_moby->pos), &D_0013F350.x80)) {
        qcopy(&normal, &D_L00_00160680);
        qcopy(&surface_motion, &D_L00_00160660);
        transform_vector_by_basis(&surface_motion, &surface_motion, missile_moby->mtx);
        transform_vector_by_basis(&normal, &normal, missile_moby->mtx);
        if (!D_0013E520.bB) {
            mine_spawn_drops(missile_moby, D_L00_001B0770);
            mark_moby_for_removal(missile_moby);
            return;
        }
        missile_moby->bBC = 1;
        fast_vec_add(&blast_velocity, &blast_velocity, &motion);
    }
    if (missile_moby->bBC) {
        float distance, near_adjust;
        int   particles;
        int   particle_reduction;
        int   i, j;
        int   hit =
            collect_mobies_in_sphere((&missile_moby->pos), 16, missile_moby, 0, (size + size));
        particles = 10;
        predicted.q = missile_moby->pos.q;
        FUN_L00_0025a9f8(missile_moby,
                         &predicted,
                         D_L00_00177F00,
                         hit,
                         (int)skip,
                         0x830000,
                         3,
                         1,
                         3.0f,
                         1.0f,
                         1.0f);
        particle_reduction = 0;
        basis[0][0] = 0.0f;
        basis[1][0] = -normal.f[2];
        basis[2][0] = normal.f[1];
        basis[3][0] = 0.0f;
        basis[0][1] = normal.f[2];
        basis[1][1] = 0.0f;
        basis[2][1] = -normal.f[0];
        basis[3][1] = 0.0f;
        basis[0][2] = -normal.f[1];
        basis[1][2] = normal.f[0];
        basis[2][2] = 0.0f;
        basis[3][2] = 0.0f;
        basis[0][3] = 0.0f;
        basis[1][3] = 0.0f;
        basis[2][3] = 0.0f;
        basis[3][3] = 0.0f;
        multiply_matrix(basis_squared, basis, basis);
        if (reduced) {
            particle_reduction = 1;
            particles /= 3;
        }
        {
            int remaining;
            for (remaining = 0; remaining < particles; remaining++) {
                if (missile_moby->bBC == 1) {
                    float a = random_angle_radians();
                    FUN_L00_001ffd48_c(rotation_part, basis_squared, 1.0f - fast_cos(a));
                    FUN_L00_001ffd48_c(diagonal, basis, fast_sin(a));
                    FUN_L00_001ffe20_c(diagonal, diagonal, rotation_part);
                    set_identity_matrix(rotation_part);
                    FUN_L00_001ffe20_c(rotation, diagonal, rotation_part);
                    transform_vector(&predicted, &surface_motion, rotation);
                    fast_vec_normalize(&predicted,
                                       &predicted,
                                       random_float_between(8.5f, 16.5f) * D_0015ED6C * size);
                } else {
                    float speed = random_float_between(8.5f, 16.5f) * D_0015ED6C * size;
                    float a = random_angle_radians();
                    build_spherical_offset(&predicted, speed, a, random_angle_radians());
                }
                predicted.f[2] += D_0015ED6C * 5.0f;
                fast_vec_add(&predicted, &predicted, &blast_velocity);
                {
                    int   color = swap_packed_color_channels(0x4F007FFF, D_0013E520.bB);
                    int   fade = swap_packed_color_channels(0x1F00007F, D_0013E520.bB);
                    int   life;
                    float particle_scale;
                    if (fast_vec_length(&predicted) > 1.0f)
                        fast_vec_normalize(&predicted, &predicted, D_0015ED6C * 8.0f);
                    particle_scale = size * 40000.0f;
                    life = FUN_L00_00257b90(scale_game_frames(60), scale_game_frames(120)) -
                           particle_reduction * 35;
                    spawn_shrinking_effect_particle_with_gravity_and_secondary_spawns(
                        (&missile_moby->pos),
                        &predicted,
                        color,
                        fade,
                        particle_scale,
                        life,
                        1,
                        -1,
                        -1);
                }
            }
        }
        {
            V1688 random;
            random.q = 0;
            random.f[0] = random_float_between(-1.0f, 1.0f);
            random.f[1] = random_float_between(-1.0f, 1.0f);
            random.f[2] = random_float_between(-1.0f, 1.0f);
            predicted.q = random.q;
        }
        fast_vec_sub(&camera_delta, D_L00_00166DC0, (&missile_moby->pos));
        distance = fast_vec_length(&camera_delta);
        camera_delta.f[2] += distance * 0.5f;
        fast_vec_normalize(&predicted, &predicted, distance / 5.0f * D_0015ED6C * size);
        fast_vec_normalize(&camera_delta, &camera_delta, (distance + distance) * D_0015ED6C);
        fast_vec_add(&predicted, &predicted, &camera_delta);
        clamp_vector_length(&predicted, &predicted, D_0015ED6C * 10.0f);
        FUN_L00_002af450((char *)(&missile_moby->pos),
                         (char *)&predicted,
                         FUN_L00_00257b90(scale_game_frames(60), scale_game_frames(90)),
                         0,
                         0);
        if (distance < 6.0f)
            j = truncate_float_to_s32(distance) / 2;
        else
            j = 3;
        near_adjust = 0.0f;
        if (distance < 7.0f)
            near_adjust = 7.0f - distance;
        if (reduced)
            j /= 2;
        for (i = 0; i < j; i++) {
            float speed =
                random_float_between(8.0f, 10.0f) * size * D_0015ED6C - near_adjust * D_0015ED6C;
            Colors1688 color = l00_devastator_missile_impact_color_palette;
            Colors1688 fade = l00_devastator_missile_impact_fade_palette;
            int        c =
                swap_packed_color_channels(color.colors[random_integer_below(6)], D_0013E520.bB);
            int f = swap_packed_color_channels(fade.colors[random_integer_below(6)], D_0013E520.bB);
            int life = FUN_L00_00257b90(scale_game_frames(15), scale_game_frames(20)) -
                       particle_reduction * 7;
            int end = FUN_L00_00257b90(scale_game_frames(25), scale_game_frames(30)) -
                      particle_reduction * 10;
            int white_life, white_end;
            FUN_L00_0026a9f0(
                (&missile_moby->pos), &blast_velocity, c, f, 400000.0f, life, end, 0, 0, speed);
            white_life = FUN_L00_00257b90(scale_game_frames(5), scale_game_frames(10)) -
                         particle_reduction * 5;
            white_end = FUN_L00_00257b90(scale_game_frames(15), scale_game_frames(20)) -
                        particle_reduction * 7;
            FUN_L00_0026a9f0((&missile_moby->pos),
                             &blast_velocity,
                             0x7FFFFFFF,
                             0xFFFFFF,
                             400000.0f,
                             white_life,
                             white_end,
                             0,
                             0,
                             speed * 0.5f);
        }
        for (i = 0; i < particles; i++) {
            Colors1688 color = l00_devastator_missile_impact_color_palette;
            Colors1688 fade = l00_devastator_missile_impact_fade_palette;
            V1688      v;
            int        c, f, life;
            float      debris_scale;
            {
                V1688 random;
                random.q = 0;
                random.f[0] = random_float_between(-1.0f, 1.0f);
                random.f[1] = random_float_between(-1.0f, 1.0f);
                random.f[2] = random_float_between(-1.0f, 1.0f);
                v.q = random.q;
            }
            fast_vec_normalize(&v, &v, random_float_between(0.0f, 3.0f) * D_0015ED6C * size);
            c = swap_packed_color_channels(color.colors[random_integer_below(6)], D_0013E520.bB);
            f = swap_packed_color_channels(fade.colors[random_integer_below(6)], D_0013E520.bB);
            debris_scale = size * 200000.0f;
            life = FUN_L00_00257b90(scale_game_frames(20), scale_game_frames(35)) -
                   particle_reduction * 10;
            FUN_L00_0026a4c8_c((&missile_moby->pos), &v, c, f, debris_scale, life);
        }
        if (D_L00_0015F5D0 < 0.95f && distance > 9.0f) {
            int c = 127, f = 127;
            if (D_0013E520.bB) {
                c = 60;
                f = 60;
            }
            spawn_glow_particle_u8(missile_moby,
                                   (&missile_moby->pos),
                                   &blast_velocity,
                                   size * 4.0f,
                                   scale_game_frames(15),
                                   c,
                                   127,
                                   f,
                                   32);
            spawn_glow_particle_u8(missile_moby,
                                   (&missile_moby->pos),
                                   &blast_velocity,
                                   size * 4.0f,
                                   scale_game_frames(24),
                                   c,
                                   127,
                                   f,
                                   32);
        }
        {
            unsigned char color = 127;
            int           life;
            float         flash_scale = size * 4.0f;
            if (D_0013E520.bB)
                color = 32;
            life = scale_game_frames(20);
            spawn_glow_particle_u8(missile_moby,
                                   (&missile_moby->pos),
                                   &blast_velocity,
                                   flash_scale,
                                   life,
                                   color,
                                   127,
                                   0,
                                   48);
        }
        if (!reduced)
            spawn_glow_particle_u8(missile_moby,
                                   (&missile_moby->pos),
                                   &blast_velocity,
                                   (size + size),
                                   scale_game_frames(19),
                                   255,
                                   255,
                                   255,
                                   32);
        motion.f[0] = 0.0f;
        missile_moby->bBC = 0;
        l00_camera.shake_amount =
            distance < 20.0f ? 0.4f - distance * 0.0175f : 0.4f - 20.0f * 0.0175f;
        l00_camera.shake_frames = scale_game_frames(25);
        allocate_voice_for_target_entry(0, 0, (int)missile_moby);
        if (!reduced) {
            if (D_0013E520.bB)
                mine_camera_blast((char *)&gold_effect, (char *)(&missile_moby->pos), 0, 0);
            else
                mine_camera_blast((char *)&effect, (char *)(&missile_moby->pos), 0, 0);
        }
        missile_data->bounce_count++;
        if (!D_0013E520.bB || missile_data->bounce_count == 3) {
            mine_spawn_drops(missile_moby, D_L00_001B0770);
            mark_moby_for_removal(missile_moby);
            return;
        }
        {
            V1688 bounce[2];
            missile_data->target = 0;
            missile_moby->bBC = 0;
            fast_vec_normalize(&impact1688.normal, &impact1688.normal, 1.0f);
            reflect_vector(&bounce[0], &motion, &impact1688.normal);
            missile_moby->rz = fast_arc_tan(bounce[0].f[0], bounce[0].f[1]);
            missile_moby->ry = -fast_arc_tan(vector_length_xy(&bounce[0]), bounce[0].f[2]);
            missile_data->yaw_velocity = 0.0f;
            missile_data->pitch_velocity = 0.0f;
            fast_vec_normalize(&bounce[1], &impact1688.normal, 0.05f);
            fast_vec_add((&missile_moby->pos), (&missile_moby->pos), &bounce[1]);
            if (impact1688.moby)
                missile_data->owner = impact1688.moby;
            if (!missile_data->bounce_timer)
                missile_data->bounce_timer = scale_game_frames(90);
            select_gold_devastator_missile_target(
                (char *)missile_moby, (O *)missile_data, (char *)impact1688.moby);
            allocate_voice_for_target_entry(1, 0, (int)missile_moby);
        }
    }
    if (fast_dec_timer(&missile_data->bounce_timer))
        missile_data->bounce_count = 0;
}

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_2b1af0;

typedef struct {
    V_2b1af0 a;
    char     pad[0x30];
    V_2b1af0 b;
} P_2b1af0;

void fast_subtract_rotations_cf(void *) __asm__("FUN_L00_001fefc8");
void scale_2b1af0(void *, void *, float) __asm__("FUN_001f9a68");
int  random_int_between(int, int) __asm__("FUN_L00_00257b90");
void spawn_particle(void *, void *, int, int, int, int, float, float, float, float, float) __asm__("FUN_L00_002712b8");
extern P_2b1af0 l00_object_offset_particle_velocity_contributions __asm__("D_0013F450");
void emit_object_offset_particles(unsigned char *source_object) __asm__("FUN_L00_002b1af0");

void emit_object_offset_particles(unsigned char *source_object) {
    V_2b1af0 b;
    V_2b1af0 a;
    V_2b1af0 emission_position;
    V_2b1af0 particle_velocity;
    int      i, j;
    int      particle_byte_input_or_min_frames;
    if (source_object[0xBC] == 0)
        return;
    *(OvlQuad *)&a = 0;
    *(OvlQuad *)&b = 0;
    a.f[0] = 0.23f;
    a.f[1] = 0.18f;
    b.f[1] = 1.0f;
    b.f[3] = 1.0f;
    a.f[3] = 1.0f;
    fast_subtract_rotations_cf(source_object + 0xBC);
    transform_vector_by_basis(&a, &a, source_object + 0xC0);
    fast_vec_add(&emission_position, source_object + 0x10, &a);
    transform_vector_by_basis(&b, &b, source_object + 0xC0);
    for (i = 1; i >= 0; i--) {
        fast_vec_normalize(&particle_velocity, &b, random_float_between(0.01f, 0.06f));
        scale_2b1af0(&b, &l00_object_offset_particle_velocity_contributions.a, 0.7f);
        b.f[2] = 0.0f;
        fast_vec_add(&particle_velocity, &particle_velocity, &b);
        scale_2b1af0(&b, &l00_object_offset_particle_velocity_contributions.b, 0.7f);
        b.f[2] = 0.0f;
        fast_vec_add(&particle_velocity, &particle_velocity, &b);
        particle_byte_input_or_min_frames = truncate_float_to_s32(randf_sym(0.0f, 6.0f));
        spawn_particle(&emission_position,
                       &particle_velocity,
                       scale_game_frames(0x1E),
                       0x7F,
                       0xFFFFFF,
                       particle_byte_input_or_min_frames,
                       25000.0f,
                       300.0f,
                       0.98f,
                       0.0f,
                       0.85f);
    }
    for (j = 0; j >= 0; j--) {
        fast_vec_normalize(&particle_velocity, &b, random_float_between(0.01f, 0.06f));
        scale_2b1af0(&b, &l00_object_offset_particle_velocity_contributions.a, 0.55f);
        b.f[2] = 0.0f;
        fast_vec_add(&particle_velocity, &particle_velocity, &b);
        scale_2b1af0(&b, &l00_object_offset_particle_velocity_contributions.b, 0.55f);
        b.f[2] = 0.0f;
        fast_vec_add(&particle_velocity, &particle_velocity, &b);
        particle_byte_input_or_min_frames = scale_game_frames(0x14);
        spawn_burst(&emission_position,
                    &particle_velocity,
                    0x4F007FFF,
                    0x1FFFFFFF,
                    random_int_between(particle_byte_input_or_min_frames, scale_game_frames(0x28)),
                    1,
                    10000.0f);
    }
}

void build_look_at_matrix(void *, void *, void *, float) __asm__("FUN_00214890");

extern V_2b1af0 l00_object_offset_burst_reference_direction __asm__("D_0013F9B0");
void emit_object_offset_particle_burst(unsigned char *source_object) __asm__("FUN_L00_002b1de0");

void emit_object_offset_particle_burst(unsigned char *source_object) {
    V_2b1af0 particle_velocity;
    V_2b1af0 emission_offset;
    V_2b1af0 emission_position;
    V_2b1af0 scaled_global_vector;
    int      i;
    int      j;
    int      signed_random_particle_argument;
    float    angle;

    *(OvlQuad *)&emission_offset = 0;
    emission_offset.f[0] = 0.3f;
    emission_offset.f[1] = -1.2f;
    emission_offset.f[3] = 1.0f;
    transform_vector_by_basis(&emission_offset, &emission_offset, source_object + 0xC0);
    fast_vec_add(&emission_position, source_object + 0x10, &emission_offset);
    for (i = 0; i < truncate_float_to_s32(24.0f); i++) {
        angle = randf_sym(0.0f, 0.1308997f);
        build_look_at_matrix(&particle_velocity,
                             &l00_object_offset_burst_reference_direction,
                             (char *)&l00_object_offset_burst_reference_direction - 0x20,
                             (float)i * 0.2617994f + angle);
        fast_vec_normalize(
            &particle_velocity, &particle_velocity, random_float_between(0.05f, 0.1f));
        signed_random_particle_argument = random_int_between(0, 3);
        if (random_integer_below(2) != 0)
            signed_random_particle_argument = -signed_random_particle_argument;
        if (*(int *)((char *)&l00_object_offset_burst_reference_direction - 0x360) != 0) {
            scale_2b1af0(&scaled_global_vector,
                         (char *)&l00_object_offset_burst_reference_direction - 0x560,
                         1.7f);
            fast_vec_add(&particle_velocity, &particle_velocity, &scaled_global_vector);
        }
        {
            int low = scale_game_frames(30);
            int high = scale_game_frames(90);
            spawn_particle(&emission_position,
                           &particle_velocity,
                           random_int_between(low, high),
                           30,
                           0xFFFFFF,
                           signed_random_particle_argument,
                           60000.0f,
                           3000.0f,
                           0.85f,
                           -0.001f,
                           0.0f);
        }
    }
    for (j = 0; j < truncate_float_to_s32(6.0f); j++) {
        angle = random_float_between(0.0f, 0.7853982f);
        build_look_at_matrix(&particle_velocity,
                             &l00_object_offset_burst_reference_direction,
                             (char *)&l00_object_offset_burst_reference_direction - 0x20,
                             (float)j * 1.0471976f + angle);
        fast_vec_normalize(
            &particle_velocity, &particle_velocity, random_float_between(0.05f, 0.1f));
        {
            int low = scale_game_frames(20);
            int high = scale_game_frames(60);
            spawn_burst(&emission_position,
                        &particle_velocity,
                        0x4F007FFF,
                        0x1FFFFFFF,
                        random_int_between(low, high),
                        1,
                        10000.0f);
        }
    }
    source_object[0xBC] = scale_game_frames(5);
}

short D_L01_001613D0 = 0;
short D_L01_001613D4 = 0;
short D_L01_001613F0 = 0;
short D_L01_001613F4 = 0;
short D_L01_001613F8 = 0;
short D_L01_001613FC = 1;
short D_L01_00161400 = 0x1e;
short D_L01_00161404 = 0xa;
short D_L01_00161408 = 0;
short D_L01_0016140C = 0;
short D_L01_00161410 = -26214;
short D_L01_00161414 = -26214;
short D_L01_00161418 = -13107;
short D_L01_0016141C = -32513;
short D_L01_00161420 = -1;
short D_L01_00161424 = 0x4040;
short D_L01_00161428 = 0x2080;
short D_L01_0016142C = 1;
short D_L01_00161430 = 0x19;

Colors6 D_L01_0020A980 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L01_0020A998 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L01_00161438 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L01_00161448 = 0;
float D_L01_0016144C = 3.6e+02f;
f32   D_L01_00161470 = 0.9f;
s32   D_L01_00161474 = 4;

Colors1688 D_L01_0020AA50 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L01_0020AA68 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L01_001B255C NOT_SDA = 0;
s32                           D_L02_001612D0 = 0;
s32                           D_L02_001612D8 = 0;
short                         D_L02_001613A0 = 0;
short                         D_L02_001613A4 = 0;
int                           D_L02_00160770 = -1;
short                         D_L02_001613C0 = 0;
short                         D_L02_001613C4 = 0;
short                         D_L02_001613C8 = 0;
short                         D_L02_001613CC = 1;
short                         D_L02_001613D0 = 0x1e;
short                         D_L02_001613D4 = 0xa;
short                         D_L02_001613D8 = 0;
short                         D_L02_001613DC = 0;
short                         D_L02_001613E0 = -26214;
short                         D_L02_001613E4 = -26214;
short                         D_L02_001613E8 = -13107;
short                         D_L02_001613EC = -32513;
short                         D_L02_001613F0 = -1;
short                         D_L02_001613F4 = 0x4040;
short                         D_L02_001613F8 = 0x2080;
short                         D_L02_001613FC = 1;
short                         D_L02_00161400 = 0x19;

Colors6 D_L02_001FA220 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L02_001FA238 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L02_00161408 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L02_00161418 = 0;
float D_L02_0016141C = 3.6e+02f;
f32   D_L02_00161440 = 0.9f;
s32   D_L02_00161444 = 4;

Colors1688 D_L02_001FA2F0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L02_001FA308 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L02_001B26DC NOT_SDA = 0;
short                         D_L03_001613B0 = 0;
short                         D_L03_001613B4 = 0;
unsigned char *D_L03_001B2180 NOT_SDA = 0;
short                         D_L03_001613D0 = 0;
short                         D_L03_001613D4 = 0;
short                         D_L03_001613D8 = 0;
short                         D_L03_001613DC = 1;
short                         D_L03_001613E0 = 0x1e;
short                         D_L03_001613E4 = 0xa;
short                         D_L03_001613E8 = 0;
short                         D_L03_001613EC = 0;
short                         D_L03_001613F0 = -26214;
short                         D_L03_001613F4 = -26214;
short                         D_L03_001613F8 = -13107;
short                         D_L03_001613FC = -32513;
short                         D_L03_00161400 = -1;
short                         D_L03_00161404 = 0x4040;
short                         D_L03_00161408 = 0x2080;
short                         D_L03_0016140C = 1;
short                         D_L03_00161410 = 0x19;

Colors6 D_L03_001E2AB0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L03_001E2AC8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L03_00161418 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L03_00161428 = 0;
float D_L03_0016142C = 3.6e+02f;
f32   D_L03_00161450 = 0.9f;
s32   D_L03_00161454 = 4;

Colors1688 D_L03_001E2B80 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L03_001E2B98 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L03_001B21DC NOT_SDA = 0;
s32                           D_L04_001612D0 = 0;
s32                           D_L04_001612D8 = 0;
short                         D_L04_001613A0 = 0;
short                         D_L04_001613A4 = 0;
unsigned char *D_L04_001B2200 NOT_SDA = 0;
int                           D_L04_00160770 = -1;
short                         D_L04_001613C4 = 0;
short                         D_L04_001613C8 = 0;
short                         D_L04_001613CC = 0;
short                         D_L04_001613D0 = 1;
short                         D_L04_001613D4 = 0x1e;
short                         D_L04_001613D8 = 0xa;
short                         D_L04_001613DC = 0;
short                         D_L04_001613E0 = 0;
short                         D_L04_001613E4 = -26214;
short                         D_L04_001613E8 = -26214;
short                         D_L04_001613EC = -13107;
short                         D_L04_001613F0 = -32513;
short                         D_L04_001613F4 = -1;
short                         D_L04_001613F8 = 0x4040;
short                         D_L04_001613FC = 0x2080;
short                         D_L04_00161400 = 1;
short                         D_L04_00161404 = 0x19;

Colors6 D_L04_001DB6C0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L04_001DB6D8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L04_00161408 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

f32 D_L04_00161440 = 0.9f;
s32 D_L04_00161444 = 4;

Colors1688 D_L04_001DB790 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L04_001DB7A8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L04_001B225C NOT_SDA = 0;
short                         D_L05_001613B0 = 0;
short                         D_L05_001613B4 = 0;
unsigned char *D_L05_001B2500 NOT_SDA = 0;
int                           D_L05_00160770 = -1;
short                         D_L05_00161410 = 0;
short                         D_L05_00161414 = 0;
short                         D_L05_00161418 = 0;
short                         D_L05_0016141C = 1;
short                         D_L05_00161420 = 0x1e;
short                         D_L05_00161424 = 0xa;
short                         D_L05_00161428 = 0;
short                         D_L05_0016142C = 0;
short                         D_L05_00161430 = -26214;
short                         D_L05_00161434 = -26214;
short                         D_L05_00161438 = -13107;
short                         D_L05_0016143C = -32513;
short                         D_L05_00161440 = -1;
short                         D_L05_00161444 = 0x4040;
short                         D_L05_00161448 = 0x2080;
short                         D_L05_0016144C = 1;
short                         D_L05_00161450 = 0x19;

Colors6 D_L05_00218030 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L05_00218048 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L05_00161458 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L05_00161468 = 0;
float D_L05_0016146C = 3.6e+02f;
f32   D_L05_00161490 = 0.9f;
s32   D_L05_00161494 = 4;

Colors1688 D_L05_00218100 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L05_00218118 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L05_001B255C NOT_SDA = 0;
s32                           D_L06_001612D0 = 0;
s32                           D_L06_001612D8 = 0;
short                         D_L06_001613A0 = 0;
short                         D_L06_001613A4 = 0;
unsigned char *D_L06_001B2800 NOT_SDA = 0;
int                           D_L06_00160770 = -1;
short                         D_L06_001613C0 = 0;
short                         D_L06_001613C4 = 0;
short                         D_L06_001613C8 = 0;
short                         D_L06_001613CC = 1;
short                         D_L06_001613D0 = 0x1e;
short                         D_L06_001613D4 = 0xa;
short                         D_L06_001613D8 = 0;
short                         D_L06_001613DC = 0;
short                         D_L06_001613E0 = -26214;
short                         D_L06_001613E4 = -26214;
short                         D_L06_001613E8 = -13107;
short                         D_L06_001613EC = -32513;
short                         D_L06_001613F0 = -1;
short                         D_L06_001613F4 = 0x4040;
short                         D_L06_001613F8 = 0x2080;
short                         D_L06_001613FC = 1;
short                         D_L06_00161400 = 0x19;

Colors6 D_L06_00200D20 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L06_00200D38 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L06_00161408 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L06_00161418 = 0;
float D_L06_0016141C = 3.6e+02f;
f32   D_L06_00161440 = 0.9f;
s32   D_L06_00161444 = 4;

Colors1688 D_L06_00200DF0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L06_00200E08 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L06_001B285C NOT_SDA = 0;
short                         D_L07_001613A0 = 0;
short                         D_L07_001613A4 = 0;
unsigned char *D_L07_001B2100 NOT_SDA = 0;
int                           D_L07_00160770 = -1;
short                         D_L07_001613C0 = 0;
short                         D_L07_001613C4 = 0;
short                         D_L07_001613C8 = 0;
short                         D_L07_001613CC = 1;
short                         D_L07_001613D0 = 0x1e;
short                         D_L07_001613D4 = 0xa;
short                         D_L07_001613D8 = 0;
short                         D_L07_001613DC = 0;
short                         D_L07_001613E0 = -26214;
short                         D_L07_001613E4 = -26214;
short                         D_L07_001613E8 = -13107;
short                         D_L07_001613EC = -32513;
short                         D_L07_001613F0 = -1;
short                         D_L07_001613F4 = 0x4040;
short                         D_L07_001613F8 = 0x2080;
short                         D_L07_001613FC = 1;
short                         D_L07_00161400 = 0x19;

Colors6 D_L07_00210BC0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L07_00210BD8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L07_00161408 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L07_00161418 = 0;
float D_L07_0016141C = 3.6e+02f;
f32   D_L07_00161440 = 0.9f;
s32   D_L07_00161444 = 4;

Colors1688 D_L07_00210C90 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L07_00210CA8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L07_001B215C NOT_SDA = 0;
short                         D_L08_001613A0 = 0;
short                         D_L08_001613A4 = 0;
int                           D_L08_00160770 = -1;
short                         D_L08_001613C0 = 0;
short                         D_L08_001613C4 = 0;
short                         D_L08_001613C8 = 0;
short                         D_L08_001613CC = 1;
short                         D_L08_001613D0 = 0x1e;
short                         D_L08_001613D4 = 0xa;
short                         D_L08_001613D8 = 0;
short                         D_L08_001613DC = 0;
short                         D_L08_001613E0 = -26214;
short                         D_L08_001613E4 = -26214;
short                         D_L08_001613E8 = -13107;
short                         D_L08_001613EC = -32513;
short                         D_L08_001613F0 = -1;
short                         D_L08_001613F4 = 0x4040;
short                         D_L08_001613F8 = 0x2080;
short                         D_L08_001613FC = 1;
short                         D_L08_00161400 = 0x19;

Colors6 D_L08_001FB860 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L08_001FB878 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L08_00161408 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L08_00161418 = 0;
float D_L08_0016141C = 3.6e+02f;
f32   D_L08_00161440 = 0.9f;
s32   D_L08_00161444 = 4;

Colors1688 D_L08_001FB950 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L08_001FB968 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L08_001B28DC NOT_SDA = 0;
s32                           D_L09_001612D0 = 0;
s32                           D_L09_001612D8 = 0;
short                         D_L09_00161430 = 0;
short                         D_L09_00161434 = 0;
unsigned char *D_L09_001B2200 NOT_SDA = 0;
int                           D_L09_00160770 = -1;
short                         D_L09_00161450 = 0;
short                         D_L09_00161454 = 0;
short                         D_L09_00161458 = 0;
short                         D_L09_0016145C = 1;
short                         D_L09_00161460 = 0x1e;
short                         D_L09_00161464 = 0xa;
short                         D_L09_00161468 = 0;
short                         D_L09_0016146C = 0;
short                         D_L09_00161470 = -26214;
short                         D_L09_00161474 = -26214;
short                         D_L09_00161478 = -13107;
short                         D_L09_0016147C = -32513;
short                         D_L09_00161480 = -1;
short                         D_L09_00161484 = 0x4040;
short                         D_L09_00161488 = 0x2080;
short                         D_L09_0016148C = 1;
short                         D_L09_00161490 = 0x19;

Colors6 D_L09_00208510 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L09_00208528 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L09_00161498 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L09_001614A8 = 0;
float D_L09_001614AC = 3.6e+02f;
f32   D_L09_001614D0 = 0.9f;
s32   D_L09_001614D4 = 4;

Colors1688 D_L09_002085E0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L09_002085F8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L09_001B225C NOT_SDA = 0;
s32                           D_L10_001612D0 = 0;
s32                           D_L10_001612D8 = 0;
short                         D_L10_001613B0 = 0;
short                         D_L10_001613B4 = 0;
unsigned char *D_L10_001B2500 NOT_SDA = 0;
int                           D_L10_00160770 = -1;
short                         D_L10_001613D0 = 0;
short                         D_L10_001613D4 = 0;
short                         D_L10_001613D8 = 0;
short                         D_L10_001613DC = 1;
short                         D_L10_001613E0 = 0x1e;
short                         D_L10_001613E4 = 0xa;
short                         D_L10_001613E8 = 0;
short                         D_L10_001613EC = 0;
short                         D_L10_001613F0 = -26214;
short                         D_L10_001613F4 = -26214;
short                         D_L10_001613F8 = -13107;
short                         D_L10_001613FC = -32513;
short                         D_L10_00161400 = -1;
short                         D_L10_00161404 = 0x4040;
short                         D_L10_00161408 = 0x2080;
short                         D_L10_0016140C = 1;
short                         D_L10_00161410 = 0x19;

Colors6 D_L10_001DC710 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L10_001DC728 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L10_00161418 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L10_00161428 = 0;
float D_L10_0016142C = 3.6e+02f;
f32   D_L10_00161450 = 0.9f;
s32   D_L10_00161454 = 4;

Colors1688 D_L10_001DC7E0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L10_001DC7F8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L10_001B255C NOT_SDA = 0;
short                         D_L11_001613A0 = 0;
short                         D_L11_001613A4 = 0;
int                           D_L11_00160770 = -1;
short                         D_L11_00161450 = 0;
short                         D_L11_00161454 = 0;
short                         D_L11_00161458 = 0;
short                         D_L11_0016145C = 1;
short                         D_L11_00161460 = 0x1e;
short                         D_L11_00161464 = 0xa;
short                         D_L11_00161468 = 0;
short                         D_L11_0016146C = 0;
short                         D_L11_00161470 = -26214;
short                         D_L11_00161474 = -26214;
short                         D_L11_00161478 = -13107;
short                         D_L11_0016147C = -32513;
short                         D_L11_00161480 = -1;
short                         D_L11_00161484 = 0x4040;
short                         D_L11_00161488 = 0x2080;
short                         D_L11_0016148C = 1;
short                         D_L11_00161490 = 0x19;

Colors6 D_L11_0021ABA0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L11_0021ABB8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L11_00161498 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L11_001614A8 = 0;
float D_L11_001614AC = 3.6e+02f;
f32   D_L11_001614D0 = 0.9f;
s32   D_L11_001614D4 = 4;

Colors1688 D_L11_0021AC70 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L11_0021AC88 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L11_001B2ADC NOT_SDA = 0;
short                         D_L12_001613B0 = 0;
short                         D_L12_001613B4 = 0;
unsigned char *D_L12_001B2500 NOT_SDA = 0;
int                           D_L12_00160770 = -1;
short                         D_L12_001613D0 = 0;
short                         D_L12_001613D4 = 0;
short                         D_L12_001613D8 = 0;
short                         D_L12_001613DC = 1;
short                         D_L12_001613E0 = 0x1e;
short                         D_L12_001613E4 = 0xa;
short                         D_L12_001613E8 = 0;
short                         D_L12_001613EC = 0;
short                         D_L12_001613F0 = -26214;
short                         D_L12_001613F4 = -26214;
short                         D_L12_001613F8 = -13107;
short                         D_L12_001613FC = -32513;
short                         D_L12_00161400 = -1;
short                         D_L12_00161404 = 0x4040;
short                         D_L12_00161408 = 0x2080;
short                         D_L12_0016140C = 1;
short                         D_L12_00161410 = 0x19;

Colors6 D_L12_0020BE70 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L12_0020BE88 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L12_00161418 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L12_00161428 = 0;
float D_L12_0016142C = 3.6e+02f;
f32   D_L12_00161450 = 0.9f;
s32   D_L12_00161454 = 4;

Colors1688 D_L12_0020BF40 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L12_0020BF58 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L12_001B255C NOT_SDA = 0;
short                         D_L13_00161470 = 0;
short                         D_L13_00161474 = 0;
int                           D_L13_00160770 = -1;
short                         D_L13_001614A0 = 0;
short                         D_L13_001614A4 = 0;
short                         D_L13_001614A8 = 0;
short                         D_L13_001614AC = 1;
short                         D_L13_001614B0 = 0x1e;
short                         D_L13_001614B4 = 0xa;
short                         D_L13_001614B8 = 0;
short                         D_L13_001614BC = 0;
short                         D_L13_001614C0 = -26214;
short                         D_L13_001614C4 = -26214;
short                         D_L13_001614C8 = -13107;
short                         D_L13_001614CC = -32513;
short                         D_L13_001614D0 = -1;
short                         D_L13_001614D4 = 0x4040;
short                         D_L13_001614D8 = 0x2080;
short                         D_L13_001614DC = 1;
short                         D_L13_001614E0 = 0x19;

Colors6 D_L13_001F47D0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L13_001F47E8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L13_001614E8 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L13_001614F8 = 0;
float D_L13_001614FC = 3.6e+02f;
f32   D_L13_00161520 = 0.9f;
s32   D_L13_00161524 = 4;

Colors1688 D_L13_001F48E0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L13_001F48F8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L13_001B23DC NOT_SDA = 0;
s32                           D_L14_001612D0 = 0;
s32                           D_L14_001612D8 = 0;
short                         D_L14_001615B0 = 0;
short                         D_L14_001615B4 = 0;
unsigned char *D_L14_001B2780 NOT_SDA = 0;
int                           D_L14_00160770 = -1;
short                         D_L14_001615DC = 0;
short                         D_L14_001615E0 = 0;
short                         D_L14_001615E4 = 0;
short                         D_L14_001615E8 = 1;
short                         D_L14_001615EC = 0x1e;
short                         D_L14_001615F0 = 0xa;
short                         D_L14_001615F4 = 0;
short                         D_L14_001615F8 = 0;
short                         D_L14_001615FC = -26214;
short                         D_L14_00161600 = -26214;
short                         D_L14_00161604 = -13107;
short                         D_L14_00161608 = -32513;
short                         D_L14_0016160C = -1;
short                         D_L14_00161610 = 0x4040;
short                         D_L14_00161614 = 0x2080;
short                         D_L14_00161618 = 1;
short                         D_L14_0016161C = 0x19;

Colors6 D_L14_001FC150 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L14_001FC168 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L14_00161620 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L14_00161630 = 0;
float D_L14_00161634 = 3.6e+02f;
f32   D_L14_00161658 = 0.9f;
s32   D_L14_0016165C = 4;

Colors1688 D_L14_001FC220 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L14_001FC238 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L14_001B27DC NOT_SDA = 0;
short                         D_L15_00161490 = 0;
short                         D_L15_00161494 = 0;
unsigned char *D_L15_001B2680 NOT_SDA = 0;
int                           D_L15_00160770 = -1;
short                         D_L15_00161504 = 0;
short                         D_L15_00161508 = 0;
short                         D_L15_0016150C = 0;
short                         D_L15_00161510 = 1;
short                         D_L15_00161514 = 0x1e;
short                         D_L15_00161518 = 0xa;
short                         D_L15_0016151C = 0;
short                         D_L15_00161520 = 0;
short                         D_L15_00161524 = -26214;
short                         D_L15_00161528 = -26214;
short                         D_L15_0016152C = -13107;
short                         D_L15_00161530 = -32513;
short                         D_L15_00161534 = -1;
short                         D_L15_00161538 = 0x4040;
short                         D_L15_0016153C = 0x2080;
short                         D_L15_00161540 = 1;
short                         D_L15_00161544 = 0x19;

Colors6 D_L15_001E27C0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L15_001E27D8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L15_00161548 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L15_00161588 = 0;
float D_L15_0016158C = 3.6e+02f;
f32   D_L15_001615B0 = 0.9f;
s32   D_L15_001615B4 = 4;

Colors1688 D_L15_001E28B0 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L15_001E28C8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

short                         D_L16_00161460 = 0;
short                         D_L16_00161464 = 0;
unsigned char *D_L16_001B2500 NOT_SDA = 0;
int                           D_L16_00160770 = -1;
short                         D_L16_00161480 = 0;
short                         D_L16_00161484 = 0;
short                         D_L16_00161488 = 0;
short                         D_L16_0016148C = 1;
short                         D_L16_00161490 = 0x1e;
short                         D_L16_00161494 = 0xa;
short                         D_L16_00161498 = 0;
short                         D_L16_0016149C = 0;
short                         D_L16_001614A0 = -26214;
short                         D_L16_001614A4 = -26214;
short                         D_L16_001614A8 = -13107;
short                         D_L16_001614AC = -32513;
short                         D_L16_001614B0 = -1;
short                         D_L16_001614B4 = 0x4040;
short                         D_L16_001614B8 = 0x2080;
short                         D_L16_001614BC = 1;
short                         D_L16_001614C0 = 0x19;

Colors6 D_L16_001E7C70 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L16_001E7C88 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L16_001614C8 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L16_001614D8 = 0;
float D_L16_001614DC = 3.6e+02f;
f32   D_L16_00161500 = 0.9f;
s32   D_L16_00161504 = 4;

Colors1688 D_L16_001E7D40 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L16_001E7D58 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

short D_L17_00161430 = 0;
short D_L17_00161434 = 0;
int   D_L17_00160770 = -1;
short D_L17_0016149C = 0;
short D_L17_001614A0 = 0;
short D_L17_001614A4 = 0;
short D_L17_001614A8 = 1;
short D_L17_001614AC = 0x1e;
short D_L17_001614B0 = 0xa;
short D_L17_001614B4 = 0;
short D_L17_001614B8 = 0;
short D_L17_001614BC = -26214;
short D_L17_001614C0 = -26214;
short D_L17_001614C4 = -13107;
short D_L17_001614C8 = -32513;
short D_L17_001614CC = -1;
short D_L17_001614D0 = 0x4040;
short D_L17_001614D4 = 0x2080;
short D_L17_001614D8 = 1;
short D_L17_001614DC = 0x19;

Colors6 D_L17_001E6390 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L17_001E63A8 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L17_001614E0 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L17_00161500 = 0;
float D_L17_00161504 = 3.6e+02f;
f32   D_L17_00161528 = 0.9f;
s32   D_L17_0016152C = 4;

Colors1688 D_L17_001E6480 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L17_001E6498 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

unsigned char *D_L17_001B29DC NOT_SDA = 0;
short                         D_L18_00161440 = 0;
short                         D_L18_00161444 = 0;
unsigned char *D_L18_001B2A80 NOT_SDA = 0;
int                           D_L18_00160770 = -1;
short                         D_L18_00161460 = 0;
short                         D_L18_00161464 = 0;
short                         D_L18_00161468 = 0;
short                         D_L18_0016146C = 1;
short                         D_L18_00161470 = 0x1e;
short                         D_L18_00161474 = 0xa;
short                         D_L18_00161478 = 0;
short                         D_L18_0016147C = 0;
short                         D_L18_00161480 = -26214;
short                         D_L18_00161484 = -26214;
short                         D_L18_00161488 = -13107;
short                         D_L18_0016148C = -32513;
short                         D_L18_00161490 = -1;
short                         D_L18_00161494 = 0x4040;
short                         D_L18_00161498 = 0x2080;
short                         D_L18_0016149C = 1;
short                         D_L18_001614A0 = 0x19;

Colors6 D_L18_001F2050 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 D_L18_001F2068 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

C_2af5b8 D_L18_001614A8 = {
    {0x2fffffff, 0x2f00ffff, 0x2f007fff, 0x2f004fff}
};

float D_L18_001614B8 = 0;
float D_L18_001614BC = 3.6e+02f;
f32   D_L18_001614E0 = 0.9f;
s32   D_L18_001614E4 = 4;

Colors1688 D_L18_001F2120 = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 D_L18_001F2138 = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

int D_L00_00160770 = -1;

short bomb_spark_primary_color_end __asm__("D_L00_00161420") = -1;

short bomb_spark_secondary_color_end __asm__("D_L00_00161428") = 0x2080;

Colors6 l00_bomb_blast_effect_primary_palette __asm__("D_L00_001E9920") = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors6 l00_bomb_blast_effect_secondary_palette __asm__("D_L00_001E9938") = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};

Colors1688 l00_devastator_missile_impact_color_palette __asm__("D_L00_001E99F0") = {
    {0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}
};

Colors1688 l00_devastator_missile_impact_fade_palette __asm__("D_L00_001E9A08") = {
    {0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}
};
