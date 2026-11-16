#include <stdint.h>
#include "runtime.h"
#include "rnc/streaming/disc_table.h"
#include "rnc/sdk/storage/cdvd.h"
#include "rnc/sdk/graph.h"

enum {
    DISC_SECTOR_BYTES = 2048,
    DISC_TABLE_SECTOR = 0x5DC,
    DISC_TABLE_SECTORS = 6,
    DISC_STAGE_BYTES = DISC_SECTOR_BYTES * DISC_TABLE_SECTORS,
    DISC_COPY_BYTES = 0x2960,
    DISC_GUARD_BYTES = 16,
    LEVEL_EXTENT_OFFSET = 0x28C8,
    LEVEL_COUNT = 19,
    LEVEL_HEADER_SECTORS = 5,
    LEVEL_HEADER_COPY_BYTES = 0x2434,
    LEVEL_HEADER_SLOTS = 5
};

struct LevelExtent {
    u32 lsn;
    u32 size;
};

u8 disc_table_storage[DISC_STAGE_BYTES + DISC_GUARD_BYTES] __asm__("D_00137B80") __attribute__((aligned(16)));
u8 D_0015ED58 = 0;

/* On the PS2, D_0013A448 is disc_table + 0x28C8 and D_0013A4E0 directly follows
   the copied table. WebAssembly data symbols cannot alias an offset into another
   symbol, so the adapter owns them separately: the extent slice is mirrored from
   the table the original loader filled, and the level header buffer gets its
   own guarded storage. Only the 0x2434 bytes the original reader copies exist. */
struct LevelExtent level_archive_disc_extents[LEVEL_COUNT] __asm__("D_0013A448");
u8 level_header_storage[LEVEL_HEADER_COPY_BYTES + DISC_GUARD_BYTES] __asm__("D_0013A4E0")
    __attribute__((aligned(16)));

_Static_assert(LEVEL_EXTENT_OFFSET + sizeof(level_archive_disc_extents) == DISC_COPY_BYTES,
               "Level extents end where the copied disc table ends.");

static u8 staged_sectors[DISC_STAGE_BYTES + DISC_GUARD_BYTES] __attribute__((aligned(16)));
static u32 volume_sectors;
static u32 read_calls;
static u32 sync_calls;
static u32 error_checks;
static s32 load_active;
static s32 read_state;
static s32 table_ready;
static u32 expected_sector;
static u32 expected_count;
static s32 header_level;

extern s32 read_file_entry_with_retry(s32 file) __asm__("FUN_0012f368");

extern s32 load_disc_sectors_into_global_buffer(void) __asm__("FUN_0012f2b8");

_Static_assert(sizeof(struct DiscTable) <= DISC_STAGE_BYTES, "Disc table must fit the owned storage.");

s32 runtime_disc_guards_valid(void) {
    u32 index;
    for (index = 0; index < DISC_GUARD_BYTES; index++) {
        if (staged_sectors[DISC_STAGE_BYTES + index] != 0xA5 ||
            disc_table_storage[DISC_STAGE_BYTES + index] != 0xA5 ||
            level_header_storage[LEVEL_HEADER_COPY_BYTES + index] != 0xA5) {
            return 0;
        }
    }
    return 1;
}

void runtime_disc_reset(void) {
    u32 index;
    for (index = 0; index < DISC_STAGE_BYTES + DISC_GUARD_BYTES; index++) {
        staged_sectors[index] = index < DISC_STAGE_BYTES ? 0 : 0xA5;
        disc_table_storage[index] = index < DISC_STAGE_BYTES ? 0 : 0xA5;
    }
    for (index = 0; index < LEVEL_HEADER_COPY_BYTES + DISC_GUARD_BYTES; index++) {
        level_header_storage[index] = index < LEVEL_HEADER_COPY_BYTES ? 0 : 0xA5;
    }
    for (index = 0; index < LEVEL_COUNT; index++) {
        level_archive_disc_extents[index].lsn = 0;
        level_archive_disc_extents[index].size = 0;
    }
    expected_sector = 0;
    expected_count = 0;
    header_level = -1;
    volume_sectors = 0;
    read_calls = 0;
    sync_calls = 0;
    error_checks = 0;
    load_active = 0;
    read_state = 0;
    table_ready = 0;
}

u32 runtime_disc_stage_address(void) {
    return (u32)(uintptr_t)staged_sectors;
}

u32 runtime_disc_stage_size(void) {
    return DISC_STAGE_BYTES;
}

u32 runtime_disc_table_address(void) {
    return (u32)(uintptr_t)disc_table_storage;
}

u32 runtime_disc_table_size(void) {
    return DISC_COPY_BYTES;
}

s32 sceCdRead(u32 sector, u32 count, void *destination, struct sceCdRMode *mode) {
    const u8 *bytes = (const u8 *)mode;
    uintptr_t address = (uintptr_t)destination;
    uintptr_t memory_bytes = (uintptr_t)__builtin_wasm_memory_size(0) * 65536U;
    u32 index;

    if (!load_active || read_state != 0 || !runtime_disc_guards_valid() ||
        sector != expected_sector || count != expected_count ||
        address > memory_bytes || count * DISC_SECTOR_BYTES > memory_bytes - address ||
        bytes[0] != 0x20 || bytes[1] != D_0015ED58 || bytes[2] != 0 || bytes[3] != 0) {
        __builtin_trap();
    }
    for (index = 0; index < count * DISC_SECTOR_BYTES; index++) {
        ((u8 *)destination)[index] = staged_sectors[index];
    }
    read_calls++;
    read_state = 1;
    return 1;
}

s32 sceCdSync(s32 mode) {
    if (!load_active || mode != 1 || read_state != 1) {
        __builtin_trap();
    }
    sync_calls++;
    read_state = 2;
    return 0;
}

s32 sceCdGetError(void) {
    if (!load_active || read_state != 2) {
        __builtin_trap();
    }
    error_checks++;
    read_state = 3;
    return 0;
}

s32 sceGsSyncV(s32 mode) {
    (void)mode;
    __builtin_trap();
}

s32 runtime_disc_load(u32 bytes, u32 sectors) {
    s32 result;
    u32 index;
    table_ready = 0;
    header_level = -1;
    if (load_active || bytes != DISC_STAGE_BYTES ||
        sectors < DISC_TABLE_SECTOR + DISC_TABLE_SECTORS || !runtime_disc_guards_valid()) {
        return -1;
    }
    volume_sectors = sectors;
    read_calls = 0;
    sync_calls = 0;
    error_checks = 0;
    read_state = 0;
    expected_sector = DISC_TABLE_SECTOR;
    expected_count = DISC_TABLE_SECTORS;
    load_active = 1;
    result = load_disc_sectors_into_global_buffer();
    load_active = 0;
    if (result != 1 || read_state != 3 || !runtime_disc_guards_valid()) {
        return -1;
    }
    for (index = 0; index < LEVEL_COUNT * sizeof(struct LevelExtent); index++) {
        ((u8 *)level_archive_disc_extents)[index] = disc_table_storage[LEVEL_EXTENT_OFFSET + index];
    }
    table_ready = 1;
    return 1;
}

s32 runtime_disc_ready(void) {
    return table_ready && runtime_disc_guards_valid();
}

static s32 known_entry(u32 offset) {
    const s32 *entry;
    if (!runtime_disc_ready() || (offset != 0x8 && offset != 0x4F8 &&
        offset != 0x12C0 && offset != 0x13B8)) {
        return 0;
    }
    entry = (const s32 *)(disc_table_storage + offset);
    return entry[0] >= 0 && entry[1] > 0 && (u32)entry[0] <= volume_sectors &&
        (u32)entry[1] <= volume_sectors - (u32)entry[0];
}

s32 runtime_disc_entry_sector(u32 offset) {
    return known_entry(offset) ? *(s32 *)(disc_table_storage + offset) : -1;
}

s32 runtime_disc_entry_count(u32 offset) {
    return known_entry(offset) ? *(s32 *)(disc_table_storage + offset + 4) : -1;
}

u32 runtime_disc_service_calls(u32 service) {
    if (service == 0) return read_calls;
    if (service == 1) return sync_calls;
    if (service == 2) return error_checks;
    return 0;
}
static s32 sector_range_valid(u32 sector, u32 count) {
    return count > 0 && sector <= volume_sectors && count <= volume_sectors - sector;
}

/* First sector of a level's 5-sector header, from the extent table the
   original disc-table loader filled. */
s32 runtime_disc_level_header_sector(u32 level) {
    u32 sector;
    if (!runtime_disc_ready() || level >= LEVEL_COUNT) return -1;
    sector = level_archive_disc_extents[level].lsn;
    return sector_range_valid(sector, LEVEL_HEADER_SECTORS) ? (s32)sector : -1;
}

/* Runs the unchanged original read_file_entry_with_retry against 5 staged
   sectors, which must be the sectors at runtime_disc_level_header_sector. */
s32 runtime_disc_level_header_load(u32 level, u32 bytes) {
    s32 result;
    s32 sector = runtime_disc_level_header_sector(level);
    header_level = -1;
    if (load_active || sector < 0 || bytes != LEVEL_HEADER_SECTORS * DISC_SECTOR_BYTES ||
        !runtime_disc_guards_valid()) {
        return -1;
    }
    read_state = 0;
    expected_sector = (u32)sector;
    expected_count = LEVEL_HEADER_SECTORS;
    load_active = 1;
    result = read_file_entry_with_retry((s32)level);
    load_active = 0;
    if (result != 1 || read_state != 3 || !runtime_disc_guards_valid()) return -1;
    header_level = (s32)level;
    return 1;
}

s32 runtime_disc_level_header_ready(void) {
    return header_level;
}

/* Header slots copied to D_0013A4E0. Slot 2 is disc_table.sound_archive at
   0x2970, which load_level_gameplay reads as the NTSC gameplay archive. */
static const u32 *level_slot(u32 slot) {
    const u32 *entry;
    if (header_level < 0 || slot == 0 || slot >= LEVEL_HEADER_SLOTS) return 0;
    entry = (const u32 *)(level_header_storage + slot * 8);
    return sector_range_valid(entry[0], entry[1]) ? entry : 0;
}

s32 runtime_disc_level_slot_sector(u32 slot) {
    const u32 *entry = level_slot(slot);
    return entry ? (s32)entry[0] : -1;
}

/* music_tracks[] of the loaded level header: disc_table + 0x2AA8, which is 0x148 into the bytes
   copied to D_0013A4E0. Each entry is the first sector of a track's VAG stream, or 0 for none
   (src/audio/music/music_start_track.c reads it this way). Returns -1 for an invalid request,
   0 for an empty slot, or the sector. */
s32 runtime_disc_music_track_sector(u32 track) {
    enum { MUSIC_TRACKS_OFFSET = 0x2AA8 - 0x2960, MUSIC_TRACK_SLOTS = 21 };
    u32 sector;
    if (header_level < 0 || track >= MUSIC_TRACK_SLOTS) return -1;
    __builtin_memcpy(&sector, level_header_storage + MUSIC_TRACKS_OFFSET + track * 4, 4);
    if (sector == 0) return 0;
    return sector_range_valid(sector, 1) ? (s32)sector : -1;
}

s32 runtime_disc_level_slot_count(u32 slot) {
    const u32 *entry = level_slot(slot);
    return entry ? (s32)entry[1] : -1;
}
