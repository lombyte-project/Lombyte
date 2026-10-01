/* Ported from rac1-decomp, the PAL decompilation (src/overlays/shared/pause_00277208.c, func_L00_00284620). */
extern void *func_00226720_a(int) __asm__("func_00225490");
extern int FUN_00225c18(int);
extern void func_00225AB8(void *);
extern char D_00186F40[];
/* sets up a pause-menu helper moby in front of the camera */
int FUN_00225588(char *h) {
    char *m;
    int r;
    m = func_00226720_a(0x7A5);
    *(int *)(h + 0x34) = 0;
    if (m != 0) {
        char *d;
        char *g = D_00186F40;
        *(char **)(h + 0x44) = m;
        *(short *)(m + 0x34) = 0;
        *(float *)(m + 0x10) = *(float *)(g + 0x140) + 2.2f;
        *(float *)(m + 0x14) = *(float *)(g + 0x144) + 0.0f;
        *(float *)(m + 0x18) = *(float *)(g + 0x148) + -1.6f;
        d = *(char **)(m + 0x78);
        *(void **)(m + 0x74) = func_00225AB8;
        *(float *)(m + 0x48) = 3.1415927f;
        *(char **)d = h;
        *(int *)(d + 4) = 0;
        *(int *)(d + 8) = 0;
    } else {
        *(int *)(h + 0x34) = 3;
    }
    r = FUN_00225c18(1);
    *(int *)(h + 0x3C) = r;
    if (r == 0) {
        *(int *)(h + 0x34) = 3;
    }
    return 0;
}

extern __typeof__(FUN_00225588) func_00225588 __attribute__((alias("FUN_00225588")));
