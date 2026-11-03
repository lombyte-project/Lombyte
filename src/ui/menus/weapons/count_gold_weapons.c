#include "types.h"
#include "rnc/game/item_state.h"

s32 count_gold_weapons(void) __asm__("FUN_00215300");

s32 count_gold_weapons(void) {
    s32 purchased_gold_weapon_count = 0;
    s32 i;

    for (i = 0; i < 0x25; i++) {
        if (item_text_variant[i] != 0) {
            purchased_gold_weapon_count++;
        }
    }
    if (purchased_gold_weapon_count < 0) {
        purchased_gold_weapon_count = 0;
    }
    return (purchased_gold_weapon_count < 0xB) ? purchased_gold_weapon_count : 0xA;
}

extern __typeof__(count_gold_weapons) func_00215300 __attribute__((alias("FUN_00215300")));
