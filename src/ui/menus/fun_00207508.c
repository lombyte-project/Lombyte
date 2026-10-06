/* Ported from rac1-decomp (src/game/menu.c, func_00207D38). */
/* `(a && b) ? 1 : 0` gives retail's bc1f then bc1tl; the other arm is
   a plain `? 1 : 0`. */
int FUN_00207508(int arg0, float unused1, float unused2, float arg1) {
    if (arg0 < 0xE0) {
        return (arg1 >= 44.0f && arg1 <= 45.0f) ? 1 : 0;
    }
    return (arg1 >= 37.0f) ? 1 : 0;
}

extern __typeof__(FUN_00207508) func_00207508 __attribute__((alias("FUN_00207508")));
