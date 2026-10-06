#include "types.h"

extern s32 draw_moby_list() __asm__("func_0020D330");
s32 draw_moby_entries_from_object(s32 object) __asm__("FUN_00225a68");

s32 draw_moby_entries_from_object(s32 object) {
    s32 *slot;
    s32 moby_list;
    s32 remaining;
    remaining = 0x17;
    slot = (s32 *)(object + 0x44);
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
