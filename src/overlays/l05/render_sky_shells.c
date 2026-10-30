/* Level 05 sky: render_sky_shells, which sets up sky GIF paging and draws the sky shells with their particles. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/overlay/shared/rendering/sky/update_sky_shells_animated.h"
#include "rnc/rendering/sky/do_sky_gif_paging.h"
#include "rnc/rendering/sky/setup_sky_gif_paging.h"

extern void
draw_sky_shells_with_particles_and_modulo_50000_rotation(void) __asm__("FUN_L00_00289bd8");

void render_sky_shells(void) __asm__("FUN_L05_00267520");

void render_sky_shells(void) {
    setup_sky_gif_paging();
    if (current_level_index == 0x5) {
        draw_sky_shells_with_particles_and_modulo_50000_rotation();
    } else {
        update_sky_shells_static();
    }
    do_sky_gif_paging();
    vu1_add_g_sregister(0x47, 0x5360B);
    vu1_add_g_sregister(0x4E, 0x1000000 | (depth_buffer_address >> 13));
}
