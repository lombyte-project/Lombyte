#include "types.h"

extern u32 D_0015EE74;
extern u32 D_0015EE8C;
extern u32 texture_alloc_counter __asm__("D_0015EF20");

void InitializeStreamingState(void) {
    D_0015EE74 = D_0015EE8C;
    texture_alloc_counter = 0;
}
