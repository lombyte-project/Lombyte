#ifndef LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_PLATFORM_DATA_UPDATE_RACER_PATH_AND_WEAPON_H
#define LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_PLATFORM_DATA_UPDATE_RACER_PATH_AND_WEAPON_H

#include "types.h"

/* Scalar data for src/overlays/l14/gameplay/entities/platform.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float weapon_aim_final_path_step __asm__("D_L14_00161A40") __attribute__((sda));
extern float muzzle_flash_joint_axis_offset __asm__("D_L14_00161A44") __attribute__((sda));
extern float muzzle_flash_particle_full_blend_endpoint __asm__("D_L14_00161A48") __attribute__((sda));
extern float muzzle_light_full_blend_position_w __asm__("D_L14_00161A4C") __attribute__((sda));
extern float muzzle_light_color_component_x __asm__("D_L14_00161A50") __attribute__((sda));
extern float muzzle_light_color_component_y __asm__("D_L14_00161A54") __attribute__((sda));
extern float muzzle_light_color_component_z __asm__("D_L14_00161A58") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_PLATFORM_DATA_UPDATE_RACER_PATH_AND_WEAPON_H */
