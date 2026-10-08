#include "rnc/gameplay/hero.h"
/* Ported from rac1-decomp (src/game/menu.c, func_002080B0). */
#include "sda.h"
extern unsigned char D_0013D3E2 NOT_SDA;
/* The struct Hero state test of func_00207340, computed up front (retail
   evaluates it before the arg0 branch), gates the arg0 < 0x15F arm; the
   other arm is func_00208160's second test with D_0013D3E2. */
int FUN_00207880(int arg0, float unused1, float unused2, float arg1) {
    struct Hero *s = &hero;
    int a = s->state.control_mode == 17 || s->state.control_mode == 18 || s->base_condition == 1;

    if (arg0 < 0x15F) {
        return (arg1 >= 200.0f) ? a : 0;
    }
    return arg1 >= 233.0f && arg1 <= 235.0f && D_0013D3E2 != 0;
}

extern __typeof__(FUN_00207880) func_00207880 __attribute__((alias("FUN_00207880")));
