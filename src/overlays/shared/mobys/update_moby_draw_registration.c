/* Shared moby class code: update_moby_draw_registration (classes 0x13d and 0x1cc). */
#include "types.h"
#include "qcopy.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/rendering/build_interpolated_dma_packet.h"

extern char draw_registration_value_target[] __asm__("D_L07_001D3400");
extern void l07_moby_draw_callback(void) __asm__("FUN_L07_0030d420");

/* Ported from rac1-decomp (src/overlays/shared/vendor_002F9438.c: func_L07_002F9438), where it is exact; names translated to the US level program. */

void update_moby_draw_registration(unsigned char *moby) __asm__("FUN_L07_002f8058");

/* Moby classes 317, 460 */
void update_moby_draw_registration(unsigned char *moby) {
    enum {
        DRAW_REGISTRATION_INITIALIZE = 0,
        DRAW_REGISTRATION_ENQUEUE_CALLBACK = 1
    };
    switch (moby[0x20]) {
    case DRAW_REGISTRATION_INITIALIZE:
        moby[0x20] = 1;
        moby[0x30] = 0xFF;
        prepare_scaled_float_table_with_target_byte(draw_registration_value_target, 0x80, 1.0f);
        break;
    case DRAW_REGISTRATION_ENQUEUE_CALLBACK:
        add_draw_callback(l07_moby_draw_callback, moby);
        break;
    }
}
