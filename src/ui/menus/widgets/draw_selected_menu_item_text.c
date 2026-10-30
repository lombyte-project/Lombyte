#include "types.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/data_labels.h"

extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern s32  do_gif_paging() __asm__("func_001F4398");
extern s32  font_print_center(s32, s32, u64, s32, s32) __asm__("func_001F6AF0");
extern s32  get_help_message_text() __asm__("func_001FDD10");
s32         draw_selected_menu_item_text(struct MenuScreen *menu) __asm__("FUN_002220f0");

s32 draw_selected_menu_item_text(struct MenuScreen *menu) {
    s32                height;
    s32                center_y;
    s32                mid_x;
    s32                lower_y;
    s32                center_x;
    s32                upper_y;
    s32                center_x2;
    s32                entry_index;
    s32                entry_offset;
    u32                menu_width;
    u32                width;
    u32                width2;
    struct MenuScreen *focus_list;

    focus_list = menu_system.current->focus;
    entry_index = focus_list->data.list.items[focus_list->data.list.selected].param.value;
    setup_gif_paging(0);
    if (entry_index == -1) {
        goto block_2;
    }
    goto block_4;
block_2:
    height = menu->height;
    width = menu->width;
    center_x = (s32)(width + ((u32)width >> 0x1F)) >> 1;
    center_y = ((s32)(height + ((u32)height >> 0x1F)) >> 1) - 8;
    font_print_center(center_x, center_y, 0x80FFA888, get_help_message_text(0x5019), -1);
    goto block_5;
block_4:
    entry_offset = entry_index * 0xC;
    menu_width = menu->width;
    mid_x = (s32)(menu_width + ((u32)menu_width >> 0x1F)) >> 1;
    upper_y = ((s32)menu->height / 3) - 8;
    font_print_center(mid_x,
                      upper_y,
                      0x80FFA888,
                      get_help_message_text(*(s32 *)(void *)(entry_offset + D_001DDD40)),
                      -1);
    width2 = menu->width;
    center_x2 = (s32)(width2 + ((u32)width2 >> 0x1F)) >> 1;
    lower_y = ((s32)(menu->height * 2) / 3) - 8;
    font_print_center(center_x2,
                      lower_y,
                      0x80FFA888,
                      get_help_message_text(*(s32 *)((u8 *)(D_001DDD40 + entry_offset) + 0x4)),
                      -1);
block_5:
    do_gif_paging();
    return 2;
}
