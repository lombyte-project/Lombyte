#include "types.h"
#include "sda.h"

typedef struct {
    u8 pad_0[0x20];
    s32 width;
    u8 pad_24[0x1C];
    s32 selected_slot;
} SaveMenu;

typedef struct {
    s32 id;
    s32 bolts;
    s32 count;
    s32 time;
    u8 pad_10[5];
    u8 b15;
    u8 b16;
    u8 b17;
    u8 pad_18[4];
} SaveSlot;

typedef struct {
    u8 pad_0[8];
    s32 state;
    u8 pad_C[0x14];
    SaveSlot slots[5];
    u8 pad_AC[0x28];
    s32 xD4;
    s32 pad_D8;
    s32 xDC;
} SaveInfo;

extern SaveInfo D_0013D290;
extern s32 D_0015ED80 MACRO_ADDR;
extern s32 D_001601B0 __attribute__((sda));
extern char D_001602A0[];
extern char D_001602D0[];
extern char D_00160300[];
extern char D_00160310[];
extern char D_00160320[];

extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern void font_print_small(s32, s32, u64, char *, s32) __asm__("func_001F65B0");
extern void font_print_center_small(s32, s32, u64, char *, s32) __asm__("func_001F6B88");
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern s32 find_valid_animation_frame_index(s32, s32) __asm__("FUN_001ff960");
extern void draw_hud_sprite(s32, s32, s32, s32, s32, s32) __asm__("func_001FFC30");
extern void append_screen_sprite(s32, s32, s32, s32, u64, s32) __asm__("func_00200E08");
extern s32 sprintf(char *, const char *, ...);

s32 draw_save_slot_list(SaveMenu *menu) __asm__("FUN_002239e0");

s32 draw_save_slot_list(SaveMenu *menu) {
    char text[0x50];
    SaveSlot *slot;
    s32 color;
    s32 width;
    s32 inner_right;
    s32 highlight_top;
    s32 highlight_bottom;
    s32 inner_top;
    s32 inner_bottom;
    s32 i;
    s32 y;
    s32 play_ticks;
    s32 hours;
    s32 bolts;
    s32 minutes;

    y = 4;
    setup_gif_paging(0);
    for (i = 0; i < 5; i++) {
        width = menu->width;
        highlight_top = y - 4;
        highlight_bottom = y + 0x34;
        inner_right = width - 3;
        inner_top = y - 1;
        inner_bottom = y + 0x31;
        if (D_0013D290.xD4 < 3 && D_0013D290.xDC < 0 && menu->selected_slot == i) {
            append_screen_sprite(0, highlight_top, width, highlight_bottom, 0x8020FFFF, 0);
            append_screen_sprite(3, inner_top, inner_right, inner_bottom, D_001601B0, 0);
        }
        append_screen_sprite(3, inner_top, inner_right, inner_bottom, 0x80303030, 0);
        slot = &D_0013D290.slots[i];
        if (D_0013D290.xD4 >= 3 || D_0013D290.xDC >= 0 || D_0013D290.state != 2) {
            y += 0x30;
        } else {
            if (slot->id == -1) {
                y += 0x10;
                font_print_center_small(menu->width / 2, y, 0x80FFA888, get_help_message_text(0x5217), -1);
                y += 0x20;
            } else {
                play_ticks = slot->time;
                hours = play_ticks / 216000;
                minutes = play_ticks / 3600 - hours * 60;
                color = menu->selected_slot == i ? (s32)0x8020FFFF : (s32)0x80FFA888;
                if (hours > 99) {
                    hours = 99;
                }
                sprintf(text, D_00160300, hours, minutes);
                draw_hud_sprite(find_valid_animation_frame_index(0xE99E, 3), 4, y, 0x10, 0x10, 0x80);
                font_print_small(0x16, y, color, text, -1);
                if (slot->count != 0) {
                    draw_hud_sprite(find_valid_animation_frame_index(0xE99E, 4), 0x4E, y, 0x10, 0x10, 0x80);
                    sprintf(text, D_001602A0, slot->count < 100 ? slot->count : 99);
                    font_print_small(0x60, y, color, text, -1);
                }
                bolts = slot->bolts;
                if (bolts > 9999999) {
                    bolts = 9999999;
                }
                y += 0x10;
                if (bolts < 1000) {
                    sprintf(text, D_001602A0, bolts);
                } else if (bolts < 1000000) {
                    sprintf(text, D_001602D0, bolts / 1000, bolts % 1000);
                } else {
                    sprintf(text, D_00160310, bolts / 1000000, bolts % 1000000 / 1000, bolts % 1000);
                }
                draw_hud_sprite(find_valid_animation_frame_index(0x754F, 0xF), 4, y, 0x10, 0x10, 0x80);
                font_print_small(0x16, y, color, text, -1);
                y += 0x10;
                sprintf(text, D_00160320, slot->b16, slot->b15, slot->b17);
                draw_hud_sprite(find_valid_animation_frame_index(0xE99E, 2), 4, y, 0x10, 0x10, 0x80);
                font_print_small(0x16, y, color, text, -1);
                y += 0x10;
            }
        }
        if (D_0015ED80 != 0) {
            y += 0x24;
        } else {
            y += 0x1B;
        }
    }
    do_gif_paging();
    return 2;
}

extern __typeof__(draw_save_slot_list) func_002239E0 __attribute__((alias("FUN_002239e0")));
