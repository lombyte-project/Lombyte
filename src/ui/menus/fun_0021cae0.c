#include "types.h"
extern s32 initialize_graphics_buffer_descriptors() __asm__("FUN_00225ac0");
s32 FUN_0021cae0(void) {
    initialize_graphics_buffer_descriptors(1);
    return 0;
}
