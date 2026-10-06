#include "types.h"
#include "rnc/ui/menus/fun_0021fce0.h"

extern s32 select_next_stream_buffer() __asm__("FUN_00225c18");
s32 FUN_0021fce0(struct MenuScreen *menu) {
    menu->unk44 = 0;
    menu->unk48 = select_next_stream_buffer(menu->unk34 & 0x200);
    menu->unk4C = select_next_stream_buffer(menu->unk34 & 0x200);
    if (!(menu->unk34 & 0x200)) {
        if (menu->unk48 == 0) {
            menu->unk48 = select_next_stream_buffer(1);
        }
        if (menu->unk4C == 0) {
            menu->unk4C = select_next_stream_buffer(1);
        }
    }
    menu->unk5C = 0;
    menu->unk50 = -1;
    menu->unk54 = -1;
    return 0;
}

extern s32 func_0021FCE0(struct MenuScreen *menu) __attribute__((alias("FUN_0021fce0")));
