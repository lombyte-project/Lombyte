#include "types.h"
#define W(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define L(p, o) (*(s64 *)((u8 *)(p) + (o)))
extern u8 D_00153A10[];
extern void D_0012CB30();
extern void D_0012CB40();
extern void *memset(void *, s32, u32);
extern void _Error(void *, u8 *);
extern void InitializeStateRecord(void *, void *, s32);
extern s32 reserve_aligned_buffer_space(void *, void *, s32, s32) __asm__("func_0012BC20");
extern void func_0012BC00(void *);
extern void _clearOnce(void *);
extern s32 sceMpegReset(void *);
extern s32 ClearMpegReferenceBuffer(void *);

s32 sceMpegCreate(void *mp, u8 *work, s32 size) {
    u8 *d;
    u8 *state;
    s32 rest;
    s32 r;

    memset(work, 0, size);
    d = (u8 *)((((u32)work + 3) >> 2) << 2);
    rest = size - (d - work);
    if ((u32)rest < 0x10C0) {
        _Error(d, D_00153A10);
        return 0;
    }
    state = d + 0x108;
    W(mp, 0x40) = (s32)d;
    InitializeStateRecord(state, d + 0x10C0, rest - 0x10C0);
    W(mp, 0x0) = 0;
    W(mp, 0x4) = 0;
    W(mp, 0x8) = 0;
    L(mp, 0x10) = -1;
    L(mp, 0x18) = -1;
    L(mp, 0x20) = 0;
    L(mp, 0x28) = -1;
    L(mp, 0x30) = -1;
    L(mp, 0x38) = 0;
    W(d, 0xB4) = 0;
    W(d, 0xB8) = 0;
    W(d, 0xBC) = 0;
    W(d, 0xC0) = 0;
    W(d, 0xC4) = 0;
    W(d, 0xC8) = 0;
    W(d, 0xCC) = 0;
    W(d, 0xD0) = 0;
    W(d, 0xD4) = 0;
    W(d, 0xD8) = 0;
    W(d, 0xDC) = 0;
    W(d, 0xE0) = 0;
    W(d, 0xE4) = 0;
    W(d, 0xE8) = 0;
    W(d, 0xF8) = 0;
    W(d, 0xC) = 0;
    W(d, 0x14) = 0;
    W(d, 0x2C) = 0;
    W(d, 0x34) = 0;
    W(d, 0x3C) = 0;
    L(d, 0xF0) = -1;
    W(d, 0x1C) = (s32)D_0012CB30;
    W(d, 0x24) = (s32)D_0012CB40;
    r = reserve_aligned_buffer_space(d, state, 0x600, 8);
    W(d, 0x48) = 0;
    W(d, 0xFC) = 0;
    W(d, 0x100) = 0;
    W(d, 0x104) = 0;
    W(d, 0x70) = 0;
    L(d, 0x78) = 0;
    W(d, 0x80) = -1;
    L(d, 0x88) = 0;
    W(d, 0x90) = 0;
    W(d, 0xAC) = 0;
    W(d, 0x94) = -1;
    W(d, 0x98) = -1;
    W(d, 0x9C) = -1;
    W(d, 0x858) = (s32)mp;
    W(d, 0x44) = r;
    W(d, 0xB0) = 1;
    _clearOnce(d);
    sceMpegReset(mp);
    ClearMpegReferenceBuffer(mp);
    W(d, 0x1B8) = (s32)(d + 0x1E8);
    W(d, 0x1BC) = (s32)(d + 0x250);
    W(d, 0x1C4) = (s32)(d + 0x2B8);
    W(d, 0x1C8) = (s32)(d + 0x320);
    W(d, 0x1CC) = (s32)(d + 0x388);
    W(d, 0x1D4) = (s32)(d + 0x3F0);
    W(d, 0x1D8) = (s32)(d + 0x458);
    W(d, 0x1DC) = (s32)(d + 0x4C0);
    W(d, 0x1E4) = (s32)(d + 0x528);
    func_0012BC00(state);
    W(d, 0x850) = -1;
    W(d, 0x84C) = 0;
    W(d, 0x81C) = 0x70003600;
    W(d, 0x854) = 0;
}
