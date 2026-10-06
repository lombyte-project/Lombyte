#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit _sequenceHeader; includes target internal entry symbols. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/sequenceheader/_sequenceHeader.s",
            _sequenceHeader);
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/sequenceheader/func_0012C4C8.s",
            func_0012C4C8);
#else

#include "rnc/sdk/library/sequenceheader.h"
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

void _sequenceHeader(struct MpegDecoder *mpeg) {
    s32 height;
    u32 temp_2_12;
    u32 temp_2_24;
    u32 intra_qm_flag;
    u32 nonintra_qm_flag;
    u32 temp_vbv;

    mpeg->unkD4 = 0;
    temp_2_12 = _nextBit(mpeg, 0x20);
    height = (temp_2_12 >> 8) & 0xFFF;
    mpeg->unk124 = (u32)temp_2_12 >> 0x14;
    mpeg->unk128 = height;
    if (height >= 0xAF1) {
        _Error(mpeg, D_00153AE8);
    }
    temp_2_24 = _nextBit(mpeg, 0x1E);
    temp_2_12 = temp_2_24;
    temp_vbv = temp_2_12 >> 1;
    temp_2_12 >>= 0xC;
    temp_vbv &= 0x3FF;
    mpeg->unk134 = temp_2_12;
    mpeg->unk138 = temp_vbv;
    intra_qm_flag = _nextBit(mpeg, 1);
    mpeg->unk840 = intra_qm_flag;
    if (intra_qm_flag != 0) {
        _waitIpuIdle(mpeg);
        _sendIpuCommand(mpeg, 0x50000000);
        _waitIpuIdle(mpeg);
    } else {
        _setDefaultQM(mpeg, 0x50000000, D_00132FC0);
    }
    nonintra_qm_flag = _nextBit(mpeg, 1);
    mpeg->unk844 = nonintra_qm_flag;
    if (nonintra_qm_flag != 0) {
        _waitIpuIdle(mpeg);
        _sendIpuCommand(mpeg, 0x58000000);
        _waitIpuIdle(mpeg);
    } else {
        _setDefaultQM(mpeg, 0x58000000, D_00133000);
    }
    _extensionAndUserData(mpeg);
    func_0012C4C8((struct MpegContext *)mpeg->unk858);
}

extern s32 InitializeReferenceImage();
extern s32 _initRefImages();
extern s32 func_0012BC10();
extern s32 reserve_aligned_buffer_space() __asm__("func_0012BC20");

void func_0012C4C8(struct MpegContext *arg0) {
    s32 *sp30;
    s32 *sp34;
    s32 *sp38;
    s32 *sp3C;
    s32 *sp40;
    s32 *sp44;
    s32 chroma_height;
    s32 height;
    s32 width;
    s32 temp_6_16;
    s32 var_2_40;
    u32 temp_16_77;
    s32 *temp_17_63;
    s32 *temp_19_67;
    s32 *temp_20_71;
    s32 *temp_21_73;
    struct MpegDecoderPriv *temp_30_15;
    u8 *var_2_53;

    mpeg = arg0->unk40;
    temp_6_16 = mpeg->unk848;
    if (temp_6_16 == 0) {
        mpeg->unk174 = 3;
        mpeg->unk13C = 1;
        mpeg->unk140 = 1;
        mpeg->unk188 = 1;
        mpeg->unk17C = 1;
        mpeg->unk144 = 5;
    }
    mpeg->unk12C = (s32)((s32)(mpeg->unk124 + 0xF) >> 4);
    if (temp_6_16 != 0) {
        if (mpeg->unk13C == 0) {
            var_2_40 = ((s32)(mpeg->unk128 + 0x1F) >> 5) * 2;
        } else {
            goto block_6;
        }
    } else {
    block_6:
        var_2_40 = (s32)(mpeg->unk128 + 0xF) >> 4;
    }
    temp_30_15->unk130 = var_2_40;
    temp_22_48 = var_2_40 << 4;
    temp_23_51 = temp_30_15->unk12C << 4;
    if (temp_23_51 == arg0->unk0) {
        if (temp_22_48 == arg0->unk4) {
            return;
        }
        var_2_53 = ((u8 *)temp_30_15 + 0x528);
    }
    var_2_53 = ((u8 *)mpeg + 0x528);
    {
        arg0->unk0 = temp_23_51;
        arg0->unk4 = temp_22_48;
        temp_17_63 = ((u8 *)temp_30_15 + (0x108));
        sp30 = ((u8 *)temp_30_15 + (0x320));
        temp_19_67 = ((u8 *)temp_30_15 + (0x1E8));
        sp34 = ((u8 *)temp_30_15 + (0x388));
        temp_20_71 = ((u8 *)temp_30_15 + (0x250));
        temp_21_73 = ((u8 *)temp_30_15 + (0x2B8));
        sp38 = ((u8 *)temp_30_15 + (0x3F0));
        temp_18_75 = temp_22_48 >> 1;
        temp_16_77 = (u32)((0x180 * temp_22_48) * temp_23_51) >> 8;
        sp44 = var_2_53;
        sp3C = ((u8 *)temp_30_15 + (0x458));
        sp40 = ((u8 *)temp_30_15 + (0x4C0));
        func_0012BC10(temp_17_63);
        mpeg->unkFC = reserve_aligned_buffer_space(mpeg, temp_17_63, temp_16_77, 0x40);
        mpeg->unk100 = reserve_aligned_buffer_space(mpeg, temp_17_63, temp_16_77, 0x40);
        mpeg->unk104 = reserve_aligned_buffer_space(mpeg, temp_17_63, temp_16_77, 0x40);
        _initRefImages(temp_19_67, temp_20_71, temp_21_73, sp30, sp34, sp38, sp3C, sp40, var_2_53,
                       mpeg->unkFC, mpeg->unk100, mpeg->unk104, width,
                       height);
        InitializeReferenceImage(temp_19_67, width, height);
        InitializeReferenceImage(temp_20_71, width, height);
        InitializeReferenceImage(temp_21_73, width, height);
        InitializeReferenceImage(sp30, width, chroma_height);
        InitializeReferenceImage(sp34, width, chroma_height);
        InitializeReferenceImage(sp38, width, chroma_height);
        InitializeReferenceImage(sp3C, width, chroma_height);
        InitializeReferenceImage(sp40, width, chroma_height);
        InitializeReferenceImage(sp44, width, chroma_height);
    }
}
#endif /* NON_MATCHING */
