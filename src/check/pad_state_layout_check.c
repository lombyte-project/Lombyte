/* Compiled by make check with the EE compiler; never linked. */
#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]

#include "rnc/input/pad_state.h"

OFFSET_CHECK(analog, struct PadState, analog, 0x100);
OFFSET_CHECK(analog_prev, struct PadState, analog_prev, 0x140);
OFFSET_CHECK(button_profile, struct PadState, button_profile, 0x180);
OFFSET_CHECK(vibration_profile, struct PadState, vibration_profile, 0x184);
OFFSET_CHECK(unk188, struct PadState, unk188, 0x188);
OFFSET_CHECK(unk189, struct PadState, unk189, 0x189);
OFFSET_CHECK(hist_idx, struct PadState, hist_idx, 0x18E);
OFFSET_CHECK(hist_count, struct PadState, hist_count, 0x190);
OFFSET_CHECK(socket, struct PadState, socket, 0x194);
OFFSET_CHECK(profile_state, struct PadState, profile_state, 0x198);
OFFSET_CHECK(device_state, struct PadState, device_state, 0x19C);
OFFSET_CHECK(held, struct PadState, held, 0x1A0);
OFFSET_CHECK(pressed, struct PadState, pressed, 0x1A4);
OFFSET_CHECK(released, struct PadState, released, 0x1A8);
OFFSET_CHECK(prev_held, struct PadState, prev_held, 0x1AC);
OFFSET_CHECK(raw_held, struct PadState, raw_held, 0x1B0);
OFFSET_CHECK(raw_pressed, struct PadState, raw_pressed, 0x1B4);
OFFSET_CHECK(raw_released, struct PadState, raw_released, 0x1B8);
OFFSET_CHECK(prev_raw_held, struct PadState, prev_raw_held, 0x1BC);
OFFSET_CHECK(held_unmasked, struct PadState, held_unmasked, 0x1C0);
OFFSET_CHECK(pressed_unmasked, struct PadState, pressed_unmasked, 0x1C4);
OFFSET_CHECK(released_unmasked, struct PadState, released_unmasked, 0x1C8);
OFFSET_CHECK(mode, struct PadState, mode, 0x1CC);
OFFSET_CHECK(no_buttons, struct PadState, no_buttons, 0x1D0);
OFFSET_CHECK(no_direction, struct PadState, no_direction, 0x1D4);
OFFSET_CHECK(stick_moved, struct PadState, stick_moved, 0x1D8);
OFFSET_CHECK(pressure_mode_id, struct PadState, pressure_mode_id, 0x1DC);
OFFSET_CHECK(pressed_history, struct PadState, pressed_history, 0x1E0);
OFFSET_CHECK(angle_history, struct PadState, angle_history, 0x258);
OFFSET_CHECK(float_history, struct PadState, float_history, 0x2D0);
OFFSET_CHECK(saved_held, struct PadState, saved_held, 0x348);
OFFSET_CHECK(repeat_delay, struct PadState, repeat_delay, 0x34D);
OFFSET_CHECK(repeat_rate, struct PadState, repeat_rate, 0x34E);
OFFSET_CHECK(repeat_timer, struct PadState, repeat_timer, 0x34F);

typedef char pad_state_size_check[sizeof(struct PadState) == 0x350 ? 1 : -1];

OFFSET_CHECK(words_held_pressed, struct PadStateWords, buttons.held_pressed, 0x1A0);
OFFSET_CHECK(words_unmasked, struct PadStateWords, unmasked.held_pressed, 0x1C0);
OFFSET_CHECK(words_unmasked_pressed, struct PadStateWords, unmasked.w.pressed, 0x1C4);
OFFSET_CHECK(words_stick_moved, struct PadStateWords, stick_moved, 0x1D8);
typedef char pad_unmasked_pair_width_check
    [sizeof(((struct PadStateWords *)0)->unmasked.held_pressed) == 8 ? 1 : -1];
