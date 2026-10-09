#include "types.h"
#include "rnc/globals.h"
extern s32 update_sky_effects() __asm__("func_0022AE70");
extern s32 setup_sky_gif_paging() __asm__("func_0022B4C8");
extern s32 do_sky_gif_paging() __asm__("func_0022B558");
extern void vu1_add_g_sregister(s32, s64) __asm__("func_00233980");

void transition_draw_sky(void) __asm__("FUN_001e9ab8");

void transition_draw_sky(void) {
    setup_sky_gif_paging();
    update_sky_effects();
    do_sky_gif_paging();
    vu1_add_g_sregister(0x47, 0x5360B);
    vu1_add_g_sregister(0x4E, 0x01000000 | ((s32)depth_buffer_address >> 0xD));
}

extern __typeof__(transition_draw_sky) func_001E9AB8 __attribute__((alias("FUN_001e9ab8")));
