/* Built only by `make layout-check-sys` (never linked). gcc 2.95 has no
   _Static_assert: a negative array size fails the compile. */
#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[ \
        ((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(name, type, size) \
    typedef char size_check_##name[(sizeof(type) == (size)) ? 1 : -1]

#include "rnc/storage/disc_table.h"

OFFSET_CHECK(level_chunk, struct DiscTable, level_chunk, 0x4F8);
OFFSET_CHECK(music_50000, struct DiscTable, music_50000, 0xF00);
OFFSET_CHECK(sound_bank, struct DiscTable, sound_bank, 0x14E0);
OFFSET_CHECK(unk1530, struct DiscTable, unk1530, 0x1530);
OFFSET_CHECK(memcard_data, struct DiscTable, memcard_data, 0x10);
OFFSET_CHECK(animation_table, struct DiscTable, animation_table, 0x1610);
OFFSET_CHECK(unk1808, struct DiscTable, unk1808, 0x1808);
OFFSET_CHECK(movies, struct DiscTable, movies, 0x1938);
OFFSET_CHECK(unk19F8, struct DiscTable, unk19F8, 0x19F8);
OFFSET_CHECK(unk1A28, struct DiscTable, unk1A28, 0x1A28);
OFFSET_CHECK(start_movies, struct DiscTable, start_movies, 0x1A78);
OFFSET_CHECK(shared_archive, struct DiscTable, shared_archive, 0x2968);
OFFSET_CHECK(music_20000, struct DiscTable, music_20000, 0x2988);
OFFSET_CHECK(music_tracks, struct DiscTable, music_tracks, 0x2AA8);
