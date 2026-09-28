/* Ported from rac1-decomp, the PAL decompilation (src/game/loaders.c, func_00204FC0). */
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
 * loaders.cpp in the original source; text 0x202AA8-0x205520.
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





extern int D_001601C0 MACRO_ADDR;
extern char D_001CE500[];
extern int D_001CE300[];
typedef struct { int a; int b; } Pair_2F00;
extern Pair_2F00 D_001CDD00[];
extern void FUN_001f9838(void *, void *, int);
extern int FUN_001f97a0(int);

/* ParseParticleTexs */

extern int FUN_001f97a0(int);
extern int D_0015F55C MACRO_ADDR;
extern char D_0018D540[];

typedef struct {
    unsigned long flags;
    short y;
    short x;
    short u;
    short v;
} LoadPoint;
extern LoadPoint D_0018D540_p[] __asm__("D_0018D540");
extern short func_001F9968_s(int) __asm__("FUN_001f97a0");

/* Unpack count point records (x, y, u, v words, 12.4 fixed point for
   x/y) into D_0018D540. Adapted from Lombyte (MIT) for PAL. */

extern char *D_0016055C MACRO_ADDR;

typedef struct {
    long tag;
    short unk08;
    short unk0A;
    short unk0C;
    short unk0E;
} SkyPageL;
/* A shell is a row of 0x20-byte records: a header holding the count,
   then one record per item whose first word is an offset to relocate. */
typedef struct {
    char *ptr;
    char pad04[0x1C];
} SkyShellItemL;
typedef struct SkyShellL {
    int count;
    char pad04[0x1C];
} SkyShellL;
typedef struct {
    int unk00;
    short unk04;
    short count;      /* 0x06: shells */
    int unk08;
    short npages;     /* 0x0C */
    short unk0E;
    SkyPageL *pages;  /* 0x10 */
    char *unk14;
    char *unk18;
    char *unk1C;
    struct SkyShellL *shells[1]; /* 0x20 */
} SkyDefL;
extern SkyDefL *D_0016055C_s __asm__("D_0016055C") MACRO_ADDR;

/* Relocates the sky definition s just loaded (its pointers are offsets
   from s) and makes it the current one (D_0016055C): the page table and
   the +0x14/+0x18/+0x1C blocks (+0x1C only when present), then each GIF
   page, whose four words are rewritten in place as two 1/16 values, two
   FUN_001f97a0 results and a cleared tag, then each shell and the items
   in it. The pages are reached through D_0016055C at every use, as
   retail reloads it after each call. The item pointer is a separate
   `sh + k` local: loop.c then reduces it to one register starting at sh
   and reaches the item at +0x20 from it, as retail does. */



/*
 * The heap cursor. Retail reaches it with the one-register macro form
 * (both the load and the $at store), so it is MACRO_ADDR; D_0019A500
 * next to it is the ordinary split lui/%lo, so it stays plain.
 *
 * FUN_00202d10 is 3/100: two addu operand orders are reversed
 * (`addu $2,$4,$2` and the $3/$4 pair in the tail). Every spelling of
 * both address expressions -- base-first, index-first, array indexing
 * on a cast pointer, the base hoisted into a local -- compiles to the
 * identical instruction stream, so this is the known
 * operand-order-is-not-source-steerable case.
 */
extern int *D_0015EF4C MACRO_ADDR;
extern char *D_0019A500;
extern void func_0020C468(int);

/* LoadCompressedHudBank(int, char *). Each scaled index in its own local
   gives retail's base-first addu; written inline, the multiply goes
   first. */



extern long D_0019E640[];
extern long D_0019E7C0[];
extern long D_0019E7D8[];

/* Fills four A+D qwords (low dwords only) at out: a TEX1-style word
   (MXL bits from the second word of row arg5 of the 3-dword table
   D_0019E640, MMAG 1, MMIN arg2, K arg1), then arg3 | arg4 << 2 |
   arg5 << 24, then the row's first and third words. A negative arg5 has
   no row: -2 and -3 take the rows D_0019E7C0 and D_0019E7D8 (and 5 for
   the second word), -1 a fixed constant and 0. The row is read before
   the sign test, as in retail. The OR operands are named locals so fold
   does not move the 0x20 to the end of the chain. */

extern int D_0015EF8C MACRO_ADDR;
extern int D_0015EF78 MACRO_ADDR;
extern int D_0015EF74 MACRO_ADDR;
/* libgraph: sceGsSetDefLoadImage, sceGsExecLoadImage, sceGsSyncPath;
   func_00118D80 is the kernel's FlushCache. */
extern int sceGsSetDefLoadImage(void *, short, short, short, short, short, short, short);
extern int sceGsExecLoadImage(void *, void *);
extern void FlushCache(int);
extern int FUN_00120558(int, unsigned short);

typedef struct {
    int type;   /* GS pixel format: 0x13 PSMT8, 2 PSMCT16, 0 PSMCT32 */
    int packed; /* width in the low half, height in the high half */
    int pad8;
    int size;   /* offset of the image data from the base */
} Chunk;

typedef struct {
    long w[12];
} GsLoadImage __attribute__((aligned(16)));

/* Uploads `count` images described by `list` into GS memory, starting at
   the VRAM cursor D_0015EF74 (reset from D_0015EF8C) and advancing it by
   each image's size, then records the end in D_0015EF78. An 8-bit image
   is w*h bytes (at least 0x100) with a buffer width of w/64 (at least 1);
   the other two formats are fixed 16x16 uploads. The prototypes are the
   SDK's (short arguments), the clamps are written `< 1` / `< 0x100` so the
   loop pass hoists their constants into saved registers, and the pointer
   advances in the for increment so the reversed counter's decrement
   lands in FlushCache's delay slot. */




extern int D_00160000 MACRO_ADDR;
extern unsigned char D_001B3E40[] NOT_SDA;
extern short D_001B3C80[];
extern char *D_001B3580[] NOT_SDA;
extern int D_001B6500[];
extern void FUN_00212d68(int);
extern void FUN_00203338(void *, int, int, int);

/* Registers class arg3's data arg0 in the next moby-class slot
   (D_00160000): class -> slot in D_001B3E40, slot -> class in
   D_001B3C80, slot -> data in D_001B3580; then FUN_00212d68(arg3) and
   the slot count is bumped. A non-null arg0 also has its +0x2C field
   kept in D_001B6500 and is set up by FUN_00203338 once the count is
   bumped. The slot byte is D_00160000 read again as a byte. */

extern int D_00137C80[];
extern char D_1FF7FF0[];
extern int func_002175C8(int, int, int);




struct PartList {
    unsigned char pad00[6];
    unsigned char flag;          /* 0x06 */
    unsigned char pad07[5];
    unsigned char used;          /* 0x0C */
    unsigned char pad0D[0x3B];
    int entries[1];   /* 0x48 */
};

struct ColorSrc {
    unsigned char pad00[0x38];
    unsigned long color;        /* 0x38 */
};

struct RenderGlobals {
    unsigned char pad00[0x2080];
    struct ColorSrc *color_src;  /* 0x2080 */
};

struct GlobalIndex {
    unsigned char pad00[0x26];
    short slot;         /* 0x26 */
};

struct Moby {
    unsigned char pad00[0x24];
    struct PartList *parts;  /* 0x24 */
    unsigned char pad28[0xA];
    unsigned short unk32;        /* 0x32 */
    unsigned short unk34;        /* 0x34 */
    unsigned char pad36[2];
    unsigned long color;        /* 0x38 */
    unsigned char pad40[8];
    int attach;       /* 0x48 */
    unsigned char pad4C[6];
    unsigned char idx;           /* 0x52 */
    unsigned char slot;          /* 0x53 */
    unsigned char pad54[0x1E];
    unsigned char flag72;        /* 0x72 */
    unsigned char flag73;        /* 0x73 */
    unsigned char pad74[4];
    int model;        /* 0x78 */
    unsigned char pad7C[0x18];
    int unk94;        /* 0x94 */
};

struct ModelRec {
    unsigned short flags;        /* 0x00 */
    unsigned char pad02[2];
    int part_count;   /* 0x04 */
    unsigned short pad08;
    unsigned char pad0A[2];
    unsigned short num_parts;    /* 0x0C */
    unsigned char pad0E[2];
    int end_off;      /* 0x10 */
    int part_off[1];  /* 0x14 */
};

struct TransferState {
    unsigned char pad00[0x38];
    int unk38;        /* 0x38 */
    unsigned char pad3C[4];
    unsigned short unk40;        /* 0x40 */
    unsigned char pad42[2];
    short num_parts;    /* 0x44 */
    unsigned char pad46[2];
    unsigned short unk46;        /* 0x48 */
    unsigned char pad4A[2];
    int unk4C;        /* 0x4C */
    int unk50;        /* 0x50 */
    int unk54;        /* 0x54 */
    struct ModelRec *rec;     /* 0x58 */
    int unk5C;        /* 0x5C */
    unsigned char pad60[0x118];
    struct Moby *slots[1];    /* 0x178 */
};

extern struct TransferState D_0018CB20_t __asm__("D_0018CB20") NOT_SDA;
extern struct GlobalIndex D_0013E030_g __asm__("D_0013E030");
extern struct RenderGlobals D_00160488_r __asm__("D_0013F350");
extern int D_0015F604 MACRO_ADDR;
extern int D_00160488[];
extern int func_0020C468_2(int, int) __asm__("FUN_0020b618");
extern struct Moby *func_0020D348_m(int) __asm__("FUN_0020c4f8");

/* UpdateWorldObjectAnimation: relocate the model record loaded for the
   current chunk (D_0018CC20+0x58) and hook each of its parts to a moby,
   spawning (FUN_0020c4f8) and initialising the moby the first time; each
   part's pointer table is made absolute. Adapted from Lombyte (MIT) for
   PAL. */
void update_world_object_animation(void *arg0) __asm__("FUN_00204790");

void update_world_object_animation(void *arg0) {
    struct ModelRec *rec;
    struct Moby *mob;
    int *cp;
    int *sp;
    int i;
    int k;
    int idx;
    int id;
    int off;
    int endp;
    int pc;

    FlushCache(0);
    func_0020C468_2(D_0018CB20_t.unk5C, (int)D_0018CB20_t.rec);
    FlushCache(0);

    rec = D_0018CB20_t.rec;
    D_0018CB20_t.unk38 = 0;
    cp = rec->part_off;
    D_0018CB20_t.unk40 = rec->flags;
    pc = rec->part_count;
    D_0018CB20_t.unk46 = rec->pad08;
    D_0018CB20_t.num_parts = rec->num_parts;
    D_0018CB20_t.unk54 = (int)rec + rec->end_off;
    if (pc < 0x400) {
        D_0018CB20_t.unk4C = 0;
    } else {
        D_0018CB20_t.unk4C = (int)rec + pc;
    }

    for (i = 0; i < D_0018CB20_t.num_parts; i++) {
        off = *cp++;
        sp = (int *)((unsigned char *)rec + off);
        id = sp[0];
        sp = (int *)((unsigned char *)sp + 0xC);
        endp = (int)rec + sp[0];
        sp = (int *)((unsigned char *)sp + 4);
        if (D_0015F604 == 6 && i == 0 && id == 0x215) {
            id = D_00160488[D_0013E030_g.slot];
        }
        mob = D_0018CB20_t.slots[i];
        if (mob == 0) {
            mob = func_0020D348_m(id);
            idx = mob->parts->used;
            mob->parts->used = idx + 1;
            mob->idx = idx;
            mob->slot = idx;
            mob->unk32 = 0x1FF;
            mob->unk34 |= 6;
            mob->flag72 = 0xFF;
            mob->unk94 = 0;
            if (D_00160488_r.color_src == 0) {
                mob->color = 0x38383800000000;
            } else {
                mob->color = D_00160488_r.color_src->color;
            }
            if (mob->parts->flag) {
                mob->flag73 = 0x18;
            }
            D_0018CB20_t.slots[i] = mob;
        }
        mob->model = endp;
        mob->parts->entries[mob->idx] = sp;
        for (k = 0; k < ((unsigned char *)sp)[0x10]; k++) {
            int *w = (int *)((unsigned char *)sp + 0x1C);

            w[k] = (int)sp + w[k];
        }
    }
}



extern void update_world_object_animation(void *);
extern int D_0018CC20 NOT_SDA;
extern int D_001941C8 NOT_SDA;
extern int D_0016100C;

extern int D_0016100C_m __asm__("D_0016100C") MACRO_ADDR;

/* ParseSpaceSceneChunk(int): run update_world_object_animation on chunk index's slot,
   with the chunk's value as the current one meanwhile. The slot address is
   written base - (-(index * 4)), which keeps retail's base-first addu
   (adapted from Lombyte (MIT) for PAL). */

extern __typeof__(update_world_object_animation) func_00204790 __attribute__((alias("FUN_00204790")));
