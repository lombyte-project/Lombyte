#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CUTSCENES_00295100_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CUTSCENES_00295100_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/cutscenes/00295100.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern int controller_idle_frames __asm__("D_L00_00160FF0");
extern f32 vendor_camera_eye_local_offset_x __asm__("D_L00_00161060");
extern f32 vendor_camera_eye_local_offset_z __asm__("D_L00_00161068");
extern int vendor_close_hero_local_offset __asm__("D_L00_00161070");
extern s32 vendor_exit_hero_local_offset __asm__("D_L00_00161100");
extern s32 vendor_exit_cutscene_local_offset __asm__("D_L00_001610F0");
extern int vendor_auxiliary_reaction_started __asm__("D_L00_00161190");
extern int vendor_selected_item_local_offset __asm__("D_L00_00161090");
extern s32 vendor_purchase_confirmed __asm__("D_L00_00161E0C");
extern s32 vendor_ammo_quantity_adjustment_enabled __asm__("D_L00_00161E10");
extern s32 vendor_ammo_decrease_held_frames __asm__("D_L00_00161E28");
extern s32 vendor_ammo_increase_held_frames __asm__("D_L00_00161E2C");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CUTSCENES_00295100_GLOBALS_H */
