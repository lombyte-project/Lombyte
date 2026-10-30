#include "types.h"

extern s32 draw_map_overlay() __asm__("FUN_00205640");
extern s32 vu1_add_g_sregister() __asm__("FUN_00233980");
s32        draw_map_screen_overlay(void) __asm__("FUN_0021f330");

s32 draw_map_screen_overlay(void) {
    vu1_add_g_sregister(0x42, 0x44);
    vu1_add_g_sregister(0x47, 0xB);
    draw_map_overlay();
    return 8;
}
