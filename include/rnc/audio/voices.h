#ifndef LOMBYTE_RNC_AUDIO_VOICES_H
#define LOMBYTE_RNC_AUDIO_VOICES_H

#include "rnc/audio/voice_pool.h"
#include "types.h"

s32 allocate_voice_for_group_entry(s32, s32, s32) __asm__("FUN_0022dba0");
s32 allocate_voice_slot(VoiceDefinition *, u32, VoiceMoby *, VoiceVector *, s32) __asm__("FUN_0022d7f0");
s32  is_voice_active(s32, s32) __asm__("FUN_001eb740");
void snd_play_sound_vol_pan_pmpb(s32, s32, s32, s32, s32, s32, s32, s32) __asm__("FUN_0012e308");
void snd_set_sound_params_cb(s32, s32, s32, s32, s32, s32, s32, s32) __asm__("FUN_0012e4c0");
void snd_sound_is_still_playing_cb(s32, s32, s32) __asm__("FUN_0012e448");

#endif /* LOMBYTE_RNC_AUDIO_VOICES_H */
