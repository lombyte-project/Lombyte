/* Level 11 moby class bubble (class 0x5F4): rises, pops and fades, with its surface effects and sparks. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/rendering/effects/spawn_spark.h"

extern float frame_scale_inv_macro[] __asm__("D_0015ED68") MACRO_ADDR;
extern float scale_time(float) __asm__("FUN_001f96b0");
extern f32   approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern float D_L11_0015F580[] MACRO_ADDR;
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern float D_L11_0015F580[];
/* Bubble update: rises to the surface, then pops and fades, drifting with its velocity. */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031BF78), where it is exact; names translated to the US level program. */

extern float approach_value(float *p, float target, float maxstep);
extern float vector_length_xy(void *);
extern int   scale_game_frames(int);
void         spawn_bubble_surface_effects(struct Moby *source_moby,
                                          float       *ground,
                                          int          spark_count,
                                          int          ground_effect_count) __asm__("FUN_L11_0031ae68");

void         spawn_bubble_surface_effects(struct Moby *m,
                                          float       *ground,
                                          int          n1,
                                          int          n2) __asm__("FUN_L11_0031ae68");
extern void  FUN_L11_0031b098_c(char *) __asm__("FUN_L11_0031b098");
extern char *func_L00_0026FF20_p(void *, float *, float, float) __asm__("FUN_L00_0026f080");

extern short bubble_pre_surface_vertical_acceleration_gp __asm__("D_L11_00162400")
    __attribute__((sda));
extern short bubble_shrink_vertical_acceleration_gp __asm__("D_L11_00162404") __attribute__((sda));
extern short bubble_horizontal_deceleration_gp __asm__("D_L11_00162408") __attribute__((sda));
extern short bubble_surface_crossing_velocity_scale_gp __asm__("D_L11_0016240C")
    __attribute__((sda));
extern short bubble_spark_spawn_random_limit_gp __asm__("D_L11_00162410") __attribute__((sda));
extern short D_L11_00162414 __attribute__((sda));
extern short bubble_shrink_effect_color_range_start_gp __asm__("D_L11_00162418")
    __attribute__((sda));
extern short bubble_shrink_effect_color_range_end_gp __asm__("D_L11_0016241C") __attribute__((sda));
void         bubble_update(struct Moby *bubble_moby) __asm__("FUN_L11_0031ab08");

/* Moby class 1524 */
void bubble_update(struct Moby *bubble_moby) {
    enum {
        BUBBLE_MOVE_UNTIL_SURFACE_CROSSING = 1,
        BUBBLE_SHRINK_TO_REMOVAL = 2
    };
    char *bubble_vars = (char *)bubble_moby->pvars;
    float randomized_effect_vector[4];
    float horizontal_speed;
    float rate;
    char *spawned_effect;
    if (bubble_moby->visible == 0) {
        if (++*(short *)(bubble_vars + 0x22) > scale_game_frames(0xF)) {
            mark_moby_for_removal(bubble_moby);
            return;
        }
    } else {
        *(short *)(bubble_vars + 0x22) = 0;
    }
    switch (bubble_moby->state) {
    case BUBBLE_MOVE_UNTIL_SURFACE_CROSSING:
        *(float *)(bubble_vars + 8) -=
            *(float *)&bubble_pre_surface_vertical_acceleration_gp * frame_time_sq;
        if (**(float **)(bubble_vars + 0x24) + 3.0f < bubble_moby->pos.z) {
            if (random_integer_below(*(int *)&bubble_spark_spawn_random_limit_gp) == 0) {
                FUN_L11_0031b098_c(bubble_moby);
            }
        }
        if (bubble_moby->pos.z < **(float **)(bubble_vars + 0x24)) {
            fast_vec_scale(
                bubble_vars, bubble_vars, *(float *)&bubble_surface_crossing_velocity_scale_gp);
            bubble_moby->state = 2;
            spawn_bubble_surface_effects(
                (struct Moby *)bubble_moby, *(float **)(bubble_vars + 0x24), 8, 4);
        }
        break;
    case BUBBLE_SHRINK_TO_REMOVAL:
        *(float *)(bubble_vars + 8) -=
            *(float *)&bubble_shrink_vertical_acceleration_gp * frame_time_sq;
        rate = bubble_moby->pclass->scale / (float)scale_game_frames(0x3C);
        if ((level_frame & 3) == 0) {
            spawn_bubble_surface_effects(
                (struct Moby *)bubble_moby, *(float **)(bubble_vars + 0x24), 0, 1);
            random_vector(randomized_effect_vector, 0.0f, frame_scale_inv_macro[1]);
            randomized_effect_vector[2] = absolute_float(randomized_effect_vector[2]);
            spawned_effect = func_L00_0026FF20_p(&bubble_moby->pos,
                                                 randomized_effect_vector,
                                                 *(float *)&D_L11_00162414,
                                                 **(float **)(bubble_vars + 0x24));
            if (spawned_effect != 0) {
                *(int *)(spawned_effect + 4) =
                    fast_tween_color(*(int *)&bubble_shrink_effect_color_range_start_gp,
                                     *(int *)&bubble_shrink_effect_color_range_end_gp,
                                     random_float_between(0.0f, 1.0f));
            }
        }
        approach_value(&bubble_moby->scale, 0.0f, rate);
        if (bubble_moby->scale == 0.0f) {
            mark_moby_for_removal(bubble_moby);
            return;
        }
        break;
    }
    fast_vec_add(&bubble_moby->pos, &bubble_moby->pos, bubble_vars);
    horizontal_speed = vector_length_xy(bubble_vars);
    approach_value(
        &horizontal_speed, 0.0f, *(float *)&bubble_horizontal_deceleration_gp * frame_time_sq);
    set_vector_length_xy(bubble_vars, bubble_vars, horizontal_speed);
}

/* Bursts n1 sparks and n2 smoke puffs at the moby, the puffs sitting on the ground height. */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031C2D8), where it is exact; names translated to the US level program. */

extern char bubble_surface_effect_color_range_start_gp[4] __asm__("D_L11_00162420")
    __attribute__((sda));
extern char bubble_surface_effect_color_range_end_gp[4] __asm__("D_L11_00162424")
    __attribute__((sda));
extern void  spawn_debris_chunk(void *, void *, int, int) __asm__("FUN_L00_0026f548");
extern char *func_L00_00272770_p(void *, void *, void *, float, float) __asm__("FUN_L00_002718d0");

void spawn_bubble_surface_effects(struct Moby *source_moby,
                                  float       *ground,
                                  int          spark_count,

                                  int ground_effect_count) {
    float effect_spawn_scratch[4];
    int   i;
    for (i = 0; i < spark_count; i++) {
        float emission_angle = random_angle_radians();
        float horizontal_emission_scale =
            random_float_between(frame_scale_inv_macro[1] * 0.0f, frame_scale_inv_macro[1] * 6.0f);
        int k;
        effect_spawn_scratch[0] = fast_cos(emission_angle) * horizontal_emission_scale;
        effect_spawn_scratch[1] = fast_sin(emission_angle) * horizontal_emission_scale;
        effect_spawn_scratch[2] =
            random_float_between(frame_scale_inv_macro[1] * 3.0f, frame_scale_inv_macro[1] * 6.5f);
        k = random_int_between(0x5A, 0x78);
        spawn_debris_chunk(&source_moby->pos, effect_spawn_scratch, random_integer_below(2), k);
    }
    for (i = 0; i < ground_effect_count; i++) {
        char *spawned_ground_effect;
        random_vector(effect_spawn_scratch, 1.0f, 1.0f);
        fast_vec_add(effect_spawn_scratch, effect_spawn_scratch, &source_moby->pos);
        effect_spawn_scratch[2] = *ground + 0.05f;
        spawned_ground_effect = func_L00_00272770_p(effect_spawn_scratch,
                                                    D_L11_0015F580,
                                                    ground,
                                                    random_float_between(3.0f, 6.0f),
                                                    i == 0 ? 1.0f : -1.0f);
        if (spawned_ground_effect != 0) {
            spawned_ground_effect[9] = truncate_float_to_s32(4.0f) + 0x40;
            *(short *)(spawned_ground_effect + 0xA) =
                truncate_float_to_s32(scale_time(random_float_between(30.0f, 90.0f)));
            *(int *)(spawned_ground_effect + 4) =
                fast_tween_color(*(int *)&bubble_surface_effect_color_range_start_gp,
                                 *(int *)&bubble_surface_effect_color_range_end_gp,
                                 random_float_between(0.0f, 1.0f));
        }
    }
}

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_0031C508), where it is exact; names translated to the US level program. */

extern short bubble_spark_random_velocity_radius_gp __asm__("D_L11_00162428") __attribute__((sda));
extern short bubble_spark_vertical_range_start_gp __asm__("D_L11_0016242C") __attribute__((sda));
extern short bubble_spark_vertical_range_end_gp __asm__("D_L11_00162430") __attribute__((sda));
extern short bubble_spark_first_vector_w_gp __asm__("D_L11_00162434") __attribute__((sda));
extern short bubble_spark_second_vector_w_gp __asm__("D_L11_00162438") __attribute__((sda));
extern short bubble_spark_initial_color_range_start_gp __asm__("D_L11_0016243C")
    __attribute__((sda));
extern short bubble_spark_initial_color_range_end_gp __asm__("D_L11_00162440") __attribute__((sda));
extern short bubble_spark_second_color_range_start_gp __asm__("D_L11_00162444")
    __attribute__((sda));
extern short bubble_spark_second_color_range_end_gp __asm__("D_L11_00162448") __attribute__((sda));
extern short bubble_spark_base_frame_count_gp __asm__("D_L11_0016244C") __attribute__((sda));
extern short bubble_spark_base_second_frame_count_gp __asm__("D_L11_00162450") __attribute__((sda));
extern short bubble_spark_base_third_frame_count_gp __asm__("D_L11_00162454") __attribute__((sda));
void         spawn_bubble_spark(struct Moby *source_moby) __asm__("FUN_L11_0031b098");

void spawn_bubble_spark(struct Moby *source_moby) {
    float randomized_velocity[4];
    float vertical_spark_vector[4];
    char *moby_vars;
    int   initial_spark_color, second_spark_color, spark_frame_count, second_spark_frame_count,
        third_spark_frame_count;
    moby_vars = (char *)source_moby->pvars;
    random_vector(randomized_velocity,
                  0.0f,
                  *(float *)&bubble_spark_random_velocity_radius_gp * frame_scale_inv_macro[1]);
    fast_vec_add(randomized_velocity, randomized_velocity, moby_vars);
    clear_u64_value(vertical_spark_vector);
    vertical_spark_vector[2] = random_float_between(*(float *)&bubble_spark_vertical_range_start_gp,
                                                    *(float *)&bubble_spark_vertical_range_end_gp) *
                               frame_scale_inv_macro[1];
    randomized_velocity[3] = *(float *)&bubble_spark_first_vector_w_gp;
    vertical_spark_vector[3] = *(float *)&bubble_spark_second_vector_w_gp;
    initial_spark_color = fast_tween_color(*(int *)&bubble_spark_initial_color_range_start_gp,
                                           *(int *)&bubble_spark_initial_color_range_end_gp,
                                           random_float_between(0.0f, 1.0f));
    second_spark_color = fast_tween_color(*(int *)&bubble_spark_second_color_range_start_gp,
                                          *(int *)&bubble_spark_second_color_range_end_gp,
                                          random_float_between(0.0f, 1.0f));
    spark_frame_count = truncate_float_to_s32(
        scale_time(random_float_between((float)*(int *)&bubble_spark_base_frame_count_gp * 0.8f,
                                        (float)*(int *)&bubble_spark_base_frame_count_gp * 1.2f)));
    second_spark_frame_count = truncate_float_to_s32(scale_time(
        random_float_between((float)*(int *)&bubble_spark_base_second_frame_count_gp * 0.8f,
                             (float)*(int *)&bubble_spark_base_second_frame_count_gp * 1.2f)));
    third_spark_frame_count = truncate_float_to_s32(scale_time(
        random_float_between((float)*(int *)&bubble_spark_base_third_frame_count_gp * 0.8f,
                             (float)*(int *)&bubble_spark_base_third_frame_count_gp * 1.2f)));
    spawn_spark(&source_moby->pos,
                randomized_velocity,
                vertical_spark_vector,
                initial_spark_color,
                second_spark_color,
                spark_frame_count,
                second_spark_frame_count,
                third_spark_frame_count,
                -1);
}

short bubble_spark_spawn_random_limit_gp __asm__("D_L11_00162410") = 3;
