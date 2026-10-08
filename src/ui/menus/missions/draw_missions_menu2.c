#include "types.h"
#include "rnc/ui/map/map_state.h"

struct MissionsMenu {
    u8 pad_0[0x20];
    s32 unk20;
    s32 unk24;
    u8 pad28[0x8];
    s32 sel[1];
};

struct Rect {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
    s16 unk08;
    s16 unk0A;
    s16 unk0C;
    s16 unk0E;
    u16 unk10;
    u16 unk12;
    s16 unk14;
    s16 unk16;
};

extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging() __asm__("func_001F4398");
extern void func_001F61E8();
extern void func_001F61F8();
extern void font_print_large(s32, s32, u64, s32, s32) __asm__("func_001F6530");
extern s32 font_print_wrapped_small(s32, s32, s32, s32, s32, s32, s32) __asm__("func_001F6FD0");
extern void font_print_window_small(void *, u64, void *, s32) __asm__("func_001F75F0");
extern u32 get_help_message_text(s32) __asm__("func_001FDD10");
extern s32 collect_mission_ids() __asm__("func_0020BC00");
extern void draw_menu_selection_marker(s32, s32, s32) __asm__("func_0021F8E8");
extern void vu1_add_g_sregister(s32, s32) __asm__("func_00233980");

s32 draw_missions_menu2(struct MissionsMenu *menu) __asm__("FUN_0021f688");

s32 draw_missions_menu2(struct MissionsMenu *menu) {
    struct Rect dst;
    struct Rect src;
    s32 mask;
    s32 count;
    s32 i;
    s32 off;
    s32 flag;
    s32 col;
    s32 step;
    s32 h;
    s32 bit;
    s32 y;
    u32 *p;

    off = 0x18;
    i = 0;
    mask = 0;
    setup_gif_paging(0);
    count = collect_mission_ids(0x70000000, &mask, 0, 0);
    vu1_add_g_sregister(0x42, 0x44);
    vu1_add_g_sregister(0x47, 0x10B);
    font_print_large(4, 4, 0x80FFA888, get_help_message_text(0x4F59), -1);
    if (i < count) {
        p = (u32 *)0x70000000;
        do {
            y = off + 0xA;
            flag = menu->sel[D_001A00F0.level] == i;
            col = flag ? 0x8020FFFF : 0x80FFA888;
            if (flag) {
                func_001F61F8();
            }
            h = font_print_wrapped_small(0x10, off, menu->unk20 - 0x11, 0x3E8, col,
                                         get_help_message_text(*p), -1);
            if (flag) {
                func_001F61E8();
            }
            step = (h >= 0x11) ? 2 : 1;
            if (step == 2) {
                y = off + 0x10;
            }
            bit = (mask >> i) & 1;
            i++;
            draw_menu_selection_marker(9, y, bit);
            p++;
            off += step * 16;
        } while (i < count);
    }
    if (mask < 0) {
        memset(&src, 0, 0x18);
        src.unk02 = menu->unk24;
        src.unk06 = menu->unk20;
        src.unk08 = menu->unk24 >> 1;
        src.unk0A = off + 8;
        src.unk10 = 0x10;
        src.unk12 = 1;
        dst = src;
        font_print_window_small(&dst, 0x8020FFFF, get_help_message_text(0x523D), -1);
    }
    do_gif_paging();
    return 2;
}

extern __typeof__(draw_missions_menu2) func_0021F688 __attribute__((alias("FUN_0021f688")));
