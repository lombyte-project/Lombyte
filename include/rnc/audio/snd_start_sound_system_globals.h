#ifndef LOMBYTE_RNC_AUDIO_SND_START_SOUND_SYSTEM_GLOBALS_H
#define LOMBYTE_RNC_AUDIO_SND_START_SOUND_SYSTEM_GLOBALS_H

#include "types.h"

/* The two sound command batches: a command count, then the commands.
 * snd_start_sound_system leaves 0xFFC bytes free behind the count. */
extern s32 snd_command_buffer1[0x400] __asm__("D_00133280");
extern s32 snd_command_buffer2[0x400] __asm__("D_00134280");

#endif /* LOMBYTE_RNC_AUDIO_SND_START_SOUND_SYSTEM_GLOBALS_H */
