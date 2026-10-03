#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021a328/FUN_0021a328.s", FUN_0021a328);
#else
#include "types.h"

#include "sda.h"

typedef struct {
    short s[12];
} TextBox;

typedef struct {
    u8 pad0[0x20];
    s32 width;          /* 0x20 */
    s32 height;          /* 0x24 */
    u8 pad28[8];
    int flags;      /* 0x30 */
    int text_id;     /* 0x34 */
    unsigned int text_stride; /* 0x38 */
    int scroll_offset;     /* 0x3C */
    u8 pad40[4];
    int fade_timer;      /* 0x44 */
    int cached_value;        /* 0x48 */
    int value_variant;        /* 0x4C */
} ConfiguredTextLabel;

typedef struct {
    u8 pad0[4];
    short texture_group;        /* 0x4 */
    short item_id;        /* 0x6 */
    u8 pad8[2];
} MenuGridEntry;

typedef struct {
    short item_id;
    u8 pad2[10];
} MenuListEntry;

typedef struct {
    u8 pad0[0x34];
    MenuListEntry *items;   /* 0x34 */
    u8 pad38[4];
    int cursor;           /* 0x3C */
    int selected_entry;          /* 0x40 */
    u8 pad44[4];
    MenuGridEntry *entries; /* 0x48 */
} MenuDescriptor;

typedef struct {
    u8 pad0[0x40];
    MenuDescriptor *page;
} MenuPage;

extern int menu_input_repeat_state[] __asm__("D_0013CAE0");
extern u8 alternate_item_available[] __asm__("D_0013D388");
extern u8 item_unlocked[] __asm__("D_0013D408");
extern u8 item_available[] __asm__("D_0013D4C0");
extern u8 item_text_variant[] __asm__("D_0013E520");
extern int pal_mode __asm__("D_0015ED80") __attribute__((sda));
extern int current_level_index __asm__("D_0015ED84") __attribute__((sda));
extern int menu_text_color __asm__("D_001601B0") __attribute__((sda));
extern int menu_fade_duration __asm__("D_001601B4") __attribute__((sda));
extern int text_shadow_x __asm__("D_001601B8") __attribute__((sda));
extern int text_shadow_y __asm__("D_001601BC") __attribute__((sda));
extern int text_vertical_inset __asm__("D_00160258") __attribute__((sda));
extern int text_line_spacing __asm__("D_00160268") __attribute__((sda));
extern char empty_label_text[] __asm__("D_00160270");
extern char unavailable_label_text[] __asm__("D_00160278");
extern char label_format[] __asm__("D_00160280");
extern char fallback_label_text[] __asm__("D_00160288");
extern int selected_level_index[] __asm__("D_001A0314");
extern MenuPage *active_menu_page[] __asm__("D_001D5BF4");
extern u8 normal_font_metrics[] __asm__("D_001DF050");
extern u8 small_font_metrics[] __asm__("D_001DF3F0");
extern u8 large_font_metrics[] __asm__("D_001DF790");

extern void func_001F4280(int);
extern void func_001F4398(void);
extern long func_001F44B8(int);
extern void EnableGlobalStateFlag(void) __asm__("func_001F61E8");
extern void DisableGlobalStateFlag(void) __asm__("func_001F61F8");
extern void func_001F7090(TextBox *, long, char *, int, long, u8 *);
extern int func_001F96F8(int);
extern long func_001FA6E0(int, int, float);
extern char *func_001FDD10(int);
extern int func_001FECC8(short, int, u16 *);
extern long func_0021B6D8(int, long, int);
extern void FUN_00233980(int, long);
extern void *memset(void *, int, unsigned int);
extern int sprintf(char *, const char *, ...);

int render_configured_text_label(ConfiguredTextLabel *label) __asm__("FUN_0021a328");

int render_configured_text_label(ConfiguredTextLabel *label)
{
    char formatted_text[64];
    u8 *font;
    int font_texture_index;
    char *text;
    int value_variant;
    int value_index;
    int flags;
    int draw_flags;
    int x;
    int y;
    int text_style;
    long texture_tex0;
    long color;
    int remaining_frames;
    MenuDescriptor *page;
    MenuGridEntry *entry;
    u8 *availability_table;
    int item_id;

    font = normal_font_metrics;
    font_texture_index = 1;
    text = empty_label_text;
    value_variant = 0;
    flags = label->flags;
    if (flags & 8) {
        font_texture_index = 3;
        font = large_font_metrics;
    }
    if (flags & 0x10) {
        font_texture_index = 2;
        font = small_font_metrics;
    }
    FUN_00233980(0x42, 0x44);
    FUN_00233980(0x47, 0x2004B);
    flags = label->flags;
    if (flags & 0x20) {
        value_index = current_level_index - 1;
        if ((unsigned int)value_index >= 0x12) {
            value_index = -1;
        }
    } else if (flags & 0x40) {
        value_index = selected_level_index[0] - 1;
    } else if (flags & 4) {
        if (label->fade_timer < func_001F96F8(menu_fade_duration)) {
            label->fade_timer = func_001F96F8(menu_fade_duration);
        }
        value_index = 0;
        label->cached_value = 0;
        label->value_variant = 0;
    } else if (flags & 0x80) {
        value_index = active_menu_page[0]->page->selected_entry;
        if (flags & 0x8000) {
            value_variant = pal_mode != 0;
        }
    } else if (flags & 0x100) {
        page = active_menu_page[0]->page;
        entry = &page->entries[page->cursor];
        if (entry->texture_group == 0) {
            availability_table = item_available;
        } else {
            availability_table = alternate_item_available;
        }
        if (availability_table[entry->item_id] == 0) {
            value_index = -1;
        } else {
            value_index = page->cursor;
        }
    } else if (flags & 0x1000) {
        page = active_menu_page[0]->page;
        value_index = page->selected_entry;
        {
            short sid = page->items[value_index].item_id;
            label->text_id = 0xFFFF;
            func_001FECC8(sid, 1, (u16 *)&label->text_id);
        }
    } else {
        page = active_menu_page[0]->page;
        item_id = page->entries[page->cursor].item_id;
        value_variant = item_text_variant[item_id] != 0;
        value_index = item_id;
    }

    if (label->fade_timer == -1) {
        label->fade_timer = func_001F96F8(menu_fade_duration);
        label->cached_value = value_index;
        label->value_variant = value_variant;
    }
    if (value_index != label->cached_value) {
        if (func_001F96F8(menu_fade_duration) < label->fade_timer) {
            label->fade_timer = func_001F96F8(menu_fade_duration);
        }
        remaining_frames = label->fade_timer;
        remaining_frames = remaining_frames < 1 ? 0 : remaining_frames - 1;
        remaining_frames = remaining_frames < 1 ? 0 : remaining_frames - 1;
        remaining_frames = remaining_frames < 1 ? 0 : remaining_frames - 1;
        label->fade_timer = remaining_frames;
        if (remaining_frames != 0) {
            value_index = label->cached_value;
            value_variant = label->value_variant;
        } else {
            label->cached_value = value_index;
            label->value_variant = value_variant;
            label->flags &= ~0x400;
            label->scroll_offset = 0;
        }
    } else {
        label->fade_timer += 3;
    }

    flags = label->flags;
    if (flags & 4) {
        if (label->text_id == 0) {
            return 1;
        }
        text = func_001FDD10(label->text_id);
    } else if (flags & 0x1000) {
        if (label->text_id == 0xFFFF) {
            return 1;
        }
        text = func_001FDD10(label->text_id);
    } else if ((flags & 0x100) && value_index == -1) {
        text = unavailable_label_text;
    } else if (label->text_id != 0) {
        text = func_001FDD10(((int *)label->text_id + value_variant)[value_index * label->text_stride / sizeof(int)]);
    }
    if (!(label->flags & 0x11E4) && item_available[value_index] == 0) {
        text = unavailable_label_text;
    }
    if (label->flags & 0x200) {
        item_id = *((int *)label->text_id + value_index * label->text_stride / sizeof(int));
        if (item_id != 0x4ED2 && item_id != 0x4ED9 && item_id != 0x4EDD) {
            sprintf(formatted_text, label_format, func_001FDD10(0x4ECC), text);
            text = formatted_text;
        }
    }

    flags = label->flags;
    draw_flags = flags;
    x = 4;
    y = 4;
    if ((draw_flags & 0x4004) == 0x4004 && label->text_id == 0x523E) {
        draw_flags |= 1;
        y = 12;
    }
    if ((flags & 0x800) && item_unlocked[value_index] == 0) {
        draw_flags |= 3;
        text = func_001FDD10(0x4F54);
    }
    if (text == 0) {
        text = fallback_label_text;
    }
    text_style = 8;
    if (draw_flags & 1) {
        text_style = 9;
        x = label->width / 2;
    }
    if (draw_flags & 2) {
        text_style |= 2;
        y = label->height / 2;
    }
    func_001F4280(0);
    texture_tex0 = func_001F44B8(font_texture_index);
    {
        TextBox c = { { text_vertical_inset, label->height - text_vertical_inset, 1, label->width - 4, x,
                        y - (label->scroll_offset >> 4), [8] = text_line_spacing, text_style,
                        [11] = -(label->scroll_offset & 0xF) } };

        if (label->flags & 0x10000) {
            c.s[1] = label->height - 1;
        }
        color = func_0021B6D8(label->fade_timer, func_001FA6E0(menu_text_color, 0x80FFA888, 0.5f), 0x80FFA888);
        c.s[9] |= 4;
        func_001F7090(&c, color, text, -1, texture_tex0, font);
        c.s[9] ^= 4;
        flags = label->flags;
        if (!(flags & 0x2000) && c.s[7] + 4 >= c.s[1] - c.s[0]) {
            if (!(flags & 0x400)) {
                label->flags = flags | 0x400;
                label->scroll_offset = -(label->height * 8);
            }
        } else if (label->flags & 0x400) {
            label->scroll_offset = 0;
            label->flags ^= 0x400;
        }
        c.s[5] = y - (label->scroll_offset >> 4);
        c.s[0] += text_shadow_y;
        c.s[1] += text_shadow_y;
        c.s[2] += text_shadow_x;
        c.s[3] += text_shadow_x;
        c.s[4] += text_shadow_x;
        c.s[5] += text_shadow_y;
        DisableGlobalStateFlag();
        func_001F7090(&c, 0x80000000L, text, -1, texture_tex0, font);
        EnableGlobalStateFlag();
        c.s[0] -= text_shadow_y;
        c.s[1] -= text_shadow_y;
        c.s[2] -= text_shadow_x;
        c.s[3] -= text_shadow_x;
        c.s[4] -= text_shadow_x;
        c.s[5] -= text_shadow_y;
        func_001F7090(&c, color, text, -1, texture_tex0, font);
        if (label->flags & 0x400) {
            c.s[5] += c.s[7] + text_line_spacing * 3;
            c.s[0] += text_shadow_y;
            c.s[1] += text_shadow_y;
            c.s[2] += text_shadow_x;
            c.s[3] += text_shadow_x;
            c.s[4] += text_shadow_x;
            c.s[5] += text_shadow_y;
            DisableGlobalStateFlag();
            func_001F7090(&c, 0x80000000L, text, -1, texture_tex0, font);
            EnableGlobalStateFlag();
            c.s[0] -= text_shadow_y;
            c.s[1] -= text_shadow_y;
            c.s[2] -= text_shadow_x;
            c.s[3] -= text_shadow_x;
            c.s[4] -= text_shadow_x;
            c.s[5] -= text_shadow_y;
            func_001F7090(&c, color, text, -1, texture_tex0, font);
            if (label->flags & 0x400) {
                label->scroll_offset += (menu_input_repeat_state[0] & 1) ? 10 : 3;
                label->scroll_offset %= (c.s[7] + text_line_spacing * 3) * 16;
            }
        }
    }
    func_001F4398();
    return 2;
}

extern __typeof__(render_configured_text_label) func_0021A328 __attribute__((alias("FUN_0021a328")));

#endif /* NON_MATCHING */
