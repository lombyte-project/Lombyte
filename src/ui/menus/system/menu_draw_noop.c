/* Ported from rac1-decomp (src/game/pause.c, func_0021EF30). */
int menu_draw_noop(void) __asm__("FUN_0021df28");

int menu_draw_noop(void) {
    return 0;
}

extern __typeof__(menu_draw_noop) func_0021DF28 __attribute__((alias("FUN_0021df28")));
