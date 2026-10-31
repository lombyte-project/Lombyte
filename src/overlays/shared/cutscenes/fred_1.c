/* Shared moby class fred_1 (and fred_2, fred_3). */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"

extern void FUN_0020c828(void *);

void fred_1_update(char *m) __asm__("FUN_L03_002dc310");

/* Moby classes 915-917 */
void fred_1_update(char *moby) {
    float unused_stack_padding[12]; /* unused; sets the frame size */
    fast_vec_dist(moby + 0x10, hero.motion.pos.f);
    fast_difference_between_rotations(fast_arc_tan(hero.motion.pos.f[0] - MOBY(moby)->pos.x,
                                                   hero.motion.pos.f[1] - MOBY(moby)->pos.y),
                                      MOBY(moby)->rot.z);
    FUN_0020c828(moby);
}
