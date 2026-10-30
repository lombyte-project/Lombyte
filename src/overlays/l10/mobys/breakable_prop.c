/* Level 10 moby class breakable_prop (0x740): when hit, sheds bolts and leaves its broken placement moby (class 0x741). */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"

/* UpdateMoby 1856: waits for a hit, then plays its sound, sheds bolts, spawns its broken placement moby (class 0x741) and deletes itself. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_002E30F8.c: func_L10_002ECBA0), where it is exact; names translated to the US level program. */

extern void spawn_moby_from_placement(void *, int) __asm__("FUN_L01_00278e20");

void update_breakable_prop(struct Moby *moby) __asm__("FUN_L10_002eb7e0");

void update_breakable_prop(struct Moby *moby) {
    enum {
        HIT_RESPONSE_STATE_INITIALIZE = 0,
        HIT_RESPONSE_STATE_WAIT_FOR_HIT = 1,
        HIT_RESPONSE_STATE_RESPOND_AND_REMOVE = 2
    };
    int   ok = 0;
    char *hit_result = find_moby_hit(moby, 0x10000, 0);
    switch (moby->state) {
    case HIT_RESPONSE_STATE_INITIALIZE:
        moby->state = 1;
        break;
    case HIT_RESPONSE_STATE_WAIT_FOR_HIT:
        if (hit_result != 0) {
            if (0.0f < *(float *)(hit_result + 0x2C))
                ok = 1;
        }
        if (ok != 0)
            moby->state = 2;
        break;
    case HIT_RESPONSE_STATE_RESPOND_AND_REMOVE:
        allocate_voice_for_target_entry(0, 0, (int)moby);
        spawn_proximity_gated_bolts(moby);
        spawn_moby_from_placement(moby, 0x741);
        mark_moby_for_removal(moby);
        break;
    }
}
