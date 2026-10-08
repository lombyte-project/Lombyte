#include "types.h"
#include "rnc/ui/menus/menu_screen.h"
extern s32 complete_stream_buffer_transfer() __asm__("FUN_00225cd8");
s32 FUN_00222f58(struct MenuScreen *arg0) {
    arg0->data.raw.unk48 = complete_stream_buffer_transfer(arg0->data.raw.unk48);
    return 0;
}
