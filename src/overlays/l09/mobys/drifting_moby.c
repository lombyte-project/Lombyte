/* Level 09 moby class 0x1a1: drifting moby. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/game/item_state.h"
#include "sda.h"

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002F1390), where it is exact; names translated to the US level program. */
void update_drifting_moby(struct Moby *moby) __asm__("FUN_L09_002f0040");

void update_drifting_moby(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    fast_vec_add(&moby->pos, &moby->pos, data);
    fast_vec_scale(data, data, 1.0f + frame_scale * -0.019999980926513672f);
    if (moby->anim_flags & 2) {
        mark_moby_for_removal(moby);
    }
}
