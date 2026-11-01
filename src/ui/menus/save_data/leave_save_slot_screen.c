#include "types.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/ui/menus/stream_buffers/release_menu_stream_buffer.h"

s32 leave_save_slot_screen(struct MenuScreen *screen) __asm__("FUN_00222f58");

s32 leave_save_slot_screen(struct MenuScreen *screen) {
    screen->data.save.save_data = release_menu_stream_buffer(screen->data.save.save_data);
    return 0;
}
