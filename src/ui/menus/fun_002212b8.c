#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/input/pad_state.h"

extern struct PadState D_0013C940;
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

s32 FUN_002212b8(struct MenuScreen *menu) {
    struct MenuChoice *item;
    s32 *first;
    s32 *opt;
    s32 prev;
    s32 cur;
    s32 n;

    if (menu_system.current->focus != menu) {
        return 0;
    }
    if ((D_0013C940.pressed_unmasked & 0xD00) && menu_system.close_locked == 0) {
        return 1;
    }
    if (D_0013C940.pressed_unmasked & 0x10) {
        if (menu_system.current->back != 0) {
            menu_system.next = menu_system.current->back;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    }
    prev = menu->data.choices.selection;
    if ((D_0013C940.pressed_unmasked & 0x1000) && prev != 0) {
        menu->data.choices.selection = prev - 1;
    }
    if (D_0013C940.pressed_unmasked & 0x4000) {
        cur = menu->data.choices.selection;
        if (menu->data.choices.list[cur + 1].unk0 != 0) {
            menu->data.choices.selection = cur + 1;
        }
    }
    if (prev != menu->data.choices.selection) {
        allocate_voice_for_target_entry(1, 0x11, menu->moby);
    }
    item = &menu->data.choices.list[menu->data.choices.selection];
    first = item->option;
    n = 0;
    if (first[0] != 0) {
        opt = &item->option[0];
        do {
            n++;
            opt++;
            if (*opt == 0) {
                break;
            }
        } while (n < 4);
    }
    if (D_0013C940.pressed_unmasked & 0x40) {
        if (item->value != NULL) {
            *item->value = (*item->value + 1) % n;
            allocate_voice_for_target_entry(0, 0x11, menu->moby);
        }
    }
    return 0;
}
