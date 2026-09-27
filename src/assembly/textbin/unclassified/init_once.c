#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/unclassified/init_once/FUN_00201650.s", FUN_00201650);
#else
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
extern s32 func_001204B8();
extern s32 func_00120558();
extern s32 func_00121190();
extern s32 func_0012F208();
extern s32 func_0012F2B8();
extern s32 func_001E9338();
extern s32 func_001F2C60();
extern s32 func_001F2D98();
extern s32 func_001F34E8();
extern s32 func_001F7A30();
extern s32 func_00201520();
extern s32 func_002015D8();
extern s32 func_00209030();
extern s32 func_0020AC58();
extern s32 func_0020B418();
extern s32 func_0020B618();
extern s32 func_00217048();
extern s32 func_0022C8D0();
extern s32 func_00232CE0();
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

void init_once(void) {    u8 sp33;
u8 sp_slot[0x840];    s32 temp_16_103;
    s32 temp_16_98;
    s32 temp_2_215;
    s32 temp_2_70;
    s32 *temp_17_101;

    func_001204B8();
    sceDmaReset(1);
    sceCdInit(0);
    func_00121190(0);
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
    func_00121190(0);
    sceCdMmode(2);
    sceFsReset();
    func_0012F208(0x121, 1, sp_slot);
    FlushCache(0);
    temp_2_70 = sp33 != 0x4E;
    D_0015EE90 = temp_2_70;
    D_0015ED80 = temp_2_70;
    func_00209030(sp_slot);
    sceGsResetGraph(0, 1, (D_0015ED80 == 0) ? 2 : 3, 0);
    func_0020B418();
    func_001F34E8();
    func_002334D8(D_0010E4C0);
    func_0012F2B8();
    temp_16_98 = (s32) D_24135F & 0xFFFFC000;
    temp_17_101 = D_1FF8000 - (D_00137B80.unk12C4 << 0xB);
    temp_16_103 = temp_16_98 + 0x2C0000;
    func_0012F208(D_00137B80.unk12C0, D_00137B80.unk12C4, temp_17_101);
    FlushCache(0);
    func_0020B618(temp_17_101, temp_16_103);
    FlushCache(0);
    func_00201520(*(volatile u32 *)(0x2C0070 + temp_16_98) + temp_16_103, *(volatile u32 *)(0x2C0074 + temp_16_98));
    func_00201520(*(volatile u32 *)(0x2C0078 + temp_16_98) + temp_16_103, *(volatile u32 *)(0x2C007C + temp_16_98));
    func_00201520(*(volatile u32 *)(0x2C0080 + temp_16_98) + temp_16_103, *(volatile u32 *)(0x2C0084 + temp_16_98));
    func_00201520(*(volatile u32 *)(0x2C0088 + temp_16_98) + temp_16_103, *(volatile u32 *)(0x2C008C + temp_16_98));
    func_00201520(*(volatile u32 *)(0x2C0090 + temp_16_98) + temp_16_103, *(volatile u32 *)(0x2C0094 + temp_16_98));
    func_00201520(*(volatile u32 *)(0x2C0098 + temp_16_98) + temp_16_103, *(volatile u32 *)(0x2C009C + temp_16_98));
    func_00201520(*(volatile u32 *)(0x2C0098 + temp_16_98) + temp_16_103, *(volatile u32 *)(0x2C009C + temp_16_98));
    func_00201520(*(volatile u32 *)(0x2C00A8 + temp_16_98) + temp_16_103, *(volatile u32 *)(0x2C00AC + temp_16_98));
    func_00201520(*(volatile u32 *)(0x2C00B0 + temp_16_98) + temp_16_103, *(volatile u32 *)(0x2C00B4 + temp_16_98));
    func_00201520(*(volatile u32 *)(0x2C00A0 + temp_16_98) + temp_16_103, *(volatile u32 *)(0x2C00A4 + temp_16_98));
    DebugPrint(D_001E7AF8);
    func_00121190(0);
    func_00217048();
    sceGsSyncVCallback(FUN_0012f1c8);
    D_00160F0C = 0x160000;
    func_002015D8();
    func_00121190(0);
    func_0020AC58();
    func_001F2C60();
    func_001F2D98();
    func_002335D0();
    func_00121190(0);
    func_0022C8D0();
    FillTransferWords(D_001941C0, 0x80808080, 0x100);
    sceGsSetDefLoadImage(sp_slot, 0x3FFB, 1, 0, 0, 0, 8, 8);
    FlushCache(0);
    sceGsExecLoadImage(sp_slot, D_001941C0);
    func_00120558(0, 0);
    func_001E9338();
    *(s32 *)0x10000810 = 0x82;
    *(s32 *)0x10000800 = 0;
    func_00232CE0(0x82);
    func_001F7A30();
    temp_2_215 = sceScfGetLanguage();
    switch (temp_2_215) {
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
        DebugPrint(D_001E7B10, temp_2_215);
        /* fallthrough */
    case 1:
        D_0015ED88 = 0;
        return;
    }
}

extern void func_00201650(void) __attribute__((alias("FUN_00201650")));
#endif /* NON_MATCHING */
