#include "types.h"
struct MenuItem {
    u8 pad0[0xA];
    s16 state;
};
struct Menu {
    u8 pad0[0x34];
    struct MenuItem items[4];
};
struct Labels {
    u8 pad0[0x14];
    s32 text[4];
};
extern struct Menu D_001D2800;
extern struct Labels D_001D2878;
extern s32 count_nonzero_entries_up_to_30(void) __asm__("func_00215348");
extern s32 count_nonzero_entries_up_to_10(void) __asm__("func_00215300");
void update_skill_point_and_gold_weapon_menu_entries(void) __asm__("FUN_00226e58");

void update_skill_point_and_gold_weapon_menu_entries(void) {
    s32 has_at_least_15_skill_points;
    s32 b;
    s32 has_at_least_10_gold_weapons;

    has_at_least_15_skill_points = count_nonzero_entries_up_to_30() >= 15;
    b = count_nonzero_entries_up_to_30() >= 30;
    has_at_least_10_gold_weapons = count_nonzero_entries_up_to_10() >= 10;
    D_001D2800.items[0].state = has_at_least_15_skill_points ? 3 : 2;
    D_001D2800.items[1].state = b ? 3 : 2;
    D_001D2800.items[2].state = has_at_least_10_gold_weapons ? 10 : 2;
    D_001D2800.items[3].state = has_at_least_10_gold_weapons ? 3 : 2;
    D_001D2878.text[0] = has_at_least_15_skill_points ? 0x4FD3 : 0x4FD9;
    D_001D2878.text[1] = b ? 0x4FD4 : 0x4FDA;
    D_001D2878.text[2] = has_at_least_10_gold_weapons ? 0x4FD7 : 0x4FDB;
    D_001D2878.text[3] = has_at_least_10_gold_weapons ? 0x4FD8 : 0x4FDB;
}

extern __typeof__(update_skill_point_and_gold_weapon_menu_entries) func_00226E58 __attribute__((alias("FUN_00226e58")));
