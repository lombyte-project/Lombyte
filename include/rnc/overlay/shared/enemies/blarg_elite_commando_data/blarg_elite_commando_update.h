#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_BLARG_ELITE_COMMANDO_DATA_BLARG_ELITE_COMMANDO_UPDATE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_BLARG_ELITE_COMMANDO_DATA_BLARG_ELITE_COMMANDO_UPDATE_H

#include "types.h"

/* Scalar data for src/overlays/shared/enemies/blarg_elite_commando.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern int blarg_elite_commando_bobbing_and_voice_enabled_gp __asm__("D_L16_00161A74")
    __attribute__((sda));
extern int blarg_elite_commando_joint_aim_enabled_gp __asm__("D_L16_00161A78") __attribute__((sda));
extern int blarg_elite_commando_joint_sparks_enabled_gp __asm__("D_L16_00161A7C")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_BLARG_ELITE_COMMANDO_DATA_BLARG_ELITE_COMMANDO_UPDATE_H */
