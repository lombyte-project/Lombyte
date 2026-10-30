#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PROPS_NOVALIS_ELEVATOR_DATA_SPAWN_AMOEBOID_MOVEMENT_PARTICLES_2_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PROPS_NOVALIS_ELEVATOR_DATA_SPAWN_AMOEBOID_MOVEMENT_PARTICLES_2_H

#include "types.h"

/* Scalar data for src/overlays/shared/props/novalis_elevator.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int amoeboid_first_movement_particle_header_and_data_value_gp __asm__("D_L01_001619B0")
    __attribute__((sda));
extern int amoeboid_first_movement_particle_data_value_gp __asm__("D_L01_001619B4")
    __attribute__((sda));
extern int amoeboid_second_movement_particle_header_and_data_value_gp __asm__("D_L01_001619C8")
    __attribute__((sda));
extern int amoeboid_second_movement_particle_data_value_gp __asm__("D_L01_001619CC")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PROPS_NOVALIS_ELEVATOR_DATA_SPAWN_AMOEBOID_MOVEMENT_PARTICLES_2_H */
