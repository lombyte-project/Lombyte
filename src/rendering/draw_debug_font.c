#include "types.h"
extern f32 font_depth_bias __asm__("D_0015F348") __attribute__((sda));
extern s32 font_quad_count __asm__("D_0015F478");
extern s32 font_queue_vu_state() __asm__("func_001F76A0");
extern s32 func_001F89A4();
extern s32 func_001F8FF0();
extern void vu1_add_g_sregister(s32, s64) __asm__("func_00233980");
/* retail small-data globals, declared to GAS before the body */

void draw_debug_font(void) __asm__("FUN_001f79a8");

void draw_debug_font(void) {
    if (font_quad_count != 0) {
        vu1_add_g_sregister(8, 5);
        vu1_add_g_sregister(0x14, 0x61);
        vu1_add_g_sregister(0x47, 0x513F1);
        vu1_add_g_sregister(0x4A, 1);
        func_001F8FF0();
        font_depth_bias = -0.04f;
        font_queue_vu_state();
        func_001F89A4();
        font_depth_bias = 0;
        vu1_add_g_sregister(0x4A, 0);
    }
}

extern __typeof__(draw_debug_font) func_001F79A8 __attribute__((alias("FUN_001f79a8")));
/* Recovered original symbol name. */
extern __typeof__(draw_debug_font) drawDebugFont __attribute__((alias("FUN_001f79a8")));
