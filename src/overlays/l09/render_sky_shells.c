/* Level 09 sky: draws the sky shells. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/overlay/shared/rendering/sky/update_sky_shells_animated.h"
#include "rnc/rendering/sky/do_sky_gif_paging.h"
#include "rnc/rendering/sky/setup_sky_gif_paging.h"

extern void draw_sky_shells_with_modulo_25000_rotation(void) __asm__("FUN_L00_0028a3a8");

void render_sky_shells(void) __asm__("FUN_L09_0025d648");

void render_sky_shells(void) {
    setup_sky_gif_paging();
    if (current_level_index == 0x9) {
        draw_sky_shells_with_modulo_25000_rotation();
    } else {
        update_sky_shells_static();
    }
    do_sky_gif_paging();
    vu1_add_g_sregister(0x47, 0x5360B);
    vu1_add_g_sregister(0x4E, 0x1000000 | (depth_buffer_address >> 13));
}
