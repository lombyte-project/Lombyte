/* Level 04 moby class 0x248: keeps a matrix of how far it moved since the last frame (translation_delta_matrix_update). */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "qcopy.h"

/* UpdateMoby_584 */
/* Ported from rac1-decomp (src/overlays/l04_eudora/vendor_002CB800.c: func_L04_002D4960), where it is exact; names translated to the US level program. */

extern void copy_hero_moby_field_38(void *) __asm__("FUN_L00_0025d1b8");
extern void update_entity_matrix(char *, void *, void *, void *) __asm__("FUN_L00_00260738");

void translation_delta_matrix_update(char *moby) __asm__("FUN_L04_002d3580");

/* Moby class 584 */
void translation_delta_matrix_update(char *moby) {
    char *moby_vars = ((char *)MOBY(moby)->pvars);
    float position_delta[4];
    fast_vec_sub(position_delta, moby + 0x10, moby_vars + 0xA0);
    update_entity_matrix(moby_vars + 0x60, position_delta, moby + 0x40, moby + 0x40);
    qcopy(moby_vars + 0xA0, moby + 0x10);
    if (MOBY(moby)->state == 0) {
        *(int *)(moby_vars + 0x20) = 0;
        *(short *)(moby_vars + 0x24) = 0;
        moby_vars[0x28] = 4;
        *(short *)(moby_vars + 0x3E) = 0xD;
        copy_hero_moby_field_38(moby);
        moby[0x20] = 1;
    }
}
