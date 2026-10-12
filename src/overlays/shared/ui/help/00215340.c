/* FUN_L00_00215340 split out of 00214658.c so it builds with the SN
   compiler on its own. Hero pose table offsets (D_L00_0017A680) are kept
   as byte offsets. */
#include "types.h"
#include "rnc/gameplay/hero.h"
typedef struct { f32 f[0x300]; } T_215340;
extern T_215340 D_L00_0017A680;
extern f32 D_0015ED6C;
extern void FUN_L00_0020a1e0(f32, f32);
extern void FUN_L00_0020a1f8(f32, f32);
extern void FUN_L00_0020a210(f32, f32);
extern void FUN_L00_0020a228(f32, f32);
extern f32 FUN_001f99c0(f32);
extern f32 FUN_001f9e90(f32, f32);
extern f32 FUN_001fa5c8(f32, f32);
extern f32 FUN_001f9de0(f32);
extern f32 FUN_001f9dc8(f32);
extern f32 FUN_001fa580(f32, f32);

#define TF(o) (D_L00_0017A680.f[(o) / 4])

void FUN_L00_00215340(void) {
    f32 v, nv, k, a, s, ang;
    f32 w, nw;
    f32 b, c1, c2, c3;
    f32 x;

    if (hero.state.current == HERO_STATE_RUN) {
        FUN_L00_0020a1e0(0.04f, 0.2f);
        FUN_L00_0020a1f8(0.04f, 0.2f);
        FUN_L00_0020a210(0.04f, 0.2f);
        FUN_L00_0020a228(0.02f, 0.2f);
        if (hero.state.step == 1) {
            v = hero.motion.unk188;
            if (1.4f < v) v = 1.4f;
            else if (v < -1.4f) v = -1.4f;
            nv = -v;
            TF(0x60) = nv * 0.14f;
            k = hero.motion.unk164 / (D_0015ED6C * 5.7f);
            if (1.0f < k) k = 1.0f;
            if (k < 0.0f) k = 0.0f;
            TF(0x60) = nv * 0.14f * k;
            TF(0x118) = v * 0.16f;
            TF(0x1C0) = v * -0.12f;
            TF(0x278) = v * 0.65f;
            TF(0x274) = FUN_001f99c0(v) * -0.15f;
            TF(0x270) = FUN_001f99c0(v) * 0.16f;
            TF(0x958) = nv * 0.35f;
            TF(0xA08) = nv * 0.3f;
            TF(0xAB8) = nv * 0.3f;
            TF(0xB68) = nv * 0.3f;
            TF(0x954) = FUN_001f99c0(v) * 0.35f;
            TF(0xA04) = FUN_001f99c0(v) * 0.2f;
        } else {
            w = hero.motion.unk188 * 0.5f;
            if (0.8f < w) w = 0.8f;
            else if (w < -0.8f) w = -0.8f;
            w = w * FUN_001f99c0(w) * 1.25f;
            TF(0x118) = w;
            TF(0x278) = w;
            a = FUN_001f99c0(w);
            nw = -w;
            TF(0x274) = a * 0.25f;
            TF(0x958) = nw * 0.35f;
            TF(0xA08) = nw * 0.3f;
            TF(0xAB8) = nw * 0.3f;
            TF(0xB68) = nw * 0.3f;
        }
    } else if (hero.state.current == HERO_STATE_LAND || hero.state.current == 0x81) {
        FUN_L00_0020a1e0(0.008f, 0.08f);
        FUN_L00_0020a1f8(0.015f, 0.08f);
        FUN_L00_0020a210(0.015f, 0.08f);
        FUN_L00_0020a228(0.008f, 0.1f);
        b = hero.motion.unk188 * 1.6f;
        if (1.25f < b) b = 1.25f;
        else if (b < -1.25f) b = -1.25f;
        if (hero.state.current == 0x81) {
            b = hero.motion.unk188 * 0.8f;
            if (1.1f < b) b = 1.1f;
            else if (b < -1.1f) b = -1.1f;
        }
        c1 = -b * 0.52f;
        TF(0x60) = c1;
        if (0.28f < c1) TF(0x60) = 0.28f;
        else if (c1 < -0.28f) TF(0x60) = -0.28f;
        c2 = b * 1.2f;
        TF(0x278) = c2;
        TF(0x118) = b * 0.58f;
        TF(0x1C0) = b * -0.28f;
        if (0.6f < c2) TF(0x278) = 0.6f;
        else if (c2 < -0.6f) TF(0x278) = -0.6f;
        c3 = FUN_001f99c0(b) * 0.2f;
        TF(0x274) = c3;
        if (0.2f < c3) TF(0x274) = 0.2f;
        else if (c3 < -0.2f) TF(0x274) = -0.2f;
        TF(0x270) = FUN_001f99c0(b) * 0.16f;
        if (hero.state.current == 0x81) {
            if (D_0015ED6C * 0.5f < hero.motion.unk164) {
                ang = FUN_001fa5c8(FUN_001f9e90(hero.motion.displacement.f[0], hero.motion.displacement.f[1]), hero.motion.rot.f[2]);
                s = hero.motion.unk164 * 3.0f;
                TF(0x60) = -FUN_001f9de0(ang) * s;
                TF(0x64) = FUN_001f9dc8(ang) * s;
                if (0.43633232f < TF(0x60)) TF(0x60) = 0.43633232f;
                else if (TF(0x60) < -0.43633232f) TF(0x60) = -0.43633232f;
                if (0.43633232f < TF(0x64)) TF(0x64) = 0.43633232f;
                else if (TF(0x64) < -0.43633232f) TF(0x64) = -0.43633232f;
            }
            TF(0x64) = FUN_001fa580(TF(0x64), -0.12217305f);
        }
        TF(0x958) = -b * 0.35f;
        TF(0xA08) = -b * 0.3f;
        TF(0xAB8) = -b * 0.3f;
        TF(0xB68) = -b * 0.3f;
    } else if (hero.state.control_mode == 2 || hero.state.control_mode == 4) {
        FUN_L00_0020a1e0(0.04f, 0.2f);
        FUN_L00_0020a1f8(0.04f, 0.2f);
        FUN_L00_0020a210(0.04f, 0.2f);
        FUN_L00_0020a228(0.02f, 0.2f);
        x = hero.motion.unk188;
        if (1.4f < x) x = 1.4f;
        else if (x < -1.4f) x = -1.4f;
        TF(0x118) = x * 0.29f;
        TF(0x1C0) = x * -0.27f;
        TF(0x278) = x * 0.87f;
        TF(0x958) = -x * 0.35f;
        TF(0xA08) = -x * 0.3f;
        TF(0xAB8) = -x * 0.3f;
        TF(0xB68) = -x * 0.3f;
    }
}
