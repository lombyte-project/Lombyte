/* Level 12 moby class lamp. */
#include "types.h"
#include "asm.h"

#include "sda.h"
#include "rnc/mobys/moby.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
/* Lamp: once hit, plays its sound, drops bolts, breaks into class 0x748 fragments, spawns class 0x747 and removes itself. */

extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void spawn_moby_from_placement(void *, int) __asm__("FUN_L01_00278e20");

void lamp_update(struct Moby *m) __asm__("FUN_L12_0030b6c8");

/* Moby class 1862 */
void lamp_update(struct Moby *lamp_moby) {
    enum {
        LAMP_STATE_INITIALIZE = 0,
        LAMP_STATE_WAIT_FOR_HIT = 1,
        LAMP_STATE_REMOVE = 2
    };
    int          hit = 0;
    struct Moby *hit_moby = find_moby_hit(lamp_moby, 0x10000, 0);
    switch (lamp_moby->state) {
    case LAMP_STATE_INITIALIZE:
        lamp_moby->state = 1;
        break;
    case LAMP_STATE_WAIT_FOR_HIT:
        if (hit_moby != 0 && hit_moby->scale > 0.0f)
            hit = 1;
        if (hit)
            lamp_moby->state = 2;
        break;
    case LAMP_STATE_REMOVE:
        allocate_voice_for_target_entry(0, 0, (int)lamp_moby);
        spawn_proximity_gated_bolts(lamp_moby);
        spawn_moby_break_fragments(lamp_moby, 0x748, 1, 0x748, 1, 0xb, 2);
        spawn_moby_from_placement(lamp_moby, 0x747);
        mark_moby_for_removal(lamp_moby);
        break;
    }
}
