#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "sda.h"

/* Preserves selected state across func_00209370 using scratchpad copies.
   Only listed bytes of item_available are restored; each D_00141EA0 entry is
   cleared when its referenced byte is zero. The two saved flags are then
   restored, the counter is advanced, the clock is refreshed, and a
   nonnegative slot updates the checkpoint state. The save menu supplies
   a cursor slot; the global state caller supplies -1 to skip that update. */

typedef struct {
    u8 pad0[0x14];
    s32 slot;
    u8 pad18[8];
    struct {
        s32 unk0;
        u8 pad4[0x18];
    } entries[5];
    u8 pad[0xC0 - 0x20 - 5 * 0x1C];
    s32 unkC0;
    u8 padC4[0x18];
    s32 unkDC;
    s32 unkE0;
    u8 padE4[8];
    s32 unkEC;
} Checkpoints;

extern Checkpoints D_0013D290;
#include "rnc/gameplay/state/item_state.h"
extern u8 D_0014BEC0[];
extern s32 D_00141EA0[];
extern u8 D_0015EDD0[] MACRO_ADDR;
extern s32 D_0015EDA0;
extern s32 D_0015ED98;
extern u8 D_0015EE1C;
extern u8 D_0015EE1D;
extern s32 D_0015EE20;
extern u8 D_0015EE98[] MACRO_ADDR;
extern s32 D_001D5BA0[];

extern void func_001F9838(void *dst, void *src, s32 size);
extern void load_and_initialize_level_chunk(void) __asm__("func_00209370");
extern void memcard_make_whole_save(s32) __asm__("func_0020ABB0");
extern s32 sceCdReadClock(u8 *clock);
extern void sceScfGetLocalTimefromRTC(u8 *clock);

void FUN_00226b08(s32 slot) {
    s32 saved;
    u32 count;
    u8 flag4;
    u8 flag5;
    s32 *items = (s32 *)0x70000150;
    u8 *bytes;
    s32 i;
    s32 j;

    func_001F9838((void *)0x70000000, item_text_variant, 0x28);
    func_001F9838((void *)0x70000030, item_available, 0x25);
    func_001F9838((void *)0x70000060, weapon_ammo_counts, 0x94);
    func_001F9838((void *)0x70000100, D_0014BEC0, 0x50);
    func_001F9838((void *)0x70000150, D_00141EA0, 0x20);
    func_001F9838((void *)0x70000170, D_0015EDD0, 0xC);
    func_001F9838((void *)0x70000180, item_unlocked, 0x20);
    saved = D_0015ED98;
    count = D_0015EE20;
    flag4 = alternate_item_available[4] != 0;
    flag5 = alternate_item_available[5] != 0;
    bytes = (u8 *)0x70000030;
    load_and_initialize_level_chunk();
    func_001F9838(item_text_variant, (void *)0x70000000, 0x28);
    for (j = 0; D_001D5BA0[j] != -1; j++) {
        item_available[D_001D5BA0[j]] = bytes[D_001D5BA0[j]];
    }
    func_001F9838(weapon_ammo_counts, (void *)0x70000060, 0x94);
    func_001F9838(D_0014BEC0, (void *)0x70000100, 0x50);
    for (i = 0; i < 8; i++) {
        s32 item = items[i];
        D_00141EA0[i] = item_available[item] ? item : 0;
    }
    func_001F9838(D_0015EDD0, (void *)0x70000170, 0xC);
    func_001F9838(item_unlocked, (void *)0x70000180, 0x20);
    D_0015ED98 = saved;
    if (flag4) {
        alternate_item_available[4] = 1;
        D_0015EDA0 = 5;
    }
    if (flag5) {
        alternate_item_available[5] = 1;
        D_0015EDA0 = 8;
    }
    D_0015EE20 = count + 1;
    D_0015EE1C = 0;
    D_0015EE1D = 0;
    sceCdReadClock(D_0015EE98);
    sceScfGetLocalTimefromRTC(D_0015EE98);
    if (slot >= 0) {
        D_0013D290.slot = slot;
        D_0013D290.entries[slot].unk0 = 0;
        memcard_make_whole_save(menu_system.unkE0);
        D_0013D290.unkEC = menu_system.unkE0;
        D_0013D290.unkC0 = 0;
        if (D_0013D290.unkDC < 0) {
            D_0013D290.unkE0 = 0;
            D_0013D290.unkDC = 0x13;
        }
    }
}

extern __typeof__(FUN_00226b08) func_00226B08 __attribute__((alias("FUN_00226b08")));
