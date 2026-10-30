/* Level 13 poison vessel fragment moby class (0x662): a spinning piece the breaking vessel throws out, trailing sparks, and its spawn helper. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/overlay/collision.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/world/collision/collision_line.h"
#include "qzero.h"
#include "rnc/overlay/l13/mobys/poison_vessel_fragment_globals.h"

extern char *create_moby(int) __asm__("FUN_0020c4f8");

char *spawn_poison_vessel_fragment(void *position,
                                   void *initial_velocity,
                                   int   duration_frames,
                                   int   effect_mode) __asm__("FUN_L13_0030c138");

extern char *spawn_poison_vessel_fragment(void *, void *, int, int) __asm__("FUN_L13_0030c138");

extern s32 D_001413D0_lui[] __asm__("D_001413D0") NOT_SDA;

extern float D_0015ED70;

extern char *create_moby(int);

char *spawn_poison_vessel_fragment(void *position,
                                   void *initial_velocity,
                                   int   duration_frames,

                                   int effect_mode) {
    unsigned char *moby = (unsigned char *)create_moby(0x662);
    if (moby != 0) {
        char *data;
        float rotation_x_increment, dd;
        data = ((char *)MOBY(moby)->pvars);
        moby[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x23] = random_int_between(0x40, 0x80);
        set_moby_color(moby, 0x20, 0x60, 0x10);
        qcopy(moby + 0x10, position);
        qcopy(data, initial_velocity);
        rotation_x_increment =
            random_float_between(frame_time * 6.2831855f, frame_time * 12.566371f);
        dd = frame_time;
        *(float *)(data + 0x10) = rotation_x_increment;
        *(float *)(data + 0x14) = random_float_between(dd * 6.2831855f, dd * 12.566371f);
        *(short *)(data + 0x18) = duration_frames;
        *(short *)(data + 0x1A) = duration_frames;
        if (effect_mode == 0) {
            MOBY(moby)->scale = MOBY(moby)->scale * random_float_between(0.5f, 0.75f);
        }
        *(float *)(data + 0x1C) = MOBY(moby)->scale;
        MOBY(moby)->scale = MOBY(moby)->scale * random_float_between(2.0f, 5.0f);
        moby[0xBC] = effect_mode;
        update_entity(moby);
    }
    return (char *)moby;
}

extern float FUN_001f96b0_c3b8(float) __asm__("FUN_L13_0020a9f8");

extern void FUN_0020c828(void *);

extern void FUN_L13_0030c2b0(void *);

extern void
spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");

#include "rnc/overlay/l13/mobys/poison_vessel_fragment_data/update_poison_vessel_fragment_4.h"

#include "rnc/overlay/l13/mobys/poison_vessel_fragment_data/update_poison_vessel_fragment_3.h"

#include "rnc/overlay/l13/mobys/poison_vessel_fragment_data/update_poison_vessel_fragment_2.h"

extern float moby_spark_first_batch_spread_degrees_gp __asm__("D_L13_00161F50")
    __attribute__((sda));

extern float moby_spark_second_batch_spread_degrees_gp __asm__("D_L13_00161F54")
    __attribute__((sda));

#include "rnc/overlay/l13/mobys/poison_vessel_fragment_data/update_poison_vessel_fragment.h"

int moby_spark_first_batch_initial_word_gp __asm__("D_L13_00161F08") = 0x30108010;

int moby_spark_first_batch_second_word_gp __asm__("D_L13_00161F0C") = 0x10108010;

int moby_spark_second_batch_initial_word_gp __asm__("D_L13_00161F10") = 0x8010ff10;

int moby_spark_second_batch_second_word_gp __asm__("D_L13_00161F14") = 0x8010ff10;

typedef union {
    OvlQuad quad;
    float   f[4];
} L13EffectVector;

void update_poison_vessel_fragment(struct Moby *moby) __asm__("FUN_L13_0030c3b8");

void update_poison_vessel_fragment(struct Moby *moby) {
    char           *data = (char *)moby->pvars;
    L13EffectVector old_pos, position, velocity, acceleration, second_pos;
    float           angle, direction, amount;
    int             spark_frame_count, second_spark_frame_count, third_spark_frame_count;
    int             i;
    qcopy(&old_pos, &moby->pos);
    if (moby->command == 1) {
        velocity.f[3] = moby_spark_first_batch_discarded_velocity_w_initializer_gp;
        acceleration.f[3] = moby_spark_first_batch_acceleration_w_initializer_gp;
        i = 0;
        do {
            qcopy(&position, &moby->pos);
            position.f[0] += random_float_between(-0.065f, 0.065f);
            position.f[1] += random_float_between(-0.065f, 0.065f);
            position.f[2] += random_float_between(-0.065f, 0.065f);
            qzero(&velocity);
            i++;
            velocity.f[2] = random_float_between(
                moby_spark_first_batch_vertical_velocity_base_gp * frame_time * 0.9f,
                moby_spark_first_batch_vertical_velocity_base_gp * frame_time * 1.1f);
            angle = random_float_between(-moby_spark_first_batch_spread_degrees_gp,
                                         moby_spark_first_batch_spread_degrees_gp) *
                    0.017453292f;
            direction =
                fast_add_rotations(fast_arc_tan(*(float *)data, *(float *)(data + 4)), angle);
            acceleration.f[0] = fast_cos(direction) *
                                (moby_spark_first_batch_horizontal_acceleration_gp * frame_time);
            direction =
                fast_add_rotations(fast_arc_tan(*(float *)data, *(float *)(data + 4)), angle);
            acceleration.f[1] = fast_sin(direction) *
                                (moby_spark_first_batch_horizontal_acceleration_gp * frame_time);
            acceleration.f[2] = 0.0f;
            acceleration.f[2] = random_float_between(
                moby_spark_first_batch_vertical_acceleration_base_gp * frame_time * 0.9f,
                moby_spark_first_batch_vertical_acceleration_base_gp * frame_time * 1.1f);
            spark_frame_count = truncate_float_to_s32(FUN_001f96b0_c3b8(
                random_float_between(moby_spark_first_batch_frame_count_base_gp,
                                     moby_spark_first_batch_frame_count_base_gp +
                                         moby_spark_first_batch_frame_count_base_gp)));
            second_spark_frame_count = truncate_float_to_s32(FUN_001f96b0_c3b8(
                random_float_between(moby_spark_first_batch_second_frame_count_base_gp,
                                     moby_spark_first_batch_second_frame_count_base_gp * 1.2f)));
            third_spark_frame_count = truncate_float_to_s32(FUN_001f96b0_c3b8(
                random_float_between(moby_spark_first_batch_third_frame_count_base_gp,
                                     moby_spark_first_batch_third_frame_count_base_gp * 1.2f)));
            spawn_spark(&position,
                        &velocity,
                        &acceleration,
                        moby_spark_first_batch_initial_word_gp,
                        moby_spark_first_batch_second_word_gp,
                        spark_frame_count,
                        second_spark_frame_count,
                        third_spark_frame_count,
                        -1);
        } while (i < 2);
        amount = 0.017453292f;
        velocity.f[3] = moby_spark_second_batch_discarded_velocity_w_initializer_gp;
        acceleration.f[3] = moby_spark_second_batch_acceleration_w_initializer_gp;
        i = 1;
        do {
            qcopy(&second_pos, &moby->pos);
            second_pos.f[0] += random_float_between(-0.1f, 0.1f);
            second_pos.f[1] += random_float_between(-0.1f, 0.1f);
            second_pos.f[2] += random_float_between(-0.1f, 0.1f);
            random_float_between(-moby_spark_second_batch_discarded_random_bound_gp,
                                 moby_spark_second_batch_discarded_random_bound_gp);
            qzero(&velocity);
            i--;
            velocity.f[2] = random_float_between(
                moby_spark_second_batch_vertical_velocity_base_gp * frame_time * 0.7f,
                moby_spark_second_batch_vertical_velocity_base_gp * frame_time * 1.5f);
            angle = random_float_between(-moby_spark_second_batch_spread_degrees_gp,
                                         moby_spark_second_batch_spread_degrees_gp) *
                    amount;
            direction =
                fast_add_rotations(fast_arc_tan(*(float *)data, *(float *)(data + 4)), angle);
            acceleration.f[0] = fast_cos(direction) *
                                (moby_spark_second_batch_horizontal_acceleration_gp * frame_time);
            direction =
                fast_add_rotations(fast_arc_tan(*(float *)data, *(float *)(data + 4)), angle);
            acceleration.f[1] = fast_sin(direction) *
                                (moby_spark_second_batch_horizontal_acceleration_gp * frame_time);
            acceleration.f[2] = 0.0f;
            acceleration.f[2] = random_float_between(
                moby_spark_second_batch_vertical_acceleration_base_gp * frame_time * 0.7f,
                moby_spark_second_batch_vertical_acceleration_base_gp * frame_time * 1.5f);
            spark_frame_count = truncate_float_to_s32(FUN_001f96b0_c3b8(
                random_float_between(moby_spark_second_batch_frame_count_base_gp,
                                     moby_spark_second_batch_frame_count_base_gp +
                                         moby_spark_second_batch_frame_count_base_gp)));
            second_spark_frame_count = truncate_float_to_s32(FUN_001f96b0_c3b8(
                random_float_between(moby_spark_second_batch_second_frame_count_base_gp,
                                     moby_spark_second_batch_second_frame_count_base_gp * 1.2f)));
            third_spark_frame_count = truncate_float_to_s32(FUN_001f96b0_c3b8(
                random_float_between(moby_spark_second_batch_third_frame_count_base_gp,
                                     moby_spark_second_batch_third_frame_count_base_gp * 1.2f)));
            spawn_spark(&second_pos,
                        &velocity,
                        &acceleration,
                        moby_spark_second_batch_initial_word_gp,
                        moby_spark_second_batch_second_word_gp,
                        spark_frame_count,
                        second_spark_frame_count,
                        third_spark_frame_count,
                        -1);
        } while (i >= 0);
    }
    moby->rot.x = fast_add_rotations(moby->rot.x, *(float *)(data + 0x10));
    moby->rot.y = fast_add_rotations(moby->rot.y, *(float *)(data + 0x14));
    fast_vec_add(&moby->pos, &moby->pos, data);
    *(float *)(data + 8) -= D_0015ED70 * 14.6f;
    if (!(moby->pos.x < 0.0f) && !(moby->pos.y < 0.0f) && !(moby->pos.z < 0.0f)) {
        short life = *(short *)(data + 0x18);
        short duration = *(short *)(data + 0x1a);
        if (life < duration / 4) {
            *(float *)((char *)moby + 0x2c) = *(float *)(data + 0x1c) *
                                              convert_integer_to_float(life) /
                                              convert_integer_to_float(*(short *)(data + 0x1a) / 4);
        }
        if (fast_dec_timer_s16(data + 0x18) == 0)
            goto collision;
    }
    FUN_0020c828(moby);
    return;
collision:
    if (moby->command != 1)
        return;
    if (collision_line(&old_pos, &moby->pos, 2, (void *)(*(int *)&D_001413D0_lui), 0) == 0)
        return;
    FUN_L13_0030c2b0(moby);
    FUN_0020c828(moby);
}
