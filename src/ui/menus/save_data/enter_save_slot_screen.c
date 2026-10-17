#include "types.h"
#include "rnc/ui/menus/menu_screen.h"
extern s32 initialize_graphics_buffer_descriptors() __asm__("FUN_00225ac0");
extern s32 select_next_stream_buffer() __asm__("FUN_00225c18");
s32 enter_save_slot_screen(struct MenuScreen *arg0) __asm__("FUN_00222f18");

s32 enter_save_slot_screen(struct MenuScreen *menu_screen) {
    initialize_graphics_buffer_descriptors(1);
    menu_screen->data.save.save_data = select_next_stream_buffer(0);
    menu_screen->data.save.step = 0;
    return 0;
}
