#include "types.h"
#include "rnc/ui/menus/stream_buffers/initialize_menu_stream_buffers.h"

s32 reset_menu_graphics_buffers(void) __asm__("FUN_0021cae0");

s32 reset_menu_graphics_buffers(void) {
    initialize_menu_stream_buffers(1);
    return 0;
}
