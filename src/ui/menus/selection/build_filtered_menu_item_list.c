#include "types.h"

struct Item {
    u16 icon;
    s16 unk2;
    s16 unk4;
    s16 id;
    s16 index;
};

#include "rnc/game/item_state.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/ui/menus/menu_pages.h"

extern struct Item filtered_menu_item_records[] __asm__("D_001D61B8");

s32 build_filtered_menu_item_list(void) __asm__("FUN_002216c0");

s32 build_filtered_menu_item_list(void) {
    s32           filtered_item_count;
    s32           i;
    s32           id;
    MenuItemInfo *item_info;

    filtered_item_count = 0;
    for (i = 0; i < 14; i++) {
        if (i < 6) {
            id = pause_hand_gadget_cells[i].id;
        } else if (i < 9) {
            id = pause_pack_gadget_cells[i - 6].id;
        } else if (i < 12) {
            id = pause_head_gadget_cells[i - 9].id;
        } else {
            id = pause_foot_gadget_cells[i - 12].id;
        }
        if (item_available[id] != 0) {
            item_info = &menu_item_info[id];
            filtered_menu_item_records[filtered_item_count].icon = item_info->unk38;
            filtered_menu_item_records[filtered_item_count].unk2 = 0;
            filtered_menu_item_records[filtered_item_count].unk4 = 0;
            filtered_menu_item_records[filtered_item_count].id = id;
            gadget_help_name_text_ids[filtered_item_count] = item_info->unk40;
            gadget_help_text_ids[filtered_item_count] = item_info->unk44;
            filtered_menu_item_records[filtered_item_count].index = i;
            filtered_item_count++;
        }
    }
    gadget_help_icon_list.icons.count = filtered_item_count;
    return 0;
}

extern __typeof__(build_filtered_menu_item_list) func_002216C0
    __attribute__((alias("FUN_002216c0")));

/* Written by build_filtered_menu_item_list: at most one entry per candidate (14). */
struct Item filtered_menu_item_records[14] = {0};

/* Text ids of each entry's item (menu_item_info unk40 / unk44), the tables of
   the Help / Gadgets name and text labels. */
s32 gadget_help_name_text_ids[14] = {0};

s32 gadget_help_text_ids[14] = {0};
