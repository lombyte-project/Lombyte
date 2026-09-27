/* Ported from rac1-decomp, the PAL decompilation (src/game/stream.c, func_00217A60). */
extern short D_001516D0[];
void FUN_00216bc0(int arg0, long arg1) {
    char *p = (char *)(int)arg1;
    short *b;
    if (p == 0) {
        return;
    }
    *(int *)(p + 0x18) = arg0;
    if (*(short *)(p + 0x10) == 0) {
        return;
    }
    b = D_001516D0;
    if (b[0x10] != 1) {
        return;
    }
    if (arg0 == 0) {
        return;
    }
    b[0x10] = 2;
    *(int *)((char *)b + 0x24) = *(int *)(p + 0x18);
    *(int *)((char *)b + 0x28) = *(int *)(p + 0x18) / 4;
}

extern __typeof__(FUN_00216bc0) func_00216BC0 __attribute__((alias("FUN_00216bc0")));
