#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_MOBY_CONTROLLED_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_MOBY_CONTROLLED_CAMERA_H

#include "types.h"

int  update_moby_camera_control(char *) __asm__("FUN_L02_002fc298");
void release_moby_camera_control(char *) __asm__("FUN_L06_00316330");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_MOBY_CONTROLLED_CAMERA_H */
