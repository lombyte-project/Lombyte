/* Level 12 moby class blarg_helicopter: path following, tilt and hits. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"

extern char D_L12_00167240[];

#include "rnc/overlay/level_path.h"

extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");

#include "sda.h"

#include "qcopy.h"

extern int scale_game_frames(int);

#include "rnc/overlay/quad.h"

typedef struct Moby Moby;
int                 FUN_L00_00257b90(int lo, int hi);
void                FUN_L00_0025a120(char *o);

#include "rnc/overlay/level_globals.h"

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

void blarg_helicopter_update(struct Moby *moby) {
    enum {
        BLARG_HELICOPTER_INITIALIZE = 0,
        BLARG_HELICOPTER_WAIT_FOR_ACTIVATION = 1,
        BLARG_HELICOPTER_FOLLOW_INITIAL_PATH = 2,
        BLARG_HELICOPTER_AIM_AND_FIRE = 3,
        BLARG_HELICOPTER_MOVE_BETWEEN_PATH_ENDPOINTS = 4,
        BLARG_HELICOPTER_KNOCKBACK = 5,
        BLARG_HELICOPTER_EXPLODE_AND_REMOVE_OR_RESET = 6
    };
    char        *data = (char *)moby->pvars;
    struct Moby *child;
    float        target[20] __attribute__((aligned(16)));
    OvlVec4      position, delta, aim, velocity;
    int          index, count, limit, a, b, color, life, size, triggered;
    float        distance, angle, value, spread;
    if (D_L12_0015F5C4 == 2)
        return;
    find_point_ahead(moby, target, 70.0f);
    child = *(struct Moby **)(data + 0xac);
    if (child != 0 && (signed char)child->state >= 0)
        FUN_L00_00263ac8(child, 0, data + 0xd0, 2.7f);
    process_blarg_helicopter_hits(moby);
    if (D_001413F4 == 2) {
        moby->scale = moby->pclass->scale * 0.5f;
        {
            struct Moby *child = *(struct Moby **)(data + 0xac);
            if (child != 0)
                child->scale = child->pclass->scale * 0.5f;
        }
    } else {
        moby->scale = moby->pclass->scale;
        {
            struct Moby *child = *(struct Moby **)(data + 0xac);
            if (child != 0)
                child->scale = child->pclass->scale;
        }
    }
    if ((unsigned int)(moby->state - 2) < 4) {
        if (*(int *)(data + 0xb0) == -1)
            *(int *)(data + 0xb0) = allocate_voice_for_target_entry(3, 4, moby);
    } else {
        int index = *(int *)(data + 0xb0);
        if (index != -1 &&
            (void *)((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.owner == (void *)moby &&
            ((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.state)
            release_voice_slot(index);
        *(int *)(data + 0xb0) = -1;
    }
    index = *(int *)(data + 0xc0);
    if (current_level_index == 12 && D_L12_00161F00 != 0) {
        if (index == 0) {
            if (l12_camera.focus_moby == moby)
                l12_camera.focus_moby = 0;
            if (!moby->visible) {
                if (*(struct Moby **)(data + 0xac)) {
                    mark_moby_for_removal(*(struct Moby **)(data + 0xac));
                    *(struct Moby **)(data + 0xac) = 0;
                }
                mark_moby_for_removal(moby);
                return;
            }
            index = *(int *)(data + 0xc0);
            goto check_mode;
        }
    } else {
    check_mode:
        if (index == 0)
            goto update_state;
    }
    if (visited52[moby->mission + current_level_index * 16] == 255) {
        if (*(struct Moby **)(data + 0xac)) {
            mark_moby_for_removal(*(struct Moby **)(data + 0xac));
            *(struct Moby **)(data + 0xac) = 0;
        }
        mark_moby_for_removal(moby);
        return;
    }
    if (index == 0)
        goto update_state;
    if (D_L12_001619A0 != 0) {
        if (*(struct Moby **)(data + 0xac)) {
            start_blarg_commander_knockback(moby, *(struct Moby **)(data + 0xac));
            *(struct Moby **)(data + 0xac) = 0;
        }
        mark_moby_for_removal(moby);
        return;
    }
update_state:
    switch (moby->state) {
    case BLARG_HELICOPTER_INITIALIZE: {
        int a, b, limit;
        *(short *)(data + 0x24) = 6;
        *(float *)(data + 0x20) = 6.0f;
        data[0x28] = 2;
        data[0x29] = 0;
        *(float *)(data + 0x94) = 0.0f;
        *(float *)(data + 0x98) = 0.0f;
        *(float *)(data + 0x9c) = 0.0f;
        moby->collision = 0;
        moby->flags = (moby->flags | 0x41) & 0xefff;
        cache_consecutive_point_distances(*(int *)(data + 0xa0));
        cache_consecutive_point_distances(*(int *)(data + 0xa0));
        cache_consecutive_point_distances(*(int *)(data + 0xa4));
        if (moby->draw_dist < 128)
            moby->draw_dist = 128;
        if (*(struct Moby **)(data + 0xac) == 0) {
            *(struct Moby **)(data + 0xac) = create_class_55b_moby_from_source(moby);
            (*(struct Moby **)(data + 0xac))->update_dist = 0;
            (*(struct Moby **)(data + 0xac))->flags |= 0x41;
        }
        qcopy(&moby->pos, ((char *)l12_paths[*(int *)(data + 0xa0)]) + 0x10);
        if (*(int *)(data + 0xc0) != 0 && *(int *)(data + 0xc4) == 0) {
            a = scale_game_frames(240);
            b = scale_game_frames(400);
            a = FUN_L00_00257b90(a, b);
            {
                int limit = D_L12_00161F20 - 1;
                *(int *)(data + 0xc4) = a;
                if (limit > 10)
                    limit = 10;
                if (limit < 0)
                    limit = 0;
                *(int *)(data + 0xc4) += limit * scale_game_frames(35);
            }
        }
        *(int *)(data + 0xb4) = 0;
        moby->state = 1;
        break;
    }
    case BLARG_HELICOPTER_WAIT_FOR_ACTIVATION: {
        int          triggered, a;
        struct Moby *child;
        triggered = 0;
        if (*(int *)(data + 0xc0) && D_L12_00161F00)
            triggered = fast_dec_timer((int *)(data + 0xc4)) != 0;
        if ((*(int *)(data + 0x80) != -1 &&
             is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0x80))) ||
            triggered) {
            data[0x58] = 22;
            data[0x59] = 1;
            data[0x5a] = 7;
            moby->hit_slot = 255;
            moby->flags &= 0xffbe;
            child = *(struct Moby **)(data + 0xac);
            if (child)
                child->flags &= 0xffbe;
            moby->flags |= 0x1000;
            a = moby->pclass->collision;
            moby->state = 2;
            moby->collision = a;
        }
        break;
    }
    case BLARG_HELICOPTER_FOLLOW_INITIAL_PATH:
        if (*(int *)(data + 0xc0) == 0 && l12_camera.focus_moby == 0) {
            l12_camera.focus_moby = moby;
            l12_camera.focus_time = 0;
        }
        if (*(int *)(data + 0xc0)) {
            count = *(short *)(data + 0x8c);
            limit = D_L12_00161F20 - 3;
            if (limit > 5)
                limit = 5;
            if (limit < 0)
                limit = 0;
            if (count < limit * scale_game_frames(15))
                *(short *)(data + 0x8c) = limit * scale_game_frames(15);
        }
        if (follow_blarg_helicopter_path(moby, *(int *)(data + 0xa0)))
            moby->state = 3;
        break;
    case BLARG_HELICOPTER_AIM_AND_FIRE: {
        int   count, limit, a, b, color, life, size;
        float distance, angle, value, spread;
        if (current_level_index == 12 && !D_0013D4CB && !D_00141C50)
            help_request(0x2ee2, 0x5d);
        distance = vector_distance_xy(&moby->pos, &hero.motion.pos);
        if (*(int *)(data + 0xc0) == 0) {
            if (distance < 21.0f && moby->pos.z - hero.motion.pos.f[2] > 1.0f) {
                if (l12_camera.focus_moby == 0) {
                    l12_camera.focus_moby = moby;
                    l12_camera.focus_time = 0;
                }
            } else if (l12_camera.focus_moby == moby)
                l12_camera.focus_moby = 0;
        }
        distance = vector_distance_xy(&moby->pos, target);
        if ((distance < 40.0f && moby->pos.z - target[2] > -2.0f) || *(int *)(data + 0xc0)) {
            angle = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
            moby->rot.z = spring_turn(
                (float *)(data + 0x88), moby->rot.z, angle, 0.004f, 0.3f, frame_time * 0.7853982f);
            value = vector_distance_xy(target, &moby->pos);
            angle = fast_arc_tan(value, moby->pos.z - target[2]);
            moby->rot.y = spring_turn(
                (float *)(data + 0x9c), moby->rot.y, angle, 0.004f, 0.3f, frame_time * 0.7853982f);
            clear_u64_value(data + 0x70);
            update_blarg_helicopter_motion_tilt(moby);
            if (fast_dec_timer_s16(data + 0x8c)) {
                ++*(short *)(data + 0xbc);
                moby_point_position(moby, *(int *)(data + 0xb8), &position);
                *(int *)(data + 0xb8) = *(int *)(data + 0xb8) == 0;
                qcopy(&aim, target + 8);
                if (*(int *)(data + 0xc0)) {
                    qcopy(&aim, D_L12_00167240);
                    aim.f[2] += random_float_between(-0.5f, 0.1f);
                    aim.f[0] += randf_sym(0.0f, 0.4f);
                }
                fast_vec_sub(&delta, &aim, &position);
                if (*(int *)(data + 0xc0))
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
                if (*(int *)(data + 0xc0)) {
                    if (D_L12_00161F20 > 10)
                        limit = 2;
                    if (D_L12_00161F20 > 6)
                        limit = 3;
                }
                if (*(short *)(data + 0xbc) >= limit && *(int *)(data + 0xa4) != -1) {
                    *(short *)(data + 0xbc) = 0;
                    moby->state = 4;
                } else
                    *(short *)(data + 0x8c) = scale_game_frames(40);
            }
        }
        break;
    }
    case BLARG_HELICOPTER_MOVE_BETWEEN_PATH_ENDPOINTS:
        angle = fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y);
        moby->rot.z = spring_turn(
            (float *)(data + 0x88), moby->rot.z, angle, 0.004f, 0.3f, frame_time * 0.7853982f);
        {
            char *path = ((char *)l12_paths[*(int *)(data + 0xa4)]);
            if (*(short *)(data + 0xbe) != 0)
                fast_vec_sub(&position, path + 16, &moby->pos);
            else
                fast_vec_sub(&position, path + *(int *)path * 16, &moby->pos);
        }
        distance = fast_vec_length(&position);
        if (!(distance > 5.5f)) {
            approach_value(data + 0x94, 0.0f, frame_time_sq * 9.0f);
            if (*(float *)(data + 0x94) == 0.0f) {
                *(short *)(data + 0x8c) = scale_game_frames(50);
                *(short *)(data + 0xbe) = *(short *)(data + 0xbe) == 0;
                moby->state = 3;
                break;
            }
        } else
            approach_value(data + 0x94, frame_time * 10.0f, frame_time_sq * 7.0f);
        fast_vec_normalize(data + 0x70, &position, *(float *)(data + 0x94));
        fast_vec_add(&moby->pos, &moby->pos, data + 0x70);
        update_blarg_helicopter_motion_tilt(moby);
        break;
    case BLARG_HELICOPTER_KNOCKBACK:
        if (fast_dec_timer_s16(data + 0x8e)) {
            moby->state = moby->command;
        } else {
            value = *(float *)(data + 0x94) - frame_time_sq * 4.0f;
            *(float *)(data + 0x94) = value;
            if (value < 0.0f)
                *(float *)(data + 0x94) = 0.0f;
            value = fast_cos(moby->rot.z);
            position.f[0] = value * -*(float *)(data + 0x94);
            value = fast_sin(moby->rot.z);
            position.f[1] = value * -*(float *)(data + 0x94);
            position.f[2] = 0.0f;
            fast_vec_add(&moby->pos, &moby->pos, &position);
        }
        break;
    case BLARG_HELICOPTER_EXPLODE_AND_REMOVE_OR_RESET: {
        int index;
        if (l12_camera.focus_moby == moby)
            l12_camera.focus_moby = 0;
        if (*(struct Moby **)(data + 0xac)) {
            start_blarg_commander_knockback(moby, *(struct Moby **)(data + 0xac));
            *(struct Moby **)(data + 0xac) = 0;
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
        if (*(int *)(data + 0xc0) == 0)
            FUN_L00_00257470(moby, 0, -1);
        index = *(int *)(data + 0xb0);
        if (index != -1 &&
            (void *)((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.owner == (void *)moby &&
            ((VoicePoolWindow *)(D_0013E550 + index * 0x70))->voice.state)
            release_voice_slot(index);
        *(int *)(data + 0xb0) = -1;
        if (*(int *)(data + 0xc0) == 0) {
        remove:
            mark_moby_for_removal(moby);
            return;
        }
        a = scale_game_frames(240);
        b = scale_game_frames(400);
        *(int *)(data + 0xc4) = FUN_L00_00257b90(a, b);
        moby->collision = 0;
        moby->state = 0;
        moby->flags = (moby->flags | 0x41) & 0xefff;
        break;
    }
    }
    moby->pos.z -= fast_cos(*(float *)(data + 0x90)) * 0.17f;
    a = scale_game_frames(120);
    angle = fast_add_rotations(*(float *)(data + 0x90), 6.2831855f / (float)a);
    *(float *)(data + 0x90) = angle;
    moby->pos.z += fast_cos(angle) * 0.17f;
    {
        struct Moby *child = *(struct Moby **)(data + 0xac);
        if (child) {
            *(OvlQuad *)&child->pos = *(OvlQuad *)&moby->pos;
            child = *(struct Moby **)(data + 0xac);
            *(OvlQuad *)&child->rot = *(OvlQuad *)&moby->rot;
            advance_moby_animation(*(struct Moby **)(data + 0xac));
            FUN_L00_00250df8_x(*(struct Moby **)(data + 0xac));
            (*(struct Moby **)(data + 0xac))->flags |= 6;
        }
    }
    if (*(int *)(data + 0xc0) == 0 && moby->state > 1 && moby->visible &&
        fast_vec_dist(&moby->pos, D_L12_00167240) < 30.0f) {
        FUN_L00_0025a120((char *)moby);
        moby->shadow_range = 24;
    }
}

void update_blarg_helicopter_motion_tilt(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float ang = fast_subtract_rotations(
        fast_arc_tan(*(float *)(data + 0x70), *(float *)(data + 0x74)), moby->rot.z);
    float target_rotation_y =
        *(float *)(data + 0x94) * 0.2268928f * fast_cos(ang) / (frame_time * 10.0f);
    float target_rotation_x =
        *(float *)(data + 0x94) * -0.2268928f * fast_sin(ang) / (frame_time * 10.0f);
    float updated_rotation_y, k0, rotation_y_damping, rotation_y_step_limit, k1, rotation_x_damping,
        rotation_x_step_limit;
    k0 = frame_time_sq * 0.5235988f;
    rotation_y_damping = frame_time_sq * 1.0471976f;
    rotation_y_step_limit = frame_time * 0.7853982f;
    updated_rotation_y = spring_turn((float *)(data + 0x9C),
                                     moby->rot.y,
                                     target_rotation_y,
                                     k0,
                                     rotation_y_damping,
                                     rotation_y_step_limit);
    k1 = frame_time_sq * 1.0471976f;
    rotation_x_damping = frame_time_sq * 2.0943952f;
    rotation_x_step_limit = frame_time * 1.2217305f;
    moby->rot.y = updated_rotation_y;
    moby->rot.x = spring_turn((float *)(data + 0x98),
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
    char *path = ((char *)l12_paths[path_index]);
    char *moby_vars = (char *)moby->pvars;
    float sum = 0.0f;
    float
        target_record_or_node_offset[20]; /* target record from the finder; also the step vector */
    int i;
    float target_heading;
    for (i = *(int *)(moby_vars + 0xB4); i < *(int *)path - 1; i++)
        sum += *(float *)(path + i * 16 + 0x1C);
    if (sum > 5.0f) {
        target_heading =
            fast_arc_tan(*(float *)((*(int *)(moby_vars + 0xB4) << 4) + path + 0x10) - moby->pos.x,
                         *(float *)((*(int *)(moby_vars + 0xB4) << 4) + path + 0x14) - moby->pos.y);
        ((M_af68 *)moby)->rz = spring_turn((float *)(moby_vars + 0x88),
                                           ((M_af68 *)moby)->rz,
                                           target_heading,
                                           0.004f,
                                           0.3f,
                                           frame_time * 0.7853982f);
        approach_value(moby_vars + 0x94, frame_time * 10.0f, frame_time_sq * 7.0f);
    } else {
        ahead_f(moby, target_record_or_node_offset, 48.0f);
        target_heading = fast_arc_tan(target_record_or_node_offset[0] - moby->pos.x,
                                      target_record_or_node_offset[1] - moby->pos.y);
        ((M_af68 *)moby)->rz = spring_turn((float *)(moby_vars + 0x88),
                                           ((M_af68 *)moby)->rz,
                                           target_heading,
                                           0.004f,
                                           0.3f,
                                           frame_time * 0.7853982f);
        approach_value(moby_vars + 0x94, 0.0f, frame_time_sq * 9.0f);
        sum = *(float *)(moby_vars + 0x94);
        if (sum == 0.0f)
            return 1;
    }
    fast_vec_sub(target_record_or_node_offset,
                 &((N_af68 *)path)[*(int *)(moby_vars + 0xB4) + 1],
                 &moby->pos);
    fast_vec_normalize(
        moby_vars + 0x70, target_record_or_node_offset, *(float *)(moby_vars + 0x94));
    fast_vec_add(&moby->pos, &moby->pos, moby_vars + 0x70);
    if (fast_vec_dist(&moby->pos, &((N_af68 *)path)[*(int *)(moby_vars + 0xB4) + 1]) < 2.0f) {
        (*(int *)(moby_vars + 0xB4))++;
        if (*(int *)path - 1 < *(int *)(moby_vars + 0xB4))
            *(int *)(moby_vars + 0xB4) = *(int *)path - 1;
    }
    update_blarg_helicopter_motion_tilt(moby);
    return 0;
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002C0310.c: func_L12_002EC5A0), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern void trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

void process_blarg_helicopter_hits(struct Moby *moby) {
    int   hit_processing_status;
    float fv;
    int   hit_reaction_code;
    char *vars;
    char *hit_record;
    float hit_to_moby_angle, hit_yaw_offset, knockback_rotation_y, knockback_rotation_x;
    vars = (char *)moby->pvars;
    fv = 0.0f;
    hit_record = find_moby_hit(moby, 0x330000, 0);
    hit_reaction_code =
        hit_reaction(moby, hit_record, vars + 0x20, 0, &hit_processing_status, &fv, 0, 4);
    if (hit_processing_status != 1 && moby->state != 6 &&
        *(short *)(*(char **)(hit_record + 0x20) + 0xA6) != 0xB8 &&
        *(short *)(*(char **)(hit_record + 0x20) + 0xA6) != 0x199) {
        float remaining_health;
        if (*(short *)(*(char **)(hit_record + 0x20) + 0xA6) == 0x1CA)
            fv = 10.0f;
        remaining_health = *(float *)(vars + 0x20) - fv;
        *(float *)(vars + 0x20) = remaining_health;
        if (hit_reaction_code == 1 || remaining_health <= 0.0f) {
            allocate_voice_for_target_entry(2, 0, moby);
            moby->state = 6;
        } else if (hit_reaction_code != 0xB) {
            *(unsigned char *)(vars + 0x67) = 0xB4;
            trooper_anim_update(moby, (short *)(vars + 0x60));
            if (*(float *)(hit_record + 0x1C) == 5627.925f && moby->state != 5) {
                hit_to_moby_angle = fast_arc_tan(moby->pos.x - *(float *)(hit_record + 0),
                                                 moby->pos.y - *(float *)(hit_record + 4));
                hit_yaw_offset = fast_subtract_rotations(hit_to_moby_angle, moby->rot.z);
                knockback_rotation_y = fast_cos(hit_yaw_offset) * 0.08726646f;
                knockback_rotation_x = fast_sin(hit_yaw_offset) * -0.34906584f;
                if (absolute_float(moby->rot.x) < absolute_float(knockback_rotation_x)) {
                    moby->rot.x = knockback_rotation_x;
                    *(float *)(vars + 0x98) = 0.0f;
                }
                if (absolute_float(moby->rot.y) < absolute_float(knockback_rotation_y)) {
                    moby->rot.y = knockback_rotation_y;
                    *(float *)(vars + 0x9C) = 0.0f;
                }
                *(float *)(vars + 0x94) = frame_time * 4.0f;
                *(short *)(vars + 0x8E) = scale_game_frames(0x3C);
                moby->command = moby->state;
                moby->state = 5;
                allocate_voice_for_target_entry(1, 0, moby);
            }
        }
    }
    moby->hit_slot = 0xFF;
    trooper_anim_finish(moby, vars + 0x60);
}
