/* Level 05 moby class taxi: picks a route and moves between its stops. */
#include "rnc/input/pad_globals.h"
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
#include "rnc/overlay/shared/world/level_path.h"
#include "rnc/overlay/shared/world/cuboid.h"
#include "rnc/math/interpolation/advance_accelerated_scalar.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"

extern char          D_0013E550[];
extern int           allocate_voice_for_target_entry() __asm__("FUN_0022da68");
extern unsigned char D_0014C050[][16];
extern void          FUN_L00_00260738(char *a, void *b, void *c, void *d);
extern float         fast_add_rotations(float, float);
extern float         fast_subtract_rotations(float, float);
/* Same source as the exact taxi_update, with its moby data 8 bytes further on, an x range on the pickup test and a table test for state 6. */

void        initialize_taxi_stop_state(void *moby_v) __asm__("FUN_L05_00319208");
extern void select_taxi_route(struct Moby *) __asm__("FUN_L05_003193a8");
extern void sample_camera_path(int  *path,
                               s32   loop,
                               void *position,
                               f32  *rotation,
                               s32   flags,
                               f32   progress) __asm__("FUN_00214e58");

void taxi_update(struct Moby *moby) __asm__("FUN_L05_00318c78");

typedef struct TaxiVars {
    u8    pad_0[0x20];
    u8    unk20;
    u8    pad_21[0x3F];
    f32   unk60[4];
    f32   unk70[4];
    s32   unk80;
    u8    pad_84[0x10];
    s32   path_94;
    s32   path_98;
    s32   path_9C;
    s32   path_A0;
    s32   path_A4;
    s32   path_A8;
    char *unkAC;
    s16   unkB0;
    s16   voice;
    s16   unkB4;
    s16   unkB6;
    s32   unkB8;
    s32   unkBC;
    f32   unkC0;
    s32   unkC4;
    u8    pad_C8[0xC];
    f32   unkD4;
    f32   unkD8;
} TaxiVars;

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
    enum {
        TAXI_HIDDEN_STOP = 2
    };
    TaxiVars *taxi_data = (TaxiVars *)moby->pvars;
    float     old_position[4], old_rotation[4], position_delta[4],
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
        taxi_data->voice = taxi_data->unkB0 = -1;
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
                if (pad_pressed_words[0] & 0x10) {
                    moby->state = TAXI_BLEND_TO_PATH_START;
                    destination_stop_moby = find_moby_in_owner_list_by_id(moby, taxi_data->unkB6);
                    if (destination_stop_moby != 0) {
                        destination_stop_moby->state = TAXI_HIDDEN_STOP;
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
            &taxi_data->unkC0,
            (float *)&taxi_data->unkC4);
        qa = rotation_or_start_position_scratch;
        s = *(int *)((u8 *)taxi_data + 0x80 - (-(taxi_data->unkB4 * 4)));
        tab = l05_cuboids;
        qcopy(qa, &tab[s].pos);
        qcopy(interpolation_start_angles, &tab[s].rot);
        z = 0.0f;
        /* Fakematch: retail moves these arguments in another order; the cast keeps it. */
        ((void (*)(float, int *, int, void *, float *, int))sample_camera_path)(
            z, (int *)taxi_data->unkAC, 0, interpolation_end_position, interpolation_end_angles, 0);
        lerp_vector(taxi_data->unk60,
                    rotation_or_start_position_scratch,
                    interpolation_end_position,
                    taxi_data->unkC0);
        taxi_data->unk70[1] = fast_add_rotations(
            fast_subtract_rotations(interpolation_end_angles[1], interpolation_start_angles[1]) *
                taxi_data->unkC0,
            interpolation_start_angles[1]);
        taxi_data->unk70[2] = fast_add_rotations(
            fast_subtract_rotations(interpolation_end_angles[2], interpolation_start_angles[2]) *
                taxi_data->unkC0,
            interpolation_start_angles[2]);
        if (1.0f <= taxi_data->unkC0) {
            taxi_data->unkC0 = z;
            *(float *)&taxi_data->unkC4 = z;
            moby->state = TAXI_FOLLOW_PATH;
        }
        break;
    case TAXI_FOLLOW_PATH:
        if (advance_moby_path(moby)) {
            *(int *)&taxi_data->unkC0 = 0;
            taxi_data->unkC4 = 0;
            moby->state = TAXI_BLEND_TO_DESTINATION;
        }
        break;
    case TAXI_BLEND_TO_DESTINATION:
        /* Fakematch: retail moves these arguments in another order; the cast keeps it. */
        ((float (*)(float, float, float, float, float *, float *))advance_accelerated_scalar)(
            1.0f,
            frame_time_sq,
            frame_time_sq,
            frame_time * 0.5f,
            &taxi_data->unkC0,
            (float *)&taxi_data->unkC4);
        qa = interpolation_end_position;
        s = *(int *)((u8 *)taxi_data + 0x80 - (-(taxi_data->unkB6 * 4)));
        tab = l05_cuboids;
        qcopy(qa, &tab[s].pos);
        qcopy(interpolation_end_angles, &tab[s].rot);
        sample_camera_path((int *)taxi_data->unkAC,
                           0,
                           rotation_or_start_position_scratch,
                           interpolation_start_angles,
                           0,
                           (float)*(int *)taxi_data->unkAC);
        lerp_vector(taxi_data->unk60,
                    rotation_or_start_position_scratch,
                    interpolation_end_position,
                    taxi_data->unkC0);
        taxi_data->unk70[1] = fast_add_rotations(
            fast_subtract_rotations(interpolation_end_angles[1], interpolation_start_angles[1]) *
                taxi_data->unkC0,
            interpolation_start_angles[1]);
        taxi_data->unk70[2] = fast_add_rotations(
            fast_subtract_rotations(interpolation_end_angles[2], interpolation_start_angles[2]) *
                taxi_data->unkC0,
            interpolation_start_angles[2]);
        if (1.0f <= taxi_data->unkC0) {
            *(int *)&taxi_data->unkC0 = 0;
            taxi_data->unkC4 = 0;
            moby->state = TAXI_WAIT_FOR_PICKUP;
            reset_selected_moby_to_current_placement(moby);
        }
        break;
    case TAXI_WAIT_FOR_ACTIVATION:
        if (D_0014C050[current_level_index][taxi_data->unkB8] == 0xFF ||
            D_0014C050[current_level_index][taxi_data->unkBC] == 0xFF) {
            char          *model = moby->pclass;
            unsigned short flags = moby->flags;
            int            class_collision = *(int *)(model + 0x10);
            moby->visible = 1;
            moby->collision = class_collision;
            moby->flags = flags & 0xFFFE;
            moby->state = TAXI_WAIT_FOR_PICKUP;
        }
        break;
    }
    apply_moby_position_and_rotation_wobble(moby);
    if (moby->state >= TAXI_BLEND_TO_PATH_START && moby->state <= TAXI_BLEND_TO_DESTINATION) {
        travel_hero = &hero;
        travel_hero->jump_lock_timer = 2;
        travel_hero->edge_brake_lock_timer = 2;
        transform_moby_vector_table_to_world_space(moby);
        if (voice_is_playing(moby, taxi_data->voice) == 0) {
            taxi_data->voice = allocate_voice_for_target_entry(0, 4, moby);
        }
    } else if (voice_is_playing(moby, taxi_data->voice) != 0) {
        voice_slot_index = taxi_data->voice;
        if (voice_slot_index != -1) {
            voice_slot = D_0013E550 + voice_slot_index * 0x70;
            if (*(char **)(voice_slot + 0x88) == moby &&
                *(unsigned char *)(voice_slot + 0x74) != 0) {
                release_voice_slot(voice_slot_index);
            }
        }
        taxi_data->voice = -1;
    }
    fast_vec_sub(position_delta, &moby->pos, old_position);
    FUN_L00_00260738(&taxi_data->unk20, position_delta, old_rotation, &moby->rot);
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
    char     *moby = moby_v;
    TaxiVars *taxi_stop_vars = (TaxiVars *)MOBY(moby)->pvars;
    int      *list = &taxi_stop_vars->unk80;
    int       i;
    int       selected_pose_byte_offset;
    char     *pose_table_base;
    taxi_stop_vars->unkB4 = -1;
    for (i = 0; i < 5; i++) {
        if (fast_vec_dist(moby + 0x10, &l05_cuboids[list[i]].pos) < 1.0f) {
            taxi_stop_vars->unkB4 = i;
        }
    }
    switch (taxi_stop_vars->unkB4) {
    case TAXI_BRANCHING_STOP: {
        int k = current_level_index << 4;
        if (visited52[taxi_stop_vars->unkB8 + k] != 0xFF &&
            visited52[taxi_stop_vars->unkBC + k] != 0xFF) {
            moby[0x20] = 6;
            MOBY(moby)->flags = MOBY(moby)->flags | 1;
            moby[0x31] = 0;
            MOBY(moby)->collision = 0;
            break;
        } else {
            int v = MOBY(moby)->pclass->collision;
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
        int v = MOBY(moby)->pclass->collision;
        moby[0x31] = 1;
        MOBY(moby)->collision = v;
        MOBY(moby)->flags = MOBY(moby)->flags & 0xFFFE;
        moby[0x20] = 1;
        break;
    }
    }
    {
        int *selected = list;
        selected += taxi_stop_vars->unkB4;
        selected_pose_byte_offset = *selected << 7;
    }
    pose_table_base = (char *)l05_cuboids;
    qcopy(taxi_stop_vars->unk60, (void *)(selected_pose_byte_offset + (int)pose_table_base + 0x30));
    qcopy(taxi_stop_vars->unk70, (void *)(selected_pose_byte_offset + (int)pose_table_base + 0x70));
}
/* Picks the path for the current point (five points, point 2 chosen by the table entry) and sets the travel speeds; same source as FUN_L16_002e4a58. */

void select_taxi_route(struct Moby *moby) {
    TaxiVars *taxi_data = (TaxiVars *)moby->pvars;
    char     *selected_path;
    float     inv, frame_scaled_travel_step, frame_squared_travel_step, first_path_segment_length;
    switch (taxi_data->unkB4) {
    case TAXI_PAIRED_ROUTE_FIRST_STOP:
        taxi_data->unkAC = ((char *)l05_paths[taxi_data->path_94]);
        taxi_data->unkB6 = 1;
        break;
    case TAXI_PAIRED_ROUTE_SECOND_STOP:
        taxi_data->unkAC = ((char *)l05_paths[taxi_data->path_98]);
        taxi_data->unkB6 = 0;
        break;
    case TAXI_BRANCHING_STOP:
        if (D_0014C050[current_level_index][taxi_data->unkB8] != 0xFF) {
            taxi_data->unkAC = ((char *)l05_paths[taxi_data->path_9C]);
            taxi_data->unkB6 = 3;
        } else {
            taxi_data->unkAC = ((char *)l05_paths[taxi_data->path_A4]);
            taxi_data->unkB6 = 4;
        }
        break;
    case TAXI_PRIMARY_BRANCH_STOP:
        taxi_data->unkAC = ((char *)l05_paths[taxi_data->path_A0]);
        taxi_data->unkB6 = 2;
        break;
    case TAXI_FALLBACK_BRANCH_STOP:
        taxi_data->unkAC = ((char *)l05_paths[taxi_data->path_A8]);
        taxi_data->unkB6 = 2;
        break;
    }
    selected_path = taxi_data->unkAC;
    first_path_segment_length = fast_vec_dist(selected_path + 0x10, selected_path + 0x20);
    frame_scaled_travel_step = frame_time * 20.0f;
    frame_squared_travel_step = frame_time_sq * 5.0f;
    inv = 1.0f / (first_path_segment_length * (float)*(int *)taxi_data->unkAC);
    taxi_data->unkC4 = 0;
    *(int *)&taxi_data->unkC0 = 0;
    taxi_data->unkD4 = frame_scaled_travel_step * inv;
    taxi_data->unkD8 = frame_squared_travel_step * inv;
}
