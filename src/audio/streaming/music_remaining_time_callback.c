/* Ported from rac1-decomp (src/game/stream.c, func_00217A60). */
extern short D_001516D0[];
void music_remaining_time_callback(int remaining_time, long context) __asm__("FUN_00216bc0");

void music_remaining_time_callback(int remaining_time, long context) {
    char *channel = (char *)(int)context;
    short *music_state;
    if (channel == 0) {
        return;
    }
    *(int *)(channel + 0x18) = remaining_time;
    if (*(short *)(channel + 0x10) == 0) {
        return;
    }
    music_state = D_001516D0;
    if (music_state[0x10] != 1) {
        return;
    }
    if (remaining_time == 0) {
        return;
    }
    music_state[0x10] = 2;
    *(int *)((char *)music_state + 0x24) = *(int *)(channel + 0x18);
    *(int *)((char *)music_state + 0x28) = *(int *)(channel + 0x18) / 4;
}

extern __typeof__(music_remaining_time_callback) func_00216BC0
    __attribute__((alias("FUN_00216bc0")));
