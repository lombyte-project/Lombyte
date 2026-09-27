#include "types.h"

extern volatile s32 D_001611E0;

void ClearStageStateFlag(void) {
    D_001611E0 = 0;
}
