/* Ported from rac1-decomp, the PAL decompilation (src/game/stream.c, func_002179C8). */
void FUN_00216b28(int arg0, long arg1) {
    short *p = (short *)(int)arg1;
    if (p != 0) {
        *(int *)p = arg0;
        if (arg0 != 0) {
            if (p[5] == 1) {
                p[5] = 8;
            }
        } else {
            p[5] = 0;
        }
    }
}

extern __typeof__(FUN_00216b28) func_00216B28 __attribute__((alias("FUN_00216b28")));
