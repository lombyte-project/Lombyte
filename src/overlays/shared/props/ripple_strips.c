/* Shared moby class ripple_strips_moby: draws the radial water ripple mesh. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"

extern void draw_moby_radial_ripple_mesh_buffers(void) __asm__("FUN_L15_002eac90");

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D7C00.c: func_L15_002EC490), where it is exact; names translated to the US level program. */
void ripple_strips_moby_update(struct Moby *moby) __asm__("FUN_L15_002eb0a0");

void ripple_strips_moby_update(struct Moby *moby) {
    if (moby->state == 0) {
        set_moby_color(moby, 0x40, 0x40, 0x40);
        moby->state = 1;
    } else {
        add_draw_callback((void *)draw_moby_radial_ripple_mesh_buffers, moby);
        moby->draw_dist = 0;
        moby->flags |= 1;
    }
}
