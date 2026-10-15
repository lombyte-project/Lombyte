#include "types.h"
#include "rnc/ui/menus/menu_screen.h"
extern s32 complete_stream_buffer_transfer() __asm__("FUN_00225cd8");
s32 leave_save_slot_screen(struct MenuScreen *arg0) __asm__("FUN_00222f58");

s32 leave_save_slot_screen(struct MenuScreen *arg0) {
    arg0->data.save.save_data = complete_stream_buffer_transfer(arg0->data.save.save_data);
    return 0;
}
