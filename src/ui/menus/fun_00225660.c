#include "types.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/audio/music/music_stream_state.h"
extern s32 delete_moby() __asm__("FUN_00225530");
extern s32 complete_stream_buffer_transfer() __asm__("func_00225CD8");
s32 FUN_00225660(struct MenuScreen *menu) {
    s32 *slot;
    s32 remaining;

    remaining = 0x17;
    slot = ((u8 *)menu + (0x44));
    do {
        remaining -= 1;
        *slot = delete_moby(*slot);
        slot += 1;
    } while (remaining >= 0);
    menu->data.raw.unk3C = complete_stream_buffer_transfer(menu->data.raw.unk3C);
    if ((u32)((u16)music_stream_state.secondary.state - 6) >= 2U) {
        music_stream_state.secondary.state = 5;
    }
    return 0;
}
