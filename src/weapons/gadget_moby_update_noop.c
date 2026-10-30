/* Ported from rac1-decomp (src/game/pause.c, func_00225DF0). */
void gadget_moby_update_noop(void) __asm__("FUN_00224b60");

void gadget_moby_update_noop(void) {
}

extern __typeof__(gadget_moby_update_noop) func_00224B60 __attribute__((alias("FUN_00224b60")));
