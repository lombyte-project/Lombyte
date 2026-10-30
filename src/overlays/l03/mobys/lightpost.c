/* Level 03 moby class lightpost. */
#include "types.h"
#include "asm.h"
#include "rnc/mobys/moby.h"

#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
/* Update for a moby that waits for a float flag on a nearby object then runs a delete sequence. */

extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void spawn_moby_from_placement(void *, int) __asm__("FUN_L01_00278e20");

void lightpost_update(struct Moby *m) __asm__("FUN_L03_002e2948");

void lightpost_update(struct Moby *lightpost_moby) {
    enum {
        LIGHTPOST_INITIALIZE = 0,
        LIGHTPOST_WAIT_FOR_HIT = 1,
        LIGHTPOST_REMOVE = 2
    };
    int   hit = 0;
    char *hit_result = find_moby_hit(lightpost_moby, 0x10000, 0);
    switch (lightpost_moby->state) {
    case LIGHTPOST_INITIALIZE:
        lightpost_moby->state = 1;
        break;
    case LIGHTPOST_WAIT_FOR_HIT:
        if (hit_result != 0 && *(float *)(hit_result + 0x2C) > 0.0f)
            hit = 1;
        if (hit)
            lightpost_moby->state = 2;
        break;
    case LIGHTPOST_REMOVE:
        allocate_voice_for_target_entry(0, 0, (int)lightpost_moby);
        spawn_proximity_gated_bolts(lightpost_moby);
        spawn_moby_break_fragments(lightpost_moby, 0x725, 1, 0x725, 1, 0x1, 2);
        spawn_moby_from_placement(lightpost_moby, 0x724);
        mark_moby_for_removal(lightpost_moby);
        break;
    }
}
