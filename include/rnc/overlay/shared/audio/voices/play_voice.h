#ifndef LOMBYTE_RNC_OVERLAY_SHARED_AUDIO_VOICES_PLAY_VOICE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_AUDIO_VOICES_PLAY_VOICE_H

#include "types.h"

s32 engine_voice_set_pitch(s32, s32) __asm__("FUN_L00_0028df38");
s32 play_voice(s32, s32, void *, s32) __asm__("FUN_L00_0028ddd8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_AUDIO_VOICES_PLAY_VOICE_H */
