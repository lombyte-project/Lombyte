/* Level 18 sky: draws the sky shells with GIF texture paging. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/overlay/shared/rendering/sky/update_sky_shells_animated.h"
#include "rnc/rendering/sky/do_sky_gif_paging.h"
#include "rnc/rendering/sky/setup_sky_gif_paging.h"

extern void
draw_seven_sky_shells_with_modulo_40000_and_60000_rotation(void) __asm__("FUN_L00_0028b080");

void draw_level_sky_shells(void) __asm__("FUN_L18_00240840");

void draw_level_sky_shells(void) {
    setup_sky_gif_paging();
    if (current_level_index == 0x12) {
        draw_seven_sky_shells_with_modulo_40000_and_60000_rotation();
    } else {
        update_sky_shells_static();
    }
    do_sky_gif_paging();
    vu1_add_g_sregister(0x47, 0x5360B);
    vu1_add_g_sregister(0x4E, 0x1000000 | (depth_buffer_address >> 13));
}
