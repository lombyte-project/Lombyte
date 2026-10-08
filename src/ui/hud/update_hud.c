#include "types.h"
#include "rnc/ui/hud/hud_state.h"
#include "rnc/globals.h"

struct HudSlot {
    u8 pad_0[0x18];
    s32 unk18;
};
extern s32 D_0015F444;
extern s32 D_0015F680;
extern s32 D_0015F684;
extern s32 D_0015F688;
extern u8 D_001993D8[];
extern u8 D_00199428[];
extern u8 D_00199B60[];
extern s32 scale_game_frames() __asm__("func_001F96F8");
extern s32 draw_framed_text() __asm__("func_00201200");
void update_hud(void) __asm__("FUN_001ff780");

void update_hud(void) {
    s32 update_fn;
    short new_var2;
    u8 *slot;
    s32 text_color;
    s32 temp_2_59;
    s32 temp_2_76;
    s32 remaining;
    if (hud_state.unk30 == 0) {
        goto block_2;
    }
    hud_state.unk30 = 0;
    return;
block_2:
    if (D_0015F444 == 0) {
        goto block_5;
    }

    hud_state.unk30 = 0;
    return;
block_5:
    slot = D_00199B60;

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
    if (D_0015F680 != 0) {
        goto block_11;
    }
    if (D_0015F684 == 0) {
        goto block_27;
    }
block_11:
    if (game_mode != 0) {
        goto block_28;
    }

    if (D_0015F680 == 0) {
        goto block_15;
    }
    temp_2_59 = D_0015F684 + (0x80 / scale_game_frames(8));
    D_0015F684 = temp_2_59;
    if (temp_2_59 < 0x81) {
        goto block_17;
    }
    D_0015F684 = 0x80;
    goto block_17;
block_15:
    temp_2_76 = D_0015F684 - (0x80 / scale_game_frames(8));

    D_0015F684 = temp_2_76;
    if (temp_2_76 >= 0) {
        goto block_17;
    }
    D_0015F684 = 0;
block_17:
    text_color = (D_0015F684 << 0x18) + 0xF0F0F0;

    draw_framed_text(0x100, D_0015F688, text_color, D_001993D8);
    if (D_00199428[0] == 0) {
        goto block_20;
    }
    if (D_0015F680 < 0x3E9) {
        goto block_21;
    }
    draw_framed_text(0x100, D_0015F688, text_color, D_00199428);
block_20:
block_21:
    if (D_0015F680 == 0) {
        goto block_23;
    }

    D_0015F680 -= 1;
block_23:
    if (D_0015F680 != 0x3E8) {
        goto block_25;
    }

    D_0015F680 = 0;
    return;
block_25:
    return;

block_27:
block_28:
    D_0015F688 = 0x64;

    return;
}

__attribute__((alias("FUN_001ff780"))) extern void func_001FF780(void);
