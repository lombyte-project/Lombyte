/* Level 03 moby class mine_layer: patrols a path and lobs mines. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/enemies/trooper_set_knockback.h"
#include "rnc/hero/hero.h"
#include "qcopy.h"
#include "rnc/overlay/l03/mobys/select_moby_target_with_hero_fallback.h"
#include "rnc/overlay/l03/mobys/mine_layer_globals.h"

/* Patrol state of the l03 trooper: walks back and forth along a rail path. */
typedef struct {
    s32        idx; /* 0x00: path point it is walking to */
    s8         dir; /* 0x04: +1 or -1 */
    u8         pad5[0xB];
    LevelPath *path;       /* 0x10 */
    s32        path_index; /* 0x14: index into D_L03_001B05B0 */
} TrooperPatrol;

/* Pvars of the l03 trooper. */
typedef struct {
    u8            pad0[0x20];
    f32           health; /* 0x20 */
    s16           unk24;
    u8            pad26[2];
    u8            unk28;
    u8            unk29;
    u8            pad2A[0xE];
    s32           alerted; /* 0x38: set when it is hit */
    u8            pad3C[4];
    u8            effect[0x18]; /* 0x40: debris record (spawn_explosion, FUN_L00_00259028) */
    u8            unk58;
    u8            pad59;
    u8            unk5A;
    u8            pad5B[5];
    u8            anim[0x10];      /* 0x60: animation driver */
    u8            knockback[0x10]; /* 0x70 */
    f32           kb_speed;        /* 0x80 */
    u8            pad84[4];
    f32           kb_lift;    /* 0x88 */
    f32           kb_gravity; /* 0x8C */
    u8            pad90[4];
    s32           kb_frames; /* 0x94 */
    u8            pad98[0x15];
    u8            kb_landed; /* 0xAD */
    u8            padAE[0x12];
    f32           kb_min; /* 0xC0 */
    f32           kb_max; /* 0xC4 */
    u8            padC8[8];
    u8            walk[8]; /* 0xD0: walk driver (FUN_L01_0026d930, FUN_L00_00259028) */
    f32           unkD8;
    f32           unkDC;
    u8            padE0[8];
    f32           fall_speed; /* 0xE8 */
    u8            padEC[8];
    f32           unkF4;
    u8            padF8[0x14];
    f32           unk10C;
    f32           unk110;
    f32           unk114;
    u8            pad118[8];
    TrooperPatrol patrol; /* 0x120 */
    u8            pad138[0x18];
    u8            turn[4];      /* 0x150: turn driver (FUN_L00_00258278) */
    s16           fire_timer;   /* 0x154 */
    s16           patrol_timer; /* 0x156 */
    u8            pad158[4];
    s32           alert_timer;  /* 0x15C */
    u8            unk160[0x10]; /* 0x160: FUN_L00_00263ac8 record */
} TrooperVars;

s32        mine_layer_fire_interval_frames_gp __asm__("D_L03_00161918") = 0x12c;
extern f32 mine_layer_patrol_animation_speed_gp __asm__("D_L03_0016191C") __attribute__((sda));
extern f32 mine_layer_initial_scale_multiplier_gp __asm__("D_L03_00161920") __attribute__((sda));
extern f32 mine_layer_projectile_lateral_spread_speed_gp __asm__("D_L03_00161910")
    __attribute__((sda));
extern f32 mine_layer_projectile_vertical_launch_speed_gp __asm__("D_L03_00161914")
    __attribute__((sda));
extern char D_L03_00166EC0[];
extern void select_moby_target_with_hero_fallback(void *, void *, f32) __asm__("FUN_L03_0024e830");
extern void FUN_L00_00263ac8(struct Moby *, int, void *, f32);

void handle_mine_layer_damage(struct Moby *m) __asm__("FUN_L03_002ca808");

char       *spawn_mine_layer_projectile(char *p, void *pos, float *dir) __asm__("FUN_L03_002cc888");
extern void FUN_L01_0026d930(void *);
extern void blend_moby_animation(struct Moby *, int, int, int) __asm__("FUN_00212f90");
extern void FUN_L00_00258278(f32, f32, f32, f32, struct Moby *, void *);
extern void FUN_L00_00259028(struct Moby *, void *, void *, void *);
extern void trooper_play_sound(int, int, struct Moby *, int) __asm__("FUN_L00_0028dc90");
extern int  step_moby_motion(struct Moby *, void *) __asm__("FUN_L00_0025c698");
extern void spawn_explosion(struct Moby *,
                            void *,
                            void *,
                            f32,
                            f32,
                            s32,
                            s32,
                            s32,
                            f32,
                            f32,
                            f32,
                            f32,
                            s32,
                            f32,
                            s32,
                            s32,
                            s32,
                            s32) __asm__("FUN_L00_0025e450");
extern void FUN_L00_00257470(struct Moby *, int, int);
extern f32  probe_ground_height(void *, int, f32) __asm__("FUN_00213508");
extern void FUN_L00_0025a120(struct Moby *);

/* l03 trooper update: patrols its rail path, turns to face the hero once in sight,
   fires three-shot bursts, falls under gravity and blows up when shot down or
   when it drops into a pit. */
void mine_layer_update(struct Moby *m) __asm__("FUN_L03_002c9eb8");

void mine_layer_update(struct Moby *mine_layer_moby) {
    enum {
        MINE_LAYER_INITIALIZE = 0,
        MINE_LAYER_FACE_TARGET_AND_WAIT_FOR_RANGE = 1,
        MINE_LAYER_WAIT_FOR_ANIMATION_BEFORE_PATROL = 2,
        MINE_LAYER_PATROL_AND_FIRE = 3,
        MINE_LAYER_KNOCKBACK_AND_RECOVER = 4,
        MINE_LAYER_KNOCKBACK_UNTIL_EXPLOSION = 5
    };
    TrooperVars        *mine_layer_vars = (TrooperVars *)mine_layer_moby->pvars;
    TrooperPatrol      *pt;
    LevelPath          *path;
    float               sight;
    MobyTargetSelection target;
    float               muzzle[4];
    float               projectile_launch_velocity[4];
    float               spread[4];
    float               scale;
    float               ground;
    s32                 patrol_point_index;
    s32                 i;

    select_moby_target_with_hero_fallback(mine_layer_moby, &target, 100.0f);
    sight = 14.0f;
    FUN_L00_00263ac8(mine_layer_moby, 0, mine_layer_vars->unk160, 4.5f);
    handle_mine_layer_damage(mine_layer_moby);
    if (mine_layer_vars->alerted) {
        mine_layer_vars->alerted = 0;
        mine_layer_vars->alert_timer = scale_game_frames(240);
    }
    if (!fast_dec_timer(&mine_layer_vars->alert_timer)) {
        sight = 28.0f;
    }
    switch (mine_layer_moby->state) {
    case MINE_LAYER_INITIALIZE:
        FUN_L01_0026d930(mine_layer_vars->walk);
        mine_layer_vars->unk10C = frame_time_sq * 6.2831855f;
        mine_layer_vars->unk114 = frame_time * 6.2831855f;
        mine_layer_vars->unk110 = frame_time_sq * 18.849556f;
        mine_layer_vars->unkF4 = frame_time * 5.0f;
        mine_layer_vars->unk58 = 8;
        mine_layer_vars->unk5A = 8;
        mine_layer_vars->unkDC = 2.0f;
        mine_layer_vars->unkD8 = 2.0f;
        mine_layer_vars->health = 2.0f;
        mine_layer_vars->unk24 = 2;
        mine_layer_vars->unk28 = 1;
        mine_layer_vars->unk29 = 0;
        path = l03_paths[mine_layer_vars->patrol.path_index];
        mine_layer_vars->patrol.path = path;
        qcopy(&mine_layer_moby->pos, &path->points[0]);
        mine_layer_vars->patrol.idx = 1;
        mine_layer_moby->scale *= mine_layer_initial_scale_multiplier_gp;
        mine_layer_moby->state = 1;
        if (mine_layer_moby->prev_seq != 0) {
            blend_moby_animation(mine_layer_moby, 0, 0, 0);
        }
        break;
    case MINE_LAYER_FACE_TARGET_AND_WAIT_FOR_RANGE:
        FUN_L00_00258278(fast_arc_tan(target.aim.f[0] - mine_layer_moby->pos.x,
                                      target.aim.f[1] - mine_layer_moby->pos.y),
                         0.01f,
                         0.3f,
                         0.1f,
                         mine_layer_moby,
                         mine_layer_vars->turn);
        if (vector_distance_xy(&mine_layer_moby->pos, &target.aim) < sight &&
            absolute_float(target.aim.f[2] - mine_layer_moby->pos.z) < 4.0f) {
            mine_layer_moby->state = 2;
            if (mine_layer_moby->prev_seq != 1) {
                blend_moby_animation(mine_layer_moby, 1, 0, scale_game_frames(10));
            }
        }
        break;
    case MINE_LAYER_WAIT_FOR_ANIMATION_BEFORE_PATROL:
        if (mine_layer_moby->anim_flags & 2) {
            mine_layer_moby->state = 3;
            mine_layer_vars->patrol_timer = scale_game_frames(120);
            if (mine_layer_moby->prev_seq != 2) {
                blend_moby_animation(mine_layer_moby, 2, 0, scale_game_frames(10));
            }
            mine_layer_moby->anim_speed = mine_layer_patrol_animation_speed_gp;
            mine_layer_vars->fire_timer = scale_game_frames(120);
        }
        break;
    case MINE_LAYER_PATROL_AND_FIRE:
        pt = &mine_layer_vars->patrol;
        patrol_point_index = pt->idx;
        FUN_L00_00259028(mine_layer_moby,
                         mine_layer_vars->walk,
                         &pt->path->points[patrol_point_index],
                         mine_layer_vars->effect);
        mine_layer_moby->anim_speed = mine_layer_patrol_animation_speed_gp;
        fast_dec_timer_s16(&mine_layer_vars->patrol_timer);
        if (fast_vec_dist(&mine_layer_moby->pos, &pt->path->points[patrol_point_index]) < 1.0f) {
            if (mine_layer_vars->patrol_timer == 0 &&
                vector_distance_xy(&mine_layer_moby->pos, &target.aim) > 25.0f) {
                mine_layer_moby->state = 1;
                if (mine_layer_moby->prev_seq != 0) {
                    blend_moby_animation(mine_layer_moby, 0, 0, scale_game_frames(10));
                }
            } else {
                patrol_point_index = (pt->idx + pt->path->count + pt->dir) % pt->path->count;
                if (mine_layer_vars->patrol_timer == 0) {
                    scale = vector_distance_xy(&target.aim, &pt->path->points[patrol_point_index]);
                    if (scale < vector_distance_xy(&mine_layer_moby->pos,
                                                   &pt->path->points[patrol_point_index])) {
                        pt->dir *= -1;
                        mine_layer_vars->patrol_timer = scale_game_frames(120);
                    }
                }
                pt->idx = (pt->idx + pt->path->count + pt->dir) % pt->path->count;
            }
        }
        if (fast_dec_timer_s16(&mine_layer_vars->fire_timer)) {
            mine_layer_vars->fire_timer = scale_game_frames(mine_layer_fire_interval_frames_gp);
            qcopy(muzzle, &mine_layer_moby->pos);
            muzzle[2] += 2.0f;
            trooper_play_sound(1, 0, mine_layer_moby, 0x273);
            for (i = -2; i < 3; i++) {
                scale = convert_integer_to_float(i) * random_float_between(0.9f, 1.1f);
                set_vector_length_xy(muzzle, &mine_layer_moby->rot_row1, (float)i * 0.5f);
                fast_vec_add(muzzle, muzzle, &mine_layer_moby->pos);
                muzzle[2] += 2.0f;
                set_vector_length_xy(spread,
                                     &mine_layer_moby->rot_row0,
                                     random_float_between(-1.0f, 1.0f) * frame_time);
                set_vector_length_xy(projectile_launch_velocity,
                                     &mine_layer_moby->rot_row1,
                                     mine_layer_projectile_lateral_spread_speed_gp * frame_time);
                fast_vec_scale(projectile_launch_velocity, projectile_launch_velocity, scale);
                fast_vec_add(projectile_launch_velocity, projectile_launch_velocity, spread);
                projectile_launch_velocity[2] = mine_layer_projectile_vertical_launch_speed_gp *
                                                frame_time * random_float_between(0.9f, 1.1f);
                spawn_mine_layer_projectile(
                    (char *)mine_layer_moby, muzzle, projectile_launch_velocity);
            }
        }
        break;
    case MINE_LAYER_KNOCKBACK_AND_RECOVER:
        if (step_moby_motion(mine_layer_moby, mine_layer_vars->knockback) & 0x60) {
            if (mine_layer_moby->prev_seq != 0) {
                blend_moby_animation(mine_layer_moby, 0, 0, scale_game_frames(10));
            }
            mine_layer_moby->state = 1;
        } else if (mine_layer_moby->pos.z < 5.0f) {
            qcopy(muzzle, &mine_layer_moby->pos);
            muzzle[2] += 0.5f;
            spawn_explosion(mine_layer_moby,
                            mine_layer_vars->effect,
                            muzzle,
                            0.0f,
                            0.0f,
                            5,
                            2,
                            4,
                            2.0f,
                            1.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            FUN_L00_00257470(mine_layer_moby, 0, -1);
            mark_moby_for_removal(mine_layer_moby);
            return;
        }
        break;
    case MINE_LAYER_KNOCKBACK_UNTIL_EXPLOSION:
        if ((step_moby_motion(mine_layer_moby, mine_layer_vars->knockback) & 0x60) ||
            mine_layer_moby->pos.z < 5.0f) {
            qcopy(muzzle, &mine_layer_moby->pos);
            muzzle[2] += 0.5f;
            spawn_explosion(mine_layer_moby,
                            mine_layer_vars->effect,
                            muzzle,
                            0.0f,
                            0.0f,
                            5,
                            2,
                            4,
                            2.0f,
                            1.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            FUN_L00_00257470(mine_layer_moby, 0, -1);
            mark_moby_for_removal(mine_layer_moby);
            return;
        }
        break;
    }
    if (mine_layer_moby->state < 4) {
        mine_layer_vars->fall_speed += frame_time_sq * 9.8f;
        mine_layer_moby->pos.z -= mine_layer_vars->fall_speed - 2.0f;
        ground = probe_ground_height(&mine_layer_moby->pos, 0, 0.5f);
        mine_layer_moby->pos.z -= 2.0f;
        if (mine_layer_moby->pos.z < ground) {
            mine_layer_moby->pos.z = ground;
            mine_layer_vars->fall_speed = 0.0f;
        } else if (mine_layer_moby->pos.z < 5.0f) {
            qcopy(muzzle, &mine_layer_moby->pos);
            muzzle[2] += 0.5f;
            spawn_explosion(mine_layer_moby,
                            mine_layer_vars->effect,
                            muzzle,
                            0.0f,
                            0.0f,
                            5,
                            2,
                            4,
                            2.0f,
                            1.0f,
                            9.0f,
                            1.0f,
                            -1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            FUN_L00_00257470(mine_layer_moby, 0, -1);
            mark_moby_for_removal(mine_layer_moby);
            return;
        }
    }
    if (mine_layer_moby->visible != 0 &&
        fast_vec_dist(&mine_layer_moby->pos, D_L03_00166EC0) < 32.0f) {
        FUN_L00_0025a120(mine_layer_moby);
    }
}

typedef struct {
    u8           pad0[0x10];
    Vec4         pos;    /* 0x10: where the hit landed */
    struct Moby *source; /* 0x20: moby that dealt it */
} TrooperHit;

extern f32 mine_layer_lethal_knockback_speed_scale_gp __asm__("D_L03_001618F8")
    __attribute__((sda));
extern f32 mine_layer_nonlethal_knockback_speed_scale_gp __asm__("D_L03_001618FC")
    __attribute__((sda));

#include "rnc/overlay/l03/mobys/mine_layer_data/handle_mine_layer_damage.h"

extern TrooperHit *trooper_find_hit(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern s32 fun_00213928(struct Moby *, TrooperHit *, f32 *, s32, s32 *, f32 *, s32, s32) __asm__("FUN_00213928");
extern void trooper_knockback_arc(Vec4 *, f32 *, f32 *, f32 *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(f32, struct Moby *, void *, s32, s32, s32) __asm__("FUN_L00_0025c558");
extern void trooper_anim_update(struct Moby *, void *) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(struct Moby *, void *) __asm__("FUN_L00_0025d538");

extern f32 frame_time_sq_macro __asm__("D_0015ED70") MACRO_ADDR;

/* Trooper hit handler: takes damage, then knocks back away from the attacker by hit strength (dying on lethal hits). */
void handle_mine_layer_damage(struct Moby *damaged_moby) {
    enum {
        TROOPER_DAMAGE_LETHAL = 1,
        TROOPER_DAMAGE_SHORT_KNOCKBACK = 3,
        TROOPER_DAMAGE_LONG_KNOCKBACK = 4,
        TROOPER_DAMAGE_LONG_KNOCKBACK_ALTERNATE = 5,
        TROOPER_DAMAGE_SHORT_KNOCKBACK_ALTERNATE = 6
    };
    TrooperVars *trooper_vars = (TrooperVars *)damaged_moby->pvars;
    TrooperHit  *hit_record;
    Vec4         at;
    s32          hit;
    f32          dmg;
    f32          knockback_arc_angle;
    s32          damage_response;

    dmg = 0.0f;
    hit_record = trooper_find_hit(damaged_moby, 0x330000, 0);
    damage_response =
        fun_00213928(damaged_moby, hit_record, &trooper_vars->health, 0, &hit, &dmg, 0, 4);
    if (hit != 1 && damaged_moby->state != 5) {
        if (hit_record->source != NULL) {
            if (hit_record->source != hero.items[0].moby)
                knockback_arc_angle = fast_arc_tan(damaged_moby->pos.x - hit_record->source->pos.x,
                                                   damaged_moby->pos.y - hit_record->source->pos.y);
            else
                knockback_arc_angle = fast_arc_tan(damaged_moby->pos.x - hero.motion.pos.f[0],
                                                   damaged_moby->pos.y - hero.motion.pos.f[1]);
        } else {
            knockback_arc_angle = fast_arc_tan(damaged_moby->pos.x - hero.motion.pos.f[0],
                                               damaged_moby->pos.y - hero.motion.pos.f[1]);
        }
        trooper_vars->health -= dmg;
        if (trooper_vars->health <= 0.0f)
            damage_response = 1;
        switch (damage_response) {
        case TROOPER_DAMAGE_SHORT_KNOCKBACK:
        case TROOPER_DAMAGE_SHORT_KNOCKBACK_ALTERNATE:
            trooper_vars->kb_speed =
                mine_layer_nonlethal_knockback_speed_scale_gp * frame_time_sq_macro;
            trooper_set_knockback(trooper_vars->knockback,
                                  mine_layer_nonlethal_knockback_horizontal_speed_scale_gp,
                                  mine_layer_nonlethal_knockback_vertical_launch_input_gp);
            trooper_vars->kb_landed = 0;
            trooper_vars->kb_frames = 9;
            *(u128 *)&at = *(u128 *)&hit_record->pos;
            trooper_knockback_arc(
                &at, &knockback_arc_angle, &trooper_vars->kb_lift, &trooper_vars->kb_gravity);
            trooper_start_knockback(fast_arc_tan(damaged_moby->pos.x - hero.motion.pos.f[0],
                                                 damaged_moby->pos.y - hero.motion.pos.f[1]),
                                    damaged_moby,
                                    trooper_vars->knockback,
                                    3,
                                    1,
                                    0);
            trooper_vars->kb_min = 5.0f;
            trooper_vars->kb_max = 10.0f;
            damaged_moby->state = 4;
            break;
        case TROOPER_DAMAGE_LONG_KNOCKBACK:
        case TROOPER_DAMAGE_LONG_KNOCKBACK_ALTERNATE:
            trooper_vars->kb_speed =
                mine_layer_nonlethal_knockback_speed_scale_gp * frame_time_sq_macro;
            trooper_set_knockback(trooper_vars->knockback,
                                  mine_layer_nonlethal_knockback_horizontal_speed_scale_gp,
                                  mine_layer_nonlethal_knockback_vertical_launch_input_gp);
            trooper_vars->kb_landed = 0;
            trooper_vars->kb_frames = 9;
            *(u128 *)&at = *(u128 *)&hit_record->pos;
            trooper_knockback_arc(
                &at, &knockback_arc_angle, &trooper_vars->kb_lift, &trooper_vars->kb_gravity);
            trooper_start_knockback(fast_arc_tan(damaged_moby->pos.x - hero.motion.pos.f[0],
                                                 damaged_moby->pos.y - hero.motion.pos.f[1]),
                                    damaged_moby,
                                    trooper_vars->knockback,
                                    4,
                                    1,
                                    0);
            trooper_vars->kb_min = 7.5f;
            trooper_vars->kb_max = 15.0f;
            damaged_moby->state = 4;
            break;
        case TROOPER_DAMAGE_LETHAL:
            trooper_vars->kb_speed =
                mine_layer_lethal_knockback_speed_scale_gp * frame_time_sq_macro;
            trooper_set_knockback(trooper_vars->knockback,
                                  mine_layer_lethal_knockback_horizontal_speed_scale_gp,
                                  mine_layer_lethal_knockback_vertical_launch_input_gp);
            trooper_vars->kb_landed = 0;
            trooper_vars->kb_frames = 9;
            *(u128 *)&at = *(u128 *)&hit_record->pos;
            trooper_knockback_arc(
                &at, &knockback_arc_angle, &trooper_vars->kb_lift, &trooper_vars->kb_gravity);
            trooper_start_knockback(fast_arc_tan(damaged_moby->pos.x - hero.motion.pos.f[0],
                                                 damaged_moby->pos.y - hero.motion.pos.f[1]),
                                    damaged_moby,
                                    trooper_vars->knockback,
                                    5,
                                    1,
                                    0);
            trooper_vars->kb_min = 8.0f;
            trooper_vars->kb_max = 15.0f;
            damaged_moby->flags &= 0xEFFF;
            *((u8 *)trooper_vars + 0x67) = 0xFA;
            damaged_moby->state = 5;
            break;
        }
        trooper_anim_update(damaged_moby, trooper_vars->anim);
    }
    damaged_moby->hit_slot = 0xFF;
    trooper_anim_finish(damaged_moby, trooper_vars->anim);
}

extern void set_path_2cc888(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");

/* Spawns a projectile moby (class 0x273) at pos, heading along dir,
 * owned by parent p. */
char *spawn_mine_layer_projectile(char *parent_moby, void *spawn_position, float *launch_velocity) {
    char *spawned_moby = create_moby(0x273);
    char *spawned_moby_vars;
    float frame_scale_squared;

    if (spawned_moby == 0) {
        return 0;
    }
    MOBY(spawned_moby)->draw_dist = ((unsigned short)MOBY(parent_moby)->draw_dist);
    spawned_moby[0x30] = parent_moby[0x32];
    spawned_moby[0x31] = 1;
    *(long long *)(spawned_moby + 0x38) = *(long long *)(parent_moby + 0x38);
    qcopy(spawned_moby + 0x10, spawn_position);
    spawned_moby_vars = ((char *)MOBY(spawned_moby)->pvars);
    frame_scale_squared = frame_scale_sq;
    *(float *)(spawned_moby_vars + 0x10) = frame_scale_squared * 0.008f;
    *(float *)(spawned_moby_vars + 0x14) = frame_scale_squared * 0.0005f;
    *(float *)(spawned_moby_vars + 0x18) = vector_length_xy(launch_velocity);
    *(float *)(spawned_moby_vars + 0x1C) = launch_velocity[2];
    *(int *)(spawned_moby_vars + 0x20) = 0x400;
    *(float *)(spawned_moby_vars + 0x28) = 1.0f;
    *(float *)(spawned_moby_vars + 0x30) = 0.75f;
    *(float *)(spawned_moby_vars + 0x34) = 0.5f;
    *(float *)(spawned_moby_vars + 0x38) = 0.65f;
    *(float *)(spawned_moby_vars + 0x48) = 0.2f;
    *(float *)(spawned_moby_vars + 0x4C) = 0.01f;
    spawned_moby_vars[0x3D] = 0;
    *(int *)(spawned_moby_vars + 0x24) = 1;
    set_path_2cc888(spawned_moby,
                    spawned_moby_vars,
                    0,
                    1,
                    0,
                    fast_arc_tan(launch_velocity[0], launch_velocity[1]));
    *(char **)(spawned_moby_vars + 0x68) = parent_moby;
    *(int *)(spawned_moby_vars + 0x6C) = 0;
    spawned_moby[0x20] = 1;
    return spawned_moby;
}
