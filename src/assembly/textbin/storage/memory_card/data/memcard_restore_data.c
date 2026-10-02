#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/storage/memory_card/data/memcard_restore_data/FUN_0020af20.s", FUN_0020af20);
#else
#include "types.h"
#include "sda.h"

struct RestoreEntry {
    u8 *data;
    s32 size;
    s32 id;
    s32 status;
};

struct RestoreBlock {
    s32 id;
    s32 size;
    u8 data[1];
};

typedef struct {
    u8 pad0[0xAC];
    s32 errors; /* 0xAC */
    u8 padB0[8];
} CardSlot; /* 0xB8 */

typedef struct {
    CardSlot slot[1];
    u8 padB8[0xC];
    s32 cur; /* 0xC4 */
} CardState;

extern CardState D_0013D290;
extern s32 D_0015FE90 __attribute__((sda));
extern s32 func_0020AD38(u8 *header);
extern s32 memcmp(const void *, const void *, s32);
extern void func_001F9838(void *, void *, s32);
extern s32 GetDmaPacketSpanBytes(struct RestoreEntry *tbl);

s32 memcard_restore_data(u8 *buf, s32 slot, struct RestoreEntry *tbl) __asm__("FUN_0020af20");

s32 memcard_restore_data(u8 *buf, s32 slot, struct RestoreEntry *tbl) {
    struct RestoreEntry *e;
    s32 errors;
    s32 total;
    s32 i;
    s32 n;
    u8 *dst;

    if (func_0020AD38(buf) == 0) {
        return 1;
    }
    buf += 8;
    errors = 0;
    total = 8;
    for (i = 0; tbl[i].data != 0; i++) {
        tbl[i].status = 0;
    }
    while (((struct RestoreBlock *)buf)->id != -1) {
        i = 0;
        if (tbl[0].data == 0) {
            goto missing;
        }
        while (tbl[i].id != ((struct RestoreBlock *)buf)->id) {
            i++;
            if (tbl[i].data == 0) {
                goto missing;
            }
        }
        e = (struct RestoreEntry *)(((u32)i << 4) + (u32)tbl);
        if (e->data != 0) {
            s32 bsize = ((struct RestoreBlock *)buf)->size;

            dst = e->data + slot * e->size;
            if (e->size == bsize) {
                n = e->size;
                bsize = 1;
                e->status = bsize;
            } else if (bsize < e->size) {
                n = bsize;
                e->status = -1;
            } else {
                n = e->size;
                e->status = -2;
            }
            if (memcmp(dst, ((struct RestoreBlock *)buf)->data, n) != 0) {
                D_0015FE90++;
            }
            func_001F9838(dst, ((struct RestoreBlock *)buf)->data, n);
            total += ((n + 3) & ~3) + 8;
        } else {
        missing:
            errors++;
        }
        buf += ((((struct RestoreBlock *)buf)->size + 3) & ~3) + 8;
    }
    total += 8;
    if (total != GetDmaPacketSpanBytes(tbl)) {
        errors++;
    }
    buf += 8;
    for (i = 0; tbl[i].data != 0 && tbl[i].id != ((struct RestoreBlock *)buf)->id; i++) {
        if (tbl[i].status <= 0) {
            errors++;
        }
    }
    D_0013D290.slot[D_0013D290.cur].errors = errors;
    return errors;
}
#endif /* NON_MATCHING */
