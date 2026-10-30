/* Troopers: sets a trooper's knockback, and emits a radial particle burst. */
#include "types.h"
#include "asm.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00249488.c: func_L03_00251A58), where it is exact; names translated to the US level program. */

void trooper_set_knockback(float *p, float a, float b) __asm__("FUN_L03_00250a78");
void trooper_set_knockback(float *knockback_state,
                           float  horizontal_knockback_scale,

                           float vertical_knockback_scale) {
    float root =
        square_root((vertical_knockback_scale + vertical_knockback_scale) * knockback_state[4]);
    float x = (root + root) / knockback_state[4];
    knockback_state[7] = root;
    knockback_state[6] = (horizontal_knockback_scale + horizontal_knockback_scale) / x;
    knockback_state[5] = (horizontal_knockback_scale + horizontal_knockback_scale) / (x * x);
}

#include "eetypes.h"
#include "rnc/math/fast_math.h"

extern void *spawn_particle(void *, void *, s32, s32, s32, s32, f32, f32, f32, f32, f32) __asm__("FUN_L00_002712b8");
extern void *spawn_burst(void *, void *, s32, s32, s32, s32, f32) __asm__("FUN_L00_0026cbb0");

void emit_radial_particle_burst(char *orientation_source,
                                u128 *emission_position,
                                void *velocity_addend) __asm__("FUN_L03_00250ae8");

void emit_radial_particle_burst(char *orientation_source,
                                u128 *emission_position,

                                void *velocity_addend) {
    struct {
        u128 position;
        u128 velocity;
        u128 temp;
    } vectors;
    s32   i;
    s32   j;
    u128 *position = &vectors.position;
    s32   spin;
    s32   duration;
    s32   minimum;
    s32   maximum;
    f32   angle;

    vectors.position = *emission_position;

    for (i = 0; i < truncate_float_to_s32(24.0f); i++) {
        angle = randf_sym(0.0f, 0.1308997f);
        build_look_at_matrix(&vectors.velocity,
                             orientation_source + 0xe0,
                             orientation_source + 0xc0,
                             (f32)i * 0.2617994f + angle);
        /* Fakematch: the empty scopes add 20 notes to loop 1, past haifa's 100-luid region limit,
         * so sched1 schedules this loop block by block as retail does (they emit no code). */
        { s32 pad; }
        { s32 pad; }
        { s32 pad; }
        { s32 pad; }
        { s32 pad; }
        { s32 pad; }
        { s32 pad; }
        { s32 pad; }
        { s32 pad; }
        { s32 pad; }
        fast_vec_normalize(&vectors.velocity, &vectors.velocity, random_float_between(0.05f, 0.1f));
        spin = random_int_between(0, 3);
        if (random_integer_below(2))
            spin = -spin;
        minimum = scale_game_frames(30);
        maximum = scale_game_frames(90);
        duration = random_int_between(minimum, maximum);
        if (velocity_addend)
            add_vectors(&vectors.temp, &vectors.velocity, velocity_addend);
        spawn_particle(position,
                       &vectors.velocity,
                       duration,
                       30,
                       0xffffff,
                       spin,
                       60000.0f,
                       3000.0f,
                       0.85f,
                       -0.001f,
                       0.85f);
    }
    for (j = 0; j < truncate_float_to_s32(6.0f);) {
        angle = random_float_between(0.0f, 0.7853982f);
        build_look_at_matrix(&vectors.velocity,
                             orientation_source + 0xe0,
                             orientation_source + 0xc0,
                             (f32)j++ * 1.0471976f + angle);
        fast_vec_normalize(&vectors.velocity, &vectors.velocity, random_float_between(0.05f, 0.1f));
        minimum = scale_game_frames(20);
        maximum = scale_game_frames(60);
        duration = random_int_between(minimum, maximum);
        if (velocity_addend)
            add_vectors(&vectors.temp, &vectors.velocity, velocity_addend);
        spawn_burst(position, &vectors.velocity, 0x4f007fff, 0x1fffffff, duration, 1, 10000.0f);
    }
}
