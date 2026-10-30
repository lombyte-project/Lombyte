/* Ported from rac1-decomp (src/game/pause.c, func_00227C78). */

#include "sda.h"
#include "types.h"
#include "rnc/data_labels.h"

#include "rnc/save/memory_card_state.h"
#include "rnc/globals.h"
#include "rnc/ui/map/copy_level_object_positions.h"

extern void memcard_make_whole_save(char *out) __asm__("func_0020ABB0");
extern void prepare_save_game(int, int) __asm__("FUN_002269c0");
extern char D_0015EE98[] MACRO_ADDR;
extern void sceCdReadClock(void *);
extern void sceScfGetLocalTimefromRTC(void *);
extern void save_map_coverage(void *) __asm__("func_00207B08");

void prepare_save_game(int save_data, int slot) __asm__("FUN_002269c0");

void prepare_save_game(int save_data, int slot) {
    struct MemoryCardState *card_state = &memory_card_state;
    int                     saving = 1;
    sceCdReadClock(D_0015EE98);
    sceScfGetLocalTimefromRTC(D_0015EE98);
    copy_level_object_positions();
    save_map_coverage(D_00141EC0 + (current_level_index << 11));
    memcard_make_whole_save((char *)save_data);
    card_state->unkF4 = saving;
    card_state->buf = (void *)save_data;
    card_state->card[0].save_index = slot;
    card_state->active_card = 0;
    if (card_state->pending_state < 0) {
        card_state->pending_card = 0;
        card_state->pending_state = 0x13;
    }
}

extern __typeof__(prepare_save_game) func_002269C0 __attribute__((alias("FUN_002269c0")));
