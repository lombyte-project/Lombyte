#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00237a78/FUN_00237a78.s", FUN_00237a78);
#else
#include "types.h"
#include "eetypes.h"
struct Screen { u8 pad[8]; s32 offx; s32 offy; };
struct Display { u8 pad[0x190]; f32 sx; f32 sy; };
extern struct Screen D_0013E500;
extern struct Display D_0018CD00;
extern u8 D_00187080[];
extern void func_001F9A28(void *, void *, void *);
extern void func_001F9A68(void *, void *, f32);
extern void func_001F9D20(void *, void *, void *);
extern s32 func_001FA6D0(f32);

void FUN_00237a78(f32 *a, f32 *b, s32 *o2, s32 *o3, s32 *o0, s32 *o1) {
    f32 u[4];
    f32 v[4];
    f32 *pv = v;

    *(u128 *)u = *(u128 *)a;
    *(u128 *)v = *(u128 *)b;
    func_001F9A28(u, u, D_00187080);
    func_001F9A28(pv, pv, D_00187080);
    func_001F9A68(u, u, 1024.0f);
    func_001F9A68(pv, pv, 1024.0f);
    pv[3] = 1.0f;
    u[3] = 1.0f;
    func_001F9D20(u, u, D_00187080 - 0x40);
    func_001F9D20(pv, pv, D_00187080 - 0x40);
    u[0] *= 1.0f / u[3];
    u[1] *= 1.0f / u[3];
    v[0] *= 1.0f / pv[3];
    pv[1] *= 1.0f / pv[3];
    u[0] *= D_0018CD00.sx;
    u[1] *= D_0018CD00.sy;
    v[0] *= D_0018CD00.sx;
    pv[1] *= D_0018CD00.sy;
    *o0 = func_001FA6D0(u[0] * 0.25f + (f32)D_0013E500.offx);
    *o1 = func_001FA6D0(u[1] * 0.25f + (f32)D_0013E500.offy);
    *o2 = func_001FA6D0((v[0] - u[0]) * 0.25f);
    *o3 = func_001FA6D0((pv[1] - u[1]) * 0.25f);
}
#endif /* NON_MATCHING */
