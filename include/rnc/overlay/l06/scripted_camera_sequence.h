#ifndef LOMBYTE_RNC_OVERLAY_L06_SCRIPTED_CAMERA_SEQUENCE_H
#define LOMBYTE_RNC_OVERLAY_L06_SCRIPTED_CAMERA_SEQUENCE_H

#include "types.h"

/* Set while one of the level 06 mobys (red button, breakable window, countdown sequence) runs its scripted camera. */
extern int scripted_camera_sequence_active __asm__("D_L06_0015F404");

#endif /* LOMBYTE_RNC_OVERLAY_L06_SCRIPTED_CAMERA_SEQUENCE_H */
