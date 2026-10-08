#ifndef LOMBYTE_RNC_STORAGE_MEMORY_CARD_MEMORY_CARD_STATE_H
#define LOMBYTE_RNC_STORAGE_MEMORY_CARD_MEMORY_CARD_STATE_H

#include "types.h"

/* D_0013D290: the memory card driver state shared by the memcard code
   (memcard_update_state and friends) and the save/load menus. One card
   record, then the driver's command/state block. Field names follow their
   use: port/slot/type/free/format are the sceMcGetInfo/sceMcOpen arguments,
   cmd/result the sceMcSync outputs, fd the open file, state/sub the card
   operation state machine; pending_state/pending_card are copied into
   state/cur when a queued operation starts. */

/* One save entry in the card directory listing (0x1C bytes, five per card). */
struct MemoryCardSaveEntry {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    u8 unk10[8];
    s32 unk18;
};

struct MemoryCard {
    s32 port;          /* 0x00 */
    s32 slot;          /* 0x04 */
    s32 type;          /* 0x08: sceMcGetInfo type; the menus switch on it */
    s32 free;          /* 0x0C */
    s32 format;        /* 0x10 */
    s32 save_index;    /* 0x14: save file number (sprintf'd into the name); <0 = none */
    s32 unk18;         /* 0x18 */
    s32 sync_result;   /* 0x1C: sceMcSync result kept on a failed command; menus test != 0 */
    struct MemoryCardSaveEntry entries[5]; /* 0x20 */
    s32 errors;        /* 0xAC: header size mismatches, -1 = unread */
    s32 unkB0;         /* 0xB0 */
    s32 unkB4;         /* 0xB4 */
};

struct MemoryCardState {
    struct MemoryCard card[1]; /* 0x00 */
    s32 cmd;           /* 0xB8 */
    s32 result;        /* 0xBC */
    s32 active_card;   /* 0xC0: card[] index of the inserted card, -1 = none */
    s32 cur;           /* 0xC4: index into card[] */
    s32 unkC8;         /* 0xC8 */
    s32 busy;          /* 0xCC */
    s32 fd;            /* 0xD0 */
    s32 state;         /* 0xD4: card operation state */
    s32 sub;           /* 0xD8 */
    s32 pending_state; /* 0xDC: queued operation, <0 = none */
    s32 pending_card;  /* 0xE0 */
    s32 err;           /* 0xE4 */
    s32 err_card;      /* 0xE8 */
    void *buf;         /* 0xEC */
    s32 size;          /* 0xF0 */
    s32 unkF4;         /* 0xF4 */
};

extern struct MemoryCardState memory_card_state __asm__("D_0013D290");

/* gcc 2.95 has no _Static_assert: a negative array size fails the build. */
#define MEMORY_CARD_OFFSET_CHECK(name, type, field, off) \
    typedef char memory_card_offset_check_##name[ \
        ((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]
MEMORY_CARD_OFFSET_CHECK(entry_unk18, struct MemoryCardSaveEntry, unk18, 0x18);
MEMORY_CARD_OFFSET_CHECK(card_save_index, struct MemoryCard, save_index, 0x14);
MEMORY_CARD_OFFSET_CHECK(card_sync_result, struct MemoryCard, sync_result, 0x1C);
MEMORY_CARD_OFFSET_CHECK(card_entries, struct MemoryCard, entries, 0x20);
MEMORY_CARD_OFFSET_CHECK(card_errors, struct MemoryCard, errors, 0xAC);
MEMORY_CARD_OFFSET_CHECK(card_unkB4, struct MemoryCard, unkB4, 0xB4);
MEMORY_CARD_OFFSET_CHECK(cmd, struct MemoryCardState, cmd, 0xB8);
MEMORY_CARD_OFFSET_CHECK(active_card, struct MemoryCardState, active_card, 0xC0);
MEMORY_CARD_OFFSET_CHECK(cur, struct MemoryCardState, cur, 0xC4);
MEMORY_CARD_OFFSET_CHECK(state, struct MemoryCardState, state, 0xD4);
MEMORY_CARD_OFFSET_CHECK(pending_state, struct MemoryCardState, pending_state, 0xDC);
MEMORY_CARD_OFFSET_CHECK(pending_card, struct MemoryCardState, pending_card, 0xE0);
MEMORY_CARD_OFFSET_CHECK(err, struct MemoryCardState, err, 0xE4);
MEMORY_CARD_OFFSET_CHECK(buf, struct MemoryCardState, buf, 0xEC);
MEMORY_CARD_OFFSET_CHECK(unkF4, struct MemoryCardState, unkF4, 0xF4);

#endif /* LOMBYTE_RNC_STORAGE_MEMORY_CARD_MEMORY_CARD_STATE_H */
