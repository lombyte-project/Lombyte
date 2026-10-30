#include "types.h"
extern void UpdateRfuDispatchState(void);
extern void startlevel(void) __asm__("func_001E9658");
extern void *parse_bin(void) __asm__("func_0012D8F8");
extern s32 FlushCache(s32);
void run_game_main_loop(void) __asm__("FUN_0012d9d8");

void run_game_main_loop(void) {
    void (*step)(void);

    UpdateRfuDispatchState();
    step = startlevel;
    while (1) {
        step();
        step = parse_bin();
        FlushCache(0);
        FlushCache(2);
    }
}

extern __typeof__(run_game_main_loop) func_0012D9D8 __attribute__((alias("FUN_0012d9d8")));
