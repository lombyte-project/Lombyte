#include "types.h"
struct MpegDecoder {
    u8 pad_0[0x80];
    s32 unk80;
    u8 pad_84[0x4];
    u64 unk88;
    u8 pad_90[0x20];
    s32 unkB0;
    s32 unkB4;
    s32 unkB8;
    s32 unkBC;
    s32 unkC0;
    s32 unkC4;
    s32 unkC8;
    s32 unkCC;
    s32 unkD0;
    u8 pad_D4[0x784];
    struct MpegDisplayState *unk858;
};

struct MpegRefImage {
    u8 pad_0[0x28];
    s32 unk28;
    u8 pad_2C[0x18];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
};

struct MpegDisplayState {
    u8 pad_0[0x10];
    s32 unk10;
    u8 pad_14[0xC];
    s64 unk20;
};

extern u32 D_00132E98[];
extern s32 _cpr8();
extern s32 _csc_storeRefImage();
extern s32 _getPtsDtsFlags();
extern s32 _isOutSizeOK();
void _dispRefImage(struct MpegDecoder *decoder, struct MpegRefImage *arg1) {
    u64 temp_6_28;
    struct MpegDisplayState *ref_info;
    s32 *temp_7_9;

    temp_7_9 = decoder->unk858;
    _getPtsDtsFlags(decoder, arg1, (s32 *)((u8 *)temp_7_9 + 0x10), (s32 *)((u8 *)temp_7_9 + 0x18),
                    (s32 *)((u8 *)temp_7_9 + 0x20));
    ref_info = decoder->unk858;
    decoder->unk80 = (s32)ref_info->unk10;
    temp_6_28 = D_00132E98[(s32)(ref_info->unk20 >> 5) & 0xF];
    decoder->unkCC = (s32)arg1->unk5C;
    decoder->unk88 = temp_6_28;
    decoder->unkD0 = (s32)arg1->unk60;
    decoder->unkB4 = (s32)arg1->unk44;
    decoder->unkB8 = (s32)arg1->unk48;
    decoder->unkBC = (s32)arg1->unk4C;
    decoder->unkC0 = (s32)arg1->unk50;
    decoder->unkC4 = (s32)arg1->unk54;
    decoder->unkC8 = (s32)arg1->unk58;
    if (_isOutSizeOK(decoder, arg1) != 0) {
        if (arg1->unk28 == 1) {
            if (decoder->unkB0 != 0) {
                _csc_storeRefImage(decoder, arg1);
            } else {
                _cpr8(decoder, arg1);
            }
            func_00129B38(decoder);
        }
    }
}
