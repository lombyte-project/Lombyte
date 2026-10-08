/* Built only by `make layout-check-sys` (never linked). gcc 2.95 has no
   _Static_assert: a negative array size fails the compile. */
#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[ \
        ((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(name, type, size) \
    typedef char size_check_##name[(sizeof(type) == (size)) ? 1 : -1]

#include "rnc/audio/music/music_stream_state.h"

SIZE_CHECK(cd_mode, struct MusicCdReadMode, 0x4);
OFFSET_CHECK(ch_state, struct MusicStreamChannel, state, 0xA);
OFFSET_CHECK(ch_crossfade_enabled, struct MusicStreamChannel, crossfade_enabled, 0x10);
OFFSET_CHECK(ch_remaining_time, struct MusicStreamChannel, remaining_time, 0x18);
SIZE_CHECK(channel, struct MusicStreamChannel, 0x1C);
OFFSET_CHECK(read_state, struct MusicStreamState, read_state, 0x8);
OFFSET_CHECK(read_dst, struct MusicStreamState, read_dst, 0x14);
OFFSET_CHECK(queued_secondary_track, struct MusicStreamState, queued_secondary_track, 0x1C);
OFFSET_CHECK(crossfade_state, struct MusicStreamState, crossfade_state, 0x20);
OFFSET_CHECK(retry_timer, struct MusicStreamState, retry_timer, 0x2C);
OFFSET_CHECK(cd_mode, struct MusicStreamState, cd_mode, 0x30);
OFFSET_CHECK(primary, struct MusicStreamState, primary, 0x34);
OFFSET_CHECK(secondary, struct MusicStreamState, secondary, 0x50);
OFFSET_CHECK(secondary_state, struct MusicStreamState, secondary.state, 0x5A);
OFFSET_CHECK(transition, struct MusicStreamState, transition, 0x6C);
SIZE_CHECK(state, struct MusicStreamState, 0x88);
