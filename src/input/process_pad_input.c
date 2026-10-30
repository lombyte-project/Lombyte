#include "types.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

typedef struct Pad {
    u8    pad0[0x100];
    float axes[16]; /* 0x100 */
    float prev[16]; /* 0x140 */
    u8    pad180[0xE];
    s16   idx;   /* 0x18E */
    s32   count; /* 0x190 */
    u8    pad194[0xC];
    s32   held;     /* 0x1A0 */
    s32   pressed;  /* 0x1A4 */
    s32   released; /* 0x1A8 */
    s32   old;      /* 0x1AC */
    s32   raw;      /* 0x1B0 */
    s32   x1B4;     /* 0x1B4 */
    s32   x1B8;     /* 0x1B8 */
    s32   x1BC;     /* 0x1BC */
    s32   x1C0;     /* 0x1C0 */
    s32   x1C4;     /* 0x1C4 */
    s32   x1C8;     /* 0x1C8 */
    s32   mode;     /* 0x1CC */
    s32   none;     /* 0x1D0 */
    s32   nodir;    /* 0x1D4 */
    s32   moving;   /* 0x1D8 */
    u8    pad1DC[4];
    s32   hist_btn[30]; /* 0x1E0 */
    float hist_ang[30]; /* 0x258 */
    float hist_mag[30]; /* 0x2D0 */
    s32   x348;         /* 0x348 */
    u8    pad34C;
    u8    rep_delay; /* 0x34D */
    u8    rep_rate;  /* 0x34E */
    u8    rep_timer; /* 0x34F */
} Pad;

extern volatile u8 D_0015EDB4;
extern float       fast_difference_between_rotations(float, float) __asm__("func_001FA688");

#define SWAPLR(x)                       \
    if ((x) & 0x8000) {                 \
        (x) = ((x) & ~0x8000) | 0x2000; \
    } else if ((x) & 0x2000) {          \
        (x) = ((x) & ~0x2000) | 0x8000; \
    }

void process_pad_input(Pad *p, u8 *buf, s32 len) __asm__("FUN_00217328");

void process_pad_input(Pad *pad_state, u8 *pad_read_data, s32 pad_read_byte_count) {
    float axis_vector[2];
    s32   i, j, k;
    s32   held_buttons, old, ncur, nold;
    float mag, axis_vector_angle;

    pad_state->raw = pad_state->held = ((pad_read_data[0] << 8) | pad_read_data[1]) ^ 0xFFFF;
    for (i = 15; i >= 0; i--) {
        pad_state->axes[i] = 0;
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
                pad_state->axes[i] = scaled_axis_magnitude;
                if (scaled_axis_magnitude > 1.0f) {
                    pad_state->axes[i] = 1.0f;
                }
                if (pad_read_data[i + 2] < 0x7F) {
                    pad_state->axes[i] = -pad_state->axes[i];
                }
            }
        }
    }
    if (pad_read_byte_count >= 0x12) {
        for (i = 4; i < 16; i++) {
            pad_state->axes[i] = convert_integer_to_float(pad_read_data[i + 2]) * 0.003921569f;
        }
    }
    if (D_0015EDB4) {
        pad_state->axes[2] = -pad_state->axes[2];
        pad_state->axes[0] = -pad_state->axes[0];
        SWAPLR(pad_state->held);
        pad_state->raw = pad_state->held;
    }
    {
        float *axis_copy_destination = pad_state->prev, *axis_copy_source = pad_state->axes;
        for (i = 15; i >= 0; i--) {
            *axis_copy_destination++ = *axis_copy_source++;
        }
    }
    if (((volatile float *)pad_state->axes)[2] != 0.0f ||
        ((volatile float *)pad_state->axes)[3] != 0.0f) {
        pad_state->moving = 1;
    } else {
        pad_state->moving = 0;
    }
    if (pad_state->axes[2] < 0.0f)
        pad_state->held |= 0x8000;
    if (pad_state->axes[2] > 0.0f)
        pad_state->held |= 0x2000;
    if (pad_state->axes[3] < 0.0f)
        pad_state->held |= 0x1000;
    if (pad_state->axes[3] > 0.0f)
        pad_state->held |= 0x4000;
    pad_state->nodir = (pad_state->held & 0xF000) == 0;
    pad_state->none = pad_state->held == 0;
    pad_state->pressed = ~pad_state->old & pad_state->held;
    pad_state->x1B4 = ~pad_state->old & pad_state->raw;
    pad_state->released = ~pad_state->held & pad_state->old;
    pad_state->x1B8 = ~pad_state->held & pad_state->x1BC;
    pad_state->x1C4 = pad_state->pressed;
    pad_state->x1C8 = pad_state->released;
    pad_state->x1C0 = pad_state->held;
    pad_state->x348 = pad_state->held;
    if (D_0015EDB4) {
        pad_state->prev[2] = -pad_state->prev[2];
        pad_state->prev[0] = -pad_state->prev[0];
        SWAPLR(pad_state->x1C0);
        SWAPLR(pad_state->x1C4);
        SWAPLR(pad_state->x1C8);
    }
    if (pad_state->mode == 1) {
        pad_state->held = *(volatile s32 *)&pad_state->held & ~0x5030;
        pad_state->pressed &= ~0x5030;
        pad_state->released &= ~0x5030;
        pad_state->axes[0] = 0.0f;
        pad_state->axes[1] = 0.0f;
        pad_state->mode = 0;
    }
    if (pad_state->mode == 2) {
        pad_state->held = *(volatile s32 *)&pad_state->held & 0x900;
        pad_state->pressed &= 0x900;
        pad_state->released &= 0x900;
        pad_state->raw = *(volatile s32 *)&pad_state->raw & 0x900;
        pad_state->nodir = 1;
        pad_state->axes[2] = 0.0f;
        pad_state->axes[3] = 0.0f;
        pad_state->mode = 0;
    }
    axis_vector[0] = pad_state->axes[2];
    axis_vector[1] = pad_state->axes[3];
    mag = vector_length_xy(axis_vector);
    axis_vector_angle = fast_arc_tan(axis_vector[0], axis_vector[1]);
    pad_state->hist_mag[pad_state->idx] = mag;
    pad_state->hist_ang[pad_state->idx] = axis_vector_angle;
    if (mag > 0.9f) {
        for (j = 1; j < scale_game_frames(4); j++) {
            float historical_magnitude = pad_state->hist_mag[(pad_state->idx - j + 30) % 30];
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
                        pad_state->hist_ang[(pad_state->idx - k + 30) % 30], axis_vector_angle) >
                    0.9599311f) {
                    pad_state->pressed |= 0x10000;
                    break;
                }
            }
        }
    }
    pad_state->hist_btn[pad_state->idx] = pad_state->pressed;
    pad_state->idx = (pad_state->idx + 1) % 30;
    if (++pad_state->count > 30) {
        pad_state->count = 30;
    }
    if (pad_state->rep_rate) {
        held_buttons = pad_state->held;
        if (held_buttons != 0 && held_buttons == pad_state->old) {
            u8 repeat_timer_value = --pad_state->rep_timer;
            if (0 == repeat_timer_value || repeat_timer_value == 0xFF) {
                pad_state->rep_timer = pad_state->rep_rate;
                pad_state->pressed = held_buttons;
            }
        } else {
            pad_state->rep_timer = pad_state->rep_delay;
        }
    }
}

extern __typeof__(process_pad_input) func_00217328 __attribute__((alias("FUN_00217328")));
