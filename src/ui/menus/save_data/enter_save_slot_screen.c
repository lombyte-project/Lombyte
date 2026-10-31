#include "types.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/ui/menus/stream_buffers/acquire_menu_stream_buffer.h"
#include "rnc/ui/menus/stream_buffers/initialize_menu_stream_buffers.h"

s32 enter_save_slot_screen(struct MenuScreen *menu_screen) __asm__("FUN_00222f18");

s32 enter_save_slot_screen(struct MenuScreen *menu_screen) {
    initialize_menu_stream_buffers(1);
    menu_screen->data.save.save_data = acquire_menu_stream_buffer(0);
    menu_screen->data.save.step = 0;
    return 0;
}
