/* Level 07 sky: draws the sky with GIF paging. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/overlay/shared/rendering/sky/update_sky_shells_animated.h"
#include "rnc/rendering/sky/do_sky_gif_paging.h"
#include "rnc/rendering/sky/setup_sky_gif_paging.h"

extern void
draw_three_sky_shells_with_particles_and_modulo_50000_rotation(void) __asm__("FUN_L00_0028a208");

void draw_sky_with_gif_paging(void) __asm__("FUN_L07_00265b58");

void draw_sky_with_gif_paging(void) {
    setup_sky_gif_paging();
    if (current_level_index == 0x7) {
        draw_three_sky_shells_with_particles_and_modulo_50000_rotation();
    } else {
        update_sky_shells_static();
    }
    do_sky_gif_paging();
    vu1_add_g_sregister(0x47, 0x5360B);
    vu1_add_g_sregister(0x4E, 0x1000000 | (depth_buffer_address >> 13));
}
