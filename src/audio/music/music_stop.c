#include "types.h"

#include "rnc/music_stream_state.h"

extern struct MusicStreamState D_001516D0;
extern s32 func_0012DC80();
extern s32 func_0012EBD0();
extern s32 func_0012ED30();
void music_stop(void) __asm__("FUN_00215ee8");

void music_stop(void) {
    s32 cur;

    if (D_001516D0.primary_handle == 0xFFFFFFFF) {
        do {
            func_0012DC80();
        } while (D_001516D0.primary_handle == 0xFFFFFFFF);
    }
    if (D_001516D0.transition_handle == 0xFFFFFFFF) {
        do {
            func_0012DC80();
        } while (D_001516D0.transition_handle == 0xFFFFFFFF);
    }
    if (D_001516D0.secondary_handle == 0xFFFFFFFF) {
        do {
            func_0012DC80();
        } while (D_001516D0.secondary_handle == 0xFFFFFFFF);
    }
    func_0012EBD0();
    do {

    } while (func_0012DC80() != 0);
    func_0012ED30(1);
    cur = D_001516D0.requested_track;
    D_001516D0.primary_state = 0;
    D_001516D0.primary_flags = 0;
    D_001516D0.primary_handle = 0;
    if (cur != -1) {
        D_001516D0.primary_track = cur;
    }
    D_001516D0.secondary_state = 0;
    D_001516D0.secondary_flags = 0;
    D_001516D0.secondary_handle = 0;
    D_001516D0.transition_state = 0;
    D_001516D0.transition_flags = 0;
    D_001516D0.transition_handle = 0;
    D_001516D0.crossfade_state = 0;
    D_001516D0.requested_track = -1;
    D_001516D0.requested_transition_track = -1;
}

extern __typeof__(music_stop) func_00215EE8 __attribute__((alias("FUN_00215ee8")));
