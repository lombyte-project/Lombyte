#ifndef LOMBYTE_RNC_OVERLAY_L05_MOBYS_TAXI_H
#define LOMBYTE_RNC_OVERLAY_L05_MOBYS_TAXI_H

#include "types.h"

struct Moby;

void taxi_update(struct Moby *) __asm__("FUN_L05_00318c78");
void initialize_taxi_stop_state(void *) __asm__("FUN_L05_00319208");

#endif /* LOMBYTE_RNC_OVERLAY_L05_MOBYS_TAXI_H */
