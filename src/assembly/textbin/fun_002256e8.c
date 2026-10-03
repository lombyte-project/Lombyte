#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_002256e8/FUN_002256e8.s", FUN_002256e8);
#else
#include "types.h"

typedef struct {
    u8 pad_0[0x20];
    u8 phase;
    u8 pad_21[0x52 - 0x21];
    u8 primary_animation;
    u8 secondary_animation;
    u8 pad_54[0x70 - 0x54];
    u8 transition_flags;
    u8 pad_71[0xBC - 0x71];
    u8 elapsed_frames;
} StreamedAnimationMoby;

typedef struct {
    u8 pad_0[0x48];
    u8 *animation_tables[4];
} StreamedClassResource;

typedef struct {
    s32 offset;
    s32 pad;
} AnimationTableHeader;

typedef struct {
    u8 pad_0[0x34];
    s32 state;
    s32 pad_38;
    u8 *buffer;
    s32 read_offset;
    StreamedAnimationMoby *moby;
} MobyAnimationStream;

extern s32 level_archive[] __asm__("D_00137B80");
extern s16 cd_read_active[] __asm__("D_001516D8");
extern s32 queued_dialogue_id[] __asm__("D_001516EC");
extern s16 dialogue_playback_phase[] __asm__("D_0015172A");
extern s32 dialogue_language __asm__("D_0015ED88");
extern s32 alternate_animation_sequence __asm__("D_0015EE20");
extern StreamedClassResource *moby_class_resources[] __asm__("D_001B3200");
extern u8 streamed_moby_class_slot[] __asm__("D_001B4265");
extern u8 animation_asset_read_active[] __asm__("D_001D5CBB");

extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern void relocate_asset_entry_pointers() __asm__("FUN_002032e0");
extern void decompress_wad(u8 *, u8 *) __asm__("FUN_0020b618");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern s32 continue_audio_stream_if_ready(void) __asm__("FUN_00215b10");
extern s32 start_audio_stream_read(u8 *, s32, s32) __asm__("FUN_00216788");
extern s32 FUN_00225dd8(u8 *);

s32 update_streamed_moby_animation(MobyAnimationStream *stream) __asm__("FUN_002256e8");

s32 update_streamed_moby_animation(MobyAnimationStream *stream) {
    StreamedAnimationMoby *moby;
    AnimationTableHeader *header;
    StreamedClassResource **class_resource;
    s32 table_index;
    s32 dialogue_column;
    u8 *animation_table;
    s32 animation_index;

    switch (stream->state) {
    case 0:
        if (cd_read_active[0] != 0) {
            break;
        }
        table_index = 0x4F000 - (level_archive[0x1614 / 4] << 11);
        if (start_audio_stream_read(stream->buffer + table_index, level_archive[0x1610 / 4], level_archive[0x1614 / 4]) != 0) {
            stream->read_offset = table_index;
            stream->state = 1;
            animation_asset_read_active[0] = 1;
            FUN_00225dd8(stream->buffer);
            return 0;
        }
        stream->state = 3;
        break;
    case 1:
        /* Decompress the completed read and relocate three animation tables. */
        if (cd_read_active[0] != 0) {
            break;
        }
        animation_asset_read_active[0] = 0;
        decompress_wad(stream->buffer + stream->read_offset, stream->buffer);
        class_resource = &moby_class_resources[streamed_moby_class_slot[0]];
        header = (AnimationTableHeader *)stream->buffer;
        table_index = 0;
    next:
        animation_table = stream->buffer + header[table_index].offset;
        table_index++;
        (*class_resource)->animation_tables[table_index] = animation_table;
        relocate_asset_entry_pointers(*class_resource, table_index);
        if (table_index < 3) {
            goto next;
        }
        stream->state = 2;
        if (alternate_animation_sequence == 0) {
            stream->moby->phase = 0;
        } else {
            stream->moby->phase = 4;
        }
        stream->moby->elapsed_frames = 0;
        break;
    case 2:
        /* Even phases queue dialogue; odd phases wait for animation completion. */
        moby = stream->moby;
        switch (moby->phase) {
        case 0:
        case 2:
        case 4:
            dialogue_column = dialogue_language - 1;
            if (dialogue_column < 0) {
                dialogue_column = 0;
            }
            animation_index = moby->phase >> 1;
            if (moby->elapsed_frames == 0) {
                queued_dialogue_id[0] = animation_index * 6 + dialogue_column + 60000;
            }
            moby->elapsed_frames++;
            if (moby->elapsed_frames > scale_game_frames(0x78)) {
                moby->elapsed_frames = scale_game_frames(0x78);
            }
            if (moby->elapsed_frames < scale_game_frames(0x78)) {
                return 0;
            }
            if (dialogue_playback_phase[0] != 3) {
                return 0;
            }
            moby->elapsed_frames = 0;
            moby->phase++;
            blend_moby_animation(moby, animation_index + 1, 0, scale_game_frames(0x18));
            break;
        case 1:
        case 3:
        case 5:
            if (moby->elapsed_frames == 0 && moby->primary_animation == moby->secondary_animation) {
                continue_audio_stream_if_ready();
                moby->elapsed_frames = 1;
            }
            if (moby->transition_flags & 2) {
                if (moby->phase == 1) {
                    moby->phase = moby->phase + 1;
                } else {
                    moby->phase = 6;
                }
                moby->elapsed_frames = 0;
                blend_moby_animation(moby, 0, 0, scale_game_frames(0x18));
                break;
            }
            break;
        case 6:
            moby->elapsed_frames++;
            if (moby->elapsed_frames > scale_game_frames(0xF0)) {
                moby->elapsed_frames = scale_game_frames(0xF0);
            }
            if (moby->elapsed_frames < scale_game_frames(0xF0)) {
                return 0;
            }
            if (alternate_animation_sequence == 0) {
                moby->phase = 0;
            } else {
                moby->phase = 4;
            }
            moby->elapsed_frames = 0;
            break;
        }
        break;
    }
    return 0;
}
#endif /* NON_MATCHING */
