/* Level 04 moby class transport_ship. */
#include "types.h"
#include "rnc/mobys/moby_spawn.h"

/* Marks the moby for removal; the unused buffer is kept for its frame. */
void transport_ship_update(void *moby) __asm__("FUN_L04_002ce060");

/* Moby class 484 */
void transport_ship_update(void *moby) {
    char unused_frame_padding[0x50];

    mark_moby_for_removal(moby);
}
