#ifndef LOMBYTE_RNC_OVERLAY_L05_MOBYS_ELECTRIC_GATE_DATA_UPDATE_TRIGGERED_PARTICLE_SPRAY_EMITTER_H
#define LOMBYTE_RNC_OVERLAY_L05_MOBYS_ELECTRIC_GATE_DATA_UPDATE_TRIGGERED_PARTICLE_SPRAY_EMITTER_H

#include "types.h"

/* Scalar data for src/overlays/l05/mobys/electric_gate.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int   spray_big_particle_kind_gp __asm__("D_L05_00161E98") __attribute__((sda));
extern int   spray_small_particle_kind_gp __asm__("D_L05_00161E9C") __attribute__((sda));
extern float spray_big_particle_base_scale_gp __asm__("D_L05_00161EA0") __attribute__((sda));
extern float spray_small_particle_base_scale_gp __asm__("D_L05_00161EA4") __attribute__((sda));
extern float spray_particle_speed_upper_bound_gp __asm__("D_L05_00161EA8") __attribute__((sda));
extern float spray_big_particle_speed_lower_bound_gp __asm__("D_L05_00161EAC") __attribute__((sda));
extern float spray_small_particle_speed_lower_bound_gp __asm__("D_L05_00161EB0")
    __attribute__((sda));
extern float spray_particle_lateral_spread_and_height_offset_gp __asm__("D_L05_00161EB4")
    __attribute__((sda));
extern float spray_particle_count_limit_per_update_gp __asm__("D_L05_00161EB8")
    __attribute__((sda));
extern float spray_big_particle_selection_threshold_percent_gp __asm__("D_L05_00161EBC")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L05_MOBYS_ELECTRIC_GATE_DATA_UPDATE_TRIGGERED_PARTICLE_SPRAY_EMITTER_H */
