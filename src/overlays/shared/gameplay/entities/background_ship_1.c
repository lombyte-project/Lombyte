/* Shared moby classes: background_ship_1, background_ship_2, white_flower. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"

#define NOT_SDA

#define MACRO_ADDR

/* Walks the moby list matching this moby's id and pulses each match's data slots while it is bright. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002ED740), where it is exact; names translated to the US level program. */

extern int get_first_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002591d0");
extern int get_next_moby_in_group(int *, int, int, int) __asm__("FUN_L00_002592b8");
extern void draw_offset_textured_effect_quad(void *, int, float, float) __asm__("FUN_L00_00263618");

void draw_background_ship_1_joint_billboards(char *self) __asm__("FUN_L02_002ec308");

void draw_background_ship_1_joint_billboards(char *self) {
    char *current_moby;
    get_first_moby_in_group((int *)&current_moby, *(unsigned char *)(self + 0x21), 0, 0);
    while (current_moby != 0) {
        if (*(unsigned char *)(current_moby + 0x31) != 0) {
            char *data = *(char **)(current_moby + 0x78);
            if (*(short *)(current_moby + 0xA6) == *(short *)(self + 0xA6)) {
                if ((*(int *)(data + 0xB0) & 0xFF) > 0x80) {
                    char *joint_position = data + 0x80;
                    int i;
                    for (i = 2; i >= 0; i--) {
                        draw_offset_textured_effect_quad(joint_position, *(int *)(data + 0xB0), 0.9f, 0.45f);
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

extern int D_L02_0015F5CC;
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void enqueue_callback_list_1(void (*)(void), void *) __asm__("FUN_001f4600");
void draw_background_ship_1_joint_billboards(char *self) __asm__("FUN_L02_002ec308");

void prepare_background_ship_1_joint_billboards(char *moby) __asm__("FUN_L02_002ec3f0");

void prepare_background_ship_1_joint_billboards(char *moby) {
    int i;
    char *data;
    char *joint_position_out;
    int current_global_value;
    int r, g, b, packed_color;
    int *cached_global_value_ptr;

    if (*(unsigned char *)(moby + 0x21) == 0xFF) {
        return;
    }
    data = *(char **)(moby + 0x78);
    joint_position_out = data + 0x80;
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
    *(int *)(data + 0xB0) = packed_color;
    if (current_global_value != 0) {
        cached_global_value_ptr = *(int **)(data + 0xB4);
        if (current_global_value != *cached_global_value_ptr) {
            *cached_global_value_ptr = current_global_value;
            enqueue_callback_list_1(draw_background_ship_1_joint_billboards, moby);
        }
    }
}
#include "qcopy.h"

/* Update for a background ship moby: eases its attach point along a path, spawns effects and drives its state machine. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002ED8F0), where it is exact; names translated to the US level program. */

extern char *D_L02_001B0AB0[];
extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char D_001413D4[];
extern char D_L02_001FAD60[];
extern char D_L02_001FAD80[];
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 random_angle_radians(void) __asm__("func_00213308");
#include "rnc/overlay/shared/gameplay/entities/background_ship_1_globals.h"
#include "rnc/overlay/shared/gameplay/entities/background_ship_1_globals.h"
extern int stub_printf() __asm__("FUN_001e93b0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern float D_0015ED60;
extern float D_L02_0015F580;
extern float background_ship_1_path_travel_speed_gp __asm__("D_L02_00161F64") __attribute__((sda));
extern float background_ship_1_color_phase_rate_degrees_gp __asm__("D_L02_00161F70") __attribute__((sda));
extern short D_L02_00161F74 __attribute__((sda));
extern short D_L02_00161F78 __attribute__((sda));
extern unsigned char D_0013D408[];
extern void *FUN_L00_00263fd8(char *, int, float *, void *, int, int, float, float *, float *, float *);
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern void spawn_background_ship_1_joint_sparks(char *m, void *arg1) __asm__("FUN_L02_002eca30");
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void delete_moby(void *) __asm__("func_0020C828");
extern void sample_camera_path(void *, s32, void *, void *, s32, f32) __asm__("func_00214E58");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
s32 allocate_voice_for_bank_entry(s32 entry_index, s32 flags, s32 owner) __asm__("FUN_0022db10");
void prepare_background_ship_1_joint_billboards(char *moby) __asm__("FUN_L02_002ec3f0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int func_0022ED80_r(int, int, int) __asm__("FUN_0022da68");

void background_ship_1_update(char *moby) __asm__("FUN_L02_002ec4b8");
void background_ship_1_update(char *moby) {
    enum {
        BACKGROUND_SHIP_1_INITIALIZE = 0,
        BACKGROUND_SHIP_1_FOLLOW_PATH = 1,
        BACKGROUND_SHIP_1_REMOVE = 2
    };
    char *moby_data;
    char *path;
    int hit_record;
    int flag = 0;
    float tv[4];
    float first_path_segment_length;
    float path_travel_distance;
    float x;
    int first_effect_frame_count;
    int second_effect_frame_count;
    int third_effect_frame_count;
    moby_data = *(char **)(moby + 0x78);
    path = D_L02_001B0AB0[*(int *)(moby_data + 0x60)];
    hit_record = (int)find_moby_hit(moby, 0x10000, 0);
    switch (*(unsigned char *)(moby + 0x20)) {
    case BACKGROUND_SHIP_1_INITIALIZE: {
        int invalid_index;
        *(unsigned char *)(moby + 0x30) = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        invalid_index = -1;
        if (*(int *)(moby_data + 0x60) == invalid_index) {
            stub_printf(D_L02_001FAD60, *(short *)(moby + 0xB2));
            delete_moby(moby);
            return;
        }
        if (*(int *)D_L02_001B0AB0[*(int *)(moby_data + 0x60)] == 0) {
            stub_printf(D_L02_001FAD80, *(short *)(moby + 0xB2));
            delete_moby(moby);
            return;
        }
        first_path_segment_length = distance_xyz(D_L02_001B0AB0[*(int *)(moby_data + 0x60)] + 0x10, D_L02_001B0AB0[*(int *)(moby_data + 0x60)] + 0x20);
        *(unsigned char *)(moby_data + 0x2C) = 0x28;
        *(float *)(moby_data + 0x70) = first_path_segment_length;
        *(unsigned char *)(moby_data + 0x2B) = 1;
        qcopy(moby + 0x10, D_L02_001B0AB0[*(int *)(moby_data + 0x60)] + 0x10);
        *(float *)(moby_data + 0x68) = random_angle_radians();
        *(int *)(moby_data + 0x6C) = invalid_index;
        *(unsigned char *)(moby + 0x20) = 1;
        *(unsigned short *)(moby + 0x34) |= 0x10;
        *(float *)(moby_data + 0x64) = *(float *)(moby_data + 0x74) * (float)*(int *)path;
        break;
    }
    case BACKGROUND_SHIP_1_FOLLOW_PATH:
        qcopy(tv, moby + 0x10);
        if (voice_is_playing(moby, *(int *)(moby_data + 0x6C)) == 0) {
            *(int *)(moby_data + 0x6C) = func_0022ED80_r(1, 4, (int)moby);
        }
        path_travel_distance = *(float *)&background_ship_1_path_travel_speed_gp * frame_time;
        *(float *)(moby_data + 0x64) = *(float *)(moby_data + 0x64) + path_travel_distance / *(float *)(moby_data + 0x70);
        if ((float)*(int *)path < *(float *)(moby_data + 0x64)) {
            *(float *)(moby_data + 0x64) = *(float *)(moby_data + 0x64) - (float)*(int *)path;
        }
        if (current_level_index == 9) {
            sample_camera_path((int *)path, 1, moby + 0x10, (float *)(moby + 0x40), 0, *(float *)(moby_data + 0x64));
        } else {
            sample_camera_path((int *)path, 0, moby + 0x10, (float *)(moby + 0x40), 0, *(float *)(moby_data + 0x64));
        }
        if (*(float *)(moby + 0x10) < 8.0f) {
            *(int *)(moby + 0x94) = 0;
        } else if (*(float *)(moby + 0x14) < 8.0f) {
            *(int *)(moby + 0x94) = 0;
        } else {
            *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
        }
        if (*(unsigned char *)(moby + 0x31) != 0) {
            spawn_background_ship_1_joint_sparks(moby, tv);
        }
        if (hit_record != 0 && 0.0f < *(float *)(hit_record + 0x2C)) {
            flag = 1;
        }
        if (flag) {
            *(unsigned char *)(moby + 0x20) = 2;
        }
        break;
    case BACKGROUND_SHIP_1_REMOVE:
        if (current_level_index == 2) {
            level_2_ship_removal_count = level_2_ship_removal_count + 1;
            if (level_2_ship_removal_count >= 3) {
                unsigned char *p = D_0013D408;
                if (p[2] == 0) {
                    p[2] = 1;
                    allocate_voice_for_bank_entry(1, 0, 0);
                    queue_dialogue_message(0x53D6, -1);
                }
            }
        }
        if (current_level_index == 9) {
            if (*(int *)(D_001413D4) == 0x32) {
                level_9_ship_removal_count = level_9_ship_removal_count + 1;
                if (level_9_ship_removal_count >= 5) {
                    unsigned char *p = D_0013D408;
                    if (p[0xF] == 0) {
                        p[0xF] = 1;
                        allocate_voice_for_bank_entry(1, 0, 0);
                        queue_dialogue_message(0x53D6, -1);
                    }
                }
            }
        }
        func_0022ED80_r(0, 0, (int)moby);
        scale_vector_xyz(tv, moby + 0xC0, background_ship_1_removal_effect_vector_scale_gp * D_0015ED60);
        tv[2] = tv[2] + background_ship_1_removal_effect_vector_z_bias_gp * D_0015ED60;
        first_effect_frame_count = scale_game_frames(90);
        FUN_L00_00263fd8(moby, 0x5F2, (float *)(moby + 0x10), (void *)(moby + 0x40), first_effect_frame_count, 0, frame_time_sq * 12.0f, tv, &D_L02_0015F580, &D_L02_0015F580);
        second_effect_frame_count = scale_game_frames(90);
        FUN_L00_00263fd8(moby, 0x5F5, (float *)(moby + 0x10), (void *)(moby + 0x40), second_effect_frame_count, 0, frame_time_sq * 12.0f, tv, &D_L02_0015F580, &D_L02_0015F580);
        third_effect_frame_count = scale_game_frames(90);
        FUN_L00_00263fd8(moby, 0x790, (float *)(moby + 0x10), (void *)(moby + 0x40), third_effect_frame_count, 0, frame_time_sq * 12.0f, tv, &D_L02_0015F580, &D_L02_0015F580);
        delete_moby(moby);
        break;
    default:
        break;
    }
    if (*(unsigned char *)(moby + 0x31) != 0) {
        x = fast_sin(*(float *)(moby_data + 0x68)) * 4.0f - 3.0f;
        if (1.0f < x) {
            x = 1.0f;
        } else if (x < 0.0f) {
            x = 0.0f;
        }
        {
            int tweened_color;
            float k;
            tweened_color = fast_tween_color(*(int *)&D_L02_00161F74, *(int *)&D_L02_00161F78, x);
            k = *(float *)&background_ship_1_color_phase_rate_degrees_gp * 0.017453292f * frame_time;
            *(int *)(moby + 0x90) = tweened_color;
            *(float *)(moby_data + 0x68) = fast_add_rotations(*(float *)(moby_data + 0x68), k);
        }
        prepare_background_ship_1_joint_billboards(moby);
    }
}
/* Spawns three particles around a moby with randomized direction, speed and colour. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002EDE68), where it is exact; names translated to the US level program. */

extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern short D_L02_00161F84 __attribute__((sda));
extern short D_L02_00161F88 __attribute__((sda));
extern short D_L02_00161F8C __attribute__((sda));
extern short D_L02_00161F90 __attribute__((sda));
extern short D_L02_00161F94 __attribute__((sda));
extern short D_L02_00161F98 __attribute__((sda));
extern short D_L02_00161F9C __attribute__((sda));
extern short D_L02_00161FA0 __attribute__((sda));
extern short D_L02_00161FA4 __attribute__((sda));
extern short D_L02_00161FA8 __attribute__((sda));
extern short D_L02_00161FAC __attribute__((sda));
extern short D_L02_00161FB0 __attribute__((sda));
extern short D_L02_00161FB4 __attribute__((sda));
extern short D_L02_00161FB8 __attribute__((sda));
extern short D_L02_00161FBC __attribute__((sda));
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void spawn_background_ship_1_joint_sparks(char *moby, void *previous_position) {
    float first_spark_vector[4], second_spark_vector[4], position_delta[4], random_offset[4], emission_position[4];
    int i;
    fast_vec_sub(position_delta, moby + 0x10, previous_position);
    for (i = 0; i < 3; i++) {
        int first_spark_color, second_spark_color, frame_count, second_frame_count, third_frame_count;
        char *spark_part;
        moby_point_position(moby, i, emission_position);
        fast_vec_scale(first_spark_vector, position_delta, *(float *)&D_L02_00161F84);
        fast_vec_scale(second_spark_vector, position_delta, *(float *)&D_L02_00161F88);
        random_vector(random_offset, 0.0f, *(float *)&D_L02_00161F8C * frame_time);
        add_vector_xyz(second_spark_vector, second_spark_vector, random_offset);
        first_spark_vector[3] = random_float_between(*(float *)&D_L02_00161FA0, *(float *)&D_L02_00161FA4);
        second_spark_vector[3] = random_float_between(*(float *)&D_L02_00161FA8, *(float *)&D_L02_00161FAC);
        first_spark_color = fast_tween_color(*(int *)&D_L02_00161FB0, *(int *)&D_L02_00161FB4,
                         random_float_between(0.0f, 1.0f));
        second_spark_color = fast_tween_color(*(int *)&D_L02_00161FB8, *(int *)&D_L02_00161FBC,
                         random_float_between(0.0f, 1.0f));
        frame_count = func_001FA898_r(multiply_global_factor_ed64(
            (float)*(int *)&D_L02_00161F90 * random_float_between(0.0f, 1.0f) + 1.0f));
        second_frame_count = func_001FA898_r(multiply_global_factor_ed64(
            (float)*(int *)&D_L02_00161F94 *
            (random_float_between(-*(float *)&D_L02_00161F9C, *(float *)&D_L02_00161F9C) +
             1.0f)));
        third_frame_count = func_001FA898_r(multiply_global_factor_ed64(
            (float)*(int *)&D_L02_00161F98 *
            (random_float_between(-*(float *)&D_L02_00161F9C, *(float *)&D_L02_00161F9C) +
             1.0f)));
        spark_part = spawn_spark(emission_position, first_spark_vector, second_spark_vector, first_spark_color, second_spark_color, frame_count, second_frame_count, third_frame_count, -1);
        if (spark_part != 0)
            spark_part[9] = func_001FA898_r(8.0f) + 0x60;
    }
}
void draw_background_ship_1_joint_billboards(char *self) __asm__("FUN_L02_002ec308");
void draw_matching_moby_slot_quads(char *self) __asm__("FUN_L02_002ecc68");

void prepare_background_ship_2_joint_draw_data(char *moby) __asm__("FUN_L02_002ecd50");

void prepare_background_ship_2_joint_draw_data(char *moby) {
    int i;
    char *data;
    char *joint_position_out;
    int current_global_value;
    int r, g, b, packed_color;
    int *cached_global_value_ptr;

    if (*(unsigned char *)(moby + 0x21) == 0xFF) {
        return;
    }
    data = *(char **)(moby + 0x78);
    joint_position_out = data + 0x80;
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
    *(int *)(data + 0xE0) = packed_color;
    if (current_global_value != 0) {
        cached_global_value_ptr = *(int **)(data + 0xE4);
        if (current_global_value != *cached_global_value_ptr) {
            *cached_global_value_ptr = current_global_value;
            enqueue_callback_list_1(draw_matching_moby_slot_quads, moby);
        }
    }
}
/* Ported from rac1-decomp src/overlays/shared/vendor_002A5218.c (func_L02_002EE250) */

/* Update for a ship moby: eases its attach point along a path, spawns effects and drives its state machine. */

extern float background_ship_2_path_travel_speed_gp __asm__("D_L02_00161FC0") __attribute__((sda));
extern char D_L02_001FADB0[];
extern char D_L02_001FADD0[];
extern float D_0015ED60_f __asm__("D_0015ED60");
extern float background_ship_2_color_phase_rate_degrees_gp __asm__("D_L02_00161FC4") __attribute__((sda));
extern int background_ship_2_color_tween_start_gp __asm__("D_L02_00161FC8") __attribute__((sda));
extern int background_ship_2_color_tween_end_gp __asm__("D_L02_00161FCC") __attribute__((sda));
extern void prepare_background_ship_2_joint_draw_data(char *m) __asm__("FUN_L02_002ecd50");
extern void spawn_background_ship_2_joint_sparks(char *m, void *arg1) __asm__("FUN_L02_002ed420");
extern void spawn_background_ship_2_level_10_joint_sparks(char *moby, void *arg) __asm__("FUN_L02_002ed658");
s32 allocate_voice_for_bank_entry(s32 entry_index, s32 flags, s32 owner) __asm__("FUN_0022db10");
extern char D_001413F4[];

void background_ship_2_update(char *moby) __asm__("FUN_L02_002ece18");
void background_ship_2_update(char *moby) {
    enum {
        BACKGROUND_SHIP_2_INITIALIZE = 0,
        BACKGROUND_SHIP_2_FOLLOW_PATH = 1,
        BACKGROUND_SHIP_2_REMOVE = 2
    };
    char *moby_data;
    char *path;
    int hit_record;
    int flag = 0;
    float tv[4];
    float first_path_segment_length;
    float path_travel_distance;
    float x;
    int first_effect_frame_count;
    int second_effect_frame_count;
    int third_effect_frame_count;
    moby_data = *(char **)(moby + 0x78);
    path = D_L02_001B0AB0[*(int *)(moby_data + 0x60)];
    hit_record = (int)find_moby_hit(moby, 0x10000, 0);
    switch (*(unsigned char *)(moby + 0x20)) {
    case BACKGROUND_SHIP_2_INITIALIZE: {
        int invalid_index;
        *(unsigned char *)(moby + 0x30) = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        if (current_level_index == 10) {
            *(float *)(moby + 0x2C) = *(float *)(moby + 0x2C) * 0.5f;
        }
        invalid_index = -1;
        if (*(int *)(moby_data + 0x60) == invalid_index) {
            stub_printf(D_L02_001FADB0, *(short *)(moby + 0xB2));
            delete_moby(moby);
            return;
        }
        if (*(int *)D_L02_001B0AB0[*(int *)(moby_data + 0x60)] == 0) {
            stub_printf(D_L02_001FADD0, *(short *)(moby + 0xB2));
            delete_moby(moby);
            return;
        }
        first_path_segment_length = distance_xyz(D_L02_001B0AB0[*(int *)(moby_data + 0x60)] + 0x10, D_L02_001B0AB0[*(int *)(moby_data + 0x60)] + 0x20);
        *(unsigned char *)(moby_data + 0x2C) = 0x28;
        *(float *)(moby_data + 0x70) = first_path_segment_length;
        *(unsigned char *)(moby_data + 0x2B) = 1;
        qcopy(moby + 0x10, D_L02_001B0AB0[*(int *)(moby_data + 0x60)] + 0x10);
        *(float *)(moby_data + 0x68) = random_angle_radians();
        *(int *)(moby_data + 0x6C) = invalid_index;
        *(unsigned char *)(moby + 0x20) = 1;
        *(unsigned short *)(moby + 0x34) |= 0x10;
        *(float *)(moby_data + 0x64) = *(float *)(moby_data + 0x74) * (float)*(int *)path;
        break;
    }
    case BACKGROUND_SHIP_2_FOLLOW_PATH:
        if (*(unsigned char *)D_001413F4 == 1 && current_level_index == 10) {
            delete_moby(moby);
            return;
        }
        qcopy(tv, moby + 0x10);
        if (voice_is_playing(moby, *(int *)(moby_data + 0x6C)) == 0) {
            *(int *)(moby_data + 0x6C) = func_0022ED80_r(1, 4, (int)moby);
        }
        path_travel_distance = background_ship_2_path_travel_speed_gp * frame_time;
        if (current_level_index == 10) {
            path_travel_distance = path_travel_distance * 0.25f;
        }
        *(float *)(moby_data + 0x64) = *(float *)(moby_data + 0x64) + path_travel_distance / *(float *)(moby_data + 0x70);
        if ((float)*(int *)path < *(float *)(moby_data + 0x64)) {
            *(float *)(moby_data + 0x64) = *(float *)(moby_data + 0x64) - (float)*(int *)path;
        }
        if ((unsigned int)(current_level_index - 9) < 2) {
            sample_camera_path((int *)path, 1, moby + 0x10, (float *)(moby + 0x40), 0, *(float *)(moby_data + 0x64));
        } else {
            sample_camera_path((int *)path, 0, moby + 0x10, (float *)(moby + 0x40), 0, *(float *)(moby_data + 0x64));
        }
        if (*(float *)(moby + 0x10) < 8.0f) {
            *(int *)(moby + 0x94) = 0;
        } else if (*(float *)(moby + 0x14) < 8.0f) {
            *(int *)(moby + 0x94) = 0;
        } else {
            *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
        }
        if (*(unsigned char *)(moby + 0x31) != 0) {
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
            *(unsigned char *)(moby + 0x20) = 2;
        }
        break;
    case BACKGROUND_SHIP_2_REMOVE:
        if (current_level_index == 2) {
            level_2_ship_removal_count = level_2_ship_removal_count + 1;
            if (level_2_ship_removal_count >= 3) {
                unsigned char *p = D_0013D408;
                if (p[2] == 0) {
                    p[2] = 1;
                    allocate_voice_for_bank_entry(1, 0, 0);
                    queue_dialogue_message(0x53D6, -1);
                }
            }
        }
        if (current_level_index == 9) {
            if (*(int *)D_001413D4 == 0x32) {
                level_9_ship_removal_count = level_9_ship_removal_count + 1;
                if (level_9_ship_removal_count >= 5) {
                    unsigned char *p = D_0013D408;
                    if (p[0xF] == 0) {
                        p[0xF] = 1;
                        allocate_voice_for_bank_entry(1, 0, 0);
                        queue_dialogue_message(0x53D6, -1);
                    }
                }
            }
        }
        func_0022ED80_r(0, 0, (int)moby);
        scale_vector_xyz(tv, moby + 0xC0, background_ship_2_removal_effect_vector_scale_gp * D_0015ED60_f);
        tv[2] = tv[2] + background_ship_2_removal_effect_vector_z_bias_gp * D_0015ED60_f;
        first_effect_frame_count = scale_game_frames(90);
        FUN_L00_00263fd8(moby, 0x5F6, (float *)(moby + 0x10), (void *)(moby + 0x40), first_effect_frame_count, 0, frame_time_sq * 12.0f, tv, &D_L02_0015F580, &D_L02_0015F580);
        second_effect_frame_count = scale_game_frames(90);
        FUN_L00_00263fd8(moby, 0x5F8, (float *)(moby + 0x10), (void *)(moby + 0x40), second_effect_frame_count, 0, frame_time_sq * 12.0f, tv, &D_L02_0015F580, &D_L02_0015F580);
        third_effect_frame_count = scale_game_frames(90);
        FUN_L00_00263fd8(moby, 0x5F9, (float *)(moby + 0x10), (void *)(moby + 0x40), third_effect_frame_count, 0, frame_time_sq * 12.0f, tv, &D_L02_0015F580, &D_L02_0015F580);
        delete_moby(moby);
        return;
    default:
        break;
    }
    if (*(unsigned char *)(moby + 0x31) != 0) {
        x = fast_sin(*(float *)(moby_data + 0x68)) * 4.0f - 3.0f;
        if (1.0f < x) {
            x = 1.0f;
        } else if (x < 0.0f) {
            x = 0.0f;
        }
        {
            int tweened_color;
            float k;
            tweened_color = fast_tween_color(background_ship_2_color_tween_start_gp, background_ship_2_color_tween_end_gp, x);
            k = background_ship_2_color_phase_rate_degrees_gp * 0.017453292f * frame_time;
            *(int *)(moby + 0x90) = tweened_color;
            *(float *)(moby_data + 0x68) = fast_add_rotations(*(float *)(moby_data + 0x68), k);
        }
        prepare_background_ship_2_joint_draw_data(moby);
    }
}
extern short D_L02_00161FD8 __attribute__((sda));
extern short D_L02_00161FDC __attribute__((sda));
extern short D_L02_00161FE0 __attribute__((sda));
extern short D_L02_00161FE4 __attribute__((sda));
extern short D_L02_00161FE8 __attribute__((sda));
extern short D_L02_00161FEC __attribute__((sda));
extern short D_L02_00161FF0 __attribute__((sda));
extern short D_L02_00161FF4 __attribute__((sda));
extern short D_L02_00161FF8 __attribute__((sda));
extern short D_L02_00161FFC __attribute__((sda));
extern short D_L02_00162000 __attribute__((sda));
extern short D_L02_00162004 __attribute__((sda));
extern short D_L02_00162008 __attribute__((sda));
extern short D_L02_0016200C __attribute__((sda));
extern short D_L02_00162010 __attribute__((sda));

void spawn_background_ship_2_joint_sparks(char *moby, void *previous_position) {
    float first_spark_vector[4], second_spark_vector[4], position_delta[4], random_offset[4], emission_position[4];
    int i;

    fast_vec_sub(position_delta, moby + 0x10, previous_position);
    for (i = 0; i < 2; i++) {
        int first_spark_color, second_spark_color, frame_count, second_frame_count, third_frame_count;
        char *spark_part;

        moby_point_position(moby, i, emission_position);
        fast_vec_scale(first_spark_vector, position_delta, *(float *)&D_L02_00161FD8);
        fast_vec_scale(second_spark_vector, position_delta, *(float *)&D_L02_00161FDC);
        random_vector(random_offset, 0.0f, *(float *)&D_L02_00161FE0 * frame_time);
        add_vector_xyz(second_spark_vector, second_spark_vector, random_offset);
        first_spark_vector[3] = random_float_between(*(float *)&D_L02_00161FF4, *(float *)&D_L02_00161FF8);
        second_spark_vector[3] = random_float_between(*(float *)&D_L02_00161FFC, *(float *)&D_L02_00162000);
        first_spark_color = fast_tween_color(*(int *)&D_L02_00162004, *(int *)&D_L02_00162008,
                         random_float_between(0.0f, 1.0f));
        second_spark_color = fast_tween_color(*(int *)&D_L02_0016200C, *(int *)&D_L02_00162010,
                         random_float_between(0.0f, 1.0f));
        frame_count = func_001FA898_r(multiply_global_factor_ed64(
            (float)*(int *)&D_L02_00161FE4 * random_float_between(0.0f, 1.0f) + 1.0f));
        second_frame_count = func_001FA898_r(multiply_global_factor_ed64(
            (float)*(int *)&D_L02_00161FE8 *
            (random_float_between(-*(float *)&D_L02_00161FF0, *(float *)&D_L02_00161FF0) +
             1.0f)));
        third_frame_count = func_001FA898_r(multiply_global_factor_ed64(
            (float)*(int *)&D_L02_00161FEC *
            (random_float_between(-*(float *)&D_L02_00161FF0, *(float *)&D_L02_00161FF0) +
             1.0f)));
        spark_part = spawn_spark(emission_position, first_spark_vector, second_spark_vector, first_spark_color, second_spark_color, frame_count, second_frame_count, third_frame_count, -1);
        if (spark_part != 0) {
            spark_part[9] = func_001FA898_r(8.0f) + 0x60;
        }
    }
}
extern short D_L02_00162014 __attribute__((sda));
extern short D_L02_00162018 __attribute__((sda));
extern short D_L02_0016201C __attribute__((sda));
extern short D_L02_00162020 __attribute__((sda));
extern short D_L02_00162024 __attribute__((sda));
extern short D_L02_00162028 __attribute__((sda));
extern short D_L02_0016202C __attribute__((sda));

void spawn_background_ship_2_level_10_joint_sparks(char *moby, void *previous_position) {
    float first_spark_vector[4], second_spark_vector[4], position_delta[4], random_offset[4], emission_position[4];
    int i;

    fast_vec_sub(position_delta, moby + 0x10, previous_position);
    for (i = 0; i < 2; i++) {
        int first_spark_color, second_spark_color, frame_count, second_frame_count, third_frame_count;
        char *spark_part;

        moby_point_position(moby, i, emission_position);
        fast_vec_scale(first_spark_vector, position_delta, *(float *)&D_L02_00162014);
        fast_vec_scale(second_spark_vector, position_delta, *(float *)&D_L02_00162018);
        random_vector(random_offset, 0.0f, *(float *)&D_L02_0016201C * frame_time);
        add_vector_xyz(second_spark_vector, second_spark_vector, random_offset);
        first_spark_vector[3] =
            random_float_between(*(float *)&D_L02_00161FF4, *(float *)&D_L02_00161FF8) / 1.5f;
        second_spark_vector[3] =
            random_float_between(*(float *)&D_L02_00161FFC, *(float *)&D_L02_00162000) / 1.5f;
        first_spark_color = fast_tween_color(*(int *)&D_L02_00162020, *(int *)&D_L02_00162024,
                         random_float_between(0.0f, 1.0f));
        second_spark_color = fast_tween_color(*(int *)&D_L02_00162028, *(int *)&D_L02_0016202C,
                         random_float_between(0.0f, 1.0f));
        frame_count = func_001FA898_r(multiply_global_factor_ed64(
            (float)*(int *)&D_L02_00161FE4 * random_float_between(0.0f, 1.0f) + 1.0f));
        second_frame_count = func_001FA898_r(multiply_global_factor_ed64(
            (float)*(int *)&D_L02_00161FE8 *
            (random_float_between(-*(float *)&D_L02_00161FF0, *(float *)&D_L02_00161FF0) +
             1.0f)));
        third_frame_count = func_001FA898_r(multiply_global_factor_ed64(
            (float)*(int *)&D_L02_00161FEC *
            (random_float_between(-*(float *)&D_L02_00161FF0, *(float *)&D_L02_00161FF0) +
             1.0f)));
        spark_part = spawn_spark(emission_position, first_spark_vector, second_spark_vector, first_spark_color, second_spark_color, frame_count, second_frame_count, third_frame_count, -1);
        if (spark_part != 0) {
            spark_part[9] = func_001FA898_r(8.0f) + 0x60;
        }
    }
}
/* Update for a moby that waits for a float flag on a nearby object then runs a delete sequence. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002F3398), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void spawn_moby_break_fragments(void *, int, int, int, int, int, int) __asm__("FUN_L00_00263e30");
extern void spawn_proximity_gated_bolts(void *) __asm__("FUN_L01_002787a0");
extern void spawn_moby_from_placement(void *, int) __asm__("FUN_L01_00278e20");
void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

void white_flower_update(char *m) __asm__("FUN_L02_002f1f60");
void white_flower_update(char *moby) {
    enum {
        WHITE_FLOWER_INITIALIZE = 0,
        WHITE_FLOWER_WAIT_FOR_HIT = 1,
        WHITE_FLOWER_REMOVE = 2
    };
    int hit = 0;
    char *hit_record = find_moby_hit(moby, 0x10000, 0);
    switch (*(unsigned char *)(moby + 0x20)) {
    case WHITE_FLOWER_INITIALIZE:
        moby[0x20] = 1;
        break;
    case WHITE_FLOWER_WAIT_FOR_HIT:
        if (hit_record != 0 && *(float *)(hit_record + 0x2C) > 0.0f)
            hit = 1;
        if (hit)
            moby[0x20] = 2;
        break;
    case WHITE_FLOWER_REMOVE:
        allocate_voice_for_target_entry(0, 0, (int)moby);
        spawn_proximity_gated_bolts(moby);
        spawn_moby_break_fragments(moby, 0x71D, 1, 0x71D, 1, 5, 2);
        spawn_moby_from_placement(moby, 0x71C);
        mark_moby_for_removal(moby);
        break;
    }
}
/* Store the selected value only while this level object is inactive. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002F79A8), where it is exact; names translated to the US level program. */

extern char *D_L02_00167400;

void set_follow_camera_control_word(int value) __asm__("FUN_L02_002f6570");

void set_follow_camera_control_word(int value) {
    char *moby = D_L02_00167400;
    if (*(short *)(moby + 0x86) == 0) {
        char *data = *(char **)(moby + 0x70);
        *(int *)(data + 0x218) = value;
    }
}
/* Update the active object's coordinates without replacing zero fields. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002F79D0), where it is exact; names translated to the US level program. */

extern char *D_L02_00167400_d __asm__("D_L02_00167400") __attribute__((section(".data")));

void set_nonzero_follow_camera_parameters(float x, float y, float z) __asm__("FUN_L02_002f6598");

void set_nonzero_follow_camera_parameters(float x, float y, float z) __asm__("FUN_L02_002f6598");
void set_nonzero_follow_camera_parameters(float x, float y, float z) {
    char *moby = D_L02_00167400_d;
    if (*(short *)(moby + 0x86) == 0) {
        char *data = *(char **)(moby + 0x70) + 0x1D0;
        if (*(float *)(data + 0x3C) != 0.0f) {
            *(float *)(data + 0x3C) = x;
        }
        if (*(float *)(data + 0x40) != 0.0f) {
            *(float *)(data + 0x40) = y;
        }
        if (*(float *)(data + 0x44) != 0.0f) {
            *(float *)(data + 0x44) = z;
        }
    }
}
#include "sda.h"

/* stores two coordinate triples in the selected object data */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002F9ED8), where it is exact; names translated to the US level program. */

void set_selected_object_float_triplets(float a, float b, float c, float d, float e, float f) __asm__("FUN_L02_002f8aa0");
void set_selected_object_float_triplets(float first_triplet_middle, float first_triplet_first, float first_triplet_last, float second_triplet_middle, float second_triplet_first, float second_triplet_last) {
    char *moby = D_L02_00167400_d;
    char *data = *(char **)(moby + 0x70);
    char *second_triplet_store_base = data + 0x30;
    *(float *)(second_triplet_store_base + 0x20) = second_triplet_first;
    *(float *)(second_triplet_store_base + 0x24) = second_triplet_middle;
    *(float *)(second_triplet_store_base + 0x28) = second_triplet_last;
    {
        char *other = *(char **)(moby + 0x70);
        *(float *)(other + 0x10) = first_triplet_first;
        *(float *)(other + 0x14) = first_triplet_middle;
        *(float *)(other + 0x18) = first_triplet_last;
    }
}
typedef struct {
    int a;
    int b;
    float c;
    int d;
    void (*e)(void);
} S_2fa6d8;

extern S_2fa6d8 D_L02_00167600_2fa6d8 __asm__("D_L02_00167600");
extern void apply_camera_offset_effect(void) __asm__("FUN_L02_002fa710");

void initialize_camera_offset_effect_state(void) __asm__("FUN_L02_002fa6d8");

void initialize_camera_offset_effect_state(void) {
    D_L02_00167600_2fa6d8.a = 0;
    D_L02_00167600_2fa6d8.b = 0;
    D_L02_00167600_2fa6d8.c = 10000.0f;
    D_L02_00167600_2fa6d8.d = 0;
    D_L02_00167600_2fa6d8.e = apply_camera_offset_effect;
}
/* Start the timed effect while its duration is nonzero. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002FBB48), where it is exact; names translated to the US level program. */

extern float D_L02_0016760C __attribute__((section(".data")));
extern void follow_camera_set_offset_scale(int, float, float) __asm__("FUN_L00_002e84f0");
extern void register_camera_update_callback(void (*)(void)) __asm__("FUN_L02_001fc008");
void update_camera_offset_effect_state(void) __asm__("FUN_L02_002fa770");

extern void update_camera_offset_effect_state(void) __asm__("FUN_L02_002fa770");

void apply_camera_offset_effect(void) {
    if (D_L02_0016760C != 0.0f) {
        follow_camera_set_offset_scale(1, D_L02_0016760C * 0.35f + 0.0f, 0.005f);
        register_camera_update_callback(update_camera_offset_effect_state);
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002FBBA8), where it is exact; names translated to the US level program. */

typedef struct {
    int a;
    int b;
    float c;
    float d;
} S;

extern S D_L02_00167600;
extern short D_L02_0016238C_gp __asm__("D_L02_0016238C") __attribute__((sda));

void update_camera_offset_effect_state(void) {
    S *effect_state = &D_L02_00167600;
    effect_state->c = 10000.0f;
    if (effect_state->b == 0) {
        float decremented_duration = effect_state->d - *(float *)&D_L02_0016238C_gp;
        effect_state->d = decremented_duration;
        if (decremented_duration < 0.0f) {
            effect_state->d = 0.0f;
        }
    } else {
        effect_state->b = 0;
    }
    effect_state->a = 0;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002FBC00), where it is exact; names translated to the US level program. */

extern char D_0013CA44[];
extern char D_L02_00167280_c[] __asm__("D_L02_00167280");
extern f32 fast_arc_sin(f32) __asm__("func_001F9DF8");
extern f32 dot_vectors_xyz(void *a, void *b) __asm__("FUN_001f9ab0");
extern f32 vector_length_xyz(void *a) __asm__("FUN_001f9af0");
extern short D_L02_0016238C;
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");

void try_select_camera_offset_effect_moby(char *candidate_moby) __asm__("FUN_L02_002fa7c8");

void try_select_camera_offset_effect_moby(char *candidate_moby) {
    float candidate_displacement[4];
    char *camera_selection_context = D_L02_00167280_c;
    char *selection_reference = *(char **)(camera_selection_context + 0x180);
    char *candidate_selection_state;
    float vector_distance;
    float accumulated_amount;
    float candidate_reference_angle_radians;
    if (*(short *)(selection_reference + 0x86) == 0) {
        if (!(0.05f < absolute_float(*(float *)(D_0013CA44)))) {
            candidate_selection_state = camera_selection_context + 0x380;
            subtract_vector_xyz(candidate_displacement, candidate_moby + 0x10, &hero.motion.pos);
            vector_distance = vector_length_xyz(candidate_displacement);
            if (vector_distance < 22.0f) {
                if (0.0f < vector_distance) {
                    if (vector_distance < *(float *)(candidate_selection_state + 8)) {
                        if (0.0f < candidate_displacement[2]) {
                            candidate_reference_angle_radians = 1.5707964f - fast_arc_sin(dot_vectors_xyz(selection_reference, candidate_displacement) / vector_distance);
                            if (candidate_reference_angle_radians < 1.1170107f) {
                                subtract_vector_xyz(candidate_displacement, candidate_moby + 0x10, selection_reference + 0x30);
                                vector_distance = vector_length_xyz(candidate_displacement);
                                if (vector_distance == 0.0f) {
                                    vector_distance = 0.0001f;
                                }
                                candidate_reference_angle_radians = 1.5707964f - fast_arc_sin(dot_vectors_xyz(selection_reference, candidate_displacement) / vector_distance);
                                if (candidate_reference_angle_radians < 0.62831855f) {
                                    accumulated_amount = *(float *)(candidate_selection_state + 0xC) + *(float *)&D_L02_0016238C;
                                    *(char **)(camera_selection_context + 0x380) = selection_reference;
                                    *(float *)(candidate_selection_state + 8) = vector_distance;
                                    *(char **)(candidate_selection_state + 4) = candidate_moby;
                                    *(float *)(candidate_selection_state + 0xC) = accumulated_amount;
                                    if (1.0f < accumulated_amount) {
                                        *(float *)(candidate_selection_state + 0xC) = 1.0f;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
extern char *D_L02_0015EF50;
extern char *D_L02_001674DC __attribute__((section(".data")));
extern float FUN_001f9b48(void *, void *);
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern int test_point_in_transformed_unit_cylinder(void *, int) __asm__("FUN_L00_0025fa38");
extern int test_point_in_transformed_unit_sphere(void *, int) __asm__("FUN_L00_0025faf0");
extern int point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");

int test_moby_camera_control_activation(char *moby) __asm__("FUN_L02_002fb648");

int test_moby_camera_control_activation(char *moby) {
    char *special = D_L02_001674DC;
    char *data = *(char **)(D_L02_0015EF50 + *(short *)(moby + 0x84) * 32 + 0x1c);
    char *target = *(char **)(data + 0x48);
    int *entry;
    int result;

    if (special != 0 && special == target) {
        return 1;
    }
    if (*(short *)(data + 0x3c) == 6) {
        return (char *)hero.ground_moby == target;
    }
    if (*(unsigned char *)(data + 0x22) == 1) {
        return FUN_001f9b48(&hero.motion.pos, target + 0x10) < *(float *)(data + 0x24);
    }
    if (*(int *)(data + 0xc) >= 0) {
        result = is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0xc));
        if (result == 0) {
            goto rejected;
        }
        return 1;
    }
    if (*(int *)(data + 0x10) >= 0) {
        result = test_point_in_transformed_unit_cylinder(&hero.motion.pos, *(int *)(data + 0x10));
        if (result == 0) {
            goto rejected;
        }
        return 1;
    }
    if (*(int *)(data + 8) >= 0) {
        result = test_point_in_transformed_unit_sphere(&hero.motion.pos, *(int *)(data + 8));
        if (result == 0) {
            goto rejected;
        }
        return 1;
    }
    if (*(int *)(data + 0x14) < 0) {
        goto rejected;
    }
    entry = (int *)D_L02_001B0AB0[*(int *)(data + 0x14)];
    result = point_in_path_xy(&hero.motion.pos, entry + 4, *entry);
    if (result == 0) {
        goto rejected;
    }
    return 1;
rejected:
    return 0;
}
/* Checks whether the target's direction is within a and b degrees of the reference axis. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002FCBC0), where it is exact; names translated to the US level program. */

extern char *D_L02_0015EF50;
extern char D_L02_00167410[] __attribute__((section(".data")));
extern char D_L02_00167280[] __attribute__((section(".data")));
extern float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern float fast_arc_sin(float) __asm__("FUN_001f9df8");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float FUN_001f99c0(float);

int test_moby_camera_target_angle_limits(char *moby, float residual_angle_limit_degrees, float b) __asm__("FUN_L02_002fb788");

int test_moby_camera_target_angle_limits(char *moby, float residual_angle_limit_degrees, float b) {
    float target_displacement[4];
    float scaled_axis_vector[4];
    float target_axis_residual[4];
    float r[4];
    char *data;
    char *shared_axis_data = D_L02_00167410;
    char *g;
    float dot1;
    float dot2;
    float lenq;
    float lenr;
    float dot3;
    float den;

    char *moby_record_table = D_L02_0015EF50;
    char *target_moby;

    data = *(char **)(moby_record_table + *(short *)(moby + 0x84) * 32 + 0x1C);
    target_moby = *(char **)(data + 0x48);
    if (residual_angle_limit_degrees == 0.0f && b == 0.0f) {
        return 1;
    }
    g = D_L02_00167280;
    fast_vec_sub(target_displacement, target_moby + 0x10, *(char **)(g + 0x180) + 0x30);
    dot1 = fast_vec_dot(target_displacement, (shared_axis_data + 0x30));
    fast_vec_scale(scaled_axis_vector, (shared_axis_data + 0x30), dot1);
    fast_vec_sub(target_axis_residual, target_displacement, scaled_axis_vector);
    lenq = fast_vec_length(target_axis_residual);
    dot2 = fast_vec_dot(*(char **)(g + 0x180), (shared_axis_data + 0x30));
    fast_vec_scale(scaled_axis_vector, (shared_axis_data + 0x30), dot2);
    fast_vec_sub(r, *(char **)(g + 0x180), scaled_axis_vector);
    lenr = fast_vec_length(r);
    if (b != 0.0f) {
        float target_axis_angle = fast_arc_tan(lenq, dot1);
        float reference_axis_angle = fast_arc_tan(lenr, dot2);
        float axis_angle_difference = fast_difference_between_rotations(target_axis_angle, reference_axis_angle);
        if (b * DEG_TO_RAD < axis_angle_difference) {
            return 0;
        }
    }
    if (residual_angle_limit_degrees == 0.0f) {
        return 1;
    }
    dot3 = fast_vec_dot(target_axis_residual, r);
    den = lenr * lenq;
    if (den == 0.0f) {
        return 0;
    }
    if (FUN_001f99c0(1.5707964f - fast_arc_sin(dot3 / den)) < residual_angle_limit_degrees * DEG_TO_RAD) {
        return 1;
    }
    return 0;
}
#include "rnc/input/pad_state.h"

extern int D_L02_0015FFD8 MACRO_ADDR;
extern int D_L02_001ABE40[];
extern struct Hero hero_lui __asm__("D_0013F350") __attribute__((section(".data")));
extern void clear_follow_camera_volume_if_current(char *) __asm__("FUN_L00_002e8388");
extern int try_select_follow_camera_volume(char *) __asm__("FUN_L00_002e83c0");
extern void FUN_L00_002e8450(int, float, float) __asm__("FUN_L00_002e8450");
extern void FUN_L00_002e84b8(float, float) __asm__("FUN_L00_002e84b8");
extern void set_follow_camera_orbit_clamp_mode(int) __asm__("FUN_L00_002e8568");
extern void follow_camera_set_lateral_spring(float, float) __asm__("FUN_L00_002e8590");
extern void follow_camera_set_up_spring(float, float) __asm__("FUN_L00_002e85d8");
extern void FUN_L00_002e8620(void);
extern void FUN_L00_002e8648(void) __asm__("FUN_L00_002e8648");
extern void set_follow_camera_angle_blend_from_direction(float, float, float *) __asm__("FUN_L00_002e8918");
extern void set_follow_camera_angle_blend_from_position(float, float, void *) __asm__("FUN_L00_002e8970");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern float absolute_float(float);

void apply_moby_camera_control(char *moby) __asm__("FUN_L02_002fb9c8");

void apply_moby_camera_control(char *moby) {
    char *data = *(char **)(D_L02_0015EF50 + *(short *)(moby + 0x84) * 32 + 0x1c);
    char *target;
    unsigned short *group;
    int id;
    int index;
    int duration;
    int count;
    int end;
    int frames;
    int active;
    int hit;
    float scale;
    float angle;
    float heading;
    float amount;
    float heading_vector[3];

    if (*(int *)(data + 0x50) != 0) {
        *(short *)(data + 0x20) = 0;
        clear_follow_camera_volume_if_current(moby);
        return;
    }
    index = *(int *)(data + 0x44);
    if (index >= 0) {
        group = (unsigned short *)D_L02_001ABE40[index];
        if (group == 0) {
            *(short *)(data + 0x20) = -1;
            clear_follow_camera_volume_if_current(moby);
            return;
        }
        {
            char *first_group_target;
            char *group_target_candidate;
            int moby_table_base;
            {
                int e = *group;
                moby_table_base = D_L02_0015FFD8;
                first_group_target = (char *)(moby_table_base + ((e & 0x7fff) << 8));
                id = e;
            }
            if (*(signed char *)(first_group_target + 0x20) < 0) {
                do {
                    if ((id << 16) < 0) {
                        *(short *)(data + 0x20) = -1;
                        clear_follow_camera_volume_if_current(moby);
                        return;
                    }
                    group++;
                    {
                        int e = *group;
                        moby_table_base = D_L02_0015FFD8;
                        group_target_candidate = (char *)(moby_table_base + ((e & 0x7fff) << 8));
                        id = e;
                    }
                } while (*(signed char *)(group_target_candidate + 0x20) < 0);
                target = group_target_candidate;
            } else {
                target = first_group_target;
            }
        }
    } else {
        target = (char *)(D_L02_0015FFD8 + (*(int *)(data + 0x28) << 8));
        if (*(signed char *)(target + 0x20) < 0) {
            *(short *)(data + 0x20) = -1;
            clear_follow_camera_volume_if_current(moby);
            return;
        }
    }
    *(char **)(data + 0x48) = target;
    if (*(short *)(data + 0x3c) == 6 && (char *)hero_lui.ground_moby != target) {
        *(short *)(data + 0x20) = 0;
        clear_follow_camera_volume_if_current(moby);
        return;
    }
    if (try_select_follow_camera_volume(moby) <= 0) {
        *(short *)(data + 0x20) = 0;
        return;
    }
    if (test_moby_camera_control_activation(moby) == 0) {
        *(short *)(data + 0x20) = 0;
        clear_follow_camera_volume_if_current(moby);
        return;
    }
    if (*(short *)(data + 0x3c) == 5) {
        angle = fast_arc_tan(*(float *)D_L02_00167400_d, *(float *)(D_L02_00167400_d + 4));
        if (*(short *)(data + 0x20) < 2)
            *(float *)(data + 0x40) = *(float *)(D_L02_0015EF50 + *(short *)(moby + 0x84) * 32 + 0x18);
        else if (absolute_float(controller_state.analog[0]) >= 0.3f || absolute_float(controller_state.analog[1]) >= 0.3f)
            *(float *)(data + 0x40) = angle;
        heading = *(float *)(data + 0x40);
        if (fast_difference_between_rotations(heading, angle) > 1.5707964f)
            fast_add_rotations(heading, 3.1415927f);
    }
    if (*(short *)(data + 0x3c) == 4) {
        heading = *(float *)(D_L02_0015EF50 + *(short *)(moby + 0x84) * 32 + 0x18);
        angle = fast_arc_tan(*(float *)D_L02_00167400_d, *(float *)(D_L02_00167400_d + 4));
        if (fast_difference_between_rotations(heading, angle) > 1.5707964f)
            heading = fast_add_rotations(heading, 3.1415927f);
        *(float *)(data + 0x40) = heading;
    }
    active = 1;
    scale = 1.0f;
    *(short *)(data + 0x20) += 1;
    if (*(short *)(data + 0x3c) == 1) {
        duration = scale_game_frames(300);
        count = scale_game_frames(400);
        end = scale_game_frames(560);
        frames = scale_game_frames(200);
    } else {
        duration = scale_game_frames(300);
        count = scale_game_frames(350);
        end = scale_game_frames(400);
        frames = scale_game_frames(200);
    }
    if (*(short *)(data + 0x3c) == 7) {
        if (controller_state.analog[0] != 0.0f) *(short *)(data + 0x20) = duration;
        count = *(short *)(data + 0x20);
        if (count >= duration) {
            active = 0;
            *(short *)(data + 0x3e) += 1;
            if (duration + scale_game_frames(30) < count) {
                scale = 0.1f;
                active = 1;
            }
        }
        if (*(short *)(data + 0x20) >= end) {
            active = 1;
            *(short *)(data + 0x3e) = 0;
            *(short *)(data + 0x20) = 1;
        }
        if (frames < *(short *)(data + 0x20) && *(short *)(data + 0x20) < duration) {
            *(short *)(data + 0x3e) = duration;
            *(short *)(data + 0x20) = frames;
        }
    } else if ((unsigned short)(*(short *)(data + 0x3c) - 1) < 2) {
        set_follow_camera_control_word(176);
        set_nonzero_follow_camera_parameters(1.0f, 12.0f, 0.11f);
        if (controller_state.analog[0] != 0.0f || controller_state.analog[1] != 0.0f)
            *(short *)(data + 0x20) = duration;
        if (*(short *)(data + 0x20) >= duration) {
            int waited = (unsigned short)*(short *)(data + 0x3e) + 1;
            *(short *)(data + 0x3e) = waited;
            if ((short)waited >= count) {
                if (test_moby_camera_target_angle_limits(moby, 30.0f, 0.0f) != 0)
                    *(short *)(data + 0x20) = end;
                else
                    active = 0;
            } else {
                active = 0;
            }
        }
        if (*(short *)(data + 0x20) >= end) {
            active = 1;
            *(short *)(data + 0x3e) = 0;
            *(short *)(data + 0x20) = 1;
        }
        if (frames < *(short *)(data + 0x20) && *(short *)(data + 0x20) < duration) {
            *(short *)(data + 0x3e) = duration;
            *(short *)(data + 0x20) = frames;
        }
    } else if (frames < *(short *)(data + 0x20)) {
        *(short *)(data + 0x20) = frames;
    }
    hit = 0;
    amount = *(float *)data * scale * 0.017453292f * ((float)*(short *)(data + 0x20) / convert_integer_to_float(frames));
    if (active) {
        if (test_moby_camera_target_angle_limits(moby, *(float *)(data + 0x30), *(float *)(data + 0x2c)) != 0) {
            hit = 1;
            if ((unsigned short)(*(short *)(data + 0x3c) - 4) < 2) {
                heading_vector[0] = fast_cos(*(float *)(data + 0x40));
                heading_vector[1] = fast_sin(*(float *)(data + 0x40));
                heading_vector[2] = 0.0f;
                set_follow_camera_angle_blend_from_direction(amount, 0.0f, heading_vector);
            } else if (*(short *)(data + 0x3c) == 3) {
                float speed;
                if (*(short *)(data + 0x20) < scale_game_frames(90))
                    speed = (amount - 0.0f) * ((float)*(short *)(data + 0x20) / convert_integer_to_float(scale_game_frames(90))) + 0.0f;
                else
                    speed = amount;
                set_follow_camera_angle_blend_from_position(speed, 0.0f, target + 0x10);
            } else {
                set_follow_camera_angle_blend_from_position(amount, 0.0f, target + 0x10);
            }
        }
    }
    if (*(float *)(data + 0x34) != 0.0f) {
        FUN_L00_002e8450(0, *(float *)(data + 0x34), 0.003f);
        FUN_L00_002e8620();
    }
    if (*(float *)(data + 0x38) != 0.0f)
        FUN_L00_002e84b8(*(float *)(data + 0x38), 0.003f);
    if (*(float *)(data + 0x4c) != 0.0f)
        follow_camera_set_offset_scale(0, *(float *)(data + 0x4c), 0.005f);
    if (*(short *)(data + 0x3c) == 3) {
        float heading;
        set_follow_camera_orbit_clamp_mode(0);
        FUN_L00_002e8620();
        FUN_L00_002e8648();
        follow_camera_set_up_spring(0.01f, 0.2f);
        if (*(short *)(data + 0x20) >= scale_game_frames(120))
            heading = 0.03f;
        else
            heading = ((float)*(short *)(data + 0x20) / convert_integer_to_float(scale_game_frames(120))) * 0.02f + 0.01f;
        follow_camera_set_lateral_spring(heading, 0.2f);
    }
    if ((unsigned short)(*(short *)(data + 0x3c) - 4) < 2) {
        set_follow_camera_orbit_clamp_mode(0);
        FUN_L00_002e8620();
        FUN_L00_002e8648();
        follow_camera_set_up_spring(0.02f, 0.2f);
        FUN_L00_002e8450(0, 5.84f, 0.003f);
        if (*(short *)(data + 0x3c) == 4) {
            float angle;
            fast_vec_sub(heading_vector, target + 0x10, &hero.motion.pos);
            angle = fast_arc_tan(heading_vector[0], heading_vector[1]);
            if (hit && fast_difference_between_rotations(angle, *(float *)(data + 0x40)) > 1.9198622f) {
                if (*(float *)(data + 0x34) != 0.0f)
                    FUN_L00_002e8450(0, *(float *)(data + 0x34) + 3.36f, 0.003f);
                else
                    FUN_L00_002e8450(0, 8.0f, 0.003f);
            }
        }
        set_follow_camera_control_word(176);
    }
    return;
}
#define NOT_SDA __attribute__((section(".data")))

#define MACRO_ADDR __attribute__((section(".sdata")))

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002FD6D0), where it is exact; names translated to the US level program. */

extern char *D_L02_0015EF50;
extern char *D_L02_00167400_c298 __asm__("D_L02_00167400") NOT_SDA;

int update_moby_camera_control(char *moby) __asm__("FUN_L02_002fc298");

int update_moby_camera_control(char *moby) {
    char *moby_camera_control_data = *(char **)(D_L02_0015EF50 + *(short *)(moby + 0x84) * 32 + 0x1C);
    if (*(short *)(D_L02_00167400_c298 + 0x86) == 0 && *(short *)(moby_camera_control_data + 0x20) >= 0)
        apply_moby_camera_control(moby);
    *(int *)(moby_camera_control_data + 0x50) = 0;
    return 0;
}

s32 D_L02_00167604[1] __attribute__((section(".data"))) = {0};
f32 D_L02_00167608[1] __attribute__((section(".data"))) = {0};
s32 D_L02_00167610[1] __attribute__((section(".data"))) = {0};
s32 D_L03_00167100[1] __attribute__((section(".data"))) = {0};
s32 D_L03_00167104[1] __attribute__((section(".data"))) = {0};
f32 D_L03_00167108[1] __attribute__((section(".data"))) = {0};
f32 D_L03_0016710C[1] __attribute__((section(".data"))) = {0};
s32 D_L03_00167110[1] __attribute__((section(".data"))) = {0};
s32 D_L04_00167180[1] __attribute__((section(".data"))) = {0};
s32 D_L04_00167184[1] __attribute__((section(".data"))) = {0};
f32 D_L04_00167188[1] __attribute__((section(".data"))) = {0};
f32 D_L04_0016718C[1] __attribute__((section(".data"))) = {0};
s32 D_L04_00167190[1] __attribute__((section(".data"))) = {0};
s32 D_L05_00167400[1] __attribute__((section(".data"))) = {0};
s32 D_L05_00167404[1] __attribute__((section(".data"))) = {0};
f32 D_L05_00167408[1] __attribute__((section(".data"))) = {0};
f32 D_L05_0016740C[1] __attribute__((section(".data"))) = {0};
s32 D_L05_00167410[1] __attribute__((section(".data"))) = {0};
s32 D_L07_00167090[1] __attribute__((section(".data"))) = {0};
s32 D_L09_00167180[1] __attribute__((section(".data"))) = {0};
s32 D_L09_00167184[1] __attribute__((section(".data"))) = {0};
f32 D_L09_00167188[1] __attribute__((section(".data"))) = {0};
f32 D_L09_0016718C[1] __attribute__((section(".data"))) = {0};
s32 D_L09_00167190[1] __attribute__((section(".data"))) = {0};
s32 D_L10_00167490[1] __attribute__((section(".data"))) = {0};
s32 D_L12_00167480[1] __attribute__((section(".data"))) = {0};
s32 D_L12_00167484[1] __attribute__((section(".data"))) = {0};
f32 D_L12_00167488[1] __attribute__((section(".data"))) = {0};
f32 D_L12_0016748C[1] __attribute__((section(".data"))) = {0};
s32 D_L12_00167490[1] __attribute__((section(".data"))) = {0};
s32 D_L15_00167600[1] __attribute__((section(".data"))) = {0};
s32 D_L15_00167604[1] __attribute__((section(".data"))) = {0};
f32 D_L15_00167608[1] __attribute__((section(".data"))) = {0};
f32 D_L15_0016760C[1] __attribute__((section(".data"))) = {0};
s32 D_L15_00167610[1] __attribute__((section(".data"))) = {0};
s32 D_L16_00167400[1] __attribute__((section(".data"))) = {0};
s32 D_L16_00167404[1] __attribute__((section(".data"))) = {0};
f32 D_L16_00167408[1] __attribute__((section(".data"))) = {0};
f32 D_L16_0016740C[1] __attribute__((section(".data"))) = {0};
s32 D_L16_00167410[1] __attribute__((section(".data"))) = {0};
