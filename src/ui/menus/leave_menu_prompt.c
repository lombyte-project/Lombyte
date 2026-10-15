#include "types.h"
#include "rnc/ui/menus/menu_screen.h"
extern s32 complete_stream_buffer_transfer() __asm__("FUN_00225cd8");
s32 leave_menu_prompt(struct MenuScreen *arg0) __asm__("FUN_00221a88");

s32 leave_menu_prompt(struct MenuScreen *arg0) {
    arg0->data.prompt.buffer = complete_stream_buffer_transfer(arg0->data.prompt.buffer);
    return 0;
}
