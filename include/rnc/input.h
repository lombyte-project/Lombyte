#ifndef LOMBYTE_RNC_INPUT_H
#define LOMBYTE_RNC_INPUT_H

#include "types.h"

struct PAD;

void poll_pad_device_state(struct PAD *) __asm__("FUN_002170c8");
void reset_pad_history(void) __asm__("func_00217020");

#endif /* LOMBYTE_RNC_INPUT_H */
