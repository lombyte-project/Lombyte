#ifndef LOMBYTE_RNC_OVERLAY_L13_MOBYS_ROCKET_DATA_UPDATE_MOBY_MOTION_AND_EMIT_SPARKS_H
#define LOMBYTE_RNC_OVERLAY_L13_MOBYS_ROCKET_DATA_UPDATE_MOBY_MOTION_AND_EMIT_SPARKS_H

#include "types.h"

/* Scalar data for src/overlays/l13/mobys/rocket.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float moby_spark_second_batch_discarded_random_bound_gp __asm__("D_L13_00161F58")
    __attribute__((sda));
extern float moby_spark_first_batch_discarded_velocity_w_initializer_gp __asm__("D_L13_00161F5C")
    __attribute__((sda));
extern float moby_spark_first_batch_acceleration_w_initializer_gp __asm__("D_L13_00161F60")
    __attribute__((sda));
extern float moby_spark_second_batch_discarded_velocity_w_initializer_gp __asm__("D_L13_00161F64")
    __attribute__((sda));
extern float moby_spark_second_batch_acceleration_w_initializer_gp __asm__("D_L13_00161F68")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L13_MOBYS_ROCKET_DATA_UPDATE_MOBY_MOTION_AND_EMIT_SPARKS_H */
