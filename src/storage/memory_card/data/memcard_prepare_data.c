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

extern void FUN_001f9838(void *, void *, s32);
extern s32 func_0020ACC0(void *, s32);

s32 memcard_prepare_data(struct SaveHeader *out, s32 slot, struct SaveBlock *blk) __asm__("FUN_0020ad78");


s32 memcard_prepare_data(struct SaveHeader *out, s32 slot, struct SaveBlock *blk) {
    u8 *p;
    s32 total;
    u8 *src;

    total = 0;
    p = (u8 *)(out + 1);
    if (blk->base != 0) {
        struct SaveBlock *block = blk;
        s32 mask = -4;

        do {
            src = block->base + slot * block->size;
            total += 8;
            ((s32 *)p)[0] = block->id;
            ((s32 *)p)[1] = block->size;
            p += 8;
            FUN_001f9838(p, src, block->size);
            p += block->size;
            total += block->size;
            p = (u8 *)(((s32)p + 3) & mask);
            total = (total + 3) & mask;
            block++;
        } while (block->base != 0);
    }
    total += 8;
    ((s32 *)p)[1] = 0;
    ((s32 *)p)[0] = -1;
    out->checksum = func_0020ACC0(out + 1, total);
    out->size = total;
    return total + 8;
}

extern __typeof__(memcard_prepare_data) func_0020AD78 __attribute__((alias("FUN_0020ad78")));
