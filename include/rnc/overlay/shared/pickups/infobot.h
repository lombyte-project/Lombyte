/* Prototypes exported by shared/pickups/infobot.c. */
#ifndef RCP2P_RNC_OVERLAY_SHARED_PICKUPS_INFOBOT_H
#define RCP2P_RNC_OVERLAY_SHARED_PICKUPS_INFOBOT_H

#include "types.h"

struct Moby;

void infobot_update(struct Moby *) __asm__("FUN_L00_002d5de8");
void initialize_moby_particle_effect(void *) __asm__("FUN_L00_002d6bf0");

#endif /* RCP2P_RNC_OVERLAY_SHARED_PICKUPS_INFOBOT_H */
