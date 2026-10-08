#include "rnc/gameplay/hero.h"
/* Ported from rac1-decomp (src/game/menu.c, func_00207340). */
/* The struct's address in a local keeps one base register for both
   field reads. */
int FUN_00206b10(int arg0, float unused1, float unused2, float arg1) {
    if (arg0 < 0x100) {
        struct Hero *s = &hero;
        return s->state.control_mode == 17 || s->state.control_mode == 18 || s->base_condition == 1;
    }
    return (arg1 >= 95.0f) ? 1 : 0;
}

extern __typeof__(FUN_00206b10) func_00206B10 __attribute__((alias("FUN_00206b10")));
