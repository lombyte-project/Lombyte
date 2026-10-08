#include "types.h"
#include "rnc/ui/menus/menu_screen.h"
extern s32 initialize_graphics_buffer_descriptors() __asm__("FUN_00225ac0");
extern s32 select_next_stream_buffer() __asm__("FUN_00225c18");
s32 FUN_00222f18(struct MenuScreen *arg0) {
    initialize_graphics_buffer_descriptors(1);
    arg0->data.save.save_data = select_next_stream_buffer(0);
    arg0->data.save.step = 0;
    return 0;
}
