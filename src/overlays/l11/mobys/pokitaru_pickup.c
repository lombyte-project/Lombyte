/* Level 11 moby class pokitaru_pickup: waits for the player to come near, then grants the item. */
#include "types.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/data_labels.h"
#include "rnc/game/item_state.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/moby_spawn.h"

/* Per-frame update of a pickup moby: waits for the player to come near, then grants the item. */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002CC828), where it is exact; names translated to the US level program. */

void pokitaru_pickup_update(struct Moby *moby) __asm__("FUN_L11_002cb668");

/* Moby class 23 */
void pokitaru_pickup_update(struct Moby *moby) {
    enum {
        PICKUP_INITIALIZE = 0,
        PICKUP_IDLE = 1,
        PICKUP_CHECK_PROXIMITY = 2
    };
    switch (moby->state) {
    case PICKUP_INITIALIZE:
        if (item_available[ITEM_O2_MASK] != 0) {
            mark_moby_for_removal(moby);
        } else {
            moby->pos.z += 0.75f;
            moby->flags |= 1;
            moby->state = PICKUP_IDLE;
        }
        break;
    case PICKUP_IDLE:
        break;
    case PICKUP_CHECK_PROXIMITY:
        fast_add_rotations(moby->rot.z, frame_time * 4.7123890f);
        if (vector_distance_xy(&moby->pos, D_0013E533 + 0xE9D) < 2.0f) {
            item_available[ITEM_O2_MASK] = 1;
            items_acquired[ITEM_O2_MASK] = 1;
            memcard_save_data(0, -1);
            help_request(0x2AF8, 0x3A);
            mark_moby_for_removal(moby);
        }
        break;
    }
}
