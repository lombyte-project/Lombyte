#include "types.h"

typedef struct {
    s32 id;
    s32 type;
    u8 pad_8[0xC];
} MenuItem;

typedef struct {
    u8 pad_0[0x20];
    s32 unk20;
    u8 pad_24[0x1C];
    s32 unk40;
    u8 pad_44[0x14];
    s32 sel;
    u8 pad_5C[0x74];
    MenuItem items[1];
} Menu;

typedef struct {
    s32 text;
    u8 pad_4[0x48];
} TextEntry;

typedef struct {
    s32 a;
    s32 b;
    u16 c;
    u16 d;
    u8 pad_C[0xC];
} StatEntry;

typedef struct {
    u8 pad_0[0x23];
    u8 unk23;
} Opts;

extern Menu D_001E63C0;
extern TextEntry D_001863D0[];
extern StatEntry D_001DFFB0[];
extern Opts D_0013D4C0;

extern void draw_framebuffer_rect(s32, s32, s32, s32, s32, s32, s32) __asm__("func_001FB8F0");
extern void draw_moby_list(s32, s32) __asm__("func_0020D330");
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern void font_print_small(s32, s32, u64, char *, s32) __asm__("func_001F65B0");
extern void format_scaled_display_value(char *, s32) __asm__("func_00238688");
extern void font_print_right_small(s32, s32, u64, char *, s32) __asm__("func_001F69D0");
extern s32 measure_text_width_regular(char *, s32) __asm__("FUN_001f6250");
extern void append_screen_sprite(s32, s32, s32, s32, u64, s32) __asm__("func_00200E08");

void render_vendor_item_details_pass(void) __asm__("FUN_002389e0");

void render_vendor_item_details_pass(void) {
    char text[0x100];
    s32 text_width;

    draw_framebuffer_rect(0, 0, 0x200, 0x80, 0x200, 0x80, 0);
    draw_moby_list(D_001E63C0.unk20 + 0x100, 1);
    draw_moby_list(D_001E63C0.unk20, 1);
    if (D_001E63C0.items[D_001E63C0.sel].type == 1) {
        font_print_small(
            6, 8, 0x80F0F0F0,
            get_help_message_text(D_001863D0[D_001E63C0.items[D_001E63C0.sel].id].text), -1);
        font_print_small(0x18, 0x18, 0x80F0F0F0, get_help_message_text(0x4F5D), -1);
        format_scaled_display_value(text, D_001DFFB0[D_001E63C0.items[D_001E63C0.sel].id].c);
        font_print_right_small(0x76, 0x65, 0x80F0F0F0, text, -1);
        if (D_001E63C0.unk40 != 0) {
            text_width = measure_text_width_regular(text, -1);
            append_screen_sprite(0x75 - text_width, 0x6D, 0x7B, 0x70, 0x20959544, 0);
            append_screen_sprite(0x76 - text_width, 0x6D, 0x7A, 0x70, 0x30959544, 0);
            append_screen_sprite(0x77 - text_width, 0x6D, 0x79, 0x70, 0x40959544, 0);
            append_screen_sprite(0x78 - text_width, 0x6D, 0x78, 0x70, 0x50959544, 0);
            append_screen_sprite(0x79 - text_width, 0x6D, 0x77, 0x70, 0x60959544, 0);
            append_screen_sprite(0x7A - text_width, 0x6D, 0x76, 0x70, 0x70959544, 0);
            append_screen_sprite(0x7B - text_width, 0x6D, 0x75, 0x70, 0x80959544, 0);
            format_scaled_display_value(
                text, D_001E63C0.unk40 ? D_001DFFB0[D_001E63C0.items[D_001E63C0.sel].id].d
                                       : D_001DFFB0[D_001E63C0.items[D_001E63C0.sel].id].c);
            font_print_right_small(0x76, 0x55, 0x80F0F0F0, text, -1);
        }
    } else {
        font_print_small(
            6, 8, 0x80F0F0F0,
            get_help_message_text(D_001863D0[D_001E63C0.items[D_001E63C0.sel].id].text), -1);
        format_scaled_display_value(text, D_001DFFB0[D_001E63C0.items[D_001E63C0.sel].id].a);
        font_print_right_small(0x76, 0x65, D_0013D4C0.unk23 ? 0x80808080 : 0x80F0F0F0, text, -1);
        if (D_0013D4C0.unk23 != 0) {
            text_width = measure_text_width_regular(text, -1);
            append_screen_sprite(0x75 - text_width, 0x6D, 0x7B, 0x70, 0x20959544, 0);
            append_screen_sprite(0x76 - text_width, 0x6D, 0x7A, 0x70, 0x30959544, 0);
            append_screen_sprite(0x77 - text_width, 0x6D, 0x79, 0x70, 0x40959544, 0);
            append_screen_sprite(0x78 - text_width, 0x6D, 0x78, 0x70, 0x50959544, 0);
            append_screen_sprite(0x79 - text_width, 0x6D, 0x77, 0x70, 0x60959544, 0);
            append_screen_sprite(0x7A - text_width, 0x6D, 0x76, 0x70, 0x70959544, 0);
            append_screen_sprite(0x7B - text_width, 0x6D, 0x75, 0x70, 0x80959544, 0);
            format_scaled_display_value(
                text, D_0013D4C0.unk23 ? D_001DFFB0[D_001E63C0.items[D_001E63C0.sel].id].b
                                       : D_001DFFB0[D_001E63C0.items[D_001E63C0.sel].id].a);
            font_print_right_small(0x76, 0x55, 0x80F0F0F0, text, -1);
        }
    }
}

extern __typeof__(render_vendor_item_details_pass) func_002389E0
    __attribute__((alias("FUN_002389e0")));
