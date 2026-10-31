/* Level 01 moby class peckbot. */
#include "types.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"

#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"

#include "rnc/overlay/level_path.h"
#include "rnc/overlay/level_labels.h"

extern float D_L01_0015F580[] MACRO_ADDR;
extern float FUN_L04_001f1978(float);
extern void  FUN_L00_00257470(void *, int, int);
extern void  trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void  trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void  select_moby_combat_target(void *, void *, float) __asm__("FUN_L00_0025fcb8");
extern void  blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");

#include "qcopy.h"

typedef union {
    u128 q;
    f32  f[4];
} Vec577;

typedef struct {
    u8  pad0[0xA6];
    u16 oclass;
} Mob577Class;

typedef struct {
    u8           pad0[0x10];
    u128         pos;
    Mob577Class *cls;
    u8           pad24[0x8];
    f32          dmg;
} Mob577Hit;

typedef struct {
    u8  pad0[0x10];
    s32 id;
} Mob577Ref;

typedef struct Mob577 {
    u8             pad0[0x10];
    Vec577         pos;
    u8             state;
    u8             group;
    u8             pad22[0x2];
    Mob577Ref     *ref;
    struct Mob577 *next;
    u8             pad2c[0x8];
    u16            flags;
    u8             pad36[0xA];
    Vec577         rot;
    u8             pad50[0x2];
    u8             anim_next;
    u8             anim;
    u8             pad54[0x1C];
    u8             anim_flags;
    u8             pad71[0x7];
    void          *vars;
    u8             pad7c[0x18];
    s32            id;
    u8             pad98[0xC];
    u8             xA4;
    u8             padA5;
    s16            oclass;
    u8             padA8[0x14];
    u8             hit;
} Mob577;

typedef struct MobyNode {
    u8               pad0[0x10];
    Vec577           pos;
    s8               state;
    u8               pad21[0x7];
    struct MobyNode *next;
    u8               pad2c[0x7A];
    s16              oclass;
} MobyNode;

typedef struct {
    u8      pad0[0x20];
    f32     health;
    s16     x24;
    s16     flee_timer;
    u8      pad28;
    u8      x29;
    u8      pad2a[0xE];
    s32     x38;
    u8      pad3c[0x4];
    Vec577  vel;
    u8      pad50[0x8];
    u8      x58;
    u8      pad59;
    u8      x5A;
    u8      pad5b[0x6D];
    s16     xC8;
    u8      padca[0x6];
    void   *xD0;
    u8      padd4[0x3C];
    u8      fx110[0x7];
    u8      x117;
    u8      pad118[0x8];
    u8      knock[0x10];
    f32     x130;
    f32     x134;
    f32     x138;
    f32     x13C;
    u8      pad140[0x4];
    s32     x144;
    u8      pad148[0x15];
    u8      x15D;
    u8      pad15e[0x12];
    f32     x170;
    f32     x174;
    u8      pad178[0x8];
    s32     x180;
    u8      pad184[0x4];
    f32     x188;
    f32     x18C;
    u8      pad190[0x8];
    f32     x198;
    u8      pad19c[0x8];
    f32     x1A4;
    u8      pad1a8[0x28];
    Vec577  home;
    Vec577  dest;
    u8      turn[0x4];
    f32     turn_off;
    f32     turn_deg;
    f32     range;
    u8      pad200[0x4];
    f32     speed;
    s16     x208;
    s16     x20A;
    s32     idle_timer;
    s32     turn_timer;
    s16     x214;
    s16     x216;
    Mob577 *leader;
    s32     path;
    u8      pad220[0x8];
    s32     x228;
    u8      pad22c[0x4];
    u8      x230[0x10];
} Mob577Vars;

typedef struct {
    Vec577 pos;
    u8     pad10[0x30];
    void  *p40;
    u8     pad44[0xC];
} Mob577Target;

#include "rnc/overlay/l01/mobys/peckbot_data/peckbot_update.h"

s32       D_L01_00161A70 = 0x9020202;
extern u8 D_L01_00161A80[] MACRO_ADDR;

extern s32  rand() __asm__("FUN_001160d8");
extern f32  FUN_L04_001f1978(f32);
extern f32  FUN_0020c9e0(void *);
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern f32  FUN_00213508(void *, s32, f32);
extern f32  approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern s32  coll_sphere(void *, s32, void *, f32) __asm__("FUN_L00_001f0d60");
extern s32  FUN_L00_001f35d8_cf(void) __asm__("FUN_001f0b58");
extern void FUN_L00_001ff500_cf(void *, void *, float) __asm__("FUN_001f9bf8");
extern void FUN_L00_00257470(void *, s32, s32);
extern void FUN_L00_00258278(void *, void *, f32, f32, f32, f32);
extern s32  FUN_L00_00258b50(void *, void *, void *, void *, f32);
extern void FUN_L00_0025a120(void *);
extern void
hit_reaction(void *, void *, void *, s32, void *, void *, s32, s32) __asm__("FUN_L00_0025a478");
extern f32  FUN_L00_0025abf0(void *, void *, void *, f32, f32);
extern void trooper_start_knockback(void *, void *, s32, s32, s32, f32) __asm__("FUN_L00_0025c558");
extern void FUN_L00_0025f730(void *, f32);
extern void select_moby_combat_target(void *, void *, f32) __asm__("FUN_L00_0025fcb8");
extern void FUN_L00_00263ac8(void *, s32, void *, f32);
extern void FUN_L00_00263fd8(void *, s32, void *, void *, s32, s32, f32, void *, void *, void *);
extern s32  FUN_L00_002dc9f0(void *, void *);
extern void FUN_L01_0026d930(void *);
extern void FUN_L01_0026e090(s32, s32);
extern void
           select_list_point_toward_target(void *, s32, void *, void *) __asm__("FUN_L01_0028b5e0");
extern s32 reserve_expired_frame_slot(void *, s32, s32) __asm__("FUN_L01_002efbf8");

#define SET_ANIM(m, a, b, c) \
    if ((m)->anim != (a))    \
    blend_moby_animation((m), (a), (b), (c))

void peckbot_update(Mob577 *self) __asm__("FUN_L01_002efc60");

/* Moby class 577 */
void peckbot_update(Mob577 *peckbot_moby) {
    enum {
        PECKBOT_STATE_INITIALIZE = 0,
        PECKBOT_STATE_GROUNDED_IDLE = 1,
        PECKBOT_STATE_FACE_AWAY_FROM_TARGET = 2,
        PECKBOT_STATE_CHASE_TARGET = 3,
        PECKBOT_STATE_ATTACK_TARGET = 4,
        PECKBOT_STATE_RETURN_HOME = 5,
        PECKBOT_STATE_NONFATAL_KNOCKBACK = 6,
        PECKBOT_STATE_WAIT_FOR_CLOSE_TARGET = 8,
        PECKBOT_STATE_FACE_HOME_AND_WAIT = 10,
        PECKBOT_STATE_HOME_TRIGGERED_KNOCKBACK = 11,
        PECKBOT_STATE_RECOVER_FROM_BLOCKED_MOVEMENT = 12,
        PECKBOT_STATE_MOVE_FORWARD_NEAR_LEADER = 13,
        PECKBOT_STATE_AIRBORNE_IDLE = 14,
        PECKBOT_STATE_MOVE_TO_DESTINATION = 15,
        PECKBOT_STATE_ASCEND = 16,
        PECKBOT_STATE_DESCEND = 17,
        PECKBOT_STATE_FATAL_KNOCKBACK = 0x63
    };
    Vec577       oldpos;
    Mob577Target target;
    Vec577       vector_scratch;
    Vec577       position_scratch;
    Vec577       home_to_target;
    s32          hitA0;
    f32          hitA4;
    f32          angle;
    f32          knockback_travel_time;
    Mob577Vars  *peckbot_vars;
    Mob577Hit   *hit;
    MobyNode    *candidate_moby;
    void        *target_moby;
    Vec577      *mp;
    Vec577      *np;
    s32          movement_result_flags;
    s32          recovery_movement_result_flags;
    s32          frame_count_or_animation_id;
    f32          hp;
    f32          rad;
    f32          idle_destination_azimuth;
    f32          descent_ground_height;
    f32          ascent_ground_height;
    f32          chase_target_height_distance_ratio;
    f32          attack_ground_height;
    f32          t99;
    f32          recovery_target_height_distance_ratio;
    f32          nearby_moby_heading_difference;
    f32          turn;
    f32          z;
    f32          forward_movement_ground_height;
    f32          desired_heading;

    peckbot_vars = peckbot_moby->vars;
    FUN_L00_00263ac8(peckbot_moby, 0, peckbot_vars->x230, 1.5f);
    if (cutscene_mode == 2) {
        peckbot_moby->id = 0;
        peckbot_moby->flags |= 0x41;
        return;
    }
    peckbot_moby->id = peckbot_moby->ref->id;
    peckbot_moby->flags &= ~0x41;
    select_moby_combat_target(peckbot_moby, &target, peckbot_vars->range + 4.0f);
    target_moby = target.p40;
    qcopy(&oldpos, &peckbot_moby->pos);
    if (peckbot_vars->x38 != 0) {
        peckbot_vars->x214 = scale_game_frames(0xF0);
    }
    peckbot_vars->x38 = 0;
    fast_dec_timer_s16(&peckbot_vars->x208);
    if (fast_dec_timer_s16(&peckbot_vars->x214) != 0) {
        peckbot_vars->range = 12.0f;
    } else {
        peckbot_vars->range = 24.0f;
    }
    hitA4 = 0.0f;
    hit = find_moby_hit(peckbot_moby, 0x330000, 0);
    hit_reaction(peckbot_moby, hit, &peckbot_vars->health, 0, &hitA0, &hitA4, 0, 4);
    fast_dec_timer_s16(&peckbot_vars->flee_timer);
    if (peckbot_vars->flee_timer < scale_game_frames(0x2D) && hit != NULL &&
        peckbot_moby->state != 0x63) {
        if ((hit->cls->oclass == 0xB0 || hit->cls->oclass == 0xB1) &&
            peckbot_vars->flee_timer != 0) {
            hit->dmg = 0.0f;
        }
        if (peckbot_moby->group != 0xFF) {
            FUN_L01_0026e090(peckbot_moby->group, 1);
        }
        if (hit->dmg != 0.0f) {
            angle = fast_arc_tan(peckbot_moby->pos.f[0] - target.pos.f[0],
                                 peckbot_moby->pos.f[1] - target.pos.f[1]);
            hp = peckbot_vars->health - hit->dmg;
            peckbot_vars->x130 = 0.008f;
            peckbot_vars->x134 = 0.0005f;
            peckbot_vars->x138 = peckbot_nonfatal_knockback_horizontal_speed_gp * frame_time;
            peckbot_vars->x13C = peckbot_nonfatal_knockback_vertical_speed_gp * frame_time;
            peckbot_vars->x144 = 9;
            peckbot_vars->health = hp;
            peckbot_vars->x15D = 0;
            if (hp <= 0.0f) {
                peckbot_moby->flags &= ~0x1000;
                peckbot_vars->x13C = peckbot_vars->x138 = frame_time * 11.0f;
                vector_scratch.q = hit->pos;
                trooper_knockback_arc(
                    &vector_scratch, &angle, &peckbot_vars->x138, &peckbot_vars->x13C);
                trooper_start_knockback(peckbot_moby, peckbot_vars->knock, 7, 1, 0, angle);
                peckbot_vars->x170 = 11.0f;
                peckbot_vars->x174 = 18.0f;
                peckbot_moby->state = 0x63;
                peckbot_vars->x117 = 0x78;
                trooper_anim_update(peckbot_moby, peckbot_vars->fx110);
                FUN_L00_00257470(peckbot_moby, 0, -1);
            } else {
                vector_scratch.q = hit->pos;
                trooper_knockback_arc(
                    &vector_scratch, &angle, &peckbot_vars->x138, &peckbot_vars->x13C);
                trooper_start_knockback(peckbot_moby, peckbot_vars->knock, 6, 1, 0, angle);
                peckbot_vars->x170 = 5.0f;
                peckbot_vars->x174 = 10.0f;
                peckbot_moby->state = 6;
                peckbot_vars->x117 = 0xFA;
                peckbot_vars->flee_timer = scale_game_frames(0x3C);
                trooper_anim_update(peckbot_moby, peckbot_vars->fx110);
            }
        }
    }
    peckbot_moby->xA4 = 0xFF;
    switch (peckbot_moby->state) {
    case PECKBOT_STATE_INITIALIZE:
        qcopy(&peckbot_vars->home, &peckbot_moby->pos);
        peckbot_vars->x29 = 0;
        if (peckbot_vars->x20A != 0) {
            peckbot_moby->state = 1;
            SET_ANIM(peckbot_moby, 4, 0, 0);
        } else {
            peckbot_moby->state = 14;
            peckbot_moby->pos.f[2] = peckbot_moby->pos.f[2] + 6.0f;
            SET_ANIM(peckbot_moby, 0, 0, 0);
        }
        peckbot_vars->x214 = 0;
        peckbot_vars->x208 = 0;
        peckbot_vars->health = 1.0f;
        peckbot_vars->x24 = 1;
        peckbot_vars->x58 = 8;
        peckbot_vars->x5A = 4;
        FUN_L01_0026d930(&peckbot_vars->x180);
        peckbot_vars->x180 = 0x38D;
        peckbot_vars->x18C = 2.0f;
        peckbot_vars->x188 = 2.0f;
        peckbot_vars->x1A4 = peckbot_vars->speed * frame_time;
        if (rand() & 1) {
            peckbot_moby->flags |= 0x8000;
        }
        peckbot_vars->xD0 = &D_L01_00161A70;
        break;
    case PECKBOT_STATE_AIRBORNE_IDLE:
        fast_vec_sub(&vector_scratch, &target.pos, &peckbot_moby->pos);
        if (fast_dec_timer(&peckbot_vars->x228) != 0 &&
            fast_vec_length(&vector_scratch) < peckbot_vars->range + 4.0f &&
            absolute_float(peckbot_moby->pos.f[2] - target.pos.f[2]) < 8.0f &&
            random_integer_below(0x13) == 0) {
            peckbot_moby->state = 17;
        } else if (peckbot_vars->x228 == 0 && peckbot_moby->hit == 1 &&
                   random_integer_below(0x13) == 0) {
            if (random_integer_below(0x100) & 1) {
                peckbot_vars->turn_deg = -peckbot_vars->turn_deg;
            }
            peckbot_vars->turn_off = peckbot_vars->turn_deg * DEG_TO_RAD;
            peckbot_vars->turn_timer = scale_game_frames(random_float_between(30.0f, 90.0f));
            peckbot_moby->state = 17;
            clear_u64_value(&peckbot_vars->vel);
            peckbot_moby->hit = 0;
        } else if (fast_dec_timer(&peckbot_vars->idle_timer) != 0) {
            peckbot_vars->idle_timer = scale_game_frames(0x78);
            rad = random_float_between(0.0f, 3.0f);
            idle_destination_azimuth = random_angle_radians();
            position_scratch.f[0] = fast_cos(idle_destination_azimuth) * rad;
            position_scratch.f[1] = fast_sin(idle_destination_azimuth) * rad;
            position_scratch.f[2] = 0.0f;
            position_scratch.f[2] = random_float_between(5.0f, 7.0f);
            fast_vec_add(&peckbot_vars->dest, &position_scratch, &peckbot_vars->home);
            peckbot_moby->state = 15;
        }
        break;
    case PECKBOT_STATE_MOVE_TO_DESTINATION:
        desired_heading = fast_arc_tan(peckbot_vars->dest.f[0] - peckbot_moby->pos.f[0],
                                       peckbot_vars->dest.f[1] - peckbot_moby->pos.f[1]);
        FUN_L00_00258278(peckbot_moby, peckbot_vars->turn, desired_heading, 0.05f, 0.3f, 0.2f);
        fast_vec_sub(&peckbot_vars->vel, &peckbot_vars->dest, &peckbot_moby->pos);
        FUN_L00_0025f730(&peckbot_vars->vel, frame_time * 4.0f);
        fast_vec_add(&vector_scratch, &peckbot_vars->vel, &peckbot_moby->pos);
        if (coll_sphere(&vector_scratch, 0, peckbot_moby, 0.4f) != 0) {
            clear_u64_value(&peckbot_vars->vel);
            peckbot_moby->state = 14;
        } else {
            qcopy(&peckbot_moby->pos, &vector_scratch);
        }
        if (fast_vec_length(&peckbot_vars->vel) == 0.0f) {
            peckbot_moby->state = 14;
        }
        break;
    case PECKBOT_STATE_DESCEND:
        descent_ground_height = FUN_00213508(&peckbot_moby->pos, 0, 0.5f);
        approach_value(&peckbot_vars->vel.f[2], -(frame_time * 8.0f), frame_time_sq * 10.8f);
        peckbot_moby->pos.f[2] = peckbot_moby->pos.f[2] + peckbot_vars->vel.f[2];
        if (peckbot_moby->anim != 1 && peckbot_moby->pos.f[2] - descent_ground_height <=
                                           frame_time * 8.0f * FUN_L04_001f1978(45.0f)) {
            SET_ANIM(peckbot_moby, 1, 0, scale_game_frames(5));
        }
        if (peckbot_moby->pos.f[2] <= descent_ground_height) {
            peckbot_moby->pos.f[2] = descent_ground_height;
            if (random_integer_below(0x100) & 1) {
                peckbot_vars->turn_deg = -peckbot_vars->turn_deg;
            }
            peckbot_vars->turn_off = peckbot_vars->turn_deg * DEG_TO_RAD;
            peckbot_vars->turn_timer = scale_game_frames(random_float_between(30.0f, 90.0f));
            peckbot_moby->state = 3;
            clear_u64_value(&peckbot_vars->vel);
            SET_ANIM(peckbot_moby, 2, 0, scale_game_frames(3));
        }
        break;
    case PECKBOT_STATE_ASCEND:
        ascent_ground_height = FUN_00213508(&peckbot_moby->pos, 0, 0.5f);
        approach_value(&peckbot_vars->vel.f[2], frame_time * 8.0f, frame_time_sq * 10.8f);
        peckbot_moby->pos.f[2] += peckbot_vars->vel.f[2];
        if (ascent_ground_height + 6.0f <= peckbot_moby->pos.f[2]) {
            peckbot_moby->pos.f[2] = ascent_ground_height + 6.0f;
            peckbot_moby->state = 14;
            peckbot_vars->x228 = scale_game_frames(0xB4);
            clear_u64_value(&peckbot_vars->vel);
        }
        break;
    case PECKBOT_STATE_GROUNDED_IDLE:
        fast_vec_sub(&vector_scratch, &target.pos, &peckbot_moby->pos);
        if (fast_vec_length(&vector_scratch) < peckbot_vars->range + 4.0f &&
            absolute_float(peckbot_moby->pos.f[2] - target.pos.f[2]) < 8.0f &&
            random_integer_below(0x13) == 0) {
            peckbot_moby->state = 2;
            SET_ANIM(peckbot_moby, 5, 0, scale_game_frames(3));
        } else if (peckbot_moby->hit == 1 && random_integer_below(0x13) == 0) {
            if (random_integer_below(0x100) & 1) {
                peckbot_vars->turn_deg = -peckbot_vars->turn_deg;
            }
            peckbot_vars->turn_off = peckbot_vars->turn_deg * DEG_TO_RAD;
            peckbot_vars->turn_timer = scale_game_frames(random_float_between(30.0f, 90.0f));
            peckbot_moby->state = 3;
            SET_ANIM(peckbot_moby, 2, 0, scale_game_frames(3));
            peckbot_moby->hit = 0;
        } else if (fast_vec_length(&vector_scratch) > peckbot_vars->range + 6.0f &&
                   peckbot_vars->x20A == 0) {
            peckbot_moby->state = 16;
            SET_ANIM(peckbot_moby, 0, 0, scale_game_frames(3));
        }
        break;
    case PECKBOT_STATE_WAIT_FOR_CLOSE_TARGET:
        fast_vec_sub(&vector_scratch, &target.pos, &peckbot_moby->pos);
        if (fast_vec_length(&vector_scratch) < peckbot_vars->range - 4.0f &&
            absolute_float(peckbot_moby->pos.f[2] - target.pos.f[2]) < 8.0f &&
            random_integer_below(0x13) == 0) {
            peckbot_moby->state = 2;
            peckbot_moby->pos.f[2] = peckbot_moby->pos.f[2] + 1.0f;
            SET_ANIM(peckbot_moby, 5, 0, scale_game_frames(3));
        }
        break;
    case PECKBOT_STATE_FACE_HOME_AND_WAIT:
        desired_heading = fast_arc_tan(peckbot_vars->home.f[0] - peckbot_moby->pos.f[0],
                                       peckbot_vars->home.f[1] - peckbot_moby->pos.f[1]);
        FUN_L00_00258278(peckbot_moby, peckbot_vars->turn, desired_heading, 0.05f, 0.3f, 0.2f);
        fast_vec_sub(&vector_scratch, &target.pos, &peckbot_vars->home);
        if ((fast_vec_length(&vector_scratch) < peckbot_vars->range &&
             absolute_float(peckbot_vars->home.f[2] - target.pos.f[2]) < 8.0f) ||
            peckbot_moby->hit == 2) {
            if (peckbot_moby->group != 0xFF) {
                FUN_L01_0026e090(peckbot_moby->group, 2);
            }
            SET_ANIM(peckbot_moby, 6, 0, scale_game_frames(3));
            peckbot_vars->x134 = 0.0f;
            peckbot_vars->x130 = peckbot_home_knockback_acceleration_gp;
            peckbot_vars->x138 = peckbot_home_knockback_speed_gp * frame_time;
            peckbot_vars->x13C = FUN_L00_0025abf0(&peckbot_moby->pos,
                                                  &peckbot_vars->home,
                                                  &knockback_travel_time,
                                                  peckbot_vars->x138,
                                                  -peckbot_home_knockback_acceleration_gp);
            peckbot_vars->x144 = 13;
            peckbot_vars->x15D = 0;
            trooper_start_knockback(
                peckbot_moby, peckbot_vars->knock, 6, 1, 0, peckbot_moby->rot.f[2]);
            peckbot_vars->x170 = 5.0f;
            peckbot_vars->x174 = 10.0f;
            peckbot_moby->state = 11;
        }
        break;
    case PECKBOT_STATE_HOME_TRIGGERED_KNOCKBACK:
        if (step_moby_motion(peckbot_moby, peckbot_vars->knock) & 1) {
            peckbot_moby->state = 3;
            if (random_integer_below(0x100) & 1) {
                peckbot_vars->turn_deg = -peckbot_vars->turn_deg;
            }
            peckbot_vars->turn_off = peckbot_vars->turn_deg * DEG_TO_RAD;
            peckbot_vars->turn_timer = scale_game_frames(random_float_between(30.0f, 90.0f));
            SET_ANIM(peckbot_moby, 2, 8, scale_game_frames(3));
            return;
        } else if (peckbot_moby->pos.f[2] < 5.0f) {
            FUN_L00_00257470(peckbot_moby, 0, -1);
            mark_moby_for_removal(peckbot_moby);
            return;
        }
        break;
    case PECKBOT_STATE_FACE_AWAY_FROM_TARGET:
        desired_heading = fast_arc_tan(peckbot_moby->pos.f[0] - target.pos.f[0],
                                       peckbot_moby->pos.f[1] - target.pos.f[1]);
        FUN_L00_00258278(peckbot_moby, peckbot_vars->turn, desired_heading, 0.02f, 0.3f, 0.1f);
        fast_vec_sub(&vector_scratch, &target.pos, &peckbot_moby->pos);
        if (fast_vec_length(&vector_scratch) < peckbot_vars->range &&
            absolute_float(peckbot_moby->pos.f[2] - target.pos.f[2]) < 8.0f &&
            random_integer_below(0x13) == 0) {
            if (peckbot_moby->group != 0xFF) {
                FUN_L01_0026e090(peckbot_moby->group, 1);
            }
            if (random_integer_below(0x100) & 1) {
                peckbot_vars->turn_deg = -peckbot_vars->turn_deg;
            }
            peckbot_vars->turn_off = peckbot_vars->turn_deg * DEG_TO_RAD;
            peckbot_vars->turn_timer = scale_game_frames(random_float_between(30.0f, 90.0f));
            peckbot_moby->state = 3;
            SET_ANIM(peckbot_moby, 2, 0, scale_game_frames(3));
            peckbot_moby->hit = 0;
        } else if (peckbot_moby->hit == 1 && random_integer_below(0x13) == 0) {
            if (random_integer_below(0x100) & 1) {
                peckbot_vars->turn_deg = -peckbot_vars->turn_deg;
            }
            peckbot_vars->turn_off = peckbot_vars->turn_deg * DEG_TO_RAD;
            peckbot_vars->turn_timer = scale_game_frames(random_float_between(30.0f, 90.0f));
            peckbot_moby->state = 3;
            SET_ANIM(peckbot_moby, 2, 0, scale_game_frames(3));
            peckbot_moby->hit = 0;
        } else if (fast_vec_length(&vector_scratch) > peckbot_vars->range + 6.0f &&
                   peckbot_vars->x20A == 0) {
            peckbot_moby->state = 16;
            SET_ANIM(peckbot_moby, 0, 0, scale_game_frames(3));
        }
        if (peckbot_moby->anim == 5 && (peckbot_moby->anim_flags & 2)) {
            blend_moby_animation(peckbot_moby, 4, 0, scale_game_frames(3));
        }
        break;
    case PECKBOT_STATE_CHASE_TARGET:
        chase_target_height_distance_ratio =
            absolute_float(peckbot_moby->pos.f[2] - target.pos.f[2]);
        chase_target_height_distance_ratio = chase_target_height_distance_ratio /
                                             vector_distance_xy(&peckbot_moby->pos, &target.pos);
        if (peckbot_vars->path != -1) {
            select_list_point_toward_target(
                peckbot_moby, ((int)l01_paths[peckbot_vars->path]), &target.pos, &position_scratch);
        } else {
            qcopy(&position_scratch, &target.pos);
        }
        desired_heading = fast_arc_tan(position_scratch.f[0] - peckbot_moby->pos.f[0],
                                       position_scratch.f[1] - peckbot_moby->pos.f[1]) +
                          peckbot_vars->turn_off;
        FUN_L00_00258278(peckbot_moby, peckbot_vars->turn, desired_heading, 0.05f, 0.3f, 0.2f);
        vector_scratch.f[0] = fast_cos(peckbot_moby->rot.f[2]) * 2.0f;
        vector_scratch.f[1] = fast_sin(peckbot_moby->rot.f[2]) * 2.0f;
        vector_scratch.f[2] = 0.0f;
        movement_result_flags = FUN_L00_00258b50(
            peckbot_moby, &peckbot_vars->x180, &vector_scratch, &position_scratch, 1.0f);
        if (fast_dec_timer(&peckbot_vars->turn_timer) != 0) {
            peckbot_vars->turn_timer = scale_game_frames(random_float_between(30.0f, 90.0f));
            peckbot_vars->turn_off = -peckbot_vars->turn_off;
        }
        fast_vec_sub(&home_to_target, &target.pos, &peckbot_vars->home);
        if (vector_distance_xy(&peckbot_moby->pos, &target.pos) < 1.5f &&
            !(movement_result_flags & 2)) {
            peckbot_moby->state = 4;
            SET_ANIM(peckbot_moby, 3, 0, scale_game_frames(3));
        } else if (chase_target_height_distance_ratio > 1.0f || (movement_result_flags & 2)) {
            peckbot_moby->pos.f[0] = oldpos.f[0];
            peckbot_moby->pos.f[1] = oldpos.f[1];
            peckbot_moby->state = 12;
            SET_ANIM(peckbot_moby, 5, 0, scale_game_frames(3));
        } else if (fast_vec_length(&home_to_target) < peckbot_vars->range + 4.0f &&
                   absolute_float(peckbot_moby->pos.f[2] - target.pos.f[2]) < 8.0f) {
            if (peckbot_moby->group != 0xFF) {
                FUN_L01_0026e090(peckbot_moby->group, 1);
            }
        } else if (peckbot_moby->hit != 1) {
            peckbot_moby->state = 5;
        }
        FUN_00213508(&peckbot_moby->pos, 0, 0.5f);
        if (FUN_L00_001f35d8_cf() == 0 || FUN_L00_001f35d8_cf() == 8) {
            peckbot_moby->state = 16;
            SET_ANIM(peckbot_moby, 0, 0, scale_game_frames(3));
        }
        peckbot_moby->hit = 0;
        break;
    case PECKBOT_STATE_ATTACK_TARGET:
        desired_heading = fast_arc_tan(target.pos.f[0] - peckbot_moby->pos.f[0],
                                       target.pos.f[1] - peckbot_moby->pos.f[1]);
        FUN_L00_00258278(peckbot_moby, peckbot_vars->turn, desired_heading, 0.05f, 0.3f, 0.2f);
        if (peckbot_moby->anim_next == peckbot_moby->anim && FUN_0020c9e0(peckbot_moby) == 13.0f &&
            absolute_float(peckbot_moby->pos.f[2] - target.pos.f[2]) < 0.5f &&
            vector_distance_xy(&peckbot_moby->pos, &target.pos) < 2.0f &&
            fast_difference_between_rotations(
                fast_arc_tan(target.pos.f[0] - peckbot_moby->pos.f[0],
                             target.pos.f[1] - peckbot_moby->pos.f[1]),
                peckbot_moby->rot.f[2]) < 0.2617994f) {
            vector_scratch.f[0] = fast_cos(peckbot_moby->rot.f[2]) * 0.2f;
            vector_scratch.f[1] = fast_sin(peckbot_moby->rot.f[2]) * 0.2f;
            vector_scratch.f[2] = 0.0f;
            qcopy(&position_scratch, &target.pos);
            position_scratch.f[2] = position_scratch.f[2] + 0.75f;
            if (target_moby != NULL) {
                spawn_nearby_effect(
                    target_moby, peckbot_moby, 1, &position_scratch, &vector_scratch, 1.0f);
            }
        }
        if ((peckbot_moby->anim_flags & 2) &&
            vector_distance_xy(&peckbot_moby->pos, &target.pos) > 1.5f) {
            peckbot_moby->state = 3;
            SET_ANIM(peckbot_moby, 2, 0, scale_game_frames(3));
        }
        attack_ground_height = FUN_00213508(&peckbot_moby->pos, 0, 0.5f);
        peckbot_vars->x198 = peckbot_vars->x198 - frame_time_sq * 9.8f;
        peckbot_moby->pos.f[2] = peckbot_moby->pos.f[2] + peckbot_vars->x198;
        if (peckbot_moby->pos.f[2] < attack_ground_height) {
            peckbot_vars->x198 = 0.0f;
            peckbot_moby->pos.f[2] = attack_ground_height;
        }
        break;
    case PECKBOT_STATE_RETURN_HOME:
        if (peckbot_vars->path != -1) {
            select_list_point_toward_target(peckbot_moby,
                                            ((int)l01_paths[peckbot_vars->path]),
                                            &peckbot_vars->home,
                                            &position_scratch);
        } else {
            qcopy(&position_scratch, &peckbot_vars->home);
        }
        desired_heading = fast_arc_tan(position_scratch.f[0] - peckbot_moby->pos.f[0],
                                       position_scratch.f[1] - peckbot_moby->pos.f[1]);
        FUN_L00_00258278(peckbot_moby, peckbot_vars->turn, desired_heading, 0.02f, 0.3f, 0.1f);
        vector_scratch.f[0] = fast_cos(peckbot_moby->rot.f[2]) * 2.0f;
        vector_scratch.f[1] = fast_sin(peckbot_moby->rot.f[2]) * 2.0f;
        vector_scratch.f[2] = 0.0f;
        movement_result_flags = FUN_L00_00258b50(
            peckbot_moby, &peckbot_vars->x180, &vector_scratch, &position_scratch, 1.0f);
        if (vector_distance_xy(&peckbot_moby->pos, &peckbot_vars->home) < 2.0f) {
            peckbot_moby->state = 1;
            SET_ANIM(peckbot_moby, 4, 0, scale_game_frames(3));
        }
        fast_vec_sub(&home_to_target, &target.pos, &peckbot_vars->home);
        if (fast_vec_length(&home_to_target) < peckbot_vars->range) {
            if (peckbot_moby->group != 0xFF) {
                FUN_L01_0026e090(peckbot_moby->group, 1);
            }
            peckbot_moby->state = 3;
            SET_ANIM(peckbot_moby, 2, 0, scale_game_frames(3));
        } else if (peckbot_moby->hit == 1) {
            peckbot_moby->state = 3;
            SET_ANIM(peckbot_moby, 2, 0, scale_game_frames(3));
        } else if (movement_result_flags & 2) {
            peckbot_moby->pos.f[0] = oldpos.f[0];
            peckbot_moby->pos.f[1] = oldpos.f[1];
            peckbot_moby->state = 12;
            SET_ANIM(peckbot_moby, 5, 0, scale_game_frames(3));
        }
        FUN_00213508(&peckbot_moby->pos, 0, 0.5f);
        if (FUN_L00_001f35d8_cf() == 0 || FUN_L00_001f35d8_cf() == 8) {
            peckbot_moby->state = 16;
            SET_ANIM(peckbot_moby, 0, 0, scale_game_frames(3));
        }
        peckbot_moby->hit = 0;
        break;
    case PECKBOT_STATE_NONFATAL_KNOCKBACK:
        if (step_moby_motion(peckbot_moby, peckbot_vars->knock) & 1) {
            peckbot_moby->state = 3;
            SET_ANIM(peckbot_moby, 2, 0, scale_game_frames(3));
            return;
        } else if (peckbot_moby->pos.f[2] < 5.0f) {
            FUN_L00_00257470(peckbot_moby, 0, -1);
            mark_moby_for_removal(peckbot_moby);
            return;
        }
        break;
    case 7:
        if (FUN_L00_002dc9f0(peckbot_moby, peckbot_vars->knock) != 0) {
            peckbot_moby->state = 14;
            peckbot_vars->x228 = scale_game_frames(0xB4);
            SET_ANIM(peckbot_moby, 0, 0, 0);
            peckbot_vars->xC8 = 0;
        }
        break;
    case PECKBOT_STATE_FATAL_KNOCKBACK:
        peckbot_moby->flags &= ~0x1000;
        if (step_moby_motion(peckbot_moby, peckbot_vars->knock) & 0x140) {
            clear_u64_value(&vector_scratch);
            frame_count_or_animation_id = scale_game_frames(1) * 60.0f;
            if (reserve_expired_frame_slot(D_L01_00161A80, 3, frame_count_or_animation_id) >= 0) {
                cart_burst(peckbot_moby, &peckbot_moby->pos, 4, 0.5f, 13.0f);
            }
            FUN_L00_00263fd8(peckbot_moby,
                             0x6D3,
                             &peckbot_moby->pos,
                             &peckbot_moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L01_0015F580,
                             D_L01_0015F580,
                             D_L01_0015F580);
            FUN_L00_00263fd8(peckbot_moby,
                             0x6D4,
                             &peckbot_moby->pos,
                             &peckbot_moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L01_0015F580,
                             D_L01_0015F580,
                             D_L01_0015F580);
            FUN_L00_00263fd8(peckbot_moby,
                             0x6D5,
                             &peckbot_moby->pos,
                             &peckbot_moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L01_0015F580,
                             D_L01_0015F580,
                             D_L01_0015F580);
            mark_moby_for_removal(peckbot_moby);
            return;
        } else if (peckbot_moby->pos.f[2] < 0.0f) {
            FUN_L00_00263fd8(peckbot_moby,
                             0x6D3,
                             &peckbot_moby->pos,
                             &peckbot_moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L01_0015F580,
                             D_L01_0015F580,
                             D_L01_0015F580);
            FUN_L00_00263fd8(peckbot_moby,
                             0x6D4,
                             &peckbot_moby->pos,
                             &peckbot_moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L01_0015F580,
                             D_L01_0015F580,
                             D_L01_0015F580);
            FUN_L00_00263fd8(peckbot_moby,
                             0x6D5,
                             &peckbot_moby->pos,
                             &peckbot_moby->rot,
                             0,
                             0,
                             0.0f,
                             D_L01_0015F580,
                             D_L01_0015F580,
                             D_L01_0015F580);
            mark_moby_for_removal(peckbot_moby);
            return;
        }
        break;
    case PECKBOT_STATE_RECOVER_FROM_BLOCKED_MOVEMENT:
        recovery_target_height_distance_ratio =
            absolute_float(peckbot_moby->pos.f[2] - target.pos.f[2]);
        recovery_target_height_distance_ratio = recovery_target_height_distance_ratio /
                                                vector_distance_xy(&peckbot_moby->pos, &target.pos);
        if (peckbot_vars->path != -1) {
            select_list_point_toward_target(peckbot_moby,
                                            ((int)l01_paths[peckbot_vars->path]),
                                            &peckbot_vars->home,
                                            &position_scratch);
        } else {
            qcopy(&position_scratch, &peckbot_vars->home);
        }
        desired_heading = fast_arc_tan(position_scratch.f[0] - peckbot_moby->pos.f[0],
                                       position_scratch.f[1] - peckbot_moby->pos.f[1]);
        FUN_L00_00258278(peckbot_moby, peckbot_vars->turn, desired_heading, 0.05f, 0.3f, 0.2f);
        vector_scratch.f[0] = fast_cos(peckbot_moby->rot.f[2]) * 2.0f;
        vector_scratch.f[1] = fast_sin(peckbot_moby->rot.f[2]) * 2.0f;
        vector_scratch.f[2] = 0.0f;
        recovery_movement_result_flags = FUN_L00_00258b50(
            peckbot_moby, &peckbot_vars->x180, &vector_scratch, &position_scratch, 1.0f);
        fast_vec_sub(&home_to_target, &target.pos, &peckbot_vars->home);
        if (vector_distance_xy(&peckbot_moby->pos, &target.pos) < 1.5f &&
            recovery_target_height_distance_ratio <= 1.0f) {
            peckbot_moby->state = 4;
            SET_ANIM(peckbot_moby, 3, 0, scale_game_frames(3));
        } else if (recovery_target_height_distance_ratio <= 1.0f &&
                   !(recovery_movement_result_flags & 2)) {
            peckbot_moby->state = 5;
            SET_ANIM(peckbot_moby, 2, 0, scale_game_frames(3));
        } else if (vector_distance_xy(&peckbot_moby->pos, &target.pos) >
                   peckbot_vars->range + 6.0f) {
            peckbot_moby->state = 16;
            SET_ANIM(peckbot_moby, 0, 0, scale_game_frames(3));
        } else if (peckbot_moby->anim_flags & 2) {
            random_integer_below(4);
            frame_count_or_animation_id = random_integer_below(4) ? 4 : 5;
            blend_moby_animation(
                peckbot_moby, frame_count_or_animation_id, 0, scale_game_frames(3));
        }
        FUN_00213508(&peckbot_moby->pos, 0, 0.5f);
        if (FUN_L00_001f35d8_cf() == 0 || FUN_L00_001f35d8_cf() == 8) {
            peckbot_moby->state = 16;
            SET_ANIM(peckbot_moby, 0, 0, scale_game_frames(3));
        }
        peckbot_moby->hit = 0;
        break;
    case PECKBOT_STATE_MOVE_FORWARD_NEAR_LEADER:
        if (peckbot_moby->anim_next != 2) {
            SET_ANIM(peckbot_moby, 2, 0, scale_game_frames(3));
        }
        vector_scratch.f[0] = fast_cos(peckbot_moby->rot.f[2]) * (peckbot_vars->speed * frame_time);
        vector_scratch.f[1] = fast_sin(peckbot_moby->rot.f[2]) * (peckbot_vars->speed * frame_time);
        vector_scratch.f[2] = 0.0f;
        fast_vec_add(&position_scratch, &peckbot_moby->pos, &vector_scratch);
        for (candidate_moby = (MobyNode *)D_L01_0015FFE4; candidate_moby != NULL;
             candidate_moby = candidate_moby->next) {
            if (candidate_moby == (MobyNode *)peckbot_moby || candidate_moby->state < 0) {
                continue;
            }
            if (candidate_moby->oclass == 0 || candidate_moby->oclass == 0x241) {
                if (vector_distance_xy(&position_scratch, &candidate_moby->pos) < 2.0f) {
                    z = peckbot_moby->pos.f[2];
                    nearby_moby_heading_difference = fast_subtract_rotations(
                        peckbot_moby->rot.f[2],
                        fast_arc_tan(candidate_moby->pos.f[0] - peckbot_moby->pos.f[0],
                                     candidate_moby->pos.f[1] - peckbot_moby->pos.f[1]));
                    turn = frame_time * 1.5707964f;
                    if (!(nearby_moby_heading_difference > 0.0f)) {
                        turn = -turn;
                    }
                    peckbot_moby->rot.f[2] = fast_add_rotations(peckbot_moby->rot.f[2], turn);
                    fast_vec_sub(&position_scratch, &peckbot_moby->pos, &candidate_moby->pos);
                    FUN_L00_001ff500_cf(&position_scratch, &position_scratch, 2.0f);
                    fast_vec_add(&position_scratch, &position_scratch, &candidate_moby->pos);
                    position_scratch.f[2] = z;
                }
            }
        }
        qcopy(&peckbot_moby->pos, &position_scratch);
        forward_movement_ground_height = FUN_00213508(&peckbot_moby->pos, 0, 0.5f);
        peckbot_vars->x198 = peckbot_vars->x198 - frame_time_sq * 9.8f;
        peckbot_moby->pos.f[2] = peckbot_moby->pos.f[2] + peckbot_vars->x198;
        if (peckbot_moby->pos.f[2] < forward_movement_ground_height) {
            peckbot_vars->x198 = 0.0f;
            peckbot_moby->pos.f[2] = forward_movement_ground_height;
        }
        if (peckbot_vars->leader == NULL ||
            vector_distance_xy(&peckbot_moby->pos, &peckbot_vars->leader->pos) >
                convert_integer_to_float(peckbot_vars->x216)) {
            peckbot_moby->state = 5;
            SET_ANIM(peckbot_moby, 2, 0, scale_game_frames(3));
        }
        peckbot_moby->hit = 0;
        break;
    }
    trooper_anim_finish(peckbot_moby, peckbot_vars->fx110);
    FUN_L00_0025a120(peckbot_moby);
}
