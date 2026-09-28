#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit _sequenceHeader; includes target internal entry symbols. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/sequenceheader/_sequenceHeader.s", _sequenceHeader);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/sequenceheader/func_0012C4C8.s", func_0012C4C8);
#else

#include "rnc/sdk_library_sequenceheader_types.h"
#include "types.h"

extern u8 D_00132FC0[];
extern u8 D_00133000[];
extern u8 D_00153AE8[];
extern s32 _Error();
extern s32 _extensionAndUserData();
extern s32 _nextBit();
extern s32 _sendIpuCommand();
extern s32 _setDefaultQM();
extern s32 _waitIpuIdle();

void _sequenceHeader(struct M2c_arg0 *arg0) {
    u32 temp_2_8;
    s32 temp_2_12;
    u32 temp_2_24;
    u32 temp_2_33;
    u32 temp_2_54;

    arg0->unkD4 = 0;
    temp_2_8 = _nextBit(arg0, 0x20);
    temp_2_12 = (temp_2_8 >> 8) & 0xFFF;
    arg0->unk124 = (u32) (temp_2_8 >> 0x14);
    arg0->unk128 = temp_2_12;
    if (temp_2_12 >= 0xAF1) {
        _Error(arg0, D_00153AE8);
    }
    temp_2_24 = _nextBit(arg0, 0x1E);
    arg0->unk134 = (u32) (temp_2_24 >> 0xC);
    arg0->unk138 = (s32) ((temp_2_24 >> 1) & 0x3FF);
    temp_2_33 = _nextBit(arg0, 1);
    arg0->unk840 = temp_2_33;
    if (temp_2_33 != 0) {
        _waitIpuIdle(arg0);
        _sendIpuCommand(arg0, 0x50000000);
        _waitIpuIdle(arg0);
    } else {
        _setDefaultQM(arg0, 0x50000000, D_00132FC0);
    }
    temp_2_54 = _nextBit(arg0, 1);
    arg0->unk844 = temp_2_54;
    if (temp_2_54 != 0) {
        _waitIpuIdle(arg0);
        _sendIpuCommand(arg0, 0x58000000);
        _waitIpuIdle(arg0);
    } else {
        _setDefaultQM(arg0, 0x58000000, D_00133000);
    }
    _extensionAndUserData(arg0);
    func_0012C4C8((struct M2c_arg0__func_0012C4C8 *)arg0->unk858);
}

extern s32 InitializeReferenceImage();
extern s32 _initRefImages();
extern s32 func_0012BC10();
extern s32 func_0012BC20();

void func_0012C4C8(struct M2c_arg0__func_0012C4C8 *arg0) {
    s32 *sp30;
    s32 *sp34;
    s32 *sp38;
    s32 *sp3C;
    s32 *sp40;
    s32 *sp44;
    s32 temp_18_75;
    s32 temp_22_48;
    s32 temp_23_51;
    s32 temp_6_16;
    s32 var_2_40;
    u32 temp_16_77;
    s32 *temp_17_63;
    s32 *temp_19_67;
    s32 *temp_20_71;
    s32 *temp_21_73;
    struct M2c_temp_30_15__func_0012C4C8 *temp_30_15;
    s32 *var_2_53;

    temp_30_15 = arg0->unk40;
    temp_6_16 = temp_30_15->unk848;
    if (temp_6_16 == 0) {
        temp_30_15->unk174 = 3;
        temp_30_15->unk17C = 1;
        temp_30_15->unk144 = 5;
        temp_30_15->unk13C = 1;
        temp_30_15->unk140 = 1;
        temp_30_15->unk188 = 1;
    }
    temp_30_15->unk12C = (s32) ((s32) (temp_30_15->unk124 + 0xF) >> 4);
    if (temp_6_16 != 0) {
        if (temp_30_15->unk13C == 0) {
            var_2_40 = ((s32) (temp_30_15->unk128 + 0x1F) >> 5) * 2;
        } else {
            goto block_6;
        }
    } else {
block_6:
        var_2_40 = (s32) (temp_30_15->unk128 + 0xF) >> 4;
    }
    temp_30_15->unk130 = var_2_40;
    temp_22_48 = var_2_40 * 0x10;
    temp_23_51 = temp_30_15->unk12C * 0x10;
    var_2_53 = ((u8 *)temp_30_15 + (0x528));
    if ((temp_23_51 != arg0->unk0) || (var_2_53 = ((u8 *)temp_30_15 + (0x528)), (temp_22_48 != arg0->unk4))) {
        arg0->unk4 = temp_22_48;
        arg0->unk0 = temp_23_51;
        sp44 = var_2_53;
        temp_17_63 = ((u8 *)temp_30_15 + (0x108));
        sp30 = ((u8 *)temp_30_15 + (0x320));
        temp_19_67 = ((u8 *)temp_30_15 + (0x1E8));
        sp34 = ((u8 *)temp_30_15 + (0x388));
        temp_20_71 = ((u8 *)temp_30_15 + (0x250));
        temp_21_73 = ((u8 *)temp_30_15 + (0x2B8));
        sp38 = ((u8 *)temp_30_15 + (0x3F0));
        temp_18_75 = temp_22_48 >> 1;
        temp_16_77 = (u32) (temp_23_51 * (temp_22_48 * 0x180)) >> 8;
        sp3C = ((u8 *)temp_30_15 + (0x458));
        sp40 = ((u8 *)temp_30_15 + (0x4C0));
        func_0012BC10(temp_17_63);
        temp_30_15->unkFC = func_0012BC20(temp_30_15, temp_17_63, temp_16_77, 0x40);
        temp_30_15->unk100 = func_0012BC20(temp_30_15, temp_17_63, temp_16_77, 0x40);
        temp_30_15->unk104 = func_0012BC20(temp_30_15, temp_17_63, temp_16_77, 0x40);
        _initRefImages(temp_19_67, temp_20_71, temp_21_73, sp30, sp34, sp38, sp3C, sp40, var_2_53, temp_30_15->unkFC, temp_30_15->unk100, temp_30_15->unk104, temp_23_51, temp_22_48);
        InitializeReferenceImage(temp_19_67, temp_23_51, temp_22_48);
        InitializeReferenceImage(temp_20_71, temp_23_51, temp_22_48);
        InitializeReferenceImage(temp_21_73, temp_23_51, temp_22_48);
        InitializeReferenceImage(sp30, temp_23_51, temp_18_75);
        InitializeReferenceImage(sp34, temp_23_51, temp_18_75);
        InitializeReferenceImage(sp38, temp_23_51, temp_18_75);
        InitializeReferenceImage(sp3C, temp_23_51, temp_18_75);
        InitializeReferenceImage(sp40, temp_23_51, temp_18_75);
        InitializeReferenceImage(sp44, temp_23_51, temp_18_75);
    }
}
#endif /* NON_MATCHING */
