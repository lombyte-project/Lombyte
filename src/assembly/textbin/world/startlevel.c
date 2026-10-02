#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/world/startlevel/FUN_001e9658.s", FUN_001e9658);
#else
#include "types.h"
#include "sda.h"

typedef struct {
    s32 off;
    s32 size;
} LevelChunk;

typedef struct {
    s32 gfx;
    s32 pad4;
    s32 gfx_alt;
    s32 padC;
    LevelChunk intro[6];
    LevelChunk loading[6];
    s32 code;
} LevelHeader;

typedef struct {
    s32 pad[7];
    s32 bank;
} SoundSlot;

extern s32 D_0015F5E8 MACRO_ADDR;
extern u8 D_00161280[];
extern u8 D_00165430[];
extern s32 D_0015F438 MACRO_ADDR;
extern s32 D_0015F604 MACRO_ADDR;
extern s32 D_0015EF5C MACRO_ADDR;
extern s32 D_0015EED8 MACRO_ADDR;
extern s32 D_0015ED80 MACRO_ADDR;
extern s32 D_0015ED84 MACRO_ADDR;
extern s32 D_0015ED88;
extern u8 D_24135F[];
typedef struct { u8 pad[0x1A4]; s32 unk1A4; } PadState;
extern PadState D_0013C940;
extern s32 D_00139378[];
extern s32 D_00139380[];
extern char D_001E76C0[];
extern s32 D_00137B80[];
extern SoundSlot D_00186100[];
extern SoundSlot D_001861E0[];
extern SoundSlot *D_0015F634;
extern s32 D_0015F630;
extern u8 D_0016034C;
extern s32 D_0015F600;
extern u8 D_0013E030[];

extern void func_00201650(void);
extern void InitializeStreamingState(void);
extern void func_002335D0(void);
extern void func_00233D00(void);
extern void ClearDmaQueueEntry(void);
extern s32 sceGsSyncV(s32);
extern void func_001FB2A8(void);
extern void PackDmaTag(s32, u64, u64);
extern void FlushCache(s32);
extern void func_0020B618(s32, s32);
extern void func_001FB2D0(void);
extern void func_001FB368(void);
extern void func_002012B8(s32);
extern void func_001FB3D0(void);
extern void func_001FB6E0(void);
extern void func_002336A0(void);
extern void func_00233630(void);
extern void func_002337B0(s32);
extern s32 check_memory_card(void) __asm__("FUN_00209168");
extern void func_00217A10(void);
extern void func_001F4A58(s32);
extern void func_0023A3B8(s32, s32, s32, s32, s32);
extern s32 FUN_001f96f8(s32);
extern void DebugPrint(char *, ...);
extern s32 func_0022D708(s32);
extern void func_0012E1A8(void);
extern s32 transition_do_transition(void) __asm__("FUN_001eb798");
extern s32 func_001204B8(void);
extern void sceGsResetGraph(s16, s16, s16, s16);
extern void func_001F34E8(void);
extern void do_space_transition(void) __asm__("FUN_00231ff0");

static inline void clear_bytes(u8 *p, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        p[i] = 0;
    }
}

void startlevel(void) __asm__("FUN_001e9658");

void startlevel(void) {
    LevelHeader *hdr;
    LevelChunk *tbl;
    s32 n;
    s32 i;
    s32 cur;
    s32 prev;
    s32 frames;
    s32 code;
    s32 bank;
    u8 *p;

    D_0015F5E8 = 1;
    func_00201650();
    InitializeStreamingState();
    for (i = 0; i < D_00165430 - D_00161280; i++) {
        D_00161280[i] = 0;
    }
    prev = 0;
    frames = 0;
    func_002335D0();
    func_00233D00();
    ClearDmaQueueEntry();
    D_0015F438 = 0;
    sceGsSyncV(0);
    D_0015F604 = 0;
    func_001FB2A8();
    PackDmaTag(0, 0, 0);
    hdr = (LevelHeader *)(((u32)D_24135F & 0xFFFFC000) + 0x2C0000);
    while ((cur = check_memory_card()) != 0 && (frames < 11 || D_0013C940.unk1A4 == 0)) {
        if (cur != prev) {
            if (cur == 1) {
                tbl = hdr->intro;
            } else {
                tbl = hdr->loading;
            }
            FlushCache(0);
            func_0020B618(tbl[D_0015ED88].off + (s32)hdr, hdr->code + (s32)hdr);
            FlushCache(0);
            func_002335D0();
            PackDmaTag(0, 0, 0);
            func_001FB2D0();
            func_001FB368();
            func_002012B8(hdr->code + (s32)hdr);
            func_001FB3D0();
            func_001FB6E0();
            func_002336A0();
            func_00233630();
            func_002337B0(1);
        }
        sceGsSyncV(0);
        prev = cur;
        func_00217A10();
        frames++;
    }
    if (prev != 0) {
        func_001F4A58(10);
    }
    D_0015EED8 = -1;
    code = hdr->code + (s32)hdr;
    D_0015EF5C = code;
    if (D_0015ED80 == 0) {
        func_0023A3B8(D_00139378[0], D_00139378[1], (code + 0x3F) & ~0x3F,
                      (code + 0x2C003F) & ~0x3F, 0);
    } else {
        func_0023A3B8(D_00139380[0], D_00139380[1], (code + 0x3F) & ~0x3F,
                      (code + 0x2C003F) & ~0x3F, 0);
    }
    D_0015EED8 = 0;
    func_001F4A58(FUN_001f96f8(0x12));
    FlushCache(0);
    if (D_0015ED80 != 0) {
        func_0020B618(hdr->gfx_alt + (s32)hdr, hdr->code + (s32)hdr);
    } else {
        func_0020B618(hdr->gfx + (s32)hdr, hdr->code + (s32)hdr);
    }
    FlushCache(0);
    func_002335D0();
    PackDmaTag(0, 0, 0);
    func_001FB2D0();
    func_001FB368();
    func_002012B8(hdr->code + (s32)hdr);
    func_001FB3D0();
    func_001FB6E0();
    func_002336A0();
    func_00233630();
    func_002337B0(1);
    sceGsSyncV(0);
    D_0015F438++;
    DebugPrint(D_001E76C0);
    bank = func_0022D708(D_00137B80[0x14E0 / 4]);
    func_0012E1A8();
    D_0015F634 = D_00186100;
    D_00186100[0].bank = bank;
    D_00186100[1].bank = bank;
    D_00186100[2].bank = bank;
    D_00186100[3].bank = bank;
    D_00186100[4].bank = bank;
    D_00186100[5].bank = bank;
    D_001861E0[0].bank = bank;
    D_001861E0[1].bank = bank;
    D_001861E0[2].bank = bank;
    D_00186100[6].bank = bank;
    D_0015F630 = 7;
    D_001861E0[3].bank = bank;
    D_001861E0[4].bank = bank;
    transition_do_transition();
    if (D_0015ED80 != D_0016034C) {
        D_0015ED80 = D_0016034C;
        func_001204B8();
        sceGsResetGraph(0, 1, D_0015ED80 != 0 ? 3 : 2, 0);
        func_001F34E8();
        func_001FB2A8();
    }
    p = D_0013E030;
    D_0015F600 = D_0015ED84;
    *(s16 *)(p + 0x2A) = 1;
    D_0015ED84 = -1;
    do_space_transition();
}
#endif /* NON_MATCHING */
