#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/gameplay/state/transition_load_wad/FUN_001ea830.s", FUN_001ea830);
#else
#include "types.h"

typedef struct {
    s32 x0;             /* 0x00 */
    s32 data;           /* 0x04 */
    s32 x8;             /* 0x08 */
    s32 xC;             /* 0x0C */
    s32 x10;            /* 0x10 */
    s32 x14;            /* 0x14 */
    s32 n18;            /* 0x18 */
    s32 x1C;            /* 0x1C */
    s32 n20;            /* 0x20 */
    s32 x24;            /* 0x24 */
    s32 n28;            /* 0x28 */
    s32 x2C;            /* 0x2C */
    s32 n30;            /* 0x30 */
    s32 x34;            /* 0x34 */
    s32 n38;            /* 0x38 */
    s32 x3C;            /* 0x3C */
    s32 n40;            /* 0x40 */
    s32 x44;            /* 0x44 */
    s32 n48;            /* 0x48 */
    s32 x4C;            /* 0x4C */
    s32 x50;            /* 0x50 */
    s32 x54;            /* 0x54 */
    s32 x58;            /* 0x58 */
    s32 x5C;            /* 0x5C */
    s32 x60;            /* 0x60 */
    s32 x64;            /* 0x64 */
    s32 x68;            /* 0x68 */
    s32 x6C;            /* 0x6C */
    s32 x70;            /* 0x70 */
    s32 x74;            /* 0x74 */
    s32 x78;            /* 0x78 */
    s32 x7C;            /* 0x7C */
    s32 x80;            /* 0x80 */
    s32 x84;            /* 0x84 */
} WadHeader;

typedef struct {
    s32 offset;
    u16 x4;
    u16 pad6;
    s32 pad8[2];
} WadTex;

typedef struct {
    s32 offset;
    s32 x4;
    s32 pad8[2];
    u8 x10[0x10];
} WadClass20;

typedef struct {
    s32 offset;
    s32 x4;
    s32 pad8[2];
    u8 x10[0x10];
    u8 x20[0x10];
} WadClass30;

typedef struct {
    s32 offset;
    s32 size;
} WadSound;

typedef struct {
    u8 pad0[0x14];
    u8 *hdr;            /* 0x14 */
    s32 x18;            /* 0x18 */
    s32 x1C;            /* 0x1C */
} LoadState;

typedef struct {
    u8 pad0[0x14E8];
    s32 x14E8;
    s32 x14EC;
    u8 pad14F0[0x38];
    s32 x1528;
    s32 x152C;
} LevelInfo;

typedef struct {
    u8 pad0[0x58];
    s32 x58;
    s32 x5C;
    s32 x60[0x46];
} SoundBanks;

typedef struct {
    s32 offset;
    s32 pad[3];
} TextEntry;

typedef struct {
    u8 pad0[0x2C];
    s32 count;
} HelpState;

typedef struct {
    u8 pad0[0xD];
    u8 xD;
    u8 padE[0x1A];
    void *x28;
} MobyClass;

extern LevelInfo D_00137B80;
extern s32 D_0015EE74;
extern s32 D_0015EE78;
extern s32 D_0015EE8C;
extern s64 D_0015EF48;
extern s32 D_0015EF58;
extern s32 D_0015EF60;
extern s32 D_0015EF64;
extern s32 D_0015F460;
extern TextEntry *D_0015F6A0;
extern s32 D_0015FF00;
extern s32 D_0015FF08;
extern s32 D_001603CC;
extern s32 D_001603EC;
extern s32 D_00160E94;
extern s32 D_00160F0C;
extern s32 D_00160F4C;
extern s32 D_00160F64;
extern u8 D_001861E0[];
extern u8 D_00186310[];
extern SoundBanks D_0018CB20;
extern LoadState D_001940C0;
extern u8 D_00194180[];
extern HelpState D_001996D0;
extern u64 D_0019E6C0[];
extern MobyClass *D_001B3200[];
extern u8 D_001B3AC0[];
extern s32 D_001B5980[];
extern s32 D_001B6180[];
extern u8 D_001B6880[];
extern s32 D_001D84B0[];
extern s32 D_001E0900[];
extern s32 D_001E2600[];

extern void CalculateDmaTransferAddress(void);
extern void FillTransferWords(void *, s32, s32);
extern void FlushCache(s32);
extern void QueueDmaTransfer(s32);
extern void func_00120558(s32, s32);
extern s32 func_001E9B10(u8 *);
extern void func_001F2C60(void);
extern void func_001F2D98(void);
extern s32 func_001F97A0(s32);
extern void func_002015D8(void);
extern void func_002026C8(u8 *, u8 *, u8 *, s32);
extern void func_00202800(u8 *, s32);
extern void func_002028E0(u8 *);
extern void func_00203120(u8 *, s32, u8 *);
extern void func_00203640(u8 *, u8 *, u8 *, s32);
extern void func_00203730(u8 *, u8 *, u8 *, s32);
extern void func_00203B08(u8 *, u8 *, u8 *, u8 *, s32);
extern void func_002040E0(u8 *, u8 *);
extern void func_002049F0(s32);
extern s32 func_0020B618(u8 *, u8 *);
extern void func_00216788(u8 *, s32, s32);
extern void func_00216828(s32, s32, s32);
extern void func_002168A8(s32);
extern void func_002335D0(void);
extern s32 sceGsSetDefLoadImage(void *, s16, s16, s16, s16, s16, s16, s16);
extern s32 sceGsExecLoadImage(void *, u8 *);

extern s32 D_0015EF64_far __asm__("D_0015EF64") __attribute__((section(".data")));
void transition_load_wad(void) __asm__("FUN_001ea830");

void transition_load_wad(void) {
    u8 li[0x60];
    WadHeader *hdr;
    u8 *data;
    u8 *base;
    s32 size;
    s32 i;
    s32 j;
    s32 k;
    s32 cnt;
    s32 v;
    s32 *out;
    WadTex *tex;
    WadClass20 *c20;
    WadClass30 *c30;
    WadSound *snd;
    TextEntry *te;
    s32 k_800 = 0x800;
    s64 t;
    s64 u;

    D_0015EF58 = 0;
    i = 0;
    CalculateDmaTransferAddress();
    func_002015D8();
    D_00160F0C = 0x100000;
    D_0015EE8C = 0x2C0000;
    D_0015EE78 = 0x2C0000;
    D_0015EE74 = 0x2C0000;
    FillTransferWords(D_00194180, 0x87654321, 0x10);
    FillTransferWords(D_001B3AC0, -1, k_800);
    FillTransferWords(D_001B6880, -1, 0xE00);
    FillTransferWords(D_001B6180, 0, 0xE0);
    func_001F2C60();
    func_001F2D98();
    func_002335D0();
    func_00216788(D_001940C0.hdr + 0x1000000, D_00137B80.x14E8, D_00137B80.x14EC);
    func_002168A8(1);
    FlushCache(0);
    size = func_0020B618(D_001940C0.hdr + 0x1000000, D_001940C0.hdr);
    FlushCache(0);
    hdr = (WadHeader *)D_001940C0.hdr;
    func_00203120((u8 *)hdr + hdr->x0, hdr->x8, (u8 *)hdr + hdr->xC);
    t = (s64)((D_0015EE8C + hdr->x70) >> 8) | 0x1D308000;
    u = ((s64)((D_0015EE8C + hdr->x74) >> 8) << 37) | ((s64)0xB800 << 19);
    D_0019E6C0[0] = (t | u) | ((s64)-1 << 63);
    data = (u8 *)hdr + hdr->data;
    D_0019E6C0[2] = 0x0040000400004000;
    base = data + hdr->x60;
    D_0019E6C0[1] = 0xFFA0000000E0;

    tex = (WadTex *)((u8 *)hdr + hdr->x34);
    cnt = hdr->n30;
    D_00160E94 = cnt;
    if (cnt > 0) {
        do {
            D_001E0900[i] = (s32)base + tex[i].offset + (func_001F97A0(tex[i].x4) << 28);
            i++;
        } while (i < D_00160E94);
    }
    k = 0;
    tex = (WadTex *)((u8 *)hdr + hdr->x3C);
    cnt = hdr->n38;
    D_0015FF08 = cnt;
    if (cnt > 0) {
        do {
            D_001B5980[k] = (s32)base + tex[k].offset + (func_001F97A0(tex[k].x4) << 28);
            k++;
        } while (k < D_0015FF08);
    }
    k = 0;
    tex = (WadTex *)((u8 *)hdr + hdr->x44);
    cnt = hdr->n40;
    D_00160F64 = cnt;
    if (cnt > 0) {
        do {
            D_001E2600[k] = (s32)base + tex[k].offset + (func_001F97A0(tex[k].x4) << 28);
            k++;
        } while (k < D_00160F64);
    }
    k = 0;
    tex = (WadTex *)((u8 *)hdr + hdr->x4C);
    cnt = hdr->n48;
    D_001603EC = cnt;
    if (cnt > 0) {
        do {
            D_001D84B0[k] = (s32)base + tex[k].offset + (func_001F97A0(tex[k].x4) << 28);
            k++;
        } while (k < D_001603EC);
    }

    func_002040E0(data + hdr->x10, (u8 *)hdr + hdr->x34);
    func_002028E0(data + hdr->x14);
    c20 = (WadClass20 *)((u8 *)hdr + hdr->x1C);
    D_0015FF00 = 0;
    D_00160F4C = 0;
    D_001603CC = 0;
    for (i = 0; i < hdr->n18; i++) {
        func_00203640(c20->offset != 0 ? data + c20->offset : 0, (u8 *)hdr + hdr->x3C, c20->x10, c20->x4);
        c20++;
    }
    c20 = (WadClass20 *)((u8 *)hdr + hdr->x24);
    for (i = 0; i < hdr->n20; i++) {
        func_00203730(data + c20->offset, (u8 *)hdr + hdr->x44, c20->x10, c20->x4);
        c20++;
    }
    c30 = (WadClass30 *)((u8 *)hdr + hdr->x2C);
    for (i = 0; i < hdr->n28; i++) {
        func_00203B08(data + c30->offset, (u8 *)hdr + hdr->x4C, c30->x10, c30->x20, c30->x4);
        c30++;
    }
    D_0015F460 = (s32)(data + hdr->x68);
    func_00202800((u8 *)hdr + hdr->x5C, hdr->x58);
    func_002026C8((u8 *)hdr + hdr->x6C, data + hdr->x64, (u8 *)hdr + hdr->x54, hdr->x50);
    sceGsSetDefLoadImage(li, (D_0015EE74 << 8) >> 16, 4, 0, 0, 0, 0x100, 0x80);
    FlushCache(0);
    sceGsExecLoadImage(li, data + hdr->x84);
    func_00120558(0, 0);
    D_001940C0.x18 = (s32)D_001940C0.hdr + size;
    v = D_0015EE74;
    D_0015EF48 = (v >> 8) | 0x20010000 | (s64)0xB800 << 19;
    D_0015EE78 = v + 0x20000;
    D_0015EE74 = v + 0x20000;
    D_001940C0.x1C = func_001E9B10(data + hdr->x7C);
    FillTransferWords(&D_0018CB20, 0, 0x1C0);
    FillTransferWords(D_00186310, 0, 0x40);
    v = D_001940C0.x1C;
    k = v + 0x40000;
    D_0018CB20.x58 = v;
    D_0018CB20.x5C = k;
    D_001940C0.x1C = k + 0x40000;
    snd = (WadSound *)(data + hdr->x80);
    j = 0;
    if (snd->size != 0) {
        out = D_0018CB20.x60;
        do {
            j++;
            *out = (s32)(data + hdr->x80) + (snd->offset + k_800);
            snd++;
            out++;
        } while (j < 0x46 && snd->size != 0);
    }
    func_002049F0(0);
    D_0015EF60 = D_001940C0.x1C;
    func_00216828(D_001940C0.x1C, D_00137B80.x1528, D_00137B80.x152C);
    D_0015EF64_far = D_0015EF60;
    D_001940C0.x1C = D_0015EF60 + (D_00137B80.x152C << 11);
    for (i = 0; i < 8; i++) {
        QueueDmaTransfer(i);
        j = 0;
        if (D_001996D0.count > 0) {
            te = D_0015F6A0;
            v = (s32)te - 8;
            do {
                te[j].offset += v;
                j++;
            } while (j < D_001996D0.count);
        }
    }
    QueueDmaTransfer(0);
    i = D_001B3AC0[0x472];
    if (i >= 0) {
        D_001B3200[i]->x28 = D_001861E0;
        D_001B3200[i]->xD = 5;
    }
}
#endif /* NON_MATCHING */
