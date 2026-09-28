/* Ported from rac1-decomp, the PAL decompilation (src/game/camera.c, func_001ED658). */
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
 * camera.cpp in the original source; text 0x1EC038-0x1EDFF8.
 * Name and boundary from the NTSC split in bordplate's RC1 project
 * (codeberg.org/bordplate/RC1), mapped to PAL by matching function
 * sizes -- see docs/DECOMP_PROGRESS.md. Compiled as C for now.
 */

/* Declarations in scope here before the split. */
extern char D_0013E650[];
extern int D_0015F694;
extern int func_001E9448(void *arg0);

/* Same signature as the declaration further down this file; duplicate
   identical declarations are legal and avoid a signature clash. */
extern void FUN_001f98d0(void *, void *, int);
extern char D_00189310[];
extern char D_001899D0[];
extern void *D_001871C0 NOT_SDA;

/* BackupCurrentCam */

extern int D_0015F08C MACRO_ADDR;
extern void (*D_001893B0[])(void);

/* ExecuteCamPostUpdFuncs: runs the D_0015F08C queued post-update
   callbacks, then empties the queue. */
/* D_0015F08C as MACRO_ADDR removes the hoisted address register (frame
   0x30); a plain indexed loop. */

/* Not a standalone function: no `jr $31` -- dead-value computation
   (`$v0 = 0` twice with intervening nops) then a store, falling through
   to whatever follows. Same fallthrough-fragment category as
   func_00113AD8 in core_text. */


extern float func_001F99C0(float);

/* Cam_InterpValues(a, b, p, c, d, e): steps *p toward b - a, clamps it
   to +-e and to +-func_001F99C0(b - a), and returns a + *p. The nop
   between the first compare and its bc1f is ps2eeas's
   (tools/ps2eeas_nops.py). */

/* Not a standalone function: single `addiu $sp,$sp,0x50`, no `jr $31` --
   fallthrough fragment, same category as func_00113AD8 in core_text. */


extern char D_001871D0[];
extern void FUN_0020c828(void *); /* DeleteMoby */

/* Camera_handleCollWithHero: spawns (via func_001E9448) or deletes the
   moby kept at D_001871D0+0xC4, depending on the flag at arg0+0x86. */
/* A `char *c` base, the `== 0` arm first, and DeleteMoby takes the slot
   as its argument. */

/* 0x14-byte dispatch records, indexed by the type id at +0x8C.
   Declared as a real struct array, not `char[]` + byte offset: the two
   forms are not codegen-equivalent here. Retail emits `addu $2,$2,$3`
   (base, index); a char-pointer form emits `addu $2,$3,$2` (index,
   base) and no amount of reordering the C addition changes it, because
   GCC canonicalises the PLUS before operand order is chosen. Indexing
   a typed array puts the base first. See FUN_001ebec8/FUN_001ec3d8. */
typedef struct {
    char unk_00[8];
    void (*fn_08)(void *);
    char unk_0C[4];
    void (*fn_10)(void *);
} DispatchRec;
extern DispatchRec D_001E8F80[];

/*
 * 1/68, and the residual is one commutative-operand-order byte: retail
 * emits `addu $2,$2,$3` (base + index), this compiler `addu $2,$3,$2`
 * (index + base). Same instruction, same destination, same size.
 *
 * Getting here took two real fixes worth reusing. Writing the field read
 * as `... * 0x14 + 8` folds the +8 into the %lo address constant instead
 * of leaving it as a `lw` offset (7/68); computing the record pointer
 * first and reading `rec + 8` separately fixes that. And building the
 * pointer with `rec += idx` rather than in the initialiser makes the sum
 * land in the base's register as retail does, rather than the index's
 * (3/68 -> 1/68) -- the documented in-place-accumulate lever.
 *
 * The last byte resisted an explicit index local and both `rec += idx`
 * and `rec = rec + idx`, which is the known scratch-register/operand
 * choice question. Kept per the same-size-tiny-diff precedent.
 */
/* Camera_runSetupToNewCam(UpdateCam *) */



typedef struct {
    char unk_00[0x10];
    int unk10;
    char unk14[8];
    int unk1C;
} CamRec20;
extern CamRec20 *D_0015F090 MACRO_ADDR;
extern CamRec20 *D_0015F040 MACRO_ADDR;
extern char D_0013F450[];
extern char D_0013F4D0[];
extern int FUN_00214720(void *arg0, int arg1);
typedef struct {
    char unk_00[4];
    int (*fn_04)(void *, void *);
    char unk_08[0xC];
} CamPrioHook;
extern CamPrioHook D_001E8F80_prio[] __asm__("D_001E8F80");

/* Camera_ActivationCheckPriority(cur, other): whether camera `cur` should
   take over from `other`. An inactive camera (+0x7C) never does; the camera
   type's +4 hook (D_001E8F80) may decide first (-1 no, 1 yes); otherwise
   the mode at +0x74 decides: 0 (and 1/2 once +0x7D is set) by priority
   byte, 4 through FUN_00214720 on the target record, 7 by the level's
   mode and target. Each case ends in `if (x) return 1;` falling out to the
   one shared `return 0;`, which is what lets cross-jumping and reorg give
   retail's branches; case 7's final test shares its `return 1` with the
   g < 0 exit so its `$v0 = 1` is not hoisted above the load. */

/* Same shape/blocker as FUN_001ebec8: indirect call via a function
   pointer loaded from a per-type dispatch table, wrapped in an
   sq-for-lone-$ra save this compiler doesn't reproduce (see
   FUN_001e9ab8's comment). Not attempted. */
/* Same vtable dispatch as FUN_001ebec8, on the +0x10 slot instead of
   +8; identical 1/68 operand-order residual, same cause. */
/* Camera_Exit(UpdateCam *) */

extern int D_00189C50[];
typedef struct { char unk_00[0xA0]; } CamSlot;
extern CamSlot D_00187510[];
extern int FUN_001ec210(void *cur, void *other);
extern void func_001EBF10(void *arg0);
/* The camera-type table (D_001E8F80) with its +0xC hook typed: DispatchRec
   above keeps that slot as bytes. */
typedef struct {
    char unk_00[0xC];
    void (*fn_0C)(void *);
    char unk_10[4];
} CamTypeHooks;
extern CamTypeHooks D_001E8F80_hooks[] __asm__("D_001E8F80");

/* Camera_ActivationCheck: exits the current camera (FUN_001ec3d8, the
   type table's +0x10 hook), then scans the 48 camera slots (D_00189C50[i]
   != 0 = enabled, D_00187510[i] = the 0xA0-byte record) and keeps
   whichever FUN_001ec210 prefers over the current one. If that changed
   the camera, func_001EBF10 switches to it. Then FUN_001ebe68, the new
   camera's +0xC hook (read before that call, as retail does), a copy of
   its fields 0x30-0x38 to 0x64-0x6C, and the post-update queue
   (FUN_001ebcf0). Returns -1, which the one caller ignores.

   The indexed loop is what gives retail's preheader: strength reduction
   builds the two slot pointers in its order, and loop reversal makes the
   count run down 47..0. Indexing the table directly by a typed +0xC
   member puts the base first in the addu. */

extern void FUN_001f9a28(void *dst, void *a, void *b);      /* dst = a - b (vector) */
extern float func_001F9AB0(void *a, void *b);                 /* dot(a, b) */
extern float FUN_001f9af0(void *a);                           /* |a| */
extern void FUN_001f9bf8(void *dst, void *src, float len);    /* dst = normalize(src) * len */
extern float func_001F9DF8(float x);                            /* approx acos(x) */
extern void FUN_00214890(void *dst, void *vec, void *axis, float angle); /* dst = vec rotated `angle` around axis */

/* The camera's angles to a target: out[0] = signed yaw between dir0 and
   (p0 - p1) off the axis, out[1] = signed pitch after rotating dir0 by
   that yaw, out[2] = |p0 - p1|. A zero length becomes 0.0001 before the
   acos. The two sign fixups are shaped differently, as in retail. */

extern void FUN_001ec530(float *out, void *p0, void *p1, void *dir0, void *dir1,
                           void *axis);
extern char D_001871B0[];

/* Builds three unit vectors from D_0013F450's +0x2080 pointer table
   (+0xC0/+0xD0/+0xE0 offsets, re-read at each call as retail does),
   stashes two of them into D_001871B0's record (+0x90, +0xA0), calls
   FUN_001ec530 to compute the camera's yaw/pitch/dist into +0x70,
   then copies +0xD0 back over +0xB0 (retail's qcopy, see common.h). */

extern void FUN_001f9a10(void *, void *, void *);
extern char D_0013F590[];

/* When the flag at +2 is clear, copies the 16-byte vector at +0x50 to
   +0xC0 with retail's qcopy, optionally runs FUN_001f9a10 on +0x60,
   then copies +0x60 to +0xD0. Sibling of FUN_001ec868 just above it. */

extern char D_001871B0[];

/* When the flag at +2 is set, copies the 16-byte vectors at +0xC0 and
   +0xD0 back to +0x50 and +0x60 with retail's qcopy (see common.h). */

/* The final mode store goes through this alias: written through
   D_001871B0 itself, gcse keeps the entry's %hi alive in a saved register
   across the calls, where retail rebuilds it. */
extern short D_001872B0_h __asm__("D_001871B0") NOT_SDA;
extern void func_002144D8(void *, void *);
extern int FUN_001f96f8(int);
extern float func_001FA6C0(int);

/* Camera mode update from arg (its +0x30 vector is copied). In mode 1
   the sub-mode at +3 picks what is captured from it: 0 the +0x50/+0x60
   pair, 2 the +0xC0/+0xD0 pair and FUN_001ec710's angles, otherwise
   D_0013F450's axes and FUN_001ec530's yaw/pitch/dist into +0x70. Other
   modes restore through FUN_001ec7f0/FUN_001ec868. Then the mode
   becomes 3, +2 takes the sub-mode, and either the +0x10 blend resets or
   the +0x70 timer advances. */

extern char D_00187180[];
extern char D_0018C418[];
extern char D_00187290[];
extern float D_0015EE60 MACRO_ADDR;
extern float FUN_002133d0(float, float, float);
extern void func_002144D8(void *, void *);
extern void FUN_001fa400(void *, void *, void *, float);
extern void FUN_001fa4f8(void *, void *);
extern void func_001FA2B8(void *, void *);

/* Camera blend step toward to: while the position (cam[3]) or rotation
   (cam[0]) blend hasn't reached 1, move cam+0x40 from cam+0x30 toward
   to's position by the eased factor (FUN_002133d0), copy it to
   D_00187180 unless the D_0018C418 flag is set, slerp the rotation
   (FUN_001fa400) into cam+0x50 and load it as the view matrix, then
   advance both blends by their rates times D_0015EE60, capped at 1.
   Returns 1 once both are complete. */



extern int D_0018C32C NOT_SDA;
extern char D_00187290[];
extern int func_001ECAF8(void *, void *);
extern int FUN_001eccd8(void *, void *);

/* Picks the update path by the flag at D_001871B0+2 (func_001ECAF8 when
   clear, FUN_001eccd8 when set), each passed arg0 and a slot inside
   D_001871B0. On success, unless D_0018C32C is set, copies four 16-byte
   vectors from arg0 into D_00187290's block (retail's qcopy); the last
   destination is -0x210 from D_00187290, a different member reached by
   pointer arithmetic on the same char array. Either way it clears
   D_001871B0's leading halfword and its flag byte. */
void FUN_001ed2b0(char *arg0) {
    char *base = D_001871B0;
    int result;

    if (*(unsigned char *)(base + 2) == 0) {
        result = func_001ECAF8(arg0, base + 0x10);
    } else {
        result = FUN_001eccd8(arg0, base + 0x70);
    }
    if (result != 0) {
        if (D_0018C32C == 0) {
            qcopy(D_00187290, arg0);
            qcopy(D_00187290 + 0x10, arg0 + 0x10);
            qcopy(D_00187290 + 0x20, arg0 + 0x20);
            qcopy(D_00187290 - 0x210, arg0 + 0x30);
        }
        *(short *)base = 0;
        base[2] = 0;
    }
}

extern void func_001F9740(int *arg0);
extern float func_001FA6C0(int arg0);
extern float FUN_001fa610(float x);
extern float func_001F9DC8(float x);
extern void FUN_001f9bf8(void *dst, void *src, float len);
extern void FUN_001f9a10(void *dst, void *a, void *b);
extern char D_001873B0[];
extern char D_00187290[];
extern char D_00187180[];

/* Camera shake update. arg0: +0 amplitude, +4 sample (out), +8 countdown,
   +0xC longest countdown. With the active camera (D_001871C0) in state 6
   both counters are cleared; with the countdown at 0 only +0xC is.
   Otherwise the countdown is decremented (func_001F9740, saturating) and
   sample = amplitude * cos(wrap(2 * count)) * (count / longest)^2
   (func_001FA6C0 int->float, FUN_001fa610 wrap to [-pi, pi],
   func_001F9DC8 cos) scales one of two directions (arg1) into the shake
   offset D_00187180 (FUN_001f9bf8 scale, FUN_001f9a10 add). The
   countdown-zero exit is the else arm after the main path (retail's
   `b L800` over it), and the state-6 stores are written 8 then 0xC so
   the scheduler emits 0xC first and cross-jumping leaves them alone. */

extern char D_00187040[];
extern char D_00194220[];
extern int D_0015F6E8 MACRO_ADDR;
extern int FUN_001efa68(void *, void *, int, int, int);
extern int FUN_001f0b58(void);
extern float func_002135F0(void *, int);

/* Camera-inside-water test: cast a ray through the camera focus from
   0.75 above to 0.75 below (up to six hits); on the first hit that is
   not a water surface, flag D_00187040+0x394 when the focus is below the
   surface height + 0.04. Off for camera mode 6 or while D_0015F6E8 is
   set. */

extern int D_0015F09C MACRO_ADDR;
extern int D_0015F0A0 MACRO_ADDR;
extern int D_0015F098 MACRO_ADDR;

/* Picks the camera's draw modes: D_0015F09C is 0x14, or 0x34 in states
   0x11/0x12 or mode 0x73 of D_0013F450, back to 0x14 when its +0x2F0
   height is below D_00187180+8 (except in state 0x11); D_0015F0A0 keeps
   the value before bit 0x80 is set. D_0015F098 is 0xB4 ORed with the +0xC0 mode of
   D_001871D0, which the first set flag of D_0013F450's 0x12E5, 0x12EB,
   0x12E6, 0x12EC, 0x12E4 overrides. Each arm ORs the new mode in itself
   (the constants fold per arm, as in retail), and each block reads
   D_0013F450 through its own local (%hi kept, %lo rebuilt). */

extern char D_00187040[];
extern float D_0015F53C MACRO_ADDR;

/* D_0015F53C is a MACRO_ADDR float: $gp-relative in the delay slots,
   lui $1 in the body. */

extern int D_001E6700;
extern char D_001871A0[];
extern unsigned char D_0015EEB4_m[4] __asm__("D_0015EEB4") MACRO_ADDR;
extern void FUN_001eda60(void);
extern void func_001ED940(void);
extern void FUN_001ed470(void);
extern void func_001EC8A0(void *);
extern void FUN_001fa298(void *, void *);
extern void FUN_00214598(void *, void *);
extern void FUN_001ee4b0(void *);
extern void func_001F9AD8(void *, void *, void *);

/* Camera update, once per frame: count the frame, run the camera
   steps, then (unless the D_0018C418 freeze flag is set) take the view
   from the target object (mode 3 blends it through FUN_001ed2b0) and
   refresh its Euler angles; run FUN_001ed360 on the two vectors at
   D_001871A0, func_001EDB98 and FUN_001ee4b0, and, with
   D_0015EEB4 set, the matrix's third row as a cross product. */

extern __typeof__(FUN_001ed2b0) func_001ED2B0 __attribute__((alias("FUN_001ed2b0")));
