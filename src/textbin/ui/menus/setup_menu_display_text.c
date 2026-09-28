/* Ported from rac1-decomp, the PAL decompilation (src/game/pause.c, func_0021FF80). */
typedef unsigned int u128 __attribute__((mode(TI), aligned(16)));
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
static __inline__ void qcopy(void *dst, void *src) {
    *(u128 *)dst = *(u128 *)src;
}

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
 * pause.cpp in the original source; text 0x219C08-0x228A58.
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
extern int D_0018CC20 NOT_SDA;
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
extern PadSlots D_001A01F0_slots __asm__("D_001A00F0");
extern int D_001A00F0[];
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
/* MACRO_ADDR: lui/lw where retail has them, $gp-relative in a delay slot
   (func_0021F7D0, FUN_00221968). */
extern int D_0015EE84 MACRO_ADDR;
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
extern unsigned char D_0014BEC0[];
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
extern void FUN_001f6530(void *a, void *b, void *c, void *d, void *e);
extern void *func_001FDD10(void);
extern int func_00205790(void);
extern void FUN_0020abb0(char *out);
extern void func_00217588(void);



extern void FUN_0012e3e8(int);
extern void func_00216EF0(int);
extern int D_0018C42C;
extern int D_0015F754 MACRO_ADDR;
extern int D_00141760;
extern unsigned char D_0014171B NOT_SDA;
extern int D_0015EFA0 MACRO_ADDR;
extern int D_0015EF20 MACRO_ADDR;
extern char D_001CE938[];
extern char D_001CEAC8[];
extern char D_001CEB18[];
extern int D_001A0414;
extern int D_001D0718;
extern int func_0020C7A0_i(void) __asm__("FUN_0020b950");
extern int func_0012DDC0_i(void) __asm__("FUN_0012dc80");
extern void func_00228160(void);
extern char D_001D5F70[] NOT_SDA;
extern int D_0015F6E8 MACRO_ADDR;

/* PauseAllSounds: pause entry. Unless D_0018C42C is set (then only
   D_0015F754 = 1), it computes the pause object's flags (0x134, 0x138,
   0xD8 "loaded", 0xDC "arg0 is 0x23"), points the D_001CE938/D_001CEAC8
   records' +0x38/+0x3C at the loaded or unloaded variants, resets the
   object for arg0 (InitializeTransferCommand's body) and D_001A0414, then calls
   FUN_0020b950 and func_00228160. Each group reads D_001D5F70 through
   its own `char *` local (retail's saved %hi); FUN_0012dc80 and
   FUN_0020b950 return values, which moves the next temporary to $v1. */






extern int D_0015EF78 MACRO_ADDR;
extern int func_002267C0(int);
extern int D_001D6120[];

typedef struct { char pad[0x44]; char *items[14]; } ObjList;

/* Pause teardown: calls each object's +0xC handler, then refreshes the
   14 D_001D6120 handles. Retail's register copies come from gcse, so the
   base is re-read inside each block (and inside the loop body). */





/*
 * Pad handler for the pause page arg0 (g->x4->x40): 1 on the 0xD00
 * buttons while g->x124 is 0; on 0x10, switch to the page's +0x38
 * target, or return -1 when it has none and g->x124 is 0. Written in
 * its family's shape (func_0021F7D0, func_00222DB0): early returns and
 * one `char *` local per block that reads a global.
 */

extern int D_001D6094;
extern void func_0022DA68(int, int, int);

/* Pause list handler (func_0021ACD8's family: one `char *` pad local per
   block). The cursor at arg0+0x3C moves on 0x1000/0x4000 within
   arg0+0x40, with a func_0022DA68 notify on change; then the scroll
   (arg0+0x60) is clamped around the cursor and the bar position at
   arg0+0x5C derived from it, or pinned to 0x52. The re-reads of 0x3C and
   0x60 are how retail's reloads come out; the page size q - 2 is formed
   after the clamp, and each arm stores the bar position itself. */

extern short D_001602B0;              /* SDA, gp -0x6A50 */
extern void func_00200E08(int, int, int, int, long, long);
extern int FUN_001ff960(int, int);
extern void FUN_001ffc30(int, int, int, int, int, int);
extern void FUN_001ffe18(int, int, int, int, int, int);
extern void func_00200080(int, int, int, int, int, int);
extern int D_0015F538 MACRO_ADDR;
extern int SubtractIntegerWithClamp(int); /* abs */
extern void func_001F4280(int);
extern void func_001F4398(void);

/* Vertical picture list: each entry (10 bytes at +0x48, count at +0x40)
   is a 0x200-square image at x, stepping 0x252 down from the scroll
   position at +0x5C; the selected one (+0x3C) gets a pulsing frame. Up
   and down arrows show when the list runs off the top or the bottom. */

extern int D_0015EF90;
extern char D_001D4B90[];
extern char D_001D4BC0[];

extern int D_0015EF90_m __asm__("D_0015EF90") MACRO_ADDR;

/* D_0015EF90 is read through a MACRO_ADDR alias: retail's one-register
   load. */

/* The two initializer tables of the local arrays below. The first is
   copied as a char block (retail's ldl/ldr for all 32 bytes), the second
   as an int block (ldl/ldr, then lw/sw for the last word). */
typedef struct { char b[0x20]; } Blk32;
typedef struct { int w[7]; } Blk28;
extern Blk32 D_001E8A58;
extern Blk28 D_001E8A78;
/* MACRO_ADDR: retail builds each address in one register (la). */
extern unsigned char D_0015EEC0[] MACRO_ADDR;
extern unsigned char D_0015EEB0[] MACRO_ADDR;
typedef struct {
    int val;
    void *addr;
    int c1;
    int c2;
    int zero;
} Entry14;
extern Entry14 D_001D3E90[];

/* Builds the D_001D3E90 list: for each id in {1, 3, 0, 7, 4, 6, 2} (up
   to the -1) whose D_0015EEC0 flag is set, an entry with its text id
   (0x501A...), &D_0015EEB0[id] and the constants 0x4F5A/0x4F5B; a zero
   val ends the list. A plain indexed for loop: strength reduction gives
   retail's pointer walk, its end test against ids + 12, and the id read
   twice per iteration (exit test and body). */










/* SDA, gp -0x6A4C: declared 2 bytes so the assembler reaches it through
   $gp, and read as the int it is. */
extern short D_001602B4;
extern int func_001FA6E0(int, int, float);

/* Fade colour for the pause menus: -1 picks the default colours
   0x80FFA888 / 0x8020FFFF and a negative start becomes 0. While the
   D_001602B4 timer (FUN_001f96f8) is still below start the blend factor
   is 1; after that it is 1 - (elapsed - start) / total, and the two
   colours are blended by func_001FA6E0. The parameters are updated in
   place; the timer is read again for each value. */

/* Pause page link walk: with arg1 clear, follow arg0's +0x4C chain for
   as long as the current page is skippable (bit 8 of its +0x30 flags
   while D_001D5F70+0x134 is set, or bit 4 while +0x138 is set), and if
   it moved at all, make the page it stopped on the current one's +0x80.
   Each block reads D_001D5F70 through its own `char *` local: the loop's
   own copy is why GCSE leaves its +0x138 load in the loop (only +0x134,
   read first in the body, is hoisted by loop.c), and the last block's
   copy is retail's kept %hi. */





/* D_001D5F70 + 0xCB. Retail addresses this one byte BOTH ways -- through
   its own %hi/%lo here and as 0xCB($16) off the D_001D5F70 base a few
   instructions later -- so it really is two names on one address in the
   original source, and splat has the second as its own linker symbol. */
extern char D_001D603B[];
extern short D_001517D0[];

/*
 * Pause/menu teardown. When the D_001517D0+8 state word is clear, drop
 * the active slot: clear the 0xCB flag byte, toggle 0x1000 in that
 * slot's flag word and mark no slot active. When it is SET instead and
 * the 0xCB byte is still set, run func_00217588 first, then clear the
 * byte and invalidate the slot outright (flags = -1, not a toggle).
 *
 * The two tests read D_001517D0[4] twice rather than being one if/else,
 * and that IS the source: the indirect store into the slot table between
 * them kills gcc 2.95's memory, so retail reloads the halfword and
 * re-materialises the base. Written as if/else it collapses to a single
 * test.
 *
 * D_001A00F0 is the struct from func_00205790 above; `sel` at +0x2A0
 * sits between the flag array and the size array, which is why flags is
 * 5 entries and not 6.
 *
 * `char *g = D_001D5F70;` is load-bearing: writing the two accesses as
 * D_001D5F70[0xCB] lets gcc fold 0xCB into the symbol addend, giving
 * %hi/%lo(D_001D5F70+203) and a base register already at +0xCB. Retail
 * keeps the UNOFFSET base in $16 and puts 0xCB in both displacements.
 * Same instruction count, but it also changes what the branch delay
 * slot gets filled with, so it is a byte difference, not just cosmetic.
 */



extern int D_001A0414;
extern int D_001CFBF4;
extern int D_001CFAD8;
extern void FUN_0020b950(void *);
extern int FUN_0020bc00(void *, void *, void *, int);




extern char *D_001D5F74 NOT_SDA;
extern int D_0015EFA4 MACRO_ADDR;
extern void func_0022DA68(int, int, int);
typedef struct {
    char pad[0xA8];
    unsigned short count;  /* 0xA8 */
    unsigned short best;   /* 0xAA */
    unsigned int mask;     /* 0xAC */
} Stats_0021D7A0;
extern Stats_0021D7A0 D_00141948;
typedef struct {
    char pad[0x14];
    int id;      /* 0x14 */
    char pad18[0x18];
    int list[8]; /* 0x30 */
    int idx;     /* 0x50 */
} SelList_0021D7A0;

/* Quick-select slot handler: 8 and 4 on the pad step the slot index at
   +0x50 forward and back (mod 8), with a func_0022DA68 notify when it
   moved. With 0x40, if the highlighted item (the current page's entry
   id) is owned, it is recorded: the use count at D_00141948+0xA8 (to
   0xFFFF), the best time +0xAA and the level mask +0xAC; then the item
   leaves any slot it held and is put in the current slot, and the index
   advances. The three conditions are one `&&` (one shared exit) and the
   time is compared call-first, as retail evaluates them. */

extern int D_00141FA0[];

typedef struct {
    char pad[0x30];
    int list[8]; /* 0x30 */
    int idx;     /* 0x50 */
} SelList_0021D9C8;

/* Reload the 8-entry list at +0x30 from D_00141FA0 (the reverse of
   func_0021DA60 below), then leave idx at +0x50 on the first empty entry,
   wrapped to 0..7. Indexing arg0->list directly in both loops (no local
   list pointer) is what gives retail's hoisted base; the scan is a do-while
   guarded by list[0] (a plain while/for rotated differently). */

extern int D_00141FA0[];


extern char D_001D0A50[];
extern char D_001D0A88[];


extern int D_001A0418 NOT_SDA;


extern void FUN_00225ac0(int);


extern int D_0015EEF0 MACRO_ADDR;
extern int D_0013E6A0;






typedef struct {
    unsigned short v;
    short pad;
} Half4;
typedef struct {
    unsigned short a;
    short b;
    char pad[8];
} Rec0C;
extern int D_0015EF30 MACRO_ADDR;
extern Rec0C D_001CFFC0[];
extern unsigned char D_00141F08[];
extern Half4 D_00199812[];



typedef struct {
    int key;
    int flags;
} PadBind;

/* Aliased rather than renamed: FUN_00225d88 further down still walks
   the same table as a flat int array. */
extern PadBind D_001D6448_t[] __asm__("D_001D6448");

extern int D_001D6078;
extern int D_00137C80[];
extern int func_00217628_3(int, int, int) __asm__("func_00217628");


/* D_0015F780 is SDA elsewhere; pause.cpp stores it through $at. */
extern int D_0015F780_m __asm__("D_0015F780") MACRO_ADDR;
extern int D_001997FC;


extern int D_0015EE88 MACRO_ADDR;
extern char D_001997D0[];
extern void FUN_001f9838(void *, void *, int);

/* The slot count at D_001997D0+0x2C, read through its own base pointer:
   retail's loop keeps a separate copy of the D_001997D0 address. */
static inline int pauseSlotCount(void) {
    char *b = D_001997D0;
    return *(int *)(b + 0x2C);
}

/* Pause sub-state machine at arg0+0x50 (func_00221688's family). State 0
   runs func_00217628 and moves to 1 or 3; state 1, once D_001517D0[4]
   clears, pulls the controller's record out of the table at
   D_001D5F70+0x108 (FUN_001f9838 moves it to the table start), swaps it
   in as D_0015F780 and the D_001997D0+0x2C count (old values kept in
   arg0+0x54/+0x38), rebases the entries' first words and moves to 2.
   Cases 2 and 3 are empty; they make gcc's case tree test 1 first, as
   retail does. The reloaded table gets its own variable so that the
   first one stays block-local, and the rebase delta is computed in the
   loop so that loop.c hoists it. */





extern unsigned char D_001414F4 NOT_SDA;

/* A short-returning inline keeps retail's branch: the select happens in
   HImode, which has no movcc pattern, so jump.c cannot turn it into the
   xori/movz a promoted `short` local gets. */
static inline short pauseFlagState(void) {
    if (D_001414F4 != 1) {
        return 3;
    }
    return 0;
}

/* The select sits in a `static inline short` helper so that it stays a
   branch: jump.c turns an if into movz/movn only when its arm sets a
   full register, and the inline's short return value is a subreg. */

extern int D_001D53A0[];
extern void func_0020D330(int, int);

/* Tears down the 24 handles at arg0+0x44, skipping the ones in use.
   The switch's table is jtbl_001E8AD0; `off` before `slots` and a second
   slots2/off2 pair give retail's copies. */




extern char D_001864D0[];
extern char D_00186F40[];
extern void *func_00226720_a(int) __asm__("func_00226720");
extern void FUN_0021e1f8(char *);

/* Keeps the item preview moby in step with the highlighted entry: drop it
   when the entry's class (D_001864D0 record +0x3A) is -1, spawn it in
   front of the camera focus when there is none yet, or respawn it with
   the old one's position and orientation when the class changed. */

extern char *D_001D5F74 NOT_SDA;
extern void func_0020D330(int, int);
extern void func_001F4280(int);
extern void func_001F4398(void);
extern void *func_001FE540_id(int) __asm__("func_001FDD10");
extern void func_00227A30(void *, char *);
extern void FUN_001f7580(void *, long, void *, int);

/* Draw/teardown callback (func_0021F610's family). The entry address is
   written inline in the index; a `char *e` local leaves an addu swap. */

extern float FUN_001fa580(float, float);




extern char *D_001D5F74 NOT_SDA;
extern void func_0020D330(int, int);

/* The entry address is written inline in the index, as in
   FUN_0021e110; a `char *e` local swapped the addu. */



extern int D_0013D48C;
extern int D_0015F6E4 MACRO_ADDR;
extern int D_0015F6FC_m __asm__("D_0015F6FC") MACRO_ADDR;
extern int D_0015F690 MACRO_ADDR;

/* Pad handler (func_0021ACD8's family). A separate `char *` local for
   each block that reads the pad word gives retail's pattern: the %hi
   kept in a register and the %lo rebuilt before each use. */

extern void memset(void *, int, int); /* memset */
extern void func_00234C98_l(int, long) __asm__("FUN_00233980");
extern void FUN_001f6af0(int, int, long, void *, int);
extern void func_001F68E8_c(int, int, long, void *, int) __asm__("FUN_001f6530");

/* DrawQuitGameMenu: a text box (zeroed, then sized from arg0+0x20/0x24)
   with text 0x4F6D, text 0x4F3F centred 0x40 above the bottom, and the
   two answers 0x5250/0x5254 at 0x28 and 0x14 above it, left-aligned so
   that the wider of the two (FUN_001f6250) is centred. The three y
   positions are computed before the calls (retail keeps them in saved
   registers); the box's y halfword is stored last, after the other
   fields in order, which gives retail's store schedule. */

extern char D_00186F40[];
extern void func_00220128(void *);
extern void *func_00226720_a(int) __asm__("func_00226720");

/* Attach marker object 0x46E to arg0, parked just above the camera
   focus in D_00186F40's 0x140 block, and point it back at its owner
   through the node at +0x78. Always returns 0. */

extern int func_002267C0(int);




extern char D_00186F40[];
/* 1.3f, in small data (gp -0x695C). The const float view is what the
   code reads (read-only, so its load is scheduled above the stores); the
   2-byte view, referenced last, makes the assembler use $gp. */
extern const float D_001603A4_f __asm__("D_001602A4") __attribute__((sda));
extern short D_001603A4_s __asm__("D_001602A4");
extern char D_001602A8[];
extern int D_001E0888[];
extern int D_0013E500[];
extern int sprintf(char *, const char *, ...);
extern void func_001F6CF8_c(int, int, long, char *, int) __asm__("func_001F6940");

/* DrawGBsShipMenu (maybe): parks the marker object at arg0+0x44 by the
   camera focus in D_00186F40, draws it, then prints "%s %d %s %d" (texts
   0x4F4F and 0x4F53, the number of D_0014BEC0 flags set for the current
   map, and the map's D_001E0888 total) twice, as shadow and text. The
   flag count reads the map index inside its loop: PRE then computes
   D_001A00F0's %hi once, straight into the register that is kept for the
   second read, as retail has it (a pointer set before the loop leaves a
   copy). */
int setup_menu_display_text(char *arg0) __asm__("FUN_0021ef78");

int setup_menu_display_text(char *arg0) {
    char buf[0x100];
    char *t = D_00186F40;
    char *o = *(char **)(arg0 + 0x44);
    int count;

    *(float *)(o + 0x10) = *(float *)(t + 0x140) + 8.0f;
    *(float *)(o + 0x14) = *(float *)(t + 0x144) + D_001603A4_f;
    *(float *)(o + 0x18) = *(float *)(t + 0x148) - 0.1f;
    if (*(int *)(arg0 + 0x44) != 0) {
        func_0020D330(*(int *)(arg0 + 0x44), 1);
    }
    func_001F4280(0);
    count = 0;
    {
        int k;
        for (k = 0; k < 4; k++) {
            if (D_0014BEC0[D_001A00F0[0x89] * 4 + k] != 0) {
                count = count + 1;
            }
        }
    }
    sprintf(buf, D_001602A8, func_001FE540_id(0x4F4F), count,
                  func_001FE540_id(0x4F53), D_001E0888[D_001A00F0[0x89]]);
    func_001F6CF8_c(*(int *)(arg0 + 0x20) - 0x10, (D_0013E500[1] >> 1) - 8,
                    0x80000000L, buf, -1);
    func_001F6CF8_c(*(int *)(arg0 + 0x20) - 0x11, (D_0013E500[1] >> 1) - 9,
                    0x80FFA888L, buf, -1);
    func_001F4398();
    if (0) {
        /* no code: registers the 2-byte view last (see above) */
        (void)D_001603A4_s;
    }
    return 8;
}

extern float FUN_001fa580(float, float);


/* FUN_00233980's second parameter is 64-bit (as in draw.c and
   mobyfunc.c); the (int, int) declaration below this point is kept for
   the functions matched against it. */
extern void func_00234C98_l(int, long) __asm__("FUN_00233980");
extern int sprintf(char *, const char *, ...); /* sprintf_pal */
extern int D_0013D530[];
extern char D_001E02B0[];
extern char D_001603A0[]; /* "%d" */
extern char D_001603B8[]; /* "%d/" */
extern char D_001603C0[]; /* "%d,%03d/" */
extern char D_001603D0[]; /* "%d,%03d" */

/* Draws the current item's count banner, gated like FUN_0021e110: the
   item's text id 0x4F52 when its D_001E02B0 record has no counter,
   otherwise "have/total" with thousands separators. The second sprintf_pal
   appends at text + the first one's length; forming that pointer in each
   arm (`q = text + sprintf_pal(...)`) lets gcc merge the two into one addu at
   the join, as retail has it. */

extern void FUN_00233980(int, int);
extern void FUN_00205640(void);


extern void func_001F61F8(void);
extern void func_001F61E8(void);
extern void func_001F6968_c(int, int, long, void *, int) __asm__("FUN_001f65b0");

/* DrawMissionsMenu: the menu's text lines (0x4EEE, 0x4EEF only when
   D_0015EE84 is set, then 0x4EFA, 0x4EFB, 0x4EFC, 0x4EE0) in one column,
   left-aligned so the widest (func_001F6620) is centred in arg0+0x20 but
   at least 2 in, and spaced arg0+0x24 / (6 or 7 lines) apart starting a
   line minus 6 down. */

extern void func_001F4280(int);
extern void func_001F4398(void);
/* FUN_001f6530 is defined above with pointer parameters; this site
   passes a packed 64-bit colour in $a2, and func_001FDD10 above is
   declared (void) while retail's caller here passes an id in $a0 --
   reach both through aliases rather than redeclaring them. */
extern void func_001F68E8_c(int, int, long, void *, int)
    __asm__("FUN_001f6530");
extern void *func_001FE540_id(int) __asm__("func_001FDD10");

/* Sibling of FUN_0021f330 above: the same two FUN_00233980 setup
   calls, then two banner draws. 0x80FFA888 is spelled `long` (64-bit)
   so it builds via ori/dsll/ori rather than a sign-extending lui. */



extern short D_001602B0;              /* SDA, gp -0x6A50 */
extern void func_00200E08(int, int, int, int, long, long);
extern int FUN_001ff960(int, int);
extern void FUN_001ffc30(int, int, int, int, int, int);




extern void FUN_001f5450(int, int, int, int, int, int, int, int, long,
                          long);
extern short D_00151880[];
extern long D_001A0448;

/* Eight register arguments ($a0-$a3, $t0-$t3) then two 64-bit stack
   slots -- both written with `sd`, so they are `long`, not `long long`
   (which would be 128-bit here). */

extern int FUN_00225c18(int);

/* Sibling of FUN_0021fd78 below: the same slot set (0x44/0x48/0x4C/
   0x50/0x54/0x5C) on the same object, seeded here instead of torn down.
   The tail stores go 5C, 50, 54. */

extern int FUN_00225cd8(int);






/* The per-controller request slots in D_00137C80 (declared above as a
   flat int array). */
typedef struct { int a, b; } Pair8;
typedef struct {
    char pad[0x2C8];
    Pair8 req0[6]; /* 0x2C8 */
    Pair8 req2[6]; /* 0x2F8 */
} Tbl137C80;
extern Tbl137C80 D_00137C80_t __asm__("D_00137C80");
extern int D_0015EE88 MACRO_ADDR;

/* Pause sub-state machine at arg0+0x44. States 0 and 2 wait for their
   request (arg0+0x48 / +0x4C) and for D_001517D0[4] to clear, then run
   func_00217628 with the controller's slot and advance (or go to -1 on
   failure); states 1 and 3 advance once D_001517D0[4] clears. Always
   returns 0. A switch with its cases in source order 0-3 gives retail's
   case tree and block order; reading the slot's two words as fields of a
   struct gives two address adds (one becomes retail's copy). */

/* Returns the packed texture handle the draw call takes as its last
   64-bit argument, so it is `long`: retail stores $v0 straight to the
   stack slot with `sd`, without sign-extending it. */
extern long FUN_00204cf0(int);

/* Sibling of FUN_0021fc68 above: same 0x44 guard and the same
   FUN_001f5450 draw, twice, with the colour held in one local because
   retail keeps it in a callee-saved register across both calls. */

extern char D_001A01F0_c[] __asm__("D_001A00F0");
extern char D_00151880_c[] __asm__("D_00151880");
extern int D_0015F538 MACRO_ADDR;
extern int D_001DE2E8[];
extern void FUN_00200468(long, int, int, int, int, int, int, int, int, int);

/* Draws the three overlay layers (textures at D_001A00F0 +0x258/0x260/
   0x268) over the whole screen (D_00151880's +0x160/+0x162 extent).
   States 6, 13 and 17 blink the upper two layers on D_0015F538; the
   others inset them by D_001DE2E8[state] and scroll the bottom one.
   Each branch reaches the two globals through its own block-scoped
   pointers, which gives retail's registers. */

extern float FUN_001fa610(float, int);
extern int SubtractIntegerWithClamp(int); /* abs */

extern char D_001864D0[];
extern int D_0015F538 MACRO_ADDR;
extern char D_001603D8[];
extern char D_001603E0[];
extern short D_001602B0; /* SDA, gp -0x6A50 */

/* The icon slots are a real member: written as pointer arithmetic, gcc
   strength-reduces the index into a 7th saved register (retail uses 6). */
typedef struct {
    char pad[0x30];
    int slots[8];
} PauseIcons;

/* Draws the pause menu's ring of 8 icons and the quick-select overlay.
   arg0 is the pause-state object: unk20/unk24 give the viewport
   width/height (used for the ring's center and radius), unk50 the
   selected slot (drawn with a pulsing highlight box), and slots[i]
   indexes an icon-info table (D_001864D0, 0x4C bytes/entry) and a byte
   flags table (D_0013E620) when nonzero. */

extern void func_0022DA68(int, int, int);
extern void func_001FBC80(int, void *, int);
extern int FUN_001f96f8(int);
extern void FUN_001f4a58(int);
extern float D_0015F53C MACRO_ADDR;
extern int D_0016044C_i __asm__("D_0016044C") MACRO_ADDR;
typedef struct {
    int w0;              /* 0x00 */
    unsigned char *buf;  /* 0x04 */
    int e[2];            /* 0x08 */
    int flags;           /* 0x10 */
} Row14;

/* Pad handler for a row list of toggles (func_002222F8's sibling, one
   `char *` pad local per block). While the timer at arg0+0x3C runs it
   counts down and sets D_0015F53C to min(t, 4) / 4. Otherwise, for the
   active item: 0xD00/0x10 leave, 0x1000/0x4000 move the cursor with a
   notify on change, and 0x40 flips the current row's byte, or for a row
   with flag 1 starts the timer and flips D_0016044C (func_001FBC80 once
   it is set). The row is indexed directly at each use (index-first addu)
   and the cursor compare is written new != old, as retail's beq has it. */

extern void func_0022DA68(int, int, int);
typedef struct {
    int w0;              /* 0x00 */
    unsigned char *buf;  /* 0x04 */
    int e[4];            /* 0x08 */
} Row18;

/* Pad handler for a row list (func_0021ACD8's gate, one `char *` pad
   local per block): the cursor at arg0+0x38 moves down on 0x1000 and up
   on 0x4000 while the next row is used, with a func_0022DA68 notify on
   change; the current row's used entries (up to 4) are counted, and 0x40
   cycles the row's byte selector through them. The row is indexed with a
   fresh read of the cursor after the notify (reorg skips that load on the
   no-notify path, where the compare already left it in $v0), and the
   count walks an `int *e = rec->e` set before its first test, which puts
   `rec + 8` in the test's delay slot as retail does. */



typedef struct {
    unsigned short a;   /* +0 */
    short b;            /* +2 */
    short c;            /* +4 */
    short id;           /* +6 */
    short idx;          /* +8 */
} Item0A;
extern Item0A D_001CF4A0[];
extern Item0A D_001D6470[];
extern int D_001D6508[];
extern char D_001864D0[];
extern char D_001D1080[];

/* Builds the pause-menu item list from the 15 entries of D_001CF4A0
   that D_0013D5C8 enables. The flag needs its own `f = b != 0`:
   `(b != 0) << 2` folds into a branch. */

extern Item0A D_001CEFA0[];
extern Item0A D_001CEFE0[];
extern Item0A D_001CF000[];
extern Item0A D_001CF020[];
extern Item0A D_001D6548[];
extern int D_001D65D8[];
extern int D_001D6610[];
extern char D_001864D0[];
extern char D_001D1408[];

/* FUN_002215f8's sibling over four source tables. */


typedef struct {
    unsigned short a;   /* +0 */
    short b;            /* +2 */
    int c;              /* +4 */
    unsigned short d;   /* +8 */
    short e;            /* +A */
} Out0C;
typedef struct {
    unsigned short a;   /* +0 */
    short pad2;
    unsigned short b;   /* +4 */
    short pad6;
    int pad8;
} Src0C;
extern int D_0013D618[];
extern Src0C D_001DE0C0[];
extern Out0C D_001D6648[];
extern short D_001602E0_s __asm__("D_001602E0");
extern int *D_001602E0_m __asm__("D_001602E0") MACRO_ADDR;

typedef struct {
    char pad00[0x30];
    int flags;          /* +0x30 */
    char pad34[0xC];
    int sel;            /* +0x40 */
} MenuObj40;

/* Menu builder: the item list from D_0013D618, then the selection. The
   scalar MACRO_ADDR view of D_001602E0 lets sched2 hoist its load, and
   the dead reference to the 2-byte view keeps `.extern D_001602E0, 2`
   last, so the assembler still uses $gp. A later 4-byte reference in
   this file would undo that. */

extern int D_0013CC04 NOT_SDA;
extern char D_001D2678[];
extern char *D_001D5F78 NOT_SDA;


extern void FUN_001fd748(int, int, int, int);


extern void func_0022DA68(int, int, int);

/* Pause sub-menu input on the pad word D_0013CC04. One if/else-if chain
   falling to a single `return 0`; early returns in the 0x10 arm give a
   movn instead. */

extern int FUN_00225c18(int);
extern char *D_001D5F74 NOT_SDA;


extern int FUN_00225cd8(int);


/*
 * Near-miss, same size (4 words differ, allocator only): in the default
 * arm retail keeps the D_001D5F70 base in $v1 (the register that held
 * D_0015EFB0) with the loaded pointer in $a0 and the value in $a1; we
 * get base $a1, pointer $a0, value $v1. Spellings tried (asm-differ
 * score, lower is better; all 0x94 bytes):
 *   this one (if/!=, base local per arm)                       25
 *   base, pointer and value as separate locals in the arm      45
 *   la-macro alias (MACRO_ADDR) for the base in the arm        235
 *   switch with default first and break                        2430
 * D_0013CBE4 is NOT a macro access: retail splits its lui into the
 * second beq's delay slot and branches past it, which only the split
 * form can do (MACRO_ADDR there trips check_macro_slots).
 */
/* D_0015EFB0 is SDA elsewhere; pause.cpp reaches it through the
   assembler macro. */
extern int D_0015EFB0_m __asm__("D_0015EFB0") MACRO_ADDR;
extern int D_0013CBE4;

/* variation: separate "g" local per arm (shadowed), not shared across
   the whole function, to see if that lets the allocator pick the
   per-block register retail uses instead of one merged pseudo. */
extern int D_0015EFB0_m __asm__("D_0015EFB0") MACRO_ADDR;
extern int D_0013CBE4;
extern char D_001D5F70[] NOT_SDA;


typedef struct {
    short s[12];
} TextBox;

extern char D_001603E8[];
extern int D_001D6044;
extern void func_00234C98_l(int, long) __asm__("FUN_00233980");
extern void func_001F4280(int);
extern void func_001F4398(void);
extern void *func_001FE540_id(int) __asm__("func_001FDD10");
extern void FUN_001f75f0(TextBox *, long, char *, int);

/* Draws the two-line prompt box (text 0x4FB3 for D_001D6044 in 0..2,
   0x4FB5 for 3, else D_001603E8) sized from arg0's +0x20/+0x24. The box
   is an aggregate initializer: this compiler clears it with a memset
   libcall, fills a temporary and copies that into the local with
   ldl/ldr/sdl/sdr pairs, exactly retail's sequence. */

extern int D_001D48A8[];

/* `D_001D48A8[(unsigned)D_0015EE84 % 19]`; the older near-miss
   predated MACRO_ADDR. */

extern void func_0022DA68(int, int, int);

/* Pause sub-menu with a 30-entry wrapping cursor at arg0+0x40, in
   func_0021F7D0's shape. */

extern void func_0022DA68(int, int, int);

/* func_00222DB0's twin with a 12-entry cursor at arg0+0x54. */

typedef struct { char c[2]; } Glyph2;
extern char D_001603F0[];
extern char D_001603F8[];
extern int FUN_001ffa10(int);
/* x, y first: arguments are evaluated in order, so the y conversion runs
   before the nested texture calls and is kept in $f20 across them, as in
   retail (and in FUN_00205640, the other caller). */
extern void func_00200E38_f(float, float, int, int, int, float, float, float)
    __asm__("func_00200600");

/* Draws the glyph "\x10" (arg0->0x38 set) or "\x11" and a gauge sprite
   below it, rotated by pi in the second case. The glyph string is a
   2-byte char struct copied onto the stack (retail's lb/lb/sb/sb). */

/* D_001DE0C0 is declared above as Src0C (halfword view); these rows are
   read here as two word-sized icon ids. */
typedef struct { int a; int b; int c; } IconRow;
extern IconRow D_001DE0C0_i[] __asm__("D_001DE0C0");
extern void FUN_001f6af0(int, int, long, void *, int);

/* Draws the current page's icons: the page record (stride 0xC) gives a
   row of D_001DE0C0, or -1 for the single icon 0x5019 centred in the
   box; otherwise the row's two icons at 1/3 and 2/3 of the height. The
   row is read before func_001F4280, and each call is written with its
   arguments inline: gcc evaluates them in order, so x and y are computed
   before the nested func_001FDD10 call. */

extern int D_0015EFB4_m __asm__("D_0015EFB4") MACRO_ADDR;
extern int D_0015EFA0 MACRO_ADDR;
extern int D_0015F6CC MACRO_ADDR;
extern char D_001D5240[];
extern void func_001FBC80(int, void *, int);
extern unsigned char D_0014C008[];
extern unsigned char D_0015EEB0[] MACRO_ADDR;
extern unsigned char D_0015EEC0[] MACRO_ADDR;
extern void FUN_001f9838(void *, void *, int);
extern void func_00209CE8(int);
extern void FUN_0020b178(int, int);
extern int FUN_001f96f8(int);
extern void FUN_001f4a58(int);
extern unsigned char D_0013F450[];
extern void FUN_00226f50(void);

/* Pad handler for the restart option (pad word D_0013CBE4): 0x20 opens
   the confirm popup; 0x40 restarts: the level reload (func_00209CE8)
   runs with the D_0013D510+0x1D byte, the 4 bytes at D_0014C008 and the
   D_0015EEB0/D_0015EEC0 tables saved and restored around it, then the
   pause state is reset. The flags are MACRO_ADDR (retail's one-register
   loads, $at stores and $gp-relative stores in delay slots); globals
   used as bases get a `char *` local per block, so retail keeps only the
   D_0013D510 %hi across the calls. */


extern int D_001D2E74;
extern char D_001D5418[];
extern char D_001D54C8[];
extern char D_001D5588[];
extern char D_001D5618[];
extern char D_001D5630[];

/* A timed text sequence (credits-style): arg0+0x50 is the step, +0x40
   a countdown (FUN_001f96f8-scaled), +0x3C a scroll that advances 10 a
   frame (20 while all four shoulder buttons are held, which also halves
   the countdown), +0x34 the current text (a D_001D5xxx page or a text
   id) with D_001D2E74 its title id. Case bodies are in retail's layout
   order (4 before 3), and the shared "wait, then next step" tail is
   written out in each case: cross-jumping merges it, leaving each
   case's own argument load in its branch's delay slot. */

/* A text box on the menu's own geometry (func_00227A30's box, then
   arg0+0x18..0x24: top y + 4, bottom y + h - 4, x, x + w, centre).
   Per state (arg0+0x50): the list states draw each text id of the -1
   terminated list at arg0+0x34 down from a start that the line count
   (arg0+0x3C >> 4) centres, taking each height from box[7] as the text
   call leaves it, and flag arg0+0x54 when the list ends 0x18 above the
   bottom; states 4, 11, 15 and 19 draw the single text id at +0x34. The
   line count's low nibble goes through `& 0xF` (retail's lbu + dsrl) and
   the start subtracts its own variable, which keeps retail's order. */

extern void memset(void *, int, int); /* memset */
typedef struct {
    short f0, f2, f4, f6;
    short x, y;
    short fC;
    unsigned short fE;
    short f10, f12, f14, f16;
} Box18;

/* ObtainAllGoldWeaponsMenu: two icon + text rows (texts 0x5187, 0x5188)
   in a box sized from arg0+0x20/0x24. The box is built zeroed in a
   temporary and copied (a struct assignment: retail's ldl/ldr copy). y is
   one variable, 4 and then the bottom of the first text (buf.fE, written
   by FUN_001f7580) plus 0x10, with each icon at y + 8; it and x stay in
   callee-saved registers across the calls, as in retail. */



/* Page records in D_0013D390: stride 0x1C from +0x20. */
typedef struct { int row; char pad[0x18]; } PageRec;
typedef struct { char pad[0x20]; PageRec rec[1]; } PageTbl;

/* FUN_002220f0's sibling for the D_0013D390 pages: if the page passes
   four gates, draws icon 0x521C centred (row -1) or the row's two
   D_001DE0C0 icons at y 4 and 0x14. The row is read before
   func_001F4280; the record is reached as a struct member off the page
   base (retail's base-first add). */

extern void FUN_00225ac0(int);



extern int D_0015EFB4_mm __asm__("D_0015EFB4") MACRO_ADDR;
extern int D_0015EF34 MACRO_ADDR;
extern int D_0015FF4C MACRO_ADDR;
extern int D_0015EE98 MACRO_ADDR;
extern int D_0015EF20 MACRO_ADDR;
extern int D_0015EF24 MACRO_ADDR;
extern int D_0015EE84_mm __asm__("D_0015EE84") MACRO_ADDR;
/* The 8-byte D_0015EF98 pair saved into a slot. As a char block in small
   data, its copy is la + ldl/ldr, as in retail. */
typedef struct {
    char b[8];
} SaveWord2;
extern SaveWord2 D_0015EF98_s __asm__("D_0015EF98") MACRO_ADDR;
extern char D_001D28F8[];
extern int D_001D29C0;
extern void FUN_002269c0(int, int);
extern int D_0015EFB0_mm __asm__("D_0015EFB0") MACRO_ADDR;

/* Save-slot menu handler (func_002243E8's sibling): on entry it may arm
   a save (0x4FB5 prompt), then while the prompt is up it either aborts
   (flag 0x80 in D_0015EFB4) or writes the current game into slot
   d+0x14 of D_0013D390's 0x1C-byte records; otherwise the usual pad
   handling, with slot selection on 0x1000/0x4000 and 0x40 either
   loading the slot (via D_001D28F8) or re-arming. Each branch reaches
   the globals through its own block-scoped pointers. */









extern int func_002279D0(void);
extern int D_0016004C MACRO_ADDR;
extern char D_001D6160[];
extern char D_001D61A0[];
extern char D_001D61E0[];
extern char D_00186410[];
extern void func_001E9410(void *);
extern void func_00225DF0(void);
extern void func_00226250(void *);

/* Menu setup: resets the pad bindings, marks D_001D5F70's selections
   unset, fetches three entries with FUN_00225c18, clears the 24 flag
   bytes at arg0+0xA4, then spawns moby 0 in front of the camera (update
   func_00225DF0, owner arg0) and moby 0x259 (update func_00226250). One
   `g` local per block reading D_001D5F70 gives retail's %hi kept in a
   saved register with %lo rebuilt; the -1 stores are written in the
   order that gives retail's schedule. */

/*
 * Exact once the short-loop padding retail's assembler adds is reproduced
 * (tools/fix_short_loops.py): both loops are five instructions and get one
 * nop each. g is unsigned char so 0xFF is built as 0xFF, not -1. Two
 * separate loop counters are load-bearing: with one variable reused, both
 * loops take $s2 and q takes $s1.
 */
extern int func_002267C0(int);
extern int FUN_00225cd8(int);
extern void func_00227A70(void);






extern char D_001864D0_a[] __asm__("D_001864D0");
extern int FUN_0020d580(void *);
extern void FUN_0020def8(void *);
extern void func_0020CCA8(int, int, void *);
extern void FUN_00214128(void *);
extern void FUN_0020e098(void *);
extern void func_001E9480(void *, void *, int, int, int);

/* The pause-screen mobys these updates drive. */
typedef struct {
    char pad00[0x10];
    float pos[4];       /* 0x10 */
    char pad20[4];
    int sound;          /* 0x24 */
    char pad28[0x28];
    int x50;            /* 0x50 */
    int x54;            /* 0x54 */
    char pad58[0x10];
    char *x68;          /* 0x68 */
    char *x6C;          /* 0x6C */
    char pad70[8];
    char **cls;         /* 0x78 */
    char pad7C[0x2A];
    short oclass;       /* 0xA6 */
    char padA8[0x18];
    float mtx[16];      /* 0xC0 */
} PauseMoby;

/* D_001864D0's 0x4C-byte per-class records. */
typedef struct {
    char pad00[0xC];
    int bone;           /* 0x0C */
    int cls;            /* 0x10 */
    char pad14[4];
    int still;          /* 0x18 */
    char pad1C[0x30];
} PauseClassRec;

extern PauseClassRec D_001864D0_r[] __asm__("D_001864D0");
extern void func_00212F90(void *, int, int, int);
extern void FUN_0020cb88(int, void *);
extern void FUN_0020cb10(int, int, void *);

/* Moby update with a per-class record: find the class in D_001864D0 (0x4C
   bytes each, 0x25 of them), refresh the matrix from the record's bone,
   and either register the moby (+0x18 set) or restart its idle sound;
   also re-sync the D_001D6160 animation group while its +1 flag is up.
   The two store groups are in the order that schedules as retail's. */

/* Moby update: refresh its matrix from bone 4 of its class (+0x44),
   copying the translation row to the position, re-register it, start
   its idle sound (6 for class 0x1B1, else 0) on the D_001864D0 table
   and reset the sound fields. */



extern void func_00212F90(void *, int, int, int);

/* Moby 0x259 update: while flag 0x02 is set, keep its animation on 1 (or,
   for class 0x25F in state 6, run the 6 animation until its timer +0x20
   runs out and then blend back to 1), then refresh its matrix from bone 5
   as func_00225FB8 does. */

/* func_00225FB8's sibling: refresh the matrix from bone 0x1E (class 0x197) or 0x1D, copy its translation row to the position and re-register. FUN_0020d580 takes the moby, and the bone test is written == 0x197 so the movn picks 0x1D as retail does. */



/* Hoisted from the func_00227A70 block below so this earlier caller can
   see it -- a second NOT_SDA extern for the same symbol is a hard
   error. */
extern unsigned char D_001B3E40[] NOT_SDA;
extern void *FUN_0020c4f8(void);
extern void FUN_0020def8(void *);
extern void PackRenderCommandFields(void *, int, int, int, int);

extern void *func_0020D348_c(int) __asm__("FUN_0020c4f8");

/* Spawns the pickup/marker object for slot arg0, unless the slot is
   disabled (0xFF in D_001B3E40). A fresh object gets 0xFF/0xFF/1 in the
   0x30 block, is registered, tinted mid-grey, and flagged 0x18 at +0x73
   when its descriptor says so. The +0x30 store is through `unsigned
   char` and comes before the halfword store, so the 0xFF stays in one
   saved register. CreateMoby takes oClass: passing arg0 on keeps $a0
   live, which puts the element address in $v0 as retail has it. */

extern int D_0015F6F0 MACRO_ADDR;
extern void FUN_0020c828(void *); /* DeleteMoby */




/*
 * Exact once the short-loop padding retail's assembler adds is reproduced
 * (tools/fix_short_loops.py); it was the whole residual. Taking the
 * D_001517D0 base into a local declared after the calls keeps its
 * %hi/%lo out of a callee-saved register, as retail has it.
 */



extern void func_0020D330(int, int);






extern int FUN_00225d88(int handle);

/* Find the first binding that is enabled (bit 0 clear, or set when
   `invert` is given), still has a key and is not already claimed, claim
   it, and scrub its buffer with 0xDEADBEEF. Returns the key, or 0 if
   there is nothing to claim. */

/* Release the binding whose key matches. Bit 1 means "bound"; bit 2 on
   top of that means it also owns the shared 0xCB latch, which has to be
   handed back through func_00217588 first. Always returns 0 so callers
   can assign it straight over their handle. The flags are read from the
   table at every use, not kept in a local. */

extern int D_001D6448[];



/* Twin of func_00227068 above, clearing bit 2 instead of setting it.
   Written in exactly that function's shape -- the separate `base` local
   is what stops %lo+4 folding into one addiu, which is what an earlier
   round's revert was missing. */



extern int D_00160450 MACRO_ADDR;
typedef struct {
    int unk00;
    int arg[13];
} Rec38;
extern Rec38 D_001D6250[];


/* Pops the head of the 8-entry queue at D_001D6250. Copying through
   `d = &D_001D6250[i - 1]; *d = d[1];` makes the store the loop's
   master giv; the plain form reverses the loop. */

extern int D_001D641C;



extern char D_001D5D58[] NOT_SDA;
extern char *D_001B3580[] NOT_SDA;

/* Drain the pending list at D_001D5F70+0xA8/+0xAC, clearing +0x48 on each
   referenced object, then reset the count. Both the start and the count
   are re-read every iteration.

   Near-miss (29/36), size-exact, and now structurally identical to
   retail block for block. Two levers got it here:
     - taking the base into a local `char *g` instead of indexing the
       extern array directly fixed the loop's offsets;
     - declaring `p` INSIDE the loop body rather than before the `while`
       moves its initialisation into the loop preheader, where retail
       has it. Spelled before the loop, gcc hoists the %hi/%lo of
       D_001D5D58 and the `sll`/`addu` above the guard, and the loop's
       .p2align then eats the slack as a nop. Let gcc build the
       induction variable itself and the preheader comes out right.

   What is left is two things, neither source-reachable:
     - the allocator permutation: retail puts D_001B3E40 in $9 and
       D_001B3580 in $8, this build swaps them, and every $v0/$v1 in
       the loop body is correspondingly transposed. Splitting the
       nested index into `int k = D_001B3E40[a];` does not move it.
     - retail COPIES the raw %hi of D_001D5F70 into $t2 in the prologue
       and rebuilds the pointer with `addiu $2,$10,%lo` for the final
       store, keeping both the full pointer and the bare high half live
       across the loop. This build re-does the whole `lui` at the end
       instead -- same instruction count, different encoding. The
       two-names-on-one-symbol trick
       (`extern char D_001D5F70_2[] __asm__("D_001D5F70");`) was tried
       here and changes nothing: it defeats CSE of the full address,
       which we already lack, not of the high half, which is what
       retail is sharing. */



extern char D_0015EF98[] MACRO_ADDR;
extern char D_00141FC0[];
extern void sceCdReadClock(void *);
extern void sceScfGetLocalTimefromRTC(void *);
extern void FUN_00208770(void);
extern void func_00207B08(void *);


/* func_00209DC0 (memcard.c) takes nothing. */
extern void func_00209DC0(void);

/* FUN_002269c0's sibling: runs func_00209DC0, the D_0015EF98 pair and
   FUN_0020abb0(arg0), then records arg0/arg1 in D_0013D390 (+0xF4,
   +0x14), clears +0xC8 and page arg1's first word, and seeds the result
   (+0xE4 = 0x13) when none is pending. func_00209DC0 is called without
   arguments: passing arg0 gives it one more reference and so the first
   callee-saved register, the reverse of retail's. */



extern int D_0015F6D0 MACRO_ADDR;
extern int D_0015EF24 MACRO_ADDR;
extern int D_0015EE80 MACRO_ADDR;


extern int FUN_00215348(void);
extern int FUN_00215300(void);
extern char D_001D2B80[];
extern char D_001D2BF8[];

/* Unlock states for the pause menu's reward rows: with at least 15 / 30
   of the D_0013D510 flags (FUN_00215348) and 10 of the D_0013E620 ones
   (FUN_00215300), rows 5-8 of the 12-byte item table D_001D2B80 get
   state 3 (10 for row 7) instead of 2, and the menu at D_001D2BF8 its
   text ids; row 8's first half is cleared while D_0015EF90 is set. The
   three tests are kept as 0/1 values. Each item store reads the table
   through its own `char *` (retail's kept %hi); the short selects stay
   branches (HImode has no conditional move), the int ones become
   movz/movn. */

extern int D_001D6860[];
extern int D_001D74C0[];
extern int D_001D6760[];




extern float D_00160470[] MACRO_ADDR;
extern float D_00160470_x __asm__("D_00160470");
extern float D_00160474;
extern float D_00160478;
extern float D_00160480[] MACRO_ADDR;
extern float D_00160480_x __asm__("D_00160480") MACRO_ADDR;
extern float D_00160484;
extern float D_00160488;
extern float D_00160490;
extern float D_00160494;
extern float D_00160498;
extern void func_001F9AD8(void *, void *, void *);

typedef struct {
    float v[4];
} __attribute__((aligned(16))) PauseVec;

/* Build a basis from dir: D_00160470 = dir normalised,
   D_00160490 = that scaled, and D_00160480 = the cross product with a
   vector built from dir's components reordered (the smallest moved), so
   the result is perpendicular, then normalised. Compiled with
   -mno-split-addresses (config/func_cflags.txt): every global goes
   through the assembler's lui $at macro, and only D_00160480, declared
   small, uses $gp when it lands in a delay slot. The aligned struct
   copy is schedulable where qcopy's asm is not. */

/*
 * Dispatch on a leading short: 0 and 1 each call a handler and advance
 * the pointer differently, anything else returns it unchanged.
 *
 * An earlier round reverted this at 4 bytes short and read the residual
 * as a delay-slot problem -- retail spends the first jal's delay slot on
 * `addiu $16,$16,0x20`, we emitted a nop. The real cause was one level
 * up: with a `return` inside each arm, gcc folds the advance into the
 * return value (`addu $2,$16,32`, one instruction), and there is then
 * nothing left for the delay slot. Retail updates the pointer and copies
 * it to $v0 separately, three times, which is what a SINGLE `return p`
 * at the join gives: the copy belongs to the join block and the
 * delay-slot filler duplicates it into both branches.
 *
 * So this is the exit-cross-jumping lever used the other way round.
 * The usual reach is to SPLIT exits that gcc merged; here retail really
 * does share one, and the fix was to stop returning early. When a
 * 4-byte shortfall looks like a missing delay-slot fill, check first
 * whether an expression got folded that retail kept in two steps.
 */

extern int *D_00161000 MACRO_ADDR;
extern char D_001D8120[];
extern short D_00160460;              /* SDA, gp -0x68A0 */
extern void FUN_00228598(int, int);




/*
 * REVERTED (size mismatch: 452 vs retail's 460 -- 8 bytes short, after
 * closing an initial 28-byte gap). Semantics recovered with confidence --
 * builds a texture-paging GIF/DMA packet: a tag header, a fixed 10-field
 * 0x50-byte block (the field at hdr+0x48 is easy to miss -- it isn't
 * adjacent to the others), then (if the tile count n=w/32 is positive) a
 * per-tile table of packed TRXPOS-style coordinates built from the
 * screen width/height at D_00151880[0xA8]/[0xA9]:
 *
 *   void FUN_00227378(unsigned long arg0) {
 *       int w, h, n, i;
 *       long ypack_a, ypack_b, xbase_a, xbase_b;
 *       char *hdr, *table, *newptr;
 *
 *       w = D_00151880[0xA8];
 *       h = D_00151880[0xA9];
 *       n = w / 32;
 *
 *       D_00161000[0] = (n + 5) | 0x10000000;
 *       D_00161000[1] = 0;
 *       D_00161000[2] = 0;
 *       D_00161000[3] = (n + 5) | 0x50000000;
 *       D_00161000 += 4;
 *
 *       hdr = (char *)D_00161000;
 *
 *       *(unsigned long *)(hdr + 0x00) = ((unsigned long)0x8000 << 45) | 1;
 *       *(unsigned long *)(hdr + 0x48) = 0x44;
 *       *(unsigned long *)(hdr + 0x08) = 0xE;
 *       *(unsigned long *)(hdr + 0x10) = 0x3D801;
 *       *(unsigned long *)(hdr + 0x18) = 0x47;
 *       *(unsigned long *)(hdr + 0x20) = ((unsigned long)0x9000 << 46) | 1;
 *       *(unsigned long *)(hdr + 0x28) = 0x10;
 *       *(unsigned long *)(hdr + 0x30) = 0x146;
 *       *(unsigned long *)(hdr + 0x38) = arg0;
 *       *(long *)(hdr + 0x40) = (long)(n | 0x8000) | ((long)0x9000 << 46);
 *
 *       if (n > 0) {
 *           long v0, v1;
 *
 *           ypack_a = (long)(0x8000 - h * 8) << 16;
 *           ypack_b = (long)(h * 8 + 0x7FF0) << 16;
 *           xbase_a = -(w * 8) + 0x8000;
 *           xbase_b = -(w * 8) + 0x8200;
 *
 *           table = hdr + 0x50;
 *           i = 0;
 *           do {
 *               v0 = xbase_a | ypack_a;
 *               v1 = xbase_b | ypack_b;
 *               *(long *)table = v0;
 *               i++;
 *               table += 8;
 *               xbase_b += 0x200;
 *               *(long *)table = v1;
 *               xbase_a += 0x200;
 *               table += 8;
 *           } while (i < n);
 *       }
 *
 *       newptr = hdr + 0x50 + n * 0x10;
 *       D_00161000 = (int *)newptr;
 *
 *       D_00161000[0] = 0x10000000;
 *       D_00161000[1] = 0;
 *       D_00161000[2] = 0x13000000;
 *       D_00161000[3] = 0;
 *       D_00161000 += 4;
 *   }
 *
 * (needs D_00161000 MACRO_ADDR). The per-tile loop is instruction-for-
 * instruction exact against retail (confirmed via diff -- this took
 * writing it as an incrementing-pointer do-while with both store values
 * precomputed up front, matching retail's exact interleaving of the
 * pointer bump between the two stores; a straightforward for-loop with
 * offset-indexed stores compiled to a different, larger schedule).
 * Residual: retail keeps BOTH w and h live in callee-saved registers
 * ($16/$17) across the whole function, needing a 0x20-byte frame; this
 * compiler only needs one saved register for the pair (keeping the other
 * in an ordinary temporary that happens to survive the header stores
 * unclobbered), needing a smaller frame -- 8 bytes under. Also builds a
 * few of the header's 64-bit constants via a different (same-length)
 * instruction encoding (`lui`+`dsll32` vs retail's `ori`+`dsll32`) for
 * the same value. Tried hoisting `i=0` earlier (made it worse: forced a
 * THIRD saved register instead of one); tried reordering the hdr+0x48
 * statement (no effect on size). Not reached further this pass.
 */

extern __typeof__(setup_menu_display_text) func_0021EF78 __attribute__((alias("FUN_0021ef78")));
