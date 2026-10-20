/* Ported from rac1-decomp (src/game/music.c, set_sound_handle_id). */
#include "rnc/storage/disc_table.h"
extern short D_001516D0[];
extern void snd_play_vag_stream_by_loc_ex_cb(int, int, int, int, short, int, int, int, int,
                                             void (*)(int, long), long) __asm__("func_0012EC08");
extern void music_primary_replace_callback(int, long) __asm__("FUN_00216ad0");
/* music_StartTrackBody(int, int, int): when the music record at +0x34 is
   already playing (its word neither 0 nor -1) and not in state 9, starts
   track track + 1 of the table at disc_table.music_tracks on it: state 9, the
   track, track_flags and volume recorded, 10 and 48000 stored, then
   snd_play_vag_stream_by_loc_ex_cb with the current word passed on as its eighth argument
   and 0x24 or 0x20 by track_flags's bit 0. The handle is read twice as in
   func_00216A90 (CSE merges the loads, and the address stays in a
   register as in retail), and the 0x24/0x20 choice comes before the
   stores, which keeps track_flags in $a1. */
void music_start_track_body(int track, int track_flags, int volume) __asm__("FUN_00215d18");

void music_start_track_body(int track, int track_flags, int volume) {
    char *music_state_base = (char *)D_001516D0;
    char *base;
    int *tbl;
    unsigned int cur;
    long track_location;
    int i;
    int flags;

    if (*(short *)(music_state_base + 0x3E) == 9) {
        return;
    }
    cur = *(unsigned int *)(music_state_base + 0x34);
    if (cur == 0 || cur == 0xFFFFFFFF) {
        return;
    }
    base = (char *)&disc_table;
    tbl = (int *)(base + 0x2AA8);
    i = track + 1;
    if (tbl[i] == 0) {
        return;
    }
    track_location = tbl[i];
    flags = (track_flags & 1) ? 0x24 : 0x20;
    *(short *)(music_state_base + 0x38) = track;
    *(short *)(music_state_base + 0x3C) = track_flags;
    *(short *)(music_state_base + 0x3E) = 9;
    *(int *)(music_state_base + 0x48) = 10;
    *(int *)(music_state_base + 0x4C) = 0xBB80;
    *(short *)(music_state_base + 0x3A) = volume;
    *(short *)(music_state_base + 0x44) = 0;
    snd_play_vag_stream_by_loc_ex_cb(track_location, 0, 0, 0, volume, 0, 1, cur, flags,
                                     music_primary_replace_callback,
                                     (long)(unsigned int)(music_state_base + 0x34));
}

extern __typeof__(music_start_track_body) func_00215D18 __attribute__((alias("FUN_00215d18")));
