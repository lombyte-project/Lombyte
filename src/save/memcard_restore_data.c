#include "types.h"
#include "asm.h"

#include "types.h"
#include "sda.h"
#include "rnc/save/memcard_get_data_size.h"

struct RestoreEntry {
    u8 *data;
    s32 size;
    s32 id;
    s32 status;
};

struct RestoreBlock {
    s32 id;
    s32 size;
    u8  data[1];
};

typedef struct {
    u8  pad0[0xAC];
    s32 errors; /* 0xAC */
    u8  padB0[8];
} CardSlot; /* 0xB8 */

typedef struct {
    CardSlot slot[1];
    u8       padB8[0xC];
    s32      cur; /* 0xC4 */
} CardState;

extern CardState D_0013D290;

#include "rnc/save/memcard_restore_data_globals.h"

extern s32  memcard_restore_differing_block_count_gp __asm__("D_0015FE90") __attribute__((sda));
extern s32  validate_data_crc(u8 *header) __asm__("func_0020AD38");
extern s32  memcmp(const void *, const void *, s32);
extern void copy_memory_aligned(void *, void *, s32) __asm__("func_001F9838");

s32 memcard_restore_data(u8 *buf, s32 slot, struct RestoreEntry *tbl) __asm__("FUN_0020af20");

s32 memcard_restore_data(u8 *save_packet, s32 slot, struct RestoreEntry *restore_entries) {
    struct RestoreEntry *matched_entry;
    s32                  errors;
    s32                  total;
    s32                  i;
    s32                  copySize;
    u8                  *slot_destination;

    if (validate_data_crc(save_packet) == 0) {
        return 1;
    }
    save_packet += 8;
    errors = 0;
    total = 8;
    for (i = 0; restore_entries[i].data != 0; i++) {
        restore_entries[i].status = 0;
    }
    while (((struct RestoreBlock *)save_packet)->id != -1) {
        i = 0;
        if (restore_entries[0].data == 0) {
            goto missing;
        }
        while (restore_entries[i].id != ((struct RestoreBlock *)save_packet)->id) {
            i++;
            if (restore_entries[i].data == 0) {
                goto missing;
            }
        }
        matched_entry = (struct RestoreEntry *)(((u32)i << 4) + (u32)restore_entries);
        if (matched_entry->data != 0) {
            s32 blockSize = ((struct RestoreBlock *)save_packet)->size;
            s32 entrySize = matched_entry->size;
            slot_destination = matched_entry->data + slot * entrySize;
            if (blockSize == entrySize) {
                s32 status = 1;
                copySize = entrySize;
                matched_entry->status = status;
            } else if (blockSize < entrySize) {
                copySize = blockSize;
                matched_entry->status = -1;
            } else {
                copySize = entrySize;
                matched_entry->status = -2;
            }
            if (memcmp(slot_destination, ((struct RestoreBlock *)save_packet)->data, copySize) !=
                0) {
                memcard_restore_differing_block_count_gp++;
            }
            copy_memory_aligned(
                slot_destination, ((struct RestoreBlock *)save_packet)->data, copySize);
            total += ((copySize + 3) & ~3) + 8;
        } else {
        missing:
            errors++;
        }
        save_packet += ((((struct RestoreBlock *)save_packet)->size + 3) & ~3) + 8;
    }
    total += 8;
    if (total != memcard_get_data_size(restore_entries)) {
        errors++;
    }
    save_packet += 8;
    for (i = 0; restore_entries[i].data != 0 &&
                restore_entries[i].id != ((struct RestoreBlock *)save_packet)->id;
         i++) {
        if (restore_entries[i].status <= 0) {
            errors++;
        }
    }
    D_0013D290.slot[D_0013D290.cur].errors = errors;
    return errors;
}

extern __typeof__(memcard_restore_data) func_0020AF20 __attribute__((alias("FUN_0020af20")));
