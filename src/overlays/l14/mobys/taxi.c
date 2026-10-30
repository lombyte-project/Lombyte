/* Level 14 taxi moby class: stops, routes and prompts. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/camera/follow_cam.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/cuboid.h"

enum TaxiStop {
    TAXI_STOP_RETURN_HUB = 0x0,
    TAXI_STOP_HUB_OUTBOUND_DESTINATION = 0x1,
    TAXI_STOP_RETURN_FEEDER_ORIGIN = 0x2,
    TAXI_STOP_RETURN_FEEDER_DESTINATION = 0x3
};

extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");

#include "rnc/overlay/l14/mobys/taxi_globals.h"

typedef struct {
    u8   pad0[0x20];
    u8   spring[0x40]; /* 0x20 */
    f32  pos[4];       /* 0x60 */
    f32  rot[4];       /* 0x70 */
    s32  points[5];    /* 0x80 */
    u8   pad94[0x18];
    s32 *path;   /* 0xAC: camera path */
    s16  hB0;    /* 0xB0 */
    s16  sound;  /* 0xB2 */
    s16  picked; /* 0xB4 */
    s16  target; /* 0xB6 */
    s32  item_a; /* 0xB8 */
    s32  item_b; /* 0xBC */
    f32  t;      /* 0xC0 */
    f32  tv;     /* 0xC4 */
    u8   padC8[0x18];
    f32  save_pos[4]; /* 0xE0 */
    f32  save_rot[4]; /* 0xF0 */
    s32  clip[3];     /* 0x100 */
} L14TaxiVars;

extern s32 D_L14_0015F5CC;

extern s32 D_L14_0015F594;

extern char D_0013CAE4[];

extern f32 advance_accelerated_scalar(f32 *value,
                                      f32 *velocity,
                                      f32  target,
                                      f32  acceleration_step,
                                      f32  braking_step,
                                      f32  maximum_speed) __asm__("FUN_00213f38");

extern int advance_moby_path(char *) __asm__("FUN_L05_003195f8");

extern void reset_selected_moby_to_current_placement(void *) __asm__("FUN_L05_00319510");

extern void apply_moby_position_and_rotation_wobble(char *) __asm__("FUN_L05_00319740");

extern void transform_moby_vector_table_to_world_space(char *) __asm__("FUN_L05_003198e8");

void initialize_taxi_stop_from_position(struct Moby *moby) __asm__("FUN_L14_002f1040");

void prepare_taxi_route(struct Moby *moby) __asm__("FUN_L14_002f1140");

struct Moby *find_group_moby_by_tag(struct Moby *moby, int tag) __asm__("FUN_L14_002f12f0");

void request_taxi_stop_prompt(struct Moby *moby) __asm__("FUN_L14_002f13f8");

extern void sample_camera_path(s32 *, int, void *, void *, int, float) __asm__("FUN_00214e58");

void taxi_update(struct Moby *moby) __asm__("FUN_L14_002f0538");

void taxi_update(struct Moby *moby) {
    enum {
        TAXI_STATE_INITIALIZE = 0,
        TAXI_STATE_WAIT_AT_STOP = 1,
        TAXI_STATE_BLEND_STOP_TO_PATH_START = 3,
        TAXI_STATE_FOLLOW_PATH = 4,
        TAXI_STATE_BLEND_PATH_END_TO_STOP = 5,
        TAXI_STATE_WAIT_FOR_ITEM_GATE = 6,
        TAXI_STATE_BLEND_SAVED_POSE_TO_PATH_START = 7
    };
    L14TaxiVars *taxi_vars = (L14TaxiVars *)moby->pvars;
    float        previous_position[4], previous_rotation[4], position_delta[4],
        blend_from_position_or_rotation_matrix[4], blend_to_position[4], blend_from_rotation[4],
        blend_to_rotation[4], hero_relative_position[4];
    float taxi_inverse_rotation_matrix[16];
    int   ticks = 10;
    int   button = 0x10;
    int   wrap = 0;
    int   flags = 0;
    qcopy(previous_position, &moby->pos);
    qcopy(previous_rotation, &moby->rot);
    set_moby_dialogue_record_value(moby, (moby->flags ^ 1) & 1);
    switch (moby->state) {
    case TAXI_STATE_INITIALIZE:
        taxi_vars->hB0 = -1;
        taxi_vars->sound = -1;
        initialize_taxi_stop_from_position(moby);
        break;
    case TAXI_STATE_WAIT_AT_STOP: {
        struct Hero *g = &hero;
        if (g->ground_moby == moby && g->air_frames.s == 0) {
            transpose_rotation_matrix(blend_from_position_or_rotation_matrix, &moby->rot_row0);
            fast_vec_sub(hero_relative_position, &g->motion.pos, &moby->pos);
            hero_relative_position[3] = 0;
            transform_vector(hero_relative_position,
                             hero_relative_position,
                             blend_from_position_or_rotation_matrix);
            if (absolute_float(hero_relative_position[1]) < 1.2f) {
                prepare_taxi_route(moby);
                request_taxi_stop_prompt(moby);
                if ((*(int *)D_0013CAE4 & button) && D_L14_0015F594 == 4) {
                    struct Moby *m;
                    moby->state = 3;
                    m = find_group_moby_by_tag(moby, taxi_vars->target);
                    if (m != 0) {
                        m->state = 2;
                        m->flags |= 1;
                        m->visible = 0;
                        m->collision = 0;
                    }
                    taxi_vars->hB0 = -1;
                }
            }
        } else {
            if (taxi_vars->clip[0] >= 0 && taxi_vars->picked != 0 &&
                is_point_inside_clip_volume(&hero.motion.pos, taxi_vars->clip[0])) {
                if (D_L14_0015F5CC < scale_game_frames(ticks)) {
                    taxi_vars->target = 0;
                    taxi_vars->picked = 0;
                    qcopy_nc(&moby->pos, &l14_cuboids[taxi_vars->points[taxi_vars->picked]].pos);
                    qcopy_nc(&moby->rot, &l14_cuboids[taxi_vars->points[taxi_vars->picked]].rot);
                    moby->state = 1;
                    qcopy_nc(taxi_vars->pos,
                             &l14_cuboids[taxi_vars->points[taxi_vars->picked]].pos);
                    qcopy_nc(taxi_vars->rot,
                             &l14_cuboids[taxi_vars->points[taxi_vars->picked]].rot);
                } else {
                    if (taxi_vars->picked == 2) {
                        taxi_vars->picked = 3;
                    } else {
                        taxi_vars->picked = 1;
                    }
                    prepare_taxi_route(moby);
                    moby->state = 3;
                }
            }
            if (taxi_vars->clip[1] >= 0 && taxi_vars->picked != 1 &&
                is_point_inside_clip_volume(&hero.motion.pos, taxi_vars->clip[1])) {
                if (D_L14_0015F5CC < scale_game_frames(ticks)) {
                    taxi_vars->target = 1;
                    taxi_vars->picked = 1;
                    qcopy_nc(&moby->pos, &l14_cuboids[taxi_vars->points[taxi_vars->picked]].pos);
                    qcopy_nc(&moby->rot, &l14_cuboids[taxi_vars->points[taxi_vars->picked]].rot);
                    moby->state = 1;
                    qcopy_nc(taxi_vars->pos,
                             &l14_cuboids[taxi_vars->points[taxi_vars->picked]].pos);
                    qcopy_nc(taxi_vars->rot,
                             &l14_cuboids[taxi_vars->points[taxi_vars->picked]].rot);
                } else {
                    taxi_vars->picked = 0;
                    prepare_taxi_route(moby);
                    moby->state = 3;
                }
            }
            if (taxi_vars->clip[2] >= 0 && taxi_vars->picked < 2 &&
                is_point_inside_clip_volume(&hero.motion.pos, taxi_vars->clip[2])) {
                if (D_L14_0015F5CC < scale_game_frames(ticks)) {
                    taxi_vars->target = 2;
                    taxi_vars->picked = 2;
                    qcopy_nc(&moby->pos, &l14_cuboids[taxi_vars->points[taxi_vars->picked]].pos);
                    qcopy_nc(&moby->rot, &l14_cuboids[taxi_vars->points[taxi_vars->picked]].rot);
                    moby->state = 1;
                    qcopy_nc(taxi_vars->pos,
                             &l14_cuboids[taxi_vars->points[taxi_vars->picked]].pos);
                    qcopy_nc(taxi_vars->rot,
                             &l14_cuboids[taxi_vars->points[taxi_vars->picked]].rot);
                } else {
                    taxi_vars->picked = 2;
                    prepare_taxi_route(moby);
                    moby->state = 3;
                }
            }
        }
        break;
    }
    case TAXI_STATE_BLEND_STOP_TO_PATH_START: {
        struct Hero *g = &hero;
        if (g->ground_moby == moby && g->air_frames.s == 0) {
            follow_camera_set_distance(0, 7.0f, 0.002f);
        }
        /* Fakematch: retail moves these arguments in another order; the cast keeps it. */
        ((float (*)(float, float, float, float, float *, float *))advance_accelerated_scalar)(
            1.0f, frame_time_sq, frame_time_sq, frame_time * 0.5f, &taxi_vars->t, &taxi_vars->tv);
        qcopy_nc(blend_from_position_or_rotation_matrix,
                 &l14_cuboids[taxi_vars->points[taxi_vars->picked]].pos);
        qcopy_nc(blend_from_rotation, &l14_cuboids[taxi_vars->points[taxi_vars->picked]].rot);
        /* Fakematch: retail moves these arguments in another order; the cast keeps it. */
        ((void (*)(float, s32 *, int, void *, void *, int))sample_camera_path)(
            0.0f, taxi_vars->path, wrap, blend_to_position, blend_to_rotation, flags);
        lerp_vector(taxi_vars->pos,
                    blend_from_position_or_rotation_matrix,
                    blend_to_position,
                    taxi_vars->t);
        taxi_vars->rot[1] = fast_add_rotations(
            fast_subtract_rotations(blend_to_rotation[1], blend_from_rotation[1]) * taxi_vars->t,
            blend_from_rotation[1]);
        taxi_vars->rot[2] = fast_add_rotations(
            fast_subtract_rotations(blend_to_rotation[2], blend_from_rotation[2]) * taxi_vars->t,
            blend_from_rotation[2]);
        if (1.0f <= taxi_vars->t) {
            taxi_vars->t = 0.0f;
            taxi_vars->tv = 0.0f;
            moby->state = 4;
        }
        break;
    }
    case TAXI_STATE_BLEND_SAVED_POSE_TO_PATH_START: {
        float hermite_blend_fraction;
        /* Fakematch: retail moves these arguments in another order; the cast keeps it. */
        ((float (*)(float, float, float, float, float *, float *))advance_accelerated_scalar)(
            1.0f, frame_time_sq, frame_time_sq, frame_time * 0.5f, &taxi_vars->t, &taxi_vars->tv);
        qcopy(blend_from_position_or_rotation_matrix, taxi_vars->save_pos);
        qcopy(blend_from_rotation, taxi_vars->save_rot);
        sample_camera_path(
            taxi_vars->path, wrap, blend_to_position, blend_to_rotation, flags, 0.0f);
        hermite_blend_fraction = cubic_hermite_scalar(1.0f, 0.0f, 1.0f, 0.0f, taxi_vars->t);
        lerp_vector(taxi_vars->pos,
                    blend_from_position_or_rotation_matrix,
                    blend_to_position,
                    hermite_blend_fraction);
        taxi_vars->rot[1] = fast_add_rotations(
            fast_subtract_rotations(blend_to_rotation[1], blend_from_rotation[1]) *
                hermite_blend_fraction,
            blend_from_rotation[1]);
        taxi_vars->rot[2] = fast_add_rotations(
            fast_subtract_rotations(blend_to_rotation[2], blend_from_rotation[2]) *
                hermite_blend_fraction,
            blend_from_rotation[2]);
        if (1.0f <= taxi_vars->t) {
            taxi_vars->t = 0.0f;
            taxi_vars->tv = 0.0f;
            moby->state = 4;
        }
        break;
    }
    case TAXI_STATE_FOLLOW_PATH: {
        struct Hero *g = &hero;
        if (g->ground_moby == moby && g->air_frames.s == 0) {
            follow_camera_set_distance(0, 7.0f, 0.003f);
        }
        if (advance_moby_path((char *)moby)) {
            taxi_vars->t = 0.0f;
            taxi_vars->tv = 0.0f;
            moby->state = 5;
        }
        break;
    }
    case TAXI_STATE_BLEND_PATH_END_TO_STOP: {
        struct Hero *g;
        /* Fakematch: retail moves these arguments in another order; the cast keeps it. */
        ((float (*)(float, float, float, float, float *, float *))advance_accelerated_scalar)(
            1.0f, frame_time_sq, frame_time_sq, frame_time * 0.25f, &taxi_vars->t, &taxi_vars->tv);
        qcopy_nc(blend_to_position, &l14_cuboids[taxi_vars->points[taxi_vars->target]].pos);
        qcopy_nc(blend_to_rotation, &l14_cuboids[taxi_vars->points[taxi_vars->target]].rot);
        sample_camera_path(taxi_vars->path,
                           wrap,
                           blend_from_position_or_rotation_matrix,
                           blend_from_rotation,
                           flags,
                           (float)*taxi_vars->path);
        lerp_vector(taxi_vars->pos,
                    blend_from_position_or_rotation_matrix,
                    blend_to_position,
                    taxi_vars->t);
        taxi_vars->rot[1] = fast_add_rotations(
            fast_subtract_rotations(blend_to_rotation[1], blend_from_rotation[1]) * taxi_vars->t,
            blend_from_rotation[1]);
        taxi_vars->rot[2] = fast_add_rotations(
            fast_subtract_rotations(blend_to_rotation[2], blend_from_rotation[2]) * taxi_vars->t,
            blend_from_rotation[2]);
        if (1.0f <= taxi_vars->t) {
            taxi_vars->t = 0.0f;
            taxi_vars->tv = 0.0f;
            moby->state = 1;
            reset_selected_moby_to_current_placement(moby);
        }
        g = &hero;
        if (g->ground_moby == moby && g->air_frames.s == 0) {
            transpose_rotation_matrix(taxi_inverse_rotation_matrix, &moby->rot_row0);
            fast_vec_sub(hero_relative_position, &g->motion.pos, &moby->pos);
            hero_relative_position[3] = 0;
            transform_vector(
                hero_relative_position, hero_relative_position, taxi_inverse_rotation_matrix);
            if (absolute_float(hero_relative_position[1]) < 1.2f) {
                request_taxi_stop_prompt(moby);
                if (*(int *)D_0013CAE4 & button) {
                    qcopy(taxi_vars->save_pos, taxi_vars->pos);
                    qcopy(taxi_vars->save_rot, taxi_vars->rot);
                    initialize_taxi_stop_from_position(moby);
                    taxi_vars->picked = taxi_vars->target;
                    prepare_taxi_route(moby);
                    moby->state = 7;
                    qcopy(taxi_vars->pos, taxi_vars->save_pos);
                    qcopy(taxi_vars->rot, taxi_vars->save_rot);
                    {
                        struct Moby *m = find_group_moby_by_tag(moby, taxi_vars->target);
                        if (m != 0) {
                            m->state = 2;
                            m->flags |= 1;
                            m->visible = 0;
                            m->collision = 0;
                        }
                    }
                }
            }
        }
        break;
    }
    case TAXI_STATE_WAIT_FOR_ITEM_GATE: {
        unsigned char *t = visited52;
        int            k = current_level_index * 16;
        if (t[taxi_vars->item_a + k] == 0xFF || t[taxi_vars->item_b + k] == 0xFF) {
            unsigned short bits = moby->flags;
            int            value = moby->pclass->collision;
            moby->visible = 1;
            moby->collision = value;
            moby->flags = bits & 0xFFFE;
            moby->state = 1;
        }
        break;
    }
    }
    apply_moby_position_and_rotation_wobble((char *)moby);
    if (moby->state >= 3 && moby->state <= 4) {
        struct Hero *g = &hero;
        if (g->ground_moby == moby && g->air_frames.s == 0) {
            g->jump_lock_timer = 2;
            g->unk1F4 = 2;
            transform_moby_vector_table_to_world_space((char *)moby);
        }
        if (voice_is_playing(moby, taxi_vars->sound) == 0) {
            taxi_vars->sound = allocate_voice_for_target_entry(0, 4, moby);
        }
    } else if (voice_is_playing(moby, taxi_vars->sound) != 0) {
        int voice_slot = taxi_vars->sound;
        if (voice_slot != -1) {
            char *voice_entry = ((char *)&voice_pool) + voice_slot * 0x70;
            if (*(struct Moby **)(voice_entry + 0x88) == moby &&
                *(unsigned char *)(voice_entry + 0x74) != 0) {
                release_voice_slot(voice_slot);
            }
        }
        taxi_vars->sound = -1;
    }
    fast_vec_sub(position_delta, &moby->pos, previous_position);
    update_entity_matrix(taxi_vars->spring, position_delta, previous_rotation, &moby->rot);
}

struct L14PickVars {
    char  pad0[0x60];
    float pos[4];
    float rot[4];
    int   points[5];
    char  pad94[0x20];
    short picked;
};

void initialize_taxi_stop_from_position(struct Moby *moby) {
    struct L14PickVars *taxi_vars = (struct L14PickVars *)moby->pvars;
    int                 i;
    taxi_vars->picked = -1;
    for (i = 0; i < 5; i++) {
        if (fast_vec_dist(&moby->pos, &l14_cuboids[taxi_vars->points[i]].pos) < 1.0f) {
            taxi_vars->picked = i;
            break;
        }
    }
    moby->state = 1;
    {
        int          k = taxi_vars->points[taxi_vars->picked];
        LevelCuboid *taxi_stops = l14_cuboids;
        qcopy(taxi_vars->pos, &taxi_stops[k].pos);
        qcopy(taxi_vars->rot, &taxi_stops[k].rot);
    }
}

void prepare_taxi_route(struct Moby *moby) {
    char *taxi_vars = (char *)moby->pvars;
    char *selected_path;
    float inv, travel_step_limit, travel_acceleration_step, first_segment_length;
    switch (*(short *)(taxi_vars + 0xB4)) {
    case TAXI_STOP_RETURN_HUB:
        *(char **)(taxi_vars + 0xAC) = ((char *)l14_paths[*(int *)(taxi_vars + 0x94)]);
        *(short *)(taxi_vars + 0xB6) = 1;
        break;
    case TAXI_STOP_HUB_OUTBOUND_DESTINATION:
        *(char **)(taxi_vars + 0xAC) = ((char *)l14_paths[*(int *)(taxi_vars + 0x98)]);
        *(short *)(taxi_vars + 0xB6) = 0;
        break;
    case TAXI_STOP_RETURN_FEEDER_ORIGIN:
        *(char **)(taxi_vars + 0xAC) = ((char *)l14_paths[*(int *)(taxi_vars + 0x9C)]);
        *(short *)(taxi_vars + 0xB6) = 3;
        break;
    case TAXI_STOP_RETURN_FEEDER_DESTINATION:
        *(char **)(taxi_vars + 0xAC) = ((char *)l14_paths[*(int *)(taxi_vars + 0xA0)]);
        *(short *)(taxi_vars + 0xB6) = 0;
        break;
    case 4:
        break;
    }
    selected_path = *(char **)(taxi_vars + 0xAC);
    first_segment_length = fast_vec_dist(selected_path + 0x10, selected_path + 0x20);
    travel_step_limit = frame_time * 5.0f;
    travel_acceleration_step = frame_time_sq * 2.5f;
    inv = 1.0f / (first_segment_length * (float)**(int **)(taxi_vars + 0xAC));
    *(int *)(taxi_vars + 0xC4) = 0;
    *(int *)(taxi_vars + 0xC0) = 0;
    *(float *)(taxi_vars + 0xD4) = travel_step_limit * inv;
    *(float *)(taxi_vars + 0xD8) = travel_acceleration_step * inv;
}

struct TaggedVars {
    u8  pad0[0xB4];
    s16 tag; /* 0xB4 */
};

extern u16 *D_L14_001ABF40[];

extern struct Moby *D_L14_0015FFD8_12f0 __asm__("D_L14_0015FFD8");

struct Moby *find_group_moby_by_tag(struct Moby *moby, int tag) {
    u16         *group_entry_cursor;
    struct Moby *pool;
    int          group_moby_index;

    if (moby->group < moby_group_index_limit &&
        (group_entry_cursor = D_L14_001ABF40[moby->group]) != 0) {
        pool = D_L14_0015FFD8_12f0;
        do {
            group_moby_index = *group_entry_cursor & 0x7FFF;
            if (((struct TaggedVars *)pool[group_moby_index].pvars)->tag == tag) {
                goto found;
            }
        } while ((s16)*group_entry_cursor++ >= 0);
    }
    return 0;
found:
    return &D_L14_0015FFD8_12f0[group_moby_index];
}

extern void try_set_help_message(int, int) __asm__("FUN_00215130");

void request_taxi_stop_prompt(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int   taxi_stop_index = *(short *)(data + 0xB4);
    if (moby->state == 5) {
        taxi_stop_index = *(short *)(data + 0xB6);
    }
    switch (taxi_stop_index) {
    case TAXI_STOP_RETURN_HUB:
        try_set_help_message(4, 0x53ED);
        break;
    case TAXI_STOP_HUB_OUTBOUND_DESTINATION:
        try_set_help_message(4, 0x53EE);
        break;
    case TAXI_STOP_RETURN_FEEDER_ORIGIN:
        try_set_help_message(4, 0x53EE);
        break;
    case TAXI_STOP_RETURN_FEEDER_DESTINATION:
        try_set_help_message(4, 0x53EE);
        break;
    case 4:
        try_set_help_message(4, 0x1399);
        break;
    default:
        try_set_help_message(4, 0x139B);
        break;
    }
}

float absolute_float(float input) __asm__("func_001F99C0");
