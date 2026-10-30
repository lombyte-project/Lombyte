#include "types.h"
extern s32 rotate_thread_ready_queue() __asm__("FUN_001188c0");
void switch_thread(void) __asm__("FUN_0023a770");

void switch_thread(void) {
    rotate_thread_ready_queue(1);
}

extern void func_0023A770(void) __attribute__((alias("FUN_0023a770")));
