#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/video/decoder/decoder_globals.h"

extern volatile s32 D_001611E4;
extern s32 sceGsSyncV();

void start_display(s32 arg0) __asm__("FUN_0023b590");

void start_display(s32 vblank_field_to_skip) {
    do {

    } while (sceGsSyncV(0) == vblank_field_to_skip);
    movie_display_active = 1;
    D_001611E4 = 0;
}

extern __typeof__(start_display) func_0023B590 __attribute__((alias("FUN_0023b590")));
