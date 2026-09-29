#include "types.h"

typedef struct {
    s32 id;
    s32 type;
    u8 pad_8[0xC];
} MenuItem;

typedef struct {
    u8 pad_0[0x20];
    s32 unk20;
    u8 pad_24[0x1C];
    s32 unk40;
    u8 pad_44[0x14];
    s32 sel;
    u8 pad_5C[0x74];
    MenuItem items[1];
} Menu;

typedef struct {
    s32 text;
    u8 pad_4[0x48];
} TextEntry;

typedef struct {
    s32 a;
    s32 b;
    u16 c;
    u16 d;
    u8 pad_C[0xC];
} StatEntry;

typedef struct {
    u8 pad_0[0x23];
    u8 unk23;
} Opts;

extern Menu D_001E63C0;
extern TextEntry D_001863D0[];
extern StatEntry D_001DFFB0[];
extern Opts D_0013D4C0;

extern void func_001FB8F0(s32, s32, s32, s32, s32, s32, s32);
extern void func_0020D330(s32, s32);
extern char *func_001FDD10(s32);
extern void func_001F65B0(s32, s32, u64, char *, s32);
extern void func_00238688(char *, s32);
extern void func_001F69D0(s32, s32, u64, char *, s32);
extern s32 FUN_001f6250(char *, s32);
extern void func_00200E08(s32, s32, s32, s32, u64, s32);

void fun_002389e0(void) __asm__("FUN_002389e0");

void fun_002389e0(void) {
    char buf[0x100];
    s32 w;

    func_001FB8F0(0, 0, 0x200, 0x80, 0x200, 0x80, 0);
    func_0020D330(D_001E63C0.unk20 + 0x100, 1);
    func_0020D330(D_001E63C0.unk20, 1);
    if (D_001E63C0.items[D_001E63C0.sel].type == 1) {
        func_001F65B0(6, 8, 0x80F0F0F0, func_001FDD10(D_001863D0[D_001E63C0.items[D_001E63C0.sel].id].text), -1);
        func_001F65B0(0x18, 0x18, 0x80F0F0F0, func_001FDD10(0x4F5D), -1);
        func_00238688(buf, D_001DFFB0[D_001E63C0.items[D_001E63C0.sel].id].c);
        func_001F69D0(0x76, 0x65, 0x80F0F0F0, buf, -1);
        if (D_001E63C0.unk40 != 0) {
            w = FUN_001f6250(buf, -1);
            func_00200E08(0x75 - w, 0x6D, 0x7B, 0x70, 0x20959544, 0);
            func_00200E08(0x76 - w, 0x6D, 0x7A, 0x70, 0x30959544, 0);
            func_00200E08(0x77 - w, 0x6D, 0x79, 0x70, 0x40959544, 0);
            func_00200E08(0x78 - w, 0x6D, 0x78, 0x70, 0x50959544, 0);
            func_00200E08(0x79 - w, 0x6D, 0x77, 0x70, 0x60959544, 0);
            func_00200E08(0x7A - w, 0x6D, 0x76, 0x70, 0x70959544, 0);
            func_00200E08(0x7B - w, 0x6D, 0x75, 0x70, 0x80959544, 0);
            func_00238688(buf, D_001E63C0.unk40 ? D_001DFFB0[D_001E63C0.items[D_001E63C0.sel].id].d
                                  : D_001DFFB0[D_001E63C0.items[D_001E63C0.sel].id].c);
            func_001F69D0(0x76, 0x55, 0x80F0F0F0, buf, -1);
        }
    } else {
        func_001F65B0(6, 8, 0x80F0F0F0, func_001FDD10(D_001863D0[D_001E63C0.items[D_001E63C0.sel].id].text), -1);
        func_00238688(buf, D_001DFFB0[D_001E63C0.items[D_001E63C0.sel].id].a);
        func_001F69D0(0x76, 0x65, D_0013D4C0.unk23 ? 0x80808080 : 0x80F0F0F0, buf, -1);
        if (D_0013D4C0.unk23 != 0) {
            w = FUN_001f6250(buf, -1);
            func_00200E08(0x75 - w, 0x6D, 0x7B, 0x70, 0x20959544, 0);
            func_00200E08(0x76 - w, 0x6D, 0x7A, 0x70, 0x30959544, 0);
            func_00200E08(0x77 - w, 0x6D, 0x79, 0x70, 0x40959544, 0);
            func_00200E08(0x78 - w, 0x6D, 0x78, 0x70, 0x50959544, 0);
            func_00200E08(0x79 - w, 0x6D, 0x77, 0x70, 0x60959544, 0);
            func_00200E08(0x7A - w, 0x6D, 0x76, 0x70, 0x70959544, 0);
            func_00200E08(0x7B - w, 0x6D, 0x75, 0x70, 0x80959544, 0);
            func_00238688(buf, D_0013D4C0.unk23 ? D_001DFFB0[D_001E63C0.items[D_001E63C0.sel].id].b
                                  : D_001DFFB0[D_001E63C0.items[D_001E63C0.sel].id].a);
            func_001F69D0(0x76, 0x55, 0x80F0F0F0, buf, -1);
        }
    }
}

extern __typeof__(fun_002389e0) func_002389E0 __attribute__((alias("FUN_002389e0")));
