#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_002256e8/FUN_002256e8.s", FUN_002256e8);
#else
#include "types.h"

typedef struct {
    u8 pad_0[0x20];
    u8 sub;
    u8 pad_21[0x52 - 0x21];
    u8 b52;
    u8 b53;
    u8 pad_54[0x70 - 0x54];
    u8 flags;
    u8 pad_71[0xBC - 0x71];
    u8 cnt;
} Track;

typedef struct {
    u8 pad_0[0x48];
    u8 *ptrs[4];
} Obj;

typedef struct {
    s32 off;
    s32 pad;
} Hdr;

typedef struct {
    u8 pad_0[0x34];
    s32 state;
    s32 pad_38;
    u8 *base;
    s32 off;
    Track *t;
} Stream;

extern s32 D_00137B80[];
extern s16 D_001516D8[];
extern s32 D_001516EC[];
extern s16 D_0015172A[];
extern s32 D_0015ED88;
extern s32 D_0015EE20;
extern Obj *D_001B3200[];
extern u8 D_001B4265[];
extern u8 D_001D5CBB[];

extern s32 func_001F96F8(s32) __asm__("FUN_001f96f8");
extern void relocate_asset_entry_pointers() __asm__("FUN_002032e0");
extern void func_0020B618(u8 *, u8 *) __asm__("FUN_0020b618");
extern void blend_moby_animation(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern s32 continue_audio_stream_if_ready(void) __asm__("FUN_00215b10");
extern s32 start_audio_stream_read(u8 *, s32, s32) __asm__("FUN_00216788");
extern s32 FUN_00225dd8(u8 *);

s32 fun_002256e8(Stream *s) __asm__("FUN_002256e8");

s32 fun_002256e8(Stream *s) {
    Track *t;
    Hdr *h;
    Obj **obj;
    s32 off;
    s32 i;
    s32 n;
    s32 k;

    switch (s->state) {
    case 0:
        if (D_001516D8[0] != 0) {
            break;
        }
        i = 0x4F000 - (D_00137B80[0x1614 / 4] << 11);
        if (start_audio_stream_read(s->base + i, D_00137B80[0x1610 / 4], D_00137B80[0x1614 / 4]) != 0) {
            s->off = i;
            s->state = 1;
            D_001D5CBB[0] = 1;
            FUN_00225dd8(s->base);
            return 0;
        }
        s->state = 3;
        break;
    case 1:
        if (D_001516D8[0] != 0) {
            break;
        }
        D_001D5CBB[0] = 0;
        func_0020B618(s->base + s->off, s->base);
        obj = &D_001B3200[D_001B4265[0]];
        h = (Hdr *)s->base;
        i = 0;
    next:
        n = (s32)(s->base + h[i].off);
        i++;
        (*obj)->ptrs[i] = (u8 *)n;
        relocate_asset_entry_pointers(*obj, i);
        if (i < 3) {
            goto next;
        }
        s->state = 2;
        if (D_0015EE20 == 0) {
            s->t->sub = 0;
        } else {
            s->t->sub = 4;
        }
        s->t->cnt = 0;
        break;
    case 2:
        t = s->t;
        switch (t->sub) {
        case 0:
        case 2:
        case 4:
            n = D_0015ED88 - 1;
            if (n < 0) {
                n = 0;
            }
            k = t->sub >> 1;
            if (t->cnt == 0) {
                D_001516EC[0] = k * 6 + n + 60000;
            }
            t->cnt++;
            if (t->cnt > func_001F96F8(0x78)) {
                t->cnt = func_001F96F8(0x78);
            }
            if (t->cnt < func_001F96F8(0x78)) {
                return 0;
            }
            if (D_0015172A[0] != 3) {
                return 0;
            }
            t->cnt = 0;
            t->sub++;
            blend_moby_animation(t, k + 1, 0, func_001F96F8(0x18));
            break;
        case 1:
        case 3:
        case 5:
            if (t->cnt == 0 && t->b52 == t->b53) {
                continue_audio_stream_if_ready();
                t->cnt = 1;
            }
            if (t->flags & 2) {
                if (t->sub == 1) {
                    t->sub = t->sub + 1;
                } else {
                    t->sub = 6;
                }
                t->cnt = 0;
                blend_moby_animation(t, 0, 0, func_001F96F8(0x18));
                break;
            }
            break;
        case 6:
            t->cnt++;
            if (t->cnt > func_001F96F8(0xF0)) {
                t->cnt = func_001F96F8(0xF0);
            }
            if (t->cnt < func_001F96F8(0xF0)) {
                return 0;
            }
            if (D_0015EE20 == 0) {
                t->sub = 0;
            } else {
                t->sub = 4;
            }
            t->cnt = 0;
            break;
        }
        break;
    }
    return 0;
}
#endif /* NON_MATCHING */
