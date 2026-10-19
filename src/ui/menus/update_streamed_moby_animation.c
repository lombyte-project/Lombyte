/* Ported from rac1-decomp (src/game/pause.c: func_00226978). */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby_class_tables.h"
#include "rnc/storage/disc_table.h"

typedef struct {
    u8 pad_0[0x20];
    u8 moby_state;
    u8 pad_21[0x52 - 0x21];
    u8 primary_animation;
    u8 secondary_animation;
    u8 pad_54[0x70 - 0x54];
    u8 transition_flags;
    u8 pad_71[0xBC - 0x71];
    u8 animation_frame_counter;
} StreamedAnimationMoby;

typedef struct {
    u8 pad_0[0x48];
    u8 *animation_tables[4];
} StreamedClassResource;

typedef struct {
    s32 offset;
    s32 reserved4;
} AnimationTableHeader;

typedef struct {
    u8 pad_0[0x34];
    s32 state;
    s32 pad_38;
    u8 *buffer;
    s32 read_offset;
    StreamedAnimationMoby *moby;
} MobyAnimationStream;

extern s16 cd_read_active[] __asm__("D_001516D8");
extern s32 queued_dialogue_id[] __asm__("D_001516EC");
extern s16 dialogue_playback_phase[] __asm__("D_0015172A");
extern s32 alternate_animation_sequence __asm__("D_0015EE20");
extern u8 animation_asset_read_active[] __asm__("D_001D5CBB");

extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern void relocate_asset_entry_pointers(StreamedClassResource *, s32) __asm__("FUN_002032e0");
extern void decompress_wad(u8 *, u8 *) __asm__("FUN_0020b618");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern s32 continue_audio_stream_if_ready(void) __asm__("FUN_00215b10");
extern s32 start_audio_stream_read(u8 *, s32, s32) __asm__("FUN_00216788");
extern s32 mark_stream_buffer_read_active(u8 *) __asm__("FUN_00225dd8");

s32 update_streamed_moby_animation(MobyAnimationStream *stream) __asm__("FUN_002256e8");

s32 update_streamed_moby_animation(MobyAnimationStream *stream) {
    enum {
        ANIMATION_STREAM_START_READ = 0,
        ANIMATION_STREAM_FINISH_READ = 1,
        ANIMATION_STREAM_UPDATE_MOBY = 2
    };
    StreamedAnimationMoby *moby;
    AnimationTableHeader *header;
    StreamedClassResource **class_resource;
    StreamedClassResource **classes;
    s32 k;
    s32 table_index;
    s32 dialogue_column;
    u8 *animation_table;
    s32 animation_index;
    u16 class_slot;

    switch (stream->state) {
    case ANIMATION_STREAM_START_READ:
        if (cd_read_active[0] != 0) {
            break;
        }
        table_index = 0x4F000 - (disc_table.animation_table.size << 11);
        if (start_audio_stream_read(stream->buffer + table_index, disc_table.animation_table.sector,
                                    disc_table.animation_table.size) != 0) {
            stream->read_offset = table_index;
            stream->state = 1;
            animation_asset_read_active[0] = 1;
            mark_stream_buffer_read_active(stream->buffer);
            return 0;
        }
        stream->state = 3;
        break;
    case ANIMATION_STREAM_FINISH_READ:
        /* Decompress the completed read and relocate three animation tables. */
        if (cd_read_active[0] != 0) {
            break;
        }
        animation_asset_read_active[0] = 0;
        decompress_wad(stream->buffer + stream->read_offset, stream->buffer);
        class_slot = resident_class_slot_by_id[0x7A5];
        classes = (StreamedClassResource **)moby_class_resources;
        header = (AnimationTableHeader *)stream->buffer;
        class_resource = &classes[class_slot];
        for (k = 0; k < 3; k++) {
            animation_table =
                stream->buffer + ((AnimationTableHeader *)((u8 *)header + (k << 3)))->offset;
            (*class_resource)->animation_tables[k + 1] = animation_table;
            relocate_asset_entry_pointers(*class_resource, k + 1);
        }
        stream->state = 2;
        if (alternate_animation_sequence == 0) {
            stream->moby->moby_state = 0;
        } else {
            stream->moby->moby_state = 4;
        }
        stream->moby->animation_frame_counter = 0;
        break;
    case ANIMATION_STREAM_UPDATE_MOBY:
        /* Even phases queue dialogue; odd phases wait for animation completion. */
        moby = stream->moby;
        switch (moby->moby_state) {
        case 0:
        case 2:
        case 4:
            dialogue_column = game_language - 1;
            if (dialogue_column < 0) {
                dialogue_column = 0;
            }
            animation_index = moby->moby_state >> 1;
            if (moby->animation_frame_counter == 0) {
                queued_dialogue_id[0] = animation_index * 6 + dialogue_column + 60000;
            }
            moby->animation_frame_counter++;
            if (moby->animation_frame_counter > scale_game_frames(0x78)) {
                moby->animation_frame_counter = scale_game_frames(0x78);
            }
            if (moby->animation_frame_counter < scale_game_frames(0x78)) {
                return 0;
            }
            if (dialogue_playback_phase[0] != 3) {
                return 0;
            }
            moby->animation_frame_counter = 0;
            moby->moby_state++;
            blend_moby_animation(moby, animation_index + 1, 0, scale_game_frames(0x18));
            break;
        case 1:
        case 3:
        case 5:
            if (moby->animation_frame_counter == 0 &&
                moby->primary_animation == moby->secondary_animation) {
                continue_audio_stream_if_ready();
                moby->animation_frame_counter = 1;
            }
            if (moby->transition_flags & 2) {
                if (moby->moby_state == 1) {
                    moby->moby_state = moby->moby_state + 1;
                } else {
                    moby->moby_state = 6;
                }
                moby->animation_frame_counter = 0;
                blend_moby_animation(moby, 0, 0, scale_game_frames(0x18));
                break;
            }
            break;
        case 6:
            moby->animation_frame_counter++;
            if (moby->animation_frame_counter > scale_game_frames(0xF0)) {
                moby->animation_frame_counter = scale_game_frames(0xF0);
            }
            if (moby->animation_frame_counter < scale_game_frames(0xF0)) {
                return 0;
            }
            if (alternate_animation_sequence == 0) {
                moby->moby_state = 0;
            } else {
                moby->moby_state = 4;
            }
            moby->animation_frame_counter = 0;
            break;
        }
        break;
    }
    return 0;
}

extern __typeof__(update_streamed_moby_animation) func_002256E8 __attribute__((alias("FUN_002256e8")));
