#include "types.h"
extern s32 FUN_001f97e8();
void init_moby_class_dists(void) __asm__("FUN_0020d1f0");

void init_moby_class_dists(void) {
    FUN_001f97e8(0x70003A00, 0x40000000, 0x380);
}

/* Recovered original symbol name. */
extern __typeof__(init_moby_class_dists) InitMobyClassDists__Fv
    __attribute__((alias("FUN_0020d1f0")));
