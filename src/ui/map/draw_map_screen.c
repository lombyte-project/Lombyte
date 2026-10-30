#include "rnc/ui/map/map_state.h"
#include "types.h"
#include "rnc/input/pad_state.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/globals.h"
#include "rnc/world/data/find_id_in_terminated_table.h"

struct MapHdr {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
};

struct MapEntry {
    s32 a;
    s32 b;
};

#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"

extern u8 map_cross_destination_page[] __asm__("D_001CF678");
extern u8 map_circle_destination_page[] __asm__("D_001CF418");
extern u8 D_0013D4E1[];

#include "rnc/game/level_state.h"
#include "rnc/ui/menus/menu_globals.h"

extern char            D_00141EC0[];
extern struct MapEntry D_001383A0[];
extern struct MapEntry D_00138438[];

extern s32  update_map_zoom_and_pan() __asm__("FUN_00205440");
extern s32  find_map_entry_slot() __asm__("FUN_002050a0");
extern s32  pick_map_slot_to_evict() __asm__("FUN_00205278");
extern void move_map_entry_slot() __asm__("FUN_00205000");
extern s32  promote_first_available_map_entry() __asm__("FUN_00204f60");
extern s32  pick_next_map() __asm__("FUN_002050e8");
extern void compose_bitmap_from_mask() __asm__("FUN_002053d8");
extern void decode_map_mask() __asm__("FUN_00206710");
extern s32  update_mission_list() __asm__("FUN_0020b950");
extern s32  acquire_menu_stream_buffer() __asm__("FUN_00225c18");
extern s32  release_menu_stream_buffer() __asm__("FUN_00225cd8");
extern s32  start_audio_stream_read() __asm__("FUN_00216788");
extern s32  stash_receive_data() __asm__("FUN_00232f20");
extern s32  decompress_wad() __asm__("FUN_0020b618");
extern s64  register_8bit_texture() __asm__("func_00204E30");
extern void load_map_chunk() __asm__("FUN_00207bb0");
extern void FUN_0020b4a8();
extern void update_map_icons() __asm__("func_0020BF90");
extern void allocate_voice_for_target_entry() __asm__("func_0022DA68");

s32 draw_map_screen(struct MenuScreen *screen) __asm__("FUN_0021be60");

s32 draw_map_screen(struct MenuScreen *screen) {
    s32              previous_selected_level_id;
    s32              selected_level_table_index;
    s32              next_level_id;
    s32              id;
    s32              slot;
    s32              eviction_slot_index;
    struct MapHdr   *hdr;
    u8              *pal;
    u8              *pix;
    s32              map_mask_buffer;
    u8              *base;
    u8              *a;
    u8              *texture_block_base;
    u8              *tbase;
    u8              *b;
    u8              *tex2_pixels;
    s32              pick;
    s32              next;
    s32              k;
    struct MapEntry *map_read_entry;

    update_map_zoom_and_pan();
    if (!(screen->data.raw.unk34 & 0x40)) {
        previous_selected_level_id = level_map_selection.level;
        if (controller_state.pressed_unmasked & (PAD_SELECT | PAD_R3 | PAD_START)) {
            if (menu_system.close_locked == 0) {
                return 1;
            }
        }
        if (controller_state.pressed_unmasked & PAD_TRIANGLE) {
            if (menu_system.current->back != 0) {
                menu_system.next = menu_system.current->back;
            } else if (menu_system.close_locked == 0) {
                return -1;
            }
        }
        if (controller_state.pressed_unmasked & PAD_CROSS) {
            menu_system.next = (struct MenuPage *)map_cross_destination_page;
        }
        if ((controller_state.pressed_unmasked & PAD_CIRCLE) && level_map_selection.level != 0) {
            menu_system.unkF0 = (struct MenuPage *)map_circle_destination_page;
            menu_system.unkF4 = 0xB;
            menu_system.close_request = 3;
            menu_system.unkE4 = level_map_selection.level;
            allocate_voice_for_target_entry(0, 0x11, screen->moby);
            return 0;
        }
        selected_level_table_index = find_id_in_terminated_table(level_map_selection.level);
        if (selected_level_table_index >= 0) {
            if (controller_state.pressed_unmasked & 8) {
                if (selected_level_table_index < 0x13) {
                    next_level_id = menu_level_indices[selected_level_table_index + 1];
                    if (next_level_id != 0) {
                        level_map_selection.level = next_level_id;
                    }
                }
            }
            if ((controller_state.pressed_unmasked & 4) && selected_level_table_index != 0) {
                if (menu_level_indices[selected_level_table_index - 1] != 0) {
                    level_map_selection.level = menu_level_indices[selected_level_table_index - 1];
                }
            }
        }
        if (level_map_selection.level != previous_selected_level_id) {
            allocate_voice_for_target_entry(1, 0x11, screen->moby);
            update_mission_list();
        }
    }

    id = level_map_selection.level;
    if (D_0013D4E1[0] != 0) {
        id += 0x100;
    }
    slot = find_map_entry_slot(id);
    if (level_map_selection.level != level_map_selection.loaded && slot != -1) {
        eviction_slot_index = pick_map_slot_to_evict();
        if (level_map_selection.slot[0] != 0 && eviction_slot_index > 0) {
            move_map_entry_slot(eviction_slot_index, 0);
            if (slot == 0) {
                slot = eviction_slot_index;
            }
        }
        decompress_wad(level_map_selection.slot[slot], level_map_selection.hdr);
        hdr = level_map_selection.hdr;
        pix = (u8 *)hdr + hdr->unk8;
        pal = (u8 *)hdr + hdr->unkC;
        map_mask_buffer = acquire_menu_stream_buffer(0);
        if (map_mask_buffer != 0) {
            base = (u8 *)level_map_selection.hdr;
            a = base + hdr->unk0 + 8;
            if (level_map_selection.level == current_level_index) {
                compose_bitmap_from_mask(pal, pix, pal, level_map_selection.mask);
            } else {
                if (level_visit_state[level_map_selection.level] != 0) {
                    load_map_chunk(map_mask_buffer,
                                   D_00141EC0 + (level_map_selection.level << 11),
                                   base + hdr->unk4);
                } else {
                    decode_map_mask(map_mask_buffer, a, a, base);
                }
                compose_bitmap_from_mask(pal, pix, pal, map_mask_buffer);
            }
        }
        if (!(screen->data.raw.unk34 & 0x80)) {
            tbase = (u8 *)level_map_selection.hdr;
            texture_block_base = tbase + hdr->unk10;
            b = tbase + hdr->unk14 + 0x420;
            tex2_pixels = tbase + hdr->unk18 + 0x420;
            level_map_selection.tex0 = register_8bit_texture(7,
                                                             7,
                                                             texture_block_base + 0x20,
                                                             texture_block_base + 0x420,
                                                             level_map_selection.tex_clut_vram,
                                                             level_map_selection.tex0_vram);
            level_map_selection.tex1 = register_8bit_texture(7,
                                                             7,
                                                             texture_block_base + 0x20,
                                                             b,
                                                             level_map_selection.tex_clut_vram,
                                                             level_map_selection.tex1_vram);
            level_map_selection.tex2 = register_8bit_texture(7,
                                                             7,
                                                             texture_block_base + 0x20,
                                                             tex2_pixels,
                                                             level_map_selection.tex_clut_vram,
                                                             level_map_selection.tex2_vram);
            if (map_mask_buffer != 0) {
                register_8bit_texture(9, 9, pal, pal, 0x3FF000, level_map_selection.map_image_vram);
            }
            FUN_0020b4a8();
        }
        if (map_mask_buffer != 0) {
            release_menu_stream_buffer(map_mask_buffer);
        }
        level_map_selection.slot[0] = (s32)pix;
        level_map_selection.loaded = level_map_selection.level;
        update_map_icons(level_map_selection.level, 0);
    }

    if (music_stream_state.read_state == 0) {
        if (level_map_selection.sel != -1) {
            menu_system.pending_buffer = 0;
            level_map_selection.slot_id[level_map_selection.sel] ^= 0x1000;
            level_map_selection.sel = -1;
        }
    }
    pick = -1;
    if (music_stream_state.read_state == 0) {
        pick = promote_first_available_map_entry();
        if (pick != -1) {
            next = pick_next_map();
            if (next != -1) {
                k = (current_level_index + level_map_selection.unk230 == 0) ? 0 : 0x100;
                if (k == next) {
                    stash_receive_data(level_map_selection.slot[pick],
                                       level_map_selection.unk22C,
                                       0,
                                       level_map_selection.unk234,
                                       0);
                    level_map_selection.slot_size[pick] = level_map_selection.unk234;
                } else {
                    if (next & 0x100) {
                        map_read_entry = &D_00138438[next ^ 0x100];
                    } else {
                        map_read_entry = &D_001383A0[next];
                    }
                    start_audio_stream_read(
                        level_map_selection.slot[pick], map_read_entry->a, map_read_entry->b);
                    menu_system.pending_buffer = 1;
                    level_map_selection.slot_size[pick] = (map_read_entry->b << 11) >> 4;
                }
                level_map_selection.slot_id[pick] = next | 0x1000;
                level_map_selection.sel = pick;
            }
        }
    }
    slot = find_map_entry_slot(id);
    if (level_map_selection.level != level_map_selection.loaded) {
        if (music_stream_state.read_state == 0 && slot == -1 && pick == slot) {
            pick_map_slot_to_evict(level_map_selection.loaded);
        }
    }
    return 0;
}

extern __typeof__(draw_map_screen) func_0021BE60 __attribute__((alias("FUN_0021be60")));
