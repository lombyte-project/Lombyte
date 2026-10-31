/* Level 07 moby class 0x3fa: ocean umbris. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/game/item_state.h"

extern void add_draw_callback(void *, void *) __asm__("FUN_001f4600");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030E820), where it is exact; names translated to the US level program. */
extern char D_L07_00166E40[];
extern void l07_moby_draw_callback(void) __asm__("FUN_L07_0030d420");
extern int  is_point_inside_clip_volume() __asm__("FUN_00214720");
extern char ocean_umbris_scaled_table[] __asm__("D_L07_00208560");
extern void prepare_scaled_table_with_byte_128(void *, float) __asm__("FUN_L02_002a40d0");
void        ocean_umbris_update(unsigned char *moby) __asm__("FUN_L07_0030d440");
extern int  is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");

/* Moby class 1018 */
void ocean_umbris_update(unsigned char *moby) {
    enum {
        OCEAN_UMBRIS_INITIALIZE = 0,
        OCEAN_UMBRIS_QUEUE_DRAW_OUTSIDE_CLIP_VOLUME = 1
    };
    switch (moby[0x20]) {
    case OCEAN_UMBRIS_INITIALIZE:
        prepare_scaled_table_with_byte_128(ocean_umbris_scaled_table, 0.66666669f);
        moby[0x20] = 1;
        moby[0x30] = 0xFF;
        break;
    case OCEAN_UMBRIS_QUEUE_DRAW_OUTSIDE_CLIP_VOLUME:
        if (!is_point_inside_clip_volume(D_L07_00166E40, *((int *)MOBY(moby)->pvars))) {
            add_draw_callback((void *)l07_moby_draw_callback, moby);
        }
        break;
    }
}
