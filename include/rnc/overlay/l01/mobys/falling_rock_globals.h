#ifndef LOMBYTE_RNC_OVERLAY_L01_MOBYS_FALLING_ROCK_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L01_MOBYS_FALLING_ROCK_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l01/mobys/falling_rock.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern f32 falling_rock_camera_rotation_angle __asm__("D_L01_00161C28");
extern f32 falling_rock_camera_distance __asm__("D_L01_00161C2C");
extern f32 falling_rock_camera_height __asm__("D_L01_00161C30");
extern f32 falling_rock_camera_offset_scale __asm__("D_L01_00161C34");
extern f32 falling_rock_camera_override_companion_value __asm__("D_L01_00161C38");

#endif /* LOMBYTE_RNC_OVERLAY_L01_MOBYS_FALLING_ROCK_GLOBALS_H */
