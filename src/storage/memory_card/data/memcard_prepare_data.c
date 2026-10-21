#include "types.h"

struct SaveBlock {
    u8 *base;
    s32 size;
    s32 id;
    s32 pad;
};

struct SaveHeader {
    s32 size;
    s32 checksum;
};

extern void copy_memory_aligned(void *, void *, s32) __asm__("FUN_001f9838");
extern s32 calculate_crc16(void *, s32) __asm__("func_0020ACC0");

s32 memcard_prepare_data(struct SaveHeader *out, s32 slot,
                         struct SaveBlock *blk) __asm__("FUN_0020ad78");

s32 memcard_prepare_data(struct SaveHeader *out, s32 slot, struct SaveBlock *blk) {
    u8 *write_cursor;
    s32 total;
    u8 *slot_payload_source;

    total = 0;
    write_cursor = (u8 *)(out + 1);
    if (blk->base != 0) {
        struct SaveBlock *block = blk;
        s32 mask = -4;

        do {
            slot_payload_source = block->base + slot * block->size;
            total += 8;
            ((s32 *)write_cursor)[0] = block->id;
            ((s32 *)write_cursor)[1] = block->size;
            write_cursor += 8;
            copy_memory_aligned(write_cursor, slot_payload_source, block->size);
            write_cursor += block->size;
            total += block->size;
            write_cursor = (u8 *)(((s32)write_cursor + 3) & mask);
            total = (total + 3) & mask;
            block++;
        } while (block->base != 0);
    }
    total += 8;
    ((s32 *)write_cursor)[1] = 0;
    ((s32 *)write_cursor)[0] = -1;
    out->checksum = calculate_crc16(out + 1, total);
    out->size = total;
    return total + 8;
}

extern __typeof__(memcard_prepare_data) func_0020AD78 __attribute__((alias("FUN_0020ad78")));
