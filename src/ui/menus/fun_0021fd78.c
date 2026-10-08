#include "types.h"
#include "rnc/ui/menus/menu_screen.h"
extern s32 complete_stream_buffer_transfer() __asm__("func_00225CD8");
s32 FUN_0021fd78(struct MenuScreen *menu) {
    menu->data.stream.buffer[0] = complete_stream_buffer_transfer(menu->data.stream.buffer[0]);
    menu->data.stream.buffer[1] = complete_stream_buffer_transfer(menu->data.stream.buffer[1]);
    menu->data.stream.loaded_entry[0] = -1;
    menu->data.stream.loaded_entry[1] = -1;
    menu->data.stream.state = -1;
    return 0;
}
