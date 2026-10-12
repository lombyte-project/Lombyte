#include "types.h"
#include "rnc/globals.h"
extern s32 level_bss_start __asm__("D_00161280");
extern void (*D_001A0438[])(void);
void run_state_handler(void) __asm__("FUN_00208840");

void run_state_handler(void) {
    s32 state = mode_freeze_state;

    D_001A0438[state]();
    level_bss_start++;
    if (mode_freeze_state != state) {
        level_bss_start = 0;
    }
}

extern __typeof__(run_state_handler) func_00208840 __attribute__((alias("FUN_00208840")));
