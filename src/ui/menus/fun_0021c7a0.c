#include "types.h"
#include "rnc/globals.h"
#include "rnc/input/pad_state.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"

struct Globals_00141848 {
    u8 pad_0[0xA8];
    u16 unkA8;
    u16 unkAA;
    s32 unkAC;
};
extern struct PadState D_0013C940;
extern u8 D_0013D4C0[];
extern struct Globals_00141848 D_00141848;
extern s32 D_0015EEA4;
extern s32 scale_game_frames() __asm__("FUN_001f96f8");
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

s32 FUN_0021c7a0(struct MenuScreen *menu) {
    s16 id;
    s32 *slots;
    s32 *scan;
    s32 cur;
    s32 tmp;
    int index;
    int start_index;
    int i;
    struct MenuScreen *grid;

    grid = menu_system.current->focus;
    index = grid->data.grid.selected_cell;
    id = grid->data.grid.cells[index].id;
    start_index = menu->data.slots.cursor;
    if (D_0013C940.pressed & 8) {
        menu->data.slots.cursor = (start_index + 1) % 8;
    }
    if (D_0013C940.pressed & 4) {
        tmp = menu->data.slots.cursor;
        menu->data.slots.cursor = (tmp + 7) % 8;
    }
    cur = menu->data.slots.cursor;
    if (cur != start_index) {
        allocate_voice_for_target_entry(1, 0x11, menu->moby);
    }
    if (id != 0) {
        if (D_0013D4C0[id] != 0 && (D_0013C940.pressed & 0x40)) {
            if ((u16)D_00141848.unkA8 <= 0xFFFEU) {
                D_00141848.unkA8 = (u16)(D_00141848.unkA8 + 1);
            }
            tmp = scale_game_frames(D_0015EEA4) / 600;
            if ((s32)D_00141848.unkAA < tmp) {
                D_00141848.unkAA = (u16)(scale_game_frames(D_0015EEA4) / 600);
            }
            slots = menu->data.slots.items;
            D_00141848.unkAC = (s32)((D_00141848.unkAC | (1 << current_level_index)) | 0x80000000);
            i = 0;
            if (menu->data.slots.items[0] != id) {
                scan = slots;
            loop_17:
                i += 1;
                scan += 1;
                if (i < 8) {
                    if ((*scan) == id) {
                        goto block_19;
                    }
                    goto loop_17;
                }
            } else {
            block_19:
                if (i < 8) {
                    menu->data.slots.items[i] = 0;
                }
            }
            menu->data.slots.items[menu->data.slots.cursor] = (s32)id;
            menu->data.slots.cursor = (menu->data.slots.cursor + 1) % 8;
        }
    }
    return 0;
}

extern __typeof__(FUN_0021c7a0) func_0021C7A0 __attribute__((alias("FUN_0021c7a0")));
