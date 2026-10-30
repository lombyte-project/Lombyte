/* Level 02 moby class platform. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/game/unlock_planet.h"

extern float D_0015ED6C;

#include "qcopy.h"

#include "qzero.h"

/* Rotates a platform and updates its transform. */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002DF038), where it is exact; names translated to the US level program. */

void platform_update(struct Moby *moby) __asm__("FUN_L02_002ddc00");

void platform_update(struct Moby *moby) {
    char  zero[16];
    char  previous_rotation[16];
    char *data = (char *)moby->pvars;
    qzero(zero);
    qcopy(previous_rotation, &moby->rot);
    moby->rot.z = fast_add_rotations(moby->rot.z, D_0015ED6C * 0.08726646f);
    update_entity_matrix(data + 0x20, zero, previous_rotation, &moby->rot);
}
