/* Shared moby class doom_bot: the robot the glove of doom releases, which hops after enemies, and the function that spawns it. */

#include "types.h"
#include "rnc/game/item_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/hero/copy_hero_moby_field_38.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/world/collision.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/mobys/get_moby_target_info.h"
#include "rnc/overlay/shared/mobys/moby_target_info.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/shared/weapons/doom_bot_movement.h"
#include "rnc/overlay/shared/mobys/ground_move_control.h"

typedef struct {
    OvlVec4       target, velocity, previous_position;
    int           timer, age;
    struct Moby  *target_moby, *last;
    float         speed, turn_velocity, vertical_velocity; /* 0x40: as GroundMoveControl */
    int           move_timer;
    float         hop_vertical_speed, hop_horizontal_speed, hop_heading, max_speed;
    struct Moby  *owner;
    float         angle;
    unsigned char mode;
} DoomBotVars;

typedef struct {
    OvlVec4 target;
    float   vertical_speed, horizontal_speed, heading, gravity, radius, launch_frame, landing_lead;
    unsigned char anim2C, anim2D, anim2E, mode2F;
    float         turn_velocity, turn_spring, turn_damping, max_turn_step;
} DoomBotJump;

typedef struct {
    char                     pad0[0x1EC0];
    struct GroundMoveControl move;   /* 0x1EC0: handed to step_mover */
    DoomBotJump              config; /* 0x1F10: hop arc state for update_arc_motion */
    char                     pad1F50[0x130];
    struct Moby             *moby;
} DoomBotParams;

void         update_entity() __asm__("FUN_L00_00250df8");
extern void  fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void  fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern int   scale_game_frames(int) __asm__("FUN_001f96f8");
float        fast_vec_length(void *) __asm__("FUN_001f9af0");
void         fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern f32   fast_arc_tan(f32, f32) __asm__("FUN_001f9e90");
extern f32   fast_difference_between_rotations(f32, f32) __asm__("FUN_001fa688");
extern f32   vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern s32   truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void *memset(void *, int, unsigned int) __asm__("FUN_001153fc");
extern void  debug_print(char *) __asm__("FUN_001e93b0");
extern void  clamp_vector_length(void *, void *, f32) __asm__("FUN_001f9c90");
extern int   coll_sphere(void *, float, int, void *) __asm__("FUN_L00_001f0d60");
extern s32   collision_line(void *, void *, s32, void *, s32) __asm__("FUN_001efa68");
extern void  spawn_explosion(float,
                             void *,
                             void *,
                             int,
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
                             int,
                             int,
                             int,
                             int) __asm__("FUN_L00_0025e450");
extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
extern void
send_blast_hits(void *, void *, void *, int, void *, int, int, int, float, float, float) __asm__(
    "FUN_L00_0025a9f8");
extern int  FUN_L00_001ff600(float, void *, void *);
extern int  probe_moby_raised_sphere_collision(char *, int, void *) __asm__("FUN_L00_002beab0");
extern void FUN_L00_00260400(void *, int, void *, void *, void *, void *);
extern int  move_moby_with_collision_integer_radius(void *, void *, int, int, float, float) __asm__(
    "FUN_L00_00258ad0");
extern void FUN_L00_00258278(void *, float *, float, float, float, float);
extern struct Moby *
select_class5_target_or_hero(void *, void *, void *, void *, int) __asm__("FUN_L00_002bed90");
extern float select_moby_heading_by_collision_probes(void *,
                                                     void *,
                                                     float,
                                                     float,
                                                     float,
                                                     float,
                                                     float,
                                                     void *,
                                                     void *,
                                                     float) __asm__("FUN_L00_002bf050");
extern int   try_start_moby_hop(void *, void *, void *, int) __asm__("FUN_L00_002bf9f0");
extern void  initialize_moby_hop_trajectory(void *, void *, void *) __asm__("FUN_L00_002bf8b0");
extern int   compute_moby_state_from_vertical_probe(void *) __asm__("FUN_L00_002bfec8");
extern int   update_arc_motion(void *, void *) __asm__("FUN_L00_0025b440");
extern int   step_mover_2e3f20(void *, void *, void *, void *) __asm__("FUN_L00_00259028");
extern void  fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");

extern CollisionHit coll_hit __asm__("D_L00_00173E40") NOT_SDA;
extern char         D_L00_00177F00[], D_L00_001E9B48[], D_L00_001E9B70[];
extern s32          D_L00_0015F5C4;
extern CollisionHit coll_hit __asm__("D_L00_00173E40");

unsigned char *
spawn_doom_bot(int a, u128 *spawn_position, u128 *initial_vector) __asm__("FUN_L00_002bfe08");

unsigned char *spawn_doom_bot(int linked_moby, u128 *spawn_position, u128 *initial_vector) {
    unsigned char *spawned_moby = create_moby(0xba);
    if (spawned_moby) {
        char *moby_vars;
        MOBY(spawned_moby)->update_dist = 0xff;
        MOBY(spawned_moby)->draw_dist = 0xff;
        MOBY(spawned_moby)->visible = 1;
        moby_vars = ((char *)MOBY(spawned_moby)->pvars);
        MOBY(spawned_moby)->state = 0;
        MOBY(spawned_moby)->command = 0;
        qcopy(&MOBY(spawned_moby)->pos, spawn_position);
        qcopy(moby_vars + 0x10, initial_vector);
        *(int *)(moby_vars + 0x60) = linked_moby;
        update_entity(spawned_moby);
        copy_hero_moby_field_38(spawned_moby);
        MOBY(spawned_moby)->scale = *(float *)(((char *)MOBY(spawned_moby)->pclass) + 0x24) * 0.1f;
    }
    return spawned_moby;
}

/* Updates an Agent of Doom's movement, target selection and detonation. */
/* Ported from rac1-decomp (nonmatching/shared/func_L00_002C12B0.c: func_L00_002C12B0). */
void doom_bot_update(struct Moby *moby) __asm__("FUN_L00_002bffa8");

/* Moby class 186 */
void doom_bot_update(struct Moby *moby) {
    enum {
        DOOM_BOT_INITIALIZE = 0,
        DOOM_BOT_WAIT_FOR_ANIMATION_BEFORE_MOVEMENT = 1,
        DOOM_BOT_SELECT_MOVEMENT_MODE = 2,
        DOOM_BOT_UPDATE_TARGET_AND_MOVEMENT = 3,
        DOOM_BOT_MOVE_FAST = 4,
        DOOM_BOT_MOVE_SLOW = 5,
        DOOM_BOT_ALIGN_TO_TARGET = 6,
        DOOM_BOT_EXECUTE_JUMP = 7,
        DOOM_BOT_COLLISION_RECOVERY_FALL = 8,
        DOOM_BOT_WAIT_FOR_ANIMATION_TO_SELECT_MOVEMENT = 9,
        DOOM_BOT_WAIT_FOR_ANIMATION_COMPLETION = 10,
        DOOM_BOT_DETONATE = 11,
        DOOM_BOT_WAIT_FOR_TRANSITION_ANIMATION = 12,
        DOOM_BOT_SQUIRM = 13,
        DOOM_BOT_INITIAL_FALL = 14,
        DOOM_BOT_RESET_TO_INITIALIZE = 15
    };
    enum {
        MOBY_STATE_PENDING_REMOVAL = 0xFE
    };
    OvlVec4         movement_and_detonation_vectors[4];
    float           clearance;
    DoomBotVars    *doom_bot_vars;
    struct Moby    *target;
    MobyTargetInfo *aim;
    float           scale, speed, z, height, reach, angle, distance;
    int             blocked, move_flags, count, arc_motion_result;
    unsigned char   old_state;
    if (!moby || !(doom_bot_vars = (DoomBotVars *)moby->pvars))
        return;
    if (D_L00_0015F5C4 == 2) {
        moby->flags |= 0x41;
        return;
    }
    moby->flags &= 0xFFBE;
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
        moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
        mark_moby_for_removal(moby);
        return;
    }
    if (find_moby_hit(moby, 0x800001, 0))
        moby->state = DOOM_BOT_DETONATE;
    scale = convert_integer_to_float(item_text_variant[0x14]) + 1.0f;
    moby->scale += (moby->pclass->scale * scale - moby->scale) * 0.05f;
    speed = convert_integer_to_float(item_text_variant[0x14]) * 0.5f + 1.0f;
    if (moby->seq == 0 || moby->seq == 6 || moby->seq == 7)
        moby->anim_speed = speed / scale;
    else
        moby->anim_speed = 1.0f;
    if (moby->state != DOOM_BOT_DETONATE) {
        int            collision_radius = truncate_float_to_s32(scale * 183.296f) + 1;
        DoomBotParams *hp = (DoomBotParams *)&hero;
        hp->move.collision_radius_1024ths = collision_radius;
        hp->move.ground_probe_depth = 0;
        hp->move.step_height = scale * 0.198f + 0.02f;
        if (moby->state == DOOM_BOT_INITIALIZE) {
            float frame, acceleration;
            memset(&hp->config, 0, 64);
            frame = frame_scale;
            hp->move.max_climb = hp->move.max_drop = scale * 0.25f;
            doom_bot_vars->turn_velocity = frame * 0.2f;
            acceleration = scale * 0.01f * frame;
            doom_bot_vars->speed = acceleration;
            hp->move.brake = 0.3f;
            hp->move.max_speed = 2.7f;
            hp->move.acceleration = acceleration;
            hp->move.vertical_velocity = 0;
            doom_bot_vars->max_speed = frame * 0.066f * speed;
            hp->config.anim2C = 2;
            hp->config.anim2D = 3;
            hp->config.anim2E = 4;
            hp->move.heading_tolerance = 3.1415927f;
            hp->move.turn_damping = frame * 0.01f;
            hp->move.turn_spring = frame * 0.02f;
            hp->move.max_turn_step = frame * 0.045f;
            hp->config.gravity = frame_time_sq * 29.7f;
            hp->config.radius = scale * 0.1f;
            hp->config.launch_frame = 7.5f;
            hp->config.landing_lead = 11.5f;
            hp->config.turn_spring = frame * 0.02f;
            hp->config.turn_damping = frame * 0.01f;
            hp->config.max_turn_step = frame * 0.045f;
            doom_bot_vars->target_moby = 0;
            moby->state = DOOM_BOT_INITIAL_FALL;
            doom_bot_vars->timer = 0;
            doom_bot_vars->max_speed = frame_scale * 0.1f * speed;
            save_state_fields_to_hero((char *)doom_bot_vars);
            save_state_vector_and_fields_to_hero((char *)doom_bot_vars);
            if (moby->prev_seq != 5)
                blend_moby_animation(moby, 5, 0, scale_game_frames(5));
        }
        if (moby->state == DOOM_BOT_SQUIRM) {
            if (!doom_bot_vars->owner || doom_bot_vars->owner->state == 0xFE ||
                doom_bot_vars->owner->state == 0xFD || moby->pos.x < 2.0f ||
                moby->pos.x > 1021.0f || moby->pos.y < 2.0f || moby->pos.y > 1021.0f ||
                moby->pos.z < 2.0f || moby->pos.z > 1021.0f) {
                mark_moby_for_removal(moby);
                return;
            }
            return;
        }
        doom_bot_vars->age++;
        if (scale_game_frames(3600) < doom_bot_vars->age)
            moby->state = DOOM_BOT_DETONATE;
        fast_dec_timer(&doom_bot_vars->timer);
        qcopy(&doom_bot_vars->previous_position, &moby->pos);
        if (probe_moby_raised_sphere_collision((char *)moby, 5, 0)) {
            if (coll_hit.moby && coll_hit.surface_type < 0 &&
                (((struct Moby *)coll_hit.moby)->flags & 0x1000) &&
                ((struct Moby *)coll_hit.moby)->pclass &&
                ((struct Moby *)coll_hit.moby)->pclass->type == 5) {
                moby->state = DOOM_BOT_DETONATE;
            } else if (!coll_hit.moby || coll_hit.surface_type < 0) {
                z = moby->pos.z;
                qcopy_nc(&moby->pos, coll_hit.pad30);
                if (absolute_float(moby->pos.z - z) > 0.6f)
                    moby->pos.z = z + 0.01f;
                qcopy(&doom_bot_vars->velocity, &coll_hit.normal_x);
                clamp_vector_length(
                    &doom_bot_vars->velocity, &doom_bot_vars->velocity, frame_scale * 0.05f);
                FUN_L00_001ff600(
                    frame_scale * 0.05f, &doom_bot_vars->velocity, &doom_bot_vars->velocity);
                moby->state = DOOM_BOT_COLLISION_RECOVERY_FALL;
                if (doom_bot_vars->target_moby) {
                    doom_bot_vars->last = doom_bot_vars->target_moby;
                    doom_bot_vars->timer += 500;
                }
                if (moby->prev_seq != 5)
                    blend_moby_animation(moby, 5, 0, scale_game_frames(5));
            }
        }
        if (moby->pos.z < 0.1f) {
            mark_moby_for_removal(moby);
            return;
        }
        if (doom_bot_vars->timer >= 4001)
            moby->state = DOOM_BOT_DETONATE;
    }
    if (probe_ground_height(&moby->pos, 0, 0.5f) != 0 && coll_hit.moby)
        FUN_L00_00260400(moby, coll_hit.moby, &moby->pos, &moby->rot, &moby->pos, &moby->rot);
    switch (moby->state) {
    case DOOM_BOT_INITIALIZE:
        debug_print(D_L00_001E9B48);
        moby->state = DOOM_BOT_WAIT_FOR_ANIMATION_BEFORE_MOVEMENT;
        break;
    case DOOM_BOT_COLLISION_RECOVERY_FALL: {
        DoomBotParams *hp;
        fast_vec_add(&movement_and_detonation_vectors[0], &moby->pos, &doom_bot_vars->velocity);
        doom_bot_vars->velocity.f[2] -= frame_time_sq * 9.0f;
        save_state_fields_to_hero((char *)doom_bot_vars);
        hp = (DoomBotParams *)&hero;
        blocked = move_moby_with_collision_integer_radius(moby,
                                                          &doom_bot_vars->velocity,
                                                          hp->move.collision_radius_1024ths,
                                                          0,
                                                          scale * 0.7f,
                                                          hp->move.ground_probe_depth) &
                  2;
        if (blocked) {
            doom_bot_vars->velocity.f[2] = 0;
            fast_vec_scale(&doom_bot_vars->velocity, &doom_bot_vars->velocity, 0.5f);
        }
        FUN_L00_001ff600(frame_scale * 0.05f, &doom_bot_vars->velocity, &doom_bot_vars->velocity);
        if (moby->anim_flags & 2) {
            if (!blocked) {
                if (moby->prev_seq != 5)
                    blend_moby_animation(moby, 5, 0, scale_game_frames(5));
            } else
                moby->state = DOOM_BOT_SELECT_MOVEMENT_MODE;
        }
        goto Falling;
    }
    case DOOM_BOT_WAIT_FOR_ANIMATION_BEFORE_MOVEMENT:
    case DOOM_BOT_WAIT_FOR_ANIMATION_TO_SELECT_MOVEMENT:
    case DOOM_BOT_WAIT_FOR_ANIMATION_COMPLETION:
        if (moby->anim_flags & 2)
            moby->state = DOOM_BOT_SELECT_MOVEMENT_MODE;
        goto Contact;
    case DOOM_BOT_SELECT_MOVEMENT_MODE:
    case DOOM_BOT_UPDATE_TARGET_AND_MOVEMENT:
    case DOOM_BOT_MOVE_FAST:
    case DOOM_BOT_MOVE_SLOW: {
        target = doom_bot_vars->target_moby;
        if (target && target->state != MOBY_STATE_PENDING_REMOVAL && target->state != 0xFD) {
            height = target->pos.z;
            aim = (MobyTargetInfo *)get_moby_target_info((char *)doom_bot_vars->target_moby);
            reach = speed;
            if (aim) {
                reach = speed + convert_integer_to_float(aim->size) * 0.125f;
                height += aim->aim_height;
            }
            if (fast_vec_dist(&moby->pos, &doom_bot_vars->target_moby->pos) < reach &&
                coll_sphere(&moby->pos, speed, 1, moby)) {
                moby->state = DOOM_BOT_DETONATE;
                break;
            }
            if (vector_distance(&moby->pos, &doom_bot_vars->target_moby->pos) < reach * 0.5f &&
                absolute_float(height - (moby->pos.z + speed * 0.1f)) < reach * 0.5f) {
                save_state_vector_and_fields_to_hero((char *)doom_bot_vars);
                initialize_moby_hop_trajectory(
                    moby, &((DoomBotParams *)&hero)->config, &doom_bot_vars->target_moby->pos);
                restore_saved_state_fields_from_hero((char *)doom_bot_vars);
                qcopy(doom_bot_vars, &doom_bot_vars->target_moby->pos);
                doom_bot_vars->max_speed = frame_scale * 0.066f * speed;
                if (moby->prev_seq != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(2));
                moby->state = DOOM_BOT_ALIGN_TO_TARGET;
            }
        }
        qcopy(&movement_and_detonation_vectors[0], &moby->pos);
        movement_and_detonation_vectors[0].f[2] += scale * 0.3f;
        if (random_integer_below(4) == 0)
            doom_bot_vars->target_moby = select_class5_target_or_hero(
                moby, &movement_and_detonation_vectors[0], &moby->rot, doom_bot_vars->last, 1);
        doom_bot_vars->max_speed = frame_scale * 0.1f * speed;
        moby->state = DOOM_BOT_MOVE_FAST;
        if (!doom_bot_vars->target_moby) {
            angle = random_float_between(-3.1415927f, 3.1415927f);
            if (random_integer_below(4) == 0) {
                float step_height = scale * 0.35f;
                float range = 4.0f;
                float radius = convert_integer_to_float(
                                   ((DoomBotParams *)&hero)->move.collision_radius_1024ths) *
                               0.0009765625f;
                doom_bot_vars->angle = select_moby_heading_by_collision_probes(moby,
                                                                               &moby->pos,
                                                                               angle,
                                                                               1.84799564f,
                                                                               range,
                                                                               step_height,
                                                                               4.0f,
                                                                               0,
                                                                               &clearance,
                                                                               radius);
            } else
                clearance = 5.0f;
        }
        if (doom_bot_vars->target_moby &&
            doom_bot_vars->target_moby != ((DoomBotParams *)&hero)->moby &&
            (!doom_bot_vars->target_moby || doom_bot_vars->target_moby->state == 0xFE ||
             doom_bot_vars->target_moby->state == 0xFD || !doom_bot_vars->target_moby->pclass ||
             doom_bot_vars->target_moby->pclass->type != 5 ||
             !(doom_bot_vars->target_moby->flags & 0x1000))) {
            doom_bot_vars->target_moby = 0;
            moby->state = DOOM_BOT_SELECT_MOVEMENT_MODE;
            doom_bot_vars->max_speed = frame_scale * 0.0273f * speed;
            break;
        }
        old_state = moby->state;
        if (!doom_bot_vars->target_moby) {
            moby->state = DOOM_BOT_MOVE_SLOW;
            if (moby->prev_seq != 6)
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            doom_bot_vars->max_speed = frame_scale * 0.0273f * speed;
        } else {
            if (random_integer_below(4) == 0 &&
                try_start_moby_hop(moby, doom_bot_vars, &doom_bot_vars->target_moby->pos, 1)) {
                qcopy(doom_bot_vars, &doom_bot_vars->target_moby->pos);
                doom_bot_vars->max_speed = frame_scale * 0.066f * speed;
                if (moby->prev_seq != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(2));
                moby->state = DOOM_BOT_ALIGN_TO_TARGET;
                break;
            }
            distance = vector_distance(&moby->pos, &doom_bot_vars->target_moby->pos);
            {
                DoomBotParams *hp = (DoomBotParams *)&hero;
                if (doom_bot_vars->target_moby == hp->moby) {
                    moby->state = DOOM_BOT_MOVE_SLOW;
                    if ((moby->anim_flags & 2) && moby->prev_seq != 6)
                        blend_moby_animation(moby, 6, 0, scale_game_frames(0));
                    doom_bot_vars->max_speed = frame_scale * 0.0273f * speed;
                }
            }
            if (!(distance < 8.0f) && distance > 20.0f) {
                moby->state = DOOM_BOT_MOVE_SLOW;
                if (moby->prev_seq != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                doom_bot_vars->max_speed = frame_scale * 0.0273f * speed;
            } else {
                moby->state = DOOM_BOT_MOVE_FAST;
                if (moby->prev_seq != 7)
                    blend_moby_animation(moby, 7, 0, scale_game_frames(10));
                doom_bot_vars->max_speed = frame_scale * 0.1f * speed;
            }
            if (random_integer_below(4) == 0) {
                DoomBotParams *hp = (DoomBotParams *)&hero;
                Vec4f         *target_pos = &doom_bot_vars->target_moby->pos;
                float          range = 4.0f;
                float          radius =
                    convert_integer_to_float(hp->move.collision_radius_1024ths) * 0.0009765625f;
                doom_bot_vars->angle = select_moby_heading_by_collision_probes(moby,
                                                                               &moby->pos,
                                                                               moby->rot.z,
                                                                               1.84799564f,
                                                                               range,
                                                                               0.35f,
                                                                               4.0f,
                                                                               target_pos,
                                                                               &clearance,
                                                                               radius);
            } else
                clearance = 5.0f;
        }
        if (moby->state == DOOM_BOT_MOVE_FAST && old_state == DOOM_BOT_MOVE_SLOW) {
            moby->state = DOOM_BOT_WAIT_FOR_TRANSITION_ANIMATION;
            if (moby->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            goto Contact;
        }
        doom_bot_vars->target.f[1] = moby->pos.y + fast_sin(doom_bot_vars->angle) * 5.0f;
        doom_bot_vars->target.f[0] = moby->pos.x + fast_cos(doom_bot_vars->angle) * 5.0f;
        doom_bot_vars->target.f[2] = moby->pos.z + 1.0f;
        save_state_fields_to_hero((char *)doom_bot_vars);
        {
            DoomBotParams *hp = (DoomBotParams *)&hero;
            FUN_L00_00258278(moby,
                             &doom_bot_vars->turn_velocity,
                             doom_bot_vars->angle,
                             hp->move.turn_spring,
                             hp->move.turn_damping,
                             hp->move.max_turn_step);
        }
        fast_vec_scale(&movement_and_detonation_vectors[3], &doom_bot_vars->velocity, 5.0f);
        movement_and_detonation_vectors[3].f[2] = 0;
        qcopy(&movement_and_detonation_vectors[1], &moby->pos);
        movement_and_detonation_vectors[1].f[2] += scale * 0.15f;
        fast_vec_add(&movement_and_detonation_vectors[2],
                     &movement_and_detonation_vectors[1],
                     &movement_and_detonation_vectors[3]);
        if (!collision_line(&movement_and_detonation_vectors[1],
                            &movement_and_detonation_vectors[2],
                            6,
                            moby,
                            0) ||
            coll_hit.moby == (s32)doom_bot_vars->target_moby ||
            (coll_hit.moby && ((struct Moby *)coll_hit.moby)->pclass &&
             ((struct Moby *)coll_hit.moby)->pclass->type == 5)) {
            z = moby->pos.z;
            move_flags = step_mover_2e3f20(moby,
                                           &((DoomBotParams *)&hero)->move.collision_radius_1024ths,
                                           doom_bot_vars,
                                           &doom_bot_vars->velocity);
            if (absolute_float(moby->pos.z - z) > 1.0f) {
                moby->pos.z = z;
                if (probe_moby_raised_sphere_collision((char *)moby, 5, 0)) {
                    qcopy(&moby->pos, coll_hit.pad30);
                    if (absolute_float(moby->pos.z - z) > 1.0f)
                        moby->pos.z = z;
                }
            }
        } else {
            clearance = 0;
            move_flags = 2;
            doom_bot_vars->velocity.f[0] = moby->pos.x - doom_bot_vars->target.f[0];
            doom_bot_vars->velocity.f[1] = moby->pos.y - doom_bot_vars->target.f[1];
            doom_bot_vars->velocity.f[2] = 0.01f;
            fast_vec_normalize(&doom_bot_vars->velocity, &doom_bot_vars->velocity, 0.1f);
        }
        restore_state_fields_from_hero((char *)doom_bot_vars);
        if ((move_flags & 2) || clearance < 4.0f) {
            if (try_start_moby_hop(moby, doom_bot_vars, doom_bot_vars, 0)) {
                doom_bot_vars->max_speed = frame_scale * 0.066f * speed;
                if (moby->prev_seq != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(2));
                moby->state = DOOM_BOT_ALIGN_TO_TARGET;
                goto Contact;
            }
            if (doom_bot_vars->target_moby) {
                doom_bot_vars->last = doom_bot_vars->target_moby;
                doom_bot_vars->timer += 500;
            }
            doom_bot_vars->velocity.f[0] = moby->pos.x - doom_bot_vars->target.f[0];
            doom_bot_vars->velocity.f[1] = moby->pos.y - doom_bot_vars->target.f[1];
            doom_bot_vars->velocity.f[2] = 0.01f;
            fast_vec_normalize(&doom_bot_vars->velocity, &doom_bot_vars->velocity, 0.1f);
            moby->state = DOOM_BOT_COLLISION_RECOVERY_FALL;
            if (moby->prev_seq != 5)
                blend_moby_animation(moby, 5, 0, scale_game_frames(5));
            break;
        }
        goto Contact;
    }
    case DOOM_BOT_ALIGN_TO_TARGET: {
        float          heading = fast_arc_tan(doom_bot_vars->target.f[0] - moby->pos.x,
                                     doom_bot_vars->target.f[1] - moby->pos.y);
        float         *velocity;
        DoomBotParams *hp;
        save_state_fields_to_hero((char *)doom_bot_vars);
        velocity = &((DoomBotParams *)&hero)->move.turn_velocity;
        hp = (DoomBotParams *)&hero;
        FUN_L00_00258278(moby,
                         velocity,
                         heading,
                         2.0f * hp->move.turn_spring,
                         0.5f * hp->move.turn_damping,
                         2.0f * hp->move.max_turn_step);
        restore_state_fields_from_hero((char *)doom_bot_vars);
        if (fast_difference_between_rotations(moby->rot.z, heading) < 0.08726646f) {
            blend_moby_animation(moby, hp->config.anim2C, 0, scale_game_frames(3));
            moby->state = DOOM_BOT_EXECUTE_JUMP;
        }
        goto Contact;
    }
    case DOOM_BOT_EXECUTE_JUMP:
        z = moby->pos.z;
        save_state_vector_and_fields_to_hero((char *)doom_bot_vars);
        arc_motion_result = update_arc_motion(moby, &((DoomBotParams *)&hero)->config);
        if (arc_motion_result == 4) {
            moby->state = arc_motion_result;
            if (moby->prev_seq != 7)
                blend_moby_animation(moby, 7, 0, scale_game_frames(3));
            doom_bot_vars->max_speed = frame_scale * 0.1f * speed;
        }
        restore_saved_state_fields_from_hero((char *)doom_bot_vars);
        absolute_float(moby->pos.z - z);
        goto Contact;
    case DOOM_BOT_DETONATE: {
        if (!(moby->pos.x < 2.0f) && !(moby->pos.x > 1021.0f) && !(moby->pos.y < 2.0f) &&
            !(moby->pos.y > 1021.0f) && !(moby->pos.z < 2.0f) && !(moby->pos.z > 1021.0f)) {
            qcopy(&movement_and_detonation_vectors[1], &moby->pos);
            movement_and_detonation_vectors[2].q = 0;
            movement_and_detonation_vectors[2].f[2] = 1.0f;
            movement_and_detonation_vectors[2].f[3] = 1.0f;
            spawn_explosion(0.0f,
                            moby,
                            &movement_and_detonation_vectors[2],
                            (int)&moby->pos,
                            0.0f,
                            2.0f * speed,
                            5,
                            2,
                            4,
                            speed,
                            9.0f,
                            1.0f,
                            15.0f,
                            -1,
                            1,
                            5,
                            -1,
                            item_text_variant[0x14]);
            movement_and_detonation_vectors[1].f[2] += scale * 0.5f;
            count = collect_mobies_in_sphere(&moby->pos, speed, 0x15, moby, 0);
            movement_and_detonation_vectors[3].q = *(OvlQuad *)&moby->pos;
            send_blast_hits(moby,
                            &movement_and_detonation_vectors[3],
                            D_L00_00177F00,
                            count,
                            0,
                            0x10000,
                            2,
                            3,
                            3.0f,
                            1.0f,
                            1.0f);
            if (item_text_variant[0x14])
                allocate_voice_for_target_entry(4, 0, moby);
            else
                allocate_voice_for_target_entry(0, 0, moby);
        }
    }
        mark_moby_for_removal(moby);
        return;
    case DOOM_BOT_WAIT_FOR_TRANSITION_ANIMATION:
        if (moby->anim_flags & 2) {
            moby->state = DOOM_BOT_MOVE_FAST;
            if (moby->prev_seq != 7)
                blend_moby_animation(moby, 7, 0, scale_game_frames(10));
            doom_bot_vars->max_speed = frame_scale * 0.1f * speed;
        }
        break;
    case DOOM_BOT_SQUIRM:
        debug_print(D_L00_001E9B70);
        break;
    case DOOM_BOT_INITIAL_FALL: {
        DoomBotParams *hp;
        fast_vec_add(&movement_and_detonation_vectors[1], &moby->pos, &doom_bot_vars->velocity);
        if (coll_sphere(&moby->pos, scale * 0.5f, 1, moby) && coll_hit.moby &&
            ((struct Moby *)coll_hit.moby)->oclass == 186) {
            fast_vec_sub(&movement_and_detonation_vectors[1], coll_hit.pad30, &moby->pos);
            distance = fast_vec_length(&movement_and_detonation_vectors[1]);
            movement_and_detonation_vectors[1].f[2] = 0;
            fast_vec_normalize(&movement_and_detonation_vectors[1],
                               &movement_and_detonation_vectors[1],
                               distance * 1.1f);
            fast_vec_add(&moby->pos, &moby->pos, &movement_and_detonation_vectors[1]);
        }
        doom_bot_vars->velocity.f[2] -= frame_time_sq * 9.0f;
        save_state_fields_to_hero((char *)doom_bot_vars);
        hp = (DoomBotParams *)&hero;
        blocked = move_moby_with_collision_integer_radius(moby,
                                                          &doom_bot_vars->velocity,
                                                          hp->move.collision_radius_1024ths,
                                                          0,
                                                          scale * 0.7f,
                                                          hp->move.ground_probe_depth) &
                  2;
        if (blocked) {
            doom_bot_vars->velocity.f[2] = 0;
            fast_vec_scale(&doom_bot_vars->velocity, &doom_bot_vars->velocity, 0.5f);
        }
        clamp_vector_length(
            &doom_bot_vars->velocity, &doom_bot_vars->velocity, frame_scale * 0.05f);
        if (!blocked) {
            if (moby->prev_seq != 5)
                blend_moby_animation(moby, 5, 0, scale_game_frames(5));
        } else
            moby->state = DOOM_BOT_SELECT_MOVEMENT_MODE;
    }
        /* Fakematch: the do-while(0) splits the scheduling region. */
    Falling:
        do {
            float direction =
                fast_arc_tan(doom_bot_vars->velocity.f[0], doom_bot_vars->velocity.f[1]);
            DoomBotParams *hp = (DoomBotParams *)&hero;
            FUN_L00_00258278(moby,
                             &doom_bot_vars->speed,
                             direction,
                             hp->move.acceleration,
                             hp->move.brake,
                             hp->move.max_speed);
            /* Fakematch: no-op update of data; it depends on the a1 setup, so a1 is scheduled before a0. */
            doom_bot_vars =
                (DoomBotVars *)((int)doom_bot_vars | ((unsigned char)(int)doom_bot_vars >> 8));
        } while (0);
        restore_state_fields_from_hero((char *)doom_bot_vars);
    Contact:
        /* Fakematch: the zero term keeps data live so combine keeps the no-op update above. */
        moby->state =
            compute_moby_state_from_vertical_probe(moby) | ((unsigned char)(int)doom_bot_vars >> 8);
        break;
    case DOOM_BOT_RESET_TO_INITIALIZE:
        moby->state = DOOM_BOT_INITIALIZE;
        break;
    default:
        moby->state = DOOM_BOT_INITIALIZE;
        break;
    }
    if (moby->pos.x < 2.0f || moby->pos.x > 1021.0f || moby->pos.y < 2.0f ||
        moby->pos.y > 1021.0f || moby->pos.z < 2.0f || moby->pos.z > 1021.0f)
        mark_moby_for_removal(moby);
    return;
}
