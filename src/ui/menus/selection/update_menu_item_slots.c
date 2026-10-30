#include "types.h"
#include "rnc/globals.h"
#include "rnc/input/pad_state.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/game/usage_stats.h"

#include "rnc/game/item_state.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"

extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

s32 update_menu_item_slots(struct MenuScreen *menu) __asm__("FUN_0021c7a0");

s32 update_menu_item_slots(struct MenuScreen *menu) {
    s16                id;
    s32               *slots;
    s32               *scan;
    s32                slot_cursor_after_input;
    s32                cursor_or_scaled_frame_quotient;
    int                index;
    int                start_index;
    int                i;
    struct MenuScreen *grid;

    grid = menu_system.current->focus;
    index = grid->data.grid.selected_cell;
    id = grid->data.grid.cells[index].id;
    start_index = menu->data.slots.cursor;
    if (controller_state.pressed_unmasked & 8) {
        menu->data.slots.cursor = (start_index + 1) % 8;
    }
    if (controller_state.pressed_unmasked & 4) {
        cursor_or_scaled_frame_quotient = menu->data.slots.cursor;
        menu->data.slots.cursor = (cursor_or_scaled_frame_quotient + 7) % 8;
    }
    slot_cursor_after_input = menu->data.slots.cursor;
    if (slot_cursor_after_input != start_index) {
        allocate_voice_for_target_entry(1, 0x11, menu->moby);
    }
    if (id != 0) {
        if (item_available[id] != 0 && (controller_state.pressed_unmasked & PAD_CROSS)) {
            if ((u16)usage_stats.stat[21].count <= 0xFFFEU) {
                usage_stats.stat[21].count = (u16)(usage_stats.stat[21].count + 1);
            }
            cursor_or_scaled_frame_quotient = scale_game_frames(D_0015EEA4) / 600;
            if ((s32)usage_stats.stat[21].unk2 < cursor_or_scaled_frame_quotient) {
                usage_stats.stat[21].unk2 = (u16)(scale_game_frames(D_0015EEA4) / 600);
            }
            slots = menu->data.slots.items;
            usage_stats.stat[21].level_mask =
                (s32)((usage_stats.stat[21].level_mask | (1 << current_level_index)) | 0x80000000);
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

extern __typeof__(update_menu_item_slots) func_0021C7A0 __attribute__((alias("FUN_0021c7a0")));

struct UsageStats usage_stats = {0};
