#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_ROCKET_DATA_SPAWN_EXPLOSION_BURST_4_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_ROCKET_DATA_SPAWN_EXPLOSION_BURST_4_H

#include "types.h"

/* Scalar data for src/overlays/l18/mobys/rocket.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int explosion_burst_particle_frame_count_range_start_gp __asm__("D_L18_00161FAC")
    __attribute__((sda));
extern int explosion_burst_particle_frame_count_range_end_gp __asm__("D_L18_00161FB0")
    __attribute__((sda));
extern int explosion_burst_particle_second_frame_count_range_start_gp __asm__("D_L18_00161FB4")
    __attribute__((sda));
extern int explosion_burst_particle_second_frame_count_range_end_gp __asm__("D_L18_00161FB8")
    __attribute__((sda));
extern int explosion_burst_first_glow_duration_frames_gp __asm__("D_L18_00161FC4")
    __attribute__((sda));
extern int explosion_burst_second_glow_duration_frames_gp __asm__("D_L18_00161FD0")
    __attribute__((sda));
extern int explosion_burst_third_glow_duration_frames_gp __asm__("D_L18_00161FDC")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_ROCKET_DATA_SPAWN_EXPLOSION_BURST_4_H */
