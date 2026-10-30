/* Level 14 moby class audio_stream_callback_keeper (0x4ec): prepares its scaled float table once, then every frame queues the draw callback that re-registers the audio stream CD callback. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"

#include "qcopy.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/audio/streaming/register_audio_stream_callback.h"
#include "rnc/overlay/shared/rendering/build_interpolated_dma_packet.h"

extern unsigned char l14_scaled_table_setup_target[] __asm__("D_L14_001E9C60");

void update_audio_stream_callback_keeper(struct Moby *moby) __asm__("FUN_L14_00303370");

void update_audio_stream_callback_keeper(struct Moby *moby) {
    enum {
        MOBY_STATE_INITIALIZE = 0,
        MOBY_STATE_REGISTER_DRAW_CALLBACK = 1
    };
    switch (moby->state) {
    case MOBY_STATE_INITIALIZE:
        prepare_scaled_float_table_with_target_byte(
            l14_scaled_table_setup_target, 0x40, 0.6666667f);
        moby->state = 1;
        moby->update_dist = 0xFF;
        break;
    case MOBY_STATE_REGISTER_DRAW_CALLBACK:
        add_draw_callback(register_audio_stream_callback, moby);
        break;
    }
}
