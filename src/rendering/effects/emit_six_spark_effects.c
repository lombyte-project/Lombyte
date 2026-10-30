#include "types.h"
#include "rnc/rendering/effects/space_effect_tables.h"
#include "rnc/rendering/level_render_state.h"
#include "sda.h"
#include "eetypes.h"
#include "qcopy.h"

#include "rnc/math/vector.h"
#include "rnc/math/fast_math.h"
#include "rnc/globals.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"

extern s32
spawn_spark(void *, void *, void *, s32, s32, s32, s32, s32, s32) __asm__("func_00218888");

void emit_six_spark_effects(u8 *m, f32 z) __asm__("FUN_0022f5b0");

void emit_six_spark_effects(u8 *source_object, f32 z) {
    Vec4 randomized_spark_vector;
    Vec4 scalar_adjusted_spark_vector;
    Vec4 spark_spawn_position;
    s32  i;
    s32  frame_count;
    s32  second_frame_count;
    s32  third_frame_base_count;

    for (i = 0; i < 6; i++) {
        randomized_spark_vector.f[0] = random_float_between(-frame_time, frame_time);
        randomized_spark_vector.f[1] = random_float_between(-frame_time, frame_time);
        randomized_spark_vector.f[2] =
            z + random_float_between(frame_time * -0.25f, frame_time * 0.25f);
        randomized_spark_vector.f[3] = 0.4f;
        qcopy(&scalar_adjusted_spark_vector, &randomized_spark_vector);
        scalar_adjusted_spark_vector.f[3] = 0.6f;
        qcopy(&spark_spawn_position,
              &spark_spawn_points_by_variant[level_render_state.content_variant][i]);
        transform_vector_by_basis(
            &spark_spawn_position, &spark_spawn_position, source_object + 0xC0);
        fast_vec_add(&spark_spawn_position, &spark_spawn_position, source_object + 0x10);
        frame_count = scale_game_frames(4);
        second_frame_count = scale_game_frames(4);
        third_frame_base_count = scale_game_frames(4);
        spawn_spark(&spark_spawn_position,
                    &randomized_spark_vector,
                    &scalar_adjusted_spark_vector,
                    0x24C0C0C0,
                    0x14C0C0C0,
                    frame_count,
                    second_frame_count,
                    third_frame_base_count + random_integer_below(scale_game_frames(4)),
                    -1);
    }
}

extern __typeof__(emit_six_spark_effects) func_0022F5B0 __attribute__((alias("FUN_0022f5b0")));

f32 spark_spawn_points_by_variant[3][6][4] = {
    { {1.1f, 0.0f, 0.7f, 0.0f},
     {2.1f, 0.0f, 0.7f, 0.0f},
     {-0.6f, -0.9f, 0.7f, 0.0f},
     {-1.6f, -0.9f, 0.7f, 0.0f},
     {-0.6f, 0.9f, 0.7f, 0.0f},
     {-1.6f, 0.9f, 0.7f, 0.0f}  },
    {{2.25f, 0.0f, 1.0f, 0.0f},
     {3.5f, 0.0f, 1.0f, 0.0f},
     {-0.25f, -1.25f, 1.0f, 0.0f},
     {-1.25f, -1.25f, 1.0f, 0.0f},
     {-0.25f, 1.25f, 1.0f, 0.0f},
     {-1.25f, 1.25f, 1.0f, 0.0f}},
    {{2.66f, 0.0f, 0.8f, 0.0f},
     {4.33f, 0.0f, 0.8f, 0.0f},
     {-2.0f, -3.2f, 0.8f, 0.0f},
     {-2.0f, -1.5f, 0.8f, 0.0f},
     {-2.0f, 3.2f, 0.8f, 0.0f},
     {-2.0f, 1.5f, 0.8f, 0.0f}  },
};
