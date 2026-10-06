/* Ported from rac1-decomp (src/game/menu.c, func_00207E28). */
/* Returning the compare as `? 1 : 0` gives bc1t with the `li 1` in its
   slot; folded into `&&` it becomes bc1tl. The two nops, after the mtc1
   and after the compare, are ps2eeas's (tools/ps2eeas_nops.py). */
int FUN_002075f8(int arg0, float unused1, float unused2, float arg1) {
    if (arg0 >= 0xE0) {
        return 0;
    }
    return (arg1 <= 38.0f) ? 1 : 0;
}

extern __typeof__(FUN_002075f8) func_002075F8 __attribute__((alias("FUN_002075f8")));
