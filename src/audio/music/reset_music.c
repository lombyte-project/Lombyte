#include "types.h"

#include "rnc/music_stream_state.h"

extern struct MusicStreamState D_001516D0;
extern s32 func_0012DC80();
extern s32 func_0012EB20();
extern s32 func_00215420();
void reset_music(void) __asm__("FUN_00215390");

void reset_music(void) {
    s32 neg = -1;

    D_001516D0.unk30 = 0x20;
    D_001516D0.unk0 = 0;
    D_001516D0.unk31 = 0;
    D_001516D0.unk32 = 0;
    D_001516D0.unk33 = 0;
    D_001516D0.primary_handle = 0;
    D_001516D0.primary_state = 0;
    D_001516D0.secondary_handle = 0;
    D_001516D0.secondary_state = 0;
    D_001516D0.transition_handle = 0;
    D_001516D0.transition_state = 0;
    D_001516D0.queued_secondary_track = neg;
    D_001516D0.requested_track = neg;
    func_0012EB20(4, 0xF000, 0, 1);
    while (func_0012DC80() != 0) {
    }
    func_00215420();
}
