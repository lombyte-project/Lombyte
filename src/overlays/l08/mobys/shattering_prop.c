/* Level 08 moby class 0x734: shattering prop; when hit, shatters into debris and bolts and leaves its broken placement moby. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"

extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void spawn_moby_from_placement(void *, int) __asm__("FUN_L01_00278e20");

void shattering_prop_update(struct Moby *moby) __asm__("FUN_L08_0030bc00");

/* Moby class 1844 */
void shattering_prop_update(struct Moby *moby) {
    enum {
        MOBY_STATE_INITIALIZE = 0,
        MOBY_STATE_WAIT_FOR_HIT = 1,
        MOBY_STATE_REMOVE = 2
    };
    int   hit = 0;
    char *hit_result = find_moby_hit(moby, 0x10000, 0);
    switch (moby->state) {
    case MOBY_STATE_INITIALIZE:
        moby->state = 1;
        break;
    case MOBY_STATE_WAIT_FOR_HIT:
        if (hit_result != 0 && *(float *)(hit_result + 0x2C) > 0.0f)
            hit = 1;
        if (hit)
            moby->state = 2;
        break;
    case MOBY_STATE_REMOVE:
        allocate_voice_for_target_entry(0, 0, (int)moby);
        spawn_proximity_gated_bolts(moby);
        spawn_moby_break_fragments(moby, 0x736, 1, 0x737, 1, 0x1, 2);
        spawn_moby_from_placement(moby, 0x735);
        mark_moby_for_removal(moby);
        break;
    }
}
