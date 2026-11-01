#ifndef LOMBYTE_RNC_AUDIO_EFFECTS_H
#define LOMBYTE_RNC_AUDIO_EFFECTS_H

#include "types.h"

void snd_auto_reverb(s32, s32, s32, s32) __asm__("FUN_0012efe0");
void snd_pre_alloc_reverb_work_area(s32, s32) __asm__("FUN_0012efa8");

#endif /* LOMBYTE_RNC_AUDIO_EFFECTS_H */
