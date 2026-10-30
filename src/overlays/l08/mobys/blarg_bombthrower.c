/* Level 08 moby class 0x14d: blarg bombthrower. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/game/fun_00213928.h"
#include "rnc/math/random.h"

/* Ported from rac1-decomp src/overlays/l08_batalia/vendor_002B9438.c (func_L08_002B94B0) */
#include "qcopy.h"

extern char *D_L08_0015FFD8;

extern void mark_moby_for_removal(void *) __asm__("func_0020C828");

/* Spawns a moby of class 0xF8 at a position and facing derived from a vector. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D35E8), where it is exact; names translated to the US level program. */
extern char *create_moby(int);
extern void  update_entity(void *) __asm__("FUN_L00_00250df8");
char        *spawn_bomb_with_velocity(int   source_moby,
                                      char *spawn_position,
                                      char *initial_velocity) __asm__("FUN_L08_002d22e0");
extern void  trooper_anim_update(void *, short *) __asm__("FUN_L00_0025d458");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern s32   D_001413D0[];

extern short *D_L08_001AC040[];

/* Picks the best candidate moby from the moby's list by heading and distance.
 * Exact since tools/ps2eeas_nops.py puts the hazard nop after the shared bc1fl's label. */
/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002DBD88), where it is exact; names translated to the US level program. */
char *select_blarg_bombthrower_waypoint(struct Moby *moby) __asm__("FUN_L08_002daa10");

typedef struct {
    char         pad0[0x40];
    struct Moby *target;
    int          result;
} Look;

typedef struct {
    char          pad0[0x20];
    float         health;
    char          pad24[5];
    unsigned char b29;
    char          pad2A[0xE];
    int           i38;
    char          pad3C[4];
    float         v40[4];
    char          pad50[8];
    unsigned char b58;
    char          pad59;
    unsigned char b5A;
    char          pad5B[5];
    short         anim[3];
    char          pad66;
    unsigned char tint;
    char          pad68[8];
    char          blk70[0x10];
    float         f80, f84, f88, f8C;
    int           pad90;
    int           i94;
    char          pad98[0x15];
    unsigned char bAD;
    char          padAE[0x12];
    float         fC0, fC4;
    char          padC8[8];
    char          blkD0[8];
    float         fD8, fDC;
    char          padE0[0x14];
    float         fF4;
    char          padF8[0x14];
    float         f10C, f110, f114;
    char          pad118[8];
    struct Moby  *waypoint;
    struct Moby  *p124;
    int           i128;
    char          blk12C[4];
    short         s130, s132;
    int           i134;
    char          pad138[0x28];
    char          blk160[0x10];
} Data_DBF68;

char        *spawn_bomb_with_velocity(int arg, char *pos, char *vec) __asm__("FUN_L08_002d22e0");
char        *FUN_L08_002daa10_c(struct Moby *moby) __asm__("FUN_L08_002daa10");
extern float D_L08_00161978_gp __asm__("D_L08_00161978") __attribute__((sda));
extern float l08_bombthrower_throw_timing_speed_gp __asm__("D_L08_0016197C") __attribute__((sda));
extern float l08_bombthrower_aim_shortening_rate_gp __asm__("D_L08_00161988") __attribute__((sda));
extern float l08_bombthrower_death_horizontal_knockback_speed_gp __asm__("D_L08_0016198C")
    __attribute__((sda));
extern float l08_bombthrower_death_vertical_knockback_speed_gp __asm__("D_L08_00161990")
    __attribute__((sda));
extern float D_L08_001675C0[];
extern float FUN_L04_001f1978(float);
extern float FUN_L00_0025abf0(void *, void *, void *, float, float);
extern int   D_L08_001619AC_gp __asm__("D_L08_001619AC") __attribute__((sda));
extern int   D_L08_001619D0_gp __asm__("D_L08_001619D0") __attribute__((sda));
extern int   FUN_L00_00259028(char *m, char *p, float *pos, char *v);
extern int
select_nearest_xy_moby_with_hero_fallback(void *, float, void *) __asm__("FUN_L08_0026a3d0");
extern void FUN_L00_0025a120(void *);
extern void
trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern void  FUN_L00_0025f730(void *, float);
extern void  FUN_L01_0026d930(char *p);
extern void  spawn_moby_table_puff_pattern(void *) __asm__("FUN_L04_002c46d0");
extern void  FUN_00212f90(struct Moby *, int, int, int);
extern void  explode_2D5FA8_c(struct Moby *, int, int) __asm__("FUN_L00_00257470");
extern void  func_L00_00264B40_x(struct Moby *, int, float, char *) __asm__("FUN_L00_00263ac8");
extern float FUN_0020c9e0(struct Moby *);
extern void
     func_L00_002592B0_x(float, void *, void *, float, float, float) __asm__("FUN_L00_00258278");
void blarg_bombthrower_update(struct Moby *moby) __asm__("FUN_L08_002dabf0");

char *spawn_bomb_with_velocity(int source_moby, char *spawn_position, char *initial_velocity) {
    char *spawned_moby = create_moby(0xF8);
    if (spawned_moby != 0) {
        char *bomb_vars = ((char *)MOBY(spawned_moby)->pvars);
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 1;
        *(int *)(bomb_vars + 0x30) = source_moby;
        *(int *)(bomb_vars + 0x20) = scale_game_frames(300);
        *(float *)(bomb_vars + 0x2C) = 1.0f;
        *(int *)(bomb_vars + 0x24) = 0;
        qcopy(spawned_moby + 0x10, spawn_position);
        qcopy(bomb_vars + 0x10, initial_velocity);
        MOBY(spawned_moby)->rot.x = random_float_between(-180.0f, 180.0f) * DEG_TO_RAD;
        MOBY(spawned_moby)->rot.z =
            fast_arc_tan(*(float *)initial_velocity, *(float *)(initial_velocity + 4)) + 1.5707964f;
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

char *select_blarg_bombthrower_waypoint(struct Moby *moby) {
    short       *group_entry = D_L08_001AC040[moby->group];
    char        *best;
    char        *candidate_moby;
    float        bestD, bestA;
    int          candidate_moby_index;
    struct Hero *base;
    if (group_entry == 0)
        return 0;
    best = 0;
    candidate_moby_index = *(unsigned short *)group_entry & 0x7FFF;
    candidate_moby = D_L08_0015FFD8 + (candidate_moby_index << 8);
    bestA = 0.0f;
    bestD = bestA;
    for (;;) {
        if (MOBY(candidate_moby)->oclass != 0x14D && MOBY(candidate_moby)->command == 0) {
            float candidate_hero_heading_difference, candidate_distance, candidate_heading;
            candidate_heading = fast_arc_tan(MOBY(candidate_moby)->pos.x - moby->pos.x,
                                             MOBY(candidate_moby)->pos.y - moby->pos.y);
            base = &hero;
            candidate_hero_heading_difference = fast_difference_between_rotations(
                candidate_heading,
                fast_arc_tan(base->motion.pos.f[0] - moby->pos.x,
                             base->motion.pos.f[1] - moby->pos.y));
            candidate_distance = fast_vec_dist(&moby->pos, candidate_moby + 0x10);
            if (candidate_hero_heading_difference < 1.5707964f) {
                if (bestA < candidate_hero_heading_difference) {
                    best = candidate_moby;
                    bestD = candidate_distance;
                    bestA = candidate_hero_heading_difference;
                }
            } else if ((candidate_distance >= 16.0f &&
                        (candidate_distance < bestD || bestD < 16.0f)) ||
                       (candidate_distance < 16.0f && bestD < candidate_distance)) {
                best = candidate_moby;
                bestD = candidate_distance;
                bestA = candidate_hero_heading_difference;
            }
        }
        if (*group_entry < -1)
            return best;
        group_entry++;
        candidate_moby_index = *(unsigned short *)group_entry & 0x7FFF;
        candidate_moby = D_L08_0015FFD8 + (candidate_moby_index << 8);
    }
}

void blarg_bombthrower_update(struct Moby *moby) {
    enum {
        BOMBTHROWER_STATE_INITIALIZE = 0,
        BOMBTHROWER_STATE_APPROACH_WAYPOINT = 1,
        BOMBTHROWER_STATE_WAIT_TO_THROW = 3,
        BOMBTHROWER_STATE_MOVE_TO_THROW_POSITION = 4,
        BOMBTHROWER_STATE_THROW_BOMB = 5,
        BOMBTHROWER_STATE_MOVE_ONTO_WAYPOINT = 6,
        BOMBTHROWER_STATE_DAMAGE_KNOCKBACK = 0x5A,
        BOMBTHROWER_STATE_WAIT_FOR_FALL_RECOVERY = 0x5B,
        BOMBTHROWER_STATE_MOTION_RECOVERY = 0x5C,
        BOMBTHROWER_STATE_FALL_RECOVERY = 0x5D,
        BOMBTHROWER_STATE_DYING = 0x63
    };
    Data_DBF68  *vars = (Data_DBF68 *)moby->pvars;
    Look         search;
    float        point[4];
    float        velocity[4];
    float        aim[4];
    int          hit;
    float        damage;
    float        scratch;
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
        float duration = convert_integer_to_float(D_L08_001619D0_gp);
        int   start_alpha = vars->s132;
        moby->alpha = truncate_float_to_s32(
            (float)start_alpha +
            (float)-start_alpha * ((float)(D_L08_001619D0_gp - vars->s130) / duration));
        spawn_moby_table_puff_pattern(moby);
        return;
    }
    fun_00213928(moby, find_moby_hit(moby, 0x330000, 0), &vars->health, 0, &hit, &damage, 0, 4);
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
                trooper_start_knockback(moby,
                                        vars->blk70,
                                        fast_arc_tan(moby->pos.x - *(float *)(hero_bytes + 0x80),
                                                     moby->pos.y - *(float *)(hero_bytes + 0x84)),
                                        6,
                                        1,
                                        0);
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
            vars->f88 = l08_bombthrower_death_horizontal_knockback_speed_gp * frame_time;
            vars->f8C = l08_bombthrower_death_vertical_knockback_speed_gp * frame_time;
            vars->bAD = 0;
            hero_bytes = (char *)&hero;
            trooper_start_knockback(moby,
                                    vars->blk70,
                                    fast_arc_tan(moby->pos.x - *(float *)(hero_bytes + 0x80),
                                                 moby->pos.y - *(float *)(hero_bytes + 0x84)),
                                    9,
                                    1,
                                    0);
            vars->fC0 = 6.0f;
            vars->fC4 = 10.0f;
            explode_2D5FA8_c(moby, 0, -1);
            vars->tint = 0xFA;
            moby->state = 0x63;
            moby->flags &= 0xEFFF;
            vars->waypoint->command = 0;
        }
        trooper_anim_update(moby, vars->anim);
    }
    moby->hit_slot = 0xFF;
    search.result = select_nearest_xy_moby_with_hero_fallback(moby, 16.0f, &search);
    if (search.target == 0) {
        search.target = *(struct Moby **)&D_001413D0;
    }
    target = search.target;
    switch (moby->state) {
    case BOMBTHROWER_STATE_INITIALIZE:
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
        vars->b58 = truncate_float_to_s32(17.0f);
        vars->b5A = truncate_float_to_s32(10.4f);
        clear_u64_value(vars->v40);
        vars->waypoint = (struct Moby *)FUN_L08_002daa10_c((char *)moby);
        vars->waypoint->command = 1;
        vars->i128 = scale_game_frames(0xB4);
        if (moby->prev_seq != 4) {
            FUN_00212f90(moby, 4, 0, 0);
        }
        moby->state = 1;
        vars->i134 = scale_game_frames(0xF0);
        break;
    case BOMBTHROWER_STATE_APPROACH_WAYPOINT:
        FUN_L00_00259028(
            (char *)moby, vars->blkD0, (float *)&vars->waypoint->pos, (char *)vars->v40);
        if (fast_vec_dist(&moby->pos, &vars->waypoint->pos) < 3.0f) {
            if (fast_difference_between_rotations(
                    moby->rot.z,
                    fast_arc_tan(vars->waypoint->pos.x - moby->pos.x,
                                 vars->waypoint->pos.y - moby->pos.y)) < 0.2617994f) {
                if (6.0f < vector_distance_xy(&moby->pos, &target->pos)) {
                    if (moby->prev_seq != 5) {
                        FUN_00212f90(moby, 5, 0, 10);
                    }
                    moby->state = 6;
                    break;
                }
                vars->i134 = scale_game_frames(0xF0);
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
        if (fast_dec_timer(&vars->i134) != 0) {
            old_waypoint = vars->waypoint;
            if ((vars->waypoint = (struct Moby *)FUN_L08_002daa10_c((char *)moby)) == 0) {
                vars->waypoint = old_waypoint;
            }
            old_waypoint->command = 0;
            vars->waypoint->command = 1;
            vars->i134 = scale_game_frames(0xF0);
        }
        break;
    case BOMBTHROWER_STATE_MOVE_ONTO_WAYPOINT:
        FUN_L00_00259028(
            (char *)moby, vars->blkD0, (float *)&vars->waypoint->pos, (char *)vars->v40);
        if (moby->anim_flags & 2) {
            FUN_00212f90(vars->waypoint, 1, 0, scale_game_frames(6));
            qcopy(&moby->pos, &vars->waypoint->pos);
            moby->pos.z -= 0.1f;
            if (moby->prev_seq != 1) {
                FUN_00212f90(moby, 1, 0, 10);
            }
            moby->state = 3;
            moby->flags &= 0xEFFF;
            moby->rot.z = fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y);
            vars->i128 = scale_game_frames(0x78);
            moby->collision = 0;
        }
        break;
    case BOMBTHROWER_STATE_WAIT_TO_THROW: {
        float target_distance_xy = vector_distance_xy(&moby->pos, &target->pos);
        func_L00_002592B0_x(fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                            moby,
                            vars->blk12C,
                            0.02f,
                            0.3f,
                            0.2f);
        if (target_distance_xy < 20.0f && 8.0f < target_distance_xy &&
            fast_dec_timer(&vars->i128) != 0) {
            LevelCamera *cam = &l08_camera;
            if (fast_difference_between_rotations(
                    cam->yaw,
                    fast_arc_tan(moby->pos.x - cam->pos.f[0], moby->pos.y - cam->pos.f[1])) <
                0.55850536f) {
                goto attack;
            }
        }
        if (vars->i38 == 0 || !(0.5f < target_distance_xy)) {
            break;
        }
    attack:
        FUN_00212f90(vars->waypoint, 1, 0, scale_game_frames(6));
        if (moby->prev_seq != 2) {
            FUN_00212f90(moby, 2, 0, scale_game_frames(7));
        }
        moby->state = 4;
        moby->flags |= 0x1000;
        moby->collision = moby->pclass->collision;
        break;
    }
    case BOMBTHROWER_STATE_MOVE_TO_THROW_POSITION:
        point[0] = fast_cos(moby->rot.z) * 2.0f;
        point[1] = fast_sin(moby->rot.z) * 2.0f;
        point[2] = 0.0f;
        fast_vec_add(point, point, &vars->waypoint->pos);
        if (0.2f < vector_distance_xy(&moby->pos, point)) {
            FUN_L00_00259028((char *)moby, vars->blkD0, point, (char *)vars->v40);
        }
        if (moby->anim_flags & 2) {
            LevelCamera *cam;
            if (vector_distance_xy(&moby->pos, &target->pos) < 6.0f ||
                (cam = &l08_camera,
                 0.55850536f <
                     fast_difference_between_rotations(
                         cam->yaw,
                         fast_arc_tan(moby->pos.x - cam->pos.f[0], moby->pos.y - cam->pos.f[1])))) {
                old_waypoint = vars->waypoint;
                if ((vars->waypoint = (struct Moby *)FUN_L08_002daa10_c((char *)moby)) == 0) {
                    vars->waypoint = old_waypoint;
                }
                old_waypoint->command = 0;
                vars->waypoint->command = 1;
                if (moby->prev_seq != 4) {
                    FUN_00212f90(moby, 4, 0, scale_game_frames(7));
                }
                moby->state = 1;
                vars->i134 = scale_game_frames(0xF0);
            } else {
                if (moby->prev_seq != 3) {
                    FUN_00212f90(moby, 3, 0, scale_game_frames(6));
                }
                moby->state = 5;
            }
        }
        break;
    case BOMBTHROWER_STATE_THROW_BOMB:
        func_L00_002592B0_x(fast_arc_tan(target->pos.x - moby->pos.x, target->pos.y - moby->pos.y),
                            moby,
                            vars->blk12C,
                            0.02f,
                            0.3f,
                            0.2f);
        if (moby->seq == moby->prev_seq && FUN_0020c9e0(moby) == 19.0f) {
            float distance, time;
            char *next;
            fast_vec_sub(aim, &target->pos, &moby->pos);
            aim[2] = 0.0f;
            distance = fast_vec_length(aim);
            time = distance / FUN_L04_001f1978(l08_bombthrower_throw_timing_speed_gp);
            distance = distance - (l08_bombthrower_aim_shortening_rate_gp * time + 2.0f);
            if (10.0f < distance) {
                distance = 10.0f;
            } else if (distance < 3.0f) {
                distance = 3.0f;
            }
            FUN_L00_0025f730(aim, distance);
            fast_vec_add(aim, aim, &moby->pos);
            moby_point_position(moby, 9, point);
            fast_vec_sub(velocity, &target->pos, point);
            set_vector_length_xy(velocity, velocity, time);
            velocity[2] = FUN_L00_0025abf0(point, aim, &scratch, time, -(frame_time_sq * 15.0f));
            next = spawn_bomb_with_velocity((int)moby, (char *)point, (char *)velocity);
            vars->p124 = (struct Moby *)next;
            if (next != 0) {
                char *next_vars = *(char **)(next + 0x78);
                *(int *)(next_vars + 0x20) = scale_game_frames(0xB4);
                *(short *)(next_vars + 0xC) = scale_game_frames(10);
                *(short *)(next_vars + 0xE) = scale_game_frames(10);
                vars->p124->state = 1;
                vars->p124->command |= 3;
                vars->i128 = scale_game_frames(0xB4);
            }
            break;
        }
        if (moby->anim_flags & 2) {
            moby->state = 1;
            if (moby->prev_seq != 4) {
                FUN_00212f90(moby, 4, 0, 3);
            }
            vars->i134 = scale_game_frames(0xF0);
        }
        break;
    case BOMBTHROWER_STATE_WAIT_FOR_FALL_RECOVERY:
        if (moby->anim_flags & 2) {
            if (moby->prev_seq != 7) {
                FUN_00212f90(moby, 7, 0, 3);
            }
            moby->state = 0x5D;
        }
        break;
    case BOMBTHROWER_STATE_DAMAGE_KNOCKBACK:
    case BOMBTHROWER_STATE_MOTION_RECOVERY:
    case BOMBTHROWER_STATE_FALL_RECOVERY:
        if (step_moby_motion(moby, vars->blk70) & 0x40) {
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
            vars->i134 = scale_game_frames(0xF0);
        }
        if (moby->pos.z < 5.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case BOMBTHROWER_STATE_DYING:
        if (step_moby_motion(moby, vars->blk70) & 0x40) {
            moby->anim_speed = 0.0f;
            moby->state = 100;
            moby->collision = 0;
            vars->s132 = 0x28;
            vars->s130 = D_L08_001619D0_gp;
            spawn_moby_table_puff_pattern(moby);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    trooper_anim_finish(moby, vars->anim);
    vars->i38 = 0;
}
