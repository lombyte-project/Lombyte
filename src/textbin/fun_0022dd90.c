/* Ported from rac1-decomp, the PAL decompilation (src/game/sound.c, func_0022F0A8). */
void FUN_0022dd90(int arg0, long arg1) {
    unsigned char *p = (unsigned char *)(int)arg1;
    if (p != 0) {
        *(int *)p = arg0;
        if (arg0 != 0) {
            if (p[4] == 1) {
                p[4] = 2;
            }
        } else {
            *(int *)(p + 0x18) = 0;
            *(int *)(p + 0x1C) = 0;
            p[4] = 0;
        }
    }
}

extern __typeof__(FUN_0022dd90) func_0022DD90 __attribute__((alias("FUN_0022dd90")));
