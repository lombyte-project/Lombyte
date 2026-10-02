#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/audio/music/music_update/FUN_00216290.s", FUN_00216290);
#else
#include "types.h"
#include "rnc/d_001516d0.h"

extern struct M2c_D_001516D0 D_001516D0;
extern u8 D_00151704[];
extern void func_0012E4C0(s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_00216B68();
extern void func_0012ECA0(s32);
extern s32 func_0012EE08(s32);
extern s32 func_001F96F8(s32);
extern s32 func_001F9740(void *);
extern void func_00215970(s32, s32, s32);
extern void func_00215B68(s32, s32, s32);
extern void func_00215C40(s32, s32, s32);
extern void func_00215D18(s32, s32, s32);
extern s32 func_00215E00(s32, s32, s32, s32);
extern void func_002160A8(void *);
extern s32 func_00216788(s32, s32, s32);

void music_update(void) __asm__("FUN_00216290");

void music_update(void) {
    struct M2c_D_001516D0 *m;
    s8 song;
    s32 n;
    s32 id;
    s16 st;

    if (D_001516D0.unkB != 0) {
        return;
    }
    if (!(D_001516D0.unk40 & 0x8000) && !(D_001516D0.unk3E & 0x8000)) {
        if (D_001516D0.unk34 == 0 && (D_001516D0.unk3C & 1) && D_001516D0.unk22 == -1) {
            func_00215C40(D_001516D0.unk38, (s16)D_001516D0.unk3C, D_001516D0.unk3A);
        } else if (D_001516D0.unk3E != 9 && D_001516D0.unk34 != 0) {
            if (D_001516D0.unk34 != 0xFFFFFFFF && D_001516D0.unk3E == 8) {
                func_00215D18(D_001516D0.unk38, D_001516D0.unk3C, D_001516D0.unk3A);
            }
        }
    }
    if (func_001F9740(&D_001516D0.unk2C) != 0) {
        song = D_001516D0.unk22;
        if (song != -1 && D_001516D0.unk20 == 0) {
            if (D_001516D0.unk38 != song) {
                if (D_001516D0.unk23 == -1 || func_00215E00(song, D_001516D0.unk23, D_001516D0.unk3C, D_001516D0.unk3A) != 0) {
                    D_001516D0.unk2C = func_001F96F8(7) * 60.0f;
                }
            } else {
                D_001516D0.unk22 = -1;
            }
        }
    }
    if (D_001516D0.unk1C >= 0) {
        if (D_001516D0.unk50 != 0) {
            if ((u16)D_001516D0.unk5A - 6 >= 2U) {
                D_001516D0.unk5A = 5;
            }
        } else {
            func_00215970(D_001516D0.unk1C, 0, 0x400);
            D_001516D0.unk1C = -1;
        }
    }
    if (D_001516D0.unk3E != 9 && D_001516D0.unk34 != 0xFFFFFFFF && !(D_001516D0.unk40 & 0x8000) && !(D_001516D0.unk3E & 0x8000)
        && !(D_001516D0.unk78 & 0x8000) && !(D_001516D0.unk76 & 0x8000)) {
        switch (D_001516D0.unk20) {
        case 2:
            n = D_001516D0.unk3A * (D_001516D0.unk28 - (D_001516D0.unk24 - D_001516D0.unk84)) / D_001516D0.unk28;
            if (n <= 0 || ((D_001516D0.unk76 != 4 || D_001516D0.unk7C == 0) && D_001516D0.unk6C == 0) || (id = D_001516D0.unk34) == 0) {
                D_001516D0.unk20 = 3;
                D_001516D0.unk3E = 5;
            } else if (D_001516D0.unk3E != 9) {
                *(u32 *)&D_001516D0.unk34 = 0xFFFFFFFF;
                func_0012E4C0(id, 5, n, 0, 0, 0, (s32)func_00216B68, (s32)&D_001516D0.unk34);
            }
            break;
        case 3:
            if (D_001516D0.unk3E == 0 && (D_001516D0.unk7C != 0 || D_001516D0.unk6C == 0)) {
                func_00215B68(D_001516D0.unk22, D_001516D0.unk3C, D_001516D0.unk3A);
                D_001516D0.unk22 = -1;
                D_001516D0.unk20 = 4;
            }
            break;
        case 4:
            if (D_001516D0.unk3E == 3) {
                if (D_001516D0.unk84 < D_001516D0.unk28 || D_001516D0.unk76 != 4 || (D_001516D0.unk7C == 0 && D_001516D0.unk6C == 0)) {
                    func_0012ECA0(D_001516D0.unk34);
                    D_001516D0.unk3E = 8;
                    D_001516D0.unk20 = 5;
                }
            }
            break;
        case 5:
            if (D_001516D0.unk76 != 4 || D_001516D0.unk7C == 0) {
                D_001516D0.unk20 = 0;
            }
            break;
        }
    }
    func_002160A8(&D_001516D0.unk34);
    func_002160A8(&D_001516D0.unk6C);
    func_002160A8(&D_001516D0.unk50);
    if (D_001516D0.unkA != 0) {
        if (func_0012EE08(1) == 0) {
            D_001516D0.unk8 = 0;
            D_001516D0.unkA = 0;
        }
    } else {
        st = D_001516D0.unk8;
        if (st == 2) {
            D_001516D0.unk8 = 0;
            func_00216788(D_001516D0.unk14, D_001516D0.unkC, D_001516D0.unk10);
            if (D_001516D0.unk8 == 0) {
                D_001516D0.unk8 = st;
            }
        }
    }
}
#endif /* NON_MATCHING */
