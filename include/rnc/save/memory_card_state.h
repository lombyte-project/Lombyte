#ifndef LOMBYTE_RNC_SAVE_MEMORY_CARD_STATE_H
#define LOMBYTE_RNC_SAVE_MEMORY_CARD_STATE_H

#include "types.h"

/* Card operations and their independent per-operation substeps.
 * MemoryCardState.state/pending_state use MemoryCardOperation; sub uses the
 * step enum for the active operation. The stored fields remain s32. */

enum MemoryCardOperation {
    MEMCARD_QUERY_INFO = 0x0,
    MEMCARD_PROCESS_INFO_RESULT = 0x1,
    MEMCARD_DISPATCH_PENDING_OPERATION = 0x2,
    MEMCARD_REQUEST_FORMAT = 0x3,
    MEMCARD_PROCESS_FORMAT_RESULT = 0x4,
    MEMCARD_REQUEST_UNFORMAT = 0x5,
    MEMCARD_PROCESS_UNFORMAT_RESULT = 0x6,
    MEMCARD_REQUEST_CHDIR = 0x7,
    MEMCARD_PROBE_SAVE_HEADER = 0x8,
    MEMCARD_BEGIN_CREATE_SAVE_DIRECTORY = 0x9,
    MEMCARD_CREATE_SAVE_DIRECTORY = 0xA,
    MEMCARD_BEGIN_DELETE_SAVE_DIRECTORY = 0xB,
    MEMCARD_DELETE_SAVE_DIRECTORY = 0xC,
    MEMCARD_BEGIN_READ_SAVE = 0xD,
    MEMCARD_READ_SAVE = 0xE,
    MEMCARD_BEGIN_UPDATE_SAVE = 0xF,
    MEMCARD_UPDATE_SAVE = 0x10,
    MEMCARD_REQUEST_SAVE_RESOURCE_READ = 0x11,
    MEMCARD_WAIT_SAVE_RESOURCE_READ_AND_SELECT_BUFFER = 0x12,
    MEMCARD_BEGIN_WRITE_FULL_SAVE = 0x13,
    MEMCARD_WRITE_FULL_SAVE = 0x14,
    MEMCARD_BEGIN_SCAN_SAVE_INFO = 0x15,
    MEMCARD_ADVANCE_SAVE_INFO_SCAN = 0x16,
    MEMCARD_READ_SCANNED_SAVE_INFO = 0x17
};

enum MemoryCardProbeStep {
    MEMCARD_PROBE_PROCESS_CHDIR_RESULT = 0x0,
    MEMCARD_PROBE_REQUEST_OPEN = 0x1,
    MEMCARD_PROBE_PROCESS_OPEN_RESULT = 0x2,
    MEMCARD_PROBE_REQUEST_HEADER_READ = 0x3,
    MEMCARD_PROBE_PROCESS_HEADER_READ_RESULT = 0x4,
    MEMCARD_PROBE_REQUEST_CLOSE = 0x5
};

enum MemoryCardCreateStep {
    MEMCARD_CREATE_CHECK_FREE_SPACE = 0x0,
    MEMCARD_CREATE_REQUEST_MKDIR = 0x1,
    MEMCARD_CREATE_PROCESS_MKDIR_RESULT = 0x2,
    MEMCARD_CREATE_REQUEST_RESOURCE_READ = 0x3,
    MEMCARD_CREATE_WAIT_RESOURCE_READ = 0x4,
    MEMCARD_CREATE_REQUEST_ICON_SYS_OPEN = 0x5,
    MEMCARD_CREATE_PROCESS_ICON_SYS_OPEN_RESULT_AND_REQUEST_WRITE = 0x6,
    MEMCARD_CREATE_PROCESS_ICON_SYS_WRITE_RESULT_AND_REQUEST_CLOSE = 0x7,
    MEMCARD_CREATE_PROCESS_ICON_SYS_CLOSE_RESULT = 0x8,
    MEMCARD_CREATE_REQUEST_STATIC_ICON_OPEN = 0x9,
    MEMCARD_CREATE_PROCESS_STATIC_ICON_OPEN_RESULT_AND_REQUEST_WRITE = 0xA,
    MEMCARD_CREATE_PROCESS_STATIC_ICON_WRITE_RESULT_AND_REQUEST_CLOSE = 0xB,
    MEMCARD_CREATE_PROCESS_STATIC_ICON_CLOSE_RESULT = 0xC,
    MEMCARD_CREATE_REQUEST_SAVE_OPEN = 0xD,
    MEMCARD_CREATE_PROCESS_SAVE_OPEN_RESULT_AND_REQUEST_WRITE = 0xE,
    MEMCARD_CREATE_PROCESS_SAVE_WRITE_RESULT_AND_REQUEST_CLOSE = 0xF,
    MEMCARD_CREATE_PROCESS_SAVE_CLOSE_RESULT = 0x10,
    MEMCARD_CREATE_ADVANCE_SAVE_INDEX = 0x11,
    MEMCARD_CREATE_REQUEST_AUXILIARY_FILE_OPEN = 0x12,
    MEMCARD_CREATE_PROCESS_AUXILIARY_FILE_OPEN_RESULT_AND_REQUEST_WRITE = 0x13,
    MEMCARD_CREATE_PROCESS_AUXILIARY_FILE_WRITE_RESULT_AND_REQUEST_CLOSE = 0x14,
    MEMCARD_CREATE_RESET_CARD_STATUS = 0x15
};

enum MemoryCardDeleteStep {
    MEMCARD_DELETE_REQUEST_FIRST_DIRECTORY_ENTRY = 0x0,
    MEMCARD_DELETE_PROCESS_ENTRY_AND_REQUEST_DELETE = 0x1,
    MEMCARD_DELETE_REQUEST_NEXT_DIRECTORY_ENTRY = 0x2,
    MEMCARD_DELETE_REQUEST_DIRECTORY_DELETE = 0x3,
    MEMCARD_DELETE_PROCESS_DIRECTORY_DELETE_RESULT = 0x4
};

enum MemoryCardReadStep {
    MEMCARD_READ_SAVE_REQUEST_OPEN = 0x0,
    MEMCARD_READ_SAVE_PROCESS_OPEN_RESULT = 0x1,
    MEMCARD_READ_SAVE_REQUEST_HEADER_READ = 0x2,
    MEMCARD_READ_SAVE_PROCESS_HEADER_READ_RESULT = 0x3,
    MEMCARD_READ_SAVE_REQUEST_MAIN_DATA_READ = 0x4,
    MEMCARD_READ_SAVE_PROCESS_MAIN_DATA_READ_RESULT = 0x5,
    MEMCARD_READ_SAVE_REQUEST_RECORD_READ = 0x6,
    MEMCARD_READ_SAVE_PROCESS_RECORD_READ_RESULT = 0x7,
    MEMCARD_READ_SAVE_REQUEST_CLOSE = 0x8
};

enum MemoryCardUpdateStep {
    MEMCARD_UPDATE_SAVE_REQUEST_OPEN = 0x0,
    MEMCARD_UPDATE_SAVE_PROCESS_OPEN_RESULT = 0x1,
    MEMCARD_UPDATE_SAVE_REQUEST_HEADER_SKIP = 0x2,
    MEMCARD_UPDATE_SAVE_PROCESS_HEADER_SKIP_RESULT = 0x3,
    MEMCARD_UPDATE_SAVE_REQUEST_MAIN_DATA_WRITE = 0x4,
    MEMCARD_UPDATE_SAVE_PROCESS_MAIN_DATA_WRITE_RESULT = 0x5,
    MEMCARD_UPDATE_SAVE_REQUEST_RECORD_SEEK = 0x6,
    MEMCARD_UPDATE_SAVE_PROCESS_RECORD_SEEK_RESULT = 0x7,
    MEMCARD_UPDATE_SAVE_REQUEST_RECORD_WRITE = 0x8,
    MEMCARD_UPDATE_SAVE_PROCESS_RECORD_WRITE_RESULT = 0x9,
    MEMCARD_UPDATE_SAVE_REQUEST_CLOSE = 0xA
};

enum MemoryCardWriteStep {
    MEMCARD_WRITE_FULL_SAVE_REQUEST_OPEN = 0x0,
    MEMCARD_WRITE_FULL_SAVE_PROCESS_OPEN_RESULT_AND_REQUEST_WRITE = 0x1,
    MEMCARD_WRITE_FULL_SAVE_PROCESS_WRITE_RESULT_AND_REQUEST_CLOSE = 0x2,
    MEMCARD_WRITE_FULL_SAVE_PROCESS_CLOSE_RESULT = 0x3
};

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
    s32 id;    /* 0x00: -1 = empty; the load menu reads it */
    s32 bolts; /* 0x04 */
    s32 count; /* 0x08 */
    s32 time;  /* 0x0C: play ticks */
    u8  pad_10[5];
    u8  b15; /* 0x15 */
    u8  b16; /* 0x16 */
    u8  b17; /* 0x17 */
    u8  pad_18[4];
};

struct MemoryCard {
    s32                        port;            /* 0x00 */
    s32                        slot;            /* 0x04 */
    s32                        type;            /* 0x08: sceMcGetInfo type; menus switch on it */
    s32                        free;            /* 0x0C */
    s32                        format;          /* 0x10 */
    s32                        save_index;      /* 0x14: save file number in the name; <0 = none */
    s32                        scan_save_index; /* 0x18: save 0..4 whose info states 21-23 read */
    s32                        sync_result;     /* 0x1C: sceMcSync result of a failed command */
    struct MemoryCardSaveEntry entries[5];      /* 0x20 */
    s32                        errors;          /* 0xAC: header size mismatches, -1 = unread */
    s32                        main_size;       /* 0xB0: header: main block size */
    s32                        record_size;     /* 0xB4: header: size of one record */
};

struct MemoryCardState {
    struct MemoryCard card[1];       /* 0x00 */
    s32               cmd;           /* 0xB8: sceMcSync cmd */
    s32               result;        /* 0xBC: sceMcSync result */
    s32               active_card;   /* 0xC0: card[] index of the inserted card, -1 = none */
    s32               cur;           /* 0xC4: index into card[] */
    s32               record_index;  /* 0xC8: record 0..19 being read or written */
    s32               busy;          /* 0xCC */
    s32               fd;            /* 0xD0: open file */
    s32               state;         /* 0xD4: card operation */
    s32               sub;           /* 0xD8: step within state */
    s32               pending_state; /* 0xDC: queued operation, <0 = none */
    s32               pending_card;  /* 0xE0 */
    s32               err;           /* 0xE4 */
    s32               err_card;      /* 0xE8 */
    void             *buf;           /* 0xEC: save data to write */
    s32               size;          /* 0xF0: bytes of the current read/write */
    s32               unkF4;         /* 0xF4: set 1 by prepare_save_game and the load menu */
};

extern struct MemoryCardState memory_card_state __asm__("D_0013D290");

#endif /* LOMBYTE_RNC_SAVE_MEMORY_CARD_STATE_H */
