#ifndef LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_MORPHORAY_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_MORPHORAY_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l14/gameplay/entities/morphoray.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float spark_contact_clearance __asm__("D_L14_00162088");
extern float spark_minimum_speed __asm__("D_L14_00162090");
extern float spark_rolling_speed_factor __asm__("D_L14_00162094");
extern float spark_initial_moby_value_multiplier __asm__("D_L14_00162098");
extern f32 morphoray_ground_height_offset __asm__("D_L14_001621E8");
extern f32 morphoray_yaw_step __asm__("D_L14_001621EC");
extern f32 morphoray_position_offset_scale __asm__("D_L14_001621F0");
extern int flying_car_path_reset_delay_frames __asm__("D_L14_0016227C");
extern s32 path_camera_active __asm__("D_L14_0015F404");

#endif /* LOMBYTE_RNC_OVERLAY_L14_GAMEPLAY_ENTITIES_MORPHORAY_GLOBALS_H */
