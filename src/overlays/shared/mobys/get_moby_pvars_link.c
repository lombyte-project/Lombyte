/* Mobys: returns the pointer at +8 of the moby data when flag 0x20 at 0x34 is set, else 0. */
#include "types.h"
#include "asm.h"
#include "rnc/mobys/moby.h"

/* Returns the pointer at +8 of the moby data when flag 0x20 at 0x34 is set, else 0. */
char *get_moby_pvars_link(struct Moby *moby) __asm__("FUN_L00_002603d0");

char *get_moby_pvars_link(struct Moby *moby) {
    if (moby == 0)
        return 0;
    if ((moby->flags & 0x20) == 0)
        return 0;
    return *(char **)(((char *)moby->pvars) + 8);
}
