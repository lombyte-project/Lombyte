#include "types.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/game_globals.h"
extern struct Moby *moby_instance_end __asm__("D_0015FF1C");
extern void update_moby_grids(struct Moby *, u32) __asm__("func_0020DC20");
void mark_moby_for_removal(struct Moby *moby) __asm__("FUN_0020c828");

void mark_moby_for_removal(struct Moby *moby) {
    if (moby < moby_instance_end) {
        moby->state = 0xFD;
    } else {
        moby->state = 0xFE;
    }
    moby->spawn_frame = world_update_time + 2;
    update_moby_grids(moby, 0x80807F7F);
}

extern __typeof__(mark_moby_for_removal) func_0020C828 __attribute__((alias("FUN_0020c828")));
