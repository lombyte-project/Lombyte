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
struct Moby {
    u8 pad0[0x24];
    struct MobyClass *cls;
    u8 pad28[0x28];
    u8 frame;
    u8 next;
    u8 seq;
    u8 prev_seq;
    u8 pad54[0x14];
    void *cur;
    void *nxt;
    u8 pad70[0xC];
    u8 unk7C;
    u8 pad7d;
    u8 unk7E;
};
extern u8 D_001AABC0[];
void update_moby_animation_state(struct Moby *m) __asm__("FUN_0020c880");

void update_moby_animation_state(struct Moby *m) {
    if (m->seq != 0xFF) {
        m->cur = m->cls->seqs[m->seq]->frames[m->frame];
        m->unk7E = m->cls->seqs[m->seq]->unk12;
        m->unk7C = m->cls->seqs[m->seq]->unk11;
    } else {
        m->unk7C = 0xFF;
        m->unk7E = 0;
        m->cur = D_001AABC0 + m->frame * 0x800;
    }
    m->nxt = m->cls->seqs[m->prev_seq]->frames[m->next];
}

extern __typeof__(update_moby_animation_state) func_0020C880 __attribute__((alias("FUN_0020c880")));
