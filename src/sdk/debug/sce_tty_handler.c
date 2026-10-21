#include "types.h"

typedef struct {
    u8 pad0[0xC];
    u8 *wp;
} TtyQueue;

typedef struct {
    u16 len;
} Deci2Hdr;

typedef struct {
    s32 socket;
    volatile s32 wlen;
    volatile s32 rlen;
    volatile s32 busy;
    u8 *wbuf;
    u8 *rbuf;
    TtyQueue *queue;
} TtyState;

extern u8 D_00152710[];
extern u8 D_00152738[];
extern u8 D_00152750[];
extern u8 D_00152768[];
extern void QueuePeekWriteDone(TtyQueue *queue);
extern s32 SceDeci2ExRecv(s32 socket, void *buffer, u16 byte_count);
extern s32 SceDeci2ExSend(s32 socket, void *buffer, u16 byte_count);
extern int kprintf(const char *format, ...);

void sceTtyHandler(s32 event, s32 param, TtyState *tty) {
    enum {
        TTY_SEND_PENDING_BYTES = 3,
        TTY_CLEAR_SEND_BUSY = 4
    };
    Deci2Hdr *hdr;
    s32 send_result;
    s32 receive_append_offset;

    switch (event) {
    case 1:
    case 2:
        if (param != 0) {
            if ((u32)(tty->rlen + param) > 0x140) {
                kprintf(D_00152710);
            }
            receive_append_offset = tty->rlen;
            param = SceDeci2ExRecv(tty->socket, tty->rbuf + receive_append_offset, param);
            if (param < 0) {
                kprintf(D_00152738);
            }
            tty->rlen += param;
        } else {
            hdr = (Deci2Hdr *)tty->rbuf;
            for (param = 12; param < hdr->len; param++) {
                *tty->queue->wp = tty->rbuf[param];
                QueuePeekWriteDone(tty->queue);
            }
            tty->rlen = 0;
        }
        break;
    case TTY_SEND_PENDING_BYTES:
        send_result = SceDeci2ExSend(tty->socket, tty->wbuf, tty->wlen);
        if (send_result < 0) {
            kprintf(D_00152750, send_result);
            tty->busy = 0;
        } else {
            tty->wbuf += send_result;
            tty->wlen -= send_result;
        }
        break;
    case TTY_CLEAR_SEND_BUSY:
        if (tty->wlen != 0) {
            kprintf(D_00152768, tty->wlen);
        }
        tty->busy = 0;
        break;
    }
}
