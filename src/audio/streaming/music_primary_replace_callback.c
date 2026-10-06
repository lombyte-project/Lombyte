/* Ported from rac1-decomp (src/game/stream.c, func_00217970). */
#include "sda.h"
extern short D_001516F0 NOT_SDA;
void music_primary_replace_callback(int result, long context) __asm__("FUN_00216ad0");

void music_primary_replace_callback(int result, long context) {
    short *channel_state = (short *)(int)context;
    if (channel_state != 0) {
        if (result < 0) {
            *(int *)channel_state = result;
        }
        if (result != 0) {
            if (channel_state[5] == 9) {
                channel_state[5] = 4;
                if (channel_state[8] != 0) {
                    D_001516F0 = 1;
                }
            }
        } else {
            channel_state[5] = 0;
        }
    }
}

extern __typeof__(music_primary_replace_callback) func_00216AD0
    __attribute__((alias("FUN_00216ad0")));
