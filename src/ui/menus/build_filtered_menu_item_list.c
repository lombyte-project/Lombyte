#include "types.h"
struct Slot {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 id;
    s16 f8;
};
struct Info {
    u8 pad[0x38];
    u16 icon;
    u8 pad3a[6];
    s16 a;
    u8 pad42[2];
    s16 b;
    u8 pad46[6];
};
struct Item {
    u16 icon;
    s16 unk2;
    s16 unk4;
    s16 id;
    s16 index;
};
struct Menu {
    u8 pad[0x40];
    s32 count;
};
extern struct Slot D_001CEC20[];
extern struct Slot D_001CEC60[];
extern struct Slot D_001CEC80[];
extern struct Slot D_001CECA0[];
#include "rnc/gameplay/state/item_state.h"
extern struct Info D_001863D0[];
extern struct Item D_001D61B8[];
extern s32 D_001D6248[];
extern s32 D_001D6280[];
extern struct Menu D_001D1088;

s32 build_filtered_menu_item_list(void) __asm__("FUN_002216c0");

s32 build_filtered_menu_item_list(void) {
    s32 filtered_item_count;
    s32 i;
    s32 id;
    struct Info *item_info;

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
            item_info = &D_001863D0[id];
            D_001D61B8[filtered_item_count].icon = item_info->icon;
            D_001D61B8[filtered_item_count].unk2 = 0;
            D_001D61B8[filtered_item_count].unk4 = 0;
            D_001D61B8[filtered_item_count].id = id;
            D_001D6248[filtered_item_count] = item_info->a;
            D_001D6280[filtered_item_count] = item_info->b;
            D_001D61B8[filtered_item_count].index = i;
            filtered_item_count++;
        }
    }
    D_001D1088.count = filtered_item_count;
    return 0;
}

extern __typeof__(build_filtered_menu_item_list) func_002216C0 __attribute__((alias("FUN_002216c0")));
