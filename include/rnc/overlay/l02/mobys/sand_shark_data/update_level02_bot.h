#ifndef LOMBYTE_RNC_OVERLAY_L02_MOBYS_SAND_SHARK_DATA_UPDATE_LEVEL02_BOT_H
#define LOMBYTE_RNC_OVERLAY_L02_MOBYS_SAND_SHARK_DATA_UPDATE_LEVEL02_BOT_H

#include "types.h"

/* Scalar data for src/overlays/l02/mobys/sand_shark.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern float level02_bot_attack_shot_nominal_length_gp __asm__("D_L02_00161A80")
    __attribute__((sda));
extern float level02_bot_gravity_gp __asm__("D_L02_00161A84") __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L02_MOBYS_SAND_SHARK_DATA_UPDATE_LEVEL02_BOT_H */
