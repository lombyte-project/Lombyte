#include "types.h"

struct Goal {
    s32 bolts;
    s32 boltsHard;
    u16 cost;
    u16 costHard;
    u16 pad0C;
    u16 need;
    u8 pad10[8];
};

struct Slot {
    s32 id;
    s32 kind;
    u8 pad8[0xC];
};

struct Shop {
    u8 pad0[0x40];
    s32 hard;
    u8 pad44[0x14];
    s32 cur;
    s32 open;
    u8 pad60[0x70];
    struct Slot slots[1];
};

extern struct Shop D_001E63C0;
extern struct Goal D_001DFFB0[];
extern s32 D_0013D428[];
extern u8 D_0013D4E3[];
extern s32 D_0015ED98;
extern void draw_framebuffer_rect(s32, s32, s32, s32, s32, s32, u32) __asm__("func_001FB8F0");
extern s32 get_help_message_text(s32) __asm__("func_001FDD10");
extern void font_print_center_small(s32, s32, u64, s32, s32) __asm__("func_001F6B88");

void FUN_00239160(void) {
    struct Shop *s;
    s32 id;
    s32 cost;

    draw_framebuffer_rect(0, 0, 0x200, 0x80, 0x200, 0x80, 0);
    s = &D_001E63C0;
    if (s->open != 0) {
        if (s->slots[s->cur].kind == 1) {
            if (D_0013D428[s->slots[s->cur].id] >= D_001DFFB0[s->slots[s->cur].id].need) {
                return;
            }
        }
        if (s->slots[s->cur].kind == 1) {
            if (s->hard != 0) {
                cost = D_001DFFB0[s->slots[s->cur].id].costHard;
            } else {
                cost = D_001DFFB0[s->slots[s->cur].id].cost;
            }
            if (D_0015ED98 < cost) {
                return;
            }
            id = 0x4EE0;
        } else {
            if (D_0013D4E3[0] != 0) {
                cost = D_001DFFB0[s->slots[s->cur].id].boltsHard;
            } else {
                cost = D_001DFFB0[s->slots[s->cur].id].bolts;
            }
            if (D_0015ED98 < cost) {
                return;
            }
            id = 0x524B;
        }
        font_print_center_small(0x28, 0x14, 0x80F0F0F0, get_help_message_text(id), -1);
    } else {
        font_print_center_small(0x28, 0x14, 0x80F0F0F0, get_help_message_text(0x4EE0), -1);
    }
}

extern __typeof__(FUN_00239160) func_00239160 __attribute__((alias("FUN_00239160")));
