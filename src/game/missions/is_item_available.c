/* Ported from rac1-decomp (src/game/missionfunc.c, func_0020CCB8). */
#include "rnc/game/item_state.h"

int is_item_available(int item_index) __asm__("FUN_0020be68");

int is_item_available(int item_index) {
    return item_available[item_index] != 0;
}

extern __typeof__(is_item_available) func_0020BE68 __attribute__((alias("FUN_0020be68")));
