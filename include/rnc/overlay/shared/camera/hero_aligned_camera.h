#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_HERO_ALIGNED_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_HERO_ALIGNED_CAMERA_H

#include "types.h"

void initialize_hero_aligned_camera_mode(void) __asm__("FUN_L00_002ea7a0");
int  camera_transition_update(u8 *, char *) __asm__("FUN_L00_002ea488");
void select_camera_mode_by_hero_vector_alignment() __asm__("FUN_L00_002ea810");
void update_hero_aligned_camera_from_input(u8 *) __asm__("FUN_L00_002e9f38");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_HERO_ALIGNED_CAMERA_H */
