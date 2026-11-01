#ifndef LOMBYTE_RNC_INPUT_PAD_GLOBALS_H
#define LOMBYTE_RNC_INPUT_PAD_GLOBALS_H

#include "types.h"
#include "sda.h"

/* Button words within controller_state, at offsets 0x1A0 and 0x1A4. */
extern s32 pad_held __asm__("D_0013CAE0");
extern s32 pad_pressed __asm__("D_0013CAE4");
extern s32 pad_held_lui __asm__("D_0013CAE0") NOT_SDA;
extern s32 pad_pressed_lui __asm__("D_0013CAE4") NOT_SDA;

#endif /* LOMBYTE_RNC_INPUT_PAD_GLOBALS_H */
