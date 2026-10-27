#ifndef LOMBYTE_RNC_OVERLAY_L13_GAMEPLAY_ENTITIES_ELEVATOR_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L13_GAMEPLAY_ENTITIES_ELEVATOR_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l13/gameplay/entities/elevator.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float blarg_turret_launch_displacement_scale __asm__("D_L13_00161388");
extern float robosquawker_damage_knockback_vertical_speed __asm__("D_L13_00161390");
extern float robosquawker_damage_knockback_horizontal_speed __asm__("D_L13_00161394");
extern float robosquawker_home_hop_horizontal_speed __asm__("D_L13_00161398");

/* Additional scalar data read by this unit. */
extern float robosquawker_home_hop_gravity_gp __asm__("D_L13_0016139C") __attribute__((sda));
extern float fighter_jet_frame_scale __asm__("D_L13_0015ED60");
extern int persistence_table_row_index __asm__("D_L13_0015ED84");

#endif /* LOMBYTE_RNC_OVERLAY_L13_GAMEPLAY_ENTITIES_ELEVATOR_GLOBALS_H */
