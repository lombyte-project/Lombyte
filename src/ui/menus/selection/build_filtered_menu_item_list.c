#include "types.h"

struct Slot {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 id;
    s16 f8;
};

struct Item {
    u16 icon;
    s16 unk2;
    s16 unk4;
    s16 id;
    s16 index;
};

struct Menu {
    u8  pad[0x40];
    s32 count;
};

extern struct Slot D_001CEC20[];
extern struct Slot D_001CEC60[];
extern struct Slot D_001CEC80[];
extern struct Slot D_001CECA0[];

#include "rnc/game/item_state.h"
#include "rnc/ui/menus/menu_globals.h"

extern struct Item filtered_menu_item_records[] __asm__("D_001D61B8");
extern s32         D_001D6248[];
extern s32         D_001D6280[];
extern struct Menu D_001D1088;

s32 build_filtered_menu_item_list(void) __asm__("FUN_002216c0");

s32 build_filtered_menu_item_list(void) {
    s32           filtered_item_count;
    s32           i;
    s32           id;
    MenuItemInfo *item_info;

    filtered_item_count = 0;
    for (i = 0; i < 14; i++) {
        if (i < 6) {
            id = D_001CEC20[i].id;
        } else if (i < 9) {
            id = D_001CEC60[i - 6].id;
        } else if (i < 12) {
            id = D_001CEC80[i - 9].id;
        } else {
            id = D_001CECA0[i - 12].id;
        }
        if (item_available[id] != 0) {
            item_info = &menu_item_info[id];
            filtered_menu_item_records[filtered_item_count].icon = item_info->unk38;
            filtered_menu_item_records[filtered_item_count].unk2 = 0;
            filtered_menu_item_records[filtered_item_count].unk4 = 0;
            filtered_menu_item_records[filtered_item_count].id = id;
            D_001D6248[filtered_item_count] = item_info->unk40;
            D_001D6280[filtered_item_count] = item_info->unk44;
            filtered_menu_item_records[filtered_item_count].index = i;
            filtered_item_count++;
        }
    }
    D_001D1088.count = filtered_item_count;
    return 0;
}

extern __typeof__(build_filtered_menu_item_list) func_002216C0
    __attribute__((alias("FUN_002216c0")));
