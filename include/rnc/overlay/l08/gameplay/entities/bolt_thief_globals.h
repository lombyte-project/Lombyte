#ifndef LOMBYTE_RNC_OVERLAY_L08_GAMEPLAY_ENTITIES_BOLT_THIEF_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L08_GAMEPLAY_ENTITIES_BOLT_THIEF_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l08/gameplay/entities/bolt_thief.c. */
extern float shrub_part_spin_speed_degrees __asm__("D_L08_00161D28") __attribute__((sda));
extern float commando_part_spin_speed_degrees __asm__("D_L08_00161D38") __attribute__((sda));
extern float gunship_part_spin_speed_degrees __asm__("D_L08_00161C3C") __attribute__((sda));
extern s32 gunship_updates_disabled __asm__("D_L08_00161C4C") __attribute__((sda));
extern f32 gunship_path_speed __asm__("D_L08_00161C38") __attribute__((sda));
extern f32 gunship_turn_acceleration_degrees __asm__("D_L08_00161C44") __attribute__((sda));
extern f32 gunship_max_turn_speed_degrees __asm__("D_L08_00161C48") __attribute__((sda));
extern float bolt_thief_waypoint_speed __asm__("D_L08_00161C50") __attribute__((sda));
extern float bolt_pickup_default_surface_orbit_speed_degrees __asm__("D_L08_00161C9C");
extern float five_bolt_pickup_surface_roll_speed_degrees __asm__("D_L08_00161CA8");
extern float ten_bolt_pickup_surface_roll_speed_degrees __asm__("D_L08_00161CB0");
extern int bolt_pickup_collection_alpha_fade_enabled __asm__("D_L08_00161CC0");
extern int bolt_pickup_rotating_quad_enabled __asm__("D_L08_00161CC8");
extern float bolt_pickup_default_surface_base_tilt_degrees __asm__("D_L08_00161C64") __attribute__((sda));
extern float bolt_pickup_default_surface_planar_offset_per_scale __asm__("D_L08_00161C7C") __attribute__((sda));
extern float five_bolt_pickup_surface_planar_offset_per_scale __asm__("D_L08_00161C80") __attribute__((sda));
extern float ten_bolt_pickup_surface_normal_offset_per_scale __asm__("D_L08_00161C78") __attribute__((sda));
extern float ten_bolt_pickup_surface_planar_offset_per_scale __asm__("D_L08_00161C84") __attribute__((sda));
extern float five_bolt_pickup_surface_base_tilt_degrees __asm__("D_L08_00161C68") __attribute__((sda));
extern float ten_bolt_pickup_surface_base_tilt_degrees __asm__("D_L08_00161C6C") __attribute__((sda));
extern float bolt_pickup_default_surface_normal_offset_per_scale __asm__("D_L08_00161C70") __attribute__((sda));
extern float five_bolt_pickup_surface_normal_offset_per_scale __asm__("D_L08_00161C74") __attribute__((sda));
extern s32 shrub_updates_disabled __asm__("D_L08_00161D30") __attribute__((sda));
extern f32 shrub_path_speed __asm__("D_L08_00161D24") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L08_GAMEPLAY_ENTITIES_BOLT_THIEF_GLOBALS_H */
