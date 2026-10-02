#include "types.h"
struct MenuHelpTextState {
    u8 pad_0[0x38];
    s32 saved_text_count;
    u8 pad_3C[0x14];
    s32 load_state;
    s32 saved_text_table;
};

extern s16 D_001516D8[];
extern s32 D_0015F6A0[];
extern s32 D_001996FC[];
extern s32 request_audio_stream_break() __asm__("FUN_002166e8");
extern s32 func_00225AC0();
s32 restore_menu_help_text(struct MenuHelpTextState *menu) __asm__("FUN_0021d2c8");

s32 restore_menu_help_text(struct MenuHelpTextState *menu) {
    register s32 saved_text_table;
    register s32 saved_text_count;

    if ((D_001516D8[0] != 0) && (menu->load_state == 1)) {
        request_audio_stream_break();
    }
    func_00225AC0(1);
    saved_text_table = menu->saved_text_table;
    if (saved_text_table != 0) {
        saved_text_count = menu->saved_text_count;
        *(s32 *)0x15F6A0 = saved_text_table;
        D_001996FC[0] = saved_text_count;
    }
    return 0;
}
