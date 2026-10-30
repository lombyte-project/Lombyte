/* Puts a moby in state 2 and starts its particle effect. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/shared/pickups/infobot.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002E7578), where it is exact; names translated to the US level program. */
void initialize_moby_particle_effect_state(char *moby) __asm__("FUN_L06_002e6148");

void initialize_moby_particle_effect_state(char *moby) {
    moby[0x20] = 2;
    MOBY(moby)->flags &= 0xFFBE;
    initialize_moby_particle_effect(moby);
}
