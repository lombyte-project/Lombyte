#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/ui/text/font_state.h"

struct AnimSeq {
    u8    pad0[0x11];
    u8    sound;
    u8    unk12;
    u8    pad13[9];
    void *frames[1];
};

#include "rnc/mobys/moby.h"

void update_moby_animation_state(struct Moby *m) __asm__("FUN_0020c880");

void update_moby_animation_state(struct Moby *moby) {
    if (moby->seq != 0xFF) {
        moby->cur_frame_data = moby->pclass->seqs[moby->seq]->frames[moby->frame];
        moby->anim_trigger_count = moby->pclass->seqs[moby->seq]->unk12;
        moby->seq_sound = moby->pclass->seqs[moby->seq]->sound;
    } else {
        moby->seq_sound = 0xFF;
        moby->anim_trigger_count = 0;
        moby->cur_frame_data = debug_font_buffer + moby->frame * 0x800;
    }
    moby->prev_frame_data = moby->pclass->seqs[moby->prev_seq]->frames[moby->prev_frame];
}

extern __typeof__(update_moby_animation_state) func_0020C880 __attribute__((alias("FUN_0020c880")));
