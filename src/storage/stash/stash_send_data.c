#include "types.h"
#include "rnc/storage/stash.h"

struct SifDmaData {
    u32 data;
    u32 addr;
    u32 size;
    u32 mode;
};

extern u32 sceSifSetDma(struct SifDmaData *, s32);

s32 stash_send_data(u32 source_address, s32 transfer_qwords,
                    s32 reserve_qwords, s32 tag) __asm__("FUN_00232e40");

s32 stash_send_data(u32 source_address, s32 transfer_qwords,
                    s32 reserve_qwords, s32 tag) {
    struct SifDmaData dma;
    s32 reserve_bytes;
    s32 slot_count;
    s32 slot;
    s32 iop_address;
    s32 next_iop_address;

    reserve_bytes = reserve_qwords << 4;
    if (stash_state.capacity_bytes -
            (stash_state.next_iop_address - stash_state.iop_base) < reserve_bytes) {
        return -1;
    }
    if (stash_state.slot_count == STASH_SLOT_COUNT) {
        return -2;
    }
    dma.data = source_address;
    dma.addr = stash_state.next_iop_address;
    dma.size = transfer_qwords << 4;
    dma.mode = 0;
    sceSifSetDma(&dma, 1);
    slot_count = stash_state.slot_count;
    slot = slot_count;
    iop_address = stash_state.next_iop_address;
    next_iop_address = iop_address + reserve_bytes;
    slot_count++;
    stash_state.slot_count = slot_count;
    stash_entries[slot].iop_address = iop_address;
    stash_entries[slot].reserved_qwords = reserve_qwords;
    stash_entries[slot].tag = tag;
    stash_state.next_iop_address = next_iop_address;

    return slot;
}

extern __typeof__(stash_send_data) func_00232E40 __attribute__((alias("FUN_00232e40")));
