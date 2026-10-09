/* Reset the paired global fields used by the frame state. */

#include "rnc/input/pad_state.h"

void ResetGlobalStateFields(void) __asm__("func_00217020");

void ResetGlobalStateFields(void) {
    controller_state.unk18E = 0;
    controller_state.unk190 = 0;
}
