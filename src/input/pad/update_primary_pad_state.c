#include "types.h"
#include "rnc/input/pad_state.h"
extern s32 poll_pad_device_state() __asm__("FUN_002170c8");
void update_primary_pad_state(void) __asm__("FUN_00217a10");

void update_primary_pad_state(void) {
    poll_pad_device_state(&controller_state);
}

extern void func_00217A10(void) __attribute__((alias("FUN_00217a10")));
