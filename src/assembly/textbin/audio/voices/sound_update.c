#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/audio/voices/sound_update/FUN_0022ca50.s", FUN_0022ca50);
#else
#include "types.h"
#include "eetypes.h"
#include "qcopy.h"
#include "qzero.h"

typedef union {
    u128 q;
    f32 f[4];
} Vec4;

typedef struct {
    u8 pad00[0x18];
    u8 unk18;
    u8 unk19;
    s16 unk1A;
    s32 unk1C;
} SoundDef;

typedef struct {
    u8 pad00[0x10];
    Vec4 pos;
    u8 unk20;
    u8 pad21[0x9F];
    Vec4 unkC0;
} SoundMoby;

typedef struct {
    u32 handle;      /* 0x00 */
    u8 state;        /* 0x04 */
    u8 flags;        /* 0x05 */
    u8 pad06[2];
    SoundDef *def;   /* 0x08 */
    s32 pad0C;
    s32 volume;      /* 0x10 */
    s32 unk14;       /* 0x14 */
    SoundMoby *moby; /* 0x18 */
    s32 unk1C;       /* 0x1C */
    Vec4 pos;        /* 0x20 */
    Vec4 unk30;      /* 0x30 */
    u32 hist_pos;    /* 0x40 */
    u8 hist[36];     /* 0x44 */
    u8 pad68[8];
} Voice;

typedef struct {
    Vec4 cam_hist[4];   /* 0x000 */
    s32 cam_pos;        /* 0x040 */
    s32 pad44;
    s32 vol48;          /* 0x048 */
    s32 vol4C;
    s32 vol50;
    s32 vol54;
    s32 vol58;
    s32 vol5C;
    s32 pad60;
    s32 unk64;          /* 0x064 */
    u8 unk68;
    u8 unk69;
    u8 unk6A;
    u8 unk6B;
    s32 unk6C;          /* 0x06C */
    Voice voices[30];   /* 0x070 */
    s32 padD90[2];
    s32 unkD98;         /* 0xD98 */
    s32 padD9C;
    Vec4 unkDA0[6];     /* 0xDA0 */
} SoundState;

extern SoundState D_0013E550;
extern f32 D_0013F640[];
extern f32 D_0015ED6C;
extern s32 D_0015F5E8;
extern s32 D_0015F604;
extern s32 D_0015F60C;
extern Vec4 D_00187080;
extern u8 D_00187290[];
extern s32 D_001872D4;

extern s32 ComputeByteStringHash(u8 *, s32);
extern int ComputeSectorIndex(int arg0);
extern void FillTransferWords(void *, s32, s32);
extern void ReadGlobalTableEntry(void);
extern void func_0012DC80() __asm__("FUN_0012dc80");
extern void func_0012E208() __asm__("FUN_0012e208");
extern void func_0012E308() __asm__("FUN_0012e308");
extern void func_0012E368() __asm__("FUN_0012e368");
extern void func_0012E448() __asm__("FUN_0012e448");
extern void func_0012E4C0() __asm__("FUN_0012e4c0");
extern void func_0012EB00() __asm__("FUN_0012eb00");
extern void func_0012EF68() __asm__("FUN_0012ef68");
extern void func_0012EFE0() __asm__("FUN_0012efe0");
extern void func_001F9A10(void *out, void *a, void *b) __asm__("FUN_001f9a10");
extern void func_001F9A28(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern void func_001F9A68(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern f32 func_001F9AB0(void *a, void *b) __asm__("FUN_001f9ab0");
extern f32 func_001F9AF0(void *a) __asm__("FUN_001f9af0");
extern void func_001F9BF8(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void func_001F9CF8(void *out, void *a, void *b) __asm__("FUN_001f9cf8");
extern void func_001FA2D8(void *out, void *a) __asm__("FUN_001fa2d8");
extern f32 ConvertIntegerToFloat(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void func_00216290() __asm__("FUN_00216290");
extern void func_0022C5A8(void *) __asm__("FUN_0022c5a8");
extern u8 func_0022C658(Voice *, void *) __asm__("FUN_0022c658");
extern s32 func_0022C7E8(Voice *, Vec4 *) __asm__("FUN_0022c7e8");
extern s32 func_0022C830(Voice *, Vec4 *, void *) __asm__("FUN_0022c830");
extern void FUN_0022dd90();
extern void FUN_0022ddd8();

s32 sound_update(void) __asm__("FUN_0022ca50");

s32 sound_update(void) {
    Vec4 occl;
    Vec4 vel;
    Vec4 delta;
    s32 flags[30];
    s32 vols[30];
    f32 dots[30];
    Vec4 dir;
    Vec4 mat[4];
    s32 i;
    s32 underwater;
    f32 *pdots;
    Vec4 *pvel;
    Vec4 *pmat;
    s32 *pflags;
    s32 *pvols;
    SoundState *s;
    SoundMoby *moby;
    Vec4 *rel;
    Vec4 *d;
    f32 water_z;
    s32 cur;
    s32 next;
    s32 n;
    s32 vol;
    s32 j;
    s32 k;
    s32 m;
    s32 dead;
    s32 ratio;
    s32 count;
    s32 mode;
    s32 pan;
    s32 pitch;
    s32 prio;
    s32 handle;
    s32 f;

    water_z = 0.0f;
    underwater = D_001872D4;
    if (underwater != 0) {
        water_z = D_0013F640[0];
    }
    func_0012DC80();

    if (D_0013E550.unk6B & 8) {
        func_0012EF68(2, 0, 0, 0, 0);
    } else if (D_0013E550.unk6B & 0x13) {
        func_0012EF68(2, D_0013E550.unk68, D_0013E550.unk64, D_0013E550.unk69, D_0013E550.unk6A);
    } else if (D_0013E550.unk6B & 4) {
        func_0012EFE0(2, D_0013E550.unk64, 0xC, 3);
    }
    D_0013E550.unk6B = 0;

    if ((D_0015F604 == 0 || D_0015F604 == 2) && D_0015F5E8 == 0) {
        func_0022C5A8(&occl);
    }

    qzero(&vel);
    s = &D_0013E550;
    cur = (s->cam_pos + 1) % 4;
    s->cam_pos = cur;
    qcopy(&s->cam_hist[cur], &D_00187080);

    pvel = &vel;
    pflags = flags;
    pdots = dots;
    pmat = mat;
    pvols = vols;
    n = 0;
    next = (cur + 3) % 4;
    if (next != cur) {
        do {
            func_001F9A28(&delta, &s->cam_hist[cur], &s->cam_hist[next]);
            if (!(func_001F9AF0(&delta) < D_0015ED6C * 60.0f)) {
                break;
            }
            n++;
            func_001F9A10(pvel, pvel, &delta);
            cur = next;
            next = (cur + 3) % 4;
        } while (next != s->cam_pos);
    }
    if (n >= 2) {
        func_001F9A68(pvel, pvel, 1.0f / ConvertIntegerToFloat(n));
    }

    if (D_0015F604 == 2) {
        func_0012E208(1, 0);
        func_0012E208(0, D_0013E550.vol48 / 2);
        func_0012E208(3, D_0013E550.vol54 / 2);
    } else {
        vol = D_0013E550.vol4C;
        if (underwater) {
            vol = vol * 3 / 5;
        }
        func_0012E208(1, vol);
        func_0012E208(0, D_0013E550.vol48);
        func_0012E208(3, D_0013E550.vol54);
    }
    func_0012E208(2, D_0013E550.vol50);
    func_0012E208(4, D_0013E550.vol58);
    func_0012E208(5, D_0013E550.vol5C);

    FillTransferWords(pflags, 0, 0x78);
    FillTransferWords(pvols, 0, 0x78);
    FillTransferWords(pdots, 0, 0x78);

    for (i = 0; i < 30; i++) {
        if (D_0013E550.voices[i].state != 7) {
            if (D_0013E550.voices[i].handle == 0) {
                continue;
            }
            if (D_0013E550.voices[i].handle == -1) {
                continue;
            }
        }
        moby = D_0013E550.voices[i].moby;
        dead = 0;
        if (moby != NULL && (moby->unk20 == 0xFE || moby->unk20 == 0xFD)) {
            dead = 1;
        }
        if (dead) {
            D_0013E550.voices[i].moby = NULL;
        }
        if (D_0013E550.voices[i].state == 4 || (D_0013E550.voices[i].state != 6 && dead && D_0013E550.voices[i].def->unk18 != 0)) {
            pflags[i] = 0x20;
            continue;
        }
        if (D_0015F604 != 0 && D_0015F604 != 2 && D_0015F604 != 6 && D_0013E550.voices[i].state != 7) {
            pflags[i] = 0x10;
            continue;
        }

        if (D_0013E550.voices[i].moby != NULL && !(D_0013E550.voices[i].flags & 8)) {
            if (D_0013E550.voices[i].flags & 0x40) {
                func_001F9CF8(&dir, &D_0013E550.voices[i].unk30, &moby->unkC0);
                func_001F9A10(&dir, &dir, &moby->pos);
                func_001F9A28(&delta, &dir, &D_0013E550.voices[i].pos);
                qcopy(&D_0013E550.voices[i].pos, &dir);
            } else {
                D_0013E550.voices[i].pos.f[2] -= 1.0f;
                func_001F9A28(&delta, &D_0013E550.voices[i].moby->pos, &D_0013E550.voices[i].pos);
                qcopy(&D_0013E550.voices[i].pos, &D_0013E550.voices[i].moby->pos);
                D_0013E550.voices[i].pos.f[2] += 1.0f;
            }
        } else {
            qzero(&delta);
        }
        rel = &delta;
        d = &dir;
        func_001F9A28(rel, rel, pvel);
        func_001F9A28(d, &D_00187080, &D_0013E550.voices[i].pos);
        func_001F9BF8(d, d, 1.0f);
        pdots[i] = func_001F9AB0(d, rel);

        if (!(D_0013E550.voices[i].flags & 0x10)) {
            ratio = func_0022C7E8(&D_0013E550.voices[i], &D_0013E550.voices[i].pos);
            pvols[i] = ratio * D_0013E550.voices[i].volume / 1024;
            if (!(D_0013E550.voices[i].def->unk19 & 2)) {
                pflags[i] |= 8;
            }
        } else {
            ratio = 0x400;
            pvols[i] = D_0013E550.voices[i].volume;
        }
        if (underwater && !(D_0013E550.voices[i].def->unk19 & 4) && water_z < D_0013E550.voices[i].pos.f[2]) {
            pvols[i] /= 2;
        }
        pflags[i] |= 1;
        if (ratio < 0x20 && pvols[i] < 0x20 && (D_0013E550.voices[i].flags & 4)) {
            pflags[i] = 0x20;
            continue;
        }
        if (!(D_0013E550.voices[i].flags & 1)) {
            pflags[i] |= 2;
            if (!(D_0013E550.voices[i].flags & 0x20)) {
                pflags[i] |= 4;
            }
        }
    }

    for (i = 0; i < 30; i++) {
        if (!(pflags[i] & 8)) {
            continue;
        }
        if (D_0013E550.voices[i].state == 7) {
            if ((D_0015F604 != 0 && D_0015F604 != 2) || D_0015F5E8 != 0) {
                continue;
            }
            if (D_0015F60C != D_0013E550.unkD98) {
                for (j = 0; j < 6; j++) {
                    func_0022C5A8(&D_0013E550.unkDA0[j]);
                }
                D_0013E550.unkD98 = D_0015F60C;
            }
            count = 0;
            for (j = 0, k = 0; j < 36; j += 6, k++) {
                D_0013E550.voices[i].hist[j] = func_0022C658(&D_0013E550.voices[i], &D_0013E550.unkDA0[k]);
                for (m = 1; m < 6; m++) {
                    D_0013E550.voices[i].hist[j + m] = D_0013E550.voices[i].hist[j];
                }
                if (D_0013E550.voices[i].hist[j]) {
                    count += 6;
                }
            }
            if (count >= 36) {
                pvols[i] = 0;
            } else if (count >= 19) {
                pvols[i] = (36 - count) * pvols[i] / 18;
            }
        } else {
            if ((D_0015F604 == 0 || D_0015F604 == 2) && D_0015F5E8 == 0) {
                D_0013E550.voices[i].hist_pos = (D_0013E550.voices[i].hist_pos + 1) % 36;
                f = (i ^ D_0015F60C) & 1;
                if (!(D_0013E550.voices[i].flags & 4)) {
                    f = ((D_0015F60C ^ i) & 3) == 0;
                }
                if (f) {
                    D_0013E550.voices[i].hist[D_0013E550.voices[i].hist_pos] = func_0022C658(&D_0013E550.voices[i], &occl);
                } else {
                    D_0013E550.voices[i].hist[D_0013E550.voices[i].hist_pos] = D_0013E550.voices[i].hist[(D_0013E550.voices[i].hist_pos + 35) % 36];
                }
            }
            count = ComputeByteStringHash(D_0013E550.voices[i].hist, 36);
            if (count >= 36) {
                pvols[i] = 0;
            } else if (count >= 19) {
                pvols[i] = (36 - count) * pvols[i] / 18;
            }
        }
    }

    func_001FA2D8(pmat, D_00187290);
    for (i = 0; i < 30; i++) {
        f = pflags[i];
        if (f == 0) {
            continue;
        }
        handle = D_0013E550.voices[i].handle;
        D_0013E550.voices[i].handle = -1;
        if (f & 0x20) {
            if (D_0013E550.voices[i].state == 7) {
                D_0013E550.voices[i].state = 0;
                D_0013E550.voices[i].moby = NULL;
                D_0013E550.voices[i].unk1C = 0;
            } else {
                func_0012E368(handle);
                D_0013E550.voices[i].state = 6;
                func_0012E448(handle, FUN_0022ddd8, &D_0013E550.voices[i]);
            }
        } else if (f & 0x10) {
            func_0012E448(handle, FUN_0022ddd8, &D_0013E550.voices[i]);
        } else {
            prio = D_0013E550.voices[i].unk14;
            mode = prio ? 0x11 : 1;
            pan = 0;
            pitch = 0;
            if (f & 2) {
                pan = func_0022C830(&D_0013E550.voices[i], &D_0013E550.voices[i].pos, pmat);
                mode |= 6;
            }
            if (pflags[i] & 4) {
                mode |= 8;
                pitch = ComputeSectorIndex(truncate_float_to_s32(pdots[i] * 300.0f));
            }
            if (underwater && !(D_0013E550.voices[i].def->unk19 & 8)) {
                mode |= 8;
                pitch -= 0x5F4;
            }
            if (D_0013E550.voices[i].state != 7) {
                func_0012E4C0(handle, mode, pvols[i], pan, pitch, prio, FUN_0022ddd8, &D_0013E550.voices[i]);
            } else {
                D_0013E550.voices[i].state = 1;
                func_0012E308(D_0013E550.voices[i].def->unk1C, D_0013E550.voices[i].def->unk1A, pvols[i], pan, pitch, prio, FUN_0022dd90, &D_0013E550.voices[i]);
            }
        }
    }

    func_00216290();
    func_0012EB00();
    func_0012DC80();
    ReadGlobalTableEntry();
    D_0013E550.unk6C = 0;
    return 0;
}
#endif /* NON_MATCHING */
