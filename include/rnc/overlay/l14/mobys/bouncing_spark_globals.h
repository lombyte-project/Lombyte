#ifndef LOMBYTE_RNC_OVERLAY_L14_MOBYS_BOUNCING_SPARK_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L14_MOBYS_BOUNCING_SPARK_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l14/mobys/bouncing_spark.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float spark_contact_clearance __asm__("D_L14_00162088");
extern float spark_minimum_speed __asm__("D_L14_00162090");
extern float spark_rolling_speed_factor __asm__("D_L14_00162094");
extern float spark_initial_moby_value_multiplier __asm__("D_L14_00162098");
extern float spark_first_bounce_horizontal_speed_gp __asm__("D_L14_001620A4") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L14_MOBYS_BOUNCING_SPARK_GLOBALS_H */
