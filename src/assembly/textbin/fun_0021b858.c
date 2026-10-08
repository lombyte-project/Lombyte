#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021b858/FUN_0021b858.s", FUN_0021b858);
#else
#include "types.h"
#include "rnc/ui/menus/menu_system.h"

typedef struct MenuGrid MenuGrid;

typedef struct {
    u8 pad0[6];
    s16 item;
    u8 pad8[2];
} MenuGridEntry;

struct MenuGrid {
    u8 pad0[0x14];
    s32 sound_owner;
    u8 pad18[0x18];
    s32 flags;
    u8 pad34[0x8];
    s32 cursor;
    s32 rows;
    s32 columns;
    MenuGridEntry *entries;
    MenuGrid *up;
    MenuGrid *down;
    MenuGrid *left;
    MenuGrid *right;
};

typedef struct {
    u8 pad0[0x38];
    s32 back_page;
    u8 pad3C[0x4];
    MenuGrid *focus;
    u8 pad44[0x3C];
    MenuGrid *next;
} MenuPage;

typedef struct {
    u8 pad0[0x1C4];
    s32 pressed_buttons;
} MenuControllerState;

typedef struct {
    u8 pad0[0x8];
    s32 slot;
    u8 padC[0x40];
} MenuItemInfo;

typedef struct {
    u8 pad0[0x1FF5];
    u8 gadget_enabled;
    u8 pad1FF6;
    u8 gadget_count;
} MenuPlayerState;

extern MenuControllerState controller_state __asm__("D_0013C940");
extern u8 item_available[] __asm__("D_0013D4C0");
extern MenuPlayerState player_state __asm__("D_0013F350");
extern MenuItemInfo menu_item_info[] __asm__("D_001863D0");
extern s32 func_001E9468();
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

s32 update_menu_grid_selection(MenuGrid *grid) __asm__("FUN_0021b858");

s32 update_menu_grid_selection(MenuGrid *grid) {
    s32 column_count;
    s32 row_count;
    s32 cursor;
    s32 row;
    s32 column_index;
    s32 column;
    s32 hidden;
    s32 next_rows;
    s32 back_page;
    s32 total;
    s32 next_column_count;
    s32 next_columns;
    MenuGrid *next;
    MenuGridEntry *entry;
    s32 slot;

    if (((MenuPage *)menu_system.current)->focus != grid) {
        return 0;
    }
    if ((controller_state.pressed_buttons & 0xD00) && menu_system.close_blocked == 0) {
        return 1;
    }
    if (controller_state.pressed_buttons & 0x10) {
        back_page = ((MenuPage *)menu_system.current)->back_page;
        if (back_page != 0) {
            menu_system.next = (struct MenuPage *)back_page;
        } else if (menu_system.close_blocked == 0) {
            return -1;
        }
    }

    column_count = grid->columns;
    cursor = grid->cursor;
    row_count = grid->rows;
    row = cursor / column_count;
    column_index = cursor % column_count;
    column = column_index;

    /* Process every pressed direction in order; a page change can also adjust
       column before the following direction checks. Retail's UP-clear branch
       reaches a redundant controller address calculation before DOWN. */
    if (controller_state.pressed_buttons & 0x1000) {
        if (row != 0) {
            grid->cursor -= column_count;
        } else if (grid->up != NULL) {
            next = grid;
            do {
                next = next->up;
                next_rows = next->rows;
                next_columns = next->columns;
                hidden = 0;
                if (menu_system.unk134 != 0 && (next->flags & 8)) {
                    hidden = 1;
                }
                if (menu_system.unk138 != 0 && (next->flags & 4)) {
                    hidden = 1;
                }
            } while (hidden);
            ((MenuPage *)menu_system.current)->next = next;
            if (next_columns == 5 && grid->columns == 3) {
                column++;
            }
            if (next_columns == 3 && grid->columns == 5) {
                column--;
                if (column > 2) {
                    column = 2;
                } else if (column < 0) {
                    column = 0;
                }
            }
            next->cursor = (next_rows - 1) * next_columns +
                           (column < next_columns - 1 ? column : next_columns - 1);
        } else if (!(grid->flags & 0x8000)) {
            grid->cursor = column_count * (row_count - 1) + cursor;
        }
    }

    if (controller_state.pressed_buttons & 0x4000) {
        if (row + 1 < row_count) {
            grid->cursor = grid->cursor + column_count;
        } else if (grid->down != NULL) {
            next = grid;
            do {
                next = next->down;
                next_column_count = next->columns;
                hidden = 0;
                if (menu_system.unk134 != 0 && (next->flags & 8)) {
                    hidden = 1;
                }
                if (menu_system.unk138 != 0 && (next->flags & 4)) {
                    hidden = 1;
                }
            } while (hidden);
            ((MenuPage *)menu_system.current)->next = next;
            if (next_column_count == 5 && grid->columns == 3) {
                column++;
            }
            if (next_column_count == 3 && grid->columns == 5) {
                column--;
                if (column > 2) {
                    column = 2;
                } else if (column < 0) {
                    column = 0;
                }
            }
            next->cursor = column < next_column_count - 1 ? column : next_column_count - 1;
        } else if (!(grid->flags & 0x8000)) {
            grid->cursor = grid->cursor - grid->columns * (grid->rows - 1);
        }
    }

    if (controller_state.pressed_buttons & 0x8000) {
        if (column != 0) {
            grid->cursor = grid->cursor - 1;
        } else if (grid->left != NULL) {
            ((MenuPage *)menu_system.current)->next = grid->left;
        } else if (!(grid->flags & 0x8000)) {
            grid->cursor += grid->columns - 1;
        }
    }

    if (controller_state.pressed_buttons & 0x2000) {
        if (column + 1 < column_count) {
            grid->cursor = grid->cursor + 1;
        } else if (grid->right != NULL) {
            ((MenuPage *)menu_system.current)->next = grid->right;
        } else if (!(grid->flags & 0x8000)) {
            grid->cursor -= grid->columns - 1;
        }
    }

    if (grid->cursor != cursor || ((MenuPage *)menu_system.current)->next != NULL) {
        allocate_voice_for_target_entry(1, 0x11, grid->sound_owner);
    }

    if ((controller_state.pressed_buttons & 0x40) && ((grid->flags ^ 1) & 1)) {
        entry = &grid->entries[grid->cursor];
        if (entry->item != 0 && item_available[entry->item] != 0) {
            slot = menu_item_info[entry->item].slot;
            allocate_voice_for_target_entry(0, 0x11, grid->sound_owner);
            if (menu_system.equipped[slot] == entry->item && slot != 0 && slot != 3) {
                menu_system.equipped[slot] = 0;
            } else if (entry->item == 0x18) {
                if (func_001E9468(0x18) != 0 && menu_system.state == 3) {
                    next_columns = func_001E9468(0x18);
                    player_state.gadget_enabled = 1;
                    total = player_state.gadget_count + next_columns;
                    if (total > 6) {
                        total = 6;
                    }
                    player_state.gadget_count = total;
                }
            } else {
                menu_system.equipped[slot] = entry->item;
            }
        } else {
            allocate_voice_for_target_entry(2, 0x11, grid->sound_owner);
        }
    }
    return 0;
}

extern __typeof__(update_menu_grid_selection) func_0021B858 __attribute__((alias("FUN_0021b858")));

#endif /* NON_MATCHING */
