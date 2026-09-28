/* Ported from rac1-decomp, the PAL decompilation (src/game/space.c, func_0022F128). */
/* ---- inlined common.h ---- */
#ifndef COMMON_H
#define COMMON_H


/* Standard fixed-width types for PS2 Emotion Engine (GCC 2.95.3) */
typedef signed char s8;
typedef short s16;
typedef int s32;
typedef long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef float f32;
typedef double f64;

/*
 * Keeps a variable OUT of the small-data area.
 *
 * Retail addresses some globals via $gp (the MIPS small-data area, base
 * 0x00166D00 from its own .reginfo, window 0x15ED00..0x16ED00). To
 * reproduce those we must build with a nonzero -G, but that makes the
 * compiler use $gp for EVERY small global -- including the ~60 that live
 * far outside the window, whose references then can't reach
 * ("relocation truncated to fit: R_MIPS_GPREL16").
 *
 * Placement is decided per variable by its declared size, but the
 * incomplete-array trick (`extern char x[];`) changes how the variable
 * must be spelled at every use site, which is unacceptable for scalars
 * inside already-matching functions. An explicit section attribute
 * achieves the same thing while leaving use sites untouched: the
 * compiler knows the variable isn't in .sdata and falls back to lui/lo.
 *
 * Verified directly: a plain `extern unsigned char x;` compiles to
 * `lbu $v0,0($gp)` at -G8, the same declaration with this macro compiles
 * to `lui`/`lbu`. (`aligned` does NOT work -- it stays gp-relative.)
 */
#define NOT_SDA __attribute__((section(".data")))

/*
 * Loads a global with retail's one-register form:
 *     lui $2,%hi(D) / lw $2,%lo(D)($2)
 * where plain declarations give the split form this compiler prefers:
 *     lui $2,%hi(D) / lw $3,%lo(D)($2)
 *
 * The section name makes the compiler treat the symbol as small data, so
 * it emits the unsplit assembler macro `lw $2,D`. The declared size is
 * still over -G2, so the assembler does not use $gp for it and expands the
 * macro through the destination register. The register choice around the
 * load follows, because the compiler allocated one pseudo, not two.
 *
 * Loads only. A store to such a symbol becomes a two-instruction macro
 * (`lui $at` / `sw`) that the compiler still counts as one instruction, so
 * it can land in a delay slot; the assembler then warns "macro used after
 * .set nomacro". Check the make log for that warning.
 */
#define MACRO_ADDR __attribute__((section(".sdata")))

/*
 * Copies one 16-byte quadword from src to dst through $2, the way retail's
 * own source did: an inline-asm copy shaped like libvu0's sceVu0CopyVector
 * (which uses $6). Each address goes into its own register and is read at
 * offset 0, which no C copy reproduces: a long long or aligned-struct copy
 * folds the offset into lq/sq. First matched on FUN_001ec868 (camera.c).
 *
 * This is the one sanctioned inline asm. Candidates call it; they never
 * write asm inside a function themselves (tools/integrate.py refuses that).
 */
#include "qcopy.h"

#endif /* COMMON_H */
/* ---- inlined structs.h ---- */
#ifndef STRUCTS_H
#define STRUCTS_H

/*
 * Recovered struct layouts.
 *
 * These exist to retire the wall of `*(int *)(s + 0x174)` in src/, which
 * is the long-term goal for this project. They are held to one hard
 * rule: **introducing a struct must not change a single byte**. Field
 * access through a correctly-laid-out struct compiles identically to the
 * offset arithmetic it replaces, so every conversion is verified with
 * tools/sweep_matches.py and reverted if the count moves.
 *
 * Fields are named only where their purpose is actually established.
 * `unkNN` is deliberate: a wrong name is worse than no name, and this
 * file is read as documentation.
 */

/*
 * Node at +0x1E4 of a larger object. Ghidra's caller cross-reference is
 * what identified the embedding: std(parent + 0x1E4, 4, 0,
 * parent), i.e. it is constructed in place and handed a back-pointer to
 * its parent, which is what `owner` holds.
 *
 * The four function pointers at 0x20..0x2C are installed together by
 * that same constructor and always with the same four routines, so this
 * is a fixed dispatch block rather than a per-instance vtable.
 *
 * The dispatch routines themselves (__sread, __sseek) fill
 * in the rest: they pass `owner` and `handle` down to the layer below,
 * accumulate into `pos`, and set/clear bit 0x1000 of `flags` to mark
 * whether that call succeeded. Note 0x1C and 0x54 are different things
 * -- 0x1C is a pointer to the node itself, 0x54 is the parent -- which
 * is only visible once the constructor and a dispatch routine are read
 * together.
 */
typedef struct Node1E4 {
    /* 0x00 */ int   unk00;
    /* 0x04 */ int   unk04;
    /* 0x08 */ int   unk08;
    /* 0x0C */ short flags;     /* bit 0x1000: last dispatch succeeded */
    /* 0x0E */ short handle;    /* passed down to the layer below */
    /* 0x10 */ int   unk10;
    /* 0x14 */ int   unk14;
    /* 0x18 */ int   unk18;
    /* 0x1C */ void *self;      /* points at this node */
    /* 0x20 */ void *fn20;
    /* 0x24 */ void *fn24;
    /* 0x28 */ void *fn28;
    /* 0x2C */ void *fn2C;
    /* 0x30 */ char  unk30[0x20];
    /* 0x50 */ int   pos;       /* accumulated by the dispatch routines */
    /* 0x54 */ void *owner;     /* the parent object this is embedded in */
} Node1E4;

/*
 * Object handled by the func_0023Cxxx family. Recovered by reading the
 * family together rather than one function at a time: the constructor
 * (FUN_0023ad10) shows which fields are cleared, FUN_0023acb8 shows
 * which are handed to the layer below, and func_0023AEE0/FUN_0023aef0
 * show which are tested.
 *
 * Only `state` is named. It is set to 0 by the constructor, set to 2
 * after the submit in FUN_0023acb8, and tested non-zero before
 * teardown in FUN_0023aef0 -- that is enough to call it a state. The
 * rest keep unkNN: 0x4C is rounded down to a 0x400 multiple and 0x50 is
 * compared against 0x1000, which hints at a buffer size and a fill
 * level, but hinting is not knowing and a wrong name here would
 * propagate into every caller.
 */
typedef struct Obj23C {
    /* 0x00 */ int  state;
    /* 0x04 */ char unk04[0x10];
    /* 0x14 */ int  unk14;
    /* 0x18 */ int  unk18;
    /* 0x1C */ char unk1C[0x14];
    /* 0x30 */ int  unk30;
    /* 0x34 */ int  unk34;
    /* 0x38 */ int  unk38;
    /* 0x3C */ int  unk3C;
    /* 0x40 */ int  unk40;
    /* 0x44 */ int  unk44;
    /* 0x48 */ int  unk48;
    /* 0x4C */ int  unk4C;
    /* 0x50 */ int  unk50;
    /* 0x54 */ int  unk54;
    /* 0x58 */ int  unk58;
    /* 0x5C */ int  unk5C;
} Obj23C;

/*
 * A 3-element array at +0x1B8 of some parent object, stride 0x10.
 *
 * The stride is not a guess from one function: ClearMpegReferenceBuffer walks
 * +0x1B8/+0x1C8/+0x1D8 and then +0x1BC/+0x1CC/+0x1DC (the +0x00 and
 * +0x04 fields, column-major), while the banked decode of _outputFrame
 * independently uses +0x1B8/+0x1C4, +0x1C8/+0x1D4, +0x1D8/+0x1E4 -- the
 * +0x00 and +0x0C fields of the same three bases. Two unrelated
 * functions agreeing on the same 0x10 grid is what makes this a real
 * layout rather than a pattern in one function's offsets.
 *
 * Nothing is named. unk00 and unk04 are pointers to objects that have
 * something at their own +0x28 (ClearMpegReferenceBuffer zeroes it). unk0C is
 * chosen *instead of* unk00 in _outputFrame depending on a mode field,
 * so it is probably a pointer of the same kind -- "probably" is why it
 * keeps its unk name. unk08 is never touched by anything decompiled so
 * far and is a placeholder holding the stride, not an observed field.
 *
 * Note the array runs 0x1B8..0x1E8, so its last element covers 0x1E4.
 * That is NOT the Node1E4 above: a different parent object happens to
 * have a node at the same offset. Do not conflate them.
 */
typedef struct Slot1B8 {
    /* 0x00 */ void *unk00;
    /* 0x04 */ void *unk04;
    /* 0x08 */ int   unk08;
    /* 0x0C */ void *unk0C;
} Slot1B8;  /* 0x10 */

/*
 * The object a wrapper holds at its +0x40, and the owner of the Slot1B8
 * array above.
 *
 * The identification is by call site, not by pattern-matching offsets:
 * _sceMpegFlush computes `inner = *(p + 0x40)` and then calls
 * _lastFrame(inner) directly, which is what proves _lastFrame's
 * argument is this object and not the wrapper. FUN_00129b38 shares the
 * 0x08/0xAC/0x118 field set with it, so it takes this object too.
 *
 * Nothing here is named -- the field roles are not established yet.
 * unk118 and unk0AC are subtracted from each other in _sceMpegFlush to
 * produce the wrapper's 0x08, and unk008 is compared against 2 as a
 * state, but that is suggestive rather than settled.
 *
 * SIZE IS NOT KNOWN. The trailing padding runs to 0x820 only because
 * that is the highest field anything decompiled so far touches
 * (FUN_00129b38), so sizeof(Obj40) is a floor, not the real size. Do
 * not embed this by value, allocate it, or index an array of it -- it is
 * only ever used through a pointer to memory the game already owns.
 */
/*
 * One entry of the handler table at Obj40 +0x0C.
 *
 * This is what explains a shape that looks wrong in the raw offsets.
 * AddMpegCallback indexes with a stride of 8 but writes at +0x0C and
 * +0x10, which overlaps the next entry and reads as nonsense; so does
 * _dispatchMpegCallback, which loads a function pointer from +0x0C and calls it
 * with the value at +0x10. Both make sense the moment the array is
 * based at +0x0C instead of at +0x00: entry i is at 0x0C + i*8, fn at
 * its +0x00 and data at its +0x04. The two functions then line up
 * exactly -- AddMpegCallback installs the pair that _dispatchMpegCallback later
 * loads and invokes.
 *
 * The array LENGTH (0x14) is not established. It is the span between
 * +0x0C and the next known field at +0xAC, so it is an upper bound on
 * what fits, not a count anything observed. The two indexed accessors
 * take their index from the caller and never bound it. func_0012CC80
 * is the one place a constant index appears -- it passes +0x4C, which
 * is exactly &handlers[8] -- and that is the only direct evidence the
 * array reaches even that far.
 */
/*
 * WARNING -- converting callers to this struct is NOT byte-neutral.
 *
 * The three functions in this family (ClearMpegReferenceBuffer, AddMpegCallback,
 * _dispatchMpegCallback) were converted to struct access and all three changed
 * size against retail: 84->88, 36->32, 80->84. They have been restored
 * to raw offset arithmetic and are byte-exact again. A size mismatch is
 * the most expensive mistake in this project -- it shifts every later
 * function -- so this is not a near-miss to tolerate.
 *
 * The layout below is still believed correct and is kept for reading:
 * basing the array at +0x0C rather than +0x00 is what makes
 * AddMpegCallback's stride-8 indexing with writes at +0xC/+0x10 stop
 * overlapping the next entry, and _dispatchMpegCallback loads and calls exactly
 * the pair AddMpegCallback installs. But "the layout explains the code"
 * and "the struct compiles to the same instructions" are different
 * claims, and only the first one is established here.
 *
 * Before reusing this for conversion, rebuild and check the sweep --
 * do not assume byte-neutrality from the layout being right.
 */
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

/*
 * The wrapper that holds an Obj40 at its +0x40. Small, but it is the
 * object most of the func_0012Bxxx/func_0012Cxxx entry points actually
 * receive -- they immediately load ->obj and work through that, which
 * is the indirection _sceMpegFlush makes explicit by loading it and
 * passing it straight to _lastFrame.
 */
typedef struct Wrapper {
    /* 0x00 */ char   unk00[0x8];
    /* 0x08 */ int    unk08;
    /* 0x0C */ char   unk0C[0x34];
    /* 0x40 */ Obj40 *obj;
} Wrapper;

/*
 * OPEN QUESTION -- deliberately NOT defined: the global at D_0013D390.
 *
 * Two functions touch it. func_00209290 gives 0xB0, 0xCC, 0xE4, 0xE8
 * and 0x20/0x3C/0x58/0x74/0x90; func_00209418 gives 0x1C, 0xE4, 0xE8.
 * The 0x20..0x90 run is a clean stride-0x1C array of five elements,
 * each with a sentinel of -1 in its first word.
 *
 * The blocker is that func_00209290 also indexes `(b + 0xB0) + idx *
 * 0xC0`, and a 0xC0 stride starting at 0xB0 swallows 0xCC, 0xE4 and
 * 0xE8, which the same function writes as plain fields. Those two
 * readings cannot both be siblings in one struct, and two functions is
 * not enough to say which is wrong -- 0xB0 may not be the true array
 * base, or the index may be bounded in a way neither function shows.
 *
 * Forcing a layout here would bake in a guess, so this stays raw until
 * a third user of D_0013D390 is decompiled and settles it.
 */

#endif /* STRUCTS_H */

/*
 * space.cpp in the original source; text 0x22F128-0x233FF8.
 * Name and boundary from the NTSC split in bordplate's RC1 project
 * (codeberg.org/bordplate/RC1), mapped to PAL by matching function
 * sizes -- see docs/DECOMP_PROGRESS.md. Compiled as C for now.
 */

/* Declarations in scope here before the split. */
extern char D_0013E650[];
extern int D_0015F694;
extern void FUN_001f98d0(void *, void *, int);
extern char D_00189310[];
extern char D_001899D0[];
extern void *D_001871C0 NOT_SDA;
typedef struct {
    char unk_00[8];
    void (*fn_08)(void *);
    char unk_0C[4];
    void (*fn_10)(void *);
} DispatchRec;
extern DispatchRec D_001E8F80[];
extern int D_0018A3B0[];
extern void FUN_001f9810();
extern void FUN_001f2820(void);
extern int D_0018C434 NOT_SDA;
extern char D_001940C0[];
extern long D_00151888[3];
extern int D_0015F6FC;
extern short D_0015F534;
extern void FUN_001fb368(void);
extern void FUN_001f39d0(void);
extern int D_0015F564;
extern int D_0018DD40[];
extern int D_0018DC40[];
extern short D_0015F59C;
extern int FUN_001f6200(unsigned char *arg0, int arg1, void *arg2);
extern unsigned char D_001DF3D0[];
extern unsigned char D_001DF770[];
extern unsigned char D_001DFB10[];
extern void FUN_001f62b0(void *, void *, void *, void *, void *, int,
                          unsigned char *);
extern int FUN_001f6250(unsigned char *, int);
extern int func_001F6620(unsigned char *, int);
extern int FUN_001f44b8(int);
extern void FUN_001f7090(void *, void *, void *, void *, int, unsigned char *);
extern void FUN_001fb2d0(void);
extern void FUN_001f2c60(void);
extern void FUN_001f2d98(void);
extern int D_0018E840[];
extern long D_00152178 NOT_SDA;
extern int FUN_001fdca0(void);
extern char D_00199A68[];
extern short D_0015F780;
extern int D_001941CC NOT_SDA;
extern int D_0019A4E8 NOT_SDA;
extern int FUN_001fee38(int);
typedef struct {
    char b[0x13];
} Cfg13;
extern Cfg13 D_0019A540 NOT_SDA;
extern Cfg13 D_001E7DD8 NOT_SDA;
extern int func_00116810(void);
extern void func_001166FC(Cfg13 *, void *);
extern short D_0015F9D0;
extern void FUN_00201128(int, int, int, int, int);
extern void FUN_00201ba8(int);
extern void FUN_00201f88(int);
extern void FUN_00204790(void *);
extern int D_0018CB20 NOT_SDA;
extern int D_001941C8 NOT_SDA;
extern int D_0016100C;
extern int D_001A0468[];
extern void func_00205830(int a, int b);
typedef struct {
    int _pad0[0x9E];
    int use[5];   /* +0x278 */
    int flags[5]; /* +0x28C */
    int sel;      /* +0x2A0 -- index of the active slot, -1 for none */
    int size[5];  /* +0x2A4 */
} PadSlots;
extern PadSlots D_001A01F0_slots __asm__("D_001A01F0");
extern int D_001A01F0[];
extern int *D_001602E0;
extern unsigned char D_0013D49C NOT_SDA;
extern unsigned char D_0013D49D NOT_SDA;
extern unsigned char D_0013D4A5 NOT_SDA;
extern short D_0015FE24;
extern unsigned char D_0013D4AC NOT_SDA;
extern unsigned char D_0013D4AD NOT_SDA;
extern unsigned char D_0013D4AE NOT_SDA;
extern unsigned char D_0013D4AF NOT_SDA;
extern unsigned char D_0013D4B5 NOT_SDA;
extern int D_001A04B4 NOT_SDA;
extern unsigned char D_0013D4C5 NOT_SDA;
extern int D_001414DC NOT_SDA;
extern unsigned char D_0013D4C0 NOT_SDA;
extern unsigned char D_0013D4C1 NOT_SDA;
extern unsigned char D_0013D4C2 NOT_SDA;
extern unsigned char D_0013D4D3 NOT_SDA;
extern unsigned char D_0013D4D4 NOT_SDA;
extern unsigned char D_0013D4D5 NOT_SDA;
extern unsigned char D_0013D4E0;
extern unsigned char D_0013D4DC NOT_SDA;
extern unsigned char D_0013D4DD NOT_SDA;
extern unsigned char D_0013D4DE NOT_SDA;
extern unsigned char D_0013D4DF NOT_SDA;
extern unsigned char D_0013D4E1 NOT_SDA;
extern unsigned char D_0013D4E9 NOT_SDA;
extern unsigned char D_0013D502 NOT_SDA;
extern unsigned char D_0013D503 NOT_SDA;
extern unsigned char D_0013D504 NOT_SDA;
extern unsigned char D_0013D505 NOT_SDA;
extern unsigned char D_0013D50F NOT_SDA;
extern int D_0013D668[];
extern void func_00208810(void);
extern int FUN_001fa860(void *dst, int size, int a, int b);
extern void func_00208030(void *dst);
extern short D_0015EE84;
extern int D_0015EE84_far __asm__("D_0015EE84") NOT_SDA;
extern int D_001A0218[] NOT_SDA;
extern void FUN_00207c28(void *, unsigned char *, int);
extern void func_00207E58(void *, unsigned char *);
extern char D_0013D390[];
extern short D_0015EFB0;
extern int D_0015EFB4;
extern int D_001A05C0[];
extern int D_001A08C0[];
extern int GetDmaPacketSpanBytes(int *p);
extern int FUN_0020ad78(void *dst, int i, int *table);
extern int func_001236F0(void);
extern int DebugPrint();
extern char D_001E8690[];
extern int D_0013D844 NOT_SDA;
extern unsigned char D_0013D4A8 NOT_SDA;
extern int D_0013D9B4 NOT_SDA;
extern unsigned char D_0013D490[];
extern unsigned char D_0013D5CA NOT_SDA;
extern int D_0013D6B8 NOT_SDA;
extern int D_0013DAE4 NOT_SDA;
extern unsigned char D_0013D4E5 NOT_SDA;
extern int D_0013DB24 NOT_SDA;
extern unsigned char D_0013D4F1 NOT_SDA;
extern int D_0013DC34 NOT_SDA;
extern unsigned char D_0013D605 NOT_SDA;
extern int D_0013D5C8 NOT_SDA;
extern unsigned char D_0013D4B0 NOT_SDA;
extern unsigned char D_0013DE55 NOT_SDA;
extern unsigned char D_0013D5DD NOT_SDA;
extern unsigned char D_0013D5E7 NOT_SDA;
extern int D_001B2F40[];
extern void func_001FA460_2(void *, void *) __asm__("FUN_001fa298");
extern void FUN_001f9a68(void *, void *, float);
extern void FUN_001f9a10(void *, void *, void *);
extern void FUN_00210850(void *, int, int *, void *);
extern void FUN_001fa378(void *, void *, void *);
extern void FUN_002106f8(void *, int, void *, void *);
extern void FUN_001f9cf8(void *, void *, void *);
extern int D_001414D0 NOT_SDA;
extern float D_001CAE00[] NOT_SDA;
extern void FUN_0020d510(void *, void *);
extern float FUN_001f9e90(float, float);
extern float func_001F9DC8(float);
extern float func_001F9DE0(float);
extern void func_00118D80(int);
extern void FUN_00211728(int, int);
extern char D_00165600[];
extern int D_0015F718;
extern short D_0015F71C;
extern char D_001B3200[];
extern int rand(void);
extern float FUN_00213308(void);
extern float FUN_002132a8(float, float);
extern void FUN_00214db0(void *, float, float, float);
extern void FUN_001f9bf8(void *, void *, float);
extern void FUN_001fa298(void *);
extern void FUN_00214260(void *, void *);
extern void func_001FA2B8(void *, void *);
extern float FUN_0020c9e0(void);
extern float FUN_00214c48(int, float);
extern unsigned char D_0014BFC0[];
extern unsigned char D_0013E620[];
extern unsigned char D_0013D510[];
extern void FUN_0012ef28(void *);
extern void func_002177F0(int);
extern short D_001517D0[];
extern void FUN_0012eca0(void *);
extern void FUN_0012eea8(void);
extern char D_001E8980[];
extern int snd_StreamSafeCdRead(int, int, int, void *);
extern void RaiseKernelTrap(void);
extern int func_00217628_v(void) __asm__("func_00217628");
extern void sceGsSyncV(int);
extern void func_00217130(void);
extern void FUN_0012eb00(void);
extern void FUN_0012dc80(void);
extern void ReadGlobalTableEntry(void);
extern int FUN_0012eef0(void);
extern void FUN_00215970(short, short, short);
extern void FUN_00215b68(short, short, short);
extern short D_001517F0 NOT_SDA;
extern char D_0013CA40[];
extern int D_001CDAE0 NOT_SDA;
extern void sceDbcInit(void);
extern void scePad2Init(int);
extern int scePad2CreateSocket(void *, void *);
extern void func_00217F68(void *);
extern int D_0015EF90;
extern char D_001D4B90[];
extern char D_001D4BC0[];
extern char D_001D5F70[] NOT_SDA;
extern char D_001D603B[];
extern int D_001A0414;
extern int D_001CFBF4;
extern int D_001CFAD8;
extern void FUN_0020b950(void *);
extern int FUN_0020bc00(void *, void *, void *, int);
extern int D_00141FA0[];
extern char D_001D0A50[];
extern char D_001D0A88[];
extern int D_001A0418 NOT_SDA;
extern void FUN_00225ac0(int);
extern float FUN_001fa580(float, float);
extern char *D_001D5F74 NOT_SDA;
extern void FUN_0020d330(int, int);
extern char D_00187040[];
extern void func_00220128(void *);
extern void *func_00226720_a(int) __asm__("func_00226720");
extern int func_002267C0(int);
extern void FUN_00233980(int, int);
extern void FUN_00205640(void);
extern void FUN_001f4280(int);
extern void FUN_001f4398(void);
extern void func_001F68E8_c(int, int, long, void *, int)
    __asm__("FUN_001f6530");
extern void *func_001FE540_id(int) __asm__("FUN_001fdd10");
extern short D_001602B0;
extern void func_00200E08(int, int, int, int, long, long);
extern int FUN_001ff960(int, int);
extern void FUN_001ffc30(int, int, int, int, int, int);
extern void FUN_001f5450(int, int, int, int, int, int, int, int, long,
                          long);
extern short D_00151880[];
extern long D_001A0448;
extern int FUN_00225c18(int);
extern int FUN_00225cd8(int);
extern int D_0013CC04 NOT_SDA;
extern char D_001D2678[];
extern char *D_001D5F78 NOT_SDA;
extern void FUN_001fd748(int, int, int, int);
extern unsigned char D_001B3E40[] NOT_SDA;
extern void *FUN_0020c4f8(void);
extern void FUN_0020def8(void *);
extern void PackRenderCommandFields(void *, int, int, int, int);
typedef struct {
    int key;
    int flags;
} PadBind;
extern PadBind D_001D6448_t[] __asm__("D_001D6448");
extern int FUN_00225d88(int handle);
extern int D_001D6448[];
extern char D_001D5D58[] NOT_SDA;
extern char *D_001B3580[] NOT_SDA;
extern int D_001D6860[];
extern int D_001D74C0[];
extern int D_001D6760[];
extern char D_00187180_a[] __asm__("D_00187080");
extern char D_00194220[];
extern int D_0013E6BC;
extern void func_00213358(void *, float, float);
extern void func_001F9BD8_a(void *, void *, void *) __asm__("FUN_001f9a10");
extern void func_001F9C30_a(void *, void *, float) __asm__("FUN_001f9a68");
extern void func_001F9BF0_a(void *, void *, void *) __asm__("FUN_001f9a28");
extern int func_001EFE10_a(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern char D_00187080[];
extern void FUN_001f9a28(void *, void *, void *);
extern void FUN_001f9c90(void *, void *, float);
extern void FUN_001efa68(void *, void *, int, int, int);
extern float FUN_001f9b48(int, void *);
extern void FUN_0022c6f8(void *, float, float, float);
extern void FUN_001f9d20(void *, void *, void *);
extern float FUN_001f9b20(void *);
extern float FUN_001f9bb0(float, float, float);
extern float func_001FA058_a(float, float) __asm__("FUN_001f9e90");
extern void FUN_001FA6D0(float);
extern void sceCdSync(int);
extern int FUN_0012df20(void *, int);
extern void FUN_0012ed30(int);
extern void FUN_0012ee08(int);
extern void FUN_0012e1a8(void);

extern char D_0018CB20_c[] __asm__("D_0018CB20");
extern float D_0018CDB0 NOT_SDA;
extern unsigned char D_0015EDB4_m[4] __asm__("D_0015EDB4") MACRO_ADDR;
extern void sceVu0UnitMatrix(float *);
extern void SceVu0RotMatrixX(float *, float *, float);
extern void SceVu0RotMatrixY(float *, float *, float);
extern void sceVu0RotMatrixZ(float *, float *, float);
extern void func_001F9AD8(void *, void *, void *);

/* Movie-camera keyframe step (as FUN_001eaf88): the keyframe's +0x1C
   sets D_0018CDB0, its position and X/Y/Z angles set the camera position
   and matrix rows, and with the D_0015EDB4 flag set the row at +0x220 is
   rebuilt from the other two by func_001F9AD8. Returns the flag byte.
   The flag is read through a 4-byte MACRO_ADDR alias: over -G2, so the
   macro expands to lui/lbu through the destination register. */
unsigned char build_object_rotation_matrix(void) __asm__("FUN_0022de10");

unsigned char build_object_rotation_matrix(void) {
    char *t = D_0018CB20_c;
    char *key = *(char **)(t + 0x54) + *(int *)(t + 0x38) * 32;
    unsigned char flag = key[0xC];
    float *ang = (float *)(key + 0x10);
    char *pos;
    char *cam;
    float m[16];

    D_0018CDB0 = ang[3];
    FUN_001f2d98();
    pos = D_00187080;
    qcopy(pos, key);
    sceVu0UnitMatrix(m);
    SceVu0RotMatrixX(m, m, *(float *)(key + 0x10));
    SceVu0RotMatrixY(m, m, ang[1]);
    sceVu0RotMatrixZ(m, m, ang[2]);
    cam = pos - 0x140;
    *(float *)(cam + 0x350) = -m[8];
    *(float *)(cam + 0x360) = -m[0];
    *(float *)(cam + 0x370) = m[4];
    *(float *)(cam + 0x354) = -m[9];
    *(float *)(cam + 0x364) = -m[1];
    *(float *)(cam + 0x374) = m[5];
    *(float *)(cam + 0x358) = -m[10];
    *(float *)(cam + 0x368) = -m[2];
    *(float *)(cam + 0x378) = m[6];
    if (D_0015EDB4_m[0] != 0) {
        func_001F9AD8(pos + 0x220, pos + 0x230, pos + 0x210);
    }
    return flag;
}

typedef struct {
    float v[4][4];     /* 0x00 */
    int rgba[4];       /* 0x40 */
    char uv[0x20];     /* 0x50 */
    long gs[4];        /* 0x70 */
} Quad_FBE0;

extern char D_001D9B40[];
extern void func_001F7D30(void *, int, int);
extern int D_0015F6E8 MACRO_ADDR;
extern int FUN_001f96f8(int);
extern void FUN_001f9a80(void *, void *, float);
extern int FUN_001fa820(void *, int *, float);
extern float func_00213508(void *, int, float);
typedef struct {
    char pad[0x20];
    int mode;
    short level;
    short set;
} FlareCfg;
extern FlareCfg D_0013E130;
extern float D_001D9B60[][4][4];

/* Draws a light flare quad at arg0's position (+0x00; the matrix is at
   +0xC0) when FUN_001fa820 finds it on screen, with the alpha it
   returns. In D_0015F6E8 mode 6 with flare set 3, the flare fades as
   D_0013E130's level passes FUN_001f96f8(150), and its depth comes from
   func_00213508. The corners are D_001D9B60[set]. */

/* The five nops of padding after func_0022DF40 (they sat after
   `endlabel` in its .s, so the stub carried them); see FUN_001f62b0's
   note in draw.c. */

extern int D_0015F6E4 MACRO_ADDR;
extern int D_0015F6FC_m __asm__("D_0015F6FC") MACRO_ADDR;
extern short D_0015F690; /* SDA, gp -0x7670 */






extern void func_00234C98_l(int, long) __asm__("FUN_00233980");
extern long D_00160680 MACRO_ADDR;
extern char D_001D9B40[];
extern float D_001D9E20[];
extern float D_001D9DE0[][4];
extern short D_001605F0;
extern void func_001F7D30(void *, int, int);
extern int D_0015EE84_m __asm__("D_0015EE84") MACRO_ADDR;

/* Its 0x8000000044 is built as ps2eeas built it (tools/ps2eeas_dli.py). */

/* The 64-bit argument form of FUN_00233980 is needed here (one call
   passes 0x8000000044). Retail's ori 0x8000 / dsll 24 / ori 0x44 for
   that constant is ps2eeas's expansion of the same `dli`, which
   tools/ps2eeas_dli.py reproduces; GNU as builds it differently. */
extern void func_00234C98_l(int, long) __asm__("FUN_00233980");
extern int D_0013E604;
extern long D_00160688 MACRO_ADDR;










extern int D_0013E150;
extern void FUN_001f4248(void);
extern void FUN_0022f288(void);

/* A switch on D_0013E150, whose jump table is linked into the data
   segment at retail's jtbl_001E8C90 (see rac1.ld.sh). */



typedef struct {
    int sector;
    int size;
} WadEntry;
typedef struct {
    char _pad0[0x1938];
    WadEntry movie[12];  /* 0x1938 */
    WadEntry movie2[12]; /* 0x1998 */
} WadToc;
extern WadToc D_00137C80_t __asm__("D_00137C80");
extern int D_0015EE80 MACRO_ADDR;
extern int D_0015EE88 MACRO_ADDR;
extern int D_001941D4;
extern int func_0023B670(int, int, int, int, int);
extern void FUN_00120558(int, int);
extern void sceGsSyncVCallback(void *);
extern void FUN_0012f1c8(void);
extern void FUN_001f4a58(int);

/* Plays movie id, taking its {sector, size} from the WAD table of
   contents (from the second table when D_0015EE80 is set). */

extern int *D_00161000 MACRO_ADDR;
extern int D_0013E600[];
extern char D_00160950[];

typedef union {
    struct {
        float u;
        float v;
    } f;
    long bits;
} SpaceUvPair;

/* Append a textured four-corner GIF packet. Screen coordinates use 12.4
   fixed point relative to the viewport origin; UV pairs stay as floats. */

extern __typeof__(build_object_rotation_matrix) func_0022DE10 __attribute__((alias("FUN_0022de10")));
