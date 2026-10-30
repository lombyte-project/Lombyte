#include "types.h"

struct MenuItem {
    u8  pad0[0xA];
    s16 state;
};

struct Menu {
    u8              pad0[0x34];
    struct MenuItem items[4];
};

struct Labels {
    u8  pad0[0x14];
    s32 text[4];
};

extern struct Menu   skill_point_and_gold_weapon_menu __asm__("D_001D2800");
extern struct Labels skill_point_and_gold_weapon_menu_labels __asm__("D_001D2878");
extern s32           count_skill_points(void) __asm__("func_00215348");
extern s32           count_gold_weapons(void) __asm__("func_00215300");
void                 update_skill_point_and_gold_weapon_menu_entries(void) __asm__("FUN_00226e58");

void update_skill_point_and_gold_weapon_menu_entries(void) {
    s32 has_at_least_15_skill_points;
    s32 has_at_least_30_skill_points;
    s32 has_at_least_10_gold_weapons;

    has_at_least_15_skill_points = count_skill_points() >= 15;
    has_at_least_30_skill_points = count_skill_points() >= 30;
    has_at_least_10_gold_weapons = count_gold_weapons() >= 10;
    skill_point_and_gold_weapon_menu.items[0].state = has_at_least_15_skill_points ? 3 : 2;
    skill_point_and_gold_weapon_menu.items[1].state = has_at_least_30_skill_points ? 3 : 2;
    skill_point_and_gold_weapon_menu.items[2].state = has_at_least_10_gold_weapons ? 10 : 2;
    skill_point_and_gold_weapon_menu.items[3].state = has_at_least_10_gold_weapons ? 3 : 2;
    skill_point_and_gold_weapon_menu_labels.text[0] =
        has_at_least_15_skill_points ? 0x4FD3 : 0x4FD9;
    skill_point_and_gold_weapon_menu_labels.text[1] =
        has_at_least_30_skill_points ? 0x4FD4 : 0x4FDA;
    skill_point_and_gold_weapon_menu_labels.text[2] =
        has_at_least_10_gold_weapons ? 0x4FD7 : 0x4FDB;
    skill_point_and_gold_weapon_menu_labels.text[3] =
        has_at_least_10_gold_weapons ? 0x4FD8 : 0x4FDB;
}

extern __typeof__(update_skill_point_and_gold_weapon_menu_entries) func_00226E58
    __attribute__((alias("FUN_00226e58")));
