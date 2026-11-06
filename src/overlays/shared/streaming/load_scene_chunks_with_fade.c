/* Streaming: loads scene chunks, with an optional fade. */
#include "types.h"
#include "rnc/streaming/disc_table.h"
#include "asm.h"
#include "rnc/globals.h"
#include "rnc/rendering/effects/fade_to_black.h"
#include "rnc/audio/streaming/update_audio_stream_until_idle.h"

enum {
    PAL_MODE_DISABLED = 0
};

typedef struct {
    u8  pad[0x5C];
    u8 *buf;
    s32 ptr[70];
} H;

extern H D_L00_0016C860;
void     start_audio_stream_read(void *, s32, s32) __asm__("FUN_00216788");
void     load_scene_chunks_with_fade(s32 i) __asm__("FUN_L00_002454c8");

void load_scene_chunks_with_fade(s32 i) {
    s32  pointer_index;
    s32 *offset_entry;
    if (pal_mode == PAL_MODE_DISABLED) {
        start_audio_stream_read(
            D_L00_0016C860.buf, disc_table.scene_chunk_archives[i].sector, disc_table.scene_chunk_archives[i].size);
    } else {
        start_audio_stream_read(
            D_L00_0016C860.buf, disc_table.scene_chunk_archives_alt[i].sector, disc_table.scene_chunk_archives_alt[i].size);
    }
    update_audio_stream_until_idle(0);
    fade_to_black(4);
    update_audio_stream_until_idle(1);
    offset_entry = (s32 *)D_L00_0016C860.buf;
    pointer_index = 0;
    if (offset_entry[1] != 0) {
        do {
            D_L00_0016C860.ptr[pointer_index] =
                (s32)(D_L00_0016C860.buf + (offset_entry[0] + 0x800));
            pointer_index++;
            offset_entry += 2;
        } while (pointer_index < 70 && offset_entry[1] != 0);
    }
}

typedef struct {
    u8  pad[0x5C];
    u8 *buf;
    s32 ptr[70];
} H_245610;

extern H_245610 H245610 __asm__("D_L00_0016C860");
void load_chunks_with_optional_fade(s32 i, s32 fade_frame_count) __asm__("FUN_L00_00245610");

void load_chunks_with_optional_fade(s32 i, s32 p) __asm__("FUN_L00_00245610");

void load_chunks_with_optional_fade(s32 i, s32 fade_frame_count) {
    s32  pointer_index;
    s32 *offset_entry;
    if (pal_mode == PAL_MODE_DISABLED) {
        start_audio_stream_read(
            H245610.buf, disc_table.chunk_archives[i].sector, disc_table.chunk_archives[i].size);
    } else {
        start_audio_stream_read(
            H245610.buf, disc_table.chunk_archives_alt[i].sector, disc_table.chunk_archives_alt[i].size);
    }
    if (fade_frame_count != 0) {
        update_audio_stream_until_idle(0);
        fade_to_black(fade_frame_count);
    }
    update_audio_stream_until_idle(1);
    offset_entry = (s32 *)H245610.buf;
    pointer_index = 0;
    if (offset_entry[1] != 0) {
        do {
            H245610.ptr[pointer_index] = (s32)(H245610.buf + (offset_entry[0] + 0x800));
            pointer_index++;
            offset_entry += 2;
        } while (pointer_index < 70 && offset_entry[1] != 0);
    }
}
