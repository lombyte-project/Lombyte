/* Shared moby class helipack. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"

extern char D_0013E533[];

void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D1168.c: func_L00_002D4C80), where it is exact; names translated to the US level program. */

/* Blends the moby to animation 6 when D_0013F350+0x2084 is 8. */

void helipack_update(char *a) __asm__("FUN_L00_002d37d0");

void helipack_update(char *moby) {
    if (*(int *)(D_0013E533 + 0x2EA1) == 8) {
        if (MOBY(moby)->prev_seq != 6) {
            blend_moby_animation(moby, 6, 0, scale_game_frames(10));
        }
    }
}
