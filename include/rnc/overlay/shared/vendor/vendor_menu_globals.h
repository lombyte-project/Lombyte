#ifndef LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_MENU_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_MENU_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/vendor/vendor_menu.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern f32 vendor_camera_eye_local_offset_z __asm__("D_L00_00161068");
extern int vendor_auxiliary_reaction_started __asm__("D_L00_00161190");
extern s32 vendor_ammo_decrease_held_frames __asm__("D_L00_00161E28");
extern s32 vendor_ammo_increase_held_frames __asm__("D_L00_00161E2C");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_MENU_GLOBALS_H */
