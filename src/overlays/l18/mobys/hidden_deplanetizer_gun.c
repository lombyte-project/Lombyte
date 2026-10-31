/* Level 18 moby class hidden_deplanetizer_gun (0x620, named deplanetizer_gun in the class record): hides and turns over during the deplanetizer cutscene, and shows again when it ends. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"

extern int D_L18_0015F5C4;

/* State machine: waits for level flags, then turns the moby around and back. */
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002F9D48.c: func_L18_002FBAA0), where it is exact; names translated to the US level program. */

extern int D_L18_0016D290_lui __asm__("D_L18_0016D290") NOT_SDA;

void hidden_deplanetizer_gun_update(struct Moby *moby) __asm__("FUN_L18_002fa640");

/* Moby class 1568 */
void hidden_deplanetizer_gun_update(struct Moby *moby) {
    enum {
        INITIALIZE_STATE_MACHINE = 0,
        WAIT_TO_HIDE_AND_TURN = 1,
        HIDDEN_WAIT_TO_SHOW = 2
    };
    switch (moby->state) {
    case INITIALIZE_STATE_MACHINE:
        moby->state = 1;
        break;
    case WAIT_TO_HIDE_AND_TURN:
        if (D_L18_0015F5C4 == 2 && D_L18_0016D290_lui == 3) {
            moby->flags |= 1;
            moby->visible = 0;
            moby->rot.x = fast_add_rotations(moby->rot.x, 3.1415927f);
            moby->state = 2;
        }
        break;
    case HIDDEN_WAIT_TO_SHOW:
        if (D_L18_0015F5C4 != 2) {
            unsigned short fl = moby->flags;
            moby->state = 3;
            fl &= 0xFFFE;
            moby->visible = 1;
            moby->flags = fl;
        }
        break;
    }
}
