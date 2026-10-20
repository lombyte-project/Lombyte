#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/ui/map/map_state.h"

#include "rnc/rendering/texture_upload.h"

#include "rnc/storage/memory_card/memory_card_state.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/gameplay/state/game_globals.h"

extern s32 log2dim(s32) __asm__("FUN_001f97a0");
extern s64 register_8bit_texture(s32, s32, u8 *, u8 *, s32, s32) __asm__("FUN_00204e30");
extern void FUN_0020b4a8(void);
extern void fast_decompress(u8 *, s32) __asm__("FUN_0020b618");
extern s32 start_audio_stream_read(u8 *, s32, s32) __asm__("FUN_00216788");
extern s32 get_stream_buffer_size(s32) __asm__("FUN_00225d88");
extern s32 mark_stream_buffer_read_active(s32) __asm__("FUN_00225dd8");
extern s32 clear_record_flag_by_key(s32) __asm__("FUN_00225e20");

s32 update_menu_stream_image(struct MenuScreen *stream) __asm__("FUN_0021f990");

s32 update_menu_stream_image(struct MenuScreen *stream) {
    enum {
        MENU_STREAM_IMAGE_IDLE = 0,
        MENU_STREAM_IMAGE_READ_PENDING = 1,
        MENU_STREAM_IMAGE_READY = 2,
        MENU_STREAM_IMAGE_RELOAD_PENDING = 3
    };
    s32 flags;
    s32 idx;
    s32 r;
    s32 x;
    s32 y;
    u8 *read_destination;
    struct ClutImage *b;
    u8 *clut_data;
    u8 *pixel_data;

    flags = stream->data.stream.flags;
    if (flags & 1) {
        idx = stream->data.stream.fixed_entry;
        if (idx == -1) {
            return 0;
        }
    } else if (flags & 2) {
        idx = selected_level_index[0];
    } else if (flags & 4) {
        idx = menu_system.current->focus->data.grid.selected_cell;
    } else if (flags & 0x100) {
        idx = menu_system.current->focus->data.save.slot;
        if (idx <= -1) {
            idx = 0;
        }
        if (idx >= 5) {
            idx = 4;
        }
        if (memory_card_state.state < 3 && memory_card_state.pending_state < 0) {
            if (stream->data.stream.state == -1) {
                stream->data.stream.state = 0;
            }
            idx = memory_card_state.card[0].entries[idx].id;
        } else {
            stream->data.stream.state = -1;
        }
    } else {
        idx = menu_system.current->focus->data.list.selected;
        if (idx <= -1) {
            idx = 0;
        }
    }

    switch (stream->data.stream.state) {
    case MENU_STREAM_IMAGE_IDLE:
    case MENU_STREAM_IMAGE_READY:
        if (idx == stream->data.stream.loaded_entry[0]) {
            break;
        }
        if (stream->data.stream.buffer[0] == 0) {
            break;
        }
        if (cd_read_active[0] != 0) {
            break;
        }
        if (stream->data.stream.entries[idx].sector_count == 0) {
            break;
        }
        read_destination = (u8 *)stream->data.stream.buffer[0];
        if (stream->data.stream.flags & 0x20) {
            r = get_stream_buffer_size(stream->data.stream.buffer[0]) -
                (stream->data.stream.entries[idx].sector_count << 11);
            stream->data.stream.read_offset = r;
            read_destination += r;
        }
        if (stream->data.stream.flags & 0x10) {
            r = start_audio_stream_read(read_destination, stream->data.stream.entries[idx].sector,
                                        stream->data.stream.entries[idx].sector_count);
        } else {
            r = start_audio_stream_read(read_destination, stream->data.stream.entries[idx].sector,
                                        stream->data.stream.entries[idx].sector_count);
        }
        if (r != 0) {
            mark_stream_buffer_read_active(stream->data.stream.buffer[0]);
            stream->data.stream.loaded_entry[0] = idx;
            stream->data.stream.state++;
        } else {
            stream->data.stream.state = -1;
        }
        break;
    case MENU_STREAM_IMAGE_READ_PENDING:
    case MENU_STREAM_IMAGE_RELOAD_PENDING:
        if (cd_read_active[0] != 0) {
            break;
        }
        clear_record_flag_by_key(stream->data.stream.buffer[0]);
        if (stream->data.stream.flags & 0x20) {
            fast_decompress((u8 *)stream->data.stream.buffer[0] + stream->data.stream.read_offset,
                         stream->data.stream.buffer[0]);
            stream->data.stream.read_offset = 0;
        }
        b = (struct ClutImage *)stream->data.stream.buffer[0];
        clut_data = b->clut;
        pixel_data = b->pixels;
        x = log2dim(b->width);
        y = log2dim(b->height);
        level_map_selection.tex0 = register_8bit_texture(x, y, clut_data, pixel_data, level_map_selection.tex_clut_vram, level_map_selection.tex0_vram);
        FUN_0020b4a8();
        stream->data.stream.state = 2;
        break;
    }
    return 0;
}

extern __typeof__(update_menu_stream_image) func_0021F990 __attribute__((alias("FUN_0021f990")));
