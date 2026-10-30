/* Level 04 moby class pipe. */
#include "types.h"
#include "asm.h"

#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/mobys/moby.h"
/* Update for a moby that waits for a float flag on a nearby object then runs a delete sequence. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern void  allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void  spawn_moby_from_placement(void *, int) __asm__("FUN_L01_00278e20");

void pipe_update(struct Moby *m) __asm__("FUN_L04_002e79a8");

void pipe_update(struct Moby *pipe_moby) {
    enum {
        PIPE_INITIALIZE = 0,
        PIPE_WAIT_FOR_HIT = 1,
        PIPE_REMOVE = 2
    };
    int          hit = 0;
    struct Moby *hit_moby = find_moby_hit(pipe_moby, 0x10000, 0);
    switch (pipe_moby->state) {
    case PIPE_INITIALIZE:
        pipe_moby->state = 1;
        break;
    case PIPE_WAIT_FOR_HIT:
        if (hit_moby != 0 && hit_moby->scale > 0.0f)
            hit = 1;
        if (hit)
            pipe_moby->state = 2;
        break;
    case PIPE_REMOVE:
        allocate_voice_for_target_entry(0, 0, (int)pipe_moby);
        spawn_proximity_gated_bolts(pipe_moby);
        spawn_moby_break_fragments(pipe_moby, 0x72c, 1, 0x72c, 1, 0x1, 2);
        spawn_moby_from_placement(pipe_moby, 0x72b);
        mark_moby_for_removal(pipe_moby);
        break;
    }
}
