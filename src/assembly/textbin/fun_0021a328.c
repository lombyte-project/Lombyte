#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021a328/FUN_0021a328.s", FUN_0021a328);
#else
#include "rnc/text_region.h"

struct MenuTextEntry {
    u8 pad0[4];
    s16 kind;
    s16 resource_id;
    u8 pad8[2];
};

struct MenuTextSource {
    u8 pad0[0x34];
    u8 *resource_entries;
    u8 pad38[4];
    s32 current_index;
    s32 selected_index;
    u8 pad44[4];
    struct MenuTextEntry *entries;
};

struct MenuTextContext {
    u8 pad0[0x40];
    struct MenuTextSource *focus;
};

struct ConfiguredTextLabel {
    u8 pad0[0x20];
    s32 width;
    s32 height;
    u8 pad28[8];
    s32 flags;
    u32 text_source; /* Localized ID or word-table address, selected by flags. */
    s32 text_stride;
    s32 scroll_offset;
    u8 pad40[4];
    s32 countdown;
    s32 cached_value;
    s32 cached_gate;
};

struct LabelFrame {
    u8 text[0x40];
    struct TextRegion output;
    u8 pad58[8];
    struct TextRegion input;
};

extern s32 D_0013CAE0[];
extern u8 D_0013D388[];
extern u8 D_0013D408[];
extern u8 D_0013D4C0[];
extern u8 D_0013E520[];
extern s32 D_0015ED80 __attribute__((sda));
extern s32 D_0015ED84 __attribute__((sda));
extern s32 D_001601B0 __attribute__((sda));
extern s32 D_001601B4 __attribute__((sda));
extern u16 D_001601B8 __attribute__((sda));
extern u16 D_001601BC __attribute__((sda));
extern u16 D_00160258 __attribute__((sda));
extern u16 D_00160268 __attribute__((sda));
extern u8 D_00160270[];
extern u8 D_00160278[];
extern u8 D_00160280[];
extern u8 D_00160288[];
extern s32 D_001A0314[];
extern struct MenuTextContext *D_001D5BF4[];
extern u8 D_001DF050[];
extern u8 D_001DF3F0[];
extern u8 D_001DF790[];
extern void func_001F4280(s32);
extern void func_001F4398(void);
extern s32 func_001F44B8(s32);
extern void func_001F61E8(void);
extern void func_001F61F8(void);
extern void func_001F7090(struct TextRegion *, s64, u8 *, s32, s32, void *);
extern s32 func_001F96F8(s32);
extern s32 func_001FA6E0(s32, s32, f32);
extern u8 *func_001FDD10(s32);
extern s32 func_001FECC8(s32, s32, s16 *);
extern s32 func_0021B6D8(s32, s32, s32);
extern void func_00233980(s32, s32);
extern void *memset(void *, s32, u32);
extern s32 sprintf(u8 *, const u8 *, ...);

/* Resolve the selected label, measure its region, then draw shadow and text.
   The countdown delays publication of a changed source value. */
s32 render_configured_text_label(struct ConfiguredTextLabel *label) __asm__("FUN_0021a328");

s32 render_configured_text_label(struct ConfiguredTextLabel *label)
{
    struct LabelFrame frame;
    u8 *text;
    u8 *glyph_metrics;
    u8 *resource_gate_table;
    s32 font_index;
    s16 restored_bottom;
    s16 repeat_bottom;
    s16 resource_id;
    s16 bottom;
    s16 shadow_bottom;
    s16 repeat_shadow_left;
    s16 shadow_top;
    s16 repeat_shadow_top;
    s32 region_flags;
    s32 font_texture;
    s32 source_flags;
    s32 countdown_before;
    s32 countdown_step1;
    s32 countdown_step2;
    s32 countdown_step3;
    s32 scroll_before;
    s32 scroll_after;
    s32 formatted_text_id;
    s32 old_scroll_flags;
    s32 font_flags;
    s32 text_table;
    s32 original_layout_flags;
    s32 scroll_flags;
    s32 selection_flags;
    s32 selected_gate;
    s32 layout_flags;
    s32 anchor_y;
    s32 result;
    s32 fallback_flags;
    s32 horizontal_center;
    s32 gate_value;
    s32 anchor_x;
    s32 text_id;
    s64 colour;
    s64 measure_colour;
    u16 restored_x;
    u16 repeat_x;
    u16 height;
    u16 shadow_y;
    u16 repeat_shadow_right;
    u16 shadow_right;
    u16 repeat_shadow_x;
    u16 shadow_x;
    u16 repeat_shadow_bottom;
    u16 shadow_left;
    u16 repeat_shadow_y;
    u32 level_index;
    s32 selected_value;
    struct MenuTextSource *resource_source;
    struct MenuTextSource *current_source;
    struct MenuTextSource *entry_source;
    struct MenuTextEntry *entry;
    font_index = 1;
    text = D_00160270;
    glyph_metrics = D_001DF050;
    font_flags = label->flags;
    selected_gate = 0;
    if (font_flags & 8) {
        font_index = 3;
        glyph_metrics = D_001DF790;
    }
    if (font_flags & 0x10) {
        font_index = 2;
        glyph_metrics = D_001DF3F0;
    }
    func_00233980(0x42, 0x44);
    func_00233980(0x47, 0x2004B);
    selection_flags = label->flags;
    if (selection_flags & 0x20) {
        level_index = D_0015ED84 - 1;
        selected_value = level_index;
        if (level_index >= 0x12U) {
            selected_value = -1;
        }
    }
    else if (selection_flags & 0x40) {
        selected_value = D_001A0314[0] - 1;
    }
    else if (selection_flags & 4) {
        if (label->countdown < func_001F96F8(D_001601B4)) {
            label->countdown = func_001F96F8(D_001601B4);
        }
        selected_value = 0;
        label->cached_value = 0U;
        label->cached_gate = 0;
    }
    else if (selection_flags & 0x80) {
        selected_value = D_001D5BF4[0]->focus->selected_index;
        if (selection_flags & 0x8000) {
            gate_value = D_0015ED80;
            goto select_gate;
        }
    }
    else if (selection_flags & 0x100) {
        entry_source = D_001D5BF4[0]->focus;
        selected_value = entry_source->current_index;
        entry = entry_source->entries + selected_value;
        if (entry->kind == 0) {
            resource_gate_table = D_0013D4C0;
        }
        else {
            resource_gate_table = D_0013D388;
        }
        if ((*((u8 *)resource_gate_table + entry->resource_id)) == 0) {
            selected_value = -1;
        }
    }
    else if (selection_flags & 0x1000) {
        resource_source = D_001D5BF4[0]->focus;
        selected_value = resource_source->selected_index;
        label->text_source = 0xFFFF;
        func_001FECC8(*((s16 *) ((selected_value * 0xC) + resource_source->resource_entries)), 1, (s16 *)&label->text_source);
    }
    else {
        current_source = D_001D5BF4[0]->focus;
        resource_id = current_source->entries[current_source->current_index].resource_id;
        selected_value = (u32) resource_id;
        gate_value = (s32) (*((u8 *) (resource_id + D_0013E520)));
        select_gate:
        selected_gate = gate_value != 0;

    }
    if (label->countdown == (-1)) {
        label->countdown = func_001F96F8(D_001601B4);
        label->cached_value = selected_value;
        label->cached_gate = selected_gate;
    }
    if (selected_value != label->cached_value) {
        if (func_001F96F8(D_001601B4) < label->countdown) {
            label->countdown = func_001F96F8(D_001601B4);
        }
        countdown_before = label->countdown;
        countdown_step1 = (countdown_before < 1) ? (0) : (countdown_before - 1);
        countdown_step2 = (countdown_step1 < 1) ? (0) : (countdown_step1 - 1);
        countdown_step3 = (countdown_step2 < 1) ? (0) : (countdown_step2 - 1);
        label->countdown = countdown_step3;
        if (countdown_step3 != 0) {
            selected_value = label->cached_value;
            selected_gate = label->cached_gate;
        }
        else {
            label->cached_value = selected_value;
            label->cached_gate = selected_gate;
            label->flags = (s32) (label->flags & (~0x400));
            label->scroll_offset = 0;
        }
    }
    else {
        label->countdown += 3;
    }
    source_flags = label->flags;
    if (source_flags & 4) {
        text_id = label->text_source;
        result = 1;
        if (text_id != 0) {
            goto resolve_text;
        }
        return result;
    }
    if (source_flags & 0x1000) {
        text_id = label->text_source;
        if (text_id == 0xFFFF) {
            return 1;
        }
        goto resolve_text;
    }
    if ((source_flags & 0x100) && (selected_value == (-1))) {
        text = D_00160278;
    }
    else {
        text_table = label->text_source;
        if (text_table != 0) {
            text_id = *((s32 *) (((((u32) (selected_value * label->text_stride)) >> 2) * 4) + ((selected_gate * 4) + text_table)));
            resolve_text:
            text = func_001FDD10(text_id);

        }
    }
    if ((!(label->flags & 0x11E4)) && ((*((u8 *) (selected_value + D_0013D4C0))) == 0)) {
        text = D_00160278;
    }
    if (label->flags & 0x200) {
        formatted_text_id = *((s32 *) (((((u32) (selected_value * label->text_stride)) >> 2) * 4) + label->text_source));
        if (((formatted_text_id != 0x4ED2) && (formatted_text_id != 0x4ED9)) && (formatted_text_id != 0x4EDD)) {
            sprintf(frame.text, D_00160280, func_001FDD10(0x4ECC), text);
            text = frame.text;
        }
    }
    layout_flags = label->flags;
    anchor_x = 4;
    anchor_y = 4;
    original_layout_flags = layout_flags;
    if ((layout_flags & 0x4004) == 0x4004) {
        fallback_flags = original_layout_flags & 0x800;
        if (label->text_source == 0x523E) {
            layout_flags |= 1;
            anchor_y = 0xC;
            goto select_fallback;
        }
    }
    else {
        select_fallback:
        fallback_flags = original_layout_flags & 0x800;

    }
    if ((fallback_flags != 0) && ((*((u8 *) (selected_value + D_0013D408))) == 0)) {
        layout_flags |= 3;
        text = func_001FDD10(0x4F54);
    }
    horizontal_center = layout_flags & 1;
    if (text == 0) {
        text = D_00160288;
        horizontal_center = layout_flags & 1;
    }
    region_flags = TEXT_REGION_USE_SUBPIXEL_RENDERER;
    if (horizontal_center != 0) {
        region_flags = TEXT_REGION_USE_SUBPIXEL_RENDERER | TEXT_REGION_CENTER_HORIZONTALLY;
        anchor_x = ((s32) label->width) / 2;
    }
    if (layout_flags & 2) {
        region_flags |= TEXT_REGION_CENTER_VERTICALLY;
        anchor_y = ((s32) label->height) / 2;
    }
    func_001F4280(0);
    font_texture = func_001F44B8(font_index);
    memset(&frame.input, 0, 0x18);
    height = (u16) label->height;
    bottom = height - D_00160258;
    frame.input.bottom = bottom;
    frame.input.left = 1;
    frame.input.right = ((u16) label->width) - 4;
    frame.input.anchor_x = (s16) anchor_x;
    frame.input.anchor_y = anchor_y - (((s32) label->scroll_offset) >> 4);
    frame.input.line_advance = D_00160268;
    frame.input.flags = region_flags;
    frame.input.subpixel_y_sixteenths = -(((u16) label->scroll_offset) & 0xF);
    frame.input.top = D_00160258;
    *(struct TextRegionBytes *)&frame.output = *(struct TextRegionBytes *)&frame.input;
    if (label->flags & 0x10000) {
        frame.output.bottom = height - 1;
    }
    colour = func_0021B6D8(label->countdown, func_001FA6E0(D_001601B0, 0x80FFA888, 0.5f), 0x80FFA888);
    measure_colour = colour;
    frame.output.flags |= TEXT_REGION_MEASURE_ONLY;
    func_001F7090(&frame.output, measure_colour, text, -1, font_texture, glyph_metrics);
    scroll_flags = label->flags;
    frame.output.flags ^= TEXT_REGION_MEASURE_ONLY;
    if ((!(scroll_flags & 0x2000)) && ((frame.output.rendered_height + 4) >= (frame.output.bottom - frame.output.top))) {
        if (!(scroll_flags & 0x400)) {
            label->flags = (s32) (scroll_flags | 0x400);
            label->scroll_offset = (s32) (-(label->height * 8));
        }
    }
    else {
        old_scroll_flags = label->flags;
        if (old_scroll_flags & 0x400) {
            label->scroll_offset = 0;
            label->flags = (s32) (old_scroll_flags ^ 0x400);
        }
    }
    shadow_y = (anchor_y - (((s32) label->scroll_offset) >> 4)) + D_001601BC;
    shadow_top = ((u16) frame.output.top) + D_001601BC;
    shadow_bottom = ((u16) frame.output.bottom) + D_001601BC;
    shadow_left = frame.output.left + D_001601B8;
    shadow_right = frame.output.right + D_001601B8;
    frame.output.top = shadow_top;
    shadow_x = frame.output.anchor_x + D_001601B8;
    frame.output.bottom = shadow_bottom;
    frame.output.left = shadow_left;
    frame.output.right = shadow_right;
    frame.output.anchor_x = shadow_x;
    frame.output.anchor_y = shadow_y;
    func_001F61F8();
    func_001F7090(&frame.output, (s64) 0x80000000U, text, -1, font_texture, glyph_metrics);
    func_001F61E8();
    restored_bottom = ((u16) frame.output.bottom) - D_001601BC;
    restored_x = frame.output.anchor_x - D_001601B8;
    frame.output.top = ((u16) frame.output.top) - D_001601BC;
    frame.output.left -= D_001601B8;
    frame.output.right -= D_001601B8;
    frame.output.anchor_y -= D_001601BC;
    frame.output.bottom = restored_bottom;
    frame.output.anchor_x = restored_x;
    func_001F7090(&frame.output, colour, text, -1, font_texture, glyph_metrics);
    if (label->flags & 0x400) {
        repeat_shadow_y = (frame.output.anchor_y + (((u16) frame.output.rendered_height) + (((s32) D_00160268) * 3))) + D_001601BC;
        repeat_shadow_top = ((u16) frame.output.top) + D_001601BC;
        repeat_shadow_bottom = ((u16) frame.output.bottom) + D_001601BC;
        repeat_shadow_left = frame.output.left + D_001601B8;
        repeat_shadow_right = frame.output.right + D_001601B8;
        repeat_shadow_x = frame.output.anchor_x + D_001601B8;
        frame.output.top = repeat_shadow_top;
        frame.output.bottom = repeat_shadow_bottom;
        frame.output.left = repeat_shadow_left;
        frame.output.right = repeat_shadow_right;
        frame.output.anchor_x = repeat_shadow_x;
        frame.output.anchor_y = repeat_shadow_y;
        func_001F61F8();
        func_001F7090(&frame.output, (s64) 0x80000000U, text, -1, font_texture, glyph_metrics);
        func_001F61E8();
        repeat_bottom = frame.output.bottom - D_001601BC;
        repeat_x = frame.output.anchor_x - D_001601B8;
        frame.output.top = ((u16) frame.output.top) - D_001601BC;
        frame.output.right -= D_001601B8;
        frame.output.anchor_y -= D_001601BC;
        frame.output.bottom = repeat_bottom;
        frame.output.left = ((u16) frame.output.left) - D_001601B8;
        frame.output.anchor_x = repeat_x;
        func_001F7090(&frame.output, colour, text, -1, font_texture, glyph_metrics);
        if (label->flags & 0x400) {
            scroll_before = label->scroll_offset;
            scroll_after = (D_0013CAE0[0] & 1) ? (scroll_before + 0xA) : (scroll_before + 3);
            label->scroll_offset = scroll_after;
            label->scroll_offset = (s32) (scroll_after % ((s32) ((frame.output.rendered_height + (((s32) D_00160268) * 3)) * 0x10)));
        }
    }
    func_001F4398();
    result = 2;
    return result;
}


__attribute__((alias("FUN_0021a328"))) extern s32 func_0021A328(struct ConfiguredTextLabel *label);
#endif /* NON_MATCHING */
