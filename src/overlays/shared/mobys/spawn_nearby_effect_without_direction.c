/* Mobys: spawns a nearby effect without a direction. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"

void spawn_nearby_effect_without_direction(unsigned char *effect_owner,
                                           int            x,
                                           int            y,
                                           void          *effect_position,
                                           float effect_priority) __asm__("FUN_L10_0024c7f0");

void spawn_nearby_effect_without_direction(

    unsigned char *effect_owner, int x, int y, void *effect_position, float effect_priority) {
    spawn_nearby_effect(effect_owner, x, y, effect_position, D_L10_0015F580, effect_priority);
}
