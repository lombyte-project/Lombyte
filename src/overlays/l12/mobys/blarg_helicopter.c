/* Level 12 moby class blarg_helicopter: path following, tilt and hits. */
#include "types.h"
#include "rnc/overlay/shared/mobys/hit_color_flash.h"
#include "rnc/overlay/shared/camera/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"

enum {
    BLARG_HELICOPTER_INITIALIZE = 0,
    BLARG_HELICOPTER_WAIT_FOR_ACTIVATION = 1,
    BLARG_HELICOPTER_FOLLOW_INITIAL_PATH = 2,
    BLARG_HELICOPTER_AIM_AND_FIRE = 3,
    BLARG_HELICOPTER_MOVE_BETWEEN_PATH_ENDPOINTS = 4,
    BLARG_HELICOPTER_KNOCKBACK = 5,
    BLARG_HELICOPTER_EXPLODE_AND_REMOVE_OR_RESET = 6
};

extern char D_L12_00167240[];

#include "rnc/overlay/shared/world/level_path.h"

extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");

#include "sda.h"

#include "qcopy.h"

extern int scale_game_frames(int);

#include "rnc/overlay/shared/math/quad.h"

typedef struct Moby Moby;
int                 FUN_L00_00257b90(int lo, int hi);
void                FUN_L00_0025a120(char *o);

#include "rnc/overlay/shared/world/level_globals.h"

extern void  spawn_explosion(void *,
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
extern float fast_subtract_rotations(float, float);
extern int   D_L12_0015F5C4 __asm__("D_L12_0015F5C4");
extern void  FUN_L00_00250df8_x(void *) __asm__("FUN_L00_00250df8");
extern void  moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void  FUN_L00_00257470(struct Moby *, int, int);
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern s32   is_point_inside_clip_volume(void *, s32) __asm__("FUN_00214720");

extern f32   approach_value(void *, f32, f32) __asm__("FUN_00213ed8");
extern float spring_turn(float *, float, float, float, float, float) __asm__("FUN_L00_00258110");
extern void  find_point_ahead(void *, void *, float) __asm__("FUN_L01_00274b78");
extern void  FUN_L00_00263ac8(void *, int, void *, float);
void         process_blarg_helicopter_hits(struct Moby *moby) __asm__("FUN_L12_002eb220");

extern void process_blarg_helicopter_hits(struct Moby *) __asm__("FUN_L12_002eb220");
void        update_blarg_helicopter_motion_tilt(struct Moby *moby) __asm__("FUN_L12_002eae00");

extern void update_blarg_helicopter_motion_tilt(struct Moby *) __asm__("FUN_L12_002eae00");
int follow_blarg_helicopter_path(struct Moby *moby, int path_index) __asm__("FUN_L12_002eaf68");

extern int          follow_blarg_helicopter_path(struct Moby *, int) __asm__("FUN_L12_002eaf68");
extern struct Moby *create_class_55b_moby_from_source(struct Moby *) __asm__("FUN_L12_00308670");
extern void         start_blarg_commander_knockback(struct Moby *,
                                                    struct Moby *) __asm__("FUN_L12_00308708");
extern void         release_voice_slot(int) __asm__("FUN_0022d798");
extern void         help_request(int, int) __asm__("FUN_L00_00203908");
extern void         advance_moby_animation(void *) __asm__("FUN_0020d580");
extern void spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern char *
spawn_rocket(void *, void *, int, int, float, float, float) __asm__("FUN_L12_002ecce8");
extern unsigned char D_001413F4 NOT_SDA;
extern int D_L12_00161F00       MACRO_ADDR;
extern int D_L12_00161F20       MACRO_ADDR;
extern int                      D_L12_001619A0 __attribute__((sda));

#include "rnc/game/game_globals.h"
#include "rnc/audio/voice_pool.h"

extern unsigned char             D_0013E550[];
extern unsigned char D_0013D4CB  NOT_SDA;
extern unsigned short D_00141C50 NOT_SDA;
extern int                       D_L12_0015F580_gp __asm__("D_L12_0015F580") MACRO_ADDR;

void blarg_helicopter_update(struct Moby *moby) __asm__("FUN_L12_002e9f68");
/* Ported from rac1-decomp (src/overlays/l12_hoven/mobyutil_00272D90.c: func_L12_0027CA68), where it is exact; names translated to the US level program. */

void cache_consecutive_point_distances(int point_data_index) __asm__("FUN_L12_0027ba70");

void cache_consecutive_point_distances(int point_data_index) {
    char *point_data;
    int   i;
    if (point_data_index == -1)
        return;
    point_data = ((char *)l12_paths[point_data_index]);
    for (i = 0; i < *(int *)point_data - 1; i++) {
        int point_byte_offset = i * 16;
        *(float *)(point_data + point_byte_offset + 0x1C) =
            fast_vec_dist(point_data + (point_byte_offset + 0x10),
                          point_data + (((i + 1) % *(int *)point_data) * 16 + 0x10));
    }
}

/* Springs the moby's two rotation angles toward a heading taken from its data's direction vector. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002EC180), where it is exact; names translated to the US level program. */

extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");

typedef struct BlargHelicopterVars {
    u8  pad_0[0x20];
    f32 health;
    s16 max_health;
    u8  pad_26[0x2];
    u8  hit_response_class;
    u8  wrench_hit_sound; /* 0x29: enemy target info at pvars+0x20, see moby_target_info.h */
    u8  pad_2A[0x2E];
    u8  look_range;
    u8  look_priority;
    u8  look_height;
    u8  pad_5B[0x5];
    struct HitColorFlash hit_flash;
    f32                  velocity[4];
    s32                  trigger_volume;
    u8                   pad_84[0x4];
    f32                  yaw_turn_velocity;
    s16                  rocket_fire_timer_frames;
    s16                  knockback_timer_frames;
    f32                  bob_angle;
    f32                  speed;
    f32                  roll_turn_velocity;
    f32                  pitch_turn_velocity;
    s32                  initial_path_index;
    s32                  endpoint_path_index;
    u8                   pad_A8[0x4];
    struct Moby         *commander;
    s32                  voice;
    s32                  path_point;
    s32                  muzzle_index;
    s16                  rockets_fired;
    s16                  target_first_path_endpoint;
    s32                  unkC0;
    s32                  activation_timer;
    u8                   pad_C8[0x8];
    u8                   manipulator;
} BlargHelicopterVars;

/* Moby class 326 */
void blarg_helicopter_update(struct Moby *moby) {
    BlargHelicopterVars *blarg_helicopter_vars = (BlargHelicopterVars *)moby->pvars;
    struct Moby         *child;
    float                target[20] __attribute__((aligned(16)));
    OvlVec4              position, delta, aim, velocity;
    int                  index, count, limit, a, b, color, life, size, triggered;
    float                distance, angle, value, spread;
    if (D_L12_0015F5C4 == 2)
        return;
    find_point_ahead(moby, target, 70.0f);
    child = blarg_helicopter_vars->commander;
    if (child != 0 && (signed char)child->state >= 0)
        FUN_L00_00263ac8(child, 0, &blarg_helicopter_vars->manipulator, 2.7f);
    process_blarg_helicopter_hits(moby);
    if (D_001413F4 == 2) {
        moby->scale = moby->pclass->scale * 0.5f;
        {
            struct Moby *child = blarg_helicopter_vars->commander;
            if (child != 0)
                child->scale = child->pclass->scale * 0.5f;
        }
    } else {
        moby->scale = moby->pclass->scale;
        {
            struct Moby *child = blarg_helicopter_vars->commander;
            if (child != 0)
                child->scale = child->pclass->scale;
        }
    }
    if ((unsigned int)(moby->state - 2) < 4) {
        if (blarg_helicopter_vars->voice == -1)
            blarg_helicopter_vars->voice = allocate_voice_for_target_entry(3, 4, moby);
    } else {
        int index = blarg_helicopter_vars->voice;
        if (index != -1 &&
            (void *)((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.owner == (void *)moby &&
            ((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.state)
            release_voice_slot(index);
        blarg_helicopter_vars->voice = -1;
    }
    index = blarg_helicopter_vars->unkC0;
    if (current_level_index == LEVEL_HOVEN && D_L12_00161F00 != 0) {
        if (index == 0) {
            if (l12_camera.focus_moby == moby)
                l12_camera.focus_moby = 0;
            if (!moby->visible) {
                if (blarg_helicopter_vars->commander) {
                    mark_moby_for_removal(blarg_helicopter_vars->commander);
                    blarg_helicopter_vars->commander = 0;
                }
                mark_moby_for_removal(moby);
                return;
            }
            index = blarg_helicopter_vars->unkC0;
            goto check_mode;
        }
    } else {
    check_mode:
        if (index == 0)
            goto update_state;
    }
    if (visited52[moby->mission + current_level_index * 16] == 255) {
        if (blarg_helicopter_vars->commander) {
            mark_moby_for_removal(blarg_helicopter_vars->commander);
            blarg_helicopter_vars->commander = 0;
        }
        mark_moby_for_removal(moby);
        return;
    }
    if (index == 0)
        goto update_state;
    if (D_L12_001619A0 != 0) {
        if (blarg_helicopter_vars->commander) {
            start_blarg_commander_knockback(moby, blarg_helicopter_vars->commander);
            blarg_helicopter_vars->commander = 0;
        }
        mark_moby_for_removal(moby);
        return;
    }
update_state:
    switch (moby->state) {
    case BLARG_HELICOPTER_INITIALIZE: {
        int a, b, limit;
        blarg_helicopter_vars->max_health = 6;
        blarg_helicopter_vars->health = 6.0f;
        blarg_helicopter_vars->hit_response_class = 2;
        blarg_helicopter_vars->wrench_hit_sound = 0;
        blarg_helicopter_vars->speed = 0.0f;
        blarg_helicopter_vars->roll_turn_velocity = 0.0f;
        blarg_helicopter_vars->pitch_turn_velocity = 0.0f;
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xefff;
        cache_consecutive_point_distances(blarg_helicopter_vars->initial_path_index);
        cache_consecutive_point_distances(blarg_helicopter_vars->initial_path_index);
        cache_consecutive_point_distances(blarg_helicopter_vars->endpoint_path_index);
        if (moby->draw_dist < 128)
            moby->draw_dist = 128;
        if (blarg_helicopter_vars->commander == 0) {
            blarg_helicopter_vars->commander = create_class_55b_moby_from_source(moby);
            blarg_helicopter_vars->commander->update_dist = 0;
            blarg_helicopter_vars->commander->flags |= 0x41;
        }
        qcopy(&moby->pos, ((char *)l12_paths[blarg_helicopter_vars->initial_path_index]) + 0x10);
        if (blarg_helicopter_vars->unkC0 != 0 && blarg_helicopter_vars->activation_timer == 0) {
            a = scale_game_frames(240);
            b = scale_game_frames(400);
            a = FUN_L00_00257b90(a, b);
            {
                int limit = D_L12_00161F20 - 1;
                blarg_helicopter_vars->activation_timer = a;
                if (limit > 10)
                    limit = 10;
                if (limit < 0)
                    limit = 0;
                blarg_helicopter_vars->activation_timer += limit * scale_game_frames(35);
            }
        }
        blarg_helicopter_vars->path_point = 0;
        moby->state = BLARG_HELICOPTER_WAIT_FOR_ACTIVATION;
        break;
    }
    case BLARG_HELICOPTER_WAIT_FOR_ACTIVATION: {
        int          triggered, a;
        struct Moby *child;
        triggered = 0;
        if (blarg_helicopter_vars->unkC0 && D_L12_00161F00)
            triggered = fast_dec_timer(&blarg_helicopter_vars->activation_timer) != 0;
        if ((blarg_helicopter_vars->trigger_volume != -1 &&
             is_point_inside_clip_volume(&hero.motion.pos,
                                         blarg_helicopter_vars->trigger_volume)) ||
            triggered) {
            blarg_helicopter_vars->look_range = 22;
            blarg_helicopter_vars->look_priority = 1;
            blarg_helicopter_vars->look_height = 7;
            moby->hit_slot = 255;
            moby->flags &= 0xffbe;
            child = blarg_helicopter_vars->commander;
            if (child)
                child->flags &= 0xffbe;
            moby->flags |= 0x1000;
            a = moby->pclass->collision;
            moby->state = BLARG_HELICOPTER_FOLLOW_INITIAL_PATH;
            moby->collision = a;
        }
        break;
    }
    case BLARG_HELICOPTER_FOLLOW_INITIAL_PATH:
        if (blarg_helicopter_vars->unkC0 == 0 && l12_camera.focus_moby == 0) {
            l12_camera.focus_moby = moby;
            l12_camera.focus_time = 0;
        }
        if (blarg_helicopter_vars->unkC0) {
            count = blarg_helicopter_vars->rocket_fire_timer_frames;
            limit = D_L12_00161F20 - 3;
            if (limit > 5)
                limit = 5;
            if (limit < 0)
                limit = 0;
            if (count < limit * scale_game_frames(15))
                blarg_helicopter_vars->rocket_fire_timer_frames = limit * scale_game_frames(15);
        }
        if (follow_blarg_helicopter_path(moby, blarg_helicopter_vars->initial_path_index))
            moby->state = BLARG_HELICOPTER_AIM_AND_FIRE;
        break;
    case BLARG_HELICOPTER_AIM_AND_FIRE: {
        int   count, limit, a, b, color, life, size;
        float distance, angle, value, spread;
        if (current_level_index == LEVEL_HOVEN && !D_0013D4CB && !D_00141C50)
            help_request(0x2ee2, 0x5d);
        distance = vector_distance_xy(&moby->pos, &hero.motion.pos);
        if (blarg_helicopter_vars->unkC0 == 0) {
            if (distance < 21.0f && moby->pos.z - hero.motion.pos.f[2] > 1.0f) {
                if (l12_camera.focus_moby == 0) {
                    l12_camera.focus_moby = moby;
                    l12_camera.focus_time = 0;
                }
            } else if (l12_camera.focus_moby == moby)
                l12_camera.focus_moby = 0;
        }
        distance = vector_distance_xy(&moby->pos, target);
        if ((distance < 40.0f && moby->pos.z - target[2] > -2.0f) || blarg_helicopter_vars->unkC0) {
            angle = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
            moby->rot.z = spring_turn(&blarg_helicopter_vars->yaw_turn_velocity,
                                      moby->rot.z,
                                      angle,
                                      0.004f,
                                      0.3f,
                                      frame_time * 0.7853982f);
            value = vector_distance_xy(target, &moby->pos);
            angle = fast_arc_tan(value, moby->pos.z - target[2]);
            moby->rot.y = spring_turn(&blarg_helicopter_vars->pitch_turn_velocity,
                                      moby->rot.y,
                                      angle,
                                      0.004f,
                                      0.3f,
                                      frame_time * 0.7853982f);
            clear_u64_value(blarg_helicopter_vars->velocity);
            update_blarg_helicopter_motion_tilt(moby);
            if (fast_dec_timer_s16(&blarg_helicopter_vars->rocket_fire_timer_frames)) {
                ++blarg_helicopter_vars->rockets_fired;
                moby_point_position(moby, blarg_helicopter_vars->muzzle_index, &position);
                blarg_helicopter_vars->muzzle_index = blarg_helicopter_vars->muzzle_index == 0;
                qcopy(&aim, target + 8);
                if (blarg_helicopter_vars->unkC0) {
                    qcopy(&aim, D_L12_00167240);
                    aim.f[2] += random_float_between(-0.5f, 0.1f);
                    aim.f[0] += randf_sym(0.0f, 0.4f);
                }
                fast_vec_sub(&delta, &aim, &position);
                if (blarg_helicopter_vars->unkC0)
                    fast_vec_normalize(&delta, &delta, frame_time * 22.0f);
                else
                    fast_vec_normalize(&delta, &delta, frame_time * 15.0f);
                angle = fast_arc_tan(delta.f[0], delta.f[1]);
                spread = 1.3f;
                a = scale_game_frames(130);
                spawn_rocket(&delta, &position, (int)moby, a, angle, frame_time * 15.0f, 0.0f);
                allocate_voice_for_target_entry(0, 0, moby);
                fast_vec_normalize(&delta, &delta, 0.5f);
                fast_vec_add(&position, &position, &delta);
                for (count = 24; count >= 0; --count) {
                    velocity.f[0] = randf_sym(0.0f, frame_time * spread);
                    velocity.f[1] = randf_sym(0.0f, frame_time * spread);
                    velocity.f[2] = randf_sym(0.0f, frame_time * spread);
                    a = FUN_L00_00257b90(170, 240);
                    b = FUN_L00_00257b90(110, 180);
                    color = (b << 8) | ((long)a | 0x7f000000L);
                    a = FUN_L00_00257b90(30, 70);
                    life = scale_game_frames(a);
                    size = FUN_L00_00257b90(120, 170);
                    spawn_effect(&position, &velocity, color, 255, life, 30, size, 1);
                }
                limit = 4;
                if (blarg_helicopter_vars->unkC0) {
                    if (D_L12_00161F20 > 10)
                        limit = 2;
                    if (D_L12_00161F20 > 6)
                        limit = 3;
                }
                if (blarg_helicopter_vars->rockets_fired >= limit &&
                    blarg_helicopter_vars->endpoint_path_index != -1) {
                    blarg_helicopter_vars->rockets_fired = 0;
                    moby->state = BLARG_HELICOPTER_MOVE_BETWEEN_PATH_ENDPOINTS;
                } else
                    blarg_helicopter_vars->rocket_fire_timer_frames = scale_game_frames(40);
            }
        }
        break;
    }
    case BLARG_HELICOPTER_MOVE_BETWEEN_PATH_ENDPOINTS:
        angle = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
        moby->rot.z = spring_turn(&blarg_helicopter_vars->yaw_turn_velocity,
                                  moby->rot.z,
                                  angle,
                                  0.004f,
                                  0.3f,
                                  frame_time * 0.7853982f);
        {
            char *path = ((char *)l12_paths[blarg_helicopter_vars->endpoint_path_index]);
            if (blarg_helicopter_vars->target_first_path_endpoint != 0)
                fast_vec_sub(&position, path + 16, &moby->pos);
            else
                fast_vec_sub(&position, path + *(int *)path * 16, &moby->pos);
        }
        distance = fast_vec_length(&position);
        if (!(distance > 5.5f)) {
            approach_value(&blarg_helicopter_vars->speed, 0.0f, frame_time_sq * 9.0f);
            if (blarg_helicopter_vars->speed == 0.0f) {
                blarg_helicopter_vars->rocket_fire_timer_frames = scale_game_frames(50);
                blarg_helicopter_vars->target_first_path_endpoint =
                    blarg_helicopter_vars->target_first_path_endpoint == 0;
                moby->state = BLARG_HELICOPTER_AIM_AND_FIRE;
                break;
            }
        } else
            approach_value(&blarg_helicopter_vars->speed, frame_time * 10.0f, frame_time_sq * 7.0f);
        fast_vec_normalize(
            blarg_helicopter_vars->velocity, &position, blarg_helicopter_vars->speed);
        fast_vec_add(&moby->pos, &moby->pos, blarg_helicopter_vars->velocity);
        update_blarg_helicopter_motion_tilt(moby);
        break;
    case BLARG_HELICOPTER_KNOCKBACK:
        if (fast_dec_timer_s16(&blarg_helicopter_vars->knockback_timer_frames)) {
            moby->state = moby->command;
        } else {
            value = blarg_helicopter_vars->speed - frame_time_sq * 4.0f;
            blarg_helicopter_vars->speed = value;
            if (value < 0.0f)
                blarg_helicopter_vars->speed = 0.0f;
            value = fast_cos(moby->rot.z);
            position.f[0] = value * -blarg_helicopter_vars->speed;
            value = fast_sin(moby->rot.z);
            position.f[1] = value * -blarg_helicopter_vars->speed;
            position.f[2] = 0.0f;
            fast_vec_add(&moby->pos, &moby->pos, &position);
        }
        break;
    case BLARG_HELICOPTER_EXPLODE_AND_REMOVE_OR_RESET: {
        int index;
        if (l12_camera.focus_moby == moby)
            l12_camera.focus_moby = 0;
        if (blarg_helicopter_vars->commander) {
            start_blarg_commander_knockback(moby, blarg_helicopter_vars->commander);
            blarg_helicopter_vars->commander = 0;
        }
        spawn_explosion(moby,
                        ((char *)&D_L12_0015F580_gp),
                        &moby->pos,
                        0.0f,
                        0.0f,
                        20,
                        3,
                        4,
                        4.0f,
                        2.0f,
                        100000.0f,
                        2,
                        3.0f,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        if (blarg_helicopter_vars->unkC0 == 0)
            FUN_L00_00257470(moby, 0, -1);
        index = blarg_helicopter_vars->voice;
        if (index != -1 &&
            (void *)((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.owner == (void *)moby &&
            ((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.state)
            release_voice_slot(index);
        blarg_helicopter_vars->voice = -1;
        if (blarg_helicopter_vars->unkC0 == 0) {
        remove:
            mark_moby_for_removal(moby);
            return;
        }
        a = scale_game_frames(240);
        b = scale_game_frames(400);
        blarg_helicopter_vars->activation_timer = FUN_L00_00257b90(a, b);
        moby->collision = 0;
        moby->state = BLARG_HELICOPTER_INITIALIZE;
        moby->flags = (moby->flags | 0x41) & 0xefff;
        break;
    }
    }
    moby->pos.z -= fast_cos(blarg_helicopter_vars->bob_angle) * 0.17f;
    a = scale_game_frames(120);
    angle = fast_add_rotations(blarg_helicopter_vars->bob_angle, 6.2831855f / (float)a);
    blarg_helicopter_vars->bob_angle = angle;
    moby->pos.z += fast_cos(angle) * 0.17f;
    {
        struct Moby *child = blarg_helicopter_vars->commander;
        if (child) {
            *(OvlQuad *)&child->pos = *(OvlQuad *)&moby->pos;
            child = blarg_helicopter_vars->commander;
            *(OvlQuad *)&child->rot = *(OvlQuad *)&moby->rot;
            advance_moby_animation(blarg_helicopter_vars->commander);
            FUN_L00_00250df8_x(blarg_helicopter_vars->commander);
            blarg_helicopter_vars->commander->flags |= 6;
        }
    }
    if (blarg_helicopter_vars->unkC0 == 0 && moby->state > BLARG_HELICOPTER_WAIT_FOR_ACTIVATION &&
        moby->visible && fast_vec_dist(&moby->pos, D_L12_00167240) < 30.0f) {
        FUN_L00_0025a120((char *)moby);
        moby->shadow_range = 24;
    }
}

void update_blarg_helicopter_motion_tilt(struct Moby *moby) {
    BlargHelicopterVars *blarg_helicopter_motion_vars = (BlargHelicopterVars *)moby->pvars;
    float ang = fast_subtract_rotations(fast_arc_tan(blarg_helicopter_motion_vars->velocity[0],
                                                     blarg_helicopter_motion_vars->velocity[1]),
                                        moby->rot.z);
    float target_rotation_y =
        blarg_helicopter_motion_vars->speed * 0.2268928f * fast_cos(ang) / (frame_time * 10.0f);
    float target_rotation_x =
        blarg_helicopter_motion_vars->speed * -0.2268928f * fast_sin(ang) / (frame_time * 10.0f);
    float updated_rotation_y, k0, rotation_y_damping, rotation_y_step_limit, k1, rotation_x_damping,
        rotation_x_step_limit;
    k0 = frame_time_sq * 0.5235988f;
    rotation_y_damping = frame_time_sq * 1.0471976f;
    rotation_y_step_limit = frame_time * 0.7853982f;
    updated_rotation_y = spring_turn(&blarg_helicopter_motion_vars->pitch_turn_velocity,
                                     moby->rot.y,
                                     target_rotation_y,
                                     k0,
                                     rotation_y_damping,
                                     rotation_y_step_limit);
    k1 = frame_time_sq * 1.0471976f;
    rotation_x_damping = frame_time_sq * 2.0943952f;
    rotation_x_step_limit = frame_time * 1.2217305f;
    moby->rot.y = updated_rotation_y;
    moby->rot.x = spring_turn(&blarg_helicopter_motion_vars->roll_turn_velocity,
                              moby->rot.x,
                              target_rotation_x,
                              k1,
                              rotation_x_damping,
                              rotation_x_step_limit);
}

extern void ahead_f(char *, float *, float) __asm__("FUN_L01_00274b78");

typedef struct {
    char  pad[0x48];
    float rz;
} M_af68;

typedef struct {
    float x, y, z, w;
} N_af68;

/* Follows a path: steers toward the current node, slows to a stop near the end, returns 1 once stopped. */
int follow_blarg_helicopter_path(struct Moby *moby, int path_index) {
    char                *path = ((char *)l12_paths[path_index]);
    BlargHelicopterVars *moby_vars = (BlargHelicopterVars *)moby->pvars;
    float                sum = 0.0f;
    float
        target_record_or_node_offset[20]; /* target record from the finder; also the step vector */
    int i;
    float target_heading;
    for (i = moby_vars->path_point; i < *(int *)path - 1; i++)
        sum += *(float *)(path + i * 16 + 0x1C);
    if (sum > 5.0f) {
        target_heading =
            fast_arc_tan(*(float *)((moby_vars->path_point << 4) + path + 0x10) - moby->pos.x,
                         *(float *)((moby_vars->path_point << 4) + path + 0x14) - moby->pos.y);
        ((M_af68 *)moby)->rz = spring_turn(&moby_vars->yaw_turn_velocity,
                                           ((M_af68 *)moby)->rz,
                                           target_heading,
                                           0.004f,
                                           0.3f,
                                           frame_time * 0.7853982f);
        approach_value(&moby_vars->speed, frame_time * 10.0f, frame_time_sq * 7.0f);
    } else {
        ahead_f(moby, target_record_or_node_offset, 48.0f);
        target_heading = fast_arc_tan(target_record_or_node_offset[0] - moby->pos.x,
                                      target_record_or_node_offset[1] - moby->pos.y);
        ((M_af68 *)moby)->rz = spring_turn(&moby_vars->yaw_turn_velocity,
                                           ((M_af68 *)moby)->rz,
                                           target_heading,
                                           0.004f,
                                           0.3f,
                                           frame_time * 0.7853982f);
        approach_value(&moby_vars->speed, 0.0f, frame_time_sq * 9.0f);
        sum = moby_vars->speed;
        if (sum == 0.0f)
            return 1;
    }
    fast_vec_sub(
        target_record_or_node_offset, &((N_af68 *)path)[moby_vars->path_point + 1], &moby->pos);
    fast_vec_normalize(moby_vars->velocity, target_record_or_node_offset, moby_vars->speed);
    fast_vec_add(&moby->pos, &moby->pos, moby_vars->velocity);
    if (fast_vec_dist(&moby->pos, &((N_af68 *)path)[moby_vars->path_point + 1]) < 2.0f) {
        (moby_vars->path_point)++;
        if (*(int *)path - 1 < moby_vars->path_point)
            moby_vars->path_point = *(int *)path - 1;
    }
    update_blarg_helicopter_motion_tilt(moby);
    return 0;
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002EC5A0), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern void start_hit_color_flash(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void update_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d538");

void process_blarg_helicopter_hits(struct Moby *moby) {
    enum {
        HIT_PROCESSING_SKIP_HIT = 1
    };
    int                  hit_processing_status;
    float                fv;
    int                  hit_reaction_code;
    BlargHelicopterVars *blarg_helicopter_hit_vars;
    char                *hit_record;
    float hit_to_moby_angle, hit_yaw_offset, knockback_rotation_y, knockback_rotation_x;
    blarg_helicopter_hit_vars = (char *)moby->pvars;
    fv = 0.0f;
    hit_record = find_moby_hit(moby, 0x330000, 0);
    hit_reaction_code = hit_reaction(
        moby, hit_record, &blarg_helicopter_hit_vars->health, 0, &hit_processing_status, &fv, 0, 4);
    if (hit_processing_status != HIT_PROCESSING_SKIP_HIT &&
        moby->state != BLARG_HELICOPTER_EXPLODE_AND_REMOVE_OR_RESET &&
        *(short *)(*(char **)(hit_record + 0x20) + 0xA6) != 0xB8 &&
        *(short *)(*(char **)(hit_record + 0x20) + 0xA6) != 0x199) {
        float remaining_health;
        if (*(short *)(*(char **)(hit_record + 0x20) + 0xA6) == 0x1CA)
            fv = 10.0f;
        remaining_health = blarg_helicopter_hit_vars->health - fv;
        blarg_helicopter_hit_vars->health = remaining_health;
        if (hit_reaction_code == 1 || remaining_health <= 0.0f) {
            allocate_voice_for_target_entry(2, 0, moby);
            moby->state = BLARG_HELICOPTER_EXPLODE_AND_REMOVE_OR_RESET;
        } else if (hit_reaction_code != 0xB) {
            blarg_helicopter_hit_vars->hit_flash.flash_red = 0xB4;
            start_hit_color_flash(moby, &blarg_helicopter_hit_vars->hit_flash);
            if (*(float *)(hit_record + 0x1C) == 5627.925f &&
                moby->state != BLARG_HELICOPTER_KNOCKBACK) {
                hit_to_moby_angle = fast_arc_tan(moby->pos.x - *(float *)(hit_record + 0),
                                                 moby->pos.y - *(float *)(hit_record + 4));
                hit_yaw_offset = fast_subtract_rotations(hit_to_moby_angle, moby->rot.z);
                knockback_rotation_y = fast_cos(hit_yaw_offset) * 0.08726646f;
                knockback_rotation_x = fast_sin(hit_yaw_offset) * -0.34906584f;
                if (absolute_float(moby->rot.x) < absolute_float(knockback_rotation_x)) {
                    moby->rot.x = knockback_rotation_x;
                    blarg_helicopter_hit_vars->roll_turn_velocity = 0.0f;
                }
                if (absolute_float(moby->rot.y) < absolute_float(knockback_rotation_y)) {
                    moby->rot.y = knockback_rotation_y;
                    blarg_helicopter_hit_vars->pitch_turn_velocity = 0.0f;
                }
                blarg_helicopter_hit_vars->speed = frame_time * 4.0f;
                blarg_helicopter_hit_vars->knockback_timer_frames = scale_game_frames(0x3C);
                moby->command = moby->state;
                moby->state = BLARG_HELICOPTER_KNOCKBACK;
                allocate_voice_for_target_entry(1, 0, moby);
            }
        }
    }
    moby->hit_slot = 0xFF;
    update_hit_color_flash(moby, &blarg_helicopter_hit_vars->hit_flash);
}
