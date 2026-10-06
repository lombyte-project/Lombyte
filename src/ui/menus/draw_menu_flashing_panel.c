#include "types.h"

struct MenuFlashingPanel {
    u8 pad0[0x48];
    s32 active;
    s32 time;
    s32 x;
    s32 y;
    s32 width;
    s32 height;
};

struct MenuPanelOwner {
    u8 pad0[0x78];
    struct MenuFlashingPanel *flash;
};

extern s32 D_0015ED80;
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern s32 get_icon_frame(s32, s32) __asm__("func_001FF960");
extern void draw_hud_sprite_subpixel(s32, s32, s32, s32, s32, s32) __asm__("func_00200080");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 SubtractIntegerWithClamp(s32);
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64,
                               s64) __asm__("func_001F5450");

void draw_menu_flashing_panel(struct MenuPanelOwner *owner) __asm__("FUN_00223e28");

void draw_menu_flashing_panel(struct MenuPanelOwner *owner) {
    struct MenuFlashingPanel *panel;
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    s32 u;
    s32 v;
    s32 alpha;
    s32 width_adjustment;
    s32 height_adjustment;

    if (owner == 0) {
        return;
    }
    panel = owner->flash;
    if (panel == 0) {
        return;
    }
    x = panel->x;
    y = panel->y;
    width = panel->width;
    height = panel->height;
    if (x >= 0x200 || x + width < 0) {
        return;
    }
    if (!(D_0015ED80 != 0 ? y < 0x1C1 : y < 0x1A1) || y + height < 0) {
        return;
    }
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x42, 0x8000000044ULL);
    draw_hud_sprite_subpixel(get_icon_frame(0xE99E, 7), x << 4, y << 4,
                             width << 4, height << 4, 0x80);
    if (panel->active) {
        panel->time += 2;
        u = random_integer_below(200);
        v = random_integer_below(200);
        alpha = 0x80 - SubtractIntegerWithClamp(panel->time - 0x80);
        vu1_add_g_sregister(8, 0);
        alpha = alpha * 2;
        vu1_add_g_sregister(0x42, ((u64)(alpha > 0x80 ? 0x80 : alpha) << 32) | 0x68);
        draw_textured_quad(x, y, width, height, u, v, width, height, 0x808080,
                           get_effect_texture(0x1A));
        if (panel->time >= 0x100) {
            panel->active = 0;
        }
    } else if (random_integer_below(2000) == 0) {
        panel->time = 0;
        panel->active = 1;
    }
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x42, 0x8000000044ULL);
    width_adjustment = -2;
    height_adjustment = -2;
    draw_textured_quad(x, y, width, height, 0, 0, width, (height * 3) >> 1, 0x50606060,
                       get_effect_texture(0x1C));
    if (width >= 0x4C) {
        width_adjustment = -1;
    }
    if (height >= 0x4C) {
        height_adjustment = -1;
    }
    if (width >= 0x97) {
        width_adjustment = 0;
    }
    if (height >= 0x97) {
        height_adjustment = 0;
    }
    draw_textured_quad(x + 1, y + 1, width + width_adjustment, height + height_adjustment, 1, 1,
                       0x3E, 0x3E, 0x80808080, get_effect_texture(0x19));
}

extern __typeof__(draw_menu_flashing_panel) func_00223E28 __attribute__((alias("FUN_00223e28")));
