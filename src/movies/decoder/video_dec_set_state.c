#include "rnc/movies/decoder/video_dec.h"

/* Returns the previous state. */
s32 video_dec_set_state(struct VideoDec *vd, s32 state) __asm__("func_0023CC88");

s32 video_dec_set_state(struct VideoDec *vd, s32 state) {
    s32 previous = vd->state;
    vd->state = state;
    return previous;
}
