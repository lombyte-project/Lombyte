#ifndef LOMBYTE_RNC_AUDIO_MUSIC_H
#define LOMBYTE_RNC_AUDIO_MUSIC_H

#include "types.h"

void music_pause(s32);
void music_start_track(s32, s32, s32) __asm__("FUN_00215c40");
void music_start_track_10000(s32, s32, s32) __asm__("FUN_002158a0");
void music_start_track_20000(s32, s32, s32) __asm__("FUN_002157d0");
void music_start_track_30000(s32, s32, s32) __asm__("FUN_002156d8");
void music_start_track_40000(s32, s32, s32) __asm__("FUN_00215600");
void music_start_track_50000(s32, s32, s32) __asm__("FUN_00215518");
void music_start_track_60000(s32, s32, s32) __asm__("FUN_00215440");
void music_update(void) __asm__("FUN_00216290");
void reset_music(void) __asm__("FUN_00215390");

#endif /* LOMBYTE_RNC_AUDIO_MUSIC_H */
