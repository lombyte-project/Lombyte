#ifndef LOMBYTE_RNC_STORAGE_MEMORY_CARD_MEMORY_CARD_STATE_H
#define LOMBYTE_RNC_STORAGE_MEMORY_CARD_MEMORY_CARD_STATE_H

#include "types.h"

/*
 * D_0013D290: memory card driver state, shared by memcard_update_state and
 * the save/load menus. port/slot/type/free/format are the sceMcGetInfo and
 * sceMcOpen arguments, cmd/result the sceMcSync outputs, state/sub the card
 * operation state machine. A save file holds a main block (read into
 * D_0014EED0, at most 0x1800 bytes) and 20 records (read into D_001506D0,
 * at most 0x1000 bytes each); the file header stores both sizes.
 * Offset checks: memory_card_layout_check.c.
 */

/* One save entry in the card directory listing (0x1C bytes, five per card). */
struct MemoryCardSaveEntry {
    s32 id;                    /* 0x00: -1 = empty; the load menu reads it */
    s32 bolts;                 /* 0x04 */
    s32 count;                 /* 0x08 */
    s32 time;                  /* 0x0C: play ticks */
    u8 pad_10[5];
    u8 b15;                    /* 0x15 */
    u8 b16;                    /* 0x16 */
    u8 b17;                    /* 0x17 */
    u8 pad_18[4];
};

struct MemoryCard {
    s32 port;                  /* 0x00 */
    s32 slot;                  /* 0x04 */
    s32 type;                  /* 0x08: sceMcGetInfo type; menus switch on it */
    s32 free;                  /* 0x0C */
    s32 format;                /* 0x10 */
    s32 save_index;            /* 0x14: save file number in the name; <0 = none */
    s32 scan_save_index;       /* 0x18: save 0..4 whose info states 21-23 read */
    s32 sync_result;           /* 0x1C: sceMcSync result of a failed command */
    struct MemoryCardSaveEntry entries[5]; /* 0x20 */
    s32 errors;                /* 0xAC: header size mismatches, -1 = unread */
    s32 main_size;             /* 0xB0: header: main block size */
    s32 record_size;           /* 0xB4: header: size of one record */
};

struct MemoryCardState {
    struct MemoryCard card[1]; /* 0x00 */
    s32 cmd;                   /* 0xB8: sceMcSync cmd */
    s32 result;                /* 0xBC: sceMcSync result */
    s32 active_card;           /* 0xC0: card[] index of the inserted card, -1 = none */
    s32 cur;                   /* 0xC4: index into card[] */
    s32 record_index;          /* 0xC8: record 0..19 being read or written */
    s32 busy;                  /* 0xCC */
    s32 fd;                    /* 0xD0: open file */
    s32 state;                 /* 0xD4: card operation */
    s32 sub;                   /* 0xD8: step within state */
    s32 pending_state;         /* 0xDC: queued operation, <0 = none */
    s32 pending_card;          /* 0xE0 */
    s32 err;                   /* 0xE4 */
    s32 err_card;              /* 0xE8 */
    void *buf;                 /* 0xEC: save data to write */
    s32 size;                  /* 0xF0: bytes of the current read/write */
    s32 unkF4;                 /* 0xF4: set 1 by prepare_save_game and the load menu */
};

extern struct MemoryCardState memory_card_state __asm__("D_0013D290");

#endif /* LOMBYTE_RNC_STORAGE_MEMORY_CARD_MEMORY_CARD_STATE_H */
