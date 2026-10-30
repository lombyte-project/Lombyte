#include "rnc/movies/decoder/video_dec.h"

s32 video_dec_abort(struct VideoDec *vd) __asm__("func_0023CC70");

s32 video_dec_abort(struct VideoDec *vd) {
    s32 state = VD_STATE_ABORT;

    /* Fakematch: the block makes the store and the return share one register. */
    do {
        vd->state = state;
    } while (0);
    return state;
}
