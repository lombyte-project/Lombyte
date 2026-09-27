/* Ported from rac1-decomp, the PAL decompilation (src/game/menu.c, func_00207CE0). */
/* The two unused float parameters put the threshold's argument in
   $f14 (floats count consecutively from $f12). The nops after the mtc1
   and the compare are ps2eeas's (tools/ps2eeas_nops.py). */
int FUN_002074b0(int arg0, float unused1, float unused2, float arg1) {
    if (arg0 < 0xE0) {
        return (arg1 >= 47.75f) ? 1 : 0;
    }
    return (arg1 < 29.0f) ? 1 : 0;
}

extern __typeof__(FUN_002074b0) func_002074B0 __attribute__((alias("FUN_002074b0")));
