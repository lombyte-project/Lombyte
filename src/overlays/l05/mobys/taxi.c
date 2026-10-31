/* Level 05 moby class taxi: picks a route and moves between its stops. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/game_globals.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/vehicles/taxi_path.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/cuboid.h"

extern char          D_0013E550[];
extern int           allocate_voice_for_target_entry() __asm__("FUN_0022da68");
extern int           voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern unsigned char D_0014C050[][16];
extern void          FUN_L00_00260738(char *a, void *b, void *c, void *d);
extern float         fast_add_rotations(float, float);
extern float         fast_subtract_rotations(float, float);
/* Same source as the exact taxi_update, with its moby data 8 bytes further on, an x range on the pickup test and a table test for state 6. */

extern char D_0013CAE4[];
extern f32  advance_accelerated_scalar(f32 *value,
                                       f32 *velocity,
                                       f32  target,
                                       f32  acceleration_step,
                                       f32  braking_step,
                                       f32  maximum_speed) __asm__("FUN_00213f38");
void        initialize_taxi_stop_state(void *moby_v) __asm__("FUN_L05_00319208");
extern void select_taxi_route(struct Moby *) __asm__("FUN_L05_003193a8");
extern void sample_camera_path(int  *path,
                               s32   loop,
                               void *position,
                               f32  *rotation,
                               s32   flags,
                               f32   progress) __asm__("FUN_00214e58");

void taxi_update(struct Moby *moby) __asm__("FUN_L05_00318c78");

/* Moby class 998 */
void taxi_update(struct Moby *moby) {
    enum {
        TAXI_INITIALIZE = 0,
        TAXI_WAIT_FOR_PICKUP = 1,
        TAXI_BLEND_TO_PATH_START = 3,
        TAXI_FOLLOW_PATH = 4,
        TAXI_BLEND_TO_DESTINATION = 5,
        TAXI_WAIT_FOR_ACTIVATION = 6
    };
    char *taxi_data = (char *)moby->pvars;
    float old_position[4], old_rotation[4], position_delta[4],
        rotation_or_start_position_scratch[4], interpolation_end_position[4],
        interpolation_start_angles[4], interpolation_end_angles[4], hero_offset[4];
    struct Hero *pickup_hero;
    struct Hero *travel_hero;
    float        z;
    struct Moby *destination_stop_moby;
    char        *voice_slot;
    LevelCuboid *tab;
    float       *qa;
    int          voice_slot_index;
    int          s;
    qcopy(old_position, &moby->pos);
    qcopy(old_rotation, &moby->rot);
    set_moby_dialogue_record_value(moby, (moby->flags ^ 1) & 1);
    switch (moby->state) {
    case TAXI_INITIALIZE:
        *(short *)(taxi_data + 0xB2) = *(short *)(taxi_data + 0xB0) = -1;
        initialize_taxi_stop_state(moby);
        break;
    case TAXI_WAIT_FOR_PICKUP:
        pickup_hero = &hero;
        if (((char *)pickup_hero->ground_moby) == moby && pickup_hero->air_frames.s == 0) {
            transpose_rotation_matrix(rotation_or_start_position_scratch, &moby->rot_row0);
            fast_vec_sub(hero_offset, &pickup_hero->motion.pos, &moby->pos);
            hero_offset[3] = 0;
            transform_vector(hero_offset, hero_offset, rotation_or_start_position_scratch);
            if (absolute_float(hero_offset[1]) < 1.2f && hero_offset[0] < 1.0f &&
                hero_offset[0] > -0.75f) {
                select_taxi_route(moby);
                request_moby_variant_help_message(moby);
                if (*(int *)(D_0013CAE4) & 0x10) {
                    moby->state = 3;
                    destination_stop_moby =
                        find_moby_in_owner_list_by_id(moby, *(short *)(taxi_data + 0xB6));
                    if (destination_stop_moby != 0) {
                        destination_stop_moby->state = 2;
                        destination_stop_moby->flags |= 1;
                        destination_stop_moby->visible = 0;
                        destination_stop_moby->collision = 0;
                    }
                }
            }
        }
        break;
    case TAXI_BLEND_TO_PATH_START:
        /* Fakematch: retail moves these arguments in another order; the cast keeps it. */
        ((float (*)(float, float, float, float, float *, float *))advance_accelerated_scalar)(
            1.0f,
            frame_time_sq,
            frame_time_sq,
            frame_time * 0.5f,
            (float *)(taxi_data + 0xC0),
            (float *)(taxi_data + 0xC4));
        qa = rotation_or_start_position_scratch;
        s = *(int *)(taxi_data + 0x80 - (-(*(short *)(taxi_data + 0xB4) * 4)));
        tab = l05_cuboids;
        qcopy(qa, &tab[s].pos);
        qcopy(interpolation_start_angles, &tab[s].rot);
        z = 0.0f;
        /* Fakematch: retail moves these arguments in another order; the cast keeps it. */
        ((void (*)(float, int *, int, void *, float *, int))sample_camera_path)(
            z,
            *(int **)(taxi_data + 0xAC),
            0,
            interpolation_end_position,
            interpolation_end_angles,
            0);
        lerp_vector(taxi_data + 0x60,
                    rotation_or_start_position_scratch,
                    interpolation_end_position,
                    *(float *)(taxi_data + 0xC0));
        *(float *)(taxi_data + 0x74) = fast_add_rotations(
            fast_subtract_rotations(interpolation_end_angles[1], interpolation_start_angles[1]) *
                *(float *)(taxi_data + 0xC0),
            interpolation_start_angles[1]);
        *(float *)(taxi_data + 0x78) = fast_add_rotations(
            fast_subtract_rotations(interpolation_end_angles[2], interpolation_start_angles[2]) *
                *(float *)(taxi_data + 0xC0),
            interpolation_start_angles[2]);
        if (1.0f <= *(float *)(taxi_data + 0xC0)) {
            *(float *)(taxi_data + 0xC0) = z;
            *(float *)(taxi_data + 0xC4) = z;
            moby->state = 4;
        }
        break;
    case TAXI_FOLLOW_PATH:
        if (advance_moby_path(moby)) {
            *(int *)(taxi_data + 0xC0) = 0;
            *(int *)(taxi_data + 0xC4) = 0;
            moby->state = 5;
        }
        break;
    case TAXI_BLEND_TO_DESTINATION:
        /* Fakematch: retail moves these arguments in another order; the cast keeps it. */
        ((float (*)(float, float, float, float, float *, float *))advance_accelerated_scalar)(
            1.0f,
            frame_time_sq,
            frame_time_sq,
            frame_time * 0.5f,
            (float *)(taxi_data + 0xC0),
            (float *)(taxi_data + 0xC4));
        qa = interpolation_end_position;
        s = *(int *)(taxi_data + 0x80 - (-(*(short *)(taxi_data + 0xB6) * 4)));
        tab = l05_cuboids;
        qcopy(qa, &tab[s].pos);
        qcopy(interpolation_end_angles, &tab[s].rot);
        sample_camera_path(*(int **)(taxi_data + 0xAC),
                           0,
                           rotation_or_start_position_scratch,
                           interpolation_start_angles,
                           0,
                           (float)**(int **)(taxi_data + 0xAC));
        lerp_vector(taxi_data + 0x60,
                    rotation_or_start_position_scratch,
                    interpolation_end_position,
                    *(float *)(taxi_data + 0xC0));
        *(float *)(taxi_data + 0x74) = fast_add_rotations(
            fast_subtract_rotations(interpolation_end_angles[1], interpolation_start_angles[1]) *
                *(float *)(taxi_data + 0xC0),
            interpolation_start_angles[1]);
        *(float *)(taxi_data + 0x78) = fast_add_rotations(
            fast_subtract_rotations(interpolation_end_angles[2], interpolation_start_angles[2]) *
                *(float *)(taxi_data + 0xC0),
            interpolation_start_angles[2]);
        if (1.0f <= *(float *)(taxi_data + 0xC0)) {
            *(int *)(taxi_data + 0xC0) = 0;
            *(int *)(taxi_data + 0xC4) = 0;
            moby->state = 1;
            reset_selected_moby_to_current_placement(moby);
        }
        break;
    case TAXI_WAIT_FOR_ACTIVATION:
        if (D_0014C050[current_level_index][*(int *)(taxi_data + 0xB8)] == 0xFF ||
            D_0014C050[current_level_index][*(int *)(taxi_data + 0xBC)] == 0xFF) {
            char          *model = moby->pclass;
            unsigned short flags = moby->flags;
            int            class_collision = *(int *)(model + 0x10);
            moby->visible = 1;
            moby->collision = class_collision;
            moby->flags = flags & 0xFFFE;
            moby->state = 1;
        }
        break;
    }
    apply_moby_position_and_rotation_wobble(moby);
    if (moby->state >= 3 && moby->state <= 5) {
        travel_hero = &hero;
        travel_hero->jump_lock_timer = 2;
        travel_hero->unk1F4 = 2;
        transform_moby_vector_table_to_world_space(moby);
        if (voice_is_playing(moby, *(short *)(taxi_data + 0xB2)) == 0) {
            *(short *)(taxi_data + 0xB2) = allocate_voice_for_target_entry(0, 4, moby);
        }
    } else if (voice_is_playing(moby, *(short *)(taxi_data + 0xB2)) != 0) {
        voice_slot_index = *(short *)(taxi_data + 0xB2);
        if (voice_slot_index != -1) {
            voice_slot = D_0013E550 + voice_slot_index * 0x70;
            if (*(char **)(voice_slot + 0x88) == moby &&
                *(unsigned char *)(voice_slot + 0x74) != 0) {
                release_voice_slot(voice_slot_index);
            }
        }
        *(short *)(taxi_data + 0xB2) = -1;
    }
    fast_vec_sub(position_delta, &moby->pos, old_position);
    FUN_L00_00260738(taxi_data + 0x20, position_delta, old_rotation, &moby->rot);
}

/* Taxi stop selection shared by route helpers. */
enum TaxiStop {
    TAXI_PAIRED_ROUTE_FIRST_STOP = 0x0,
    TAXI_PAIRED_ROUTE_SECOND_STOP = 0x1,
    TAXI_BRANCHING_STOP = 0x2,
    TAXI_PRIMARY_BRANCH_STOP = 0x3,
    TAXI_FALLBACK_BRANCH_STOP = 0x4
};

void initialize_taxi_stop_state(void *moby_v) {
    char *moby = moby_v;
    char *taxi_stop_vars = ((char *)MOBY(moby)->pvars);
    int  *list = (int *)(taxi_stop_vars + 0x80);
    int   i;
    int   selected_pose_byte_offset;
    char *pose_table_base;
    *(short *)(taxi_stop_vars + 0xB4) = -1;
    for (i = 0; i < 5; i++) {
        if (fast_vec_dist(moby + 0x10, &l05_cuboids[list[i]].pos) < 1.0f) {
            *(short *)(taxi_stop_vars + 0xB4) = i;
        }
    }
    switch (*(short *)(taxi_stop_vars + 0xB4)) {
    case TAXI_BRANCHING_STOP: {
        int k = current_level_index << 4;
        if (visited52[*(int *)(taxi_stop_vars + 0xB8) + k] != 0xFF &&
            visited52[*(int *)(taxi_stop_vars + 0xBC) + k] != 0xFF) {
            moby[0x20] = 6;
            MOBY(moby)->flags = MOBY(moby)->flags | 1;
            moby[0x31] = 0;
            MOBY(moby)->collision = 0;
            break;
        } else {
            int v = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
            moby[0x31] = 1;
            MOBY(moby)->collision = v;
            MOBY(moby)->flags = MOBY(moby)->flags & 0xFFFE;
            moby[0x20] = 1;
            break;
        }
    }
    case TAXI_PAIRED_ROUTE_FIRST_STOP:
    case TAXI_PAIRED_ROUTE_SECOND_STOP:
    case TAXI_PRIMARY_BRANCH_STOP:
    case TAXI_FALLBACK_BRANCH_STOP: {
        int v = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
        moby[0x31] = 1;
        MOBY(moby)->collision = v;
        MOBY(moby)->flags = MOBY(moby)->flags & 0xFFFE;
        moby[0x20] = 1;
        break;
    }
    }
    {
        int *selected = list;
        selected += *(short *)(taxi_stop_vars + 0xB4);
        selected_pose_byte_offset = *selected << 7;
    }
    pose_table_base = (char *)l05_cuboids;
    qcopy(taxi_stop_vars + 0x60, (void *)(selected_pose_byte_offset + (int)pose_table_base + 0x30));
    qcopy(taxi_stop_vars + 0x70, (void *)(selected_pose_byte_offset + (int)pose_table_base + 0x70));
}
/* Picks the path for the current point (five points, point 2 chosen by the table entry) and sets the travel speeds; same source as FUN_L16_002e4a58. */

void select_taxi_route(struct Moby *moby) {
    char *taxi_data = ((char *)moby->pvars);
    char *selected_path;
    float inv, frame_scaled_travel_step, frame_squared_travel_step, first_path_segment_length;
    switch (*(short *)(taxi_data + 0xB4)) {
    case TAXI_PAIRED_ROUTE_FIRST_STOP:
        *(char **)(taxi_data + 0xAC) = ((char *)l05_paths[*(int *)(taxi_data + 0x94)]);
        *(short *)(taxi_data + 0xB6) = 1;
        break;
    case TAXI_PAIRED_ROUTE_SECOND_STOP:
        *(char **)(taxi_data + 0xAC) = ((char *)l05_paths[*(int *)(taxi_data + 0x98)]);
        *(short *)(taxi_data + 0xB6) = 0;
        break;
    case TAXI_BRANCHING_STOP:
        if (D_0014C050[current_level_index][*(int *)(taxi_data + 0xB8)] != 0xFF) {
            *(char **)(taxi_data + 0xAC) = ((char *)l05_paths[*(int *)(taxi_data + 0x9C)]);
            *(short *)(taxi_data + 0xB6) = 3;
        } else {
            *(char **)(taxi_data + 0xAC) = ((char *)l05_paths[*(int *)(taxi_data + 0xA4)]);
            *(short *)(taxi_data + 0xB6) = 4;
        }
        break;
    case TAXI_PRIMARY_BRANCH_STOP:
        *(char **)(taxi_data + 0xAC) = ((char *)l05_paths[*(int *)(taxi_data + 0xA0)]);
        *(short *)(taxi_data + 0xB6) = 2;
        break;
    case TAXI_FALLBACK_BRANCH_STOP:
        *(char **)(taxi_data + 0xAC) = ((char *)l05_paths[*(int *)(taxi_data + 0xA8)]);
        *(short *)(taxi_data + 0xB6) = 2;
        break;
    }
    selected_path = *(char **)(taxi_data + 0xAC);
    first_path_segment_length = fast_vec_dist(selected_path + 0x10, selected_path + 0x20);
    frame_scaled_travel_step = frame_time * 20.0f;
    frame_squared_travel_step = frame_time_sq * 5.0f;
    inv = 1.0f / (first_path_segment_length * (float)**(int **)(taxi_data + 0xAC));
    *(int *)(taxi_data + 0xC4) = 0;
    *(int *)(taxi_data + 0xC0) = 0;
    *(float *)(taxi_data + 0xD4) = frame_scaled_travel_step * inv;
    *(float *)(taxi_data + 0xD8) = frame_squared_travel_step * inv;
}
