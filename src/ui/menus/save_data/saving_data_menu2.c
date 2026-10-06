#include "types.h"

typedef struct {
    u8 pad0[0x1A4];
    s32 held1;
    u8 pad1A8[0xC];
    s32 held2;
    u8 pad1B8[0xC];
    s32 pressed;
} PadState;

typedef struct {
    u8 pad0[0x8];
    s32 mode;
    u8 padC[0x8];
    s32 slot;
    u8 pad18[0xBC];
    s32 unkD4;
    u8 padD8[0x4];
    s32 unkDC;
    u8 padE0[0x4];
    s32 unkE4;
    u8 padE8[0xC];
    s32 unkF4;
} SaveState;

typedef struct {
    u8 pad0[0x84];
    s32 done;
} SubMenu;

typedef struct {
    u8 pad0[0x38];
    void *back;
    u8 pad3C[0x4];
    void *focus;
} MenuPage;

typedef struct {
    s32 state;
    MenuPage *page;
    void *next;
    u8 padC[0xC4];
    SubMenu *sub;
    s32 unkD4;
    u8 padD8[0x4C];
    s32 busy;
    s32 saving;
    s32 timer;
} MenuState;

typedef struct {
    u8 pad0[0x14];
    s32 sound;
    u8 pad18[0x18];
    s32 flags;
    u8 pad34[0xC];
    s32 cursor;
    u8 pad44[0x4];
    s32 unk48;
    s32 stage;
} Widget;

typedef struct __attribute__((packed)) {
    s64 v;
} Unaligned64;

extern PadState D_0013C940;
extern SaveState D_0013D290;
extern volatile u16 D_0013E05A[];
extern s32 D_0015ED84;
extern s32 D_0015ED98;
extern s32 D_0015EE20;
extern s32 D_0015EE24;
extern s32 D_0015EE34;
extern Unaligned64 D_0015EE98[1];
extern s32 D_0015EEB0;
extern s32 D_0015EEB4;
extern s32 D_001D2640[];
extern SubMenu D_001D4E38[];
extern SubMenu D_001D4F98[];
extern MenuState D_001D5BF0;
extern void InitializeGlobalStateEntry(s32 value);
extern void mode_freeze_init() __asm__("func_001FBAB8");
extern s32 load_and_initialize_level_chunk() __asm__("func_00209370");
extern void FUN_00226a70(s32 arg0, s32 arg1);
extern s32 FUN_00226b08();
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

s32 saving_data_menu2(Widget *w) __asm__("FUN_002235b8");

s32 saving_data_menu2(Widget *w) {
    s32 old;
    s32 pad;
    s32 cursor;
    s32 cur;
    s32 fl;
    void *back;

    if (D_001D5BF0.page->focus != w) {
        return 0;
    }
    old = w->cursor;
    if (w->stage == 0 && D_001D5BF0.sub == D_001D4E38 && D_001D5BF0.sub->done != 0) {
        w->stage = 1;
    }
    if (w->stage == 0 && D_001D5BF0.sub == D_001D4F98 && D_001D5BF0.sub->done != 0) {
        w->stage = 1;
    }
    if (w->stage == 1) {
        if (w->flags & 0x2000) {
            FUN_00226b08(w->cursor);
        } else {
            FUN_00226a70(w->unk48, w->cursor);
        }
        D_001D5BF0.saving = 1;
        D_001D5BF0.timer = 0x4FB5;
    }
    w->stage = 2;

    if (D_001D5BF0.saving != 0) {
        if (D_0013D290.unkD4 >= 3 || D_0013D290.unkDC >= 0) {
            return 0;
        }
        D_001D5BF0.saving = 0;
        if (D_0013D290.unkE4 != 0) {
            D_0015EEB4 |= 0x80;
            mode_freeze_init(3, D_001D5BF0.page);
            return 0;
        }
        *(s32 *)((u8 *)&D_0013D290 + D_0013D290.slot * 0x1C + 0x24) = D_0015ED98;
        *(s32 *)((u8 *)&D_0013D290 + D_0013D290.slot * 0x1C + 0x20) = D_0015ED84;
        *(s32 *)((u8 *)&D_0013D290 + D_0013D290.slot * 0x1C + 0x2C) = D_0015EE24;
        *(Unaligned64 *)((u8 *)&D_0013D290 + D_0013D290.slot * 0x1C + 0x30) = D_0015EE98[0];
        *(s32 *)((u8 *)&D_0013D290 + D_0013D290.slot * 0x1C + 0x28) = D_0015EE20;
        InitializeGlobalStateEntry(0);
        D_0013E05A[0] = 1;
    }

    if ((D_0013C940.pressed & 0xD00) && D_001D5BF0.busy == 0) {
        return 1;
    }
    if (D_0013C940.pressed & 0x10) {
        back = D_001D5BF0.page->back;
        if (back != NULL) {
            D_001D5BF0.next = back;
        } else if (D_001D5BF0.busy == 0) {
            return -1;
        }
    }
    if (D_0015EEB0 != 0x10 && D_0015EEB0 != 1) {
        D_001D5BF0.next = D_001D5BF0.page->back;
        return 0;
    }
    if (D_0013D290.unkD4 >= 3 || D_0013D290.unkDC >= 0 || D_0013D290.mode != 2) {
        return 0;
    }

    if (w->flags & 1) {
        pad = D_0013C940.held2;
    } else {
        pad = D_0013C940.held1;
    }
    w->cursor = D_0015EE34;
    if ((pad & 0x1000) && D_0015EE34 != 0) {
        w->cursor = D_0015EE34 - 1;
    }
    if (pad & 0x4000) {
        cur = w->cursor;
        if (cur < 4) {
            w->cursor = cur + 1;
        }
    }
    cursor = w->cursor;
    D_0015EE34 = cursor;
    if ((pad & 0x40) && D_0013D290.mode == 2) {
        if (*(s32 *)((u8 *)&D_0013D290 + cursor * 0x1C + 0x20) != -1) {
            D_001D5BF0.next = (w->flags & 0x2000) ? D_001D4F98 : D_001D4E38;
            D_001D5BF0.unkD4 = (w->flags & 0x2000) ? 2 : 1;
            D_001D2640[0] = w->cursor;
        } else {
            w->stage = 1;
        }
    } else if (pad & 0x20) {
        D_0013D290.unkF4 = 0;
        fl = D_0015EEB4 & ~2;
        D_0015EEB4 = fl & ~4;
        load_and_initialize_level_chunk();
        InitializeGlobalStateEntry(0);
        D_0013E05A[0] = 1;
    }
    if (w->cursor != old) {
        allocate_voice_for_target_entry(1, 0x11, w->sound);
    }
    return 0;
}

extern __typeof__(saving_data_menu2) func_002235B8 __attribute__((alias("FUN_002235b8")));
