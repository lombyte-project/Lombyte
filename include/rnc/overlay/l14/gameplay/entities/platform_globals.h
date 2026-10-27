#ifndef LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_PLATFORM_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_PLATFORM_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l14/gameplay/entities/platform.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float joint_light_position_w __asm__("D_L14_00161A60");

/* Additional scalar data read by this unit. */
extern float weapon_aim_final_path_step __asm__("D_L14_00161A40") __attribute__((sda));
extern float muzzle_flash_joint_axis_offset __asm__("D_L14_00161A44") __attribute__((sda));
extern float muzzle_flash_particle_full_blend_endpoint __asm__("D_L14_00161A48") __attribute__((sda));
extern float muzzle_light_full_blend_position_w __asm__("D_L14_00161A4C") __attribute__((sda));
extern float muzzle_light_color_component_x __asm__("D_L14_00161A50") __attribute__((sda));
extern float muzzle_light_color_component_y __asm__("D_L14_00161A54") __attribute__((sda));
extern float muzzle_light_color_component_z __asm__("D_L14_00161A58") __attribute__((sda));
extern float unused_platform_tilt_argument __asm__("D_L14_00161AFC") __attribute__((sda));
extern float platform_max_tilt_degrees __asm__("D_L14_00161B00") __attribute__((sda));
extern float platform_tilt_direction_z_offset __asm__("D_L14_00161B04") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_PLATFORM_GLOBALS_H */
