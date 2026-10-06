/* Ported from rac1-decomp (src/game/stream.c, func_00217920). */
#include "sda.h"
extern short D_001516F0 NOT_SDA;
void music_transition_start_callback(int handle, long context) __asm__("FUN_00216a80");

void music_transition_start_callback(int handle, long context) {
    short *channel_state = (short *)(int)context;
    if (channel_state != 0) {
        *(int *)channel_state = handle;
        if (handle != 0) {
            short previous_state = channel_state[5];
            if (previous_state == 1) {
                channel_state[5] = 4;
                if (channel_state[8] != 0) {
                    D_001516F0 = previous_state;
                }
            }
        } else {
            channel_state[5] = 0;
        }
    }
}

extern __typeof__(music_transition_start_callback) func_00216A80
    __attribute__((alias("FUN_00216a80")));
