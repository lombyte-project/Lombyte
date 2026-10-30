/* Level 10 moby class shattering_prop (0x73F): when hit, shatters into class 0x742 fragments and bolts and deletes itself. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"

/* UpdateMoby 1855: waits for a hit, then plays its sound, sheds bolts, shatters into class 0x742 fragments and deletes itself. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_002E30F8.c: func_L10_002ECAB0), where it is exact; names translated to the US level program. */

void update_shattering_prop(struct Moby *moby) __asm__("FUN_L10_002eb6f0");

void update_shattering_prop(struct Moby *moby) {
    enum {
        HIT_RESPONSE_STATE_INITIALIZE = 0,
        HIT_RESPONSE_STATE_WAIT_FOR_HIT = 1,
        HIT_RESPONSE_STATE_RESPOND_AND_REMOVE = 2
    };
    int   hit = 0;
    char *hit_record = find_moby_hit(moby, 0x10000, 0);
    switch (moby->state) {
    case HIT_RESPONSE_STATE_INITIALIZE:
        moby->state = 1;
        break;
    case HIT_RESPONSE_STATE_WAIT_FOR_HIT:
        if (hit_record != 0 && *(float *)(hit_record + 0x2C) > 0.0f)
            hit = 1;
        if (hit)
            moby->state = 2;
        break;
    case HIT_RESPONSE_STATE_RESPOND_AND_REMOVE:
        allocate_voice_for_target_entry(0, 0, (int)moby);
        spawn_proximity_gated_bolts(moby);
        spawn_moby_break_fragments(moby, 0x742, 1, 0x742, 1, 11, 2);
        mark_moby_for_removal(moby);
        break;
    }
}
