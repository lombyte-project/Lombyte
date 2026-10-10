/* Clear the core global word. */

#include "rnc/runtime/core_state.h"

void clear_core_global(void) __asm__("func_00118BC0");

void clear_core_global(void) {
    CoreGlobalWord = 0;
}
