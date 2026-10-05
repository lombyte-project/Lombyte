#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/inventory/draw_items_menu/FUN_0021eb20.s", FUN_0021eb20);
#else
#include "types.h"

typedef struct { s16 s[12]; } FontWindow;

#include "types.h"

typedef struct 
{
  u8 pad0[0x20];
  s32 width;
  u16 height;
  u8 pad26[0x1E];
  s32 help_tip;
} ItemsMenu;
struct ScreenDimensions
{
  s32 pad[1];
  s32 height;
};
extern struct ScreenDimensions D_0013E500[];



extern s32 D_0015ED88[] __attribute__((section(".sdata")));



extern s32 D_001601B8 __attribute__((sda));



extern s32 D_001601BC __attribute__((sda));
extern char D_001602A0[];
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");



extern void font_print_right(s32, s32, u64, char *, s32) __asm__("FUN_001f6940");



extern void font_print_left(s32, s32, u64, char *, s32) __asm__("FUN_001f6a60");
extern void font_print_window_regular(FontWindow *, u64, char *, s32) __asm__("func_001F7580");
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern void func_00200E08(s32, s32, s32, s32, u64, s32);
extern void draw_moby_list(s32, s32) __asm__("func_0020D330");
extern s32 compute_clamped_count_difference(void) __asm__("func_00215248");
extern s32 count_nonzero_entries_up_to_40(void) __asm__("func_00215290");
extern s32 count_nonzero_entries_up_to_10(void) __asm__("func_00215300");
extern void *memset(void *, s32, u32);
extern int sprintf(char *, const char *, ...);
extern char *strchr(const char *, int);
extern char *strcpy(char *, const char *);
extern u32 strlen(const char *);
s32 draw_items_menu(ItemsMenu *menu);
static inline int add_offset(s32 arg0, int arg1)
{
  return arg0 + arg1;
}
static inline char *add_string_offset(char *a, int n)
{
  return a + n;
}
static inline int divide_coordinate(s32 a, int n)
{
  return a / n;
}

s32 draw_items_menu(ItemsMenu *menu) __asm__("FUN_0021eb20");

s32 draw_items_menu(ItemsMenu *menu)
{
  char text_buffer[0x80];
  char *hyphen;
  char *source_end;
  char *destination_end;
  char *format;
  s32 text_length;
  s32 column_divisor;
  s32 x;
  s32 y;
  s32 collected_count;
  if (menu->help_tip != 0)
  {
    draw_moby_list(menu->help_tip, 1);
  }
  setup_gif_paging(0);
  column_divisor = 3;
  {
    FontWindow text_window = {{0, menu->height, 8, menu->width / column_divisor, 0, 0, 0, 0, 0x10, 5}};
    text_window.s[4] = add_offset(text_window.s[2], text_window.s[3]) >> 1;
    strcpy(text_buffer, get_help_message_text(0x4F4E));
    if (D_0015ED88[0] == column_divisor)
    {
      hyphen = strchr(text_buffer, 0x2D);
      if (hyphen != 0)
      {
        text_length = strlen(text_buffer);
        source_end = text_buffer + text_length;
        for (destination_end = source_end; hyphen < source_end; source_end = destination_end)
        {
          destination_end[1] = *source_end;
          destination_end--;
          text_length--;
        }
        text_buffer[add_offset(text_length, 1)] = 0x20;
      }
    }
    font_print_window_regular(&text_window, 0x8000C0C0L, text_buffer, -1);
    text_window.s[9] ^= 4;
    text_window.s[5] = (D_0013E500[0].height - text_window.s[7]) >> 1;
    font_print_window_regular(&text_window, 0x8000C0C0L, text_buffer, -1);
  }
  x = add_offset(D_001601B8, 0xC8);
  y = add_offset(D_001601BC, 0x1D);
  font_print_right(x, y, 0x80000000L, get_help_message_text(0x4F4F), -1);
  x = add_offset(D_001601B8, 0xC8);
  y = add_offset(D_001601BC, 0x36);
  font_print_right(x, y, 0x80000000L, get_help_message_text(0x4F50), -1);
  x = add_offset(D_001601B8, 0xC8);
  y = add_offset(D_001601BC, 0x54);
  font_print_right(x, y, 0x80000000L, get_help_message_text(0x4F51), -1);
  font_print_right(0xC8, 0x1D, 0x80FFA888L, get_help_message_text(0x4F4F), -1);
  font_print_right(0xC8, 0x36, 0x80FFA888L, get_help_message_text(0x4F50), -1);
  font_print_right(0xC8, 0x54, 0x80FFA888L, get_help_message_text(0x4F51), -1);
  collected_count = count_nonzero_entries_up_to_40();
  sprintf(text_buffer, D_001602A0, collected_count);
  font_print_left(add_offset(D_001601B8, 0xF0), add_offset(D_001601BC, 0x1D), 0x80000000L, text_buffer, -1);
  font_print_left(0xF0, 0x1D, 0x80FFA888L, text_buffer, -1);
  sprintf(text_buffer, D_001602A0, count_nonzero_entries_up_to_10() * 4);
  font_print_left(add_offset(D_001601B8, 0xF0), add_offset(D_001601BC, 0x36), 0x80000000L, text_buffer, -1);
  font_print_left(0xF0, 0x36, 0x80FFA888L, text_buffer, -1);
  sprintf(text_buffer, D_001602A0, compute_clamped_count_difference());
  font_print_left(add_offset(D_001601B8, 0xF0), add_offset(D_001601BC, 0x54), 0x80000000L, text_buffer, -1);
  font_print_left(0xF0, 0x54, 0x80FFA888L, text_buffer, -1);
  func_00200E08(add_offset(D_001601B8, 0xD0), add_offset(D_001601BC, 0x4D), add_offset(D_001601B8, 0xF2), add_offset(D_001601BC, 0x50), 0x80000000L, 0);
  func_00200E08(0xD0, 0x4D, 0xF2, 0x50, 0x80FFA888L, 0);
  do_gif_paging();
  return 8;
}
extern __typeof__(draw_items_menu) func_0021EB20 __attribute__((alias("FUN_0021eb20")));
#endif /* NON_MATCHING */
