#include "types.h"
#include "asm.h"
#include "rnc/globals.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00238f08/FUN_00238f08.s", FUN_00238f08);
#else
#include "types.h"
#include "rnc/ui/text/text_region.h"
#include "rnc/ui/vendor/vendor_capture.h"

typedef struct TextRegion FontWindow;

struct VendorSelectionEntry {
    s32 item_index;
    s32 purchase_kind;
    u8 pad8[0xC];
};

struct VendorState {
    u8 pad0[0x40];
    s32 discount_ammo_pricing;
    u8 pad44[0x14];
    s32 selected_entry;
    s32 selection_active;
    u8 pad60[0x70];
    struct VendorSelectionEntry entries[1];
};

extern struct VendorState vendor_state __asm__("D_001E63C0");
#include "rnc/gameplay/state/item_state.h"
extern void draw_framebuffer_rect(s32, s32, s32, s32, s32, s32, u32) __asm__("func_001FB8F0");
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern void font_print_window_small(void *, u64, void *, s32) __asm__("func_001F75F0");
extern void *memset(void *, s32, u32) __asm__("func_001153FC");

void render_vendor_buy_label_pass(s32 capture_context, s32 target_width,
                                  s32 target_height) __asm__("FUN_00238f08");

void render_vendor_buy_label_pass(s32 capture_context, s32 target_width, s32 target_height) {
    FontWindow region;
    s32 price;
    s32 evaluated_message_id;

    draw_framebuffer_rect(0, 0, 0x200, 0x80, 0x200, 0x80, 0);
    memset(&region, 0, 0x18);
    region.bottom = target_height;
    region.right = target_width;
    region.anchor_x = target_width >> 1;
    region.anchor_y = (target_height >> 1) - 7;
    region.line_advance = 0x10;
    region.flags = 1;
    if (vendor_state.selection_active != 0) {
        u8 *entry;
        if (vendor_state.entries[vendor_state.selected_entry].purchase_kind == 1 &&
            (entry = (u8 *)&vendor_state +
                         vendor_state.selected_entry * sizeof(struct VendorSelectionEntry),
             price = *(s32 *)(entry + 0xD0),
             vendor_item_prices[price].ammo_capacity <= weapon_ammo_counts[price])) {
            evaluated_message_id = 0x5233;
        } else if (vendor_state.entries[vendor_state.selected_entry].purchase_kind == 1) {
            if (vendor_state.discount_ammo_pricing != 0) {
                price =
                    vendor_item_prices[vendor_state.entries[vendor_state.selected_entry].item_index]
                        .discounted_ammo_price;
            } else {
                price =
                    vendor_item_prices[vendor_state.entries[vendor_state.selected_entry].item_index]
                        .ammo_price;
            }
            evaluated_message_id = price <= current_bolt_count ? 0x5234 : 0x5233;
        } else {
            if (discount_purchase_pricing[0] != 0) {
                price =
                    vendor_item_prices[vendor_state.entries[vendor_state.selected_entry].item_index]
                        .discounted_purchase_price;
            } else {
                price =
                    vendor_item_prices[vendor_state.entries[vendor_state.selected_entry].item_index]
                        .purchase_price;
            }
            evaluated_message_id = price <= current_bolt_count ? 0x524E : 0x5233;
        }
    } else {
        evaluated_message_id = 0x5234;
    }
    /* Retail evaluates the selection above, then always displays Buy. */
    if (evaluated_message_id != 0) {
        region.flags |= 4;
        font_print_window_small(&region, 0x80F0F0F0, get_help_message_text(0x5234), -1);
        region.flags ^= 4;
        region.anchor_y = (target_height - (s16)region.rendered_height) >> 1;
        font_print_window_small(&region, 0x80F0F0F0, get_help_message_text(0x5234), -1);
    }
}

extern __typeof__(render_vendor_buy_label_pass) func_00238F08
    __attribute__((alias("FUN_00238f08")));

#endif /* NON_MATCHING */
