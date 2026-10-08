#ifndef LOMBYTE_RNC_STORAGE_DISC_TABLE_H
#define LOMBYTE_RNC_STORAGE_DISC_TABLE_H

#include "types.h"

/* Where one file sits on the disc: first sector and length. */
struct DiscFile {
    s32 sector;
    s32 size;
};

/*
 * The disc file table at D_00137B80, as far as the code in src/ reads it.
 * The per-language arrays are indexed by game_language.
 */
struct DiscTable {
    u8 pad_0[0x8];
    struct DiscFile debug_font;   /* 0x8 */
    u8 pad_10[0x2B8];
    struct DiscFile unk2C8[6];    /* 0x2C8, one per language */
    struct DiscFile unk2F8[6];    /* 0x2F8, one per language */
    u8 pad_328[0xF98];
    struct DiscFile unk12C0;      /* 0x12C0 */
    u8 pad_12C8[0x260];
    struct DiscFile unk1528;      /* 0x1528 */
};

extern struct DiscTable disc_table __asm__("D_00137B80");

#endif /* LOMBYTE_RNC_STORAGE_DISC_TABLE_H */
