/* Shared moby class code: draw_only_moby_update, the update of draw-only mobys (classes 0x773-0x7a0) that queue the level 16 reflection mesh draw. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"

extern void draw_level16_reflection_mesh_batch(void) __asm__("FUN_L16_002e9ce0");

void draw_only_moby_update(struct Moby *moby) __asm__("FUN_L16_002e9df0");

/* Moby classes 1907, 1908, 1910, 1911, 1913, 1918, 1952 */
void draw_only_moby_update(struct Moby *moby) {
    enum {
        MOBY_DRAW_ONLY_STATE_INITIALIZE = 0,
        MOBY_DRAW_ONLY_STATE_ENQUEUE_DRAW_CALLBACK = 1
    };
    switch (moby->state) {
    case MOBY_DRAW_ONLY_STATE_INITIALIZE:
        moby->state = 1;
        break;
    case MOBY_DRAW_ONLY_STATE_ENQUEUE_DRAW_CALLBACK:
        add_draw_callback((int)draw_level16_reflection_mesh_batch, (int)moby);
        break;
    }
}
