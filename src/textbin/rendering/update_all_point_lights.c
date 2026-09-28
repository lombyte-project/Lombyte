/* Ported from rac1-decomp, the PAL decompilation (src/game/lights.c, func_00202260). */
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
 * lights.cpp in the original source; text 0x202260-0x202AA8.
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

extern char D_00187098[];
extern char D_0019BDC0[];
extern char D_0019C1C0[];
extern char D_0019C3C0[];
extern float func_001FA580(float, float);
extern float func_001F9DC8(float);
extern float func_001F9DE0(float);
extern float FUN_001f9b48(void *, void *);
extern void FUN_00201ba8(int);
extern void FUN_00201f58(int);

/* Per-frame light update: the ambient colour (0.8 grey, -0.3) and a key
   light 0.8 rad behind the camera heading, tilted down; then every
   active point light (0x30-byte records at D_0019C3C0, positions from
   D_0019C1C0) that has moved more than 8 units is re-placed and either
   created (state 1 -> 2) or refreshed (state 2). */
void update_all_point_lights(void) __asm__("FUN_00201a28");

void update_all_point_lights(void) {
    char *l = D_0019BDC0;
    float ang;
    int i;

    *(float *)(l + 0x34C) = -0.3f;
    *(float *)(l + 0x340) = 0.8f;
    *(float *)(l + 0x344) = 0.8f;
    *(float *)(l + 0x348) = 0.8f;
    ang = func_001FA580(*(float *)D_00187098, -0.8f);
    *(float *)(l + 0x350) = func_001F9DC8(ang) * 0.866f;
    *(float *)(l + 0x354) = func_001F9DE0(ang) * 0.866f;
    *(float *)(l + 0x358) = -0.5f;
    *(int *)(l + 0x35C) = 0;
    for (i = 0; i < 8; i++) {
        char *src = D_0019C1C0 + i * 0x20;
        char *dst = D_0019C3C0 + i * 0x30;

        if (*(int *)(dst + 0x10) != 0 && FUN_001f9b48(src + 0x10, dst + 0x20) > 8.0f) {
            qcopy(dst + 0x20, src + 0x10);
            if (*(int *)(dst + 0x10) == 1) {
                FUN_00201ba8(i);
                *(int *)(dst + 0x10) = 2;
            } else if (*(int *)(dst + 0x10) == 2) {
                FUN_00201f58(i);
            }
        }
    }
}



extern void FUN_00201ba8(int);
extern void FUN_00201f88(int);

/* RefreshPointLight */
/* Retail aligns the next function to 16 bytes, and this function's .s
   stub carried one padding word to do it. Decompiling to C drops that
   padding, shifting every later function in the object by -4 and
   producing spurious `jal` diffs far from the cause -- so restore it
   explicitly. */
__asm__(".align 4");

struct LightRecord32C {
    u8 pad_0[0x1B];
    u8 unk1B;
    u8 pad_1C[0x2];
    u16 unk1E;
};

struct LightRecord64 {
    u8 pad_0[0x35];
    u8 unk35;
    u16 unk36;
};

struct LightRecord32A {
    u8 pad_0[0x1B];
    u8 unk1B;
    u8 pad_1C[0x2];
    u16 unk1E;
};

struct PointLightLinks {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s32 unkC;
};

extern u8 *D_001604D4 MACRO_ADDR;
extern u8 *D_00160F8C MACRO_ADDR;
extern u8 *D_00161050 MACRO_ADDR;

extern u8 D_001E7E80[];
extern u8 D_001E7EA0[];
extern u8 D_001E7EC8[];
extern s32 DebugPrint();

/* Remove one point light from three packed attachment lists. Each 16-bit
   reference stores up to four light IDs in nibbles; deleting one shifts
   the higher IDs down and marks the record free when all are 0xF. */

extern __typeof__(update_all_point_lights) func_00201A28 __attribute__((alias("FUN_00201a28")));
