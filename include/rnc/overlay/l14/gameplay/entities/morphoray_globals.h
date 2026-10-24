#ifndef LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_MORPHORAY_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_MORPHORAY_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l14/gameplay/entities/morphoray.c. */
extern float spark_contact_clearance __asm__("D_L14_00162088") __attribute__((sda));
extern float spark_minimum_speed __asm__("D_L14_00162090") __attribute__((sda));
extern float spark_rolling_speed_factor __asm__("D_L14_00162094") __attribute__((sda));
extern float spark_initial_moby_value_multiplier __asm__("D_L14_00162098") __attribute__((sda));
extern f32 morphoray_ground_height_offset __asm__("D_L14_001621E8") __attribute__((sda));
extern f32 morphoray_yaw_step __asm__("D_L14_001621EC") __attribute__((sda));
extern f32 morphoray_position_offset_scale __asm__("D_L14_001621F0") __attribute__((sda));
extern int flying_car_path_reset_delay_frames __asm__("D_L14_0016227C") __attribute__((sda));
extern s32 path_camera_active __asm__("D_L14_0015F404");

#endif /* LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_MORPHORAY_GLOBALS_H */
