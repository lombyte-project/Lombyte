#include "types.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/ui/map.h"

s32 draw_map_screen_overlay(void) __asm__("FUN_0021f330");

s32 draw_map_screen_overlay(void) {
    vu1_add_g_sregister(0x42, 0x44);
    vu1_add_g_sregister(0x47, 0xB);
    draw_map_overlay();
    return 8;
}
