#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00204428/FUN_00204428.s", FUN_00204428);
#else
#include "types.h"
#include "sda.h"

struct LevelArchiveDiscTable {
    u8 pad_0[0x2968];
    s32 shared_start_sector;
    s32 shared_sector_count;
    s32 sound_start_sector;
    s32 sound_sector_count;
    s32 alternate_sound_start_sector;
    s32 alternate_sound_sector_count;
};

struct LevelArchiveDiscEntry {
    u8 pad_0[0x12C8];
    s32 start_sector;
    s32 sector_count;
};

struct LevelArchiveHeader {
    u8 pad_0[8];
    s32 sound_bank_offset;
};

extern struct LevelArchiveDiscTable D_00137B80;
extern s16 D_0013E056[];
extern s32 D_0015ED58 MACRO_ADDR;
extern s32 D_0015ED5C MACRO_ADDR;
extern s32 D_0015ED80 MACRO_ADDR;
extern u16 D_0015EE48 MACRO_ADDR;
extern s16 D_0015EE4A;
extern struct LevelArchiveHeader *D_0015EE4C;
extern u8 *D_0015EE50;
extern u8 *D_0015EE54;
extern s32 D_0015EEBC __attribute__((sda));
extern s32 D_0015EEC0 MACRO_ADDR;
extern u8 D_1FF8000[];
extern void store_async_sound_bank_handle() __asm__("FUN_0022dd78");
extern s32 snd_flush_sound_commands() __asm__("func_0012DC80");
extern void snd_bank_load_from_ee_cb(s32, s32, u64) __asm__("func_0012E088");
extern void snd_resolve_bank_xrefs() __asm__("func_0012E1A8");
extern s32 snd_unload_bank(s32) __asm__("func_0012E1D8");
extern s32 snd_stop_all_sounds() __asm__("func_0012E3B8");
extern void submit_audio_stream_io_request(s32, s32, s32) __asm__("func_00216728");
extern s32 sceCdBreak();
extern s32 sceCdGetError();
extern s32 sceCdSync(s32);

s32 service_level_archive_load(void) __asm__("FUN_00204428");

s32 service_level_archive_load(void) {
    s32 stage;
    s32 retry_stage;
    s32 archive_start_or_bytes;
    s32 level_archive_sectors;
    s32 archive_start_or_sectors;
    u8 *sound_archive_buffer;
    u8 *level_archive_buffer;
    u8 *shared_archive_buffer;
    struct LevelArchiveDiscEntry *disc_entry;
    s32 level_index;
    struct LevelArchiveHeader *shared_header;

    level_index = D_0013E056[0] + 1;
    if (sceCdSync(1) != 0) {
        D_0015EEBC = D_0015EEBC + 1;
        if (D_0015ED58 == 1) {
            if (D_0015EEBC >= 0x2D1) {
                D_0015EEC0 = D_0015ED58;
                retry_stage = D_0015EE48 - 1;
                D_0015ED58 = 0;
                if ((u16)retry_stage < 3) {
                    D_0015EE48 = retry_stage;
                }
                sceCdBreak();
            }
        }
        return 0;
    }
    if (sceCdGetError() != 0) {
        if (D_0015EEC0 == 0) {
            D_0015EEC0 = 1;
            retry_stage = D_0015EE48 - 1;
            D_0015ED58 = 0;
            if ((u16)retry_stage < 3) {
                D_0015EE48 = retry_stage;
            }
        }
    }
    stage = (s16)D_0015EE48;
    switch (stage) {
    case 0:
        if (D_0015ED80 != 0) {
            archive_start_or_bytes = ((D_00137B80.alternate_sound_sector_count << 11) + 0xFFF) & 0xFFFFF000;
        } else {
            archive_start_or_bytes = ((D_00137B80.sound_sector_count << 11) + 0xFFF) & 0xFFFFF000;
        }
        sound_archive_buffer = D_1FF8000 - archive_start_or_bytes;
        disc_entry = (struct LevelArchiveDiscEntry *)((u8 *)&D_00137B80 + level_index * 8);
        level_archive_sectors = disc_entry->sector_count;
        level_archive_buffer = sound_archive_buffer - (((level_archive_sectors << 11) + 0xFFF) & 0xFFFFF000);
        archive_start_or_sectors = D_00137B80.shared_sector_count;
        shared_archive_buffer = level_archive_buffer - (((archive_start_or_sectors << 11) + 0xFFF) & 0xFFFFF000);
        archive_start_or_bytes = D_00137B80.shared_start_sector;
        D_0015EE54 = level_archive_buffer;
        D_0015EE50 = sound_archive_buffer;
        D_0015EE4C = (struct LevelArchiveHeader *)shared_archive_buffer;
        submit_audio_stream_io_request((s32)shared_archive_buffer, archive_start_or_bytes, archive_start_or_sectors);
        D_0015EE48 = D_0015EE48 + 1;
        break;
    case 1:
        disc_entry = (struct LevelArchiveDiscEntry *)((u8 *)&D_00137B80 + level_index * 8);
        sound_archive_buffer = D_0015EE54;
        level_archive_sectors = disc_entry->sector_count;
        archive_start_or_sectors = disc_entry->start_sector;
        submit_audio_stream_io_request((s32)sound_archive_buffer, archive_start_or_sectors, level_archive_sectors);
        D_0015EE48 = D_0015EE48 + 1;
        break;
    case 2:
        if (D_0015ED80 != 0) {
            submit_audio_stream_io_request((s32)D_0015EE50, D_00137B80.alternate_sound_start_sector, D_00137B80.alternate_sound_sector_count);
        } else {
            submit_audio_stream_io_request((s32)D_0015EE50, D_00137B80.sound_start_sector, D_00137B80.sound_sector_count);
        }
        D_0015EE48 = D_0015EE48 + 1;
        break;
    case 3:
        if (D_0015EE4A != 0) {
            return 0;
        }
        snd_stop_all_sounds();
        if (D_0015ED5C != 0) {
            D_0015EE48 = D_0015EE48 + 1;
        } else {
            D_0015EE48 = 6;
        }
        break;
    case 4:
        if (snd_flush_sound_commands() != 0) {
            return 0;
        }
        snd_unload_bank(D_0015ED5C);
        D_0015ED5C = 0;
        D_0015EE48 = D_0015EE48 + 1;
        break;
    case 5:
        if (snd_flush_sound_commands() != 0) {
            return 0;
        }
        snd_resolve_bank_xrefs();
        D_0015EE48 = D_0015EE48 + 1;
        break;
    case 6:
        if (snd_flush_sound_commands() != 0) {
            return 0;
        }
        shared_header = D_0015EE4C;
        D_0015ED5C = -1;
        snd_bank_load_from_ee_cb(shared_header->sound_bank_offset + (s32)shared_header, (s32)store_async_sound_bank_handle, (u32)&D_0015ED5C);
        D_0015EE48 = D_0015EE48 + 1;
        break;
    case 7:
        if (snd_flush_sound_commands() != 0) {
            return 0;
        }
        if (D_0015ED5C == -1) {
            return 0;
        }
        snd_resolve_bank_xrefs();
        return 1;
    }
    return 0;
}
#endif /* NON_MATCHING */
