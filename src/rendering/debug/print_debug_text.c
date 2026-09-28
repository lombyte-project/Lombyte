#include "types.h"
struct DebugText { s32 x; s32 y; s32 color; char *text; };
/* The debug-text cursor this file owns: the function below reaches both
   gp-relative, which needs their definitions ahead of it. */
char *D_0015F000 = (char *)0x0018A300;
s32 D_0015F004 = 0;
extern struct DebugText D_0018AB00[];
extern char D_0015F008[];
extern s32 sprintf(char *, const char *, ...);
void print_debug_text(s32 x, s32 y, s32 color, char *text) __asm__("FUN_001f0bd0");

void print_debug_text(s32 x, s32 y, s32 color, char *text) {
    D_0018AB00[D_0015F004].x = x;
    D_0018AB00[D_0015F004].y = y;
    D_0018AB00[D_0015F004].color = color;
    D_0018AB00[D_0015F004].text = D_0015F000;
    D_0015F004++;
    D_0015F000 += sprintf(D_0015F000, D_0015F008, text) + 1;
}

extern __typeof__(print_debug_text) func_001F0BD0 __attribute__((alias("FUN_001f0bd0")));
