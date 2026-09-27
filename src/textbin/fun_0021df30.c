/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_0021EF38). */
int FUN_0021df30(void *arg0) {
    char *p = (char *)arg0;
    *(float *)(p + 0x38) = 3.14159274f;
    *(int *)(p + 0x34) = 0;
    *(int *)(p + 0x44) = 0;
    *(int *)(p + 0x48) = 0;
    return 0;
}

extern __typeof__(FUN_0021df30) func_0021DF30 __attribute__((alias("FUN_0021df30")));
