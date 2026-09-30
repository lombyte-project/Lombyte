#include "types.h"
struct M2c_arg0 {
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
    struct M2c_temp_7_14 * unk858;
};

struct M2c_arg1 {
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

struct M2c_temp_7_14 {
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
void _dispRefImage(struct M2c_arg0 *arg0, struct M2c_arg1 *arg1) {
    u64 temp_6_28;
    struct M2c_temp_7_14 *temp_7_14;
    s32 *temp_7_9;

    temp_7_9 = arg0->unk858;
    _getPtsDtsFlags(arg0, arg1, (s32 *) ((u8 *) temp_7_9 + 0x10), (s32 *) ((u8 *) temp_7_9 + 0x18), (s32 *) ((u8 *) temp_7_9 + 0x20));
    temp_7_14 = arg0->unk858;
    arg0->unk80 = (s32) temp_7_14->unk10;
    temp_6_28 = D_00132E98[(s32) (temp_7_14->unk20 >> 5) & 0xF];
    arg0->unkCC = (s32) arg1->unk5C;
    arg0->unk88 = temp_6_28;
    arg0->unkD0 = (s32) arg1->unk60;
    arg0->unkB4 = (s32) arg1->unk44;
    arg0->unkB8 = (s32) arg1->unk48;
    arg0->unkBC = (s32) arg1->unk4C;
    arg0->unkC0 = (s32) arg1->unk50;
    arg0->unkC4 = (s32) arg1->unk54;
    arg0->unkC8 = (s32) arg1->unk58;
    if (_isOutSizeOK(arg0, arg1) != 0) {
        if (arg1->unk28 == 1) {
            if (arg0->unkB0 != 0) {
                _csc_storeRefImage(arg0, arg1);
            } else {
                _cpr8(arg0, arg1);
            }
            func_00129B38(arg0);
        }
    }
}
