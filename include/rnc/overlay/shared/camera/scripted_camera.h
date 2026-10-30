#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_SCRIPTED_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_SCRIPTED_CAMERA_H

#include "types.h"

void set_camera_position(void *) __asm__("FUN_L00_002ea9d8");
void jet_mount(void *, void *, s32, s32, s32) __asm__("FUN_L00_002eaaa0");
void jet_end_mount(s32) __asm__("FUN_L00_002eac18");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_SCRIPTED_CAMERA_H */
