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
    s32 unk0;
    s32 unk4;
    u8  unk8[0x4];
    u8  unkC[0x4];
    u8  unk10[0x4];
    s32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    s32 unk38;
    s32 unk3C;
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
    if (fast_dec_timer((int *)(&spark_data->unk3C)) == 0)
        return;
    if (spark_data->unk38 != 0) {
        spark_data->unk3C = spark_emitter_default_timer_reset_gp;
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
        spark_data->unk3C = spark_data->unk14;
        euler_rotation_matrix(spark_rotation_matrix, moby + 0x40);
        qcopy(spark_position, &MOBY(moby)->pos);
        spark_position[0] += random_float_between(-0.05f, 0.05f);
        spark_position[1] += random_float_between(-0.05f, 0.05f);
        spark_position[2] += random_float_between(-0.05f, 0.05f);
        random_float_between(-0.17453292f, 0.17453292f);
        fast_vec_normalize(spark_motion_primary,
                           spark_rotation_matrix,
                           random_float_between(spark_data->unk18, spark_data->unk18 * 1.2f) *
                               frame_time);
        fast_vec_normalize(spark_motion_offset,
                           spark_rotation_matrix + 4,
                           random_float_between(-spark_data->unk30, spark_data->unk30) *
                               frame_time);
        fast_vec_add(spark_motion_primary, spark_motion_primary, spark_motion_offset);
        spark_motion_primary[2] +=
            random_float_between(spark_data->unk1C, spark_data->unk1C * 1.2f) * frame_time;
        qcopy(spark_motion_secondary, spark_motion_primary);
        fast_vec_normalize(spark_motion_secondary,
                           spark_rotation_matrix,
                           random_float_between(spark_data->unk20, spark_data->unk20 * 1.5f) *
                               frame_time);
        fast_vec_normalize(spark_motion_offset,
                           spark_rotation_matrix + 4,
                           random_float_between(-spark_data->unk34, spark_data->unk34) *
                               frame_time);
        fast_vec_add(spark_motion_secondary, spark_motion_secondary, spark_motion_offset);
        spark_motion_secondary[2] +=
            random_float_between(spark_data->unk24, spark_data->unk24 * 1.5f) * frame_time;
        spark_motion_primary[3] = spark_data->unk28;
        spark_motion_secondary[3] = spark_data->unk2C;
        spark_frame_count = truncate_float_to_s32(scale_time(random_float_between(
            (f32) * (s32 *)(&spark_data->unk8), (f32) * (s32 *)(&spark_data->unk8) * 1.2f)));
        spark_second_frame_count = truncate_float_to_s32(scale_time(random_float_between(
            (f32) * (s32 *)(&spark_data->unkC), (f32) * (s32 *)(&spark_data->unkC) * 1.2f)));
        spark_third_frame_count = truncate_float_to_s32(scale_time(random_float_between(
            (f32) * (s32 *)(&spark_data->unk10), (f32) * (s32 *)(&spark_data->unk10) * 1.2f)));
        spawn_spark(spark_position,
                    spark_motion_primary,
                    spark_motion_secondary,
                    spark_data->unk0,
                    spark_data->unk4,
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
