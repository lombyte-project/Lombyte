#ifndef LOMBYTE_RNC_INPUT_H
#define LOMBYTE_RNC_INPUT_H

#include "types.h"

struct PadState;

void poll_pad_device_state(struct PadState *) __asm__("FUN_002170c8");
void clear_pad_input(struct PadState *) __asm__("func_002172C0");
void process_pad_input(struct PadState *, u8 *, s32) __asm__("func_00217328");
void reset_pad_history(void) __asm__("func_00217020");

#endif /* LOMBYTE_RNC_INPUT_H */
