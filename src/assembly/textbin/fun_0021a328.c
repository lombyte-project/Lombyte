#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021a328/FUN_0021a328.s", FUN_0021a328);
#else
#include "types.h"
#include "rnc/ui/text/text_region.h"

#include "sda.h"

#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/ui/text/font_metrics.h"

#include "rnc/input/pad_state.h"
#include "rnc/gameplay/state/item_state.h"
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

extern void setup_gif_paging(int) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern long get_effect_texture(int) __asm__("func_001F44B8");
extern void EnableGlobalStateFlag(void) __asm__("func_001F61E8");
extern void DisableGlobalStateFlag(void) __asm__("func_001F61F8");
extern void font_print_window(struct TextRegion *, long, char *, int, long,
                              u8 *) __asm__("func_001F7090");
extern int scale_game_frames(int) __asm__("func_001F96F8");
extern long func_001FA6E0(int, int, float);
extern char *get_help_message_text(int) __asm__("func_001FDD10");
extern int find_help_entry(short, int, u16 *) __asm__("func_001FECC8");
extern long func_0021B6D8(int, long, int);
extern void vu1_add_g_sregister(int, long) __asm__("FUN_00233980");
extern void *memset(void *, int, unsigned int);
extern int sprintf(char *, const char *, ...) __asm__("func_00116248");

int render_configured_text_label(struct MenuScreen *label) __asm__("FUN_0021a328");

int render_configured_text_label(struct MenuScreen *label) {
    char formatted_text[64];
    u8 *font;
    int font_texture_index;
    char *text;
    int value_variant;
    int value_index;
    int flags;
    int selector_flags;
    int render_flags;
    int text_flags;
    int draw_flags;
    int x;
    int y;
    int text_style;
    long texture_tex0;
    long color;
    int remaining_frames;
    int visible_height;
    int text_extent;
    struct MenuScreen *page;
    struct MenuGridCell *entry;
    u8 *availability_table;
    int item_id;

    text = empty_label_text;
    font = normal_font_metrics;
    font_texture_index = 1;
    value_variant = 0;
    if (label->data.label.flags & 8) {
        font_texture_index = 3;
        font = large_font_metrics;
    }
    if (label->data.label.flags & 0x10) {
        font_texture_index = 2;
        font = small_font_metrics;
    }
    vu1_add_g_sregister(0x42, 0x44);
    vu1_add_g_sregister(0x47, 0x2004B);
    selector_flags = label->data.label.flags;
    if (selector_flags & 0x20) {
        value_index = current_level_index - 1;
        if ((unsigned int)value_index >= 0x12) {
            value_index = -1;
        }
    } else if (selector_flags & 0x40) {
        value_index = selected_level_index[0] - 1;
    } else if (selector_flags & 4) {
        if (label->data.label.fade_timer < scale_game_frames(menu_fade_duration)) {
            label->data.label.fade_timer = scale_game_frames(menu_fade_duration);
        }
        value_index = 0;
        label->data.label.cached_value = 0;
        label->data.label.value_variant = 0;
    } else if (selector_flags & 0x80) {
        value_index = menu_system.current->focus->data.list.selected;
        if (selector_flags & 0x8000) {
            value_variant = pal_mode != 0;
        }
    } else if (selector_flags & 0x100) {
        page = menu_system.current->focus;
        value_index = page->data.grid.selected_cell;
        entry = &page->data.grid.cells[value_index];
        item_id = entry->id;
        availability_table = entry->kind == 0 ? item_available : alternate_item_available;
        if (availability_table[item_id] != 0) {
            value_index = page->data.grid.selected_cell;
        } else {
            value_index = -1;
        }
    } else if (selector_flags & 0x1000) {
        page = menu_system.current->focus;
        value_index = page->data.list.selected;
        {
            short sid = page->data.list.items[value_index].text;
            label->data.label.text_id = 0xFFFF;
            find_help_entry(sid, 1, (u16 *)&label->data.label.text_id);
        }
    } else {
        page = menu_system.current->focus;
        item_id = page->data.grid.cells[page->data.grid.selected_cell].id;
        value_variant = item_text_variant[item_id] != 0;
        value_index = item_id;
    }

    if (label->data.label.fade_timer == -1) {
        label->data.label.fade_timer = scale_game_frames(menu_fade_duration);
        label->data.label.cached_value = value_index;
        label->data.label.value_variant = value_variant;
    }
    if (value_index != label->data.label.cached_value) {
        if (scale_game_frames(menu_fade_duration) < label->data.label.fade_timer) {
            label->data.label.fade_timer = scale_game_frames(menu_fade_duration);
        }
        remaining_frames = label->data.label.fade_timer;
        remaining_frames = remaining_frames < 1 ? 0 : remaining_frames - 1;
        remaining_frames = remaining_frames < 1 ? 0 : remaining_frames - 1;
        remaining_frames = remaining_frames < 1 ? 0 : remaining_frames - 1;
        label->data.label.fade_timer = remaining_frames;
        if (remaining_frames != 0) {
            value_index = label->data.label.cached_value;
            value_variant = label->data.label.value_variant;
        } else {
            label->data.label.cached_value = value_index;
            label->data.label.value_variant = value_variant;
            label->data.label.flags &= ~0x400;
            label->data.label.scroll_offset = 0;
        }
    } else {
        label->data.label.fade_timer += 3;
    }

    text_flags = label->data.label.flags;
    if (text_flags & 4) {
        if (label->data.label.text_id == 0) {
            return 1;
        }
        text = get_help_message_text(label->data.label.text_id);
    } else if (text_flags & 0x1000) {
        if (label->data.label.text_id == 0xFFFF) {
            return 1;
        }
        text = get_help_message_text(label->data.label.text_id);
    } else if ((text_flags & 0x100) && value_index == -1) {
        text = unavailable_label_text;
    } else if (label->data.label.text_id != 0) {
        text = get_help_message_text(
            ((int *)label->data.label.text_id +
             value_variant)[value_index * label->data.label.text_stride / sizeof(int)]);
    }
    if (!(label->data.label.flags & 0x11E4) && item_available[value_index] == 0) {
        text = unavailable_label_text;
    }
    if (label->data.label.flags & 0x200) {
        item_id = *((int *)label->data.label.text_id +
                    value_index * label->data.label.text_stride / sizeof(int));
        if (item_id != 0x4ED2 && item_id != 0x4ED9 && item_id != 0x4EDD) {
            sprintf(formatted_text, label_format, get_help_message_text(0x4ECC), text);
            text = formatted_text;
        }
    }

    render_flags = label->data.label.flags;
    draw_flags = render_flags;
    x = 4;
    y = 4;
    if ((draw_flags & 0x4004) == 0x4004 && label->data.label.text_id == 0x523E) {
        draw_flags |= 1;
        y = 12;
    }
    if ((render_flags & 0x800) && item_unlocked[value_index] == 0) {
        draw_flags |= 3;
        text = get_help_message_text(0x4F54);
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
    setup_gif_paging(0);
    texture_tex0 = get_effect_texture(font_texture_index);
    {
        struct TextRegion *window;
        struct TextRegion c = {text_vertical_inset,
                               label->height - text_vertical_inset,
                               1,
                               label->width - 4,
                               x,
                               y - (label->data.label.scroll_offset >> 4),
                               .line_advance = text_line_spacing,
                               text_style,
                               .subpixel_y_sixteenths = -(label->data.label.scroll_offset & 0xF)};

        window = &c;
        if (label->data.label.flags & 0x10000) {
            c.bottom = label->height - 1;
        }
        color = func_0021B6D8(label->data.label.fade_timer,
                              func_001FA6E0(menu_text_color, 0x80FFA888, 0.5f), 0x80FFA888);
        c.flags |= 4;
        font_print_window(&c, color, text, -1, texture_tex0, font);
        window->flags ^= 4;
        flags = label->data.label.flags;
        text_extent = c.rendered_height + 4;
        visible_height = c.bottom - c.top;
        if (!(flags & 0x2000) && text_extent >= visible_height) {
            if (!(flags & 0x400)) {
                label->data.label.flags = flags | 0x400;
                label->data.label.scroll_offset = -(label->height * 8);
            }
        } else if (label->data.label.flags & 0x400) {
            label->data.label.scroll_offset = 0;
            label->data.label.flags ^= 0x400;
        }
        c.anchor_y = y - (label->data.label.scroll_offset >> 4);
        c.top += text_shadow_y;
        c.bottom += text_shadow_y;
        c.left += text_shadow_x;
        c.right += text_shadow_x;
        c.anchor_x += text_shadow_x;
        c.anchor_y += text_shadow_y;
        DisableGlobalStateFlag();
        font_print_window(&c, 0x80000000L, text, -1, texture_tex0, font);
        EnableGlobalStateFlag();
        c.top -= text_shadow_y;
        c.bottom -= text_shadow_y;
        c.left -= text_shadow_x;
        c.right -= text_shadow_x;
        c.anchor_x -= text_shadow_x;
        c.anchor_y -= text_shadow_y;
        font_print_window(&c, color, text, -1, texture_tex0, font);
        if (label->data.label.flags & 0x400) {
            c.anchor_y += c.rendered_height + text_line_spacing * 3;
            c.top += text_shadow_y;
            c.bottom += text_shadow_y;
            c.left += text_shadow_x;
            c.right += text_shadow_x;
            c.anchor_x += text_shadow_x;
            c.anchor_y += text_shadow_y;
            DisableGlobalStateFlag();
            font_print_window(&c, 0x80000000L, text, -1, texture_tex0, font);
            EnableGlobalStateFlag();
            c.top -= text_shadow_y;
            c.bottom -= text_shadow_y;
            c.left -= text_shadow_x;
            c.right -= text_shadow_x;
            c.anchor_x -= text_shadow_x;
            c.anchor_y -= text_shadow_y;
            font_print_window(&c, color, text, -1, texture_tex0, font);
            if (label->data.label.flags & 0x400) {
                label->data.label.scroll_offset += (controller_state.held & 1) ? 10 : 3;
                label->data.label.scroll_offset %= (c.rendered_height + text_line_spacing * 3) * 16;
            }
        }
    }
    do_gif_paging();
    return 2;
}

extern __typeof__(render_configured_text_label) func_0021A328
    __attribute__((alias("FUN_0021a328")));

#endif /* NON_MATCHING */
