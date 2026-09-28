#include "types.h"
extern s32 D_0015EE30;
extern u8 D_00141E08[];
extern s32 func_001FECC8(s16, s32, u16 *);
void push_help_history(s32 id) __asm__("FUN_001fed30");

void push_help_history(s32 id) {
    s32 slot;
    s32 i;

    slot = func_001FECC8(id, 0, 0);
    if (slot == -1) {
        return;
    }
    for (i = 0; D_00141E08[i] != slot && i < D_0015EE30; i++) {
    }
    if (i < D_0015EE30) {
        for (; i < D_0015EE30 - 1; i++) {
            D_00141E08[i] = D_00141E08[i + 1];
        }
        D_00141E08[i] = 0;
        D_0015EE30--;
    }
    D_00141E08[D_0015EE30] = slot;
    D_0015EE30++;
}

extern __typeof__(push_help_history) func_001FED30 __attribute__((alias("FUN_001fed30")));
