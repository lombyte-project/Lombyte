#ifndef LOMBYTE_RNC_OVERLAY_L03_H
#define LOMBYTE_RNC_OVERLAY_L03_H

#include "types.h"

void initialize_hero_relative_camera(void *) __asm__("FUN_L03_002eb978");
int  FUN_L03_002ebd70(void);
void update_camera_position_and_orientation(void *) __asm__("FUN_L03_002ebdb0");
void update_camera_state_outside_clip_volume(char *) __asm__("FUN_L03_002ebde8");

#endif /* LOMBYTE_RNC_OVERLAY_L03_H */
