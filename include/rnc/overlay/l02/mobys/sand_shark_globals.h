#ifndef LOMBYTE_RNC_OVERLAY_L02_MOBYS_SAND_SHARK_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L02_MOBYS_SAND_SHARK_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l02/mobys/sand_shark.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float sand_shark_dust_vertical_velocity_scale __asm__("D_L02_001619D4");
extern float sand_shark_dust_position_jitter_radius __asm__("D_L02_001619DC");
extern float sand_shark_vertical_motion_z_range_start __asm__("D_L02_00161920");
extern float sand_shark_vertical_motion_z_range_end __asm__("D_L02_00161924");
extern float sand_shark_radial_motion_z_range_start __asm__("D_L02_00161928");
extern float sand_shark_radial_motion_z_range_end __asm__("D_L02_0016192C");
extern float sand_shark_radial_motion_xy_range_start __asm__("D_L02_00161930");
extern float sand_shark_radial_motion_xy_range_end __asm__("D_L02_00161934");
extern float sand_shark_vertical_motion_w __asm__("D_L02_00161938");
extern float sand_shark_radial_motion_w __asm__("D_L02_0016193C");
extern int   sand_shark_particle_frame_count_range_start __asm__("D_L02_00161940");
extern int   sand_shark_particle_frame_count_range_end __asm__("D_L02_00161944");
extern int   sand_shark_particle_second_frame_count_range_start __asm__("D_L02_00161948");
extern int   sand_shark_particle_second_frame_count_range_end __asm__("D_L02_0016194C");
extern int   sand_shark_particle_third_frame_count_range_start __asm__("D_L02_00161950");
extern int   sand_shark_particle_third_frame_count_range_end __asm__("D_L02_00161954");
extern float sand_shark_single_particle_vertical_motion_z_range_start __asm__("D_L02_00161964");
extern float sand_shark_single_particle_vertical_motion_z_range_end __asm__("D_L02_00161968");
extern float sand_shark_single_particle_radial_motion_z_range_start __asm__("D_L02_0016196C");
extern float sand_shark_single_particle_radial_motion_z_range_end __asm__("D_L02_00161970");
extern float sand_shark_single_particle_radial_motion_xy_range_start __asm__("D_L02_00161974");
extern float sand_shark_single_particle_radial_motion_xy_range_end __asm__("D_L02_00161978");
extern float sand_shark_single_particle_vertical_motion_w __asm__("D_L02_0016197C");
extern float sand_shark_single_particle_radial_motion_w __asm__("D_L02_00161980");
extern s32   sand_shark_single_particle_frame_count_range_start __asm__("D_L02_00161984");
extern s32   sand_shark_single_particle_frame_count_range_end __asm__("D_L02_00161988");
extern s32   sand_shark_single_particle_second_frame_count_range_start __asm__("D_L02_0016198C");
extern s32   sand_shark_single_particle_second_frame_count_range_end __asm__("D_L02_00161990");
extern s32   sand_shark_single_particle_third_frame_count_range_start __asm__("D_L02_00161994");
extern s32   sand_shark_single_particle_third_frame_count_range_end __asm__("D_L02_00161998");

#endif /* LOMBYTE_RNC_OVERLAY_L02_MOBYS_SAND_SHARK_GLOBALS_H */
