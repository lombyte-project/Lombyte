#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/rpc/sce_sif_init_cmd/sceSifInitCmd.s",
            sceSifInitCmd);
#else
#include "types.h"
struct M2c_D_00154E40 {
    u8 pad_0[0xC];
    s32 unkC;
    s32 unk10;
};

struct M2c_D_00154E58 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
};

struct M2c_D_00154E80 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

struct M2c_var_3_42 {
    s32 unk0;
    s32 unk4;
};

extern void FUN_0011a428();
extern void FUN_0011a448();
extern u8 D_0012FC04[];
extern u8 D_00154D80[];
extern u8 D_00154E00[];
extern struct M2c_D_00154E40 D_00154E40;
extern u32 D_00154E54[];
extern struct M2c_D_00154E58 D_00154E58;
extern struct M2c_D_00154E80 D_00154E80;
extern u8 D_00154F80[];
extern s32 AddDmacHandler();
extern s32 DIntr();
extern s32 EnableInterrupts();
extern s32 FlushCache();
extern s32 enable_dmac() __asm__("func_00119160");
extern s32 sceSifGetReg();
extern s32 sceSifSetDChain();
extern s32 sceSifSetReg();
extern void _sceSifCmdIntrHdlr();
/* Initializes the fixed command state once, then submits a warm or cold SIF
 * handshake. The cold path polls register 4 until bit 0x20000 is set. */
void sceSifInitCmd(void) {
    s32 ipval;
    struct M2c_var_3_42 *var_3_42;
    struct M2c_var_3_42 *handlers;
    s32 *var_2_61;
    s32 temp_2_113;
    s32 temp_2_141;
    s32 poll_value;
    s32 temp_5_29;
    s32 temp_6_30;
    s32 var_16_46;
    s32 var_16_59;

    DIntr();
    if (*(s32 *)D_0012FC04 == 0) {
        goto block_3;
    }
    EnableInterrupts();
    return;
block_3:
    *(s32 *)D_0012FC04 = 1;
    ipval = 0x20;
    temp_6_30 = (s32)D_00154D80 | 0x20000000;
    temp_5_29 = (s32)D_00154E00 | 0x20000000;
    D_00154E58.unk0 = temp_6_30;
    D_00154E58.unk4 = temp_5_29;
    D_00154E58.unk1C = D_00154F80;
    handlers = (struct M2c_var_3_42 *)&D_00154E80;
    var_3_42 = handlers;
    D_00154E58.unk8 = 0;

    var_16_46 = 0x1F;
    D_00154E58.unkC = &D_00154E80;
    D_00154E58.unk14 = 0;
    D_00154E58.unk18 = 0;
    D_00154E58.unk10 = ipval;
loop_4:
    var_3_42->unk0 = 0;
    var_16_46 -= 1;
    var_3_42->unk4 = 0;
    var_3_42 += 1;
    if (var_16_46 >= 0) {
        goto loop_4;
    }
    var_16_59 = 0x1F;
    var_2_61 = D_00154F80 + 0x7C;
loop_6:
    *var_2_61 = 0;
    var_16_59 -= 1;
    var_2_61 -= 1;
    if (var_16_59 >= 0) {
        goto loop_6;
    }
    handlers[0].unk0 = FUN_0011a448;
    handlers[1].unk0 = FUN_0011a428;
    handlers[0].unk4 = &D_00154E58;
    handlers[1].unk4 = &D_00154E58;
    EnableInterrupts();
    FlushCache(0);
    {
        volatile u32 *ip0 = (volatile u32 *)0x1000E010;
        if (*ip0 & 0x20) {
            *ip0 = ipval;
        }
    }
    {
        volatile u32 *ip1 = (volatile u32 *)0x1000C000;
        if (!(*ip1 & 0x100)) {
            sceSifSetDChain();
        }
    }
    D_00154E54[0] = AddDmacHandler(5, &_sceSifCmdIntrHdlr, 0);
    enable_dmac(5);
    temp_2_113 = sceSifGetReg(0x80000000);
    D_00154E58.unk8 = temp_2_113;
    if (temp_2_113 == 0) {
        goto block_14;
    }
    D_00154E40.unk10 = D_00154D80;
    sceSifSendCmd(0x80000000, &D_00154E40, 0x14, 0, 0, 0);
    return;
block_14:
    do {
        poll_value = sceSifGetReg(4);
    } while (!(poll_value & 0x20000));
    temp_2_141 = sceSifGetReg(2);
    D_00154E58.unk8 = temp_2_141;
    sceSifSetReg(0x80000000, temp_2_141);
    sceSifSetReg(0x80000001, (s32)&D_00154E58);
    D_00154E40.unk10 = D_00154D80;
    D_00154E40.unkC = 0;
    sceSifSendCmd(0x80000002, &D_00154E40, 0x14, 0, 0, 0);
    return;
}
#endif /* NON_MATCHING */
