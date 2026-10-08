#include "types.h"
#include "rnc/ui/menus/menu_screen.h"
extern s32 complete_stream_buffer_transfer() __asm__("FUN_00225cd8");
s32 FUN_00221a88(struct MenuScreen *arg0) {
    arg0->data.raw.unk54 = complete_stream_buffer_transfer(arg0->data.raw.unk54);
    return 0;
}
