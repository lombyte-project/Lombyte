#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PROPS_SPINNING_FRAGMENT_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PROPS_SPINNING_FRAGMENT_H

#include "types.h"

struct Moby;

void           falling_spinning_moby_update(struct Moby *) __asm__("FUN_L06_00300b90");
unsigned char *spawn_spinning_moby_fragment(char *, float *, float *) __asm__("FUN_L06_00300c60");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PROPS_SPINNING_FRAGMENT_H */
