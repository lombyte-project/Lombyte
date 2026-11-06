#include "types.h"
#include "rnc/globals.h"
#include "rnc/ui/vendor/vendor_capture.h"

struct Slot {
    s32 id;
    s32 kind;
    u8  pad8[0xC];
};

struct Shop {
    u8          pad0[0x40];
    s32         hard;
    u8          pad44[0x14];
    s32         cur;
    s32         open;
    u8          pad60[0x70];
    struct Slot slots[1];
};

extern struct Shop D_001E63C0;

#include "rnc/game/item_state.h"

extern void draw_framebuffer_rect(s32, s32, s32, s32, s32, s32, u32) __asm__("func_001FB8F0");
extern s32  get_help_message_text(s32) __asm__("func_001FDD10");
extern void font_print_center_small(s32, s32, u64, s32, s32) __asm__("func_001F6B88");

void render_vendor_purchase_prompt_pass(void) __asm__("FUN_00239160");

void render_vendor_purchase_prompt_pass(void) {
    struct Shop *vendor_shop;
    s32          id;
    s32          cost;

    draw_framebuffer_rect(0, 0, 0x200, 0x80, 0x200, 0x80, 0);
    vendor_shop = &D_001E63C0;
    if (vendor_shop->open != 0) {
        if (vendor_shop->slots[vendor_shop->cur].kind == 1) {
            if (weapon_ammo_counts[vendor_shop->slots[vendor_shop->cur].id] >=
                vendor_item_prices[vendor_shop->slots[vendor_shop->cur].id].ammo_capacity) {
                return;
            }
        }
        if (vendor_shop->slots[vendor_shop->cur].kind == 1) {
            if (vendor_shop->hard != 0) {
                cost = vendor_item_prices[vendor_shop->slots[vendor_shop->cur].id]
                           .discounted_ammo_price;
            } else {
                cost = vendor_item_prices[vendor_shop->slots[vendor_shop->cur].id].ammo_price;
            }
            if (current_bolt_count < cost) {
                return;
            }
            id = 0x4EE0;
        } else {
            if (item_available[ITEM_VENDOR_DISCOUNT] != 0) {
                cost = vendor_item_prices[vendor_shop->slots[vendor_shop->cur].id]
                           .discounted_purchase_price;
            } else {
                cost = vendor_item_prices[vendor_shop->slots[vendor_shop->cur].id].purchase_price;
            }
            if (current_bolt_count < cost) {
                return;
            }
            id = 0x524B;
        }
        font_print_center_small(0x28, 0x14, 0x80F0F0F0, get_help_message_text(id), -1);
    } else {
        font_print_center_small(0x28, 0x14, 0x80F0F0F0, get_help_message_text(0x4EE0), -1);
    }
}

extern __typeof__(render_vendor_purchase_prompt_pass) func_00239160
    __attribute__((alias("FUN_00239160")));
