#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "sda.h"
#include "rnc/globals.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/game/game_globals.h"
#include "rnc/sdk/scf.h"
#include "rnc/sdk/storage/cdvd.h"

/* Preserves selected state across load_and_initialize_level_chunk using scratchpad copies.
   Only listed bytes of item_available are restored; each D_00141EA0 entry is
   cleared when its referenced byte is zero. The two saved flags are then
   restored, the counter is advanced, the clock is refreshed, and a
   nonnegative slot updates the checkpoint state. The save menu supplies
   a cursor slot; the global state caller supplies -1 to skip that update. */

typedef struct {
    u8  pad0[0x14];
    s32 save_index; /* 0x14: struct MemoryCard save_index */
    u8  pad18[8];
    struct {
        s32 id; /* MemoryCardSaveEntry id */
        u8  pad4[0x18];
    } entries[5];
    u8  pad[0xC0 - 0x20 - 5 * 0x1C];
    s32 active_card; /* 0xC0: struct MemoryCardState fields from here */
    u8  padC4[0x18];
    s32 pending_state; /* 0xDC */
    s32 pending_card;  /* 0xE0 */
    u8  padE4[8];
    s32 buf;   /* 0xEC */
} Checkpoints; /* the start of struct MemoryCardState (rnc/save/memory_card_state.h) */

extern Checkpoints D_0013D290;

#include "rnc/game/item_state.h"

extern u8 D_0015EE98[] MACRO_ADDR;
/* Item ids whose availability survives the reload; -1 ends the list. */
extern s32 item_availability_restore_indices[20] __asm__("D_001D5BA0");

extern void copy_memory_aligned(void *dst, void *src, s32 size) __asm__("func_001F9838");
extern void load_and_initialize_level_chunk(void) __asm__("func_00209370");
extern void memcard_make_whole_save(s32) __asm__("func_0020ABB0");

extern u8 D_0015EDD0_macro[] __asm__("D_0015EDD0") MACRO_ADDR;
void      load_level_zero_preserving_progress(s32 slot) __asm__("FUN_00226b08");

void load_level_zero_preserving_progress(s32 slot) {
    s32  saved;
    u32  saved_alternate_animation_sequence;
    u8   flag4;
    u8   flag5;
    s32 *items = (s32 *)0x70000150;
    u8  *bytes;
    s32  i;
    s32  j;

    copy_memory_aligned((void *)0x70000000, item_text_variant, 0x28);
    copy_memory_aligned((void *)0x70000030, item_available, 0x25);
    copy_memory_aligned((void *)0x70000060, weapon_ammo_counts, 0x94);
    copy_memory_aligned((void *)0x70000100, gold_bolts_collected, 0x50);
    copy_memory_aligned((void *)0x70000150, pause_iop_sprite_lists, 0x20);
    copy_memory_aligned((void *)0x70000170, D_0015EDD0_macro, 0xC);
    copy_memory_aligned((void *)0x70000180, item_unlocked, 0x20);
    saved = current_bolt_count;
    saved_alternate_animation_sequence = alternate_animation_sequence;
    flag4 = alternate_item_available[4] != 0;
    flag5 = alternate_item_available[5] != 0;
    bytes = (u8 *)0x70000030;
    load_and_initialize_level_chunk();
    copy_memory_aligned(item_text_variant, (void *)0x70000000, 0x28);
    for (j = 0; item_availability_restore_indices[j] != -1; j++) {
        item_available[item_availability_restore_indices[j]] =
            bytes[item_availability_restore_indices[j]];
    }
    copy_memory_aligned(weapon_ammo_counts, (void *)0x70000060, 0x94);
    copy_memory_aligned(gold_bolts_collected, (void *)0x70000100, 0x50);
    for (i = 0; i < 8; i++) {
        s32 saved_item_index = items[i];
        pause_iop_sprite_lists[i] = item_available[saved_item_index] ? saved_item_index : 0;
    }
    copy_memory_aligned(D_0015EDD0_macro, (void *)0x70000170, 0xC);
    copy_memory_aligned(item_unlocked, (void *)0x70000180, 0x20);
    current_bolt_count = saved;
    if (flag4) {
        alternate_item_available[4] = 1;
        hero_max_health = 5;
    }
    if (flag5) {
        alternate_item_available[5] = 1;
        hero_max_health = 8;
    }
    alternate_animation_sequence = saved_alternate_animation_sequence + 1;
    D_0015EE1C = 0;
    D_0015EE1D = 0;
    sceCdReadClock(D_0015EE98);
    sceScfGetLocalTimefromRTC(D_0015EE98);
    if (slot >= 0) {
        D_0013D290.save_index = slot;
        D_0013D290.entries[slot].id = 0;
        memcard_make_whole_save(menu_system.save_slot);
        D_0013D290.buf = menu_system.save_slot;
        D_0013D290.active_card = 0;
        if (D_0013D290.pending_state < 0) {
            D_0013D290.pending_card = 0;
            D_0013D290.pending_state = 0x13;
        }
    }
}

extern __typeof__(load_level_zero_preserving_progress) func_00226B08
    __attribute__((alias("FUN_00226b08")));

s32 hero_max_health = 4;

s32 item_availability_restore_indices[20] = {
    0x9,  0xA,  0xB,  0xD,  0xE,  0xF,  0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23, -1,
};
