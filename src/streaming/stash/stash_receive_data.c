#include "types.h"
#include "rnc/streaming/stash/stash.h"

extern s32 sceSifCallRpc(void *, u32, u32, void *, s32, void *, s32, void (*)(void *), void *);
s32 stash_receive_data(u8 *dst, u32 slot, s32 offset_qwords, s32 qword_count, u32 rpc_mode) __asm__("FUN_00232f20");

s32 stash_receive_data(

    u8 *receive_destination, u32 slot, s32 offset_qwords, s32 qword_count, u32 rpc_mode) {
    s32 chunk_qwords;
    s32 request[4];

    if (qword_count == -1) {
        qword_count = stash_get_reserved_qwords(slot);
    }
    if (slot >= STASH_SLOT_COUNT || stash_entries[slot].reserved_qwords == 0) {
        return -3;
    }
    if (stash_entries[slot].reserved_qwords < offset_qwords + qword_count) {
        return -1;
    }
    request[0] = stash_entries[slot].iop_address + offset_qwords * STASH_QWORD_BYTES;
    while (qword_count != 0) {
        chunk_qwords = qword_count > STASH_MAX_FETCH_QWORDS ? STASH_MAX_FETCH_QWORDS : qword_count;
        sceSifCallRpc(stash_rpc_client_address,
                      STASH_RPC_FETCH,
                      rpc_mode,
                      request,
                      STASH_QWORD_BYTES,
                      receive_destination,
                      chunk_qwords * STASH_QWORD_BYTES,
                      0,
                      0);
        qword_count -= chunk_qwords;
        receive_destination += chunk_qwords * STASH_QWORD_BYTES;
        request[0] += chunk_qwords * STASH_QWORD_BYTES;
    }
    return 0;
}

extern __typeof__(stash_receive_data) func_00232F20 __attribute__((alias("FUN_00232f20")));
