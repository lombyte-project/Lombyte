#include "rnc/movies/decoder/video_dec.h"

void video_dec_reset(struct VideoDec *vd) __asm__("func_0023CC30");

void video_dec_reset(struct VideoDec *vd) {
    vd->state = VD_STATE_NORMAL;
}

/* Recovered original symbol name. */
extern __typeof__(video_dec_reset) videoDecReset__FP8VideoDec
    __attribute__((alias("func_0023CC30")));
