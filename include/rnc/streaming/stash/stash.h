#ifndef LOMBYTE_RNC_STREAMING_STASH_STASH_H
#define LOMBYTE_RNC_STREAMING_STASH_STASH_H

#include "types.h"
#include "sifrpc.h"

#define STASH_SLOT_COUNT       64
#define STASH_QWORD_BYTES      16
#define STASH_MAX_FETCH_QWORDS 0xFFFF
#define STASH_RPC_SERVER       0x11
#define STASH_RPC_FETCH        1
#define STASH_RPC_INFO         2

/* EE bookkeeping for the IOP buffer returned by STASH_RPC_INFO. */
struct StashState {
    s32                     iop_base;
    s32                     capacity_bytes;
    struct sceSifClientData rpc_client;
    s32                     next_iop_address;
    s32                     slot_count;
};

/* A reserved range; the DMA may populate fewer qwords than are reserved. */
struct StashEntry {
    s32 iop_address;
    s32 reserved_qwords;
    s32 tag;
    s32 pad_0C;
};

extern struct StashState stash_state __asm__("D_001DD1A0");
extern struct StashEntry stash_entries[STASH_SLOT_COUNT] __asm__("D_001DD1D8")
    __attribute__((section(".data")));
/* Same client as stash_state.rpc_client; retain the fetch unit's direct label. */
extern u8 stash_rpc_client_address[] __asm__("D_001DD1A8");

s32 stash_get_reserved_qwords(s32 slot) __asm__("LookupResourceEntry");

#endif /* LOMBYTE_RNC_STREAMING_STASH_STASH_H */
