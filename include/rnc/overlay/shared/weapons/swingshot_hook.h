#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_SWINGSHOT_HOOK_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_SWINGSHOT_HOOK_H

#include "types.h"

unsigned char *spawn_swingshot_hook(unsigned char *owner_moby,
                                    float          initial_hook_speed) __asm__("FUN_L00_002c6f88");

void swingshot_hook_update(unsigned char *) __asm__("FUN_L00_002c7070");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WEAPONS_SWINGSHOT_HOOK_H */
