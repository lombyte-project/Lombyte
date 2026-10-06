#include "types.h"
struct AnimSeq {
    u8 pad0[0x11];
    u8 unk11;
    u8 unk12;
    u8 pad13[9];
    void *frames[1];
};
struct MobyClass {
    u8 pad0[0x48];
    struct AnimSeq *seqs[1];
};
#include "rnc/gameplay/entities/moby.h"
extern u8 D_001AABC0[];
void update_moby_animation_state(struct Moby *m) __asm__("FUN_0020c880");

void update_moby_animation_state(struct Moby *m) {
    if (m->seq != 0xFF) {
        m->cur_frame_data = m->pclass->seqs[m->seq]->frames[m->frame];
        m->unk7E = m->pclass->seqs[m->seq]->unk12;
        m->unk7C = m->pclass->seqs[m->seq]->unk11;
    } else {
        m->unk7C = 0xFF;
        m->unk7E = 0;
        m->cur_frame_data = D_001AABC0 + m->frame * 0x800;
    }
    m->prev_frame_data = m->pclass->seqs[m->prev_seq]->frames[m->prev_frame];
}

extern __typeof__(update_moby_animation_state) func_0020C880 __attribute__((alias("FUN_0020c880")));
