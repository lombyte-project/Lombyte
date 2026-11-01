#ifndef LOMBYTE_RNC_AUDIO_STREAMING_MUSIC_CALLBACKS_H
#define LOMBYTE_RNC_AUDIO_STREAMING_MUSIC_CALLBACKS_H

#include "types.h"

void music_channel_ready_callback(int, long) __asm__("FUN_00216990");
void music_primary_replace_callback(int, long) __asm__("FUN_00216ad0");
void music_primary_start_callback(int, long) __asm__("FUN_00216b28");
void music_remaining_time_callback(int, long) __asm__("FUN_00216bc0");
void music_secondary_start_callback(u32, s64) __asm__("FUN_002169c0");
void music_transition_start_callback(int, long) __asm__("FUN_00216a80");

#endif /* LOMBYTE_RNC_AUDIO_STREAMING_MUSIC_CALLBACKS_H */
