#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00207580/func_00207580.s", func_00207580);
#else
int func_00207580(int frame, float unused0, float unused1, float value) {
    /* The frame comparison is signed; both float thresholds are inclusive. */
    if (frame < 0xE0) {
        return value >= 42.0f && value <= 43.0f;
    }
    if (value >= 39.0f) {
        return 1;
    }
    return 0;
}
#endif /* NON_MATCHING */
