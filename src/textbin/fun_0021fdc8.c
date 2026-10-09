#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/storage/memory_card/memory_card_state.h"

typedef struct {
    s32 values[6];
} LanguageResourceOffsets;

extern LanguageResourceOffsets menu_language_resource_offsets __asm__("D_001E87D0");

extern u8 skill_point_completed[] __asm__("D_0013D408");
extern s16 cd_read_active[] __asm__("D_001516D8");
extern s32 dialogue_language_column __asm__("D_0015ED88");
extern s32 menu_resource_selection[] __asm__("D_001A0314");
extern s32 scale_game_frames() __asm__("func_001F96F8");
extern s32 decompress_wad() __asm__("func_0020B618");
extern s32 start_audio_stream_read() __asm__("FUN_00216788");
/* Retail keeps both flag-selected call sites of this read helper; the plain
 * declaration names the same symbol without the asm label, so the two calls
 * are not cross-jumped. */
extern s32 FUN_00216788();
extern s32 get_stream_buffer_size() __asm__("func_00225D88");
extern s32 mark_stream_buffer_read_active() __asm__("FUN_00225dd8");
extern s32 clear_record_flag_by_key() __asm__("FUN_00225e20");
s32 update_menu_resource_stream(struct MenuScreen *stream) __asm__("FUN_0021fdc8");

s32 update_menu_resource_stream(struct MenuScreen *stream) {
    LanguageResourceOffsets language_offsets;
    s32 *completed_buffer_slot;
    s32 primary_entry_offset;
    s32 secondary_entry_offset;
    s32 replacement_entry_offset;
    s32 stream_flags;
    s32 slot_index;
    s32 primary_read_offset;
    s32 secondary_read_offset;
    s32 replacement_read_offset;
    s32 stream_state;
    s32 primary_buffer;
    s32 completed_buffer;
    s32 secondary_buffer;
    s32 replacement_buffer;
    s32 selection_flags;
    s32 resource_index;
    s32 primary_read_address;
    s32 secondary_read_address;
    s32 replacement_read_address;
    s32 primary_read_started;
    s32 secondary_read_started;
    s32 replacement_read_started;
    struct MenuStreamEntry *primary_entry;
    struct MenuStreamEntry *primary_entry_alternate;
    struct MenuStreamEntry *secondary_entry;
    struct MenuStreamEntry *secondary_entry_alternate;
    struct MenuStreamEntry *replacement_entry;
    struct MenuStreamEntry *replacement_entry_alternate;
    struct MenuScreen *item_selection;
    struct MenuScreen *group_selection;
    u8 *item_table_entry;

    selection_flags = stream->data.stream.flags;
    stream->data.stream.elapsed_frames = (s32)(stream->data.stream.elapsed_frames + 1);
    if (selection_flags & 1) {
        resource_index = stream->data.stream.fixed_entry;
        if (resource_index == -1) {
            return 0;
        }
        goto process_stream_state;
    } else {
        if (selection_flags & 2) {
            resource_index = menu_resource_selection[0];
        } else if (selection_flags & 4) {
            resource_index = menu_system.current->focus->data.grid.selected_cell;
        } else if (selection_flags & 0x100) {
            resource_index = menu_system.current->focus->data.save.slot;
            if (resource_index < 0) {
                resource_index = 0;
            }
            if (resource_index >= 5) {
                resource_index = 4;
            }
            if (memory_card_state.state < 3) {
                if (memory_card_state.pending_state < 0) {
                    if (stream->data.stream.state == -1) {
                        stream->data.stream.state = 0;
                    }
                    resource_index =
                        *(s32 *)((u8 *)&memory_card_state + resource_index * 0x1C + 0x20);
                    if (resource_index == -1) {
                        stream->data.stream.state = resource_index;
                    }
                    if (memory_card_state.card[0].type != 2) {
                        stream->data.stream.state = -1;
                    }
                } else {
                    goto disable_stream;
                }
            } else {
                goto disable_stream;
            }
        } else if (selection_flags & 8) {
            group_selection = menu_system.current->focus;
            resource_index = (s32) * (s16 *)((u8 *)((group_selection->data.grid.selected_cell * 0xA) +
                                                    (s32)group_selection->data.grid.cells) +
                                             0x8);
            if (group_selection->data.grid.rows == 0) {
            disable_stream:
                stream->data.stream.state = -1;
            }
        } else if (selection_flags & 0x400) {
            resource_index = (s32)((s32)stream->data.stream.elapsed_frames / scale_game_frames(0x12C)) % 19;
        } else if (selection_flags & 0x1000) {
            language_offsets = menu_language_resource_offsets;
            resource_index = *stream->data.stream.language_base +
                             language_offsets.values[dialogue_language_column];
        } else {
            item_selection = menu_system.current->focus;
            resource_index = item_selection->data.list.selected;
            if (resource_index < 0) {
                resource_index = 0;
            }
            if (selection_flags & 0x4000) {
                item_table_entry = (u8 *)item_selection->data.list.items;
                item_table_entry += resource_index * 0xC;
                resource_index = (*(s16 *)(item_table_entry + 2) == 2) ? 9 : resource_index;
            }
        }
    process_stream_state:
        if (stream->data.stream.flags & 0x2000) {
            resource_index = (skill_point_completed[resource_index] == 0) ? 0x1E : resource_index;
        }
        stream_state = stream->data.stream.state;
        switch (stream_state) {
        case 0:
            primary_buffer = stream->data.stream.buffer[0];
            if (primary_buffer != 0 && cd_read_active[0] == 0) {
                primary_entry_offset = resource_index * 8;
                if (*(s32 *)((u8 *)(primary_entry_offset + (s32)stream->data.stream.entries) + 0x4) !=
                    0) {
                    primary_read_address = primary_buffer;
                    if (stream->data.stream.flags & 0x20) {
                        primary_read_offset =
                            get_stream_buffer_size(primary_buffer) -
                            (*(s32 *)((u8 *)(primary_entry_offset + (s32)stream->data.stream.entries) +
                                      0x4)
                             << 0xB);
                        stream->data.stream.read_offset = primary_read_offset;
                        primary_read_address += primary_read_offset;
                    }
                    if (stream->data.stream.flags & 0x10) {
                        primary_entry = (struct MenuStreamEntry *)(primary_entry_offset +
                                                                     (s32)stream->data.stream.entries);
                        primary_read_started = start_audio_stream_read(
                            primary_read_address, primary_entry->sector, primary_entry->sector_count);
                    } else {
                        primary_entry_alternate =
                            (struct MenuStreamEntry *)(primary_entry_offset +
                                                         (s32)stream->data.stream.entries);
                        primary_read_started = FUN_00216788(
                            primary_read_address, primary_entry_alternate->sector,
                            primary_entry_alternate->sector_count);
                    }
                    if (primary_read_started != 0) {
                        mark_stream_buffer_read_active(stream->data.stream.buffer[0]);
                        stream->data.stream.loaded_entry[0] = resource_index;
                        stream->data.stream.state = stream->data.stream.state + 1;
                    } else {
                        stream->data.stream.state = -1;
                    }
                }
            }
            break;
        case 1:
        case 3:
        case 5:
            if (cd_read_active[0] == 0) {
                stream_flags = stream->data.stream.flags;
                if (stream_flags & 0x20) {
                    completed_buffer_slot = &stream->data.stream.buffer[0];
                    slot_index = stream->data.stream.state == 3;
                    completed_buffer_slot += slot_index;
                    clear_record_flag_by_key(*completed_buffer_slot);
                    completed_buffer = *completed_buffer_slot;
                    decompress_wad(completed_buffer + stream->data.stream.read_offset, completed_buffer);
                    stream->data.stream.read_offset = 0;
                }
                stream->data.stream.state = stream->data.stream.state + 1;
            }
            break;
        case 6:
            stream->data.stream.state = 2;
            /* fallthrough */
        case 2:
            if (resource_index != stream->data.stream.loaded_entry[0]) {
                if (resource_index == stream->data.stream.loaded_entry[1]) {
                    stream->data.stream.state = 4;
                } else {
                    secondary_buffer = stream->data.stream.buffer[1];
                    if (secondary_buffer == 0) {
                        stream->data.stream.state = 0;
                    } else if (cd_read_active[0] == 0) {
                        secondary_entry_offset = resource_index * 8;
                        if (*(s32 *)((u8 *)(secondary_entry_offset + (s32)stream->data.stream.entries) +
                                     0x4) != 0) {
                            secondary_read_address = secondary_buffer;
                            if (stream->data.stream.flags & 0x20) {
                                secondary_read_offset =
                                    get_stream_buffer_size(secondary_buffer) -
                                    (*(s32 *)((u8 *)(secondary_entry_offset +
                                                     (s32)stream->data.stream.entries) +
                                              0x4)
                                     << 0xB);
                                stream->data.stream.read_offset = secondary_read_offset;
                                secondary_read_address += secondary_read_offset;
                            }
                            if (stream->data.stream.flags & 0x10) {
                                secondary_entry =
                                    (struct MenuStreamEntry *)(secondary_entry_offset +
                                                                 (s32)stream->data.stream.entries);
                                secondary_read_started = start_audio_stream_read(
                                    secondary_read_address, secondary_entry->sector,
                                    secondary_entry->sector_count);
                            } else {
                                secondary_entry_alternate =
                                    (struct MenuStreamEntry *)(secondary_entry_offset +
                                                                 (s32)stream->data.stream.entries);
                                secondary_read_started = FUN_00216788(
                                    secondary_read_address, secondary_entry_alternate->sector,
                                    secondary_entry_alternate->sector_count);
                            }
                            if (secondary_read_started != 0) {
                                mark_stream_buffer_read_active(stream->data.stream.buffer[1]);
                                stream->data.stream.loaded_entry[1] = resource_index;
                                stream->data.stream.state = stream->data.stream.state + 1;
                            } else {
                                stream->data.stream.state = -1;
                            }
                        }
                    }
                }
            }
            break;
        case 4:
            if (resource_index != stream->data.stream.loaded_entry[1]) {
                if (resource_index == stream->data.stream.loaded_entry[0]) {
                    stream->data.stream.state = 2;
                } else {
                    replacement_buffer = stream->data.stream.buffer[0];
                    if (replacement_buffer == 0) {
                        stream->data.stream.state = -1;
                    } else if (cd_read_active[0] == 0) {
                        replacement_entry_offset = resource_index * 8;
                        if (*(s32 *)((u8 *)(replacement_entry_offset +
                                            (s32)stream->data.stream.entries) +
                                     0x4) != 0) {
                            replacement_read_address = replacement_buffer;
                            if (stream->data.stream.flags & 0x20) {
                                replacement_read_offset =
                                    get_stream_buffer_size(replacement_buffer) -
                                    (*(s32 *)((u8 *)(replacement_entry_offset +
                                                     (s32)stream->data.stream.entries) +
                                              0x4)
                                     << 0xB);
                                stream->data.stream.read_offset = replacement_read_offset;
                                replacement_read_address += replacement_read_offset;
                            }
                            if (stream->data.stream.flags & 0x10) {
                                replacement_entry =
                                    (struct MenuStreamEntry *)(replacement_entry_offset +
                                                                 (s32)stream->data.stream.entries);
                                replacement_read_started = start_audio_stream_read(
                                    replacement_read_address, replacement_entry->sector,
                                    replacement_entry->sector_count);
                            } else {
                                replacement_entry_alternate =
                                    (struct MenuStreamEntry *)(replacement_entry_offset +
                                                                 (s32)stream->data.stream.entries);
                                replacement_read_started = FUN_00216788(
                                    replacement_read_address,
                                    replacement_entry_alternate->sector,
                                    replacement_entry_alternate->sector_count);
                            }
                            if (replacement_read_started != 0) {
                                mark_stream_buffer_read_active(stream->data.stream.buffer[0]);
                                stream->data.stream.loaded_entry[0] = resource_index;
                                stream->data.stream.state = stream->data.stream.state + 1;
                            } else {
                                stream->data.stream.state = -1;
                            }
                        }
                    }
                }
            }
            break;
        default:
            break;
        }
    }
    return 0;
}

extern s32 func_0021FDC8(struct MenuScreen *stream) __attribute__((alias("FUN_0021fdc8")));

LanguageResourceOffsets menu_language_resource_offsets = {{0, 0, 0xc, 0x24, 0x30, 0x18}};
