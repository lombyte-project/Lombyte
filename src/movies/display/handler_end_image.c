#include "types.h"
#include "kernel.h"

#include "rnc/movies/display/handler_end_image_globals.h"
#include "rnc/movies/decoder/decoder_globals.h"
#include "rnc/movies/player/movie_player.h"

s32 handler_end_image(void) __asm__("FUN_0023b540");

s32 handler_end_image(void) {
    if (image_buffer_count_decrement_pending != 0) {
        vo_buf_dec_count(&movie_player->vo_buf);
        *(s32 *)0x1611E8 = 0;
    }
    CpuSync();
    CpuEnableInt();
    return 0;
}
