#include "types.h"
#include "rnc/ui/hud/hud_state.h"
#include "rnc/globals.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"

struct HudSlot {
    u8  pad_0[0x18];
    s32 unk18;
};

extern s32 hud_msg_count __asm__("D_0015F680");
extern u8  hud_msg_text1[] __asm__("D_001993D8");
extern u8  hud_msg_text2[] __asm__("D_00199428");
extern s32 scale_game_frames() __asm__("func_001F96F8");
extern s32 draw_framed_text() __asm__("func_00201200");
void       update_hud(void) __asm__("FUN_001ff780");

extern s32 D_0015F684;

#include "rnc/ui/hud/update_hud_globals.h"

void update_hud(void) {
    s32   update_fn;
    short new_var2;
    u8   *slot;
    s32   text_color;
    s32   raised_alpha;
    s32   lowered_alpha;
    s32   remaining;
    if (hud_state.unk30 == 0) {
        goto block_2;
    }
    hud_state.unk30 = 0;
    return;
block_2:
    if (occl_chain_active == 0) {
        goto block_5;
    }

    hud_state.unk30 = 0;
    return;
block_5:
    slot = hud_chan_slots;

    hud_state.z = 0xFFFFF0;
    remaining = 0xC;
loop_6:
    update_fn = ((struct HudSlot *)slot)->unk18;

    if (update_fn == 0) {
        goto block_8;
    }
    ((s32(*)())update_fn)(slot);
block_8:
    remaining -= 1;

    slot += 0x90;
    if (remaining >= 0) {
        goto loop_6;
    }
    if (hud_msg_count != 0) {
        goto block_11;
    }
    if (D_0015F684 == 0) {
        goto block_27;
    }
block_11:
    if (game_mode != 0) {
        goto block_28;
    }

    if (hud_msg_count == 0) {
        goto block_15;
    }
    raised_alpha = D_0015F684 + (0x80 / scale_game_frames(8));
    D_0015F684 = raised_alpha;
    if (raised_alpha < 0x81) {
        goto block_17;
    }
    D_0015F684 = 0x80;
    goto block_17;
block_15:
    lowered_alpha = D_0015F684 - (0x80 / scale_game_frames(8));

    D_0015F684 = lowered_alpha;
    if (lowered_alpha >= 0) {
        goto block_17;
    }
    D_0015F684 = 0;
block_17:
    text_color = (D_0015F684 << 0x18) + 0xF0F0F0;

    draw_framed_text(0x100, hud_msg_y, text_color, hud_msg_text1);
    if (hud_msg_text2[0] == 0) {
        goto block_20;
    }
    if (hud_msg_count < 0x3E9) {
        goto block_21;
    }
    draw_framed_text(0x100, hud_msg_y, text_color, hud_msg_text2);
block_20:
block_21:
    if (hud_msg_count == 0) {
        goto block_23;
    }

    hud_msg_count -= 1;
block_23:
    if (hud_msg_count != 0x3E8) {
        goto block_25;
    }

    hud_msg_count = 0;
    return;
block_25:
    return;

block_27:
block_28:
    hud_msg_y = 0x64;

    return;
}

__attribute__((alias("FUN_001ff780"))) extern void func_001FF780(void);

s32 hud_msg_y = 0;
