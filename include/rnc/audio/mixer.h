#ifndef LOMBYTE_RNC_AUDIO_MIXER_H
#define LOMBYTE_RNC_AUDIO_MIXER_H

#include "types.h"

void snd_set_group_voice_range(s32, s32, s32) __asm__("FUN_0012e2b8");
void snd_set_mixer_mode(s32, s32) __asm__("FUN_0012e280");

#endif /* LOMBYTE_RNC_AUDIO_MIXER_H */
