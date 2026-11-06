#include "types.h"
#include "rnc/globals.h"

/* Ported from rac1-decomp (src/game/vendor.c: func_0023A220). */
#include "sda.h"
#include "rnc/game/item_state.h"
#include "rnc/ui/text/text_region.h"
#include "rnc/ui/vendor/vendor_capture.h"

struct VendorSelectionEntry {
    s32 item_index;
    s32 purchase_kind;
    u8  pad8[0xC];
};

struct VendorState {
    u8                          pad0[0x40];
    s32                         discount_ammo_pricing;
    u8                          pad44[0x14];
    s32                         selected_entry;
    s32                         selection_active;
    u8                          pad60[0x70];
    struct VendorSelectionEntry entries[1];
};

extern struct VendorState vendor_state __asm__("D_001E63C0");
extern s32                current_bolt_count_macro __asm__("D_0015ED98") MACRO_ADDR;
extern void  draw_framebuffer_rect(int, int, int, int, int, int, int) __asm__("func_001FB8F0");
extern void *memset(void *, int, unsigned int) __asm__("func_001153FC");
extern void *get_help_message_text(int) __asm__("func_001FDD10");
extern void  font_print_window_small(void *, long, void *, int) __asm__("func_001F75F0");

void render_vendor_buy_label_pass(char *unused, int width, int height) __asm__("FUN_00238f08");

void render_vendor_buy_label_pass(char *unused, int width, int height) {
    struct TextRegion   region;
    struct VendorState *vendor_state_ptr;
    int                 price;
    long                message_id;
    int                 stride = sizeof(struct VendorItemPricing);

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
                ((struct VendorItemPricing
                      *)((char *)vendor_item_prices +
                         vendor_state.entries[vendor_state.selected_entry].item_index * stride))
                    ->ammo_capacity) {
            message_id = 0x5233;
        } else {
            vendor_state_ptr = &vendor_state;
            if (vendor_state_ptr->entries[vendor_state_ptr->selected_entry].purchase_kind == 1) {
                if (vendor_state_ptr->discount_ammo_pricing != 0) {
                    price =
                        vendor_item_prices
                            [vendor_state_ptr->entries[vendor_state_ptr->selected_entry].item_index]
                                .discounted_ammo_price;
                } else {
                    price =
                        vendor_item_prices
                            [vendor_state_ptr->entries[vendor_state_ptr->selected_entry].item_index]
                                .ammo_price;
                }
                message_id = 0x5233;
                if (current_bolt_count_macro >= price)
                    message_id = 0x5234;
            } else {
                int cost;
                if (item_available[ITEM_VENDOR_DISCOUNT] != 0) {
                    cost =
                        vendor_item_prices
                            [vendor_state_ptr->entries[vendor_state_ptr->selected_entry].item_index]
                                .discounted_purchase_price;
                } else {
                    cost =
                        vendor_item_prices
                            [vendor_state_ptr->entries[vendor_state_ptr->selected_entry].item_index]
                                .purchase_price;
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

extern __typeof__(render_vendor_buy_label_pass) func_00238F08
    __attribute__((alias("FUN_00238f08")));

struct VendorItemPricing vendor_item_prices[43] = {
    {0}, /* item 0 */
    {0}, /* item 1 */
    {1000, 1000, 0, 0, 0, 0, 0, 0, 0}, /* item 2 */
    {2000, 2000, 0, 0, 0, 0, 0, 0, 0}, /* item 3 */
    {2000, 2000, 0, 0, 0, 0, 0, 0, 0}, /* item 4 */
    {0}, /* item 5 */
    {0}, /* item 6 */
    {0}, /* item 7 */
    {0}, /* item 8 */
    {0, 0, 0, 0, 0, 0, 0, 0, 10000}, /* item 9 */
    {0, 0, 5, 25, 3, 40, 10, 10, 20000}, /* item 10 */
    {10000, 7500, 50, 100, 2, 20, 10, 10, 60000}, /* item 11 */
    {1000, 1000, 0, 0, 0, 0, 0, 0, 0}, /* item 12 */
    {15000, 10000, 100, 200, 1, 20, 10, 10, 0}, /* item 13 */
    {2500, 1250, 0, 0, 0, 0, 0, 0, 0}, /* item 14 */
    {2500, 1250, 1, 10, 25, 200, 100, 100, 20000}, /* item 15 */
    {2500, 1250, 1, 5, 20, 240, 120, 120, 30000}, /* item 16 */
    {7500, 5000, 5, 15, 5, 50, 25, 25, 10000}, /* item 17 */
    {7500, 5000, 0, 0, 0, 0, 0, 0, 0}, /* item 18 */
    {40000, 30000, 1, 10, 20, 240, 120, 120, 60000}, /* item 19 */
    {7500, 5000, 40, 100, 1, 10, 3, 3, 10000}, /* item 20 */
    {0, 0, 0, 0, 0, 0, 0, 0, 20000}, /* item 21 */
    {0}, /* item 22 */
    {150000, 150000, 20, 50, 1, 50, 25, 25, 0}, /* item 23 */
    {7500, 5000, 40, 100, 1, 10, 3, 3, 0}, /* item 24 */
    {7500, 5000, 10, 50, 2, 20, 10, 10, 10000}, /* item 25 */
    {0}, /* item 26 */
    {0}, /* item 27 */
    {0}, /* item 28 */
    {2000, 2000, 0, 0, 0, 0, 0, 0, 0}, /* item 29 */
    {0}, /* item 30 */
    {0}, /* item 31 */
    {1000, 1000, 0, 0, 0, 0, 0, 0, 0}, /* item 32 */
    {0}, /* item 33 */
    {0}, /* item 34 */
    {0}, /* item 35 */
    {0}, /* item 36 */
    {500, 500, 0, 0, 0, 0, 0, 0, 0}, /* item 37 */
    {4000, 4000, 0, 0, 0, 0, 0, 0, 0}, /* item 38 */
    {2000, 2000, 0, 0, 0, 0, 0, 0, 0}, /* item 39 */
    {4000, 4000, 0, 0, 0, 0, 0, 0, 0}, /* item 40 */
    {30000, 30000, 0, 0, 0, 0, 0, 0, 0}, /* item 41 */
    {2000, 2000, 0, 0, 0, 0, 0, 0, 0}, /* item 42 */
};
