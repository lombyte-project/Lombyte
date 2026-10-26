/* Reset the paired global fields used by the frame state. */

#include "rnc/input/pad_state.h"

void reset_global_state_fields(void) __asm__("func_00217020");

void reset_global_state_fields(void) {
    controller_state.hist_idx = 0;
    controller_state.hist_count = 0;
}
