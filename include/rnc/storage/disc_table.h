#ifndef LOMBYTE_RNC_STORAGE_DISC_TABLE_H
#define LOMBYTE_RNC_STORAGE_DISC_TABLE_H

#include "types.h"

/* Where one file sits on the disc: first sector and length in sectors. */
struct DiscFile {
    s32 sector;
    s32 size;
};

/*
 * The disc file table at D_00137B80, as far as the code in src/ reads it.
 * load_disc_sectors_into_global_buffer copies its first 0x2960 bytes from
 * disc sector 0x5DC. Per-language arrays are indexed by game_language.
 * Open-ended arrays are sized up to the next known field; code indexes
 * them by track or id and the sizes are not proven, except where an
 * overlay reads an x/y pair of tables (x[i] or y[i], picked by D_0015ED80,
 * passed to the movie/stream players): there the y table starts right after
 * x, so the pair is packed and both halves have the same count.
 */
struct DiscTable {
    u8 pad_0[0x8];
    struct DiscFile debug_font;            /* 0x8 */
    struct DiscFile memcard_data;          /* 0x10, read by memcard_update_state and memcard_restore_game */
    struct DiscFile animation_streams[49]; /* 0x18, streamed animations by archive index */
    struct DiscFile music_10000[37];       /* 0x1A0, music tracks 10000.. */
    struct DiscFile unk2C8[6];             /* 0x2C8, one per language */
    struct DiscFile unk2F8[6];             /* 0x2F8, one per language */
    u8 pad_328[0x1D0];
    struct DiscFile level_chunk;           /* 0x4F8, read by load_level_chunk_from_disc */
    u8 pad_500[0xA00];
    s32 music_50000[40][6];                /* 0xF00, tracks 50000.., one location per language */
    struct DiscFile unk12C0;               /* 0x12C0 */
    struct DiscFile level_archives[24];    /* 0x12C8, by level index */
    struct DiscFile loading_slides[6];     /* 0x1388, one per language */
    struct DiscFile unk13B8;               /* 0x13B8 */
    struct DiscFile music_40000[36];       /* 0x13C0, music tracks 40000.. */
    s32 sound_bank;                        /* 0x14E0, passed to load_audio_bank_by_location */
    s32 pad_14E4;
    struct DiscFile unk14E8;               /* 0x14E8 */
    u8 pad_14F0[0x8];
    struct DiscFile wad_chunks[6];         /* 0x14F8, level wad chunks */
    struct DiscFile help_text;             /* 0x1528, per-language archive read by update_menu_help_text_load */
    struct DiscFile unk1530[14];           /* 0x1530, overlays: x of an x/y pair picked by D_0015ED80 */
    struct DiscFile unk15A0[14];           /* 0x15A0, the y half of the 0x1530 pair */
    struct DiscFile animation_table;       /* 0x1610, read with animation_asset_read_active set */
    struct DiscFile music_60000[62];       /* 0x1618, music tracks 60000.. */
    struct DiscFile unk1808[19];           /* 0x1808, overlays: x of an x/y pair picked by D_0015ED80 */
    struct DiscFile unk18A0[19];           /* 0x18A0, the y half of the 0x1808 pair */
    struct DiscFile movies[12];            /* 0x1938, level transition movies */
    struct DiscFile movies_alt[12];        /* 0x1998, picked by the video mode flag */
    struct DiscFile unk19F8[3];            /* 0x19F8, overlays: x/y pair picked by D_0015ED80 */
    struct DiscFile unk1A10[3];            /* 0x1A10, the y half of the 0x19F8 pair */
    struct DiscFile unk1A28[5];            /* 0x1A28, overlays: x/y pair picked by D_0015ED80 */
    struct DiscFile unk1A50[5];            /* 0x1A50, the y half of the 0x1A28 pair */
    struct DiscFile start_movies[4];       /* 0x1A78, played by start_level */
    struct DiscFile start_movies_alt[4];   /* 0x1A98, while D_0015ED80 is set */
    u8 pad_1AB8[0xEB0];
    struct DiscFile shared_archive;        /* 0x2968 */
    struct DiscFile sound_archive;         /* 0x2970 */
    struct DiscFile sound_archive_alt;     /* 0x2978, while D_0015ED80 is set */
    u8 pad_2980[0x8];
    struct DiscFile music_20000[36];       /* 0x2988, music tracks 20000.. */
    s32 music_tracks[1];                   /* 0x2AA8, by track number */
};

extern struct DiscTable disc_table __asm__("D_00137B80");

#endif /* LOMBYTE_RNC_STORAGE_DISC_TABLE_H */
