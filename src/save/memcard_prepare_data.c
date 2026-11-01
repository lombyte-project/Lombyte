#include "types.h"
#include "rnc/math/asm_helpers.h"

struct SaveBlock {
    u8 *base;
    s32 size;
    s32 id;
    s32 pad;
};

/* Each block's data follows its own id and size; id -1 ends the list. */
struct SaveBlockHeader {
    s32 id;
    s32 size;
};

struct SaveHeader {
    s32 size;
    s32 checksum;
};

extern s32 calculate_crc16(void *, s32) __asm__("func_0020ACC0");

s32 memcard_prepare_data(struct SaveHeader *out,
                         s32                slot,
                         struct SaveBlock  *blk) __asm__("FUN_0020ad78");

s32 memcard_prepare_data(struct SaveHeader *save_header, s32 slot, struct SaveBlock *blk) {
    u8 *write_cursor;
    s32 total;
    u8 *slot_payload_source;

    total = 0;
    write_cursor = (u8 *)(save_header + 1);
    if (blk->base != 0) {
        struct SaveBlock *block = blk;
        s32               mask = -4;

        do {
            slot_payload_source = block->base + slot * block->size;
            total += 8;
            ((struct SaveBlockHeader *)write_cursor)->id = block->id;
            ((struct SaveBlockHeader *)write_cursor)->size = block->size;
            write_cursor += sizeof(struct SaveBlockHeader);
            copy_memory_aligned(write_cursor, slot_payload_source, block->size);
            write_cursor += block->size;
            total += block->size;
            write_cursor = (u8 *)(((s32)write_cursor + 3) & mask);
            total = (total + 3) & mask;
            block++;
        } while (block->base != 0);
    }
    total += 8;
    ((struct SaveBlockHeader *)write_cursor)->size = 0;
    ((struct SaveBlockHeader *)write_cursor)->id = -1;
    save_header->checksum = calculate_crc16(save_header + 1, total);
    save_header->size = total;
    return total + 8;
}

extern __typeof__(memcard_prepare_data) func_0020AD78 __attribute__((alias("FUN_0020ad78")));
