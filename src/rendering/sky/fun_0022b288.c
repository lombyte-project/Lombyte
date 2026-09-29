#include "types.h"
#include "eetypes.h"
#include "qcopy.h"
struct SF {
    u8 pad_0[4];
    u16 unk4;
    s16 unk6;
};
struct Mat { u8 pad[0x30]; u64 unk30; };

extern f32 D_00160404 __attribute__((sda));
extern struct SF *D_0016045C;
extern u8 D_00160460;
extern struct Mat D_001D96E0;
extern s32 D_0015EE88;
extern void func_001F99F8(f32 *);
extern void FUN_001f9a80(void *, void *, f32);
extern void FUN_001f9fc8(void *);
extern void FUN_001fa070(void *, f32 *);
extern f32 func_001FA580(f32, f32);
extern void func_0022B4C8(void);
extern void func_0022B558(void);
extern void func_0022B690(s32);
extern void func_00233980(s32, s64) ;

void FUN_0022b288(void) {
    f32 v[4];
    f32 s;
    u8 *m;
    s32 i;

    i = 0;
    func_0022B4C8();
    D_0016045C->unk4 = 0;
    FUN_001f9fc8(&D_001D96E0);
    func_001F99F8(v);
    if (D_0016045C->unk6 > 0) {
        do {
            s = 1.0f;
            switch (i) {
            case 0:
                *(s32 *)&v[1] = 0;
                v[2] = D_00160404;
            case 1:
                *(s32 *)&v[1] = 0;
                v[2] = func_001FA580(D_00160404, v[1]);
                s = 1.0f;
                break;
            case 2:
                v[1] = -0.075f;
                v[2] = func_001FA580(D_00160404, -0.15f);
                s = 1.25f;
                break;
            case 3:
                v[1] = 0.05f;
                v[2] = func_001FA580(D_00160404, 0.125f);
                s = 1.5f;
                break;
            case 4:
                v[1] = 0.1f;
                v[2] = func_001FA580(D_00160404, -0.05f);
                s = 1.75f;
                break;
            case 5:
                v[1] = -0.15f;
                v[2] = func_001FA580(D_00160404, 0.1f);
                s = 2.0f;
                break;
            }
            FUN_001fa070(&D_001D96E0, v);
            m = (u8 *)&D_001D96E0;
            FUN_001f9a80(m, m, s);
            FUN_001f9a80(m + 0x10, m + 0x10, s);
            FUN_001f9a80(m + 0x20, m + 0x20, s);
            m += 0x30;
            qcopy(m, &D_00160460);
            func_0022B690(i);
            i++;
        } while (i < D_0016045C->unk6);
    }
    func_0022B558();
    func_00233980(0x47, 0x5360B);
    func_00233980(0x4E, 0x1000000 | (D_0015EE88 >> 13));
}

extern __typeof__(FUN_0022b288) func_0022B288 __attribute__((alias("FUN_0022b288")));
