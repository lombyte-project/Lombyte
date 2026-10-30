/* Shared moby class trackdrive_left. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"

extern unsigned int l02_trackdrive_left_moby_address_cutoff __asm__("D_L02_0015FFDC");

/* Ported from rac1-decomp (src/overlays/shared/vendor_002A5218.c: func_L02_002CCD18), where it is exact; names translated to the US level program. */

void trackdrive_left_update(char *moby) __asm__("FUN_L02_002cb978");

void trackdrive_left_update(char *moby) {
    if ((unsigned int)moby < l02_trackdrive_left_moby_address_cutoff) {
        *(int *)(moby + 0x58) = 0;
        MOBY(moby)->flags |= 0x40;
    }
}
