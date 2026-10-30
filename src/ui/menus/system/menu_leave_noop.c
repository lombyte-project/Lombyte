/* Ported from rac1-decomp (src/game/pause.c, rotate_item_preview_moby). */
int menu_leave_noop(void) __asm__("FUN_0021d1f0");

int menu_leave_noop(void) {
    return 0;
}

extern __typeof__(menu_leave_noop) func_0021D1F0 __attribute__((alias("FUN_0021d1f0")));
