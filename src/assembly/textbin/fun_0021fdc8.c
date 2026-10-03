#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021fdc8/FUN_0021fdc8.s", FUN_0021fdc8);
#else
#include "types.h"

typedef struct {
    s32 values[6];
} LanguageResourceOffsets;
extern LanguageResourceOffsets menu_language_resource_offsets __asm__("D_001E87D0");

struct SavePreviewState {
    u8 pad_0[0x8];
    s32 menu_mode;
    u8 pad_C[0xC8];
    s32 card_operation_state;
    u8 pad_D8[0x4];
    s32 pending_card_operation;
};

struct MenuPreviewContext {
    u8 pad_0[0x40];
    struct MenuSelectionState *selection;
};

struct MenuSelectionState {
    u8 pad0[0x34];
    s32 item_table_address;
    u8 pad38[4];
    s32 selected_group;
    s32 selected_item;
    u8 pad44[4];
    s32 group_table_address;
};

struct MenuResourceStream {
    u8 pad_0[0x30];
    s32 entry_table_address;
    s32 flags;
    u8 pad_38[0x8];
    s32 language_base_address;
    s32 state;
    s32 primary_buffer;
    s32 secondary_buffer;
    s32 primary_resource_index;
    s32 secondary_resource_index;
    s32 fixed_resource_index;
    s32 elapsed_frames;
    s32 read_offset;
};

struct MenuResourceEntry {
    s32 sector;
    s32 sector_count;
};

extern struct SavePreviewState save_preview_state __asm__("D_0013D290");
extern u8 skill_point_completed[] __asm__("D_0013D408");
extern s16 cd_read_active[] __asm__("D_001516D8");
extern s32 dialogue_language_column __asm__("D_0015ED88");
extern s32 menu_resource_selection[] __asm__("D_001A0314");
extern struct MenuPreviewContext *menu_preview_context[] __asm__("D_001D5BF4");
extern s32 scale_game_frames() __asm__("func_001F96F8");
extern s32 decompress_wad() __asm__("func_0020B618");
extern s32 start_audio_stream_read() __asm__("func_00216788");
/* Retail keeps separate flag-selected call sites for this shared read helper. */
extern s32 start_audio_stream_read_alternate() __asm__("FUN_00216788");
extern s32 get_stream_buffer_size() __asm__("func_00225D88");
extern s32 mark_stream_buffer_read_active() __asm__("func_00225DD8");
extern s32 clear_record_flag_by_key() __asm__("func_00225E20");
s32 update_menu_resource_stream(struct MenuResourceStream *stream) __asm__("FUN_0021fdc8");

s32 update_menu_resource_stream(struct MenuResourceStream *stream) {
    LanguageResourceOffsets language_offsets;
    s32 *completed_buffer_slot;
    s32 primary_entry_offset;
    s32 secondary_entry_offset;
    s32 replacement_entry_offset;
    s32 stream_flags;
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
    s32 next_state;
    s32 secondary_read_started;
    s32 replacement_read_started;
    struct MenuResourceEntry *primary_entry;
    struct MenuResourceEntry *primary_entry_alternate;
    struct MenuResourceEntry *secondary_entry;
    struct MenuResourceEntry *secondary_entry_alternate;
    struct MenuResourceEntry *replacement_entry;
    struct MenuResourceEntry *replacement_entry_alternate;
    struct MenuSelectionState *item_selection;
    struct MenuSelectionState *group_selection;

    selection_flags = stream->flags;
    stream->elapsed_frames = (s32) (stream->elapsed_frames + 1);
    if (selection_flags & 1) {
        resource_index = stream->fixed_resource_index;
        if (resource_index != -1) {
            goto process_stream_state;
        }
    } else {
        if (selection_flags & 2) {
            resource_index = menu_resource_selection[0];
        } else if (selection_flags & 4) {
            resource_index = menu_preview_context[0]->selection->selected_group;
        } else if (selection_flags & 0x100) {
            resource_index = menu_preview_context[0]->selection->selected_item;
            if (resource_index < 0) {
                resource_index = 0;
            }
            if (resource_index >= 5) {
                resource_index = 4;
            }
            if (save_preview_state.card_operation_state < 3) {
                if (save_preview_state.pending_card_operation < 0) {
                    if (stream->state == -1) {
                        stream->state = 0;
                    }
                    resource_index = *(s32 *)((u8 *)&save_preview_state + resource_index * 0x1C + 0x20);
                    if (resource_index == -1) {
                        stream->state = resource_index;
                    }
                    if (save_preview_state.menu_mode != 2) {
                        stream->state = -1;
                    }
                } else {
                    goto disable_stream;
                }
            } else {
                goto disable_stream;
            }
        } else if (selection_flags & 8) {
            group_selection = menu_preview_context[0]->selection;
            resource_index = (s32) *(s16 *)((u8 *)((group_selection->selected_group * 0xA) + group_selection->group_table_address) + 0x8);
            if (group_selection->selected_item == 0) {
disable_stream:
                stream->state = -1;
            }
        } else if (selection_flags & 0x400) {
            resource_index = (s32) ((s32) stream->elapsed_frames / scale_game_frames(0x12C)) % 19;
        } else if (selection_flags & 0x1000) {
            language_offsets = menu_language_resource_offsets;
            resource_index = *(s32 *)stream->language_base_address + language_offsets.values[dialogue_language_column];
        } else {
            item_selection = menu_preview_context[0]->selection;
            resource_index = item_selection->selected_item;
            if (resource_index < 0) {
                resource_index = 0;
            }
            if (selection_flags & 0x4000) {
                resource_index = (*(s16 *)((u8 *)(item_selection->item_table_address + (resource_index * 0xC)) + 0x2) == 2) ? 9 : resource_index;
            }
        }
process_stream_state:
        stream_flags = stream->flags;
        if (stream_flags & 0x2000) {
            resource_index = (skill_point_completed[resource_index] == 0) ? 0x1E : resource_index;
        }
        stream_state = stream->state;
        switch (stream_state) {
        case 0:
            primary_buffer = stream->primary_buffer;
            if (primary_buffer != 0) {
                if (cd_read_active[0] == 0) {
                    primary_entry_offset = resource_index * 8;
                    if (*(s32 *)((u8 *)(primary_entry_offset + stream->entry_table_address) + 0x4) != 0) {
                        primary_read_address = primary_buffer;
                        if (stream->flags & 0x20) {
                            primary_read_offset = get_stream_buffer_size(primary_buffer) - (*(s32 *)((u8 *)(primary_entry_offset + stream->entry_table_address) + 0x4) << 0xB);
                            stream->read_offset = primary_read_offset;
                            primary_read_address += primary_read_offset;
                        }
                        if (stream->flags & 0x10) {
                            primary_entry = (struct MenuResourceEntry *)(primary_entry_offset + stream->entry_table_address);
                            primary_read_started = start_audio_stream_read(primary_read_address, primary_entry->sector, primary_entry->sector_count);
                        } else {
                            primary_entry_alternate = (struct MenuResourceEntry *)(primary_entry_offset + stream->entry_table_address);
                            primary_read_started = start_audio_stream_read_alternate(primary_read_address, primary_entry_alternate->sector, primary_entry_alternate->sector_count);
                        }
                        if (primary_read_started == 0) {
                            stream->state = -1;
                            return 0;
                        }
                        goto publish_primary_read;
                    }
                }
            }
            break;
        case 1:
        case 3:
        case 5:
            if (cd_read_active[0] == 0) {
                completed_buffer_slot = (s32 *)((u8 *)stream + 0x48);
                if (stream_flags & 0x20) {
                    completed_buffer_slot += stream->state == 3;
                    clear_record_flag_by_key(*completed_buffer_slot);
                    completed_buffer = *completed_buffer_slot;
                    decompress_wad(completed_buffer + stream->read_offset, completed_buffer);
                    stream->read_offset = 0;
                }
                next_state = stream->state + 1;
                goto set_stream_state;
            }
            break;
        case 6:
            stream->state = 2;
            /* fallthrough */
        case 2:
            if (resource_index != stream->primary_resource_index) {
                if (resource_index != stream->secondary_resource_index) {
                    secondary_buffer = stream->secondary_buffer;
                    if (secondary_buffer == 0) {
                        stream->state = 0;
                    } else if (cd_read_active[0] == 0) {
                        secondary_entry_offset = resource_index * 8;
                        if (*(s32 *)((u8 *)(secondary_entry_offset + stream->entry_table_address) + 0x4) != 0) {
                            secondary_read_address = secondary_buffer;
                            if (stream->flags & 0x20) {
                                secondary_read_offset = get_stream_buffer_size(secondary_buffer) - (*(s32 *)((u8 *)(secondary_entry_offset + stream->entry_table_address) + 0x4) << 0xB);
                                stream->read_offset = secondary_read_offset;
                                secondary_read_address += secondary_read_offset;
                            }
                            if (stream->flags & 0x10) {
                                secondary_entry = (struct MenuResourceEntry *)(secondary_entry_offset + stream->entry_table_address);
                                secondary_read_started = start_audio_stream_read(secondary_read_address, secondary_entry->sector, secondary_entry->sector_count);
                            } else {
                                secondary_entry_alternate = (struct MenuResourceEntry *)(secondary_entry_offset + stream->entry_table_address);
                                secondary_read_started = start_audio_stream_read_alternate(secondary_read_address, secondary_entry_alternate->sector, secondary_entry_alternate->sector_count);
                            }
                            next_state = -1;
                            if (secondary_read_started != 0) {
                                mark_stream_buffer_read_active(stream->secondary_buffer);
                                stream->secondary_resource_index = resource_index;
                                goto advance_stream_state;
                            }
                            goto set_stream_state;
                        }
                    }
                } else {
                    next_state = 4;
                    goto set_stream_state;
                }
            }
            break;
        case 4:
            if (resource_index != stream->secondary_resource_index) {
                if (resource_index != stream->primary_resource_index) {
                    replacement_buffer = stream->primary_buffer;
                    if (replacement_buffer != 0) {
                        if (cd_read_active[0] == 0) {
                            replacement_entry_offset = resource_index * 8;
                            if (*(s32 *)((u8 *)(replacement_entry_offset + stream->entry_table_address) + 0x4) != 0) {
                                replacement_read_address = replacement_buffer;
                                if (stream->flags & 0x20) {
                                    replacement_read_offset = get_stream_buffer_size(replacement_buffer) - (*(s32 *)((u8 *)(replacement_entry_offset + stream->entry_table_address) + 0x4) << 0xB);
                                    stream->read_offset = replacement_read_offset;
                                    replacement_read_address += replacement_read_offset;
                                }
                                if (stream->flags & 0x10) {
                                    replacement_entry = (struct MenuResourceEntry *)(replacement_entry_offset + stream->entry_table_address);
                                    replacement_read_started = start_audio_stream_read(replacement_read_address, replacement_entry->sector, replacement_entry->sector_count);
                                } else {
                                    replacement_entry_alternate = (struct MenuResourceEntry *)(replacement_entry_offset + stream->entry_table_address);
                                    replacement_read_started = start_audio_stream_read_alternate(replacement_read_address, replacement_entry_alternate->sector, replacement_entry_alternate->sector_count);
                                }
                                next_state = -1;
                                if (replacement_read_started != 0) {
                                    goto publish_primary_read;
                                }
                                goto set_stream_state;
                            }
                        }
                    } else {
                        next_state = -1;
                        goto set_stream_state;
                    }
                } else {
                    next_state = 2;
                    goto set_stream_state;
                }
            }
            break;
        default:
            break;
        }
    }
    return 0;
publish_primary_read:
    mark_stream_buffer_read_active(stream->primary_buffer);
    stream->primary_resource_index = resource_index;
advance_stream_state:
    stream->state = stream->state + 1;
    return 0;
set_stream_state:
    stream->state = next_state;
    return 0;
}

extern s32 func_0021FDC8(struct MenuResourceStream *stream) __attribute__((alias("FUN_0021fdc8")));

#endif /* NON_MATCHING */
