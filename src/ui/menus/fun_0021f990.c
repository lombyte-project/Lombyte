#include "types.h"

typedef struct {
    s32 a;
    s32 b;
} Entry;

typedef struct {
    u8 pad0[0x8];
    s32 unk8;
    s32 unkC;
    u8 pad10[0x10];
    u8 data20[0x400];
    u8 data420[1];
} Buf;

typedef struct {
    u8 pad0[0x30];
    Entry *tbl;
    s32 flags;
    u8 pad38[0xC];
    s32 state;
    Buf *buf;
    u8 pad4C[0x4];
    s32 last;
    u8 pad54[0x4];
    s32 idx;
    u8 pad5C[0x4];
    s32 off;
} Obj;

typedef struct {
    s32 unk0;
    s32 val;
    u8 pad8[0x14];
} Slot;

typedef struct {
    u8 pad0[0x1C];
    Slot slots[6];
    u8 padC4[0x10];
    s32 unkD4;
    u8 padD8[0x4];
    s32 unkDC;
} Cfg;

typedef struct {
    u8 pad0[0x3C];
    s32 unk3C;
    s32 unk40;
} Sub;

typedef struct {
    u8 pad0[0x40];
    Sub *sub;
} Game;

typedef struct {
    u8 pad0[0x244];
    s32 unk244;
    s32 unk248;
    u8 pad24C[0xC];
    s64 unk258;
} State;

extern Cfg D_0013D290;
extern s16 D_001516D8[];
extern State D_001A00F0;
extern s32 D_001A0314[];
extern Game *D_001D5BF4[];

extern s32 FUN_001f97a0(s32);
extern s64 FUN_00204e30(s32, s32, u8 *, u8 *, s32, s32);
extern void FUN_0020b4a8(void);
extern void FUN_0020b618(u8 *, Buf *);
extern s32 start_audio_stream_read(u8 *, s32, s32) __asm__("FUN_00216788");
extern s32 get_stream_buffer_size(Buf *) __asm__("FUN_00225d88");
extern s32 FUN_00225dd8(Buf *);
extern s32 clear_record_flag_by_key(Buf *) __asm__("FUN_00225e20");

s32 FUN_0021f990(Obj *o) {
    s32 flags;
    s32 idx;
    s32 r;
    s32 x;
    s32 y;
    u8 *p;
    Buf *b;
    u8 *p20;
    u8 *p420;

    flags = o->flags;
    if (flags & 1) {
        idx = o->idx;
        if (idx == -1) {
            return 0;
        }
    } else if (flags & 2) {
        idx = D_001A0314[0];
    } else if (flags & 4) {
        idx = D_001D5BF4[0]->sub->unk3C;
    } else if (flags & 0x100) {
        idx = D_001D5BF4[0]->sub->unk40;
        if (idx <= -1) {
            idx = 0;
        }
        if (idx >= 5) {
            idx = 4;
        }
        if (D_0013D290.unkD4 < 3 && D_0013D290.unkDC < 0) {
            if (o->state == -1) {
                o->state = 0;
            }
            idx = D_0013D290.slots[idx].val;
        } else {
            o->state = -1;
        }
    } else {
        idx = D_001D5BF4[0]->sub->unk40;
        if (idx <= -1) {
            idx = 0;
        }
    }

    switch (o->state) {
    case 0:
    case 2:
        if (idx == o->last) {
            break;
        }
        if (o->buf == 0) {
            break;
        }
        if (D_001516D8[0] != 0) {
            break;
        }
        if (o->tbl[idx].b == 0) {
            break;
        }
        p = (u8 *)o->buf;
        if (o->flags & 0x20) {
            r = get_stream_buffer_size(o->buf) - (o->tbl[idx].b << 11);
            o->off = r;
            p += r;
        }
        if (o->flags & 0x10) {
            r = start_audio_stream_read(p, o->tbl[idx].a, o->tbl[idx].b);
        } else {
            r = start_audio_stream_read(p, o->tbl[idx].a, o->tbl[idx].b);
        }
        if (r != 0) {
            FUN_00225dd8(o->buf);
            o->last = idx;
            o->state++;
        } else {
            o->state = -1;
        }
        break;
    case 1:
    case 3:
        if (D_001516D8[0] != 0) {
            break;
        }
        clear_record_flag_by_key(o->buf);
        if (o->flags & 0x20) {
            FUN_0020b618((u8 *)o->buf + o->off, o->buf);
            o->off = 0;
        }
        b = o->buf;
        p20 = b->data20;
        p420 = b->data420;
        x = FUN_001f97a0(b->unk8);
        y = FUN_001f97a0(b->unkC);
        D_001A00F0.unk258 = FUN_00204e30(x, y, p20, p420, D_001A00F0.unk244, D_001A00F0.unk248);
        FUN_0020b4a8();
        o->state = 2;
        break;
    }
    return 0;
}

extern __typeof__(FUN_0021f990) func_0021F990 __attribute__((alias("FUN_0021f990")));
