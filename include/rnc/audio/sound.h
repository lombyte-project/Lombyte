#ifndef LOMBYTE_RNC_AUDIO_SOUND_H
#define LOMBYTE_RNC_AUDIO_SOUND_H

#include "rnc/audio/voice_pool.h"
#include "types.h"

struct VoiceVolumeDefinition;

s32  calculate_voice_distance_volume(struct VoiceVolumeDefinition *,
                                     f32,
                                     f32,
                                     f32) __asm__("FUN_0022c6f8");
s32  calculate_voice_pan(s32, f32 *, void *) __asm__("FUN_0022c830");
void clamp_voice_position_to_collision(s32) __asm__("FUN_0022c5a8");
void initialize_gameplay_sound_system(void) __asm__("FUN_0022c8d0");
int  snd_get_doppler_pitch_mod(int);
void store_sound_bank_handle_callback(int, long) __asm__("FUN_0022dd78");
int  test_voice_occlusion(VoiceSlot *, void *) __asm__("FUN_0022c658");
void voice_start_callback(int, long) __asm__("FUN_0022dd90");
void voice_update_callback(int, long) __asm__("FUN_0022ddd8");

#endif /* LOMBYTE_RNC_AUDIO_SOUND_H */
