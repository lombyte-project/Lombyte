/* Level 13 purple ship moby class. */
#include "types.h"
#include "rnc/rendering/level_render_state.h"

typedef struct {
    char           pad0[0x34];
    unsigned short flags;
    char           pad36[0x5E];
    int            field94;
} Level13VendorMoby;

void purple_ship_update(Level13VendorMoby *moby) __asm__("FUN_L13_002f3750");

void purple_ship_update(Level13VendorMoby *moby) {
    if (*(void **)(((char *)&level_render_state)) != moby) {
        moby->field94 = 0;
        moby->flags |= 3;
    }
}
