/* Ported from rac1-decomp (src/game/pause.c, func_00226D48). */
void helper_moby_update_noop(void) __asm__("FUN_00225ab8");
void helper_moby_update_noop(void) {
}

extern __typeof__(helper_moby_update_noop) func_00225AB8 __attribute__((alias("FUN_00225ab8")));
