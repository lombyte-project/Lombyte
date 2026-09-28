/* Ported from rac1-decomp, the PAL decompilation (src/core/00125630.c, func_00129C78). */
typedef struct Slot1B8 {
    /* 0x00 */ void *unk00;
    /* 0x04 */ void *unk04;
    /* 0x08 */ int   unk08;
    /* 0x0C */ void *unk0C;
} Slot1B8;  /* 0x10 */
typedef struct Handler {
    /* 0x00 */ void *fn;
    /* 0x04 */ int   data;
} Handler;  /* 0x8 */
typedef struct Obj40 {
    /* 0x000 */ char    unk000[0x4];
    /* 0x004 */ int     unk004;
    /* 0x008 */ int     unk008;
    /* 0x00C */ Handler handlers[0x14];
    /* 0x0AC */ int     unk0AC;
    /* 0x0B0 */ char    unk0B0[0x68];
    /* 0x118 */ int     unk118;
    /* 0x11C */ char    unk11C[0x4];
    /* 0x120 */ int     unk120;
    /* 0x124 */ char    unk124[0x2C];
    /* 0x150 */ int     unk150;
    /* 0x154 */ char    unk154[0x20];
    /* 0x174 */ int     unk174;
    /* 0x178 */ char    unk178[0x40];
    /* 0x1B8 */ Slot1B8 slots[3];
    /* 0x1E8 */ char    unk1E8[0x638];
    /* 0x820 */ int     unk820;
} Obj40;
/* Does the request at arg1 fit the heap described by arg0? A sized
   request (+0xE0 non-zero) has to fit both the byte budget at +0xDC and
   the entry budget at +0xE0; an unsized one has to fit width * height
   against +0xE4. On a refusal, format the two figures into the message
   at D_00153A80 and report it. Returns whether it fits. */
int FUN_00129b38(void *arg0) {
    Obj40 *s = (Obj40 *)arg0;
    int r = 1;
    if (s->unk008 != 2) {
        int v = s->unk118;
        s->unk008 = 2;
        s->unk0AC = v;
    }
    s->unk820 = r;
    return r;
}

extern __typeof__(FUN_00129b38) func_00129B38 __attribute__((alias("FUN_00129b38")));
