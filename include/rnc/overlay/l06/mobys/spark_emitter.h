#ifndef LOMBYTE_RNC_OVERLAY_L06_MOBYS_SPARK_EMITTER_H
#define LOMBYTE_RNC_OVERLAY_L06_MOBYS_SPARK_EMITTER_H

#include "types.h"

struct Moby;

void spark_emitter_update(struct Moby *) __asm__("FUN_L06_00308c68");
void activate_listed_spark_emitters(int) __asm__("FUN_L06_00309240");

#endif /* LOMBYTE_RNC_OVERLAY_L06_MOBYS_SPARK_EMITTER_H */
