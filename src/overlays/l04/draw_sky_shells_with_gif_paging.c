/* Level 04 sky: draw_sky_shells_with_gif_paging, which sets up sky GIF paging and draws the sky shells with fast and slow rotation. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/overlay/shared/rendering/sky/update_sky_shells_animated.h"
#include "rnc/rendering/sky/do_sky_gif_paging.h"
#include "rnc/rendering/sky/setup_sky_gif_paging.h"

extern void draw_sky_shells_with_fast_and_slow_rotation(void) __asm__("FUN_L00_00289a90");

void draw_sky_shells_with_gif_paging(void) __asm__("FUN_L04_00230f48");

void draw_sky_shells_with_gif_paging(void) {
    setup_sky_gif_paging();
    if (current_level_index == 0x4) {
        draw_sky_shells_with_fast_and_slow_rotation();
    } else {
        update_sky_shells_static();
    }
    do_sky_gif_paging();
    vu1_add_g_sregister(0x47, 0x5360B);
    vu1_add_g_sregister(0x4E, 0x1000000 | (depth_buffer_address >> 13));
}
