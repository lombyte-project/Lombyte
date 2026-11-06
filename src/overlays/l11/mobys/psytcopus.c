/* Level 11 moby class psytcopus: hits and its path state machine. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/knockback_motion.h"

enum {
    PSYTCOPUS_STATE_INITIALIZE = 0,
    PSYTCOPUS_STATE_FACE_PLAYER = 3,
    PSYTCOPUS_STATE_CREATE_PROJECTILE = 4,
    PSYTCOPUS_STATE_FIRE_PROJECTILE = 5,
    PSYTCOPUS_STATE_ATTACK_RECOVERY = 6,
    PSYTCOPUS_STATE_KNOCKBACK = 7,
    PSYTCOPUS_STATE_DEATH_KNOCKBACK = 8
};

extern float
approach_angle(float *p, float a, float *v, float b, float c, float d) __asm__("FUN_L00_0025be00");
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int   resolve_moby_hit_response(void *, void *, float *, int, int *, int, int, int) __asm__("FUN_00213928");

/* pvars of the moby run by psytcopus_update (FUN_L11_00310698 does its hit handling) */
struct PokiPathVars {
    u8                     pad_00[0x20];
    f32                    health;     /* 0x20 */
    s16                    max_health; /* 0x24 */
    u8                     pad_26[2];
    u8                     hit_response_class; /* 0x28 */
    u8                     pad_29;
    u8                     unk2A; /* 0x2A */
    u8                     pad_2B[0x2D];
    u8                     unk58; /* 0x58 */
    u8                     pad_59;
    u8                     unk5A; /* 0x5A */
    u8                     pad_5B[5];
    u8                     move[7]; /* 0x60: passed to trooper_anim_update / trooper_anim_finish */
    u8                     hit_reaction_channel_target; /* 0x67 */
    u8                     pad_68[8];
    struct KnockbackMotion knockback; /* 0x70 */
    Vec4f                  goal;      /* 0xD0 */
    u8                     pad_E0[0x30];
    struct Moby           *target; /* 0x110 */
    s32                    unk114; /* 0x114 */
    u8                     pad_118[0x18];
    s32                    unk130;     /* 0x130 */
    s32                    start_path; /* 0x134 */
    u8                     pad_138[4];
    s32                    path_index; /* 0x13C */
    f32                    reach;      /* 0x140 */
    u8                     pad_144[8];
    f32                    yaw_turn_velocity; /* 0x14C */
    u8                     pad_150[8];
    s32                    knockback_extension_timer; /* 0x158 */
    u8                     pad_15C[4];
    void                  *held_projectile; /* 0x160 */
    u8                     pad_164[0x7C];
    u8                     manipulator[0x10]; /* 0x1E0 */
};

extern struct Moby *D_001413D0 NOT_SDA;
extern char                    D_L11_001677C0[];

#include "rnc/overlay/shared/world/level_path.h"

#include "rnc/overlay/l11/mobys/psytcopus_globals.h"

extern float psytcopus_projectile_forward_offset_length_gp __asm__("D_L11_00161FC8")
    __attribute__((sda));
float       psytcopus_projectile_z_offset_gp __asm__("D_L11_00161FCC") = 1.0f;
void        handle_psytcopus_hits(struct Moby *m) __asm__("FUN_L11_00310698");
extern void FUN_L00_00263ac8_0180(struct Moby *, int, void *, float) __asm__("FUN_L00_00263ac8");
extern void FUN_L00_0025a120_p(struct Moby *) __asm__("FUN_L00_0025a120");
extern int  is_value_within_interpolated_window(struct Moby *, float) __asm__("FUN_00214cc8");
extern void *
spawn_class_511_moby_with_sparks(struct Moby *, void *, int) __asm__("FUN_L11_00318d10");
extern void set_moby_state_two_data_and_timer(void *, void *) __asm__("FUN_L11_00318e98");
extern int  FUN_L00_0025c698_p(struct Moby *, void *) __asm__("FUN_L00_0025c698");
extern int  FUN_L00_00259740_p(void *, void *, int) __asm__("FUN_L00_00259740");
extern void FUN_L00_00257470_p(struct Moby *, int, int) __asm__("FUN_L00_00257470");
extern void
FUN_L00_0025f3e8_p(struct Moby *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");

#include "rnc/overlay/l11/mobys/psytcopus_data/psytcopus_update.h"

extern float D_L11_00161F9C __attribute__((sda));
extern void set_moby_position_and_refresh_removal_timer(void *, void *) __asm__("FUN_L11_00318e78");
extern void FUN_L00_0025ab48_p(void *, float *, float *, float *) __asm__("FUN_L00_0025ab48");
extern void
FUN_L00_0025c558_p(struct Moby *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern void FUN_L00_002371e0_p(void *) __asm__("FUN_L00_002371e0");
extern void FUN_L00_0025d458_p(struct Moby *, void *) __asm__("FUN_L00_0025d458");
extern void FUN_L00_0025d538_p(struct Moby *, void *) __asm__("FUN_L00_0025d538");
extern int
FUN_L00_0025ff38_p(struct Moby *, void *, int, int, void *, int, float) __asm__("FUN_L00_0025ff38");

/* Path moby update: hit handling first, then a state machine that turns toward the player or its target,
   spawns and fires a projectile (pvars 0x160) and dies by explosion or by reaching the end of its path. */
void psytcopus_update(struct Moby *m) __asm__("FUN_L11_00310180");

/* Moby class 1231 */
void psytcopus_update(struct Moby *psytcopus) {
    struct PokiPathVars *psytcopus_vars = (struct PokiPathVars *)psytcopus->pvars;
    struct Moby         *target;
    Vec4f                projectile_vector_scratch;
    float                angle;
    s32                 *path;
    int                  knockback_result_flags;

    handle_psytcopus_hits(psytcopus);
    target = psytcopus_vars->target;
    FUN_L00_00263ac8_0180(psytcopus, 0, psytcopus_vars->manipulator, 2.5f);
    if (psytcopus->visible != 0 && fast_vec_dist(&psytcopus->pos, D_L11_001677C0) < 28.0f) {
        FUN_L00_0025a120_p(psytcopus);
        psytcopus->shadow_range = 0x16;
    }
    switch (psytcopus->state) {
    case PSYTCOPUS_STATE_INITIALIZE:
        psytcopus_vars->max_health = 3;
        psytcopus_vars->health = 3.0f;
        psytcopus_vars->hit_response_class = 2;
        psytcopus_vars->unk2A = 6;
        psytcopus_vars->unk58 = truncate_float_to_s32(8.0f);
        psytcopus_vars->unk5A = truncate_float_to_s32(8.0f);
        psytcopus->state = PSYTCOPUS_STATE_FACE_PLAYER;
        if (psytcopus->prev_seq != 0) {
            blend_moby_animation(psytcopus, 0, 0, scale_game_frames(10));
        }
        if (psytcopus_vars->unk130 != 0) {
            psytcopus->state = 1;
            qcopy(&psytcopus->pos, (char *)(s32 *)l11_paths[psytcopus_vars->start_path] + 0x10);
            break;
        }
        psytcopus->state = PSYTCOPUS_STATE_FACE_PLAYER;
        if (psytcopus->prev_seq != 0) {
            blend_moby_animation(psytcopus, 0, 0, scale_game_frames(10));
        }
        break;
    case PSYTCOPUS_STATE_FACE_PLAYER: {
        float *yaw = &psytcopus->rot.z;
        angle = fast_arc_tan(D_001413D0->pos.x - psytcopus->pos.x,
                             D_001413D0->pos.y - psytcopus->pos.y);
        approach_angle(yaw,
                       angle,
                       &psytcopus_vars->yaw_turn_velocity,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 6.2831855f);
    }
        if (psytcopus_vars->unk114 != 2) {
            psytcopus->state = PSYTCOPUS_STATE_CREATE_PROJECTILE;
            if (psytcopus->prev_seq != 2) {
                blend_moby_animation(psytcopus, 2, 0, scale_game_frames(10));
            }
        }
        break;
    case PSYTCOPUS_STATE_CREATE_PROJECTILE: {
        float *yaw = &psytcopus->rot.z;
        angle = fast_arc_tan(target->pos.x - psytcopus->pos.x, target->pos.y - psytcopus->pos.y);
        approach_angle(yaw,
                       angle,
                       &psytcopus_vars->yaw_turn_velocity,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 6.2831855f);
    }
        if (is_value_within_interpolated_window(psytcopus, 15.0f) != 0) {
            fast_vec_normalize(&projectile_vector_scratch,
                               &psytcopus->rot_row0,
                               psytcopus_projectile_forward_offset_length_gp);
            fast_vec_add(&projectile_vector_scratch, &projectile_vector_scratch, &psytcopus->pos);
            projectile_vector_scratch.z += psytcopus_projectile_z_offset_gp;
            psytcopus_vars->held_projectile =
                spawn_class_511_moby_with_sparks(psytcopus, &projectile_vector_scratch, 5);
            break;
        }
        if (psytcopus->anim_flags & 2) {
            psytcopus->state = PSYTCOPUS_STATE_FIRE_PROJECTILE;
            if (psytcopus->prev_seq != 3) {
                blend_moby_animation(psytcopus, 3, 0, scale_game_frames(10));
            }
        }
        break;
    case PSYTCOPUS_STATE_FIRE_PROJECTILE: {
        float *yaw = &psytcopus->rot.z;
        angle = fast_arc_tan(target->pos.x - psytcopus->pos.x, target->pos.y - psytcopus->pos.y);
        approach_angle(yaw,
                       angle,
                       &psytcopus_vars->yaw_turn_velocity,
                       frame_time_sq * 6.2831855f,
                       frame_time_sq * 6.2831855f,
                       frame_time * 6.2831855f);
    }
        if (psytcopus_vars->held_projectile != 0 &&
            is_value_within_interpolated_window(psytcopus, 33.0f) != 0) {
            fast_vec_normalize(
                &projectile_vector_scratch, &psytcopus->rot_row0, frame_time * 30.0f);
            set_moby_state_two_data_and_timer(psytcopus_vars->held_projectile,
                                              &projectile_vector_scratch);
            psytcopus_vars->held_projectile = 0;
        }
        if (psytcopus->anim_flags & 2) {
            psytcopus->state = PSYTCOPUS_STATE_ATTACK_RECOVERY;
            if (psytcopus->prev_seq != 4) {
                blend_moby_animation(psytcopus, 4, 0, scale_game_frames(10));
            }
        }
        break;
    case PSYTCOPUS_STATE_ATTACK_RECOVERY:
        if (psytcopus->anim_flags & 2) {
            psytcopus->state = PSYTCOPUS_STATE_FACE_PLAYER;
            if (psytcopus->prev_seq != 0) {
                blend_moby_animation(psytcopus, 0, 0, scale_game_frames(10));
            }
        }
        break;
    case PSYTCOPUS_STATE_KNOCKBACK:
        knockback_result_flags = FUN_L00_0025c698_p(psytcopus, &psytcopus_vars->knockback);
        if (knockback_result_flags & 0x41) {
            path = (s32 *)l11_paths[psytcopus_vars->path_index];
            if (FUN_L00_00259740_p(&psytcopus->pos, path + 4, *path) != 0) {
                psytcopus->state = PSYTCOPUS_STATE_FACE_PLAYER;
                if (psytcopus->prev_seq != 0) {
                    blend_moby_animation(psytcopus, 0, 0, scale_game_frames(10));
                }
                break;
            }
            knockback_result_flags = 0x100;
        }
        if (knockback_result_flags & 0x120) {
            FUN_L00_00257470_p(psytcopus, 0, -1);
            FUN_L00_0025f3e8_p(psytcopus, &psytcopus->pos, -1, 0.5f, 10.0f);
            mark_moby_for_removal(psytcopus);
        }
        break;
    case PSYTCOPUS_STATE_DEATH_KNOCKBACK:
        if (FUN_L00_0025c698_p(psytcopus, &psytcopus_vars->knockback) & 0x160) {
            FUN_L00_0025f3e8_p(psytcopus, &psytcopus->pos, -1, 1.0f, 10.0f);
            FUN_L00_00257470_p(psytcopus, 0, -1);
            mark_moby_for_removal(psytcopus);
        }
        break;
    }
}

/* Hit handling for the psytcopus_update moby: takes damage, picks the hurt or death reaction, keeps it on its path. */
void handle_psytcopus_hits(struct Moby *psytcopus) {
    enum {
        HIT_REACTION_NONE = 0,
        HIT_REACTION_DEATH = 1,
        HIT_REACTION_DEATH_KNOCKBACK = 2,
        HIT_REACTION_KNOCKBACK = 3,
        HIT_REACTION_KNOCKBACK_AND_CANCEL_PROJECTILE = 4,
        HIT_REACTION_KNOCKBACK_WITH_TIMER_EXTENSION = 5,
        HIT_REACTION_KNOCKBACK_WITH_PROJECTILE_CANCELLATION_AND_TIMER_EXTENSION = 6,
        HIT_REACTION_KNOCKBACK_WITH_SOURCE_RELATIVE_HEADING = 7,
        HIT_REACTION_KNOCKBACK_WITH_SOURCE_RELATIVE_HEADING_AND_TIMER_EXTENSION = 8,
        HIT_REACTION_HURT_WITHOUT_KNOCKBACK = 9,
        HIT_REACTION_HURT_WITHOUT_STATE_CHANGE = 10,
        HIT_REACTION_NO_RESPONSE = 11
    };
    struct PokiPathVars *psytcopus_vars = (struct PokiPathVars *)psytcopus->pvars;
    Vec4f                projectile_and_hit_vector_scratch;
    int                  hits;
    float                damage;
    float                angle1;
    float                angle2;
    char                *psytcopus_hit_record;
    s32                 *path;
    u32                  hit_reaction;
    float                source_to_psytcopus_heading;

    fast_dec_timer(&psytcopus_vars->knockback_extension_timer);
    psytcopus->scale = psytcopus->pclass->scale * D_L11_00161F9C;
    if (psytcopus_vars->held_projectile != 0) {
        fast_vec_normalize(&projectile_and_hit_vector_scratch,
                           &psytcopus->rot_row0,
                           psytcopus_projectile_forward_offset_length_gp);
        fast_vec_add(&projectile_and_hit_vector_scratch,
                     &projectile_and_hit_vector_scratch,
                     &psytcopus->pos);
        projectile_and_hit_vector_scratch.z += psytcopus_projectile_z_offset_gp;
        set_moby_position_and_refresh_removal_timer(psytcopus_vars->held_projectile,
                                                    &projectile_and_hit_vector_scratch);
    }
    damage = 0.0f;
    psytcopus_hit_record = find_moby_hit(psytcopus, 0x330000, 0);
    hit_reaction = resolve_moby_hit_response(
        psytcopus, psytcopus_hit_record, &psytcopus_vars->health, 0, &hits, &damage, 0, 4);
    if (hits != 1 && psytcopus->state != PSYTCOPUS_STATE_DEATH_KNOCKBACK) {
        psytcopus_vars->health -= damage;
        if (psytcopus_vars->health <= 0.0f) {
            hit_reaction = 1;
        }
        psytcopus_vars->knockback.collision_radius_1024ths = truncate_float_to_s32(512.0f);
        psytcopus_vars->knockback.collision_z_offset = 0.5f;
        psytcopus_vars->knockback.gravity = psytcopus_knockback_base_gravity_gp * frame_time_sq;
        psytcopus_vars->knockback.speed_approach_step = 0.0005f;
        psytcopus_vars->knockback.flags = 9;
        psytcopus_vars->knockback.unk3D = 0;
        switch (hit_reaction) {
        case HIT_REACTION_NONE:
        case HIT_REACTION_NO_RESPONSE:
            break;
        case HIT_REACTION_HURT_WITHOUT_KNOCKBACK:
        case HIT_REACTION_HURT_WITHOUT_STATE_CHANGE:
            psytcopus_vars->hit_reaction_channel_target = 0xFA;
            break;
        case HIT_REACTION_KNOCKBACK:
        case HIT_REACTION_KNOCKBACK_AND_CANCEL_PROJECTILE:
        case HIT_REACTION_KNOCKBACK_WITH_TIMER_EXTENSION:
        case HIT_REACTION_KNOCKBACK_WITH_PROJECTILE_CANCELLATION_AND_TIMER_EXTENSION:
        case HIT_REACTION_KNOCKBACK_WITH_SOURCE_RELATIVE_HEADING:
        case HIT_REACTION_KNOCKBACK_WITH_SOURCE_RELATIVE_HEADING_AND_TIMER_EXTENSION:
            psytcopus_vars->knockback.speed =
                psytcopus_hurt_knockback_base_horizontal_speed_gp * frame_time;
            psytcopus_vars->knockback.vertical_speed =
                psytcopus_hurt_knockback_base_vertical_speed_gp * frame_time;
            psytcopus_vars->knockback.apex_animation_frame = 3.0f;
            psytcopus_vars->knockback.landing_animation_frame = 6.0f;
            source_to_psytcopus_heading = fast_arc_tan(
                psytcopus->pos.x - (*(struct Moby **)(psytcopus_hit_record + 0x20))->pos.x,
                psytcopus->pos.y - (*(struct Moby **)(psytcopus_hit_record + 0x20))->pos.y);
            *(u128 *)&projectile_and_hit_vector_scratch = *(u128 *)(psytcopus_hit_record + 0x10);
            angle1 = source_to_psytcopus_heading;
            FUN_L00_0025ab48_p(&projectile_and_hit_vector_scratch,
                               &angle1,
                               &psytcopus_vars->knockback.speed,
                               &psytcopus_vars->knockback.vertical_speed);
            FUN_L00_0025c558_p(psytcopus, &psytcopus_vars->knockback, 5, 1, 0, angle1);
            psytcopus->state = PSYTCOPUS_STATE_KNOCKBACK;
            psytcopus_vars->hit_reaction_channel_target = 0x78;
            if (psytcopus_vars->held_projectile != 0) {
                FUN_L00_002371e0_p(psytcopus_vars->held_projectile);
                psytcopus_vars->held_projectile = 0;
            }
            psytcopus_vars->knockback_extension_timer += scale_game_frames(60);
            break;
        case HIT_REACTION_DEATH:
        case HIT_REACTION_DEATH_KNOCKBACK:
            psytcopus->flags &= 0xEFFF;
            psytcopus_vars->knockback.gravity = psytcopus_knockback_base_gravity_gp * frame_time_sq;
            psytcopus_vars->knockback.speed =
                psytcopus_death_knockback_base_horizontal_speed_gp * frame_time;
            psytcopus_vars->knockback.vertical_speed =
                psytcopus_death_knockback_base_vertical_speed_gp * frame_time;
            psytcopus_vars->knockback.apex_animation_frame = 4.0f;
            psytcopus_vars->knockback.landing_animation_frame = 10.0f;
            source_to_psytcopus_heading = fast_arc_tan(
                psytcopus->pos.x - (*(struct Moby **)(psytcopus_hit_record + 0x20))->pos.x,
                psytcopus->pos.y - (*(struct Moby **)(psytcopus_hit_record + 0x20))->pos.y);
            *(u128 *)&projectile_and_hit_vector_scratch = *(u128 *)(psytcopus_hit_record + 0x10);
            angle2 = source_to_psytcopus_heading;
            FUN_L00_0025ab48_p(&projectile_and_hit_vector_scratch,
                               &angle2,
                               &psytcopus_vars->knockback.speed,
                               &psytcopus_vars->knockback.vertical_speed);
            FUN_L00_0025c558_p(psytcopus, &psytcopus_vars->knockback, 6, 1, 0, angle2);
            psytcopus->state = PSYTCOPUS_STATE_DEATH_KNOCKBACK;
            psytcopus_vars->hit_reaction_channel_target = 0xF0;
            if (psytcopus_vars->held_projectile != 0) {
                FUN_L00_002371e0_p(psytcopus_vars->held_projectile);
                psytcopus_vars->held_projectile = 0;
            }
            FUN_L00_00257470_p(psytcopus, 0, -1);
            break;
        }
        FUN_L00_0025d458_p(psytcopus, psytcopus_vars->move);
    }
    psytcopus->hit_slot = 0xFF;
    FUN_L00_0025d538_p(psytcopus, psytcopus_vars->move);
    psytcopus_vars->reach = 24.0f;
    path = (s32 *)l11_paths[psytcopus_vars->path_index];
    if (FUN_L00_0025ff38_p(
            psytcopus, &psytcopus_vars->goal, 0, 0, path + 4, *path, psytcopus_vars->reach) != 2 &&
        (psytcopus_vars->reach < vector_distance_xy(&psytcopus->pos, &psytcopus_vars->goal) ||
         absolute_float(psytcopus->pos.z - psytcopus_vars->goal.z) > 3.0f)) {
        psytcopus_vars->unk114 = 2;
    }
    if (psytcopus_vars->target == 0) {
        psytcopus_vars->target = D_001413D0;
    }
}

float psytcopus_projectile_forward_offset_length_gp __asm__("D_L11_00161FC8") = 2.0f;
