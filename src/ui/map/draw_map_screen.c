#include "rnc/ui/map/map_state.h"
#include "types.h"
#include "rnc/globals.h"

struct Pad {
    u8 pad0[0x1C4];
    u32 pressed;
};



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

struct Hdr16 {
    u8 pad0[0x8];
    s16 unk8;
};

extern struct Pad D_0013C940;
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
extern struct Hdr16 D_001516D0;
extern u8 D_001CF678[];
extern u8 D_001CF418[];
extern s32 *D_001601E0 __attribute__((sda));
extern u8 D_0013D4E1[];
extern u8 D_0013DD58[];
extern u8 D_00141EC0[];
extern struct MapEntry D_001383A0[];
extern struct MapEntry D_00138438[];

extern s32 update_map_zoom_and_pan() __asm__("FUN_00205440");
extern s32 find_id_in_terminated_table(s32 id) __asm__("FUN_00205220");
extern s32 find_map_entry_slot() __asm__("FUN_002050a0");
extern s32 pick_map_slot_to_evict() __asm__("FUN_00205278");
extern void move_map_entry_slot() __asm__("FUN_00205000");
extern s32 promote_first_available_map_entry() __asm__("FUN_00204f60");
extern s32 pick_next_map() __asm__("FUN_002050e8");
extern void compose_bitmap_from_mask() __asm__("FUN_002053d8");
extern void decode_map_mask() __asm__("FUN_00206710");
extern s32 update_mission_list() __asm__("FUN_0020b950");
extern s32 select_next_stream_buffer() __asm__("FUN_00225c18");
extern s32 complete_stream_buffer_transfer() __asm__("FUN_00225cd8");
extern s32 start_audio_stream_read() __asm__("FUN_00216788");
extern s32 stash_receive_data() __asm__("FUN_00232f20");
extern s32 func_0020B618_l() __asm__("FUN_0020b618");
extern s64 func_00204E30();
extern void load_map_chunk() __asm__("FUN_00207bb0");
extern void FUN_0020b4a8();
extern void update_map_icons() __asm__("func_0020BF90");
extern void allocate_voice_for_target_entry() __asm__("func_0022DA68");

s32 draw_map_screen(struct MenuScreen *screen) __asm__("FUN_0021be60");

s32 draw_map_screen(struct MenuScreen *screen) {
    s32 prev;
    s32 idx;
    s32 t;
    s32 id;
    s32 slot;
    s32 n;
    struct MapHdr *hdr;
    u8 *pal;
    u8 *pix;
    s32 buf;
    u8 *base;
    u8 *a;
    u8 *tex;
    u8 *tbase;
    u8 *b;
    u8 *c;
    s32 pick;
    s32 next;
    s32 k;
    struct MapEntry *e;

    update_map_zoom_and_pan();
    if (!(screen->data.raw.unk34 & 0x40)) {
        prev = D_001A00F0.level;
        if (D_0013C940.pressed & 0xD00) {
            if (menu_system.close_locked == 0) {
                return 1;
            }
        }
        if (D_0013C940.pressed & 0x10) {
            if (menu_system.current->back != 0) {
                menu_system.next = menu_system.current->back;
            } else if (menu_system.close_locked == 0) {
                return -1;
            }
        }
        if (D_0013C940.pressed & 0x40) {
            menu_system.next = (struct MenuPage *)D_001CF678;
        }
        if ((D_0013C940.pressed & 0x20) && D_001A00F0.level != 0) {
            menu_system.unkF0 = (struct MenuPage *)D_001CF418;
            menu_system.unkF4 = 0xB;
            menu_system.close_request = 3;
            menu_system.unkE4 = D_001A00F0.level;
            allocate_voice_for_target_entry(0, 0x11, screen->moby);
            return 0;
        }
        idx = find_id_in_terminated_table(D_001A00F0.level);
        if (idx >= 0) {
            if (D_0013C940.pressed & 8) {
                if (idx < 0x13) {
                    t = D_001601E0[idx + 1];
                    if (t != 0) {
                        D_001A00F0.level = t;
                    }
                }
            }
            if ((D_0013C940.pressed & 4) && idx != 0) {
                if (D_001601E0[idx - 1] != 0) {
                    D_001A00F0.level = D_001601E0[idx - 1];
                }
            }
        }
        if (D_001A00F0.level != prev) {
            allocate_voice_for_target_entry(1, 0x11, screen->moby);
            update_mission_list();
        }
    }

    id = D_001A00F0.level;
    if (D_0013D4E1[0] != 0) {
        id += 0x100;
    }
    slot = find_map_entry_slot(id);
    if (D_001A00F0.level != D_001A00F0.loaded && slot != -1) {
        n = pick_map_slot_to_evict();
        if (D_001A00F0.slot[0] != 0 && n > 0) {
            move_map_entry_slot(n, 0);
            if (slot == 0) {
                slot = n;
            }
        }
        func_0020B618_l(D_001A00F0.slot[slot], D_001A00F0.hdr);
        hdr = D_001A00F0.hdr;
        pix = (u8 *)hdr + hdr->unk8;
        pal = (u8 *)hdr + hdr->unkC;
        buf = select_next_stream_buffer(0);
        if (buf != 0) {
            base = (u8 *)D_001A00F0.hdr;
            a = base + hdr->unk0 + 8;
            if (D_001A00F0.level == current_level_index) {
                compose_bitmap_from_mask(pal, pix, pal, D_001A00F0.mask);
            } else {
                if (D_0013DD58[D_001A00F0.level] != 0) {
                    load_map_chunk(buf, D_00141EC0 + (D_001A00F0.level << 11), base + hdr->unk4);
                } else {
                    decode_map_mask(buf, a, a, base);
                }
                compose_bitmap_from_mask(pal, pix, pal, buf);
            }
        }
        if (!(screen->data.raw.unk34 & 0x80)) {
            tbase = (u8 *)D_001A00F0.hdr;
            tex = tbase + hdr->unk10;
            b = tbase + hdr->unk14 + 0x420;
            c = tbase + hdr->unk18 + 0x420;
            D_001A00F0.tex0 =
                func_00204E30(7, 7, tex + 0x20, tex + 0x420, D_001A00F0.tex_clut_vram, D_001A00F0.tex0_vram);
            D_001A00F0.tex1 =
                func_00204E30(7, 7, tex + 0x20, b, D_001A00F0.tex_clut_vram, D_001A00F0.tex1_vram);
            D_001A00F0.tex2 =
                func_00204E30(7, 7, tex + 0x20, c, D_001A00F0.tex_clut_vram, D_001A00F0.tex2_vram);
            if (buf != 0) {
                func_00204E30(9, 9, pal, pal, 0x3FF000, D_001A00F0.map_image_vram);
            }
            FUN_0020b4a8();
        }
        if (buf != 0) {
            complete_stream_buffer_transfer(buf);
        }
        D_001A00F0.slot[0] = (s32)pix;
        D_001A00F0.loaded = D_001A00F0.level;
        update_map_icons(D_001A00F0.level, 0);
    }

    if (D_001516D0.unk8 == 0) {
        if (D_001A00F0.sel != -1) {
            menu_system.pending_buffer = 0;
            D_001A00F0.slot_id[D_001A00F0.sel] ^= 0x1000;
            D_001A00F0.sel = -1;
        }
    }
    pick = -1;
    if (D_001516D0.unk8 == 0) {
        pick = promote_first_available_map_entry();
        if (pick != -1) {
            next = pick_next_map();
            if (next != -1) {
                k = (current_level_index + D_001A00F0.unk230 == 0) ? 0 : 0x100;
                if (k == next) {
                    stash_receive_data(D_001A00F0.slot[pick], D_001A00F0.unk22C, 0,
                                       D_001A00F0.unk234, 0);
                    D_001A00F0.slot_size[pick] = D_001A00F0.unk234;
                } else {
                    if (next & 0x100) {
                        e = &D_00138438[next ^ 0x100];
                    } else {
                        e = &D_001383A0[next];
                    }
                    start_audio_stream_read(D_001A00F0.slot[pick], e->a, e->b);
                    menu_system.pending_buffer = 1;
                    D_001A00F0.slot_size[pick] = (e->b << 11) >> 4;
                }
                D_001A00F0.slot_id[pick] = next | 0x1000;
                D_001A00F0.sel = pick;
            }
        }
    }
    slot = find_map_entry_slot(id);
    if (D_001A00F0.level != D_001A00F0.loaded) {
        if (D_001516D0.unk8 == 0 && slot == -1 && pick == slot) {
            pick_map_slot_to_evict(D_001A00F0.loaded);
        }
    }
    return 0;
}

extern __typeof__(draw_map_screen) func_0021BE60 __attribute__((alias("FUN_0021be60")));
