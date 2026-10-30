#include "types.h"
#include "rnc/ui/map/map_state.h"

struct MissionsMenu {
    u8  pad_0[0x20];
    s32 unk20;
    s32 unk24;
    u8  pad28[0x8];
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
extern void enable_global_flag() __asm__("func_001F61E8");
extern void disable_global_flag() __asm__("func_001F61F8");
extern void font_print_large(s32, s32, u64, s32, s32) __asm__("func_001F6530");
extern s32  font_print_wrapped_small(s32, s32, s32, s32, s32, s32, s32) __asm__("func_001F6FD0");
extern void font_print_window_small(void *, u64, void *, s32) __asm__("func_001F75F0");
extern u32  get_help_message_text(s32) __asm__("func_001FDD10");
extern s32  collect_mission_ids() __asm__("func_0020BC00");
extern void draw_menu_selection_marker(s32, s32, s32) __asm__("func_0021F8E8");
extern void vu1_add_g_sregister(s32, s32) __asm__("func_00233980");

s32 draw_missions_menu2(struct MissionsMenu *menu) __asm__("FUN_0021f688");

s32 draw_missions_menu2(struct MissionsMenu *menu) {
    struct Rect trailing_text_region;
    struct Rect text_region_settings;
    s32         mask;
    s32         count;
    s32         i;
    s32         mission_text_y;
    s32         flag;
    s32         col;
    s32         step;
    s32         wrapped_text_result;
    s32         bit;
    s32         y;
    u32        *mission_id_cursor;

    mission_text_y = 0x18;
    i = 0;
    mask = 0;
    setup_gif_paging(0);
    count = collect_mission_ids(0x70000000, &mask, 0, 0);
    vu1_add_g_sregister(0x42, 0x44);
    vu1_add_g_sregister(0x47, 0x10B);
    font_print_large(4, 4, 0x80FFA888, get_help_message_text(0x4F59), -1);
    if (i < count) {
        mission_id_cursor = (u32 *)0x70000000;
        do {
            y = mission_text_y + 0xA;
            flag = menu->sel[level_map_selection.level] == i;
            col = flag ? 0x8020FFFF : 0x80FFA888;
            if (flag) {
                disable_global_flag();
            }
            wrapped_text_result =
                font_print_wrapped_small(0x10,
                                         mission_text_y,
                                         menu->unk20 - 0x11,
                                         0x3E8,
                                         col,
                                         get_help_message_text(*mission_id_cursor),
                                         -1);
            if (flag) {
                enable_global_flag();
            }
            step = (wrapped_text_result >= 0x11) ? 2 : 1;
            if (step == 2) {
                y = mission_text_y + 0x10;
            }
            bit = (mask >> i) & 1;
            i++;
            draw_menu_selection_marker(9, y, bit);
            mission_id_cursor++;
            mission_text_y += step * 16;
        } while (i < count);
    }
    if (mask < 0) {
        memset(&text_region_settings, 0, 0x18);
        text_region_settings.unk02 = menu->unk24;
        text_region_settings.unk06 = menu->unk20;
        text_region_settings.unk08 = menu->unk24 >> 1;
        text_region_settings.unk0A = mission_text_y + 8;
        text_region_settings.unk10 = 0x10;
        text_region_settings.unk12 = 1;
        trailing_text_region = text_region_settings;
        font_print_window_small(
            &trailing_text_region, 0x8020FFFF, get_help_message_text(0x523D), -1);
    }
    do_gif_paging();
    return 2;
}

extern __typeof__(draw_missions_menu2) func_0021F688 __attribute__((alias("FUN_0021f688")));
