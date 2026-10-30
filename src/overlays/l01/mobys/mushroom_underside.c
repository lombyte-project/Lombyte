/* Level 01 moby class mushroom_underside. */
#include "types.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"

#include "qcopy.h"

void mushroom_underside_update(void *self) __asm__("FUN_L01_00307c48");

void mushroom_underside_update(void *self) {
    u8 *hit;

    hit = find_moby_hit(self, 0x10000, 0);
    if (hit != NULL && *(f32 *)(hit + 0x2C) > 0.0f) {
        mark_moby_for_removal(self);
    }
}
