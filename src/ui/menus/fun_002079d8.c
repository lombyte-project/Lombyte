/* Ported from rac1-decomp (src/game/menu.c, func_00208208). */
/* arg0 is unused; the two unused float parameters put arg3 in $f14
   (floats count consecutively from $f12, one register each; see
   func_00207CE0). With `r` defaulting to 1, reorg turns the reset into
   retail's bc1fl with `r = 0` in its delay slot. The nop after the mtc1
   is ps2eeas's (tools/ps2eeas_nops.py). */
int FUN_002079d8(void *arg0, int arg1, float unused1, float unused2, float arg3) {
    int r = 1;

    if (arg1 < 0x141 && !(63.5f <= arg3)) {
        r = 0;
    }
    return r;
}

extern __typeof__(FUN_002079d8) func_002079D8 __attribute__((alias("FUN_002079d8")));
