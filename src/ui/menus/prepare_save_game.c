/* Ported from rac1-decomp (src/game/pause.c, func_00227C78). */

#include "sda.h"

extern int D_0015ED84 MACRO_ADDR;
#include "rnc/storage/memory_card/memory_card_state.h"
extern void memcard_make_whole_save(char *out) __asm__("func_0020ABB0");
extern void prepare_save_game(int, int) __asm__("FUN_002269c0");
extern char D_0015EE98[] MACRO_ADDR;
extern char D_00141EC0[];
extern void sceCdReadClock(void *);
extern void sceScfGetLocalTimefromRTC(void *);
extern void FUN_00208770(void);
extern void func_00207B08(void *);

void prepare_save_game(int save_data, int slot) __asm__("FUN_002269c0");

void prepare_save_game(int save_data, int slot) {
    struct MemoryCardState *b = &memory_card_state;
    int saving = 1;
    sceCdReadClock(D_0015EE98);
    sceScfGetLocalTimefromRTC(D_0015EE98);
    FUN_00208770();
    func_00207B08(D_00141EC0 + (D_0015ED84 << 11));
    memcard_make_whole_save((char *)save_data);
    b->unkF4 = saving;
    b->buf = (void *)save_data;
    b->card[0].save_index = slot;
    b->active_card = 0;
    if (b->pending_state < 0) {
        b->pending_card = 0;
        b->pending_state = 0x13;
    }
}

extern __typeof__(prepare_save_game) func_002269C0 __attribute__((alias("FUN_002269c0")));
