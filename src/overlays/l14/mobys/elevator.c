/* Level 14 elevator moby class: path state, travel, endpoints and idle glow. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/overlay/level_path.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/game/item_state.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/game/unlock_planet.h"

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern float FUN_L00_0025b8c0(float *p, float *v, float t, float u1, float u2, float eps);

extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

extern LevelSaveState D_L14_001BB930;

extern void vendor_volume(s32 voice, s32 volume) __asm__("FUN_L01_002a1968");

extern int vendor_ftoi(float) __asm__("FUN_001fa6d0");

extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");

extern float vendor_spring(float *p, float t, float *v, float u1, float u2, float eps) __asm__("FUN_L00_0025b8c0");

void initialize_elevator_path_state(struct Moby *moby) __asm__("FUN_L14_002eef60");

void initialize_elevator_path_state(struct Moby *m) __asm__("FUN_L14_002eef60");

int advance_elevator_along_path(struct Moby *elevator_moby) __asm__("FUN_L14_002ef150");

int advance_elevator_along_path(struct Moby *m) __asm__("FUN_L14_002ef150");

void settle_elevator_at_path_endpoint(struct Moby *elevator_moby) __asm__("FUN_L14_002ef3e0");

void settle_elevator_at_path_endpoint(struct Moby *m) __asm__("FUN_L14_002ef3e0");

void rebuild_elevator_world_path(u8 *moby) __asm__("FUN_L14_002ef630");

void update_elevator_idle_glow(struct Moby *moby) __asm__("FUN_L14_002ef6f0");

typedef struct {
    u8  pad0[0x60];
    u8  spring[0x40]; /* 0x60 */
    s32 path;         /* 0xA0 */
    s32 segment;      /* 0xA4 */
    f32 along;        /* 0xA8 */
    f32 dir;          /* 0xAC */
    s32 travelled;    /* 0xB0 */
    f32 lengthB4;     /* 0xB4 */
    s32 path2;        /* 0xB8 */
    f32 lengthBC;     /* 0xBC */
    f32 vel[4];       /* 0xC0 */
    f32 rot[4];       /* 0xD0 */
    u8  padE0[0x30];
    s32 item;         /* 0x110 */
    s32 target;       /* 0x114 */
    s32 clip_a;       /* 0x118 */
    s32 clip_b;       /* 0x11C */
    s32 clip_c;       /* 0x120 */
    s16 bought;       /* 0x124 */
    s16 level;        /* 0x126 */
    f32 level_vel;    /* 0x128 */
    s32 motor;        /* 0x12C */
    s32 motor_volume; /* 0x130 */
    s32 hum_volume;   /* 0x134 */
    s32 i138;         /* 0x138 */
} L14VendorVars;

void elevator_update(struct Moby *m) __asm__("FUN_L14_002ee8d8");

void elevator_update(struct Moby *elevator_moby) {
    enum {
        ELEVATOR_STATE_INITIALIZE = 0,
        ELEVATOR_STATE_WAIT_FOR_RIDER = 1,
        ELEVATOR_STATE_FOLLOW_PATH = 2,
        ELEVATOR_STATE_SETTLE_AT_ENDPOINT = 3,
        ELEVATOR_STATE_WAIT_FOR_RIDER_EXIT = 4
    };
    L14VendorVars *elevator_vars = (L14VendorVars *)elevator_moby->pvars;
    Vec4           old;
    Vec4           rot;
    Vec4           delta;
    float          level;

    if (elevator_moby->state != 0 && elevator_moby->command == 2 && elevator_vars->dir < 0.0f) {
        int *path = (int *)l14_paths[elevator_vars->path2];
        elevator_vars->dir = -elevator_vars->dir;
        elevator_vars->segment = *path - 2;
        elevator_vars->along = *(float *)((char *)path + (elevator_vars->segment << 4) + 0x1C);
        elevator_moby->command = 1;
    }
    qcopy(&old, &elevator_moby->pos);
    qcopy(&rot, &elevator_moby->rot);
    switch (elevator_moby->state) {
    case ELEVATOR_STATE_INITIALIZE: {
        u16 id;
        elevator_moby->command = 0;
        elevator_moby->state = 1;
        initialize_elevator_path_state(elevator_moby);
        qcopy(&elevator_moby->pos, ((char *)l14_paths[elevator_vars->path]) + 0x10);
        qcopy(&elevator_moby->rot, elevator_vars->rot);
        id = elevator_moby->save_id;
        if (D_L14_001BB930.cleared[(short)id] == 0 &&
            (((D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 0x1F)) ^ 1) & 1) &&
            visited52[elevator_vars->item + current_level_index * 16] == 0xFF)
            elevator_vars->bought = 1;
        else
            elevator_vars->bought = 0;
        elevator_moby->update_dist = 0xFF;
        elevator_moby->glow_color = 0x80303030;
        elevator_vars->motor = -1;
        elevator_vars->motor_volume = -1;
        elevator_vars->hum_volume = -1;
        elevator_vars->i138 = 0;
        break;
    }
    case ELEVATOR_STATE_WAIT_FOR_RIDER:
        update_elevator_idle_glow(elevator_moby);
        if (elevator_vars->clip_a >= 0 &&
            is_point_inside_clip_volume(&hero.motion.pos, elevator_vars->clip_a) != 0) {
            elevator_moby->command = 0;
            elevator_vars->dir = -1.0f;
            elevator_vars->segment = 0;
            elevator_vars->along = 0;
        }
        if (elevator_vars->clip_b >= 0 &&
            is_point_inside_clip_volume(&hero.motion.pos, elevator_vars->clip_b) != 0) {
            char **e;
            elevator_moby->command = 0;
            e = ((char **)&l14_paths[elevator_vars->path]);
            elevator_vars->segment = **(int **)e - 2;
            elevator_vars->along = *(float *)(*e + (elevator_vars->segment << 4) + 0x1C);
            elevator_vars->dir = 1.0f;
        }
        if (elevator_vars->clip_c >= 0 &&
            is_point_inside_clip_volume(&hero.motion.pos, elevator_vars->clip_c) != 0) {
            char **e;
            elevator_moby->command = 1;
            e = ((char **)&l14_paths[elevator_vars->path2]);
            elevator_vars->segment = **(int **)e - 2;
            elevator_vars->along = *(float *)(*e + (elevator_vars->segment << 4) + 0x1C);
            elevator_vars->dir = 1.0f;
        }
        if (hero.ground_moby == elevator_moby && hero.frames_off_ground.s == 0 &&
            vector_distance_xy(&hero.motion.pos, &elevator_moby->pos) < 1.0f) {
            int voice = allocate_voice_for_target_entry(1, 0, elevator_moby);
            if (voice >= 0) {
                if (elevator_vars->hum_volume < 0)
                    elevator_vars->hum_volume = *(int *)(((char *)&voice_pool) + 0xF0);
                vendor_volume(voice, elevator_vars->hum_volume);
            }
            if (elevator_vars->dir == -1.0f) {
                if (elevator_vars->bought != 0)
                    elevator_moby->command = 2;
                else
                    elevator_moby->command = 0;
            }
            elevator_moby->state = 2;
            elevator_vars->travelled = 0;
            elevator_vars->dir = -elevator_vars->dir;
        }
        break;
    case ELEVATOR_STATE_FOLLOW_PATH:
        if (voice_is_playing(elevator_moby, elevator_vars->motor) == 0) {
            elevator_vars->motor = allocate_voice_for_target_entry(0, 4, elevator_moby);
            if (elevator_vars->motor != 0) {
                if (elevator_vars->motor_volume < 0)
                    elevator_vars->motor_volume = *(int *)(((char *)&voice_pool) + 0x80);
                vendor_volume(elevator_vars->motor, elevator_vars->motor_volume);
            }
        }
        elevator_moby->glow_color = fast_tween_color(elevator_moby->glow_color, 0x80808040, 0.1f);
        if (advance_elevator_along_path(elevator_moby) != 0) {
            elevator_moby->state = 3;
            elevator_vars->level = elevator_vars->motor_volume;
        }
        rebuild_elevator_world_path((u8 *)elevator_moby);
        fast_vec_sub(&delta, &elevator_moby->pos, &old);
        update_entity_matrix(elevator_vars->spring, &delta, &rot, &elevator_moby->rot);
        break;
    case ELEVATOR_STATE_SETTLE_AT_ENDPOINT:
        if (fast_vec_length(elevator_vars->vel) >= 0.001f) {
            if (voice_is_playing(elevator_moby, elevator_vars->motor) == 0)
                elevator_vars->motor = allocate_voice_for_target_entry(0, 4, elevator_moby);
            level = convert_integer_to_float(elevator_vars->level);
            vendor_spring(&level,
                          convert_integer_to_float(elevator_vars->motor_volume) * 0.25f,
                          &elevator_vars->level_vel,
                          0.003f,
                          0.2f,
                          0.0f);
            elevator_vars->level = vendor_ftoi(level);
            if (elevator_vars->motor >= 0)
                vendor_volume(elevator_vars->motor, elevator_vars->level);
            rebuild_elevator_world_path((u8 *)elevator_moby);
        } else {
            int voice;
            update_elevator_idle_glow(elevator_moby);
            if (elevator_vars->motor != -1) {
                char *e = ((char *)&voice_pool) + elevator_vars->motor * 0x70;
                if (*(struct Moby **)(e + 0x88) == elevator_moby && ((u8 *)e)[0x74] != 0)
                    release_voice_slot(elevator_vars->motor);
            }
            elevator_vars->motor = -1;
            voice = allocate_voice_for_target_entry(1, 0, elevator_moby);
            if (voice >= 0)
                vendor_volume(voice, elevator_vars->hum_volume / 3);
            elevator_moby->state = 4;
            if (hero.ground_moby != elevator_moby)
                elevator_moby->state = 1;
        }
        break;
    case ELEVATOR_STATE_WAIT_FOR_RIDER_EXIT:
        update_elevator_idle_glow(elevator_moby);
        if (hero.ground_moby != elevator_moby)
            elevator_moby->state = 1;
        break;
    }
    if (elevator_moby->state != 2 && elevator_moby->state != 0) {
        settle_elevator_at_path_endpoint(elevator_moby);
        if (hero.ground_moby == elevator_moby) {
            fast_vec_sub(&delta, &elevator_moby->pos, &old);
            update_entity_matrix(elevator_vars->spring, &delta, &rot, &elevator_moby->rot);
        }
    }
}

void initialize_elevator_path_state(struct Moby *moby) {
    char *moby_vars = (char *)moby->pvars;
    int  *path;
    int  *primary_path;
    int  *first;
    int   i;

    if (*(int *)(moby_vars + 0xA0) == -1) {
        mark_moby_for_removal(moby);
        return;
    }
    primary_path = (int *)l14_paths[*(int *)(moby_vars + 0xA0)];
    if (*primary_path == 0 || *(int *)(moby_vars + 0x114) == -1 ||
        *(int *)l14_paths[*(int *)(moby_vars + 0x114)] != 11) {
        mark_moby_for_removal(moby);
        return;
    }

    *(float *)(moby_vars + 0xB4) = 0.0f;
    path = primary_path;
    first = path + 4;
    for (i = 0; i < *path - 1; i++) {
        ((f32 *)path)[i * 4 + 7] = fast_vec_dist(&path[i * 4 + 4], &path[i * 4 + 8]);
        *(float *)(moby_vars + 0xB4) += ((f32 *)path)[i * 4 + 7];
    }
    {
        f32   len = fast_vec_dist(&((f32 *)primary_path)[i * 4 + 4], first);
        char *pt = (char *)primary_path + i * 16;
        *(f32 *)(pt + 0x1C) = len;
    }

    path = (int *)l14_paths[*(int *)(moby_vars + 0xB8)];
    *(float *)(moby_vars + 0xBC) = 0.0f;
    for (i = 0; i < *path - 1; i++) {
        ((f32 *)path)[i * 4 + 7] = fast_vec_dist(&path[i * 4 + 4], &path[i * 4 + 8]);
        *(float *)(moby_vars + 0xBC) += ((f32 *)path)[i * 4 + 7];
    }
    {
        f32   len = fast_vec_dist(&((f32 *)path)[i * 4 + 4], path + 4);
        char *pt = (char *)path + i * 16;
        *(f32 *)(pt + 0x1C) = len;
    }
    *(float *)(moby_vars + 0xAC) = -1.0f;
    *(int *)(moby_vars + 0xA8) = 0;
    *(int *)(moby_vars + 0xA4) = 0;
}

extern float interpolate_angle(float, float, float) __asm__("FUN_L00_0025b750");

extern int
FUN_L00_0025d808_2f05d8(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");

extern short elevator_path_step_rate_gp __asm__("D_L14_00161D14") __attribute__((sda));

int advance_elevator_along_path(struct Moby *elevator_moby) {
    char *moby_vars = (char *)elevator_moby->pvars;
    float rot[4];
    float pos[4];
    char *selected_path;
    float total;
    float step;
    float rotation_blend;
    float x_rotation_blend;
    int   path_endpoint_reached;
    if (elevator_moby->command == 0) {
        selected_path = ((char *)l14_paths[*(int *)(moby_vars + 0xA0)]);
        total = *(float *)(moby_vars + 0xB4);
        qcopy(rot, moby_vars + 0xE0);
    } else {
        selected_path = ((char *)l14_paths[*(int *)(moby_vars + 0xB8)]);
        total = *(float *)(moby_vars + 0xBC);
        qcopy(rot, moby_vars + 0xF0);
    }
    step = *(float *)&elevator_path_step_rate_gp * frame_time;
    path_endpoint_reached = FUN_L00_0025d808_2f05d8(selected_path,
                                                    pos,
                                                    moby_vars + 0xA4,
                                                    moby_vars + 0xA8,
                                                    0,
                                                    step * *(float *)(moby_vars + 0xAC));
    FUN_L00_0025b8c0(
        &elevator_moby->pos.x, (float *)(moby_vars + 0xC0), pos[0], 0.003f, 0.2f, 0.0f);
    FUN_L00_0025b8c0(
        &elevator_moby->pos.y, (float *)(moby_vars + 0xC4), pos[1], 0.003f, 0.2f, 0.0f);
    FUN_L00_0025b8c0(
        &elevator_moby->pos.z, (float *)(moby_vars + 0xC8), pos[2], 0.003f, 0.2f, 0.0f);
    *(float *)(moby_vars + 0xB0) += step;
    rotation_blend = *(float *)(moby_vars + 0xB0) / total;
    if (1.0f < rotation_blend)
        rotation_blend = 1.0f;
    if (path_endpoint_reached != 0)
        rotation_blend = 1.0f;
    if (*(float *)(moby_vars + 0xAC) < 0.0f)
        rotation_blend = 1.0f - rotation_blend;
    if (0.0f < *(float *)(moby_vars + 0xAC)) {
        x_rotation_blend = 0.0f;
        if (0.5f < rotation_blend)
            x_rotation_blend = (rotation_blend - 0.5f) + (rotation_blend - 0.5f);
    } else {
        x_rotation_blend = 1.0f;
        if (rotation_blend <= 0.5f)
            x_rotation_blend = rotation_blend + rotation_blend;
    }
    if (1.0f < x_rotation_blend)
        x_rotation_blend = 1.0f;
    elevator_moby->rot.x =
        interpolate_angle(*(float *)(moby_vars + 0xD0), rot[0], x_rotation_blend);
    elevator_moby->rot.y = interpolate_angle(*(float *)(moby_vars + 0xD4), rot[1], rotation_blend);
    elevator_moby->rot.z = interpolate_angle(*(float *)(moby_vars + 0xD8), rot[2], rotation_blend);
    if (path_endpoint_reached != 0) {
        if (0.0f < *(float *)(moby_vars + 0xAC)) {
            int last_segment_index = *(int *)selected_path - 2;
            *(int *)(moby_vars + 0xA4) = last_segment_index;
            *(float *)(moby_vars + 0xA8) =
                *(float *)(selected_path + (last_segment_index << 4) + 0x1C);
        } else {
            *(int *)(moby_vars + 0xA4) = 0;
            *(float *)(moby_vars + 0xA8) = 0.0f;
        }
        return 1;
    }
    return 0;
}

void settle_elevator_at_path_endpoint(struct Moby *elevator_moby) {
    char *moby_vars = (char *)elevator_moby->pvars;
    float pos[4];
    float rot[4];
    char *selected_path;
    if (elevator_moby->command == 0) {
        selected_path = ((char *)l14_paths[*(int *)(moby_vars + 0xA0)]);
        qcopy(rot, moby_vars + 0xE0);
    } else {
        selected_path = ((char *)l14_paths[*(int *)(moby_vars + 0xB8)]);
        qcopy(rot, moby_vars + 0xF0);
    }
    if (*(float *)(moby_vars + 0xAC) > 0.0f) {
        qcopy(pos, selected_path + *(int *)selected_path * 16);
    } else {
        qcopy(pos, selected_path + 0x10);
    }
    FUN_L00_0025b8c0(
        &elevator_moby->pos.x, (float *)(moby_vars + 0xC0), pos[0], 0.003f, 0.2f, 0.0f);
    FUN_L00_0025b8c0(
        &elevator_moby->pos.y, (float *)(moby_vars + 0xC4), pos[1], 0.003f, 0.2f, 0.0f);
    FUN_L00_0025b8c0(
        &elevator_moby->pos.z, (float *)(moby_vars + 0xC8), pos[2], 0.003f, 0.2f, 0.0f);
    if (0.0f < *(float *)(moby_vars + 0xAC)) {
        FUN_L00_0025b8c0(
            &elevator_moby->rot.x, (float *)(moby_vars + 0x100), rot[0], 0.003f, 0.2f, 0.0f);
        FUN_L00_0025b8c0(
            &elevator_moby->rot.y, (float *)(moby_vars + 0x104), rot[1], 0.003f, 0.2f, 0.0f);
        FUN_L00_0025b8c0(
            &elevator_moby->rot.z, (float *)(moby_vars + 0x108), rot[2], 0.003f, 0.2f, 0.0f);
    }
}

extern u8 elevator_local_path_points[] __asm__("D_L14_001E03B0");

void rebuild_elevator_world_path(u8 *moby) {
    u8          *data = ((u8 *)MOBY(moby)->pvars);
    u8          *local_path_point_cursor = elevator_local_path_points;
    u8          *world_path_point_cursor = ((u8 *)l14_paths[*(s32 *)(data + 0x114)]) + 0x10;
    s32          i = 10;
    f32          one = 1.0f;
    struct Hero *x;
    do {
        transform_vector_by_basis(world_path_point_cursor, local_path_point_cursor, moby + 0xC0);
        local_path_point_cursor += 0x10;
        i--;
        fast_vec_add(world_path_point_cursor, world_path_point_cursor, moby + 0x10);
        *(f32 *)(world_path_point_cursor + 0xC) = one;
        world_path_point_cursor += 0x10;
    } while (i >= 0);
    x = &hero;
    x->path_constrain_pending = *(u16 *)(data + 0x114);
}

extern float fast_sin(float);

void update_elevator_idle_glow(struct Moby *moby) {
    char        *data = (char *)moby->pvars;
    struct Hero *x = &hero;
    if (absolute_float(moby->pos.x - x->motion.pos.f[0]) > 40.0f ||
        absolute_float(moby->pos.y - x->motion.pos.f[1]) > 40.0f) {
        moby->glow_color = fast_tween_color(moby->glow_color, 0x80303030, 0.1f);
    } else {
        int   i, col;
        float a = fast_add_rotations(*(float *)(data + 0x138), frame_time * 6.2831855f);
        *(float *)(data + 0x138) = a;
        i = func_001FA898_r((fast_sin(a) * 4.0f - 3.0f) * 128.0f);
        if (i > 0x80)
            i = 0x80;
        else if (i < 0x20)
            i = 0x20;
        col = (i << 16) | 0x80000000 | (i << 8) | i;
        moby->glow_color = fast_tween_color(moby->glow_color, col, 0.1f);
    }
}

float absolute_float(float input) __asm__("func_001F99C0");
