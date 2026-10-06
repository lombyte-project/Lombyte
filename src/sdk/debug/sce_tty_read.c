#include "types.h"
typedef struct {
    u8 pad_0[4];
    volatile s32 avail;
} TtyQueue;
typedef struct {
    u8 pad_0[8];
    u8 *data;
} TtyRcv;
typedef struct {
    u8 pad_0[0x18];
    TtyRcv *rcv;
} TtyState;
extern TtyQueue *D_00154A68[];
extern TtyState D_00154A50;
extern void QueuePeekReadDone();
s32 sceTtyRead(s8 *buf, s32 size) {
    s32 i;

    for (i = 0; i < size; i++) {
        while (D_00154A68[0]->avail == 0) {
        }
        buf[i] = *D_00154A50.rcv->data;
        QueuePeekReadDone(D_00154A50.rcv);
        if (buf[i] == 0xA || buf[i] == 0xD) {
            return i + 1;
        }
    }
    return i;
}
