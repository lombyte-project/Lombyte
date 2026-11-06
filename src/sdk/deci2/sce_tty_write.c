#include "types.h"
#include "asm.h"

#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/deci2.h"
#include "rnc/sdk/interrupts.h"

struct TtyState {
    volatile s32 socket;
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
    s8  dest;
    u32 unk8;
    u8  data[0xF4];
};

extern struct TtyState D_00154A50;
extern s32             DIntr();

s32 sceTtyWrite(s8 *text, s32 text_len) {
    s32              written_count; /* characters consumed -> return value */
    s32              remaining;     /* countdown, hits -1 after len+1 tests */
    s32              window_bytes;  /* bytes resident in the payload window */
    s8              *text_cursor;   /* source cursor */
    struct Deci2Hdr *hdr;
    u8              *payload_cursor; /* payload cursor */

    written_count = 0;
    remaining = text_len;
    window_bytes = 0;
    text_cursor = text;
    if (D_00154A50.busy == 0) {
        DIntr();
        D_00154A50.busy = 1;
        hdr = (struct Deci2Hdr *)((u32)D_00154A80 | 0x20000000);
        D_00154A50.wbuf = (s32)hdr;
        payload_cursor = hdr->data;
        do {
            remaining = remaining - 1;
            if (remaining == -1) {
                break;
            }
            if (*text_cursor == 0xA) {
                *payload_cursor = 0xD;
                window_bytes = window_bytes + 1;
                payload_cursor = payload_cursor + 1;
                if (window_bytes >= 0x100) {
                    break;
                }
            }
            *payload_cursor = (u8)*text_cursor;
            window_bytes = window_bytes + 1;
            text_cursor = text_cursor + 1;
            payload_cursor = payload_cursor + 1;
            written_count = written_count + 1;
        } while (window_bytes < 0x100);
        D_00154A50.wlen = window_bytes + 0xC;
        hdr->len = D_00154A50.wlen;
        if (CallDebugCharacter(D_00154A50.socket, hdr->dest) < 0) {
            *(s32 *)&D_00154A50.busy = 0;
            EnableInterrupts();
            return -1;
        }
        if (D_00154A50.busy != 0) {
            do {
                SceDeci2Poll(D_00154A50.socket);
            } while (D_00154A50.busy != 0);
        }
        EnableInterrupts();
        return written_count;
    }
    return -1;
}
