#ifndef LOMBYTE_RNC_OVERLAY_L06_MOBYS_SWITCHABLE_BEAM_BARRIER_DATA_UPDATE_SWITCHABLE_BEAM_BARRIER_H
#define LOMBYTE_RNC_OVERLAY_L06_MOBYS_SWITCHABLE_BEAM_BARRIER_DATA_UPDATE_SWITCHABLE_BEAM_BARRIER_H

#include "types.h"

/* Scalar data for src/overlays/l06/mobys/switchable_beam_barrier.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float vector_stream_moby_scale_multiplier_gp __asm__("D_L06_00162210") __attribute__((sda));
extern int   vector_stream_burst_color_blend_a_gp __asm__("D_L06_00162214") __attribute__((sda));
extern int   vector_stream_burst_color_blend_b_gp __asm__("D_L06_00162218") __attribute__((sda));
extern float vector_stream_burst_min_particle_scale_gp __asm__("D_L06_0016221C")
    __attribute__((sda));
extern float vector_stream_burst_max_particle_scale_gp __asm__("D_L06_00162220")
    __attribute__((sda));
extern int vector_stream_burst_min_lifetime_frames_gp __asm__("D_L06_00162224")
    __attribute__((sda));
extern int vector_stream_burst_max_lifetime_frames_gp __asm__("D_L06_00162228")
    __attribute__((sda));
extern float vector_stream_burst_velocity_random_scale_bound_a_gp __asm__("D_L06_0016222C")
    __attribute__((sda));
extern float vector_stream_burst_velocity_random_scale_bound_b_gp __asm__("D_L06_00162230")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L06_MOBYS_SWITCHABLE_BEAM_BARRIER_DATA_UPDATE_SWITCHABLE_BEAM_BARRIER_H */
