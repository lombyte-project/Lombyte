/* Ported from rac1-decomp (src/game/music.c, set_sound_handle_id). */
#include "rnc/streaming/disc_table.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/audio/streaming/music_callbacks.h"

extern void snd_play_vag_stream_by_loc_ex_cb(int,
                                             int,
                                             int,
                                             int,
                                             short,
                                             int,
                                             int,
                                             int,
                                             int,
                                             void (*)(int, long),
                                             long) __asm__("func_0012EC08");
/* The handle is read twice as in func_00216A90 (CSE merges the loads, and
   the address stays in a register as in retail), and the 0x24/0x20 choice
   comes before the stores, which keeps track_flags in $a1. */
void music_start_track_body(int track, int track_flags, int volume) __asm__("FUN_00215d18");

void music_start_track_body(int track, int track_flags, int volume) {
    struct MusicStreamState *music = &music_stream_state;
    struct DiscTable        *tables;
    int                     *music_track_locations;
    unsigned int             active_stream_handle;
    long                     track_location;
    int                      i;
    int                      flags;

    if (music->primary.state == 9) {
        return;
    }
    active_stream_handle = *(unsigned int *)&music->primary.handle;
    if (active_stream_handle == 0 || active_stream_handle == 0xFFFFFFFF) {
        return;
    }
    tables = &disc_table;
    music_track_locations = tables->music_tracks;
    i = track + 1;
    if (music_track_locations[i] == 0) {
        return;
    }
    track_location = music_track_locations[i];
    flags = (track_flags & 1) ? 0x24 : 0x20;
    music->primary.track = track;
    music->primary.flags = track_flags;
    music->primary.state = 9;
    music->primary.poll_interval = 10;
    music->primary.remaining_time = 0xBB80;
    music->primary.volume = volume;
    music->primary.crossfade_enabled = 0;
    snd_play_vag_stream_by_loc_ex_cb(track_location,
                                     0,
                                     0,
                                     0,
                                     volume,
                                     0,
                                     1,
                                     active_stream_handle,
                                     flags,
                                     music_primary_replace_callback,
                                     (long)(unsigned int)&music->primary.handle);
}

extern __typeof__(music_start_track_body) func_00215D18 __attribute__((alias("FUN_00215d18")));
