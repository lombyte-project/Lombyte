/* Level 16 moby class taxi: its update, stop setup, departure path and the helpers it calls. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/watch.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/ui/help/try_set_help_message.h"
#include "rnc/audio/voice_alloc.h"

extern float fast_add_rotations(float, float);
extern float fast_subtract_rotations(float, float);
extern float frame_time, D_0015EE70;

#include "sda.h"

/* Update the moving pickup, interpolate its path transitions, and manage its sound handle. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E5848), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad00[0x30];
    float position[4];
    char  pad40[0x30];
    float direction[4];
} L16PickupPose;

extern L16PickupPose *D_L16_001600EC;
extern char          *find_group_moby_by_data_id(struct Moby *, int) __asm__("FUN_L16_002e4be0");
extern char           D_0013CAE4[];
extern f32            advance_accelerated_scalar(f32 *value,
                                                 f32 *velocity,
                                                 f32  target,
                                                 f32  acceleration_step,
                                                 f32  braking_step,
                                                 f32  maximum_speed) __asm__("FUN_00213f38");
extern float          D_0015EE6C_u;
extern int            advance_taxi_path(struct Moby *) __asm__("FUN_L16_002e4c40");
extern s32            D_L16_0015F594;
extern unsigned char D_0013D4E1 NOT_SDA;
extern void transfer_moby_placement_to_linked_moby(void *) __asm__("FUN_L16_002e4b58");
extern void request_taxi_destination_help_message(struct Moby *) __asm__("FUN_L16_002e4cd8");
extern void apply_taxi_wobble(struct Moby *) __asm__("FUN_L16_002e4d88");
extern void transform_moby_table_entry_vectors(struct Moby *) __asm__("FUN_L16_002e4f30");
extern void initialize_taxi_stop_state(void *) __asm__("FUN_L16_002e4900");
extern void FUN_L16_002e4a58_c(char *) __asm__("FUN_L16_002e4a58");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void sample_camera_path(int  *path,
                               s32   loop,
                               void *position,
                               f32  *rotation,
                               s32   flags,
                               f32   progress) __asm__("FUN_00214e58");

void taxi_update(struct Moby *moby) __asm__("FUN_L16_002e43e0");

void taxi_update(struct Moby *moby) {
    enum {
        TAXI_INITIALIZE = 0,
        TAXI_WAIT_FOR_DEPARTURE = 1,
        TAXI_BLEND_TO_PATH_START = 3,
        TAXI_FOLLOW_PATH = 4,
        TAXI_BLEND_TO_DESTINATION_POSE = 5,
        TAXI_WAIT_FOR_ENABLE = 6
    };
    char *taxi_data = (char *)moby->pvars;
    float previous_position[4], previous_rotation[4], position_delta[4],
        transposed_rotation_or_start_position[4], transition_end_position[4],
        transition_start_rotation[4], transition_end_rotation[4], hero_local_offset[4];
    struct Hero   *player;
    struct Hero   *travel_player;
    float          z;
    char          *matched_moby;
    char          *voice_slot_base;
    L16PickupPose *tab;
    float         *qa;
    int            voice_slot_index;
    int            s;
    qcopy(previous_position, &moby->pos);
    qcopy(previous_rotation, &moby->rot);
    set_moby_dialogue_record_value(moby, (moby->flags ^ 1) & 1);
    switch (moby->state) {
    case TAXI_INITIALIZE:
        *(short *)(taxi_data + 0xAA) = -1;
        initialize_taxi_stop_state(moby);
        break;
    case TAXI_WAIT_FOR_DEPARTURE:
        player = &hero;
        if (((char *)player->ground_moby) == moby && player->air_frames.s == 0) {
            transpose_rotation_matrix(transposed_rotation_or_start_position, &moby->rot_row0);
            fast_vec_sub(hero_local_offset, &player->motion.pos, &moby->pos);
            hero_local_offset[3] = 0;
            transform_vector(
                hero_local_offset, hero_local_offset, transposed_rotation_or_start_position);
            if (absolute_float(hero_local_offset[1]) < 1.2f) {
                FUN_L16_002e4a58_c(moby);
                request_taxi_destination_help_message(moby);
                if ((*(int *)(D_0013CAE4) & 0x10) && *(int *)&D_L16_0015F594 == 8) {
                    moby->state = 3;
                    matched_moby = find_group_moby_by_data_id(moby, *(short *)(taxi_data + 0xAE));
                    if (matched_moby != 0) {
                        matched_moby[0x20] = 2;
                        MOBY(matched_moby)->flags |= 1;
                        matched_moby[0x31] = 0;
                        MOBY(matched_moby)->collision = 0;
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
            (float *)(taxi_data + 0xB0),
            (float *)(taxi_data + 0xB4));
        qa = transposed_rotation_or_start_position;
        s = *(int *)(taxi_data + 0x80 - (-(*(short *)(taxi_data + 0xAC) * 4)));
        tab = D_L16_001600EC;
        qcopy(qa, tab[s].position);
        qcopy(transition_start_rotation, tab[s].direction);
        z = 0.0f;
        /* Fakematch: retail moves these arguments in another order; the cast keeps it. */
        ((void (*)(float, int *, int, void *, float *, int))sample_camera_path)(
            z, *(int **)(taxi_data + 0xA4), 0, transition_end_position, transition_end_rotation, 0);
        lerp_vector(taxi_data + 0x60,
                    transposed_rotation_or_start_position,
                    transition_end_position,
                    *(float *)(taxi_data + 0xB0));
        *(float *)(taxi_data + 0x74) = fast_add_rotations(
            fast_subtract_rotations(transition_end_rotation[1], transition_start_rotation[1]) *
                *(float *)(taxi_data + 0xB0),
            transition_start_rotation[1]);
        *(float *)(taxi_data + 0x78) = fast_add_rotations(
            fast_subtract_rotations(transition_end_rotation[2], transition_start_rotation[2]) *
                *(float *)(taxi_data + 0xB0),
            transition_start_rotation[2]);
        if (1.0f <= *(float *)(taxi_data + 0xB0)) {
            *(float *)(taxi_data + 0xB0) = z;
            *(float *)(taxi_data + 0xB4) = z;
            moby->state = 4;
        }
        break;
    case TAXI_FOLLOW_PATH:
        if (advance_taxi_path(moby)) {
            *(int *)(taxi_data + 0xB0) = 0;
            *(int *)(taxi_data + 0xB4) = 0;
            moby->state = 5;
        }
        break;
    case TAXI_BLEND_TO_DESTINATION_POSE:
        /* Fakematch: retail moves these arguments in another order; the cast keeps it. */
        ((float (*)(float, float, float, float, float *, float *))advance_accelerated_scalar)(
            1.0f,
            frame_time_sq,
            frame_time_sq,
            frame_time * 0.5f,
            (float *)(taxi_data + 0xB0),
            (float *)(taxi_data + 0xB4));
        qa = transition_end_position;
        s = *(int *)(taxi_data + 0x80 - (-(*(short *)(taxi_data + 0xAE) * 4)));
        tab = D_L16_001600EC;
        qcopy(qa, tab[s].position);
        qcopy(transition_end_rotation, tab[s].direction);
        sample_camera_path(*(int **)(taxi_data + 0xA4),
                           0,
                           transposed_rotation_or_start_position,
                           transition_start_rotation,
                           0,
                           (float)**(int **)(taxi_data + 0xA4));
        lerp_vector(taxi_data + 0x60,
                    transposed_rotation_or_start_position,
                    transition_end_position,
                    *(float *)(taxi_data + 0xB0));
        *(float *)(taxi_data + 0x74) = fast_add_rotations(
            fast_subtract_rotations(transition_end_rotation[1], transition_start_rotation[1]) *
                *(float *)(taxi_data + 0xB0),
            transition_start_rotation[1]);
        *(float *)(taxi_data + 0x78) = fast_add_rotations(
            fast_subtract_rotations(transition_end_rotation[2], transition_start_rotation[2]) *
                *(float *)(taxi_data + 0xB0),
            transition_start_rotation[2]);
        if (1.0f <= *(float *)(taxi_data + 0xB0)) {
            *(int *)(taxi_data + 0xB0) = 0;
            *(int *)(taxi_data + 0xB4) = 0;
            moby->state = 1;
            transfer_moby_placement_to_linked_moby(moby);
        }
        break;
    case TAXI_WAIT_FOR_ENABLE:
        if (D_0013D4E1) {
            char          *model = moby->pclass;
            unsigned short flags = moby->flags;
            int            value = *(int *)(model + 0x10);
            moby->visible = 1;
            moby->collision = value;
            moby->flags = flags & 0xFFFE;
            moby->state = 1;
        }
        break;
    }
    apply_taxi_wobble(moby);
    if (moby->state >= 3 && moby->state <= 5) {
        travel_player = &hero;
        travel_player->jump_lock_timer = 2;
        travel_player->unk1F4 = 2;
        transform_moby_table_entry_vectors(moby);
        if (voice_is_playing(moby, *(short *)(taxi_data + 0xAA)) == 0) {
            *(short *)(taxi_data + 0xAA) = allocate_voice_for_target_entry(0, 4, moby);
        }
    } else if (voice_is_playing(moby, *(short *)(taxi_data + 0xAA)) != 0) {
        voice_slot_index = *(short *)(taxi_data + 0xAA);
        if (voice_slot_index != -1) {
            voice_slot_base = ((char *)&voice_pool) + voice_slot_index * 0x70;
            if (*(char **)(voice_slot_base + 0x88) == moby &&
                *(unsigned char *)(voice_slot_base + 0x74) != 0) {
                release_voice_slot(voice_slot_index);
            }
        }
        *(short *)(taxi_data + 0xAA) = -1;
    }
    fast_vec_sub(position_delta, &moby->pos, previous_position);
    update_entity_matrix(taxi_data + 0x20, position_delta, previous_rotation, &moby->rot);
}

extern char *D_L16_001601AC_slots_e4900 __asm__("D_L16_001600EC");

void initialize_taxi_stop_state(void *moby_v) {
    char *moby = moby_v;
    char *data = ((char *)MOBY(moby)->pvars);
    int  *list = (int *)(data + 0x80);
    int   i;
    int   selected_pose_byte_offset;
    char *pose_table_base;
    *(short *)(data + 0xAC) = -1;
    for (i = 0; i < 3; i++) {
        if (fast_vec_dist(moby + 0x10, D_L16_001601AC_slots_e4900 + (list[i] << 7) + 0x30) < 1.0f) {
            *(short *)(data + 0xAC) = i;
        }
    }
    {
        if (item_available[0x21] != 0 || *(short *)(data + 0xAC) == 2) {
            int model_collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
            moby[0x31] = 1;
            MOBY(moby)->collision = model_collision;
            MOBY(moby)->flags = MOBY(moby)->flags & 0xFFFE;
            moby[0x20] = 1;
        } else {
            moby[0x20] = 6;
            moby[0x31] = 0;
            MOBY(moby)->flags = MOBY(moby)->flags | 1;
            MOBY(moby)->collision = 0;
        }
    }
    {
        int *selected = list;
        selected += *(short *)(data + 0xAC);
        selected_pose_byte_offset = *selected << 7;
    }
    pose_table_base = D_L16_001601AC_slots_e4900;
    qcopy(data + 0x60, (void *)(selected_pose_byte_offset + (int)pose_table_base + 0x30));
    qcopy(data + 0x70, (void *)(selected_pose_byte_offset + (int)pose_table_base + 0x70));
}

extern char *D_L16_001B0930[];
/* Pick the path for the current segment and set the travel speeds from its length. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E5EC0), where it is exact; names translated to the US level program. */

void prepare_taxi_departure_path(struct Moby *moby) __asm__("FUN_L16_002e4a58");

void prepare_taxi_departure_path(struct Moby *moby) {
    enum {
        TAXI_DEPART_FROM_FIRST_STOP = 0,
        TAXI_DEPART_FROM_SECOND_STOP = 1,
        TAXI_DEPART_FROM_THIRD_STOP = 2
    };
    char *taxi_data = (char *)moby->pvars;
    char *selected_path;
    float inv, frame_scaled_travel_step, frame_time_squared_travel_factor,
        first_path_segment_length;
    switch (*(short *)(taxi_data + 0xAC)) {
    case TAXI_DEPART_FROM_FIRST_STOP:
        *(char **)(taxi_data + 0xA4) = D_L16_001B0930[*(int *)(taxi_data + 0x90)];
        *(short *)(taxi_data + 0xAE) = 1;
        break;
    case TAXI_DEPART_FROM_SECOND_STOP:
        *(char **)(taxi_data + 0xA4) = D_L16_001B0930[*(int *)(taxi_data + 0x94)];
        *(short *)(taxi_data + 0xAE) = 0;
        break;
    case TAXI_DEPART_FROM_THIRD_STOP:
        *(char **)(taxi_data + 0xA4) = D_L16_001B0930[*(int *)(taxi_data + 0x9C)];
        *(short *)(taxi_data + 0xAE) = 1;
        break;
    }
    selected_path = *(char **)(taxi_data + 0xA4);
    first_path_segment_length = fast_vec_dist(selected_path + 0x10, selected_path + 0x20);
    frame_scaled_travel_step = frame_time * 20.0f;
    frame_time_squared_travel_factor = frame_time_sq * 5.0f;
    inv = 1.0f / (first_path_segment_length * (float)**(int **)(taxi_data + 0xA4));
    *(int *)(taxi_data + 0xB4) = 0;
    *(int *)(taxi_data + 0xB0) = 0;
    *(float *)(taxi_data + 0xC4) = frame_scaled_travel_step * inv;
    *(float *)(taxi_data + 0xC8) = frame_time_squared_travel_factor * inv;
}

extern char *D_L16_001601AC_m_19510 __asm__("D_L16_001600EC");
/* Moves the group moby whose id is the taxi's destination to the current stop's placement, then makes the destination the current stop. */
void transfer_moby_placement_to_linked_moby(void *moby_v) __asm__("FUN_L16_002e4b58");

void transfer_moby_placement_to_linked_moby(void *moby_v) {
    struct Moby *moby = moby_v;
    char        *data = ((char *)moby->pvars);
    char        *spawn = find_group_moby_by_data_id(moby, *(short *)(data + 0xae));
    char        *position;
    if (spawn != 0) {
        int   index;
        char *slot = data;
        char *entry;
        position = spawn + 0x10;
        slot += *(short *)(data + 0xac) << 2;
        index = *(int *)(slot + 0x80) << 7;
        entry = D_L16_001601AC_m_19510;
        qcopy(position, (void *)(index + (int)entry + 0x30));
        qcopy(spawn + 0x40, (void *)(index + (int)entry + 0x70));
        initialize_taxi_stop_state(spawn);
    }
    *(short *)(data + 0xac) = *(unsigned short *)(data + 0xae);
}

/* Finds the listed moby whose data id at 0xAC matches, or returns null. */
extern unsigned short *D_L16_001ABCC0_4be0[] __asm__("D_L16_001ABCC0");

typedef struct {
    char  pad[0x78];
    char *data;
    char  pad2[0x84];
} M_4be0;

extern char *D_L16_0015FFD8_gp __asm__("D_L16_0015FFD8") __attribute__((sda));
char        *find_group_moby_by_data_id(struct Moby *group_source_moby,
                                        int          id) __asm__("FUN_L16_002e4be0");

char *find_group_moby_by_data_id(struct Moby *group_source_moby, int id) {
    unsigned short *moby_index_entry = D_L16_001ABCC0_4be0[group_source_moby->group];
    char           *pool;
    int             moby_pool_index;
    if (moby_index_entry != 0) {
        pool = D_L16_0015FFD8_gp;
        do {
            moby_pool_index = *moby_index_entry & 0x7FFF;
            if (*(short *)(((M_4be0 *)pool)[moby_pool_index].data + 0xAC) == id) {
                goto found;
            }
        } while ((short)*moby_index_entry++ >= 0);
    }
    return 0;
found:
    return (char *)&((M_4be0 *)D_L16_0015FFD8_gp)[moby_pool_index];
}
/* Advances a moby's path: steps the path position and returns whether it has reached the end. */

int advance_taxi_path(struct Moby *moby) __asm__("FUN_L16_002e4c40");

int advance_taxi_path(struct Moby *moby) {
    char *moby_data = (char *)moby->pvars;
    int  *path;
    /* Fakematch: retail moves these arguments in another order; the cast keeps it. */
    ((float (*)(float, float, float, float, float *, float *))advance_accelerated_scalar)(
        1.0f,
        *(float *)(moby_data + 0xC8),
        *(float *)(moby_data + 0xC8),
        *(float *)(moby_data + 0xc4),
        (float *)(moby_data + 0xb0),
        (float *)(moby_data + 0xb4));
    path = *(int **)(moby_data + 0xa4);
    sample_camera_path(path,
                       0,
                       moby_data + 0x60,
                       (float *)(moby_data + 0x70),
                       0,
                       *(float *)(moby_data + 0xb0) * (float)(*path - 1));
    return *(float *)(moby_data + 0xb0) >= 1.0f;
}

/* Shows the help message for the taxi's destination. */

void request_taxi_destination_help_message(struct Moby *moby) __asm__("FUN_L16_002e4cd8");

void request_taxi_destination_help_message(struct Moby *moby) {
    switch (*(short *)((char *)moby->pvars + 0xae)) {
    case 0:
        try_set_help_message(8, 0x3e89);
        break;
    case 1:
        try_set_help_message(8, 0x3e8a);
        break;
    case 2:
        try_set_help_message(8, 0x139A);
        break;
    case 3:
        try_set_help_message(8, 0x139C);
        break;
    case 4:
        try_set_help_message(8, 0x1399);
        break;
    default:
        try_set_help_message(8, 0x139B);
        break;
    }
}
/* Wobbles a moby's position and rotation from its sine-driven state. */

void apply_taxi_wobble(struct Moby *moby) __asm__("FUN_L16_002e4d88");

void apply_taxi_wobble(struct Moby *moby) {
    char *moby_data = (char *)moby->pvars;
    float wobble_phase, wobble_value, wobble_phase_step;
    qcopy(&moby->pos, moby_data + 0x60);
    qcopy(&moby->rot, moby_data + 0x70);
    wobble_phase = fast_add_rotations(*(float *)(moby_data + 0xb8), frame_time * 2.0943951f);
    *(float *)(moby_data + 0xb8) = wobble_phase;
    wobble_value = fast_sin(wobble_phase);
    wobble_phase_step = frame_time * 1.6929693f;
    moby->pos.z += wobble_value * 0.25f;
    wobble_phase = fast_add_rotations(*(float *)(moby_data + 0xbc), wobble_phase_step);
    *(float *)(moby_data + 0xbc) = wobble_phase;
    wobble_value = fast_add_rotations(moby->rot.y, fast_sin(wobble_phase) * 0.08726646f);
    wobble_phase_step = frame_time * 0.9250245f;
    moby->rot.y = wobble_value;
    wobble_phase = fast_add_rotations(*(float *)(moby_data + 0xc0), wobble_phase_step);
    *(float *)(moby_data + 0xc0) = wobble_phase;
    moby->rot.x = fast_add_rotations(moby->rot.x, fast_sin(wobble_phase) * 0.08726646f);
    if (moby->rot.x > 0.17453292f)
        moby->rot.x = 0.17453292f;
    else if (moby->rot.x < -0.17453292f)
        moby->rot.x = -0.17453292f;
    if (moby->rot.y > 0.17453292f)
        moby->rot.y = 0.17453292f;
    else if (moby->rot.y < -0.17453292f)
        moby->rot.y = -0.17453292f;
}
/* Transforms the 7 vectors of a level table entry into the moby's space and records its index. */

extern char table_entry_source_vectors[] __asm__("D_L16_001D96E0");

void transform_moby_table_entry_vectors(struct Moby *moby) __asm__("FUN_L16_002e4f30");

void transform_moby_table_entry_vectors(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    int   table_entry_index = *(int *)(data + 0xa0);
    if (table_entry_index != -1) {
        char *table_entry = D_L16_001B0930[table_entry_index];
        if (*(int *)table_entry == 7) {
            int          i;
            struct Hero *hero_state;
            for (i = 0; i < 7; i++) {
                transform_vector_by_basis(table_entry + 0x10 + i * 16,
                                          table_entry_source_vectors + i * 16,
                                          &moby->rot_row0);
                fast_vec_add(table_entry + 0x10 + i * 16, table_entry + 0x10 + i * 16, &moby->pos);
            }
            hero_state = &hero;
            *(unsigned short *)&hero_state->path_constrain_pending =
                *(unsigned short *)(data + 0xa0);
        }
    }
}
