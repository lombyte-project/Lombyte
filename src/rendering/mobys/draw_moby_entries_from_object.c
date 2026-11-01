#include "types.h"
#include "rnc/ui/menus/menu_screen.h"

extern s32 draw_moby_list() __asm__("func_0020D330");
s32 draw_moby_entries_from_object(struct MenuStreamedMobyScreen *screen) __asm__("FUN_00225a68");

s32 draw_moby_entries_from_object(struct MenuStreamedMobyScreen *screen) {
    s32 *slot;
    s32  moby_list;
    s32  remaining;
    remaining = 0x17;
    slot = (s32 *)screen->mobys;
    do {
        moby_list = *slot;
        if (moby_list != 0) {
            draw_moby_list(moby_list, 1);
        }
        remaining -= 1;
        slot += 1;
    } while (remaining >= 0);
    return 4;
}
