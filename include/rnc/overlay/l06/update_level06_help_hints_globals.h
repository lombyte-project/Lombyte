#ifndef LOMBYTE_RNC_OVERLAY_L06_GAMEPLAY_VENDOR_003083B0_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L06_GAMEPLAY_VENDOR_003083B0_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l06/update_level06_help_hints.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float spark_burst_vector_scale_variation __asm__("D_L06_001622FC");
extern float spark_spray_z_subtraction_rate __asm__("D_L06_00162338");
extern float spark_spray_vector_scale_variation __asm__("D_L06_0016233C");
extern int   spark_spray_timer_range_start __asm__("D_L06_00162374");
extern int   spark_spray_timer_range_end __asm__("D_L06_00162378");

/* Additional scalar data read by this unit. */
extern float spark_burst_rotation_row0_scale __asm__("D_L06_001622F0") __attribute__((sda));
extern float spark_burst_initial_z_scale __asm__("D_L06_001622F4") __attribute__((sda));
extern float spark_burst_rotation_row1_scale_variation __asm__("D_L06_001622F8")
    __attribute__((sda));
extern int   spark_burst_emission_random_span __asm__("D_L06_00162300") __attribute__((sda));
extern float spark_burst_emission_scale_variation __asm__("D_L06_0016230C") __attribute__((sda));
extern float spark_burst_rotation_vector_w_range_start __asm__("D_L06_00162310")
    __attribute__((sda));
extern float spark_burst_rotation_vector_w_range_end __asm__("D_L06_00162314") __attribute__((sda));
extern float spark_burst_z_adjusted_vector_w_range_start __asm__("D_L06_00162318")
    __attribute__((sda));
extern float spark_burst_z_adjusted_vector_w_range_end __asm__("D_L06_0016231C")
    __attribute__((sda));
extern float spark_spray_rotation_row0_scale __asm__("D_L06_00162330") __attribute__((sda));
extern float spark_spray_second_vector_scale __asm__("D_L06_00162334") __attribute__((sda));
extern float spark_spray_offset_random_range_end __asm__("D_L06_00162340") __attribute__((sda));
extern int   spark_spray_emission_random_span __asm__("D_L06_00162344") __attribute__((sda));
extern float spark_spray_emission_scale_variation __asm__("D_L06_00162350") __attribute__((sda));
extern float spark_spray_rotation_vector_w_range_start __asm__("D_L06_00162354")
    __attribute__((sda));
extern float spark_spray_rotation_vector_w_range_end __asm__("D_L06_00162358") __attribute__((sda));
extern float spark_spray_z_adjusted_vector_w_range_start __asm__("D_L06_0016235C")
    __attribute__((sda));
extern float spark_spray_z_adjusted_vector_w_range_end __asm__("D_L06_00162360")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L06_GAMEPLAY_VENDOR_003083B0_GLOBALS_H */
