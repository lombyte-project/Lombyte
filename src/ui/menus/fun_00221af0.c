/* Ported from rac1-decomp, the PAL decompilation (src/overlays/shared/pause_00277208.c, func_L00_00280740). */
#define NOT_SDA __attribute__((section(".data")))
extern unsigned char D_0013A4E0[] NOT_SDA;
extern char D_001D5BF0[] NOT_SDA;
int FUN_00221af0(void) {
    int f = *(int *)(D_0013A4E0 + 0x2604);
    if (f & 0x20) {
        char *p = D_001D5BF0;
        char *q;
        *(int *)(p + 0xD4) = 0;
        q = *(char **)(p + 4);
        *(int *)(p + 8) = *(int *)(q + 0x38);
        *(int *)(q + 0x84) = 1;
    } else if (f & 0x10) {
        char *p = D_001D5BF0;
        char *q = *(char **)(p + 4);
        *(int *)(p + 8) = *(int *)(q + 0x38);
        *(int *)(q + 0x84) = 0;
    }
    return 0;
}

extern __typeof__(FUN_00221af0) func_00221AF0 __attribute__((alias("FUN_00221af0")));
