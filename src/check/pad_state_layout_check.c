/* Built only by `make layout-check-sys` (never linked). gcc 2.95 has no
   _Static_assert: a negative array size fails the compile. */
#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[ \
        ((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]

#include "rnc/input/pad_state.h"

OFFSET_CHECK(analog, struct PadState, analog, 0x100);
OFFSET_CHECK(analog_prev, struct PadState, analog_prev, 0x140);
OFFSET_CHECK(socket, struct PadState, socket, 0x194);
OFFSET_CHECK(held, struct PadState, held, 0x1A0);
OFFSET_CHECK(pressed, struct PadState, pressed, 0x1A4);
OFFSET_CHECK(raw_pressed, struct PadState, raw_pressed, 0x1B4);
OFFSET_CHECK(profile_state, struct PadState, profile_state, 0x198);
OFFSET_CHECK(device_state, struct PadState, device_state, 0x19C);
OFFSET_CHECK(raw_released, struct PadState, raw_released, 0x1B8);
OFFSET_CHECK(prev_raw_held, struct PadState, prev_raw_held, 0x1BC);
OFFSET_CHECK(held_unmasked, struct PadState, held_unmasked, 0x1C0);
OFFSET_CHECK(pressed_unmasked, struct PadState, pressed_unmasked, 0x1C4);
OFFSET_CHECK(mode, struct PadState, mode, 0x1CC);
OFFSET_CHECK(no_direction, struct PadState, no_direction, 0x1D4);
OFFSET_CHECK(unk1DC, struct PadState, unk1DC, 0x1DC);
