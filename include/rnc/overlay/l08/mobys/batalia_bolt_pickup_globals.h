#ifndef LOMBYTE_RNC_OVERLAY_L08_MOBYS_BATALIA_BOLT_PICKUP_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L08_MOBYS_BATALIA_BOLT_PICKUP_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l08/mobys/batalia_bolt_pickup.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float bolt_pickup_default_surface_orbit_speed_degrees __asm__("D_L08_00161C9C");
extern float five_bolt_pickup_surface_roll_speed_degrees __asm__("D_L08_00161CA8");
extern float ten_bolt_pickup_surface_roll_speed_degrees __asm__("D_L08_00161CB0");
extern int   bolt_pickup_collection_alpha_fade_enabled __asm__("D_L08_00161CC0");
extern int   bolt_pickup_rotating_quad_enabled __asm__("D_L08_00161CC8");
extern float bolt_pickup_default_surface_base_tilt_degrees __asm__("D_L08_00161C64");
extern float bolt_pickup_default_surface_planar_offset_per_scale __asm__("D_L08_00161C7C");
extern float five_bolt_pickup_surface_planar_offset_per_scale __asm__("D_L08_00161C80");
extern float ten_bolt_pickup_surface_normal_offset_per_scale __asm__("D_L08_00161C78");
extern float ten_bolt_pickup_surface_planar_offset_per_scale __asm__("D_L08_00161C84");
extern float five_bolt_pickup_surface_base_tilt_degrees __asm__("D_L08_00161C68");
extern float ten_bolt_pickup_surface_base_tilt_degrees __asm__("D_L08_00161C6C");
extern float bolt_pickup_default_surface_normal_offset_per_scale __asm__("D_L08_00161C70");
extern float five_bolt_pickup_surface_normal_offset_per_scale __asm__("D_L08_00161C74");

#endif /* LOMBYTE_RNC_OVERLAY_L08_MOBYS_BATALIA_BOLT_PICKUP_GLOBALS_H */
