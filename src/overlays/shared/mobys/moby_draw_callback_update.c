/* Shared moby class 0x3E2: prepares its scaled table, then queues the audio stream draw callback each frame. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/audio/streaming/register_audio_stream_callback.h"
#include "qcopy.h"
#include "rnc/overlay/shared/rendering/build_interpolated_dma_packet.h"

typedef struct Moby Moby;

extern void          add_draw_callback(void *, void *) __asm__("FUN_001f4600");
extern unsigned char l03_draw_callback_setup_byte_target[] __asm__("D_L03_001DC1A0");

void moby_draw_callback_update(struct Moby *moby) __asm__("FUN_L03_002dc9b0");

/* Moby class 994 */
void moby_draw_callback_update(struct Moby *moby) {
    enum {
        DRAW_CALLBACK_INITIALIZE = 0,
        DRAW_CALLBACK_REGISTER = 1
    };
    switch (moby->state) {
    case DRAW_CALLBACK_INITIALIZE:
        prepare_scaled_float_table_with_target_byte(
            l03_draw_callback_setup_byte_target, 0x20, 0.6666667f);
        moby->state = 1;
        moby->update_dist = 0xFF;
        break;
    case DRAW_CALLBACK_REGISTER:
        add_draw_callback(register_audio_stream_callback, moby);
        break;
    }
}
