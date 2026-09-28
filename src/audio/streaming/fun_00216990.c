/* Ported from rac1-decomp, the PAL decompilation (src/game/stream.c, func_00217830). */
void FUN_00216990(int arg0, long arg1) {
    short *p = (short *)(int)arg1;
    if (p != 0 && arg0 != 0 && p[5] == 2) {
        p[5] = 3;
    }
}

extern __typeof__(FUN_00216990) func_00216990 __attribute__((alias("FUN_00216990")));
