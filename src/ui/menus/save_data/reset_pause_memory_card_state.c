/* Ported from rac1-decomp (src/game/pause.c, func_0021DAC8). */
#include "sda.h"
extern int pause_memory_card_state __asm__("D_001A0318") NOT_SDA;
int reset_pause_memory_card_state(void) __asm__("FUN_0021cac8");

int reset_pause_memory_card_state(void) {
    pause_memory_card_state = -1;
    return 0;
}

extern __typeof__(reset_pause_memory_card_state) func_0021CAC8 __attribute__((alias("FUN_0021cac8")));
