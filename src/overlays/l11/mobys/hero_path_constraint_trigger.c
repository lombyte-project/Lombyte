/* Level 11 moby class hero_path_constraint_trigger (class 361): hands its path constraint to the hero while the hero is high up. */
#include "types.h"

#include "rnc/hero/hero.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/mobys/moby_spawn.h"

void mark_moby_for_removal(struct Moby *obj) __asm__("FUN_0020c828");

void hero_path_constraint_trigger_update(struct Moby *moby) __asm__("FUN_L11_002f3350");
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002F46C0), where it is exact; names translated to the US level program. */

/* Moby class 361 */
void hero_path_constraint_trigger_update(struct Moby *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_COPY_PVAR_TO_HERO = 1
    };
    char *state = (char *)moby->pvars;
    switch (moby->state) {
    case MOBY_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        if (*(int *)state == -1)
            mark_moby_for_removal(moby);
        break;
    case MOBY_COPY_PVAR_TO_HERO:
        if (hero.motion.pos.f[2] > 194.0f && hero.state.current != 0x32) {
            *(short unsigned int *)&hero.path_constrain_pending = *(unsigned short *)state;
        }
        break;
    }
}
