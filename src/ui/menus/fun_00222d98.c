#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/storage/memory_card/memory_card_state.h"
#include "rnc/ui/menus/menu_screen.h"
extern u8 D_001DDD40[];
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern s32 do_gif_paging() __asm__("func_001F4398");
extern s32 font_print_center(s32, s32, u64, s32, s32) __asm__("func_001F6AF0");
extern s32 get_help_message_text() __asm__("func_001FDD10");
s32 FUN_00222d98(struct MenuScreen *menu) {
    s32 center_y;
    s32 temp_16_80;
    s32 temp_17_21;
    s32 center_x;
    s32 temp_18_57;
    s32 temp_16_62;
    u32 height;
    u32 temp_16_56;
    u32 temp_16_74;
    u32 width;
    struct MemoryCardState *base;
    base = &memory_card_state;
    temp_17_21 = *((s32 *)((((u8 *)base) - (-(menu_system.current->focus->data.save.slot * 0x1C))) + 0x20));
    setup_gif_paging(0);
    if (((base->state < 3) && (base->pending_state < 0)) && (base->card[0].type == 2)) {
        if (temp_17_21 == (-1)) {
            height = menu->height;
            width = menu->width;
            center_x = ((s32)(width + (((u32)width) >> 0x1F))) >> 1;
            center_y = (((s32)(height + (((u32)height) >> 0x1F))) >> 1) - 8;
            font_print_center(center_x, center_y, 0x80FFA888, get_help_message_text(0x5217),
                              -1);
        } else {
            temp_16_56 = menu->width;
            temp_18_57 = temp_17_21 * 0xC;
            temp_16_62 = ((s32)(temp_16_56 + ((((u32)temp_16_56) >> 24) >> 7))) >> 1;
            font_print_center(temp_16_62, 4, 0x80FFA888,
                              get_help_message_text(*((s32 *)((void *)(temp_18_57 + D_001DDD40)))),
                              -1);
            temp_16_74 = menu->width;
            if (1) {
                temp_16_80 = ((s32)(temp_16_74 + (((u32)temp_16_74) >> 0x1F))) >> 1;
                font_print_center(
                    temp_16_80, 0x14, 0x80FFA888,
                    get_help_message_text(*((s32 *)(((u8 *)(D_001DDD40 + temp_18_57)) + 0x4))), -1);
            }
        }
    }
    do_gif_paging();
    return 2;
}
