/* Shared moby class 0x329: sets up the level's frame-scaled timings and advances a wrapped value; also the accumulator it resets. */
#include "types.h"
#include "rnc/math/fast_math.h"
#include "rnc/overlay/shared/props/particle_spawner_smoke_fire_controller_data/update_frame_timing_and_wrapped_value_2.h"
#include "rnc/overlay/shared/props/particle_spawner_smoke_fire_controller_data/update_frame_timing_and_wrapped_value.h"
#include "rnc/globals.h"
#include "qcopy.h"

extern int configured_unscaled_frame_count_gp __asm__("D_L00_00161368") __attribute__((sda));
extern int configured_reciprocal_frame_count_gp __asm__("D_L00_0016136C") __attribute__((sda));

extern int   D_L00_0016137C_gp __asm__("D_L00_0016137C") __attribute__((sda));
extern int   D_L00_00161380 __attribute__((sda));
extern int   D_L00_00161388;
extern float D_L00_00161374_gp __asm__("D_L00_00161374") __attribute__((sda));
extern float wrapped_value_decrement_rate_gp __asm__("D_L00_00161378") __attribute__((sda));
extern float D_L00_00161384_gp __asm__("D_L00_00161384") __attribute__((sda));
extern float D_L00_0016138C_gp __asm__("D_L00_0016138C") __attribute__((sda));

void update_frame_timing_and_wrapped_value(unsigned char *state_object) __asm__("FUN_L00_002a4cd8");

void update_frame_timing_and_wrapped_value(unsigned char *state_object) {
    enum {
        INITIALIZE_FRAME_TIMING = 0,
        UPDATE_WRAPPED_VALUE = 1
    };
    float one;
    int   scaled_frame_count;
    switch (state_object[0x20]) {
    case INITIALIZE_FRAME_TIMING:
        fetch_add_accumulator = 0;
        state_object[0x20] = 1;
        state_object[0x30] = 0xFF;
        one = 1.0f;
        D_L00_0016137C_gp = scale_game_frames(configured_unscaled_frame_count_gp);
        scaled_frame_count = scale_game_frames(configured_reciprocal_frame_count_gp);
        D_L00_00161380 = scaled_frame_count;
        D_L00_00161384_gp = one / convert_integer_to_float(scaled_frame_count);
        scaled_frame_count = scale_game_frames(configured_reciprocal_denominator_frames);
        D_L00_00161388 = scaled_frame_count;
        D_L00_0016138C_gp = one / (float)scaled_frame_count;
        break;
    case UPDATE_WRAPPED_VALUE:
        D_L00_00161374_gp -= wrapped_value_decrement_rate_gp * frame_scale;
        if (D_L00_00161374_gp <= -8.0f)
            D_L00_00161374_gp += 8.0f;
        break;
    }
}

int fetch_and_add_global_accumulator(int addend) __asm__("FUN_L00_002a4dc8");

int fetch_and_add_global_accumulator(int addend) {
    int previous_value = fetch_add_accumulator;
    fetch_add_accumulator = previous_value + addend;
    return previous_value;
}

int   D_L01_00161368 = 0x3c;
int   D_L01_0016136C = 0x3c;
int   D_L01_0016137C = 0;
int   D_L01_00161380 = 0;
int   D_L01_00161388 = 0;
float D_L01_00161374 = 0;
float D_L01_00161378 = 0.0025f;
float D_L01_00161384 = 0;
float D_L01_0016138C = 0;

int   D_L14_00161368 = 0x3c;
int   D_L14_0016136C = 0x3c;
int   D_L14_0016137C = 0;
int   D_L14_00161380 = 0;
int   D_L14_00161388 = 0;
float D_L14_00161374 = 0;
float D_L14_00161378 = 0.0025f;
float D_L14_00161384 = 0;
float D_L14_0016138C = 0;

int configured_unscaled_frame_count_gp __asm__("D_L00_00161368") = 0x3c;
