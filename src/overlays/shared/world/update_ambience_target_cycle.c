/* Level ambience (wind and sway): picks a random target, then eases toward it. */
#include "types.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "qcopy.h"

struct Amb {
    float base_rotation;
    float rotation;
    float rotation_step;
    short hC;
    short hE;
    float base_scalar;
    float scalar;
    float scalar_step;
    short h1C;
    short h1E;
};

extern float      D_0015ED6C;
extern float      scale_time(float) __asm__("FUN_001f96b0");
extern struct Amb D_L00_001CB7C0;

/* per-frame level ambience (wind/sway) state machine: picks a random target, then eases toward it */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5138.c: func_L00_002A5B20), where it is exact; names translated to the US level program. */

void update_ambience_target_cycle(void) __asm__("FUN_L00_002a4890");

void update_ambience_target_cycle(void) {
    enum {
        AMBIENCE_WAIT_AND_SELECT_TARGET = 0,
        AMBIENCE_INTERPOLATE_TO_TARGET = 1
    };
    enum {
        AMBIENCE_WAIT_AND_SELECT_SECOND_TARGET = 2,
        AMBIENCE_INTERPOLATE_TO_SECOND_TARGET = 3
    };
    enum {
        AMBIENCE_HOLD_STATE = 1,
        AMBIENCE_SINGLE_TARGET_CYCLE = 2,
        AMBIENCE_TWO_TARGET_CYCLE = 3
    };
    struct Amb *ambience_state = &D_L00_001CB7C0;
    float       interpolation_duration, target_scalar, target_rotation, scalar_magnitude,
        scalar_clamp_value;
    switch (ambience_state->hC) {
    case AMBIENCE_HOLD_STATE:
        break;
    case AMBIENCE_SINGLE_TARGET_CYCLE:
        switch (ambience_state->hE) {
        case AMBIENCE_WAIT_AND_SELECT_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                interpolation_duration = convert_integer_to_float(ambience_state->h1C) *
                                         random_float_between(0.5f, 1.5f);
                target_rotation = fast_add_rotations(ambience_state->base_rotation,
                                                     random_float_between(-1.0471976f, 1.0471976f));
                target_scalar = random_float_between(ambience_state->base_scalar * 0.5f,
                                                     ambience_state->base_scalar * 1.5f);
                ambience_state->rotation_step =
                    fast_subtract_rotations(target_rotation, ambience_state->rotation) /
                    interpolation_duration;
                ambience_state->scalar_step =
                    (target_scalar - ambience_state->scalar) / interpolation_duration;
                ambience_state->h1E = truncate_float_to_s32(interpolation_duration);
                ambience_state->hE = 1;
            }
            break;
        case AMBIENCE_INTERPOLATE_TO_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                ambience_state->h1E =
                    truncate_float_to_s32(convert_integer_to_float(ambience_state->h1C) *
                                          random_float_between(2.0f, 3.0f));
                ambience_state->hE = 0;
            } else {
                ambience_state->rotation =
                    fast_add_rotations(ambience_state->rotation, ambience_state->rotation_step);
                ambience_state->scalar = ambience_state->scalar + ambience_state->scalar_step;
                scalar_magnitude = absolute_float(ambience_state->scalar);
                scalar_clamp_value = D_0015ED6C * 9.6f;
                if (scalar_clamp_value < scalar_magnitude) {
                    if (ambience_state->scalar < 0.0f)
                        scalar_clamp_value = -scalar_clamp_value;
                    ambience_state->scalar = scalar_clamp_value;
                }
            }
            break;
        }
        break;
    case AMBIENCE_TWO_TARGET_CYCLE:
        switch (ambience_state->hE) {
        case AMBIENCE_WAIT_AND_SELECT_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                interpolation_duration = scale_time(random_float_between(45.0f, 60.0f));
                target_rotation = fast_add_rotations(ambience_state->base_rotation,
                                                     random_float_between(-1.5707964f, 1.5707964f));
                target_scalar =
                    random_float_between(ambience_state->base_scalar * 1.5f,
                                         ambience_state->base_scalar + ambience_state->base_scalar);
                ambience_state->rotation_step =
                    fast_subtract_rotations(target_rotation, ambience_state->rotation) /
                    interpolation_duration;
                ambience_state->scalar_step =
                    (target_scalar - ambience_state->scalar) / interpolation_duration;
                ambience_state->h1E = truncate_float_to_s32(interpolation_duration);
                ambience_state->hE = 1;
            }
            break;
        case AMBIENCE_INTERPOLATE_TO_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                ambience_state->h1E =
                    truncate_float_to_s32(convert_integer_to_float(ambience_state->h1C) *
                                          random_float_between(1.0f, 1.5f));
                ambience_state->hE = 2;
            } else {
                ambience_state->rotation =
                    fast_add_rotations(ambience_state->rotation, ambience_state->rotation_step);
                ambience_state->scalar = ambience_state->scalar + ambience_state->scalar_step;
                scalar_magnitude = absolute_float(ambience_state->scalar);
                scalar_clamp_value = D_0015ED6C * 9.6f;
                if (scalar_clamp_value < scalar_magnitude) {
                    if (ambience_state->scalar < 0.0f)
                        scalar_clamp_value = -scalar_clamp_value;
                    ambience_state->scalar = scalar_clamp_value;
                }
            }
            break;
        case AMBIENCE_WAIT_AND_SELECT_SECOND_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                interpolation_duration = scale_time(random_float_between(45.0f, 60.0f));
                target_rotation = fast_add_rotations(ambience_state->base_rotation,
                                                     random_float_between(-0.785398f, 0.785398f));
                target_scalar = random_float_between(ambience_state->base_scalar * 0.8f,
                                                     ambience_state->base_scalar * 1.2f);
                ambience_state->rotation_step =
                    fast_subtract_rotations(target_rotation, ambience_state->rotation) /
                    interpolation_duration;
                ambience_state->scalar_step =
                    (target_scalar - ambience_state->scalar) / interpolation_duration;
                ambience_state->h1E = truncate_float_to_s32(interpolation_duration);
                ambience_state->hE = 3;
            }
            break;
        case AMBIENCE_INTERPOLATE_TO_SECOND_TARGET:
            if (fast_dec_timer_s16(&ambience_state->h1E)) {
                ambience_state->h1E =
                    truncate_float_to_s32(convert_integer_to_float(ambience_state->h1C) *
                                          random_float_between(3.0f, 5.0f));
                ambience_state->hE = 0;
            } else {
                ambience_state->rotation =
                    fast_add_rotations(ambience_state->rotation, ambience_state->rotation_step);
                ambience_state->scalar = ambience_state->scalar + ambience_state->scalar_step;
                scalar_magnitude = absolute_float(ambience_state->scalar);
                scalar_clamp_value = D_0015ED6C * 9.6f;
                if (scalar_clamp_value < scalar_magnitude) {
                    if (ambience_state->scalar < 0.0f)
                        scalar_clamp_value = -scalar_clamp_value;
                    ambience_state->scalar = scalar_clamp_value;
                }
            }
            break;
        }
        break;
    }
}

s16 D_L01_001CBC4C[1] __attribute__((section(".data"))) = {0};

s16 D_L02_001CBDCC[1] __attribute__((section(".data"))) = {0};

s16 D_L03_001CB8CC[1] __attribute__((section(".data"))) = {0};

s16 D_L05_001CBC4C[1] __attribute__((section(".data"))) = {0};

s16 D_L06_001CBF4C[1] __attribute__((section(".data"))) = {0};

s16 D_L07_001CB84C[1] __attribute__((section(".data"))) = {0};

s16 D_L08_001CBFCC[1] __attribute__((section(".data"))) = {0};

s16 D_L09_001CB94C[1] __attribute__((section(".data"))) = {0};

s16 D_L10_001CBC4C[1] __attribute__((section(".data"))) = {0};

s16 D_L11_001CC1CC[1] __attribute__((section(".data"))) = {0};

s16 D_L12_001CBC4C[1] __attribute__((section(".data"))) = {0};

s16 D_L13_001CBACC[1] __attribute__((section(".data"))) = {0};

s16 D_L14_001CBECC[1] __attribute__((section(".data"))) = {0};

s16 D_L15_001CBDCC[1] __attribute__((section(".data"))) = {0};

s16 D_L16_001CBC4C[1] __attribute__((section(".data"))) = {0};

s16 D_L17_001CC0CC[1] __attribute__((section(".data"))) = {0};

s16 D_L18_001CC1CC[1] __attribute__((section(".data"))) = {0};

struct Amb D_L01_001CBC40 = {0};

struct Amb D_L02_001CBDC0 = {0};

struct Amb D_L03_001CB8C0 = {0};

struct Amb D_L05_001CBC40 = {0};

struct Amb D_L06_001CBF40 = {0};

struct Amb D_L07_001CB840 = {0};

struct Amb D_L08_001CBFC0 = {0};

struct Amb D_L09_001CB940 = {0};

struct Amb D_L10_001CBC40 = {0};

struct Amb D_L11_001CC1C0 = {0};

struct Amb D_L12_001CBC40 = {0};

struct Amb D_L13_001CBAC0 = {0};

struct Amb D_L14_001CBEC0 = {0};

struct Amb D_L15_001CBDC0 = {0};

struct Amb D_L16_001CBC40 = {0};

struct Amb D_L17_001CC0C0 = {0};

struct Amb D_L18_001CC1C0 = {0};
