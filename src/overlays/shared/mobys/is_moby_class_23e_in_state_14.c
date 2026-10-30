/* Moby state helpers: tests for a class 0x23E moby in state 14, and puts a moby in state 14. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"

/* Recognize the active vendor object state. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_00310738), where it is exact; names translated to the US level program. */

int is_moby_class_23e_in_state_14(char *moby) __asm__("FUN_L03_002c6c60");

int is_moby_class_23e_in_state_14(char *moby) {
    if (MOBY(moby)->oclass == 0x23e && ((unsigned char *)moby)[0x20] == 14) {
        return 1;
    }
    return 0;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_00301090), where it is exact; names translated to the US level program. */

void set_moby_state_14(unsigned char *moby) __asm__("FUN_L03_002c6c90");

void set_moby_state_14(unsigned char *moby) {
    moby[0x20] = 14;
}
