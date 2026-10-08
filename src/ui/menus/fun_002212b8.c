#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/input/pad_state.h"

typedef struct MenuItem {
    s32 unk0;
    u8 *value;
    s32 option[4];
} MenuItem;

typedef struct Menu {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x1C];
    MenuItem *items;
    s32 cursor;
} Menu;

extern struct PadState D_0013C940;
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

s32 FUN_002212b8(Menu *menu) {
    MenuItem *item;
    s32 *first;
    s32 *opt;
    s32 prev;
    s32 cur;
    s32 n;

    if (menu_system.current->focus != menu) {
        return 0;
    }
    if ((D_0013C940.pressed & 0xD00) && menu_system.unk124 == 0) {
        return 1;
    }
    if (D_0013C940.pressed & 0x10) {
        if (menu_system.current->back != 0) {
            menu_system.next = menu_system.current->back;
        } else if (menu_system.unk124 == 0) {
            return -1;
        }
    }
    prev = menu->cursor;
    if ((D_0013C940.pressed & 0x1000) && prev != 0) {
        menu->cursor = prev - 1;
    }
    if (D_0013C940.pressed & 0x4000) {
        cur = menu->cursor;
        if (menu->items[cur + 1].unk0 != 0) {
            menu->cursor = cur + 1;
        }
    }
    if (prev != menu->cursor) {
        allocate_voice_for_target_entry(1, 0x11, menu->unk14);
    }
    item = &menu->items[menu->cursor];
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
    if (D_0013C940.pressed & 0x40) {
        if (item->value != NULL) {
            *item->value = (*item->value + 1) % n;
            allocate_voice_for_target_entry(0, 0x11, menu->unk14);
        }
    }
    return 0;
}
