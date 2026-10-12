#include "sda.h"
extern int D_0015EDF0 MACRO_ADDR;
extern int pause_sound_volume __asm__("D_0013E5A0") NOT_SDA;

int FUN_0021cb00(void) {
    pause_sound_volume = D_0015EDF0 * 8 / 10;
    return 0;
}

extern __typeof__(FUN_0021cb00) func_0021CB00 __attribute__((alias("FUN_0021cb00")));
