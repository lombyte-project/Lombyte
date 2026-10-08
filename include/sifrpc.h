#ifndef LOMBYTE_SIFRPC_H
#define LOMBYTE_SIFRPC_H

#include "types.h"

/*
 * SIF RPC client state, laid out as in the PS2 SDK <sifrpc.h>
 * (sceSifRpcData / sceSifClientData). The tags are spelled `struct sce...`
 * here because the code refers to them that way. Size 0x28.
 */
struct sceSifRpcData {
    void *paddr;              /* 0x0: packet in flight; released by _request_end */
    u32 pid;                  /* 0x4 */
    s32 tid;                  /* 0x8: semaphore signalled by _request_end when >= 0 */
    u32 mode;                 /* 0xC */
};

struct sceSifClientData {
    struct sceSifRpcData rpcd; /* 0x0 */
    u32 command;              /* 0x10 */
    void *buff;               /* 0x14 */
    void *cbuff;              /* 0x18 */
    void (*func)(void *);     /* 0x1C: end function, called with para when the call completes */
    void *para;               /* 0x20 */
    void *serve;              /* 0x24: server bound by sceSifBindRpc; 0 until bound */
};

#endif /* LOMBYTE_SIFRPC_H */
