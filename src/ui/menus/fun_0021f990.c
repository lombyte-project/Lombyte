#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/ui/map/map_state.h"

#include "rnc/rendering/texture_upload.h"

#include "rnc/storage/memory_card/memory_card_state.h"
extern s16 D_001516D8[];
extern s32 D_001A0314[];

extern s32 FUN_001f97a0(s32);
extern s64 FUN_00204e30(s32, s32, u8 *, u8 *, s32, s32);
extern void FUN_0020b4a8(void);
extern void FUN_0020b618(u8 *, s32);
extern s32 start_audio_stream_read(u8 *, s32, s32) __asm__("FUN_00216788");
extern s32 get_stream_buffer_size(s32) __asm__("FUN_00225d88");
extern s32 FUN_00225dd8(s32);
extern s32 clear_record_flag_by_key(s32) __asm__("FUN_00225e20");

s32 FUN_0021f990(struct MenuScreen *stream) {
    s32 flags;
    s32 idx;
    s32 r;
    s32 x;
    s32 y;
    u8 *p;
    struct ClutImage *b;
    u8 *p20;
    u8 *p420;

    flags = stream->data.stream.flags;
    if (flags & 1) {
        idx = stream->data.stream.fixed_entry;
        if (idx == -1) {
            return 0;
        }
    } else if (flags & 2) {
        idx = D_001A0314[0];
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
            idx = memory_card_state.card[0].entries[idx].unk0;
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
    case 0:
    case 2:
        if (idx == stream->data.stream.loaded_entry[0]) {
            break;
        }
        if (stream->data.stream.buffer[0] == 0) {
            break;
        }
        if (D_001516D8[0] != 0) {
            break;
        }
        if (stream->data.stream.entries[idx].sector_count == 0) {
            break;
        }
        p = (u8 *)stream->data.stream.buffer[0];
        if (stream->data.stream.flags & 0x20) {
            r = get_stream_buffer_size(stream->data.stream.buffer[0]) -
                (stream->data.stream.entries[idx].sector_count << 11);
            stream->data.stream.read_offset = r;
            p += r;
        }
        if (stream->data.stream.flags & 0x10) {
            r = start_audio_stream_read(p, stream->data.stream.entries[idx].sector,
                                        stream->data.stream.entries[idx].sector_count);
        } else {
            r = start_audio_stream_read(p, stream->data.stream.entries[idx].sector,
                                        stream->data.stream.entries[idx].sector_count);
        }
        if (r != 0) {
            FUN_00225dd8(stream->data.stream.buffer[0]);
            stream->data.stream.loaded_entry[0] = idx;
            stream->data.stream.state++;
        } else {
            stream->data.stream.state = -1;
        }
        break;
    case 1:
    case 3:
        if (D_001516D8[0] != 0) {
            break;
        }
        clear_record_flag_by_key(stream->data.stream.buffer[0]);
        if (stream->data.stream.flags & 0x20) {
            FUN_0020b618((u8 *)stream->data.stream.buffer[0] + stream->data.stream.read_offset,
                         stream->data.stream.buffer[0]);
            stream->data.stream.read_offset = 0;
        }
        b = (struct ClutImage *)stream->data.stream.buffer[0];
        p20 = b->clut;
        p420 = b->pixels;
        x = FUN_001f97a0(b->width);
        y = FUN_001f97a0(b->height);
        D_001A00F0.tex0 = FUN_00204e30(x, y, p20, p420, D_001A00F0.tex_clut_vram, D_001A00F0.tex0_vram);
        FUN_0020b4a8();
        stream->data.stream.state = 2;
        break;
    }
    return 0;
}

extern __typeof__(FUN_0021f990) func_0021F990 __attribute__((alias("FUN_0021f990")));
