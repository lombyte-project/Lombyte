/* Ported from rac1-decomp, the PAL decompilation (src/game/stream.c, func_00217830). */
void music_channel_ready_callback(int result, long context) __asm__("FUN_00216990");

void music_channel_ready_callback(int result, long context) {
    short *channel_state = (short *)(int)context;
    if (channel_state != 0 && result != 0 && channel_state[5] == 2) {
        channel_state[5] = 3;
    }
}

extern __typeof__(music_channel_ready_callback) func_00216990 __attribute__((alias("FUN_00216990")));
