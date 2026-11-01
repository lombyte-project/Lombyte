#ifndef LOMBYTE_RNC_OVERLAY_SHARED_AUDIO_EMITTERS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_AUDIO_EMITTERS_H

#include "types.h"

void update_box_emitter(char *) __asm__("FUN_L01_00319928");
void update_sound_emitter_playback_by_distance(u8 *) __asm__("FUN_L00_002ed950");
void update_ground_height_modulated_voice(char *) __asm__("FUN_L08_00316fa8");
void update_listener_emitter(int) __asm__("FUN_L01_0031a078");
void update_music_switch_emitter(int *) __asm__("FUN_L01_0031a128");
void update_sound_emitter_playback_at_random_position(u8 *) __asm__("FUN_L00_002edad8");
void update_reverb_box_emitter(char *) __asm__("FUN_L01_00319f18");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_AUDIO_EMITTERS_H */
