/* Clear the core global word. */

#include "rnc/runtime/core_state.h"

void ClearCoreGlobal(void) __asm__("func_00118BC0");

void ClearCoreGlobal(void) {
    CoreGlobalWord = 0;
}
