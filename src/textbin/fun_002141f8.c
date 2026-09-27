/* Ported from rac1-decomp, the PAL decompilation (src/game/mobyutil.c, func_00215048). */
/* A goto into the first `return 0` gives retail's backward beqz; the two
   nops before it are short-loop padding (tools/ps2eeas_nops.py). */
int FUN_002141f8(char *arg0) {
    if (arg0 == 0) {
    ret0:
        return 0;
    }
    if ((*(unsigned short *)(arg0 + 0x34) & 0x20) == 0) {
        goto ret0;
    }
    return **(int **)(arg0 + 0x78);
}

extern __typeof__(FUN_002141f8) func_002141F8 __attribute__((alias("FUN_002141f8")));
