#include "types.h"
#include "asm.h"
#include "rnc/globals.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/gameplay/vendor/render_vendor_buy_label_pass/FUN_00238f08.s", FUN_00238f08);
#else
/* Near miss (game, 14 rows): retail keeps one register for the %hi of vendor_item_prices
   from the first test on; ours computes it twice in that block. The stride local gives
   the first test retail's mult. */
/* Ported from rac1-decomp (src/game/vendor.c: func_0023A220). */
#include "sda.h"
#include "rnc/gameplay/state/item_state.h"
#include "rnc/ui/text/text_region.h"
#include "rnc/ui/vendor/vendor_capture.h"

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
extern s32 current_bolt_count_macro __asm__("D_0015ED98") MACRO_ADDR;
extern void draw_framebuffer_rect(int, int, int, int, int, int, int) __asm__("func_001FB8F0");
extern void *memset(void *, int, unsigned int) __asm__("func_001153FC");
extern void *get_help_message_text(int) __asm__("func_001FDD10");
extern void font_print_window_small(void *, long, void *, int) __asm__("func_001F75F0");

void render_vendor_buy_label_pass(char *unused, int width, int height) __asm__("FUN_00238f08");

void render_vendor_buy_label_pass(char *unused, int width, int height) {
    struct TextRegion region;
    struct VendorState *v;
    int price;
    int message_id;
    int stride = sizeof(struct VendorItemPricing);

    draw_framebuffer_rect(0, 0, 0x200, 0x80, 0x200, 0x80, 0);
    memset(&region, 0, sizeof(region));
    region.bottom = height;
    region.right = width;
    region.anchor_x = width >> 1;
    region.anchor_y = (height >> 1) - 7;
    region.line_advance = 0x10;
    region.flags = TEXT_REGION_CENTER_HORIZONTALLY;
    if (vendor_state.selection_active != 0) {
        if (vendor_state.entries[vendor_state.selected_entry].purchase_kind == 1 &&
            weapon_ammo_counts[vendor_state.entries[vendor_state.selected_entry].item_index] >=
                ((struct VendorItemPricing *)((char *)vendor_item_prices + vendor_state.entries[vendor_state.selected_entry].item_index * stride))->ammo_capacity) {
            message_id = 0x5233;
        } else {
            v = &vendor_state;
            if (v->entries[v->selected_entry].purchase_kind == 1) {
                if (v->discount_ammo_pricing != 0) {
                    price = vendor_item_prices[v->entries[v->selected_entry].item_index].discounted_ammo_price;
                } else {
                    price = vendor_item_prices[v->entries[v->selected_entry].item_index].ammo_price;
                }
                message_id = 0x5233;
                if (current_bolt_count_macro >= price)
                    message_id = 0x5234;
            } else {
                int cost;
                if (discount_purchase_pricing[0] != 0) {
                    cost = vendor_item_prices[v->entries[v->selected_entry].item_index].discounted_purchase_price;
                } else {
                    cost = vendor_item_prices[v->entries[v->selected_entry].item_index].purchase_price;
                }
                message_id = 0x5233;
                if (current_bolt_count_macro >= cost)
                    message_id = 0x524E;
            }
        }
    } else {
        message_id = 0x5234;
    }
    if (message_id != 0) {
        region.flags |= TEXT_REGION_MEASURE_ONLY;
        font_print_window_small(&region, 0x80F0F0F0L, get_help_message_text(0x5234), -1);
        region.flags ^= TEXT_REGION_MEASURE_ONLY;
        region.anchor_y = (height - region.rendered_height) >> 1;
        font_print_window_small(&region, 0x80F0F0F0L, get_help_message_text(0x5234), -1);
    }
}
#endif /* NON_MATCHING */
