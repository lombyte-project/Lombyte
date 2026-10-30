/* Level 07 moby class 0x211: purple ship. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/game/item_state.h"

extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_002FD198), where it is exact; names translated to the US level program. */
extern void func_00233AB8(void) __asm__("FUN_L07_002ba260");
void        purple_ship_update(char *arg) __asm__("FUN_L07_002fbdb8");

void purple_ship_update(char *purple_ship_moby) {
    purple_ship_moby[0x30] = 8;
    MOBY(purple_ship_moby)->oclass = 0x215;
    add_draw_callback((void *)func_00233AB8, purple_ship_moby);
}
