/* Level 03 moby classes: mine_layer, mine, taxi, robo_shack_sign, helga, big_al, robot_qwark. */
#include "types.h"
#include "rnc/game/level_object_states.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/collision.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/vector.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/audio/streaming/set_moby_dialogue_record_value.h"
#include "rnc/overlay/shared/mobys/initialize_moby_grounding.h"
#include "rnc/overlay/shared/mobys/record_moby_context_bytes.h"
#include "rnc/overlay/shared/enemies/trooper_set_knockback.h"
#include "rnc/ui/help/try_set_help_message.h"

/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_002CB280.c: func_L03_002CDBF0), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");

void handle_mine_hit(unsigned char *mine_moby) __asm__("FUN_L03_002cc828");

void handle_mine_hit(unsigned char *mine_moby) {
    char *item = find_moby_hit(mine_moby, 0x130000, 0);
    if (item != 0) {
        char *other = *(char **)(item + 0x20);
        if (other != 0 && *(short *)(other + 0xA6) != MOBY(mine_moby)->oclass) {
            mine_moby[0x20] = 4;
        }
    }
    mine_moby[0xA4] = 0xFF;
}
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_002CB280.c: func_L03_002D44C8), where it is exact; names translated to the US level program. */

void request_taxi_return_help_message(void *taxi_moby, int mode) __asm__("FUN_L03_002d3100");

void request_taxi_return_help_message(void *taxi_moby, int mode) {
    if (mode == 1) {
        try_set_help_message(8, 0xBC9);
    } else {
        try_set_help_message(8, 0xBC9);
    }
}
/* Sets two speed pairs on a moby's data from level constants scaled by degrees-to-radians and the frame scale. */
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_002CB280.c: func_L03_002D5008), where it is exact; names translated to the US level program. */

#include "rnc/overlay/l03/mobys/taxi_globals.h"

extern float taxi_bobbing_amplitude_gp __asm__("D_L03_00161AF0") __attribute__((sda));
extern float taxi_bobbing_phase_rate_degrees_gp __asm__("D_L03_00161AF4") __attribute__((sda));
extern float taxi_rocking_amplitude_gp __asm__("D_L03_00161AF8") __attribute__((sda));
extern float taxi_rocking_first_phase_rate_degrees_gp __asm__("D_L03_00161AFC")
    __attribute__((sda));
extern float taxi_rocking_second_phase_rate_degrees_gp __asm__("D_L03_00161B00")
    __attribute__((sda));
extern void update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
extern void update_moby_rotation_oscillation(char *, char *, char *, float, float, float) __asm__("FUN_L00_00262b80");

void update_taxi_bobbing_and_rocking(struct Moby *taxi_moby) __asm__("FUN_L03_002d3c40");

void update_taxi_bobbing_and_rocking(struct Moby *taxi_moby) {
    char *moby_vars = (char *)taxi_moby->pvars;
    update_moby_vertical_oscillation(taxi_bobbing_amplitude_gp,
                                     taxi_bobbing_phase_rate_degrees_gp * DEG_TO_RAD * frame_time,
                                     taxi_moby,
                                     (float *)(moby_vars + 0xEC),
                                     (float *)(moby_vars + 0xE8));
    update_moby_rotation_oscillation(
        taxi_moby,
        moby_vars + 0xF0,
        moby_vars + 0xF4,
        taxi_rocking_amplitude_gp,
        taxi_rocking_first_phase_rate_degrees_gp * DEG_TO_RAD * frame_time,
        taxi_rocking_second_phase_rate_degrees_gp * DEG_TO_RAD * frame_time);
}

#include "rnc/hero/hero.h"
#include "qcopy.h"
#include "rnc/audio/voice_pool.h"

struct RailPath {
    s32  count;
    u8   pad4[0xC];
    Vec4 pts[1]; /* 0x10: xyz point, w length of the segment to the next */
};

/* What FUN_L03_0024e830 finds within range of a moby (rocket, trooper). */
typedef struct {
    Vec4 aim; /* 0x00: point to aim at (the trooper turns toward it) */
    u8   pad10[0x10];
    Vec4 pos; /* 0x20 */
    u8   pad30[0x10];
    s32  found; /* 0x40 */
    u8   pad44[0xC];
} RocketTarget;

/* Patrol state of the l03 trooper: walks back and forth along a rail path. */
typedef struct {
    s32              idx; /* 0x00: path point it is walking to */
    s8               dir; /* 0x04: +1 or -1 */
    u8               pad5[0xB];
    struct RailPath *path;       /* 0x10 */
    s32              path_index; /* 0x14: index into D_L03_001B05B0 */
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
void        handle_mine_layer_damage(struct Moby *damaged_moby) __asm__("FUN_L03_002ca808");

void  handle_mine_layer_damage(struct Moby *m) __asm__("FUN_L03_002ca808");
char *spawn_mine_layer_projectile(char  *parent_moby,
                                  void  *pos,
                                  float *launch_velocity) __asm__("FUN_L03_002cc888");

char       *spawn_mine_layer_projectile(char *p, void *pos, float *dir) __asm__("FUN_L03_002cc888");
extern void FUN_L01_0026d930(void *);
extern void blend_moby_animation(struct Moby *, int, int, int) __asm__("FUN_00212f90");
extern void FUN_L00_00258278(f32, f32, f32, f32, struct Moby *, void *);
extern void FUN_L00_00259028(struct Moby *, void *, void *, void *);
extern void trooper_play_sound(int, int, struct Moby *, int) __asm__("FUN_L00_0028dc90");
extern int  mine_fall(struct Moby *, void *) __asm__("FUN_L00_0025c698");
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
    TrooperVars     *mine_layer_vars = (TrooperVars *)mine_layer_moby->pvars;
    TrooperPatrol   *pt;
    struct RailPath *path;
    float            sight;
    RocketTarget     target;
    float            muzzle[4];
    float            projectile_launch_velocity[4];
    float            spread[4];
    float            scale;
    float            ground;
    s32              patrol_point_index;
    s32              i;

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
        path = ((struct RailPath *)l03_paths[mine_layer_vars->patrol.path_index]);
        mine_layer_vars->patrol.path = path;
        qcopy(&mine_layer_moby->pos, &path->pts[0]);
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
                         &pt->path->pts[patrol_point_index],
                         mine_layer_vars->effect);
        mine_layer_moby->anim_speed = mine_layer_patrol_animation_speed_gp;
        fast_dec_timer_s16(&mine_layer_vars->patrol_timer);
        if (fast_vec_dist(&mine_layer_moby->pos, &pt->path->pts[patrol_point_index]) < 1.0f) {
            if (mine_layer_vars->patrol_timer == 0 &&
                vector_distance_xy(&mine_layer_moby->pos, &target.aim) > 25.0f) {
                mine_layer_moby->state = 1;
                if (mine_layer_moby->prev_seq != 0) {
                    blend_moby_animation(mine_layer_moby, 0, 0, scale_game_frames(10));
                }
            } else {
                patrol_point_index = (pt->idx + pt->path->count + pt->dir) % pt->path->count;
                if (mine_layer_vars->patrol_timer == 0) {
                    scale = vector_distance_xy(&target.aim, &pt->path->pts[patrol_point_index]);
                    if (scale < vector_distance_xy(&mine_layer_moby->pos,
                                                   &pt->path->pts[patrol_point_index])) {
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
        if (mine_fall(mine_layer_moby, mine_layer_vars->knockback) & 0x60) {
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
        if ((mine_fall(mine_layer_moby, mine_layer_vars->knockback) & 0x60) ||
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

#include "rnc/overlay/l03/mobys/taxi_data/handle_mine_layer_damage.h"

extern TrooperHit *trooper_find_hit(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern s32         trooper_take_damage(
            struct Moby *, TrooperHit *, f32 *, s32, s32 *, f32 *, s32, s32) __asm__("FUN_00213928");
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
        trooper_take_damage(damaged_moby, hit_record, &trooper_vars->health, 0, &hit, &dmg, 0, 4);
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

#include "rnc/overlay/quad.h"

typedef float MineVec[4] __attribute__((aligned(16)));

/* Explosion description handed to mine_camera_blast (0x50 bytes, copied from a level constant). */
typedef struct {
    OvlQuad q[5];
} MineBlast;

/* Pvars of the l03 mine. */
typedef struct {
    u8           pad0[0x60]; /* 0x00: fall record (mine_fall) */
    s32          unk60;
    s32          timer;  /* 0x64: arming delay */
    struct Moby *target; /* 0x68: moby it is stuck to or homing on */
    s32          age;    /* 0x6C: frames alive */
} MineVars;

extern MineBlast    l03_mine_camera_blast_template __asm__("D_L03_001E32B0");
extern struct Moby *D_L03_00178000[];
extern char         mine_moby_index_list[] __asm__("D_L03_001B08B0");
extern void allocate_voice_for_target_entry(int, int, struct Moby *) __asm__("FUN_0022da68");
extern int
collect_mobies_in_sphere(void *, int, struct Moby *, void *, f32) __asm__("FUN_L00_001f2868");
extern void
send_blast_hits(struct Moby *, void *, void *, int, int, int, int, int, f32, f32, f32) __asm__("FUN_L00_0025a9f8");
extern s32  random_int_between(s32, s32) __asm__("FUN_L00_00257b90");
extern int  mine_blend_color(int, int, f32) __asm__("FUN_L00_002371e0");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("FUN_00214db0");
extern void mine_debris(void *, void *, int, int, f32, int, int) __asm__("FUN_L00_0026bed0");
extern void mine_camera_blast(void *, void *, int, int) __asm__("FUN_L00_002d3838");
extern void mine_spawn_drops(struct Moby *, void *) __asm__("FUN_L00_0025f800");

/* l03 mine update: falls until it lands, sticks to or homes on its target, arms,
   and explodes on contact (debris, camera shake, blast) or shrinks away when old. */
void update_mine(struct Moby *mine_moby) __asm__("FUN_L03_002cbea8");

void update_mine(struct Moby *mine_moby) {
    enum {
        MINE_IDLE = 0,
        MINE_FALL_UNTIL_LANDING = 1,
        MINE_HOME_ON_TARGET = 2,
        MINE_ARM_AND_MONITOR_PROXIMITY = 3,
        MINE_EXPLODE_AND_REMOVE = 4,
        MINE_SHRINK_AND_REMOVE = 5
    };
    enum {
        MINE_DEBRIS_FIXED_PALETTE_COLOR = 0,
        MINE_DEBRIS_WIDE_SPAWN_RADIUS = 1,
        MINE_DEBRIS_RANDOM_PALETTE_COLOR = 2,
        MINE_DEBRIS_SPHERICAL_VELOCITY = 3
    };
    MineBlast    blast = l03_mine_camera_blast_template;
    MineVec      vector_scratch;
    MineVars    *mine_vars = (MineVars *)mine_moby->pvars;
    struct Moby *target_moby;
    float        explosion_reference_distance;
    int          nearby_moby_count;
    int          i;
    int          k;

    handle_mine_hit((unsigned char *)mine_moby);
    mine_vars->unk60 = 0;
    mine_vars->age++;
    switch (mine_moby->state) {
    case MINE_IDLE:
        break;
    case MINE_FALL_UNTIL_LANDING:
        if (mine_fall(mine_moby, mine_vars) & 1) {
            mine_moby->state = 3;
            if (mine_moby->prev_seq != 1) {
                blend_moby_animation(mine_moby, 1, 0, scale_game_frames(10));
            }
            mine_vars->timer = scale_game_frames(60);
        } else if (mine_moby->pos.z < 5.0f) {
            mark_moby_for_removal(mine_moby);
            return;
        }
        break;
    case MINE_HOME_ON_TARGET:
        fast_vec_sub(vector_scratch, &mine_vars->target->pos, &mine_moby->pos);
        if (fast_vec_length(vector_scratch) < 0.25f) {
            mark_moby_for_removal(mine_moby);
            return;
        }
        fast_vec_normalize(vector_scratch, vector_scratch, frame_time * 14.0f);
        vector_scratch[2] += frame_time * 4.0f;
        fast_vec_add(&mine_moby->pos, &mine_moby->pos, vector_scratch);
        break;
    case MINE_ARM_AND_MONITOR_PROXIMITY:
        fast_dec_timer(&mine_vars->timer);
        target_moby = mine_vars->target;
        if (target_moby != 0 && target_moby->state != 0xFE && target_moby->state != 0xFD &&
            vector_distance_xy(&mine_moby->pos, &target_moby->pos) < 3.0f &&
            mine_vars->timer == 0) {
            mine_moby->state = 2;
            allocate_voice_for_target_entry(2, 0, mine_moby);
        }
        if (scale_game_frames(3600) < mine_vars->age &&
            (mine_vars->target == 0 || mine_vars->target->state == 0xFE ||
             mine_vars->target->state == 0xFD || scale_game_frames(5400) < mine_vars->age)) {
            mine_moby->state = 5;
        }
        if (vector_distance_xy(&hero.moby->pos, &mine_moby->pos) < 0.5f &&
            absolute_float(mine_moby->pos.z - hero.motion.pos.f[2]) < 0.25f) {
            mine_moby->state = 4;
            break;
        }
        nearby_moby_count = collect_mobies_in_sphere(&mine_moby->pos, 0x10, mine_moby, 0, 1.0f);
        for (k = 0; k < nearby_moby_count; k++) {
            struct Moby *nearby_moby = D_L03_00178000[k];

            if ((nearby_moby->flags & 0x1000) &&
                (nearby_moby != mine_vars->target || mine_vars->timer == 0)) {
                mine_moby->state = 4;
                break;
            }
        }
        break;
    case MINE_EXPLODE_AND_REMOVE: {
        int debris_spawn_count;

        explosion_reference_distance = vector_distance_xy(&mine_moby->pos, D_L03_00166EC0);
        nearby_moby_count = collect_mobies_in_sphere(&mine_moby->pos, 0x10, mine_moby, 0, 2.0f);
        *(OvlQuad *)vector_scratch = *(OvlQuad *)&mine_moby->pos;
        send_blast_hits(mine_moby,
                        vector_scratch,
                        D_L03_00178000,
                        nearby_moby_count,
                        0,
                        0x810001,
                        4,
                        1,
                        3.0f,
                        0.25f,
                        1.5f);
        debris_spawn_count = truncate_float_to_s32(explosion_reference_distance * 15.0f);
        for (i = 0; i < debris_spawn_count; i++) {
            int kind = random_integer_below(4);

            *(Vec4f *)vector_scratch =
                (Vec4f){frame_time * 14.0f, frame_time * 22.0f, frame_time * 12.0f, 0.0f};
            {
                MineVec debris_velocity;
                MineVec debris_vector_scratch;
                float   rr;
                float   debris_azimuth;

                *(OvlQuad *)debris_vector_scratch = 0;
                debris_vector_scratch[0] =
                    random_float_between(frame_time * -0.5f, frame_time * 0.5f);
                debris_vector_scratch[1] =
                    random_float_between(frame_time * -0.5f, frame_time * 0.5f);
                debris_vector_scratch[2] =
                    random_float_between(frame_time * 4.0f, vector_scratch[kind]);
                *(OvlQuad *)debris_velocity = *(OvlQuad *)debris_vector_scratch;
                rr = random_float_between(0.0f, kind == 1 ? 0.5f : 0.25f);
                debris_azimuth = random_angle_radians();
                debris_vector_scratch[0] = fast_cos(debris_azimuth) * rr;
                debris_vector_scratch[1] = fast_sin(debris_azimuth) * rr;
                debris_vector_scratch[2] = 0.0f;
                fast_vec_add(debris_vector_scratch, debris_vector_scratch, &mine_moby->pos);
                debris_vector_scratch[2] -= rr * fast_cos(0.785398f);
                debris_velocity[2] -= rr * frame_time * 8.0f;
                switch (kind) {
                case MINE_DEBRIS_FIXED_PALETTE_COLOR:
                    mine_debris(
                        debris_vector_scratch,
                        debris_velocity,
                        0x1F101820,
                        0x101010,
                        random_float_between(200000.0f, 300000.0f),
                        random_int_between(scale_game_frames(0xB4), scale_game_frames(0xF0)),
                        0);
                    break;
                case MINE_DEBRIS_WIDE_SPAWN_RADIUS:
                    mine_debris(debris_vector_scratch,
                                debris_velocity,
                                0x3F081020,
                                0x0F081020,
                                random_float_between(50000.0f, 100000.0f),
                                scale_game_frames(0xB4),
                                1);
                    break;
                case MINE_DEBRIS_RANDOM_PALETTE_COLOR:
                    mine_debris(
                        debris_vector_scratch,
                        debris_velocity,
                        random_integer_below(100) < 0x28 ? 0x5FF8F8F8 : 0x2F486078,
                        0x0F000020,
                        150000.0f,
                        random_int_between(scale_game_frames(0x1E), scale_game_frames(0x2D)),
                        2);
                    break;
                case MINE_DEBRIS_SPHERICAL_VELOCITY: {
                    int debris_spawn_color =
                        mine_blend_color(0x7F000000, 0x7F182030, random_float_between(0.25f, 1.0f));
                    int debris_grayscale_color =
                        mine_blend_color(0, 0x5F5F5F, random_float_between(0.5f, 1.0f));

                    build_spherical_offset(debris_velocity,
                                           random_float_between(0.0f, 1.0f) * frame_time,
                                           debris_azimuth,
                                           random_angle_radians());
                    mine_debris(
                        debris_vector_scratch,
                        debris_velocity,
                        debris_spawn_color,
                        debris_grayscale_color,
                        random_float_between(200000.0f, 300000.0f),
                        random_int_between(scale_game_frames(0xF0), scale_game_frames(0x12C)),
                        3);
                    break;
                }
                }
            }
        }
        allocate_voice_for_target_entry(0, 0, mine_moby);
        l03_camera.shake_amount = explosion_reference_distance < 20.0f
                                      ? 0.4f - explosion_reference_distance * 0.0175f
                                      : 0.050000012f;
        l03_camera.shake_frames = scale_game_frames(0x19);
        mine_camera_blast(&blast, &mine_moby->pos, 0, 0);
        mine_spawn_drops(mine_moby, mine_moby_index_list);
        mark_moby_for_removal(mine_moby);
        return;
    }
    case MINE_SHRINK_AND_REMOVE:
        mine_moby->scale *= frame_scale * -0.074f + 1.0f;
        if (mine_moby->scale < mine_moby->pclass->scale * 0.01f) {
            mine_spawn_drops(mine_moby, mine_moby_index_list);
            mark_moby_for_removal(mine_moby);
        }
        break;
    }
}

#include "qcopy.h"

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

/* Test the observed halfword and byte range without inferring object identity. */
s32 is_taxi_in_round_trip_phase(u8 *object) __asm__("FUN_L03_002d30d8");

s32 is_taxi_in_round_trip_phase(u8 *object) {
    if (*(s16 *)(object + 0xa6) != 0x330) {
        return 0;
    }
    return (u32)(object[0x20] - 2) < 5;
}

#include "rnc/mobys/moby.h"

/* Pvars of a rail taxi: up to five routes, each called by a trigger volume and
   made of an outbound path, a return path and an optional onward path. */
struct RailTaxiVars {
    u8  pad0[0x20];
    u8  coll[0x40]; /* 0x20: collision record moved with the taxi */
    f32 speed;      /* 0x60: FUN_L03_002d3918 state (RailFlyerVars) */
    s32 idx;        /* 0x64 */
    f32 yaw_vel;    /* 0x68 */
    f32 pitch_vel;  /* 0x6C */
    f32 roll_vel;   /* 0x70 */
    s32 route;      /* 0x74: route kept while the hero stays aboard (-1: none) */
    s32 snd;        /* 0x78: travel sound voice */
    s32 active;     /* 0x7C */
    s32 volume[5];  /* 0x80: trigger clip volume of each route (-1: none) */
    s32 out[5];     /* 0x94: rail path index of each route's outbound leg (-1: none) */
    s32 back[5];    /* 0xA8: return leg */
    s32 onward[5];  /* 0xBC: optional leg taken when the hero walks away (-1: none) */
    s32 help[5];    /* 0xD0: help message mode at each route's stop (FUN_L03_002d3100) */
    s32 unkE4;      /* 0xE4: set to -1 on arrival */
};

s32        taxi_path_point_debug_draw_enabled_gp __asm__("D_L03_00161B04") = 0;
extern f32 rail_flyer_max_speed_gp __asm__("D_L03_00161B08") __attribute__((sda));
extern f32 rail_flyer_pitch_acceleration_degrees_gp __asm__("D_L03_00161B10") __attribute__((sda));
extern f32 rail_flyer_roll_acceleration_degrees_gp __asm__("D_L03_00161B18") __attribute__((sda));

#include "rnc/overlay/level_save_state.h"

extern s32 D_L03_0015F594 __attribute__((sda));
extern void
draw_number(void *, struct Moby *, s32, u32, s32, s32, s32, f32) __asm__("FUN_L01_00285768");
extern int  is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern int  is_hero_on_moby(void *) __asm__("FUN_L01_00277fb8");
extern void request_hero_target_pose(void *, float, int) __asm__("FUN_L00_00233ee8");
extern s32  get_hero_target_pose_status(void) __asm__("FUN_L00_00233f38");
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern f32  approach_angle(f32 *, f32, f32 *, f32, f32, f32) __asm__("FUN_L00_0025be00");
s32         advance_moby_along_rail_path(struct Moby *rail_moby,
                                         s32          path,
                                         s32         *snd) __asm__("FUN_L03_002d3918");

s32 advance_moby_along_rail_path(struct Moby *m, s32 path, s32 *snd) __asm__("FUN_L03_002d3918");

/* Rail taxi: waits at its stop until the hero enters a route's trigger volume,
   flies the outbound path, lets the hero off, flies back (or onward) and returns
   to waiting. With the debug flag set it draws its first route's paths. */
void update_rail_taxi(struct Moby *taxi_moby) __asm__("FUN_L03_002d3198");

void update_rail_taxi(struct Moby *taxi_moby) {
    enum {
        TAXI_INITIALIZE = 0,
        TAXI_WAIT_FOR_ROUTE_TRIGGER = 1,
        TAXI_FOLLOW_OUTBOUND_PATH = 2,
        TAXI_WAIT_FOR_RETURN_REQUEST = 3,
        TAXI_WAIT_FOR_RETURN_DEPARTURE = 4,
        TAXI_FOLLOW_RETURN_PATH = 5,
        TAXI_WAIT_FOR_HERO_TO_MOVE_AWAY = 6,
        TAXI_FOLLOW_ONWARD_PATH = 7
    };
    struct RailTaxiVars *taxi_vars = (struct RailTaxiVars *)taxi_moby->pvars;
    Vec4                 oldPos, oldRot, move;
    struct RailPath     *route_path;
    s32                  i;
    s32                  route_index;

    set_moby_dialogue_record_value(taxi_moby, is_taxi_in_round_trip_phase((u8 *)taxi_moby));
    if (taxi_path_point_debug_draw_enabled_gp != 0) {
        for (i = 0; i < ((struct RailPath *)l03_paths[taxi_vars->out[0]])->count; i++) {
            draw_number(&((struct RailPath *)l03_paths[taxi_vars->out[0]])->pts[i],
                        taxi_moby,
                        4,
                        0x800000FF,
                        0x7F,
                        2,
                        0xFF,
                        0.333f);
        }
        for (i = 0; i < ((struct RailPath *)l03_paths[taxi_vars->back[0]])->count; i++) {
            draw_number(&((struct RailPath *)l03_paths[taxi_vars->back[0]])->pts[i],
                        taxi_moby,
                        4,
                        0x8000FF00,
                        0x7F,
                        2,
                        0xFF,
                        0.333f);
        }
        for (i = 0; i < ((struct RailPath *)l03_paths[taxi_vars->onward[0]])->count; i++) {
            draw_number(&((struct RailPath *)l03_paths[taxi_vars->onward[0]])->pts[i],
                        taxi_moby,
                        4,
                        0x80FF0000,
                        0x7F,
                        2,
                        0xFF,
                        0.333f);
        }
    }
    qcopy(&oldPos, &taxi_moby->pos);
    qcopy(&oldRot, &taxi_moby->rot);
    switch (taxi_moby->state) {
    case TAXI_INITIALIZE:
        if (taxi_vars->out[0] != -1) {
            qcopy_nc(&taxi_moby->pos, &((struct RailPath *)l03_paths[taxi_vars->out[0]])->pts[0]);
            taxi_moby->rot.z = fast_arc_tan(
                ((struct RailPath *)l03_paths[taxi_vars->out[0]])->pts[1].f[0] - taxi_moby->pos.x,
                ((struct RailPath *)l03_paths[taxi_vars->out[0]])->pts[1].f[1] - taxi_moby->pos.y);
        } else if (taxi_vars->back[0] != -1) {
            qcopy_nc(&taxi_moby->pos, &((struct RailPath *)l03_paths[taxi_vars->back[0]])->pts[0]);
            taxi_moby->rot.z = fast_arc_tan(
                ((struct RailPath *)l03_paths[taxi_vars->back[0]])->pts[1].f[0] - taxi_moby->pos.x,
                ((struct RailPath *)l03_paths[taxi_vars->back[0]])->pts[1].f[1] - taxi_moby->pos.y);
        }
        taxi_moby->draw_dist = 0;
        taxi_moby->update_dist = 0xFF;
        taxi_vars->route = -1;
        taxi_moby->state = 1;
        taxi_moby->collision = 0;
        if (l03_save_state.cleared[(s16)taxi_moby->save_id] != 0 ||
            (D_0014C190_257218[current_level_index][(s16)taxi_moby->save_id >> 5] >>
             (taxi_moby->save_id & 0x1F)) &
                1) {
            mark_moby_for_removal(taxi_moby);
            return;
        }
        break;
    case TAXI_WAIT_FOR_ROUTE_TRIGGER:
        if (taxi_vars->active == 0) {
            break;
        }
        for (route_index = 0; route_index < 5; route_index++) {
            if ((taxi_vars->volume[route_index] != -1 || taxi_vars->route == route_index) &&
                (is_point_inside_clip_volume(&hero.motion.pos, taxi_vars->volume[route_index]) ||
                 taxi_vars->route == route_index)) {
                taxi_moby->collision = taxi_moby->pclass->collision;
                if (taxi_vars->out[route_index] != -1) {
                    route_path = ((struct RailPath *)l03_paths[taxi_vars->out[route_index]]);
                    qcopy(&taxi_moby->pos, &route_path->pts[0]);
                    taxi_moby->rot.z = fast_arc_tan(route_path->pts[1].f[0] - taxi_moby->pos.x,
                                                    route_path->pts[1].f[1] - taxi_moby->pos.y);
                } else if (taxi_vars->back[route_index] != -1) {
                    route_path = ((struct RailPath *)l03_paths[taxi_vars->back[route_index]]);
                    qcopy(&taxi_moby->pos, &route_path->pts[0]);
                    taxi_moby->rot.z = fast_arc_tan(route_path->pts[1].f[0] - taxi_moby->pos.x,
                                                    route_path->pts[1].f[1] - taxi_moby->pos.y);
                }
                taxi_moby->command = route_index;
                taxi_moby->state = 2;
                taxi_moby->draw_dist = 0x80;
                taxi_vars->idx = 1;
                taxi_vars->speed = rail_flyer_max_speed_gp * frame_time;
                taxi_vars->route = -1;
                break;
            }
        }
        break;
    case TAXI_FOLLOW_OUTBOUND_PATH:
        if (taxi_vars->out[taxi_moby->command] == -1 ||
            advance_moby_along_rail_path(
                taxi_moby, taxi_vars->out[taxi_moby->command], &taxi_vars->snd)) {
            taxi_moby->state = 3;
            taxi_vars->unkE4 = -1;
        }
        break;
    case TAXI_WAIT_FOR_RETURN_REQUEST:
        approach_angle(&taxi_moby->rot.y,
                       0.0f,
                       &taxi_vars->pitch_vel,
                       rail_flyer_pitch_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       rail_flyer_pitch_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       frame_time * 12.566371f);
        approach_angle(&taxi_moby->rot.x,
                       0.0f,
                       &taxi_vars->roll_vel,
                       rail_flyer_roll_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       rail_flyer_roll_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       frame_time * 12.566371f);
        if (hero.ground_moby == taxi_moby && hero.air_frames.s == 0) {
            request_taxi_return_help_message(taxi_moby, taxi_vars->help[taxi_moby->command]);
            if ((D_0013CAE4 & 0x10) && D_L03_0015F594 == 8) {
                request_hero_target_pose(&taxi_moby->pos, taxi_moby->rot.z, 0);
                taxi_moby->state = 4;
                taxi_vars->idx = 1;
            }
        }
        break;
    case TAXI_WAIT_FOR_RETURN_DEPARTURE:
        if (!get_hero_target_pose_status()) {
            taxi_moby->state = 5;
        }
        break;
    case TAXI_FOLLOW_RETURN_PATH:
        if (is_hero_on_moby(taxi_moby)) {
            hero.jump_lock_timer = 4;
            hero.unk1F4 = 4;
        }
        if (advance_moby_along_rail_path(
                taxi_moby, taxi_vars->back[taxi_moby->command], &taxi_vars->snd)) {
            taxi_moby->state = 6;
        }
        break;
    case TAXI_WAIT_FOR_HERO_TO_MOVE_AWAY:
        approach_angle(&taxi_moby->rot.y,
                       0.0f,
                       &taxi_vars->pitch_vel,
                       rail_flyer_pitch_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       rail_flyer_pitch_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       frame_time * 12.566371f);
        approach_angle(&taxi_moby->rot.x,
                       0.0f,
                       &taxi_vars->roll_vel,
                       rail_flyer_roll_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       rail_flyer_roll_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                       frame_time * 12.566371f);
        if (taxi_vars->onward[taxi_moby->command] != -1 &&
            fast_vec_dist(&taxi_moby->pos, &hero.motion.pos) > 15.0f) {
            taxi_moby->state = 7;
            taxi_vars->idx = 1;
        } else if (taxi_vars->onward[taxi_moby->command] == -1 &&
                   vector_distance_xy(&taxi_moby->pos, &hero.motion.pos) > 5.0f) {
            taxi_moby->state = 1;
        }
        break;
    case TAXI_FOLLOW_ONWARD_PATH:
        if (advance_moby_along_rail_path(
                taxi_moby, taxi_vars->onward[taxi_moby->command], &taxi_vars->snd)) {
            taxi_moby->draw_dist = 0;
            taxi_moby->state = 1;
            taxi_moby->collision = 0;
        }
        break;
    }
    update_taxi_bobbing_and_rocking(taxi_moby);
    fast_vec_sub(&move, &taxi_moby->pos, &oldPos);
    update_entity_matrix(taxi_vars->coll, &move, &oldRot, &taxi_moby->rot);
}

/* Pvars of a moby flying along a rail path. */
typedef struct {
    u8  pad0[0x60];
    f32 speed;     /* 0x60 */
    s32 idx;       /* 0x64: next path point */
    f32 yaw_vel;   /* 0x68 */
    f32 pitch_vel; /* 0x6C */
    f32 roll_vel;  /* 0x70 */
} RailFlyerVars;

extern f32  rail_flyer_speed_acceleration_gp __asm__("D_L03_00161B0C") __attribute__((sda));
extern f32  rail_flyer_yaw_acceleration_degrees_gp __asm__("D_L03_00161B14") __attribute__((sda));
extern f32  D_L03_00161B1C __attribute__((sda));
extern f32  advance_accelerated_scalar(f32 *, f32, f32, f32, f32, f32 *) __asm__("FUN_00213f38");
extern s32  sound_is_playing(void *, s32) __asm__("FUN_L00_0028d8c0");
extern s32  play_moby_sound(s32, s32, void *, s32) __asm__("FUN_L00_0028dc90");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");

/* Moves m along rail path `path`, easing speed toward the end and banking into turns;
   keeps the travel sound playing and releases it on arrival. Returns 1 at the last point. */
s32 advance_moby_along_rail_path(struct Moby *rail_moby, s32 path, s32 *snd) {
    RailFlyerVars    *rail_flyer_vars = (RailFlyerVars *)rail_moby->pvars;
    struct RailPath **pp = ((struct RailPath **)&l03_paths[path]);
    Vec4f             target_path_point, movement_delta;
    f32               acc;
    f32               target_pitch;
    f32               distance_to_path_end;
    s32               done;
    s32               sound_voice;

    done = 0;
    acc = 0.0f;
    qcopy(&target_path_point, &(*pp)->pts[rail_flyer_vars->idx]);
    fast_vec_sub(&movement_delta, &target_path_point, &rail_moby->pos);
    if (fast_vec_length(&movement_delta) < rail_flyer_max_speed_gp * frame_time * 2.0f) {
        rail_flyer_vars->idx++;
        done = rail_flyer_vars->idx == (*pp)->count;
    }
    distance_to_path_end = fast_vec_dist(&rail_moby->pos, &(*pp)->pts[(*pp)->count - 1]);
    advance_accelerated_scalar(&acc,
                               distance_to_path_end,
                               rail_flyer_speed_acceleration_gp * frame_time_sq_macro,
                               rail_flyer_speed_acceleration_gp * frame_time_sq_macro,
                               rail_flyer_max_speed_gp * frame_time,
                               &rail_flyer_vars->speed);
    fast_vec_normalize(&movement_delta, &movement_delta, rail_flyer_vars->speed);
    fast_vec_add(&rail_moby->pos, &movement_delta, &rail_moby->pos);
    target_pitch = -fast_arc_tan(vector_length_xy(&movement_delta), movement_delta.z);
    if (target_pitch > 0.2617994f)
        target_pitch = 0.2617994f;
    else if (target_pitch < -0.2617994f)
        target_pitch = -0.2617994f;
    approach_angle(&rail_moby->rot.y,
                   target_pitch,
                   &rail_flyer_vars->pitch_vel,
                   rail_flyer_pitch_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                   rail_flyer_pitch_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                   frame_time * 12.566371f);
    approach_angle(&rail_moby->rot.z,
                   fast_arc_tan(target_path_point.x - rail_moby->pos.x,
                                target_path_point.y - rail_moby->pos.y),
                   &rail_flyer_vars->yaw_vel,
                   rail_flyer_yaw_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                   rail_flyer_yaw_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                   frame_time * 12.566371f);
    approach_angle(&rail_moby->rot.x,
                   rail_flyer_vars->yaw_vel * rail_flyer_vars->speed * D_L03_00161B1C,
                   &rail_flyer_vars->roll_vel,
                   rail_flyer_roll_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                   rail_flyer_roll_acceleration_degrees_gp * DEG_TO_RAD * frame_time_sq_macro,
                   frame_time * 12.566371f);
    sound_voice = *snd;
    if (!done) {
        if (sound_is_playing(rail_moby, sound_voice) == 0)
            *snd = play_moby_sound(0, 4, rail_moby, 0x330);
    } else {
        if (sound_voice != -1) {
            VoicePoolWindow *voice_slot =
                (VoicePoolWindow *)((u8 *)&voice_pool + sound_voice * 0x70);
            if ((struct Moby *)voice_slot->voice.owner == rail_moby && voice_slot->voice.state)
                release_voice_slot(sound_voice);
        }
        *snd = -1;
    }
    return done;
}

/* Spinner: turns about z at a fixed rate (27.5 degrees times D_0015ED6C) and carries its collision record along. */
struct SpinnerVars {
    u8 pad0[0x20];
    u8 coll[0x10]; /* 0x20: collision record moved with the rotation */
};

void robo_shack_sign_update(struct Moby *m) __asm__("FUN_L03_002d3e58");

void robo_shack_sign_update(struct Moby *sign_moby) {
    Vec4                move;
    Vec4                oldRot;
    struct SpinnerVars *sign_vars;
    move.q = 0;
    oldRot.q = *(u128 *)&sign_moby->rot;
    sign_vars = (struct SpinnerVars *)sign_moby->pvars;
    sign_moby->draw_dist = 0xFF;
    sign_moby->update_dist = 0xFF;
    sign_moby->rot.z = fast_add_rotations(sign_moby->rot.z, frame_time * 0.47996554f);
    update_entity_matrix(sign_vars->coll, &move, &oldRot, &sign_moby->rot);
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_002CB280.c: func_L03_002D5650), where it is exact; names translated to the US level program. */

extern CollisionHit coll_hit __asm__("D_L03_00173F60");
extern int          collision_line(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void         update_entity(void *) __asm__("FUN_L00_00250df8");

char *
FUN_L03_002d4288(char *a, char *pos, struct Moby *parent, int n, float f0, float f1, float f2);
char *FUN_L03_002d4288(char        *heading_vector,
                       char        *pos,
                       struct Moby *parent,
                       int          timer_frames,
                       float        initial_heading,
                       float        initial_speed,

                       float f2) {
    char *spawned_moby = (char *)create_moby(0x341);
    char *moby_vars;
    if (spawned_moby != 0) {
        float collision_start_pos[4];
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0x7E;
        moby_vars = ((char *)MOBY(spawned_moby)->pvars);
        spawned_moby[0x31] = 1;
        spawned_moby[0x20] = 1;
        qcopy(spawned_moby + 0x10, pos);
        qcopy(moby_vars, heading_vector);
        MOBY(spawned_moby)->rot.z = fast_arc_tan(*(float *)moby_vars, *(float *)(moby_vars + 4));
        *(int *)(moby_vars + 0x14) = scale_game_frames(timer_frames);
        *(float *)(moby_vars + 0x18) = f2;
        *(float *)(moby_vars + 0x1C) = initial_heading;
        *(float *)(moby_vars + 0x24) = initial_speed;
        *(char **)(moby_vars + 0x10) = parent;
        *(int *)(moby_vars + 0x28) = 0;
        *(OvlQuad *)collision_start_pos = *(OvlQuad *)&parent->pos;
        collision_start_pos[2] = *(float *)(pos + 8);
        if (collision_line(collision_start_pos, pos, 2, parent, 0) != 0) {
            *(int *)(moby_vars + 0x14) = 0;
            *(OvlQuad *)&MOBY(spawned_moby)->pos = *(OvlQuad *)&coll_hit;
            *(int *)(moby_vars + 0x24) = 0;
        }
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

#include "qcopy.h"
#include "rnc/mobys/moby.h"

/* Pvars of a homing rocket. */
typedef struct {
    Vec4  vel;    /* 0x00 */
    void *ignore; /* 0x10: collision owner to skip */
    s32   timer;  /* 0x14: frames left */
    u8    pad18[4];
    f32   heading;  /* 0x1C */
    f32   turn_vel; /* 0x20 */
    f32   speed;    /* 0x24 */
} RocketVars;

/* Damage record passed to FUN_L00_00259a88. */
typedef struct {
    Vec4 dir; /* 0x00: w is the force */
    u8   pad10[8];
    u8   kind;   /* 0x18 */
    u8   source; /* 0x19 */
    s16  oclass; /* 0x1A */
    u8   pad1C[0x14];
} RocketHit;

extern void  add_vectors(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern int   FUN_L00_00257b90(int, int);
extern void  spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
extern int   coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void  make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern void  record_moby_hit(void *, void *) __asm__("FUN_L00_00259a88");
extern void  cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern float rocket_trail_offset_z_scale_gp __asm__("D_L03_00161B28") __attribute__((sda));
extern CollisionHit D_L03_00173F40;

/* Homing rocket update: flies on, trails smoke, steers up or down toward a target, explodes on impact or timeout. */
extern Vec4 D_0013F490_v __asm__("D_0013F490");
void        update_homing_rocket(struct Moby *rocket_moby) __asm__("FUN_L03_002d43c8");

void update_homing_rocket(struct Moby *rocket_moby) {
    enum {
        ROCKET_FLY = 1,
        ROCKET_BURST_AND_REMOVE = 2
    };
    Vec4         old;
    Vec4         side;
    Vec4         back;
    Vec4         trail_vector_offset;
    RocketTarget target;
    Vec4         trail;
    RocketHit    hit;
    RocketVars  *rocket_vars = (RocketVars *)rocket_moby->pvars;
    Vec4f       *pos;
    u8           state = rocket_moby->state;
    void       **level;
    float        pitch;
    float        range;

    switch (state) {
    case ROCKET_FLY:
        pos = &rocket_moby->pos;
        range = convert_integer_to_float(rocket_vars->timer * 2) * rocket_vars->speed;
        select_moby_target_with_hero_fallback(rocket_moby, &target, range);
        qcopy(&old, pos);
        fast_vec_add(pos, pos, rocket_vars);
        trail_vector_offset.q = D_0013F490_v.q;
        trail_vector_offset.f[2] *= rocket_trail_offset_z_scale_gp;
        add_vectors(&trail, pos, &trail_vector_offset);
        rocket_moby->rot.z = rocket_vars->heading;
        rocket_moby->rot.x = fast_add_rotations(rocket_moby->rot.x, frame_time * 6.2831855f);
        side.f[0] = fast_cos(rocket_vars->heading) * rocket_vars->speed * -0.5f;
        side.f[1] = fast_sin(rocket_vars->heading) * rocket_vars->speed * -0.5f;
        side.f[2] = 0.0f;
        fast_vec_add(&trail, &side, rocket_vars);
        side.q = trail.q;
        fast_vec_add(&trail, &side, &trail_vector_offset);
        side.q = trail.q;
        fast_vec_normalize(&back, rocket_vars, -0.2f);
        add_vectors(&trail, &back, pos);
        spawn_effect(&back,
                     &side,
                     0x6F00AFFF,
                     0xFF,
                     scale_game_frames(FUN_L00_00257b90(0xF, 0x16)),
                     0x28,
                     FUN_L00_00257b90(0x14, 0x23),
                     1);
        spawn_effect(&back,
                     &side,
                     0x1FFFFFFF,
                     0x4F4F4F,
                     scale_game_frames(FUN_L00_00257b90(0x1E, 0x3C)),
                     0x28,
                     FUN_L00_00257b90(0x32, 0x4B),
                     0);
        if (target.found != 0) {
            pitch = -fast_arc_tan(vector_distance_xy(pos, &target.pos),
                                  target.pos.f[2] - rocket_moby->pos.z);
            rocket_moby->rot.y = spring_turn(&rocket_vars->turn_vel,
                                             rocket_moby->rot.y,
                                             pitch,
                                             frame_time_sq_macro * 0.7853982f,
                                             frame_time_sq_macro * 3.1415927f,
                                             frame_time * 0.5235988f);
            rocket_moby->pos.z += fast_sin(-rocket_moby->rot.y) * rocket_vars->speed;
        }
        if (collision_line(pos, &old, 0, rocket_vars->ignore, 0) != 0 ||
            coll_sphere(0.2f, pos, 0, rocket_vars->ignore) != 0) {
            level = (void **)&D_L03_00173F40;
            if (level[6] != NULL) {
                make_hit_descriptor(&hit, rocket_moby, 0x10001, 1.0f, rocket_vars);
                hit.dir.f[0] = fast_cos(rocket_vars->heading);
                hit.dir.f[1] = fast_sin(rocket_vars->heading);
                hit.dir.f[2] = 1.0f;
                hit.dir.f[3] = 5627.925f;
                hit.kind = state;
                hit.source = state;
                hit.oclass = rocket_moby->oclass;
                record_moby_hit(level[6], &hit);
            }
            rocket_moby->state = 2;
        }
        if (fast_dec_timer(&rocket_vars->timer) != 0) {
            rocket_moby->state = 2;
        }
        break;
    case ROCKET_BURST_AND_REMOVE:
        cart_burst(rocket_moby, &rocket_moby->pos, 0, 0.25f, 13.0f);
        mark_moby_for_removal(rocket_moby);
        break;
    }
}

typedef struct {
    char  pad0[0x30];
    int   mode;
    int   t;
    char  pad38[0xC];
    short n;
    char  pad46[0x132];
    char *list[1];
} G_2da710;

extern int D_L03_0015F5C4;

#include "rnc/game/options.h"

extern G_2da710 D_L03_0016C960;
extern char     D_L03_0017C540[];
extern float    helga_big_head_attachment_uniform_value_gp __asm__("D_L03_00161BB4")
    __attribute__((sda));
extern void attach_manipulator(void *, int, void *) __asm__("FUN_0020cb10");

/* During the cutscene, nudges a camera value at two time windows and attaches
 * the shared manipulator to every listed moby of the same class as m. */
void apply_helga_big_head_cutscene_adjustments(struct Moby *class_reference_moby) __asm__("FUN_L03_002da710");

void apply_helga_big_head_cutscene_adjustments(struct Moby *class_reference_moby) {
    char *shared_attachment_record;
    int   i;

    if (D_L03_0015F5C4 == 2 && big_head_cheat_enabled != 0) {
        if (D_L03_0016C960.mode == 2 && D_L03_0016C960.t >= 0x1EA && D_L03_0016C960.t < 0x266) {
            helga_big_head_cutscene_adjustment = -0.2f;
        }
        if (D_L03_0016C960.mode == 2 && D_L03_0016C960.t >= 0x2EE && D_L03_0016C960.t < 0x321) {
            helga_big_head_cutscene_adjustment = 0.2f;
        }
        for (i = 0; i < D_L03_0016C960.n; i++) {
            if (*(short *)(D_L03_0016C960.list[i] + 0xA6) == class_reference_moby->oclass) {
                shared_attachment_record = D_L03_0017C540;
                if (((unsigned char *)shared_attachment_record)[1] == 0) {
                    attach_manipulator(D_L03_0016C960.list[i], 0, shared_attachment_record);
                    *(float *)(shared_attachment_record + 0x20) =
                        helga_big_head_attachment_uniform_value_gp;
                    *(float *)(shared_attachment_record + 0x24) =
                        helga_big_head_attachment_uniform_value_gp;
                    *(float *)(shared_attachment_record + 0x28) =
                        helga_big_head_attachment_uniform_value_gp;
                }
            }
        }
    }
}

#include "rnc/globals.h"

/* Pvars of the l03 dialogue mobys (the record initialize_npc_dialogue / update_npc_dialogue run). */
struct TalkState {
    u8    pad0[4];
    s16   mode; /* 0x04: 1 talked, 2 talked and accepted */
    u8    pad6[0x1A];
    void *script; /* 0x20 */
    u8    pad24[0x12];
    s16   unk36;
    u8    pad38[0x10];
    s32   unk48;
    s32   warp; /* 0x4C: index into l03_cuboids, -1 for none */
};

/* Pvars of the Kerwan quest giver helga_update drives. */
struct QuestGiverVars {
    u8               pad0[0x80];
    struct TalkState talk; /* 0x80 */
    s32 reward; /* 0xD0: moby index removed (and recorded) when the deal is accepted, -1 for none */
    s32 link;   /* 0xD4: moby index of the class 0x3F4 helper it drives, -1 for none */
    s32 link_arg; /* 0xD8: stored into the helper when it is released */
    u8  padDC[4];
    u8  body[0x64]; /* 0xE0: body look-at driver (update_joint_animation) */
    f32 pitch;      /* 0x144 */
    f32 yaw;        /* 0x148 */
    u8  pad14C[4];
    f32 height; /* 0x150 */
    u8  pad154[0xC];
    u8  head[0x80];    /* 0x160: head look-at driver */
    Vec4 target;       /* 0x1E0: point it looks at while idle */
    s32  watch_timer;  /* 0x1F0: frames left watching the hero */
    s32  target_timer; /* 0x1F4: frames until a new idle target */
};

extern u8           npc_dialogue_suppression_flags[] __asm__("D_0013D4CC");
extern char        *D_001413D0[];
extern struct Moby *D_L03_0015FFD8;
extern s32          D_L03_0015FFD8_i __asm__("D_L03_0015FFD8");
extern s32          helga_helper_flag_context_records[] __asm__("D_L03_001BA720");
extern char         helga_dialogue_script[] __asm__("D_L03_00161BB8");

#include "rnc/game/game_globals.h"

extern u8             mission_collection_recording_filter[] __asm__("D_L03_0015FC88");
extern LevelSaveState l03_moby_collection_state __asm__("D_L03_001BA6D0");

#include "rnc/overlay/cuboid.h"

extern void  initialize_npc_dialogue(struct Moby *, struct TalkState *) __asm__("FUN_L00_002668a0");
extern int   update_npc_dialogue(struct Moby *, struct TalkState *) __asm__("FUN_L00_00266448");
extern void  begin_dialogue_camera_transition(struct Moby *, f32) __asm__("FUN_L01_002783a8");
extern void  jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void  jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void  grant_inventory_item(int, int) __asm__("FUN_L00_00260860");
extern void  queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern void  memcard_save_data(int, int) __asm__("FUN_0020b178");
extern float scale_time(float) __asm__("FUN_001f96b0");
extern void  build_spherical_offset(void *, float, float, float) __asm__("FUN_00214db0");
extern void  update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");

/* Quest giver: talks when approached (driving its class 0x3F4 helper), warps the
   hero or, once the deal is accepted, removes the reward moby, records it and saves.
   While idle it turns its head toward the hero or toward random points. */
void helga_update(struct Moby *m) __asm__("FUN_L03_002da870");

void helga_update(struct Moby *helga_moby) {
    enum {
        HELGA_INITIALIZE = 0,
        HELGA_UPDATE_DIALOGUE = 1,
        HELGA_RESOLVE_DIALOGUE = 2
    };
    struct QuestGiverVars *helga_vars = (struct QuestGiverVars *)helga_moby->pvars;
    struct TalkState      *talk = &helga_vars->talk;
    struct Moby           *helper_moby;
    struct Moby           *reward_moby;
    u8                     group;
    s32                   *flag;
    Vec4                  *hero_position;
    struct Hero           *hero_state;
    float                  scratch_target[4];
    float                  eye[4];
    float                  delta[4];
    float                  rate;
    float                  head_rate;
    float                  yaw;
    float                  pitch;
    int                    tracking;

    apply_helga_big_head_cutscene_adjustments(helga_moby);
    switch (helga_moby->state) {
    case HELGA_INITIALIZE:
        if (npc_dialogue_suppression_flags[0] != 0) {
            if (helga_vars->link != -1) {
                helper_moby = (struct Moby *)((helga_vars->link << 8) + D_L03_0015FFD8_i);
                if (helper_moby->oclass == 0x3F4) {
                    flag = (s32 *)(helper_moby->pvars + 0x8C);
                    *flag = 1;
                    record_moby_context_bytes(
                        flag, 4, helga_moby, 2, helga_helper_flag_context_records);
                }
            }
            mark_moby_for_removal(helga_moby);
            return;
        }
        if (visited52[helga_moby->mission + current_level_index * 16] == 0xFF &&
            helga_vars->link != -1) {
            helper_moby = (struct Moby *)((helga_vars->link << 8) + D_L03_0015FFD8_i);
            if (helper_moby->oclass == 0x3F4) {
                flag = (s32 *)(helper_moby->pvars + 0x8C);
                *flag = 1;
                record_moby_context_bytes(
                    flag, 4, helga_moby, 2, helga_helper_flag_context_records);
            }
        }
        helga_moby->spawn_frame = *(u64 *)(D_001413D0[0] + 0x38);
        helga_moby->state = 1;
        talk->script = helga_dialogue_script;
        helga_vars->talk.unk48 = 5;
        initialize_npc_dialogue(helga_moby, talk);
        initialize_moby_grounding(helga_moby);
        break;
    case HELGA_UPDATE_DIALOGUE:
        if (update_npc_dialogue(helga_moby, talk)) {
            begin_dialogue_camera_transition(helga_moby, 2.3f);
            helga_moby->state = 2;
            if (helga_vars->link != -1) {
                helper_moby = (struct Moby *)((helga_vars->link << 8) + D_L03_0015FFD8_i);
                if (helper_moby->oclass == 0x3F4) {
                    flag = (s32 *)(helper_moby->pvars + 0x8C);
                    *flag = 1;
                    record_moby_context_bytes(
                        flag, 4, helga_moby, 2, helga_helper_flag_context_records);
                }
            }
        }
        break;
    case HELGA_RESOLVE_DIALOGUE:
        if (D_L03_0015F5C4 != 2) {
            if (helga_vars->talk.mode != 2) {
                jet_clear_persistent(helga_moby->mission);
                if (helga_vars->link != -1) {
                    helper_moby = (struct Moby *)((helga_vars->link << 8) + D_L03_0015FFD8_i);
                    if (helper_moby->oclass == 0x3F4) {
                        flag = (s32 *)(helper_moby->pvars + 0x8C);
                        *flag = 1;
                        record_moby_context_bytes(
                            flag, 4, helga_moby, 2, helga_helper_flag_context_records);
                    }
                }
                if (helga_vars->talk.warp != -1) {
                    jet_reset_camera(&l03_cuboids[helga_vars->talk.warp].pos,
                                     &l03_cuboids[helga_vars->talk.warp].rot);
                }
            }
            helga_moby->state = 1;
            if (helga_vars->talk.mode == 2) {
                grant_inventory_item(0xC, 1);
                queue_dialogue_message(0xBC8, -1);
                if (helga_vars->reward != -1) {
                    l03_moby_collection_state
                        .cleared[(s16)((struct Moby *)((helga_vars->reward << 8) +
                                                       D_L03_0015FFD8_i))
                                     ->save_id] =
                        ((struct Moby *)((helga_vars->reward << 8) + D_L03_0015FFD8_i))->mission +
                        2;
                    reward_moby = (struct Moby *)((helga_vars->reward << 8) + D_L03_0015FFD8_i);
                    /* Fakematch: same record as the || test in the level's other reward
                       code; the do/while (0) blocks keep the loop notes that make gcc
                       predict the 0xFF branch taken and keep the group load ahead of the
                       table load. */
                    if (reward_moby->mission == 0xFF)
                        goto record;
                    do {
                        group = reward_moby->mission;
                    } while (0);
                    if (mission_collection_recording_filter[group] != 0xFF &&
                        visited52[group + current_level_index * 16] == 0xFF) {
                        /* Fakematch: see above. */
                        do {
                        record:
                            l03_save_state.cleared[(s16)reward_moby->save_id] =
                                reward_moby->mission + 2;
                        } while (0);
                    }
                    mark_moby_for_removal(&D_L03_0015FFD8[helga_vars->reward]);
                }
                if (helga_vars->link != -1) {
                    helper_moby = (struct Moby *)((helga_vars->link << 8) + D_L03_0015FFD8_i);
                    if (helper_moby->oclass == 0x3F4) {
                        u8 *tv = helper_moby->pvars;
                        *(s32 *)(tv + 0x7C) = 0;
                        *(s32 *)(tv + 0x8C) = 0;
                        *(s32 *)(tv + 0xA0) = helga_vars->link_arg;
                    }
                }
                memcard_save_data(0, -1);
                mark_moby_for_removal(helga_moby);
                return;
            }
        } else if (helga_vars->talk.unk36 == 2 && helga_vars->link != -1) {
            helper_moby = (struct Moby *)((helga_vars->link << 8) + D_L03_0015FFD8_i);
            if (helper_moby->oclass == 0x3F4) {
                u8 *tv = helper_moby->pvars;
                *(s32 *)(tv + 0x7C) = 0;
                *(s32 *)(tv + 0x8C) = 0;
                *(s32 *)(tv + 0xA0) = helga_vars->link_arg;
            }
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (helga_moby->prev_seq == 0) {
        hero_position = &hero.motion.pos;
        tracking = 1;
        if (vector_distance_xy(&helga_moby->pos, hero_position) < 8.0f &&
            (hero_state = (struct Hero *)((u8 *)hero_position - 0x80),
             fast_difference_between_rotations(
                 helga_moby->rot.z,
                 fast_arc_tan(hero_state->motion.body_center.f[0] - helga_moby->pos.x,
                              hero_state->motion.body_center.f[1] - helga_moby->pos.y))) <
                1.5707964f) {
            if (fast_vec_length((u8 *)hero_position + 0x80) > 0.01f) {
                helga_vars->watch_timer = scale_game_frames(120);
            } else {
                fast_dec_timer(&helga_vars->watch_timer);
            }
        } else if (helga_vars->watch_timer) {
            helga_vars->watch_timer = 0;
            qcopy(&helga_vars->target, &hero.motion.body_center);
        }
        if (fast_dec_timer(&helga_vars->target_timer)) {
            float heading;
            helga_vars->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading = fast_add_rotations(helga_moby->rot.z,
                                         random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(
                &helga_vars->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(&helga_vars->target, &helga_vars->target, &helga_moby->pos);
        }
        if (helga_vars->watch_timer) {
            qcopy(scratch_target, &hero.motion.body_center);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch_target, &helga_vars->target);
        }
    }
    if (tracking) {
        qcopy(eye, &helga_moby->pos);
        eye[2] += 1.0f;
        fast_vec_sub(delta, scratch_target, eye);
        yaw = fast_subtract_rotations(fast_arc_tan(delta[0], delta[1]), helga_moby->rot.z);
        pitch = -fast_arc_tan(vector_length_xy(delta), delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        helga_vars->pitch = pitch;
        helga_vars->yaw = yaw * 0.5f;
    }
    if (big_head_cheat_enabled)
        helga_vars->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, helga_moby, helga_vars->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, helga_moby, helga_vars->head, 1);
}

/* Moves the moby along its path: advances the distance, places it on the
 * current segment, and dies at the end of the path. */
void update_path_mover(struct Moby *path_moby) __asm__("FUN_L03_002db020");

void update_path_mover(struct Moby *path_moby) {
    float old[4];
    float movement_scratch[4];
    float segment_offset[4];
    char *path_moby_vars;
    char *path;
    int   i;
    float segment_fraction;

    *(OvlQuad *)old = *(OvlQuad *)&path_moby->pos;
    path_moby_vars = (char *)path_moby->pvars;
    *(float *)(path_moby_vars + 0x64) += path_mover_distance_step;
    movement_scratch[0] = fast_cos(path_moby->rot.z) * *(float *)(path_moby_vars + 0x64);
    movement_scratch[1] = fast_sin(path_moby->rot.z) * *(float *)(path_moby_vars + 0x64);
    *(int *)&movement_scratch[2] = 0;
    fast_vec_add(movement_scratch, movement_scratch, *(char **)(path_moby_vars + 0x60) + 0x10);
    i = truncate_float_to_s32(*(float *)(path_moby_vars + 0x64) /
                              *(float *)(*(char **)(path_moby_vars + 0x60) + 0x1C));
    segment_fraction = convert_integer_to_float(i);
    path = *(char **)(path_moby_vars + 0x60);
    segment_fraction =
        (*(float *)(path_moby_vars + 0x64) - segment_fraction * *(float *)(path + 0x1C)) /
        *(float *)(path + 0x1C);
    if (i >= *(int *)path - 2) {
        mark_moby_for_removal(path_moby);
        return;
    }
    fast_vec_sub(segment_offset, ((float(*)[4])path)[i + 2], ((float(*)[4])path)[i + 1]);
    fast_vec_scale(segment_offset, segment_offset, segment_fraction);
    fast_vec_add(&path_moby->pos, segment_offset, (*(float(**)[4])(path_moby_vars + 0x60))[i + 1]);
    fast_vec_sub(movement_scratch, &path_moby->pos, old);
    update_entity_matrix(path_moby_vars + 0x20, movement_scratch, &path_moby->rot, &path_moby->rot);
}

extern void copy_hero_moby_field_38(struct Moby *) __asm__("FUN_L00_0025d1b8");

/* Spawns a path mover (class 0x382, moved by FUN_L03_002db020) on path, dist
 * units along it, facing the path's last point. */
struct Moby *spawn_path_mover(struct RailPath *path,
                              float            initial_path_distance) __asm__("FUN_L03_002db198");

struct Moby *spawn_path_mover(struct RailPath *path, float initial_path_distance) {
    struct Moby *spawned_moby = create_moby(0x382);
    u8          *moby_vars;
    u8          *move;
    Vec4        *end;

    if (spawned_moby != 0) {
        spawned_moby->update_dist = 0x80;
        spawned_moby->draw_dist = 0x40;
        spawned_moby->visible = 1;
        spawned_moby->state = 0;
        moby_vars = spawned_moby->pvars;
        spawned_moby->flags |= 0x20;
        copy_hero_moby_field_38(spawned_moby);
        qcopy(&spawned_moby->pos, &path->pts[0]);
        clear_u64_value(&spawned_moby->rot);
        end = (Vec4 *)((u8 *)path + (path->count - 1) * 16) + 1;
        spawned_moby->rot.z =
            fast_arc_tan(end->f[0] - spawned_moby->pos.x, end->f[1] - spawned_moby->pos.y);
        move = moby_vars + 0x20;
        *(float *)(moby_vars + 0x64) = initial_path_distance;
        *(struct RailPath **)(moby_vars + 0x60) = path;
        *(u8 **)(moby_vars + 0x8) = move;
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

struct RailSpawnerVars {
    s32 path; /* index into D_L03_001B05B0, -1 for none */
    s32 timer;
    s32 voice;
};

struct RailSpawnerMoby {
    u8                      pad0[0x20];
    u8                      state;
    u8                      pad21[0xF];
    u8                      unk30;
    u8                      pad31[0x47];
    struct RailSpawnerVars *vars; /* 0x78 */
    u8                      pad7C[0x2C];
    u16                     uid; /* 0xA8 */
};

extern char         path_mover_spawner_bad_path_format[] __asm__("D_L03_001E35F0");
extern void         debug_print(char *, int) __asm__("FUN_001e93b0");
extern struct Moby *spawn_path_mover(struct RailPath *, float) __asm__("FUN_L03_002db198");
extern int          voice_is_playing(struct RailSpawnerMoby *, int) __asm__("FUN_L00_0028d8c0");
extern int          FUN_L00_0028dc90(int, int, struct RailSpawnerMoby *, int);

void update_path_mover_spawner(struct RailSpawnerMoby *moby) __asm__("FUN_L03_002db280");

void update_path_mover_spawner(struct RailSpawnerMoby *moby) {
    struct RailSpawnerVars *vars = moby->vars;
    struct RailPath        *path;
    float                   spawn_distance;
    int                     i;

    if (moby->state == 0) {
        if (vars->path == -1) {
            debug_print(path_mover_spawner_bad_path_format, moby->uid);
            mark_moby_for_removal((char *)moby);
            return;
        }
        path = ((struct RailPath *)l03_paths[vars->path]);
        path_mover_distance_step = frame_time * 3.0f;
        for (i = 0; i < path->count - 2; i++)
            path->pts[i].f[3] = fast_vec_dist(&path->pts[i], &path->pts[i + 1]);
        vars->timer = 0;
        moby->state = 1;
        vars->voice = -1;
        for (spawn_distance = 2.9f; spawn_distance < path->count * path->pts[0].f[3];
             spawn_distance += 2.9f)
            spawn_path_mover(path, spawn_distance);
        moby->unk30 = 0x60;
    }
    if (fast_dec_timer(&vars->timer)) {
        vars->timer = truncate_float_to_s32(2.9f / path_mover_distance_step);
        spawn_path_mover(((struct RailPath *)l03_paths[vars->path]), 0.0f);
    }
    if (!voice_is_playing(moby, vars->voice))
        vars->voice = FUN_L00_0028dc90(0, 4, moby, 0x382);
}

/* Dialogue record run by initialize_npc_dialogue / update_npc_dialogue (only the fields read here). */
struct TalkRecord {
    u8  pad0[4];
    s16 mode; /* 0x04: 2 when the talk was accepted */
    u8  pad6[0x30];
    s16 unk36;
    u8  pad38[8];
};

/* Look-at driver run by update_joint_animation. */
struct LookAt {
    u8  pad0[0x64];
    f32 pitch; /* 0x64 */
    f32 yaw;   /* 0x68 */
    u8  pad6C[4];
    f32 height; /* 0x70 */
    u8  pad74[0xC];
};

/* Pvars of the Kerwan guide big_al_update drives. */
struct GuideVars {
    struct TalkRecord talk;
    struct LookAt     head;   /* 0x40 */
    struct LookAt     body;   /* 0xC0 */
    s32               turned; /* 0x140: set once it has turned around to face the hero */
    s32  reward; /* 0x144: moby index of the class 0x3E5 reward it releases, -1 for none */
    s32  link;   /* 0x148: moby index of its class 0x3F4 helper, -1 for none */
    s32  warp;   /* 0x14C: index into l03_cuboids, -1 for none */
    Vec4 target; /* 0x150: point it looks at while idle */
    u8   pad160[4];
    s32  watch_timer;  /* 0x164 */
    s32  target_timer; /* 0x168 */
};

extern u8    D_0013D4C2[];
extern char *D_001413D0_db558[] __asm__("D_001413D0");

#include "rnc/game/game_globals.h"

extern void
attach_shared_manipulator_to_matching_class_moby(struct Moby *) __asm__("FUN_L03_002db480");
extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern int  guide_save(int, int) __asm__("FUN_0020b178");

/* Kerwan guide: talks when approached (turning round to face the hero first),
   releases its reward moby once the item is owned, can warp the hero, and turns
   its head toward the hero or random points while idle. */
void big_al_update(struct Moby *m) __asm__("FUN_L03_002db558");

void big_al_update(struct Moby *guide_moby) {
    enum {
        BIG_AL_INITIALIZE = 0,
        BIG_AL_UPDATE_DIALOGUE = 1,
        BIG_AL_RESOLVE_DIALOGUE = 2
    };
    struct GuideVars *v = (struct GuideVars *)guide_moby->pvars;
    struct Moby      *referenced_moby;
    struct Moby      *reward_moby;
    Vec4             *hero_position;
    struct Hero      *hero_state;
    float             scratch_target[4];
    float             eye[4];
    float             delta[4];
    float             rate;
    float             head_rate;
    float             yaw;
    float             pitch;
    int               tracking;
    u8                group;

    attach_shared_manipulator_to_matching_class_moby(guide_moby);
    switch (guide_moby->state) {
    case BIG_AL_INITIALIZE:
        if (level_object_states[20].state == 0) {
            guide_moby->rot.z = fast_add_rotations(guide_moby->rot.z, 3.1415927f);
            v->turned = 0;
        } else {
            v->turned = 1;
        }
        if (visited52[guide_moby->mission + current_level_index * 16] == 0xFF && v->link != -1) {
            referenced_moby = (struct Moby *)((v->link << 8) + D_L03_0015FFD8_i);
            if (referenced_moby->oclass == 0x3F4) {
                *(s32 *)(referenced_moby->pvars + 0x8C) = 1;
            }
        }
        guide_moby->spawn_frame = *(u64 *)(D_001413D0_db558[0] + 0x38);
        guide_moby->state = 1;
        initialize_npc_dialogue(guide_moby, (struct TalkState *)&v->talk);
        initialize_moby_grounding(guide_moby);
        if (D_0013D4C2[0] != 0) {
            v->talk.unk36 = -1;
        }
        break;
    case BIG_AL_UPDATE_DIALOGUE:
        if (item_available[2] != 0 && v->reward != -1) {
            referenced_moby = (struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i);
            if (referenced_moby->oclass == 0x3E5 && referenced_moby->state == 0) {
                referenced_moby->state = 1;
                l03_moby_collection_state
                    .cleared[(s16)((struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i))->save_id] =
                    ((struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i))->mission + 2;
                reward_moby = (struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i);
                {
                    s32 k = reward_moby->mission;
                    u32 u = k & 0xFF;
                    if (k == 0xFF || (mission_collection_recording_filter[u] != 0xFF &&
                                      visited52[u + current_level_index * 16] == 0xFF)) {
                        l03_save_state.cleared[(s16)reward_moby->save_id] = k + 2;
                    }
                }
            }
        }
        if (item_available[2] != 0 &&
            visited52[guide_moby->mission + current_level_index * 16] == 0xFF) {
            break;
        }
        if (update_npc_dialogue(guide_moby, (struct TalkState *)&v->talk)) {
            if (v->turned == 0) {
                guide_moby->rot.z = fast_add_rotations(guide_moby->rot.z, 3.1415927f);
                v->turned = 1;
            }
            begin_dialogue_camera_transition(guide_moby, 2.7f);
            guide_moby->anim_speed = 0.0f;
            guide_moby->state = 2;
        }
        break;
    case BIG_AL_RESOLVE_DIALOGUE:
        if (D_L03_0015F5C4 == 2) {
            break;
        }
        guide_moby->anim_speed = 1.0f;
        if (v->talk.mode != 2) {
            jet_clear_persistent(guide_moby->mission);
            if (v->link != -1) {
                referenced_moby = (struct Moby *)((v->link << 8) + D_L03_0015FFD8_i);
                if (referenced_moby->oclass == 0x3F4) {
                    *(s32 *)(referenced_moby->pvars + 0x8C) = 1;
                }
            }
            if (v->warp != -1) {
                jet_reset_camera(&l03_cuboids[v->warp].pos, &l03_cuboids[v->warp].rot);
            }
        }
        guide_moby->state = 1;
        if (v->talk.mode != 2) {
            break;
        }
        grant_inventory_item(2, 1);
        queue_dialogue_message(0xBC7, -1);
        if (v->reward != -1) {
            referenced_moby = (struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i);
            if (referenced_moby->oclass == 0x3E5 && referenced_moby->state == 0) {
                referenced_moby->state = 1;
                l03_moby_collection_state
                    .cleared[(s16)((struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i))->save_id] =
                    ((struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i))->mission + 2;
                reward_moby = (struct Moby *)((v->reward << 8) + D_L03_0015FFD8_i);
                /* Fakematch: same record test as above; the goto and the do/while (0)
                   loop note keep the group byte's zero-extend ahead of the table address. */
                if (reward_moby->mission == 0xFF)
                    goto rec2;
                do {
                    group = reward_moby->mission;
                } while (0);
                if (mission_collection_recording_filter[group] != 0xFF &&
                    visited52[group + current_level_index * 16] == 0xFF) {
                rec2:
                    l03_save_state.cleared[(s16)reward_moby->save_id] = reward_moby->mission + 2;
                }
            }
        }
        if (v->warp != -1) {
            jet_reset_camera(&l03_cuboids[v->warp].pos, &l03_cuboids[v->warp].rot);
        }
        guide_save(0, -1);
        set_race_camera(&l03_cuboids[v->warp].pos, &l03_cuboids[v->warp].rot, 0, 1);
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (guide_moby->prev_seq == 0) {
        hero_position = &hero.motion.pos;
        tracking = 1;
        if (vector_distance_xy(&guide_moby->pos, hero_position) < 8.0f &&
            (hero_state = (struct Hero *)((u8 *)hero_position - 0x80),
             fast_difference_between_rotations(
                 guide_moby->rot.z,
                 fast_arc_tan(hero_state->motion.body_center.f[0] - guide_moby->pos.x,
                              hero_state->motion.body_center.f[1] - guide_moby->pos.y))) <
                1.5707964f) {
            if (fast_vec_length((u8 *)hero_position + 0x80) > 0.01f) {
                v->watch_timer = scale_game_frames(120);
            } else {
                fast_dec_timer(&v->watch_timer);
            }
        } else if (v->watch_timer) {
            v->watch_timer = 0;
            qcopy(&v->target, &hero.motion.body_center);
        }
        if (fast_dec_timer(&v->target_timer)) {
            float heading;
            v->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading = fast_add_rotations(guide_moby->rot.z,
                                         random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            build_spherical_offset(
                &v->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(&v->target, &v->target, &guide_moby->pos);
        }
        if (v->watch_timer) {
            qcopy(scratch_target, &hero.motion.body_center);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch_target, &v->target);
        }
    }
    if (tracking) {
        qcopy(eye, &guide_moby->pos);
        eye[2] += 1.0f;
        fast_vec_sub(delta, scratch_target, eye);
        yaw = fast_subtract_rotations(fast_arc_tan(delta[0], delta[1]), guide_moby->rot.z);
        pitch = -fast_arc_tan(vector_length_xy(delta), delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        v->head.pitch = pitch;
        v->body.yaw = v->head.yaw = yaw * 0.5f;
    }
    if (big_head_cheat_enabled)
        v->head.height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, guide_moby, &v->head, 1);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, guide_moby, &v->body, 0);
}

/* Pvars of the dialogue moby robot_qwark_update drives (initialize_npc_dialogue / update_npc_dialogue state). */
struct TalkerVars {
    u8  pad0[4];
    s16 mode; /* 0x04: 2 when the talk ends in a warp */
    u8  pad6[0x30];
    s16 unk36;
    u8  pad38[0x14];
    s32 warp; /* 0x4C: index into l03_cuboids, -1 for none */
};

typedef struct {
    u8           pad0[0x20];
    struct Moby *source; /* 0x20 */
    u32          pad24;
    u8           type; /* 0x28 */
} TalkerHit;

extern s32        D_L03_001BA5D0[];
extern void       set_moby_animation(struct Moby *, int, int) __asm__("FUN_00212ed8");
extern TalkerHit *talker_find_hit(struct Moby *, s32, s32) __asm__("FUN_L00_0025a420");
extern void       allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");
extern void       talker_spawn_debris(struct Moby *,
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
                                      s32,
                                      f32,
                                      f32,
                                      s32,
                                      s32,
                                      s32,
                                      s32) __asm__("FUN_L00_0025e450");
extern int        talker_init(struct Moby *, struct TalkerVars *) __asm__("FUN_L00_002668a0");
extern int        talker_update(struct Moby *, struct TalkerVars *) __asm__("FUN_L00_00266448");
extern void spawn_effect_particle_with_target_scalar(void *, void *, u32, u32, s32, f32) __asm__("FUN_L00_0026ced0");

/* A talking moby that can be shot down: talks (and may warp the hero) when
 * approached, and once hit by a type 3 hit or already recorded as collected
 * it breaks, records its save bit, and keeps sparking. */
void robot_qwark_update(struct Moby *m) __asm__("FUN_L03_002dbd90");

void robot_qwark_update(struct Moby *robot_qwark_moby) {
    enum {
        ROBOT_QWARK_INITIALIZE = 0,
        ROBOT_QWARK_UPDATE_DIALOGUE = 1,
        ROBOT_QWARK_RESOLVE_DIALOGUE = 2,
        ROBOT_QWARK_BROKEN_SPARKING = 3
    };
    struct TalkerVars *dialogue_state = (struct TalkerVars *)robot_qwark_moby->pvars;
    TalkerHit         *hit;
    u8                 type;
    char              *warp_record;
    OvlVec4            particle_velocity;
    OvlVec4            pos;
    float              unused_vector_scratch[4];

    if (robot_qwark_moby->visible != 0 &&
        fast_vec_dist(&robot_qwark_moby->pos, D_L03_00166EC0) < 26.0f) {
        FUN_L00_0025a120(robot_qwark_moby);
        robot_qwark_moby->shadow_range = 0x15;
    }
    if (l03_save_state.cleared[(s16)robot_qwark_moby->save_id] != 0 ||
        (D_0014C190_257218[current_level_index][(s16)robot_qwark_moby->save_id >> 5] >>
         (robot_qwark_moby->save_id & 0x1F)) &
            1) {
        if (robot_qwark_moby->state != 3) {
            robot_qwark_moby->state = 3;
            set_moby_animation(robot_qwark_moby, 1, 0);
        }
    }
    hit = talker_find_hit(robot_qwark_moby, -1, 0);
    if (hit != 0 && hit->source != 0 && (type = hit->type) == 3 &&
        l03_save_state.cleared[(s16)robot_qwark_moby->save_id] == 0 &&
        (((D_0014C190_257218[current_level_index][(s16)robot_qwark_moby->save_id >> 5] >>
           (robot_qwark_moby->save_id & 0x1F)) ^
          1) &
         1)) {
        if (item_unlocked[5] == 0) {
            item_unlocked[5] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53D6, -1);
        }
        pos.q = *(OvlQuad *)&robot_qwark_moby->pos;
        pos.f[2] += 1.0f;
        particle_velocity.q = 0;
        talker_spawn_debris(robot_qwark_moby,
                            &particle_velocity,
                            &pos,
                            0.0f,
                            0.0f,
                            10,
                            3,
                            16,
                            4.0f,
                            2.0f,
                            9.0f,
                            0,
                            1.0f,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
        robot_qwark_moby->state = type;
        set_moby_animation(robot_qwark_moby, 1, 0);
        D_0014C190_257218[current_level_index][(s16)robot_qwark_moby->save_id >> 5] |=
            1 << (robot_qwark_moby->save_id & 0x1F);
        D_L03_001BA5D0[(s16)robot_qwark_moby->save_id >> 5] |=
            1 << (robot_qwark_moby->save_id & 0x1F);
    }
    robot_qwark_moby->hit_slot = 0xFF;
    if (npc_dialogue_suppression_flags[0] != 0 && robot_qwark_moby->state != 3) {
        return;
    }
    switch (robot_qwark_moby->state) {
    case ROBOT_QWARK_INITIALIZE:
        robot_qwark_moby->spawn_frame = *(u64 *)(D_001413D0[0] + 0x38);
        robot_qwark_moby->state = 1;
        talker_init(robot_qwark_moby, dialogue_state);
        if (alternate_item_available[0x19] != 0) {
            dialogue_state->mode = 1;
            dialogue_state->unk36 = 2;
        }
        break;
    case ROBOT_QWARK_UPDATE_DIALOGUE:
        if (talker_update(robot_qwark_moby, dialogue_state)) {
            alternate_item_available[0x19] = 1;
            begin_dialogue_camera_transition(robot_qwark_moby, 2.7f);
            robot_qwark_moby->state = 2;
        }
        break;
    case ROBOT_QWARK_RESOLVE_DIALOGUE:
        if (D_L03_0015F5C4 == 2) {
            break;
        }
        robot_qwark_moby->state = 1;
        if (dialogue_state->mode == 2 && dialogue_state->warp != -1) {
            warp_record = (char *)l03_cuboids + dialogue_state->warp * 128;
            jet_reset_camera(warp_record + 0x30, warp_record + 0x70);
        }
        break;
    case ROBOT_QWARK_BROKEN_SPARKING:
        particle_velocity.f[0] =
            fast_cos(random_angle_radians()) * (random_float_between(0.0f, 0.25f) * frame_time);
        particle_velocity.f[1] =
            fast_sin(random_angle_radians()) * (random_float_between(0.0f, 0.25f) * frame_time);
        particle_velocity.f[2] = 0.0f;
        pos.f[0] = fast_cos(random_angle_radians()) * random_float_between(0.0f, 0.5f) *
                   random_float_between(0.0f, 1.0f);
        pos.f[1] = fast_sin(random_angle_radians()) * random_float_between(0.0f, 0.5f) *
                   random_float_between(0.0f, 1.0f);
        pos.f[2] = 0.0f;
        add_vectors(unused_vector_scratch, &pos, &robot_qwark_moby->pos);
        if (random_integer_below(3) == 0) {
            spawn_effect_particle_with_target_scalar(
                &pos,
                &particle_velocity,
                0x20202020,
                0x7F7F7F,
                scale_game_frames(FUN_L00_00257b90(0x3C, 0x78)),
                157500.0f);
        }
        break;
    }
}

int is_rail_lift_started(struct Moby *moby) __asm__("FUN_L03_002dcb30");

int is_rail_lift_started(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    if (moby->oclass != 0x3F4) {
        return 0;
    }
    if (moby->state && *(int *)(data + 0x7C) == 1) {
        return 1;
    }
    return 0;
}
/* Shows a help message for the mode, then keeps a hud queue entry alive. */
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_002CB280.c: func_L03_002DDF90), where it is exact; names translated to the US level program. */

extern void reset_numeric_hud_element_32x32_base(void) __asm__("FUN_L00_00235dc0");
extern void initialize_numeric_hud_element(void) __asm__("FUN_L00_00237190");
extern void hud_keep_visible(int, int) __asm__("FUN_L00_00235e18");
extern void FUN_L00_002371e0(void);
extern void draw_hud_text_panel(void) __asm__("FUN_L00_00237200");
s32         queue_animation_update(s32 chan, s32 id, s32 fn, s32 d, s32 e, s32 c, s32 b) __asm__("FUN_001ff308");

void update_rail_lift_help_and_hud(struct Moby *lift_moby, int mode) __asm__("FUN_L03_002dcbc8");

void update_rail_lift_help_and_hud(struct Moby *lift_moby, int mode) {
    char *lift_vars = (char *)lift_moby->pvars;
    int   hud_key;
    switch (mode) {
    case 1:
        try_set_help_message(8, 0xBCE);
        break;
    case 2:
        try_set_help_message(8, 0xBCF);
        break;
    case 3:
        try_set_help_message(8, 0xBCB);
        break;
    case 4:
        try_set_help_message(8, 0xBCD);
        break;
    default:
        try_set_help_message(8, -1);
        break;
    }
    hud_key = *(int *)(lift_vars + 0xA4);
    if (hud_key == -1) {
        *(int *)(lift_vars + 0xA4) = queue_animation_update(0xC,
                                                            0,
                                                            (int)initialize_numeric_hud_element,
                                                            (int)FUN_L00_002371e0,
                                                            (int)draw_hud_text_panel,
                                                            0,
                                                            0);
    } else {
        hud_keep_visible(hud_key, 0xA);
    }
}

/* Pvars of a rail lift that carries the hero between the ends of two rail paths. */
struct RailLiftVars {
    u8  pad0[0x20];
    u8  coll[0x44]; /* 0x20: collision record moved with the lift */
    s32 unk64;      /* 0x64: set to 1 when a ride starts */
    u8  pad68[0x10];
    s32 snd;       /* 0x78: travel sound voice (FUN_L03_002d3918) */
    s32 started;   /* 0x7C: set once the first ride starts */
    u8  start_end; /* 0x80: path end the lift spawns at */
    u8  pad81[3];
    s32 ready;     /* 0x84: set when the hero walks away from a stopped lift */
    s32 timer;     /* 0x88: frames to wait after a ride */
    s32 autostart; /* 0x8C: start without waiting for the trigger volume */
    s32 path[2];   /* 0x90: rail path index of each end (-1: none) */
    s32 help[2];   /* 0x98: help message mode for each end (FUN_L03_002dcbc8) */
    s32 volume;    /* 0xA0: trigger clip volume */
    s32 hud;       /* 0xA4: hud queue entry (FUN_L03_002dcbc8) */
};

void update_moby_oscillation(struct Moby *m) __asm__("FUN_L03_002dd0f0");

void update_rail_lift(struct Moby *lift_moby) __asm__("FUN_L03_002dccc0");

void update_rail_lift(struct Moby *lift_moby) {
    enum {
        LIFT_INITIALIZE = 0,
        LIFT_WAIT_FOR_DEPARTURE = 1,
        LIFT_WAIT_FOR_HERO_ANIMATION = 2,
        LIFT_FOLLOW_PATH = 3
    };
    struct RailLiftVars *lift_vars = (struct RailLiftVars *)lift_moby->pvars;
    Vec4                 oldPos, oldRot, move;
    s32                  hero_end, lift_end;

    if (lift_vars->path[0] == -1 || lift_vars->path[1] == -1)
        return;
    set_moby_dialogue_record_value(lift_moby, is_rail_lift_started((unsigned char *)lift_moby));
    qcopy(&oldPos, &lift_moby->pos);
    oldRot.q = *(u128 *)&lift_moby->rot;
    fast_dec_timer(&lift_vars->timer);
    switch (lift_moby->state) {
    case LIFT_INITIALIZE:
        lift_moby->command = lift_vars->start_end;
        qcopy(&lift_moby->pos,
              &((struct RailPath *)l03_paths[lift_vars->path[lift_moby->command]])->pts[0]);
        lift_vars->hud = -1;
        lift_vars->timer = 0;
        lift_moby->state = 1;
        lift_moby->update_dist = 0xFF;
        lift_moby->draw_dist = 0xFF;
        break;
    case LIFT_WAIT_FOR_DEPARTURE:
        hero_end = fast_vec_dist(&hero.motion.pos,
                                 &((struct RailPath *)l03_paths[lift_vars->path[0]])->pts[0]) <
                   fast_vec_dist(&hero.motion.pos,
                                 &((struct RailPath *)l03_paths[lift_vars->path[1]])->pts[0]);
        lift_end = fast_vec_dist(&lift_moby->pos,
                                 &((struct RailPath *)l03_paths[lift_vars->path[0]])->pts[0]) <
                   fast_vec_dist(&lift_moby->pos,
                                 &((struct RailPath *)l03_paths[lift_vars->path[1]])->pts[0]);
        if (!lift_vars->started) {
            if (lift_vars->autostart ||
                is_point_inside_clip_volume(&hero.motion.pos, lift_vars->volume)) {
                lift_moby->collision = lift_moby->pclass->collision;
                lift_moby->command = hero_end;
                lift_moby->flags &= 0xFFBE;
                *(u128 *)&lift_moby->pos =
                    *(u128 *)&((struct RailPath *)l03_paths[lift_vars->path[hero_end]])->pts[0];
                lift_moby->state = 3;
                lift_vars->unk64 = 1;
                lift_vars->started = 1;
                lift_vars->ready = 0;
                break;
            }
        }
        if (!lift_vars->started) {
            lift_moby->collision = 0;
            lift_moby->flags |= 0x41;
        } else if (lift_vars->ready && is_hero_on_moby(lift_moby)) {
            update_rail_lift_help_and_hud((char *)lift_moby,
                                          lift_vars->help[lift_moby->command ^ 1]);
            if ((D_0013CAE4 & 0x10) && D_L03_0015F594 == 8) {
                request_hero_target_pose(&lift_moby->pos, lift_moby->rot.z, 0);
                lift_moby->command ^= 1;
                lift_vars->unk64 = 1;
                lift_vars->ready = 0;
                lift_moby->state = 2;
            }
        } else if (hero_end != lift_end) {
            lift_moby->state = 3;
            lift_moby->command ^= 1;
            lift_vars->unk64 = 1;
            lift_vars->ready = 0;
        }
        break;
    case LIFT_WAIT_FOR_HERO_ANIMATION:
        if (!get_hero_target_pose_status())
            lift_moby->state = 3;
        break;
    case LIFT_FOLLOW_PATH:
        if (is_hero_on_moby(lift_moby)) {
            hero.jump_lock_timer = 4;
            hero.unk1F4 = 4;
        }
        if (advance_moby_along_rail_path(
                lift_moby, lift_vars->path[lift_moby->command], &lift_vars->snd)) {
            lift_moby->state = 1;
            lift_vars->timer = scale_game_frames(0xF);
        }
        break;
    }
    update_moby_oscillation(lift_moby);
    fast_vec_sub(&move, &lift_moby->pos, &oldPos);
    update_entity_matrix(lift_vars->coll, &move, &oldRot, &lift_moby->rot);
    if (!is_hero_on_moby(lift_moby) &&
        vector_distance_xy(&lift_moby->pos, &hero.motion.pos) > 2.0f && lift_vars->timer == 0)
        lift_vars->ready = 1;
}

float path_mover_distance_step = 0;
