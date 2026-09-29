#include "types.h"
#include "sda.h"

typedef struct {
    u8 pad_0[4];
    s16 unk4;
} SpriteFile;

typedef struct {
    u8 pad_0[8];
    s32 unk8;
    s32 pad_C;
    s32 unk10;
    s32 pad_14;
    s32 unk18;
    s32 pad_1C;
    s32 unk20;
    s32 pad_24;
    s32 unk28;
    s32 pad_2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
} DrawFlags;

extern u8 D_00100AE0[];
extern u16 D_0010FA90[];
extern u8 D_0010FAA0[];
extern s32 D_0015ED80 MACRO_ADDR;
extern u8 D_0015EE40;
extern s32 D_0015F34C __attribute__((sda));
extern s32 D_0015F350 __attribute__((sda));
extern s32 D_0015F370[2] __attribute__((sda));
extern u8 D_0015F380[];
extern u8 D_0015F390[];
extern u8 D_0015F3A0[];
extern u8 D_0015F3B0[];
extern u8 D_0015F3C0[];
extern u8 D_0015F3D0[];
extern u8 D_0015F3E0[];
extern u8 D_0015F3F0[];
extern u8 D_0015F3F8[];
extern u8 D_0015F400[];
extern u8 D_0015F410[];
extern u8 D_0015F418[];
extern u8 D_0015F428[];
extern s32 D_0015F434 MACRO_ADDR;
extern f32 D_0015F43C MACRO_ADDR;
extern f32 D_0015F440 MACRO_ADDR;
extern s32 D_0015F464 __attribute__((sda));
extern s32 D_0015F468 __attribute__((sda));
extern s32 D_0015F46C __attribute__((sda));
extern s32 D_0015F470 __attribute__((sda));
extern s32 D_0015F604 MACRO_ADDR;
extern f32 D_0015F614 MACRO_ADDR;
extern s32 D_0015F620 MACRO_ADDR;
extern s32 D_0015F648 MACRO_ADDR;
extern SpriteFile *D_0016045C;
extern u8 D_001610C0 MACRO_ADDR;
extern u8 D_001610C1 MACRO_ADDR;
extern u8 D_001610C2 MACRO_ADDR;
extern u8 D_001610C3 MACRO_ADDR;
extern s32 D_001872D4[];
extern DrawFlags D_0018A2B0;
extern s32 D_0018C34C[];
extern u8 D_001D8EB0[];
extern u8 D_001E1300[];
extern u8 D_001E3200[];
extern u8 D_001E78A0[];
extern u8 D_001E78B8[];

extern void AppendDmaTag(u32);
extern void FlushCache(s32);
extern void func_001E9AB8(void);
extern void func_001EDC50(void);
extern void FUN_001ee338(void);
extern void func_001F21B0(void *, s32);
extern void func_001F21B8(void *, s32);
extern void func_001F2260(void);
extern void func_001F2588(void);
extern void FUN_001f2c10(void);
extern void func_001F3868(void);
extern void func_001F4280(s32);
extern void func_001F4398(void);
extern void func_001F4650(void);
extern void func_001F46C8(void);
extern void func_001F4740(void);
extern void func_001F4808(void);
extern void func_001F4880(void);
extern void func_001F4BE0(void);
extern void func_001F4D98(void);
extern void func_001F4FB8(void);
extern void func_001F5138(void *);
extern void func_001F5210(s32, s32, s32, s32);
extern void func_001F79A8(void);
extern void FUN_001f92b0(void);
extern f32 func_001FA6C0(s32);
extern s32 func_001FA6D0(f32);
extern void func_001FB368(void);
extern void func_001FB680(void);
extern void func_001FE980(void);
extern void func_001FF780(void);
extern void FUN_0020cc60(void);
extern void func_0020CEF8(void);
extern void func_0020D460(void);
extern void func_00217C18(void);
extern void func_00228A30(void);
extern void func_00228B38(void);
extern void FUN_0022a5e0(void *);
extern void func_00233308(void);
extern void func_002333A8(void);
extern void func_002334D8(void *);
extern void func_002337B0(s32);
extern void func_00233830(void *, s32);
extern void func_00233980(s32, u64);
extern void func_00233BC8(void);
extern void func_00233C28(void);
extern void FUN_00234f98(void *);
extern void FUN_00235780(void);
extern void FUN_00235840(void);
extern void FUN_00235898(void);
extern void func_002358C8(void);
extern void func_00235990(void);
extern void FUN_00237370(void *);
extern void func_00237A70(void);

void draw_debug_profiler(void) __asm__("FUN_001f39d0");

void draw_debug_profiler(void) {
    f32 t;
    f32 div;

    if (D_0016045C == NULL || D_0016045C->unk4 != 0 || ((D_0015F434 ^ 1) & 1) || D_0018A2B0.unk8 == 0 ||
        D_0018C34C[0] != 0) {
        func_001FB368();
    }
    func_001F2260();
    FUN_0020cc60();
    FUN_001f2c10();
    func_001F3868();
    D_0015F620 = -1;
    func_001F21B0(D_0015F380, 0xF);
    func_001F21B8(D_0015F380, 0xF);
    if (D_0016045C != NULL && (D_0015F434 & 1)) {
        if (D_0018A2B0.unk8 != 0) {
            func_001E9AB8();
        }
        func_001F21B8(D_0015F390, 0xE);
        func_001F21B0(D_0015F390, 0xE);
    }
    if (D_0015F434 & 2) {
        func_002333A8();
    }
    AppendDmaTag(0x02010000);
    if (D_0015F434 & 4) {
        if (D_0015ED80 != 0) {
            func_00235990();
        } else {
            func_002358C8();
        }
    }
    AppendDmaTag(0x02020000);
    if (D_0018A2B0.unk34 != 0 && D_0015F46C != 0) {
        func_00233C28();
        func_001F4280(1);
        func_001F46C8();
        func_001F4398();
        func_00233BC8();
    }
    func_001F21B0(D_0015F3A0, 6);
    func_001F21B8(D_0015F3A0, 6);
    if (D_0015F434 & 8) {
        func_00228B38();
    }
    AppendDmaTag(0x02040000);
    if (D_0018A2B0.unk44 != 0 && D_0015F370[0] != 0) {
        func_001F5138(D_0015F370);
    }
    if (D_0015F434 & 0x20) {
        func_001F4280(1);
        func_001F79A8();
        func_001F4398();
    }
    if (D_0018A2B0.unk34 != 0 && D_0015F470 != 0) {
        func_00233C28();
        func_001F4280(1);
        func_001F4740();
        func_001F4398();
        func_00233BC8();
    }
    if (D_0015F434 & 0x10) {
        func_0020D460();
    }
    AppendDmaTag(0x02080000);
    func_001F4280(0);
    if ((D_0015F434 & 0x20) && D_0018A2B0.unk30 != 0 && D_0015F620 != 6) {
        func_00233830(D_0010FAA0, D_0010FA90[0]);
        D_0015F620 = 6;
    }
    func_001F21B8(D_0015F3B0, 4);
    func_001F21B0(D_0015F3B0, 4);
    if (D_0015F434 & 0x20) {
        func_00233C28();
        if (D_0018A2B0.unk34 != 0) {
            if (D_0015F464 != 0) {
                func_001F4650();
            }
            func_00233980(0x42, 0x8000000048);
            func_001EDC50();
            func_00233C28();
            func_001F4880();
        }
        func_001F21B0(D_0015F3C0, 6);
        func_001F21B8(D_0015F3C0, 6);
        if (D_0018A2B0.unk38 != 0) {
            func_00233980(8, 5);
            func_00233C28();
            FlushCache(0);
            func_00217C18();
            D_0015F620 = 8;
        }
        func_001F21B0(D_0015F3D0, 8);
        func_001F21B8(D_0015F3D0, 8);
        if (D_0018A2B0.unk3C != 0) {
            if (D_0015F468 != 0) {
                func_00233C28();
                func_001F4808();
            }
            if (D_0015F604 == 0) {
                FUN_001f92b0();
            }
            func_00233980(0x42, 0x8000000044);
            FUN_001ee338();
        }
        func_001F21B0(D_0015F3E0, 6);
        func_001F21B8(D_0015F3E0, 6);
    }
    if (D_0018A2B0.unk48 != 0) {
        func_001FB680();
    }
    func_001F21B8(D_0015F3F0, 0xF);
    func_00233C28();
    if (D_0015F434 & 0x10000) {
        func_00237A70();
    }
    if ((D_0015F434 & 0x80) && D_0018A2B0.unk40 != 0) {
        func_001FF780();
        func_001FE980();
        func_001F4D98();
    }
    if (D_0015F604 == 2 && D_0015EE40 != 0) {
        func_001F4BE0();
    }
    func_001F21B8(D_0015F3F8, 0xE);
    func_001F21B0(D_0015F3F8, 0xE);
    func_001F4398();
    if (D_0015F434 & 0x40) {
        if (D_0018A2B0.unk44 != 0) {
            func_00233980(0x42, 0x8000000044);
            if (D_001872D4[0] != 0) {
                func_001F5210(D_001610C0, D_001610C1, D_001610C2, D_001610C3);
            }
            if (D_0015F43C > 0.0f) {
                if (D_0015F43C > 1.0f) {
                    D_0015F43C = 1.0f;
                }
                func_001F5210(0, 0, 0, func_001FA6D0(D_0015F43C * 128.0f));
            }
            if (D_0015F440 > 0.0f) {
                if (D_0015F440 > 1.0f) {
                    D_0015F440 = 1.0f;
                }
                func_001F5210(0xFF, 0xFF, 0xFF, func_001FA6D0(D_0015F440 * 128.0f));
            }
            if (D_0015F34C != 0 && D_0015F350 != 0) {
                func_001F4FB8();
            }
        }
        func_001F21B8(D_0015F400, 0xA);
    }
    func_002334D8(D_00100AE0);
    FlushCache(0);
    t = func_001FA6C0(*(volatile s32 *)0x10000800);
    D_0015F614 = t / (D_0015ED80 != 0 ? 11520.0f : 9600.0f);
    func_002337B0(2);
    func_001F21B0(D_0015F410, 0x11);
    if (D_0015F434 & 2) {
        if (D_0018A2B0.unk10 != 0) {
            FUN_00234f98(D_001E1300);
            func_00233308();
        }
        func_001F21B0(D_001E78A0, 2);
    }
    func_002337B0(4);
    func_001F21B0(D_0015F410, 0x11);
    if (D_0015F434 & 4) {
        if (D_0018A2B0.unk18 != 0) {
            if (D_0015ED80 != 0) {
                FUN_00235898();
                FUN_00235840();
            } else {
                FUN_00237370(D_001E3200);
                FUN_00235780();
            }
        }
        func_001F21B0(D_0015F418, 5);
    }
    func_002337B0(8);
    func_001F21B0(D_0015F410, 0x11);
    if (D_0015F434 & 8) {
        if (D_0018A2B0.unk20 != 0) {
            FUN_0022a5e0(D_001D8EB0);
            func_00228A30();
        }
        func_001F21B0(D_001E78B8, 7);
    }
    func_002337B0(0x10);
    func_001F21B0(D_0015F410, 0x11);
    if (D_0015F434 & 0x10) {
        if (D_0018A2B0.unk28 != 0) {
            func_0020CEF8();
        }
        func_001F21B0(D_0015F428, 3);
    }
    func_001F2588();
    D_0015F648 = 0;
}

extern __typeof__(draw_debug_profiler) func_001F39D0 __attribute__((alias("FUN_001f39d0")));
