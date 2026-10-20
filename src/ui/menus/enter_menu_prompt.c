#include "types.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/ui/menus/menu_system.h"
extern s32 acquire_menu_stream_buffer() __asm__("func_00225C18");
s32 enter_menu_prompt(struct MenuScreen *arg0) __asm__("FUN_00221a48");

s32 enter_menu_prompt(struct MenuScreen *menu_screen) {
    menu_system.current->confirmed = 0;
    menu_screen->data.prompt.buffer = acquire_menu_stream_buffer(0);
    return 0;
}
