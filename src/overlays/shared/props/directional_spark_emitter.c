/* Directional spark emitter (class 0x288): sprays sparks along its facing direction. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/props/directional_spark_emitter_data/update_directional_spark_emitter_2.h"
#include "rnc/overlay/shared/props/directional_spark_emitter_data/directional_spark_emitter_update.h"

typedef struct DirectionalSparkEmitterVars {
    s32 color_1;
    s32 color_2;
    u8  phase_1_frames[0x4];
    u8  phase_2_frames[0x4];
    u8  phase_3_frames[0x4];
    s32 emit_interval;
    f32 forward_speed_1;
    f32 rise_speed_1;
    f32 forward_speed_2;
    f32 rise_speed_2;
    f32 vector_1_w;
    f32 vector_2_w;
    f32 side_spread_1;
    f32 side_spread_2;
    s32 use_default_settings;
    s32 emit_timer;
} DirectionalSparkEmitterVars;

extern void
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");

extern float scale_time(float) __asm__("FUN_001f96b0");

extern s32 spark_emitter_default_initial_spark_word_gp __asm__("D_L08_00161F74")
    __attribute__((sda));
extern s32 spark_emitter_default_second_spark_word_gp __asm__("D_L08_00161F78")
    __attribute__((sda));
extern f32 spark_emitter_default_primary_z_increment_base_gp __asm__("D_L08_00161F7C")
    __attribute__((sda));
extern f32 spark_emitter_default_primary_magnitude_base_gp __asm__("D_L08_00161F80")
    __attribute__((sda));
extern f32 spark_emitter_default_secondary_z_increment_base_gp __asm__("D_L08_00161F84")
    __attribute__((sda));
extern f32 spark_emitter_default_secondary_magnitude_base_gp __asm__("D_L08_00161F88")
    __attribute__((sda));

extern s32 spark_emitter_default_timer_reset_gp __asm__("D_L08_00161F98") __attribute__((sda));

extern f32 spark_emitter_default_primary_spread_limit_gp __asm__("D_L08_00161FA4")
    __attribute__((sda));
extern f32 spark_emitter_default_secondary_spread_limit_gp __asm__("D_L08_00161FA8")
    __attribute__((sda));

void directional_spark_emitter_update(char *moby) __asm__("FUN_L08_002f5830");

/* Moby class 648 */
void directional_spark_emitter_update(char *moby) {
    f32 spark_motion_primary[4], spark_motion_secondary[4], spark_position[4],
        spark_motion_offset[4], spark_rotation_matrix[16];
    DirectionalSparkEmitterVars *spark_data;
    s32 spark_frame_count, spark_second_frame_count, spark_third_frame_count;

    spark_data = (DirectionalSparkEmitterVars *)MOBY(moby)->pvars;
    if (fast_dec_timer((int *)(&spark_data->emit_timer)) == 0)
        return;
    if (spark_data->use_default_settings != 0) {
        spark_data->emit_timer = spark_emitter_default_timer_reset_gp;
        euler_rotation_matrix(spark_rotation_matrix, moby + 0x40);
        qcopy(spark_position, &MOBY(moby)->pos);
        spark_position[0] += random_float_between(-0.05f, 0.05f);
        spark_position[1] += random_float_between(-0.05f, 0.05f);
        spark_position[0] += random_float_between(-0.05f, 0.05f);
        random_float_between(-0.17453292f, 0.17453292f);
        fast_vec_normalize(
            spark_motion_primary,
            spark_rotation_matrix,
            random_float_between(spark_emitter_default_primary_magnitude_base_gp,
                                 spark_emitter_default_primary_magnitude_base_gp * 1.2f) *
                frame_time);
        fast_vec_normalize(spark_motion_offset,
                           spark_rotation_matrix + 4,
                           random_float_between(-spark_emitter_default_primary_spread_limit_gp,
                                                spark_emitter_default_primary_spread_limit_gp) *
                               frame_time);
        fast_vec_add(spark_motion_primary, spark_motion_primary, spark_motion_offset);
        spark_motion_primary[2] +=
            random_float_between(spark_emitter_default_primary_z_increment_base_gp,
                                 spark_emitter_default_primary_z_increment_base_gp * 1.2f) *
            frame_time;
        qcopy(spark_motion_secondary, spark_motion_primary);
        fast_vec_normalize(
            spark_motion_secondary,
            spark_rotation_matrix,
            random_float_between(spark_emitter_default_secondary_magnitude_base_gp,
                                 spark_emitter_default_secondary_magnitude_base_gp * 1.5f) *
                frame_time);
        fast_vec_normalize(spark_motion_offset,
                           spark_rotation_matrix + 4,
                           random_float_between(-spark_emitter_default_secondary_spread_limit_gp,
                                                spark_emitter_default_secondary_spread_limit_gp) *
                               frame_time);
        fast_vec_add(spark_motion_secondary, spark_motion_secondary, spark_motion_offset);
        spark_motion_secondary[2] +=
            random_float_between(spark_emitter_default_secondary_z_increment_base_gp,
                                 spark_emitter_default_secondary_z_increment_base_gp * 1.5f) *
            frame_time;
        spark_motion_primary[3] = spark_emitter_default_primary_motion_w_gp;
        spark_motion_secondary[3] = spark_emitter_default_secondary_motion_w_gp;
        spark_frame_count = truncate_float_to_s32(scale_time(
            random_float_between((f32)spark_emitter_default_frame_count_base_gp,
                                 (f32)spark_emitter_default_frame_count_base_gp * 1.2f)));
        spark_second_frame_count = truncate_float_to_s32(scale_time(
            random_float_between((f32)spark_emitter_default_second_frame_count_base_gp,
                                 (f32)spark_emitter_default_second_frame_count_base_gp * 1.2f)));
        spark_third_frame_count = truncate_float_to_s32(scale_time(
            random_float_between((f32)spark_emitter_default_third_frame_count_base_gp,
                                 (f32)spark_emitter_default_third_frame_count_base_gp * 1.2f)));
        spawn_spark(spark_position,
                    spark_motion_primary,
                    spark_motion_secondary,
                    spark_emitter_default_initial_spark_word_gp,
                    spark_emitter_default_second_spark_word_gp,
                    spark_frame_count,
                    spark_second_frame_count,
                    spark_third_frame_count,
                    -1);
    } else {
        spark_data->emit_timer = spark_data->emit_interval;
        euler_rotation_matrix(spark_rotation_matrix, moby + 0x40);
        qcopy(spark_position, &MOBY(moby)->pos);
        spark_position[0] += random_float_between(-0.05f, 0.05f);
        spark_position[1] += random_float_between(-0.05f, 0.05f);
        spark_position[2] += random_float_between(-0.05f, 0.05f);
        random_float_between(-0.17453292f, 0.17453292f);
        fast_vec_normalize(
            spark_motion_primary,
            spark_rotation_matrix,
            random_float_between(spark_data->forward_speed_1, spark_data->forward_speed_1 * 1.2f) *
                frame_time);
        fast_vec_normalize(
            spark_motion_offset,
            spark_rotation_matrix + 4,
            random_float_between(-spark_data->side_spread_1, spark_data->side_spread_1) *
                frame_time);
        fast_vec_add(spark_motion_primary, spark_motion_primary, spark_motion_offset);
        spark_motion_primary[2] +=
            random_float_between(spark_data->rise_speed_1, spark_data->rise_speed_1 * 1.2f) *
            frame_time;
        qcopy(spark_motion_secondary, spark_motion_primary);
        fast_vec_normalize(
            spark_motion_secondary,
            spark_rotation_matrix,
            random_float_between(spark_data->forward_speed_2, spark_data->forward_speed_2 * 1.5f) *
                frame_time);
        fast_vec_normalize(
            spark_motion_offset,
            spark_rotation_matrix + 4,
            random_float_between(-spark_data->side_spread_2, spark_data->side_spread_2) *
                frame_time);
        fast_vec_add(spark_motion_secondary, spark_motion_secondary, spark_motion_offset);
        spark_motion_secondary[2] +=
            random_float_between(spark_data->rise_speed_2, spark_data->rise_speed_2 * 1.5f) *
            frame_time;
        spark_motion_primary[3] = spark_data->vector_1_w;
        spark_motion_secondary[3] = spark_data->vector_2_w;
        spark_frame_count = truncate_float_to_s32(
            scale_time(random_float_between((f32) * (s32 *)(&spark_data->phase_1_frames),
                                            (f32) * (s32 *)(&spark_data->phase_1_frames) * 1.2f)));
        spark_second_frame_count = truncate_float_to_s32(
            scale_time(random_float_between((f32) * (s32 *)(&spark_data->phase_2_frames),
                                            (f32) * (s32 *)(&spark_data->phase_2_frames) * 1.2f)));
        spark_third_frame_count = truncate_float_to_s32(
            scale_time(random_float_between((f32) * (s32 *)(&spark_data->phase_3_frames),
                                            (f32) * (s32 *)(&spark_data->phase_3_frames) * 1.2f)));
        spawn_spark(spark_position,
                    spark_motion_primary,
                    spark_motion_secondary,
                    spark_data->color_1,
                    spark_data->color_2,
                    spark_frame_count,
                    spark_second_frame_count,
                    spark_third_frame_count,
                    -1);
    }
}

s32 D_L10_00161954 = 0x20706060;
s32 D_L10_00161958 = 0x20f0d0d0;
f32 D_L10_0016195C = 0.4f;
f32 D_L10_00161960 = 0;
f32 D_L10_00161964 = 0.4f;
f32 D_L10_00161968 = 0.3f;
s32 D_L10_00161978 = 0x3c;
f32 D_L10_00161984 = 0;
f32 D_L10_00161988 = 0.3f;

s32 spark_emitter_default_timer_reset_gp __asm__("D_L08_00161F98") = 0x3c;
