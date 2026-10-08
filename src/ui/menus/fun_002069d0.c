#include "rnc/gameplay/hero.h"
/* Ported from rac1-decomp (src/game/menu.c, func_00207200). */
/* Menu hit test: arg3 is the third float ($f14). b (state 16) is
   computed before a, as retail evaluates it; the second if's own
   `arg1 >= 0xC8` is retail's second test of $a1. */
int FUN_002069d0(void *arg0, int arg1, float unused1, float unused2, float arg3) {
    struct Hero *s = &hero;
    int b = s->state.control_mode == 16;
    int a = s->state.control_mode == 17 || s->state.control_mode == 18 || s->base_condition == 1;

    if (arg1 < 0xC8 && arg3 >= 39.5f && arg3 <= 42.5f && !a) {
        return 1;
    }
    if (arg1 >= 0xC8 && arg3 >= 87.0f && !b) {
        return 1;
    }
    return 0;
}

extern __typeof__(FUN_002069d0) func_002069D0 __attribute__((alias("FUN_002069d0")));
