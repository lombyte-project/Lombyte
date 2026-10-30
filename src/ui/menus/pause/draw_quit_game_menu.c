typedef signed char        s8;
typedef unsigned char      u8;
typedef signed short       s16;
typedef unsigned short     u16;
typedef signed int         s32;
typedef unsigned int       u32;
typedef signed long long   s64;
typedef unsigned long long u64;
typedef float              f32;
typedef double             f64;

struct QuitMenu {
    u8  pad_0[0x20];
    s32 unk20;
    s32 unk24;
};

extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging() __asm__("func_001F4398");
extern s32  measure_text_width_regular() __asm__("func_001F6250");
extern void font_print_large(s32, s32, u64, s32, s32) __asm__("func_001F6530");
extern void font_print_center(s32, s32, u64, s32, s32) __asm__("func_001F6AF0");
extern void font_print_window_regular(s32, u64, s32, s32) __asm__("func_001F7580");
extern s32  get_help_message_text() __asm__("func_001FDD10");
extern void vu1_add_g_sregister() __asm__("func_00233980");
extern void memset() __asm__("func_001153FC");
s32         draw_quit_game_menu(struct QuitMenu *quit_menu) __asm__("FUN_0021e890");

s32 draw_quit_game_menu(struct QuitMenu *quit_menu) {
    unsigned long new_var;
    s16           packet[0xC];
    s32           no_width;
    s32           menu_height;
    register s32  text_x;
    s32           half_width;
    s32           yes_width;
    s32           menu_width;
    menu_width = quit_menu->unk20;
    menu_height = quit_menu->unk24;
    memset(packet, 0, 0x18);
    half_width = (s32)(menu_width + (((u32)menu_width) >> 0x1F));
    half_width = half_width >> 1;
    packet[1] = menu_height;
    packet[2] = 4;
    packet[3] = menu_width - 4;
    packet[4] = half_width;
    packet[5] = 6;
    packet[6] = menu_width;
    packet[8] = 16;
    packet[9] = 1;
    packet[7] = menu_height;
    setup_gif_paging(0);
    new_var = 0x2004B;
    vu1_add_g_sregister(0x47, new_var);
    font_print_window_regular(packet, 0x80FFA888, get_help_message_text(0x4F6D), -1);
    font_print_center(
        half_width, menu_height - 0x40, 0x80FFA888, get_help_message_text(0x4F3F), -1);
    no_width = measure_text_width_regular(get_help_message_text(0x524B), -1);
    yes_width = measure_text_width_regular(get_help_message_text(0x524F), -1);
    text_x = ((s32)(quit_menu->unk20 - ((yes_width >= no_width) ? (yes_width) : (no_width)))) >> 1;
    font_print_large(text_x, menu_height - 0x28, 0x80FFA888, get_help_message_text(0x524B), -1);
    font_print_large(text_x, menu_height - 0x14, 0x80FFA888, get_help_message_text(0x524F), -1);
    do_gif_paging();
    return 2;
}
