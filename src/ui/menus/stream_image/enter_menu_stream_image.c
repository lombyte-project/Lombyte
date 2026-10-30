#include "types.h"
#include "rnc/ui/menus/menu_screen.h"

extern s32 acquire_menu_stream_buffer() __asm__("FUN_00225c18");
s32        enter_menu_stream_image(struct MenuScreen *menu) __asm__("FUN_0021fce0");

s32 enter_menu_stream_image(struct MenuScreen *menu) {
    menu->data.stream.state = 0;
    menu->data.stream.buffer[0] = acquire_menu_stream_buffer(menu->data.stream.flags & 0x200);
    menu->data.stream.buffer[1] = acquire_menu_stream_buffer(menu->data.stream.flags & 0x200);
    if (!(menu->data.stream.flags & 0x200)) {
        if (menu->data.stream.buffer[0] == 0) {
            menu->data.stream.buffer[0] = acquire_menu_stream_buffer(1);
        }
        if (menu->data.stream.buffer[1] == 0) {
            menu->data.stream.buffer[1] = acquire_menu_stream_buffer(1);
        }
    }
    menu->data.stream.elapsed_frames = 0;
    menu->data.stream.loaded_entry[0] = -1;
    menu->data.stream.loaded_entry[1] = -1;
    return 0;
}

extern s32 func_0021FCE0(struct MenuScreen *menu) __attribute__((alias("FUN_0021fce0")));
