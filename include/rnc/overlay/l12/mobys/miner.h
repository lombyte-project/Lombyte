#ifndef LOMBYTE_RNC_OVERLAY_L12_MOBYS_MINER_H
#define LOMBYTE_RNC_OVERLAY_L12_MOBYS_MINER_H

#include "rnc/mobys/moby.h"

/* Defined in src/overlays/l12/mobys/miner.c; blarg_trooper.c uses it too. */

/* Places the child moby on bone `mode` of the parent moby. */
void update_moby_bone_attachment(struct Moby   *parent,
                                 unsigned char *child,
                                 int            mode) __asm__("FUN_L12_0027b9c0");

#endif
