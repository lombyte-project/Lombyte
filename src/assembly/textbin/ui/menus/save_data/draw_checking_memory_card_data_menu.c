#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/save_data/draw_checking_memory_card_data_menu/FUN_00220348.s", FUN_00220348);
#else
#include "types.h"

struct Box {
    s16 v[12];
};

struct Entry {
    u8 pad0[4];
    s16 type;
    s16 id;
    u8 pad8[2];
};

struct Level {
    u8 pad0[0x48];
    struct Entry *entries;
};

struct Planet {
    u8 pad0[0x40];
    struct Level *level;
};

struct MemMenu {
    u8 pad0[0x20];
    s32 w;
    s32 h;
    u8 pad28[0xC];
    s32 flags;
    s32 texw;
    s32 texh;
    u8 pad40[4];
    s32 state;
    s32 tex0;
    s32 tex1;
    s32 entry[2];
};

struct McState {
    u8 pad0[8];
    s32 phase;
    u8 padC[0xC8];
    s32 unkD4;
    u8 padD8[4];
    s32 unkDC;
};

struct Game {
    u8 pad0[0x128];
    s32 hasText;
    s32 textId;
};

struct Screen {
    u8 pad0[0x160];
    s16 w;
    s16 h;
};

extern struct McState D_0013D290;
extern struct Game D_001D5BF0;
extern struct Planet *D_001D5BF4;
extern u8 D_0013D4C0[];
extern u8 D_0013D388[];
extern struct Screen D_00151780;
extern void func_001F4280(s32);
extern void func_001F4398(void);
extern s32 func_001FDD10(s32);
extern void memset(void *, s32, u32);
extern void func_001F75F0(struct Box *, u64, s32, s32);
extern s32 func_00204CF0(s32);
extern void func_001F5450(s32, s32, s32, s32, s32, s32, s32, s32, s64, s64);

s32 draw_checking_memory_card_data_menu(struct MemMenu *m) __asm__("FUN_00220348");

s32 draw_checking_memory_card_data_menu(struct MemMenu *m) {
    struct Box tmp;
    s16 box[12];
    struct Entry *e;
    s32 txt;
    s32 tex;

    if (m->state < 2) {
        if (!(m->flags & 0x100)) {
            return 1;
        }
        if (D_0013D290.phase != 2) {
            return 2;
        }
        if (D_0013D290.unkD4 < 3 && D_0013D290.unkDC < 0) {
            return 2;
        }
        func_001F4280(0);
        txt = func_001FDD10(D_001D5BF0.hasText ? D_001D5BF0.textId : 0x4FB9);
        memset(box, 0, sizeof(box));
        box[1] = m->h + 1;
        box[0] = 1;
        box[2] = 1;
        box[3] = m->w + 1;
        box[4] = m->w >> 1;
        box[5] = 5;
        box[8] = 16;
        box[9] = 5;
        tmp = *(struct Box *)box;
        func_001F75F0(&tmp, 0x80000000, txt, -1);
        tmp.v[5] = (m->h - tmp.v[7]) >> 1;
        tmp.v[9] ^= 4;
        func_001F75F0(&tmp, 0x80000000, txt, -1);
        tmp.v[0]--;
        tmp.v[1]--;
        tmp.v[2]--;
        tmp.v[3]--;
        tmp.v[4]--;
        tmp.v[5]--;
        func_001F75F0(&tmp, 0x80FFA888, txt, -1);
        func_001F4398();
        return 2;
    }
    if (m->flags & 4) {
        e = &D_001D5BF4->level->entries[*(s32 *)((u8 *)m->entry + (((m->state < 4) ^ 1) << 2))];
        if (e->type == 0 && D_0013D4C0[e->id] == 0) {
            return 1;
        }
        if (e->type == 1 && D_0013D388[e->id] == 0) {
            return 1;
        }
    }
    func_001F4280(0);
    tex = func_00204CF0(m->state < 4 ? m->tex0 : m->tex1);
    func_001F5450(0, 0, D_00151780.w, D_00151780.h, 0, 0, m->texw, m->texh, 0x80808080, tex);
    func_001F4398();
    return 0x10;
}
#endif /* NON_MATCHING */
