#ifndef LOMBYTE_RNC_STREAMING_DISC_TABLE_H
#define LOMBYTE_RNC_STREAMING_DISC_TABLE_H

#include "types.h"

/* Where one file sits on the disc: first sector and length in sectors. */
struct DiscFile {
    s32 sector;
    s32 size;
};

/* One sector list of the cutscene chunk loader: start sectors, read as
   [i + 1] - [i] for the length of chunk i. */
struct DiscSectorList {
    s32 sectors[71];     /* 0x000 */
    s32 sectors_alt[77]; /* 0x11C, while D_0015ED80 is set */
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
    u8              pad_0[0x8];
    struct DiscFile debug_font;   /* 0x8 */
    struct DiscFile memcard_data; /* 0x10, read by memcard_update_state and memcard_restore_game */
    struct DiscFile animation_streams[28]; /* 0x18, streamed animations by archive index */
    struct DiscFile
        misc_animations[20]; /* 0xF8, loaded into the stash by load_misc_animations_into_stash */
    struct DiscFile vendor_animation; /* 0x198, streamed by draw_vendor_inset_quads */
    struct DiscFile music_10000[37];  /* 0x1A0, music tracks 10000.. */
    struct DiscFile
        controls_help_left[6]; /* 0x2C8: left half of the controls picture, one per language */
    struct DiscFile controls_help_right[6]; /* 0x2F8: right half */
    u8              pad_328[0x1D0];
    struct DiscFile level_chunk; /* 0x4F8, read by load_level_chunk_from_disc */
    u8              pad_500[0x3B8];
    /* 0x8B8: render archive sector/length records; extent reaches the next
       known field, not a proven count of valid archives. */
    struct DiscFile render_archives[201];
    s32             music_50000[40][6]; /* 0xF00, tracks 50000.., one location per language */
    struct DiscFile startup_archive;    /* 0x12C0 */
    struct DiscFile level_archives[4];  /* 0x12C8, by level index */
    struct DiscFile
        chunk_archives[10]; /* 0x12E8, overlays: x of an x/y pair picked by D_0015ED80 */
    struct DiscFile chunk_archives_alt[10]; /* 0x1338, the y half of chunk_archives */
    struct DiscFile loading_slides[6];      /* 0x1388, one per language */
    struct DiscFile
        space_transition_archive; /* 0x13B8: read by do_space_transition (initialize_level_runtime) */
    struct DiscFile music_40000[36]; /* 0x13C0, music tracks 40000.. */
    s32             sound_bank;      /* 0x14E0, passed to load_audio_bank_by_location */
    s32             pad_14E4;
    struct DiscFile
        transition_archive; /* 0x14E8: read by transition_do_transition (transition_load_wad) */
    u8  pad_14F0[0x8];
    struct DiscFile wad_chunks[6]; /* 0x14F8, level wad chunks */
    struct DiscFile help_text; /* 0x1528, per-language archive read by update_menu_help_text_load */
    struct DiscFile
        scene_chunk_archives[14]; /* 0x1530, overlays: x of an x/y pair picked by D_0015ED80 */
    struct DiscFile scene_chunk_archives_alt[14]; /* 0x15A0, the y half of scene_chunk_archives */
    struct DiscFile animation_table;  /* 0x1610, read with animation_asset_read_active set */
    struct DiscFile music_60000[62];  /* 0x1618, music tracks 60000.. */
    struct DiscFile scene_movies[19]; /* 0x1808, overlays: x of an x/y pair picked by D_0015ED80 */
    struct DiscFile scene_movies_alt[19]; /* 0x18A0, the y half of scene_movies */
    struct DiscFile movies[12];           /* 0x1938, level transition movies */
    struct DiscFile movies_alt[12];       /* 0x1998, picked by the video mode flag */
    struct DiscFile streams[3];           /* 0x19F8, overlays: x/y pair picked by D_0015ED80 */
    struct DiscFile streams_alt[3];       /* 0x1A10, the y half of streams */
    struct DiscFile queued_movies[5];     /* 0x1A28, overlays: x/y pair picked by D_0015ED80 */
    struct DiscFile queued_movies_alt[5]; /* 0x1A50, the y half of queued_movies */
    struct DiscFile start_movies[4];      /* 0x1A78, played by start_level */
    struct DiscFile start_movies_alt[4];  /* 0x1A98, while D_0015ED80 is set */
    u8              pad_1AB8[0xEB0];
    struct DiscFile shared_archive;    /* 0x2968 */
    struct DiscFile sound_archive;     /* 0x2970 */
    struct DiscFile sound_archive_alt; /* 0x2978, while D_0015ED80 is set */
    u8              pad_2980[0x8];
    struct DiscFile music_20000[36];       /* 0x2988, music tracks 20000.. */
    s32             music_tracks[21];      /* 0x2AA8, by track number */
    struct DiscSectorList sector_lists[1]; /* 0x2AFC, by the chunk loader's list index */
};

extern struct DiscTable disc_table __asm__("D_00137B80");

#endif /* LOMBYTE_RNC_STREAMING_DISC_TABLE_H */
