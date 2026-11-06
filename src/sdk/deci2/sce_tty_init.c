#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/runtime/memory.h"
#include "rnc/sdk/deci2.h"

struct TtyState {
    s32          socket;
    volatile s32 wlen;
    volatile s32 rlen;
    volatile s32 busy;
    s32          wbuf;
    s32          rbuf;
    s32          queue;
};

struct Deci2Hdr {
    u16 len;
    u16 rsvd;
    u16 proto;
    u8  src;
    u8  dest;
    u32 unk8;
};

extern struct TtyState D_00154A50;
extern u8              tty_receive_packet[] __asm__("D_00154BC0");
extern s32             FlushCache();

s32 sceTtyInit(void) {
    struct TtyState *state = &D_00154A50;
    struct Deci2Hdr *hdr;

    FlushCache(0);
    *(volatile s32 *)&state->socket = sceDeci2Open(0x210, state, &sceTtyHandler);
    if (state->socket < 0) {
        return 0;
    }
    state->busy = 0;
    state->wlen = 0;
    state->rlen = 0;
    state->rbuf = (u32)tty_receive_packet | 0x20000000;
    hdr = (struct Deci2Hdr *)((u32)D_00154A80 | 0x20000000);
    state->wbuf = (s32)hdr;
    hdr->rsvd = 0;
    hdr->proto = 0x210;
    hdr->src = 'E';
    hdr->dest = 'H';
    hdr->unk8 = 0;
    state->queue = init_ring_buffer(0x100);
    return 1;
}
