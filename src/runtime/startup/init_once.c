#include "types.h"
struct M2c_D_00137B80 {
    u8 pad_0[0x12C0];
    s32 unk12C0;
    s32 unk12C4;
};

extern u8 D_0010E4C0[];
extern void FUN_0012f1c8();
extern struct M2c_D_00137B80 D_00137B80;
extern s32 D_0015ED80;
extern s32 D_0015ED88;
extern s32 D_0015EE90;
extern u8 D_0015FA88[];
extern s32 D_00160F0C;
extern u8 D_001941C0[];
extern u8 D_001E7AE0[];
extern u8 D_001E7AF8[];
extern u8 D_001E7B10[];
extern u8 D_1FF8000[];
extern u8 D_24135F[];
extern s32 DIntr();
extern s32 DebugPrint();
extern s32 EnableCache();
extern s32 EnableInterrupts();
extern s32 FillTransferWords();
extern void FlushCache();
extern s32 FUN_001204b8();
extern s32 FUN_00120558();
extern s32 FUN_00121190();
extern s32 FUN_0012f208();
extern s32 func_0012F2B8();
extern s32 FUN_001e9338();
extern s32 func_001F2C60();
extern s32 FUN_001f2d98();
extern s32 func_001F34E8();
extern s32 func_001F7A30();
extern s32 func_00201520();
extern s32 func_002015D8();
extern s32 func_00209030();
extern s32 func_0020AC58();
extern s32 func_0020B418();
extern s32 FUN_0020b618();
extern s32 func_00217048();
extern s32 FUN_0022c8d0();
extern s32 FUN_00232ce0();
extern s32 func_002334D8();
extern s32 func_002335D0();
extern s32 sceCdInit();
extern s32 sceCdMmode();
extern s32 sceDmaReset();
extern s32 sceFsReset();
extern s32 sceGsExecLoadImage();
extern s32 sceGsResetGraph();
extern s32 sceGsSetDefLoadImage();
extern s32 sceGsSyncVCallback();
extern s32 sceScfGetLanguage();
extern s32 sceSifInitIopHeap();
extern s32 sceSifInitRpc();
extern s32 sceSifRebootIop();
extern s32 sceSifSyncIop();
void init_once(void) __asm__("FUN_00201650");

void init_once(void) {
    u8 buf[0x800];
    s32 lang;
    u32 b;
    u32 base;
    s32 *dst;
    s32 flag;

    FUN_001204b8();
    sceDmaReset(1);
    sceCdInit(0);
    FUN_00121190(0);
    do {

    } while (sceSifRebootIop(D_001E7AE0) == 0);
    do {

    } while (sceSifSyncIop() == 0);
    DebugPrint(D_0015FA88);
    EnableCache(3);
    sceSifInitRpc(0);
    DIntr();
    sceSifInitIopHeap();
    EnableInterrupts();
    sceCdInit(0);
    FUN_00121190(0);
    sceCdMmode(2);
    sceFsReset();
    FUN_0012f208(0x121, 1, buf);
    FlushCache(0);
    flag = buf[0x33] != 0x4E;
    D_0015ED80 = flag;
    D_0015EE90 = flag;
    func_00209030(buf);
    sceGsResetGraph(0, 1, D_0015ED80 ? 3 : 2, 0);
    func_0020B418();
    func_001F34E8();
    func_002334D8(D_0010E4C0);
    func_0012F2B8();
    b = (u32)D_24135F & 0xFFFFC000;
    base = b + 0x2C0000;
    dst = (s32 *)(D_1FF8000 - (D_00137B80.unk12C4 << 0xB));
    FUN_0012f208(D_00137B80.unk12C0, D_00137B80.unk12C4, dst);
    FlushCache(0);
    FUN_0020b618(dst, base);
    FlushCache(0);
    func_00201520(*(u32 *)(base + 0x70) + base, *(u32 *)(base + 0x74));
    func_00201520(*(u32 *)(base + 0x78) + base, *(u32 *)(base + 0x7C));
    func_00201520(*(u32 *)(base + 0x80) + base, *(u32 *)(base + 0x84));
    func_00201520(*(u32 *)(base + 0x88) + base, *(u32 *)(base + 0x8C));
    func_00201520(*(u32 *)(base + 0x90) + base, *(u32 *)(base + 0x94));
    func_00201520(*(u32 *)(base + 0x98) + base, *(u32 *)(base + 0x9C));
    func_00201520(*(u32 *)(base + 0x98) + base, *(u32 *)(base + 0x9C));
    func_00201520(*(u32 *)(base + 0xA8) + base, *(u32 *)(base + 0xAC));
    func_00201520(*(u32 *)(base + 0xB0) + base, *(u32 *)(base + 0xB4));
    func_00201520(*(u32 *)(base + 0xA0) + base, *(u32 *)(base + 0xA4));
    DebugPrint(D_001E7AF8);
    FUN_00121190(0);
    func_00217048();
    sceGsSyncVCallback(FUN_0012f1c8);
    D_00160F0C = 0x160000;
    func_002015D8();
    FUN_00121190(0);
    func_0020AC58();
    func_001F2C60();
    FUN_001f2d98();
    func_002335D0();
    FUN_00121190(0);
    FUN_0022c8d0();
    FillTransferWords(D_001941C0, 0x80808080, 0x100);
    sceGsSetDefLoadImage(buf, 0x3FFB, 1, 0, 0, 0, 8, 8);
    FlushCache(0);
    sceGsExecLoadImage(buf, D_001941C0);
    FUN_00120558(0, 0);
    FUN_001e9338();
    *(volatile s32 *)0x10000810 = 0x82;
    *(volatile s32 *)0x10000800 = 0;
    FUN_00232ce0();
    func_001F7A30();
    lang = sceScfGetLanguage();
    switch (lang) {
    case 2:
        D_0015ED88 = 2;
        return;
    case 4:
        D_0015ED88 = 3;
        return;
    case 3:
        D_0015ED88 = 4;
        return;
    case 5:
        D_0015ED88 = 5;
        return;
    default:
        DebugPrint(D_001E7B10, lang);
        /* fallthrough */
    case 1:
        D_0015ED88 = 0;
        return;
    }
}

extern __typeof__(init_once) func_00201650 __attribute__((alias("FUN_00201650")));
