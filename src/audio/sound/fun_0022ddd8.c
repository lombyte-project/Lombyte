/* Ported from rac1-decomp, the PAL decompilation (src/game/sound.c, func_0022F0F0). */
void voice_update_callback(int handle, long context) __asm__("FUN_0022ddd8");

void voice_update_callback(int handle, long context) {
    int *voice = (int *)(int)context;
    if (voice != 0) {
        *voice = handle;
        if (handle == 0) {
            *(int *)((char *)voice + 0x18) = 0;
            *(int *)((char *)voice + 0x1C) = 0;
            *(unsigned char *)((char *)voice + 4) = 0;
        }
    }
}

extern __typeof__(voice_update_callback) func_0022DDD8 __attribute__((alias("FUN_0022ddd8")));
