#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/game/game_globals.h"

struct MenuHelpTextState {
    u8  pad_0[0x38];
    s32 saved_text_count;
    u8  pad_3C[0x14];
    s32 load_state;
    s32 saved_text_table;
};

extern s32 request_audio_stream_break() __asm__("FUN_002166e8");
extern s32 initialize_menu_stream_buffers() __asm__("func_00225AC0");
s32        restore_menu_help_text(struct MenuHelpTextState *menu) __asm__("FUN_0021d2c8");

s32 restore_menu_help_text(struct MenuHelpTextState *menu) {
    register s32 saved_text_table;
    register s32 saved_text_count;

    if ((cd_read_active[0] != 0) && (menu->load_state == 1)) {
        request_audio_stream_break();
    }
    initialize_menu_stream_buffers(1);
    saved_text_table = menu->saved_text_table;
    if (saved_text_table != 0) {
        saved_text_count = menu->saved_text_count;
        *(s32 *)0x15F6A0 = saved_text_table;
        help_msg_count[0] = saved_text_count;
    }
    return 0;
}
