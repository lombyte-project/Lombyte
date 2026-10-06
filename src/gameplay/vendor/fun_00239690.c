#include "types.h"
extern void func_001FB440(s32, s32, s32);
extern void configure_graphics_projection(s32, s32, f32, f32, f32, f32,
                                          f32) __asm__("func_001F33B8");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
void FUN_00239690(s32 width_log2, s32 height_log2, f32 fparg0) {
    s32 sum;
    sum = width_log2 + height_log2;
    sum = (sum < 17) ? sum : 16;
    func_001FB440(width_log2, height_log2, ((0x3FF000 - (4 << sum)) >> 13) << 13);
    configure_graphics_projection(1 << width_log2, 1 << height_log2, fparg0, 0.0f, 524288.0f, 255.0f, 0.0f);
    vu1_add_g_sregister(0x47, 0x30000);
    vu1_add_g_sregister(0x42, (((u64)0x8000) << 24) | 0x44);
}
