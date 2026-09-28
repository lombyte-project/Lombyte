/* Ported from rac1-decomp, the PAL decompilation (src/game/draw.c, func_001F4C30). */
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
 * draw.cpp in the original source; text 0x1F0F30-0x1F7C60.
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




typedef struct {
    int x;
    int y;
    int color;
    char *str;
} DrawTextRec;
extern DrawTextRec D_0018AC00[];
extern short D_0015F100;
extern short D_0015F104;
extern char D_0015F108[];
extern int sprintf();

/* Queues one text item: D_0018AC00[n] = {x, y, colour, pool position},
   then sprintf(pool, "%s", str) (D_0015F108 is "%s") advances the
   D_0015F100 string pool past the copy. Indexing the table at every
   store gives retail's two addu forms; a `DrawTextRec *` local folds
   them into one register and comes out 12 bytes short. */



extern int D_00189EC0[];

/* Draws `str` centred on x: sums the per-character widths in D_00189EC0
   (indexed by char - 0x20, anything past the table using entry 0x20),
   moves x left by half the total and queues the text with
   FUN_001f0bd0. Returns the adjusted x. `idx = ch` followed by the
   out-of-range override gives retail's sltiu 0x60 + movn; the reverse
   (default first, then `if (ch < 0x60)`) becomes sltu + movz. */


extern void FUN_001f9fc8(void *);
extern void FUN_001fa378(void *, void *, void *);
extern void FUN_001f9a68(void *, void *, float);
extern void FUN_001f9d20(void *, void *, void *);
extern float D_0018D010;

typedef struct {
    char pad0[0x40];
    float viewMtx[4][4];   /* +0x40 */
    char pad1[0xC0];
    float focus[3];        /* +0x140 */
} CameraBlock;
/* The camera block as a struct: all four accesses then share one base
   register with field offsets, as in retail. */
extern CameraBlock D_00187040_cam __asm__("D_00187040");

typedef struct {
    float x, y, z;
} Vec3f;

/* Projects camera-space point a1 through the camera matrix, divides by
   depth (D_0018D010 / w) and scales to x16 screen units. FUN_002333a8
   in tfragfunc.c builds the same matrix. */





extern int D_0015F6FC_m __asm__("D_0015F6FC") MACRO_ADDR;
extern int D_0015EE80 MACRO_ADDR;
extern int D_0015EF78 MACRO_ADDR;
extern int *D_00161000 MACRO_ADDR;

extern void FUN_00233d90(void);
extern int sceDmaReset(int);
extern void func_0020B418(void);
extern void sceGsResetGraph(int, int, int, int);
extern void FUN_001204b8(void);
extern void FUN_002335d0(void);
extern void FUN_001f34e8(void);
extern void FUN_00233d00(void);
extern volatile int D_00160FE0_v __asm__("D_00160FE0") MACRO_ADDR;

/* Render setup: raises D_0015F6FC, clears D_00160FE0, runs the setup calls,
   picks sceGsResetGraph's mode from D_0015EE80 (retail's movz), then runs
   FUN_001f34e8 with D_0015EF78 saved around it and D_00161000 cleared.
   The D_00160FE0 flag is volatile: retail keeps its store out of
   FUN_00233d90's delay slot, where the plain MACRO_ADDR store would go.
   The two stores that do sit in delay slots (D_00161000, D_0015EF78) are
   MACRO_ADDR and come out $gp-relative there. */

/* Retail carries 4 bytes of inter-function padding after this endlabel. */

/* A fog preset: an RGB byte triple and four floats. */
typedef struct {
    unsigned char r, g, b, pad;
    float f[4];
} FogPreset;
extern int D_001873D4;
extern FogPreset D_001611C4 MACRO_ADDR;
extern FogPreset D_0015F584 MACRO_ADDR;
extern int D_0018CE00[];
extern int D_001601BC MACRO_ADDR;
extern int D_0015F598 MACRO_ADDR;
extern void FUN_001f2d98(void);

/* UpdateFog(int): copies the fog preset (the fixed D_001611C4 when
   D_001873D4 is set, else the current D_0015F584) into the draw context
   at D_0018CE00+0x218, sets the mode word D_001601BC (0x40000 or
   0x1F4000), runs UpdateViewContext (FUN_001f2d98) and clears
   D_0015F598. The presets and the two words it writes are MACRO_ADDR:
   retail reads each field with the one-register macro, and the mode
   word's store fits the branch delay slot ($gp-relative there). */

extern char *D_0015F720 MACRO_ADDR;

/* ParseOcclGrid(x, y, z): walks the three-level occlusion grid at
   D_0015F720. Each level is {u16 start, u16 count, u16 entry[count]};
   the coordinate minus start must fall in [0, count), and its entry is
   the next level's offset in words from the grid (levels 1 and 2, 0 =
   empty) or, at the last level, a 128-byte cell index from the root
   (grid + grid[0]), 0xFFFF = empty. Returns the cell or 0. Each level's
   coordinate goes in its own local: that keeps y and x in their argument
   registers and the level pointer in $a3. The last level's bounds share
   one `if`, which is what leaves retail's shared failure return after
   level 2 and a separate one for the 0xFFFF test. */


extern int FUN_001f2690(int, int, int);

/* GetOcclGridFromPair(int, int, int, int, int, int, float) */



/* Unprototyped deliberately: two call sites need incompatible arg1
   types (-1 and a pointer) and both callers are byte-exact, so
   neither may be edited. Codegen is identical either way -- int and
   pointer are both 32-bit in the same arg register. */
extern void FUN_001f9810();
extern void FUN_001f2820(void);
extern int D_0018C434 NOT_SDA;
extern char D_001940C0[];

/* UpdateOcclusion(void) */

extern short D_00151880[];
extern int D_0013E600[];
extern float func_001FA6C0(int);

/* InitViewContext: the same set-up as SetScreenSize (FUN_001f33b8) for
   the display's size, D_00151880[0xA8]/[0xA9], with fixed extras. The
   GS viewport record D_0013E600 gets width, height, their halves and the
   four <<4 edges around the 0x800 centre. The draw context D_0018CE00
   gets 32, 745472 and 0.63 at +0xA0/+0xA4/+0xB0, the half extents as
   floats (func_001FA6C0 is int to float) at +0x200/+0x204, four times
   each at +0x208/+0x20C, and 0, 524288, 255, 0 at +0x218/+0x21C/+0x228/
   +0x22C. The size is read into `short` locals (lhu, then sll/sra) through
   a base pointer that stays in $s1, and the height is read again from
   memory for the second conversion. */


extern int D_0013E600[];
extern int D_0018CE00[];
extern float func_001FA6C0(int);
extern void FUN_001f2d98(void);

/* Sets the screen size: the GS viewport record at D_0013E600 gets width,
   height, their halves and the four <<4 edges around the 0x800 centre;
   the draw context at D_0018CE00 gets a2 at +0xB0, 32 and 524288 at
   +0xA0/+0xA4, the half extents as floats (func_001FA6C0 is int to
   float) at +0x200/+0x204, four times each at +0x208/+0x20C, and a3..a6
   at +0x218/+0x21C/+0x228/+0x22C; then UpdateViewContext (FUN_001f2d98).
   The context is reached through one local pointer, which keeps its
   full address in $s0 as retail does (indexing the global directly
   folds +0xB0 into the base and spends $a0 on the %hi). */



/*
 * ResetDrawGlobals. Seventeen zero stores in a row, in three addressing
 * forms that are all one assembler macro: lui/$at for most, plain $gp
 * for the three that really are small-data, and $gp again for the last
 * one because it lands in the jr delay slot where a two-instruction
 * expansion will not fit.
 */
extern int D_0015F430 MACRO_ADDR;
extern int D_0015F434 MACRO_ADDR;
extern short D_0015F44C;              /* SDA, gp -0x78B4 */
extern short D_0015F460;              /* SDA, gp -0x78A0 */
extern short D_0015F470;              /* SDA, gp -0x7890 */
extern int D_0015F544 MACRO_ADDR;
extern int D_0015F548 MACRO_ADDR;
extern int D_0015F564 MACRO_ADDR;
extern int D_0015F568 MACRO_ADDR;
extern int D_0015F56C MACRO_ADDR;
extern int D_0015F570 MACRO_ADDR;
extern int D_0015F474 MACRO_ADDR;
extern int D_0015F728 MACRO_ADDR;
extern int D_00161290 MACRO_ADDR;
extern int D_00161294 MACRO_ADDR;
extern int D_00161298 MACRO_ADDR;
extern int D_0016129C MACRO_ADDR;


extern int *D_00161000 MACRO_ADDR;
extern void FUN_00233980(int, long);
extern char D_0013D0C0[];
extern char D_0013D010[];
extern int D_0018CE00[];

/* ResetGsRegisters(void) */

extern long D_00151888[3];

/* GS privileged-register writes (0x1200_00XX = the GS's memory-mapped
   register block): CSR ack, PMODE, then SMODE2/DISPFB1/DISPFB2/DISPLAY1/
   DISPLAY2/BGCOLOR set from a 3-entry table. */
/* ResetGsRegistersPr(void) */



extern int D_0015F6FC;
extern short D_0015F534;              /* SDA, gp -0x77CC */
extern void FUN_001fb368(void);
extern void FUN_001f39d0(void);

extern int D_0015F6FC_m __asm__("D_0015F6FC") MACRO_ADDR;

/* D_0015F6FC is read through a MACRO_ADDR alias: retail's one-register
   lui $2 / lw $2 (an older note filed it as an allocator question). */



extern int *D_00161000 MACRO_ADDR;
extern int *D_0015F550 MACRO_ADDR;
extern int D_0015EF78 MACRO_ADDR;
extern int D_0015EF74 MACRO_ADDR;
extern int D_0015F55C MACRO_ADDR;
extern int D_0015EF8C MACRO_ADDR;
extern short D_0015F558;
typedef struct {
    long unk0;
    long unk8;
} PageSlot;
extern PageSlot D_0018D540[];
extern char D_0019A4E8[];

static inline char *PagingArena(void) {
    return D_0019A4E8;
}

/* SetupGifPaging(int): marks the D_00161000 packet in D_0015F550 and
   reserves 0x10 bytes, copies D_0015EF78 to D_0015EF74, clears
   D_0015F558 and the first dword of the D_0015F55C paging slots, then,
   when arg0 is 0, clears the +4 half of the arena's 8-byte list entries:
   the list at D_0019A4E8 + 0x24 (count at +0x44 of the record at +0x18)
   where it is at least D_0015EF8C >> 8, and all of the list at + 0x28
   (count at +0x24). The slot loop has its own counter, which loop
   reversal copies from the count (retail's $v1). The first list reads
   the arena through a static inline accessor (a fresh pseudo per read
   gives retail's copy for the loop) with the element offset first
   (`i * 8 + base`); the second through a block-local pointer, whose
   %hi retail keeps. */

/* Retail carries 4 bytes of inter-function padding after this endlabel. */

extern int *D_00161000 MACRO_ADDR;
extern int *D_0015F550 MACRO_ADDR;
extern int *D_0015F554 MACRO_ADDR;
extern int D_0018A3DC;
extern void FUN_0020b4a8(void);
extern void FUN_00233b68(void);

/* DoGifPaging: pushes two 4-word GIF tags (0x20000000 in the first word)
   onto the D_00161000 packet, D_0015F554 marking where it started and
   D_0015F550's tag pointing at the second one; between the two, when
   D_0018A3DC is set, FUN_0020b4a8 and FUN_00233b68 add their own.
   The first advance goes through a local advanced in place, which keeps
   the old and new pointer in one register as retail does. */

/* Retail carries 4 bytes of inter-function padding after this endlabel. */

/*
 * Four parallel callback lists, each a (function, argument) pair of
 * arrays with its own count, plus a "register" and a "run them all"
 * function per list. The counts are reached with retail's one-register
 * macro form, so they are MACRO_ADDR.
 */
typedef void (*DrawCallback)(void *);
extern DrawCallback D_0018DC40[];
extern void *D_0018DD40[];
extern DrawCallback D_0018DE40[];
extern void *D_0018DF40[];
extern DrawCallback D_0018E040[];
extern void *D_0018E140[];
extern DrawCallback D_0018E240[];
extern void *D_0018E340[];







typedef float FVec4[4] __attribute__((aligned(16)));
typedef struct {
    FVec4 v;
} FRow;
typedef struct {
    FRow pos;
    FRow dir;
} LightRec;
extern FRow D_0018CAA0[4];
extern LightRec D_0018E340_l[] __asm__("D_0018E340");
extern int FUN_001f44b8(int);
extern void FUN_001f9bf8(void *dst, void *src, float len);
extern void func_001F7D30(void *, int, int);

/* Builds a 4-row matrix per light in D_0018E340 (count D_0015F474):
   each row starts as the light's position and is pushed along the
   corner D_0018CAA0[k], projected off the normalised light direction,
   scaled by the position's w; func_001F7D30 draws it. The packet header,
   colours and UVs are filled on the stack but never sent. Both copies
   are qcopy (retail's lq/sq stay inside the loop), and the inner loop
   needs its own counter, not the one the setup loop used. */
void FUN_001f4880(void) {
    FRow m[4];
    int colors[4];
    float uv[4][2];
    unsigned long pkt[4];
    FRow dir;
    int i;
    int j;

    pkt[1] = FUN_001f44b8(0);
    pkt[2] = 0xFF9000000260;
    pkt[0] = 5;
    pkt[3] = 0x8000000044;
    for (j = 0; j < 4; j++) {
        uv[j][0] = D_0018CAA0[j].v[2];
        uv[j][1] = D_0018CAA0[j].v[3];
        colors[j] = 0x40808080;
    }
    for (i = 0; i < D_0015F474; i++) {
        float s;
        int k;

        qcopy(&dir, &D_0018E340_l[i].dir);
        FUN_001f9bf8(&dir, &dir, 1.0f);
        s = D_0018E340_l[i].pos.v[3];
        for (k = 0; k < 4; k++) {
            float cx = D_0018CAA0[k].v[0];
            float vx = dir.v[0];
            float cy = D_0018CAA0[k].v[1];
            float d = cx * vx + cy * dir.v[1];

            qcopy(&m[k], &D_0018E340_l[i].pos);
            m[k].v[0] += (cx - vx * d) * s;
            m[k].v[1] += (cy - dir.v[1] * d) * s;
            m[k].v[2] -= dir.v[2] * d * s;
        }
        func_001F7D30(m, 0, 0);
    }
}

extern void FUN_002337b0(int mask);
extern int func_00122598_i(int) __asm__("sceGsSyncV");
extern void FUN_002335d0(void);
extern void FUN_00233630(void);
extern void FUN_002336a0(void);
extern void FUN_001fb2d0(void);
extern void FUN_001fb368(void);
extern void FUN_001fb3d0(void);
extern void FUN_001f5210(int, int, int, int);
extern void FUN_00233980(int, long);
extern int *D_00161000 MACRO_ADDR;
extern int D_0015F538 MACRO_ADDR;
extern char D_0013CED0[];

/* FadeToBlack(frames, color): `frames` VU1 chains, each drawing a
   full-screen quad (D_0013CED0's GS packet) with its alpha ramped down
   from 0x80 by (n << 7) / (n + 1) of the count still to go; `color` is
   not read. Each chain is bracketed by FUN_002337b0/sceGsSyncV and a
   D_0015F538 bump. sceGsSyncV (sceGsSyncV) returns int: declared that
   way, the bump's temporary moves to $v1 as in retail. D_0015F538 is
   MACRO_ADDR so each bump reloads it in one register. */

typedef struct {
    short start;   /* 0x0 */
    short end;     /* 0x2 */
    short text[6]; /* 0x4: string offsets, one per language */
} Subtitle;
typedef struct {
    char pad00[0x34];
    int time;      /* 0x34 */
    char pad38[0x14];
    char *subs;    /* 0x4C */
} SubState;
extern SubState D_0018CC20_s __asm__("D_0018CC20");
extern int D_0015EE88 MACRO_ADDR;
extern int D_0013E600[];
extern void InitializeDmaPacket(void *arg0, int a1, int a2, int a3, int a4, int a5,
                          int a6, int a7, int a8);
extern void func_001F7560_l(void *, long, char *, int) __asm__("FUN_001f7580");
extern void FUN_001f5f18(int, int, int, int, int);

/* Draws the subtitle showing at the current time (D_0018CC20+0x34): the
   list at +0x4C holds 16-byte entries (start, end, and one string offset
   into the list per language; a negative start ends it). The language
   D_0015EE88 picks the string (2..5 map to 1..4, anything else to 0). The
   text is measured in a FontSetWindow buffer (InitializeDmaPacket/7560), its
   box is kept 0x14 above the bottom of the screen (D_0013E600[1]), the
   frame is drawn (FUN_001f5f18) and the text printed with the measure
   flag (4) cleared. The list is tested and then read again for the
   loop, which gives retail's copy of it; the clamp test is written
   bottom-first, which gives retail's registers. */

extern char D_00160920[];
extern char D_00160930[];

/* Letterbox bars: while D_0015F544 is set the bar height D_0015F548
   grows to 24, otherwise it shrinks to 0. While it is non-zero, append
   a GIF packet (the D_00160920/D_00160930 register descriptors, PRIM
   0x104) drawing two full-width strips, the height in 16ths reaching in
   from the top and bottom of the D_0013E600 viewport, the same packet
   steps as FUN_001f52a0. */

typedef struct {
    char pad0[4];
    int color;
    long enable;
    int step0;
    int color0;
    long enable0;
    int step1;
    int color1;
    long enable1;
} Stripes;
extern short D_0015F450;             /* SDA, gp -0x78B0: Stripes * */
extern void FUN_001f52a0(int, int, int, int, unsigned long);

#define STRIPES (*(Stripes **)&D_0015F450)

/* Fills the screen in vertical stripes: an optional full-screen colour
   first, then alternating stripes of widths step0/step1 in colours
   color0/color1, each with its own GS register 0x42 blend word when set.
   Adapted from Lombyte (MIT) for PAL. */

#undef STRIPES

extern void FUN_001f52a0(int, int, int, int, unsigned long);
extern int D_0015EF88 MACRO_ADDR;
extern short D_00151880[];

/* GS register writes around an overlay: blend register 0x42 from the
   64-bit word at +8 while it is set, and when the colour at +4 has an
   alpha byte, register 0x4E switched around a full-screen
   FUN_001f52a0 draw. The 64-bit constants are ps2eeas's dli
   sequences (tools/ps2eeas_dli.py). */

/* Sets GS register 1 from four bytes packed into one 64-bit value, then
   restores the default register set. The parameters are int, widened in
   the expression: with long parameters the scheduler hoists the last
   dsll one slot early (it was a 6/144 near-miss that way). */
extern void FUN_00233980(int, long);
extern int *D_00161000 MACRO_ADDR;
extern char D_0013CD90[];


extern char D_00160920[];
extern char D_00160930[];

/* DrawRectOverlay: append a GIF packet drawing the rectangle x0..x1,
   y0..y1 (in 16ths, offset by the viewport origin D_0013E600[4]/[5]
   - 8) as a PRIM 0x144 sprite pair in colour rgba: the tag, the
   D_00160920 and D_00160930 register descriptors (ids 0x8001/0x8004),
   then four XYZ values at Z 0xFFFFF0. Each packet step has its own
   block-scoped base pointer, which gives retail's registers. */



extern char D_00160940[];

/* Append a textured sprite as a four-vertex strip (PRIM 0x154): screen
   corners in 12.4 fixed point relative to the viewport origin, UVs from
   (u, v) to (u + uw, v + vh) in 16ths. */

extern int func_001FA898_r(float) __asm__("FUN_001FA6D0");

/* FUN_001f5450 with a float screen rectangle: each corner is rounded
   (FUN_001FA6D0) from 16ths before the viewport offset. */







/* Draws a bevelled frame: the box itself, then three shrinking bars
   above and three below it, all in grey 0x040404 with alpha a. */

/* Emits a 9-point cross/star pattern of FUN_001f52a0 draws around
   (a0, a1, a2, a3), offset by +-1/3/5 along each axis, all sharing the
   colour/flags word a4. The first call passes a4 with only its top byte
   kept and the low nibble forced to 4; the rest pass a4 unchanged. */

/* gp-relative: declared as a 2-byte type purely so -G2 places it in the
   small-data area (placement is decided by DECLARED size), then accessed
   as the 4-byte word it really is. gp base 0x166D00 - 0x7764 = 0x15F59C. */
extern short D_0015F59C;



/* String width: sums the signed width byte (+3 of each 4-byte entry of
   `table`, indexed by character) over at most `count` characters of `str`,
   stopping at the NUL. Called by FUN_001f6250/20/40 with the three font
   tables. A do-while behind an entry test, with `p = str` set inside the
   if: the body's `*p` then sits after the loop label, so CSE keeps it
   apart from the entry test's `*str`, and `i = 0` is not folded into the
   first `i++`, both as in retail. */

extern int FUN_001f6200(unsigned char *arg0, int arg1, void *arg2);
extern unsigned char D_001DF3D0[];
extern unsigned char D_001DF770[];
extern unsigned char D_001DFB10[];




/*
 * Retail has 8 bytes of nop padding between func_001F6640 and
 * FUN_001f62b0, and it lives *after* `endlabel` in
 * asm/nonmatchings/text/func_001F6640.s -- so the asm stub was
 * supplying it, and replacing that stub with C silently dropped it,
 * shifting every later function in the segment by -8 and corrupting
 * their `jal` targets (FUN_001f7978 read 1/44 while being
 * instruction-for-instruction identical to retail). Emitted explicitly
 * to preserve the layout.
 *
 * Check for this whenever converting a stub: content after a .s file's
 * `endlabel` is inter-function padding the stub was carrying, and it has
 * to be reproduced or everything downstream drifts. Alignment directives
 * do not cover it -- both boundaries here are already 8-byte aligned.
 */

extern void FUN_001f62b0(void *, void *, void *, void *, void *, int,
                          unsigned char *);

/* Same shape as FUN_001f7580/FUN_001f75f0 below, one argument wider:
   mode 1 vs 2, D_001DF3D0 vs D_001DF770. Seven arguments, so EABI puts
   the fifth through seventh in $8/$9/$10. */
/* FontPrintLarge */

/* FontPrintSmall */







extern int FUN_001f6250(unsigned char *, int);
extern int func_001F6620(unsigned char *, int);

/* FUN_001f6940/FUN_001f69d0/FUN_001f6a60 are FUN_001f6530's family
   with a leading measure call: the same mode/table triple (1, 2, 3 and
   D_001DF3D0, D_001DF770, D_001DFB10), each paired with its own
   measuring helper, and the first argument stepped back by whatever that
   helper returns. */



/* FUN_001f6af0/FUN_001f6c20/func_001F6FD8 are the FUN_001f6940 triple
   centred instead of left-aligned: the step-back is half the measured
   value, and the adjusted position is returned. Typed all-int to match
   the extern func_001F7288 already declares for func_001F6FD8. */
/* FontPrintCenter */

/* FontPrintCenterSmall */

/* FontPrintCenterLarge */



extern int FUN_001f44b8(int);
extern void FUN_001f7090(void *, void *, void *, void *, int, unsigned char *);

/* FUN_001f7580 and FUN_001f75f0 are the same call with a different
   mode (1 vs 2) and a different table. Six arguments: EABI passes the
   fifth and sixth in $8/$9, which is why they appear alongside $4-$7
   rather than on the stack. */




/* FontSetWindow */



extern short D_0015F448;              /* SDA, gp -0x78B8 */
extern int D_0015F704 MACRO_ADDR;
extern unsigned short D_0010E800 NOT_SDA;
extern char D_0010E810[];
extern char D_00187180[];
extern void FUN_001f9ff8(float *, float);
extern void FUN_00233830(void *, int);
extern void func_00233C90(void);

/* Build the VU1 setup packet: load microprogram 7 if another is
   resident, then a DIRECT/UNPACK chain with the two camera matrices
   (each scaled to 1024 and biased in z by D_0015F448), the screen scale
   and offsets from D_0018CE00, and the GIF register setup; the DMA tag's
   qword count is patched in at the end. */

extern void FUN_001fb440(int, int, int);
extern short D_001519EE NOT_SDA;

/* Sets up a (1 << a) x (1 << b) area, as vendor.c's FUN_00239690 does:
   FUN_001fb440 gets the sizes and a base address, which is D_001519EE
   pages when `flag` is set, else D_0015EF8C less 4 << min(a + b, 16)
   bytes rounded down to a page (8 KB); FUN_001f33b8 gets the sizes and
   the float setup (f, 0, 524288, 255, 0); GS registers 0x47 (0 with the
   flag, 0x30000 without) and 0x42 are then written. Each arm makes its
   own page-aligned base, so the two trailing `<< 13`s are cross-jumped
   into the one retail has ahead of the argument moves. */


extern void FUN_001fb2d0(void);
extern void FUN_001f2c60(void);
extern void FUN_001f2d98(void);


/* Two prototypes for one symbol: FUN_001f5210 passes a 64-bit value
   (retail shifts it with dsll), func_001F79A8 passes plain ints
   (addiu, not daddiu). */
extern int D_0015F578 MACRO_ADDR;
extern short D_0015F448;              /* SDA, gp -0x78B8 */
extern void func_001F91B8(void);
extern void func_001F76A0(void);
extern void FUN_001f89a4(void);


extern int D_0018E840[];

extern __typeof__(FUN_001f4880) func_001F4880 __attribute__((alias("FUN_001f4880")));
