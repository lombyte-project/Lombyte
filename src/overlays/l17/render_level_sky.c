/* Level 17 sky: renders the sky shells with GIF texture paging. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/overlay/shared/rendering/sky/update_sky_shells_animated.h"
#include "rnc/rendering/sky/do_sky_gif_paging.h"
#include "rnc/rendering/sky/setup_sky_gif_paging.h"
#include "rnc/overlay/shared/rendering/sky/sky_particle_effects.h"

void render_level_sky(void) __asm__("FUN_L17_00238c50");

void render_level_sky(void) {
    setup_sky_gif_paging();
    if (current_level_index == 0x11) {
        render_three_shell_particle_sky();
    } else {
        update_sky_shells_static();
    }
    do_sky_gif_paging();
    vu1_add_g_sregister(0x47, 0x5360B);
    vu1_add_g_sregister(0x4E, 0x1000000 | (depth_buffer_address >> 13));
}
