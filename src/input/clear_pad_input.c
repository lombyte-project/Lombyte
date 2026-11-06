#include "types.h"
#include "rnc/input/pad_state.h"

/* One PadState.analog_prev entry, cleared as a word. */
struct WordCell {
    s32 bits;
};

void clear_pad_input(struct PadState *pad_state) __asm__("FUN_002172c0");

void clear_pad_input(struct PadState *pad_state) {
    s32              remaining;
    struct WordCell *slot;

    slot = ((u8 *)pad_state + (0x140));
    remaining = 0xF;
    pad_state->held = 0;
    pad_state->pressed = 0;
    pad_state->released = 0;
    pad_state->no_buttons = 1;
    pad_state->raw_pressed = 0;
    pad_state->raw_released = 0;
    pad_state->held_unmasked = 0;
    pad_state->pressed_unmasked = 0;
    pad_state->released_unmasked = 0;
    pad_state->stick_moved = 0;
    pad_state->no_direction = 1;
    pad_state->raw_held = 0;
    do {
        *(s32 *)((u8 *)slot - 0x40) = 0;
        remaining -= 1;
        slot->bits = 0;
        slot += 1;
    } while (remaining >= 0);
}

extern void func_002172C0(struct PadState *pad_state) __attribute__((alias("FUN_002172c0")));