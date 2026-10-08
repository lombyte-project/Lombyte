#include "types.h"
#include "rnc/ui/menus/menu_screen.h"
extern s32 complete_stream_buffer_transfer() __asm__("func_00225CD8");
s32 FUN_0021fd78(struct MenuScreen *menu) {
    menu->data.raw.unk48 = complete_stream_buffer_transfer(menu->data.raw.unk48);
    menu->data.raw.unk4C = complete_stream_buffer_transfer(menu->data.raw.unk4C);
    menu->data.raw.unk50 = -1;
    menu->data.raw.unk54 = -1;
    menu->data.raw.unk44 = -1;
    return 0;
}
