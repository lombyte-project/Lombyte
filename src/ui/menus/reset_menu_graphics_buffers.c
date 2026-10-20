#include "types.h"
extern s32 initialize_menu_stream_buffers() __asm__("FUN_00225ac0");
s32 reset_menu_graphics_buffers(void) __asm__("FUN_0021cae0");

s32 reset_menu_graphics_buffers(void) {
    initialize_menu_stream_buffers(1);
    return 0;
}
