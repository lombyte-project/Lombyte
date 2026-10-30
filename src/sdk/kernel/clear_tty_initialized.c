#include "rnc/runtime/core_state.h"

void clear_tty_initialized(void) __asm__("func_00118BC0");

void clear_tty_initialized(void) {
    tty_initialized = 0;
}
