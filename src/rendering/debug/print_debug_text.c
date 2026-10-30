#include "types.h"

struct DebugText {
    s32   x;
    s32   y;
    s32   color;
    char *text;
};

/* The debug-text cursor this file owns: the function below reaches both
   gp-relative, which needs their definitions ahead of it. */
char                   *font_text_cursor __asm__("D_0015F000") = (char *)0x0018A300;
s32                     font_text_slot_index __asm__("D_0015F004") = 0;
extern struct DebugText font_text_slots[] __asm__("D_0018AB00");
extern char             font_text_format[] __asm__("D_0015F008");
extern s32              sprintf(char *, const char *, ...);
void print_debug_text(s32 x, s32 y, s32 color, char *text) __asm__("FUN_001f0bd0");

void print_debug_text(s32 x, s32 y, s32 color, char *text) {
    font_text_slots[font_text_slot_index].x = x;
    font_text_slots[font_text_slot_index].y = y;
    font_text_slots[font_text_slot_index].color = color;
    font_text_slots[font_text_slot_index].text = font_text_cursor;
    font_text_slot_index++;
    font_text_cursor += sprintf(font_text_cursor, font_text_format, text) + 1;
}

extern __typeof__(print_debug_text) func_001F0BD0 __attribute__((alias("FUN_001f0bd0")));
