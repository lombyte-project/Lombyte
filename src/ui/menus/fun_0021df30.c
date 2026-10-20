/* Ported from rac1-decomp (src/game/pause.c, func_0021EF38). */
int FUN_0021df30(void *destination) {
    char *destination_bytes = (char *)destination;
    *(float *)(destination_bytes + 0x38) = 3.14159274f;
    *(int *)(destination_bytes + 0x34) = 0;
    *(int *)(destination_bytes + 0x44) = 0;
    *(int *)(destination_bytes + 0x48) = 0;
    return 0;
}

extern __typeof__(FUN_0021df30) func_0021DF30 __attribute__((alias("FUN_0021df30")));
