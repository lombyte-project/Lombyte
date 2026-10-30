#ifndef LOMBYTE_RNC_OVERLAY_L08_MOBYS_BOMBER_TANK_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L08_MOBYS_BOMBER_TANK_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l08/mobys/bomber_tank.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float bomber_tank_projectile_speed __asm__("D_L08_001618A8");
extern int   bomber_tank_part_3_destroyed_by_class_661 __asm__("D_L08_0015F5B8");
extern float bomber_tank_part_reference_speed __asm__("D_L08_001618A0");
extern float bomber_tank_joint_particle_jitter_magnitude_base __asm__("D_L08_001618DC");
extern float bomber_tank_joint_particle_vertical_spread __asm__("D_L08_001618E0");
extern float bomber_tank_joint_particle_horizontal_spread_range_end __asm__("D_L08_001618E8");
extern int   available_moby_slot_count __asm__("D_L08_0015FFBC");

#endif /* LOMBYTE_RNC_OVERLAY_L08_MOBYS_BOMBER_TANK_GLOBALS_H */
