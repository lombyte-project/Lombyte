/* Level 07 moby class 0x456: homing mine. */
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

extern s32 collect_mobies_in_sphere(f32, void *, s32, void *, void *) __asm__("FUN_L00_001f2868");
extern int resolve_moby_hit_response(void *, void *, void *, int, int *, int, int, int) __asm__("FUN_00213928");
extern void  FUN_L00_00257470(void *, int, int);
extern void  FUN_00212f90(void *, int, int, int);
extern float D_0015ED6C;
extern Vec4  D_L07_00166E40;

#include "rnc/game/item_state.h"

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
extern void FUN_L00_0025a120(struct Moby *);
extern int  get_target_sight(void *, void *, float) __asm__("FUN_L07_002886d8");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void jet_animate(struct Moby *, float) __asm__("FUN_L00_00259fe8");
extern Vec4 l07_sphere_collision_resolved_center __asm__("D_L07_00173EF0");
extern f32  D_0015ED6C;
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
extern void jet_animate(struct Moby *, f32) __asm__("FUN_L00_00259fe8");
extern s32  coll_sphere(f32, void *, s32, struct Moby *) __asm__("FUN_L00_001f0d60");
extern void set_moby_list_state(s32, s32) __asm__("FUN_L01_0026e0e0");

typedef struct {
    u8           pad0[0x20];
    f32          health; /* 0x20: lowered by resolve_moby_hit_response */
    u8           pad24[0x14];
    struct Moby *target; /* 0x38 */
    u8           pad3C[0x24];
    Vec4         home;      /* 0x60: spawn position */
    f32          speed;     /* 0x70 */
    s32          sound;     /* 0x74: -1 when none */
    s32          timeout;   /* 0x78 */
    s32          bob_timer; /* 0x7C */
} Mine;

extern struct Moby *debris_target __asm__("D_L07_00173ED8") NOT_SDA;

/* Floating mine: wakes when its target or a seen point comes close, then bobs and chases the seen
   point. It blows up when a hit lands (not from classes 0x456/0x365/0x367), on contact with a
   solid moby or when its timer runs out, and is removed once 64 units from home. */
void                    homing_mine_update(struct Moby *moby) __asm__("FUN_L07_00319040");
extern void             spawn_explosion(void *,
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
extern float D_0015ED6C MACRO_ADDR;

void homing_mine_update(struct Moby *moby) {
    enum {
        HOMING_MINE_INITIALIZE = 0,
        HOMING_MINE_WAIT_FOR_PROXIMITY = 1,
        HOMING_MINE_START_CHASE = 2,
        HOMING_MINE_CHASE = 3
    };
    Mine *vars;
    f32  *health;
    char  sight[0x50];
    char *hit;
    Vec4  chase_step;
    u8    contact[0x30];
    Vec4  probe;
    Vec4  push;
    s32   touching;
    f32   speed;
    f32   turn;
    f32   new_speed;
    f32   max_speed;
    s32   timer;

    if (moby == NULL)
        return;
    vars = (Mine *)moby->pvars;
    if (vars == NULL)
        return;

    get_target_sight(moby, sight, 8.0f);
    if (moby->visible && fast_vec_dist(&moby->pos, &D_L07_00166E40) < 27.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 0x15;
    }
    health = &vars->health;
    hit = find_moby_hit(moby, 0x230000, 0);
    resolve_moby_hit_response(moby, hit, health, 0, &touching, 0, 0, 4);
    if (touching >= 2 && hit != NULL && (*(struct Moby **)(hit + 0x20))->oclass != 0x456 &&
        (*(struct Moby **)(hit + 0x20))->oclass != 0x365 &&
        (*(struct Moby **)(hit + 0x20))->oclass != 0x367) {
        clear_u64_value(chase_step.f);
        spawn_explosion(moby,
                        &chase_step,
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
        mark_moby_for_removal(moby);
        return;
    }

    switch (moby->state) {
    case HOMING_MINE_INITIALIZE:
        moby->flags |= 0x1000;
        qcopy(&vars->home, &moby->pos);
        moby->collision = moby->pclass->collision;
        moby->state = 1;
        break;
    case HOMING_MINE_WAIT_FOR_PROXIMITY:
        timer = scale_game_frames(600);
        turn = D_0015ED6C * 6.2831855f;
        vars->timeout = timer;
        moby->rot.z = fast_add_rotations(moby->rot.z, turn);
        if ((vars->target != NULL && vector_distance_xy(&moby->pos, &(vars->target)->pos) < 20.0f &&
             absolute_float(moby->pos.z - (vars->target)->pos.z) < 3.0f) ||
            (*(s32 *)(sight + 0x40) != 0 && fast_vec_dist(&moby->pos, sight) < 8.0f)) {
            vars->speed = 0.0f;
            vars->timeout = scale_game_frames(600);
            moby->command = 1;
            vars->bob_timer = scale_game_frames(7);
            moby->state = 3;
            if (vars->sound != -1)
                set_moby_list_state(vars->sound, 2);
            moby->state = 2;
        }
        break;
    case HOMING_MINE_START_CHASE:
        if (moby->prev_seq != 1)
            FUN_00212f90(moby, 1, 0, 10);
        moby->state = 3;
    case HOMING_MINE_CHASE:
        if ((moby->anim_flags & 2) && moby->seq == 1 && moby->prev_seq != 2) {
            FUN_00212f90(moby, 2, 0, 10);
        }
        speed = frame_time_sq;
        new_speed = vars->speed + speed * 30.0f;
        max_speed = D_0015ED6C * 6.0f;
        vars->speed = new_speed;
        if (max_speed < new_speed)
            vars->speed = max_speed;
        if (moby->command == 1) {
            moby->pos.z += frame_scale * 0.1f;
            if (fast_dec_timer(&vars->bob_timer)) {
                vars->bob_timer = scale_game_frames(7);
                moby->command = 2;
            }
        } else if (moby->command == 2) {
            moby->pos.z -= frame_scale * 0.1f;
            if (fast_dec_timer(&vars->bob_timer))
                moby->command = 0;
        }
        fast_vec_sub(&chase_step, sight, &moby->pos);
        fast_vec_normalize(&chase_step, &chase_step, vars->speed);
        fast_vec_add(&moby->pos, &moby->pos, &chase_step);
        if (fast_vec_dist(&moby->pos, &vars->home) > 64.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        make_hit_descriptor(contact, moby, 0x10001, 1.0f, &chase_step);
        qcopy(&probe, &moby->pos);
        probe.f[2] += 0.75f;
        if (collect_mobies_in_sphere(0.75f, &probe, 0x10, moby, contact) && debris_target != NULL &&
            debris_target->oclass != 0x363 && debris_target->oclass != 0x456 &&
            debris_target->oclass != 0x458 && debris_target->oclass != 0x365 &&
            debris_target->oclass != 0x367) {
            spawn_explosion(moby,
                            &chase_step,
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
            mark_moby_for_removal(moby);
            return;
        }
        qcopy(&probe, &moby->pos);
        probe.f[2] += 0.55f;
        if (coll_sphere(0.55f, &probe, 0, moby)) {
            fast_vec_sub(&push, &l07_sphere_collision_resolved_center, &probe);
            fast_vec_add(&moby->pos, &moby->pos, &push);
        }
        if (!fast_dec_timer(&vars->timeout))
            break;
        spawn_explosion(moby,
                        &chase_step,
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
        mark_moby_for_removal(moby);
        return;
    case 4:
        break;
    }
    jet_animate(moby, 0.7f);
}
