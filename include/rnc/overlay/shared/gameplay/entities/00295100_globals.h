#ifndef LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_00295100_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_00295100_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/shared/gameplay/entities/00295100.c. */
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

/* Additional scalar data read by this unit. */
extern f32 vendor_basis_local_vector_x_gp __asm__("D_L00_001610D0") __attribute__((sda));
extern s32 vendor_auxiliary_moby_local_offset_gp __asm__("D_L00_00161080") __attribute__((sda));
extern s32 vendor_item_display_common_local_offset_gp __asm__("D_L00_001610A0") __attribute__((sda));
extern s32 vendor_rotating_display_local_offset_gp __asm__("D_L00_001610B0") __attribute__((sda));
extern s32 vendor_menu_rotating_display_local_offset_gp __asm__("D_L00_001610C0") __attribute__((sda));
extern s32 vendor_close_delay_frames_remaining __asm__("D_L00_00161E14");
extern s32 vendor_opening_countdown_frames __asm__("D_L00_00161E1C");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_GAMEPLAY_ENTITIES_00295100_GLOBALS_H */
