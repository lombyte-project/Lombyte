#include "types.h"
#include "rnc/ui/menus/stream_buffers.h"

s32 clear_menu_stream_buffer_read_active(s32 buffer_address) __asm__("FUN_00225e20");

s32 clear_menu_stream_buffer_read_active(s32 buffer_address) {
    s32 i;

    for (i = 0; i < MENU_STREAM_BUFFER_COUNT; i++) {
        if (menu_stream_buffers[i].address == buffer_address) {
            menu_stream_buffers[i].flags &= ~MENU_STREAM_BUFFER_READ_ACTIVE;
            return 0;
        }
    }
    return 1;
}
