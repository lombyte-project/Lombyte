#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x44];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
};

extern void setup_gif_paging() __asm__("func_001F4280");
extern void do_gif_paging() __asm__("func_001F4398");
extern void draw_textured_quad() __asm__("func_001F5450");
extern s64 func_00204CF0();
s32 draw_two_texture_panels(struct M2c_arg0 *arg0) __asm__("FUN_00220790");

s32 draw_two_texture_panels(struct M2c_arg0 *arg0) {
    s64 temp_16_14;

    if (arg0->unk44 < 4) {
        return 0;
    }
    temp_16_14 = (s64)0x80808080;
    setup_gif_paging(0);
    draw_textured_quad(0, 0, 0x100, 0x100, 0, 0, 0x100, 0x100, temp_16_14,
                       func_00204CF0(arg0->unk48));
    draw_textured_quad(0x100, 0, 0x100, 0x100, 0, 0, 0x100, 0x100, temp_16_14,
                       func_00204CF0(arg0->unk4C));
    do_gif_paging();
    return 8;
}
