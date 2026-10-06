#include "types.h"
extern s32 D_0013E504[];
extern s64 D_00160588;
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64,
                               s64) __asm__("func_001F5450");
extern void vu1_add_g_sregister(s32, s64) __asm__("func_00233980");
void draw_resident_textured_banner(s32 alpha) __asm__("FUN_0022ea08");

void draw_resident_textured_banner(s32 alpha) {
    vu1_add_g_sregister(0x47, 0x31801);
    vu1_add_g_sregister(0x42, (((s64)0x8000) << 24) | 0x44);
    draw_textured_quad(0x20, D_0013E504[0] - 0x58, 0x100, 0x20, 0, 0, 0x100, 0x20,
                       (s64)((alpha << 24) | 0x808080), D_00160588);
    vu1_add_g_sregister(0x47, 0x5360B);
}
