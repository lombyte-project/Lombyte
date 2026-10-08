#include "types.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/ui/menus/menu_system.h"
extern u8 D_001DDD40[];
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern s32 do_gif_paging() __asm__("func_001F4398");
extern s32 font_print_center(s32, s32, u64, s32, s32) __asm__("func_001F6AF0");
extern s32 get_help_message_text() __asm__("func_001FDD10");
s32 FUN_002220f0(struct MenuScreen *menu) {
    s32 height;
    s32 center_y;
    s32 temp_16_61;
    s32 temp_16_88;
    s32 center_x;
    s32 temp_17_64;
    s32 temp_17_85;
    s32 temp_20_22;
    s32 temp_20_48;
    u32 temp_16_50;
    u32 width;
    u32 temp_17_75;
    struct MenuScreen *temp_3_16;

    temp_3_16 = menu_system.current->focus;
    temp_20_22 = temp_3_16->data.list.items[temp_3_16->data.list.selected].param.value;
    setup_gif_paging(0);
    if (temp_20_22 == -1) {
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
    temp_20_48 = temp_20_22 * 0xC;
    temp_16_50 = menu->width;
    temp_16_61 = (s32)(temp_16_50 + ((u32)temp_16_50 >> 0x1F)) >> 1;
    temp_17_64 = ((s32)menu->height / 3) - 8;
    font_print_center(temp_16_61, temp_17_64, 0x80FFA888,
                      get_help_message_text(*(s32 *)(void *)(temp_20_48 + D_001DDD40)), -1);
    temp_17_75 = menu->width;
    temp_17_85 = (s32)(temp_17_75 + ((u32)temp_17_75 >> 0x1F)) >> 1;
    temp_16_88 = ((s32)(menu->height * 2) / 3) - 8;
    font_print_center(temp_17_85, temp_16_88, 0x80FFA888,
                      get_help_message_text(*(s32 *)((u8 *)(D_001DDD40 + temp_20_48) + 0x4)), -1);
block_5:
    do_gif_paging();
    return 2;
}
