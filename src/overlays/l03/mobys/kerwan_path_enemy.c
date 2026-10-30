/* Level 03 moby class kerwan_path_enemy: an enemy that walks a path ring and turns toward its target. */
#include "types.h"
#include "rnc/math/rotations/matrix_to_euler.h"
#include "rnc/overlay/collision.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "qcopy.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/l03/mobys/homing_rocket.h"
#include "rnc/overlay/l03/mobys/robot_dog.h"

extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_002C81E0), where it is exact; names translated to the US level program. */

extern short D_L03_0015FFD8_u __asm__("D_L03_0015FFD8");

int try_start_kerwan_path_enemy_heading_turn(float        desired_heading,
                                             struct Moby *moby,
                                             struct Moby *target) __asm__("FUN_L03_002c6e18");

int try_start_kerwan_path_enemy_heading_turn(float        desired_heading,
                                             struct Moby *moby,

                                             struct Moby *target) {
    char         *data = (char *)moby->pvars;
    int           linked_moby_index;
    float         heading_delta;
    unsigned char previous_state;
    if (*(int *)&target->rot != 0) {
        linked_moby_index = *(int *)(data + 0x264);
        if (linked_moby_index != -1) {
            char *linked_moby = (char *)(linked_moby_index << 8) + (int)*(char **)&D_L03_0015FFD8_u;
            if (MOBY(linked_moby)->oclass == 0x336) {
                if ((unsigned char)linked_moby[0xBC] != 4)
                    return 0;
            }
        }
        heading_delta = fast_subtract_rotations(desired_heading, moby->rot.z);
        if (absolute_float(heading_delta) >= 0.7853982f && *(unsigned char *)(data + 0x263) == 0 &&
            moby->prev_seq != 1) {
            goto go;
        }
        if (absolute_float(heading_delta) > 1.5707964f) {
        go:
            if (vector_distance_xy(&moby->pos, target) < 20.0f) {
                if (absolute_float(heading_delta) > 1.1780972f) {
                    blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                    moby->anim_speed = 1.0f;
                } else {
                    blend_moby_animation(moby, 9, 0, scale_game_frames(10));
                }
                *(float *)(data + 0x28C) = desired_heading;
                previous_state = moby->state;
                moby->state = 0xC;
                moby->command = previous_state;
                return 1;
            }
        }
    }
    return 0;
}

extern char  D_L03_00166EC0[];
extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern float D_L03_0015F580_macro[] __asm__("D_L03_0015F580") MACRO_ADDR;

/* Update for moby class 574 on Kerwan: a path-walking enemy that turns to, chases and attacks its target, jumps, falls under gravity and is replaced by debris when it dies. */
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_002C8398), where it is exact; names translated to the US level program. */

typedef struct {
    float pos[4];
    float v10[4];
    float dir[4];
    float v30[4];
    void *obj;
    int   pad[3];
} Tgt2C8398;

typedef struct {
    float          v[4];
    void          *m;
    int            flags;
    unsigned char  b0;
    unsigned char  b1;
    unsigned short id;
    float          f;
} Hit2C8398;

typedef struct {
    char          p0[8];
    float         f8;
    char          pC[4];
    float         f10;
    float         f14;
    float         f18;
    float         f1C;
    int           i20;
    int           i24;
    float         f28;
    char          p2C[0x11];
    unsigned char b3D;
    short         s3E;
    char          p40[0xC];
    float         f4C;
    float         f50;
    float         f54;
    char          p58[8];
} Jump2C8398;

typedef struct {
    char  p0[8];
    float f8;
    float fC;
    char  p10[8];
    float f18;
    char  p1C[8];
    float f24;
    float f28;
    float f2C;
    char  p30[8];
    int   i38;
    float f3C;
    float f40;
    float f44;
    char  p48[8];
} Mover2C8398;

typedef struct {
    int         idx;
    signed char step;
    char        p5[0xB];
    char       *path;
    int         id;
} PathF2C8398;

typedef struct {
    char  p0[0x68];
    float f68;
    char  p6C[0x14];
} Look2C8398;

typedef struct Data2C8398 {
    char          p00[0x20];
    float         f20;
    short         s24;
    char          p26[2];
    unsigned char b28;
    unsigned char b29;
    char          p2A[6];
    float         f30;
    char          p34[4];
    int           i38;
    char          p3C[4];
    float         v40[4];
    char          p50[8];
    unsigned char b58;
    char          p59;
    unsigned char b5A;
    char          p5B[0xC];
    unsigned char b67;
    char          p68[8];
    Jump2C8398    q;
    Mover2C8398   mv;
    PathF2C8398   pf;
    char          p138[0x18];
    Look2C8398    look0;
    Look2C8398    look1;
    short         s250;
    short         s252;
    unsigned char b254;
    unsigned char b255;
    short         s256;
    float         f258;
    char          p25C[6];
    unsigned char b262;
    unsigned char b263;
    int           i264;
    char          p268[4];
    float         f26C;
    float         f270;
    int           i274;
    float         f278;
    float         f27C;
    int           i280;
    int           i284;
    short         s288;
    short         s28A;
    float         f28C;
    int           i290;
    float         f294;
    int           i298;
    int           i29C;
} Data2C8398;

typedef struct Moby2C8398 {
    char           p00[0x10];
    float          pos[4];
    unsigned char  state;
    unsigned char  b21;
    char           p22[2];
    char          *p24;
    char           p28[9];
    unsigned char  b31;
    unsigned short s32;
    unsigned short flags;
    char           p36[2];
    long           l38;
    float          rot[4];
    char           p50[2];
    unsigned char  b52;
    unsigned char  anim;
    char           p54[4];
    float          f58;
    char           p5C[0x14];
    unsigned char  b70;
    char           p71[7];
    Data2C8398    *data;
    char           p7C[3];
    unsigned char  b7F;
    char           p80[0x14];
    int            i94;
    char           p98[0xC];
    unsigned char  bA4;
    char           pA5;
    unsigned short cls;
    char           pA8[8];
    unsigned char  bB0;
    char           pB1[0xB];
    unsigned char  bBC;
    char           pBD[0x43];
} Moby2C8398;

extern CollisionHit D_L03_00173F40;
extern char         sphere_collision_resolved_position[] __asm__("D_L03_00173F70");
extern char         D_L03_00173F80[];
extern float        allocate_voice_for_target_entry_cf(void *) __asm__("FUN_L00_00259710");
extern float        truncate_float_to_s32_cf(float) __asm__("FUN_001f96b0");
extern int          collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern int          D_L03_0015F5C4;
extern int          D_L03_0015F5CC;
extern Moby2C8398  *D_L03_0015FFD8_m __asm__("D_L03_0015FFD8");
int                 D_L03_001618F4_i __asm__("D_L03_001618F4") = 0;
extern int          coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern int          is_value_within_interpolated_window(char *, float) __asm__("FUN_00214cc8");
extern int FUN_L00_0025df68_cf(char *m, char *p, float *pos, char *v) __asm__("FUN_L00_00259028");
extern int fast_subtract_rotations_cf(void *) __asm__("FUN_L00_001fefc8");
extern int probe_ground_height_cf(int unused, char *m, char *a, void *b, char *c, void *d) __asm__("FUN_L00_00260400");
extern short          D_L03_001618F4 __attribute__((sda));
extern unsigned char *FUN_L03_0024fba8_cf(int) __asm__("FUN_L00_002603d0");
extern void          *FUN_L00_00257470_cf(char *,
                                          int,
                                          float *,
                                          void *,
                                          int,
                                          int,
                                          float,
                                          float *,
                                          float *,
                                          float *) __asm__("FUN_L00_00263fd8");
extern void           FUN_001f96b0_cf(char *p) __asm__("FUN_L01_0026d930");
extern void           FUN_001f9c48_cf(
              char *moby, float *vel, float target, float k, float d, float max) __asm__("FUN_L00_00258278");
extern void FUN_L00_001fefc8_cf(void *) __asm__("FUN_L03_002c95a8");
extern void record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern void FUN_L00_00259710_cf(void *, int, int) __asm__("FUN_L00_00257470");
extern void FUN_L00_0025c698_cf(
    void *, void *, void *, int *, float *, int, float, float, float) __asm__("FUN_L00_0025df68");
extern void select_moby_combat_target(void *, void *, float) __asm__("FUN_L00_0025fcb8");
extern void FUN_L00_00260400_cf(void *) __asm__("FUN_L00_0025a120");
extern void manipulator_set_scale(void *, float) __asm__("FUN_L00_00263b38");
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
                            int,
                            float,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");
extern void FUN_L03_00250ae8_cf(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern void emit_radial_particle_burst(void *, void *, int) __asm__("FUN_L03_00250ae8");
extern void is_value_within_interpolated_window_cf(void *) __asm__("FUN_L03_002c9ca0");

typedef unsigned int AxisAngleQuad __attribute__((mode(TI), aligned(16)));
extern void          FUN_L00_001ffd48(void *, void *, float);
extern void          FUN_L00_001ffe20(void *, void *, void *);

/* Builds the rotation matrix of angle about a unit axis (Rodrigues: skew = cross-product matrix of axis,
 * sin_part = sin*skew, cos_part = (1-cos)*skew^2, plus identity) and converts it to Euler angles in a
 * local copy of point. */
void axis_angle_rotation_to_euler(AxisAngleQuad *axis,
                                  AxisAngleQuad *point,
                                  float          angle) __asm__("FUN_L03_0024fba8");

void axis_angle_rotation_to_euler(AxisAngleQuad *axis, AxisAngleQuad *point, float angle) {
    float  axis_v[4] __attribute__((aligned(16)));
    float  point_v[4] __attribute__((aligned(16)));
    float  skew[4][4] __attribute__((aligned(16)));
    float  skew_sq[4][4] __attribute__((aligned(16)));
    float  sin_part[4][4] __attribute__((aligned(16)));
    float  cos_part[4][4] __attribute__((aligned(16)));
    float  axis_rotation_matrix[4][4] __attribute__((aligned(16)));
    float *point_p = point_v;
    *(AxisAngleQuad *)axis_v = *axis;
    *(AxisAngleQuad *)point_p = *point;
    skew[0][0] = 0.0f;
    skew[1][0] = -axis_v[2];
    skew[2][0] = axis_v[1];
    skew[3][0] = 0.0f;
    skew[0][1] = axis_v[2];
    skew[1][1] = 0.0f;
    skew[2][1] = -axis_v[0];
    skew[3][1] = 0.0f;
    skew[0][2] = -axis_v[1];
    skew[1][2] = axis_v[0];
    skew[2][2] = 0.0f;
    skew[3][2] = 0.0f;
    skew[0][3] = 0.0f;
    skew[1][3] = 0.0f;
    skew[2][3] = 0.0f;
    skew[3][3] = 0.0f;
    multiply_matrix(skew_sq, skew, skew);
    FUN_L00_001ffd48(cos_part, skew_sq, 1.0f - fast_cos(angle));
    FUN_L00_001ffd48(sin_part, skew, fast_sin(angle));
    FUN_L00_001ffe20(sin_part, sin_part, cos_part);
    set_identity_matrix(cos_part);
    FUN_L00_001ffe20(axis_rotation_matrix, sin_part, cos_part);
    matrix_to_euler(axis_rotation_matrix, point_p);
}

/* True for a moby of class 0x23E in state 9. */
int is_kerwan_path_enemy_in_state_9(struct Moby *moby) __asm__("FUN_L03_002c6d58");

int is_kerwan_path_enemy_in_state_9(struct Moby *moby) {
    if (moby && moby->state != 0xFE && moby->state != 0xFD && moby->oclass == 0x23e &&
        moby->state == 0x9) {
        return 1;
    }
    return 0;
}

void update_kerwan_path_enemy(Moby2C8398 *m) __asm__("FUN_L03_002c6fd0");

void update_kerwan_path_enemy(Moby2C8398 *m) {
    enum {
        ENEMY_INITIALIZE = 0,
        ENEMY_WAIT_FOR_ACTIVATION = 1,
        ENEMY_WAIT_FOR_LINKED_MOBY = 2,
        ENEMY_PROJECTILE_ATTACK_WINDUP = 4,
        ENEMY_PROJECTILE_ATTACK = 5,
        ENEMY_MELEE_ATTACK = 6,
        ENEMY_MOVE_TO_PATH_NODE = 7,
        ENEMY_FOLLOW_PATH_TO_END = 8,
        ENEMY_FOLLOW_ALTERNATE_PATH = 9,
        ENEMY_KNOCKBACK_RECOVERY = 10,
        ENEMY_FALL_UNDER_GRAVITY = 11,
        ENEMY_TURN_TO_HEADING = 12,
        ENEMY_DEATH_KNOCKBACK = 13,
        ENEMY_HIDDEN_AT_PATH_START = 14,
        ENEMY_RESPAWN_AT_PATH_START = 15
    };
    enum {
        ENEMY_ANIM_LOCOMOTION = 1,
        ENEMY_ANIM_HEADING_TURN = 9
    };
    Tgt2C8398   selected_target;
    Data2C8398 *moby_data = m->data;
    float       target_distance;

    select_moby_combat_target(m, &selected_target, moby_data->f270);
    target_distance = vector_distance_xy(m->pos, &selected_target);
    manipulator_set_scale(&moby_data->look0, 2.5f);
    if (moby_data->b262 == 1) {
        if ((visited52)[m->bB0 + (current_level_index << 4)] == 0xFF) {
            m->flags |= 0x41;
            m->i94 = 0;
        }
        return;
    }
    if (D_L03_0015F5C4 == 2) {
        m->flags |= 0x41;
    } else if (m->state != 0xE) {
        m->flags &= 0xFFBE;
    }
    if (m->pos[0] < 2.0f || 1021.0f < m->pos[0] || m->pos[1] < 2.0f || 1021.0f < m->pos[1] ||
        m->pos[2] < 2.0f || 1021.0f < m->pos[2] || (moby_data->pf.id == -1 && m->state != 0xD)) {
        mark_moby_for_removal(m);
        return;
    }
    FUN_L00_001fefc8_cf(m);
    fast_subtract_rotations_cf(&moby_data->b263);
    if (m->b31 != 0 || target_distance < 24.0f) {
        if (m->state == 0xC) {
            moby_data->look0.f68 = fast_subtract_rotations(moby_data->f28C, m->rot[2]) * 0.25f;
            moby_data->look1.f68 = fast_subtract_rotations(moby_data->f28C, m->rot[2]) * 0.75f;
        } else if (m->state < 7 && target_distance < moby_data->f270) {
            moby_data->look1.f68 =
                fast_subtract_rotations(fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                                     selected_target.pos[1] - m->pos[1]),
                                        m->rot[2]);
        }
        if (moby_data->look0.f68 > 0.7853982f) {
            moby_data->look0.f68 = 0.7853982f;
        } else if (moby_data->look0.f68 < -0.7853982f) {
            moby_data->look0.f68 = -0.7853982f;
        }
        if (moby_data->look1.f68 > 0.7853982f) {
            moby_data->look1.f68 = 0.7853982f;
        } else if (moby_data->look1.f68 < -0.7853982f) {
            moby_data->look1.f68 = -0.7853982f;
        }
        update_joint_animation(
            (char *)m, (char *)&moby_data->look0, 2, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f);
        update_joint_animation(
            (char *)m, (char *)&moby_data->look1, 3, frame_scale_sq * 0.03f, frame_scale_sq * 0.3f);
    }
    switch (m->state) {
    case ENEMY_INITIALIZE:
        m->l38 = (((Moby2C8398 *)hero.moby))->l38;
        m->b7F = 0x1C;
        FUN_001f96b0_cf((char *)&moby_data->mv);
        moby_data->mv.fC = 3.0f;
        moby_data->mv.f18 = 0.0f;
        moby_data->mv.f3C = frame_time_sq * 6.2831855f;
        moby_data->mv.f40 = frame_time_sq * 12.566371f;
        moby_data->mv.f44 = frame_time * 3.1415927f;
        moby_data->mv.f24 = frame_time * 6.0f;
        moby_data->mv.f8 = 2.0f;
        moby_data->mv.f28 = 3.1415927f;
        moby_data->mv.f2C = moby_data->mv.f24 * truncate_float_to_s32_cf(15.0f);
        moby_data->mv.i38 = 9;
        moby_data->f20 = 3.0f;
        moby_data->s24 = truncate_float_to_s32(3.0f);
        moby_data->b29 = 1;
        moby_data->b28 = 2;
        moby_data->f30 = 1.75f;
        moby_data->q.f28 = 0.7f;
        moby_data->q.i20 = truncate_float_to_s32(716.8f);
        moby_data->q.f4C = 0.0f;
        moby_data->b58 = 0xA;
        moby_data->b5A = 0xC;
        clear_u64_value(moby_data->v40);
        m->flags |= 0x1000;
        moby_data->s250 = 0;
        {
            char *np = ((char *)l03_paths[moby_data->pf.id]);
            moby_data->pf.idx = 0;
            moby_data->pf.path = np;
            qcopy(m->pos, np + 0x10);
        }
        m->state = 1;
        moby_data->s288 = m->s32;
        moby_data->s28A = D_L03_001618F4 & 7;
        D_L03_001618F4_i++;
        moby_data->b263 = 0;
        {
            char *path = moby_data->pf.path;
            if (path != 0) {
                int i;
                for (i = 0; i < *(int *)path - 2; i++) {
                    *(float *)(path + 0x1C + i * 0x10) =
                        fast_vec_dist(path + 0x10 + i * 0x10, path + 0x20 + i * 0x10);
                }
            }
        }
        if (moby_data->i284 != -1 && moby_data->i290 == -1) {
            blend_moby_animation(m, 0, 0, scale_game_frames(10));
            m->state = 2;
        }
        break;
    case ENEMY_WAIT_FOR_ACTIVATION:
        if ((m->b70 & 2) && m->b52 != 0) {
            blend_moby_animation(m, 0, 0, scale_game_frames(10));
        }
        if (moby_data->i290 != -1) {
            char *np = ((char *)l03_paths[moby_data->i290]);
            *(OvlQuad *)m->pos = *(OvlQuad *)(np + 0x10);
            m->rot[2] = fast_arc_tan(*(float *)(np + 0x20) - *(float *)(np + 0x10),
                                     *(float *)(np + 0x24) - *(float *)(np + 0x14));
            if (is_point_inside_clip_volume_cf(m->b21) ||
                is_point_inside_clip_volume(&selected_target, moby_data->i29C) ||
                (visited52)[m->bB0 + (current_level_index << 4)] == 0xFF ||
                (target_distance < moby_data->f294 &&
                 absolute_float(selected_target.pos[2] - m->pos[2]) < 4.0f)) {
                moby_data->i298 = 0;
                m->state = 9;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
        } else if (moby_data->i284 != -1) {
            blend_moby_animation(m, 0, 0, scale_game_frames(10));
            m->state = 2;
        } else {
            if (target_distance < moby_data->f270 || target_distance < moby_data->f27C) {
                if (try_start_kerwan_path_enemy_heading_turn(
                        fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                     selected_target.pos[1] - m->pos[1]),
                        (unsigned char *)m,
                        (char *)&selected_target)) {
                    break;
                }
            }
            if (target_distance < moby_data->f27C &&
                absolute_float(selected_target.pos[2] - m->pos[2]) < 4.0f) {
                moby_data->f27C = 0.0f;
                moby_data->s256 = 1;
                m->state = 8;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
            if (target_distance < 3.5f &&
                absolute_float(selected_target.pos[2] - m->pos[2]) < 3.0f) {
                m->state = 6;
                if (m->anim != 2) {
                    blend_moby_animation(m, 2, 0, scale_game_frames(10));
                }
            } else if (target_distance < moby_data->f270 &&
                       absolute_float(selected_target.pos[2] - m->pos[2]) < 10.0f &&
                       fast_dec_timer_s16(&moby_data->s250)) {
                if (fast_difference_between_rotations(
                        m->rot[2],
                        fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                     selected_target.pos[1] - m->pos[1])) < 0.7853982f) {
                    if (m->b31 != 0) {
                        m->state = 4;
                        if (m->anim != 3) {
                            blend_moby_animation(m, 3, 0, scale_game_frames(10));
                        }
                    }
                }
            }
        }
        break;
    case ENEMY_WAIT_FOR_LINKED_MOBY:
        if (target_distance < moby_data->f270 &&
            absolute_float(selected_target.pos[2] - m->pos[2]) < 4.0f && m->anim != 0xA) {
            blend_moby_animation(m, 0xA, 0, scale_game_frames(10));
            break;
        }
        {
            Moby2C8398 *o = &D_L03_0015FFD8_m[moby_data->i284];
            if (o == 0 || o->state == 0xFE || o->state == 0xFD || (m->b52 == 0xA && (m->b70 & 2))) {
                m->state = 1;
                blend_moby_animation(m, 0, 0, scale_game_frames(10));
                moby_data->s250 = scale_game_frames(0x3C);
                moby_data->i284 = -1;
            } else {
                try_start_kerwan_path_enemy_heading_turn(
                    fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                 selected_target.pos[1] - m->pos[1]),
                    (unsigned char *)m,
                    (char *)&selected_target);
            }
        }
        break;
    case ENEMY_PROJECTILE_ATTACK_WINDUP:
        if (m->b70 & 2) {
            m->state = 5;
            moby_data->b254 = 0;
            if (m->anim != 4) {
                blend_moby_animation(m, 4, 0, 0);
            }
        } else if (target_distance < 3.5f &&
                   absolute_float(selected_target.pos[2] - m->pos[2]) < 3.0f) {
            m->state = 6;
            if (m->anim != 2) {
                blend_moby_animation(m, 2, 0, scale_game_frames(10));
            }
        } else if (target_distance < moby_data->f278 &&
                   absolute_float(selected_target.pos[2] - m->pos[2]) < 10.0f) {
            if (moby_data->i274 != 0) {
                Data2C8398 *d2 = m->data;
                if (d2->pf.idx < *(int *)d2->pf.path - 1) {
                    d2->pf.idx++;
                    m->state = 7;
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                }
            } else {
                is_value_within_interpolated_window_cf(m);
                m->state = 7;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
        }
        break;
    case ENEMY_PROJECTILE_ATTACK: {
        if (is_value_within_interpolated_window((char *)m, 8.0f)) {
            float a[4];
            float b[4];
            float scaled_launch_vector[4];
            float launch_endpoint[4];
            float ang;
            float df;
            moby_data->b254++;
            moby_point_position(m, 0, a);
            fast_vec_sub(b, selected_target.dir, a);
            fast_vec_normalize(b, b, frame_time * 6.0f);
            b[2] *= 0.5f;
            ang = fast_arc_tan(b[0], b[1]);
            df = fast_subtract_rotations(ang, m->rot[2]);
            if (df > 0.5235988f) {
                ang = fast_add_rotations(m->rot[2], 0.5235988f);
                b[0] = fast_cos(ang) * (frame_time * 6.0f);
                b[1] = fast_sin(ang) * (frame_time * 6.0f);
                b[2] = 0.0f;
            } else if (df < -0.5235988f) {
                ang = fast_add_rotations(m->rot[2], -0.5235988f);
                b[0] = fast_cos(ang) * (frame_time * 6.0f);
                b[1] = fast_sin(ang) * (frame_time * 6.0f);
                b[2] = 0.0f;
            }
            fast_vec_scale(scaled_launch_vector, b, truncate_float_to_s32_cf(10.0f));
            FUN_L03_00250ae8_cf(launch_endpoint, scaled_launch_vector, a);
            emit_radial_particle_burst(m, scaled_launch_vector, 0);
            spawn_homing_rocket(
                b, a, (struct Moby *)m, scale_game_frames(0x82), ang, frame_time * 6.0f, 0.75f);
        }
        if (m->b70 & 2) {
            if (moby_data->b254 == 0) {
                break;
            }
            if (target_distance < moby_data->f27C &&
                absolute_float(selected_target.pos[2] - m->pos[2]) < 4.0f) {
                moby_data->f27C = 0.0f;
                m->state = 8;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
            if (try_start_kerwan_path_enemy_heading_turn(
                    fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                 selected_target.pos[1] - m->pos[1]),
                    (unsigned char *)m,
                    (char *)&selected_target)) {
                break;
            }
            if (target_distance < moby_data->f270 &&
                absolute_float(selected_target.pos[2] - m->pos[2]) < 10.0f &&
                fast_dec_timer_s16(&moby_data->s250) &&
                fast_difference_between_rotations(
                    m->rot[2],
                    fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                 selected_target.pos[1] - m->pos[1])) < 0.08726646f &&
                m->b31 != 0) {
                break;
            }
            if (target_distance < 3.5f &&
                absolute_float(selected_target.pos[2] - m->pos[2]) < 3.0f) {
                m->state = 6;
                if (m->anim != 2) {
                    blend_moby_animation(m, 2, 0, scale_game_frames(10));
                }
            }
        } else if (target_distance < 3.5f &&
                   absolute_float(selected_target.pos[2] - m->pos[2]) < 3.0f) {
            if (try_start_kerwan_path_enemy_heading_turn(
                    fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                 selected_target.pos[1] - m->pos[1]),
                    (unsigned char *)m,
                    (char *)&selected_target)) {
                break;
            }
            m->state = 6;
            if (m->anim != 2) {
                blend_moby_animation(m, 2, 0, scale_game_frames(10));
            }
        } else if (target_distance < moby_data->f278 &&
                   absolute_float(selected_target.pos[2] - m->pos[2]) < 10.0f) {
            if (moby_data->i274 != 0) {
                Data2C8398 *d2 = m->data;
                if (d2->pf.idx < *(int *)d2->pf.path - 1) {
                    d2->pf.idx++;
                    m->state = 7;
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                }
            } else {
                is_value_within_interpolated_window_cf(m);
                m->state = 7;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
        }
        break;
    }
    case ENEMY_MELEE_ATTACK: {
        {
            float     v[4];
            Hit2C8398 hit;
            float     fr;
            float     ang;
            fr = compute_interpolated_record_value(m);
            ang = fast_arc_tan(selected_target.pos[0] - m->pos[0],
                               selected_target.pos[1] - m->pos[1]);
            FUN_001f9c48_cf((char *)m, &moby_data->f258, ang, 0.03f, 0.3f, 0.3f);
            moby_point_position(m, 0, v);
            if (25.0f <= fr && fr <= 31.0f &&
                absolute_float(m->pos[2] - selected_target.pos[2]) < 2.0f &&
                vector_distance_xy(m->pos, &selected_target) < 3.5f &&
                fast_difference_between_rotations(fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                                               selected_target.pos[1] - m->pos[1]),
                                                  m->rot[2]) < 0.2617994f) {
                hit.m = m;
                hit.flags = 0x10001;
                hit.f = 1.0f;
                hit.v[0] = fast_cos(m->rot[2]) * 0.2f;
                hit.v[1] = fast_sin(m->rot[2]) * 0.2f;
                hit.v[2] = 0.0f;
                set_vector_length_xy(hit.v, hit.v, 1.0f);
                hit.v[2] = 1.0f;
                hit.v[3] = 5627.925f;
                hit.b0 = 0;
                hit.b1 = 1;
                hit.id = m->cls;
                if (selected_target.obj != 0) {
                    record_moby_hit(selected_target.obj, &hit);
                }
            } else if (m->b70 & 2) {
                if (moby_data->i274 != 0) {
                    Data2C8398 *d2 = m->data;
                    if (d2->pf.idx < *(int *)d2->pf.path - 1) {
                        d2->pf.idx++;
                        m->state = 7;
                        if (m->anim != 1) {
                            blend_moby_animation(m, 1, 0, scale_game_frames(10));
                        }
                    } else {
                        m->state = 1;
                        if (m->anim != 0) {
                            blend_moby_animation(m, 0, 0, scale_game_frames(10));
                        }
                        try_start_kerwan_path_enemy_heading_turn(
                            fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                         selected_target.pos[1] - m->pos[1]),
                            (unsigned char *)m,
                            (char *)&selected_target);
                    }
                } else {
                    is_value_within_interpolated_window_cf(m);
                    m->state = 7;
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                }
            }
        }
        break;
    }
    case ENEMY_MOVE_TO_PATH_NODE: {
        PathF2C8398 *pf = &moby_data->pf;
        if (moby_data->s256 != 0) {
            m->state = 8;
            break;
        }
        {
            int path_node_byte_offset = pf->idx * 0x10;
            if (try_start_kerwan_path_enemy_heading_turn(
                    fast_arc_tan(*(float *)(pf->path + path_node_byte_offset + 0x10) - m->pos[0],
                                 *(float *)(pf->path + path_node_byte_offset + 0x14) - m->pos[1]),
                    (unsigned char *)m,
                    (char *)&selected_target)) {
                break;
            }
        }
        if (!(FUN_L00_0025df68_cf((char *)m,
                                  (char *)&moby_data->mv,
                                  (float *)(pf->path + 0x10 + pf->idx * 0x10),
                                  (char *)moby_data->v40) &
              0x14)) {
            break;
        }
        if (target_distance < moby_data->f278 &&
            absolute_float(selected_target.pos[2] - m->pos[2]) < 10.0f) {
            if (moby_data->i274 != 0) {
                Data2C8398 *d2 = m->data;
                if (d2->pf.idx < *(int *)d2->pf.path - 1) {
                    d2->pf.idx++;
                    m->state = 7;
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                } else {
                    m->state = 1;
                    if (m->anim != 0) {
                        blend_moby_animation(m, 0, 0, scale_game_frames(10));
                    }
                    try_start_kerwan_path_enemy_heading_turn(
                        fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                     selected_target.pos[1] - m->pos[1]),
                        (unsigned char *)m,
                        (char *)&selected_target);
                }
            } else {
                is_value_within_interpolated_window_cf(m);
                m->state = 7;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
        } else {
            m->state = 1;
            if (m->anim != 0) {
                blend_moby_animation(m, 0, 0, scale_game_frames(10));
            }
            try_start_kerwan_path_enemy_heading_turn(
                fast_arc_tan(selected_target.pos[0] - m->pos[0],
                             selected_target.pos[1] - m->pos[1]),
                (unsigned char *)m,
                (char *)&selected_target);
        }
        break;
    }
    case ENEMY_FOLLOW_PATH_TO_END: {
        {
            PathF2C8398 *pf = &moby_data->pf;
            float        v[4];
            int          seg;
            float        frac;
            FUN_L00_0025c698_cf(pf->path, m->pos, v, &seg, &frac, 0, 999.0f, 5.0f, 0.0f);
            if (pf->idx < seg) {
                pf->idx = seg;
            }
            if (!(FUN_L00_0025df68_cf((char *)m,
                                      (char *)&moby_data->mv,
                                      (float *)(pf->path + 0x10 + pf->idx * 0x10),
                                      (char *)moby_data->v40) &
                  0x14)) {
                break;
            }
            if (pf->idx < *(int *)pf->path - 1) {
                pf->idx++;
            } else {
                m->state = 1;
                if (m->anim != 0) {
                    blend_moby_animation(m, 0, 0, scale_game_frames(10));
                }
                try_start_kerwan_path_enemy_heading_turn(
                    fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                 selected_target.pos[1] - m->pos[1]),
                    (unsigned char *)m,
                    (char *)&selected_target);
            }
        }
        break;
    }
    case ENEMY_FOLLOW_ALTERNATE_PATH: {
        {
            char *path = ((char *)l03_paths[moby_data->i290]);
            float v[4];
            int   seg;
            float frac;
            FUN_L00_0025c698_cf(path, m->pos, v, &seg, &frac, 0, 999.0f, 5.0f, 0.0f);
            if (moby_data->i298 < seg) {
                moby_data->i298 = seg;
            }
            if (!(FUN_L00_0025df68_cf((char *)m,
                                      (char *)&moby_data->mv,
                                      (float *)(path + 0x10 + moby_data->i298 * 0x10),
                                      (char *)moby_data->v40) &
                  0x14)) {
                break;
            }
            if (moby_data->i298 < *(int *)path - 1) {
                moby_data->i298++;
            } else {
                moby_data->i290 = -1;
                m->state = 1;
                if (m->anim != 0) {
                    blend_moby_animation(m, 0, 0, scale_game_frames(10));
                }
                if (moby_data->i284 != -1) {
                    blend_moby_animation(m, 0, 0, scale_game_frames(10));
                    m->state = 2;
                } else {
                    try_start_kerwan_path_enemy_heading_turn(
                        fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                     selected_target.pos[1] - m->pos[1]),
                        (unsigned char *)m,
                        (char *)&selected_target);
                }
            }
        }
        break;
    }
    case ENEMY_KNOCKBACK_RECOVERY: {
        int   fl;
        float g;
        if (moby_data->q.s3E != 2) {
            fl = step_moby_motion(m, &moby_data->q);
        } else {
            m->f58 = 1.0f;
            fl = (m->b70 << 5) & 0x40;
        }
        moby_data->i290 = -1;
        if (fl & 0x40) {
            if (moby_data->i274 != 0) {
                Data2C8398 *d2 = m->data;
                if (d2->pf.idx < *(int *)d2->pf.path - 1) {
                    d2->pf.idx++;
                    m->state = 7;
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                } else {
                    m->state = 1;
                    if (m->anim != 0) {
                        blend_moby_animation(m, 0, 0, scale_game_frames(10));
                    }
                    try_start_kerwan_path_enemy_heading_turn(
                        fast_arc_tan(selected_target.pos[0] - m->pos[0],
                                     selected_target.pos[1] - m->pos[1]),
                        (unsigned char *)m,
                        (char *)&selected_target);
                }
            } else {
                is_value_within_interpolated_window_cf(m);
                m->state = 7;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
        } else if (m->pos[2] < 5.0f || (fl & 0x20)) {
            {
                float v[4];
                qcopy(v, m->pos);
                v[2] += 1.0f;
                spawn_explosion(m,
                                moby_data->v40,
                                v,
                                0.0f,
                                0.0f,
                                5,
                                2,
                                4,
                                2.0f,
                                1.0f,
                                9.0f,
                                6,
                                1.0f,
                                15.0f,
                                1,
                                1,
                                -1,
                                0);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D0,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D1,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D2,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00259710_cf(m, 0, -1);
                if (moby_data->i264 == -1) {
                    mark_moby_for_removal(m);
                    return;
                }
                *(OvlQuad *)m->pos = *(OvlQuad *)(moby_data->pf.path + 0x10);
                clear_u64_value(moby_data->v40);
                moby_data->mv.f18 = 0.0f;
                m->state = 0xE;
                m->flags |= 0x41;
                m->i94 = 0;
                return;
            }
        }
        m->pos[2] = m->pos[2] + 2.0f;
        g = allocate_voice_for_target_entry_cf(m->pos);
        m->pos[2] = m->pos[2] - 2.0f;
        if (g + 5.0f < m->pos[2]) {
            allocate_voice_for_target_entry(0xA, 0x21, (int)m);
            blend_moby_animation(m, 0xB, 0, scale_game_frames(10));
            m->state = 0xB;
        }
        break;
    }
    case ENEMY_FALL_UNDER_GRAVITY: {
        {
            Jump2C8398 *q = &moby_data->q;
            float       a[4];
            float       b[4];
            float       speed;
            float       want;
            q->f8 = q->f8 - q->f10;
            speed = vector_length_xy(q);
            want = 0.0f;
            if (q->s3E != 2) {
                want = q->f4C;
            }
            approach_value(&speed, want, q->f14);
            set_vector_length_xy(q, q, speed);
            qcopy(a, m->pos);
            a[2] += q->f28;
            fast_vec_add(m->pos, m->pos, q);
            qcopy(b, m->pos);
            b[2] += q->f28;
            if (fast_vec_length(q) > (float)q->i20) {
                collision_line(a, b, 0x24, m, 0);
                qcopy(b, &D_L03_00173F40.point);
            }
            if (coll_sphere(convert_integer_to_float(q->i20) * 0.0009765625f, b, 0x24, m)) {
                qcopy(m->pos, sphere_collision_resolved_position);
                m->pos[2] = a[2] + q->f8 - q->f28;
            }
            if (m->pos[2] < 5.0f) {
                {
                    float v[4];
                    qcopy(v, m->pos);
                    v[2] += 1.0f;
                    spawn_explosion(m,
                                    moby_data->v40,
                                    v,
                                    0.0f,
                                    0.0f,
                                    5,
                                    2,
                                    4,
                                    2.0f,
                                    1.0f,
                                    9.0f,
                                    6,
                                    1.0f,
                                    15.0f,
                                    1,
                                    1,
                                    -1,
                                    0);
                    FUN_L00_00257470_cf((char *)m,
                                        0x6D0,
                                        m->pos,
                                        m->rot,
                                        0,
                                        0,
                                        0.0f,
                                        D_L03_0015F580_macro,
                                        D_L03_0015F580_macro,
                                        D_L03_0015F580_macro);
                    FUN_L00_00257470_cf((char *)m,
                                        0x6D1,
                                        m->pos,
                                        m->rot,
                                        0,
                                        0,
                                        0.0f,
                                        D_L03_0015F580_macro,
                                        D_L03_0015F580_macro,
                                        D_L03_0015F580_macro);
                    FUN_L00_00257470_cf((char *)m,
                                        0x6D2,
                                        m->pos,
                                        m->rot,
                                        0,
                                        0,
                                        0.0f,
                                        D_L03_0015F580_macro,
                                        D_L03_0015F580_macro,
                                        D_L03_0015F580_macro);
                    FUN_L00_00259710_cf(m, 0x200, -1);
                    if (moby_data->i264 == -1) {
                        mark_moby_for_removal(m);
                        return;
                    }
                    *(OvlQuad *)m->pos = *(OvlQuad *)(moby_data->pf.path + 0x10);
                    clear_u64_value(moby_data->v40);
                    moby_data->mv.f18 = 0.0f;
                    m->state = 0xE;
                    m->flags |= 0x41;
                    m->i94 = 0;
                    return;
                }
            }
        }
    }
    case ENEMY_TURN_TO_HEADING:
        switch (m->anim) {
        case ENEMY_ANIM_LOCOMOTION:
            FUN_001f9c48_cf((char *)m, &moby_data->f258, moby_data->f28C, 0.09f, 0.27f, 0.15f);
            if (fast_difference_between_rotations(m->rot[2], moby_data->f28C) < 0.034906585f) {
                moby_data->b263 = scale_game_frames(0x3C);
                if (m->bBC >= 7 && m->bBC <= 8) {
                    if (m->bBC == 7) {
                        is_value_within_interpolated_window_cf(m);
                    }
                    m->state = m->bBC;
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                    m->f58 = 1.0f;
                } else {
                    m->state = 1;
                    if (m->anim != 0) {
                        blend_moby_animation(m, 0, 0, scale_game_frames(10));
                    }
                    moby_data->s250 = scale_game_frames(10);
                }
            }
            break;
        case ENEMY_ANIM_HEADING_TURN:
            FUN_001f9c48_cf((char *)m, &moby_data->f258, moby_data->f28C, 0.01f, 0.3f, 0.2f);
            if (fast_difference_between_rotations(m->rot[2], moby_data->f28C) < 0.034906585f) {
                moby_data->b263 = scale_game_frames(0x3C);
                if (m->bBC >= 7 && m->bBC <= 8) {
                    if (m->bBC == 7) {
                        is_value_within_interpolated_window_cf(m);
                    }
                    m->state = m->bBC;
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                } else {
                    m->state = 1;
                    if (m->anim != 0) {
                        blend_moby_animation(m, 0, 0, scale_game_frames(10));
                    }
                    moby_data->s250 = scale_game_frames(10);
                }
            }
            break;
        }
        break;
    case ENEMY_DEATH_KNOCKBACK: {
        int   fl = step_moby_motion(m, &moby_data->q);
        float g;
        moby_data->b255++;
        g = allocate_voice_for_target_entry_cf(m->pos);
        if ((fl & 0x60) && g + 5.0f < m->pos[2] && m->bBC != 0xB) {
            allocate_voice_for_target_entry(0xA, 0, (int)m);
            m->bBC = 0xB;
        }
        m->flags &= 0xEFFF;
        if (fl & 1) {
            moby_data->b255 = 0;
        }
        if (fl & 0x60) {
            {
                float v[4];
                qcopy(v, m->pos);
                v[2] += 1.0f;
                spawn_explosion(m,
                                moby_data->v40,
                                v,
                                0.0f,
                                0.0f,
                                5,
                                2,
                                4,
                                2.0f,
                                1.0f,
                                9.0f,
                                6,
                                1.0f,
                                15.0f,
                                1,
                                1,
                                -1,
                                0);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D0,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D1,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D2,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00259710_cf(m, 0, -1);
                if (moby_data->i264 == -1) {
                    mark_moby_for_removal(m);
                    return;
                }
                *(OvlQuad *)m->pos = *(OvlQuad *)(moby_data->pf.path + 0x10);
                clear_u64_value(moby_data->v40);
                moby_data->mv.f18 = 0.0f;
                m->state = 0xE;
                m->flags |= 0x41;
                m->i94 = 0;
                return;
            }
        }
        if (m->pos[2] < 5.0f) {
            FUN_L00_00259710_cf(m, 0, -1);
            if (moby_data->i264 == -1) {
                mark_moby_for_removal(m);
                return;
            }
            *(OvlQuad *)m->pos = *(OvlQuad *)(moby_data->pf.path + 0x10);
            clear_u64_value(moby_data->v40);
            moby_data->mv.f18 = 0.0f;
            m->state = 0xE;
            m->flags |= 0x41;
            m->i94 = 0;
            return;
        }
        break;
    }
    case ENEMY_HIDDEN_AT_PATH_START: {
        PathF2C8398 *pf = &moby_data->pf;
        if (m->anim != 0) {
            blend_moby_animation(m, 0, 0, scale_game_frames(10));
        }
        m->flags &= 0xEFFF;
        *(OvlQuad *)m->pos = *(OvlQuad *)(pf->path + 0x10);
        m->flags |= 0x41;
        m->i94 = 0;
        break;
    }
    case ENEMY_RESPAWN_AT_PATH_START:
        if ((visited52)[m->bB0 + (current_level_index << 4)] == 0xFF) {
            mark_moby_for_removal(m);
            return;
        }
        m->i94 = *(int *)(m->p24 + 0x10);
        m->flags = (m->flags & 0xFFBE) | 0x1000;
        moby_data->f20 = convert_integer_to_float(moby_data->s24);
        qcopy(m->pos, moby_data->pf.path + 0x10);
        m->state = 1;
        break;
    }
    if (m->b31 != 0) {
        if (fast_vec_dist(m->pos, D_L03_00166EC0) < 36.0f) {
            FUN_L00_00260400_cf(m);
        }
    }
    {
        int         idx = moby_data->i280;
        Moby2C8398 *o;
        if (idx == -1 || (o = &D_L03_0015FFD8_m[idx]) == 0 || o->state == 0xFE ||
            o->state == 0xFD) {
            moby_data->i280 = -1;
        }
    }
    if (moby_data->i280 != -1) {
        {
            float        out[4];
            Moby2C8398  *base;
            Moby2C8398  *other;
            PathF2C8398 *pf;
            int          i;
            *(OvlQuad *)out = 0;
            base = D_L03_0015FFD8_m;
            other = &base[moby_data->i280];
            pf = &moby_data->pf;
            if ((short)other->cls == 0x335 || (short)other->cls == 0x4BA) {
                for (i = 0; i < *(int *)pf->path; i++) {
                    float path_node_rotation[4];
                    *(OvlQuad *)path_node_rotation = 0;
                    probe_ground_height_cf((int)m,
                                           (char *)other,
                                           pf->path + 0x10 + i * 0x10,
                                           path_node_rotation,
                                           pf->path + 0x10 + i * 0x10,
                                           path_node_rotation);
                }
            }
            probe_ground_height_cf(
                (int)m, (char *)other, (char *)m->pos, m->rot, (char *)m->pos, out);
            m->rot[2] = out[2];
        }
    }
    if (!(m->state >= 0xB && m->state <= 0xE)) {
        float g;
        if (m->state == 0xA) {
            moby_data->mv.f18 = 0.0f;
        } else {
            moby_data->mv.f18 = moby_data->mv.f18 + frame_time_sq * 9.8f;
        }
        m->pos[2] = m->pos[2] - (moby_data->mv.f18 - 2.0f);
        g = probe_ground_height(m->pos, 0, 0.5f);
        m->pos[2] = m->pos[2] - 2.0f;
        if (m->pos[2] < g) {
            CollisionHit *hit = &D_L03_00173F40;
            if (hit->moby != 0 && FUN_L03_0024fba8_cf(hit->moby) != 0) {
                moby_data->i280 = (Moby2C8398 *)hit->moby - D_L03_0015FFD8_m;
            } else if (moby_data->i280 != -1) {
                short cls = D_L03_0015FFD8_m[moby_data->i280].cls;
                if (cls != 0x335 && cls != 0x4BA) {
                    moby_data->i280 = -1;
                }
            }
            m->pos[2] = g;
            moby_data->mv.f18 = 0.0f;
            if (moby_data->i280 != -1) {
                float a[4];
                float b[4];
                *(OvlQuad *)a = *(OvlQuad *)D_L03_00173F80;
                *(OvlQuad *)b = *(OvlQuad *)m->rot;
                axis_angle_rotation_to_euler(a, b, m->rot[2]);
            }
        } else if (g + 5.0f < m->pos[2]) {
            blend_moby_animation(m, 0xB, 0, scale_game_frames(10));
            m->state = 0xB;
            clear_u64_value(&moby_data->q);
            moby_data->q.f8 = -moby_data->mv.f18;
            moby_data->q.f10 = frame_time_sq * 19.6f;
        }
    }
    if (moby_data->b262 == 0) {
        if (vector_distance_xy(m->pos, D_L03_00166EC0) < 70.0f &&
            (D_L03_0015F5CC & 7) == moby_data->s28A) {
            PathF2C8398 *pf = &moby_data->pf;
            float        v[4];
            int          seg;
            float        frac;
            FUN_L00_0025c698_cf(pf->path, m->pos, v, &seg, &frac, 0, 999.0f, 5.0f, 0.0f);
            if (m->pos[2] < 5.0f || m->pos[2] < v[2] - 10.0f) {
                float burst_position[4];
                qcopy(burst_position, m->pos);
                burst_position[2] += 1.0f;
                spawn_explosion(m,
                                moby_data->v40,
                                burst_position,
                                0.0f,
                                0.0f,
                                5,
                                2,
                                4,
                                2.0f,
                                1.0f,
                                9.0f,
                                6,
                                1.0f,
                                15.0f,
                                1,
                                1,
                                -1,
                                0);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D0,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D1,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00257470_cf((char *)m,
                                    0x6D2,
                                    m->pos,
                                    m->rot,
                                    0,
                                    0,
                                    0.0f,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro,
                                    D_L03_0015F580_macro);
                FUN_L00_00259710_cf(m, 0x200, -1);
                if (moby_data->i264 == -1) {
                    mark_moby_for_removal(m);
                    return;
                }
                *(OvlQuad *)m->pos = *(OvlQuad *)(pf->path + 0x10);
                clear_u64_value(moby_data->v40);
                moby_data->mv.f18 = 0.0f;
                m->state = 0xE;
                m->flags |= 0x41;
                m->i94 = 0;
            }
        }
    }
}

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(float, void *, void *, int, int, int) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");

extern float D_L03_001618E0 __attribute__((sda));

#include "rnc/overlay/l03/mobys/kerwan_path_enemy_data/update_kerwan_path_enemy_hit_reaction.h"

extern int
fun_00213928(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_00213928");
extern void set_moby_animation(void *, int, int) __asm__("FUN_00212ed8");

/* Launch speed for a jump of height h at gravity g (per-frame units). */
#define JUMP_SPEED(g, h) square_root(2.0f * (g) * (frame_time_sq * (h)))

/* Hit reaction for class 574 (the path-walking enemy): takes damage from the hit, turns towards the
 * attacker (the hero when hit by the wrench or by nothing) and starts a knock-back jump whose size depends
 * on the hit kind; a lethal hit starts the death jump. */
void update_kerwan_path_enemy_hit_reaction(Moby2C8398 *moby) __asm__("FUN_L03_002c95a8");

void update_kerwan_path_enemy_hit_reaction(Moby2C8398 *moby) {
    enum {
        DEATH_KNOCKBACK_RESPONSE = 1,
        KNOCKBACK_WITHOUT_FACING_CHANGE_RESPONSE = 3,
        ALTERNATE_ANIMATION_KNOCKBACK_RESPONSE = 4,
        ANIMATION_SIX_KNOCKBACK_RESPONSE = 5,
        TURN_AWAY_KNOCKBACK_RESPONSE = 6,
        ANIMATION_FIVE_KNOCKBACK_RESPONSE = 7,
        ANIMATION_FIVE_KNOCKBACK_WITHOUT_FACING_CHANGE_RESPONSE = 8,
        NO_KNOCKBACK_HIT_RESPONSE = 12,
        ZERO_DAMAGE_NO_KNOCKBACK_RESPONSE = 13
    };
    float       hit_knockback_vector[4];
    int         kind;
    float       damage;
    float       angle;
    Data2C8398 *moby_data;
    char       *hit;
    char       *other;
    int         result;

    damage = 0.0f;
    moby_data = moby->data;
    if (moby_data->i38 != 0) {
        moby_data->i38 = 0;
        moby_data->s252 = scale_game_frames(240);
    }
    if (fast_dec_timer_s16(&moby_data->s252)) {
        moby_data->f270 = moby_data->f26C;
    } else {
        moby_data->f270 = moby_data->f26C + 5.0f;
    }
    hit = find_moby_hit(moby, 0x330000, 0);
    result = fun_00213928(moby, hit, &moby_data->f20, 0, &kind, &damage, 0, 4);
    if (hit != 0 && kind != 1 && moby->state != 0xD) {
        other = *(char **)(hit + 0x20);
        if (other != 0) {
            if (other != (char *)hero.items[0].moby) {
                angle = fast_arc_tan(moby->pos[0] - *(float *)(other + 0x10),
                                     moby->pos[1] - *(float *)(other + 0x14));
            } else {
                angle = fast_arc_tan(moby->pos[0] - hero.motion.pos.f[0],
                                     moby->pos[1] - hero.motion.pos.f[1]);
            }
        } else {
            angle = fast_arc_tan(moby->pos[0] - hero.motion.pos.f[0],
                                 moby->pos[1] - hero.motion.pos.f[1]);
        }
        moby_data->f20 -= damage;
        if (moby_data->f20 <= 0.0f) {
            result = 1;
        }
        switch (result) {
        case NO_KNOCKBACK_HIT_RESPONSE:
        case ZERO_DAMAGE_NO_KNOCKBACK_RESPONSE:
            moby_data->b67 = 0x78;
            break;
        case TURN_AWAY_KNOCKBACK_RESPONSE:
            moby_data->b67 = 0x78;
            moby_data->q.f10 = frame_time_sq * 80.0f;
            moby_data->q.f18 =
                (kerwan_path_enemy_knockback_horizontal_distance /
                     (2.0f * square_root(2.0f * D_L03_001618E0 * moby_data->q.f10) /
                      (frame_time_sq * 80.0f)) +
                 kerwan_path_enemy_knockback_horizontal_deceleration * frame_time_sq * 0.5f *
                     (2.0f * JUMP_SPEED(D_L03_001618E0, 80.0f) / (frame_time_sq * 80.0f))) *
                0.5f;
            moby_data->q.f1C = JUMP_SPEED(D_L03_001618E0, 80.0f);
            moby_data->q.i24 = 9;
            moby_data->q.b3D = 0;
            *(OvlQuad *)hit_knockback_vector = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(
                hit_knockback_vector, &angle, &moby_data->q.f18, &moby_data->q.f1C);
            if (fast_difference_between_rotations(moby->rot[2], angle) < 1.0471976f) {
                moby->rot[2] = fast_add_rotations(angle, 3.1415927f);
            }
            trooper_start_knockback(angle, moby, (char *)moby_data + 0x70, 5, 5, 0);
            set_moby_animation(moby, 5, 5);
            moby_data->q.f50 = 14.0f;
            moby_data->q.f54 = 28.0f;
            moby->state = 10;
            allocate_voice_for_target_entry(1, 0, (int)moby);
            break;
        case KNOCKBACK_WITHOUT_FACING_CHANGE_RESPONSE:
        case ANIMATION_FIVE_KNOCKBACK_RESPONSE:
        case ANIMATION_FIVE_KNOCKBACK_WITHOUT_FACING_CHANGE_RESPONSE:
            moby_data->b67 = 0x78;
            moby_data->q.f10 = frame_time_sq * 80.0f;
            moby_data->q.f14 = kerwan_path_enemy_knockback_horizontal_deceleration * frame_time_sq;
            moby_data->q.f18 =
                kerwan_path_enemy_knockback_horizontal_distance /
                    (2.0f * square_root(2.0f * D_L03_001618E0 * moby_data->q.f10) /
                     (frame_time_sq * 80.0f)) +
                kerwan_path_enemy_knockback_horizontal_deceleration * frame_time_sq * 0.5f *
                    (2.0f * JUMP_SPEED(D_L03_001618E0, 80.0f) / (frame_time_sq * 80.0f));
            moby_data->q.f1C = JUMP_SPEED(D_L03_001618E0, 80.0f);
            moby_data->q.i24 = 9;
            moby_data->q.b3D = 0;
            *(OvlQuad *)hit_knockback_vector = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(
                hit_knockback_vector, &angle, &moby_data->q.f18, &moby_data->q.f1C);
            trooper_start_knockback(angle, moby, (char *)moby_data + 0x70, 5, 5, 0);
            moby_data->q.f50 = 6.0f;
            moby_data->q.f54 = 13.0f;
            moby->state = 10;
            allocate_voice_for_target_entry(1, 0, (int)moby);
            break;
        case ALTERNATE_ANIMATION_KNOCKBACK_RESPONSE:
        case ANIMATION_SIX_KNOCKBACK_RESPONSE:
            moby_data->b67 = 0x78;
            moby_data->q.f10 = frame_time_sq * 80.0f;
            moby_data->q.f14 = kerwan_path_enemy_knockback_horizontal_deceleration * frame_time_sq;
            moby_data->q.f18 =
                kerwan_path_enemy_knockback_horizontal_distance /
                    (2.0f * square_root(2.0f * D_L03_001618E0 * moby_data->q.f10) /
                     (frame_time_sq * 80.0f)) +
                kerwan_path_enemy_knockback_horizontal_deceleration * frame_time_sq * 0.5f *
                    (2.0f * JUMP_SPEED(D_L03_001618E0, 80.0f) / (frame_time_sq * 80.0f));
            moby_data->q.f1C = JUMP_SPEED(D_L03_001618E0, 80.0f);
            moby_data->q.i24 = 9;
            moby_data->q.b3D = 0;
            *(OvlQuad *)hit_knockback_vector = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(
                hit_knockback_vector, &angle, &moby_data->q.f18, &moby_data->q.f1C);
            trooper_start_knockback(angle, moby, (char *)moby_data + 0x70, 6, 5, 2);
            moby_data->q.f50 = 5.0f;
            moby_data->q.f54 = 10.0f;
            moby->state = 10;
            allocate_voice_for_target_entry(1, 0, (int)moby);
            break;
        case DEATH_KNOCKBACK_RESPONSE:
            moby_data->q.f10 = frame_time_sq * 40.0f;
            moby_data->q.f14 = kerwan_path_enemy_knockback_horizontal_deceleration * frame_time_sq;
            moby_data->q.f18 =
                3.0f /
                    (2.0f * square_root(2.0f * 2.0f * moby_data->q.f10) / (frame_time_sq * 40.0f)) +
                kerwan_path_enemy_knockback_horizontal_deceleration * frame_time_sq * 0.5f *
                    (2.0f * JUMP_SPEED(2.0f, 40.0f) / (frame_time_sq * 40.0f));
            moby_data->q.f1C = JUMP_SPEED(2.0f, 40.0f);
            moby_data->q.i24 = 9;
            moby_data->q.b3D = 0;
            *(OvlQuad *)hit_knockback_vector = *(OvlQuad *)(hit + 0x10);
            trooper_knockback_arc(
                hit_knockback_vector, &angle, &moby_data->q.f18, &moby_data->q.f1C);
            trooper_start_knockback(angle, moby, (char *)moby_data + 0x70, 7, 5, 2);
            moby_data->q.f50 = 10.0f;
            moby_data->q.f54 = 20.0f;
            moby_data->q.f4C = 2.0f * frame_time;
            moby_data->b255 = 0;
            moby->flags &= 0xEFFF;
            moby_data->b67 = 0xFA;
            moby->state = 0xD;
            allocate_voice_for_target_entry(3, 0, (int)moby);
            if (moby->anim != 7) {
                blend_moby_animation(moby, 7, 0, scale_game_frames(6));
            }
            break;
        }
        trooper_anim_update(moby, (short *)((char *)moby_data + 0x60));
    }
    moby->bA4 = 0xFF;
    trooper_anim_finish(moby, (char *)moby_data + 0x60);
}

extern int FUN_L00_0025df68(void *, void *, void *, int *, float *, int, float, float, float);
extern int select_moby_target_with_hero_fallback(char *, char *, float) __asm__("FUN_L03_0024e830");

/* Picks the path node to head for next: walks the path ring from the start and keeps the node best aligned
 * with the target direction, else the one nearest the target (but at least 16 units away). */
void FUN_L03_002c9ca0(Moby2C8398 *moby) {
    Tgt2C8398    target;
    float        near[4];
    float        pt[4];
    int          seg;
    float        path_fraction_or_node_target_distance;
    Data2C8398  *moby_data;
    PathF2C8398 *pf;
    int          best;
    float        best_ang;
    float        best_dist;
    float        node_target_heading_difference;

    moby_data = moby->data;
    best = 0;
    best_ang = 0.0f;
    pf = &moby_data->pf;
    best_dist = best_ang;
    select_moby_target_with_hero_fallback((char *)moby, (char *)&target, moby_data->f270);
    seg = pf->idx;
    FUN_L00_0025df68(pf->path,
                     moby->pos,
                     near,
                     &seg,
                     &path_fraction_or_node_target_distance,
                     0,
                     999.0f,
                     5.0f,
                     0.0f);
    pf->idx = 0;
    do {
        if (pf->idx != seg) {
            qcopy(pt, pf->path + 0x10 + pf->idx * 0x10);
            node_target_heading_difference =
                fast_arc_tan(pt[0] - moby->pos[0], pt[1] - moby->pos[1]);
            node_target_heading_difference = fast_difference_between_rotations(
                node_target_heading_difference,
                fast_arc_tan(target.pos[0] - moby->pos[0], target.pos[1] - moby->pos[1]));
            path_fraction_or_node_target_distance = fast_vec_dist(&target, pt);
            if (node_target_heading_difference < 1.5707964f) {
                if (best_ang < node_target_heading_difference) {
                    best = pf->idx;
                    best_dist = path_fraction_or_node_target_distance;
                    best_ang = node_target_heading_difference;
                }
            } else if ((path_fraction_or_node_target_distance >= 16.0f &&
                        (path_fraction_or_node_target_distance < best_dist || best_dist < 16.0f)) ||
                       (path_fraction_or_node_target_distance < 16.0f &&
                        best_dist < path_fraction_or_node_target_distance)) {
                best = pf->idx;
                best_dist = path_fraction_or_node_target_distance;
                best_ang = node_target_heading_difference;
            }
        }
    } while ((pf->idx = (pf->idx + *(int *)pf->path + pf->step) % *(int *)pf->path) != 0);
    pf->idx = best;
}
