#include "types.h"
#include "rnc/ui/menus/menu_screen.h"

extern s32 release_menu_stream_buffer() __asm__("func_00225CD8");
s32        leave_menu_stream_image(struct MenuScreen *menu) __asm__("FUN_0021fd78");

s32 leave_menu_stream_image(struct MenuScreen *menu) {
    menu->data.stream.buffer[0] = release_menu_stream_buffer(menu->data.stream.buffer[0]);
    menu->data.stream.buffer[1] = release_menu_stream_buffer(menu->data.stream.buffer[1]);
    menu->data.stream.loaded_entry[0] = -1;
    menu->data.stream.loaded_entry[1] = -1;
    menu->data.stream.state = -1;
    return 0;
}
