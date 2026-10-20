/* Ported from rac1-decomp (src/game/memcard.c, func_0020BFC8). */
#include "sda.h"
#include "types.h"
extern char D_0013D290[];
extern int mc_gamedata[] __asm__("D_001A04C0");
extern int mc_leveldata[] __asm__("D_001A07C0");
extern int memcard_prepare_data(void *dst, int i, int *table) __asm__("func_0020AD78");
extern void sceCdReadClock(void *);
extern void sceScfGetLocalTimefromRTC(void *);
extern void copy_level_object_positions(void) __asm__("FUN_00208770");
extern void save_map_coverage(void *) __asm__("func_00207B08");
extern char D_0015EE98[] MACRO_ADDR;
extern char D_00141EC0[];
#include "rnc/gameplay/state/level_state.h"
extern int D_0015EE24 MACRO_ADDR;
extern int alternate_animation_sequence_macro __asm__("D_0015EE20") MACRO_ADDR;
extern int mode_freeze_flags __asm__("D_0015EEB4") MACRO_ADDR;
extern u8 D_0014EED0[];
extern u8 D_001506D0[];
/* memcard_save_data(slot, flags): starts the save (sceCdReadClock/sceScfGetLocalTimefromRTC
   on D_0015EE98, copy_level_object_positions, save_map_coverage on the D_00141EC0 page for
   D_0015ED84). With no valid card slot (D_0013D290+0xC8, its record's
   +0x14) it returns whether `slot` is 0. Otherwise it ORs `slot` into the
   pending mask (+0xFC); unless nothing is pending, a save is running
   (+0xDC >= 3) or a result is waiting (+0xE4 >= 0), it stamps +0xD0 with
   D_0015ED84 (temporarily switching D_0015ED84 to `flags` and setting that
   level_visit_state byte), fills entry e[+0x14] (four globals and the 8-byte
   name), serialises both tables (memcard_prepare_data), restores the byte and
   D_0015ED84, and sets result 0xF. Returns whether the result is 0xF.
   D_0015EE98 is MACRO_ADDR (retail rebuilds its address at each use), the
   name field is reached from a `q + 0x30` base, and each block reads
   D_0013D290 through its own `char *` local (retail keeps only the %hi). */
int memcard_save_data(int slot, int flags) __asm__("FUN_0020b178");

extern s32 current_level_index __asm__("D_0015ED84") MACRO_ADDR;
extern s32 current_bolt_count_macro __asm__("D_0015ED98") MACRO_ADDR;
int memcard_save_data(int slot, int flags) {
    unsigned char saved;

    sceCdReadClock(D_0015EE98);
    sceScfGetLocalTimefromRTC(D_0015EE98);
    copy_level_object_positions();
    save_map_coverage(D_00141EC0 + (current_level_index << 11));
    {
        char *memory_card_control = D_0013D290;
        int active = *(int *)(memory_card_control + 0xC0);
        int mask;

        if (active == -1 || *(int *)(memory_card_control + active * 0xB8 + 0x14) < 0) {
            return slot == 0;
        }
        mask = *(int *)(memory_card_control + 0xF4) | slot;
        *(int *)(memory_card_control + 0xF4) = mask;
        if (mask == 0) {
            goto done;
        }
        if (slot == 0) {
            mode_freeze_flags |= 0x200;
        }
        if (*(int *)(memory_card_control + 0xD4) >= 3) {
            goto done;
        }
        if (*(int *)(memory_card_control + 0xDC) >= 0) {
            goto done;
        }
        *(int *)(memory_card_control + 0xC8) = current_level_index;
        saved = 0;
        if (flags >= 0) {
            current_level_index = flags;
            saved = level_visit_state[flags];
            if (saved == 0) {
                level_visit_state[flags] = 1;
            }
        }
    }
    {
        char *memory_card_state = D_0013D290;
        char *names = memory_card_state + 0x30;

        *(int *)(memory_card_state + *(int *)(memory_card_state + 0x14) * 0x1C + 0x24) = current_bolt_count_macro;
        *(int *)(memory_card_state + *(int *)(memory_card_state + 0x14) * 0x1C + 0x20) = current_level_index;
        *(int *)(memory_card_state + *(int *)(memory_card_state + 0x14) * 0x1C + 0x2C) = D_0015EE24;
        memcpy(names + *(int *)(memory_card_state + 0x14) * 0x1C, D_0015EE98, 8);
        *(int *)(memory_card_state + *(int *)(memory_card_state + 0x14) * 0x1C + 0x28) = alternate_animation_sequence_macro;
        memcard_prepare_data(D_0014EED0, 0, mc_gamedata);
        memcard_prepare_data(D_001506D0, *(int *)(memory_card_state + 0xC8), mc_leveldata);
        if (flags >= 0) {
            level_visit_state[current_level_index] = saved;
            current_level_index = *(int *)(memory_card_state + 0xC8);
        }
        if (*(int *)(memory_card_state + 0xDC) < 0) {
            *(int *)(memory_card_state + 0xDC) = 0xF;
            *(int *)(memory_card_state + 0xE0) = *(int *)(memory_card_state + 0xC0);
        }
    }
done: {
    char *r = D_0013D290;
    return *(int *)(r + 0xDC) == 0xF;
}
}

extern __typeof__(memcard_save_data) func_0020B178 __attribute__((alias("FUN_0020b178")));
