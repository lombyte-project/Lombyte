#ifndef LOMBYTE_RNC_AUDIO_MUSIC_MUSIC_STREAM_STATE_H
#define LOMBYTE_RNC_AUDIO_MUSIC_MUSIC_STREAM_STATE_H

#include "types.h"

/*
 * Music streaming state at D_001516D0 (snd/music units). Widths follow the
 * retail loads (majority sign where lh/lhu both occur).
 *
 * Three channel records: primary, secondary and transition. The address of
 * a record's `handle` is the context of the stream callbacks
 * (music_primary_preseek_callback, music_secondary_start_callback,
 * music_remaining_time_callback) and of music_update_stream.
 * Offset checks: music_stream_layout_check.c.
 */

/* One music channel record (0x1C bytes). */
struct MusicStreamChannel {
    s32 handle;            /* 0x00: stream handle from the start callback; -1 while starting */
    s16 track;             /* 0x04 */
    s16 volume;            /* 0x06 */
    s16 flags;             /* 0x08 */
    s16 state;             /* 0x0A: set 1 on start, 1 -> 2 in the start callback */
    s16 fade_flags;        /* 0x0C: -0x8000 on music_pause, 4 on music_unpause */
    s16 unkE;              /* 0x0E: cleared by music_pause */
    s16 crossfade_enabled; /* 0x10: remaining-time callback starts the crossfade */
    u8 pad_12[0x2];
    s32 poll_interval;     /* 0x14 */
    s32 remaining_time;    /* 0x18: set by music_remaining_time_callback */
};

/* sceCdRead read mode (the SDK's sceCdRMode). */
struct MusicCdReadMode {
    u8 trycount;           /* 0x20 from reset_music */
    u8 spindlctrl;
    u8 datapattern;
    u8 pad;
};

struct MusicStreamState {
    s32 unk0;                          /* 0x00: cleared by reset_music */
    u8 pad_4[0x4];
    s16 read_state;                    /* 0x08: 0 idle, 1 reading, 2 done */
    u8 break_requested;                /* 0x0A: set by request_audio_stream_break */
    u8 updates_suspended;              /* 0x0B: set around space transitions */
    s32 read_sector;                   /* 0x0C */
    s32 read_sector_count;             /* 0x10 */
    s32 read_dst;                      /* 0x14 */
    u8 pad_18[0x4];
    s32 queued_secondary_track;        /* 0x1C: -1 = none; overlays queue track + 40000 (l13 lines: + 50000) and wait for secondary.state == 3 */
    s16 crossfade_state;               /* 0x20: 1 -> 2 in music_remaining_time_callback */
    s8 requested_track;                /* 0x22: -1 = none */
    s8 requested_transition_track;     /* 0x23 */
    s32 crossfade_remaining_time;      /* 0x24 */
    s32 crossfade_interval;            /* 0x28: remaining time / 4 */
    s32 retry_timer;                   /* 0x2C */
    struct MusicCdReadMode cd_mode;    /* 0x30: 4th sceCdRead argument */
    struct MusicStreamChannel primary;    /* 0x34 */
    struct MusicStreamChannel secondary;  /* 0x50 */
    struct MusicStreamChannel transition; /* 0x6C */
};

extern struct MusicStreamState music_stream_state __asm__("D_001516D0");

#endif /* LOMBYTE_RNC_AUDIO_MUSIC_MUSIC_STREAM_STATE_H */
