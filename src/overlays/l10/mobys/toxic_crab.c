/* Level 10 moby class toxic_crab: sight, targeting, group approach, particles and hit reactions. */
#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/rendering/effects/spawn_spark.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/l10/mobys/screamer.h"
#include "rnc/overlay/l10/mobys/aligned_vector.h"
#include "rnc/mobys/resolve_moby_hit_response.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"

typedef struct Moby Moby;
extern struct Moby *D_L10_0015FFD0_macro[] __asm__("D_L10_0015FFD0") MACRO_ADDR;
extern float        D_0015ED6C;
extern float        D_0015ED70;
extern void
spawn_color_swapped_explosion_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");
extern float D_0015ED6C, D_0015ED70;
extern void  trooper_anim_update(void *, void *) __asm__("FUN_L00_0025d458");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern int
activate_effect_l08(float, void *, void *, int, int, void *, int) __asm__("FUN_L00_0025ff38");

extern float  scale_time(float) __asm__("FUN_001f96b0");
extern short *moby_group_member_lists[] __asm__("D_L10_001ABCC0");

#include "rnc/overlay/quad.h"

extern float FUN_L04_001f1978(float);
extern void  trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void  trooper_anim_update(void *m, void *p) __asm__("FUN_L00_0025d458");

/* Update for the toxic crab (moby class 1202): line-of-sight test, then a 13-state walk/chase/attack/death machine. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_002E30F8.c: func_L10_002E30F8), where it is exact; names translated to the US level program. */

typedef struct {
    float          v[4];
    char          *m;
    int            flags;
    unsigned char  b0;
    unsigned char  b1;
    unsigned short id;
    float          f;
    int            n;
    int            pad[3];
} Hit2E30F8;

typedef struct Crab2E30F8 {
    char          p000[0x20];
    float         f20;
    short         s24;
    char          p26[2];
    unsigned char b28;
    unsigned char b29;
    char          p2A[6];
    float         f30;
    char          p34[0x24];
    unsigned char b58;
    char          p59;
    unsigned char b5A;
    char          p5B[5];
    char          q60[7];
    unsigned char b67;
    char          p68[8];
    float         v70[2];
    float         f78;
    char          p7C[4];
    float         f80;
    char          p84[0xC];
    int           i90;
    int           i94;
    float         f98;
    char          p9C[0x11];
    unsigned char bAD;
    char          pAE[0x12];
    float         fC0;
    float         fC4;
    char          pC8[8];
    int           iD0;
    float         fD4;
    float         fD8;
    float         fDC;
    char          pE0[0x14];
    float         fF4;
    char          pF8[0x28];
    float         f120;
    float         f124;
    char          p128[0x38];
    char         *tgt;
    int           i164;
    char          p168[8];
    float         v170[4];
    char          p180[0x10];
    float         f190;
    float         f194;
    float         f198;
    char          p19C[4];
    float         f1A0;
    float         f1A4;
    int           i1A8;
    char          p1AC[8];
    int           i1B4;
    int           i1B8;
    int           i1BC;
    float         v1C0[4];
    char          p1D0[8];
    int           i1D8;
    int           i1DC;
    char          p1E0[0xD0];
    unsigned char q2B0[4];
} Crab2E30F8;

typedef struct Moby2E30F8 {
    char           p00[0x10];
    float          pos[4];
    unsigned char  state;
    unsigned char  b21;
    char           p22[2];
    char          *p24;
    char           p28[4];
    float          f2C;
    char           p30;
    unsigned char  b31;
    char           p32[2];
    unsigned short s34;
    char           p36[0x12];
    float          f48;
    char           p4C[7];
    unsigned char  anim;
    char           p54[4];
    float          f58;
    char           p5C[0x14];
    unsigned char  b70;
    char           p71[7];
    Crab2E30F8    *data;
    char           p7C[3];
    unsigned char  b7F;
    char           p80[0x26];
    unsigned short cls;
    char           pA8[8];
    unsigned char  bB0;
    char           pB1;
    unsigned short id;
} Moby2E30F8;

extern char D_L10_00167240[];

#include "rnc/overlay/level_labels.h"

float compute_toxic_crab_group_approach_angle(struct Moby *self) __asm__("FUN_L10_002e4840");

extern float compute_toxic_crab_group_approach_angle(struct Moby *self) __asm__("FUN_L10_002e4840");
extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern float fast_cos(float);
extern float fast_sin(float);
extern int   D_L10_0015F5CC;
extern int   D_L10_0015FFD8;
extern int   FUN_L00_00258b50(void *, void *, void *, void *, float);
extern float l10_toxic_crab_size_scale __asm__("D_L10_00161E24") __attribute__((sda));

#include "rnc/overlay/l10/mobys/toxic_crab_data/toxic_crab_update_2.h"

extern f32 D_L10_00161E40_gp __asm__("D_L10_00161E40") __attribute__((sda));

#include "rnc/overlay/l10/mobys/toxic_crab_data/toxic_crab_update.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"

extern void *
FUN_L00_00263fd8(char *, int, float *, void *, int, int, float, float *, float *, float *);
extern void
collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_00258278(char *moby, float *vel, float target, float k, float d, float max);
extern void FUN_L00_0025a120(void *);
extern void FUN_L00_0025d458(void *m, short *p);
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);
extern void FUN_L01_0026d930(char *p);
extern void trooper_set_knockback(float *p, float a, float b) __asm__("FUN_L03_00250a78");
extern void FUN_L10_002e3a90_u(void *) __asm__("FUN_L10_002e3a90");
extern void spawn_type_52_particle_near_moby(char *moby) __asm__("FUN_L10_002e3de8");
extern void update_toxic_crab_hit_reactions_and_targeting(void *) __asm__("FUN_L10_002e3ef0");
extern char D_0014C190[];
extern void func_L00_0025D5B0_b(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern void func_L00_002592B0_b(
    char *moby, float target, float k, float d, float max, float *vel) __asm__("FUN_L00_00258278");

void         toxic_crab_update(Moby2E30F8 *m) __asm__("FUN_L10_002e1d38");
extern float D_L10_00161E40_gp __asm__("D_L10_00161E40") __attribute__((sda));
extern void  trooper_set_knockback(float *, float, float) __asm__("FUN_L03_00250a78");

/* Computes the aim angle of a moby relative to its owner, narrowed by nearby table entries. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_002E30F8.c: func_L10_002E5C00), where it is exact; names translated to the US level program. */

extern float fast_vec_dist(void *, void *);

/* Moby class 1202 */
void toxic_crab_update(Moby2E30F8 *m) {
    enum {
        TOXIC_CRAB_STATE_INITIALIZE = 0,
        TOXIC_CRAB_STATE_WAIT_FOR_ACTIVATION = 1,
        TOXIC_CRAB_STATE_ACTIVATE = 2,
        TOXIC_CRAB_STATE_FACE_TRACKED_POSITION = 3,
        TOXIC_CRAB_STATE_WAIT_TO_PURSUE = 4,
        TOXIC_CRAB_STATE_PURSUE_TARGET = 5,
        TOXIC_CRAB_STATE_PAUSE_PURSUIT = 6,
        TOXIC_CRAB_STATE_MELEE_ATTACK = 7,
        TOXIC_CRAB_STATE_RETURN_HOME = 8,
        TOXIC_CRAB_STATE_BACK_AWAY_FROM_TARGET = 9,
        TOXIC_CRAB_STATE_HIT_KNOCKBACK = 10,
        TOXIC_CRAB_STATE_DEATH_EFFECTS = 11,
        TOXIC_CRAB_STATE_FALL_TO_DEATH = 12
    };
    int         see = 0;
    Crab2E30F8 *crab_data = m->data;
    char       *target_moby;
    float       from[4];
    float       to[4];

    update_toxic_crab_hit_reactions_and_targeting(m);
    target_moby = crab_data->tgt;
    m->f2C = *(float *)(m->p24 + 0x24) * l10_toxic_crab_size_scale;
    FUN_L00_00263ac8(2.5f, (int)m, 2, crab_data->q2B0);
    *(OvlQuad *)from = *(OvlQuad *)m->pos;
    from[2] += 1.0f;
    *(OvlQuad *)to = *(OvlQuad *)&MOBY(target_moby)->pos;
    to[2] += 1.0f;
    if (fast_vec_dist(from, to) < 15.0f &&
        !(from[0] < 2.0f || 1021.0f < from[0] || from[1] < 2.0f || 1021.0f < from[1] ||
          from[2] < 2.0f || 1021.0f < from[2]) &&
        !(to[0] < 2.0f || 1021.0f < to[0] || to[1] < 2.0f || 1021.0f < to[1] || to[2] < 2.0f ||
          1021.0f < to[2])) {
        see = collision_line(from, to, 2, m, 0) == 0;
    }
    if (!see || (!(absolute_float(MOBY(target_moby)->pos.z - m->pos[2]) < 1.0f) &&
                 is_moby_in_class_range_500_to_540(((int)hero.ground_moby)) == 0)) {
        see = 0;
    }
    switch (m->state) {
    case TOXIC_CRAB_STATE_INITIALIZE:
        if (crab_data->i1DC != 0 && ((unsigned char *)&D_0013E533)[0x2EC1] == 1) {
            mark_moby_for_removal(m);
            return;
        }
        crab_data->f30 = 0.5f;
        crab_data->s24 = 5;
        crab_data->b29 = 1;
        crab_data->f20 = 5.0f;
        crab_data->b28 = 3;
        crab_data->b58 = truncate_float_to_s32(10.0f);
        crab_data->b5A = truncate_float_to_s32(16.0f);
        m->s34 |= 0x5000;
        qcopy(crab_data->v170, m->pos);
        crab_data->v170[3] = m->f48;
        FUN_L01_0026d930((char *)&crab_data->iD0);
        {
            float k = l10_toxic_crab_size_scale * 1024.0f;
            float initial_movement_step;
            float initial_movement_size_scale;
            int   r;
            crab_data->fDC = 0.5f;
            crab_data->fD8 = 2.0f;
            r = truncate_float_to_s32(k);
            initial_movement_step = crab_data->f1A4 * frame_time;
            initial_movement_size_scale = l10_toxic_crab_size_scale;
            crab_data->iD0 = r;
            crab_data->fD4 = initial_movement_size_scale;
            crab_data->fF4 = initial_movement_step;
        }
        {
            int k = crab_data->i1BC;
            if (k != -1) {
                Moby2E30F8 *p = (Moby2E30F8 *)((char *)D_L10_0015FFD0_macro[2] + (k << 8));
                if (p == 0 || p->state == 0xFE || p->state == 0xFD ||
                    (((*(int *)(((char *)&D_0014C190) +
                                (((short)p->id >> 5) * 4 + (current_level_index << 8))) >>
                       (p->id & 0x1F)) &
                      1) &&
                     visited52[p->bB0 + (current_level_index << 4)] == 0xFF)) {
                    crab_data->i1BC = -1;
                }
            }
        }
        m->state = 1;
        if (m->anim != 0) {
            blend_moby_animation(m, 0, 0, 0);
        }
        crab_data->f1A0 = *(float *)&D_L10_00161E40_gp;
        break;
    case TOXIC_CRAB_STATE_WAIT_FOR_ACTIVATION:
        FUN_L00_00258278(
            (char *)m, &crab_data->f190, crab_data->v170[3], 0.02f, 0.3f, frame_time * 6.2831855f);
        if (crab_data->i1BC != -1) {
            char       *moby_instance_array_base = D_L10_0015FFD8;
            Moby2E30F8 *p = (Moby2E30F8 *)(moby_instance_array_base + (crab_data->i1BC << 8));
            if (p == 0 || p->state == 0xFE || p->state == 0xFD ||
                ((*(int *)(((char *)&D_0014C190) +
                           (((short)p->id >> 5) * 4 + (current_level_index << 8))) >>
                  (p->id & 0x1F)) &
                 1)) {
                m->state = 2;
                crab_data->i1A8 = scale_game_frames(1);
                if (m->b21 != 0xFF) {
                    activate_waiting_moby_group_members((struct Moby *)m);
                }
            }
        }
        if (crab_data->i164 != 2 || crab_data->i1B4 != 0) {
            m->state = 2;
            crab_data->i1A8 = scale_game_frames(1);
            if (m->b21 != 0xFF) {
                activate_waiting_moby_group_members((struct Moby *)m);
            }
        }
        break;
    case TOXIC_CRAB_STATE_ACTIVATE:
        if (m->anim == 0) {
            if (crab_data->i1A8 == 0) {
                crab_data->i1A8 =
                    truncate_float_to_s32(FUN_L04_001f1978(random_float_between(0.0f, 20.0f)));
            } else if (fast_dec_timer(&crab_data->i1A8)) {
                if (crab_data->i1B4 != 0) {
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(10));
                    }
                } else {
                    if (m->anim != 2) {
                        blend_moby_animation(m, 2, 0, scale_game_frames(10));
                    }
                }
            }
        } else if (m->b70 & 2) {
            if (vector_distance_xy(m->pos, &hero.motion.pos) < *(float *)&D_L10_00161E40_gp) {
                float x;
                m->state = 5;
                crab_data->i1D8 = 0;
                qcopy(crab_data->v1C0, m->pos);
                crab_data->fF4 = 0.0f;
                x = crab_data->f198 * DEG_TO_RAD;
                crab_data->f194 = random_float_between(x, -x);
                if (m->anim != 3) {
                    blend_moby_animation(m, 3, 0, scale_game_frames(5));
                }
            } else {
                m->state = 3;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(10));
                }
            }
        }
        break;
    case TOXIC_CRAB_STATE_FACE_TRACKED_POSITION: {
        float ang = fast_arc_tan(crab_data->f120 - m->pos[0], crab_data->f124 - m->pos[1]);
        FUN_L00_00258278((char *)m, &crab_data->f190, ang, 0.02f, 0.3f, frame_time * 6.2831855f);
        if (crab_data->i164 != 2) {
            m->state = 4;
            crab_data->i1A8 =
                truncate_float_to_s32(random_float_between(0.0f, (float)crab_data->i1B8));
        }
        break;
    }
    case TOXIC_CRAB_STATE_WAIT_TO_PURSUE:
        if (fast_dec_timer(&crab_data->i1A8)) {
            float x;
            m->state = 5;
            crab_data->i1D8 = 0;
            qcopy(crab_data->v1C0, m->pos);
            crab_data->fF4 = 0.0f;
            x = crab_data->f198 * DEG_TO_RAD;
            crab_data->f194 = random_float_between(x, -x);
            if (m->anim != 3) {
                blend_moby_animation(m, 3, 0, scale_game_frames(5));
            }
        }
        break;
    case TOXIC_CRAB_STATE_PURSUE_TARGET: {
        {
            float dir[4];
            float out[4];
            float pre_movement_position[4];
            float v[4];
            float target_distance;
            float ang;
            float diff;
            float aim;

            target_distance = fast_vec_dist(m->pos, target_moby + 0x10);
            if (target_distance > (MOBY(target_moby)->oclass == 0x359 ? 2.0f : 3.57f)) {
                approach_value(&crab_data->fF4, crab_data->f1A4 * frame_time, frame_time_sq * 8.0f);
            } else {
                approach_value(
                    &crab_data->fF4, crab_data->f1A4 * frame_time * 0.3f, frame_time_sq * 20.0f);
            }
            *(OvlQuad *)pre_movement_position = *(OvlQuad *)m->pos;
            if (see &&
                absolute_float(target_distance -
                               (MOBY(target_moby)->oclass == 0x359 ? 2.0f : 3.57f)) < 0.35f) {
                approach_value(&crab_data->fF4, 0.0f, frame_time_sq * 30.0f);
            } else if (crab_data->i1D8 != 0) {
                approach_value(&crab_data->fF4, 0.0f, frame_time_sq * 10.0f);
            }
            ang = fast_arc_tan(MOBY(target_moby)->pos.x - m->pos[0],
                               MOBY(target_moby)->pos.y - m->pos[1]);
            diff = fast_difference_between_rotations(ang, m->f48);
            if (0.87266463f < diff) {
                float lim = crab_data->f1A4 * frame_time * 0.25f;
                if (lim < crab_data->fF4) {
                    crab_data->fF4 = lim;
                }
            }
            if (toxic_crab_pursuit_heading_threshold_degrees_gp * DEG_TO_RAD < diff ||
                crab_data->i1D8 == 0) {
                FUN_L00_00258278(
                    (char *)m, &crab_data->f190, ang, 0.02f, 0.3f, frame_time * 6.2831855f);
            }
            aim = compute_toxic_crab_group_approach_angle((struct Moby *)m);
            v[0] = fast_cos(aim) * (MOBY(target_moby)->oclass == 0x359 ? 2.0f : 3.57f);
            v[1] = fast_sin(aim) * (MOBY(target_moby)->oclass == 0x359 ? 2.0f : 3.57f);
            v[2] = 0.0f;
            fast_vec_add(v, v, target_moby + 0x10);
            if (hero.vehicle_mode != 1) {
                float rot = m->f48;
                int   r;
                m->f48 = fast_arc_tan(v[0] - m->pos[0], v[1] - m->pos[1]);
                dir[0] = fast_cos(m->f48) * 2.0f;
                dir[1] = fast_sin(m->f48) * 2.0f;
                dir[2] = 0.0f;
                clear_u64_value(out);
                {
                    float from2[4];
                    float to2[4];
                    *(OvlQuad *)from2 = *(OvlQuad *)m->pos;
                    from2[2] += 1.0f;
                    *(OvlQuad *)to2 = *(OvlQuad *)&MOBY(target_moby)->pos;
                    to2[2] += 1.0f;
                    if (collision_line(from2, to2, 2, m, 0)) {
                        char *h = ground_hit;
                        if ((*(char **)(h + 0x18) == 0 ||
                             *(short *)(*(char **)(h + 0x18) + 0xA6) != 0x31A) &&
                            fast_vec_dist(from2, h + 0x20) <
                                toxic_crab_obstacle_pause_distance_gp) {
                            m->state = 6;
                            crab_data->i1A8 = scale_game_frames(0x3C);
                            if (m->anim != 1) {
                                blend_moby_animation(m, 1, 0, scale_game_frames(7));
                            }
                        }
                    }
                }
                r = FUN_L00_00258b50(m, &crab_data->iD0, dir, out, 1.0f);
                m->f48 = rot;
                if (r & 3) {
                    if (vector_distance_xy(m->pos, pre_movement_position) <
                        crab_data->fF4 * 0.25f) {
                        crab_data->i1D8 = 1;
                    } else {
                        crab_data->i1D8 = 0;
                    }
                    if (D_L10_0015F5CC % 7 == 0 && crab_data->i1A8 == 0) {
                        if (vector_distance_xy(crab_data->v1C0, m->pos) < crab_data->fF4 &&
                            diff <= toxic_crab_pursuit_heading_threshold_degrees_gp * DEG_TO_RAD) {
                            m->state = 6;
                            crab_data->i1A8 = scale_game_frames(0x3C);
                            if (m->anim != 1) {
                                blend_moby_animation(m, 1, 0, scale_game_frames(7));
                            }
                        } else if (m->anim == 1) {
                            blend_moby_animation(m, 3, 0, scale_game_frames(7));
                        }
                        qcopy(crab_data->v1C0, m->pos);
                    }
                } else {
                    crab_data->i1D8 = 0;
                }
                if (m->state == 5 && fast_dec_timer(&crab_data->i1A8) && 0.0f < crab_data->fF4 &&
                    crab_data->i1D8 == 0 && m->anim == 1) {
                    blend_moby_animation(m, 3, 0, scale_game_frames(7));
                }
            } else {
                if (absolute_float(crab_data->f190) > frame_time * 0.08726646f) {
                    if (m->anim != 3) {
                        blend_moby_animation(m, 3, 0, scale_game_frames(7));
                    }
                } else {
                    if (m->anim != 1) {
                        blend_moby_animation(m, 1, 0, scale_game_frames(7));
                    }
                }
            }
            m->f58 = 1.2f;
            {
                float home = fast_vec_dist(m->pos, crab_data->v170);
                if ((hero.vehicle_mode == 1 ? 1.0f : 30.0f) < home || crab_data->i164 == 2) {
                    m->state = 8;
                    blend_moby_animation(m, 4, 0, scale_game_frames(0xC));
                } else if (vector_distance_xy(m->pos, target_moby + 0x10) <
                           (MOBY(target_moby)->oclass == 0x359 ? 2.45f : 4.02f)) {
                    if (hero.invulnerable_frames < scale_game_frames(10) && diff < 0.34906584f &&
                        see) {
                        int anim = 7;
                        if (random_integer_below(100) & 1) {
                            anim = 6;
                        }
                        m->state = 7;
                        blend_moby_animation(m, anim, 0, scale_game_frames(7));
                    }
                }
            }
        }
        break;
    }
    case TOXIC_CRAB_STATE_PAUSE_PURSUIT: {
        float diff;

        diff = fast_difference_between_rotations(fast_arc_tan(MOBY(target_moby)->pos.x - m->pos[0],
                                                              MOBY(target_moby)->pos.y - m->pos[1]),
                                                 m->f48);
        if (0.87266463f < diff) {
            float lim = crab_data->f1A4 * frame_time * 0.25f;
            if (lim < crab_data->fF4) {
                crab_data->fF4 = lim;
            }
        }
        if (fast_dec_timer(&crab_data->i1A8) ||
            toxic_crab_pursuit_heading_threshold_degrees_gp * DEG_TO_RAD < diff) {
            float from2[4];
            float to2[4];
            int   ok = 1;
            *(OvlQuad *)from2 = *(OvlQuad *)m->pos;
            from2[2] += 1.0f;
            *(OvlQuad *)to2 = *(OvlQuad *)&MOBY(target_moby)->pos;
            to2[2] += 1.0f;
            if (collision_line(from2, to2, 2, m, 0)) {
                char *h = ground_hit;
                if ((*(char **)(h + 0x18) == 0 ||
                     *(short *)(*(char **)(h + 0x18) + 0xA6) != 0x31A) &&
                    fast_vec_dist(from2, h + 0x20) < toxic_crab_obstacle_pause_distance_gp) {
                    ok = 0;
                }
            }
            if (ok) {
                m->state = 5;
                crab_data->i1D8 = 0;
                qcopy(crab_data->v1C0, m->pos);
                crab_data->fF4 = 0.0f;
                crab_data->i1A8 = scale_game_frames(0xF);
            }
        }
        {
            float home = fast_vec_dist(m->pos, crab_data->v170);
            if ((hero.vehicle_mode == 1 ? 1.0f : 30.0f) < home || crab_data->i164 == 2) {
                m->state = 8;
                blend_moby_animation(m, 4, 0, scale_game_frames(0xC));
            } else if (vector_distance_xy(m->pos, target_moby + 0x10) <
                       (MOBY(target_moby)->oclass == 0x359 ? 2.45f : 4.02f)) {
                if (hero.invulnerable_frames < scale_game_frames(10) &&
                    diff < toxic_crab_pursuit_heading_threshold_degrees_gp * DEG_TO_RAD && see) {
                    int anim = 7;
                    if (random_integer_below(100) & 1) {
                        anim = 6;
                    }
                    m->state = 7;
                    blend_moby_animation(m, anim, 0, scale_game_frames(7));
                }
            }
        }
    }
    case TOXIC_CRAB_STATE_MELEE_ATTACK: {
        float fr;
        float ang;

        fr = compute_interpolated_record_value(m);
        ang = fast_arc_tan(MOBY(target_moby)->pos.x - m->pos[0],
                           MOBY(target_moby)->pos.y - m->pos[1]);
        FUN_L00_00258278((char *)m, &crab_data->f190, ang, 0.03f, 0.3f, frame_time * 6.2831855f);
        if (m->anim >= 6 && m->anim <= 7 && 4.0f <= fr && fr <= 7.0f) {
            Hit2E30F8 hit;
            float     v[4];
            float     pos[4];
            *(OvlQuad *)pos = *(OvlQuad *)m->pos;
            pos[2] += 0.25f;
            hit.m = (char *)m;
            hit.flags = 0x10001;
            hit.f = 3.1f;
            hit.n = 1;
            hit.v[0] = fast_cos(m->f48);
            hit.v[1] = fast_sin(m->f48);
            hit.v[2] = 1.0f;
            hit.v[3] = 5627.925f;
            hit.b0 = 0;
            hit.b1 = 1;
            hit.id = m->cls;
            if (m->anim == 6) {
                moby_point_position(m, 0, v);
                collect_mobies_in_sphere(v, 0.75f, 0, m, &hit);
                v[2] += 0.5f;
                collect_mobies_in_sphere(v, 0.75f, 0, m, &hit);
                collision_line(pos, v, 9, m, &hit);
            } else {
                moby_point_position(m, 1, v);
                collect_mobies_in_sphere(v, 0.75f, 0, m, &hit);
                v[2] += 0.5f;
                collect_mobies_in_sphere(v, 0.75f, 0, m, &hit);
                collision_line(pos, v, 9, m, &hit);
            }
        } else if (m->anim >= 6 && m->anim <= 7 && (m->b70 & 2)) {
            float home = fast_vec_dist(m->pos, crab_data->v170);
            if ((((unsigned char *)&D_0013E533)[0x2EC1] == 1 ? 1.0f : 30.0f) < home ||
                crab_data->i164 == 2) {
                m->state = 8;
                blend_moby_animation(m, 4, 0, scale_game_frames(0xC));
            } else {
                m->state = 5;
                crab_data->i1D8 = 0;
                qcopy(crab_data->v1C0, m->pos);
                crab_data->fF4 = 0.0f;
                if (m->anim != 3) {
                    blend_moby_animation(m, 3, 0, scale_game_frames(7));
                }
            }
        }
        break;
    }
    case TOXIC_CRAB_STATE_BACK_AWAY_FROM_TARGET: {
        {
            float dir[4];
            float out[4];
            float rot;
            float ang;

            approach_value(
                &crab_data->fF4, crab_data->f1A4 * 3.0f * frame_time, frame_time_sq * 20.0f);
            ang = fast_arc_tan(MOBY(target_moby)->pos.x - m->pos[0],
                               MOBY(target_moby)->pos.y - m->pos[1]);
            FUN_L00_00258278(
                (char *)m, &crab_data->f190, ang, 0.02f, 0.3f, frame_time * 6.2831855f);
            rot = m->f48;
            m->f48 = fast_add_rotations(fast_arc_tan(MOBY(target_moby)->pos.x - m->pos[0],
                                                     MOBY(target_moby)->pos.y - m->pos[1]),
                                        3.14159f);
            dir[0] = fast_cos(m->f48) * 2.0f;
            dir[1] = fast_sin(m->f48) * 2.0f;
            dir[2] = 0.0f;
            FUN_L00_00258b50(m, &crab_data->iD0, dir, out, 1.0f);
            m->f48 = rot;
            if (see) {
                if (fast_dec_timer(&crab_data->i1A8) ||
                    toxic_crab_back_away_distance_limit_gp <
                        fast_vec_dist(m->pos, target_moby + 0x10)) {
                    int anim;
                    m->state = 7;
                    crab_data->fF4 = frame_time * 5.0f;
                    anim = 7;
                    if (random_integer_below(100) & 1) {
                        anim = 6;
                    }
                    blend_moby_animation(m, anim, 0, scale_game_frames(6));
                }
            }
        }
        break;
    }
    case TOXIC_CRAB_STATE_RETURN_HOME: {
        {
            float dir[4];
            float out[4];
            float ang;

            approach_value(&crab_data->fF4, crab_data->f1A4 * frame_time, frame_time_sq * 5.0f);
            ang = fast_arc_tan(crab_data->v170[0] - m->pos[0], crab_data->v170[1] - m->pos[1]);
            func_L00_002592B0_b(
                (char *)m, ang, 0.02f, 0.3f, frame_time * 6.2831855f, &crab_data->f190);
            if (0.87266463f < fast_difference_between_rotations(ang, m->f48)) {
                float lim = crab_data->f1A4 * frame_time * 0.25f;
                if (lim < crab_data->fF4) {
                    crab_data->fF4 = lim;
                }
            }
            dir[0] = fast_cos(m->f48) * 2.0f;
            dir[1] = fast_sin(m->f48) * 2.0f;
            dir[2] = 0.0f;
            FUN_L00_00258b50(m, &crab_data->iD0, dir, out, 1.0f);
            if (fast_vec_dist(m->pos, crab_data->v170) < 1.0f) {
                m->state = 3;
                if (m->anim != 1) {
                    blend_moby_animation(m, 1, 0, scale_game_frames(0x14));
                }
            } else if (crab_data->i164 != 2) {
                m->state = 5;
                crab_data->i1D8 = 0;
                qcopy(crab_data->v1C0, m->pos);
                crab_data->fF4 = 0.0f;
                if (m->anim != 3) {
                    blend_moby_animation(m, 3, 0, scale_game_frames(7));
                }
            }
        }
        break;
    }
    case TOXIC_CRAB_STATE_HIT_KNOCKBACK:
        if (step_moby_motion(m, crab_data->v70) & 0x41) {
            int anim;
            m->state = 7;
            crab_data->fF4 = frame_time * 5.0f;
            anim = 7;
            if (random_integer_below(100) & 1) {
                anim = 6;
            }
            blend_moby_animation(m, anim, 0, scale_game_frames(6));
        } else if (m->pos[2] < crab_data->v170[2] - 0.2f) {
            if (m->anim != 0xE) {
                blend_moby_animation(m, 0xE, 0, scale_game_frames(6));
            }
            m->state = 0xC;
            crab_data->f78 = crab_data->f78 / 3.0f;
        }
        break;
    case TOXIC_CRAB_STATE_FALL_TO_DEATH: {
        {
            float fall_motion_scratch[4];
            int   r;

            crab_data->f78 = crab_data->f78 - crab_data->f80;
            add_vectors(fall_motion_scratch, m->pos, crab_data->v70);
            if (m->pos[2] < probe_ground_height(m->pos, 0, 0.5f)) {
                int   knockback_collision_radius_fixed;
                float x;
                r = collision_hit_material();
                if (r == 0 || r == 1 || r == 3) {
                    FUN_L00_00257470(m, 0x200, -1);
                    mark_moby_for_removal(m);
                    return;
                }
                m->s34 &= 0xEFFF;
                m->state = 0xB;
                knockback_collision_radius_fixed = truncate_float_to_s32(768.0f);
                x = toxic_crab_death_knockback_gravity_gp * frame_time_sq;
                crab_data->bAD = 0;
                crab_data->i90 = knockback_collision_radius_fixed;
                crab_data->f98 = 0.75f;
                crab_data->i94 = 9;
                crab_data->f80 = x;
                trooper_set_knockback(crab_data->v70, 2.0f, 1.5f);
                crab_data->fC0 = 7.0f;
                crab_data->fC4 = 13.0f;
                func_L00_0025D5B0_b(
                    m, crab_data->v70, 0xD, 1, 0, fast_add_rotations(m->f48, 3.1415927f));
                crab_data->b67 = 0xFA;
                FUN_L00_0025d458(m, (short *)crab_data->q60);
                FUN_L00_00257470(m, 0, -1);
                break;
            }
        }
    }
    case TOXIC_CRAB_STATE_DEATH_EFFECTS: {
        {
            float p[4];

            {
                spawn_type_52_particle_near_moby((char *)m);
                {
                    float *dp = D_L10_0015F580;
                    float  zz = 0.0f;
                    char  *pb;
                    FUN_L10_002e3a90_u(m);
                    pb = (char *)m + 0x40;
                    FUN_L10_002e3a90_u(m);
                    spawn_color_swapped_explosion_burst(m, m->pos, -1, 1.25f, 20.0f);
                    FUN_L00_00263fd8((char *)m, 0x775, m->pos, pb, 0, 2, zz, dp, dp, dp);
                    FUN_L00_00263fd8((char *)m, 0x77A, m->pos, pb, 0, 2, zz, dp, dp, dp);
                    FUN_L00_00263fd8((char *)m, 0x77B, m->pos, pb, 0, 2, zz, dp, dp, dp);
                    FUN_L00_00263fd8((char *)m, 0x77C, m->pos, pb, 0, 2, zz, dp, dp, dp);
                    qcopy(p, m->pos);
                    p[2] += 0.55f;
                    p[0] += randf_sym(0.0f, 0.3f);
                    p[1] += randf_sym(0.0f, 0.3f);
                    FUN_L00_00263fd8((char *)m, 0x623, m->pos, pb, 0, 2, zz, dp, dp, dp);
                    FUN_L00_00263fd8((char *)m, 0x624, m->pos, pb, 0, 2, zz, dp, dp, dp);
                    FUN_L00_00257470(m, 0, -1);
                    mark_moby_for_removal(m);
                    return;
                }
            }
        }
    }
    }
    if (m->state == 3 || m->state == 5 || m->state == 7 || m->state == 8) {
        float ground_height = probe_ground_height(m->pos, 0, 0.5f);
        if (ground_height < m->pos[2]) {
            crab_data->f78 = crab_data->f78 - crab_data->f80;
            if (crab_data->f78 < frame_time * -7.0f) {
                crab_data->f78 = frame_time * -7.0f;
            }
            m->pos[2] = m->pos[2] + crab_data->f78;
            if (m->pos[2] < ground_height) {
                m->pos[2] = ground_height;
            }
            if (ground_height < crab_data->v170[2] - 2.0f) {
                if (m->anim != 0xE) {
                    blend_moby_animation(m, 0xE, 0, scale_game_frames(6));
                }
                m->state = 0xC;
            }
        }
    }
    if (m->b31 != 0) {
        if (fast_vec_dist(m->pos, D_L10_00167240) < 18.0f) {
            FUN_L00_0025a120(m);
            m->b7F = 0xE;
        }
    }
}

extern int   D_L10_00161E74 __attribute__((sda));
extern int   D_L10_00161E78 __attribute__((sda));
extern float D_L10_00161E7C __attribute__((sda));
extern float D_L10_00161E80 __attribute__((sda));
extern float D_L10_00161E84 __attribute__((sda));
extern float D_L10_00161E88 __attribute__((sda));
extern float D_L10_00161E8C __attribute__((sda));
extern float D_L10_00161E90 __attribute__((sda));
extern float D_L10_00161E94 __attribute__((sda));
extern float D_L10_00161E98 __attribute__((sda));
extern float D_L10_00161E9C __attribute__((sda));
extern float D_L10_00161EA0 __attribute__((sda));
extern int   D_L10_00161EA4 __attribute__((sda));
extern int   D_L10_00161EA8 __attribute__((sda));
extern int   D_L10_00161EAC __attribute__((sda));
extern int   D_L10_00161EB0 __attribute__((sda));
extern int   D_L10_00161EB4 __attribute__((sda));
extern int   D_L10_00161EB8 __attribute__((sda));
extern int   D_L10_00161EBC __attribute__((sda));
extern void  add_random_offset_to_vector_xyz(void *, float) __asm__("FUN_L00_0025f8e0");
extern int   fast_tween_color_3a90(int, int, float) __asm__("FUN_001fa6e0");

void emit_moby_particles(struct Moby *moby) __asm__("FUN_L10_002e3a90");

void emit_moby_particles(struct Moby *moby) {
    float  pos[4];
    float  vel[4];
    float  accel[4];
    float  push[4];
    char  *vars;
    int    count;
    float *pos_ptr;
    void  *src;
    int    i;
    int    j;
    float  angle;
    float  speed;
    float  rise;
    float  size;
    float  time_base;
    int    start_color;
    int    end_color;
    int    life;
    int    fade;

    count = D_L10_00161E74;
    vars = (char *)moby->pvars;
    for (i = 0; i < count; i++) {
        pos_ptr = pos;
        src = &moby->pos;
        qcopy_nc(pos, src);
        add_random_offset_to_vector_xyz(pos_ptr, D_L10_00161E7C);
        pos[2] += 0.55f;
        angle = random_angle_radians();
        speed = random_float_between(D_L10_00161E88, D_L10_00161E8C);
        rise = random_float_between(D_L10_00161E90, D_L10_00161E94);
        if (*((u8 *)moby + 0x20) == 11) {
            speed *= 0.55f;
            rise *= 0.55f;
            pos[2] -= 0.2f;
        }
        vel[0] = fast_cos(angle) * (speed * D_0015ED6C);
        vel[1] = fast_sin(angle) * (speed * D_0015ED6C);
        vel[2] = 0.0f + rise * D_0015ED6C;
        fast_vec_scale(push, vars + 0x70, 0.5f);
        push[2] = 0.0f;
        fast_vec_add(vel, vel, push);
        qcopy(accel, vel);
        accel[2] -= D_L10_00161E98 * D_0015ED70 * (float)scale_game_frames(D_L10_00161EB8);
        for (j = 0; j < D_L10_00161E78; j++) {
            size = random_float_between(0.5f, 1.5f);
            vel[3] = size * D_L10_00161E9C;
            accel[3] = size * D_L10_00161EA0;
            add_random_offset_to_vector_xyz(pos, D_L10_00161E80);
            add_random_offset_to_vector_xyz(accel, D_L10_00161E84 * D_0015ED6C);
            start_color = fast_tween_color_3a90(
                D_L10_00161EA4, D_L10_00161EAC, random_float_between(0.0f, 1.0f));
            end_color = fast_tween_color_3a90(
                D_L10_00161EA8, D_L10_00161EB0, random_float_between(0.0f, 1.0f));
            life = scale_game_frames(D_L10_00161EB4);
            fade = scale_game_frames(D_L10_00161EB8);
            time_base = (float)D_L10_00161EBC;
            spawn_spark(pos,
                        vel,
                        accel,
                        start_color,
                        end_color,
                        life,
                        fade,
                        truncate_float_to_s32(
                            scale_time(random_float_between(time_base * 0.5f, time_base * 2.5f))),
                        -1);
        }
    }
}
/* Level 10 toxic crab: per-frame sight, target and state update. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_002E30F8.c: func_L10_002E52B0), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef int u128_2E52B0 __attribute__((mode(TI)));

typedef struct D_2E52B0 {
    char          p0[0x10];
    struct Moby  *f10;
    char          p14[0xC];
    float         f20;
    char          p24[2];
    short         h26;
    char          p28[0x10];
    int           f38;
    char          p3C[0x24];
    char          a60[7];
    unsigned char b67;
    char          p68[8];
    float         a70[4];
    float         f80;
    char          p84[4];
    float         f88;
    float         f8C;
    int           f90;
    int           f94;
    float         f98;
    char          p9C[0x11];
    unsigned char bAD;
    char          pAE[0x12];
    float         fC0;
    float         fC4;
    char          pC8[0x58];
    AlignedVec4f  a120;
    char          p130[0x30];
    struct Moby  *f160;
    int           f164;
    char          p168[8];
    AlignedVec4f  a170;
    char          p180[0x20];
    float         f1A0;
    char          p1A4[8];
    int           f1AC;
    int           f1B0;
    char          p1B4[0x20];
    int           f1D4;
} D_2E52B0;

typedef struct Hit_2E52B0 {
    char           p0[0x10];
    AlignedVec4f   pos;
    Moby          *f20;
    char           p24[4];
    unsigned char  b28;
    char           p29;
    unsigned short h2A;
} Hit_2E52B0;

#include "rnc/overlay/l10/mobys/toxic_crab_globals.h"

extern float toxic_crab_hit_knockback_gravity_gp __asm__("D_L10_00161E2C") __attribute__((sda));
extern void  func_L10_002E14F8_2E52B0(void *) __asm__("FUN_L10_002e0138");
extern float func_001F9878_2E52B0(float) __asm__("FUN_001f96b0");
extern void
func_L00_0025D5B0_2E52B0(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern int          func_L00_00260D30_2E52B0(void *, void *, float) __asm__("FUN_L01_00274b78");
extern AlignedVec4f D_L10_00174380_2E52B0 __asm__("D_L10_00174300");

void update_toxic_crab_hit_reactions_and_targeting(void *mv) {
    enum {
        NO_STATE_TRANSITION_HIT_RESPONSE = 0,
        LETHAL_HIT_RESPONSE = 1,
        DEATH_HIT_RESPONSE = 2,
        KNOCKBACK_HIT_RESPONSE = 3,
        VEHICLE_SCALED_KNOCKBACK_HIT_RESPONSE = 4,
        VEHICLE_DEPENDENT_KNOCKBACK_HIT_RESPONSE = 5,
        FIXED_STRENGTH_KNOCKBACK_HIT_RESPONSE = 6,
        TIMED_KNOCKBACK_HIT_RESPONSE = 7,
        ANIMATED_KNOCKBACK_HIT_RESPONSE = 8,
        HIT_DIRECTED_KNOCKBACK_HIT_RESPONSE = 9,
        ARC_ADJUSTED_KNOCKBACK_HIT_RESPONSE = 10,
        SUPPRESSED_HIT_RESPONSE = 11
    };
    Moby        *moby = mv;
    AlignedVec4f vA;
    AlignedVec4f vB;
    AlignedVec4f W;
    AlignedVec4f X;
    AlignedVec4f hit_to_moby_offset;
    int          A;
    float        B;
    float        fx;
    D_2E52B0    *moby_data = (D_2E52B0 *)moby->pvars;
    Hit_2E52B0  *hit_record;
    Moby        *hit_moby;
    int          hit_response_code;

    if (((Hero *)(((char *)&hero)))->items[0].item_id == 0x15) {
        if (((D_2E52B0 *)((Hero *)(((char *)&hero)))->items[0].moby->pvars)->f10 == moby) {
            func_L10_002E14F8_2E52B0(moby);
            if (moby->state == 1)
                moby->state = 2;
        }
    }
    if (moby_data->f38 != 0) {
        func_L10_002E14F8_2E52B0(moby);
        moby_data->f1AC =
            truncate_float_to_s32(func_001F9878_2E52B0(random_float_between(180.0f, 240.0f)));
        moby_data->f38 = 0;
    }
    fast_dec_timer(&moby_data->f1AC);
    A = 1;
    B = 0.0f;
    hit_record = find_moby_hit(moby, 0xB30000, 0);
    hit_response_code =
        resolve_moby_hit_response(moby, hit_record, &moby_data->f20, 0, &A, &B, 0, 4);
    fast_dec_timer_s16(&moby_data->h26);
    if ((unsigned int)A >= 2 && moby->state != 0xB && moby->state != 0xC) {
        if (current_level_index == 10 && hit_record != 0 && hit_record->f20 != 0 &&
            hit_record->f20->oclass == 0x31A) {
            if (moby_data->f1AC != 0 && item_unlocked[0x11] == 0) {
                item_unlocked[0x11] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
            }
        }
        if (moby_data->h26 != 0 && hit_record->b28 == 0) {
            hit_response_code = 11;
            if (B < 3.0f)
                B = 0.0f;
        }
        if (((Hero *)(((char *)&hero)))->state.current == 32 ||
            (((Hero *)(((char *)&hero)))->state.prev == 32 &&
             ((Hero *)(((char *)&hero)))->state_timer < scale_game_frames(30))) {
            if (2.0f < B)
                B = 2.0f;
        }
        if (hit_record->h2A == moby->oclass) {
            B = 0.0f;
            hit_response_code = 11;
        }
        if ((unsigned int)(moby->prev_seq - 6) < 2) {
            if (((Hero *)(((char *)&hero)))->vehicle_mode == 1 &&
                hit_record->f20 == ((Hero *)(((char *)&hero)))->moby) {
                B = 0.0f;
                hit_response_code = 11;
            }
        }
        if (hit_record->h2A == 0x47) {
            *(u128_2E52B0 *)&vA = *(u128_2E52B0 *)(((char *)&hero) + 0x80);
            *(u128_2E52B0 *)&vB = *(u128_2E52B0 *)((AlignedVec4f *)&moby->pos);
            vA.z += 1.0f;
            vB.z += 1.0f;
            if (collision_line(&vA, &vB, 2, moby, 0) != 0) {
                hit_response_code = 11;
                B = 0.0f;
            }
        } else if ((hit_moby = hit_record->f20) != 0 && hit_moby->oclass != 0x359) {
            *(u128_2E52B0 *)&vA = *(u128_2E52B0 *)((AlignedVec4f *)&hit_moby->pos);
            *(u128_2E52B0 *)&vB = *(u128_2E52B0 *)((AlignedVec4f *)&moby->pos);
            vB.z += 1.0f;
            if (hit_record->f20->oclass == 0xBA || hit_record->f20->oclass == 0)
                vA.z += 0.25f;
            if (collision_line(&vA, &vB, 0x12, moby, 0) != 0) {
                fast_vec_sub(&hit_to_moby_offset, &vB, &vA);
                *(u128_2E52B0 *)&W = *(u128_2E52B0 *)&hit_to_moby_offset;
                fast_vec_normalize(&W, &W, 1.0f);
                fast_vec_normalize(&X, &D_L10_00174380_2E52B0, 1.0f);
                if (0.5f < absolute_float(fast_vec_dot(&W, &X))) {
                    B = 0.0f;
                    hit_response_code = 11;
                }
            }
        }
        moby_data->f20 -= B;
        if (moby_data->f20 <= 0.0f)
            hit_response_code = 1;
        func_L10_002E14F8_2E52B0(moby);
        moby_data->f90 = truncate_float_to_s32(768.0f);
        moby_data->f98 = 0.75f;
        moby_data->f94 = 9;
        moby_data->bAD = 0;
        fx = fast_arc_tan(hit_record->pos.x, hit_record->pos.y);
        switch (hit_response_code) {
        case KNOCKBACK_HIT_RESPONSE:
        case FIXED_STRENGTH_KNOCKBACK_HIT_RESPONSE:
        case TIMED_KNOCKBACK_HIT_RESPONSE:
        case ANIMATED_KNOCKBACK_HIT_RESPONSE:
        case HIT_DIRECTED_KNOCKBACK_HIT_RESPONSE:
        case ARC_ADJUSTED_KNOCKBACK_HIT_RESPONSE:
            moby->state = 10;
            moby_data->h26 = scale_game_frames(60);
            moby_data->f80 = toxic_crab_hit_knockback_gravity_gp * D_0015ED70;
            trooper_set_knockback(moby_data->a70, 0.5f, 0.5f);
            moby_data->fC0 = 8.0f;
            moby_data->fC4 = 10.0f;
            *(u128_2E52B0 *)&vA = *(u128_2E52B0 *)&hit_record->pos;
            trooper_knockback_arc(&vA, &fx, &moby_data->f88, &moby_data->f8C);
            func_L00_0025D5B0_2E52B0(moby, moby_data->a70, 9, 1, 0, fx);
            moby_data->b67 = 0xFA;
            trooper_anim_update(moby, moby_data->a60);
            break;
        case VEHICLE_SCALED_KNOCKBACK_HIT_RESPONSE:
        case VEHICLE_DEPENDENT_KNOCKBACK_HIT_RESPONSE: {
            float knockback_strength;
            moby->state = 10;
            moby_data->h26 = scale_game_frames(60);
            moby_data->f80 = toxic_crab_hit_knockback_gravity_gp * D_0015ED70;
            knockback_strength = ((Hero *)(((char *)&hero)))->vehicle_mode == 1 ? 1.0f : 3.0f;
            trooper_set_knockback(moby_data->a70, knockback_strength, 0.5f);
            moby_data->fC0 = 3.5f;
            moby_data->fC4 = 7.0f;
            *(u128_2E52B0 *)&vA = *(u128_2E52B0 *)&hit_record->pos;
            trooper_knockback_arc(&vA, &fx, &moby_data->f88, &moby_data->f8C);
            func_L00_0025D5B0_2E52B0(
                moby, moby_data->a70, 9, 1, 0, fast_arc_tan(hit_record->pos.x, hit_record->pos.y));
            moby_data->b67 = 0xFA;
            trooper_anim_update(moby, moby_data->a60);
            break;
        }
        case LETHAL_HIT_RESPONSE:
        case DEATH_HIT_RESPONSE:
            moby->state = 11;
            moby->flags &= 0xEFFF;
            break;
        case NO_STATE_TRANSITION_HIT_RESPONSE:
        case SUPPRESSED_HIT_RESPONSE:
            break;
        }
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, moby_data->a60);
    if ((unsigned char)(moby->state - 10) < 3) {
        moby_data->f1D4 = 0;
        return;
    }
    if (moby_data->f1AC != 0) {
        moby_data->f1A0 = D_L10_00161E40_gp + 6.0f;
    } else if (moby->state == 1) {
        if (moby == ((Moby *)((Hero *)(((char *)&hero)))->unk240) ||
            moby == ((Moby *)((Hero *)(((char *)&hero)))->coll_hit_moby)) {
            moby_data->f1A0 = 6.0f;
        } else {
            moby_data->f1A0 = 0.0f;
        }
        if (((Hero *)(((char *)&hero)))->vehicle_mode == 1)
            func_L10_002E14F8_2E52B0(moby);
    } else {
        moby_data->f1A0 = D_L10_00161E40_gp;
    }
    if (moby_data->f1AC != 0) {
        func_L00_00260D30_2E52B0(moby, &moby_data->a120, moby_data->f1A0);
        if (moby_data->f1A0 < vector_distance_xy(&moby_data->a170, &moby_data->a120) ||
            3.0f < absolute_float(moby->pos.z - moby_data->a120.z)) {
            moby_data->f164 = 2;
        }
    } else if (moby_data->f1A0 < vector_distance_xy(&moby_data->a170, &moby_data->a120) ||
               3.0f < absolute_float(moby->pos.z - moby_data->a120.z)) {
        func_L00_00260D30_2E52B0(moby, &moby_data->a120, moby_data->f1A0);
        moby_data->f164 = 2;
    } else {
        activate_effect_l08(moby_data->f1A0,
                            moby,
                            &moby_data->a120,
                            0,
                            0,
                            l10_paths[moby_data->f1B0]->points,
                            l10_paths[moby_data->f1B0]->count);
    }
    if (moby->state != 1 && moby_data->f164 == 2) {
        moby_data->f1D4++;
        if (scale_game_frames(300) < moby_data->f1D4) {
            if (1.0f < fast_vec_dist(&moby->pos, &moby_data->a170)) {
                moby->state = 8;
                if (moby->prev_seq != 4)
                    blend_moby_animation(moby, 4, 0, scale_game_frames(12));
            } else if (((Hero *)(((char *)&hero)))->vehicle_mode != 1) {
                moby->state = 1;
                if (moby->prev_seq != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(20));
            }
        }
    } else {
        moby_data->f1D4 = 0;
    }
    if (moby_data->f160 == 0)
        moby_data->f160 = ((Hero *)(((char *)&hero)))->moby;
}

extern float toxic_crab_group_approach_angle_limit_degrees_gp __asm__("D_L10_00161E48")
    __attribute__((sda));

float compute_toxic_crab_group_approach_angle(struct Moby *toxic_crab) {
    char  *pursuit_target = *(char **)(toxic_crab->pvars + 0x160);
    float  hi, lo, owner_to_self_angle, owner_to_self_distance, translated_radial_point[3];
    short *group_entry;
    if (toxic_crab->group == 0xFF) {
        return fast_arc_tan(toxic_crab->pos.x - *(float *)(pursuit_target + 0x10),
                            toxic_crab->pos.y - *(float *)(pursuit_target + 0x14));
    }
    lo = -(toxic_crab_group_approach_angle_limit_degrees_gp * DEG_TO_RAD);
    hi = toxic_crab_group_approach_angle_limit_degrees_gp * DEG_TO_RAD;
    owner_to_self_angle = fast_arc_tan(toxic_crab->pos.x - *(float *)(pursuit_target + 0x10),
                                       toxic_crab->pos.y - *(float *)(pursuit_target + 0x14));
    owner_to_self_distance = fast_vec_dist(pursuit_target + 0x10, &toxic_crab->pos);
    translated_radial_point[0] = fast_cos(owner_to_self_angle) * 2.0f;
    translated_radial_point[1] = fast_sin(owner_to_self_angle) * 2.0f;
    translated_radial_point[2] = 0.0f;
    fast_vec_add(
        translated_radial_point, translated_radial_point, ((unsigned char *)&D_0013E533) + 0xE9D);
    group_entry = moby_group_member_lists[toxic_crab->group];
    if (group_entry != 0) {
        do {
            int listed_entity_index = *(unsigned short *)group_entry & 0x7FFF;
            if (D_L10_0015FFD0_macro[2][listed_entity_index].state == 5 ||
                fast_vec_dist(&D_L10_0015FFD0_macro[2][listed_entity_index].pos.x,
                              pursuit_target + 0x10) < owner_to_self_distance) {
                float d = fast_arc_tan(D_L10_0015FFD0_macro[2][listed_entity_index].pos.x -
                                           *(float *)(pursuit_target + 0x10),
                                       D_L10_0015FFD0_macro[2][listed_entity_index].pos.y -
                                           *(float *)(pursuit_target + 0x14));
                float group_angle_offset = fast_subtract_rotations(d, owner_to_self_angle);
                if (group_angle_offset > 0.0f && group_angle_offset < hi) {
                    hi = group_angle_offset;
                } else if (group_angle_offset < 0.0f && group_angle_offset > lo) {
                    lo = group_angle_offset;
                }
            }
        } while (*group_entry++ >= 0);
    }
    return fast_add_rotations(
        fast_add_rotations(lo, fast_difference_between_rotations(hi, lo) * 0.5f),
        owner_to_self_angle);
}
