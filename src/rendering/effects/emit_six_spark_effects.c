#include "types.h"
#include "rnc/rendering/level_render_state.h"
#include "sda.h"
#include "eetypes.h"
#include "qcopy.h"

#include "rnc/math/vector.h"


extern u128 D_001D9B60[][6];

extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 spawn_spark(void *, void *, void *, s32, s32, s32, s32, s32, s32) __asm__("func_00218888");

void emit_six_spark_effects(u8 *m, f32 z) __asm__("FUN_0022f5b0");

extern f32 frame_time __asm__("D_0015ED6C") MACRO_ADDR;
void emit_six_spark_effects(u8 *source_object, f32 z) {
    Vec4 vel;
    Vec4 vel2;
    Vec4 pos;
    s32 i;
    s32 frame_count;
    s32 b;
    s32 third_frame_base_count;

    for (i = 0; i < 6; i++) {
        vel.f[0] = random_float_between(-frame_time, frame_time);
        vel.f[1] = random_float_between(-frame_time, frame_time);
        vel.f[2] = z + random_float_between(frame_time * -0.25f, frame_time * 0.25f);
        vel.f[3] = 0.4f;
        qcopy(&vel2, &vel);
        vel2.f[3] = 0.6f;
        qcopy(&pos, &D_001D9B60[level_render_state.content_variant][i]);
        transform_vector_by_basis(&pos, &pos, source_object + 0xC0);
        add_vector_xyz(&pos, &pos, source_object + 0x10);
        frame_count = scale_game_frames(4);
        b = scale_game_frames(4);
        third_frame_base_count = scale_game_frames(4);
        spawn_spark(&pos, &vel, &vel2, 0x24C0C0C0, 0x14C0C0C0, frame_count, b,
                      third_frame_base_count + random_integer_below(scale_game_frames(4)), -1);
    }
}

extern __typeof__(emit_six_spark_effects) func_0022F5B0 __attribute__((alias("FUN_0022f5b0")));
