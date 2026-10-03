/* Ported from rac1-decomp, the PAL decompilation (src/game/stream.c, func_002179C8). */
void music_primary_start_callback(int handle, long context) __asm__("FUN_00216b28");

void music_primary_start_callback(int handle, long context) {
    short *channel_state = (short *)(int)context;
    if (channel_state != 0) {
        *(int *)channel_state = handle;
        if (handle != 0) {
            if (channel_state[5] == 1) {
                channel_state[5] = 8;
            }
        } else {
            channel_state[5] = 0;
        }
    }
}

extern __typeof__(music_primary_start_callback) func_00216B28 __attribute__((alias("FUN_00216b28")));
