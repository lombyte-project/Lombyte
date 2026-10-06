#include "types.h"
extern s32 setup_gif_paging() __asm__("func_001F4280");
extern s32 do_gif_paging() __asm__("func_001F4398");
extern s32 font_print_large(s32, s32, u64, s32, s32) __asm__("func_001F6530");
extern s32 get_help_message_text() __asm__("func_001FDD10");
extern s32 vu1_add_g_sregister() __asm__("func_00233980");
s32 draw_mission_menu_labels(void) __asm__("FUN_0021f5f8");

s32 draw_mission_menu_labels(void) {
    vu1_add_g_sregister(0x42, 0x44);
    vu1_add_g_sregister(0x47, 0xB);
    setup_gif_paging(0);
    font_print_large(4, 7, 0x80FFA888, get_help_message_text(0x4EE0), -1);
    font_print_large(4, 0x17, 0x80FFA888, get_help_message_text(0x4F05), -1);
    do_gif_paging();
    return 2;
}
