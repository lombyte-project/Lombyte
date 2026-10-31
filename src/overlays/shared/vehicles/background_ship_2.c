/* Shared moby class background_ship_2: a ship flying in the background, with sparks on its joints. */

#include "types.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/rendering/effects/spawn_spark.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/overlay/shared/vehicles/background_ship_1_globals.h"

extern int debug_print() __asm__("FUN_001e93b0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void *
FUN_L00_00263fd8(char *, int, float *, void *, int, int, float, float *, float *, float *);
extern void  sample_camera_path(void *, s32, void *, void *, s32, f32) __asm__("func_00214E58");
extern int   allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern float scale_time(float) __asm__("FUN_001f96b0");
void         draw_matching_moby_slot_quads(char *self) __asm__("FUN_L02_002ecc68");
extern void
            spawn_background_ship_2_joint_sparks(char *moby,
                                                 void *previous_position) __asm__("FUN_L02_002ed420");
extern void spawn_background_ship_2_level_10_joint_sparks(char *moby,
                                                          void *arg) __asm__("FUN_L02_002ed658");

extern int   D_L02_0015F5CC;
extern char *D_L02_001B0AB0[];
extern float D_L02_0015F580;
float        background_ship_2_path_travel_speed_gp __asm__("D_L02_00161FC0") = 2e+01f;
extern char  background_ship_2_missing_path_message[] __asm__("D_L02_001FADB0");
extern char  background_ship_2_empty_path_message[] __asm__("D_L02_001FADD0");
extern float background_ship_2_color_phase_rate_degrees_gp __asm__("D_L02_00161FC4")
    __attribute__((sda));
extern int   background_ship_2_color_tween_start_gp __asm__("D_L02_00161FC8") __attribute__((sda));
extern int   background_ship_2_color_tween_end_gp __asm__("D_L02_00161FCC") __attribute__((sda));
extern char  D_001413F4[];
extern short background_ship_2_first_spark_position_delta_scale_gp __asm__("D_L02_00161FD8")
    __attribute__((sda));
extern short background_ship_2_second_spark_position_delta_scale_gp __asm__("D_L02_00161FDC")
    __attribute__((sda));
extern short background_ship_2_spark_random_offset_radius_scale_gp __asm__("D_L02_00161FE0")
    __attribute__((sda));
extern short background_ship_2_spark_frame_count_random_span_gp __asm__("D_L02_00161FE4")
    __attribute__((sda));
extern short background_ship_2_spark_second_frame_count_base_gp __asm__("D_L02_00161FE8")
    __attribute__((sda));
extern short background_ship_2_spark_third_frame_count_base_gp __asm__("D_L02_00161FEC")
    __attribute__((sda));
extern short background_ship_2_spark_frame_count_variation_fraction_gp __asm__("D_L02_00161FF0")
    __attribute__((sda));
extern short background_ship_2_first_spark_fourth_component_range_start_gp __asm__("D_L02_00161FF4")
    __attribute__((sda));
extern short background_ship_2_first_spark_fourth_component_range_end_gp __asm__("D_L02_00161FF8")
    __attribute__((sda));
extern short
    background_ship_2_second_spark_fourth_component_range_start_gp __asm__("D_L02_00161FFC")
        __attribute__((sda));
extern short background_ship_2_second_spark_fourth_component_range_end_gp __asm__("D_L02_00162000")
    __attribute__((sda));
extern short background_ship_2_first_spark_color_tween_start_gp __asm__("D_L02_00162004")
    __attribute__((sda));
extern short background_ship_2_first_spark_color_tween_end_gp __asm__("D_L02_00162008")
    __attribute__((sda));
extern short background_ship_2_second_spark_color_tween_start_gp __asm__("D_L02_0016200C")
    __attribute__((sda));
extern short background_ship_2_second_spark_color_tween_end_gp __asm__("D_L02_00162010")
    __attribute__((sda));
extern short
    background_ship_2_level_10_first_spark_position_delta_scale_gp __asm__("D_L02_00162014")
        __attribute__((sda));
extern short
    background_ship_2_level_10_second_spark_position_delta_scale_gp __asm__("D_L02_00162018")
        __attribute__((sda));
extern short
    background_ship_2_level_10_spark_random_offset_radius_scale_gp __asm__("D_L02_0016201C")
        __attribute__((sda));
extern short background_ship_2_level_10_first_spark_color_tween_start_gp __asm__("D_L02_00162020")
    __attribute__((sda));
extern short background_ship_2_level_10_first_spark_color_tween_end_gp __asm__("D_L02_00162024")
    __attribute__((sda));
extern short background_ship_2_level_10_second_spark_color_tween_start_gp __asm__("D_L02_00162028")
    __attribute__((sda));
extern short background_ship_2_level_10_second_spark_color_tween_end_gp __asm__("D_L02_0016202C")
    __attribute__((sda));

float D_L09_00161D2C = 2e+01f;
float D_L09_00161D30 = 3.6e+02f;
int   D_L09_00161D34 = 0x80302020;
int   D_L09_00161D38 = 0x803030c0;
short D_L09_00161D44 = 0x7efa;
short D_L09_00161D48 = 0x7efa;
short D_L09_00161D4C = 0;
short D_L09_00161D50 = 0xf;
short D_L09_00161D54 = 0xf;
short D_L09_00161D58 = 0xf;
short D_L09_00161D5C = 0;
short D_L09_00161D60 = 0x7efa;
short D_L09_00161D64 = 0;
short D_L09_00161D68 = 0;
short D_L09_00161D6C = 0;
short D_L09_00161D70 = -32513;
short D_L09_00161D74 = 0x4060;
short D_L09_00161D78 = 0x2040;
short D_L09_00161D7C = 0x1020;
short D_L09_00161D80 = 0x7efa;
short D_L09_00161D84 = 0x7efa;
short D_L09_00161D88 = 0;
short D_L09_00161D8C = -32513;
short D_L09_00161D90 = 0x4060;
short D_L09_00161D94 = 0x6020;
short D_L09_00161D98 = 0x6050;
float D_L10_0016205C = 2e+01f;
float D_L10_00162060 = 3.6e+02f;
int   D_L10_00162064 = 0x80302020;
int   D_L10_00162068 = 0x803030c0;
short D_L10_00162074 = 0x7efa;
short D_L10_00162078 = 0x7efa;
short D_L10_0016207C = 0;
short D_L10_00162080 = 0xf;
short D_L10_00162084 = 0xf;
short D_L10_00162088 = 0xf;
short D_L10_0016208C = 0;
short D_L10_00162090 = 0x7efa;
short D_L10_00162094 = 0;
short D_L10_00162098 = 0;
short D_L10_0016209C = 0;
short D_L10_001620A0 = -32513;
short D_L10_001620A4 = 0x4060;
short D_L10_001620A8 = 0x2040;
short D_L10_001620AC = 0x1020;
short D_L10_001620B0 = 0x7efa;
short D_L10_001620B4 = 0x7efa;
short D_L10_001620B8 = 0;
short D_L10_001620BC = -32513;
short D_L10_001620C0 = 0x4060;
short D_L10_001620C4 = 0x6020;
short D_L10_001620C8 = 0x6050;

void prepare_background_ship_2_joint_draw_data(char *moby) __asm__("FUN_L02_002ecd50");

void prepare_background_ship_2_joint_draw_data(char *moby) {
    int   i;
    char *background_ship_2_data;
    char *joint_position_out;
    int   current_global_value;
    int   r, g, b, packed_color;
    int  *cached_global_value_ptr;

    if (MOBY(moby)->group == 0xFF) {
        return;
    }
    background_ship_2_data = ((char *)MOBY(moby)->pvars);
    joint_position_out = background_ship_2_data + 0x80;
    for (i = 0; i < 6; i++) {
        moby_point_position(moby, i + 2, joint_position_out);
        joint_position_out += 0x10;
    }
    current_global_value = D_L02_0015F5CC;
    r = *(unsigned char *)(moby + 0x92);
    g = *(unsigned char *)(moby + 0x91);
    b = *(unsigned char *)(moby + 0x90);
    packed_color = (r << 16) | 0x30000000;
    packed_color |= g << 8;
    packed_color |= b;
    *(int *)(background_ship_2_data + 0xE0) = packed_color;
    if (current_global_value != 0) {
        cached_global_value_ptr = *(int **)(background_ship_2_data + 0xE4);
        if (current_global_value != *cached_global_value_ptr) {
            *cached_global_value_ptr = current_global_value;
            add_draw_callback(draw_matching_moby_slot_quads, moby);
        }
    }
}

/* Ported from rac1-decomp src/overlays/shared/vendor_002A5218.c (func_L02_002EE250) */

/* Update for a ship moby: eases its attach point along a path, spawns effects and drives its state machine. */
void background_ship_2_update(char *moby) __asm__("FUN_L02_002ece18");

/* Moby classes 1213, 1973 */
void background_ship_2_update(char *moby) {
    enum {
        BACKGROUND_SHIP_2_INITIALIZE = 0,
        BACKGROUND_SHIP_2_FOLLOW_PATH = 1,
        BACKGROUND_SHIP_2_REMOVE = 2
    };
    char *moby_data;
    char *path;
    int   hit_record;
    int   flag = 0;
    float tv[4];
    float first_path_segment_length;
    float path_travel_distance;
    float x;
    int   first_effect_frame_count;
    int   second_effect_frame_count;
    int   third_effect_frame_count;
    moby_data = ((char *)MOBY(moby)->pvars);
    path = D_L02_001B0AB0[*(int *)(moby_data + 0x60)];
    hit_record = (int)find_moby_hit(moby, 0x10000, 0);
    switch (MOBY(moby)->state) {
    case BACKGROUND_SHIP_2_INITIALIZE: {
        int invalid_index;
        MOBY(moby)->update_dist = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        if (current_level_index == 10) {
            MOBY(moby)->scale = MOBY(moby)->scale * 0.5f;
        }
        invalid_index = -1;
        if (*(int *)(moby_data + 0x60) == invalid_index) {
            debug_print(background_ship_2_missing_path_message, ((short)MOBY(moby)->save_id));
            mark_moby_for_removal(moby);
            return;
        }
        if (*(int *)D_L02_001B0AB0[*(int *)(moby_data + 0x60)] == 0) {
            debug_print(background_ship_2_empty_path_message, ((short)MOBY(moby)->save_id));
            mark_moby_for_removal(moby);
            return;
        }
        first_path_segment_length =
            fast_vec_dist(D_L02_001B0AB0[*(int *)(moby_data + 0x60)] + 0x10,
                          D_L02_001B0AB0[*(int *)(moby_data + 0x60)] + 0x20);
        *(unsigned char *)(moby_data + 0x2C) = 0x28;
        *(float *)(moby_data + 0x70) = first_path_segment_length;
        *(unsigned char *)(moby_data + 0x2B) = 1;
        qcopy(moby + 0x10, D_L02_001B0AB0[*(int *)(moby_data + 0x60)] + 0x10);
        *(float *)(moby_data + 0x68) = random_angle_radians();
        *(int *)(moby_data + 0x6C) = invalid_index;
        MOBY(moby)->state = 1;
        MOBY(moby)->flags |= 0x10;
        *(float *)(moby_data + 0x64) = *(float *)(moby_data + 0x74) * (float)*(int *)path;
        break;
    }
    case BACKGROUND_SHIP_2_FOLLOW_PATH:
        if (*(unsigned char *)D_001413F4 == 1 && current_level_index == 10) {
            mark_moby_for_removal(moby);
            return;
        }
        qcopy(tv, moby + 0x10);
        if (voice_is_playing(moby, *(int *)(moby_data + 0x6C)) == 0) {
            *(int *)(moby_data + 0x6C) = allocate_voice_for_target_entry(1, 4, (int)moby);
        }
        path_travel_distance = background_ship_2_path_travel_speed_gp * frame_time;
        if (current_level_index == 10) {
            path_travel_distance = path_travel_distance * 0.25f;
        }
        *(float *)(moby_data + 0x64) =
            *(float *)(moby_data + 0x64) + path_travel_distance / *(float *)(moby_data + 0x70);
        if ((float)*(int *)path < *(float *)(moby_data + 0x64)) {
            *(float *)(moby_data + 0x64) = *(float *)(moby_data + 0x64) - (float)*(int *)path;
        }
        if ((unsigned int)(current_level_index - 9) < 2) {
            sample_camera_path((int *)path,
                               1,
                               moby + 0x10,
                               (float *)&MOBY(moby)->rot,
                               0,
                               *(float *)(moby_data + 0x64));
        } else {
            sample_camera_path((int *)path,
                               0,
                               moby + 0x10,
                               (float *)&MOBY(moby)->rot,
                               0,
                               *(float *)(moby_data + 0x64));
        }
        if (MOBY(moby)->pos.x < 8.0f) {
            MOBY(moby)->collision = 0;
        } else if (MOBY(moby)->pos.y < 8.0f) {
            MOBY(moby)->collision = 0;
        } else {
            MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
        }
        if (MOBY(moby)->visible != 0) {
            if (current_level_index != 10) {
                spawn_background_ship_2_joint_sparks(moby, tv);
            } else {
                spawn_background_ship_2_level_10_joint_sparks(moby, tv);
            }
        }
        if (hit_record != 0 && 0.0f < *(float *)(hit_record + 0x2C)) {
            flag = 1;
        }
        if (flag) {
            MOBY(moby)->state = 2;
        }
        break;
    case BACKGROUND_SHIP_2_REMOVE:
        if (current_level_index == 2) {
            level_2_ship_removal_count = level_2_ship_removal_count + 1;
            if (level_2_ship_removal_count >= 3) {
                unsigned char *p = item_unlocked;
                if (p[2] == 0) {
                    p[2] = 1;
                    allocate_voice_for_bank_entry(1, 0, 0);
                    queue_dialogue_message(0x53D6, -1);
                }
            }
        }
        if (current_level_index == 9) {
            if (*(int *)camera_secondary_mode == 0x32) {
                level_9_ship_removal_count = level_9_ship_removal_count + 1;
                if (level_9_ship_removal_count >= 5) {
                    unsigned char *p = item_unlocked;
                    if (p[0xF] == 0) {
                        p[0xF] = 1;
                        allocate_voice_for_bank_entry(1, 0, 0);
                        queue_dialogue_message(0x53D6, -1);
                    }
                }
            }
        }
        allocate_voice_for_target_entry(0, 0, (int)moby);
        fast_vec_scale(
            tv, moby + 0xC0, background_ship_2_removal_effect_vector_scale_gp * frame_scale);
        tv[2] = tv[2] + background_ship_2_removal_effect_vector_z_bias_gp * frame_scale;
        first_effect_frame_count = scale_game_frames(90);
        FUN_L00_00263fd8(moby,
                         0x5F6,
                         (float *)&MOBY(moby)->pos,
                         (void *)&MOBY(moby)->rot,
                         first_effect_frame_count,
                         0,
                         frame_time_sq * 12.0f,
                         tv,
                         &D_L02_0015F580,
                         &D_L02_0015F580);
        second_effect_frame_count = scale_game_frames(90);
        FUN_L00_00263fd8(moby,
                         0x5F8,
                         (float *)&MOBY(moby)->pos,
                         (void *)&MOBY(moby)->rot,
                         second_effect_frame_count,
                         0,
                         frame_time_sq * 12.0f,
                         tv,
                         &D_L02_0015F580,
                         &D_L02_0015F580);
        third_effect_frame_count = scale_game_frames(90);
        FUN_L00_00263fd8(moby,
                         0x5F9,
                         (float *)&MOBY(moby)->pos,
                         (void *)&MOBY(moby)->rot,
                         third_effect_frame_count,
                         0,
                         frame_time_sq * 12.0f,
                         tv,
                         &D_L02_0015F580,
                         &D_L02_0015F580);
        mark_moby_for_removal(moby);
        return;
    default:
        break;
    }
    if (MOBY(moby)->visible != 0) {
        x = fast_sin(*(float *)(moby_data + 0x68)) * 4.0f - 3.0f;
        if (1.0f < x) {
            x = 1.0f;
        } else if (x < 0.0f) {
            x = 0.0f;
        }
        {
            int   tweened_color;
            float k;
            tweened_color = fast_tween_color(
                background_ship_2_color_tween_start_gp, background_ship_2_color_tween_end_gp, x);
            k = background_ship_2_color_phase_rate_degrees_gp * 0.017453292f * frame_time;
            MOBY(moby)->glow_color = tweened_color;
            *(float *)(moby_data + 0x68) = fast_add_rotations(*(float *)(moby_data + 0x68), k);
        }
        prepare_background_ship_2_joint_draw_data(moby);
    }
}

void spawn_background_ship_2_joint_sparks(char *moby, void *previous_position) {
    float first_spark_vector[4], second_spark_vector[4], position_delta[4], random_offset[4],
        emission_position[4];
    int i;

    fast_vec_sub(position_delta, moby + 0x10, previous_position);
    for (i = 0; i < 2; i++) {
        int first_spark_color, second_spark_color, frame_count, second_frame_count,
            third_frame_count;
        char *spark_part;

        moby_point_position(moby, i, emission_position);
        fast_vec_scale(first_spark_vector,
                       position_delta,
                       *(float *)&background_ship_2_first_spark_position_delta_scale_gp);
        fast_vec_scale(second_spark_vector,
                       position_delta,
                       *(float *)&background_ship_2_second_spark_position_delta_scale_gp);
        random_vector(random_offset,
                      0.0f,
                      *(float *)&background_ship_2_spark_random_offset_radius_scale_gp *
                          frame_time);
        fast_vec_add(second_spark_vector, second_spark_vector, random_offset);
        first_spark_vector[3] = random_float_between(
            *(float *)&background_ship_2_first_spark_fourth_component_range_start_gp,
            *(float *)&background_ship_2_first_spark_fourth_component_range_end_gp);
        second_spark_vector[3] = random_float_between(
            *(float *)&background_ship_2_second_spark_fourth_component_range_start_gp,
            *(float *)&background_ship_2_second_spark_fourth_component_range_end_gp);
        first_spark_color =
            fast_tween_color(*(int *)&background_ship_2_first_spark_color_tween_start_gp,
                             *(int *)&background_ship_2_first_spark_color_tween_end_gp,
                             random_float_between(0.0f, 1.0f));
        second_spark_color =
            fast_tween_color(*(int *)&background_ship_2_second_spark_color_tween_start_gp,
                             *(int *)&background_ship_2_second_spark_color_tween_end_gp,
                             random_float_between(0.0f, 1.0f));
        frame_count = truncate_float_to_s32(
            scale_time((float)*(int *)&background_ship_2_spark_frame_count_random_span_gp *
                           random_float_between(0.0f, 1.0f) +
                       1.0f));
        second_frame_count = truncate_float_to_s32(
            scale_time((float)*(int *)&background_ship_2_spark_second_frame_count_base_gp *
                       (random_float_between(
                            -*(float *)&background_ship_2_spark_frame_count_variation_fraction_gp,
                            *(float *)&background_ship_2_spark_frame_count_variation_fraction_gp) +
                        1.0f)));
        third_frame_count = truncate_float_to_s32(
            scale_time((float)*(int *)&background_ship_2_spark_third_frame_count_base_gp *
                       (random_float_between(
                            -*(float *)&background_ship_2_spark_frame_count_variation_fraction_gp,
                            *(float *)&background_ship_2_spark_frame_count_variation_fraction_gp) +
                        1.0f)));
        spark_part = spawn_spark(emission_position,
                                 first_spark_vector,
                                 second_spark_vector,
                                 first_spark_color,
                                 second_spark_color,
                                 frame_count,
                                 second_frame_count,
                                 third_frame_count,
                                 -1);
        if (spark_part != 0) {
            spark_part[9] = truncate_float_to_s32(8.0f) + 0x60;
        }
    }
}

void spawn_background_ship_2_level_10_joint_sparks(char *moby, void *previous_position) {
    float first_spark_vector[4], second_spark_vector[4], position_delta[4], random_offset[4],
        emission_position[4];
    int i;

    fast_vec_sub(position_delta, moby + 0x10, previous_position);
    for (i = 0; i < 2; i++) {
        int first_spark_color, second_spark_color, frame_count, second_frame_count,
            third_frame_count;
        char *spark_part;

        moby_point_position(moby, i, emission_position);
        fast_vec_scale(first_spark_vector,
                       position_delta,
                       *(float *)&background_ship_2_level_10_first_spark_position_delta_scale_gp);
        fast_vec_scale(second_spark_vector,
                       position_delta,
                       *(float *)&background_ship_2_level_10_second_spark_position_delta_scale_gp);
        random_vector(random_offset,
                      0.0f,
                      *(float *)&background_ship_2_level_10_spark_random_offset_radius_scale_gp *
                          frame_time);
        fast_vec_add(second_spark_vector, second_spark_vector, random_offset);
        first_spark_vector[3] =
            random_float_between(
                *(float *)&background_ship_2_first_spark_fourth_component_range_start_gp,
                *(float *)&background_ship_2_first_spark_fourth_component_range_end_gp) /
            1.5f;
        second_spark_vector[3] =
            random_float_between(
                *(float *)&background_ship_2_second_spark_fourth_component_range_start_gp,
                *(float *)&background_ship_2_second_spark_fourth_component_range_end_gp) /
            1.5f;
        first_spark_color =
            fast_tween_color(*(int *)&background_ship_2_level_10_first_spark_color_tween_start_gp,
                             *(int *)&background_ship_2_level_10_first_spark_color_tween_end_gp,
                             random_float_between(0.0f, 1.0f));
        second_spark_color =
            fast_tween_color(*(int *)&background_ship_2_level_10_second_spark_color_tween_start_gp,
                             *(int *)&background_ship_2_level_10_second_spark_color_tween_end_gp,
                             random_float_between(0.0f, 1.0f));
        frame_count = truncate_float_to_s32(
            scale_time((float)*(int *)&background_ship_2_spark_frame_count_random_span_gp *
                           random_float_between(0.0f, 1.0f) +
                       1.0f));
        second_frame_count = truncate_float_to_s32(
            scale_time((float)*(int *)&background_ship_2_spark_second_frame_count_base_gp *
                       (random_float_between(
                            -*(float *)&background_ship_2_spark_frame_count_variation_fraction_gp,
                            *(float *)&background_ship_2_spark_frame_count_variation_fraction_gp) +
                        1.0f)));
        third_frame_count = truncate_float_to_s32(
            scale_time((float)*(int *)&background_ship_2_spark_third_frame_count_base_gp *
                       (random_float_between(
                            -*(float *)&background_ship_2_spark_frame_count_variation_fraction_gp,
                            *(float *)&background_ship_2_spark_frame_count_variation_fraction_gp) +
                        1.0f)));
        spark_part = spawn_spark(emission_position,
                                 first_spark_vector,
                                 second_spark_vector,
                                 first_spark_color,
                                 second_spark_color,
                                 frame_count,
                                 second_frame_count,
                                 third_frame_count,
                                 -1);
        if (spark_part != 0) {
            spark_part[9] = truncate_float_to_s32(8.0f) + 0x60;
        }
    }
}

int   background_ship_2_color_tween_start_gp __asm__("D_L02_00161FC8") = 0x80302020;
int   background_ship_2_color_tween_end_gp __asm__("D_L02_00161FCC") = 0x803030c0;
short background_ship_2_first_spark_color_tween_start_gp __asm__("D_L02_00162004") = -32513;
short background_ship_2_first_spark_color_tween_end_gp __asm__("D_L02_00162008") = 0x4060;
short background_ship_2_second_spark_color_tween_start_gp __asm__("D_L02_0016200C") = 0x2040;
short background_ship_2_second_spark_color_tween_end_gp __asm__("D_L02_00162010") = 0x1020;

short
    background_ship_2_level_10_first_spark_color_tween_start_gp __asm__("D_L02_00162020") = -32513;

short background_ship_2_level_10_first_spark_color_tween_end_gp __asm__("D_L02_00162024") = 0x4060;

short
    background_ship_2_level_10_second_spark_color_tween_start_gp __asm__("D_L02_00162028") = 0x6020;

short background_ship_2_level_10_second_spark_color_tween_end_gp __asm__("D_L02_0016202C") = 0x6050;

float background_ship_2_color_phase_rate_degrees_gp __asm__("D_L02_00161FC4") = 3.6e+02f;

extern short background_ship_2_first_spark_position_delta_scale_gp __asm__("D_L02_00161FD8")
    __attribute__((sda));

extern short background_ship_2_second_spark_position_delta_scale_gp __asm__("D_L02_00161FDC")
    __attribute__((sda));

extern short background_ship_2_spark_random_offset_radius_scale_gp __asm__("D_L02_00161FE0")
    __attribute__((sda));

extern short background_ship_2_spark_frame_count_random_span_gp __asm__("D_L02_00161FE4")
    __attribute__((sda));

extern short background_ship_2_spark_second_frame_count_base_gp __asm__("D_L02_00161FE8")
    __attribute__((sda));

extern short background_ship_2_spark_third_frame_count_base_gp __asm__("D_L02_00161FEC")
    __attribute__((sda));

extern short background_ship_2_spark_frame_count_variation_fraction_gp __asm__("D_L02_00161FF0")
    __attribute__((sda));

extern short background_ship_2_first_spark_fourth_component_range_start_gp __asm__("D_L02_00161FF4")
    __attribute__((sda));

extern short background_ship_2_first_spark_fourth_component_range_end_gp __asm__("D_L02_00161FF8")
    __attribute__((sda));

extern short background_ship_2_second_spark_fourth_component_range_end_gp __asm__("D_L02_00162000")
    __attribute__((sda));
