/* Ported from rac1-decomp, the PAL decompilation (src/game/sound.c, func_0022F0F0). */
void FUN_0022ddd8(int arg0, long arg1) {
    int *p = (int *)(int)arg1;
    if (p != 0) {
        *p = arg0;
        if (arg0 == 0) {
            *(int *)((char *)p + 0x18) = 0;
            *(int *)((char *)p + 0x1C) = 0;
            *(unsigned char *)((char *)p + 4) = 0;
        }
    }
}

extern __typeof__(FUN_0022ddd8) func_0022DDD8 __attribute__((alias("FUN_0022ddd8")));
