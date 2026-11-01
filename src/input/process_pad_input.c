#include "types.h"
#include "rnc/input/pad_state.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

extern volatile u8 D_0015EDB4;
extern float       fast_difference_between_rotations(float, float) __asm__("func_001FA688");

#define SWAPLR(x)                       \
    if ((x) & 0x8000) {                 \
        (x) = ((x) & ~0x8000) | 0x2000; \
    } else if ((x) & 0x2000) {          \
        (x) = ((x) & ~0x2000) | 0x8000; \
    }

void process_pad_input(struct PadState *p, u8 *buf, s32 len) __asm__("FUN_00217328");

void process_pad_input(struct PadState *pad_state, u8 *pad_read_data, s32 pad_read_byte_count) {
    enum {
        PAD_INPUT_FILTER_NONE = 0,
        PAD_INPUT_FILTER_SUPPRESS_DIRECTIONS = 2
    };
    float axis_vector[2];
    s32   i, j, k;
    s32   held_buttons, old, ncur, nold;
    float mag, axis_vector_angle;

    pad_state->raw_held = pad_state->held = ((pad_read_data[0] << 8) | pad_read_data[1]) ^ 0xFFFF;
    for (i = 15; i >= 0; i--) {
        pad_state->analog[i] = 0;
    }
    if (pad_read_byte_count >= 6) {
        for (i = 0; i < 4; i++) {
            s32 axis_center_distance = pad_read_data[i + 2] - 0x7F;
            if (axis_center_distance < 0)
                axis_center_distance = -axis_center_distance;
            if (axis_center_distance >= 0x30) {
                float scaled_axis_magnitude =
                    convert_integer_to_float(axis_center_distance - 0x30) /
                    convert_integer_to_float(0x4C);
                pad_state->analog[i] = scaled_axis_magnitude;
                if (scaled_axis_magnitude > 1.0f) {
                    pad_state->analog[i] = 1.0f;
                }
                if (pad_read_data[i + 2] < 0x7F) {
                    pad_state->analog[i] = -pad_state->analog[i];
                }
            }
        }
    }
    if (pad_read_byte_count >= 0x12) {
        for (i = 4; i < 16; i++) {
            pad_state->analog[i] = convert_integer_to_float(pad_read_data[i + 2]) * 0.003921569f;
        }
    }
    if (D_0015EDB4) {
        pad_state->analog[2] = -pad_state->analog[2];
        pad_state->analog[0] = -pad_state->analog[0];
        SWAPLR(pad_state->held);
        pad_state->raw_held = pad_state->held;
    }
    {
        float *axis_copy_destination = pad_state->analog_prev,
              *axis_copy_source = pad_state->analog;
        for (i = 15; i >= 0; i--) {
            *axis_copy_destination++ = *axis_copy_source++;
        }
    }
    if (((volatile float *)pad_state->analog)[2] != 0.0f ||
        ((volatile float *)pad_state->analog)[3] != 0.0f) {
        pad_state->stick_moved = 1;
    } else {
        pad_state->stick_moved = 0;
    }
    if (pad_state->analog[2] < 0.0f)
        pad_state->held |= 0x8000;
    if (pad_state->analog[2] > 0.0f)
        pad_state->held |= 0x2000;
    if (pad_state->analog[3] < 0.0f)
        pad_state->held |= 0x1000;
    if (pad_state->analog[3] > 0.0f)
        pad_state->held |= 0x4000;
    pad_state->no_direction = (pad_state->held & 0xF000) == 0;
    pad_state->no_buttons = pad_state->held == 0;
    pad_state->pressed = ~pad_state->prev_held & pad_state->held;
    pad_state->raw_pressed = ~pad_state->prev_held & pad_state->raw_held;
    pad_state->released = ~pad_state->held & pad_state->prev_held;
    pad_state->raw_released = ~pad_state->held & pad_state->prev_raw_held;
    (*(s32 *)&pad_state->pressed_unmasked) = pad_state->pressed;
    (*(s32 *)&pad_state->released_unmasked) = pad_state->released;
    (*(s32 *)&pad_state->held_unmasked) = pad_state->held;
    pad_state->saved_held = pad_state->held;
    if (D_0015EDB4) {
        pad_state->analog_prev[2] = -pad_state->analog_prev[2];
        pad_state->analog_prev[0] = -pad_state->analog_prev[0];
        SWAPLR((*(s32 *)&pad_state->held_unmasked));
        SWAPLR((*(s32 *)&pad_state->pressed_unmasked));
        SWAPLR((*(s32 *)&pad_state->released_unmasked));
    }
    if (pad_state->mode == 1) {
        pad_state->held = *(volatile s32 *)&pad_state->held & ~0x5030;
        pad_state->pressed &= ~0x5030;
        pad_state->released &= ~0x5030;
        pad_state->analog[0] = 0.0f;
        pad_state->analog[1] = 0.0f;
        pad_state->mode = PAD_INPUT_FILTER_NONE;
    }
    if (pad_state->mode == PAD_INPUT_FILTER_SUPPRESS_DIRECTIONS) {
        pad_state->held = *(volatile s32 *)&pad_state->held & 0x900;
        pad_state->pressed &= 0x900;
        pad_state->released &= 0x900;
        pad_state->raw_held = *(volatile s32 *)&pad_state->raw_held & 0x900;
        pad_state->no_direction = 1;
        pad_state->analog[2] = 0.0f;
        pad_state->analog[3] = 0.0f;
        pad_state->mode = PAD_INPUT_FILTER_NONE;
    }
    axis_vector[0] = pad_state->analog[2];
    axis_vector[1] = pad_state->analog[3];
    mag = vector_length_xy(axis_vector);
    axis_vector_angle = fast_arc_tan(axis_vector[0], axis_vector[1]);
    pad_state->float_history[pad_state->hist_idx] = mag;
    pad_state->angle_history[pad_state->hist_idx] = axis_vector_angle;
    if (mag > 0.9f) {
        for (j = 1; j < scale_game_frames(4); j++) {
            float historical_magnitude =
                pad_state->float_history[(pad_state->hist_idx - j + 30) % 30];
            if (historical_magnitude > 0.9f)
                break;
            if (historical_magnitude < 0.25f) {
                pad_state->pressed |= 0x10000;
                break;
            }
        }
        if (!(pad_state->pressed & 0x10000)) {
            for (k = 1; k < scale_game_frames(5); k++) {
                if (fast_difference_between_rotations(
                        pad_state->angle_history[(pad_state->hist_idx - k + 30) % 30],
                        axis_vector_angle) > 0.9599311f) {
                    pad_state->pressed |= 0x10000;
                    break;
                }
            }
        }
    }
    pad_state->pressed_history[pad_state->hist_idx] = pad_state->pressed;
    pad_state->hist_idx = (pad_state->hist_idx + 1) % 30;
    if (++pad_state->hist_count > 30) {
        pad_state->hist_count = 30;
    }
    if (pad_state->repeat_rate) {
        held_buttons = pad_state->held;
        if (held_buttons != 0 && held_buttons == pad_state->prev_held) {
            u8 repeat_timer_value = --pad_state->repeat_timer;
            if (0 == repeat_timer_value || repeat_timer_value == 0xFF) {
                pad_state->repeat_timer = pad_state->repeat_rate;
                pad_state->pressed = held_buttons;
            }
        } else {
            pad_state->repeat_timer = pad_state->repeat_delay;
        }
    }
}

extern __typeof__(process_pad_input) func_00217328 __attribute__((alias("FUN_00217328")));
