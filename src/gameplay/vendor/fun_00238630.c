#include "types.h"
extern s32 D_001610A8[];
extern s32 D_001E63E4[];
extern s32 draw_framebuffer_rect() __asm__("func_001FB8F0");
extern s32 draw_moby_list() __asm__("func_0020D330");
void FUN_00238630(void) {
    draw_framebuffer_rect(0, 0, 0x200, 0x80, 0x200, 0x80, 0);
    if (*(s32 *)0x1610A8 == 1) {
        draw_moby_list(D_001E63E4[0], 1);
    }
}
