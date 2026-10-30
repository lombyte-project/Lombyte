/* Level 05 moby class ocean_rilgar: draws the ocean grid. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/rendering/build_interpolated_dma_packet.h"
#include "rnc/overlay/shared/rendering/draw_textured_grid.h"

extern char D_L05_001671C0[];
/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_00317538), where it is exact; names translated to the US level program. */

extern f32 D_L05_00211B20[];

void draw_ocean_grid_outside_clip_volumes(struct Moby *moby) __asm__("FUN_L05_00316070");

void draw_ocean_grid_outside_clip_volumes(struct Moby *moby) {
    int *clip_volume_index_cursor = ((int *)moby->pvars);
    int  i;
    int  none = 1;
    for (i = 0; i < 8; clip_volume_index_cursor++, i++) {
        if (*clip_volume_index_cursor == -1)
            continue;
        if (is_point_inside_clip_volume(D_L05_001671C0, *clip_volume_index_cursor) != 0) {
            none = 0;
            break;
        }
    }
    if (none)
        draw_textured_grid(D_L05_00211B20);
}

void         add_draw_callback(int arg0, int arg1) __asm__("FUN_001f4600");
extern int   frame_number __asm__("D_L05_0015F5CC");
extern float D_L05_00211B20[];

void ocean_rilgar_update(struct Moby *moby) __asm__("FUN_L05_00316110");

void ocean_rilgar_update(struct Moby *moby) {
    enum {
        OCEAN_RILGAR_INITIALIZE = 0,
        OCEAN_RILGAR_UPDATE_BOBBING = 1
    };
    switch ((unsigned char)((char)moby->state)) {
    case OCEAN_RILGAR_INITIALIZE:
        prepare_scaled_table_with_byte_128(D_L05_00211B20, 1.0f);
        moby->state = 1;
        ((unsigned char *)moby)[0x30] = 0xFF;
        break;
    case OCEAN_RILGAR_UPDATE_BOBBING:
        if (hero.state.control_mode == 0x16) {
            D_L05_00211B20[2] =
                fast_sin((float)(frame_number % 360) * 0.017444444f - 3.14f) * 0.25f + 61.5f;
        } else {
            D_L05_00211B20[2] =
                fast_sin((float)(frame_number % 360) * 0.017444444f - 3.14f) * 0.25f + 59.5f;
        }
        add_draw_callback(draw_ocean_grid_outside_clip_volumes, moby);
        break;
    }
}
