/* Level 07 moby class 0x458: mine; holds at its goal, dives at the hero once it sees it and bursts on contact. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/data_labels.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/moby_anim.h"

extern s32  collect_mobies_in_sphere(f32, void *, s32, void *, void *) __asm__("FUN_L00_001f2868");
extern void FUN_L00_00258278(struct Moby *, f32, void *, f32, f32, f32);
extern int  resolve_moby_hit_response(void *, void *, void *, int, int *, int, int, int) __asm__("FUN_00213928");
extern void  FUN_L00_00257470(void *, int, int);
extern float D_0015ED6C;
extern Vec4  D_L07_00166E40;

#include "rnc/game/item_state.h"

extern void         spawn_explosion(void *,
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
extern void         FUN_L00_0025a120(struct Moby *);
extern void         mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void         jet_animate(struct Moby *, float) __asm__("FUN_L00_00259fe8");
extern Vec4         l07_sphere_collision_resolved_center __asm__("D_L07_00173EF0");
extern f32          D_0015ED6C;
extern void         spawn_explosion(void *,
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
extern void         jet_animate(struct Moby *, f32) __asm__("FUN_L00_00259fe8");
extern s32          coll_sphere(f32, void *, s32, struct Moby *) __asm__("FUN_L00_001f0d60");
extern struct Moby *debris_target __asm__("D_L07_00173ED8") NOT_SDA;

/* pvars of the mine's spawner: how it hunts. */
typedef struct {
    u8  pad0[0x7C];
    f32 speed; /* 0x7C */
    u8  pad80[4];
    s32 linger_time; /* 0x84: copied into a removed mine's life_timer */
    f32 view_angle;  /* 0x88: degrees */
    f32 view_range;  /* 0x8C */
} MineSpawnerVars;

/* pvars of a mine. */
typedef struct {
    u8           pad0[0x20];
    u8           collision[9]; /* 0x20: contact state for resolve_moby_hit_response */
    u8           unk29;        /* 0x29 */
    u8           pad2A[0xE];
    struct Moby *perch; /* 0x38 */
    u8           pad3C[0x24];
    Vec4         velocity; /* 0x60 */
    Vec4         home;     /* 0x70 */
    Vec4         goal;     /* 0x80 */
    u8           pad90[8];
    s32          life_timer;  /* 0x98 */
    f32          dive_speed;  /* 0x9C */
    struct Moby *spawner;     /* 0xA0 */
    f32          turn_speed;  /* 0xA4 */
    f32          pitch_speed; /* 0xA8 */
    s32          bob_timer;   /* 0xAC */
} MineVars;

/* What get_target_sight reports about the hero. */
typedef struct {
    Vec4 pos;
    u8   pad10[0x30];
    s32  found; /* 0x40 */
} MineSight;

/* Hit record returned by the hit lookup find_moby_hit. */
typedef struct {
    u8           pad0[0x10];
    Vec4f        pos;  /* 0x10 */
    struct Moby *moby; /* 0x20: moby that dealt the hit */
    u8           pad24[8];
    f32          kind; /* 0x2C */
} MobyHit;

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_00313D28.c: func_L07_0031AA68), where it is exact; names translated to the US level program. */
void        mark_mine_for_removal(struct Moby     *moby,
                                  MineSpawnerVars *spawner_vars) __asm__("FUN_L07_00319698");
extern s32  FUN_L07_002886d8(struct Moby *, MineSight *, f32);
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
extern f32  spring_turn(f32 *, f32, f32, f32, f32, f32) __asm__("FUN_L00_00258110");

/* Mine: holds at its goal, dives at the hero once it sees it and bursts on contact. */
void                    diving_mine_update(struct Moby *moby) __asm__("FUN_L07_003196e0");
extern float D_0015ED6C MACRO_ADDR;

void mark_mine_for_removal(struct Moby *moby, MineSpawnerVars *spawner_vars) {
    MineVars *swooper_vars = (MineVars *)moby->pvars;

    if (spawner_vars != NULL) {
        swooper_vars->life_timer = scale_game_frames(spawner_vars->linger_time);
    }
    mark_moby_for_removal(moby);
}

/* Moby class 1112 */
void diving_mine_update(struct Moby *moby) {
    enum {
        MINE_INITIALIZE = 0,
        MINE_WAIT_AT_GOAL = 1,
        MINE_CHASE_TARGET = 2,
        MINE_EXPLODE = 3
    };
    MineVars        *swooper_instance_vars;
    MineSpawnerVars *spawner_vars;
    MineSight        sight;
    MobyHit         *hit;
    u8              *collision;
    Vec4             chase_acceleration;
    u8               contact[0x30];
    Vec4             probe;
    Vec4             push;
    s32              touching;
    f32              pitch;

    if (moby == NULL) {
        return;
    }
    swooper_instance_vars = (MineVars *)moby->pvars;
    if (swooper_instance_vars == NULL || swooper_instance_vars->spawner == NULL) {
        return;
    }
    collision = swooper_instance_vars->collision;
    spawner_vars = (MineSpawnerVars *)swooper_instance_vars->spawner->pvars;
    FUN_L07_002886d8(moby, &sight, spawner_vars->view_range);
    if (moby->visible && fast_vec_dist(&moby->pos, &D_L07_00166E40) < 27.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 0x15;
    }
    hit = find_moby_hit(moby, 0x230000, 0);
    resolve_moby_hit_response(moby, hit, collision, 0, &touching, 0, 0, 4);
    if (touching >= 2 && hit != NULL && hit->moby->oclass != 0x363 && hit->moby->oclass != 0x458 &&
        hit->moby->oclass != 0x367) {
        spawn_explosion(moby,
                        &swooper_instance_vars->velocity,
                        NULL,
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
                        0);
        FUN_L00_00257470(moby, 0, -1);
        mark_mine_for_removal(moby, spawner_vars);
    }
    switch (moby->state) {
    case MINE_INITIALIZE:
        if (spawner_vars == NULL) {
            break;
        }
        moby->flags |= 0x1000;
        clear_u64_value(&swooper_instance_vars->velocity);
        qcopy(&swooper_instance_vars->home, &moby->pos);
        moby->rot.y = 0.0f;
        moby->rot.z = fast_arc_tan(swooper_instance_vars->goal.f[0] - moby->pos.x,
                                   swooper_instance_vars->goal.f[1] - moby->pos.y);
        swooper_instance_vars->unk29 = 0;
        moby->state = 1;
        moby->command = 0;
        break;
    case MINE_WAIT_AT_GOAL:
        fast_vec_sub(&swooper_instance_vars->velocity, &swooper_instance_vars->goal, &moby->pos);
        FUN_L00_00258278(moby,
                         fast_arc_tan(swooper_instance_vars->goal.f[0] - moby->pos.x,
                                      swooper_instance_vars->goal.f[1] - moby->pos.y),
                         &swooper_instance_vars->turn_speed,
                         frame_time_sq * 6.2831855f,
                         D_0015ED6C * 3.1415927f,
                         D_0015ED6C * 6.2831855f);
        qcopy(&moby->pos, &swooper_instance_vars->goal);
        if ((swooper_instance_vars->perch != NULL &&
             vector_distance_xy(&moby->pos, &swooper_instance_vars->perch->pos) < 20.0f &&
             absolute_float(moby->pos.z - swooper_instance_vars->perch->pos.z) < 3.0f) ||
            (sight.found != 0 && fast_vec_dist(&moby->pos, &sight.pos) < spawner_vars->view_range &&
             vector_distance_xy(&moby->pos, &swooper_instance_vars->spawner->pos) > 2.0f &&
             fast_difference_between_rotations(
                 moby->rot.z,
                 fast_arc_tan(sight.pos.f[0] - moby->pos.x, sight.pos.f[1] - moby->pos.y)) <
                 spawner_vars->view_angle * 0.017453292f)) {
            if (moby->prev_seq != 1) {
                blend_moby_animation((MobyAnim *)moby, 1, 0, 10);
            }
            swooper_instance_vars->dive_speed = spawner_vars->speed * D_0015ED6C;
            swooper_instance_vars->life_timer = scale_game_frames(600);
            moby->command = 1;
            swooper_instance_vars->bob_timer = scale_game_frames(7);
            moby->state = 2;
        }
        break;
    case MINE_CHASE_TARGET:
        if ((moby->anim_flags & 2) && moby->seq == 1 && moby->prev_seq != 2) {
            blend_moby_animation((MobyAnim *)moby, 2, 0, 10);
        }
        if (moby->command == 1) {
            moby->pos.z += frame_scale * 0.1f;
            if (fast_dec_timer(&swooper_instance_vars->bob_timer)) {
                swooper_instance_vars->bob_timer = scale_game_frames(7);
                moby->command = 2;
            }
        } else if (moby->command == 2) {
            moby->pos.z -= frame_scale * 0.1f;
            if (fast_dec_timer(&swooper_instance_vars->bob_timer)) {
                moby->command = 0;
            }
        }
        FUN_L00_00258278(moby,
                         fast_arc_tan(sight.pos.f[0] - moby->pos.x, sight.pos.f[1] - moby->pos.y),
                         &swooper_instance_vars->turn_speed,
                         frame_time_sq * 3.1415927f,
                         D_0015ED6C * 3.1415927f,
                         D_0015ED6C * 6.2831855f);
        pitch =
            fast_arc_tan(vector_distance_xy(&moby->pos, &sight.pos), sight.pos.f[2] - moby->pos.z);
        moby->rot.y = spring_turn(&swooper_instance_vars->pitch_speed,
                                  moby->rot.y,
                                  pitch,
                                  frame_time_sq * 3.1415927f,
                                  D_0015ED6C * 3.1415927f,
                                  D_0015ED6C * 6.2831855f);
        fast_vec_sub(&chase_acceleration, &sight.pos, &moby->pos);
        if (moby->command != 0) {
            chase_acceleration.f[2] = 0.0f;
        }
        fast_vec_normalize(&chase_acceleration, &chase_acceleration, frame_time_sq * 30.0f);
        fast_vec_add(&swooper_instance_vars->velocity,
                     &swooper_instance_vars->velocity,
                     &chase_acceleration);
        clamp_vector_length(
            &swooper_instance_vars->velocity, &swooper_instance_vars->velocity, D_0015ED6C * 10.0f);
        fast_vec_add(&moby->pos, &moby->pos, &swooper_instance_vars->velocity);
        if (fast_vec_dist(&moby->pos, &swooper_instance_vars->home) > 64.0f) {
            mark_mine_for_removal(moby, spawner_vars);
        }
        make_hit_descriptor(contact, moby, 0x10001, 1.0f, &swooper_instance_vars->velocity);
        qcopy(&probe, &moby->pos);
        probe.f[2] += 0.75f;
        if (collect_mobies_in_sphere(0.75f, &probe, 0x10, moby, contact) && debris_target != NULL &&
            debris_target->oclass != 0x363 && debris_target->oclass != 0x456 &&
            debris_target->oclass != 0x458 && debris_target->oclass != 0x365 &&
            debris_target->oclass != 0x367) {
            spawn_explosion(moby,
                            &swooper_instance_vars->velocity,
                            NULL,
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
                            0);
            mark_mine_for_removal(moby, spawner_vars);
        }
        qcopy(&probe, &moby->pos);
        probe.f[2] += 0.55f;
        if (coll_sphere(0.55f, &probe, 0, moby)) {
            fast_vec_sub(&push, &l07_sphere_collision_resolved_center, &probe);
            fast_vec_add(&moby->pos, &moby->pos, &push);
        }
        if (swooper_instance_vars->life_timer == 0) {
            mark_mine_for_removal(moby, spawner_vars);
        }
        break;
    case MINE_EXPLODE:
        spawn_explosion(moby,
                        &swooper_instance_vars->velocity,
                        NULL,
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
                        0);
        mark_mine_for_removal(moby, spawner_vars);
        break;
    case 4:
        break;
    }
    jet_animate(moby, 0.7f);
}
