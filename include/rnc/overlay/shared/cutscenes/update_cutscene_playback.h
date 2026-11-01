#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CUTSCENES_UPDATE_CUTSCENE_PLAYBACK_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CUTSCENES_UPDATE_CUTSCENE_PLAYBACK_H

#include "types.h"

void start_scene(void *) __asm__("FUN_L00_00298840");
void queue_scene_movie_with_game_language(void *) __asm__("FUN_L00_00299460");

void handle_pause_input(void) __asm__("FUN_L00_00297f78");
void play_queued_movie(void) __asm__("FUN_L00_002999a8");
void update_cutscene_playback(void) __asm__("FUN_L00_00298f90");
void update_slideshow(void) __asm__("FUN_L00_00299c48");
void prepare_movie_playback(s32, s32, s32) __asm__("FUN_L00_002995d0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CUTSCENES_UPDATE_CUTSCENE_PLAYBACK_H */
