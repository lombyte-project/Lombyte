#include "types.h"
#include "rnc/storage/stash.h"
#include "sifrpc.h"
extern s32 sceSifBindRpc(struct sceSifClientData *, u32, s32);
extern s32 SceSifCheckStatRpc(struct sceSifClientData *);
extern s32 sceSifCallRpc(void *, u32, u32, void *, s32, void *, s32, void (*)(void *), void *);
void init_stash(void) __asm__("FUN_00232d00");

void init_stash(void) {
    s32 info_reply[4];
    s32 slot;

    for (;;) {
        if (sceSifBindRpc(&stash_state.rpc_client, STASH_RPC_SERVER, 1) < 0) {
            for (;;) {
            }
        }
        while (SceSifCheckStatRpc(&stash_state.rpc_client) != 0) {
        }
        if (stash_state.rpc_client.serve != 0) {
            break;
        }
        for (slot = 0xFFFF; slot != -1; slot--) {
        }
    }
    sceSifCallRpc(&stash_state.rpc_client, STASH_RPC_INFO, 0, 0, 0, info_reply, 16, 0, 0);
    stash_state.iop_base = info_reply[0];
    stash_state.capacity_bytes = info_reply[1];
    stash_state.next_iop_address = info_reply[0];
    stash_state.slot_count = 0;
    for (slot = 0; slot < STASH_SLOT_COUNT; slot++) {
        stash_entries[slot].iop_address = 0;
        stash_entries[slot].reserved_qwords = 0;
    }
}

extern __typeof__(init_stash) func_00232D00 __attribute__((alias("FUN_00232d00")));
