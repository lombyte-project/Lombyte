/* Ported from rac1-decomp (src/game/memcard.c, func_0020BFC8). */
#include "sda.h"
#include "types.h"
#include "rnc/game/level_progress.h"
#include "rnc/ui/map/copy_level_object_positions.h"
#include "rnc/sdk/scf.h"
#include "rnc/sdk/storage/cdvd.h"

extern int  memcard_prepare_data(void *dst, int i, int *table) __asm__("func_0020AD78");
extern void save_map_coverage(void *) __asm__("func_00207B08");
extern char D_0015EE98[] MACRO_ADDR;

#include "rnc/game/level_state.h"
#include "rnc/save/mc_data.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/globals.h"

extern int D_0015EE24 MACRO_ADDR;
extern int            alternate_animation_sequence_macro __asm__("D_0015EE20") MACRO_ADDR;
/* D_0015EE98 is MACRO_ADDR (retail rebuilds its address at each use), the
   clock is copied from a base taken before the entry index is scaled, and
   each block reads memory_card_state through its own local (retail keeps
   only the %hi). */
int memcard_save_data(int slot, int flags) __asm__("FUN_0020b178");

extern s32 current_bolt_count_macro __asm__("D_0015ED98") MACRO_ADDR;

int memcard_save_data(int slot, int flags) {
    enum {
        LEVEL_VISIT_UNVISITED = 0
    };
    unsigned char saved_visit_state;

    sceCdReadClock(D_0015EE98);
    sceScfGetLocalTimefromRTC(D_0015EE98);
    copy_level_object_positions();
    save_map_coverage(map_coverage_pages + (current_level_index << 11));
    {
        struct MemoryCardState *card_state = &memory_card_state;
        int                     active = card_state->active_card;
        int                     mask;

        if (active == -1 || card_state->card[active].save_index < 0) {
            return slot == 0;
        }
        mask = card_state->card_save_active | slot;
        card_state->card_save_active = mask;
        if (mask == 0) {
            goto done;
        }
        if (slot == 0) {
            mode_freeze_flags |= 0x200;
        }
        if (card_state->state >= MEMCARD_REQUEST_FORMAT) {
            goto done;
        }
        if (card_state->pending_state >= MEMCARD_QUERY_INFO) {
            goto done;
        }
        card_state->record_index = current_level_index;
        saved_visit_state = LEVEL_VISIT_UNVISITED;
        if (flags >= 0) {
            current_level_index = flags;
            saved_visit_state = level_visit_state[flags];
            if (saved_visit_state == LEVEL_VISIT_UNVISITED) {
                level_visit_state[flags] = 1;
            }
        }
    }
    {
        struct MemoryCardState     *card_state = &memory_card_state;
        struct MemoryCardSaveClock *saved_clocks = &card_state->card[0].entries[0].saved_at;

        card_state->card[0].entries[card_state->card[0].save_index].bolts =
            current_bolt_count_macro;
        card_state->card[0].entries[card_state->card[0].save_index].id = current_level_index;
        card_state->card[0].entries[card_state->card[0].save_index].time = D_0015EE24;
        memcpy((u8 *)saved_clocks +
                   card_state->card[0].save_index * sizeof(struct MemoryCardSaveEntry),
               D_0015EE98,
               8);
        card_state->card[0].entries[card_state->card[0].save_index].count =
            alternate_animation_sequence_macro;
        memcard_prepare_data(memcard_game_data_buffer, 0, mc_gamedata);
        memcard_prepare_data(memcard_level_data_buffer, card_state->record_index, mc_leveldata);
        if (flags >= 0) {
            level_visit_state[current_level_index] = saved_visit_state;
            current_level_index = card_state->record_index;
        }
        if (card_state->pending_state < MEMCARD_QUERY_INFO) {
            card_state->pending_state = MEMCARD_BEGIN_UPDATE_SAVE;
            card_state->pending_card = card_state->active_card;
        }
    }
done:
    return memory_card_state.pending_state == MEMCARD_BEGIN_UPDATE_SAVE;
}

extern __typeof__(memcard_save_data) func_0020B178 __attribute__((alias("FUN_0020b178")));
