/* Ported from rac1-decomp, the PAL decompilation (src/game/sound.c, func_0022F0A8). */
void voice_start_callback(int handle, long context) __asm__("FUN_0022dd90");

void voice_start_callback(int handle, long context) {
    unsigned char *voice = (unsigned char *)(int)context;
    if (voice != 0) {
        *(int *)voice = handle;
        if (handle != 0) {
            if (voice[4] == 1) {
                voice[4] = 2;
            }
        } else {
            *(int *)(voice + 0x18) = 0;
            *(int *)(voice + 0x1C) = 0;
            voice[4] = 0;
        }
    }
}

extern __typeof__(voice_start_callback) func_0022DD90 __attribute__((alias("FUN_0022dd90")));
