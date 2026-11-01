#include "types.h"

#include "rnc/ui/help/help_state.h"

extern u64 get_effect_texture() __asm__("func_001F44B8");
extern s32 draw_textured_quad() __asm__("func_001F5450");
extern s32 draw_ui_frame() __asm__("func_001F5F18");
void       help_draw_prompt(void) __asm__("FUN_001fe898");

void help_draw_prompt(void) {
    enum {
        HELP_STATE_FRAME_EXPAND = 1,
        HELP_STATE_FRAME_COLLAPSE = 7
    };
    s32 alpha;

    help_state.frame_half_width = 0x20;
    help_state.frame_half_height = 0x20;
    draw_ui_frame(help_state.center_y - 0x20,
                  help_state.center_y + 0x20,
                  help_state.center_x - 0x20,
                  help_state.center_x + 0x20,
                  0x60);
    if (help_state.state == HELP_STATE_FRAME_EXPAND || help_state.state == HELP_STATE_FRAME_COLLAPSE) {
        alpha = help_state.timer * 21;
    } else {
        alpha = 0x7E;
    }
    if (alpha > 0x80) {
        alpha = 0x80;
    }
    draw_textured_quad(help_state.center_x - 0x20,
                       help_state.center_y - 0x20,
                       0x40,
                       0x40,
                       0,
                       0,
                       0x40,
                       0x40,
                       (u64)(alpha << 24 | 0x808080),
                       get_effect_texture(4));
}
