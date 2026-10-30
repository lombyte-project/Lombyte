#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/data_labels.h"

extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern s32  do_gif_paging() __asm__("func_001F4398");
extern s32  font_print_center(s32, s32, u64, s32, s32) __asm__("func_001F6AF0");
extern s32  get_help_message_text() __asm__("func_001FDD10");
s32         draw_menu_selection_text(struct MenuScreen *menu) __asm__("FUN_00222d98");

s32 draw_menu_selection_text(struct MenuScreen *menu) {
    s32                     center_y;
    s32                     center_x2;
    s32                     entry_index;
    s32                     center_x;
    s32                     entry_offset;
    s32                     mid_x;
    u32                     height;
    u32                     menu_width;
    u32                     width2;
    u32                     width;
    struct MemoryCardState *base;
    base = &memory_card_state;
    entry_index =
        *((s32 *)((((u8 *)base) - (-(menu_system.current->focus->data.save.slot * 0x1C))) + 0x20));
    setup_gif_paging(0);
    if (((base->state < 3) && (base->pending_state < 0)) && (base->card[0].type == 2)) {
        if (entry_index == (-1)) {
            height = menu->height;
            width = menu->width;
            center_x = ((s32)(width + (((u32)width) >> 0x1F))) >> 1;
            center_y = (((s32)(height + (((u32)height) >> 0x1F))) >> 1) - 8;
            font_print_center(center_x, center_y, 0x80FFA888, get_help_message_text(0x5217), -1);
        } else {
            menu_width = menu->width;
            entry_offset = entry_index * 0xC;
            mid_x = ((s32)(menu_width + ((((u32)menu_width) >> 24) >> 7))) >> 1;
            font_print_center(
                mid_x,
                4,
                0x80FFA888,
                get_help_message_text(*((s32 *)((void *)(entry_offset + D_001DDD40)))),
                -1);
            width2 = menu->width;
            if (1) {
                center_x2 = ((s32)(width2 + (((u32)width2) >> 0x1F))) >> 1;
                font_print_center(
                    center_x2,
                    0x14,
                    0x80FFA888,
                    get_help_message_text(*((s32 *)(((u8 *)(D_001DDD40 + entry_offset)) + 0x4))),
                    -1);
            }
        }
    }
    do_gif_paging();
    return 2;
}
