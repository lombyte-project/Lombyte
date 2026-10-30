#include "types.h"
#include "rnc/ui/menus/menu_globals.h"

struct HudMenuState {
    u8  pad_0[0x4];
    s32 unk4;
    u8  pad_8[0x44];
    s32 unk4C;
    u8  pad_50[0x8];
    s32 unk58;
    u8  pad_5C[0x1B4];
    s32 unk210;
};

struct HudItemEntry {
    u8  pad_0[0x38];
    u16 unk38;
};

extern struct HudMenuState D_001E63C0;
extern s32                 SubtractIntegerWithClamp();
extern void                draw_framebuffer_rect() __asm__("func_001FB8F0");
extern s32                 get_icon_frame() __asm__("func_001FF960");
extern void                draw_hud_sprite() __asm__("func_001FFC30");
void                       render_vendor_item_icon_strip_pass(void) __asm__("FUN_002386e8");

void render_vendor_item_icon_strip_pass(void) {
    s32  slot_x;
    s32  entry_offset;
    s32  first_slot;
    s32  strip_x;
    s32  slot;
    s32  item_index;
    s32  sprite_x;
    s32  strip_frame;
    s32  frame_index;
    u16  icon;
    u16  item_icon;
    s32 *item;
    s32  fill_color;

    draw_framebuffer_rect(0, 0, 0x200, 0x80, 0x200, 0x80, 0);
    if (D_001E63C0.unk210 < 8) {
        sprite_x = 0xC;
        item_index = 0;
        slot_x = D_001E63C0.unk58 * 0x38;
        fill_color =
            ((SubtractIntegerWithClamp(((D_001E63C0.unk4 * 4) & 0x3F) - 0x20) + 0x40) * 0x10202) |
            0x80000000;
        draw_framebuffer_rect(slot_x + 8, 2, slot_x + 0x40, 0x3A, 0x200, 0x80, fill_color);
        draw_framebuffer_rect(slot_x + 0xA, 4, slot_x + 0x3E, 0x38, 0x200, 0x80, 0x80000000);
        if (D_001E63C0.unk210 > 0) {
            do {
                item = (s32 *)(void *)((u8 *)&D_001E63C0 + 0xD4 + item_index * 0x14);
                item_icon = ((struct HudItemEntry *)(void *)((u8 *)((u8 *)menu_item_info) +
                                                             item[-1] * 0x4C))
                                ->unk38;
                if (item[0] == 1) {
                    frame_index = get_icon_frame(item_icon, 2);
                } else {
                    frame_index = get_icon_frame(item_icon, 0);
                }
                draw_hud_sprite(frame_index, sprite_x, 6, 0x30, 0x30, 0x80);
                sprite_x += 0x38;
                item_index += 1;
            } while (item_index < D_001E63C0.unk210);
        }
    } else {
        if (D_001E63C0.unk4C < 0) {
            D_001E63C0.unk4C = (s32)(D_001E63C0.unk4C + 4);
        } else if (D_001E63C0.unk4C > 0) {
            D_001E63C0.unk4C = (s32)(D_001E63C0.unk4C - 4);
        } else {
            draw_framebuffer_rect(
                0xB0,
                2,
                0xE8,
                0x3A,
                0x200,
                0x80,
                ((SubtractIntegerWithClamp(((D_001E63C0.unk4 * 4) & 0x3F) - 0x20) + 0x40) *
                 0x10202) -
                    (s32)0x80000000);
            draw_framebuffer_rect(0xB2, 4, 0xE6, 0x38, 0x200, 0x80, 0x80000000);
        }
        strip_x = D_001E63C0.unk4C - 0x64;
        slot = -2;
        do {
            first_slot = D_001E63C0.unk58 - 3;
            entry_offset =
                ((s32)((D_001E63C0.unk210 * 2) + slot + first_slot) % (s32)D_001E63C0.unk210) *
                0x14;
            icon = ((struct HudItemEntry *)(void *)((u8 *)((u8 *)menu_item_info) +
                                                    *(s32 *)(void *)(entry_offset +
                                                                     ((u8 *)&D_001E63C0 + 0xD0)) *
                                                        0x4C))
                       ->unk38;
            if (*(s32 *)(void *)(entry_offset + ((u8 *)&D_001E63C0 + 0xD4)) == 1) {
                strip_frame = get_icon_frame(icon, 2);
            } else {
                strip_frame = get_icon_frame(icon, 0);
            }
            slot += 1;
            draw_hud_sprite(strip_frame, strip_x, 6, 0x30, 0x30, 0x80808080);
            strip_x += 0x38;
        } while (slot < 9);
    }
}
