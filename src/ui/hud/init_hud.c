#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"

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

extern u8 hud_file_str[] __asm__("D_0015F6D8");
extern struct Globals_0015FA00 *hud_bank_main __asm__("D_0015FA00");
extern s32 D_0015FA04 __attribute__((sda));
extern s32 hud_bank_end __asm__("D_0015FA08");
extern s32 hud_bank_aux __asm__("D_0015FA0C");
extern s32 fast_mem_zero16() __asm__("func_001F9810");
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
        u8 *base = hud_chan_slots;
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
    temp_2_41 = hud_bank_main;
    if (temp_2_41 == NULL) {
        hud_bank_main = hud_heap_alloc(0x2800, 0, hud_file_str, 0x115);
        hud_bank_aux = hud_heap_alloc(0x1400, 0, hud_file_str, 0x116);
    }
    hud_bank_end = (s32)((u8 *)hud_bank_main + 0x2800);
    D_0015FA04 = (s32)hud_bank_main;
    fast_mem_zero16(hud_bank_main, 0x2800);
    hud_bank_main->unk20 = 0xFF;
}
