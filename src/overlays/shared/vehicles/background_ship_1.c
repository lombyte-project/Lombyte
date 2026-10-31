/* Shared moby class background_ship_1: a ship flying in the background, with billboards and sparks on its joints. */

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

extern int  get_first_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002591d0");
extern int  get_next_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002592b8");
extern void draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");
extern int  debug_print() __asm__("FUN_001e93b0");
extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void *
FUN_L00_00263fd8(char *, int, float *, void *, int, int, float, float *, float *, float *);
extern void
             spawn_background_ship_1_joint_sparks(char *moby,
                                                  void *previous_position) __asm__("FUN_L02_002eca30");
extern void  sample_camera_path(void *, s32, void *, void *, s32, f32) __asm__("func_00214E58");
extern int   func_0022ED80_r(int, int, int) __asm__("FUN_0022da68");
extern float scale_time(float) __asm__("FUN_001f96b0");

extern int   D_L02_0015F5CC;
extern char *D_L02_001B0AB0[];
extern char  background_ship_1_missing_path_message[] __asm__("D_L02_001FAD60");
extern char  background_ship_1_empty_path_message[] __asm__("D_L02_001FAD80");
extern float D_0015ED60;
extern float D_L02_0015F580;
float        background_ship_1_path_travel_speed_gp __asm__("D_L02_00161F64") = 2e+01f;
extern float background_ship_1_color_phase_rate_degrees_gp __asm__("D_L02_00161F70")
    __attribute__((sda));
extern short background_ship_1_color_tween_start_gp __asm__("D_L02_00161F74") __attribute__((sda));
extern short background_ship_1_color_tween_end_gp __asm__("D_L02_00161F78") __attribute__((sda));
extern short background_ship_1_first_spark_position_delta_scale_gp __asm__("D_L02_00161F84")
    __attribute__((sda));
extern short background_ship_1_second_spark_position_delta_scale_gp __asm__("D_L02_00161F88")
    __attribute__((sda));
extern short background_ship_1_spark_random_offset_radius_scale_gp __asm__("D_L02_00161F8C")
    __attribute__((sda));
extern short background_ship_1_spark_frame_count_random_span_gp __asm__("D_L02_00161F90")
    __attribute__((sda));
extern short background_ship_1_spark_second_frame_count_base_gp __asm__("D_L02_00161F94")
    __attribute__((sda));
extern short background_ship_1_spark_third_frame_count_base_gp __asm__("D_L02_00161F98")
    __attribute__((sda));
extern short background_ship_1_spark_frame_count_variation_fraction_gp __asm__("D_L02_00161F9C")
    __attribute__((sda));
extern short background_ship_1_first_spark_fourth_component_range_start_gp __asm__("D_L02_00161FA0")
    __attribute__((sda));
extern short background_ship_1_first_spark_fourth_component_range_end_gp __asm__("D_L02_00161FA4")
    __attribute__((sda));
extern short
    background_ship_1_second_spark_fourth_component_range_start_gp __asm__("D_L02_00161FA8")
        __attribute__((sda));
extern short background_ship_1_second_spark_fourth_component_range_end_gp __asm__("D_L02_00161FAC")
    __attribute__((sda));
extern short background_ship_1_first_spark_color_tween_start_gp __asm__("D_L02_00161FB0")
    __attribute__((sda));
extern short background_ship_1_first_spark_color_tween_end_gp __asm__("D_L02_00161FB4")
    __attribute__((sda));
extern short background_ship_1_second_spark_color_tween_start_gp __asm__("D_L02_00161FB8")
    __attribute__((sda));
extern short background_ship_1_second_spark_color_tween_end_gp __asm__("D_L02_00161FBC")
    __attribute__((sda));

float D_L09_00161CD0 = 2e+01f;
float D_L09_00161CDC = 3.6e+02f;
short D_L09_00161CE0 = 0x2020;
short D_L09_00161CE4 = 0x30c0;
short D_L09_00161CF0 = 0x7efa;
short D_L09_00161CF4 = 0x7efa;
short D_L09_00161CF8 = 0;
short D_L09_00161CFC = 0x17;
short D_L09_00161D00 = 0xb;
short D_L09_00161D04 = 0xb;
short D_L09_00161D08 = 0;
short D_L09_00161D0C = 0x7efa;
short D_L09_00161D10 = 0;
short D_L09_00161D14 = 0;
short D_L09_00161D18 = 0;
short D_L09_00161D1C = -32513;
short D_L09_00161D20 = -24384;
short D_L09_00161D24 = 0x2040;
short D_L09_00161D28 = 0x1020;

/* Walks the moby list matching this moby's id and pulses each match's data slots while it is bright. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002ED740), where it is exact; names translated to the US level program. */
void draw_background_ship_1_joint_billboards(char *self) __asm__("FUN_L02_002ec308");

void draw_background_ship_1_joint_billboards(char *reference_moby) {
    struct Moby *current_moby;
    get_first_moby_in_group((int *)&current_moby, *(unsigned char *)(reference_moby + 0x21), 0, 0);
    while (current_moby != 0) {
        if (current_moby->visible != 0) {
            char *joint_billboard_data = ((char *)current_moby->pvars);
            if (current_moby->oclass == *(short *)(reference_moby + 0xA6)) {
                if ((*(int *)(joint_billboard_data + 0xB0) & 0xFF) > 0x80) {
                    char *joint_position = joint_billboard_data + 0x80;
                    int   i;
                    for (i = 2; i >= 0; i--) {
                        draw_offset_textured_effect_quad(
                            joint_position, *(int *)(joint_billboard_data + 0xB0), 0.9f, 0.45f);
                        joint_position += 0x10;
                    }
                }
            }
        }
        get_next_moby_in_group((int *)&current_moby, (int)current_moby, 0, 0);
    }
}

/* Refreshes a moby's three sub-objects, packs its colour bytes, and queues a redraw when the global changed. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002ED828), where it is exact; names translated to the US level program. */
void prepare_background_ship_1_joint_billboards(char *moby) __asm__("FUN_L02_002ec3f0");

void prepare_background_ship_1_joint_billboards(char *moby) {
    int   i;
    char *background_ship_data;
    char *joint_position_out;
    int   current_global_value;
    int   r, g, b, packed_color;
    int  *cached_global_value_ptr;

    if (MOBY(moby)->group == 0xFF) {
        return;
    }
    background_ship_data = ((char *)MOBY(moby)->pvars);
    joint_position_out = background_ship_data + 0x80;
    for (i = 0; i < 3; i++) {
        moby_point_position(moby, i + 3, joint_position_out);
        joint_position_out += 0x10;
    }
    current_global_value = D_L02_0015F5CC;
    r = *(unsigned char *)(moby + 0x92);
    g = *(unsigned char *)(moby + 0x91);
    b = *(unsigned char *)(moby + 0x90);
    packed_color = (r << 16) | 0x30000000;
    packed_color |= g << 8;
    packed_color |= b;
    *(int *)(background_ship_data + 0xB0) = packed_color;
    if (current_global_value != 0) {
        cached_global_value_ptr = *(int **)(background_ship_data + 0xB4);
        if (current_global_value != *cached_global_value_ptr) {
            *cached_global_value_ptr = current_global_value;
            add_draw_callback(draw_background_ship_1_joint_billboards, moby);
        }
    }
}

/* Update for a background ship moby: eases its attach point along a path, spawns effects and drives its state machine. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002ED8F0), where it is exact; names translated to the US level program. */
void background_ship_1_update(char *moby) __asm__("FUN_L02_002ec4b8");

/* Moby class 1212 */
void background_ship_1_update(char *moby) {
    enum {
        BACKGROUND_SHIP_1_INITIALIZE = 0,
        BACKGROUND_SHIP_1_FOLLOW_PATH = 1,
        BACKGROUND_SHIP_1_REMOVE = 2
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
    case BACKGROUND_SHIP_1_INITIALIZE: {
        int invalid_index;
        MOBY(moby)->update_dist = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        invalid_index = -1;
        if (*(int *)(moby_data + 0x60) == invalid_index) {
            debug_print(background_ship_1_missing_path_message, ((short)MOBY(moby)->save_id));
            mark_moby_for_removal(moby);
            return;
        }
        if (*(int *)D_L02_001B0AB0[*(int *)(moby_data + 0x60)] == 0) {
            debug_print(background_ship_1_empty_path_message, ((short)MOBY(moby)->save_id));
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
    case BACKGROUND_SHIP_1_FOLLOW_PATH:
        qcopy(tv, moby + 0x10);
        if (voice_is_playing(moby, *(int *)(moby_data + 0x6C)) == 0) {
            *(int *)(moby_data + 0x6C) = func_0022ED80_r(1, 4, (int)moby);
        }
        path_travel_distance = *(float *)&background_ship_1_path_travel_speed_gp * frame_time;
        *(float *)(moby_data + 0x64) =
            *(float *)(moby_data + 0x64) + path_travel_distance / *(float *)(moby_data + 0x70);
        if ((float)*(int *)path < *(float *)(moby_data + 0x64)) {
            *(float *)(moby_data + 0x64) = *(float *)(moby_data + 0x64) - (float)*(int *)path;
        }
        if (current_level_index == 9) {
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
            spawn_background_ship_1_joint_sparks(moby, tv);
        }
        if (hit_record != 0 && 0.0f < *(float *)(hit_record + 0x2C)) {
            flag = 1;
        }
        if (flag) {
            MOBY(moby)->state = 2;
        }
        break;
    case BACKGROUND_SHIP_1_REMOVE:
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
            if (*(int *)(camera_secondary_mode) == 0x32) {
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
        func_0022ED80_r(0, 0, (int)moby);
        fast_vec_scale(
            tv, moby + 0xC0, background_ship_1_removal_effect_vector_scale_gp * D_0015ED60);
        tv[2] = tv[2] + background_ship_1_removal_effect_vector_z_bias_gp * D_0015ED60;
        first_effect_frame_count = scale_game_frames(90);
        FUN_L00_00263fd8(moby,
                         0x5F2,
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
                         0x5F5,
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
                         0x790,
                         (float *)&MOBY(moby)->pos,
                         (void *)&MOBY(moby)->rot,
                         third_effect_frame_count,
                         0,
                         frame_time_sq * 12.0f,
                         tv,
                         &D_L02_0015F580,
                         &D_L02_0015F580);
        mark_moby_for_removal(moby);
        break;
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
            tweened_color = fast_tween_color(*(int *)&background_ship_1_color_tween_start_gp,
                                             *(int *)&background_ship_1_color_tween_end_gp,
                                             x);
            k = *(float *)&background_ship_1_color_phase_rate_degrees_gp * 0.017453292f *
                frame_time;
            MOBY(moby)->glow_color = tweened_color;
            *(float *)(moby_data + 0x68) = fast_add_rotations(*(float *)(moby_data + 0x68), k);
        }
        prepare_background_ship_1_joint_billboards(moby);
    }
}

/* Spawns three particles around a moby with randomized direction, speed and colour. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002EDE68), where it is exact; names translated to the US level program. */
void spawn_background_ship_1_joint_sparks(char *moby, void *previous_position) {
    float first_spark_vector[4], second_spark_vector[4], position_delta[4], random_offset[4],
        emission_position[4];
    int i;
    fast_vec_sub(position_delta, moby + 0x10, previous_position);
    for (i = 0; i < 3; i++) {
        int first_spark_color, second_spark_color, frame_count, second_frame_count,
            third_frame_count;
        char *spark_part;
        moby_point_position(moby, i, emission_position);
        fast_vec_scale(first_spark_vector,
                       position_delta,
                       *(float *)&background_ship_1_first_spark_position_delta_scale_gp);
        fast_vec_scale(second_spark_vector,
                       position_delta,
                       *(float *)&background_ship_1_second_spark_position_delta_scale_gp);
        random_vector(random_offset,
                      0.0f,
                      *(float *)&background_ship_1_spark_random_offset_radius_scale_gp *
                          frame_time);
        fast_vec_add(second_spark_vector, second_spark_vector, random_offset);
        first_spark_vector[3] = random_float_between(
            *(float *)&background_ship_1_first_spark_fourth_component_range_start_gp,
            *(float *)&background_ship_1_first_spark_fourth_component_range_end_gp);
        second_spark_vector[3] = random_float_between(
            *(float *)&background_ship_1_second_spark_fourth_component_range_start_gp,
            *(float *)&background_ship_1_second_spark_fourth_component_range_end_gp);
        first_spark_color =
            fast_tween_color(*(int *)&background_ship_1_first_spark_color_tween_start_gp,
                             *(int *)&background_ship_1_first_spark_color_tween_end_gp,
                             random_float_between(0.0f, 1.0f));
        second_spark_color =
            fast_tween_color(*(int *)&background_ship_1_second_spark_color_tween_start_gp,
                             *(int *)&background_ship_1_second_spark_color_tween_end_gp,
                             random_float_between(0.0f, 1.0f));
        frame_count = truncate_float_to_s32(
            scale_time((float)*(int *)&background_ship_1_spark_frame_count_random_span_gp *
                           random_float_between(0.0f, 1.0f) +
                       1.0f));
        second_frame_count = truncate_float_to_s32(
            scale_time((float)*(int *)&background_ship_1_spark_second_frame_count_base_gp *
                       (random_float_between(
                            -*(float *)&background_ship_1_spark_frame_count_variation_fraction_gp,
                            *(float *)&background_ship_1_spark_frame_count_variation_fraction_gp) +
                        1.0f)));
        third_frame_count = truncate_float_to_s32(
            scale_time((float)*(int *)&background_ship_1_spark_third_frame_count_base_gp *
                       (random_float_between(
                            -*(float *)&background_ship_1_spark_frame_count_variation_fraction_gp,
                            *(float *)&background_ship_1_spark_frame_count_variation_fraction_gp) +
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
        if (spark_part != 0)
            spark_part[9] = truncate_float_to_s32(8.0f) + 0x60;
    }
}

short background_ship_1_color_tween_start_gp __asm__("D_L02_00161F74") = 0x2020;
short background_ship_1_color_tween_end_gp __asm__("D_L02_00161F78") = 0x30c0;
short background_ship_1_first_spark_color_tween_start_gp __asm__("D_L02_00161FB0") = -32513;
short background_ship_1_first_spark_color_tween_end_gp __asm__("D_L02_00161FB4") = -24384;
short background_ship_1_second_spark_color_tween_start_gp __asm__("D_L02_00161FB8") = 0x2040;
short background_ship_1_second_spark_color_tween_end_gp __asm__("D_L02_00161FBC") = 0x1020;

float background_ship_1_color_phase_rate_degrees_gp __asm__("D_L02_00161F70") = 3.6e+02f;

extern short background_ship_1_first_spark_position_delta_scale_gp __asm__("D_L02_00161F84")
    __attribute__((sda));

extern short background_ship_1_second_spark_position_delta_scale_gp __asm__("D_L02_00161F88")
    __attribute__((sda));

extern short background_ship_1_spark_random_offset_radius_scale_gp __asm__("D_L02_00161F8C")
    __attribute__((sda));

extern short background_ship_1_spark_frame_count_random_span_gp __asm__("D_L02_00161F90")
    __attribute__((sda));

extern short background_ship_1_spark_second_frame_count_base_gp __asm__("D_L02_00161F94")
    __attribute__((sda));

extern short background_ship_1_spark_third_frame_count_base_gp __asm__("D_L02_00161F98")
    __attribute__((sda));

extern short background_ship_1_spark_frame_count_variation_fraction_gp __asm__("D_L02_00161F9C")
    __attribute__((sda));

extern short background_ship_1_first_spark_fourth_component_range_start_gp __asm__("D_L02_00161FA0")
    __attribute__((sda));

extern short background_ship_1_first_spark_fourth_component_range_end_gp __asm__("D_L02_00161FA4")
    __attribute__((sda));

extern short background_ship_1_second_spark_fourth_component_range_end_gp __asm__("D_L02_00161FAC")
    __attribute__((sda));
