/* Level 05 moby class qwarks_trailer. */
#include "types.h"
#include "rnc/mobys/moby.h"

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_0031B138), where it is exact; names translated to the US level program. */

extern int D_L05_0015F5C4_c __asm__("D_L05_0015F5C4");

void qwarks_trailer_update(struct Moby *moby) __asm__("FUN_L05_00319c28");

/* Moby class 1099 */
void qwarks_trailer_update(struct Moby *moby) {
    if (D_L05_0015F5C4_c == 2) {
        moby->visible = 0;
        moby->flags |= 1;
    } else {
        moby->visible = 1;
        moby->flags &= 0xFFFE;
    }
}
