#include "types.h"

struct HudSlotArray {
    u8  pad_0[0x64];
    s32 unk64;
};

extern struct HudSlotArray hud_chan_slots __asm__("D_00199B60");
extern s32                 queue_animation_update(s32 chan,
                                                  s32 id,
                                                  s32 fn,
                                                  s32 animation_callback,
                                                  s32 queued_animation_callback,
                                                  s32 animation_context,
                                                  s32 queued_animation_value) __asm__("func_001FF308");

s32 remove_hud_item(s32 item) __asm__("FUN_001ff480");

s32 remove_hud_item(s32 item) {
    s32 *cursor;
    s32  index;

    index = 0;
    if (hud_chan_slots.unk64 != item) {
        cursor = (s32 *)((u8 *)&hud_chan_slots + 0x64);
    loop_2:
        index += 1;
        cursor = (s32 *)((u8 *)cursor + 0x90);
        if (index < 0xD) {
            if (*cursor == item) {
                goto block_4;
            }
            goto loop_2;
        }
        goto block_5;
    }
block_4:
    if (index >= 0xD) {
    block_5:
        return 0;
    }
    queue_animation_update(index, 0xFFFF, 0, 0, 0, 0, 0);
    return 1;
}
