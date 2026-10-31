/* Level 06 moby class 0x5e8: spark emitter, and the function that starts the listed ones. */
#include "types.h"
#include "asm.h"

#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/math/random/random_vector.h"

#include "rnc/mobys/moby.h"
#include "qcopy.h"

extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float scale_time(float) __asm__("FUN_001f96b0");

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) SparkVector;

typedef struct SparkData {
    int  f0;
    int  f4;
    char pad8[0x18];
    int  f20;
    int  f24;
    int  f28;
} SparkData;

extern int random_integer_below(int) __asm__("FUN_00213260");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");

#include "rnc/overlay/l06/mobys/spark_emitter_globals.h"

float      spark_burst_vector_scale_variation_gp __asm__("D_L06_001622FC") = 0.1f;
extern int l06_spark_burst_second_frame_count_base __asm__("D_L06_00162304") __attribute__((sda));
extern int l06_spark_burst_third_frame_count_base __asm__("D_L06_00162308") __attribute__((sda));
/* Spark colours are RGBA words (0xAABBGGRR); each spark gets a random blend
   between the _from and _to colour. */
int        spark_burst_start_color_from __asm__("D_L06_00162320") = 0x80ffc0c0;
int        spark_burst_start_color_to __asm__("D_L06_00162324") = 0x80ffffc0;
int        spark_burst_end_color_from __asm__("D_L06_00162328") = 0x20c08080;
int        spark_burst_end_color_to __asm__("D_L06_0016232C") = 0x20c0c080;
float      spark_spray_z_subtraction_rate_gp __asm__("D_L06_00162338") = 5.0f;
float      spark_spray_vector_scale_variation_gp __asm__("D_L06_0016233C") = 0.6f;
extern int l06_spark_spray_second_frame_count_base __asm__("D_L06_00162348") __attribute__((sda));
extern int l06_spark_spray_third_frame_count_base __asm__("D_L06_0016234C") __attribute__((sda));
int        spark_spray_start_color_from __asm__("D_L06_00162364") = 0x804080ff;
int        spark_spray_start_color_to __asm__("D_L06_00162368") = 0x8040ffff;
int        spark_spray_end_color_from __asm__("D_L06_0016236C") = 0x80204040;
int        spark_spray_end_color_to __asm__("D_L06_00162370") = 0x80204080;
int        spark_spray_timer_range_start_gp __asm__("D_L06_00162374") = 20;
int        spark_spray_timer_range_end_gp __asm__("D_L06_00162378") = 40;
int        spark_spray_lookup_index_and_flags __asm__("D_L06_0016237C") =
    0x10000 | 25; /* spawn_spark: low half the spark lookup index, bit 16 its 0x48 blend */

extern f32 frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;
/* Level 06 moby update: a spark emitter; state 4 bursts on a random timer, state 3 sprays continuously. */
void spark_emitter_update(struct Moby *emitter) __asm__("FUN_L06_00308c68");

/* Moby class 1512 */
void spark_emitter_update(struct Moby *emitter) {
    enum {
        SPARK_EMITTER_INITIALIZE = 0,
        SPARK_EMITTER_DELAYED_ACTION = 2,
        SPARK_EMITTER_RANDOMIZED_SPRAY = 3,
        SPARK_EMITTER_TIMED_BURSTS = 4
    };
    SparkData  *emitter_data = *(void **)&emitter->pvars;
    SparkVector position_test_vector;
    SparkVector rotation_based_spark_vector;
    SparkVector z_adjusted_spark_vector;
    SparkVector spark_vector_offset;

    qcopy(&position_test_vector, &*(SparkVector *)&emitter->pos);
    position_test_vector.w = 3.0f;
    if (fast_bsphere_check(&position_test_vector, 64.0f) == -1) {
        return;
    }
    switch (emitter->state) {
    case SPARK_EMITTER_INITIALIZE:
        euler_rotation_basis(&*(SparkVector *)&emitter->rot_row0, &*(SparkVector *)&emitter->rot);
        if (emitter_data->f4 != 0) {
            emitter->state = 1;
        } else if (emitter_data->f0 != 0) {
            emitter->state = 4;
        } else {
            emitter->state = 3;
        }
        break;
    case SPARK_EMITTER_DELAYED_ACTION:
        if (fast_dec_timer(&emitter_data->f28)) {
            cart_burst(emitter, &*(SparkVector *)&emitter->pos, -1, 0.333f, 13.0f);
            if (emitter_data->f0 != 0) {
                emitter->state = 4;
            } else {
                emitter->state = 3;
            }
        }
        break;
    case SPARK_EMITTER_TIMED_BURSTS: {
        int spark_count = 1;
        int k;
        if (fast_dec_timer(&emitter_data->f24)) {
            spark_count = 5;
            emitter_data->f24 =
                truncate_float_to_s32(scale_time(random_float_between(60.0f, 120.0f)));
        }
        if (random_integer_below(4) != 0 && spark_count < 2) {
            return;
        }
        for (k = 0; k < spark_count; k++) {
            int   c19, c18, c17, c16, c2;
            float z;
            fast_vec_normalize(&rotation_based_spark_vector,
                               &*(SparkVector *)&emitter->rot_row0,
                               spark_burst_rotation_row0_scale *
                                   (random_float_between(-spark_burst_vector_scale_variation_gp,
                                                         spark_burst_vector_scale_variation_gp) +
                                    1.0f) *
                                   frame_time_macro);
            z = spark_burst_initial_z_scale *
                (random_float_between(-spark_burst_vector_scale_variation_gp,
                                      spark_burst_vector_scale_variation_gp) +
                 1.0f) *
                frame_time_macro;
            clear_u64_value(&z_adjusted_spark_vector);
            z_adjusted_spark_vector.z = z;
            fast_vec_normalize(&spark_vector_offset,
                               &*(SparkVector *)&emitter->rot_row1,
                               random_float_between(-spark_burst_rotation_row1_scale_variation,
                                                    spark_burst_rotation_row1_scale_variation) *
                                   frame_time_macro);
            fast_vec_add(&z_adjusted_spark_vector, &z_adjusted_spark_vector, &spark_vector_offset);
            rotation_based_spark_vector.w = random_float_between(
                spark_burst_rotation_vector_w_range_start, spark_burst_rotation_vector_w_range_end);
            z_adjusted_spark_vector.w =
                random_float_between(spark_burst_z_adjusted_vector_w_range_start,
                                     spark_burst_z_adjusted_vector_w_range_end);
            c19 = fast_tween_color(spark_burst_start_color_from,
                                   spark_burst_start_color_to,
                                   random_float_between(0.0f, 1.0f));
            c18 = fast_tween_color(spark_burst_end_color_from,
                                   spark_burst_end_color_to,
                                   random_float_between(0.0f, 1.0f));
            c17 = truncate_float_to_s32(scale_time(
                (float)spark_burst_emission_random_span * random_float_between(0.0f, 1.0f) + 1.0f));
            c16 = truncate_float_to_s32(
                scale_time((float)l06_spark_burst_second_frame_count_base *
                           (random_float_between(-spark_burst_emission_scale_variation,
                                                 spark_burst_emission_scale_variation) +
                            1.0f)));
            c2 = truncate_float_to_s32(
                scale_time((float)l06_spark_burst_third_frame_count_base *
                           (random_float_between(-spark_burst_emission_scale_variation,
                                                 spark_burst_emission_scale_variation) +
                            1.0f)));
            spawn_spark(&*(SparkVector *)&emitter->pos,
                        &rotation_based_spark_vector,
                        &z_adjusted_spark_vector,
                        c19,
                        c18,
                        c17,
                        c16,
                        c2,
                        -1);
        }
        break;
    }
    case SPARK_EMITTER_RANDOMIZED_SPRAY:
        if (fast_dec_timer(&emitter_data->f20)) {
            int spray_spark_count;
            int k;
            emitter_data->f20 = truncate_float_to_s32(scale_time(random_float_between(
                (float)spark_spray_timer_range_start_gp, (float)spark_spray_timer_range_end_gp)));
            spray_spark_count = random_integer_below(7) ? 1 : random_integer_below(5) + 5;
            for (k = 0; k < spray_spark_count; k++) {
                int c19, c18, c17, c16, c2;
                random_vector(&spark_vector_offset,
                              0.0f,
                              spark_spray_offset_random_range_end * frame_time_macro);
                fast_vec_normalize(
                    &rotation_based_spark_vector,
                    &*(SparkVector *)&emitter->rot_row0,
                    spark_spray_rotation_row0_scale *
                        (random_float_between(-spark_spray_vector_scale_variation_gp,
                                              spark_spray_vector_scale_variation_gp) +
                         1.0f) *
                        frame_time_macro);
                fast_vec_add(&rotation_based_spark_vector,
                             &rotation_based_spark_vector,
                             &spark_vector_offset);
                fast_vec_normalize(
                    &z_adjusted_spark_vector,
                    &rotation_based_spark_vector,
                    spark_spray_second_vector_scale *
                        (random_float_between(-spark_spray_vector_scale_variation_gp,
                                              spark_spray_vector_scale_variation_gp) +
                         1.0f) *
                        frame_time_macro);
                z_adjusted_spark_vector.z = z_adjusted_spark_vector.z -
                                            spark_spray_z_subtraction_rate_gp * frame_time_macro;
                rotation_based_spark_vector.w =
                    random_float_between(spark_spray_rotation_vector_w_range_start,
                                         spark_spray_rotation_vector_w_range_end);
                z_adjusted_spark_vector.w =
                    random_float_between(spark_spray_z_adjusted_vector_w_range_start,
                                         spark_spray_z_adjusted_vector_w_range_end);
                c19 = fast_tween_color(spark_spray_start_color_from,
                                       spark_spray_start_color_to,
                                       random_float_between(0.0f, 1.0f));
                c18 = fast_tween_color(spark_spray_end_color_from,
                                       spark_spray_end_color_to,
                                       random_float_between(0.0f, 1.0f));
                c17 = truncate_float_to_s32(scale_time((float)spark_spray_emission_random_span *
                                                           random_float_between(0.0f, 1.0f) +
                                                       1.0f));
                c16 = truncate_float_to_s32(
                    scale_time((float)l06_spark_spray_second_frame_count_base *
                               (random_float_between(-spark_spray_emission_scale_variation,
                                                     spark_spray_emission_scale_variation) +
                                1.0f)));
                c2 = truncate_float_to_s32(
                    scale_time((float)l06_spark_spray_third_frame_count_base *
                               (random_float_between(-spark_spray_emission_scale_variation,
                                                     spark_spray_emission_scale_variation) +
                                1.0f)));
                spawn_spark(&*(SparkVector *)&emitter->pos,
                            &rotation_based_spark_vector,
                            &z_adjusted_spark_vector,
                            c19,
                            c18,
                            c17,
                            c16,
                            c2,
                            spark_spray_lookup_index_and_flags);
            }
        }
        break;
    }
}

#include "rnc/overlay/entities.h"

/* finds the mobys of type 0x5E8 in state 1 in a level list and starts those within range */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_0030A680), where it is exact; names translated to the US level program. */

extern char  *D_L06_0015FFD8;
extern short *D_L06_001ABFC0[];

void activate_listed_spark_emitters(int moby_list_index) __asm__("FUN_L06_00309240");

void activate_listed_spark_emitters(int moby_list_index) {
    short *moby_index_entry;
    char  *moby;
    char  *moby_private_data;
    float  position_test_vector[4];
    if (D_L06_001ABFC0[moby_list_index] != 0) {
        moby_index_entry = D_L06_001ABFC0[moby_list_index];
        do {
            moby = D_L06_0015FFD8 + ((*(unsigned short *)moby_index_entry & 0x7FFF) << 8);
            if (MOBY(moby)->oclass == 0x5E8 && (unsigned char)moby[0x20] == 1) {
                qcopy(position_test_vector, moby + 0x10);
                position_test_vector[3] = 0.25f;
                if (fast_bsphere_check((char *)position_test_vector, 64.0f) >= 0) {
                    moby_private_data = ((char *)MOBY(moby)->pvars);
                    moby[0x20] = 2;
                    *(int *)(moby_private_data + 0x28) =
                        truncate_float_to_s32(scale_time(random_float_between(30.0f, 60.0f)));
                }
            }
        } while (*moby_index_entry++ >= 0);
    }
}
