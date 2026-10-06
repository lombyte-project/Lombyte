#include "types.h"

extern volatile s32 D_001611E0;

void end_display(void) {
    D_001611E0 = 0;
}
