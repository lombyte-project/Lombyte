#ifndef LOMBYTE_RNC_OVERLAY_L08_MOBYS_BOMBER_TANK_DATA_SPAWN_BOMBER_TANK_JOINT_PARTICLES_H
#define LOMBYTE_RNC_OVERLAY_L08_MOBYS_BOMBER_TANK_DATA_SPAWN_BOMBER_TANK_JOINT_PARTICLES_H

#include "types.h"

/* Scalar data for src/overlays/l08/mobys/bomber_tank.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int bomber_tank_joint_particle_duration_frames_range_start_gp __asm__("D_L08_001618EC")
    __attribute__((sda));
extern int bomber_tank_joint_particle_second_frame_count_range_start_gp __asm__("D_L08_001618F0")
    __attribute__((sda));
extern int bomber_tank_joint_particle_third_frame_count_range_start_gp __asm__("D_L08_001618F4")
    __attribute__((sda));
extern float bomber_tank_joint_particle_velocity_w_gp __asm__("D_L08_001618F8")
    __attribute__((sda));
extern float bomber_tank_joint_particle_spread_vector_w_gp __asm__("D_L08_001618FC")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L08_MOBYS_BOMBER_TANK_DATA_SPAWN_BOMBER_TANK_JOINT_PARTICLES_H */
