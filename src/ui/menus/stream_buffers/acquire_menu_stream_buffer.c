#include "types.h"
#include "rnc/ui/menus/stream_buffers.h"
extern void fill_transfer_words() __asm__("func_001F97E8");
extern s32 get_menu_stream_buffer_size() __asm__("func_00225D88");

s32 acquire_menu_stream_buffer(s32 require_large_buffer) __asm__("FUN_00225c18");

s32 acquire_menu_stream_buffer(s32 require_large_buffer) {
    s32 i;
    s32 selection_flags;

    for (i = 0; i < MENU_STREAM_BUFFER_COUNT; i++) {
        if (require_large_buffer != 0) {
            selection_flags = menu_stream_buffers[i].flags ^ MENU_STREAM_BUFFER_LARGE;
        } else {
            selection_flags = menu_stream_buffers[i].flags;
        }
        if (!(selection_flags & MENU_STREAM_BUFFER_LARGE)) {
            if (menu_stream_buffers[i].address != 0) {
                if (!(menu_stream_buffers[i].flags & MENU_STREAM_BUFFER_ALLOCATED)) {
                    menu_stream_buffers[i].flags |= MENU_STREAM_BUFFER_ALLOCATED;
                    fill_transfer_words(menu_stream_buffers[i].address, 0xDEADBEEF,
                                      get_menu_stream_buffer_size(menu_stream_buffers[i].address));
                    return menu_stream_buffers[i].address;
                }
            }
        }
    }
    return 0;
}

extern __typeof__(acquire_menu_stream_buffer) func_00225C18 __attribute__((alias("FUN_00225c18")));
