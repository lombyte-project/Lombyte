#include "types.h"

struct Globals_0015FA00 {
    u8 pad_0[0x20];
    u8 unk20;
    u8 pad_21[0x3];
};

struct HudSlot {
    s32 unk0;
    u8 pad_4[0x3C];
    s32 unk40;
    u8 pad_44[0x4];
    s32 unk48;
    u8 pad_4C[0xC];
    s32 unk58;
};
#include "rnc/ui/hud/hud_state.h"

extern u8 D_0015F6D8[];
extern struct Globals_0015FA00 *D_0015FA00;
extern s32 D_0015FA04 __attribute__((sda));
extern s32 D_0015FA08;
extern s32 D_0015FA0C;
extern u8 D_00199B60[];
extern s32 func_001F9810();
extern s32 hud_heap_alloc() __asm__("func_001FF288");
extern s32 queue_animation_update() __asm__("func_001FF308");
/* retail small-data globals, declared to GAS before the body */

void init_hud(void) __asm__("FUN_001fee88");

void init_hud(void) {
    s32 slot_index;
    s32 *temp_2_41;
    struct HudSlot *slot;

    slot_index = 0;
    {
        u8 *base = D_00199B60;
        slot = (struct HudSlot *)((u8 *)base + 0x24);
    }
    hud_state.serial = 0;
    hud_state.unk4 = 0;
    do {
        slot->unk40 = -1;
        *(s32 *)((u8 *)slot - 0x4) = 0x10000;
        queue_animation_update(slot_index, 0xFFFF, 0, 0, 0, 0, 1);
        slot_index += 1;
        slot->unk58 = 0;
        slot->unk48 = -6;
        *(s32 *)((u8 *)slot - 0x20) = 0;
        slot->unk0 = 0;
        slot = (struct HudSlot *)((u8 *)slot + 0x90);
    } while (slot_index < 0xD);
    temp_2_41 = D_0015FA00;
    if (temp_2_41 == NULL) {
        D_0015FA00 = hud_heap_alloc(0x2800, 0, D_0015F6D8, 0x115);
        D_0015FA0C = hud_heap_alloc(0x1400, 0, D_0015F6D8, 0x116);
    }
    D_0015FA08 = (s32)((u8 *)D_0015FA00 + 0x2800);
    D_0015FA04 = (s32)D_0015FA00;
    func_001F9810(D_0015FA00, 0x2800);
    D_0015FA00->unk20 = 0xFF;
}
