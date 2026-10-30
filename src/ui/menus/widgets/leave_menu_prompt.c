#include "types.h"
#include "rnc/ui/menus/menu_screen.h"

extern s32 release_menu_stream_buffer() __asm__("FUN_00225cd8");
s32        leave_menu_prompt(struct MenuScreen *menu_screen) __asm__("FUN_00221a88");

s32 leave_menu_prompt(struct MenuScreen *menu_screen) {
    menu_screen->data.prompt.buffer = release_menu_stream_buffer(menu_screen->data.prompt.buffer);
    return 0;
}
