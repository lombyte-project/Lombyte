#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00227548/FUN_00227548.s", FUN_00227548);
#else
#include "types.h"
#include "eetypes.h"
struct DmaTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct TagPtr { struct DmaTag *p; };
struct Rec { s32 count; s32 x4; f32 f8; f32 fC; u128 v10; };
extern struct TagPtr D_00160F00;
struct Flag { s32 v; };
extern struct Flag D_0015ED80;
extern u8 D_001D7EC0[];
extern u8 D_001D7E50[];
extern s32 D_001603A0;
extern void func_002271D0(void);
extern void func_00226FB8(f32 *, f32);
extern u8 *func_002270E8(u8 *);
extern void func_00228520(f32 *, f32 *);
extern void func_00227A08(u32, s32, s32);
extern void func_00227140(s32, s32, s32);
extern s32 func_00233980(s32, s64);
extern void func_00227378(s32);
void FUN_00227548(u8 *rec) {
    f32 a[4];
    f32 b[4];
    f32 c[4];
    struct Rec *r;
    s32 i;

    func_002271D0();
    r = (struct Rec *)rec;
    D_00160F00.p->w0 = 0x30000007;
    D_00160F00.p->addr = (u32)(D_0015ED80.v != 0 ? D_001D7EC0 : D_001D7E50);
    D_00160F00.p->w2 = 0x13000000;
    D_00160F00.p->w3 = 0x50000007;
    b[1] = b[0] = a[1] = a[0] = 0.0f;
    D_00160F00.p++;
    while (r->count != 0) {
        rec += 0x20;
        *(u128 *)c = r->v10;
        func_00226FB8(c, 1000.0f);
        a[2] = r->fC;
        b[2] = r->f8;
        for (i = 0; i < r->count; i++) {
            rec = func_002270E8(rec);
            func_00228520(a, b);
            func_00227A08(0x70000000, D_001603A0, r->x4);
            func_00227140(2, 1, 2);
            func_00227140(1, 0, 2);
        }
        r = (struct Rec *)rec;
    }
    if (D_0015ED80.v) {
        func_00233980(0x4C, 0x80080);
    } else {
        func_00233980(0x4C, 0x80070);
    }
    func_00233980(0x42, 0x2000000064LL);
    func_00227378(0);
    func_00233980(0x47, 0x5360B);
    func_00233980(0x42, 0x8000000044LL);
}
#endif /* NON_MATCHING */
