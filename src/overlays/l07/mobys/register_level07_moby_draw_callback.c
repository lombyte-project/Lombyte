/* Level 07: registers the draw callback of the class 0x214 cutscene moby in state four. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"

/* Register the level draw callback for the target moby in state four. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/initonce_002664F0.c: func_L07_002664F0), where it is exact; names translated to the US level program. */

typedef struct {
    char  pad0[0xA6];
    short class_id;
} Level07Moby;

typedef struct {
    char         pad0[0x30];
    int          state;
    char         pad34[0x154];
    Level07Moby *moby;
} Level07State;

extern Level07State D_L07_0016C8E0;
extern void         func_00233AB8(void *) __asm__("FUN_L07_002ba260");

void register_level07_moby_draw_callback(void) __asm__("FUN_L07_00265bd8");

void register_level07_moby_draw_callback(void) {
    if (current_level_index == 7 && D_L07_0016C8E0.state == 4 && D_L07_0016C8E0.moby != 0 &&
        D_L07_0016C8E0.moby->class_id == 0x214) {
        add_draw_callback(func_00233AB8, D_L07_0016C8E0.moby);
    }
}
