#include "types.h"
#include "rnc/globals.h"

typedef struct __attribute__((packed)) {
    s64 v;
} Unaligned64;

#include "rnc/input/pad_state.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/storage/memory_card/memory_card_state.h"
extern volatile u16 D_0013E05A[];
extern s32 D_0015EE20;
extern s32 D_0015EE24;
extern s32 D_0015EE34;
extern Unaligned64 D_0015EE98[1];
extern s32 D_001D2640[];
extern struct MenuPage D_001D4E38[];
extern struct MenuPage D_001D4F98[];
extern void InitializeGlobalStateEntry(s32 value);
extern void mode_freeze_init() __asm__("func_001FBAB8");
extern s32 load_and_initialize_level_chunk() __asm__("func_00209370");
extern void FUN_00226a70(s32 arg0, s32 arg1);
extern s32 FUN_00226b08();
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");

s32 saving_data_menu2(struct MenuScreen *w) __asm__("FUN_002235b8");

s32 saving_data_menu2(struct MenuScreen *w) {
    s32 old;
    s32 pad;
    s32 cursor;
    s32 cur;
    s32 fl;
    struct MenuPage *back;

    if (menu_system.current->focus != w) {
        return 0;
    }
    old = w->data.save.slot;
    if (w->data.save.step == 0 && menu_system.previous == D_001D4E38 && menu_system.previous->confirmed != 0) {
        w->data.save.step = 1;
    }
    if (w->data.save.step == 0 && menu_system.previous == D_001D4F98 && menu_system.previous->confirmed != 0) {
        w->data.save.step = 1;
    }
    if (w->data.save.step == 1) {
        if (w->data.save.flags & 0x2000) {
            FUN_00226b08(w->data.save.slot);
        } else {
            FUN_00226a70(w->data.save.save_data, w->data.save.slot);
        }
        menu_system.card_op_pending = 1;
        menu_system.card_op_text = 0x4FB5;
    }
    w->data.save.step = 2;

    if (menu_system.card_op_pending != 0) {
        if (memory_card_state.state >= 3 || memory_card_state.pending_state >= 0) {
            return 0;
        }
        menu_system.card_op_pending = 0;
        if (memory_card_state.err != 0) {
            mode_freeze_flags |= 0x80;
            mode_freeze_init(3, menu_system.current);
            return 0;
        }
        *(s32 *)((u8 *)&memory_card_state + memory_card_state.card[0].save_index * 0x1C + 0x24) = current_bolt_count;
        *(s32 *)((u8 *)&memory_card_state + memory_card_state.card[0].save_index * 0x1C + 0x20) = current_level_index;
        *(s32 *)((u8 *)&memory_card_state + memory_card_state.card[0].save_index * 0x1C + 0x2C) = D_0015EE24;
        *(Unaligned64 *)((u8 *)&memory_card_state + memory_card_state.card[0].save_index * 0x1C + 0x30) = D_0015EE98[0];
        *(s32 *)((u8 *)&memory_card_state + memory_card_state.card[0].save_index * 0x1C + 0x28) = D_0015EE20;
        InitializeGlobalStateEntry(0);
        D_0013E05A[0] = 1;
    }

    if ((controller_state.pressed_unmasked & 0xD00) && menu_system.close_locked == 0) {
        return 1;
    }
    if (controller_state.pressed_unmasked & 0x10) {
        back = menu_system.current->back;
        if (back != NULL) {
            menu_system.next = back;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    }
    if (mode_freeze_state != 0x10 && mode_freeze_state != 1) {
        menu_system.next = menu_system.current->back;
        return 0;
    }
    if (memory_card_state.state >= 3 || memory_card_state.pending_state >= 0 || memory_card_state.card[0].type != 2) {
        return 0;
    }

    if (w->data.save.flags & 1) {
        pad = controller_state.raw_pressed;
    } else {
        pad = controller_state.pressed;
    }
    w->data.save.slot = D_0015EE34;
    if ((pad & 0x1000) && D_0015EE34 != 0) {
        w->data.save.slot = D_0015EE34 - 1;
    }
    if (pad & 0x4000) {
        cur = w->data.save.slot;
        if (cur < 4) {
            w->data.save.slot = cur + 1;
        }
    }
    cursor = w->data.save.slot;
    D_0015EE34 = cursor;
    if ((pad & 0x40) && memory_card_state.card[0].type == 2) {
        if (*(s32 *)((u8 *)&memory_card_state + cursor * 0x1C + 0x20) != -1) {
            menu_system.next = (w->data.save.flags & 0x2000) ? D_001D4F98 : D_001D4E38;
            menu_system.unkD4 = (w->data.save.flags & 0x2000) ? 2 : 1;
            D_001D2640[0] = w->data.save.slot;
        } else {
            w->data.save.step = 1;
        }
    } else if (pad & 0x20) {
        memory_card_state.unkF4 = 0;
        fl = mode_freeze_flags & ~2;
        mode_freeze_flags = fl & ~4;
        load_and_initialize_level_chunk();
        InitializeGlobalStateEntry(0);
        D_0013E05A[0] = 1;
    }
    if (w->data.save.slot != old) {
        allocate_voice_for_target_entry(1, 0x11, w->moby);
    }
    return 0;
}

extern __typeof__(saving_data_menu2) func_002235B8 __attribute__((alias("FUN_002235b8")));
