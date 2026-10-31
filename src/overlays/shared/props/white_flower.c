/* Shared moby class white_flower (also lamps, lightposts and pipes): breaks into fragments and bolts when hit. */

#include "types.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/mobys/moby.h"

extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void
spawn_moby_break_fragments(void *, int, int, int, int, int, int) __asm__("FUN_L00_00263e30");
extern void spawn_proximity_gated_bolts(void *) __asm__("FUN_L01_002787a0");
extern void spawn_moby_from_placement(void *, int) __asm__("FUN_L01_00278e20");

/* Update for a moby that waits for a float flag on a nearby object then runs a delete sequence. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002F3398), where it is exact; names translated to the US level program. */
void white_flower_update(struct Moby *m) __asm__("FUN_L02_002f1f60");

/* Moby class 1819 */
void white_flower_update(struct Moby *moby) {
    enum {
        WHITE_FLOWER_INITIALIZE = 0,
        WHITE_FLOWER_WAIT_FOR_HIT = 1,
        WHITE_FLOWER_REMOVE = 2
    };
    int   hit = 0;
    char *hit_record = find_moby_hit(moby, 0x10000, 0);
    switch (moby->state) {
    case WHITE_FLOWER_INITIALIZE:
        moby->state = 1;
        break;
    case WHITE_FLOWER_WAIT_FOR_HIT:
        if (hit_record != 0 && *(float *)(hit_record + 0x2C) > 0.0f)
            hit = 1;
        if (hit)
            moby->state = 2;
        break;
    case WHITE_FLOWER_REMOVE:
        allocate_voice_for_target_entry(0, 0, (int)moby);
        spawn_proximity_gated_bolts(moby);
        spawn_moby_break_fragments(moby, 0x71D, 1, 0x71D, 1, 5, 2);
        spawn_moby_from_placement(moby, 0x71C);
        mark_moby_for_removal(moby);
        break;
    }
}
