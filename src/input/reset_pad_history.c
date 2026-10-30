#include "rnc/input/pad_state.h"

void reset_pad_history(void) __asm__("func_00217020");

void reset_pad_history(void) {
    controller_state.hist_idx = 0;
    controller_state.hist_count = 0;
}
