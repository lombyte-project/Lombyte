#include "rnc/gameplay_state_do_space_transition_types.h"
#include "sda.h"

extern struct M2c_D_0013D290 D_0013D290;
extern struct M2c_D_0013DD40 D_0013DD40;
extern u8 D_0013DD58[];
extern struct M2c_D_0013E030 D_0013E030;
extern struct M2c_D_001516D0 D_001516D0;
extern s32 D_0015ED5C MACRO_ADDR;
extern s32 D_0015ED84 MACRO_ADDR;
extern s32 D_0015ED88;
extern s16 D_0015EE48 MACRO_ADDR;
extern s16 D_0015EE4A MACRO_ADDR;
extern s32 D_0015F438 MACRO_ADDR;
extern s32 D_0015F600 MACRO_ADDR;
extern s32 D_0015F604;
extern s32 D_0015F618 MACRO_ADDR;
extern struct M2c_D_0015F634 *D_0015F634;
extern struct M2c_D_0018CD00 D_0018CD00;
extern struct M2c_D_00194100 D_00194100;
extern u8 D_001E8988[];

extern void DebugPrint();
extern void FlushCache(s32);
extern void PackDmaTag(u64, u64, u64);
extern void func_0012DC80(void);
extern void func_0012E1A8(void);
extern void FUN_0012e1d8(s32) __asm__("FUN_0012e1d8");
extern void func_0012EB00(void);
extern s32 func_0012EE08(s32);
extern s32 FUN_0012ef28(s32) __asm__("FUN_0012ef28");
extern void func_0012EF68(s32, s32, s32, s32, s32);
extern s32 func_0012F368(s32);
extern void func_001F4A58(s32);
extern s32 FUN_001f96f8(s32);
extern void func_001FB2D0(void);
extern void func_001FB3D0(void);
extern void func_001FB6E0(void);
extern void func_002043B0(void);
extern s32 func_00204428(void);
extern void func_00208840(void);
extern void func_002093D8(void);
extern void func_00215EE8(void);
extern void func_00217A10(void);
extern void FUN_00226e08(void) __asm__("FUN_00226e08");
extern void func_0022DCD0(void);
extern void func_0022F778(void);
extern void func_00230EE8(void);
extern void func_00230F60(void);
extern void func_00231608(s32);
extern void FUN_00231bd8(s32, s32, s32, s32, s32);
extern void func_00233630(void);
extern void func_002336A0(void);
extern void func_002337B0(s32);
extern void func_00233D90(void);
extern s32 sceCdSync(s32);
extern s32 sceGsSyncV(s32);

void do_space_transition(void) __asm__("FUN_00231ff0");

void do_space_transition(void)
{
    s32 lvl;
    s32 ok;
    s32 done;

    lvl = D_0015ED88 - 1;
    if (lvl < 0) {
        lvl = 0;
    }
    D_00194100.unk10 |= 0x80000000;
    D_0015F604 = 6;
    D_0013E030.unk26 = 0;
    if (D_0013DD40.unk8 != 0 || D_0015F600 >= 8) {
        D_0013E030.unk26 = 1;
    }
    if (D_0013DD40.unkE != 0 || D_0015F600 >= 14) {
        D_0013E030.unk26 = 2;
    }
    FlushCache(0);
    func_0012EF68(2, 0, 0, 0, 0);
    func_0012EB00();
    func_0012DC80();
    func_0022DCD0();
    func_00215EE8();
    D_001516D0.unkB = 1;
    if (D_0015F634 != 0) {
        FUN_0012e1d8(D_0015F634->unk1C);
        func_0012E1A8();
        DebugPrint(D_001E8988, D_0015F634->unk1C);
    }
    D_0015ED5C = 0;
    FUN_0012ef28(0);
    func_0012EE08(0);
    D_0018CD00.unk238 = 16;
    D_0018CD00.unk21C = 524288.0f;
    D_0018CD00.unk228 = 255.0f;
    D_0018CD00.unk230 = 0;
    D_0018CD00.unk234 = 0;
    D_0018CD00.unk218 = 0;
    D_0018CD00.unk22C = 128.0f;
    PackDmaTag(0, 0, 0);
    if (D_0015F600 < 0) {
        while (D_0013D290.unkD4 >= 3 || D_0013D290.unkDC >= 0) {
            func_002093D8();
            func_00208840();
        }
        func_001F4A58(FUN_001f96f8(6));
        D_0015ED84 = D_0015F600;
        func_002043B0();
        sceCdSync(0);
        D_001516D0.unkB = 0;
        func_00233D90();
        return;
    }
    if (D_0015F600 == 0 && D_0013DD58[0] == 0) {
        func_001F4A58(FUN_001f96f8(6));
        FUN_00231bd8(lvl, 0, 1, FUN_001f96f8(240), 0);
        func_00231608(0);
        FUN_00231bd8(lvl, 2, 2, FUN_001f96f8(180), 0);
        func_00231608(1);
        D_0015ED84 = D_0015F600;
        FUN_00231bd8(lvl, 3, 4, FUN_001f96f8(240), 1);
        func_00231608(2);
    } else if (D_0015ED84 == 0 && D_0015F600 == 1 && D_0013DD58[1] == 0) {
        func_001F4A58(FUN_001f96f8(6));
        FUN_00231bd8(lvl, 5, 6, FUN_001f96f8(240), 0);
        func_00231608(3);
        func_00231608(4);
        FUN_00231bd8(lvl, 7, 7, FUN_001f96f8(180), 0);
        func_00231608(5);
        D_0013DD58[D_0015ED84] = 2;
        D_0015ED84 = D_0015F600;
        FUN_00231bd8(lvl, 8, 8, FUN_001f96f8(240), 1);
    } else {
        if (D_0015F600 == 4 && D_0013DD58[4] == 0) {
            func_001F4A58(FUN_001f96f8(12));
            FUN_00231bd8(lvl, 9, 10, FUN_001f96f8(240), 0);
            func_00231608(6);
        }
        if (D_0015ED84 == 7 && D_0013DD58[7] != 2 && D_0013DD40.unk8 != 0) {
            func_001F4A58(FUN_001f96f8(12));
            FUN_00231bd8(lvl, 11, 11, FUN_001f96f8(240), 0);
            func_00231608(7);
        }
        if (D_0015F600 == 13 && D_0013DD58[13] == 0) {
            func_001F4A58(FUN_001f96f8(12));
            FUN_00231bd8(lvl, 12, 13, FUN_001f96f8(240), 0);
            func_00231608(8);
        }
        if (D_0015ED84 == 14 && D_0013DD58[14] != 2 && D_0013DD40.unkF != 0) {
            func_001F4A58(FUN_001f96f8(12));
            FUN_00231bd8(lvl, 14, 14, FUN_001f96f8(240), 0);
            func_00231608(9);
        }
        if (D_0015F600 == 16 && D_0013DD58[16] == 0) {
            func_001F4A58(FUN_001f96f8(12));
            FUN_00231bd8(lvl, 15, 16, FUN_001f96f8(240), 0);
            func_00231608(10);
        }
        if ((u32)D_0015ED84 < 19) {
            ok = 1;
            if (D_0015ED84 == 7 && D_0013DD40.unk8 == 0) {
                ok = 0;
            }
            if (D_0015ED84 == 14 && D_0013DD40.unkF == 0) {
                ok = 0;
            }
            if (ok) {
                D_0013DD58[D_0015ED84] = 2;
            }
        }
        D_0015EE4A = 1;
        done = 0;
        D_0015ED84 = D_0015F600;
        D_0015EE48 = 0;
        func_00230F60();
        func_0012F368(D_0015ED84);
        while (D_0015F618 == 0) {
            func_002336A0();
            func_00233630();
            func_001FB3D0();
            func_001FB6E0();
            func_001FB2D0();
            func_00217A10();
            func_0022F778();
            func_00230EE8();
            func_002093D8();
            func_00208840();
            func_002337B0(1);
            sceGsSyncV(0);
            D_0015F438++;
            FUN_00226e08();
            if (!done) {
                done = func_00204428();
            }
        }
        if (!done) {
            do {
                FlushCache(0);
                sceGsSyncV(0);
                func_002093D8();
                func_00208840();
                FUN_00226e08();
            } while (func_00204428() == 0);
        }
        while (D_0013D290.unkD4 != 2 || D_0013D290.unkDC >= 0) {
            FlushCache(0);
            sceGsSyncV(0);
            func_002093D8();
            func_00208840();
            FUN_00226e08();
        }
    }
    sceCdSync(0);
    D_001516D0.unkB = 0;
    func_00233D90();
}

extern __typeof__(do_space_transition) func_00231FF0 __attribute__((alias("FUN_00231ff0")));
