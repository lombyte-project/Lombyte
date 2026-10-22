#include "types.h"
#include "rnc/rendering/fs_aa_buffer.h"

struct Metrics {
    u8 pad0[4];
    s32 color;
    u64 enable;
    s32 unk10;
    s32 unk14;
    u64 unk18;
    s32 unk20;
    s32 unk24;
    u64 unk28;
};

extern struct Metrics *screen_color_effect_now __asm__("D_0015F350") __attribute__((sda));
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern void draw_rect_overlay(s32, s32, s32, s32, u64) __asm__("func_001F52A0");

void draw_screen_effect(void) __asm__("FUN_001f4fb8");

void draw_screen_effect(void) {
    s32 i;
    s16 height;
    u64 mask;
    s32 background_color;
    u64 background_alpha_state;
    s32 first_stripe_color;
    u64 first_stripe_alpha_state;
    s32 second_stripe_color;
    u64 second_stripe_alpha_state;

    i = 0;
    height = fs_aa_buffer.display_height;
    background_alpha_state = screen_color_effect_now->enable;
    if (background_alpha_state != 0) {
        vu1_add_g_sregister(0x42, background_alpha_state & 0xFF000000FFULL);
    }
    background_color = screen_color_effect_now->color;
    if (background_color & 0xFF000000) {
        draw_rect_overlay(0, height, 0, fs_aa_buffer.display_width, (u64)((s64)background_color << 0x20) >> 0x20);
    }
    if (height > 0) {
        mask = 0xFF000000FFULL;
        do {
            first_stripe_alpha_state = screen_color_effect_now->unk18;
            if (first_stripe_alpha_state != 0) {
                vu1_add_g_sregister(0x42, first_stripe_alpha_state & mask);
            }
            first_stripe_color = screen_color_effect_now->unk14;
            if (first_stripe_color & 0xFF000000) {
                draw_rect_overlay(i,
                                  (i + screen_color_effect_now->unk10 < height - 1) ? i + screen_color_effect_now->unk10 : height - 1,
                                  0, fs_aa_buffer.display_width, (u64)((s64)first_stripe_color << 0x20) >> 0x20);
            }
            i = i + screen_color_effect_now->unk10;
            second_stripe_alpha_state = screen_color_effect_now->unk28;
            if (second_stripe_alpha_state != 0) {
                vu1_add_g_sregister(0x42, second_stripe_alpha_state & mask);
            }
            second_stripe_color = screen_color_effect_now->unk24;
            if (second_stripe_color & 0xFF000000) {
                draw_rect_overlay(i,
                                  (i + screen_color_effect_now->unk20 < height - 1) ? i + screen_color_effect_now->unk20 : height - 1,
                                  0, fs_aa_buffer.display_width, (u64)((s64)second_stripe_color << 0x20) >> 0x20);
            }
            i = i + screen_color_effect_now->unk20;
        } while (i < height);
    }
}

extern __typeof__(draw_screen_effect) func_001F4FB8 __attribute__((alias("FUN_001f4fb8")));
/* Recovered original symbol name. */
extern __typeof__(draw_screen_effect) DrawScreenEffect__Fv __attribute__((alias("FUN_001f4fb8")));
