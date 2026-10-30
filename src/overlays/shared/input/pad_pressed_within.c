/* Pad: press timing tests and the pad float history. */
#include "types.h"
#include "rnc/input/pad_state.h"
#include "asm.h"

u32 pad_pressed_within(u32 mask, s32 n, s32 *out) __asm__("FUN_L00_00266d60");

u32 pad_pressed_within(u32 mask, s32 n, s32 *press_age_frames_out) {
    s32 history_search_limit = controller_state.hist_count;
    s32 i;
    if (history_search_limit >= n)
        history_search_limit = n;
    for (i = 1; i < history_search_limit + 1; i++) {
        s32 k = (controller_state.hist_idx - i + 30) % 30;
        if (controller_state.pressed_history[k] & mask) {
            if (press_age_frames_out)
                *press_age_frames_out = i - 1;
            return controller_state.pressed_history[k] & mask;
        }
    }
    return 0;
}

/* Ported from rac1-decomp (src/overlays/shared/stream_002670E0.c: func_L00_00267C48), where it is exact; names translated to the US level program. */

extern s32 SubtractIntegerWithClamp(s32);

/* True when both lookups succeed and the clamped difference of their values is below d. */
int check_pad_press_separation(int first_button_mask,
                               int second_button_mask,
                               int lookback_frames,
                               int press_difference_limit) __asm__("FUN_L00_00266e00");

int check_pad_press_separation(int first_button_mask,
                               int second_button_mask,
                               int lookback_frames,

                               int press_difference_limit) {
    int press_ages_frames[2];
    int presses_within_limit = 0;
    if (pad_pressed_within(first_button_mask, lookback_frames, &press_ages_frames[0])) {
        if (pad_pressed_within(second_button_mask, lookback_frames, &press_ages_frames[1])) {
            presses_within_limit =
                SubtractIntegerWithClamp(press_ages_frames[0] - press_ages_frames[1]) <
                press_difference_limit;
        }
    }
    return presses_within_limit;
}

/* The value n entries back in the 30-entry history ring (n is capped at the count). */
f32 read_pad_float_history(s32 history_offset) __asm__("FUN_L00_00266e80");

f32 read_pad_float_history(s32 history_offset) {
    s32 m;
    if (controller_state.hist_count < history_offset)
        history_offset = controller_state.hist_count;
    return controller_state.float_history[(controller_state.hist_idx - history_offset + 30) % 30];
}
