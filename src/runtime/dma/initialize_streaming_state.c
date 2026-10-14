#include "types.h"

extern u32 D_0015EE74;
extern s32 gs_texture_allocation_base __asm__("D_0015EE8C");
extern u32 texture_alloc_counter __asm__("D_0015EF20");

void InitializeStreamingState(void) {
    D_0015EE74 = gs_texture_allocation_base;
    texture_alloc_counter = 0;
}
