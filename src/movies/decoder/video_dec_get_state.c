#include "rnc/movies/decoder/video_dec.h"

s32 video_dec_get_state(struct VideoDec *vd) __asm__("func_0023CC80");

s32 video_dec_get_state(struct VideoDec *vd) {
    return vd->state;
}

/* Recovered original symbol name. */
extern __typeof__(video_dec_get_state) videoDecGetState __attribute__((alias("func_0023CC80")));
